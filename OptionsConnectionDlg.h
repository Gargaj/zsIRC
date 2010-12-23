#if !defined(AFX_OPTIONSCONNECTIONDLG_H__FD598A15_7ECE_4BC5_85AE_8EC4AE558889__INCLUDED_)
#define AFX_OPTIONSCONNECTIONDLG_H__FD598A15_7ECE_4BC5_85AE_8EC4AE558889__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsConnectionDlg.h : header file
//

#include "ScrollableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsConnectionDlg dialog

class COptionsConnectionDlg : public CScrollableDialog
{
// Construction
public:
	COptionsConnectionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsConnectionDlg)
	enum { IDD = IDD_OPTIONS_CONNECTION };
	CComboBox	m_SSLInvalidBehaviour;
	//}}AFX_DATA

  void ApplyChanges();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsConnectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsConnectionDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSCONNECTIONDLG_H__FD598A15_7ECE_4BC5_85AE_8EC4AE558889__INCLUDED_)
