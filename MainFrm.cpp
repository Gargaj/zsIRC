// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "zsIRC.h"

#if UNDER_CE >= 420
#include <tpcshell.h>
#endif

#include "MainFrm.h"
#include "AboutDlg.h"
#include "OptionsDlg.h"
#include "zsIRC_Settings.h"
#include "zsIRC_Socket.h"
#include "zsIRC_StringTokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define PSPC_TOOLBAR_HEIGHT 24

const DWORD dwAdornmentFlags = 0; // exit button

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_MENU_CONNECT, OnMenuConnect)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_MENU_ABOUT, OnMenuAbout)
	ON_COMMAND(ID_MENU_CHECK, OnMenuCheck)
	ON_COMMAND(ID_MENU_VISIT, OnMenuVisit)
	ON_MESSAGE(WM_HOTKEY, OnHotkey)
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_OPTIONS, OnMenuOptions)
	ON_COMMAND(ID_MENU_NEXTCHAN, OnMenuNextchan)
	ON_COMMAND(ID_MENU_NEWSERVER, OnMenuNewserver)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if(!m_wndCommandBar.Create(this))
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}

#if UNDER_CE >= 420
	SHMENUBARINFO mbi;
	memset(&mbi, 0, sizeof(SHMENUBARINFO));
	mbi.cbSize     = sizeof(SHMENUBARINFO);
	mbi.hwndParent = m_wndCommandBar.GetParent()->GetSafeHwnd();
	mbi.nToolBarId = (int)IDR_SOFTKEYTEST;
	mbi.hInstRes   = AfxFindResourceHandle(IDR_SOFTKEYTEST, RT_RCDATA); 

  SHCreateMenuBar(&mbi);

  if (mbi.hwndMB) {
    // long softkey bar works - we're on a pocket pc
    m_wndCommandBar.ResetCommandBar();
    m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME);
  } else {
    // couldn't create long softkey bar - we're on a smartphone
    m_wndCommandBar.LoadSHMenuBar(IDR_MAINFRAME);
  }
#endif
  
  m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);

  m_wndCommandBar.EnableWindow();

  CRect r;
  m_wndCommandBar.GetClientRect(&r);

  if(!m_wndEdit.Create(WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL,CRect(2,2,r.right-SETUP.nNicklistWidth-15,22),this,1357)) {
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
  }
  m_wndEdit.SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
  //m_wndEdit.SetWindowText(_T("perec!"));
  m_wndEdit.EnableWindow();
  //m_wndEdit.SetFocus();
/*
  HWND hW = ;
  CMenu hMenu;
  hMenu.Attach( (HMENU)::SendMessage(hW, SHCMBM_GETMENU, 0, 0) );
  int z = hMenu.GetMenuItemCount();
  CString s;
  CMenu * hSubMenu = hMenu.GetSubMenu(0);
  z = hSubMenu->GetMenuItemCount();
  hSubMenu->GetMenuString(hSubMenu->GetMenuItemID(0),s,NULL);
  if (s.GetLength()) {
    
  } else {
    m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME);
  }
*/  
#if UNDER_CE >= 420
  ::SendMessage(SHFindMenuBar(m_hWnd),SHCMBM_OVERRIDEKEY,VK_TBACK,MAKELPARAM (SHMBOF_NODEFAULT | SHMBOF_NOTIFY, SHMBOF_NODEFAULT | SHMBOF_NOTIFY));
#endif

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.lpszClass = _T("zsirc_wnd");
  
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnExit() 
{
  TRACE0("CMainFrame::OnExit()\n");
  //m_wndView.DestroyWindow();
  m_wndView.SendMessage(WM_CLOSE);
  //m_wndCommandBar.DestroyWindow();
  //m_wndEdit.DestroyWindow();
  SendMessage(WM_CLOSE);
 // DestroyWindow();	
}

