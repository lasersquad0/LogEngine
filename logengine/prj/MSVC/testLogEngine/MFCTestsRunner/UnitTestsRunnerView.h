// UnitTestsRunnerView.h : interface of the CUnitTestsRunnerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UnitTestsRunnerVIEW_H)
#define AFX_UnitTestsRunnerVIEW_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CUnitTestsRunnerView : public CView
{
protected: // create from serialization only
    CUnitTestsRunnerView();
    DECLARE_DYNCREATE(CUnitTestsRunnerView)

// Attributes
public:
    CUnitTestsRunnerDoc* GetDocument();

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUnitTestsRunnerView)
    public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CUnitTestsRunnerView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CUnitTestsRunnerView)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in UnitTestsRunnerView.cpp
inline CUnitTestsRunnerDoc* CUnitTestsRunnerView::GetDocument()
   { return (CUnitTestsRunnerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UnitTestsRunnerVIEW_H)
