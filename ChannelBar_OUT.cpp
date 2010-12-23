// ChannelBar.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "ChannelBar.h"
#include "ChildView.h"

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
#include "zsIRC_Format.h"
#include "MainFrm.h"

void CChannelBar::ParseCommand(TCHAR * szMessage, int nForce) {
  if (!_tcslen(szMessage) && !nForce) return;
  if (szMessage[0]==_T('/') && !nForce) {
    //////////////////////////////////////////////////////////////////////////
    // commands
    CString msg = szMessage;
    CString command;
    CString params;
    int nPos = msg.Find(_T(' '));
    if (nPos==-1) {
      command = msg.Right(msg.GetLength()-1);
    } else {
      command = msg.Mid(1,nPos-1);
      params  = msg.Mid(nPos+1,msg.GetLength()-nPos);
      params.TrimRight();
      params.TrimLeft();
    }
    command.MakeLower();

    zsIRC_StringTokenizer stToken(params);

    //////////////////////////////////////////////////////////////////////////
    if (command==_T("raw")) {
      SendIRCMessage(params.LockBuffer());
    }
    else if (command==_T("connect")) {
      Connect();
    }
    else if (command==_T("disconnect")) {
      Disconnect();
    }
    else if (command==_T("server")) {
      Disconnect();
      CString host = stToken.GetSlice(_T(' '),0);
      CString port = stToken.GetSlice(_T(' '),1);
      if (stToken.GetOccurrences(_T(' '))==0) 
        SETUP.nServerPort = 6667;
      else
        _stscanf(port.LockBuffer(),_T("%d"),&SETUP.nServerPort);
      _tcsncpy(SETUP.szServerName,host.LockBuffer(),1024);
      SETUP.nServerSecure = 0;
      Connect();
    }
    else if (command==_T("newserver")) {
      ((CMainFrame*)theApp.m_pMainWnd)->m_wndView.AddNewChannelbar();
    }
    else if (command==_T("serversecure")) {
      Disconnect();
      CString host = stToken.GetSlice(_T(' '),0);
      CString port = stToken.GetSlice(_T(' '),1);
      if (stToken.GetOccurrences(_T(' '))==0) 
        SETUP.nServerPort = 6667;
      else
        _stscanf(port.LockBuffer(),_T("%d"),&SETUP.nServerPort);
      _tcsncpy(SETUP.szServerName,host.LockBuffer(),1024);
      SETUP.nServerSecure = 1;
      Connect();
    }
    else if (command==_T("join") || command==_T("j")) {
      CString s;
      s = _T("JOIN ");
      s += stToken.GetSlice(_T(' '),0);
      if (stToken.GetOccurrences(_T(' '))>=1) {
        // password
        s += _T(" ");
        s += stToken.GetSlice(_T(' '),1);
      }
      SendIRCMessage(s.LockBuffer());
    }
    else if (command==_T("query") || command==_T("q")) {
      CString dude = stToken.GetSlice(_T(' '),0);
      AddWindow(ZSIRC_WT_QUERY,dude.LockBuffer());
    }
    else if (command==_T("nick")) {
      CString s;
      s = _T("NICK ");
      s += stToken.GetSlice(_T(' '),0);
      SendIRCMessage(s.LockBuffer());
    }
    else if (command==_T("me")) {
      zsIRC_Wnd * wnd = (zsIRC_Wnd*)daWindows[nSelected];

      if (wnd->nType == ZSIRC_WT_CHANNEL || wnd->nType == ZSIRC_WT_QUERY) {
        CString s;
        s = _T("PRIVMSG ");

        s += wnd->szCaption;
        s += _T(" :");
        s += _T("\x01");
        s += _T("ACTION ");
        s += params;
        s += _T("\x01");
        SendIRCMessage(s.LockBuffer());
      
        CString msg = zsIRC_Format::Act(sNick,params);

        if (wnd->nType==ZSIRC_WT_CHANNEL) ((zsIRC_Wnd_Channel *)wnd)->AddLine(msg.LockBuffer(),SETUP.cAction);
        if (wnd->nType==ZSIRC_WT_QUERY)   ((zsIRC_Wnd_Query   *)wnd)->AddLine(msg.LockBuffer(),SETUP.cAction);
      }
    }
    else if (command==_T("debug")) {
      SETUP.nShowDebugWindow = !SETUP.nShowDebugWindow;
      InvalidateRect(NULL);
    }
    else if (command==_T("wintitle")) {
      theApp.m_pMainWnd->SetWindowText(params);
    }
    else if (command==_T("hallo")) {
      wndStatus->AddLine(_T("was?"));
    }
    else if (command==_T("mode")) {
      //wndStatus->AddLine(_T("was?"));
      CString s;
      s = _T("MODE ");
      if (params.GetLength()<=0) return;
      if ((params[0]==_T('+') || params[0]==_T('-')) && ((zsIRC_Wnd*)daWindows[nSelected])->nType == ZSIRC_WT_CHANNEL) {
        s += ((zsIRC_Wnd*)daWindows[nSelected])->szCaption;
        s += _T(" ");
      }
      s += params;
      SendIRCMessage(s.LockBuffer());

    }
    else if (command==_T("part")) {
      if (params.GetLength()>0 && sChanTypes.Find(params[0])!=-1) {
        CString s;
        s = _T("PART ");
        s += stToken.GetSlice(_T(' '),0);
        s += _T(" :");
        s += stToken.GetRightSlice(_T(' '));
        SendIRCMessage(s.LockBuffer());
      } else {
        if ( ((zsIRC_Wnd*)daWindows[nSelected])->nType == ZSIRC_WT_CHANNEL ) {
          CString s;
          s = _T("PART ");
          s += ((zsIRC_Wnd*)daWindows[nSelected])->szCaption;
          s += _T(" :");
          s += params;
          SendIRCMessage(s.LockBuffer());
        }
      }
    }
    else if (command==_T("whois") || command==_T("w")) {
      CString s;
      s = _T("WHOIS ");
      s += params;
      SendIRCMessage(s.LockBuffer());
    }
    else if (command==_T("msg")) {
      CString s;
      s = _T("PRIVMSG ");
      s += stToken.GetSlice(_T(' '),0);
      s += _T(" :");
      s += stToken.GetRightSlice(_T(' '));
      SendIRCMessage(s.LockBuffer());
    }
    else if (command==_T("set")) {
      CString key    = stToken.GetSlice(_T(' '),0);
      CString value = stToken.GetRightSlice(_T(' '));
      for (ZSIRC_SETTING * s = g_pZsircSettings; s->szName; s++) {
        if (key.CompareNoCase(s->szName)==0) {
          switch (s->nType) {
            case ZSIRC_ST_INT: {
              _stscanf(value.LockBuffer(),_T("%d"),s->pValue);
            } break;
            case ZSIRC_ST_STRING: {
              _tcsnccpy((TCHAR*)s->pValue,value.LockBuffer(),1024);
            } break;
            case ZSIRC_ST_COLOR: {
              _stscanf(value.LockBuffer(),_T("%X"),s->pValue);
            } break;
          }
          wndStatus->AddLine((CString(_T("Value '"))+key+_T("' set to '")+value+_T("'")).LockBuffer(),SETUP.cMode);
        }
      }
      for (int i=0; i<daWindows.Num(); i++) {  
        zsIRC_Wnd * wnd = (zsIRC_Wnd *)daWindows[i];
        wnd->SendMessage(WM_SIZE);
      }
      GetParent()->SendMessage(WM_SIZE);
    }
    else if (command==_T("quit")) {
      CString s;
      s = _T("QUIT :");
      s += params;
      SendIRCMessage(s.LockBuffer());
    }
    else if (command==_T("notify")) {
      CString duud = stToken.GetSlice(_T(' '),0);

      ZSIRC_USER * u = new ZSIRC_USER;
      _tcsncpy(u->sNick,duud.LockBuffer(),256);
      u->bOnline = 0;
      daNotify.Add(u);

      CString s;
      s += _T("* Added ");
      s += duud;
      s += _T(" to notify list");
      wndStatus->AddLine(s.LockBuffer(),SETUP.cMode);
    }
    else if (command==_T("unnotify")) {
      CString duud = stToken.GetSlice(_T(' '),0);

      for (int i=0; i<daNotify.Num(); i++) 
        if ( duud.CompareNoCase(((ZSIRC_USER*)daNotify[i])->sNick)==0 ) {
          daNotify.Delete(i);

          CString s;
          s += _T("* Removed ");
          s += duud;
          s += _T(" from notify list");
          wndStatus->AddLine(s.LockBuffer(),SETUP.cMode);

          return;
        }
    }
    else if (command==_T("invite")) {
      CString duud    = stToken.GetSlice(_T(' '),0);
      CString channel = stToken.GetSlice(_T(' '),1);

      CString s=_T("INVITE ");
      s += duud;
      s += _T(" ");
      s += channel;

      ParseIRCMessage(s.LockBuffer());
    }
    else {
      if (SETUP.nPassCommandsRaw) {
        SendIRCMessage(msg.Mid(1).LockBuffer());        
      } else {
        CString m = _T("Command not found: ");
        m += command;
        wndStatus->AddLine(m.LockBuffer());
      }
    }

    //////////////////////////////////////////////////////////////////////////

  } else {
    //////////////////////////////////////////////////////////////////////////
    // say
    zsIRC_Wnd * wnd = (zsIRC_Wnd*)daWindows[nSelected];

    if (wnd->nType == ZSIRC_WT_CHANNEL || wnd->nType == ZSIRC_WT_QUERY) {
      CString s;
      s = _T("PRIVMSG ");

      s += wnd->szCaption;
      s += _T(" :");
      s += szMessage;
      SendIRCMessage(s.LockBuffer());

      s = zsIRC_Format::Say(sNick,szMessage);
      
      if (wnd->nType==ZSIRC_WT_CHANNEL) ((zsIRC_Wnd_Channel *)wnd)->AddLine(s.LockBuffer());
      if (wnd->nType==ZSIRC_WT_QUERY)   ((zsIRC_Wnd_Query   *)wnd)->AddLine(s.LockBuffer());
    }
    //((zsIRC_Wnd*)daWindows[nSelected])->evEvent = ZSIRC_WH_NOTHING;
  }
  //////////////////////////////////////////////////////////////////////////
  ((zsIRC_Wnd*)daWindows[nSelected])->evEvent = ZSIRC_WH_NOTHING;
}
