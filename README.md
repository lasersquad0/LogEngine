               LogEngine --- The C++ Log Library
            https://github.com/lasersquad0/LogEngine2
                       Version 1.02


Last revision date September, 05, 2023.


TABLE OF CONTENTS
----------------
Overview <br>
Installation <br>
Demonstration Programs <br>
Compatibility <br>
Source Files <br>
Known problems <br>
License Agreement <br>
Copyright Notes <br>
Contacts <br>
History <br>

Overview
--------

LogEngine is the C++ library for easy and flexible logging to files.
The main features are: easy to use, small amount of source code.

Bug reports are welcome. Please use the Github bug tracking
system at https://github.com/lasersquad0/LogEngine2. 

See testXXX.cpp files from test/ directory as an examples of using 
logengine library.

See test/testFiles/example.lfg - description of logengine parameters 
file.

Email to the current maintainer may be sent to
lasersquad@gmail.com.

Installation
------------
**Microsoft Windows** <br>
Use MSVC projects from prj/msvcX/LogEngine/ to compile LogEngine.lib (or LogEngined.lib with Debug info).

See prj/msvcX/testLogEngine (or prj/Builder6/testLogEngine) for examples 
of use LogEngine.

**Other systems (Linux, etc)** <br>
For other systems see INSTALL txt file for details.

Demonstration Programs
----------------------
Unit tests .h and .cpp files for LogEngine can be found in test/ folder.
These tests show you how to properly use LogEngine.

To run them under Windows pls use MS Visual Studio project from here prj/msvcX/testLogEngine/ <br>
To run tests under Linux - use appropriate Makefiule files located in test/ folder.

Compatibility
-------------

LogEngine was recently tested on Windows 10, Windows 11, Ubuntu 22.04.2. <br>
It does not require any specific libraties, and uses standard STL classes, so it should work on many other common Windows and unix systems.

History
-------
[+] means "added"
[-] means "removed"
[*] means "fixed" or "modified"

2007-02-25 <br>
[+] Improved error handling during opening .lfg file. <br>
[*] Improved check for secure CRT under VS2005 (#if _MSC_VER < 1400 replaced by #if __STDC_SECURE_LIB__)  <br> 
 <br>

2006-11-12  <br>
[+] Created separate projects for VS6, VS .NET and VS2005  <br>
[*] Fixed a couple of security warnings under VS2005  <br>


2004-03-12
documentation manual?
[*] if FileName is not specifies that AppName is got for the log file name.

2003-04-12
[*] Fixed bug with incorrect counting FBytesWritten.
[*] "DebugLevel" parameter name was changed to "DetailLevel" for terminology 
    consistence. It is not needed to change your old .lfg files, because it is 
    still possible to use old name ("DebugLevel") for back compatibility. 
[*] %DEBUGLEVEL% placeholder name was changed to the %DETAILLEVEL%. It is 
    possible to use old name for back compatibility, but in new applications 
    new name %DETAILLEVEL% should be used.

!!! Version 1.02 is released !!!

2003-04-01
[+] testLogEngine project is ported to the Borland C++ Builder 6.

2003-03-21
[+] Changes to support C++ Builder 6.
[+] Added project prj/Builder6/LogEngine to compile LogEngine.lib library
    under C++ Builder 6.
[*] Fixed bug in THArrayException.what method definition.


!!! Version 1.01 is released !!!

2003-03-11
[*] new features are buildable for Linux.

2003-03-09
[+] Added test for new IntToStr function.
[+] Added overloaded global function IntToStr(int Value, int FieldSize).
[*] Fixed bug with different log patterns.
[*] Changed implementation for placeholders %OSVERSION%, %DEBUGLEVEL%, %APPVERSION%,
    %APPNAME%, %OS%.
[+] Changed appropriate tests to support new LogEvent constructor.
[+] Added field "logEngine" to the LogEvent structure. 


2003-02-22
[+] Created implementation for %OSVERSION% placeholder.
[+] Added placeholder %DEBUGLEVEL%.


!!! Version 1.00 is released !!!

2003-02-19
[*] fixed segmentation fault in TLogEngine::ThreadProc (wrong pointer)
[*] fixed tests compilation problems under Linux

2003-02-18
[+] Added method IsStarted() to the TLogEngine class.
[*] Many tests were to use FormatXXX and the following ASSERT() instead of WriteXXX.
[*] Many tests were rewritten to use ASSERT_EQUAL() macro instead of ASSERT() to produce
    more understandable description.
[+] Added tests: testLogInitClose(), testLogStartStop().

2003-02-16
[+] Added functions FormatXXX to format log messages before writing to the file. 
[*] Global functions InitLogEngine(), CloseLogEngine(), getLogEngine() were rewritten to 
    call static methods of TLogEngine with the same names. 
[*] Internal global variable loginstance was moved as static to the 
    TLogEngine class.
[*] Constructors and destructor of TLogEngine class were moved to the 
    protected section to avoid creating several instances of logengine.

2003-02-11
[*] new features are buildable for Linux.

2003-02-09
[*] Fixed a serious bug with THArray copy constructor (file DynamicArrays.h).
[*] Fixed bug in LogException.what() method which didn't allow to print error message properly.
[+] Added overloaded InitLogEngine() function without parameters (default values will be used).
[*] Added modificator "const" in parameters of the several methods of TFileStream class.

!!! Version 1.0pre2 was released !!!

2003-02-08
[+] Added class Line for internal use.
[*] Parsing of line patters was remade to improve performance.
[*] Changes in BooToStr function.
[*] Changes in WriteXXX functions to support new line patterns parser.
[*] BytesWritten and MessageCount[msgType] counters are work properly now.

!!! Version 1.0pre1 was released !!!

2003-02-06
[+] Added bulid framework for unix systems.
[+] Fixed compilation problems.

2003-02-05
[+] Begins work to support writing log in separate thread (only for Windows now).
[+] Added LogEvent class. I am going to remake all internal calls to use LogEvent class instead of
    list of parameters)

2003-02-01
[+] Added InitLogEngine(Properties& Props) - global LogEngine initialization function.

2003-01-20
[*] GetCurrTime function is changed to return milliseconds.

2003-01-17
[*] Fixed bug with the symbol # in the middle line of config file.
[*] Several methods are added.

2003-01-12
[*] LogEngine Header files were moved from src to include directory.

2003-01-11
[+] Added MSVC project "LogEngine" for building LogEngine library.
[+] Added MSVC test project "testLogEngine" for testing LogEngine classes.
[+] Added Properties class for reading properties from config file (*.lfg).
[+] Added tests for Properties class.

2002-12-24
[+] Added functions FloatToStr, BoolToStr, StrToBool, EqualNCase into functions.cpp.
[*] functions.cpp code was reformatted for better look.
[+] Changes in IOException nethods.
[+] Added current thread ID into log line pattern.
[*] Some changes for linux support (not tested yet).
[*] LogEngine.cpp code was reformatted for better look.




