// ScrollableDialog.cpp: implementation of the CScrollableDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zsIRC.h"
#include "ScrollableDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
CScrollableDialog::CScrollableDialog()
{

}

CScrollableDialog::~CScrollableDialog()
{

}
*/

CScrollableDialog::CScrollableDialog(int nIDC) : CPropertyPage(nIDC) {
  nScrollDelta = 0;
  size.x = 0;
  size.y = 0;
}

/*
CScrollableDialog::CScrollableDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	ASSERT(HIWORD(lpszTemplateName) == 0 ||
		AfxIsValidString(lpszTemplateName));

	AFX_ZERO_INIT_OBJECT(CWnd);

	m_pParentWnd = pParentWnd;
	m_lpszTemplateName = lpszTemplateName;
	if (HIWORD(m_lpszTemplateName) == 0)
		m_nIDHelp = LOWORD((DWORD)m_lpszTemplateName);
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	m_bFullScreen = TRUE;
#endif // _WIN32_WCE_PSPC
}

CScrollableDialog::CScrollableDialog(UINT nIDTemplate, CWnd* pParentWnd)
{
	AFX_ZERO_INIT_OBJECT(CWnd);

	m_pParentWnd = pParentWnd;
	m_lpszTemplateName = MAKEINTRESOURCE(nIDTemplate);
	m_nIDHelp = nIDTemplate;
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	m_bFullScreen = TRUE;
#endif // _WIN32_WCE_PSPC
}
*/

BEGIN_MESSAGE_MAP(CScrollableDialog, CPropertyPage)
	//{{AFX_MSG_MAP(CScrollableDialog)
	ON_WM_VSCROLL()
  ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsConnectionDlg message handlers

void CScrollableDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  int minpos;
  int maxpos;
  int curpos;

  GetScrollRange(SB_VERT,&minpos,&maxpos); 
  maxpos = GetScrollLimit(SB_VERT);
  curpos = GetScrollPos(SB_VERT);

  switch (nSBCode)
  {
    case SB_LEFT: {
      curpos = minpos;
    } break;

    case SB_RIGHT: {
      curpos = maxpos;
    } break;

    case SB_ENDSCROLL: {
    } break;

    case SB_LINELEFT: {
      if (curpos > minpos) curpos--;
    } break;

    case SB_LINERIGHT: {
      if (curpos < maxpos) curpos++;
    } break;

    case SB_PAGEUP: {
      SCROLLINFO info;
      GetScrollInfo(SB_VERT, &info, SIF_ALL);

      if (curpos > minpos)
        curpos = max(minpos, curpos - (int) info.nPage);
    } break;

    case SB_PAGEDOWN: {
      SCROLLINFO info;
      GetScrollInfo(SB_VERT, &info, SIF_ALL);

      if (curpos < maxpos)
        curpos = min(maxpos, curpos + (int) info.nPage);
    } break;

    case SB_THUMBPOSITION: {
      curpos = nPos + minpos;
    } break;

    case SB_THUMBTRACK: {
      curpos = nPos + minpos;
    } break;

  }

  curpos = min(curpos,maxpos);
  curpos = max(curpos,minpos);

  SetScrollPos(SB_VERT,curpos);

  DoVScroll(-curpos);

  InvalidateRect(NULL);

}

void CScrollableDialog::DoVScroll(int nValue) {
	HWND hWndChild = ::GetWindow(m_hWnd, GW_CHILD);
	if (hWndChild != NULL)
	{
		for (; hWndChild != NULL; hWndChild = ::WCE_FCTN(GetNextWindow)(hWndChild, GW_HWNDNEXT))
		{
			CRect rect;
			::GetWindowRect(hWndChild, &rect);
			ScreenToClient(&rect);
			::SetWindowPos(hWndChild, NULL,
				rect.left, rect.top+nValue-nScrollDelta, 0, 0,
				SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
		}
	}
  nScrollDelta = nValue;
}

BOOL CScrollableDialog::OnInitDialog() {
  CPropertyPage::OnInitDialog();

  RECT rectMax;
  HWND hWndChild = ::GetWindow(m_hWnd, GW_CHILD);
  if (hWndChild != NULL)
  {
    ::GetWindowRect(hWndChild, &rectMax);
		  for (; hWndChild != NULL; hWndChild = ::WCE_FCTN(GetNextWindow)(hWndChild, GW_HWNDNEXT))
      {
        CRect rect;
        ::GetWindowRect(hWndChild, &rect);
        rectMax.top    = min(rect.top   ,rectMax.top);
        rectMax.left   = min(rect.left  ,rectMax.left);
        rectMax.right  = max(rect.right ,rectMax.right);
        rectMax.bottom = max(rect.bottom,rectMax.bottom);
      }
  }
  size.x = rectMax.right  - rectMax.left;
  size.y = rectMax.bottom - rectMax.top + 10;

  RECT r;
  GetParent()->GetClientRect(&r);
  SetWindowPos(NULL,r.left,r.top,r.right-r.left,r.bottom-r.top-50,SWP_NOZORDER);

  return TRUE;
} 

void CScrollableDialog::OnSize(UINT type,int x,int y) {
  CPropertyPage::OnSize(type,x,y);

  if (!size.y) return;

  RECT r;
  GetParent()->GetClientRect(&r);
  
  SCROLLINFO si;
  si.cbSize = sizeof(SCROLLINFO);
  si.nMin = 0;
  si.nMax = size.y;
  si.nPage = r.bottom - r.top - 50;
  si.nPos = 0;
  si.fMask = SIF_ALL;
  SetScrollInfo(SB_VERT,&si);
  DoVScroll(0);
 
}