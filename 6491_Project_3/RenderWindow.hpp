#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include <FL/Fl_Gl_Window.H>
#include <vector>

#include "Vector3f.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "CornerMesh.h"


void addToList(Sphere* sphere);

class RenderWindow : public Fl_Gl_Window
{
	//Properties
	public:
		Camera* camera;
		CornerMesh* mesh;

	protected:
	private:

	//Functions
	public:
		RenderWindow();
		~RenderWindow();

		RenderWindow(int width, int height, char* title);

		void initGraphcs();

		void draw(void);
		void update(void);

		int handle(int event);

	protected:
	private:
		void spawnSpheres(bool destroyer, int mouseX, int mouseY);

		bool collisionDetectAndRespond(Sphere* one, Sphere* two, int indexOne, int indexTwo);

		void slideNewConnection(Sphere* moving, Sphere* station, int movingIndex, int stationIndex, Vector3f* restPoint);

		bool checkForCollisions(Vector3f* position, int ignoreOne, int ignoreTwo);
};
#endif