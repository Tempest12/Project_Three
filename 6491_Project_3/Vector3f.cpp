#include <math.h>

#include "Vector3f.hpp"

//Author: Shane del Solar
//Version: 1->00

Vector3f* Vector::add(Vector3f* one, Vector3f* two)
{
	Vector3f* temp = new Vector3f(one -> x + two -> x, one -> y + two -> y, one -> z + two -> z);

	return temp;
}

Vector3f* Vector::subtract(Vector3f* one, Vector3f* two)
{
	Vector3f* temp = new Vector3f(one -> x - two -> x, one -> y - two -> y, one -> z - two -> z);

	return temp;
}

Vector3f* Vector::normalize(Vector3f* vector)
{
	Vector3f* temp = new Vector3f(vector);
	temp -> normalize();
	return temp;
}

Vector3f* Vector::scale(Vector3f* vector, float scalar)
{
	Vector3f* temp = new Vector3f(vector);
	temp -> scale(scalar);
	return temp;
}

Vector3f* Vector::crossProduct(Vector3f* one, Vector3f* two)
{
	Vector3f* temp = new Vector3f(one);
	temp -> crossProduct(two);
	return temp;
}