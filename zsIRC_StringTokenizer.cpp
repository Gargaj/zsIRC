// zsIRC_StringTokenizer.cpp: implementation of the zsIRC_StringTokenizer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zsIRC.h"
#include "zsIRC_StringTokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

zsIRC_StringTokenizer::zsIRC_StringTokenizer(CString s)
{
  m_sData = s;
}

zsIRC_StringTokenizer::~zsIRC_StringTokenizer()
{

}

int zsIRC_StringTokenizer::GetOccurrences(TCHAR cChar) {
  CString sLocal = Collapse(cChar);

  int n=0;
  for (int i=0; i<sLocal.GetLength(); i++)
    if (sLocal.GetAt(i)==cChar) n++;
  
  return n;
}

CString zsIRC_StringTokenizer::Collapse(TCHAR cChar) {
  CString original = m_sData;
  CString edited   = m_sData;
  CString oldchunk = cChar;
  oldchunk += cChar;
  CString newchunk = cChar;

  while (1) {
    CString before = edited;
    edited.Replace(oldchunk,newchunk);
    if (before==edited) break;
  }
  return edited;
}

CString zsIRC_StringTokenizer::GetSlice(TCHAR cChar,int nPiece) {
  CString s="";

  CString sLocal = Collapse(cChar);

  int nOcc = GetOccurrences(cChar);

  if (nOcc==0) {
    CString s2 = m_sData;
    return s2;
  }
  if (nPiece<0) return s;
  if (nPiece>nOcc) return s;

  if (nPiece==0) return m_sData.Left(sLocal.Find(cChar));
  if (nPiece==nOcc) return m_sData.Right(sLocal.GetLength() - sLocal.ReverseFind(cChar) - 1);
  
  int nStart = 0;
  for (int i=0; i<nPiece; i++) nStart = sLocal.Find(cChar,nStart) + 1;

  return sLocal.Mid(nStart, sLocal.Find(cChar,nStart+1) - nStart);
}

CString zsIRC_StringTokenizer::GetRightSlice(TCHAR cChar) {
  CString s="";

  CString sLocal = Collapse(cChar);

  int nPos = sLocal.Find(cChar);
  if (nPos == -1) return s;
  else return sLocal.Mid(nPos+1);
}

CString zsIRC_StringTokenizer::GetRightSliceWithoutCollapse(TCHAR cChar) {
  CString s="";

  CString sLocal = m_sData;

  int nPos = sLocal.Find(cChar);
  if (nPos == -1) return s;
  else return sLocal.Mid(nPos+1);
}

CString zsIRC_StringTokenizer::GetRightSlice(TCHAR cChar, int num) {
  CString s="";

  CString sLocal = Collapse(cChar);

  if (num>=GetOccurrences(cChar))
    return s;

  int nPos = 0;
  for (int i=0; i<num+1; i++) {
    if (nPos == -1) return s;
    nPos = sLocal.Find(cChar,nPos)+1;
  }
  return sLocal.Mid(nPos);
}
