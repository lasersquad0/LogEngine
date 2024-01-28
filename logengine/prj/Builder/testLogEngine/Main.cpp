#include <iostream>
#include <tchar.h>

#include <cppunit/TestCase.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include "MyTestProgressListener.h"



int _tmain(int argc, _TCHAR* argv[])
{
    CppUnit::TestResult controller;

    CppUnit::TestResultCollector result;
    controller.addListener( &result );

    //CppUnit::BriefTestProgressListener progressListener;
    MyTestProgressListener progressListener;
    controller.addListener( &progressListener );

    CppUnit::TestRunner runner; // creates a test executor

    // use the suite factory, then execute all tests
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());

	runner.run(controller);

}
