// OptionsNicksDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "OptionsNicksDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsNicksDlg dialog


COptionsNicksDlg::COptionsNicksDlg(CWnd* pParent /*=NULL*/)
	: CScrollableDialog(COptionsNicksDlg::IDD)
{
	//{{AFX_DATA_INIT(COptionsNicksDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionsNicksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsNicksDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsNicksDlg, CScrollableDialog)
	//{{AFX_MSG_MAP(COptionsNicksDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsNicksDlg message handlers

BOOL COptionsNicksDlg::OnInitDialog() 
{
	CScrollableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  for (int i=0; i<9; i++)
    SetDlgItemText(IDC_NICK0+i,SETUP.szDefaultNick[i]);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsNicksDlg::ApplyChanges() {
  for (int i=0; i<9; i++)
    GetDlgItemText(IDC_NICK0+i,SETUP.szDefaultNick[i],1024);
}