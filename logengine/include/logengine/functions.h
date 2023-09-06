
#include <time.h>
#include <string>
#include <chrono>

typedef std::chrono::time_point<std::chrono::system_clock> tm_point;

tm_point GetCurrTimePoint();
struct tm GetCurrDateTime();


// truncates Value to Precision digits after point
double round(const double Value,const int Precision);

// function for convert int Value to string
std::string IntToStr(int Value, int FieldSize);
std::string IntToStr(unsigned int Value, int FieldSize);
std::string IntToStr(int Value);
std::string IntToStr(unsigned int Value);

// function to convert double value to string
std::string FloatToStr(double Value);

// function to convert bool value to the string
std::string BoolToStr(bool Value);

// function to convert string value (1,0,yes,no,true,false) to the bool
bool StrToBool(std::string& Value);

// extracts filename from path with filename
std::string ExtractFileName(const std::string& FileName);

// excludes file extention from FileName
std::string StripFileExt(const std::string& FileName);

// replaces in string S all occurrences of OldPattern by NewPattern
std::string StringReplace(const std::string& S,const std::string& OldPattern,const std::string& NewPattern);

// converts date to string representation
//std::string DateToString(int Date);

// retrieves current date as std::string
std::string GetCurrDateAsString(void);

// retrieves current time as std::string
std::string GetCurrTimeAsString(void);

// retrieves current datetime as std::string
std::string GetCurrDateTimeAsString(void);

// gets formatted current datetime
std::string FormatCurrDateTime(const std::string& FormatStr);

// converts native datetime value into std::string
std::string DateTimeToStr(time_t t);

// deletes all \n symbol from string
std::string DelCRLF(const std::string& S);

// removes leading and trailing \n and \r symbols
std::string trimSPCRLF(std::string S);

// compares two strings case insensitive
bool EqualNCase(const std::string& str1,const std::string& str2);

// checks if string contains unsigned integer or not
bool isUInt(std::string& value);

size_t GetThreadIdHash();
