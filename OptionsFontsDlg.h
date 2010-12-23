#if !defined(AFX_OPTIONSFONTSDLG_H__E30A4CBD_3CF5_4C24_BBCC_DFD674F87D2E__INCLUDED_)
#define AFX_OPTIONSFONTSDLG_H__E30A4CBD_3CF5_4C24_BBCC_DFD674F87D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsFontsDlg.h : header file
//

#include "ScrollableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsFontsDlg dialog

class COptionsFontsDlg : public CScrollableDialog
{
// Construction
public:
	COptionsFontsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsFontsDlg)
	enum { IDD = IDD_OPTIONS_FONTS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

  void ApplyChanges();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsFontsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsFontsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSFONTSDLG_H__E30A4CBD_3CF5_4C24_BBCC_DFD674F87D2E__INCLUDED_)
