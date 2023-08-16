noinst_PROGRAMS = testLogEngine
testLogEngine_SOURCES = testLine.cpp \
testLog.cpp \
testLogEngine.cpp \
testProperties.cpp \
testThreadLog.cpp \
testUtils.cpp

testLogEngine_CXXFLAGS = -pthread -I../include/logengine -D_TESTMODE
testLogEngine_LDFLAGS = -Wl,-llogengine,-L../src/.libs

EXTRA_DIST=debug_support.h \
testLine.h \
testLog.h \
testProperties.h \
testThreadLog.h \
testUtils.h
