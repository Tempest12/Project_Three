#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include "Vector3f.hpp"
#include "Colour4f.hpp"
#include "Core.hpp"
#include "Config.hpp"

class Sphere
{
	//Fields
	public: 
		Vector3f* position;
		Vector3f* velocity;
		Vector3f* acceleration;

		float radius;
		Colour4f* colour;
		bool moving;
		bool destroyer;

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
			colour = new Colour4f(convertSettingToFloat("colours", "sphere_r"), convertSettingToFloat("colours", "sphere_g"), convertSettingToFloat("colours", "sphere_b"), convertSettingToFloat("colours", "sphere_a"));
			this -> moving = true;
			this -> destroyer = false;
		}

		Sphere(float radius)
		{
			this -> position = new Vector3f(0.0f);
			this -> velocity = new Vector3f(0.0f);
			this -> acceleration = new Vector3f(0.0f);

			this -> radius = radius;
			colour = new Colour4f(convertSettingToFloat("colours", "sphere_r"), convertSettingToFloat("colours", "sphere_g"), convertSettingToFloat("colours", "sphere_b"), convertSettingToFloat("colours", "sphere_a"));
			this -> moving = true;
			this -> destroyer = false;
		}

		Sphere(float x, float y, float z)
		{
			this -> position = new Vector3f(x, y, z);
			this -> velocity = new Vector3f();
			this -> acceleration = new Vector3f();

			this -> radius = 1.0f;
			colour = new Colour4f(convertSettingToFloat("colours", "sphere_r"), convertSettingToFloat("colours", "sphere_g"), convertSettingToFloat("colours", "sphere_b"), convertSettingToFloat("colours", "sphere_a"));
			this -> moving = true;
			this -> destroyer = false;
		}

		Sphere(float x, float y, float z, float radius)
		{
			this -> position = new Vector3f(x, y, z);
			this -> velocity = new Vector3f();
			this -> acceleration = new Vector3f();

			this -> radius = radius;
			colour = new Colour4f(convertSettingToFloat("colours", "sphere_r"), convertSettingToFloat("colours", "sphere_g"), convertSettingToFloat("colours", "sphere_b"), convertSettingToFloat("colours", "sphere_a"));
			this -> moving = false;
			this -> destroyer = false;
		}

		Sphere(Vector3f* position, Vector3f* velocity, float radius, bool destroyer)
		{
			this -> position = new Vector3f(position);
			this -> velocity = new Vector3f(velocity);
			this -> acceleration = new Vector3f();

			this -> radius = radius;
			this -> moving = true;
			this -> destroyer = destroyer;

			if(this->destroyer)
			{
				colour = new Colour4f(convertSettingToFloat("colours", "destroyer_r"), convertSettingToFloat("colours", "destroyer_g"), convertSettingToFloat("colours", "destroyer_b"), convertSettingToFloat("colours", "destroyer_a"));
			}
			else
			{
				colour = new Colour4f(convertSettingToFloat("colours", "sphere_r"), convertSettingToFloat("colours", "sphere_g"), convertSettingToFloat("colours", "sphere_b"), convertSettingToFloat("colours", "sphere_a"));
			}
		}

		Sphere(Vector3f* position, float radius, Colour4f* colour)
		{
			this -> position = new Vector3f(position);
			this -> velocity = new Vector3f();
			this -> acceleration = new Vector3f();

			this->colour = new Colour4f(colour);

			this -> radius = radius;
			this -> moving = false;
			this -> destroyer = false;
		}

		Sphere(Sphere* that)
		{
			this -> position = new Vector3f(that -> position);
			this -> velocity = new Vector3f(that -> velocity);
			this -> acceleration = new Vector3f(that -> acceleration);

			this -> radius = that -> radius;
			this -> colour = new Colour4f(that -> colour);
			this -> moving = that -> moving;
			this -> destroyer = that -> destroyer;
		}

		/*~Sphere(void)
		{
			delete position;
			delete velocity;
			delete acceleration;
		}*/

		void setColour(Colour4f* colour)
		{
			this -> colour -> red = colour -> red;
			this -> colour -> green = colour -> green;
			this -> colour -> blue = colour -> blue;
			this -> colour -> alpha = colour -> alpha;
		}

		void update(void)
		{
			if(moving ==  false)
			{
				return;
			}

			Vector3f* tempV = MyVector::scale(velocity, 1000.0f / refreshRate);
			//Vector3f* tempA = MyVector::scale(acceleration, 1000.0f / refreshRate);

			this -> position -> add(tempV);
			//this -> velocity -> add(tempA);
			
			//delete tempA;
			delete tempV;
		}

		void draw(void)
		{
			//Colour

			glPushMatrix();
			{
				glColor3f(this->colour->red, this->colour->green, this->colour->blue);
				glTranslatef(position -> x, position -> y, position -> z);
				glutSolidSphere(this -> radius, 17, 17);
			}
			glPopMatrix();
		}

		void addForce(void)
		{

		}

	protected:
	private:
};


#endif