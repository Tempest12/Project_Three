#include "Camera.hpp"
#include "Config.hpp"

#include <stdio.h>
#include <math.h>
#include <iostream>

const float PI = 3.141592653589793238462643383f; 
float degreeToRadian = PI / 180.0f; 

Camera::Camera()
{
	this -> position = new Vector3f();
    this -> focusPoint = new Vector3f(convertSettingToFloat("camera", "camera_focus_x"), convertSettingToFloat("camera", "camera_focus_y"), convertSettingToFloat("camera", "camera_focus_z"));
    this -> upVector = new Vector3f(convertSettingToFloat("camera", "camera_up_vector_x"), convertSettingToFloat("camera", "camera_up_vector_y"), convertSettingToFloat("camera", "camera"));

	this -> distance = convertSettingToFloat("camera", "distance");
	this -> horizontalAngle = convertSettingToFloat("camera", "horizontal_angle");
	this -> verticalAngle = convertSettingToFloat("camera", "vertical_angle");
		
	this -> horizontalDelta = convertSettingToFloat("camera", "vertical_delta");
	this -> verticalDelta = convertSettingToFloat("camera", "horizontal_delta");
	this -> distanceDelta = convertSettingToFloat("camera", "distance_delta");

	this -> updateRequired = true;
	this -> calculateCamera();
}

void Camera::calculateCamera()
{
	//std::cout << "Right" << std::endl;

	//std::cout << "Updating Camera Location Updated" << std::endl;

	/*
	cameraCoordinates[0] = sin(radians(cameraPhi)) * sin(radians(cameraTheta)) * cameraR + cameraCenter[0];
	cameraCoordinates[1] = cos(radians(cameraPhi)) * cameraR + cameraCenter[1];
    cameraCoordinates[2] = sin(radians(cameraPhi)) * cos(radians(cameraTheta)) * cameraR + cameraCenter[2]; 

	vert = phi
	*/

	this -> position -> x = distance * sin(verticalAngle * degreeToRadian) * cos(horizontalAngle * degreeToRadian);
	this -> position -> y = distance * cos(verticalAngle * degreeToRadian);
	this -> position -> z = distance * sin(verticalAngle * degreeToRadian) * sin(horizontalAngle * degreeToRadian);
							

	std::cout << "X: " << position -> x << " Y: " << position -> y << " Z: " << position -> z << std::endl;
}

void Camera::changeHorizontalAngle(bool left)
{
	this -> updateRequired = true;

	std::cout << this -> horizontalAngle << std::endl;

	if(left)
	{
		this -> horizontalAngle -= horizontalDelta;
	}
	else
	{
		this -> horizontalAngle += horizontalDelta;
	}

	std::cout << this -> horizontalAngle << std::endl;
}

void Camera::changeVerticalAngle(bool up)
{
	this -> updateRequired = true;

	if(up)
	{
		this -> verticalAngle += verticalDelta;
		if(this -> verticalAngle > 179.0f)
		{
			this -> verticalAngle = 179.0f;
		}
	}
	else
	{
		this -> verticalAngle -= verticalDelta;
		if(this -> verticalAngle < 1.0f)
		{
			this -> verticalAngle = 1.0f;
		}
	}
}

void Camera::changeDistance(bool closer)
{
	this -> updateRequired = true;

	if(closer)
	{
		this -> distance -= distanceDelta;
		if(this -> distance < 0.5f)
		{
			this -> distance = 0.5f;
		}
	}
	else
	{
		this -> distance += distanceDelta;
	}
}