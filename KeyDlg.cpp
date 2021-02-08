// KeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "KeyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPressKeyDlg dialog


CPressKeyDlg::CPressKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPressKeyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPressKeyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPressKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPressKeyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPressKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CPressKeyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPressKeyDlg message handlers

BOOL CPressKeyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_KeyPressed = 0;
   m_Shift = false;
   m_Alt = false;
   m_Ctrl = false;
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPressKeyDlg::PreTranslateMessage(MSG* pMsg) 
{
   if (pMsg->message == WM_KEYUP)
   {
      if (pMsg->wParam == VK_SHIFT)
      {
         m_Shift = false;
         return CDialog::PreTranslateMessage(pMsg);
      }
      if (pMsg->wParam == VK_CONTROL)
      {
         m_Ctrl = false;
         return CDialog::PreTranslateMessage(pMsg);
      }
      if (pMsg->wParam == VK_MENU)
      {
         m_Alt = false;
         return CDialog::PreTranslateMessage(pMsg);
      }

   }
   if (pMsg->message == WM_KEYDOWN)
   {
      if (pMsg->wParam == VK_SHIFT)
      {
         m_Shift = true;
         return CDialog::PreTranslateMessage(pMsg);
      }
      if (pMsg->wParam == VK_CONTROL)
      {
         m_Ctrl = true;
         return CDialog::PreTranslateMessage(pMsg);
      }
      if (pMsg->wParam == VK_MENU)
      {
         m_Alt = true;
         return CDialog::PreTranslateMessage(pMsg);
      }

      m_KeyPressed = pMsg->wParam;
      EndDialog(IDOK);
   }
	
	return CDialog::PreTranslateMessage(pMsg);
}
