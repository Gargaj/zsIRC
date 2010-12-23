// zsIRC_Wnd_Query.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "zsIRC_Wnd_Query.h"
#include "zsIRC_Settings.h"
#include "zsIRC_Line.h"
#include "ChildView.h"
#include "OffscreenDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// zsIRC_Wnd_Query

zsIRC_Wnd_Query::zsIRC_Wnd_Query(CChannelBar * c) : zsIRC_Wnd(c)
{
  ptSelect = CPoint(-1,-1);
  nSelectLine = -1;
}

zsIRC_Wnd_Query::~zsIRC_Wnd_Query()
{
}


BEGIN_MESSAGE_MAP(zsIRC_Wnd_Query, CWnd)
  //{{AFX_MSG_MAP(zsIRC_Wnd_Query)
  ON_WM_PAINT()
  ON_WM_CREATE()
  ON_WM_VSCROLL()
	ON_WM_DESTROY()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
//	ON_WM_ACTIVATEAPP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define isnum(x) (  _T('0')<=(x) && (x)<=_T('9')  )
/////////////////////////////////////////////////////////////////////////////
// zsIRC_Wnd_Query message handlers

void zsIRC_Wnd_Query::OnPaint() 
{
//  CPaintDC dc(this); // device context for painting
  COffscreenDC dc(this,CRect(0,0,0,0));

  //int nScrollPos = GetScrollPos(SB_VERT);

  // TODO: Add your message handler code here
  CRect r;
  GetClientRect(&r);
  CBrush b(SETUP.cBackgroundColor);
  dc.GetDC()->FillRect(&r,&b);

  if (aLines.Num()<=0) return;

  LOGFONT lf;
  ZeroMemory(&lf,sizeof(LOGFONT));
  _tcsnccpy(lf.lfFaceName,SETUP.szConversationFont,LF_FACESIZE);
  lf.lfHeight = SETUP.nConversationFontSize;
  lf.lfWeight = FW_NORMAL;
  lf.lfPitchAndFamily = DEFAULT_PITCH;
  HFONT font = CreateFontIndirect(&lf);

  HFONT fontold = (HFONT)dc.GetDC()->SelectObject(font);

  int nTop = r.bottom;

  for (int i=nScrollPos-1; i>=0; i--) {
    zsIRC_Line * line = (zsIRC_Line *) aLines[i];

    CString str = line->toString();

    if (SETUP.nStripColorCodes) 
      for (int j=0; j<str.GetLength(); j++) {
        if (str[j]==_T('\x02')) { // BOLD
          str = str.Left(j) + str.Mid(j+1);
        }
        else if (str[j]==_T('\x1F')) { // underline(?)
          str = str.Left(j) + str.Mid(j+1);
        }
        else if (str[j]==_T('\x03')) { // color
          TCHAR * szMasks[] = {
            _T("xx,yy"),
            _T("xx,y"),
            _T("x,yy"),
            _T("x,y"),
            _T("xx"),
            _T("x"),
          };
          int nFittingMask = -1;
          for (int k=0; k<6; k++) {
            if (str.GetLength() - j - 2 >= (int)_tcslen(szMasks[k])) {
              int nMaskFits = 1;
              for (int l=0; szMasks[k][l]; l++) {
                if (szMasks[k][l] == _T('x') && !isnum(str[j+l+1]))  nMaskFits = 0;
                if (szMasks[k][l] == _T('y') && !isnum(str[j+l+1]))  nMaskFits = 0;
                if (szMasks[k][l] == _T(',') && str[j+l+1]!=_T(',')) nMaskFits = 0;
              }
              if (nMaskFits) {
                nFittingMask = k;
                break;
              }
            }
          }
          if (nFittingMask != -1) {
            str = str.Left(j) + str.Mid(j+1+_tcslen(szMasks[nFittingMask]));
          } else {
            str = str.Left(j) + str.Mid(j+1);
          }
        }
    }

    int nTimes = 0;
    while (1) {
      if (nTimes++>20) break; // silly watchdog mechanism
      if (!BreakString(str,r,*dc.GetDC())) break;
    }
      
    CRect rAdjust = r;
    rAdjust.top = 0;
    int nHeight = dc.GetDC()->DrawText(str,rAdjust,DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX);
    nTop -= nHeight;

    CRect rFinal = r;
    rFinal.top = nTop;
    rFinal.bottom = nTop+nHeight;
    if (PtInRect(rFinal,ptSelect)) {
      dc.GetDC()->SetBkColor(SETUP.cForegroundColor);
      nSelectLine = i;
    } else {
      dc.GetDC()->SetBkColor(SETUP.cBackgroundColor);
    }
    dc.GetDC()->SetTextColor(line->cColor);
    dc.GetDC()->DrawText(str,rFinal,DT_WORDBREAK | DT_NOPREFIX);

    if (nTop<0) break;
  }

  dc.GetDC()->SetTextColor(SETUP.cForegroundColor);

  DeleteObject(font);
  
  // Do not call CWnd::OnPaint() for painting messages
}


