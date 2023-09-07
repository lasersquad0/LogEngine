#pragma once

#include <cppunit/TestListener.h>
#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>
#include <iostream>


/*! \brief TestListener that prints the name of each test before running it.
 * \ingroup TrackingTestExecution
 */
	class MyTestProgressListener : public CppUnit::TestListener
{
public:
	MyTestProgressListener(): m_lastTestFailed(false), m_failedTestCnt(0) { }

	/// Destructor.
	virtual ~MyTestProgressListener() {}

	void startTest(CppUnit::Test* test)
	{	
		std::cout << test->getName();
		std::cout.flush();

		m_lastTestFailed = false;
	}

	void addFailure(const CppUnit::TestFailure& failure)
	{
		if (failure.isError())
		{
			std::cout << " : ERROR at ";
			std::cout << failure.sourceLine().fileName() << " (" << failure.sourceLine().lineNumber() << ")" << std::endl;
			std::cout << failure.thrownException()->message().details();
		}
		else 
		{
			std::cout << " : ASSERTION at ";
			std::cout << failure.sourceLine().fileName() << " (" << failure.sourceLine().lineNumber() << ")" << std::endl;
			std::cout << failure.thrownException()->message().details();
		}

		m_lastTestFailed = true;
		m_failedTestCnt++;
	}

#pragma warning(push)
#pragma warning(disable : 4100)
	void endTest(CppUnit::Test* test)
	{
		if (!m_lastTestFailed) std::cout << " : OK";
		std::cout << "\n";
	}

	void endTestRun(CppUnit::Test* test, CppUnit::TestResult* result) 
	{
		std::cout << "Run Tests   :" << test->countTestCases() << std::endl;
		std::cout << "Failed Tests:" << m_failedTestCnt << std::endl;
	}
#pragma warning(pop)

private:
	/// Prevents the use of the copy constructor.
	MyTestProgressListener(const MyTestProgressListener& copy);

	/// Prevents the use of the copy operator.
	void operator =(const MyTestProgressListener& copy);

private:
	bool m_lastTestFailed;
	int m_failedTestCnt;
};

