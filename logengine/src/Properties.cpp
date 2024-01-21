/*
* Properties.cpp
*
* Copyright 2003, LogEngine Project. All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#include "Properties.h"
#include <limits.h>
#include <istream>

//namespace logengine {

std::string Properties::trim(std::string str)
{
	// remove any leading and traling spaces, just in case.
	size_t strBegin = str.find_first_not_of(' ');
	size_t strEnd = str.find_last_not_of(' ');
	str.erase(strEnd + 1, str.size() - strEnd);
	str.erase(0, strBegin);

	return str;
}

void Properties::load(std::istream& in) 
{
	Clear();
	
	std::string fullLine, command;
	std::string leftSide, rightSide;
	char linebuf[512]; // assume that we do not have lines greate than 512 symbols
	std::string::size_type length;   
    
    //std::ios_base::iostate state = in.rdstate();
    while (in.getline(linebuf, 512)) //state & std::ios_base::failbit != std::ios_base::failbit)
	{
		fullLine = linebuf;
		
		/* if the line contains a # then it is a comment
		if we find it anywhere other than at the beginning, we assume 
		there is a command on that line, and if we don't find it at all
		we assume there is a command on the line (we test for valid 
		command later) if neither is true, we continue with the next line
		*/
		length = fullLine.find('#');
		if (length == std::string::npos) 
		{
			command = fullLine;
		}
		else 
		{
			if (length > 0) 
			{
				std::string trimmed = trim(fullLine);
				if(trimmed.length() > 0)
				{
					if(trimmed[0] == '#') //if only spaces (0 or more) before # then this line is a comment. bypass it
						continue;
				}
				else
				{
					continue; // looks like it will never go here
				}
				
				command = fullLine; //.substr(0, length);
				
			}
			else
			{
				continue;
			}
		}	
		
		// check the command and handle it
		length = command.find('=');
		if (length != std::string::npos) 
		{
			leftSide = command.substr(0, length);
			rightSide = command.substr(length + 1, command.size() - length);
			_substituteVariables(rightSide);
		}
		else
		{
			continue; // bypass all lines not containing '='
		}
		
		// add to the map of properties
		SetValue(trimSPCRLF(leftSide), trimSPCRLF(rightSide)); // trimSPCRLF is required here for Linux since its \n and \r differ from Windows.
	}    
}

void Properties::save(std::ostream& out) 
{
	for(uint i = 0; i < Count(); i++) 
	{
		std::string key = GetKey(i);
		out << key << "=" << GetValue(key) << std::endl;
	}
}

int Properties::getInt(const std::string& property, int defaultValue /*=0*/) const
{
	if(!IfExists(property))
		return defaultValue;
	
	std::string value = trim(GetValue(property));
	if(value.size() == 0)
		return defaultValue;
	
	return atoi(value.c_str());
}

ulong Properties::getUInt(const std::string& property, ulong defaultValue /*=0*/) const
{
	if (!IfExists(property))
		return defaultValue;

	std::string value = trim(GetValue(property));

	if (!isUInt(value)) return defaultValue; // return defaultValue if string is NOT an integer

	ulong res = strtoul(value.c_str(), nullptr, 0);
	res = res > LONG_MAX ? defaultValue : res;
	return res;
}

bool Properties::getBool(const std::string& property, bool defaultValue /*=false*/) const
{
	if(!IfExists(property))
		return defaultValue;
	
	std::string value = trim(GetValue(property));
	
	return (EqualNCase(value, "true") || EqualNCase(value, "1") || EqualNCase(value, "yes"));
}

std::string Properties::getString(const std::string& property, const char* defaultValue /*=""*/) const 
{
	if(!IfExists(property))
		return defaultValue;
	
	return GetValue(property);
}

void Properties::_substituteVariables(std::string& value) 
{
	std::string result;
	
	value = StringReplace(value, "\\n", "\n");
	
	std::string::size_type left = 0;
	std::string::size_type right = value.find("${", left);
	if (right == std::string::npos) 
	{
		// bail out early for 99% of cases
		return;
	}
	
	while(true) 
	{
		result += value.substr(left, right - left);
		if (right == std::string::npos) 
		{
			break;
		}
		
		left = right + 2;
		right = value.find('}', left);
		if (right == std::string::npos) 
		{
			// no close tag, use string literally
			result += value.substr(left - 2);
			break;
		}
		else 
		{
			const std::string key = value.substr(left, right - left);
			if (key == "${") 
			{
				result += "${";
			}
			else 
			{
#pragma warning(suppress : 4996)
				char* val = getenv(key.c_str());
				if (val)
				{
					result += val;
				}
				else
				{
					if(IfExists(key))
					{
						result += GetValue(key);
					}
				}
			}
			left = right + 1;
		}
		
		right = value.find("${", left);
	}
	
	value = result;
}


