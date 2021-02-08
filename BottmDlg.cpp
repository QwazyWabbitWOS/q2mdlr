// DottmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "MDLGlobs.h"
#include "BottmDlg.h"

#include "MdlMsgs.h"
#include "MainFunc.h"
#include "KeyFuncs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBottomDlg dialog


CBottomDlg::CBottomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBottomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBottomDlg)
	//}}AFX_DATA_INIT
}


void CBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBottomDlg)
	DDX_Control(pDX, IDC_PANVIEWSBUT, m_PanViewsBut);
	DDX_Control(pDX, IDC_COUNTFEEDBACKEDIT, m_FeedBackEdit);
	DDX_Control(pDX, IDC_VERTEXMODEBUT, m_VertexModeBut);
	DDX_Control(pDX, IDC_OBJECTMODEBUT, m_ObjectModeBut);
	DDX_Control(pDX, IDC_BONEMODEBUT, m_BoneModeBut);
	DDX_Control(pDX, IDC_FACEMODEBUT, m_FaceModeBut);
	DDX_Control(pDX, IDC_TIMESCROLL, m_TimeScroll);
	DDX_Control(pDX, IDC_GRIDSIZEEDIT, m_GridSizeEdit);
	DDX_Control(pDX, IDC_ZEDIT, m_ZEdit);
	DDX_Control(pDX, IDC_YEDIT, m_YEdit);
	DDX_Control(pDX, IDC_XEDIT, m_XEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBottomDlg, CDialog)
	//{{AFX_MSG_MAP(CBottomDlg)
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_VERTEXMODEBUT, OnVertexModeBut)
	ON_BN_CLICKED(IDC_FACEMODEBUT, OnFaceModeBut)
	ON_BN_CLICKED(IDC_BONEMODEBUT, OnBoneModeBut)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_OBJECTMODEBUT, OnObjectmodebut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBottomDlg message handlers

void CBottomDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanged(lpwndpos);
	
	RECT     ClientRect, PanButRect, GridEditRect;
   int      PanButTop, GridEditTop;

   GetClientRect(&ClientRect);

   m_PanViewsBut.GetWindowRect(&PanButRect);
   ScreenToClient(&PanButRect);
   PanButTop = PanButRect.top;
   m_PanViewsBut.GetWindowRect(&PanButRect);
   m_PanViewsBut.ScreenToClient(&PanButRect);

   m_GridSizeEdit.GetWindowRect(&GridEditRect);
   ScreenToClient(&GridEditRect);
   GridEditTop = GridEditRect.top;
   m_GridSizeEdit.GetWindowRect(&GridEditRect);
   m_GridSizeEdit.ScreenToClient(&GridEditRect);


   m_TimeScroll.MoveWindow(6,6,ClientRect.right-6-ClientRect.left, 12);
   m_PanViewsBut.MoveWindow(ClientRect.right - PanButRect.right - 4, PanButTop, PanButRect.right, PanButRect.bottom);
   m_GridSizeEdit.MoveWindow(ClientRect.right - PanButRect.right - GridEditRect.right - 4, GridEditTop, GridEditRect.right, GridEditRect.bottom);
//   m_PanViewsBut.MoveWindow(6, 6, PanButRect.right, 54);
}


BOOL CBottomDlg::SetSelectionType(SelectType SelType)
{
   switch (SelType)   
   {
      case stVertex:
         if (m_VertexModeBut.GetCheck() == 1)
            return FALSE;
         m_VertexModeBut.SetCheck(1);
         m_FaceModeBut.SetCheck(0);
         m_BoneModeBut.SetCheck(0);
      break;
      case stTriangle:
         if (m_FaceModeBut.GetCheck() == 1)
            return FALSE;
         m_VertexModeBut.SetCheck(0);
         m_FaceModeBut.SetCheck(1);
         m_BoneModeBut.SetCheck(0);
      break;
      case stBone:
         if (m_BoneModeBut.GetCheck() == 1)
            return FALSE;
         m_VertexModeBut.SetCheck(0);
         m_FaceModeBut.SetCheck(0);
         m_BoneModeBut.SetCheck(1);
      break;
   }

	if (m_VertexModeBut.GetCheck())
   	g_MainSelectMode = stVertex;
	if (m_FaceModeBut.GetCheck())
   	g_MainSelectMode = stTriangle;
	if (m_BoneModeBut.GetCheck())
   	g_MainSelectMode = stBone;

   GetParentOwner()->SendMessage(MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL);

   return TRUE;
}

