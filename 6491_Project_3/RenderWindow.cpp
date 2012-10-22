#include <windows.h>

#include <FL/Fl.H>
#include <FL/Gl.h>
#include <GL/GLu.h>
#include <GL/Glut.h>
#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>

#include "RenderWindow.hpp"
#include "Core.hpp"
#include "Config.hpp"
#include "Sphere.hpp"

std::vector<Sphere>* sphereList;

float diskOneDistance;
float diskOneRadius;

float diskTwoDistance;
float diskTwoRadius;

int spawnCount;
float spawnRadius;
float spawnSpeed;

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

	sphereList = new std::vector<Sphere>(5);

	diskOneDistance = convertSettingToFloat("spawn", "spawn_radius");
	diskOneRadius = convertSettingToFloat("spawn", "disk_one_radius");

	diskTwoDistance = convertSettingToFloat("spawn", "diks_two_distance");
	diskTwoRadius = convertSettingToFloat("spawn", "disk_two_radius");
    
	spawnCount = convertSettingToFloat("spawn", "spawn_count");
	spawnRadius = convertSettingToFloat("spawn", "spawn_radius");
	spawnSpeed = convertSettingToFloat("spawn", "spawn_speed");
}

void RenderWindow::update(void)
{
	//Calculates the new position of the camera if it has moved
	camera -> calculateCamera();
	gluLookAt(camera -> position -> x, camera -> position -> y, camera -> position->z, camera -> focusPoint -> x, camera -> focusPoint -> y, camera -> focusPoint -> z, camera -> upVector -> x, camera -> upVector -> y, camera -> upVector -> z);

	//Do Update Call on each sphere.
	for(int index = 0; index < sphereList -> size(); index++)
	{
		(*sphereList)[index].update();		
	}

	//Do Coliision Detection
}

void RenderWindow::draw()
{
	

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	update();

	//Draw Spheres
    for(int index = 0; index < sphereList -> size(); index++)
	{
		(*sphereList)[index].draw();
	}



	glColor3f(0.0f, 0.0f, 0.8f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glutWireSphere(1.0f, 15, 15);
}

int RenderWindow::handle(int event)
{
	switch(event)
	{
		case FL_PUSH: //Mouse Down
			//std::cout << "Shout out to my PEEPS!!!" << std::endl;

			//Calling Sphere Spawn
			spawnSpheres();

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
					break; 

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

void RenderWindow::spawnSpheres()
{
	//std::cout << "Count is: " << spawnCount << std::endl;

	for(int count = 0; count < spawnCount; count++)
	{
		Sphere* temp;

		//Determine Starting Location
		float spawnDistance = (float)rand() / (float)RAND_MAX;
		float spawnAngle = (float)rand() / (float)RAND_MAX;

		Vector3f* directionVector = MyVector::subtract(camera -> position, camera -> focusPoint);
		directionVector -> normalize();

		Vector3f* diskOnePosition = MyVector::scale(directionVector, camera -> getDistance() - diskOneRadius);
		Vector3f* diskTwoPosition = MyVector::scale(directionVector, camera -> getDistance() - diskOneRadius - diskTwoRadius);

		Vector3f* otherDirectionVector = new Vector(

		Vector3f* startVelocity = MyVector::subtract(diskTwoPosition, diskOnePosition);
		startVelocity -> normalize();
		startVelocity -> scale(spawnSpeed);

		temp = new Sphere(diskOnePosition, startVelocity, spawnRadius);

		//Add to list and let's move on
		sphereList ->push_back(temp);

		delete directionVector; 
		delete diskOnePosition;
		delete diskTwoPosition;
		delete startVelocity;
	}
}