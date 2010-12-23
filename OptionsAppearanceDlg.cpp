// OptionsAppearanceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "OptionsAppearanceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsAppearanceDlg dialog


COptionsAppearanceDlg::COptionsAppearanceDlg(CWnd* pParent /*=NULL*/)
	: CScrollableDialog(COptionsAppearanceDlg::IDD)
{
	//{{AFX_DATA_INIT(COptionsAppearanceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionsAppearanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsAppearanceDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsAppearanceDlg, CScrollableDialog)
	//{{AFX_MSG_MAP(COptionsAppearanceDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsAppearanceDlg message handlers

BOOL COptionsAppearanceDlg::OnInitDialog() 
{
	CScrollableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  CheckDlgButton(IDC_TIMESTAMPS,SETUP.nDoTimestamp);
  SetDlgItemInt(IDC_CHANNELBARHEIGHT,SETUP.nChannelBarHeight);
  SetDlgItemInt(IDC_NICKLISTWIDTH,SETUP.nNicklistWidth);
  CheckDlgButton(IDC_SORTCHANNELS,SETUP.nSortButtons);
  CheckDlgButton(IDC_OFFSCREENBUFFERING,SETUP.nOffscreenBuffering);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsAppearanceDlg::ApplyChanges() {
  SETUP.nDoTimestamp = IsDlgButtonChecked(IDC_TIMESTAMPS);
  SETUP.nChannelBarHeight = GetDlgItemInt(IDC_CHANNELBARHEIGHT);
  SETUP.nNicklistWidth = GetDlgItemInt(IDC_NICKLISTWIDTH);
  SETUP.nSortButtons = IsDlgButtonChecked(IDC_SORTCHANNELS);
  SETUP.nOffscreenBuffering = IsDlgButtonChecked(IDC_OFFSCREENBUFFERING);
}