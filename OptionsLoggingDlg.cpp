// COptionsLoggingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "OptionsLoggingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsLoggingDlg dialog


COptionsLoggingDlg::COptionsLoggingDlg(CWnd* pParent /*=NULL*/)
	: CScrollableDialog(COptionsLoggingDlg::IDD)
{
	//{{AFX_DATA_INIT(COptionsLoggingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionsLoggingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsLoggingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsLoggingDlg, CScrollableDialog)
	//{{AFX_MSG_MAP(COptionsLoggingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsLoggingDlg message handlers

BOOL COptionsLoggingDlg::OnInitDialog() 
{
	CScrollableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  CheckDlgButton(IDC_LOGGING,SETUP.nDoLogging);
  SetDlgItemText(IDC_LOGDIRECTORY,SETUP.szLogDirectory);
  CheckDlgButton(IDC_DOLOGTIMESTAMP,SETUP.nDoLogTimestamp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsLoggingDlg::ApplyChanges() {
  SETUP.nDoLogging = IsDlgButtonChecked(IDC_LOGGING);
  GetDlgItemText(IDC_LOGDIRECTORY,SETUP.szLogDirectory,1024);
  SETUP.nDoLogTimestamp = IsDlgButtonChecked(IDC_DOLOGTIMESTAMP);
}