// zsIRC_Line.h: interface for the zsIRC_Line class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZSIRC_LINE_H__28651E00_5B00_4E47_AB41_37E363A4FA30__INCLUDED_)
#define AFX_ZSIRC_LINE_H__28651E00_5B00_4E47_AB41_37E363A4FA30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void PrintToLogFile(TCHAR*,TCHAR*);
void PrintToLogFileF(TCHAR * szFilename,TCHAR * szFormat, ...);

class zsIRC_Line  
{
public:
	zsIRC_Line();
	//zsIRC_Line(TCHAR * szSender, TCHAR * szMessage, COLORREF cColor);
  zsIRC_Line(TCHAR * szMessage, COLORREF cColor);
	virtual ~zsIRC_Line();

	CString toString();
	void LogToFile(TCHAR*);

	SYSTEMTIME tTimestamp;
  //CString szSender;
  CString szMessage;
  COLORREF cColor;
};

#endif // !defined(AFX_ZSIRC_LINE_H__28651E00_5B00_4E47_AB41_37E363A4FA30__INCLUDED_)
