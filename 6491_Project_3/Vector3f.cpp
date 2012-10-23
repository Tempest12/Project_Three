#include <math.h>

#include "Vector3f.hpp"

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