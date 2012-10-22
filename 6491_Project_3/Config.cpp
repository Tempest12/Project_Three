#include <iostream>
#include <fstream>
#include <string>

#include <stdlib.h>

#include "Config.hpp"
#include "MyString.hpp"

//Author: Shane del Solar
//version: 1.00

using namespace std;

static SettingSection* sections;
static int count;

struct SettingSection
{
	int size;
	string sectionName;
	string* names;
	string* values;
};

/**
 *Function that parses and creates list of settings from the config file
 *Params
 *----void----
 *Return
 *	bool -> Success of the intialization
 */
bool initConfig(void)
{
	ifstream file;

	string* temp;

	string line;
	
	int stop = 0;
	int size = 0;
	int lineCount;

	//Open the file
	file.open("config.txt", ios::in);
	if(!file.is_open())
	{
		cout << "Unable to Open config file." << endl;
		return false;
	}

	//Read the first line. It should contain the number of Sections
	getline(file, line);
	lineCount = atoi(line.c_str());
	count = lineCount;
	sections = new SettingSection[lineCount];

	//Parse through the file looking for sections
	while(file.good() && stop < count)
	{
		getline(file, line);

		//Ignoe comment lines or blank lines
		if(isWhiteSpace(&line) || line[0] == '#')
		{
			continue;
		}

		//Check for the section starting tag
		if(line.find("section") != string::npos)
		{ 
			temp = split(&line, ' ', &lineCount); 
			if(lineCount == 3)
			{	
				//Parse that section and then return			
				size = atoi(temp[2].c_str());
				if(!parseSection(&file, &(sections[stop]), &(temp[1]), size))
				{
					cout << "Parsing Config Section Failed. Name of Section: " << temp[1] << endl;
					return false;
				}
				stop++;
			}
			else
			{
				cout << "Error while parsing the section header: " << line << endl;
				return false;
			}

			delete[] temp;
		}
		else
		{
			cout << "Error when Loading Config File. Line is out of place: " << temp << endl;
		}
	}

	file.close();

	return true;
}

/**
 *Clean-up at the end of the program for the Configuration
 *Params:
 *----void----
 *Return:
 *	bool -> Success of the uninitialization
 */
bool uninitConfig(void)
{
	return true;
}

/**
 *Returns the string value of a setting from the file.
 *Params:
 *	string sectionName -> The Section of the configuration where it will be found.
 *	string settingName -> The Name of the setting
 *Return:
 *	string* -> the string from the file for a value
 */
string* getSetting(string sectionName, string settingName)
{
	for(int sectionIndex = 0; sectionIndex < count; sectionIndex++)
	{
		if(equalsIgnoreCase(sections[sectionIndex].sectionName, sectionName))
		{
			for(int stop = 0; stop < sections[sectionIndex].size; stop++)
			{
				if(equalsIgnoreCase(sections[sectionIndex].names[stop], settingName))
				{
					return &(sections[sectionIndex].values[stop]);
				}
			}
		}	
	}

	//writeError("Config search failed. Tried to search for:");
	//writeError("Section Name:");
	//writeError(sectionName);
	//writeError("Setting Name:");
	//writeError(settingName);

	return NULL;
}

/**
 *Returns the value of a setting as a int.
 *Params:
 *	string sectionName -> The Section of the configuration where it will be found.
 *	string settingName -> The Name of the setting
 *Return:
 *	int -> the setting value converted to an integer
 */
int convertSettingToInt(string sectionName, string settingName)
{
	string* setting = getSetting(sectionName, settingName);
	if(setting == NULL)
	{
		return 0;
	}

	return atoi(setting -> c_str());
}

/**
 *Returns the value of a setting as a boolean
*Params:
 *	string sectionName -> The Section of the configuration where it will be found.
 *	string settingName -> The Name of the setting
 *Return:
 *	bool -> The setting value converted to a bool
 */
bool convertSettingToBool(string sectionName, string settingName)
{
	string* setting = getSetting(sectionName, settingName);
	if(setting == NULL)
	{
		return 0;
	}

	if(setting -> compare("TRUE") == 0 || setting -> compare("True") == 0 || setting -> compare("true") == 0)
	{
		return true;
	}
	else if(setting -> compare("FALSE") == 0 || setting -> compare("False") == 0 || setting -> compare("false") == 0)
	{
		return false;
	}
	else
	{
		/*writeError("Config. Conversion to bool has failed.");
		writeError("Section Name:");
		writeError(sectionName);
		writeError("Setting Name:");
		writeError(settingName);*/

		return false;
	}	
}
/**
 *Returns the value of a setting as a float
 *Params:
 *	string sectionName -> The Section of the configuration where it will be found.
 *	string settingName -> The Name of the setting
 *Return:
 *	bool -> The setting value converted to a float
 */
float convertSettingToFloat(string sectionName, string settingName)
{
	string* setting = getSetting(sectionName, settingName);
	if(setting == NULL)
	{
		return 0;
	}

	return atof(setting -> c_str());
}

/**
 * Returns the value of a setting as a unsigned char/keyCode. Uses the Glut Keycode values for special keys.
 *Params:
 *	string sectionName -> Name of the section where the value will be found.
 *	string settingName -> Name of the setting.
 *Return:
 *	unsigned char -> Keycode for that key.
 */
