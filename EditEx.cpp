// EditEx.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "zsIRC_Wnd_Channel.h"
#include "EditEx.h"
#include "SIPApi.h"
#include "zsIRC_StringTokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MainFrm.h"
/////////////////////////////////////////////////////////////////////////////
// CEditEx

CEditEx::CEditEx()
{
}

CEditEx::~CEditEx()
{
}

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
  //{{AFX_MSG_MAP(CEditEx)
  ON_WM_KEYDOWN()
  ON_WM_LBUTTONDOWN()
  ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)
#if UNDER_CE >= 420
  ON_WM_CONTEXTMENU()
#endif
  ON_COMMAND(ID_EDIT_CUT, OnEditCut)
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define BUFFERLIMIT 20

/////////////////////////////////////////////////////////////////////////////
// CEditEx message handlers

void CEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  CMainFrame * m = (CMainFrame *)GetParent();
  // TODO: Add your message handler code here and/or call default
  if (nChar == VK_RETURN) {

    CString s;
    GetWindowText(s);
    s.TrimLeft();
    s.TrimRight();
    if (s.GetLength()) {
      m->GetActiveChannelBar()->ParseCommand(s.LockBuffer(),(GetKeyState(VK_CONTROL)&0x8000) > 0);

      if (daBuffer.Num() > BUFFERLIMIT) daBuffer.Delete(0);

      CString * s2 = new CString(s);
      daBuffer.Add(s2);
      nPosInBuffer = daBuffer.Num();
    }
    SetWindowText(_T(""));
    
    return;
  }
  else if (nChar == VK_UP) {
    if (!daBuffer.Num()) return;
    nPosInBuffer = max(nPosInBuffer-1,0);
    CString * s = (CString *)daBuffer[nPosInBuffer];
    if (s) SetWindowText(*s);
  }
  else if (nChar == VK_DOWN) {
    if (!daBuffer.Num()) return;
    if (nPosInBuffer <= daBuffer.Num()-1) {
      nPosInBuffer++;
      CString * s = (CString *)daBuffer[nPosInBuffer];
      if (s) SetWindowText(*s);
    } else {
      CString s;
      GetWindowText(s);
      s.TrimLeft();
      s.TrimRight();

      if (daBuffer.Num() > BUFFERLIMIT) daBuffer.Delete(0);

      CString * s2 = new CString(s);
      daBuffer.Add(s2);
      nPosInBuffer = daBuffer.Num();

      SetWindowText(_T(""));
    }
  }
  else if (nChar == VK_TAB) {
    int p1,p2;
    GetSel(p1,p2);
    CString cont;
    GetWindowText(cont);    

    CString leftr = cont.Left(p1), comp=_T("");
    int q = leftr.ReverseFind(_T(' '));
    if (q!=-1) {
      comp = leftr.Mid(q+1);
      comp.TrimLeft();
      comp.TrimRight();
      leftr = leftr.Left(q+1);
    } else {
      comp = leftr;
      leftr = _T("");
    }
  
    CMainFrame * m = (CMainFrame *)GetParent();
    if (comp.GetLength()) {
      if (m->GetActiveChannelBar()->sChanTypes.Find(comp[0])!=-1) {
        if (comp.GetLength()==1) {
          comp = ((zsIRC_Wnd*)m->GetActiveChannelBar()->daWindows[m->GetActiveChannelBar()->nSelected])->szCaption;
        } else {
          for (int i=0; i<m->GetActiveChannelBar()->daWindows.Num(); i++) {
            zsIRC_Wnd * w = ((zsIRC_Wnd*)m->GetActiveChannelBar()->daWindows[i]);
            CString c = w->szCaption;
            if (c.Left(comp.GetLength()).CompareNoCase(comp)==0) {
              comp = c;
              break;
            }
          }
        }
      } else {
        if ( ((zsIRC_Wnd*)m->GetActiveChannelBar()->daWindows[m->GetActiveChannelBar()->nSelected])->nType == ZSIRC_WT_CHANNEL ) {
          zsIRC_Wnd_Channel * c = ((zsIRC_Wnd_Channel*)m->GetActiveChannelBar()->daWindows[m->GetActiveChannelBar()->nSelected]);
          for (int i=0; i<c->daPeople.Num(); i++) {
            CString u = ((ZSIRC_USER*)(c->daPeople[i]))->sNick;
            if (u.Left(comp.GetLength()).CompareNoCase(comp)==0) {
              comp = u;
              break;
            }
          }
        }
      }
    }

    CString lll = leftr + comp;
    CString n = lll + cont.Mid(p2);
    SetWindowText(n.LockBuffer());
    SetSel(lll.GetLength(),lll.GetLength());
  }
    
  CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
  // TODO: Add your message handler code here and/or call default
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
	  mPopup.LoadMenu(IDR_EDIT);
	  CMenu * mSubmenu = mPopup.GetSubMenu(0);
	  mSubmenu->TrackPopupMenu(NULL, point.x, point.y, this);
  } else Default();
