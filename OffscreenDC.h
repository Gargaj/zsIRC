// OffscreenDC.h: interface for the COffscreenDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OFFSCREENDC_H__18B6FBA7_2A6C_4C34_B608_8F30276F7BE0__INCLUDED_)
#define AFX_OFFSCREENDC_H__18B6FBA7_2A6C_4C34_B608_8F30276F7BE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COffscreenDC  
{
public:
	COffscreenDC(CWnd * pWnd,CRect r, int inv = 0);
	virtual ~COffscreenDC();
  CPaintDC * GetDC();
private:
  int nInvalidate;
  CWnd * pWnd;
  CPaintDC * pdc;
  CPaintDC * dc;
	HDC hdcMem;
  CRect rWnd;
	HBITMAP hbmMem,hbmOld;
};

#endif // !defined(AFX_OFFSCREENDC_H__18B6FBA7_2A6C_4C34_B608_8F30276F7BE0__INCLUDED_)
