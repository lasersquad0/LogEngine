// UnitTestsRunnerDoc.h : interface of the CUnitTestsRunnerDoc class
//
/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_UnitTestsRunnerDOC_H)
#define AFX_UnitTestsRunnerDOC_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CUnitTestsRunnerDoc : public CDocument
{
protected: // create from serialization only
    CUnitTestsRunnerDoc();
    DECLARE_DYNCREATE(CUnitTestsRunnerDoc)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUnitTestsRunnerDoc)
    public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CUnitTestsRunnerDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CUnitTestsRunnerDoc)
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UnitTestsRunnerDOC_H)
