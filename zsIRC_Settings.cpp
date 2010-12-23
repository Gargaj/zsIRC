// zsIRC_Settings.cpp: implementation of the zsIRC_Settings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zsIRC.h"
#include "zsIRC_Settings.h"
#include "zsIRC_StringTokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZSIRC_SETTING g_pZsircSettings[] = {
  // connection
  { _T("Server"),        &SETUP.szServerName,     ZSIRC_ST_STRING },
  { _T("ServerPort"),    &SETUP.nServerPort,      ZSIRC_ST_INT },
  { _T("ServerSecure"),  &SETUP.nServerSecure,    ZSIRC_ST_INT },
  { _T("Ident"),         &SETUP.szIdent,          ZSIRC_ST_STRING },
  { _T("Realname"),      &SETUP.szRealname,       ZSIRC_ST_STRING },
  { _T("ServerPass"),    &SETUP.szServerPass,     ZSIRC_ST_STRING },
  { _T("ServerSecureBehaviour"), &SETUP.nServerSSLBehaviour, ZSIRC_ST_INT },

  { _T("Nick1"),         &SETUP.szDefaultNick[0], ZSIRC_ST_STRING },
  { _T("Nick2"),         &SETUP.szDefaultNick[1], ZSIRC_ST_STRING },
  { _T("Nick3"),         &SETUP.szDefaultNick[2], ZSIRC_ST_STRING },
  { _T("Nick4"),         &SETUP.szDefaultNick[3], ZSIRC_ST_STRING },
  { _T("Nick5"),         &SETUP.szDefaultNick[4], ZSIRC_ST_STRING },
  { _T("Nick6"),         &SETUP.szDefaultNick[5], ZSIRC_ST_STRING },
  { _T("Nick7"),         &SETUP.szDefaultNick[6], ZSIRC_ST_STRING },
  { _T("Nick8"),         &SETUP.szDefaultNick[7], ZSIRC_ST_STRING },
  { _T("Nick9"),         &SETUP.szDefaultNick[8], ZSIRC_ST_STRING },

  { _T("QuitMessage"),   &SETUP.szQuitMessage,    ZSIRC_ST_STRING },
  { _T("AutoConnect"),   &SETUP.nAutoConnect,     ZSIRC_ST_INT },
  { _T("RejoinOnConnect"),&SETUP.nRejoinOnConnect,ZSIRC_ST_INT },
  { _T("NotifyList"), &SETUP.szNotifyList,ZSIRC_ST_STRING },
  { _T("CheckForUpdates"), &SETUP.nCheckForUpdates,ZSIRC_ST_INT },
  
