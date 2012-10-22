#ifndef RENDERWINDOW_HPP
#define RENDERWINDOW_HPP

#include <FL/Fl_Gl_Window.H>
#include <vector>

#include "Vector3f.hpp"
#include "Camera.hpp"

class RenderWindow : public Fl_Gl_Window
{
	//Properties
	public:
		Camera* camera;

	protected:
	private:

	//Functions
	public:
		RenderWindow();

		RenderWindow(int width, int height, char* title);

		void initGraphcs();

		void draw();

		int handle(int event);

	protected:
	private:
};

#endif