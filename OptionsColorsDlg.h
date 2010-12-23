#if !defined(AFX_OPTIONSCOLORSDLG_H__FA965A29_9081_4412_B04A_B9A27031A172__INCLUDED_)
#define AFX_OPTIONSCOLORSDLG_H__FA965A29_9081_4412_B04A_B9A27031A172__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsColorsDlg.h : header file
//

#include "ScrollableDialog.h"
#include "ColorButton.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsColorsDlg dialog

class COptionsColorsDlg : public CScrollableDialog
{
// Construction
public:
	COptionsColorsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsColorsDlg)
	enum { IDD = IDD_OPTIONS_COLORS };
	//}}AFX_DATA

  CColorButton m_BackgroundColor;
  CColorButton m_ForegroundColor;
  CColorButton m_ActionColor;
  CColorButton m_JoinColor;
  CColorButton m_ModeColor;
  CColorButton m_NoticeColor;
  CColorButton m_PartColor;

  void ApplyChanges();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsColorsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsColorsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSCOLORSDLG_H__FA965A29_9081_4412_B04A_B9A27031A172__INCLUDED_)
