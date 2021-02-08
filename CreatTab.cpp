// CreatTab.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "CreatTab.h"

#include "MdlMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateTab dialog


CCreateTab::CCreateTab(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCreateTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateTab)
	DDX_Control(pDX, IDC_CREATEVERTEXBUT, m_CreateVertexBut);
	DDX_Control(pDX, IDC_CREATEFANBUT, m_CreateFanBut);
	DDX_Control(pDX, IDC_CREATESTRIPBUT, m_CreateStripBut);
	DDX_Control(pDX, IDC_BUILDFACEBUT, m_BuildFaceBut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateTab, CDialog)
	//{{AFX_MSG_MAP(CCreateTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateTab message handlers


BOOL CCreateTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	unsigned int  CmdID, Msg;
   HWND  hWnd;
   CWnd  *Parent;

   Parent = GetParentOwner();
   CmdID = LOWORD(wParam);
   Msg = HIWORD(wParam);
   hWnd = (HWND)lParam;

   if (Msg == BN_CLICKED)
   {
      switch(CmdID)
      {
      case IDC_CREATEVERTEXBUT:
      case IDC_BUILDFACEBUT:
      case IDC_CREATEFANBUT:
      case IDC_CREATESTRIPBUT:
         Parent->PostMessage(MM_TOOLBUTCLICK, 0, (long)hWnd);
      }
   }
	
	return CDialog::OnCommand(wParam, lParam);
}


LRESULT CCreateTab::WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam) 
{
/*   if (message == WM_KEYDOWN)
   {
      return GetParentOwner()->SendMessage(message, wParam, lParam);
   }*/
   
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CCreateTab::PreTranslateMessage(MSG* pMsg) 
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
