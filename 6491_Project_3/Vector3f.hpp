#ifndef _VECTOR3F_HPP
#define _VECTOR3F_HPP

#include <math.h>
#include <iostream>
#include <string>
#include <sstream>


class Vector3f
{
	//Fields
	public:
		float x;
		float y;
		float z;
	
	private:
		std::string Vector3fString;

	//Functions
	public:

		Vector3f(void)
		{
			this -> x = 0.0f;
			this -> y = 0.0f;
			this -> z = 0.0f;
		}

		Vector3f(float all)
		{
			this -> x = all;
			this -> y = all;
			this -> z = all;
		}

		Vector3f(float x, float y, float z)
		{
			this -> x = x;
			this -> y = y;
			this -> z = z;
		}

		Vector3f(Vector3f* that)
		{
			this -> x = that -> x;
			this -> y = that -> y;
			this -> z = that -> z;
		}
	

		/*~Vector3f(void)
		{
			return;
		}*/


		void normalize(void)
		{
			float length = this -> magnitude();
		
			this -> x /= length;
			this -> y /= length;
			this -> z /= length;
		}
		
		void Vector3f::crossProduct(Vector3f* that)
		{
			float tempX = this -> y * that -> z - this -> z * that -> y;
   			float tempY = this -> z * that -> x - this -> x * that -> z;
   			float tempZ = this -> x * that -> y - this -> y * that -> x;
   		
			x = tempX;
   			y = tempY;
   			z = tempZ;
		}		
	
		inline void copy(Vector3f* that)
		{
			this -> x = that -> x;
			this -> y = that -> y;
			this -> z = that -> z;
		}
		
		inline void zeroOut(void)
		{
			this -> x = 0.0f;
			this -> y = 0.0f;
			this -> z = 0.0f;
		}
		
		inline float magnitude(void)
		{
			return sqrt((x * x) + (y * y) + (z * z));
		}
		
		inline float magnitudeSquared(void)
		{
			return (x * x) + (y * y) + (z * z);
		}
		
		inline float dotProduct(Vector3f* that)
		{
			return (this -> x * that -> x) + (this -> y * that -> y) + (this -> z * that -> z);
		}
		
		inline float dotProductSquared(Vector3f* that)
		{
			float num = (this -> x * that -> x) + (this -> y * that -> y) + (this -> z * that -> z);
			return num * num;
		}
	
		inline void add(Vector3f* that)
		{
			this -> x += that -> x;
       		this -> y += that -> y;
			this -> z += that -> z;
		}
		
		inline void subtract(Vector3f* that)
		{        
			this -> x -= that -> x;
			this -> y -= that -> y;
			this -> z -= that -> z;
		}
		
		inline void scale(float scalar)
		{
			x *= scalar;
       		y *= scalar;
       		z *= scalar;		
		}	  
		
		inline void set(float x, float y, float z)
		{
			this -> x = x;
			this -> y = y;
			this -> z = z;
		}

		inline bool equals(Vector3f* that)
		{
			return ((this -> x == that -> x) && (this -> y == that -> y) && (this -> z == that -> z));
		}
		
		inline bool operator == (Vector3f* that)
		{
			return ((this -> x == that -> x) && (this -> y == that -> y) && (this -> z == that -> z));
		} 

		inline operator const char*()
		{
			std::ostringstream formattedVector3f;
			formattedVector3f << x << ',' << y << ',' << z;

			Vector3fString = formattedVector3f.str();
			return Vector3fString.c_str();
		}
	
	protected:
	private:
};


namespace MyVector
{
	//Namespace Functions
	Vector3f* add(Vector3f* one, Vector3f* two);
	Vector3f* subtract(Vector3f* one, Vector3f* two);

	Vector3f* scale(Vector3f* vector, float scalar);

	Vector3f* normalize(Vector3f* vector);

	Vector3f* crossProduct(Vector3f* one, Vector3f* two);

	void rotateVectorByAngle(Vector3f* vector, float angle);

	Vector3f midPoint(Vector3f* one, Vector3f* two);

	float distance(Vector3f* one, Vector3f* two);
};
#endif
