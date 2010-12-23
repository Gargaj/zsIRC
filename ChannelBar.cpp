// ChannelBar.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "ChannelBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "zsIRC_Line.h"
#include "zsIRC_Settings.h"
#include "zsIRC_EventHandler.h"
#include "zsIRC_Wnd_Query.h"
#include "zsIRC_Wnd_Channel.h"
#include "zsIRC_Hostmask.h"
#include "zsIRC_StringTokenizer.h"
#include "OffscreenDC.h"
#include "ChildView.h"
#include "MainFrm.h"

#define IDR_CUSTOM_CHANNELBARS_MENU 13579

/////////////////////////////////////////////////////////////////////////////
// CChannelBar

CChannelBar::CChannelBar() : EVENTS(this)
{
  sNetwork = _T("[not connected]");
}

CChannelBar::~CChannelBar()
{
}

void CChannelBar::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CChannelBar, CWnd)
	//{{AFX_MSG_MAP(CChannelBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
  ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
  ON_WM_DESTROY()
  ON_MESSAGE(WM_CBAR_CHECKQUEUE,OnCheckQueue)
  ON_MESSAGE(WM_CBAR_DISCONNECT,OnDisconnect)
	ON_COMMAND(IDR_CBAR_CLOSE, OnCbarClose)
	ON_COMMAND(12345, OnWTF)
  ON_COMMAND_EX_RANGE(IDR_CUSTOM_CHANNELBARS_MENU, IDR_CUSTOM_CHANNELBARS_MENU + 32, OnChannelBarSwitch)
  ON_MESSAGE(WM_NOTIFY, OnNotification)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelBar message handlers

int CChannelBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;

  SETUP.Load();
  EVENTS.Load();

  wndDebug  = (zsIRC_Wnd_Query*)AddWindow(ZSIRC_WT_STATUS,_T("Debug"));
  wndStatus = (zsIRC_Wnd_Query*)AddWindow(ZSIRC_WT_STATUS,_T("Status"));
  sNick = SETUP.szDefaultNick[0];
  sPrefixReadables = _T("ov");
  sPrefixes        = _T("@+");
  sChanTypes = _T("#");


  wndDebug ->AddLine(_T("Debug console open for business"));
  FocusWindow(1);  

//  zsIRC_Wnd_Channel * c = (zsIRC_Wnd_Channel*)AddWindow(ZSIRC_WT_CHANNEL,_T("#qqqq"));
//  c->AddUserlist(_T("aaaa bbbbb cccccc ddddd eeeee fffff"));

  CString s = _T("*** Welcome to zsIRC, version ");
  s += GetBuildDate();
  s += _T(" ***");
  wndStatus->AddLine(s.LockBuffer(),SETUP.cMode);
  wndStatus->AddLine(_T("*** Feel free to report bugs!"),SETUP.cMode);

    zsIRC_StringTokenizer stNotify(SETUP.szNotifyList);
  for (int i=0; i<=stNotify.GetOccurrences(_T(' ')); i++) {
    CString s = stNotify.GetSlice(_T(' '),i);
    s.TrimLeft();
    s.TrimRight();
    if (s.GetLength()) {
      ZSIRC_USER * u = new ZSIRC_USER;
      _tcsncpy(u->sNick,s.LockBuffer(),256);
      u->bOnline = 0;
      daNotify.Add(u);
    } 
  } 

  nThreadRunning = 0;

  if (SETUP.nAutoConnect)
    Connect();

	return 0;
}

