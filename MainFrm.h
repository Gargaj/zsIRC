// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__3FB58B4C_CDFB_4275_AFFE_522E0D7E48A7__INCLUDED_)
#define AFX_MAINFRM_H__3FB58B4C_CDFB_4275_AFFE_522E0D7E48A7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ChildView.h"

// Array for the toolbar buttons

#if (_WIN32_WCE < 201)
static TBBUTTON g_arCBButtons[] = {
	{ 0,	ID_FILE_NEW,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  0},
	{ 1,    ID_FILE_OPEN,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  1},
	{ 2,	ID_FILE_SAVE,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  2},
	{ 0,	0,				TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0, 0, -1},
	{ 3,    ID_EDIT_CUT,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  3},
	{ 4,	ID_EDIT_COPY,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  4},
	{ 5,	ID_EDIT_PASTE,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0, 0,  5}
};
#endif

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 212)
#define NUM_TOOL_TIPS 8
#endif

#include "EditEx.h"

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:
	CChildView    m_wndView;

// Operations
public:
  void CheckForUpdates(bool);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CCeCommandBar	m_wndCommandBar;
  CEditEx       m_wndEdit;

public:
  CChannelBar * GetActiveChannelBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnExit();
	afx_msg void OnMenuConnect();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuCheck();
	afx_msg void OnMenuVisit();
  afx_msg void OnHotkey(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnMenuOptions();
	afx_msg void OnMenuNextchan();
	afx_msg void OnMenuNewserver();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__3FB58B4C_CDFB_4275_AFFE_522E0D7E48A7__INCLUDED_)
