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
	if(this -> updateRequired)
	{
		this -> updateRequired = false;
	}
	else
	{
		return;
	}

	this -> position -> x = distance * sin(verticalAngle * degreeToRadian) * cos(horizontalAngle * degreeToRadian);
	this -> position -> y = distance * cos(verticalAngle * degreeToRadian);
	this -> position -> z = distance * sin(verticalAngle * degreeToRadian) * sin(horizontalAngle * degreeToRadian);
}

void Camera::changeHorizontalAngle(bool left)
{
	this -> updateRequired = true;

	//std::cout << this -> horizontalAngle << std::endl;

	//std::cout << "Flat as shit" << std::endl;

	if(left)
	{
		this -> horizontalAngle -= horizontalDelta;
	}
	else
	{
		this -> horizontalAngle += horizontalDelta;
	}

	//std::cout << this -> horizontalAngle << std::endl;
}

void Camera::changeVerticalAngle(bool up)
{
	//std::cout << "Hold the phone" << std::endl;

	this -> updateRequired = true;

	if(up)
	{
		this -> verticalAngle += verticalDelta;
	}
	else
	{
		this -> verticalAngle -= verticalDelta;
	}

	//std::cout << "WAT: " << verticalAngle << std::endl;

	if(this -> verticalAngle > 179.0f)
	{
		//std::cout << " before: " << verticalAngle << std::endl;
		this -> verticalAngle = 179.0f;
		//std::cout << " after " << verticalAngle << std::endl;
	}
	if(this -> verticalAngle < 1.0f)
	{
		this -> verticalAngle = 1.0f;
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