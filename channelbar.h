#if !defined(AFX_CHANNELBAR_H__770F7FC3_7EA7_43AC_AEFC_3F090E0A40FE__INCLUDED_)
#define AFX_CHANNELBAR_H__770F7FC3_7EA7_43AC_AEFC_3F090E0A40FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelBar.h : header file
//

#include "DynArray.h"
#include "zsIRC_Wnd.h"
#include "zsIRC_Socket.h"
#include "zsIRC_Wnd_Query.h"
#include "zsIRC_EventHandler.h"

/////////////////////////////////////////////////////////////////////////////
// CChannelBar window

#define WM_CBAR_CHECKQUEUE (WM_USER+303)
#define WM_CBAR_DISCONNECT (WM_USER+304)

class CChannelBar : public CWnd
{
// Construction
public:
	CChannelBar();
  virtual ~CChannelBar();

// Attributes
public:
  int nSelected;
  CDynArray daWindows;
  CDynArray daMessages;
  zsIRC_Socket scSocket;
  HANDLE hThread;
  DWORD hThreadID;

  CString sNick;
  int nAltNickNumber;

  CString sChanTypes;
  CString sPrefixes;
  CString sPrefixReadables;
  CString sNetwork;

  int nPackets;
  CString sSocketLeftover;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelBar)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	void CheckNotify();

  void FocusWindow(int i);
	zsIRC_Wnd * AddWindow(ZSIRC_WND_TYPE nType,TCHAR * szCaption);
	void CloseWindow(TCHAR * szCaption);
	zsIRC_Wnd * FindWindow(TCHAR * szName);
  void HideAllWindows();

  void HandleIncoming();
  int nThreadActive;
  int nThreadRunning;
  int nDisconnect;

  void Connect();
  void Disconnect();
  int Handshake();

  void ParseCommand(TCHAR*,int nForce = 0);
  void SendIRCMessage(TCHAR*);
	void ParseIRCMessage(TCHAR*msg);

  zsIRC_Wnd_Query * wndStatus;
  zsIRC_Wnd_Query * wndDebug;
  
  CDynArray daNotify;
  zsIRC_EventHandler EVENTS;

	// Generated message map functions
protected:
	//{{AFX_MSG(CChannelBar)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnDestroy();
  afx_msg void OnCheckQueue(WPARAM, LPARAM);
  afx_msg void OnDisconnect(WPARAM, LPARAM);
	afx_msg void OnCbarClose();
  afx_msg void OnWTF();
  afx_msg void OnChannelBarSwitch( UINT nID );
  afx_msg LRESULT CChannelBar::OnNotification(WPARAM wParam, LPARAM lParam);
  //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CChannelBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	//DECLARE_DISPATCH_MAP()
	//DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELBAR_H__770F7FC3_7EA7_43AC_AEFC_3F090E0A40FE__INCLUDED_)