#endif
  
//  CEdit::OnLButtonDown(nFlags, point);
}

void CEditEx::OnSetfocus() 
{
  // TODO: Add your control notification handler code here
#if UNDER_CE > 420
  SipShowIM(SIPF_ON); 
#endif
}

void CEditEx::OnContextMenu(CWnd* pWnd, CPoint point) 
{
  // TODO: Add your message handler code here
/*
  CMenu mPopup;
  mPopup.LoadMenu(IDR_EDIT);
  CMenu * mSubmenu = mPopup.GetSubMenu(0);
  mSubmenu->TrackPopupMenu(NULL, point.x, point.y, this);
*/
}

void CEditEx::OnEditCut() 
{
  // first we copy
  OnEditCopy();

  CString cont;
  int p1,p2;
  GetSel(p1,p2);
  GetWindowText(cont);
  CString n = cont.Left(p1) + cont.Mid(p2);
  SetWindowText(n.LockBuffer());
}

void CEditEx::OnEditCopy() 
{
  if (!OpenClipboard()) return;

  EmptyClipboard();

  int p1,p2;
  GetSel(p1,p2);
  CString cont;
  GetWindowText(cont);

  int nLen = cont.Mid(p1,p2-p1).GetLength() + 1;
  TCHAR * pGlobal = (TCHAR*)LocalAlloc(LPTR, sizeof(TCHAR) * nLen);
  if (!pGlobal) {
    CloseClipboard();
    return;
  }

  CopyMemory(pGlobal,cont.Mid(p1,p2-p1).LockBuffer(),sizeof(TCHAR) * nLen);

  ::SetClipboardData(CF_TTEXT,(HANDLE)pGlobal);
  CloseClipboard();
}

void CEditEx::OnEditPaste() 
{
  if (!OpenClipboard()) return;
  // TODO: Add your command handler code here
  CMainFrame * m = (CMainFrame *)GetParent();

  TCHAR * pText = (TCHAR*) GetClipboardData(CF_TTEXT);
  CString buffer(pText);
  buffer.TrimLeft();
  buffer.TrimRight();
  if (buffer.GetLength()) {
    if (buffer.Find(_T('\n'))!=-1) {
      int p1,p2;
      GetSel(p1,p2);
      CString cont;
      GetWindowText(cont);
      CString n = cont.Left(p1) + buffer + cont.Mid(p2);

      zsIRC_StringTokenizer stLines(n);
      for (int i=0; i<=stLines.GetOccurrences(_T('\n')); i++) {
        CString t = stLines.GetSlice(_T('\n'),i);
        t.TrimLeft();
        t.TrimRight();
        m->GetActiveChannelBar()->ParseCommand(t.LockBuffer());
      }
      SetWindowText(_T(""));
    } else {
      int p1,p2;
      GetSel(p1,p2);
      CString cont;
      GetWindowText(cont);
      CString n = cont.Left(p1) + buffer + cont.Mid(p2);
      SetWindowText(n.LockBuffer());
      SetSel(p1,p1+_tcslen(buffer));
    }
  }
  CloseClipboard(); 

}


void CEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
  if (nChar == VK_RETURN) 
    return;
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
