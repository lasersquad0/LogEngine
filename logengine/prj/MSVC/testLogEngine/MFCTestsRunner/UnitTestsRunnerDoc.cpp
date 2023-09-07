// UnitTestsRunnerDoc.cpp : implementation of the CUnitTestsRunnerDoc class
//

#include "stdafx.h"
#include "UnitTestsRunner.h"

#include "UnitTestsRunnerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerDoc

IMPLEMENT_DYNCREATE(CUnitTestsRunnerDoc, CDocument)

BEGIN_MESSAGE_MAP(CUnitTestsRunnerDoc, CDocument)
    //{{AFX_MSG_MAP(CUnitTestsRunnerDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerDoc construction/destruction

CUnitTestsRunnerDoc::CUnitTestsRunnerDoc()
{
    // TODO: add one-time construction code here
}

CUnitTestsRunnerDoc::~CUnitTestsRunnerDoc()
{
}



BOOL CUnitTestsRunnerDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerDoc serialization

void CUnitTestsRunnerDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }
}

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerDoc diagnostics

#ifdef _DEBUG
void CUnitTestsRunnerDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CUnitTestsRunnerDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerDoc commands
