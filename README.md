<h1>LogEngine --- The C++ Log Library </h1>
https://github.com/lasersquad0/LogEngine <br>
Version 1.03 <br><br>
lasersquad@gmail.com

Last revision date November, 05, 2023.


TABLE OF CONTENTS
----------------
Overview <br>
Installation and how to use <br>
Compatibility <br>
Known issues <br>
Changelog <br>

Overview
--------

LogEngine is the C++ library for easy and flexible logging to files.
The main features are: easy to use, platform independent, small amount of source code.

Bug reports are welcome. Please use the Github bug tracking
system at https://github.com/lasersquad0/LogEngine. 

See testXXX.cpp files from test/ directory as an examples of using 
logengine library.

See test/testFiles/example.lfg - for full description of logengine parameters file.

Email to the current maintainer may be sent to lasersquad@gmail.com.

Installation and How to use
---------------------------
**Microsoft Windows** <br>
Use MSVC project from prj/MSVC/LogEngine/ to make LogEngine.lib (or LogEngined.lib for Debug configuration).

See prj/MSVC/testLogEngine (or prj/Builder6/testLogEngine) for examples how to use LogEngine.

**Other systems (Linux, etc)** <br>
For other systems see INSTALL txt file for details.

Compatibility
-------------

LogEngine was recently tested on Windows 10, Windows 11, Ubuntu 22.04.2. <br>
It does not require any specific libraties, and uses standard STL classes, so it should work on many other common Windows and unix systems.

Known issues 
------------

No such issues at the moment

Change log 
----------
[+] means "added"
[-] means "removed"
[*] means "fixed" or "modified"

!!! Version 1.03 is released !!! <br>

2023-11-05 <br>
[*] Multithreading code now uses common thread STL functions instead of native ones. That made code much simpler <br>
[+] Added more unit tests that helped us to catch up several bugs that are now fixed <br>
[*] Code is more compatible with latest C++ language standards <br>
[*] Fixed a couple of compiler warnings (not all though) <br>
[*] Improved work with date/time functions <br>
[*] Code cleaning and refactorings <br>

