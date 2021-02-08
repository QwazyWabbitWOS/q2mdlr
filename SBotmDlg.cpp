// SBotmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"

#include "MdlGlobs.h"
#include "KeyFuncs.h"
#include "MDLMsgs.h"

#include "SBotmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinBottomToolDlg dialog


CSkinBottomToolDlg::CSkinBottomToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkinBottomToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkinBottomToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSkinBottomToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkinBottomToolDlg)
	DDX_Control(pDX, IDC_YEDIT, m_YEdit);
	DDX_Control(pDX, IDC_XEDIT, m_XEdit);
	DDX_Control(pDX, IDC_VERTEXMODEBUT, m_VertexModeBut);
	DDX_Control(pDX, IDC_OBJECTMODEBUT, m_ObjectModeBut);
	DDX_Control(pDX, IDC_FACEMODEBUT, m_FaceModeBut);
	DDX_Control(pDX, IDC_COUNTFEEDBACKEDIT, m_FeebackEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkinBottomToolDlg, CDialog)
	//{{AFX_MSG_MAP(CSkinBottomToolDlg)
	ON_BN_CLICKED(IDC_VERTEXMODEBUT, OnVertexmodebut)
	ON_BN_CLICKED(IDC_FACEMODEBUT, OnFacemodebut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinBottomToolDlg message handlers

int CSkinBottomToolDlg::GetHeight()
{
   RECT  ClientRect;
   GetClientRect(&ClientRect);
   return ClientRect.bottom;
}

BOOL CSkinBottomToolDlg::PreTranslateMessage(MSG* pMsg) 
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

BOOL CSkinBottomToolDlg::SetSelectionType(SelectType SelType)
{
   switch (SelType)   
   {
      case stVertex:
         if (m_VertexModeBut.GetCheck() == 1)
            return FALSE;
         m_VertexModeBut.SetCheck(1);
         m_FaceModeBut.SetCheck(0);
      break;
      case stTriangle:
         if (m_FaceModeBut.GetCheck() == 1)
            return FALSE;
         m_VertexModeBut.SetCheck(0);
         m_FaceModeBut.SetCheck(1);
      break;
   }

	if (m_VertexModeBut.GetCheck())
   	g_SkinSelectMode = stVertex;
	if (m_FaceModeBut.GetCheck())
   	g_SkinSelectMode = stTriangle;

   GetParentOwner()->SendMessage(MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN);

   return TRUE;
}

void CSkinBottomToolDlg::OnVertexmodebut() 
{
   KSkinVertexMode();	
}

void CSkinBottomToolDlg::OnFacemodebut() 
{
	KSkinFaceMode();
}

BOOL CSkinBottomToolDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CDialog::OnCommand(wParam, lParam);
}
