// zsIRC_EventHandler.h: interface for the zsIRC_EventHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZSIRC_EVENTHANDLER_H__E36AFA34_12C6_4748_A5FE_9B0870914E84__INCLUDED_)
#define AFX_ZSIRC_EVENTHANDLER_H__E36AFA34_12C6_4748_A5FE_9B0870914E84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChannelBar;

typedef enum {
  ZSIRC_EVENT_NONE = 0,
  ZSIRC_EVENT_CONNECT,
  ZSIRC_EVENT_JOIN,
} ZSIRC_EVENTTYPE;

class zsIRC_EventHandler  
{
public:
	zsIRC_EventHandler(CChannelBar * c);
	virtual ~zsIRC_EventHandler();

  void Load();
  void CommitCommands(ZSIRC_EVENTTYPE);

  CString sEventCommands[3];

  CChannelBar * m_ChannelBar;
};

//extern zsIRC_EventHandler EVENTS;

#endif // !defined(AFX_ZSIRC_EVENTHANDLER_H__E36AFA34_12C6_4748_A5FE_9B0870914E84__INCLUDED_)