2007-02-25 <br>
[+] Improved error handling during opening .lfg file. <br>
[*] Improved check for secure CRT under VS2005 (#if _MSC_VER < 1400 replaced by #if __STDC_SECURE_LIB__)  <br> 
 <br>

2006-11-12  <br>
[+] Created separate projects for VS6, VS .NET and VS2005  <br>
[*] Fixed a couple of security warnings under VS2005  <br>


2004-03-12 <br>
documentation manual? <br>
[*] if FileName is not specifies that AppName is got for the log file name. <br>

2003-04-12 <br>
[ * ] Fixed bug with incorrect counting FBytesWritten. <br>
[ * ] "DebugLevel" parameter name was changed to "DetailLevel" for terminology 
    consistence. It is not needed to change your old .lfg files, because it is 
    still possible to use old name ("DebugLevel") for back compatibility. <br> 
[ * ] %DEBUGLEVEL% placeholder name was changed to the %DETAILLEVEL%. It is 
    possible to use old name for back compatibility, but in new applications 
    new name %DETAILLEVEL% should be used. <br>

!!! Version 1.02 is released !!! <br>

2003-04-01 <br>
[+] testLogEngine project is ported to the Borland C++ Builder 6. <br>

2003-03-21 <br>
[+] Changes to support C++ Builder 6. <br>
[+] Added project prj/Builder6/LogEngine to compile LogEngine.lib library
    under C++ Builder 6.  <br>
[*] Fixed bug in THArrayException.what method definition. <br>


!!! Version 1.01 is released !!! <br>

2003-03-11 <br>
[*] new features are buildable for Linux. <br>

2003-03-09 <br>
[+] Added test for new IntToStr function. <br>
[+] Added overloaded global function IntToStr(int Value, int FieldSize). <br>
[ * ] Fixed bug with different log patterns. <br>
[ * ] Changed implementation for placeholders %OSVERSION%, %DEBUGLEVEL%, %APPVERSION%,
    %APPNAME%, %OS%. <br>
[+] Changed appropriate tests to support new LogEvent constructor. <br>
[+] Added field "logEngine" to the LogEvent structure.  <br>


2003-02-22 <br>
[+] Created implementation for %OSVERSION% placeholder. <br>
[+] Added placeholder %DEBUGLEVEL%. <br>


!!! Version 1.00 is released !!! <br>

2003-02-19 <br>
[ * ] fixed segmentation fault in TLogEngine::ThreadProc (wrong pointer) <br>
[ * ] fixed tests compilation problems under Linux <br>

2003-02-18 <br>
[+] Added method IsStarted() to the TLogEngine class. <br>
[ * ] Many tests were to use FormatXXX and the following ASSERT() instead of WriteXXX. <br>
[ * ] Many tests were rewritten to use ASSERT_EQUAL() macro instead of ASSERT() to produce <br>
    more understandable description. <br>
[+] Added tests: testLogInitClose(), testLogStartStop(). <br>

2003-02-16 <br>
[+] Added functions FormatXXX to format log messages before writing to the file.  <br>
[ * ] Global functions InitLogEngine(), CloseLogEngine(), getLogEngine() were rewritten to 
    call static methods of TLogEngine with the same names.  <br>
[ * ] Internal global variable loginstance was moved as static to the 
    TLogEngine class. <br>
[*] Constructors and destructor of TLogEngine class were moved to the 
    protected section to avoid creating several instances of logengine. <br>

2003-02-11 <br>
[*] new features are buildable for Linux. <br>

2003-02-09 <br>
[ * ] Fixed a serious bug with THArray copy constructor (file DynamicArrays.h). <br>
[ * ] Fixed bug in LogException.what() method which didn't allow to print error message properly. <br>
[+] Added overloaded InitLogEngine() function without parameters (default values will be used). <br>
[*] Added modificator "const" in parameters of the several methods of TFileStream class. <br>

!!! Version 1.0pre2 was released !!! <br>

2003-02-08 <br>
[+] Added class Line for internal use. <br>
[ * ] Parsing of line patters was remade to improve performance. <br>
[ * ] Changes in BooToStr function. <br>
[ * ] Changes in WriteXXX functions to support new line patterns parser. <br>
[ * ] BytesWritten and MessageCount[msgType] counters are work properly now. <br>

!!! Version 1.0pre1 was released !!! <br>

2003-02-06 <br>
[+] Added bulid framework for unix systems. <br>
[+] Fixed compilation problems. <br>

2003-02-05 <br>
[+] Begins work to support writing log in separate thread (only for Windows now). <br>
[+] Added LogEvent class. I am going to remake all internal calls to use LogEvent class instead of
    list of parameters) <br>

2003-02-01 <br>
[+] Added InitLogEngine(Properties& Props) - global LogEngine initialization function. <br>

2003-01-20 <br>
[*] GetCurrTime function is changed to return milliseconds. <br>

2003-01-17 <br>
[ * ] Fixed bug with the symbol # in the middle line of config file. <br>
[ * ] Several methods are added. <br>

2003-01-12 <br>
[*] LogEngine Header files were moved from src to include directory. <br>

2003-01-11 <br>
[+] Added MSVC project "LogEngine" for building LogEngine library. <br>
[+] Added MSVC test project "testLogEngine" for testing LogEngine classes. <br>
[+] Added Properties class for reading properties from config file (*.lfg). <br>
[+] Added tests for Properties class. <br>

2002-12-24 <br>
[+] Added functions FloatToStr, BoolToStr, StrToBool, EqualNCase into functions.cpp. <br>
[ * ] functions.cpp code was reformatted for better look. <br>
[+] Changes in IOException nethods. <br>
[+] Added current thread ID into log line pattern. <br>
[ * ] Some changes for linux support (not tested yet). <br>
[ * ] LogEngine.cpp code was reformatted for better look. <br>

