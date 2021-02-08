// ConDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "ConDlg.h"

#include "HelperFuncs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConsoleDlg dialog


CConsoleDlg::CConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConsoleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConsoleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


}


void CConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConsoleDlg)
	DDX_Control(pDX, IDC_OUTPUTEDIT, m_OutputEdit);
	DDX_Control(pDX, IDC_INPUTEDIT, m_InputEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConsoleDlg, CDialog)
	//{{AFX_MSG_MAP(CConsoleDlg)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConsoleDlg message handlers


void CConsoleDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanged(lpwndpos);
	
   RECT  WndClientRect, InputRect, OutputRect;

   GetClientRect(&WndClientRect);
   m_InputEdit.GetWindowRect(&InputRect);
   m_OutputEdit.GetWindowRect(&OutputRect);

   m_InputEdit.ScreenToClient(&InputRect);
   m_OutputEdit.ScreenToClient(&OutputRect);

   m_InputEdit.MoveWindow(1,WndClientRect.bottom - InputRect.bottom - 2 , WndClientRect.right-2, InputRect.bottom+2);
   m_OutputEdit.MoveWindow(1,1 , WndClientRect.right-2, WndClientRect.bottom - InputRect.bottom - 4);
}


void CConsoleDlg::AddLineToConsole(const char *Text)
{
   AddLineToEdit(m_OutputEdit.m_hWnd, Text);
}
