// zsIRC_Hostmask.cpp: implementation of the zsIRC_Hostmask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zsIRC.h"
#include "zsIRC_Hostmask.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

zsIRC_Hostmask::zsIRC_Hostmask(TCHAR * szHostmask)
{
  ZeroMemory(szNick,100*sizeof(TCHAR));
  ZeroMemory(szUser,100*sizeof(TCHAR));
  ZeroMemory(szHost,1024*sizeof(TCHAR));

  CString sHostmask = szHostmask;

  if (sHostmask.Find(_T('!'))==-1 && sHostmask.Find(_T('@'))==-1) {
    if (_tcsrchr(szHostmask,_T('.')))
      _tcsncpy(szHost,szHostmask,1024);
    else
      _tcsncpy(szNick,szHostmask,1024);
  } else {
    int nEMPos = sHostmask.Find(_T('!'));
    int nAtPos = sHostmask.Find(_T('@'));

    CString t;
    
    t = sHostmask.Left(nEMPos);
    _tcsncpy(szNick,t.LockBuffer(),100);

    t = sHostmask.Mid(nEMPos+1,nAtPos-nEMPos-1);
    _tcsncpy(szUser,t.LockBuffer(),100);

    t = sHostmask.Right(sHostmask.GetLength()-nAtPos-1);
    _tcsncpy(szHost,t.LockBuffer(),1024);
        
  }
}

zsIRC_Hostmask::~zsIRC_Hostmask()
{

}


CString zsIRC_Hostmask::GetShortHost() {
  CString str;
  str += szUser;
  str += _T("@");
  str += szHost;
  return str;
}

TCHAR * zsIRC_Hostmask::GetName() {
  if (_tcslen(szNick)) return szNick;
  return szHost;
}