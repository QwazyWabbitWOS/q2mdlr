// BonesTab.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "BonesTab.h"

#include "MdlMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBonesTab dialog


CBonesTab::CBonesTab(CWnd* pParent /*=NULL*/)
	: CDialog(CBonesTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBonesTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBonesTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBonesTab)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBonesTab, CDialog)
	//{{AFX_MSG_MAP(CBonesTab)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBonesTab message handlers

BOOL CBonesTab::OnCommand(WPARAM wParam, LPARAM lParam) 
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
      case IDC_BUTTONBONES:
         Parent->PostMessage(MM_TOOLBUTCLICK, 0, (long)hWnd);
      }
   }

	return CDialog::OnCommand(wParam, lParam);
}
