// SideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "SideDlg.h"

#include "MdlGlobs.h"

#include "KeyFuncs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSideToolDlg dialog


CSideToolDlg::CSideToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSideToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSideToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSideToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSideToolDlg)
	DDX_Control(pDX, IDC_PREVFRAMEBUT, m_PrevFrameBut);
	DDX_Control(pDX, IDC_NEXTFRAMEBUT, m_NextFrameBut);
	DDX_Control(pDX, IDC_FRAMENUMEDIT, m_FrameNumEdit);
	DDX_Control(pDX, IDC_FRAMENAMEEDIT, m_FrameNameEdit);
	DDX_Control(pDX, IDC_FITSELECTEDBUT, m_FitSelectedBut);
	DDX_Control(pDX, IDC_FITALLBUT, m_FitAllBut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSideToolDlg, CDialog)
	//{{AFX_MSG_MAP(CSideToolDlg)
	ON_BN_CLICKED(IDC_FITALLBUT, OnFitAllBut)
	ON_BN_CLICKED(IDC_FITSELECTEDBUT, OnFitSelectedBut)
	ON_BN_CLICKED(IDC_NEXTFRAMEBUT, OnNextFrameBut)
	ON_BN_CLICKED(IDC_PREVFRAMEBUT, OnPrevFrameBut)
	ON_BN_DOUBLECLICKED(IDC_NEXTFRAMEBUT, OnDoubleclickedNextframebut)
	ON_BN_DOUBLECLICKED(IDC_PREVFRAMEBUT, OnDoubleclickedPrevframebut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSideToolDlg message handlers

int CSideToolDlg::GetWidth()
{
   RECT  Rect;

   GetWindowRect(&Rect);
   return Rect.right-Rect.left;
}

void CSideToolDlg::OnFitAllBut() 
{
   KControlFitAll();   
}

void CSideToolDlg::OnFitSelectedBut() 
{
   KControlFitSelected();	
}

void CSideToolDlg::OnNextFrameBut() 
{
	KControlNextFrame();
}

void CSideToolDlg::OnPrevFrameBut() 
{
   KControlPrevFrame();	
}


void CSideToolDlg::OnDoubleclickedNextframebut() 
{
   KControlLastFrame();
}

void CSideToolDlg::OnDoubleclickedPrevframebut() 
{
   KControlFirstFrame();
   
}

BOOL CSideToolDlg::PreTranslateMessage(MSG* pMsg) 
{
   if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
   {
      LONG  WndStyle;

      WndStyle = GetWindowLong(pMsg->hwnd, GWL_STYLE);
      if (WndStyle & ES_AUTOHSCROLL)
      {
         // we have an edit box
         if (pMsg->message == WM_KEYDOWN)
         {
            if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
            {
               GetParentOwner()->SetFocus();
               return 1;
            }
         }
      }
      else
      return GetParentOwner()->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
   }
	
	return CDialog::PreTranslateMessage(pMsg);
}
