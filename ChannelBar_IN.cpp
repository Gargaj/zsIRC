// ChannelBar.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "ChannelBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "zsIRC_Line.h"
#include "zsIRC_Settings.h"
#include "zsIRC_Wnd_Query.h"
#include "zsIRC_Wnd_Channel.h"
#include "zsIRC_Hostmask.h"
#include "zsIRC_StringTokenizer.h"
#include "zsIRC_EventHandler.h"
#include "zsIRC_Format.h"
#include "replies.h"

/////////////////////////////////////////////////////////////////////////////
// CChannelBar

void CChannelBar::ParseIRCMessage(TCHAR * szMessage)
{
  CString s2 = _T("[<<] ");
  s2+=szMessage;
  wndDebug->AddLine(s2.LockBuffer());

  //////////////////////////////////////////////////////////////////////////
  // MAJOR PART COMES HERE

  // preprocess
  CString msg = szMessage,sender,command,params;
  if (msg.GetAt(0) == _T(':')) {
    int nPos = msg.Find(_T(' '));
    sender = msg.Mid(1,nPos-1);

    msg = msg.Right(msg.GetLength()-nPos-1);
  }
  zsIRC_Hostmask hmMask(sender.LockBuffer());

  int nPos = msg.Find(_T(' '));
  command = msg.Mid(0,nPos);
  params = msg.Right(msg.GetLength()-nPos-1);

  //////////////////////////////////////////////////////////////////////////

  zsIRC_StringTokenizer stToken(params);

  if (_T('0')<=command.GetAt(0) && command.GetAt(0)<=_T('9')) {
    int cmdnum = 0;
    cmdnum += command.GetAt(0) - _T('0'); cmdnum*=10;
    cmdnum += command.GetAt(1) - _T('0'); cmdnum*=10;
    cmdnum += command.GetAt(2) - _T('0');

    nPos = params.Find(_T(' '));
    CString recipient = params.Mid(0,nPos);
    params = params.Mid(nPos+1);

    stToken = zsIRC_StringTokenizer(params);

    //////////////////////////////////////////////////////////////////////////
    
    switch (cmdnum) {
      case RPL_WELCOME: {
        sNick = recipient; // in case we change nick before connecting

        if (SETUP.nRejoinOnConnect) {
          for (int i=0; i<daWindows.Num(); i++)
            if ( ((zsIRC_Wnd*)daWindows[i])->nType == ZSIRC_WT_CHANNEL ) {
              CString s = _T("JOIN ");
              s += ((zsIRC_Wnd*)daWindows[i])->szCaption;
              SendIRCMessage(s.LockBuffer());
            }
        }

        CString msg = stToken.GetRightSliceWithoutCollapse(_T(':'));
        wndStatus->AddLine(msg.LockBuffer());

        EVENTS.CommitCommands(ZSIRC_EVENT_CONNECT);

        if (SETUP.nCheckForUpdates)
          ((CMainFrame*)theApp.m_pMainWnd)->CheckForUpdates(true);

      } break;
      case RPL_MYINFO: {
        wndStatus->AddLine(params.LockBuffer());
    	} break;
      case RPL_ISUPPORT: {
        wndStatus->AddLine(params.LockBuffer());
        CString p = stToken.GetSlice(_T(':'),0);

        stToken = zsIRC_StringTokenizer(p);

        for (int i=0; i <= stToken.GetOccurrences(_T(' ')); i++) {
          CString s = stToken.GetSlice(_T(' '),i);
          if (s.Find(_T("CHANTYPES="))==0) {
            sChanTypes = s.Mid(s.Find(_T('='))+1);
          }
          if (s.Find(_T("PREFIX="))==0) {
            sPrefixes = s.Mid(s.Find(_T(')'))+1);
            sPrefixReadables = s.Mid(s.Find(_T('('))+1,sPrefixes.GetLength());
          }
          if (s.Find(_T("NETWORK="))==0) {
            sNetwork = s.Mid(s.Find(_T('='))+1);
            CString s = _T("Status: ") + sNetwork;
            wndStatus->SetCaption(s.LockBuffer());
          }
          
        }
    	} break;
      case RPL_YOURID: // unique id
      case RPL_LUSEROP:
      case RPL_LUSERUNKNOWN:
      case RPL_LUSERCHANNELS: {
        CString num = stToken.GetSlice(_T(' '),0);
        CString msg = stToken.GetRightSliceWithoutCollapse(_T(':'));
        wndStatus->AddLine((num+_T(' ')+msg).LockBuffer());
      } break;
      case RPL_NAMREPLY: { 
        CString whatever = stToken.GetSlice(_T(' '),0);
        CString channel  = stToken.GetSlice(_T(' '),1);
        CString list = stToken.GetRightSliceWithoutCollapse(_T(':'));
        zsIRC_Wnd * wnd = FindWindow(channel.LockBuffer());
        if (wnd) {
          if (wnd->nType==ZSIRC_WT_CHANNEL) ((zsIRC_Wnd_Channel *)wnd)->AddUserlist(list.LockBuffer());
        }
      } break;
      case RPL_ENDOFNAMES: {
        CString channel  = stToken.GetSlice(_T(' '),0);
        zsIRC_Wnd * wnd = FindWindow(channel.LockBuffer());
        if (wnd) {
          if (wnd->nType==ZSIRC_WT_CHANNEL) ((zsIRC_Wnd_Channel *)wnd)->CommitUserlist();
        }
      } break;
      case RPL_TOPIC: {
        CString channel  = stToken.GetSlice(_T(' '),0);
        CString topic = stToken.GetRightSliceWithoutCollapse(_T(':'));
        zsIRC_Wnd * wnd = FindWindow(channel.LockBuffer());

        CString str = zsIRC_Format::Topic(topic);
        if (wnd) {
          if (wnd->nType==ZSIRC_WT_CHANNEL) ((zsIRC_Wnd_Channel *)wnd)->AddLine(str.LockBuffer(),SETUP.cMode);
        }
    	} break;
      case (RPL_TOPIC+1): { //set by
        CString channel = stToken.GetSlice(_T(' '),0);
        CString host    = stToken.GetSlice(_T(' '),1);
        CString time    = stToken.GetSlice(_T(' '),2);
        time_t t;
        _stscanf(time.LockBuffer(),_T("%lu"),&t);

        LONGLONG ll;
        FILETIME ft;
        ll = Int32x32To64(t, 10000000) + 116444736000000000;
        ft.dwLowDateTime = (DWORD)ll;
        ft.dwHighDateTime = (DWORD)(ll >> 32);
        CTime tt(ft);

        CString ts;
        ts.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
          tt.GetYear(),tt.GetMonth(),tt.GetDay(),
          tt.GetHour(),tt.GetMinute(),tt.GetSecond());
       
        CString str = zsIRC_Format::TopicSetBy(host,ts);
        zsIRC_Wnd * wnd = FindWindow(channel.LockBuffer());
        if (wnd) {
          if (wnd->nType==ZSIRC_WT_CHANNEL) ((zsIRC_Wnd_Channel *)wnd)->AddLine(str.LockBuffer(),SETUP.cMode);
        }
    	} break;
      case RPL_WHOWASUSER: 
      case RPL_WHOISUSER: {
        CString nick  = stToken.GetSlice(_T(' '),0);
        CString ident = stToken.GetSlice(_T(' '),1);
        CString host  = stToken.GetSlice(_T(' '),2);
        CString wtf   = stToken.GetSlice(_T(' '),3);
        CString name  = stToken.GetRightSliceWithoutCollapse(_T(':'));
    
        CString s;
        s += nick;
        s += _T(" is ");
        s += ident;
        s += _T("@");
        s += host;
        s += _T(" - ");
        s += name;
        wndStatus->AddLine(s.LockBuffer());
      } break;
      case RPL_WHOISCHANNELS: {
        CString nick  = stToken.GetSlice(_T(' '),0);
        CString chans = stToken.GetRightSliceWithoutCollapse(_T(':'));

        CString s;
        s += nick;
        s += _T(" is on ");
        s += chans;
        wndStatus->AddLine(s.LockBuffer());
      } break;
      case RPL_WHOISSERVER: {
        CString nick = stToken.GetSlice(_T(' '),0);
        CString host = stToken.GetSlice(_T(' '),1);
        CString name = stToken.GetRightSliceWithoutCollapse(_T(':'));

        CString s;
        s += nick;
        s += _T(" is using ");
        s += host;
        s += _T(" - ");
        s += name;
        wndStatus->AddLine(s.LockBuffer());
      } break;
      case RPL_WHOISIDLE: {
        CString nick   = stToken.GetSlice(_T(' '),0);
        CString idle   = stToken.GetSlice(_T(' '),1);
        CString signon = stToken.GetSlice(_T(' '),2);
        
        int nIdle,nSignon;

        _stscanf(  idle.LockBuffer(),_T("%d"),&nIdle);
        _stscanf(signon.LockBuffer(),_T("%d"),&nSignon);

        int nIdleSec  = nIdle % 60; nIdle/=60;
        int nIdleMin  = nIdle % 60; nIdle/=60;
        int nIdleHour = nIdle % 24; nIdle/=24;
        int nIdleDay  = nIdle;

        CString s,f;
        s += nick;
        s += _T(" is idle since ");
        if (nIdleDay) {
          f.Format(_T("%d days %d hrs %d mins %d secs"),nIdleDay,nIdleHour,nIdleMin,nIdleSec);
          s +=f;
        }
        else if (nIdleHour) {
          f.Format(_T("%d hrs %d mins %d secs"),nIdleHour,nIdleMin,nIdleSec);
          s +=f;
        }
        else if (nIdleMin) {
          f.Format(_T("%d mins %d secs"),nIdleMin,nIdleSec);
          s +=f;
        }
        else if (nIdleSec) {
          f.Format(_T("%d secs"),nIdleSec);
          s +=f;
        }
        s += _T(" - Signed on at ");
        s += signon;
        wndStatus->AddLine(s.LockBuffer());
      } break;
      case RPL_AWAY: {
        CString nick   = stToken.GetSlice(_T(' '),0);
        CString reason = stToken.GetRightSliceWithoutCollapse(_T(':'));

        CString s,f;
        s += nick;
        s += _T(" is away: ");
        s += reason;
        wndStatus->AddLine(s.LockBuffer());
      } break;
      case RPL_ISON: {
        CString list = stToken.GetRightSliceWithoutCollapse(_T(':'));
        zsIRC_StringTokenizer stNotify(list);
        for (int j=0; j<daNotify.Num(); j++) {
          ZSIRC_USER * u = (ZSIRC_USER*)daNotify[j];
          int found = 0;
          for (int i=0; i<=stNotify.GetOccurrences(_T(' ')); i++) {
            if ( stNotify.GetSlice(_T(' '),i).CompareNoCase(u->sNick)==0 ) {
              found = 1;
              if (!u->bOnline) {
                u->bOnline = 1;
                CString s = zsIRC_Format::NotifyJoin(u->sNick);
                wndStatus->AddLine(s.LockBuffer(),SETUP.cNotice);
              }
            }
          }
          if (!found) {
            if (u->bOnline) {
              u->bOnline = 0;
              CString s = zsIRC_Format::NotifyQuit(u->sNick);
              wndStatus->AddLine(s.LockBuffer(),SETUP.cNotice);
            }
          }
        }
      } break;
      case 433: { // nick already taken
        CString wrongnick = stToken.GetSlice(_T(' '),0);
        CString msg = stToken.GetRightSliceWithoutCollapse(_T(':'));
        wndStatus->AddLine((wrongnick+_T(" - ")+msg).LockBuffer());
        if (nAltNickNumber<=9) {
          CString newnick = SETUP.szDefaultNick[nAltNickNumber++];
          CString str = _T("NICK ");
          str += newnick;
          SendIRCMessage(str.LockBuffer());
          wndStatus->AddLine((CString(_T("Changing nick to "))+newnick).LockBuffer());
        }        
      } break;
      default: {
        CString msg = stToken.GetRightSliceWithoutCollapse(_T(':'));
        wndStatus->AddLine(msg.LockBuffer());
      } break;
    }

  }

  else if (command==_T("JOIN")) {
    CString channel = stToken.GetRightSliceWithoutCollapse(_T(':'));
    if (!channel.GetLength()) {
      channel = params;
    }
    zsIRC_Wnd * wnd = FindWindow(channel.LockBuffer());
    if (sNick == hmMask.szNick) {
      if (wnd)
        ((zsIRC_Wnd_Channel *)wnd)->AddLine(_T("Rejoined channel."),SETUP.cJoin);
      else
        AddWindow(ZSIRC_WT_CHANNEL,channel.LockBuffer());

      EVENTS.CommitCommands(ZSIRC_EVENT_JOIN);
    } else {

      CString str = zsIRC_Format::Join(hmMask.szNick,hmMask.GetShortHost(),channel);
      if (wnd) {
        if (wnd->nType==ZSIRC_WT_CHANNEL) {
          ((zsIRC_Wnd_Channel *)wnd)->AddLine(str.LockBuffer(),SETUP.cJoin);
          ((zsIRC_Wnd_Channel *)wnd)->AddUser(hmMask.szNick);
        }
      }
    }
  }

  else if (command==_T("QUIT")) {
    CString qmsg = stToken.GetRightSliceWithoutCollapse(_T(':'));

    if (sNick == hmMask.szNick) {
      // van ijen egyaltalan?
    } else {

      CString str = zsIRC_Format::Quit(hmMask.szNick,hmMask.GetShortHost(),qmsg);

      for (int i=0; i<daWindows.Num(); i++) {
        zsIRC_Wnd * wnd = (zsIRC_Wnd *)daWindows[i];
        if (wnd->nType==ZSIRC_WT_CHANNEL && ((zsIRC_Wnd_Channel *)wnd)->isOnChannel(hmMask.szNick)) {
          ((zsIRC_Wnd_Channel *)wnd)->AddLine(str.LockBuffer(),SETUP.cPart);
          ((zsIRC_Wnd_Channel *)wnd)->RemoveUser(hmMask.szNick);
        }
        if (wnd->nType==ZSIRC_WT_QUERY && _tcsicmp(wnd->szCaption,hmMask.szNick)==0)
          ((zsIRC_Wnd_Query   *)wnd)->AddLine(str.LockBuffer(),SETUP.cPart);
      }
      //wndStatus->AddLine(str.LockBuffer(),SETUP.cPart);
    }
  }

  else if (command==_T("PART")) {
    CString channel = stToken.GetSlice(_T(' '),0);
    CString partmessage = stToken.GetRightSliceWithoutCollapse(_T(':'));

    if (sNick == hmMask.szNick) {
      CloseWindow(channel.LockBuffer());
    } else {
      zsIRC_Wnd * wnd = FindWindow(channel.LockBuffer());

      CString str = zsIRC_Format::Part(hmMask.szNick,hmMask.GetShortHost(),channel,partmessage);
      if (wnd) {
        if (wnd->nType==ZSIRC_WT_CHANNEL) {
          ((zsIRC_Wnd_Channel *)wnd)->AddLine(str.LockBuffer(),SETUP.cPart);
          ((zsIRC_Wnd_Channel *)wnd)->RemoveUser(hmMask.szNick);
        }
      }
    }
  }

  else if (command==_T("MODE")) {
    CString recipient = stToken.GetSlice(_T(' '),0);
    zsIRC_Wnd * wnd = FindWindow(recipient.LockBuffer());
    if (wnd) {
      // :Scenergy!scenergy@scenergy.dfmk.hu MODE #cns +o garg^zsirc 

      CString rest = stToken.GetRightSlice(_T(' '));
      CString str = zsIRC_Format::Mode(hmMask.GetName(),rest);

      if (wnd->nType==ZSIRC_WT_CHANNEL) {
        zsIRC_Wnd_Channel * wndC = ((zsIRC_Wnd_Channel *)wnd);
        zsIRC_StringTokenizer stRest(rest);
        wndC->AddLine(str.LockBuffer(),SETUP.cMode);

        CString mode = stRest.GetSlice(_T(' '),0);

        int modesign = 1;
        int slice = 1;
        for (int i=0; i<mode.GetLength(); i++) {
          switch (mode.GetAt(i)) {
            case _T('+'): {
              modesign = 1;
            } break;
            case _T('-'): {
              modesign = 2;
            } break;
/*
            case _T('o'): {
              CString user = stRest.GetSlice(_T(' '),slice++);
              ZSIRC_USER * u = wndC->GetUser(user.LockBuffer());
              if (u) u->bOp = (modesign == 1);
              wndC->RefreshUserList();
            } break;
            case _T('v'): {
              CString user = stRest.GetSlice(_T(' '),slice++);
              ZSIRC_USER * u = wndC->GetUser(user.LockBuffer());
              if (u) u->bVoice = (modesign == 1);
              wndC->RefreshUserList();
            } break;
*/
            case _T('b'): {
              CString hostmask = stRest.GetSlice(_T(' '),slice++);
            } break;
            case _T('e'): {
              CString hostmask = stRest.GetSlice(_T(' '),slice++);
            } break;
            case _T('k'): {
              CString hostmask = stRest.GetSlice(_T(' '),slice++);
            } break;
            case _T('l'): {
              CString hostmask = stRest.GetSlice(_T(' '),slice++);
            } break;
          }
          int nPref = sPrefixReadables.Find(mode.GetAt(i));
          if (nPref!=-1) {
            CString user = stRest.GetSlice(_T(' '),slice++);
            ZSIRC_USER * u = wndC->GetUser(user.LockBuffer());
            if (u) {
              if (modesign == 1) u->nPrefixes |=  (1<<nPref);
              if (modesign == 2) u->nPrefixes &= ~(unsigned int)(1<<nPref);
            }
            wndC->RefreshUserList();
          }
        }
      }

    } else {
      // server mode
      // :garg^zsirc MODE garg^zsirc :+i
      CString rest = stToken.GetRightSliceWithoutCollapse(_T(':'));
      CString str = zsIRC_Format::Mode(hmMask.szNick,rest);
      wndStatus->AddLine(str.LockBuffer(),SETUP.cMode);
    }
  }
  
  else if (command==_T("PRIVMSG")) {
    CString recipient = stToken.GetSlice(_T(' '),0);
    CString message = stToken.GetRightSliceWithoutCollapse(_T(':'));

    if (message.GetAt(0)==_T('\x01')) {
      //////////////////////////////////////////////////////////////////////////
      // CTCP
      message = message.Mid(1);
      message = message.Left(message.GetLength()-1); // chop chop!
      zsIRC_StringTokenizer stMsg(message);

      CString ctcpcmd = stMsg.GetSlice(_T(' '),0);
      if (ctcpcmd==_T("ACTION")) {

        CString str = zsIRC_Format::Act(hmMask.szNick,message.Mid(7));

        
        zsIRC_Wnd * wnd = NULL;
        if ( sChanTypes.Find(recipient[0])!=-1 )
          wnd = FindWindow(recipient.LockBuffer());
        else
          wnd = FindWindow(hmMask.szNick);
        if (wnd) {
          if (wnd->nType==ZSIRC_WT_CHANNEL) ((zsIRC_Wnd_Channel *)wnd)->AddLine(str.LockBuffer(),SETUP.cAction);
          if (wnd->nType==ZSIRC_WT_QUERY)   ((zsIRC_Wnd_Query   *)wnd)->AddLine(str.LockBuffer(),SETUP.cAction);
        }        
      } else if (ctcpcmd==_T("VERSION")) {

        CString str;
        str += _T("NOTICE ");
        str += hmMask.szNick;
        str += _T(" :\x01");
        str += _T("VERSION zsIRC:");
        str += GetBuildDate();
        str += _T(":PocketPC");
        str += _T("\x01");
        
        SendIRCMessage(str.LockBuffer());
      } else if (ctcpcmd==_T("PING")) {

        CString str;
        str += _T("NOTICE ");
        str += hmMask.szNick;
        str += _T(" :\x01");
        str += _T("PING ");
        str += stMsg.GetSlice(_T(' '),1);
        str += _T("\x01");

        SendIRCMessage(str.LockBuffer());

        str = "";
        str += _T("[");
        str += hmMask.szNick;
        str += _T(" PING]");
        
        wndStatus->AddLine(str.LockBuffer(),SETUP.cAction);

      } else {
        CString str;
        str += _T("[");
        str += hmMask.szNick;
        str += _T(" ");
        str += ctcpcmd;
        str += _T("] ");
        str += stMsg.GetRightSlice(_T(' '));
        
        wndStatus->AddLine(str.LockBuffer(),SETUP.cAction);
      }
    } else {
      zsIRC_Wnd * wnd = FindWindow(recipient.LockBuffer());
      CString s = zsIRC_Format::Say(hmMask.szNick,message);
      if (wnd) {
        if (wnd->nType==ZSIRC_WT_CHANNEL) ((zsIRC_Wnd_Channel *)wnd)->AddLine(s.LockBuffer());
        if (wnd->nType==ZSIRC_WT_QUERY)   ((zsIRC_Wnd_Query   *)wnd)->AddLine(s.LockBuffer());
      } else {
        wnd = (zsIRC_Wnd_Query *) FindWindow(hmMask.szNick);
        if (!wnd)
          wnd = (zsIRC_Wnd_Query *) AddWindow(ZSIRC_WT_QUERY,hmMask.szNick);
        ((zsIRC_Wnd_Query *)wnd)->AddLine(s.LockBuffer());
      }
    }
  }
  else if (command==_T("NICK")) {
    CString newnick = stToken.GetRightSliceWithoutCollapse(_T(':'));
    if (newnick.GetLength()==0) {
      newnick = stToken.GetSlice(_T(' '),stToken.GetOccurrences(_T(' ')));
    }
    CString str = zsIRC_Format::Nick(hmMask.szNick,newnick);

    for (int i=0; i<daWindows.Num(); i++) {
      zsIRC_Wnd * wnd = (zsIRC_Wnd *)daWindows[i];
      if (wnd->nType==ZSIRC_WT_CHANNEL && ((zsIRC_Wnd_Channel *)wnd)->isOnChannel(hmMask.szNick)) {
        zsIRC_Wnd_Channel * wndC = ((zsIRC_Wnd_Channel *)wnd);
        wndC->AddLine(str.LockBuffer(),SETUP.cMode);
  
        ZSIRC_USER * user = wndC->GetUser(hmMask.szNick);
        if (user) {
          unsigned int nOldPref = user->nPrefixes;
          wndC->RemoveUser(hmMask.szNick);
          wndC->AddUser(newnick.LockBuffer());
          ZSIRC_USER * nuuser = wndC->GetUser(newnick.LockBuffer());
          if (nuuser) {
            nuuser->nPrefixes = nOldPref;
            wndC->RefreshUserList();
          }
        }
      }
      if (wnd->nType==ZSIRC_WT_QUERY && _tcsicmp(wnd->szCaption,hmMask.szNick)==0) {
        ((zsIRC_Wnd_Query   *)wnd)->AddLine(str.LockBuffer(),SETUP.cMode);
        wnd->SetCaption(newnick.LockBuffer());
      }
    }
    if (sNick == hmMask.szNick) {
      sNick = newnick;
    }
  }

  else if (command==_T("NOTICE")) {
    CString recipient = stToken.GetSlice(_T(' '),0);
    CString message = stToken.GetRightSliceWithoutCollapse(_T(':'));

    if (recipient==sNick) {
      CString str;
      if (_tcslen(hmMask.GetName())) {
        str += _T("-");
        str += hmMask.GetName();
        str += _T("- ");
      }
      str += message;
      wndStatus->AddLine(str.LockBuffer(),SETUP.cNotice);
    } else {
      CString str = _T("-");
      str += hmMask.GetName();
      str += _T(" -> ");
      str += recipient;
      str += _T("- ");
      str += message;
      zsIRC_Wnd * wnd = FindWindow(recipient.LockBuffer());
      if (wnd) {
        if (wnd->nType==ZSIRC_WT_CHANNEL) ((zsIRC_Wnd_Channel *)wnd)->AddLine(str.LockBuffer(),SETUP.cNotice);
        if (wnd->nType==ZSIRC_WT_QUERY)   ((zsIRC_Wnd_Query   *)wnd)->AddLine(str.LockBuffer(),SETUP.cNotice);
      } else {
        wndStatus->AddLine(str.LockBuffer(),SETUP.cNotice);
      }
    }
  } 
  
  else if (command==_T("KICK")) {
    CString channel = stToken.GetSlice(_T(' '),0);
    CString victim  = stToken.GetSlice(_T(' '),1);
    CString message = stToken.GetRightSliceWithoutCollapse(_T(':'));

    CString str = zsIRC_Format::Kick(hmMask.szNick,victim,message);

    zsIRC_Wnd * wnd = FindWindow(channel.LockBuffer());
    if (wnd) {
      if (wnd->nType==ZSIRC_WT_CHANNEL) {
        zsIRC_Wnd_Channel * wndC = (zsIRC_Wnd_Channel *)wnd;
        wndC->AddLine(str.LockBuffer(),SETUP.cMode);
        wndC->RemoveUser(victim.LockBuffer());
        if (victim == sNick) CloseWindow(channel.LockBuffer());
      }
    }
  } 

  else if (command==_T("TOPIC")) {
    CString channel = stToken.GetSlice(_T(' '),0);
    CString topic = stToken.GetRightSliceWithoutCollapse(_T(':'));

    CString str = zsIRC_Format::TopicChange(hmMask.szNick,topic);

    zsIRC_Wnd * wnd = FindWindow(channel.LockBuffer());
    if (wnd) {
      if (wnd->nType==ZSIRC_WT_CHANNEL) ((zsIRC_Wnd_Channel *)wnd)->AddLine(str.LockBuffer(),SETUP.cMode);
    }
  }

  else if (command==_T("ERROR")) {
    CString message = stToken.GetRightSliceWithoutCollapse(_T(':'));
    message = _T("ERROR: ") + message;
    wndStatus->AddLine(message.LockBuffer(),SETUP.cPart);
  } 
    
  else if (command==_T("PING")) {
    CString server = params.Mid(1);
    CString str = _T("PONG ");
    str += server;
    SendIRCMessage(str.LockBuffer());
  } 
  
  else if (command==_T("PONG")) {
    // we swallow this command for now
  } 
  
  else if (command==_T("INVITE")) {
    CString you = stToken.GetSlice(_T(' '),0);
    CString channel = stToken.GetSlice(_T(' '),1);
    
    CString s = zsIRC_Format::Invite(hmMask.szNick,channel);
    wndStatus->AddLine(s.LockBuffer(),SETUP.cMode);
  } 
  
  else {
    CString s = _T("Unknown command from server: ");
    s += command;
    wndStatus->AddLine(s.LockBuffer(),SETUP.cPart);
  }
  //////////////////////////////////////////////////////////////////////////
  ((zsIRC_Wnd*)daWindows[nSelected])->evEvent = ZSIRC_WH_NOTHING;
}

