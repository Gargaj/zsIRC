// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "OptionsDlg.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CPropertySheet(_T("nyeeee"), pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertySheet::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
//	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CPropertySheet)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_WM_PAINT()
	ON_COMMAND(ID_OPTIONS_OK, OnOptionsOk)
	ON_COMMAND(ID_OPTIONS_CANCEL, OnOptionsCancel)
	ON_COMMAND(ID_OPTIONS_APPLY, OnOptionsApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

void COptionsDlg::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::CalcWindowRect(lpClientRect, nAdjustType);
}

void COptionsDlg::Setup() 
{

//  InsertMenu(IDR_OPTIONS, , MF_STRING, pDoc->m_aColorDef[i].m_nID, strColor);
/*
   CMenu m;
  VERIFY(m.LoadMenu(IDR_OPTIONS));
  SetMenu(NULL);
  SetMenu(&m);
  m.Detach();
*/
  
//  ::InsertMenu(
	// TODO: Add extra initialization here
//   m_TabCtrl.AddPage(0,_T("Connection"));
//   m_TabCtrl.AddPage(1,_T("Nicks"));
//   m_TabCtrl.AddPage(2,_T("Appearance"));
//   m_TabCtrl.AddPage(3,_T("Behaviour"));
//   m_TabCtrl.AddPage(4,_T("Logging"));
//   m_TabCtrl.AddPage(5,_T("Fonts"));
//   m_TabCtrl.AddPage(6,_T("Colors"));
// 
// //  m_TabCtrl.GetItemRect(0,&r);

/*
   mOptionsConnectionDlg.Create(IDD_OPTIONS_CONNECTION,&m_TabCtrl);
   mOptionsNicksDlg     .Create(IDD_OPTIONS_NICKS,     &m_TabCtrl);
   mOptionsAppearanceDlg.Create(IDD_OPTIONS_APPEARANCE,&m_TabCtrl);
   mOptionsBehaviourDlg .Create(IDD_OPTIONS_BEHAVIOUR, &m_TabCtrl);
   mOptionsLoggingDlg   .Create(IDD_OPTIONS_LOGGING,   &m_TabCtrl);
   mOptionsFontsDlg     .Create(IDD_OPTIONS_FONTS,     &m_TabCtrl);
   mOptionsColorsDlg    .Create(IDD_OPTIONS_COLORS,    &m_TabCtrl);
  */
  
  mSubdialogs[0].dialog = &mOptionsConnectionDlg;
  mSubdialogs[1].dialog = &mOptionsNicksDlg;
  mSubdialogs[2].dialog = &mOptionsAppearanceDlg;
  mSubdialogs[3].dialog = &mOptionsBehaviourDlg;
  mSubdialogs[4].dialog = &mOptionsLoggingDlg;
  mSubdialogs[5].dialog = &mOptionsFontsDlg;
  mSubdialogs[6].dialog = &mOptionsColorsDlg;

  for (int i=0; i<SUBDIALOGS; i++) {
    AddPage(mSubdialogs[i].dialog);

/*
    RECT rectMax;
	  HWND hWndChild = ::GetWindow(mSubdialogs[i].dialog->m_hWnd, GW_CHILD);
	  if (hWndChild != NULL)
	  {
			::GetWindowRect(hWndChild, &rectMax);
		  for (; hWndChild != NULL; hWndChild = ::WCE_FCTN(GetNextWindow)(hWndChild, GW_HWNDNEXT))
		  {
			  CRect rect;
			  ::GetWindowRect(hWndChild, &rect);
        rectMax.top    = min(rect.top   ,rectMax.top);
        rectMax.left   = min(rect.left  ,rectMax.left);
        rectMax.right  = max(rect.right ,rectMax.right);
        rectMax.bottom = max(rect.bottom,rectMax.bottom);
		  }
	  }
    mSubdialogs[i].size.x = rectMax.right  - rectMax.left;
    mSubdialogs[i].size.y = rectMax.bottom - rectMax.top + 10;
    mSubdialogs[i].dialog->ShowWindow(i == 0 ? SW_SHOW : SW_HIDE);
*/
  }

  
#if UNDER_CE >= 420
  SipShowIM(SIPF_OFF); 
#endif

// 	return TRUE;  // return TRUE unless you set the focus to a control
// 	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COptionsDlg::OnInitDialog()
{
  BOOL res = CPropertySheet::OnInitDialog();
  BOOL bSuccess = m_wndCommandBar.Create(this);

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
    m_wndCommandBar.InsertMenuBar(IDR_OPTIONS);
  } else {
    // couldn't create long softkey bar - we're on a smartphone
    m_wndCommandBar.LoadSHMenuBar(IDR_OPTIONS);
  }
#endif

    return res;
}

/*
void COptionsDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
  int n = m_TabCtrl.GetCurSel();
  
  for (int i=0; i<SUBDIALOGS; i++) {
    mSubdialogs[i].dialog->ShowWindow(n == i ? SW_SHOW : SW_HIDE);
    if (n==i) {
      mSubdialogs[i].dialog->SetFocus();
    }
  }

	*pResult = 0;
}
*/

void COptionsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CPropertySheet::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
  RECT r;
  GetClientRect(&r);
/*
  int nBorder = 2;
  r.bottom -= nBorder;
  r.top += nBorder+25;
  r.right -= nBorder;
  r.left += nBorder;
  m_TabCtrl.SetWindowPos(this,r.left,r.top,r.right-r.left,r.bottom-r.top,NULL);
  r.top -= 25;
  m_TabCtrl.AdjustRect(FALSE,&r);
  r.bottom -= nBorder+25;
  r.top += nBorder;
  r.right -= nBorder;
  r.left += nBorder;
*/


}

void COptionsDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
  for (int i=0; i<SUBDIALOGS; i++) 
    if (::IsWindow(mSubdialogs[i].dialog->m_hWnd))
      mSubdialogs[i].dialog->ApplyChanges();

  SETUP.Save();

}

void COptionsDlg::OnOK() 
{
	// TODO: Add extra validation here
  OnApply();
}

void COptionsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
  CWnd::OnPaint();
  
  ::SetForegroundWindow(m_wndCommandBar.m_hWnd);
	// TODO: Add your message handler code here
	
	// Do not call CPropertySheet::OnPaint() for painting messages
}

void COptionsDlg::OnOptionsOk() 
{
	// TODO: Add your command handler code here
  OnApply();	
  EndDialog(IDOK);	
}

void COptionsDlg::OnOptionsCancel() 
{
	// TODO: Add your command handler code here
  EndDialog(IDCANCEL);	
}

void COptionsDlg::OnOptionsApply() 
{
	// TODO: Add your command handler code here
  OnApply();	
}

BOOL COptionsDlg::PreTranslateMessage(MSG* pMsg)
{
  if( pMsg->message == WM_CHAR && pMsg->wParam == VK_SPACE )
  {
    GetActivePage()->GetTopWindow()->SetFocus();
    return TRUE;
  }
  
  return CPropertySheet::PreTranslateMessage(pMsg);
}