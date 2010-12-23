#if !defined(AFX_ZSIRC_WND_CHANNEL_H__4BEC52DA_C440_449E_A396_8C7C63C0FA9F__INCLUDED_)
#define AFX_ZSIRC_WND_CHANNEL_H__4BEC52DA_C440_449E_A396_8C7C63C0FA9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// zsIRC_Wnd_Channel.h : header file
//

#include "zsIRC_Settings.h"
#include "zsIRC_Wnd.h"
#include "zsIRC_Wnd_Query.h"
#include "Nicklist.h"
/////////////////////////////////////////////////////////////////////////////
// zsIRC_Wnd_Channel window

class zsIRC_Wnd_Channel : public zsIRC_Wnd
{
// Construction
public:
	zsIRC_Wnd_Channel(CChannelBar*);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(zsIRC_Wnd_Channel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~zsIRC_Wnd_Channel();

  zsIRC_Wnd_Query m_wndQuery;
	//void AddLine(TCHAR * szSender,TCHAR * szMessage,COLORREF cColor = 0);
	void AddLine(TCHAR * szMessage,COLORREF cColor = SETUP.cForegroundColor);
  LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam); 

  void AddUser(TCHAR * szNick);
  void RemoveUser(TCHAR * szNick);
  void RefreshUserList();
  void AddUserlist(TCHAR * szNick);
  void CommitUserlist();
  int isOnChannel(TCHAR * szNick);
  ZSIRC_USER * GetUser(TCHAR * szNick);

  CNickList mNicklist;
  CDynArray daPeople;
  CFont fFont;

  CString sUserlistCache;
	// Generated message map functions
protected:
	//{{AFX_MSG(zsIRC_Wnd_Channel)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZSIRC_WND_CHANNEL_H__4BEC52DA_C440_449E_A396_8C7C63C0FA9F__INCLUDED_)
