// TypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"

#include "MdlGlobs.h"
#include "TypeDlg.h"
#include "Undo.h"
#include "Undo3D.h"
#include "MdlMsgs.h"

#include "VCmdldlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTypeInDlg dialog


CTypeInDlg::CTypeInDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTypeInDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTypeInDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

   m_X = m_Y = m_Z = m_OX = m_OY = m_OZ = 0.0;
}


void CTypeInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTypeInDlg)
	DDX_Control(pDX, IDC_APPLYBUT, m_ApplyBut);
	DDX_Control(pDX, IDC_SELECTIONCENTRECHECK, m_SelectionCentreCheck);
	DDX_Control(pDX, IDC_XFORMGROUP, m_XFormGroup);
	DDX_Control(pDX, IDC_ZEDIT, m_ZEdit);
	DDX_Control(pDX, IDC_YEDIT, m_YEdit);
	DDX_Control(pDX, IDC_XEDIT, m_XEdit);
	DDX_Control(pDX, IDC_OZEDIT, m_OZEdit);
	DDX_Control(pDX, IDC_OYEDIT, m_OYEdit);
	DDX_Control(pDX, IDC_OXEDIT, m_OXEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTypeInDlg, CDialog)
	//{{AFX_MSG_MAP(CTypeInDlg)
	ON_BN_CLICKED(IDC_APPLYBUT, OnApplybut)
	ON_EN_KILLFOCUS(IDC_XEDIT, OnKillfocusXedit)
	ON_EN_KILLFOCUS(IDC_YEDIT, OnKillfocusYedit)
	ON_EN_KILLFOCUS(IDC_ZEDIT, OnKillfocusZedit)
	ON_EN_KILLFOCUS(IDC_OXEDIT, OnKillfocusOxedit)
	ON_EN_KILLFOCUS(IDC_OYEDIT, OnKillfocusOyedit)
	ON_EN_KILLFOCUS(IDC_OZEDIT, OnKillfocusOzedit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTypeInDlg message handlers

void CTypeInDlg::SetCurrentTool(ActionType Action, CTypeInTarget Target)
{
   if (Action != m_CurAction || Target != m_Target)
   {
      m_Target = Target;
      m_CurAction = Action;
      switch (Action)
      {
         case atMove:
            m_XFormGroup.SetWindowText("Move");
            m_X = m_Y = m_Z = 0;
            EnableAll(TRUE);
            EnableOrigin(FALSE);
            UpdateSpinEdits();
         break;
         case atRotate:
            m_XFormGroup.SetWindowText("Rotate");
            m_X = m_Y = m_Z = 0;
            EnableAll(TRUE);
            EnableOrigin(TRUE);
            UpdateSpinEdits();
         break;
         case atScale:
            m_XFormGroup.SetWindowText("Scale");
            m_X = m_Y = m_Z = 0;
            EnableAll(TRUE);
            EnableOrigin(TRUE);
            UpdateSpinEdits();
         break;
         default:
            m_XFormGroup.SetWindowText("No Tool Selected");
            EnableAll(FALSE);
      }
   }
}


void CTypeInDlg::UpdateSpinEdits()
{
   CString  Str;

   Str.Format("%0.4f", m_X);
   m_XEdit.SetWindowText(Str);
   Str.Format("%0.4f", m_Y);
   m_YEdit.SetWindowText(Str);
   Str.Format("%0.4f", m_Z);
   m_ZEdit.SetWindowText(Str);

   Str.Format("%0.4f", m_OX);
   m_OXEdit.SetWindowText(Str);
   Str.Format("%0.4f", m_OY);
   m_OYEdit.SetWindowText(Str);
   Str.Format("%0.4f", m_OZ);
   m_OZEdit.SetWindowText(Str);
}

void CTypeInDlg::EnableOrigin(BOOL State)
{
   m_SelectionCentreCheck.EnableWindow(State);
   m_OXEdit.EnableWindow(State);
   m_OYEdit.EnableWindow(State);
   m_OZEdit.EnableWindow(State);
}


void CTypeInDlg::EnableAll(BOOL State)
{
   EnableOrigin(State);
   m_ApplyBut.EnableWindow(State);
   m_XEdit.EnableWindow(State);
   m_YEdit.EnableWindow(State);
   m_ZEdit.EnableWindow(State);

}

void CTypeInDlg::OnApplybut() 
{
   CVCMDLDlg   *MainDlg;

   MainDlg = (CVCMDLDlg*)m_MainDlg;

   if (g_Playing)
      return;
   if (m_Target == tiModel)
   {
      g_CurMdl.UpdateFrame(g_CurFrame);

      if (m_SelectionCentreCheck.GetCheck())
      {
         Vector3  O;
         O = g_CurMdl.GetSelectionCentre(g_MainSelectMode);

         m_OX = O.x;
         m_OY = O.y;
         m_OZ = O.z;
      }

      int min,max;
      if (MainDlg->m_ModifyTab.m_EffectFrameRangeCheck.GetCheck())
      {
         min = MainDlg->m_ModifyTab.m_ModFromSpin.GetPos();
         max = MainDlg->m_ModifyTab.m_ModToSpin.GetPos();
      }
      else
      {
         min=max=g_CurFrame;
      }

      if (m_CurAction == atMove)
      {
         CMoveUndoNode *M;
         M = new CMoveUndoNode;

         // Use the undo nodes to perform the action
         M->Set(CString("Move "), &g_CurMdl,min,max, m_X, m_Y, m_Z, g_MainSelectMode);
         M->Execute();

         M->dx = -M->dx;
         M->dy = -M->dy;
         M->dz = -M->dz;

         g_MainUndo.AddNode(M);
      }
      if (m_CurAction == atRotate)
      {
         CRotateUndoNode *R;
         R = new CRotateUndoNode;

         double   D = 180.0/M_PI;

         // Use the undo nodes to perform the action
         R->Set(CString("Rotate"), &g_CurMdl,min,max, m_OX, m_OY, m_OZ, (float)(m_X/D), (float)(m_Y/D), (float)(m_Z/D), g_MainSelectMode);
         R->Execute();

         R->dx = -R->dx;
         R->dy = -R->dy;
         R->dz = -R->dz;

         g_MainUndo.AddNode(R);
      }
      if (m_CurAction == atScale)
      {
         CScaleUndoNode *S;
         S = new CScaleUndoNode;

         // Use the undo nodes to perform the action
         S->Set(CString("Move "), &g_CurMdl,min,max, m_OX, m_OY, m_OZ, m_X, m_Y, m_Z, g_MainSelectMode);
         S->Execute();

         S->dx = (float)(1.0/S->dx);
         S->dy = (float)(1.0/S->dy);
         S->dz = (float)(1.0/S->dz);

         g_MainUndo.AddNode(S);
      }

      g_CurMdl.SetCurrentFrame(g_CurFrame);
      if (m_MainDlg)
         ::SendMessage(m_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
   }
   
}

BOOL CTypeInDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   SetCurrentTool(atMove);
   SetCurrentTool(atNone);
   
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTypeInDlg::OnKillfocusXedit() 
{
   char     Str[32];
   m_XEdit.GetWindowText(Str, 32);
   m_X = (float)atof(Str);
}

void CTypeInDlg::OnKillfocusYedit() 
{
   char     Str[32];
   m_YEdit.GetWindowText(Str, 32);
   m_Y = (float)atof(Str);
}

void CTypeInDlg::OnKillfocusZedit() 
{
   char     Str[32];
   m_ZEdit.GetWindowText(Str, 32);
   m_Z = (float)atof(Str);
}

void CTypeInDlg::OnKillfocusOxedit() 
{
   char     Str[32];
   m_OXEdit.GetWindowText(Str, 32);
   m_OX = (float)atof(Str);
}

void CTypeInDlg::OnKillfocusOyedit() 
{
   char     Str[32];
   m_OYEdit.GetWindowText(Str, 32);
   m_OY = (float)atof(Str);
}

void CTypeInDlg::OnKillfocusOzedit() 
{
   char     Str[32];
   m_OZEdit.GetWindowText(Str, 32);
   m_OZ = (float)atof(Str);
}
