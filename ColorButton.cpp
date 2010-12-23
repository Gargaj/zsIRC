// ColorButton.cpp: implementation of the CColorButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zsIRC.h"
#include "ColorButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorButton::CColorButton() : m_Color(0xFFFF80FF)
{

}

CColorButton::~CColorButton()
{

}

BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
  ON_WM_PAINT()
  ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CColorButton::SetColor(COLORREF colFace)
{
	m_Color = colFace;
  if (::IsWindow(m_hWnd)) InvalidateRect(NULL);
}

COLORREF CColorButton::GetColor()
{
	return m_Color;
}

void CColorButton::OnPaint()
{
  CPaintDC dc(this);
    
	RECT rcButton;
  GetClientRect(&rcButton);

  CBrush b;
  b.CreateSolidBrush(m_Color);
  dc.FillRect(&rcButton,&b);
  dc.Draw3dRect(rcButton.left,rcButton.top,rcButton.right-rcButton.left,rcButton.bottom-rcButton.top,0,0);
}

typedef BOOL (APIENTRY * LPCHOOSECOLORFUNC) (LPCHOOSECOLOR);

void CColorButton::OnLButtonUp(UINT nFlags, CPoint point) {
  COLORREF savedcol[16];
  ZeroMemory(savedcol,16*sizeof(COLORREF));

  CHOOSECOLOR cc;
  cc.lStructSize = sizeof(CHOOSECOLOR);
  cc.hwndOwner = GetParent()->GetSafeHwnd();
  cc.lpCustColors = savedcol;
  cc.rgbResult = GetColor();
  cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
  HINSTANCE pCommonDialogDLL = LoadLibrary(_T("commdlg.dll"));
  if (pCommonDialogDLL) {
    LPCHOOSECOLORFUNC lpChooseColorFunc = (LPCHOOSECOLORFUNC)GetProcAddress(pCommonDialogDLL,_T("ChooseColor"));
    if (lpChooseColorFunc) {
      if (lpChooseColorFunc(&cc)) SetColor(cc.rgbResult);
    } else {
      MessageBox(_T("Sorry, something went really weird! o_O"));
    }
  } else {
    MessageBox(_T("Sorry, your device doesn't support common dialogs (Samsung and Motorola Q Smartphone?), so you have to get commdlg.dll from somewhere first!"));
  }
}