// zsIRC_EventHandler.cpp: implementation of the zsIRC_EventHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChildView.h"
#include "zsIRC.h"
#include "zsIRC_EventHandler.h"
#include "zsIRC_StringTokenizer.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

zsIRC_EventHandler::zsIRC_EventHandler(CChannelBar * c)
{
  m_ChannelBar = c;
}

zsIRC_EventHandler::~zsIRC_EventHandler()
{

}

void zsIRC_EventHandler::Load()
{
  TCHAR buffer[MAX_PATH];
  GetModuleFileName(NULL,buffer,MAX_PATH);

  CString path = buffer;
  int nPos = path.ReverseFind(_T('\\'));

  if (nPos==-1) path = "";
  else path = path.Left(nPos);

  path += "\\zsIRC_events.ini";

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
    
    ZSIRC_EVENTTYPE event = ZSIRC_EVENT_NONE;
    int n = stFile.GetOccurrences(_T('\n'));
    for (int i=0; i<=n; i++) {
      CString line = stFile.GetSlice(_T('\n'),i);
      line.TrimLeft();
      line.TrimRight();
      if (line.CompareNoCase(_T("[Connect]"))==0)
        event = ZSIRC_EVENT_CONNECT;
      else if (line.CompareNoCase(_T("[Join]"))==0)
        event = ZSIRC_EVENT_JOIN;
      else {
        if (line.Find(_T(';')) >= 0) {
          line.SetAt( line.Find(_T(';')), _T('\0') );
          line.TrimRight();
        }
        sEventCommands[event] += line + _T("\r\n");
      }
    }
  }
}

void zsIRC_EventHandler::CommitCommands( ZSIRC_EVENTTYPE event )
{
  zsIRC_StringTokenizer token(sEventCommands[event]);
  for (int i=0; i<=token.GetOccurrences(_T('\n')); i++) {
    CString slice = token.GetSlice(_T('\n'),i);
    slice.TrimLeft();
    slice.TrimRight();
    if (slice.GetLength())
      m_ChannelBar->ParseCommand(slice.LockBuffer());
  }
}