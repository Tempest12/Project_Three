#ifndef CORNERMESH_H
#define CORNERMESH_H

#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "Vector3f.hpp"
#include "Sphere.hpp"

using namespace std;

class CornerMesh
{
	private:
		vector<Vector3f> GTable;
		vector<int> CTable;
	
		vector<int> VTable;
		vector<int> OTable;

		int numTriangles, numVerts;

		map<int,int> sphereToG;

		//e1 and e2 should be the clockwise orientation of the vertexes within the triangle
		//This will rotate the roller counter-clockwise
		int pivotAroundEdge(vector<Sphere>& spheres, int e1, int e2, Sphere& roller);
		bool containsTriangle(int v1, int v2, int v3);
		int reverseSphereToG(int G);

	public:
		CornerMesh()	
			: GTable(),
			  CTable(),
			  VTable(),
			  OTable(),
			  sphereToG(),
			  numTriangles(0),
			  numVerts(0){}

		~CornerMesh(){}

		inline int triOfCorner(int corner){ return corner/3; }
		inline int oppositeCorner(int corner){ return OTable[corner]; }
		inline int nextCorner(int corner){ return (3*triOfCorner(corner) + (corner+1) % 3); }
		inline int prevCorner(int corner){ return nextCorner(nextCorner(corner)); }
		inline int swingCorner(int corner){ return nextCorner(oppositeCorner(nextCorner(corner))); }
		inline int unswingCorner(int corner){ return prevCorner(oppositeCorner(prevCorner(corner))); }
		inline Vector3f getVertex(int corner){ return GTable[VTable[corner]]; }

		void saveMesh(string filename);
		void loadMesh(string filename);
		void renderMesh();

		void shell(vector<Sphere>& spheres, float rollerRadius);
};


#endif