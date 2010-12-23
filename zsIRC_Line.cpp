// zsIRC_Line.cpp: implementation of the zsIRC_Line class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zsIRC.h"
#include "zsIRC_Line.h"
#include "zsIRC_Settings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

zsIRC_Line::zsIRC_Line()
{
  
}

zsIRC_Line::zsIRC_Line(TCHAR * szMessage, COLORREF cColor)
{
  GetLocalTime(&tTimestamp);
  //this->szSender = szSender;
  this->szMessage = szMessage;
  this->cColor = cColor;
}

zsIRC_Line::~zsIRC_Line()
{

}

CString zsIRC_Line::toString()
{
  CString rv;

  if (SETUP.nDoTimestamp) {
    TCHAR sTemp[1024];
    _sntprintf(sTemp,1024,_T("[%02d:%02d:%02d]"),tTimestamp.wHour,tTimestamp.wMinute,tTimestamp.wSecond);
    rv += sTemp;
    rv += _T(" ");
  }
/*
  if (szSender.GetLength()) {
    rv += _T("<");
    rv += szSender;
    rv += _T("> ");
  }
*/

  rv += szMessage;

  return rv;
} 

void PrintToLogFile(TCHAR * szFilename,TCHAR * szMessage) {
  if (!SETUP.nDoLogging) return;

  CString sInvalidChars = _T("\\/?*:|\"'<>");
  
  CString sSolidFilename = szFilename;
  
  for (int i=0; i<sSolidFilename.GetLength(); i++)
    if (sInvalidChars.Find(sSolidFilename[i]) >= 0)
      sSolidFilename.SetAt(i,_T('_'));

  TCHAR buffer[1000];
  GetModuleFileName(NULL,buffer,1000);

  CString path = buffer;
  int nPos = path.ReverseFind(_T('\\'));

  if (nPos==-1) path = "";
  else path = path.Left(nPos);
  
  path += _T("\\");
  path += SETUP.szLogDirectory;

  CreateDirectory(path.LockBuffer(),NULL);

  path += _T("\\");
  path += sSolidFilename;

  HANDLE hFile = CreateFile(path.LockBuffer(),GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,NULL,NULL);
  if (hFile!=INVALID_HANDLE_VALUE) {
    DWORD nWritten;

    char s8[2048];
    //AfxW2AHelper(s,szMessage,2048);
    WideCharToMultiByte(SETUP.nCodepage, 0, szMessage, -1, s8, 2048, NULL, NULL);
    
    SetFilePointer(hFile,NULL,NULL,FILE_END);
    WriteFile(hFile,s8,strlen(s8),&nWritten,NULL);
    CloseHandle(hFile);
  }
}

void PrintToLogFileF(TCHAR * szFilename,TCHAR * szFormat, ...) {
  TCHAR text[2048];
  va_list ap;

  va_start(ap, szFormat);
  _vsntprintf(text, 2048, szFormat, ap);
  va_end(ap);

  PrintToLogFile(szFilename,text);
}

void zsIRC_Line::LogToFile(TCHAR * szFilename)
{
  CString rv;

  if (SETUP.nDoLogTimestamp) {
    TCHAR sTemp[1024];
    _sntprintf(sTemp,1024,_T("[%02d:%02d:%02d]"),tTimestamp.wHour,tTimestamp.wMinute,tTimestamp.wSecond);
    rv += sTemp;
    rv += _T(" ");
  }
/*
  if (szSender.GetLength()) {
    rv += _T("<");
    rv += szSender;
    rv += _T("> ");
  }
*/
  rv += szMessage;
  rv += _T("\r\n");

  PrintToLogFile(szFilename,rv.LockBuffer());
}