//  { _T("PopupNotify"), &SETUP.nPopupNotify,ZSIRC_ST_INT },

  // string formatting
  { _T("Timestamps"),    &SETUP.nDoTimestamp,     ZSIRC_ST_INT },
  { _T("Logging"),       &SETUP.nDoLogging,       ZSIRC_ST_INT },
  { _T("LogDirectory"),  &SETUP.szLogDirectory,   ZSIRC_ST_STRING },
  { _T("LogTimestamps"), &SETUP.nDoLogTimestamp,  ZSIRC_ST_INT},

  // appearance
  { _T("ButtonFont"),      &SETUP.szButtonFont,      ZSIRC_ST_STRING },
  { _T("ConversationFont"),&SETUP.szConversationFont,ZSIRC_ST_STRING },
  { _T("NicklistFont"),    &SETUP.szNicklistFont,    ZSIRC_ST_STRING },
  { _T("ButtonFontSize"),      &SETUP.nButtonFontSize,      ZSIRC_ST_INT },
  { _T("ConversationFontSize"),&SETUP.nConversationFontSize,ZSIRC_ST_INT },
  { _T("NicklistFontSize"),    &SETUP.nNicklistFontSize,    ZSIRC_ST_INT },

  // gui
  { _T("ChannelBarHeight"), &SETUP.nChannelBarHeight, ZSIRC_ST_INT },
  { _T("NicklistWidth"), &SETUP.nNicklistWidth, ZSIRC_ST_INT },
  { _T("SortButtons"), &SETUP.nSortButtons, ZSIRC_ST_INT },
  

  // colors
  { _T("BackgroundColor"), &SETUP.cBackgroundColor, ZSIRC_ST_COLOR },
  { _T("ForegroundColor"), &SETUP.cForegroundColor, ZSIRC_ST_COLOR },

  { _T("ActionColor"), &SETUP.cAction, ZSIRC_ST_COLOR },
  { _T("JoinColor"),   &SETUP.cJoin,   ZSIRC_ST_COLOR },
  { _T("ModeColor"),   &SETUP.cMode,   ZSIRC_ST_COLOR },
  { _T("PartColor"),   &SETUP.cPart,   ZSIRC_ST_COLOR },
  { _T("NoticeColor"), &SETUP.cNotice, ZSIRC_ST_COLOR },

  { _T("PassCommandsRaw"), &SETUP.nPassCommandsRaw, ZSIRC_ST_INT },
  { _T("StripColorCodes"), &SETUP.nStripColorCodes, ZSIRC_ST_INT },
  { _T("Codepage"), &SETUP.nCodepage, ZSIRC_ST_CODEPAGE },
  { _T("OffscreenBuffering"), &SETUP.nOffscreenBuffering, ZSIRC_ST_INT },

  { NULL, NULL, (ZSIRC_SETTING_TYPE)NULL },
};

ZSIRC_SETTING_CODEPAGE g_pZsircSettingsCodepages[] = {
  { CP_ACP, _T("acp") },
  { CP_OEMCP, _T("oem") },
  { CP_MACCP, _T("mac") },
  { CP_SYMBOL, _T("sym") },
  { CP_UTF7, _T("utf7") },
  { CP_UTF8, _T("utf8") },
  { NULL, NULL },
};

zsIRC_Settings::zsIRC_Settings() {

  for (ZSIRC_SETTING * s = g_pZsircSettings; s->szName; s++) {
    switch (s->nType) {
      case ZSIRC_ST_INT:      { *(int*)s->pValue = 0; } break;
      case ZSIRC_ST_COLOR:    { *(int*)s->pValue = 0; } break;
      case ZSIRC_ST_CODEPAGE: { *(int*)s->pValue = 0; } break;
      case ZSIRC_ST_STRING:   { ZeroMemory((TCHAR*)s->pValue,1024*sizeof(TCHAR)); } break;
    }
  }

  _tcsnccpy(szButtonFont,_T("Tahoma"),LF_FACESIZE);
  nButtonFontSize = 12;

  _tcsnccpy(szConversationFont,_T("Tahoma"),LF_FACESIZE);
  nConversationFontSize = 11;

  _tcsnccpy(szNicklistFont,_T("Tahoma"),LF_FACESIZE);
  nNicklistFontSize = 11;

  nChannelBarHeight = 15;

  cBackgroundColor = 0xFFFFFF;
  cForegroundColor = 0x000000;

  nAutoConnect = 0;
  nRejoinOnConnect = 1;

  nDoTimestamp = 0;
  nDoLogTimestamp = 1;
  nDoLogging = 1;
  nShowDebugWindow = 0;
  nShowHosts = 1;
  _tcsnccpy(szServerName,_T("us.ircnet.org"),1024);
  nServerPort = 6667;
  nStripColorCodes = 0;
  nCodepage = CP_ACP;
  nOffscreenBuffering = 0;
  
  _tcsnccpy(szLogDirectory,_T("logs"),1024);

  nNicklistWidth = 50;

  _tcsnccpy(szDefaultNick[0],_T("zsircuser"),1024);
  _tcsnccpy(szIdent,_T("zsirc"),1024);
  _tcsnccpy(szRealname,_T("This person is using zsIRC now, please tolerate."),1024);

  _tcsnccpy(szQuitMessage,_T("Standard zsIRC quit-message (meh)"),1024);

  cJoin   = RGB(0x00,0x80,0x00);
  cPart   = RGB(0x00,0x00,0xFF);
  cMode   = RGB(0x00,0x80,0x00);
  cAction = RGB(0x80,0x00,0x80);
  cNotice = RGB(0x00,0x00,0x80);

  nPopupNotify = 1;
}