/*
unsigned char convertSettingToKeyCode(string sectionName, string settingName)
{
	string* setting = getSetting(sectionName, settingName);
	if(setting == NULL)
	{
		return 0;
	}
	else if(setting -> length() == 1)
	{
		return setting[0][0];
	}
	else if(equalsIgnoreCase(*setting, "F1"))
	{
		return GLUT_KEY_F1;
	}
	else if(equalsIgnoreCase(*setting, "F2"))
	{
		return GLUT_KEY_F2;
	}
	else if(equalsIgnoreCase(*setting, "F3"))
	{
		return GLUT_KEY_F3;
	}
	else if(equalsIgnoreCase(*setting, "F4"))
	{
		return GLUT_KEY_F4;
	}
	else if(equalsIgnoreCase(*setting, "F5"))
	{
		return GLUT_KEY_F5;
	}
	else if(equalsIgnoreCase(*setting, "F6"))
	{
		return GLUT_KEY_F6;
	}
	else if(equalsIgnoreCase(*setting, "F7"))
	{
		return GLUT_KEY_F7;
	}
	else if(equalsIgnoreCase(*setting, "F8"))
	{
		return GLUT_KEY_F8;
	}
	else if(equalsIgnoreCase(*setting, "F9"))
	{
		return GLUT_KEY_F9;
	}
	else if(equalsIgnoreCase(*setting, "F10"))
	{
		return GLUT_KEY_F10;
	}
	else if(equalsIgnoreCase(*setting, "F11"))
	{
		return GLUT_KEY_F11;
	}
	else if(equalsIgnoreCase(*setting, "F12"))
	{
		return GLUT_KEY_F12;
	}
	else if(equalsIgnoreCase(*setting, "left"))
	{
		return GLUT_KEY_LEFT;
	}
	else if(equalsIgnoreCase(*setting, "right"))
	{
		return GLUT_KEY_RIGHT;
	}
	else if(equalsIgnoreCase(*setting, "up"))
	{
		return GLUT_KEY_UP;
	}
	else if(equalsIgnoreCase(*setting, "down"))
	{
		return GLUT_KEY_DOWN;
	}
	else if(equalsIgnoreCase(*setting, "page_down"))
	{
		return GLUT_KEY_PAGE_DOWN;
	}
	else if(equalsIgnoreCase(*setting, "page_up"))
	{
		return GLUT_KEY_PAGE_UP;
	}
	else if(equalsIgnoreCase(*setting, "home"))
	{
		return GLUT_KEY_HOME;
	}
	else if(equalsIgnoreCase(*setting, "end"))
	{
		return GLUT_KEY_END;
	}
	else if(equalsIgnoreCase(*setting, "insert"))
	{
		return GLUT_KEY_INSERT;
	}
	else if(equalsIgnoreCase(*setting, "escape"))
	{
		return 27;//Key Code for escape.
	}
	else if(equalsIgnoreCase(*setting, "enter"))
	{
		return '\n';
	} 
	else if(equalsIgnoreCase(*setting, "spacebar"))
	{
		return ' ';
	}
	else
	{
		writeDebug("Unknown Key:");
		writeDebug(*setting);
		return 0;
	}
}*/
 
/**
 *Parses an individual section of the file at a time.
 *Params:
 *	ifstream* file -> Handler for the file.
 *	SettingSection* section -> the struct for storing all of the data
 *	string* name -> the name of the section
 *	int size -> number of setting in that section
 *Return
 *	bool -> success on parsing that section
 */
bool parseSection(ifstream* file, SettingSection* section, string* name, int size)
{
	int stop = 0;
	int lineCount = 0;
	string line;
	string* temp;

	//Fill in the struct, and allocate space for the values and names
	section -> size = size;
	section -> sectionName.assign(name -> c_str());
	section -> names = new string[size];
	section -> values = new string[size];

	//This condition should never exit the loop
	while(file -> good())
	{
		getline(*file, line);

		//Ignore Comments and Whitespace
		if(isWhiteSpace(&line) || line[0] == '#')
		{
			continue;
		}
		//Stop at the end of a section
		else if(line.compare("end_section") == 0)
		{
			return true;
		}
		
		temp = split(&(line), ' ', &lineCount);

		if(lineCount == 0)
		{
			cout << "ERROR while reading section: " << section -> sectionName << " on line: " << stop << " Line appears to be blank" << endl;
			cout << "Parsing stopped" << endl;
			return false;	
		}
		else if(lineCount != 2)
		{
			if(temp[0].compare("section") == 0)
			{
				cout << "ERROR! Section missing \"end_section\" tag. Section name is \"" << section -> sectionName << "\"" << endl;
				cout << "Parsing stopped." << endl;
				return false;
			}
			else
			{
				cout << "ERROR! incorrect number of arguements. Section: " << section -> sectionName << " line: " << stop << endl;
				cout << "Arg count: " << lineCount << endl;
				cout << "Parsing stopped." << endl;
				return false;
			}
		}

		//Assuming everything looks good save the data and free the temporary array
		section -> names[stop].assign(temp[0].c_str());
		section -> values[stop].assign(temp[1].c_str());
		delete[] temp;
		stop++;
	}

	cout << "ERROR! Missing \"end_section\" tag for section for \"" << name << "\"." << endl;
	cout << "Parsing stopped." << endl;
	return false;
}

/**
 *Prints the sections the the console. Used for debugging.
 *Params:
 *----void----
 *Return
 *----void----
 */
void printSections(void)
{
	for(int stop = 0; stop < count; stop++)
	{
		cout << "Section Name: " << sections[stop].sectionName << endl;
		for(int index = 0; index < sections[stop].size; index++)
		{
			cout << "Index: " << index << " Key: " << sections[stop].names[index] << " Value: " << sections[stop].values[index] << endl;
		}
	}
}
