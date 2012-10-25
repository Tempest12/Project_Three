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

		glColor3f(0.0,0.5,0.0);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(c.x, c.y, c.z);
	}
	glEnd();
	glFlush(); //not sure if needed
}


void CornerMesh::shell(vector<Sphere>& spheres, float rollerRadius)
{
	/*map<int,int> sphereToG = map<int,int>();

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
	Sphere s = spheres[highestYIndex];
	GTable.push_back(Vector3f(s.position));
	sphereToG[highestYIndex] = GTable.size()-1;

	Sphere roller = Sphere(s.position->x, s.position->y+rads+rollerRadius, s.position->z, rollerRadius);
	Vector3f rollNormal = Vector3f(0,0,1);

	//Find an initial edge
	float minAngle = */
}