void zsIRC_Wnd_Query::AddLine(TCHAR * szMessage,COLORREF cColor) {
  zsIRC_Line * line = new zsIRC_Line(szMessage,cColor);
  aLines.Add(line);
  nScrollPos = aLines.Num();

  SetScrollRange(SB_VERT,1,nScrollPos,TRUE);  
  SetScrollPos(SB_VERT,nScrollPos,TRUE);

/*
  SCROLLINFO s;
  ZeroMemory(&s,sizeof(SCROLLINFO));
  s.cbSize=sizeof(SCROLLINFO);
  s.nMin=1;
  s.nMax=nScrollPos;
  s.nPage=3;
  s.fMask=SIF_ALL;
  s.nPos=nScrollPos;
  s.nTrackPos=0;
  SetScrollInfo(SB_VERT,&s);
*/

  InvalidateRect(NULL);

  CString filename = "";
  if (nType == ZSIRC_WT_STATUS) filename += _T("_");
  filename += szCaption;
  filename += _T(".log");
#ifndef _DEBUG
  if (filename!=_T("_Debug.log"))
#endif
    line->LogToFile(filename.LockBuffer());

  if (cColor == SETUP.cForegroundColor) SetEventLevel(ZSIRC_WH_MESSAGE);
  else SetEventLevel(ZSIRC_WH_EVENT);
  
  if (GetChannelBar()) GetChannelBar()->InvalidateRect(NULL);
}

int zsIRC_Wnd_Query::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CWnd::OnCreate(lpCreateStruct) == -1)
    return -1;
  
  // TODO: Add your specialized creation code here

  evEvent = ZSIRC_WH_NOTHING;

  SCROLLINFO s;
  ZeroMemory(&s,sizeof(SCROLLINFO));
  s.cbSize=sizeof(SCROLLINFO);
  s.nMin=0;
  s.nMax=0;
  s.nPage=3;
  s.fMask=SIF_ALL;
  s.nPos=0;
  s.nTrackPos=0;
  SetScrollInfo(SB_VERT,&s);

  nScrollPos = 0;
  
  MarkLog(_T("Start"));

  return 0;
}

void zsIRC_Wnd_Query::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
  CWnd::OnActivate(nState, pWndOther, bMinimized);
  
  // TODO: Add your message handler code here
}

void zsIRC_Wnd_Query::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  // TODO: Add your message handler code here and/or call default
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

  nScrollPos = curpos;

  InvalidateRect(NULL);
}


void zsIRC_Wnd_Query::OnDestroy() 
{
	CWnd::OnDestroy();

  MarkLog(_T("Close"));
}

void zsIRC_Wnd_Query::MarkLog(TCHAR * szSession) {
  CString filename = "";
  if (nType == ZSIRC_WT_STATUS) filename += _T("_");
  filename += szCaption;
  filename += _T(".log");
  SYSTEMTIME st;
  GetLocalTime(&st);
  TCHAR ts[1000];
  _sntprintf(ts,1000,_T("%sSession %s: %04d/%02d/%02d %02d:%02d:%02d\r\n"),
    _tcscmp(szSession,_T("Start"))==0 ? _T("\r\n") : _T(""),
    szSession,st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
  
#ifndef _DEBUG
  if (filename!=_T("_Debug.log"))
#endif
  PrintToLogFile(filename.LockBuffer(),ts);
}

void zsIRC_Wnd_Query::SetCaption(TCHAR * szNewCaption) {
  MarkLog(_T("Close"));
  _tcsncpy(szCaption,szNewCaption,50);
  MarkLog(_T("Start"));
}

int zsIRC_Wnd_Query::BreakString(CString &sString, CRect r, CPaintDC& dc)
{
  CRect rAdjust = r;
  rAdjust.top = 0;

  int nHeight = dc.DrawText(sString,rAdjust,DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX);
  
  if (rAdjust.right > r.right) {
    // we need to force a linebreak somehow
    for (int i=sString.GetLength()-1; i>0; i--) {
      if (isspace(sString[i])) break; // we got to a whitespace, there isnt much more we can do from here.
      if (!isalnum(sString[i]) && !isspace(sString[i]) && !isspace(sString[i-1])) {
        rAdjust = r;
        rAdjust.top = 0;
        int nHeight2 = dc.DrawText(sString.Left(i),rAdjust,DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX);
        if (rAdjust.right <= r.right) {
          sString = sString.Left(i) + _T(" ") + sString.Mid(i);
          nHeight = nHeight2;
          return 1;
        }
      }
    }
    // we couldnt break a line at non-alpha chars, so we just mutilate the strings as is
    for (i=sString.GetLength()-1; i>=0; i--) {
      rAdjust = r;
      rAdjust.top = 0;
      int nHeight2 = dc.DrawText(sString.Left(i),rAdjust,DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX);
      if (rAdjust.right <= r.right) {
        sString = sString.Left(i) + _T(" ") + sString.Mid(i);
        nHeight = nHeight2;
        return 1;
      }
    }
  }
  return 0;
}

void zsIRC_Wnd_Query::OnLButtonDown(UINT nFlags, CPoint point) {
  ptSelect = point;
  InvalidateRect(NULL);
}

void zsIRC_Wnd_Query::OnLButtonUp(UINT nFlags, CPoint point) {
  if (nSelectLine==-1) return;
  if (!OpenClipboard()) return;

  EmptyClipboard();

  zsIRC_Line * line = (zsIRC_Line *) aLines[nSelectLine];
  
  int nLen = line->szMessage.GetLength() + 1;
  TCHAR * pGlobal = (TCHAR*)LocalAlloc(LPTR, sizeof(TCHAR) * nLen);
  if (!pGlobal) {
    CloseClipboard();
    return;
  }

  CopyMemory(pGlobal,line->szMessage.LockBuffer(),sizeof(TCHAR) * nLen);

  ::SetClipboardData(CF_TTEXT,(HANDLE)pGlobal);
  CloseClipboard();

  //////////////////////////////////////////////////////////////////////////
  
  ptSelect = CPoint(-1,-1);
  nSelectLine = -1;
  InvalidateRect(NULL);
}
