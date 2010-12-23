// zsIRC_Wnd_Channel.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "zsIRC_Wnd_Channel.h"
#include "zsIRC_line.h"
#include "ChildView.h"
#include <wceres.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// zsIRC_Wnd_Channel

zsIRC_Wnd_Channel::zsIRC_Wnd_Channel(CChannelBar*c) : zsIRC_Wnd(c), m_wndQuery(c)
{
}

zsIRC_Wnd_Channel::~zsIRC_Wnd_Channel()
{
}


BEGIN_MESSAGE_MAP(zsIRC_Wnd_Channel, CWnd)
	//{{AFX_MSG_MAP(zsIRC_Wnd_Channel)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// zsIRC_Wnd_Channel message handlers

void zsIRC_Wnd_Channel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
  CRect r;
  CBrush b(0x00FFFF);
  GetClientRect(&r);
  dc.FillRect(&r,&b);	
	// Do not call CWnd::OnPaint() for painting messages
}

int zsIRC_Wnd_Channel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

  evEvent = ZSIRC_WH_NOTHING;

  _tcscpy(m_wndQuery.szCaption,szCaption);

  //PrintToLogFileF(_T("__teszt__.log"),_T("*** creating windows '%s'\n"),szCaption);

  CRect r;
  GetClientRect(&r);
  if (!m_wndQuery.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, CRect(r.left, r.top, r.right-SETUP.nNicklistWidth, r.bottom), this, NULL, NULL)) {
		TRACE0("Failed to create new query window from channel window\n");
		return -1;
	} 

  mNicklist.Create(WS_CHILD | WS_VSCROLL,CRect(r.right-SETUP.nNicklistWidth, r.top, r.right, r.bottom),this,1235);

  m_wndQuery.ShowWindow(SW_SHOW);
  mNicklist .ShowWindow(SW_SHOW);
  
  LOGFONT lf;
  ZeroMemory(&lf,sizeof(LOGFONT));
  _tcsnccpy(lf.lfFaceName,SETUP.szNicklistFont,LF_FACESIZE);
  lf.lfHeight = SETUP.nNicklistFontSize;
  lf.lfWeight = FW_NORMAL;
  lf.lfPitchAndFamily = DEFAULT_PITCH;

  fFont.CreateFontIndirect(&lf);
  mNicklist.SetFont(&fFont);

  CString talk = _T("* Now talking in ");
  talk += szCaption;
  m_wndQuery.AddLine(talk.LockBuffer(),SETUP.cMode);
	return 0;
}

void zsIRC_Wnd_Channel::AddLine(TCHAR * szMessage,COLORREF cColor) {
  m_wndQuery.AddLine(szMessage,cColor);
  if (cColor == SETUP.cForegroundColor) SetEventLevel(ZSIRC_WH_MESSAGE);
  else SetEventLevel(ZSIRC_WH_EVENT);
  if (GetParent() && ::IsWindow(GetChannelBar()->m_hWnd))
    GetChannelBar()->InvalidateRect(NULL);
}


LRESULT zsIRC_Wnd_Channel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
  TRACE2("zsIRC_Wnd_Channel %08X gets message %04X\n",this,message);
  return CWnd::WindowProc(message, wParam, lParam);
}

void zsIRC_Wnd_Channel::OnSize(UINT nType, int cx, int cy) 
{
  CRect r;
  GetClientRect(&r);

  m_wndQuery.MoveWindow(CRect(r.left, r.top, r.right-SETUP.nNicklistWidth, r.bottom));
   mNicklist.MoveWindow(CRect(r.right-SETUP.nNicklistWidth, r.top, r.right, r.bottom));
	
	// TODO: Add your message handler code here
	
}

void zsIRC_Wnd_Channel::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}

