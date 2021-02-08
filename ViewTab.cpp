// ViewTab.cpp : implementation file
//
#include "stdafx.h"

#include "MDLGlobs.h"
#include "VCMDL.h"
#include "ViewTab.h"

#include "MdlMsgs.h"
#include "MainFunc.h"
#include "KeyFuncs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTab dialog


CViewTab::CViewTab(CWnd* pParent /*=NULL*/)
	: CDialog(CViewTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CViewTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewTab)
	DDX_Control(pDX, IDC_INTERPOLATEBUT, m_InterpolateBut);
	DDX_Control(pDX, IDC_PLAYCAMBUT, m_PlayBut);
	DDX_Control(pDX, IDC_VIEW_FPSEDIT, m_FPSEdit);
	DDX_Control(pDX, IDC_VIEW_FPSSPIN, m_FPSSpin);
	DDX_Control(pDX, IDC_VIEW_TOSPIN, m_ToAnimSpin);
	DDX_Control(pDX, IDC_VIEW_TOEDIT, m_ToAnimEdit);
	DDX_Control(pDX, IDC_VIEW_FROMSPIN, m_FromAnimSpin);
	DDX_Control(pDX, IDC_VIEW_FROMEDIT, m_FromAnimEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewTab, CDialog)
	//{{AFX_MSG_MAP(CViewTab)
	ON_BN_CLICKED(IDC_PLAYCAMBUT, OnPlayCamBut)
	ON_BN_CLICKED(IDC_INTERPOLATEBUT, OnInterpolateBut)
	ON_NOTIFY(UDN_DELTAPOS, IDC_VIEW_FPSSPIN, OnDeltaposViewFpsspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_VIEW_FROMSPIN, OnDeltaposViewFromspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_VIEW_TOSPIN, OnDeltaposViewTospin)
	ON_BN_CLICKED(IDC_HIDESELECTEDBUT, OnHideselectedbut)
	ON_BN_CLICKED(IDC_HIDEUNSELECTEDBUT, OnHideunselectedbut)
	ON_BN_CLICKED(IDC_UNHIDEALLBUT, OnUnhideallbut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTab message handlers

BOOL CViewTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_FromAnimEdit.SetWindowText("0");
   m_FromAnimSpin.SetBuddy(&m_FromAnimEdit);
   m_ToAnimEdit.SetWindowText("0");
   m_ToAnimSpin.SetBuddy(&m_ToAnimEdit);
   m_FPSEdit.SetWindowText("10");
   m_FPSSpin.SetBuddy(&m_FPSEdit);
   m_FPSSpin.SetPos(10);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewTab::CheckSpinners()
{
   int maxv,minv;

   maxv = m_ToAnimSpin.GetPos();
   m_ToAnimSpin.SetRange(0,g_CurMdl.Head.num_frames - 1);
   m_ToAnimSpin.SetPos(0);
   m_ToAnimSpin.SetPos(maxv);

   minv = m_FromAnimSpin.GetPos();
   m_FromAnimSpin.SetRange(0,m_ToAnimSpin.GetPos());
   m_FromAnimSpin.SetPos(0);
   m_FromAnimSpin.SetPos(minv);

   CString  Text;
//   Text.Format("%n",m_ToAnimSpin.GetPos());
//   m_ToAnimEdit.SetWindowText(Text);
//   Text.Format("%n",m_FromAnimSpin.GetPos());
//   m_FromAnimEdit.SetWindowText(Text);

	m_ToAnimSpin.Invalidate();
	m_FromAnimSpin.Invalidate();

}


void CViewTab::OnPlayCamBut() 
{
   if (m_PlayBut.GetCheck() == 1)
   {
      m_PlayBut.SetCheck(0);
   }
	
   CheckSpinners();

   InterFrame = -8192;
	g_Playing = !g_Playing;

   if (g_Playing)
   {
      m_PlayBut.SetCheck(1);
   }
   else
   {
      m_PlayBut.SetCheck(0);
   }

   if (g_Playing)
	{
      g_Drawing = true;
      for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
      {
         g_CurMdl.Frames[g_CurFrame][n].x = g_CurMdl.Verts[n].x;
         g_CurMdl.Frames[g_CurFrame][n].y = g_CurMdl.Verts[n].y;
         g_CurMdl.Frames[g_CurFrame][n].z = g_CurMdl.Verts[n].z;
      }
      CalcAllNormals();
      g_Drawing = false;
   	PlayFrame = g_CurFrame;
   }
   else
   {
   	g_Drawing = true;
   	g_CurFrame = PlayFrame;
      for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
      {
         g_CurMdl.Verts[n].x = g_CurMdl.Frames[g_CurFrame][n].x;
         g_CurMdl.Verts[n].y = g_CurMdl.Frames[g_CurFrame][n].y;
         g_CurMdl.Verts[n].z = g_CurMdl.Frames[g_CurFrame][n].z;
      }
      CalcAllNormals();
      g_Drawing = false;

      ::SendMessage(GetParentOwner()->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
      ::SendMessage(GetParentOwner()->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);
   }
   
}

void CViewTab::OnInterpolateBut() 
{
   if (m_InterpolateBut.GetCheck() == 1)
   {
      m_InterpolateBut.SetCheck(0);
   }
   else
   {
      m_InterpolateBut.SetCheck(1);
   }
   
}

BOOL CViewTab::PreTranslateMessage(MSG* pMsg) 
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

void CViewTab::OnDeltaposViewFpsspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   CheckSpinners();   
	
	*pResult = 0;
}

void CViewTab::OnDeltaposViewFromspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
   
   CheckSpinners();   
	
	*pResult = 0;
}

void CViewTab::OnDeltaposViewTospin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   CheckSpinners();   
	
	*pResult = 0;
}

void CViewTab::ResetSpinners()
{
   m_FromAnimSpin.SetRange(0, g_CurMdl.Head.num_frames);  
   m_ToAnimSpin.SetRange(0, g_CurMdl.Head.num_frames);  
   m_FromAnimSpin.SetPos(0);
   m_ToAnimSpin.SetPos(g_CurMdl.Head.num_frames-1);
   CheckSpinners();
}

void CViewTab::OnHideselectedbut() 
{
	KControlHideSelected();
}

void CViewTab::OnHideunselectedbut() 
{
   KControlHideUnSelected()	;
}

void CViewTab::OnUnhideallbut() 
{
   KControlUnhide();	
}

