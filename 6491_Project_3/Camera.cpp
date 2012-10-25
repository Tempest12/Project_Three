#include "Camera.hpp"
#include "Config.hpp"

#include <stdio.h>
#include <math.h>
#include <iostream>

const float PI = 3.141592653589793238462643383f; 
float degreeToRadian = PI / 180.0f; 

Camera::Camera()
{
	this->position = new Vector3f(convertSettingToFloat("camera", "camera_start_x"), convertSettingToFloat("camera", "camera_start_y"), convertSettingToFloat("camera", "camera_start_z"));
	this->focusPoint = new Vector3f();
	//this->focusPoint = new Vector3f(convertSettingToFloat("camera", "camera_focus_x"), convertSettingToFloat("camera", "camera_focus_y"), convertSettingToFloat("camera", "camera_focus_z"));
    this->upVector = new Vector3f(convertSettingToFloat("camera", "camera_up_vector_x"), convertSettingToFloat("camera", "camera_up_vector_y"), convertSettingToFloat("camera", "camera"));

	this->distance = convertSettingToFloat("camera", "distance");
	this->horizontalAngle = convertSettingToFloat("camera", "horizontal_angle");
	this->verticalAngle = convertSettingToFloat("camera", "vertical_angle");
		
	this->horizontalDelta = convertSettingToFloat("camera", "vertical_delta");
	this->verticalDelta = convertSettingToFloat("camera", "horizontal_delta");
	this->distanceDelta = convertSettingToFloat("camera", "distance_delta");

	this->updateRequired = true;
	this->calculateCamera();

	//std::cout << "Pan" << convertSettingToFloat("moveSpeed", "pan_speed");
}

void Camera::calculateCamera()
{
	if(this->updateRequired)
	{
		this->updateRequired = false;
	}
	else
	{
		return;
	}

	this->focusPoint->x = sin(verticalAngle * degreeToRadian) * cos(horizontalAngle * degreeToRadian);
	this->focusPoint->y = cos(verticalAngle * degreeToRadian);
	this->focusPoint->z = sin(verticalAngle * degreeToRadian) * sin(horizontalAngle * degreeToRadian);
	
	this->focusPoint->add(this->position);
		
	//std::cout << "Position";
	//this->position->print();
	//std::cout << "Focus Point: ";
	//this->focusPoint->print();
}

void Camera::moveForward()
{
	//Calculate the Focus Point. We'll be moving in that direction
	if(this->updateRequired)
	{
		calculateCamera();
	}

	Vector3f* direction = MyVector::subtract(this->focusPoint, this->position); 
	direction->normalize();

	this->position->add(direction);

	//this->position->print();

	this->updateRequired = true;

	delete direction;
}

void Camera::moveLeft()
{
	//this->position->print();

	if(this->updateRequired)
	{
		calculateCamera();
	}

	//Vector3f* blah = MyVector::subtract(this->focusPoint, this->position);
	Vector3f* direction = MyVector::crossProduct(this->upVector, this->focusPoint);
	direction->normalize();
	direction->scale(convertSettingToFloat("moveSpeed", "left_speed"));

	this->position->add(direction);

	this->updateRequired = true;

	//this->position->print();

	delete direction;
}

void Camera::moveRight()
{
	if(this->updateRequired)
	{
		calculateCamera();
	}

	Vector3f* direction = MyVector::crossProduct(this->focusPoint, this->upVector);
	direction->normalize();
	direction->scale(convertSettingToFloat("moveSpeed", "left_speed"));

	this->position->add(direction);

	this->updateRequired = true;

	delete direction;
}

void Camera::moveBackward()
{
	//Calculate the focus Point we'll be moving away from it
	if(this->updateRequired)
	{
		calculateCamera();
	}

	Vector3f* direction = MyVector::subtract(this->position, this->focusPoint); 
	direction->normalize();

	this->position->add(direction);

	this->updateRequired = true;

	delete direction;
}

void Camera::moveVertical(bool up)
{
	this->updateRequired = true;

	if(up)
	{
		this->position->y += convertSettingToFloat("moveSpeed", "up_speed");
	}
	else
	{
		this->position->y -= convertSettingToFloat("moveSpeed", "down_speed");
	}
}

void Camera::changeAngle(float horizontalDelta, float verticalDelta)
{
	this->updateRequired = true;
	this->horizontalAngle += horizontalDelta * convertSettingToFloat("moveSpeed", "pan_speed");
	this->verticalAngle += verticalDelta * convertSettingToFloat("moveSpeed", "pan_speed");

	if(this->horizontalAngle > 360.0f)
	{
		this->horizontalAngle = (int)this->horizontalAngle % (int)360.0f;
	}
	else if(this->horizontalAngle < 0.0f)
	{
		this->horizontalAngle += 360.0f;
	}

	//std::cout << "Pan" << convertSettingToFloat("moveSpeed", "pan_speed");
}

/*void Camera::changeHorizontalAngle(bool left)
{
	this->updateRequired = true;

	//std::cout << this->horizontalAngle << std::endl;

	//std::cout << "Flat as shit" << std::endl;

	if(left)
	{
		this->horizontalAngle -= horizontalDelta;
	}
	else
	{
		this->horizontalAngle += horizontalDelta;
	}

	//std::cout << this->horizontalAngle << std::endl;
}

void Camera::changeVerticalAngle(bool up)
{
	//std::cout << "Hold the phone" << std::endl;

	this->updateRequired = true;

	if(up)
	{
		this->verticalAngle += verticalDelta;
	}
	else
	{
		this->verticalAngle -= verticalDelta;
	}

	//std::cout << "WAT: " << verticalAngle << std::endl;

	if(this->verticalAngle > 179.0f)
	{
		//std::cout << " before: " << verticalAngle << std::endl;
		this->verticalAngle = 179.0f;
		//std::cout << " after " << verticalAngle << std::endl;
	}
	if(this->verticalAngle < 1.0f)
	{
		this->verticalAngle = 1.0f;
	}

}

void Camera::changeDistance(bool closer)
{
	this->updateRequired = true;

	if(closer)
	{
		this->distance -= distanceDelta;
		if(this->distance < 0.5f)
		{
			this->distance = 0.5f;
		}
	}
	else
	{
		this->distance += distanceDelta;
	}
}*/