// ModTab.cpp : implementation file
//
#include "stdafx.h"

#include "MDLGlobs.h"
#include "VCMDL.h"
#include "ModTab.h"

#include "KeyFuncs.h"
#include "MdlMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyTab dialog


CModifyTab::CModifyTab(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyTab)
	//}}AFX_DATA_INIT
}


void CModifyTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyTab)
	DDX_Control(pDX, IDC_ZBUT, m_ZBut);
	DDX_Control(pDX, IDC_YBUT, m_YBut);
	DDX_Control(pDX, IDC_XBUT, m_XBut);
	DDX_Control(pDX, IDC_EFFECTFRAMERANGECHECK, m_EffectFrameRangeCheck);
	DDX_Control(pDX, IDC_EXTUDEBUT, m_ExtrudeBut);
	DDX_Control(pDX, IDC_FLIPNORMALSBUT, m_FlipNormalsBut);
	DDX_Control(pDX, IDC_MIRRORBUT, m_MirrorBut);
	DDX_Control(pDX, IDC_MOVEBUT, m_MoveBut);
	DDX_Control(pDX, IDC_ROTATEBUT, m_RotateBut);
	DDX_Control(pDX, IDC_SCALEBUT, m_ScaleBut);
	DDX_Control(pDX, IDC_SELECTBUT, m_SelectBut);
	DDX_Control(pDX, IDC_MOD_TOSPIN, m_ModToSpin);
	DDX_Control(pDX, IDC_MOD_TOEDIT, m_ModToEdit);
	DDX_Control(pDX, IDC_MOD_FROMSPIN, m_ModFromSpin);
	DDX_Control(pDX, IDC_MOD_FROMEDIT, m_ModFromEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyTab, CDialog)
	//{{AFX_MSG_MAP(CModifyTab)
	ON_BN_CLICKED(IDC_FLIPNORMALSBUT, OnFlipNormalsBut)
	ON_BN_CLICKED(IDC_WELDBUT, OnWeldbut)
	ON_BN_CLICKED(IDC_MIRRORBUT, OnMirrorbut)
	ON_BN_CLICKED(IDC_DELSELECTEDBUT, OnDelselectedbut)
	ON_BN_CLICKED(IDC_SUBDIVIDEBUT, OnSubdividebut)
	ON_BN_CLICKED(IDC_EXTUDEBUT, OnExtudebut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyTab message handlers

BOOL CModifyTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_ModFromSpin.SetBuddy(&m_ModFromEdit);
   m_ModToSpin.SetBuddy(&m_ModToEdit);
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CModifyTab::CheckSpinners()
{
   int maxv,minv;

   maxv = m_ModToSpin.GetPos();
   m_ModToSpin.SetRange(0,g_CurMdl.Head.num_frames - 1);
   m_ModToSpin.SetPos(0);
   m_ModToSpin.SetPos(maxv);

   minv = m_ModFromSpin.GetPos();
   m_ModFromSpin.SetRange(0,m_ModFromSpin.GetPos());
   m_ModFromSpin.SetPos(0);
   m_ModFromSpin.SetPos(minv);

}

BOOL CModifyTab::OnCommand(WPARAM wParam, LPARAM lParam) 
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
      case IDC_TURNEDGEBUT:
         Parent->PostMessage(MM_TOOLBUTCLICK, 0, (long)hWnd);
         break;
      case IDC_XBUT:
      case IDC_YBUT:
      case IDC_ZBUT:
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

void CModifyTab::OnFlipNormalsBut() 
{
   KToolFlipNormal();
}

BOOL CModifyTab::PreTranslateMessage(MSG* pMsg) 
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

void CModifyTab::ResetSpinners()
{
   m_ModFromSpin.SetRange(0, g_CurMdl.Head.num_frames);
   m_ModToSpin.SetRange(0, g_CurMdl.Head.num_frames);
   m_ModFromSpin.SetPos(0);
   m_ModToSpin.SetPos(g_CurMdl.Head.num_frames-1);
   CheckSpinners();
}

void CModifyTab::OnWeldbut() 
{
   KToolWeld();	
}

void CModifyTab::OnMirrorbut() 
{
   KToolMirror();	
}

void CModifyTab::OnDelselectedbut() 
{
   KToolDelete();
   
}

void CModifyTab::OnSubdividebut() 
{
   KToolSubdivide();	
}

void CModifyTab::OnExtudebut() 
{
   KToolExtrude();	
}
