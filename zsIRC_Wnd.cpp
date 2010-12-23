// zsIRC_Wnd.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "zsIRC_Wnd.h"
#include "zsIRC_Settings.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// zsIRC_Wnd

zsIRC_Wnd::zsIRC_Wnd(CChannelBar*c)
{
  m_ChannelBar = c;
}

zsIRC_Wnd::~zsIRC_Wnd()
{
}


BEGIN_MESSAGE_MAP(zsIRC_Wnd, CWnd)
	//{{AFX_MSG_MAP(zsIRC_Wnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// zsIRC_Wnd message handlers

void zsIRC_Wnd::SetEventLevel(ZSIRC_WND_HIGHLIGHT h) {
  if (h > evEvent) evEvent = h;

/*
  if (evEvent >= ZSIRC_WH_MESSAGE && SETUP.nPopupNotify && g_ChannelBar) {
    //if (!g_ChannelBar->GetParent()->IsWindowVisible())
    {
      SHNOTIFICATIONDATA shnd;
      ZeroMemory(&shnd,sizeof(SHNOTIFICATIONDATA));
      shnd.cbStruct = sizeof(SHNOTIFICATIONDATA);
      shnd.dwID = 13371;
      shnd.csDuration = 5;
      shnd.npPriority = SHNP_INFORM;
      shnd.hicon = theApp.LoadIcon(IDI_NOTIFYICON); // TODO
      shnd.grfFlags = SHNF_STRAIGHTTOTRAY;
      shnd.hwndSink = g_ChannelBar->m_hWnd;
      shnd.pszHTML = _T("töf <A HREF='cmd:3'>WWWWWWWW</A>");
      shnd.pszTitle = _T("zsIRC");
      LRESULT l = SHNotificationAdd(&shnd);

      TRACE1("Added notification to %08X...\n",g_ChannelBar->m_hWnd);
    }
    //Sleep(0);
  }
*/
}

//void zsIRC_Wnd::AddLine(TCHAR * szSender,TCHAR * szMessage,COLORREF cColor) {}
void zsIRC_Wnd::AddLine(TCHAR * szMessage,COLORREF cColor) {}

void zsIRC_Wnd::SetCaption(TCHAR * szNewCaption) {
  _tcsncpy(szCaption,szNewCaption,50);
}

CChannelBar * zsIRC_Wnd::GetChannelBar() {
  return m_ChannelBar;
}