void CBottomDlg::OnVertexModeBut() 
{
   KModeVertex();
}

void CBottomDlg::OnFaceModeBut() 
{
   KModeFace();
}

void CBottomDlg::OnBoneModeBut() 
{
   KModeBones();
}



void CBottomDlg::UpdateSelectionCounters()
{
   if (g_MainSelectMode == stVertex)
   {
      int xyz_visible=0, xyz_selected=0;
      
      for (int v=0;v<g_CurMdl.Head.num_xyz;v++)
      {
         if (g_CurMdl.Verts[v].Visible)
         {
            xyz_visible ++;
            if (g_CurMdl.Verts[v].Selected)
               xyz_selected ++;
         }
      }
      
      CString  Feedback;
      Feedback.Format("%i/%i/%i",xyz_selected, xyz_visible, g_CurMdl.Head.num_xyz);
      m_FeedBackEdit.SetWindowText(Feedback);
   }
   else
   if (g_MainSelectMode == stTriangle)
   {
      int tri_visible=0, tri_selected=0;
      
      for (int t=0;t<g_CurMdl.Head.num_tris;t++)
      {
         if (g_CurMdl.Tris[t].Visible)
         {
            tri_visible ++;
            if (g_CurMdl.Tris[t].Selected)
               tri_selected ++;
         }
      }
      
      CString  Feedback;
      Feedback.Format("%i/%i/%i",tri_selected, tri_visible, g_CurMdl.Head.num_tris);
      m_FeedBackEdit.SetWindowText(Feedback);

   
   }
   else
   if (g_MainSelectMode == stBone)
   {
      // not implemented yet
      m_FeedBackEdit.SetWindowText("-/-/-");
   }
      

}

int CBottomDlg::GetHeight()
{
   RECT  ClientRect;
   GetClientRect(&ClientRect);
   return ClientRect.bottom;
}

void CBottomDlg::SetXYZLabels(float X, float Y, float Z, unsigned int nFlags)
{
   CString T;

   if (nFlags & XYZ_X)
   {
      T.Format("%.3f",X);
      m_XEdit.SetWindowText(T);
   }
   if (nFlags & XYZ_Y)
   {
      T.Format("%.3f",Y);
      m_YEdit.SetWindowText(T);
   }
   if (nFlags & XYZ_Z)
   {
      T.Format("%.3f",Z);
      m_ZEdit.SetWindowText(T);
   }

}

BOOL CBottomDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
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
      case IDC_PANVIEWSBUT:
         Parent->PostMessage(MM_TOOLBUTCLICK, 0, (long)hWnd);
      }
   }
	
	return CDialog::OnCommand(wParam, lParam);
}

void CBottomDlg::OnHScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) 
{
   if (nSBCode != SB_THUMBPOSITION && nSBCode != SB_THUMBTRACK)
      nPos = pScrollBar->GetScrollPos();
   if (nSBCode == SB_LINELEFT)
      nPos --;
   if (nSBCode == SB_LINERIGHT)
      nPos ++;
   if (nSBCode == SB_PAGELEFT)
      nPos -= 10;
   if (nSBCode == SB_PAGERIGHT)
      nPos += 10;


   if (!g_Playing)
      g_CurMdl.UpdateFrame(g_CurFrame);

   g_CurFrame = nPos;

   if (g_CurFrame < 0)
      g_CurFrame = g_CurMdl.Head.num_frames + g_CurFrame;
   else
   if (g_CurFrame >= g_CurMdl.Head.num_frames)
      g_CurFrame -= g_CurMdl.Head.num_frames;


   
   g_CurMdl.SetCurrentFrame(g_CurFrame);
   if (g_pRef)
      g_pRef->SetCurrentFrame(g_CurFrame);

   ::SendMessage(this->GetParentOwner()->m_hWnd, MM_UPDATEFRAMELABEL, MMC_NOSCROLL, 0);

	CalcAllNormals();

   ::SendMessage(this->GetParentOwner()->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	
   pScrollBar->SetScrollPos(nPos);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBottomDlg::PreTranslateMessage(MSG* pMsg) 
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

void CBottomDlg::UpdateGridLabel()
{
   CString  Str;
   Str.Format("Grid: %.1f", g_GridSize);
   m_GridSizeEdit.SetWindowText(Str);
  
}

void CBottomDlg::OnObjectmodebut() 
{
   KModeObject();
}
