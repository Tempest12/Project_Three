#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <map>
#include <math.h>

#include "Vector3f.hpp"
#include "CornerMesh.h"
#include "Sphere.hpp"


using namespace std;


void CornerMesh::saveMesh(string filename)
{
	ofstream writer;
	writer.open(filename, ios_base::out|ios_base::trunc);

	if(writer.is_open())
	{
		writer << numVerts << endl;

		for(int i = 0; i < numVerts; ++i)
			writer << GTable[i] << endl;

		writer << numTriangles << endl;

		int count = 0;
		for(int i = 0; i < 3*numTriangles; ++i)
		{
			writer << VTable[i];

			++count;
			if(count > 2)
			{
				count = 0;
				writer << endl;
			}
			else
			{
				writer << ',';
			}
		}

	}

	writer.close();
}

void CornerMesh::loadMesh(string filename)
{
	ifstream reader;
	reader.open(filename, ifstream::in);

	reader >> numVerts;

	GTable = vector<Vector3f>(numVerts);
	

	string value;
	for(int i = 0; i < numVerts; ++i)
	{
		getline(reader, value, ',');
		istringstream(value) >> GTable[i].x;

		getline(reader, value, ',');
		istringstream(value) >> GTable[i].y;

		getline(reader, value);
		istringstream(value) >> GTable[i].z;
	}

	reader >> numTriangles;

	CTable = vector<int>(numVerts);
	VTable = vector<int>(3*numTriangles);
	OTable = vector<int>(3*numTriangles);

	vector<list<int>> SLists = vector<list<int>>(numVerts, list<int>());

	for(int i = 0; i < numTriangles; ++i)
	{
		getline(reader, value, ',');
		istringstream(value) >> VTable[i*3];
		CTable[VTable[i*3]] = i*3;

		getline(reader, value, ',');
		istringstream(value) >> VTable[i*3+1];
		CTable[VTable[i*3+1]] = i*3+1;

		getline(reader, value);
		istringstream(value) >> VTable[i*3+2];
		CTable[VTable[i*3+2]] = i*3+2;

		//Swing list optimization to find opposites
		for(int currentCorner = i*3; currentCorner <= i*3+2; ++currentCorner)
		{
			list<int>& potentialOpps = SLists[VTable[prevCorner(currentCorner)]];

			if(potentialOpps.empty())
			{
				SLists[VTable[nextCorner(currentCorner)]].push_back(currentCorner);
				continue;
			}
			
			bool found = false;
			for(list<int>::iterator it =  potentialOpps.begin(); it != potentialOpps.end(); ++it)
			{
				int candidateCorner = *it;

				if(candidateCorner != currentCorner && VTable[nextCorner(currentCorner)] == VTable[prevCorner(candidateCorner)]
					&& VTable[prevCorner(currentCorner)] == VTable[nextCorner(candidateCorner)] )
				{
					OTable[currentCorner] = candidateCorner;
					OTable[candidateCorner] = currentCorner;

					potentialOpps.erase(it);
					found = true;
					break;
				}
			}

			if(!found)
				SLists[VTable[nextCorner(currentCorner)]].push_back(currentCorner);
		}
	}

	reader.close();
}

void CornerMesh::renderMesh()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	for(int i = 0; i < numTriangles; ++i)
	{
		Vector3f a = getVertex(i*3);
		Vector3f b = getVertex(i*3+1);
		Vector3f c = getVertex(i*3+2);

		glColor3f(0.5,0.0,0.0);
		glVertex3f(a.x, a.y, a.z);
		glColor3f(0.0,0.5,0.0);
		glVertex3f(b.x, b.y, b.z);
		glColor3f(0.0,0.0,0.5);
		glVertex3f(c.x, c.y, c.z);
	}
	glEnd();
	glFlush(); //not sure if needed
}


