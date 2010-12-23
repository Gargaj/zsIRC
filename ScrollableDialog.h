// ScrollableDialog.h: interface for the CScrollableDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCROLLABLEDIALOG_H__C38DEB09_EB2D_49B3_A287_E4EC05975563__INCLUDED_)
#define AFX_SCROLLABLEDIALOG_H__C38DEB09_EB2D_49B3_A287_E4EC05975563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zsIRC_Settings.h"

class CScrollableDialog : public CPropertyPage  
{
public:
//	DECLARE_DYNAMIC(CScrollableDialog)
// 	CScrollableDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
// 	CScrollableDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
  CScrollableDialog(int nIDC);

public:
  void DoVScroll(int nValue);
  BOOL OnInitDialog();
  POINT size;
  virtual void ApplyChanges() = NULL;

private:
  int nScrollDelta;

	// Generated message map functions
	//{{AFX_MSG(COptionsConnectionDlg)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnSize(UINT,int,int);
	//}}AFX_MSG
protected:
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCROLLABLEDIALOG_H__C38DEB09_EB2D_49B3_A287_E4EC05975563__INCLUDED_)
