# Microsoft Developer Studio Project File - Name="LogEngine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LogEngine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LogEngine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LogEngine.mak" CFG="LogEngine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LogEngine - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LogEngine - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LogEngine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../../include/logengine" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\LogEngine.lib"

!ELSEIF  "$(CFG)" == "LogEngine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../../include/logengine" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\lib\LogEngined.lib"

!ENDIF 

# Begin Target

# Name "LogEngine - Win32 Release"
# Name "LogEngine - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\Compare.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\DynamicArrays.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\FileStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\functions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Line.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\LogEngine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\LogEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Properties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\SynchronizedQueue.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\include\logengine\Compare.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\logengine\DynamicArrays.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\logengine\FileStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\logengine\functions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\logengine\Line.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\logengine\LogEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\logengine\LogEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\logengine\Properties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\logengine\SynchronizedQueue.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\logengine\threads.h
# End Source File
# End Group
# End Target
# End Project
