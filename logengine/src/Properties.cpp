/*
* Properties.cpp
*
* Copyright 2003, LogEngine Project. All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/

#include "Properties.h"
#include <stdlib.h>
#include <istream>

//namespace logengine {

Properties::Properties() 
{
}

Properties::~Properties() 
{
}


std::string Properties::trim(std::string str)
{
    if(0 == str.length())
        return str;

    int beg = 0; 
	int end = str.length() - 1;
	int i;

	for(i = 0; i < (int)str.length(); i++)
	{
		if(str[i] == ' ')
			beg = i + 1;
		else
			break;
	}
	
	for(i = str.length() - 1; i >= 0 ; --i)
	{
		if(str[i] == ' ')
			end = i - 1;
		else
			break;
	}
	
    if(beg >= (int)str.length()) // string contains only spaces
        return "";

	std::string s = str.substr(beg, end - beg + 1);
	return s;
}

void Properties::load(std::istream& in) 
{
	Clear();
	
	std::string fullLine, command;
	std::string leftSide, rightSide;
	char line[256];
	std::string::size_type length;   
    
    //std::ios_base::iostate state = in.rdstate();
    while (in.getline(line, 512)) //state & std::ios_base::failbit != std::ios_base::failbit)
	{
		fullLine = line;
		
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
					if(trimmed[0] == '#')
						continue;
				}
				else
				{
					continue;
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
			continue;
		}
		
		
		// add to the map of properties
		SetValue(trim(leftSide), trim(rightSide));
	}    
}

void Properties::save(std::ostream& out) 
{
	for(int i = 0; i < Count(); i++) 
	{
		std::string key = GetKey(i);
		out << key << "=" << GetValue(key) << std::endl;
	}
}

int Properties::getInt(const std::string& property, int defaultValue /*=0*/) const
{
	Compare<std::string> cmp;

	if(!IfExists(property, cmp))
		return defaultValue;
	
	std::string value = trim(GetValue(property, cmp));
	if(value.size() == 0)
		return defaultValue;
	
	return atoi(value.c_str());
}

bool Properties::getBool(const std::string& property, bool defaultValue /*=false*/) const
{
	Compare<std::string> cmp;

	if(!IfExists(property, cmp))
		return defaultValue;
	
	std::string value = trim(GetValue(property, cmp));
	
	return (EqualNCase(value, "true") || EqualNCase(value, "1") || EqualNCase(value, "yes"));
}

std::string Properties::getString(const std::string& property, 
								  const char* defaultValue /*=""*/) const 
{
	Compare<std::string> cmp;

	if(!IfExists(property, cmp))
		return defaultValue;
	
	return GetValue(property, cmp);
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
//}
