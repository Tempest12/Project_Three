#include <Windows.h>
#include <iostream>

#include "Vector3f.hpp"
#include "Config.hpp"
#include "Core.hpp"

#include "Main.hpp"

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
	test();
	run();
	

	//uninit();
}

void uninit(void)
{
	uninitCore();

	uninitConfig();
}

void test(void)
{
	Vector3f* temp = new Vector3f(1, 0, 0);

	MyVector::rotateVectorByAngle(temp, 90);
}