void CChannelBar::OnPaint() 
{
	//CPaintDC dc(this); // device context for painting
	COffscreenDC dc(this,CRect(0,0,0,0));

  CBrush b;
  b.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

  CRect r;
  GetClientRect(r);

  dc.GetDC()->FillRect(r,&b);

  int nWindows = daWindows.Num();
  int nButtons = nWindows;
  if (!nButtons) return;
  if (!SETUP.nShowDebugWindow) nButtons--;

  int nButtonWidth = r.Width() / nButtons;

  LOGFONT lf;
  ZeroMemory(&lf,sizeof(LOGFONT));
  _tcsnccpy(lf.lfFaceName,SETUP.szButtonFont,LF_FACESIZE);
  lf.lfHeight = SETUP.nButtonFontSize;
  lf.lfWeight = FW_NORMAL;
  lf.lfPitchAndFamily = DEFAULT_PITCH;
  HFONT font = CreateFontIndirect(&lf);

  HFONT fontold = (HFONT)dc.GetDC()->SelectObject(font);

  int nChan=0,nQuery=0;

  for (int i=0; i<nButtons; i++) {  
    int p = i + (SETUP.nShowDebugWindow ? 0 : 1);
    zsIRC_Wnd * wnd = (zsIRC_Wnd *)daWindows[p];

    int n_1 = ((i+0) * nButtonWidth);
    int n_2 = ((i+1) * nButtonWidth);

    CString str;

    str = wnd->szCaption;

    if (p==nSelected)
      dc.GetDC()->Draw3dRect(n_1,r.top,nButtonWidth,r.Height(),GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DHIGHLIGHT));
    else
      dc.GetDC()->Draw3dRect(n_1,r.top,nButtonWidth,r.Height(),GetSysColor(COLOR_3DHIGHLIGHT),GetSysColor(COLOR_3DSHADOW));
    
    CSize s = dc.GetDC()->GetTextExtent(str);
    CRect tr(n_1+2,1,n_2-2,SETUP.nChannelBarHeight-1);
    dc.GetDC()->SetBkMode(TRANSPARENT);

    if (p==nSelected || wnd==wndDebug) wnd->evEvent = ZSIRC_WH_NOTHING;
    switch(wnd->evEvent) {
      case ZSIRC_WH_NOTHING:   dc.GetDC()->SetTextColor(RGB(0x00,0x00,0x00)); break;
      case ZSIRC_WH_EVENT:     dc.GetDC()->SetTextColor(RGB(0x00,0x00,0xFF)); break;
      case ZSIRC_WH_MESSAGE:   dc.GetDC()->SetTextColor(RGB(0xFF,0x00,0x00)); break;
      case ZSIRC_WH_HIGHLIGHT: dc.GetDC()->SetTextColor(RGB(0x00,0xFF,0xFF)); break;
    }
    dc.GetDC()->DrawText(str,tr,(s.cx < tr.Width() ? DT_CENTER : DT_LEFT) | DT_VCENTER);
  }
  DeleteObject(font);
}

void CChannelBar::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}

void CChannelBar::OnLButtonDown(UINT nFlags, CPoint point)
{
  int nWindows = daWindows.Num();
  int nButtons = nWindows;
  if (!nButtons) return;
  if (!SETUP.nShowDebugWindow) nButtons--;

  CRect r;
  GetClientRect(r);

  int nButtonWidth = r.Width() / nButtons;

  int s = point.x / nButtonWidth;
  if (!SETUP.nShowDebugWindow) s++;
  
  FocusWindow(s);

#if UNDER_CE >= 420
  SHRGINFO shrgi = {0};
  shrgi.cbSize        = sizeof(SHRGINFO);
  shrgi.hwndClient    = m_hWnd;
  shrgi.ptDown.x      = point.x;
  shrgi.ptDown.y      = point.y;
  shrgi.dwFlags       = SHRG_RETURNCMD;

  if(GN_CONTEXTMENU == ::SHRecognizeGesture(&shrgi)) {
    ClientToScreen(&point);
    if (nSelected > 1) { 
	    CMenu mPopup;
	    mPopup.LoadMenu(IDR_CBAR);
	    CMenu * mSubmenu = mPopup.GetSubMenu(0);

      mSubmenu->EnableMenuItem(IDR_CBAR_CLOSE,nSelected>1 ? MF_ENABLED : MF_GRAYED);

	    mSubmenu->TrackPopupMenu(NULL, point.x, point.y, this);
    } else if (nSelected == 1) {
      CChildView * wndView = &((CMainFrame*)theApp.m_pMainWnd)->m_wndView;
      if (wndView->daChannelBars.Num() > 1) {
        CMenu mPopup; 
        mPopup.CreatePopupMenu();

        for (int i=0; i<wndView->daChannelBars.Num(); i++) {
          CChannelBar * w = (CChannelBar *)wndView->daChannelBars[i];
          UINT state = MF_STRING;
          if (w == this) state |= MF_CHECKED;
          mPopup.AppendMenu(state, IDR_CUSTOM_CHANNELBARS_MENU + i, w->sNetwork.LockBuffer());
        }
        mPopup.AppendMenu(MF_SEPARATOR);
        mPopup.AppendMenu(MF_STRING, IDR_CBAR_CLOSE, _T("&Close"));
        mPopup.TrackPopupMenu(NULL, point.x, point.y, this);
      }
    }
  } 
#endif
}

