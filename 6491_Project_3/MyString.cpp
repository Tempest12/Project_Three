#include <string>
#include <iostream>

#include <ctype.h>

#include "MyString.hpp"

//Author: Shane del Solar
//version: 1.01

using namespace std;

/**
 * Includes some useful string operations that aren't in the STL library.
 *		Includes:
 *			split
 *          		isWhiteSpace
 *			equals
 *			equalsIgnoreCase
 */

/************************************************************************/
/************************************************************************/
 
/**
 *Splits a string by a given delimiter and returns a new array of strings.
 *Params
 *	string* whole -> The String to Split.
 *  char delimiter -> The delimiter to split the string with
 *  int* numOf -> Pointer where the count will be stored.
 *Return
 *  string -> An array of split string without the delimiter character.
 *  *int -> the int pointer will be set to the length of the array 
 */

string* split(string* whole, char delimiter, int* numOf)
{
	string* array = NULL;
	int start = 0;
	int length = 0;
	int count = 0;

	*numOf = 0;

	if(whole == NULL)
	{
		return /*NULL*/array;
	}

	//Get the length Characters we care about. Should be all of the except the last character is the delimiter.
	length = whole -> length();

	if(length == 1)
	{
		array = new string[1];	
		*numOf = 1;
		if(whole[0][0] == delimiter)
		{
			//Return an empty string if the delimiter is the only character.
			*numOf = 0;
			return array;	
		}
		else
		{
			//Return the given input if the only chararcter isn't the delimiter
			*numOf = 1;
			array[0].assign(whole -> c_str());
			return array;
		}

	}

	//Check the ends of the String for the delimiter and adjust accordling
	if(whole[0][0] == delimiter)
	{
		start = 1;
	}
	if(whole[0][length -1] == delimiter)
	{
		length--;
	}

	
	for(int stop = start; stop  < length; stop++)
	{
		//Count the number of string pieces that we will have to make. Ignoring ones that would be empty.(Delimiter followed a delimiter)
		if(whole[0][stop] == delimiter && whole[0][stop -1] != delimiter)
		{
			count++;
		}
	}

	//Was not found at all or found only at the ends of string. Return the given input.
	if(count == 0)
	{
		*numOf = 1;
		array = new string[1];
		array -> assign(whole -> substr(start, length - start));
		return array;
	}

	count++;
	*numOf = count;
	array = new string[count];
	count = 0;

	//Parse through the string char by char
	for(int stop = start; stop < length; stop++)
	{
		if(whole[0][stop] != delimiter)
		{
			array[count] += whole[0][stop];
		}
		else if(whole[0][stop] == delimiter && whole[0][stop -1] != delimiter)
		{
			count++;
		}
	} 

	return array;	
}

/**
 *Checks to see if the given string is only white space characters.
 *Params:
 *	string* line -> String to look at.
 *Return:
 *	bool -> True - all whiteSpace, False - has non-white space character.
 */
bool isWhiteSpace(string* line)
{
	//Iterate through the string and check to see if each character is a white space Character. Return False if we find something that isn't. 
	for(unsigned int stop = 0; stop < line -> length(); stop++)
	{
		if(isspace(line[0][stop]))
		{	
			continue;
		}
		else
		{
			return false;
		}
	}

	return true;
}

/**
 *Check to see if two strings are equal to each other. *CASE SENSITIVE*
 *Params:
 *	string* one -> The first string
 *	string* two -> The second string
 *Return
 *	bool -> Equality of the two strings
 */	
bool equals(string* one, string* two)
{
	if(one != NULL && two != NULL)
	{
		if(one[0].compare(two[0]) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/**
 *Check to see if two strings are equal to each other. *NOT CASE SENSITIVE*
 *Params:
 *	string* one -> The first string 
 *	string* two -> The second string
 *Return
 *	bool -> Equality of the two Strings
 */
bool equalsIgnoreCase(string one, string two)
{
	unsigned int lengthOne = 0;
	unsigned int lengthTwo = 0;

	lengthOne = one.length();
	lengthTwo = two.length();

	//If the length's aren't the same then obviously the strings are different.
	if(lengthOne != lengthTwo)
	{
		return false;
	}

	for(unsigned int stop = 0; stop < lengthOne; stop++)
	{
		//If Both letters then compare the lower case version of the letters
		if(isalpha(one[stop]) & isalpha(two[stop]))
		{
			if(tolower(one[stop]) != tolower(two[stop]))
			{
				return false;
			}
		}
		else
		{
			//If not letters then chars must be equal
			if(one[stop] != two[stop])
			{
				return false;
			}
		}		
	}

	//All chars the same
	return true;
}
