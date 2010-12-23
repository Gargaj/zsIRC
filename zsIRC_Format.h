// zsIRC_Format.h: interface for the zsIRC_Format class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZSIRC_FORMAT_H__AEF0BE93_1288_4CFE_A6B3_EB7DB44EEB75__INCLUDED_)
#define AFX_ZSIRC_FORMAT_H__AEF0BE93_1288_4CFE_A6B3_EB7DB44EEB75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class zsIRC_Format  
{
public:
	zsIRC_Format();
	virtual ~zsIRC_Format();

  static CString Say(CString nick,CString message);

  static CString Act(CString nick,CString message);

  static CString Join(CString nick,CString host,CString channel);
  static CString Part(CString nick,CString host,CString channel,CString qmsg);
  static CString Quit(CString nick,CString host,CString qmsg);

  static CString Nick(CString oldnick,CString newnick);
  static CString Mode(CString nick,CString mode);
  static CString Notice(CString nick,CString mode,CString rest);
  static CString Kick(CString nick,CString victim,CString message);

  static CString Topic(CString topic);
  static CString TopicSetBy(CString host,CString time);
  static CString TopicChange(CString nick,CString topic);

  static CString NotifyJoin(CString nick);
  static CString NotifyQuit(CString nick);

  static CString Invite(CString nick,CString channel);
};

#endif // !defined(AFX_ZSIRC_FORMAT_H__AEF0BE93_1288_4CFE_A6B3_EB7DB44EEB75__INCLUDED_)