void CChannelBar::OnChannelBarSwitch( UINT nID ) {
  int n = nID - IDR_CUSTOM_CHANNELBARS_MENU;

  CChildView * wndView = &((CMainFrame*)theApp.m_pMainWnd)->m_wndView;
  wndView->SetActiveChannelBar( n );
}

int __cdecl ButtonSortCallback(const void * a, const void * b) {
  zsIRC_Wnd * wndA = *(zsIRC_Wnd **)a;
  zsIRC_Wnd * wndB = *(zsIRC_Wnd **)b;
  if (wndA->nType < wndB->nType) return -1;
  if (wndA->nType > wndB->nType) return 1;
  return _tcsicmp(wndA->szCaption,wndB->szCaption);
}

zsIRC_Wnd * CChannelBar::AddWindow(ZSIRC_WND_TYPE nType,TCHAR * szCaption)
{
  if (FindWindow(szCaption)) return NULL;

  zsIRC_Wnd * wnd = NULL;
  switch (nType) {
    case ZSIRC_WT_QUERY: 
    case ZSIRC_WT_STATUS: {
      wnd = new zsIRC_Wnd_Query(this);
    } break;
    case ZSIRC_WT_CHANNEL: {
      wnd = new zsIRC_Wnd_Channel(this);
    } break;
  }

  _tcsnccpy(wnd->szCaption,szCaption,50);
  daWindows.Add(wnd);

  wnd->nType = nType;

  if (SETUP.nSortButtons)
    daWindows.Sort(ButtonSortCallback);

  CRect r;
//  GetClientRect(r);
  GetParent()->GetParent()->GetClientRect(r); 
  
  CWnd * wndParent = GetParent();
  if (!wnd->Create(NULL, szCaption, WS_CHILD | WS_VISIBLE | (nType==ZSIRC_WT_CHANNEL ? 0 : WS_VSCROLL),
    CRect(r.left, r.top, r.right, r.bottom-SETUP.nChannelBarHeight), wndParent, NULL)) {
		TRACE0("Failed to create new window from channel bar\n");
		return NULL;
	}

  FocusWindow(daWindows.Find(wnd));

  return wnd;
}

void CChannelBar::CloseWindow(TCHAR * szCaption) {
  for (int i=0; i<daWindows.Num(); i++) {  
    zsIRC_Wnd * wnd = (zsIRC_Wnd *)daWindows[i];
    if (_tcsicmp(wnd->szCaption,szCaption)==0) {
      wnd->SendMessage(WM_CLOSE);
      daWindows.Delete(i);
      FocusWindow(i-1);
      return;
    }
  }
}

void CChannelBar::OnSize(UINT nType, int cx, int cy)
{
  CRect r;
  GetParent()->GetParent()->GetClientRect(r);

  for (int i=0; i<daWindows.Num(); i++)
    ((zsIRC_Wnd*)daWindows[i])->MoveWindow(CRect(r.left, 0, r.right, r.bottom-SETUP.nChannelBarHeight));
}

void CChannelBar::OnDestroy() {
  TRACE0("CChannelBar::OnDestroy()\n");

  CString s = _T("QUIT :");
  s += SETUP.szQuitMessage;
  SendIRCMessage(s.LockBuffer());

  Disconnect();

  while(daWindows.Num()) {  
    zsIRC_Wnd * wnd = (zsIRC_Wnd *)daWindows[0];
    wnd->SendMessage(WM_CLOSE);
    daWindows.Delete(0);
  }
}

