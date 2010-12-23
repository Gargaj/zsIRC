#pragma once

class CChannelBar;

typedef enum {
  ZSIRC_WT_STATUS = 0,
  ZSIRC_WT_CHANNEL = 1,
  ZSIRC_WT_QUERY = 2,
} ZSIRC_WND_TYPE;

typedef struct {
  TCHAR sNick[256];
//  char bOp;
//  char bVoice;
  char bOnline;
  unsigned int nPrefixes;
} ZSIRC_USER;

typedef enum {
  ZSIRC_WH_NOTHING = 0,
  ZSIRC_WH_EVENT = 1,
  ZSIRC_WH_MESSAGE = 2,
  ZSIRC_WH_HIGHLIGHT = 3,
} ZSIRC_WND_HIGHLIGHT;

/////////////////////////////////////////////////////////////////////////////
// zsIRC_Wnd window

class zsIRC_Wnd : public CWnd
{
// Construction
public:
	zsIRC_Wnd(CChannelBar * c);

// Attributes
public:
  TCHAR szCaption[50];
  ZSIRC_WND_TYPE nType;
  ZSIRC_WND_HIGHLIGHT evEvent;
private:
  CChannelBar * m_ChannelBar;

// Operations
public:
  virtual void SetEventLevel(ZSIRC_WND_HIGHLIGHT);
	virtual void AddLine(TCHAR * szMessage,COLORREF cColor = 0);
  virtual void SetCaption(TCHAR * szNewCaption);
  CChannelBar * GetChannelBar();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(zsIRC_Wnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~zsIRC_Wnd();
	// Generated message map functions
protected:
	//{{AFX_MSG(zsIRC_Wnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
