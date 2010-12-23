// ColorButton.h: interface for the CColorButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORBUTTON_H__0727C2C3_3046_4409_BE31_35A8989DE2E1__INCLUDED_)
#define AFX_COLORBUTTON_H__0727C2C3_3046_4409_BE31_35A8989DE2E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CColorButton : public CButton  
{
public:
	CColorButton();
	virtual ~CColorButton();

  void SetColor(COLORREF);
  COLORREF GetColor();

private:
//	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  void OnPaint();
  void OnLButtonUp(UINT nFlags, CPoint point);

  COLORREF m_Color;

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_COLORBUTTON_H__0727C2C3_3046_4409_BE31_35A8989DE2E1__INCLUDED_)
