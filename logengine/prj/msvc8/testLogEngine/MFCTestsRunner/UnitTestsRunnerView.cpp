// UnitTestsRunnerView.cpp : implementation of the CUnitTestsRunnerView class
//

#include "stdafx.h"
#include "UnitTestsRunner.h"

#include "UnitTestsRunnerDoc.h"
#include "UnitTestsRunnerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerView

IMPLEMENT_DYNCREATE(CUnitTestsRunnerView, CView)

BEGIN_MESSAGE_MAP(CUnitTestsRunnerView, CView)
    //{{AFX_MSG_MAP(CUnitTestsRunnerView)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerView construction/destruction

CUnitTestsRunnerView::CUnitTestsRunnerView()
{
    // TODO: add construction code here

}

CUnitTestsRunnerView::~CUnitTestsRunnerView()
{
}

BOOL CUnitTestsRunnerView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerView drawing

void CUnitTestsRunnerView::OnDraw(CDC* pDC)
{
    CUnitTestsRunnerDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerView printing

BOOL CUnitTestsRunnerView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void CUnitTestsRunnerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void CUnitTestsRunnerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerView diagnostics

#ifdef _DEBUG
void CUnitTestsRunnerView::AssertValid() const
{
    CView::AssertValid();
}

void CUnitTestsRunnerView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CUnitTestsRunnerDoc* CUnitTestsRunnerView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUnitTestsRunnerDoc)));
    return (CUnitTestsRunnerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUnitTestsRunnerView message handlers
