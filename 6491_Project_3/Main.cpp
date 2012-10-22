#include <Windows.h>
#include <iostream>

#include "Config.hpp"
#include "Core.hpp"

void init(void)
{
	std::cout << "Beginning Initialization......";

	initConfig();

	initCore();

	std::cout << "SUCCESS" << std::endl;
}

int main(int argc, char** argv)
{
	init();
	//Calls Core.run to begin Application loop
	run();
	//test();

	//uninit();
}

void uninit(void)
{
	uninitCore();

	uninitConfig();
}