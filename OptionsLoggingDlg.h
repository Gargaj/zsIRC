#if !defined(AFX_COptionsLoggingDlg_H__702A1D9D_AA6E_4152_8065_8B490E7A1A96__INCLUDED_)
#define AFX_COptionsLoggingDlg_H__702A1D9D_AA6E_4152_8065_8B490E7A1A96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// COptionsLoggingDlg.h : header file
//

#include "ScrollableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsLoggingDlg dialog

class COptionsLoggingDlg : public CScrollableDialog
{
// Construction
public:
	COptionsLoggingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsLoggingDlg)
	enum { IDD = IDD_OPTIONS_LOGGING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

  void ApplyChanges();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsLoggingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsLoggingDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COptionsLoggingDlg_H__702A1D9D_AA6E_4152_8065_8B490E7A1A96__INCLUDED_)
