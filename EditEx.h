#if !defined(AFX_EDITEX_H__A6F77842_5081_4505_95E8_471F2955B360__INCLUDED_)
#define AFX_EDITEX_H__A6F77842_5081_4505_95E8_471F2955B360__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditEx.h : header file
//

#include "DynArray.h"
/////////////////////////////////////////////////////////////////////////////
// CEditEx window

class CEditEx : public CEdit
{
// Construction
public:
	CEditEx();

// Attributes
public:
  CDynArray daBuffer;
  int nPosInBuffer;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditEx)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetfocus();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITEX_H__A6F77842_5081_4505_95E8_471F2955B360__INCLUDED_)
