#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Vector3f.hpp"

extern float degreeToRadian;

class Camera
{
	//Properties
	public:
		Vector3f* position;
		Vector3f* focusPoint;
		Vector3f* upVector;

	protected:
	private:
		bool updateRequired;

		float horizontalAngle;
		float verticalAngle;
		float distance;

		float verticalDelta;
		float horizontalDelta;
		float distanceDelta;

	//Functions
	public:

		Camera();

		void calculateCamera(void);

		void moveForward(void);
		void moveBackward(void);
		void moveLeft(void);
		void moveRight(void);
		void moveVertical(bool up);
		void changeAngle(float horizontal, float vertical);

		//void changeHorizontalAngle(bool left);
		//void changeVerticalAngle(bool up);
		//void changeDistance(bool closer);

		float getDistance()
		{
			return this -> distance;
		}

	protected:
	private:
};

#endif