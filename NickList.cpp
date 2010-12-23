// NickList.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "NickList.h"
#include "ChildView.h"
#include <wceres.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNickList

CNickList::CNickList()
{
}

CNickList::~CNickList()
{
}


BEGIN_MESSAGE_MAP(CNickList, CListBox)
	//{{AFX_MSG_MAP(CNickList)
	ON_COMMAND(IDR_USER_QUERY, OnUserQuery)
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnDblclk)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDR_USER_WHOIS, OnUserWhois)
	ON_COMMAND(IDR_USER_WHOISEX, OnUserWhoisex)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNickList message handlers

//DEL void CNickList::OnContextMenu(CWnd* pWnd, CPoint point) 
//DEL {
//DEL 	// TODO: Add your message handler code here
//DEL //  OnLButtonDown(NULL, point);
//DEL }


CString CNickList::GetCurSelNick()
{
  int nIndex = GetCurSel();
  if (nIndex<0) return CString("");
  CString s;
  GetText(nIndex,s);
//  if (s[0] == _T('@')) s = s.Mid(1);
//  if (s[0] == _T('+')) s = s.Mid(1);
  while ( GetChannelBar()->sPrefixes.Find( s[0] ) != -1 ) s = s.Mid(1);
  return s;
}

void CNickList::OnUserQuery() 
{
	// TODO: Add your command handler code here
  CString s = GetCurSelNick();
  if (!s.GetLength()) return;

  GetChannelBar()->AddWindow(ZSIRC_WT_QUERY,s.LockBuffer());
}

void CNickList::OnDblclk() 
{
	// TODO: Add your control notification handler code here
}

void CNickList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
  CString s = GetCurSelNick();
  if (!s.GetLength()) return;

  GetChannelBar()->AddWindow(ZSIRC_WT_QUERY,s.LockBuffer());
	
	CListBox::OnLButtonDblClk(nFlags, point);
}

void CNickList::OnUserWhois() 
{
	// TODO: Add your command handler code here
  CString s = GetCurSelNick();
  if (!s.GetLength()) return;

  CString str = _T("WHOIS ");	
  str += s;

  GetChannelBar()->SendIRCMessage(str.LockBuffer());
}

void CNickList::OnUserWhoisex() 
{
	// TODO: Add your command handler code here
  CString s = GetCurSelNick();
  if (!s.GetLength()) return;

  CString str = _T("WHOIS ");	
  str += s;
  str += _T(" ");
  str += s;

  GetChannelBar()->SendIRCMessage(str.LockBuffer());
	
}

void CNickList::OnLButtonDown(UINT nFlags, CPoint point) 
{
#if UNDER_CE >= 420
  SHRGINFO shrgi = {0};

  shrgi.cbSize        = sizeof(SHRGINFO);
  shrgi.hwndClient    = m_hWnd;
  shrgi.ptDown.x      = point.x;
  shrgi.ptDown.y      = point.y;
  shrgi.dwFlags       = SHRG_RETURNCMD;

  if(GN_CONTEXTMENU == ::SHRecognizeGesture(&shrgi)) {
    ClientToScreen(&point);
	  CMenu mPopup;
	  mPopup.LoadMenu(IDR_USER);
	  CMenu * mSubmenu = mPopup.GetSubMenu(0);
	  mSubmenu->TrackPopupMenu(NULL, point.x, point.y, this);
  } else Default();
#endif
}

int CNickList::Compare(CString& lpszText1, CString& lpszText2) {
//   ASSERT(lpszText1 != NULL);
//   ASSERT(lpszText2 != NULL);

  int n1 = GetChannelBar()->sPrefixes.Find( lpszText1[0] );
  int n2 = GetChannelBar()->sPrefixes.Find( lpszText2[0] );

  if (n1!=-1) {
    if (n2!=-1) {
      if (n1 < n2) return -1;
      if (n1 > n2) return 1;
      int t = _tcsicmp(lpszText1.Mid(1), lpszText2.Mid(1));
      //TRACE3("... _tcsicmp %s to %s = %d ... ",lpszText1.Mid(1), lpszText2.Mid(1), t);
      return t;
    } else {
      return -1;
    }
  } else if (n2!=-1) {
    return 1;
  }

  return _tcsicmp(lpszText1, lpszText2);
}

BOOL CNickList::PreCreateWindow(CREATESTRUCT &cs) {
  if (!CListBox::PreCreateWindow (cs))
    return FALSE;
  
  cs.style &= ~(LBS_HASSTRINGS | LBS_SORT);
  cs.style |= LBS_NOINTEGRALHEIGHT;
  return TRUE;
}

int CNickList::InsertStringSorted(TCHAR*sz) {
  //TRACE1("=== %s ===\n",sz);
  for (int i=0; i<GetCount(); i++) {
    CString pivot;
    GetText(i,pivot);
    //TRACE3("comparing %s to %s (%d)",sz,pivot,i);
    if (Compare(pivot,CString(sz)) > 0) {
      //TRACE0(" ... PASSED\n");
      InsertString(i,sz);
      return i;
    }
    //TRACE0(" ... next\n");
  }
  //TRACE0("LAST\n");
  InsertString(-1,sz);
  return -1;
}

CChannelBar * CNickList::GetChannelBar() {
  return ((zsIRC_Wnd*)GetParent())->GetChannelBar();
}