// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__064A1697_D4B9_4817_9164_0145A1BCBC00__INCLUDED_)
#define AFX_CHILDVIEW_H__064A1697_D4B9_4817_9164_0145A1BCBC00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChannelBar.h"

//extern CChannelBar * g_ChannelBar;

/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
  //CChannelBar m_ChannelBar;
  int nDestroyed;
// Operations
public:
  void InvalidateChannels();
  int AddNewChannelbar();
  CChannelBar * GetActiveChannelBar();
  void SetActiveChannelBar(int i);
  void RemoveActiveChannelBar();

  int nSelected;
  CDynArray daChannelBars;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL


// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCancelMode();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__064A1697_D4B9_4817_9164_0145A1BCBC00__INCLUDED_)
