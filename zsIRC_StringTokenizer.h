// zsIRC_StringTokenizer.h: interface for the zsIRC_StringTokenizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZSIRC_STRINGTOKENIZER_H__204BDF58_26A3_4F21_9A8A_7E40E6C395CD__INCLUDED_)
#define AFX_ZSIRC_STRINGTOKENIZER_H__204BDF58_26A3_4F21_9A8A_7E40E6C395CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class zsIRC_StringTokenizer  
{
public:
	zsIRC_StringTokenizer(CString s);
	virtual ~zsIRC_StringTokenizer();

  int GetOccurrences(TCHAR);
  CString Collapse(TCHAR);
  CString GetSlice(TCHAR,int);
  CString GetRightSlice(TCHAR);
  CString GetRightSliceWithoutCollapse(TCHAR cChar);
  CString GetRightSlice(TCHAR,int);

private:
  CString m_sData;
};

#endif // !defined(AFX_ZSIRC_STRINGTOKENIZER_H__204BDF58_26A3_4F21_9A8A_7E40E6C395CD__INCLUDED_)
