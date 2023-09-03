AUTOMAKE_OPTIONS = subdir-objects

noinst_PROGRAMS = testLogEngineApp
testLogEngineApp_SOURCES = testLine.cpp \
testLog.cpp \
testRunner.cpp \
testProperties.cpp \
testThreadLog.cpp \
testUtils.cpp


testLogEngineApp_CXXFLAGS = -Wall -Wno-reorder -I../../../cppunit-1.11.6/include -I../include/logengine -D_TESTMODE
testLogEngineApp_LDFLAGS = -llogengine -lcppunit -L../src/.libs -L../../../cppunit-1.11.6/src/cppunit/.libs

EXTRA_DIST=debug_support.h \
testLine.h \
testLog.h \
testProperties.h \
testThreadLog.h \
testUtils.h
