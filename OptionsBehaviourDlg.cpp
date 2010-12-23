// OptionsBehaviourDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "OptionsBehaviourDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsBehaviourDlg dialog


COptionsBehaviourDlg::COptionsBehaviourDlg(CWnd* pParent /*=NULL*/)
	: CScrollableDialog(COptionsBehaviourDlg::IDD)
{
	//{{AFX_DATA_INIT(COptionsBehaviourDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionsBehaviourDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsBehaviourDlg)
	DDX_Control(pDX, IDC_CODEPAGE, m_Codepage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsBehaviourDlg, CScrollableDialog)
	//{{AFX_MSG_MAP(COptionsBehaviourDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsBehaviourDlg message handlers

BOOL COptionsBehaviourDlg::OnInitDialog() 
{
	CScrollableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  SetDlgItemText(IDC_QUITMESSAGE,SETUP.szQuitMessage);
  CheckDlgButton(IDC_AUTOCONNECT,SETUP.nAutoConnect);
  CheckDlgButton(IDC_REJOINONCONNECT,SETUP.nRejoinOnConnect);
  CheckDlgButton(IDC_PASSCOMMANDSRAW,SETUP.nPassCommandsRaw);
  CheckDlgButton(IDC_STRIPCOLORCODES,SETUP.nStripColorCodes);
  m_Codepage.ResetContent();
  int nSelect = 0;
  for (ZSIRC_SETTING_CODEPAGE * c = g_pZsircSettingsCodepages; c->szName; c++) {
    m_Codepage.AddString(c->szName);
    if (SETUP.nCodepage == c->nCodepage)
      nSelect = m_Codepage.GetCount()-1;
  }
  m_Codepage.SetCurSel(nSelect);

  CheckDlgButton(IDC_CHECKFORUPDATES,SETUP.nCheckForUpdates);
  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsBehaviourDlg::ApplyChanges() {
  GetDlgItemText(IDC_QUITMESSAGE,SETUP.szQuitMessage,1024);
  SETUP.nAutoConnect = IsDlgButtonChecked(IDC_AUTOCONNECT);
  SETUP.nRejoinOnConnect = IsDlgButtonChecked(IDC_REJOINONCONNECT);
  SETUP.nPassCommandsRaw = IsDlgButtonChecked(IDC_PASSCOMMANDSRAW);
  SETUP.nStripColorCodes = IsDlgButtonChecked(IDC_STRIPCOLORCODES);
  SETUP.nCodepage = g_pZsircSettingsCodepages[m_Codepage.GetCurSel()].nCodepage;
  SETUP.nCheckForUpdates = IsDlgButtonChecked(IDC_CHECKFORUPDATES);
}