#if !defined(AFX_OPTIONSNICKSDLG_H__8745E433_189A_4840_95BC_D7E4F2F83BF2__INCLUDED_)
#define AFX_OPTIONSNICKSDLG_H__8745E433_189A_4840_95BC_D7E4F2F83BF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsNicksDlg.h : header file
//

#include "ScrollableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsNicksDlg dialog

class COptionsNicksDlg : public CScrollableDialog
{
// Construction
public:
	COptionsNicksDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsNicksDlg)
	enum { IDD = IDD_OPTIONS_NICKS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

  void ApplyChanges();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsNicksDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsNicksDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSNICKSDLG_H__8745E433_189A_4840_95BC_D7E4F2F83BF2__INCLUDED_)
