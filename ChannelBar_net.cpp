// ChannelBar.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "ChannelBar.h"

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

/////////////////////////////////////////////////////////////////////////////
// CChannelBar message handlers

DWORD __stdcall threadproxy(LPVOID lpParam) {
  CChannelBar * t = ((CChannelBar*)lpParam);
  t->nThreadRunning = 1;
  t->nPackets = 0;
  t->sSocketLeftover = "";
  t->nDisconnect = 0;

  if (!t->Handshake()) {
    t->nThreadRunning = 0;
    t->SendMessage(WM_CBAR_DISCONNECT);
    return -1;
  }

  unsigned int lt = GetTickCount();
  while (t->nThreadActive) {
    t->HandleIncoming();
    if (GetTickCount() - lt > 30*1000) {
      lt = GetTickCount();
      t->CheckNotify();
    }
    Sleep(10);
  }

  t->nThreadRunning = 0;
  if (t->nDisconnect) 
    t->SendMessage(WM_CBAR_DISCONNECT);

  TRACE0("Leaving thread...\n");
  return 303;
}

int isValidEncoding(unsigned int nCodePage, char * szString, int nSize) {
  char * sa = new char[nSize];
  TCHAR * sw = new TCHAR[nSize];

  MultiByteToWideChar(nCodePage, NULL, szString, -1, sw, nSize);
  WideCharToMultiByte(nCodePage, NULL, sw,       -1, sa, nSize, NULL, NULL);

  int v = (strlen(szString) == strlen(sa));
  delete[] sa;
  delete[] sw;
  return v;
}

void CChannelBar::HandleIncoming() {
  if (!scSocket.hasData()) return;

  CString string = sSocketLeftover;
  char buffer[1024]={0};
  
  while (1) {
    ZeroMemory(buffer,1024);
    int nReturn = scSocket.Read(buffer,1023);

    if (nReturn==-1) {
      DWORD e = WSAGetLastError();
      TCHAR msg[2000];
      _sntprintf(msg,2000,_T("Connection lost while reading: %d"),e);
      wndDebug ->AddLine(msg,SETUP.cMode);
      wndStatus->AddLine(msg,SETUP.cMode);

      nThreadActive = 0;
      nDisconnect = 1;

      return; 
    }
#ifdef _UNICODE
    TCHAR pwBuffer[1024];
    ZeroMemory(pwBuffer,1024*sizeof(TCHAR));
    int defEnc = SETUP.nCodepage;
    
    if (!isValidEncoding(defEnc,buffer,1024)) defEnc = CP_ACP;

    MultiByteToWideChar(defEnc, NULL, buffer, -1, pwBuffer, 1024);
    string += pwBuffer;
#else
    string += buffer;
#endif
    if (nReturn<1023) break;
  }

  if (string.Right(1)!=_T("\n")) {
    int nPos = string.ReverseFind(_T('\n'));
    if (nPos==-1) {
      sSocketLeftover = string;
      return;
    } else {
      sSocketLeftover = string.Mid(nPos+1);
      string = string.Left(nPos+1);
    }
  } else {
    sSocketLeftover = "";
  }

  nPackets++;

  int nStart = 0;
  while (1) {
    int nPos = string.Find(_T("\n"),nStart);
    if (nPos<0) break;
    CString * substr = new CString(string.Mid(nStart,nPos-nStart));
    substr->TrimLeft();
    substr->TrimRight();
    daMessages.Add(substr);
    nStart = nPos+1;
  }
  SendMessage(WM_CBAR_CHECKQUEUE);
}

void CChannelBar::SendIRCMessage(TCHAR*msg) {

  if (!scSocket.isConnected()) {
    wndStatus->AddLine(_T("Not connected!"));
    return;
  }

  CString s = msg;

  CString s2 = _T("[>>] ");
  s2+=msg;
  wndDebug->AddLine(s2.LockBuffer(),RGB(0x80,0x80,0x80));

  if (s.Find(_T("\r\n"))==-1) s += _T("\r\n");

  char s8[2048];
#ifdef _UNICODE
  WideCharToMultiByte(SETUP.nCodepage, 0, s, -1, s8, 2048, NULL, NULL);
#else
  strncpy(s8,msg,2048);
#endif
    //AfxW2AHelper(s8,s,2048);
//  FILE * f = fopen("dump.raw","wb");
//  fwrite(s8,2048,1,f);
//  fclose(f);
  if (scSocket.Write(s8,strlen(s8))==SOCKET_ERROR) {
    DWORD e = WSAGetLastError();
    TCHAR msg[2000];
    _sntprintf(msg,2000,_T("Connection lost while writing: %d"),e);
    wndDebug ->AddLine(msg,SETUP.cMode);
    wndStatus->AddLine(msg,SETUP.cMode);

    Disconnect();
  }
  //s.UnlockBuffer();
}

void CChannelBar::OnCheckQueue(WPARAM wParam, LPARAM lParam) {
  int nMsg = daMessages.Num();
  for (int i=0; i<nMsg; i++) {
    CString * str = (CString *)daMessages[0];
    CString copy = *str;
    ParseIRCMessage(copy.LockBuffer());
    daMessages.Delete(0);
  }
}

void CChannelBar::OnDisconnect(WPARAM wParam, LPARAM lParam) {
  Disconnect();
}

