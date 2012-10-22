#ifndef SPHERE_HPP
#define SPEHER_HPP

#include "Vector3f.hpp"
#include "Colour4f.hpp"

class Sphere
{
	//Fields
	public: 
		Vector3f* position;
		Vector3f* velocity;
		Vector3f* acceleration;

		float radius;
		Colour4f* colour;

	protected:
	private:

	//Functions
	public: 

		Sphere(void)
		{
			this -> position = new Vector3f();
			this -> velocity = new Vector3f();
			this -> acceleration = new Vector3f();

			this -> radius = 1.0f;
			this -> colour = new Colour4f();
		}

		Sphere(float radius)
		{
			this -> position = new Vector3f();
			this -> velocity = new Vector3f();
			this -> acceleration = new Vector3f();

			this -> radius = radius;
			this -> colour = new Colour4f();
		}

		Sphere(float x, float y, float z)
		{
			this -> position = new Vector3f(x, y, z);
			this -> velocity = new Vector3f();
			this -> acceleration = new Vector3f();

			this -> radius = 1.0f;
			this -> colour = new Colour4f();
		}

		Sphere(float x, float y, float z, float radius)
		{
			this -> position = new Vector3f(x, y, z);
			this -> velocity = new Vector3f();
			this -> acceleration = new Vector3f();

			this -> radius = radius;
			this -> colour = new Colour4f();
		}

		Sphere(Vector3f* position, Vector3f velocity, float radius)
		{
			this -> position = new Vector3f(position);
			this -> velocity = new Vector3f(velocity);
			this -> acceleration = new Vector3f();

			this -> radius = radius;
			this -> colour = new Colour4f();
		}

		Sphere(Sphere* that)
		{
			this -> position = new Vector3f(that -> position);
			this -> velocity = new Vector3f(that -> velocity);
			this -> acceleration = new Vector3f(that -> acceleration);

			this -> radius = that -> radius;
			this -> colour = new Colour4f(that -> colour);
		}

		~Sphere(void)
		{
			delete position;
			delete velocity;
			delete acceleration;
		}

		void setColour(Colour4f* colour)
		{
			this -> colour -> red = colour -> red;
			this -> colour -> green = colour -> green;
			this -> colour -> blue = colour -> blue;
			this -> colour -> alpha = colour -> alpha;
		}

		void update(void)
		{

		}

		void draw(void)
		{

		}

		void addForce(void)
		{

		}

	protected:
	private:
};


#endif