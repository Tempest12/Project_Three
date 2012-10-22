#include <windows.h>

#include <FL/Fl.H>
#include <FL/Gl.h>
#include <GL/GLu.h>
#include <GL/Glut.h>
#include <iostream>
#include <vector>
#include <list>

#include "RenderWindow.hpp"
#include "Core.hpp"
#include "Config.hpp"
#include "Sphere.hpp"

std::list<Sphere>* sphereList;

RenderWindow::RenderWindow() : Fl_Gl_Window(0, 0, "")
{
	camera = new Camera();
}

RenderWindow::RenderWindow(int width, int height, char* title) : Fl_Gl_Window(width, height, title)
{
   mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);

   camera = new Camera();
}

void RenderWindow::initGraphcs()
{
	glClearColor(convertSettingToFloat("colours", "bg_colour_r"), convertSettingToFloat("colours", "bg_colour_g"), convertSettingToFloat("colours", "bg_colour_b"), convertSettingToFloat("colours", "bg_colour_a"));

	sphereList = new std::list<Sphere>();
}

int hi = 0;

void RenderWindow::draw()
{
	update();

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//std::cout << "Yes" << std::endl;
	camera -> calculateCamera();
	gluLookAt(camera -> position -> x, camera -> position -> y, camera -> position->z, camera -> focusPoint -> x, camera -> focusPoint -> y, camera -> focusPoint -> z, camera -> upVector -> x, camera -> upVector -> y, camera -> upVector -> z);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//Draw Spheres
    //for(int index = 0; index <

	//Test spheres
	glColor3f(0.0f, 0.0f, 0.8f);


	glPushMatrix();
	    //glTranslatef(0.0f, 0.0f, hi++);
		glutWireSphere(1.0f, 25, 25); 
	glPopMatrix();

	//this -> redraw();
}

int RenderWindow::handle(int event)
{
	//std::cout << "Handling input Event: " << event << std::endl;

	switch(event)
	{
		case FL_PUSH: //Mouse Down

			break;

		case FL_DRAG: //Mouse Drag
		
			break;

		case FL_RELEASE: //Mouse Release
			
			break;

		case FL_KEYBOARD:
			char key = Fl::event_key();

			//std::cout << "Keyboard Event Recieved" << std::endl;

			switch(key)
			{
				case 'a':
					//std::cout << 'a' << std::endl;
					camera -> changeHorizontalAngle(true);
					break;

				case 's':
					//std::cout << 's' << std::endl;
					if(convertSettingToBool("camera", "invert_y"))
					{
						camera -> changeVerticalAngle(true);
					}
					else
					{
						camera -> changeVerticalAngle(false);
					}

				case 'd':
					//std::cout << 'd' << std::endl;
					camera -> changeHorizontalAngle(false);
					break;

				case 'w':
					//std::cout << 'w' << std::endl;
					if(convertSettingToBool("camera", "invert_y"))
					{
						camera -> changeVerticalAngle(false);
					}
					else
					{
						camera -> changeVerticalAngle(true);
					}
					break;

				case 'q':
					//std::cout << 'q' << std::endl;
					camera -> changeDistance(false);
					break;

				case 'e':
					//std::cout << 'e' << std::endl;
					camera -> changeDistance(true);
					break;
			}

			break;
	}

	return 0;
}