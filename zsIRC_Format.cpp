// zsIRC_Format.cpp: implementation of the zsIRC_Format class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zsIRC.h"
#include "zsIRC_Format.h"
#include "zsIRC_Settings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

zsIRC_Format::zsIRC_Format()
{

}

zsIRC_Format::~zsIRC_Format()
{

}

CString zsIRC_Format::Say(CString nick,CString message) {
  CString s;
  s += _T("<");
  s += nick;
  s += _T("> ");
  s += message;
  return s;
}

CString zsIRC_Format::Kick(CString nick,CString victim,CString message) {
  CString str = _T("* ");
  str += victim;
  str += _T(" was kicked by ");
  str += nick;
  if (message.GetLength()) {
    str += _T(" (");
    str += message;
    str += _T(")");
  }
  return str;
}

CString zsIRC_Format::Nick(CString oldnick,CString newnick) {
  CString str = _T("* ");
  str += oldnick;
  str += _T(" is now known as ");
  str += newnick;
  return str;
}

CString zsIRC_Format::Act(CString nick,CString message) {
  CString str = _T("* ");
  str += nick;
  str += _T(" ");
  str += message;
  return str;
}

CString zsIRC_Format::Mode(CString nick,CString mode) {
  CString str = _T("* ");
  str += nick;
  str += " sets mode: ";
  str += mode;
  return str;
}

CString zsIRC_Format::Part(CString nick,CString host,CString channel,CString qstr) {
  CString str = _T("* ");
  str += nick;
  if (SETUP.nShowHosts) {
    str += _T(" (");
    str += host;
    str += _T(")");
  }
  str += _T(" has left ");
  str += channel;
  if (qstr.GetLength()) {
    str += _T(" (");
    str += qstr;
    str += _T(")");
  }
  return str;
}


CString zsIRC_Format::Quit(CString nick,CString host,CString qstr) {
  CString str = _T("* ");
  str += nick;
  if (SETUP.nShowHosts) {
    str += _T(" (");
    str += host;
    str += _T(")");
  }
  str += _T(" has quit IRC");
  if (qstr.GetLength()) {
    str += _T(" (");
    str += qstr;
    str += _T(")");
  }
  return str;
}

CString zsIRC_Format::Join(CString nick,CString host,CString channel) {
  CString str = _T("* ");
  str += nick;
  if (SETUP.nShowHosts) {
    str += _T(" (");
    str += host;
    str += _T(")");
  }
  str += _T(" has joined ");
  str += channel;
  return str;
}

CString zsIRC_Format::Topic(CString topic) {
  CString str = _T("* Topic is '");
  str += topic;
  str += _T("'");
  return str;
}

CString zsIRC_Format::TopicSetBy(CString host,CString time) {
  CString str = _T("* Set by ");
  str += host;
  str += _T(" at ");
  str += time;
  return str;
}

CString zsIRC_Format::TopicChange(CString nick,CString topic) {
  CString str = _T("* ");
  str += nick;
  str += _T(" sets topic to '");
  str += topic;
  str += _T("'");
  return str;
}

CString zsIRC_Format::NotifyJoin(CString nick) {
  CString str = _T("* ");
  str += nick;
  str += _T(" has connected IRC");
  return str;
}

CString zsIRC_Format::NotifyQuit(CString nick) {
  CString str = _T("* ");
  str += nick;
  str += _T(" has left IRC");
  return str;
}

CString zsIRC_Format::Invite(CString nick,CString channel) {
  CString str = _T("* ");
  str += nick;
  str += _T(" has invited you to ");
  str += channel;
  return str;
}