// OptionsFontsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "OptionsFontsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsFontsDlg dialog


COptionsFontsDlg::COptionsFontsDlg(CWnd* pParent /*=NULL*/)
	: CScrollableDialog(COptionsFontsDlg::IDD)
{
	//{{AFX_DATA_INIT(COptionsFontsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionsFontsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsFontsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsFontsDlg, CScrollableDialog)
	//{{AFX_MSG_MAP(COptionsFontsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsFontsDlg message handlers

BOOL COptionsFontsDlg::OnInitDialog() 
{
	CScrollableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  SetDlgItemText(IDC_CONVERSATIONFONT,SETUP.szConversationFont);
  SetDlgItemInt(IDC_CONVERSATIONFONTSIZE,SETUP.nConversationFontSize);
  SetDlgItemText(IDC_BUTTONFONT,SETUP.szButtonFont);
  SetDlgItemInt(IDC_BUTTONFONTSIZE,SETUP.nButtonFontSize);
  SetDlgItemText(IDC_NICKLISTFONT,SETUP.szNicklistFont);
  SetDlgItemInt(IDC_NICKLISTFONTSIZE,SETUP.nNicklistFontSize);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsFontsDlg::ApplyChanges() {
  GetDlgItemText(IDC_CONVERSATIONFONT,SETUP.szConversationFont,1024);
  SETUP.nConversationFontSize = GetDlgItemInt(IDC_CONVERSATIONFONTSIZE);
  GetDlgItemText(IDC_BUTTONFONT,SETUP.szButtonFont,1024);
  SETUP.nButtonFontSize = GetDlgItemInt(IDC_BUTTONFONTSIZE);
  GetDlgItemText(IDC_NICKLISTFONT,SETUP.szNicklistFont,1024);
  SETUP.nNicklistFontSize = GetDlgItemInt(IDC_NICKLISTFONTSIZE);
}