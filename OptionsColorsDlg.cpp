// OptionsColorsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "OptionsColorsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsColorsDlg dialog


COptionsColorsDlg::COptionsColorsDlg(CWnd* pParent /*=NULL*/)
	: CScrollableDialog(COptionsColorsDlg::IDD)
{
	//{{AFX_DATA_INIT(COptionsColorsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionsColorsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsColorsDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsColorsDlg, CScrollableDialog)
	//{{AFX_MSG_MAP(COptionsColorsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsColorsDlg message handlers

BOOL COptionsColorsDlg::OnInitDialog() 
{
//	CScrollableDialog::OnInitDialog();
  CScrollableDialog::OnInitDialog();
	
  UINT nIDS[] = {
    IDC_BACKGROUNDCOLOR,
    IDC_FOREGROUNDCOLOR,
    IDC_ACTIONCOLOR,
    IDC_JOINCOLOR,
    IDC_MODECOLOR,
    IDC_NOTICECOLOR,
    IDC_PARTCOLOR,
  };
  CColorButton * pElements[] = {
    &m_BackgroundColor,
    &m_ForegroundColor,
    &m_ActionColor,
    &m_JoinColor,
    &m_ModeColor,
    &m_NoticeColor,
    &m_PartColor,
  };
  for (int i=0; i<7; i++) {
    CWnd * w = GetDlgItem(nIDS[i]);
    pElements[i]->Detach();
    CRect r;
    w->GetWindowRect(&r);
    ScreenToClient(&r);
    w->ShowWindow(SW_HIDE);
    pElements[i]->Create(_T(""),WS_CHILD | WS_VISIBLE,r,this,12345+i);
  }
  
  m_BackgroundColor.SetColor(SETUP.cBackgroundColor);
  m_ForegroundColor.SetColor(SETUP.cForegroundColor);
  
  m_ActionColor.SetColor(SETUP.cAction);
  m_JoinColor.SetColor(SETUP.cJoin);
  m_ModeColor.SetColor(SETUP.cMode);
  m_PartColor.SetColor(SETUP.cPart);
  m_NoticeColor.SetColor(SETUP.cNotice);
  
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsColorsDlg::ApplyChanges() {
  SETUP.cBackgroundColor = m_BackgroundColor.GetColor();
  SETUP.cForegroundColor = m_ForegroundColor.GetColor();
  
  SETUP.cAction = m_ActionColor.GetColor();
  SETUP.cJoin = m_JoinColor.GetColor();
  SETUP.cMode = m_ModeColor.GetColor();
  SETUP.cPart = m_PartColor.GetColor();
  SETUP.cNotice = m_NoticeColor.GetColor();
}