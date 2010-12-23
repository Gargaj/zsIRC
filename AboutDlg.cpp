// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zsIRC.h"
#include "AboutDlg.h"
#include "OffscreenDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_BUILD, m_Build);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CANCELMODE()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

#include "spinobj.h"

typedef struct {
	float x,y,z;
} V3D;

V3D rotate3d(V3D v, float x, float y, float z) {
	V3D w,q;

  q.y=v.y*cos(x)+v.z*sin(x);
  q.z=v.z*cos(x)-v.y*sin(x);
  w=q;

  q.x=v.x*cos(y)+w.z*sin(y);
  q.z=w.z*cos(y)-v.x*sin(y);
  w=q;

  q.x=w.x*cos(z)+w.y*sin(z);
  q.y=w.y*cos(z)-w.x*sin(z);
  w=q;

	return w;
}

void CAboutDlg::OnPaint() 
{
  if (nHidden) {	
    CRect fxbox(30,110,210,290);  
    COffscreenDC odc(this,fxbox,1);
    odc.GetDC()->FillSolidRect(0,0,fxbox.Width(),fxbox.Height(),GetSysColor(COLOR_WINDOW));

    int w = fxbox.Width();
    int h = fxbox.Height();
    V3D verts[NUMVERT];
    for (int n=0; n<NUMVERT; n++) {
      V3D a = {spinnyobject[n*3+0],spinnyobject[n*3+1],spinnyobject[n*3+2]};
      verts[n] = rotate3d(a,GetTickCount()/600.0f,GetTickCount()/870.0f,0);
      //verts[n] = rotate3d(a,GetTickCount()/1200.0f,0,0);
    }
    for (n=0; n<NUMEDGE; n++) {
      V3D * a = verts+edges[n*2+0];
      V3D * b = verts+edges[n*2+1];    

      int size = 30000;
      int ww = 50;
      int px=(ww*a->x)/(size+a->z)+w/2;
      int py=(ww*a->y)/(size+a->z)+h/2;
      int qx=(ww*b->x)/(size+b->z)+w/2;
      int qy=(ww*b->y)/(size+b->z)+h/2;

      odc.GetDC()->MoveTo(px,py);
      odc.GetDC()->LineTo(qx,qy);
    }
  } else {
	  CPaintDC dc(this);
  }

	// Do not call CDialog::OnPaint() for painting messages
}

int CAboutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CAboutDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  CString sDate = _T("Build version: ") + GetBuildDate();

  m_Build.SetWindowText(sDate.LockBuffer());	
	
  ZeroMemory(szCode,4*sizeof(TCHAR));
  nHidden = 0;

  CPaintDC dc(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
  szCode[0] = szCode[1];
  szCode[1] = szCode[2];
  szCode[2] = nChar;

  if (_tcsicmp(szCode,_T("cns"))==0) {
    nHidden = 1;
    InvalidateRect(NULL);
  }
	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}
