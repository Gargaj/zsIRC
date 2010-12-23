// OffscreenDC.cpp: implementation of the COffscreenDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zsIRC.h"
#include "OffscreenDC.h"
#include "zsIRC_Settings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COffscreenDC::COffscreenDC(CWnd * p,CRect r, int inv)
{
  this->pWnd = p;
  dc = new CPaintDC(pWnd);
  if (SETUP.nOffscreenBuffering) {
  	pdc = new CPaintDC(pWnd);
    nInvalidate = inv;
    if (r.Width()==0)
      //pWnd->GetWindowRect(&rWnd);
      pWnd->GetClientRect(&rWnd);
    else
      rWnd = r;
	  hdcMem = CreateCompatibleDC(*pdc);
	  hbmMem = CreateCompatibleBitmap(*pdc,rWnd.Width(),rWnd.Height());
	  hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
	  dc->Detach();
	  dc->Attach(hdcMem);
  }
}

COffscreenDC::~COffscreenDC()
{
  if (SETUP.nOffscreenBuffering) {
    pdc->BitBlt(rWnd.left,rWnd.top,rWnd.Width(),rWnd.Height(),dc,0,0,SRCCOPY);

    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);

    if (nInvalidate) pWnd->InvalidateRect(NULL,FALSE);

    delete pdc;
  }
  delete dc;
}

CPaintDC * COffscreenDC::GetDC() {
  return dc;
}