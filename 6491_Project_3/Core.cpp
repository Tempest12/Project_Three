//STL Includes
#include <windows.h>
#include <time.h>
#include <cstdlib>
#include <iostream>

#include <Fl/Fl.h>
#include <vector>
#include <string>

//Project Imports
#include "Core.hpp"
#include "RenderWindow.hpp"
#include "Config.hpp"

//3rd Party Imports

//Globl Data
RenderWindow* window;

//Private Data


void initCore(void)
{
	std::string* windowName = getSetting("window", "title");

	window = new RenderWindow(convertSettingToInt("window", "width"), convertSettingToInt("window", "height"), (char*)windowName -> c_str());

	window -> initGraphcs();
}

void uninitCore(void)
{

}

void CALLBACK redrawIt(HWND hWnd,
  _In_      UINT_PTR nIDEvent,
  _In_      UINT uElapse,
  _In_opt_  TIMERPROC lpTimerFunc)
{
	//std::cout << "FUCKING WINDOWS" << std::endl;
	window -> redraw();
}

void run(void)
{
	window -> show();

	//Start timer
	SetTimer(NULL, NULL, 1000.0f/30.0f, (TIMERPROC)redrawIt);

	Fl::run();
}

void update(void)
{

}