zsIRC_Settings::~zsIRC_Settings()
{

}

void zsIRC_Settings::Load() {
  TCHAR buffer[MAX_PATH];
  GetModuleFileName(NULL,buffer,MAX_PATH);

  CString path = buffer;
  int nPos = path.ReverseFind(_T('\\'));

  if (nPos==-1) path = "";
  else path = path.Left(nPos);

  path += "\\zsIRC.ini";

  HANDLE hFile = CreateFile(path.LockBuffer(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
  if (hFile!=INVALID_HANDLE_VALUE) {
    int nSize = GetFileSize(hFile,NULL);
    char * buffer = new char[nSize+1];
    ZeroMemory(buffer,nSize+1);
    DWORD nBytes;
    ReadFile(hFile,buffer,nSize,&nBytes,NULL);
    CloseHandle(hFile);
    CString file = buffer;
    delete[] buffer;

    zsIRC_StringTokenizer stFile(file);
    
    int n = stFile.GetOccurrences(_T('\n'));
    for (int i=0; i<=n; i++) {
      CString line = stFile.GetSlice(_T('\n'),i);

      if (line.Find(_T('='))>=0 && line[0]!=_T(';')) {
        zsIRC_StringTokenizer stLine(line);
        CString key = stLine.GetSlice(_T('='),0);
        key.TrimLeft();
        key.TrimRight();
        CString value = stLine.GetRightSlice(_T('='));
        value.TrimLeft();
        value.TrimRight();

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
              case ZSIRC_ST_CODEPAGE: {
                value.MakeLower();
                for (ZSIRC_SETTING_CODEPAGE * c = g_pZsircSettingsCodepages; c->szName; c++)
                  if (value == c->szName)
                    *(int*)s->pValue = c->nCodepage;
/*
                if (value==_T("acp"))  SETUP.nCodepage = CP_ACP;
                if (value==_T("oem"))  SETUP.nCodepage = CP_OEMCP;
                if (value==_T("mac"))  SETUP.nCodepage = CP_MACCP;
                if (value==_T("sym"))  SETUP.nCodepage = CP_SYMBOL;
                if (value==_T("utf7")) SETUP.nCodepage = CP_UTF7;
                if (value==_T("utf8")) SETUP.nCodepage = CP_UTF8;
*/
              } break;
            }
          }
        }
      }

    }
  }

    
}

