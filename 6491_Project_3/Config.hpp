#ifndef _CONFIG_H
#define _CONFIG_H

#include <Windows.h>
#include <string>

struct SettingSection;

////////////////////////////////////////
///	      Function Prototypes        ///
////////////////////////////////////////
bool initConfig(void);
bool uninitConfig(void);

std::string* getSetting(std::string, std::string);

int convertSettingToInt(std::string, std::string);
bool convertSettingToBool(std::string, std::string);
float convertSettingToFloat(std::string, std::string);
unsigned char convertSettingToKeyCode(std::string, std::string);

bool parseSection(std::ifstream*, SettingSection*, std::string*, int);
void printSections(void);

#endif
