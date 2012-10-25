#ifndef CORNERMESH_H
#define CORNERMESH_H

#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "Vector3f.hpp"

using namespace std;

class CornerMesh
{
	private:
		vector<Vector3f> GTable;
		vector<int> CTable;
	
		vector<int> VTable;
		vector<int> OTable;

		int numTriangles, numVerts;

	public:
		CornerMesh()	
			: GTable(),
			  CTable(),
			  VTable(),
			  OTable(),
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
};


#endif