zsIRC_Wnd * CChannelBar::FindWindow(TCHAR * szName) {
  for (int i=0; i<daWindows.Num(); i++) {  
    zsIRC_Wnd * wnd = (zsIRC_Wnd *)daWindows[i];
    if (_tcsicmp(wnd->szCaption,szName)==0) 
      return wnd;
  }
  return NULL;
}

//DEL void CChannelBar::OnContextMenu(CWnd* pWnd, CPoint point) 
//DEL {
//DEL   int nWindows = daWindows.Num();
//DEL   int nButtons = nWindows;
//DEL   if (!nButtons) return;
//DEL   if (!SETUP.nShowDebugWindow) nButtons--;
//DEL 
//DEL   CRect r;
//DEL   GetClientRect(r);
//DEL 
//DEL   int nButtonWidth = r.Width() / nButtons;
//DEL 
//DEL   nSelected = point.x / nButtonWidth;
//DEL   if (!SETUP.nShowDebugWindow) nSelected++;
//DEL 
//DEL 	CMenu mPopup;
//DEL 	mPopup.LoadMenu(IDR_CBAR);
//DEL 
//DEL 	CMenu * mSubmenu = mPopup.GetSubMenu(0);
//DEL   mSubmenu->EnableMenuItem(IDR_CBAR_CLOSE,nSelected>1 ? MF_ENABLED : MF_GRAYED);
//DEL   
//DEL 	mSubmenu->TrackPopupMenu(NULL, point.x, point.y, this);
//DEL }

void CChannelBar::OnCbarClose() 
{
	// TODO: Add your command handler code here
  if (nSelected>1) {
    zsIRC_Wnd * w = (zsIRC_Wnd*)daWindows[nSelected];
    if (w->nType == ZSIRC_WT_CHANNEL) {
      CString s;
      s += _T("PART ");
      s += w->szCaption;
      SendIRCMessage(s.LockBuffer());
    } else
    if (w->nType == ZSIRC_WT_QUERY) {
      CloseWindow(w->szCaption);
    }
  } else if (nSelected == 1) {
    if (MessageBox(_T("Are you sure you want to close and disconnect this server window?"),_T("zsIRC"),MB_YESNO | MB_ICONQUESTION) == IDYES) {
      CChildView * wndView = &((CMainFrame*)theApp.m_pMainWnd)->m_wndView;
      wndView->RemoveActiveChannelBar();
    }
  }
}

void CChannelBar::FocusWindow(int n)
{
  nSelected = n;
  if (nSelected < 0) nSelected = 0;

  for (int i=0; i<daWindows.Num(); i++) {
    ((zsIRC_Wnd*)daWindows[i])->ShowWindow(i == nSelected ? SW_SHOW : SW_HIDE);
  }

  ((zsIRC_Wnd*)daWindows[nSelected])->evEvent = ZSIRC_WH_NOTHING;

  InvalidateRect(NULL);
}

void CChannelBar::HideAllWindows() {
  for (int i=0; i<daWindows.Num(); i++) {
    ((zsIRC_Wnd*)daWindows[i])->ShowWindow(SW_HIDE);
  }
}

LRESULT CChannelBar::OnNotification(WPARAM wParam, LPARAM lParam) {
  GetParent()->ShowWindow(SW_SHOW);
  return 1;
}

void CChannelBar::OnWTF() {
  GetParent()->ShowWindow(SW_SHOW);
}

void CChannelBar::CheckNotify()
{
  if (daNotify.Num()) {
    CString user = _T("ISON ");
    for (int j=0; j<daNotify.Num(); j++) {
      ZSIRC_USER * u = (ZSIRC_USER*)daNotify[j];
      user += u->sNick;
      user += _T(" ");
    }
    SendIRCMessage(user.LockBuffer());
  } else {
    CString cmd = _T("PING :");
    cmd += SETUP.szServerName;
    SendIRCMessage(cmd.LockBuffer());
  }
}
