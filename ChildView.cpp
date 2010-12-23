// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "zsIRC.h"
#include "ChildView.h"
#include "zsIRC_Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CChannelBar * g_ChannelBar = NULL;

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
  nSelected = 0;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CANCELMODE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		NULL, HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

  if (AddNewChannelbar() != 0)
    return -1;

	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);

  CRect r;
  GetParent()->GetClientRect(r);
  for (int i=0; i<daChannelBars.Num(); i++) {
    ((CChannelBar*)daChannelBars[i])->MoveWindow(CRect(r.left, r.bottom-SETUP.nChannelBarHeight, r.right, r.bottom));
  }
	
//  for (int i=0; i<m_ChannelBar.daWindows.Num(); i++)
//    m_ChannelBar.daWindows[i]->MoveWindow(CRect(r.left, 0, r.right, r.bottom-SETUP.nChannelBarHeight));
	
}

void CChildView::OnCancelMode() 
{
	CWnd ::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CChildView::OnDestroy() 
{
//  g_ChannelBar = NULL;
  TRACE0("CChildView::OnDestroy()\n");
	CWnd ::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CChildView::InvalidateChannels() {
//  if (!nDestroyed)
//    m_ChannelBar.InvalidateRect(NULL);
}

int CChildView::AddNewChannelbar() {
  nDestroyed = 1;
  
  CChannelBar * m_ChannelBar = new CChannelBar();
  CRect r;
  GetParent()->GetClientRect(r);
  // TODO: Add your specialized creation code here
  if (!m_ChannelBar->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
    CRect(r.left, r.bottom-SETUP.nChannelBarHeight, r.right, r.bottom), this, NULL, NULL))
  {
    delete m_ChannelBar;
    TRACE0("Failed to create childview window\n");
    return -1;
  }
  daChannelBars.Add(m_ChannelBar);

  SetActiveChannelBar( daChannelBars.Num() - 1 );

  nDestroyed = 0;

  return 0;
}

CChannelBar * CChildView::GetActiveChannelBar() {
  return ((CChannelBar*)daChannelBars[nSelected]);
}

void CChildView::SetActiveChannelBar(int w) {
  if (w < 0) return;
  if (w >= daChannelBars.Num()) return;
  for (int i=0; i<daChannelBars.Num(); i++) {
    if ( i == w ) {
      ((CChannelBar*)daChannelBars[i])->ShowWindow( SW_SHOW );
      ((CChannelBar*)daChannelBars[i])->FocusWindow( ((CChannelBar*)daChannelBars[i])->nSelected );
      nSelected = i;
    } else {
      ((CChannelBar*)daChannelBars[i])->ShowWindow( SW_HIDE );
      ((CChannelBar*)daChannelBars[i])->HideAllWindows();
    }
  }
}

void CChildView::RemoveActiveChannelBar() {
  ((CChannelBar*)daChannelBars[nSelected])->SendMessage(WM_CLOSE);
  //delete ((CChannelBar*)daChannelBars[nSelected]);
  daChannelBars.Delete( nSelected );
  SetActiveChannelBar( max( 0, nSelected - 1 ) );
}