void zsIRC_Wnd_Channel::AddUser(TCHAR * szNick) {
  if (!_tcslen(szNick)) return;

  ZSIRC_USER * uUser = new ZSIRC_USER;
  ZeroMemory(uUser,sizeof(ZSIRC_USER));
    
  CChannelBar * c = GetChannelBar();
  TCHAR * s = szNick;
  while ( c->sPrefixes.Find( s[0] ) != -1) {

    int nPref = c->sPrefixes.Find(s[0]);
    uUser->nPrefixes |= (1<<nPref);

    s++;
    //uUser->cStatus = szNick[0];
//    if (szNick[0]==_T('@')) uUser->bOp    = 1;
//    if (szNick[0]==_T('+')) uUser->bVoice = 1;
  }
  _tcsncpy(uUser->sNick,s,256);

  daPeople.Add(uUser);

  mNicklist.InsertStringSorted(szNick);
}

void zsIRC_Wnd_Channel::RemoveUser(TCHAR * szNick) {
  CString userw,user;
  CChannelBar * c = GetChannelBar();
  for (int i=0; i<daPeople.Num(); i++) {
    userw = "";
    for (int j=0; j<c->sPrefixes.GetLength(); j++) 
      if (((ZSIRC_USER*)daPeople[i])->nPrefixes & (1<<j))
        userw += c->sPrefixes.GetAt(j);
//     else if (((ZSIRC_USER*)daPeople[i])->bVoice) userw += _T("+");
      
      //userw += ((ZSIRC_USER*)daPeople[i])->cStatus;
    userw += ((ZSIRC_USER*)daPeople[i])->sNick;
    user   = ((ZSIRC_USER*)daPeople[i])->sNick;

    if (user.CompareNoCase(szNick)==0) {
      daPeople.Delete(i);
      break;
    }
  }
  if (user.GetLength()) {
    int nPos = mNicklist.FindString(0,userw.LockBuffer());
    if (nPos!=-1) mNicklist.DeleteString(nPos);
  }
}

int zsIRC_Wnd_Channel::isOnChannel(TCHAR * szNick) {
  for (int i=0; i<daPeople.Num(); i++) 
    if (_tcsicmp( ((ZSIRC_USER*)daPeople[i])->sNick, szNick )==0) return 1;
  return 0;
}

void zsIRC_Wnd_Channel::RefreshUserList() {
  mNicklist.ResetContent();
  CChannelBar * c = GetChannelBar();
  for (int i=0; i<daPeople.Num(); i++) {
    CString u;
    ZSIRC_USER * user = ((ZSIRC_USER*)daPeople[i]);
//     if (user->bOp) u += "@";
//     else if (user->bVoice) u += "+";
    for (int j=0; j<c->sPrefixes.GetLength(); j++) 
      if (((ZSIRC_USER*)daPeople[i])->nPrefixes & (1<<j))
        u += c->sPrefixes.GetAt(j);
    u += user->sNick;
    mNicklist.InsertStringSorted(u.LockBuffer());
  }
}

ZSIRC_USER * zsIRC_Wnd_Channel::GetUser(TCHAR * szNick) {
  for (int j=0; j<daPeople.Num(); j++)
    if (_tcsicmp(szNick,((ZSIRC_USER*)daPeople[j])->sNick)==0)
      return (ZSIRC_USER*)daPeople[j];
  return NULL;
}

void zsIRC_Wnd_Channel::CommitUserlist() {
  daPeople.Flush();
  mNicklist.ResetContent();

  CString str = sUserlistCache;
  str += _T(" ");
  int nStart = 0;
  while (1) {
    int nPos = str.Find(_T(' '),nStart);
    if (nPos==-1) break;

    CString name = str.Mid(nStart,nPos-nStart);
    name.TrimLeft();
    name.TrimRight();
    AddUser(name.LockBuffer());

    nStart = nPos + 1;    
  }
  sUserlistCache = _T("");
}

void zsIRC_Wnd_Channel::AddUserlist(TCHAR * szNicklist) {
  sUserlistCache += szNicklist;
  sUserlistCache += _T(" ");
}