//e1 and e2 should be the clockwise orientation of the vertexes within the triangle
//This will rotate the roller counter-clockwise
int CornerMesh::pivotAroundEdge(vector<Sphere>& spheres, int e1, int e2, Sphere& roller)
{
	const float PI = 3.14159265;
	int earliestI = -1;
	float earliestTrajectory = 2*PI;
	Vector3f bestPos = Vector3f(roller.position);

	Sphere a = spheres[e1];
	Sphere b = spheres[e2];

	Vector3f mid = Vector3f((a.position->x + b.position->x)/2.0, (a.position->y + b.position->y)/2.0, (a.position->z + b.position->z)/2.0);
	Vector3f trajectory = Vector3f(roller.position->x - mid.x, roller.position->y - mid.y, roller.position->z - mid.z);
	float trajectoryRadius = trajectory.magnitude();
	
	for(int i = 0; i < spheres.size(); ++i)
	{
		Sphere c = spheres[i];

		Vector3f midToC = Vector3f(c.position->x - mid.x, c.position->y - mid.y, c.position->z - mid.z);
		if(midToC.magnitude() >= 2*roller.radius || i == e1 || i == e2)
			continue;

		//Sphere-plane intersection
		Sphere ts = Sphere(c.position->x, c.position->y, c.position->z, roller.radius);

		//Plane
		Vector3f planeNormal = Vector3f(a.position->x - b.position->x, a.position->y - b.position->y, a.position->z - b.position->z);
		planeNormal.normalize();
		float D = planeNormal.x*-mid.x + planeNormal.y*-mid.y + planeNormal.z*-mid.z;

		//Circle center
		float numerator = planeNormal.x*ts.position->x + planeNormal.y*ts.position->y + planeNormal.z*ts.position->z + D;
		float denominator = planeNormal.x*planeNormal.x + planeNormal.y*planeNormal.y + planeNormal.z*planeNormal.z;
		float xc = c.position->x - (planeNormal.x*numerator)/denominator;
		float yc = c.position->y - (planeNormal.y*numerator)/denominator;
		float zc = c.position->z - (planeNormal.z*numerator)/denominator;

		//Circle radius
		float d = abs(numerator)/sqrt(denominator);
		float r = sqrt(roller.radius*roller.radius - d*d);

		if(ts.radius <= d)
			continue;

		//Trajectory circle radius is trajectoryRadius
		//Trajectory circle center is mid
		//Use trajectory for dot product calculations


		//Calculate I and J to get new coordinates
		Vector3f I = Vector3f(xc - mid.x, yc - mid.y, zc - mid.z);
		float af = I.magnitude();
		I.normalize();
		Vector3f J = Vector3f(MyVector::crossProduct(&planeNormal, &I));
		J.normalize();

		float x = 0.5*(af + (trajectoryRadius*trajectoryRadius - r*r)/af);
		float y = sqrt(trajectoryRadius*trajectoryRadius - x*x);

		Vector3f currentPos = Vector3f(mid.x + I.x*x + J.x*y, mid.y +I.y*x + J.y*y, mid.z + I.z*x + J.z*x);

		Vector3f toCurrentPos = Vector3f(currentPos.x - mid.x, currentPos.y - mid.y, currentPos.z - mid.z);

		float angle = MyVector::angleBetween(trajectory, toCurrentPos, Vector3f(-planeNormal.x, -planeNormal.y, -planeNormal.z));

		if(angle < earliestTrajectory)
		{
			earliestTrajectory = angle;
			earliestI = i;
			bestPos = currentPos;
		}
	}

	roller.position->x = bestPos.x;
	roller.position->y = bestPos.y;
	roller.position->z = bestPos.z;

	return earliestI;
}


