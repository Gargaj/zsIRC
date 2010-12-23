// zsIRC.h : main header file for the ZSIRC application
//

#if !defined(AFX_ZSIRC_H__690687A3_F8F0_4F2D_A1B7_BC763123CA49__INCLUDED_)
#define AFX_ZSIRC_H__690687A3_F8F0_4F2D_A1B7_BC763123CA49__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CZsIRCApp:
// See zsIRC.cpp for the implementation of this class
//

CString GetBuildDate();

class CZsIRCApp : public CWinApp
{
public:
	CZsIRCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZsIRCApp)
	public: 
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CZsIRCApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CZsIRCApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZSIRC_H__690687A3_F8F0_4F2D_A1B7_BC763123CA49__INCLUDED_)
