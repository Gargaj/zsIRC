#if !defined(AFX_ZSIRC_WND_QUERY_H__75A889D6_1885_40AA_9F1A_80AF89526D5A__INCLUDED_)
#define AFX_ZSIRC_WND_QUERY_H__75A889D6_1885_40AA_9F1A_80AF89526D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// zsIRC_Wnd_Query.h : header file
//

#include "zsIRC_Wnd.h"
#include "zsIRC_Settings.h"
#include "DynArray.h"
/////////////////////////////////////////////////////////////////////////////
// zsIRC_Wnd_Query window

class zsIRC_Wnd_Query : public zsIRC_Wnd
{
// Construction
public:
	zsIRC_Wnd_Query(CChannelBar*);

// Attributes
public:
  CDynArray aLines;
  int nScrollPos;
  CPoint ptSelect;
  int nSelectLine;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(zsIRC_Wnd_Query)
	//}}AFX_VIRTUAL
  void SetCaption(TCHAR * szNewCaption);

// Implementation
public:
	int BreakString(CString& sString, CRect r, CPaintDC& dc);
	//void AddLine(TCHAR * szSender,TCHAR * szMessage,COLORREF cColor = 0);
	void AddLine(TCHAR * szMessage,COLORREF cColor = SETUP.cForegroundColor);
	virtual ~zsIRC_Wnd_Query();

  void MarkLog(TCHAR * szSession);

	// Generated message map functions
protected:
	//{{AFX_MSG(zsIRC_Wnd_Query)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZSIRC_WND_QUERY_H__75A889D6_1885_40AA_9F1A_80AF89526D5A__INCLUDED_)
