// UnitTestsRunner.h : main header file for the UnitTestsRunner application
//

#if !defined(AFX_UnitTestsRunner_H__A9C94DE7_1663_11D2_A499_00805FC1C042__INCLUDED_)
#define AFX_UnitTestsRunner_H__A9C94DE7_1663_11D2_A499_00805FC1C042__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerApp:
// See UnitTestsRunner.cpp for the implementation of this class
//

class CUnitTestsRunnerApp : public CWinApp
{
public:
    CUnitTestsRunnerApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUnitTestsRunnerApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CUnitTestsRunnerApp)
    afx_msg void OnAppAbout();
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP();

private:
  void RunUnitTests();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UnitTestsRunner_H__A9C94DE7_1663_11D2_A499_00805FC1C042__INCLUDED_)
