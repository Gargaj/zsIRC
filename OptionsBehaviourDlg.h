#if !defined(AFX_OPTIONSBEHAVIOURDLG_H__A01810D0_492C_4BFB_919C_379DEDA3F34C__INCLUDED_)
#define AFX_OPTIONSBEHAVIOURDLG_H__A01810D0_492C_4BFB_919C_379DEDA3F34C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsBehaviourDlg.h : header file
//

#include "ScrollableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsBehaviourDlg dialog

class COptionsBehaviourDlg : public CScrollableDialog
{
// Construction
public:
	COptionsBehaviourDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsBehaviourDlg)
	enum { IDD = IDD_OPTIONS_BEHAVIOUR };
	CComboBox	m_Codepage;
	//}}AFX_DATA

  void ApplyChanges();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsBehaviourDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsBehaviourDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSBEHAVIOURDLG_H__A01810D0_492C_4BFB_919C_379DEDA3F34C__INCLUDED_)
