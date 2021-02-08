// SToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "SToolDlg.h"

#include "MdlGlobs.h"
#include "KeyFuncs.h"
#include "MdlMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinSideToolDlg dialog


CSkinSideToolDlg::CSkinSideToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkinSideToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkinSideToolDlg)
	//}}AFX_DATA_INIT
}


void CSkinSideToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkinSideToolDlg)
	DDX_Control(pDX, IDC_ZOOMOUTBUT, m_ZoomOutBut);
	DDX_Control(pDX, IDC_ZOOMINBUT, m_ZoomInBut);
	DDX_Control(pDX, IDC_PREVSKINBUT, m_PrevSkinBut);
	DDX_Control(pDX, IDC_NEXTSKINBUT, m_NextSkinBut);
	DDX_Control(pDX, IDC_SKINNUMEDIT, m_SkinNumEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkinSideToolDlg, CDialog)
	//{{AFX_MSG_MAP(CSkinSideToolDlg)
	ON_BN_CLICKED(IDC_ZOOMINBUT, OnZoominbut)
	ON_BN_CLICKED(IDC_ZOOMOUTBUT, OnZoomoutbut)
	ON_BN_CLICKED(IDC_NEXTSKINBUT, OnNextskinbut)
	ON_BN_CLICKED(IDC_PREVSKINBUT, OnPrevskinbut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinSideToolDlg message handlers

int CSkinSideToolDlg::GetWidth()
{
   RECT  Rect;

   GetWindowRect(&Rect);
   return Rect.right-Rect.left;
}

void CSkinSideToolDlg::OnZoominbut() 
{
   KSkinZoomIn();
   
}

void CSkinSideToolDlg::OnZoomoutbut() 
{
   KSkinZoomOut();
   
}

BOOL CSkinSideToolDlg::PreTranslateMessage(MSG* pMsg) 
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

void CSkinSideToolDlg::OnNextskinbut() 
{
   g_CurSkin ++;
   if (g_CurSkin >= g_CurMdl.Head.num_skins)
      g_CurSkin = 0;
   
   GetParentOwner()->SendMessage(MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN,MMC_UPDATE_DEPENDANTS);
   UpdateFrameLabels();
}

void CSkinSideToolDlg::OnPrevskinbut() 
{
   g_CurSkin --;
   if (g_CurSkin < 0)
      g_CurSkin = g_CurMdl.Head.num_skins-1;
	
   GetParentOwner()->SendMessage(MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN,MMC_UPDATE_DEPENDANTS);
   UpdateFrameLabels();
}

void CSkinSideToolDlg::UpdateFrameLabels()
{
   CString  Str;
   Str.Format("%i", g_CurSkin);
   m_SkinNumEdit.SetWindowText(Str);
}
