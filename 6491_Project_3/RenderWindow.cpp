#include <windows.h>

#include <FL/Fl.H>
#include <FL/Gl.h>
#include <GL/GLu.h>
#include <GL/Glut.h>
#include <iostream>
#include <vector>
#include <list>
#include <stdlib.h>
#include <math.h>

#include "RenderWindow.hpp"
#include "Core.hpp"
#include "Config.hpp"
#include "Sphere.hpp"
#include "CornerMesh.h"

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
	mesh = new CornerMesh();
}

RenderWindow::~RenderWindow()
{
	delete camera;
	delete mesh;
}

RenderWindow::RenderWindow(int width, int height, char* title) : Fl_Gl_Window(width, height, title)
{
   mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);

   camera = new Camera();
   mesh = new CornerMesh();
}

void RenderWindow::initGraphcs()
{
	glClearColor(convertSettingToFloat("colours", "bg_colour_r"), convertSettingToFloat("colours", "bg_colour_g"), convertSettingToFloat("colours", "bg_colour_b"), convertSettingToFloat("colours", "bg_colour_a"));

	sphereList = new std::vector<Sphere>();

	diskOneDistance = convertSettingToFloat("spawn", "spawn_radius");
	diskOneRadius = convertSettingToFloat("spawn", "disk_one_radius");

	diskTwoDistance = convertSettingToFloat("spawn", "diks_two_distance");
	diskTwoRadius = convertSettingToFloat("spawn", "disk_two_radius");
    
	spawnCount = convertSettingToFloat("spawn", "spawn_count");
	spawnRadius = convertSettingToFloat("spawn", "spawn_radius");
	spawnSpeed = convertSettingToFloat("spawn", "spawn_speed");

	Sphere first = Sphere(spawnRadius);
	first.moving = false;
	sphereList -> push_back(first);

	mesh->loadMesh("../models/tetra.vts");
	mesh->saveMesh("../models/retet.vts");
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
	
	//int fuck = sphereList -> size();

	//Do Coliision Detection
	for(int indexOne = (int)sphereList ->size() - 1; indexOne >= 0; indexOne--)
	{
		if((*sphereList)[indexOne].moving == false)
		{
			continue;//Skip the spheres that are no longer moving.
		}

		for(int indexTwo = indexOne - 1; indexTwo >= 0; indexTwo--)
		{
			bool temp = collisionDetectAndRespond(&(*sphereList)[indexOne], &(*sphereList)[indexTwo]);

			if(temp)
			{
				break;
			}
		}
	}
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

	glColor3f(0.0f, 0.0f, 0.8f);

	mesh->renderMesh();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Draw Spheres
    for(int index = 0; index < sphereList -> size(); index++)
	{
		(*sphereList)[index].draw();
	}
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

		//Vector3f* otherDirectionVector = new Vector(

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

bool RenderWindow::collisionDetectAndRespond(Sphere* one, Sphere* two)
{
	float distance = sqrt((one->position->x - two->position->x) * (one->position->x - two->position->x) + (one->position->y - two->position->y) * (one->position->y - two->position->y) + (one->position->z - two->position->z) * (one->position->z - two->position->z));

	if(distance < one->radius + two->radius)
	{
		//So respond to the collision
		//by sliding the moving sphere backwards until the collision is a non factor.
		if(one->moving && two->moving)
		{
			//This should never ever happen.. but in any case let them keep going until they collide with a not moving object
			return false;
		}
		else if(one->moving && !two->moving)
		{
			Vector3f* restPoint = MyVector::subtract(one->position, two->position);

			restPoint->normalize();
			restPoint->scale(one->radius + two->radius);

			restPoint->add(two->position);

			one->position->copy(restPoint);
			one->moving = false;

			delete restPoint;
		}
		else if(!one->moving && two->moving)
		{
			Vector3f* restPoint = MyVector::subtract(two->position, one->position);

			restPoint->normalize();
			restPoint->scale(one->radius + two->radius);

			restPoint->add(one->position);

			two->position->copy(restPoint);
			two->moving = false;

			delete restPoint;
		}

		return true;
	}
	else
	{
		return false;
	}
}