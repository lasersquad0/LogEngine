/*
 * Line.cpp
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include "Line.h"
#include "LogEngine.h"


std::string AppNameHolder::format(LogEvent& event)
{
	if(event.m_logEngine)
		return event.m_logEngine->GetAppName();
	else
		return "";
}
	
std::string AppVersionHolder::format(LogEvent& event)
{
	if(event.m_logEngine)
		return event.m_logEngine->GetVersionInfo(); 
	else
		return "";
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Line::Line(Line& line)
{
   holders = line.holders;
   // move ownership of holder pointers to new Line object
   line.holders.Clear();
}

Line::Line(std::string pattern)
{
	parsePattern(pattern);
}

Line& Line::operator=(Line& line)
{
	holders = line.holders;
    // move ownership of holder pointers to new Line object
	line.holders.Clear();
	return *this;
}

void Line::ProcessPattern(const std::string pattern)
{
    parsePattern(pattern);
}

void Line::parsePattern(const std::string pattern)
{
	THArray<std::string> placeHolders;

	placeHolders.AddFillValues(12); // do not replace by SetCapacity()

	placeHolders.SetValue(0, ""); // for LiteralHolder
	placeHolders.SetValue(1, "%DATE%");
	placeHolders.SetValue(2, "%TIME%");
	placeHolders.SetValue(3, "%DATETIME%");
	placeHolders.SetValue(4, "%MSG%");
	placeHolders.SetValue(5, "%THREAD%");
	placeHolders.SetValue(6, "%APPNAME%");
	placeHolders.SetValue(7, "%APPVERSION%");
	placeHolders.SetValue(8, "%OS%");
	placeHolders.SetValue(9, "%OSVERSION%");
	placeHolders.SetValue(10, "%DETAILLEVEL%");
	placeHolders.SetValue(11, "%DEBUGLEVEL%");

	clearHolders();
	
	unsigned int i = 0;
	while(i < pattern.size())
	{
		if(pattern[i] == '%')
		{
			std::string temp = "%";
			i++;
			
			while((i < pattern.size()) && (pattern[i] != '%'))
				temp += pattern[i++];
			
			if (i < pattern.size())
				temp += pattern[i++];


			int j = placeHolders.IndexOf(temp);
	
				switch(j)
				{
				case 1: 
					{
						Holder* a = new DateHolder();
						holders.AddValue(a);
						break;
					}
				case 2:
					{
						Holder* a = new TimeHolder();
						holders.AddValue(a);
						break;
					}
				case 3:
					{
						Holder* a = new DateTimeHolder();
						holders.AddValue(a);
						break;
					}
				case 4:
					{
						Holder* a = new MessageHolder();
						holders.AddValue(a);
						break;
					}
				case 5:
					{
						Holder* a = new ThreadHolder();
						holders.AddValue(a);
						break;
					}
				case 6:
					{
						Holder* a = new AppNameHolder();
						holders.AddValue(a);
						break;
					}	
				case 7:
					{
						Holder* a = new AppVersionHolder();
						holders.AddValue(a);
						break;
					}
				case 8:
					{
						Holder* a = new OSHolder();
						holders.AddValue(a);
						break;
					}
				case 9:
					{
						Holder* a = new OSVersionHolder();
						holders.AddValue(a);
						break;
					}
				case 10:
				case 11:
					{
						Holder* a = new DetailLevelHolder();
						holders.AddValue(a);
						break;
					}
					
				default:
					{
						Holder* a = new LiteralHolder(temp);
						holders.AddValue(a);
						break;
					}
				}
		}
		else
		{
			std::string temp = "";
			while((i < pattern.size()) && (pattern[i] != '%'))
				temp += pattern[i++];

			Holder* a = new LiteralHolder(temp);
			holders.AddValue(a);
		}
		
	}
	
}

void Line::clearHolders()
{
	for (uint i = 0; i < holders.Count(); i++)
	{
		Holder* h = holders[i];
		delete h;
	}

	holders.Clear();

}

Line::~Line()
{
	clearHolders();
}

std::string Line::format(LogEvent& event)
{
	std::string result;
	
	for(uint i = 0; i < holders.Count(); i++)
	{
		result.append(holders[i]->format(event));
	}
	
	return result;
}