void CMainFrame::OnMenuConnect() 
{
	// TODO: Add your command handler code here

  //if (m_wndView.m_ChannelBar.scSocket.isConnected())
  if (GetActiveChannelBar()->nThreadRunning)
    GetActiveChannelBar()->Disconnect();
  else
    GetActiveChannelBar()->Connect();	
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
  if (nState==WA_ACTIVE || nState==WA_CLICKACTIVE) {
    m_wndEdit.SetFocus();
  }
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnMenuAbout() 
{
	// TODO: Add your command handler code here
  CAboutDlg a;
  a.DoModal();	
}

void CMainFrame::OnMenuCheck() 
{
  CheckForUpdates(false);
}

void CMainFrame::CheckForUpdates(bool bSilent) {
	// TODO: Add your command handler code here

  zsIRC_Socket scInternet;

  if (!scInternet.Connect("zsirc.com",80,NULL)) {
    CString msg;
    msg.Format(_T("Unable to open update URL: %d"),GetLastError() );
    if (!bSilent)
      MessageBox(msg.LockBuffer());
    return;
  }

  char t[] =
    "GET /update.php HTTP/1.1\r\n"
    "Host: zsirc.com\r\n"
    "Connection: Close\r\n\r\n";
  scInternet.Write(t,strlen(t));

  char buffer[10240];
  ZeroMemory(buffer,10240);
  scInternet.Read(buffer,10240);

  scInternet.Disconnect();

  CString answer(buffer);
  int nPos = answer.Find(_T("\r\n\r\n"));
  if (nPos==-1) return;
  answer = answer.Mid(nPos+4);

  zsIRC_StringTokenizer stLines(answer);

  int nNeedUpdate = 0;
  CString sUrl;
  CString sNewVersion;
  for (int i=0; i<=stLines.GetOccurrences(_T('|')); i++) {
    zsIRC_StringTokenizer stLine(stLines.GetSlice(_T('|'),i));
  
    CString key = stLine.GetSlice(_T('='),0);
    key.TrimLeft();
    key.TrimRight();
    CString value = stLine.GetRightSlice(_T('='));
    value.TrimLeft();
    value.TrimRight();

    if (key.CompareNoCase(_T("version"))==0) {

      if (value>GetBuildDate()) nNeedUpdate = 1;
      sNewVersion = value;
    }
    if (key.CompareNoCase(_T("url"))==0) sUrl = value;
  }

  if (nNeedUpdate) {
    CString msg;
    msg.Format(_T("New version %s is available.\nVisit the zsIRC website now?"),sNewVersion);
    if (MessageBox(msg.LockBuffer(),_T("Update found!"),MB_YESNO | MB_ICONQUESTION) == IDYES) {
      SHELLEXECUTEINFO sei;
      ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));
      sei.cbSize = sizeof(SHELLEXECUTEINFO);
      sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
      sei.lpFile = sUrl.LockBuffer();
      sei.nShow = SW_SHOW;
      ShellExecuteEx(&sei);
    }
  } else {
    if (!bSilent)
      MessageBox(_T("No new version is available"));
  }
}

void CMainFrame::OnMenuVisit() 
{
	// TODO: Add your command handler code here
  TCHAR * t = _T("http://zsirc.com");

  SHELLEXECUTEINFO sei;
  ZeroMemory(&sei,sizeof(SHELLEXECUTEINFO));
  sei.cbSize = sizeof(SHELLEXECUTEINFO);
  sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
  sei.lpFile = t;
  sei.nShow = SW_SHOW;
  ShellExecuteEx(&sei);	
}

void CMainFrame::OnHotkey(WPARAM wParam, LPARAM lParam) {
#if UNDER_CE >= 420
  if (HIWORD (lParam) == VK_TBACK)
    SHSendBackToFocusWindow(WM_HOTKEY, wParam, lParam);
#endif
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

  CRect r;
  GetClientRect(&r);
  m_wndEdit.MoveWindow(CRect(2,2,r.right-SETUP.nNicklistWidth-15,22));
	
	// TODO: Add your message handler code here
	
}

void CMainFrame::OnMenuOptions() 
{
	// TODO: Add your command handler code here
  COptionsDlg o;
  o.Setup();
  o.DoModal();
}

void CMainFrame::OnMenuNextchan() 
{
	// TODO: Add your command handler code here
  GetActiveChannelBar()->FocusWindow((GetActiveChannelBar()->nSelected + 1) % GetActiveChannelBar()->daWindows.Num());
}

CChannelBar * CMainFrame::GetActiveChannelBar() {
  return m_wndView.GetActiveChannelBar();
}

void CMainFrame::OnMenuNewserver() 
{
	// TODO: Add your command handler code here
  m_wndView.AddNewChannelbar();	
}
