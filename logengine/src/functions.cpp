/*
 * functions.cpp
 *
 * Copyright 2003, LogEngine Project. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef WIN32 // required for Linux
#include <string.h>
#endif

#include <sstream>
#include <thread>
#include <sys/timeb.h>
#include <functions.h>

// truncates Value to Precision digits after point
double round(const double Value,const int Precision)
{
	int i = Precision;
	double ret;
	double p = 1;

	if(Precision >= 0)
		while(i--)p *= 10;
	else 
		while(i++)p /= 10;

	double temp = Value * p;

	double ttt = temp - (int)temp;
	if (ttt == 0)
		return temp/p;
	
	if (ttt >= 0.5 && Value > 0)
	{
		ret = int(temp + 0.5);
		return ret/p;
	}
	if (ttt <= -0.5 && Value < 0)
	{
		ret = int(temp - 0.5);
		return ret/p;
	}
	ret = int(temp);
	return ret/p;
}

#define CONV_BUF 20

// function for convert int Value to string
std::string IntToStr(int Value, int FieldSize)
{
	char buf[CONV_BUF];
	char buf2[CONV_BUF];

#ifdef WIN32 //__STDC_SECURE_LIB__ //_MSC_VER < 1400
    sprintf_s(buf2, CONV_BUF,"%%-%dd", FieldSize);
    sprintf_s(buf, CONV_BUF, buf2, Value);
#else
    sprintf(buf2, "%%-%dd", FieldSize);	
    sprintf(buf, buf2, Value);    
#endif

	return buf;
}

// function for convert int Value to string
std::string IntToStr(unsigned int Value, int FieldSize)
{
	char buf[CONV_BUF];
	char buf2[CONV_BUF];

#ifdef WIN32 // __STDC_SECURE_LIB__ //_MSC_VER < 1400
	sprintf_s(buf2, CONV_BUF, "%%-%du", FieldSize);
	sprintf_s(buf, CONV_BUF, buf2, Value);
#else
	sprintf(buf2, "%%-%du", FieldSize);
	sprintf(buf, buf2, Value);
#endif

	return buf;
}
	
// function for convert int Value to string
std::string IntToStr(int Value)
{
	char buf[CONV_BUF];
#ifdef WIN32 // __STDC_SECURE_LIB__ //_MSC_VER < 1400
	sprintf_s(buf, CONV_BUF, "%d", Value);
#else
    sprintf(buf, "%d", Value);    
#endif
	return buf;
}

// function for convert int Value to string
std::string IntToStr(unsigned int Value)
{
	char buf[CONV_BUF];
#ifdef WIN32 // __STDC_SECURE_LIB__ //_MSC_VER < 1400
	sprintf_s(buf, CONV_BUF, "%u", Value);
#else
	sprintf(buf, "%u", Value);
#endif
	return buf;
}

// function for convert long Value to string
std::string IntToStr(unsigned long Value)
{
	char buf[CONV_BUF];
#ifdef WIN32 
	sprintf_s(buf, CONV_BUF, "%lu", Value);
#else
	sprintf(buf, "%lu", Value);
#endif
	return buf;
}

// function for convert double Value to string
std::string FloatToStr(double Value)
{
	char buf[50];

#ifdef WIN32 // __STDC_SECURE_LIB__ //_MSC_VER < 1400    
	sprintf_s(buf, 50, "%f", Value);
#else
    sprintf(buf, "%f", Value);    
#endif

	return buf;
}

// function for convert bool Value to string
std::string BoolToStr(bool Value)
{
	if(Value)
		return "1";
	else
		return "0";

}
bool StrToBool(std::string& Value)
{
	return EqualNCase(Value, "1") || EqualNCase(Value, "yes") || EqualNCase(Value, "true");
}

// extracts filename from path with filename
std::string ExtractFileName(const std::string& FileName)
{
	int i = (int)FileName.length();
	while(i >= 0)
	{
		if((FileName[i] == '\\')||(FileName[i] == '/'))
			break;
		i--;
	}
	
	return FileName.substr(i+1);
}

// extracts file dir from path with filename
std::string ExtractFileDir(const std::string& FileName)
{
	int i = (int)FileName.length();
	while (i >= 0)
	{
		if ((FileName[i] == '\\') || (FileName[i] == '/'))
			break;
		i--;
	}

	return FileName.substr(0, i+1);
}

std::string StripFileExt(const std::string& FileName)
{
	int i = (int)FileName.length();

	if(i == 0)
		return "";

	bool f = false;
	
	i--; //last index
	while(i >= 0)
	{
		if(FileName[i] == '.')
		{
			f = true; // we found a dot
			break;
		}
		i--;
	}

	if(f)
		return FileName.substr(0, i);
	else 
		return FileName;
}

std::string StringReplace(const std::string& S, const std::string& SearchPattern, const std::string& ReplacePattern)
{
	if(SearchPattern.size() == 0 || S.size() == 0)
		return "";

	const char *temp1;
	char *temp2;
	std::string SearchStr(S);
	std::string Result;
	SearchStr[0] = S[0];

	temp1 = SearchStr.c_str();
	while(true)
	{
		temp2 = (char*)strstr(temp1, SearchPattern.c_str());// explicit cast requred by C++ Builder 6
		if(temp2 == nullptr)
		{
			Result += temp1;
			break;
		}
		(*temp2) = '\0';
		Result += temp1;
		Result += ReplacePattern;
		temp1   = temp2 + SearchPattern.length();
	}
	return Result;
}

#define DATETIME_BUF 100

tm_point GetCurrTimePoint()
{
	return std::chrono::system_clock::now();
}

struct tm GetCurrDateTime()
{
	const std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
#ifdef WIN32
	struct tm t;
	localtime_s(&t, &tt);
	return t;
#else
	return *localtime(&tt);
#endif
}


// retrieves current time as std::string
std::string GetCurrTimeAsString(void)
{ 
    //tm tp;
    //tm *ptp = &tp;

	//timeb t;
	//ftime(&t);
//#if __STDC_SECURE_LIB__ //_MSC_VER < 1400
	//localtime_s(ptp, &(t.time));
//#else
 //   ptp = localtime(&(t.time));    
    
//#endif
	std::chrono::system_clock::time_point stime = GetCurrTimePoint();
	std::chrono::system_clock::time_point sstime = std::chrono::time_point_cast<std::chrono::seconds>(stime); // round to seconds
	long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(stime - sstime).count();

	const std::time_t tt = std::chrono::system_clock::to_time_t(stime);

    char ss[DATETIME_BUF];
	struct tm t;
#ifdef WIN32
	localtime_s(&t, &tt);
#else
	struct tm* lt = localtime(&tt);
	t = *lt;
#endif
    std::strftime(ss, DATETIME_BUF, "%X", &t);

	char sss[20];
#ifdef WIN32 //__STDC_SECURE_LIB__ //_MSC_VER < 1400    
	sprintf_s(sss, 20, ".%03lld", millis);
#else
    sprintf(sss, ".%03lld", millis);    
#endif

	std::string s = DelCRLF(ss);
	return s + sss;
}

// retrieves current date as std::string
std::string GetCurrDateAsString(void)
{  	
	struct tm t = GetCurrDateTime();
    char ss[DATETIME_BUF];
    std::strftime(ss, DATETIME_BUF, "%d-%m-%Y", &t);

	//std::string s = DelCRLF(ss);
	return ss;
}

// retrieves current datetime as string
std::string GetCurrDateTimeAsString(void)
{
	struct tm t = GetCurrDateTime();
	char ss[DATETIME_BUF];
	std::strftime(ss, DATETIME_BUF, "%c", &t);

	return ss;
}

// converts native datetime value into string
std::string DateTimeToStr(time_t t)
{
	struct tm ttm;
#ifdef WIN32
	localtime_s(&ttm, &t);
#else
	struct tm* lt;
	lt = localtime(&t);
	ttm = *lt;
#endif
	char ss[DATETIME_BUF];
	strftime(ss, DATETIME_BUF, "%F %T", &ttm);

	return ss;
}

// converts native datetime value into string
std::string DateTimeToStr(struct tm& t)
{
	char ss[DATETIME_BUF];
	strftime(ss, DATETIME_BUF, "%F %T", &t);

	return ss;
}

// gets formatted current datetime
std::string FormatCurrDateTime(const std::string& FormatStr)
{   	
	struct tm t = GetCurrDateTime();
	char ss[DATETIME_BUF];
	std::strftime(ss, DATETIME_BUF, FormatStr.c_str(), &t);

	return ss;
}

std::string DelCRLF(const std::string& S)
{
	int j = 0;
	std::string res;
	res.resize(S.length());
	for(unsigned int i = 0; i < S.length(); i++)
	{
		if(S[i] != '\n' && S[i] != '\r')
			res[j++] = S[i];
		res[j] = '\0';
	}
	res = res.erase(strlen(res.c_str()));
	return res;
}

std::string trimSPCRLF(std::string S)
{
	// remove any leading and traling spaces, just in case.
	size_t strBegin = S.find_first_not_of(" \0x0D\0x0A");
	size_t strEnd = S.find_last_not_of(" \0x0D\0x0A");
	S.erase(strEnd + 1, S.size() - strEnd);
	S.erase(0, strBegin);
	
	return S;
}

bool EqualNCase(const std::string& str1, const std::string& str2)
{
	//const char* s1, * s2;

	//s1 = str1.c_str();
	//s2 = str2.c_str();
	//if ((s1 == nullptr) && (s2 == nullptr))
	//	return true;
	//if ((s1 == nullptr) && (s2 != nullptr))
	//	return false;
	//if ((s1 != nullptr) && (s2 == nullptr))
	//	return false;


	if (str1.length() == 0 && str2.length() == 0)
		return true;
	if ((str1.length() == 0) && (str2.length() > 0))
		return false;
	if ((str1.length() > 0) && (str2.length() == 0))
		return false;

//#ifdef HAVE_STRCASECMP
//	return strcasecmp(s1, s2) == 0;
//#else

	//for (; *s1 != '\0' && *s2 != '\0'; s1++, s2++)
	for (unsigned int i = 0; i < str1.length() && i < str2.length(); i++)
		if (toupper(static_cast<unsigned char>(str1[i])) != toupper(static_cast<unsigned char>(str2[i])))
			return false;

	if (str1.length() == str2.length())
		return true;
	else
		return false;
//#endif
}

int CompareNCase(const std::string& str1, const std::string& str2)
{	
	if ((str1.length() == 0) && (str2.length() == 0)) // two empty strings are equal
		return 0;

	if ((str1.length() == 0)) //empty string is less any non empty 
		return -1;

	if (str2.length() == 0) // non-empty string is larger any empty one
		return 1;

//#ifdef HAVE_STRCASECMP
//	return strcasecmp(s1, s2);
//#else
	for (uint i = 0; i < str1.length() && str2.length(); i++)
	{
		int upper1 = toupper(static_cast<unsigned char>(str1[i]));
		int upper2 = toupper(static_cast<unsigned char>(str2[i]));

		if (upper1 > upper2)
			return 1;
		else if (upper1 < upper2)
			return -1;
	}

	if (str1.length() > str2.length())
		return 1;
	else if (str1.length() < str2.length())
		return -1;
	else 
		return 0;
//#endif
}

bool isUInt(std::string & value)
{
	if (value.size() == 0) return false;

	size_t start = 0;
	if (value[0] == '+') start = 1; // we need only positive numbers here

	return value.find_first_not_of("0123456789", start) == std::string::npos;
}

uint GetThreadID()
{
	std::stringstream ss;
	ss << std::this_thread::get_id();
	uint thrID;
	ss >> thrID;
	return thrID;
}
