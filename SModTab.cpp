// SModTab.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "SModTab.h"

#include "MdlMsgs.h"
#include "MdlGlobs.h"
#include "KeyFuncs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinModifyTab dialog


CSkinModifyTab::CSkinModifyTab(CWnd* pParent /*=NULL*/)
	: CDialog(CSkinModifyTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkinModifyTab)
	//}}AFX_DATA_INIT
}


void CSkinModifyTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkinModifyTab)
	DDX_Control(pDX, IDC_YBUT, m_YBut);
	DDX_Control(pDX, IDC_XBUT, m_XBut);
	DDX_Control(pDX, IDC_WELDVERTSBUT, m_WeldVertsBut);
	DDX_Control(pDX, IDC_SELECTBUT, m_SelectBut);
	DDX_Control(pDX, IDC_SCALEBUT, m_ScaleBut);
	DDX_Control(pDX, IDC_ROTATEBUT, m_RotateBut);
	DDX_Control(pDX, IDC_MOVEBUT, m_MoveBut);
	DDX_Control(pDX, IDC_MIRRORBUT, m_MirrorBut);
	DDX_Control(pDX, IDC_DETACHBUT, m_DetachBut);
	DDX_Control(pDX, IDC_DELETESELBUT, m_DeleteSelBut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkinModifyTab, CDialog)
	//{{AFX_MSG_MAP(CSkinModifyTab)
	ON_BN_CLICKED(IDC_MIRRORBUT, OnMirrorbut)
	ON_BN_CLICKED(IDC_WELDVERTSBUT, OnWeldvertsbut)
	ON_BN_CLICKED(IDC_DETACHBUT, OnDetachbut)
	ON_BN_CLICKED(IDC_DELETESELBUT, OnDeleteselbut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinModifyTab message handlers

BOOL CSkinModifyTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	unsigned int     CmdID, Msg;
   HWND     hWnd;
   CWnd     *Parent;
   WPARAM   CheckState;

   Parent = GetParentOwner();
   CmdID = LOWORD(wParam);
   Msg = HIWORD(wParam);
   hWnd = (HWND)lParam;

   if (Msg == BN_CLICKED)
   {
      switch(CmdID)
      {
      case IDC_SELECTBUT:
      case IDC_MOVEBUT:
      case IDC_ROTATEBUT:
      case IDC_SCALEBUT:
         Parent->PostMessage(MM_TOOLBUTCLICK, 0, (long)hWnd);
         break;
      case IDC_XBUT:
      case IDC_YBUT:
         CheckState = ::SendMessage(hWnd, BM_GETCHECK,0,0);
         if (CheckState == 1)
            ::SendMessage(hWnd, BM_SETCHECK,0,0);
         else
            ::SendMessage(hWnd, BM_SETCHECK,1,0);
         break;
         
      }
   }
	

   
	return CDialog::OnCommand(wParam, lParam);
}

void CSkinModifyTab::ToolButClick()
{
}

BOOL CSkinModifyTab::PreTranslateMessage(MSG* pMsg) 
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

void CSkinModifyTab::OnMirrorbut() 
{
   KSkinToolMirror();	
}

void CSkinModifyTab::OnWeldvertsbut() 
{
   KSkinToolWeldVerts();	
}

void CSkinModifyTab::OnDetachbut() 
{
   KSkinToolDetach();
}

void CSkinModifyTab::OnDeleteselbut() 
{
   KSkinToolDelete();	
}
