// OptionsConnectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "OptionsDlg.h"
#include "OptionsConnectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsConnectionDlg dialog


COptionsConnectionDlg::COptionsConnectionDlg(CWnd* pParent /*=NULL*/)
	: CScrollableDialog(COptionsConnectionDlg::IDD)
{
	//{{AFX_DATA_INIT(COptionsConnectionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionsConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsConnectionDlg)
	DDX_Control(pDX, IDC_SSL_INVALID, m_SSLInvalidBehaviour);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionsConnectionDlg, CScrollableDialog)
	//{{AFX_MSG_MAP(COptionsConnectionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL COptionsConnectionDlg::OnInitDialog() 
{
	CScrollableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  SetDlgItemText(IDC_SERVER_NAME,SETUP.szServerName);
  SetDlgItemInt(IDC_SERVER_PORT,SETUP.nServerPort);
  CheckDlgButton(IDC_SERVER_SECURE,SETUP.nServerSecure);  
  SetDlgItemText(IDC_SERVER_PASS,SETUP.szServerPass);
  SetDlgItemText(IDC_IDENT,SETUP.szIdent);
  SetDlgItemText(IDC_REALNAME,SETUP.szRealname);

  m_SSLInvalidBehaviour.ResetContent();
  m_SSLInvalidBehaviour.AddString(_T("Prompt user for action"));
  m_SSLInvalidBehaviour.AddString(_T("Always accept"));
  m_SSLInvalidBehaviour.AddString(_T("Never accept"));
  m_SSLInvalidBehaviour.SetCurSel(SETUP.nServerSSLBehaviour);

  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsConnectionDlg::ApplyChanges() {
  GetDlgItemText(IDC_SERVER_NAME,SETUP.szServerName,1024);
  SETUP.nServerPort = GetDlgItemInt(IDC_SERVER_PORT);
  SETUP.nServerSecure = IsDlgButtonChecked(IDC_SERVER_SECURE);
  GetDlgItemText(IDC_SERVER_PASS,SETUP.szServerPass,1024);
  GetDlgItemText(IDC_IDENT,SETUP.szIdent,1024);
  GetDlgItemText(IDC_REALNAME,SETUP.szRealname,1024);
  
  SETUP.nServerSSLBehaviour = (ZSIRC_SSLCERTIFICATE_BEHAVIOUR)m_SSLInvalidBehaviour.GetCurSel();
}