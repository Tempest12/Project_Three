#include <windows.h>

#include <list>
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

float scrollFactor;

int oldMouseX;
int oldMouseY;

//bool shiftDown = false;
bool displayPlane = false;

int planeSize = 15;
Sphere plane [15][15];
float planeDistance = 10.0f;
Colour4f planeColour;
float planeScroll;

float sineOfSixty;

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
	
	//std::cout << spawnRadius;
	scrollFactor = convertSettingToFloat("spawn", "scroll_factor");
	planeScroll = convertSettingToFloat("spawn", "plane_scroll");

	Sphere first = Sphere(spawnRadius);
	first.moving = false;
	sphereList -> push_back(first);

	mesh->loadMesh("../models/tetra.vts");
	mesh->saveMesh("../models/retet.vts");

	sineOfSixty = sin(60 * degreeToRadian);
}

void RenderWindow::update(void)
{
	//Calculates the new position of the camera if it has moved
	camera -> calculateCamera();
	gluLookAt(camera -> position -> x, camera -> position -> y, camera -> position->z, camera -> focusPoint -> x, camera -> focusPoint -> y, camera -> focusPoint -> z, camera -> upVector -> x, camera -> upVector -> y, camera -> upVector -> z);
	//gluLookAt(0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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
			bool temp = collisionDetectAndRespond(&(*sphereList)[indexOne], &(*sphereList)[indexTwo], indexOne, indexTwo);

			if(temp)
			{
				//If One sphere is a destroyer we wipe out both
				if((*sphereList)[indexOne].destroyer || (*sphereList)[indexTwo].destroyer)
				{
					sphereList->erase(sphereList->begin() + indexTwo);
					sphereList->erase(sphereList->begin() + indexOne - 1);

					if((*sphereList)[indexOne].destroyer == false)
					{
						(*sphereList)[indexOne].removeAllConnections();	
					}
					else if((*sphereList)[indexTwo].destroyer == false)
					{
						(*sphereList)[indexTwo].removeAllConnections();
					}
					else
					{
						//What?
					}

					indexOne--;
					indexTwo--;
				}
				else
				{
					(*sphereList)[indexOne].addConnection(&(*sphereList)[indexTwo]);
					(*sphereList)[indexTwo].addConnection(&(*sphereList)[indexOne]);
				}

				break;
			}
		}
	}

	if(displayPlane)
	{
		//Calculate the center:
		Vector3f* lookVector = MyVector::subtract(camera->focusPoint, camera->position);
		lookVector->normalize();
		lookVector->scale(planeDistance);

		Vector3f* down = MyVector::crossProduct(lookVector, camera->upVector);
		Vector3f* across = MyVector::crossProduct(lookVector, down);
		down->normalize();
		across->normalize();

		down->scale(spawnRadius * 2);
		across->scale(spawnRadius * 2);

		Vector3f start = Vector3f();
		start.subtract(down);
		start.subtract(across);
		start.normalize();
		start.scale(planeSize * spawnRadius);
		start.add(lookVector);
		Vector3f restart = Vector3f(start);

		Vector3f xPosition = Vector3f();
		Vector3f yPosition = Vector3f();

		for(int x = 0; x < planeSize; x++)
		{
			for(int y = 0; y < planeSize; y++)
			{
				start = Vector3f(restart);

				xPosition = Vector3f(across);
				yPosition = Vector3f(down);

				xPosition.scale(x);
				yPosition.scale(y);

				start.add(&xPosition);
				start.add(&yPosition);

				start.add(camera->position);

				plane[x][y] = Sphere(&start, spawnRadius, &planeColour);
			}
		}

		delete lookVector;
		delete down;
		delete across;

		Sphere temp;

		for(int listIndex = 0; listIndex < (int)sphereList->size(); listIndex++)
		{
			

			if((*sphereList)[listIndex].moving == false || (*sphereList)[listIndex].destroyer)
			{
				continue;
			}

			for(int x = 0; x < planeSize; x++)
			{
				for(int y = 0; y < planeSize; y++)
				{
					this->collisionDetectAndRespond(&(*sphereList)[listIndex], &(plane[x][y]), -1, -1);
				}
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

	if(Fl::event_shift())
	{
		Vector3f* direction = MyVector::subtract(camera->focusPoint, camera->position);

		direction->normalize();
		direction->scale(12);
		direction->add(camera->position);

		glPushMatrix();
		{
			glColor3f(1.0f, 0.0f, 1.0f);
			glTranslatef(direction->x, direction->y, direction->z);
			glutSolidSphere(spawnRadius, 17, 17);			
		}
		glPopMatrix();

		delete direction;
	}

	if(displayPlane)
	{
		for(int x = 0; x < planeSize; x++)
		{
			for(int y = 0; y < planeSize; y++)
			{
				plane[x][y].draw();
			}
		}
	}
}

int RenderWindow::handle(int event)
{
	switch(event)
	{
		case FL_PUSH: //Mouse Down

			//Left CLick to add spheres
			if(Fl::event_button() == 1/*LEFT*/)
			{
				//std::cout << "hi";
				spawnSpheres(false, Fl::event_x(), Fl::event_y());
			}
			//Middle CLick rotate camera
			else if(Fl::event_button() == 2/*Middle*/) 
			{
				
			}
			//Right CLick to take away Spheres
			else if(Fl::event_button() == 3/*Right*/)
			{
				spawnSpheres(true, Fl::event_x(), Fl::event_y());
			}


			break;
	
		case FL_MOVE:
			oldMouseX = Fl::event_x();
			oldMouseY = Fl::event_y();

			break;

		case FL_DRAG: //Mouse Drag
			if(Fl::event_button2())
			{
				this->camera->changeAngle(Fl::event_x() - oldMouseX, Fl::event_y() - oldMouseY);
			}
			//std::cout << "X: " <<Fl::event_x << std::endl;
			//std::cout << "Y: " <<Fl::event_y << std::endl;

			oldMouseX = Fl::event_x();
			oldMouseY = Fl::event_y();

			break;

		case FL_RELEASE: //Mouse Release
			
			break;

		case FL_MOUSEWHEEL:

			if(Fl::event_shift())
			{
			    spawnRadius -= (float)Fl::event_dy() * scrollFactor;
				if(spawnRadius < 0.1f)
				{
					spawnRadius = 0.1f;	
				}
			}
			if(Fl::event_ctrl())
			{
				planeDistance -= (float)Fl::event_dy() * planeScroll;
			}
			//std::cout << spawnRadius;

			break;

		case FL_KEYBOARD:
			char key = Fl::event_key();

			//std::cout << "Keyboard Event Recieved" << std::endl;

			switch(key)
			{
				case 'a':
					camera->moveLeft();
					break;

				case 's':
					//std::cout << 's' << std::endl;
					camera->moveBackward();
					break; 

				case 'd':
					//std::cout << 'd' << std::endl;
					camera->moveRight();
					break;

				case 'w':
					//std::cout << 'w' << std::endl;
					camera->moveForward();
					break;

				case 'q':
					//std::cout << 'q' << std::endl;
					//camera -> changeDistance(false);
					break;

				case 'e':
					//std::cout << 'e' << std::endl;
					//camera -> changeDistance(true);
					break;
				case ' ':
					displayPlane = !displayPlane;
					break;
				/*default:
					if(Fl::event_shift())
					{
						shiftDown = true;
					}
					break;*/
			}

			break;
	}

	return 0;
}

void RenderWindow::spawnSpheres(bool destroyer, int mouseX, int mouseY)
{
	//std::cout << "Count is: " << spawnCount << std::endl;
	Vector3f* start = new Vector3f(camera->focusPoint);
	Vector3f* direction = new Vector3f();

	for(int count = 0; count < spawnCount; count++)
	{
		Sphere* temp;

		direction->copy(camera->focusPoint);
		direction->subtract(camera->position);
		direction->normalize();
		direction->scale(spawnSpeed);

		temp = new Sphere(start, direction, abs(spawnRadius), destroyer);

		//Add to list and let's move on
		sphereList ->push_back(temp);
	}

	//start->print();
	//direction->print();

	delete start;
	delete direction;
}

bool RenderWindow::collisionDetectAndRespond(Sphere* one, Sphere* two, int indexOne, int indexTwo)
{
	float distance = sqrt((one->position->x - two->position->x) * (one->position->x - two->position->x) + (one->position->y - two->position->y) * (one->position->y - two->position->y) + (one->position->z - two->position->z) * (one->position->z - two->position->z));

	if(distance < one->radius + two->radius)
	{
		//Don't bother responding if eihter is a destroyer.
		if(one->destroyer || two->destroyer)
		{
			return true;
		}

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

			if(two->touching->size() > 0)
			{
				slideNewConnection(one, two, indexOne, indexTwo, restPoint);
			}

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

			if(one->touching->size() > 0)
			{
				slideNewConnection(two, one, indexTwo, indexOne, restPoint);
			}

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



void RenderWindow::slideNewConnection(Sphere* moving, Sphere* station, int movingIndex, int stationIndex, Vector3f* restPoint)
{
	vector<Sphere>* touchingList = station->touching; 

	Sphere* tempSphere;
	Vector3f tempPosition;

	Vector3f midPoint;
	Vector3f* upHeight;
	Vector3f* downHeight;

	Vector3f* planeNormal;
	//Vector3f* secondCross;

	Vector3f planeOne;
	Vector3f planeTwo;

	Vector3f cVector;

	float up = 0.0f;
	float down = 0.0f;

	bool found = false;

	for(int index = 0; index < touchingList->size() && !found; index++)
	{
		tempSphere = &(*touchingList)[index];

		//Calculate MidPoint
		midPoint = MyVector::midPoint(tempSphere->position, station->position);
		
		//Calculate the Plane of the midpoint
		planeOne = Vector3f(moving->position->x - station->position->x, moving->position->y - station->position->y, moving->position->z - station->position->z);
		planeTwo = Vector3f(tempSphere->position->x - station->position->x, tempSphere->position->y - station->position->y, tempSphere->position->z - station->position->z); 

		//Get the normal of that Plane
		planeNormal = MyVector::crossProduct(&planeOne, &planeTwo);

		//Calculate the first H vector
		upHeight = MyVector::crossProduct(planeNormal, &planeTwo);
		upHeight->normalize();
		upHeight->scale(sineOfSixty/(2 * spawnRadius));

		downHeight = new Vector3f(-upHeight->x, -upHeight->y, -upHeight->z);

		up = planeOne.dotProduct(upHeight);
		down = planeTwo.dotProduct(downHeight);

		if(up == 0.0f && down == 0.0f)
		{
			//let's just use up... 
			//Calculate the new would be point
			tempPosition = Vector3f(midPoint.x + upHeight->x, midPoint.y + upHeight->y, midPoint.z + upHeight->z);
			
			//Is this point a valid postion? IE no collisions
			if(checkForCollisions(&tempPosition, movingIndex, stationIndex))
			{
				continue;
			}
			
			//False means there were no collisions
			restPoint->copy(&tempPosition);
			found = true;
		}
		else if(up > 0.0f && down < 0.0f)
		{
			//Calculate the new would be point
			tempPosition = Vector3f(midPoint.x + upHeight->x, midPoint.y + upHeight->y, midPoint.z + upHeight->z);
			
			//Is this point a valid postion? IE no collisions
			if(checkForCollisions(&tempPosition, movingIndex, stationIndex))
			{
				continue;
			}
			
			//False means there were no collisions
			restPoint->copy(&tempPosition);
			found = true;
		}
		else if(up < 0.0f && down > 0.0f)
		{
			//Calculate the new would be point
			tempPosition = Vector3f(midPoint.x + downHeight->x, midPoint.y + downHeight->y, midPoint.z + downHeight->z);
			
			//Is this point a valid postion? IE no collisions
			if(checkForCollisions(&tempPosition, movingIndex, stationIndex))
			{
				continue;
			}
			
			//False means there were no collisions
			restPoint->copy(&tempPosition);
			found = true;
		}
		else
		{
			//This should never ever happen
			std::cout << "We have a problem both H vectors we negative" << std::endl;
		}

		//delete secondCross;
		delete planeNormal;
		delete upHeight;
		delete downHeight;
	}
}

bool RenderWindow::checkForCollisions(Vector3f* tempPosition, int ignoreOne, int ignoreTwo)
{
	for(int index = 0; index < sphereList->size(); index++)
	{
		if(index == ignoreOne || index == ignoreTwo)
		{
			continue;
		}

		//Now check for a possible collision
		//float distance = sqrt((one->position->x - two->position->x) * (one->position->x - two->position->x) + (one->position->y - two->position->y) * (one->position->y - two->position->y) + (one->position->z - two->position->z) * (one->position->z - two->position->z));
		float distance = sqrt((tempPosition->x - (*sphereList)[index].position->x) * (tempPosition->x - (*sphereList)[index].position->x) + (tempPosition->y - (*sphereList)[index].position->y) * (tempPosition->y - (*sphereList)[index].position->y) + (tempPosition->z - (*sphereList)[index].position->z) * (tempPosition->z - (*sphereList)[index].position->z));  

		if(distance < spawnRadius * 2)
		{
			return true;
		}
	}

	return false;
}
