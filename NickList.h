#if !defined(AFX_NICKLIST_H__D8D0DC85_B173_496E_B547_55DFB6187BC4__INCLUDED_)
#define AFX_NICKLIST_H__D8D0DC85_B173_496E_B547_55DFB6187BC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NickList.h : header file
//

#include "channelbar.h"

/////////////////////////////////////////////////////////////////////////////
// CNickList window

class CNickList : public CListBox
{
// Construction
public:
	CNickList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNickList)
public:
  virtual BOOL PreCreateWindow (CREATESTRUCT&);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetCurSelNick();
	virtual ~CNickList();

  int InsertStringSorted(TCHAR*);
  void Sort();
  void QuickSort(int first, int last);
  int Pivot( int first, int last );
  void Swap( int x, int y );
  int Compare(CString& lpszText1, CString& lpszText2);

  CChannelBar * GetChannelBar();

  void SetText(int,CString&);
  // Generated message map functions
protected:
	//{{AFX_MSG(CNickList)
	afx_msg void OnUserQuery();
	afx_msg void OnDblclk();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUserWhois();
	afx_msg void OnUserWhoisex();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NICKLIST_H__D8D0DC85_B173_496E_B547_55DFB6187BC4__INCLUDED_)