void CChannelBar::Disconnect() {
  if (scSocket.isConnected()) {
    TCHAR t[] = _T("Disconnecting...");    
    wndDebug ->AddLine(t,SETUP.cMode);
    wndStatus->AddLine(t,SETUP.cMode);

//    CString s = _T("QUIT :");
//    s += SETUP.szQuitMessage;
//    SendIRCMessage(s.LockBuffer());

    nThreadActive = 0;

    int nWatchdog = 0;
    while(nThreadRunning) {
      if (nWatchdog++ > 500) {
        TCHAR t[] = _T("Thread stuck, terminating...");    
        wndDebug ->AddLine(t,SETUP.cMode);
        wndStatus->AddLine(t,SETUP.cMode);
        TerminateThread(hThread,1337);
        nThreadRunning = 0;
        break;
      }
      Sleep(10);
    }
    scSocket.Disconnect();
    CloseHandle(hThread);

//    daMessages.Flush();

    TCHAR t2[] = _T("Disconnected.");    
    wndDebug ->AddLine(t2,SETUP.cMode);
    wndStatus->AddLine(t2,SETUP.cMode);
  }

  if (GetParent() && GetParent()->GetParent()) {
    CMenu * m = GetParent()->GetParent()->GetMenu();

    TCHAR * t3 = _T("Connect");

    MENUITEMINFO mii;
    ZeroMemory(&mii,sizeof(MENUITEMINFO));
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_TYPE;
    mii.fType = MFT_STRING;
    mii.dwTypeData = (WORD*)t3;
    ::SetMenuItemInfo(m->m_hMenu,ID_MENU_CONNECT,FALSE,&mii);
  }

  if (!SETUP.nRejoinOnConnect) {
    while (1) {
      int nAgain = 0;
      for (int i=0; i<daWindows.Num(); i++)
        if ( ((zsIRC_Wnd*)daWindows[i])->nType == ZSIRC_WT_CHANNEL ) {
          CloseWindow( ((zsIRC_Wnd*)daWindows[i])->szCaption );
          nAgain = 1;
        }
      if (!nAgain) break;
    }
  }
}

void CChannelBar::Connect() {
  if (!nThreadRunning) {
    daMessages.Flush();

    CMenu * m = GetParent()->GetParent()->GetMenu();
    if (m) {
      TCHAR * t = _T("Disconnect");

      MENUITEMINFO mii;
      ZeroMemory(&mii,sizeof(MENUITEMINFO));
      mii.cbSize = sizeof(MENUITEMINFO);
      mii.fMask = MIIM_TYPE;
      mii.fType = MFT_STRING;
      mii.dwTypeData = (WORD*)t;
      ::SetMenuItemInfo(m->m_hMenu,ID_MENU_CONNECT,FALSE,&mii);
    }

    nThreadActive = 1;
    hThread = CreateThread(NULL, 0, threadproxy, (void*)this, 0, &hThreadID);
  }
}

//extern CChannelBar * g_ChannelBar;
void mylog(void * u, TCHAR * msg) {
  CChannelBar * g_ChannelBar = (CChannelBar *)u;
  g_ChannelBar->wndDebug ->AddLine(msg,SETUP.cMode);
  g_ChannelBar->wndStatus->AddLine(msg,SETUP.cMode);
}

int CChannelBar::Handshake() {
  char sname[2048];
  AfxW2AHelper(sname,SETUP.szServerName,2048);

  CString str;
  str.Format(_T("Connecting to %s:%d%s..."),SETUP.szServerName,SETUP.nServerPort,SETUP.nServerSecure > 0 ? _T(" (secure)") : _T(""));
  wndDebug ->AddLine(str.LockBuffer(),SETUP.cMode);
  wndStatus->AddLine(str.LockBuffer(),SETUP.cMode);

  if (!scSocket.Connect(sname,SETUP.nServerPort,mylog,this,SETUP.nServerSecure > 0)) {
/*
    DWORD e = WSAGetLastError();
    TCHAR msg[2000];
    _sntprintf(msg,2000,_T("Unable to connect: %d"),e);
    wndDebug ->AddLine(msg,SETUP.cMode);
    wndStatus->AddLine(msg,SETUP.cMode);
    //Disconnect();
//    SendMessage(WM_CBAR_DISCONNECT);
*/
    return 0;
  }

  str = _T("Connected.");
  wndDebug ->AddLine(str.LockBuffer(),SETUP.cMode);
  wndStatus->AddLine(str.LockBuffer(),SETUP.cMode);

  // pass
  if (_tcslen(SETUP.szServerPass)) {
    CString p = _T("PASS ");
    p += SETUP.szServerPass;
    SendIRCMessage(p.LockBuffer());
  }

  // nick
  CString nick = _T("NICK ");
  nick += sNick;
  SendIRCMessage(nick.LockBuffer());

  // user
  CString user = _T("USER ");
  user += SETUP.szIdent;
  user += _T(" \"dummy\" ");

  user += _T("\""); 
  user += SETUP.szServerName;
  user +=  _T("\"");
  
  user += _T(" :");
  user += SETUP.szRealname;
  SendIRCMessage(user.LockBuffer());


  nAltNickNumber = 0;

  return 1;
}