void zsIRC_Settings::Save() {
  TCHAR buffer[MAX_PATH];
  GetModuleFileName(NULL,buffer,MAX_PATH);

  CString path = buffer;
  int nPos = path.ReverseFind(_T('\\'));

  if (nPos==-1) path = "";
  else path = path.Left(nPos);

  path += "\\zsIRC.ini";

  CString bak = path;
  bak += ".bak";
  CopyFile(path.LockBuffer(),bak.LockBuffer(),FALSE);
  HANDLE hFile = CreateFile(path.LockBuffer(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
  if (hFile!=INVALID_HANDLE_VALUE) {
    int nSize = GetFileSize(hFile,NULL);
    char * buffer = new char[nSize+1];
    ZeroMemory(buffer,nSize+1);
    DWORD nBytes;
    ReadFile(hFile,buffer,nSize,&nBytes,NULL);
    CloseHandle(hFile);
    CString file = buffer;
    delete[] buffer;

    zsIRC_StringTokenizer stFile(file);

//    path += ".new";
    HANDLE hFileWrite = CreateFile(path.LockBuffer(),GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,NULL,NULL);

    int nSettings = 0;
    for (ZSIRC_SETTING * sa = g_pZsircSettings; sa->szName; sa++) nSettings++;
    
    int * set = new int[nSettings];
    ZeroMemory(set,nSettings*sizeof(int));
    
    int n = stFile.GetOccurrences(_T('\n'));
    for (int i=0; i<=n; i++) {
      CString line = stFile.GetSlice(_T('\n'),i);
      line.TrimRight();
      CString output;
      if (line.Find(_T('='))>=0 && line[0]!=_T(';')) {
        zsIRC_StringTokenizer stLine(line);
        CString key = stLine.GetSlice(_T('='),0);
        key.TrimLeft();
        key.TrimRight();
        CString value = stLine.GetRightSlice(_T('='));
        value.TrimLeft();
        value.TrimRight();

        int nSet = 0;
        for (ZSIRC_SETTING * s = g_pZsircSettings; s->szName; s++) {
          if (key.CompareNoCase(s->szName)==0) {
            output = key;
            output += _T(" = ");
            CString value;
            switch (s->nType) {
              case ZSIRC_ST_INT: {
                value.Format(_T("%d"),*(int*)s->pValue);
            	} break;
              case ZSIRC_ST_STRING: {
                value = (TCHAR*)s->pValue;
            	} break;
              case ZSIRC_ST_COLOR: {
                value.Format(_T("%06X"),*(unsigned int*)s->pValue);
            	} break;
              case ZSIRC_ST_CODEPAGE: {
                for (ZSIRC_SETTING_CODEPAGE * c = g_pZsircSettingsCodepages; c->szName; c++)
                  if (*(int*)s->pValue == c->nCodepage)
                    value = c->szName;
//                if(*(int*)s->pValue == CP_ACP)  value = _T("acp");
//                if(*(int*)s->pValue == CP_UTF8) value = _T("utf8");
            	} break;
            }
            output += value;
            set[nSet] = 1;
          }
          nSet++;
        }
      } else {
        output = line;
      }

      DWORD bytes;
      output += _T("\r\n");
      char * s8 = new char[output.GetLength()+1];
      ZeroMemory(s8,output.GetLength()+1);
      AfxW2AHelper(s8,output.LockBuffer(),output.GetLength()+1);
      //TRACE2("AfxW2AHelper caused %08X (%d)\n",GetLastError(),GetLastError());
      //WideCharToMultiByte(CP_ACP, 0, output.LockBuffer(), -1, s8, output.GetLength()+1, NULL, NULL)
      WriteFile(hFileWrite,s8,output.GetLength(),&bytes,NULL);
      delete[] s8;
    }

    // leftoverz
    int nSet = 0;
    for (ZSIRC_SETTING * s = g_pZsircSettings; s->szName; s++) {
      if (!set[nSet]) {
        CString output;
        output = s->szName;
        output += _T(" = ");
        CString value;
        switch (s->nType) {
          case ZSIRC_ST_INT: {
            value.Format(_T("%d"),*(int*)s->pValue);
          } break;
          case ZSIRC_ST_STRING: {
            value = (TCHAR*)s->pValue;
          } break;
          case ZSIRC_ST_COLOR: {
            value.Format(_T("%06X"),*(unsigned int*)s->pValue);
          } break;
          case ZSIRC_ST_CODEPAGE: {
            for (ZSIRC_SETTING_CODEPAGE * c = g_pZsircSettingsCodepages; c->szName; c++)
              if (*(int*)s->pValue == c->nCodepage)
                value = c->szName;
          } break;
        }
        if (value.GetLength() > 0) {
          output += value;

          DWORD bytes;
          output += _T("\r\n");
          char * s8 = new char[output.GetLength()+1];
          ZeroMemory(s8,output.GetLength()+1);
          AfxW2AHelper(s8,output.LockBuffer(),output.GetLength()+1);
          WriteFile(hFileWrite,s8,output.GetLength(),&bytes,NULL);
          delete[] s8;
        }
      }
      nSet++;
    }
    
    CloseHandle(hFileWrite);

    delete[] set;
  }

    
}

zsIRC_Settings SETUP;
