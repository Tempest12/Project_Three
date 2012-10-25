#include <math.h>

#include "Vector3f.hpp"
#include "Camera.hpp"

//Author: Shane del Solar
//Version: 1.00

Vector3f* MyVector::add(Vector3f* one, Vector3f* two)
{
	Vector3f* temp = new Vector3f(one -> x + two -> x, one -> y + two -> y, one -> z + two -> z);

	return temp;
}

Vector3f* MyVector::subtract(Vector3f* one, Vector3f* two)
{
	Vector3f* temp = new Vector3f(one -> x - two -> x, one -> y - two -> y, one -> z - two -> z);

	return temp;
}

Vector3f* MyVector::normalize(Vector3f* vector)
{
	Vector3f* temp = new Vector3f(vector);
	temp -> normalize();
	return temp;
}

Vector3f* MyVector::scale(Vector3f* vector, float scalar)
{
	Vector3f* temp = new Vector3f(vector);
	temp -> scale(scalar);
	return temp;
}

Vector3f* MyVector::crossProduct(Vector3f* one, Vector3f* two)
{
	Vector3f* temp = new Vector3f(one);
	temp -> crossProduct(two);
	return temp;
}

void MyVector::rotateVectorByAngle(Vector3f* vector, float angle)
{
	//Build the rotation matrix:
	float matrix [3][3];

	matrix[0][0] = cos(angle * degreeToRadian);
	matrix[0][1] = -sin(angle * degreeToRadian);
	matrix[0][2] = 0.0f;
	
	matrix[1][0] = -matrix[0][1];
	matrix[1][1] = matrix[0][0];
	matrix[1][2] = 0.0f;

	matrix[2][0] = 0.0f;
	matrix[2][1] = 0.0f;
	matrix[2][2] = 1.0f;

	Vector3f* temp = new Vector3f();

	temp->x = matrix[0][0] * vector->x + matrix [0][1] * vector->y + matrix[0][2] * vector->z;
	temp->y = matrix[1][1] * vector->x + matrix [1][1] * vector->y + matrix[1][2] * vector->z;
	temp->z = matrix[2][1] * vector->x + matrix [2][1] * vector->y + matrix[2][2] * vector->z;

	vector->copy(temp);

	delete temp;
}