void CornerMesh::shell(vector<Sphere>& spheres, float rollerRadius)
{
	GTable = vector<Vector3f>();
	CTable = vector<int>();
	VTable = vector<int>();
	OTable = vector<int>();
	numVerts = 0;
	numTriangles = 0;

	const float PI = 3.14159265;
	sphereToG = map<int,int>();

	float rads = spheres[0].radius;

	//Get highest sphere
	int highestYIndex = 0;
	float highestY = spheres[highestYIndex].position->y;

	for(int i = 1; i < spheres.size(); ++i)
	{
		if(spheres[i].position->y > highestY)
		{
			highestYIndex = i;
			highestY = spheres[i].position->y;
		}
	}

	//Add highest to GTable
	Sphere a = spheres[highestYIndex];
	GTable.push_back(Vector3f(a.position));
	++numVerts;
	sphereToG[highestYIndex] = GTable.size()-1;
	VTable.push_back(GTable.size()-1);
	OTable.push_back(VTable.size()-1);

	Sphere roller = Sphere(a.position->x, a.position->y+rollerRadius, a.position->z, rollerRadius);
	roller.moving = false;

	//Find an initial edge
	int secondSphereIndex = highestYIndex;
	float minAngle = PI;
	
	Vector3f newPos = Vector3f(roller.position);

	for(int i = 0; i < spheres.size(); ++i)
	{
		if(sphereToG.find(i) != sphereToG.end())
			continue;

		Sphere current = spheres[i];

		//Create displacement vectors
		Vector3f oldToRoller = Vector3f(roller.position->x - a.position->x, roller.position->y - a.position->y, roller.position->z - a.position->z);
		Vector3f oldToNew = Vector3f(current.position->x - a.position->x, current.position->y - a.position->y, current.position->z - a.position->z);

		if(oldToNew.magnitude() >= 2*rollerRadius)
			continue;

		//Crosses
		Vector3f firstNormal = Vector3f(oldToNew.y*oldToRoller.z - oldToNew.z*oldToRoller.y, oldToNew.z*oldToRoller.x - oldToNew.x*oldToRoller.z, oldToNew.x*oldToRoller.y - oldToNew.y*oldToRoller.x);
		Vector3f h1 = Vector3f(MyVector::crossProduct(&firstNormal, &oldToNew));
		h1.normalize();

		//Check which one to use
		float angle = acos(h1.dotProduct(&oldToRoller)/(oldToRoller.magnitude()));

		//cout << h1.dotProduct(&oldToRoller)/(oldToRoller.magnitude()) << "   " << angle << endl;

		if(angle < minAngle)
		{
			cout << "i: " << i << endl;
			secondSphereIndex = i;
			minAngle = angle;

			Vector3f mid = Vector3f((current.position->x + a.position->x)/2.0, (current.position->y + a.position->y)/2.0, (current.position->z + a.position->z)/2.0);

			double scalar = sqrt((oldToNew.magnitude()/2.0)*(oldToNew.magnitude()/2.0) + (rollerRadius)*(rollerRadius));

			h1.x = scalar*h1.x;
			h1.y = scalar*h1.y;
			h1.z = scalar*h1.z;

			newPos.x = mid.x+h1.x;
			newPos.y = mid.y+h1.y;
			newPos.z = mid.z+h1.z;
		}
	}

	roller.position->x = newPos.x;
	roller.position->y = newPos.y;
	roller.position->z = newPos.z;

	//Add second part of edge
	Sphere b = spheres[secondSphereIndex];
	GTable.push_back(Vector3f(b.position));
	++numVerts;
	sphereToG[secondSphereIndex] = GTable.size()-1;
	VTable.push_back(GTable.size()-1);
	OTable.push_back(VTable.size()-1);


	//Create seed triangle
	int thirdSphereIndex = pivotAroundEdge(spheres, secondSphereIndex, highestYIndex, roller);
	if(thirdSphereIndex == -1)
		return;

	Sphere c = spheres[thirdSphereIndex];
	GTable.push_back(Vector3f(c.position));
	++numVerts;
	sphereToG[thirdSphereIndex] = GTable.size()-1;
	VTable.push_back(GTable.size()-1);
	OTable.push_back(VTable.size()-1);
	++numTriangles;

	vector<Sphere> rollers = vector<Sphere>();
	rollers.push_back(Sphere(roller));
	



	queue<int> bfs = queue<int>();
	bfs.push(VTable.size()/3-1);


	while(!bfs.empty())
	{
		int currentTri = bfs.front();
		bfs.pop();

		cout << "Current triangle: " << currentTri << endl;

		for(int i = 0; i < 3; ++i)
		{
			int currentCorner = currentTri*3 + i;
			int currentPrev = prevCorner(currentCorner);

			Sphere roll = Sphere(&rollers[currentTri]);

			int sphereIndex = pivotAroundEdge(spheres, reverseSphereToG(VTable[currentCorner]), reverseSphereToG(VTable[currentPrev]), roll);

			if(sphereIndex == -1)
				continue;

			Sphere sph = spheres[sphereIndex];

			//Check if already in GTable
			if(sphereToG.find(sphereIndex) == sphereToG.end())
			{
				//Add to GTable
				GTable.push_back(Vector3f(sph.position));
				++numVerts;
				sphereToG[sphereIndex] = GTable.size()-1;
			}
			

			//Check if new triangle
			if(!containsTriangle(VTable[currentCorner], VTable[currentPrev], sphereToG[sphereIndex]))
			{
				VTable.push_back(VTable[currentCorner]);
				VTable.push_back(VTable[currentPrev]);
				VTable.push_back(sphereToG[sphereIndex]);
				++numTriangles;
				rollers.push_back(roll);
				cout << "Add Triangle " << currentCorner << ", " << currentPrev << ", " << sphereToG[sphereIndex] << endl;
				bfs.push(numTriangles-1);
			}
		}
	}
	

	/*for(int i = 0; i < rollers.size(); ++i)
	{
		spheres.push_back(rollers[i]);
	}*/


	cout << "Tris: " << numTriangles << endl;
	cout << "Verts: " << numVerts << endl;
}



bool CornerMesh::containsTriangle(int v1, int v2, int v3)
{
	for(int i = 0; i < VTable.size(); ++i)
	{
		if(VTable[i] == v1 && ((VTable[nextCorner(i)] == v2 && VTable[prevCorner(i)] == v3) ||
			(VTable[prevCorner(i)] == v2 && VTable[nextCorner(i)] == v3)))
			return true;
	}

	return false;
}

int CornerMesh::reverseSphereToG(int G)
{
	for(map<int,int>::iterator it = sphereToG.begin(); it != sphereToG.end(); ++it)
	{
		if(it->second == G)
			return it->first;
	}
	return -1;
}