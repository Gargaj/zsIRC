#if !defined(AFX_OPTIONSAPPEARANCEDLG_H__2057C3EE_0490_4678_88B6_30F0F2A60A6E__INCLUDED_)
#define AFX_OPTIONSAPPEARANCEDLG_H__2057C3EE_0490_4678_88B6_30F0F2A60A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsAppearanceDlg.h : header file
//

#include "ScrollableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsAppearanceDlg dialog

class COptionsAppearanceDlg : public CScrollableDialog
{
// Construction
public:
	COptionsAppearanceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsAppearanceDlg)
	enum { IDD = IDD_OPTIONS_APPEARANCE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

  void ApplyChanges();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsAppearanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsAppearanceDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSAPPEARANCEDLG_H__2057C3EE_0490_4678_88B6_30F0F2A60A6E__INCLUDED_)
