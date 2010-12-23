// zsIRC.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "zsIRC.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZsIRCApp

BEGIN_MESSAGE_MAP(CZsIRCApp, CWinApp)
	//{{AFX_MSG_MAP(CZsIRCApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	// Standard file based document commands
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZsIRCApp construction

CZsIRCApp::CZsIRCApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CZsIRCApp object

CZsIRCApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CZsIRCApp initialization

BOOL CZsIRCApp::InitInstance()
{
/*
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
*/

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("zsIRC"));

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

  pFrame->SetWindowText(_T("zsIRC"));

	// The one and only window has been initialized, so show and update it.
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CZsIRCApp message handlers





// App command to run the dialog
void CZsIRCApp::OnAppAbout()
{ 
//	CAboutDlg aboutDlg;
//	aboutDlg.DoModal();
}

CString GetBuildDate() {
  CString sDate = _T( __DATE__ );

  int m = 0;
  if (sDate.Left(3)==_T("Jan")) m = 1;
  if (sDate.Left(3)==_T("Feb")) m = 2;
  if (sDate.Left(3)==_T("Mar")) m = 3;
  if (sDate.Left(3)==_T("Apr")) m = 4;
  if (sDate.Left(3)==_T("May")) m = 5;
  if (sDate.Left(3)==_T("Jun")) m = 6;
  if (sDate.Left(3)==_T("Jul")) m = 7;
  if (sDate.Left(3)==_T("Aug")) m = 8;
  if (sDate.Left(3)==_T("Sep")) m = 9;
  if (sDate.Left(3)==_T("Oct")) m = 10;
  if (sDate.Left(3)==_T("Nov")) m = 11;
  if (sDate.Left(3)==_T("Dec")) m = 12;

  int d = 0;
  d += sDate[5] - _T('0');
  d += (sDate[4]==_T(' ') ? 0 : sDate[4] - _T('0'))*10;

  int y = 0;
  y += (sDate[ 7] - _T('0'))*1000;
  y += (sDate[ 8] - _T('0'))*100;
  y += (sDate[ 9] - _T('0'))*10;
  y += (sDate[10] - _T('0'))*1;

  CString s;
  s.Format(_T("%04d.%02d.%02d"),y,m,d);
  return s;
}