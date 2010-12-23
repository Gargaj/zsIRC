// zsIRC_Hostmask.h: interface for the zsIRC_Hostmask class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZSIRC_HOSTMASK_H__0EE94EB0_0CFF_488F_B14D_4006987886DA__INCLUDED_)
#define AFX_ZSIRC_HOSTMASK_H__0EE94EB0_0CFF_488F_B14D_4006987886DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class zsIRC_Hostmask  
{
public:
	zsIRC_Hostmask(TCHAR*);
	virtual ~zsIRC_Hostmask();

  TCHAR szNick[100];
  TCHAR szUser[100];
  TCHAR szHost[1024];

  CString GetShortHost();
  TCHAR * GetName();
};

#endif // !defined(AFX_ZSIRC_HOSTMASK_H__0EE94EB0_0CFF_488F_B14D_4006987886DA__INCLUDED_)
