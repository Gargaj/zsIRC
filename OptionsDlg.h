#if !defined(AFX_OPTIONSDLG_H__CDB9DAF0_F709_4BBB_8293_06FFAF83EA2A__INCLUDED_)
#define AFX_OPTIONSDLG_H__CDB9DAF0_F709_4BBB_8293_06FFAF83EA2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

#define SUBDIALOGS 7

#include "zsIRC_Settings.h"

#include "ScrollableDialog.h"
#include "OptionsConnectionDlg.h"
#include "OptionsAppearanceDlg.h"
#include "OptionsBehaviourDlg.h"
#include "OptionsLoggingDlg.h"
#include "OptionsFontsDlg.h"
#include "OptionsColorsDlg.h"
#include "OptionsNicksDlg.h"

typedef struct {
  POINT size;
  CScrollableDialog * dialog;
} SUBDIALOG;
/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CPropertySheet
{
// Construction
public:
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
//	enum { IDD = IDD_OPTIONS };
	//}}AFX_DATA

  SUBDIALOG mSubdialogs[SUBDIALOGS];

  COptionsConnectionDlg mOptionsConnectionDlg;
  COptionsAppearanceDlg mOptionsAppearanceDlg;
  COptionsBehaviourDlg mOptionsBehaviourDlg;
  COptionsLoggingDlg mOptionsLoggingDlg;
  COptionsFontsDlg mOptionsFontsDlg;
  COptionsColorsDlg mOptionsColorsDlg;
  COptionsNicksDlg mOptionsNicksDlg;

  CCeCommandBar	m_wndCommandBar;

  BOOL PreTranslateMessage(MSG* pMsg);
  void Setup();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnApply();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnOptionsOk();
	afx_msg void OnOptionsCancel();
	afx_msg void OnOptionsApply();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__CDB9DAF0_F709_4BBB_8293_06FFAF83EA2A__INCLUDED_)
