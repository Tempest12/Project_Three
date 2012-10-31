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

Vector3f MyVector::midPoint(Vector3f* one, Vector3f* two)
{
	return Vector3f((one->x + two->x) * 0.5f, (one->y + two->y) * 0.5f, (one->z + two->z) * 0.5f);

	//return;
}

float MyVector::angleBetween(Vector3f& one, Vector3f& two, Vector3f& ref)
{
	float cosine = (one.dotProduct(&two))/(one.magnitude()*two.magnitude());
	
	Vector3f cross = *(MyVector::crossProduct(&one, &two));

	float sine = cross.magnitude()/(one.magnitude()*two.magnitude());

	if(sine == 0)
	{
		if(cosine == 1)
			return 0;
		else
			return 3.14159265;
	}

	int sign = 0;
	float direction = cross.dotProduct(&ref);

	if(direction < 0)
		sign = -1;
	else
		sign = 1;

	float angle = sign*(float)atan2(sine, cosine);

	if(angle < 0)
		angle += 2*3.14159265;

	return angle;
}