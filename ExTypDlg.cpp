// ExTypDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "ExTypDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportNamingDlg dialog

bool NumberButChecked;


CExportNamingDlg::CExportNamingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportNamingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportNamingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExportNamingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportNamingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportNamingDlg, CDialog)
	//{{AFX_MSG_MAP(CExportNamingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportNamingDlg message handlers

unsigned int CExportNamingDlg::GetExportFilename()
{
   int ModalResult;

//   Create(IDD_EXPORTNAMINGDLG, NULL);
   
   ModalResult = DoModal();

   if (ModalResult == IDCANCEL)
      return MOD_EXP_CANCEL;

   if (NumberButChecked)
      return MOD_EXP_NUMBER;
   else
      return MOD_EXP_FRAMENAME;

}

void CExportNamingDlg::OnOK() 
{
   unsigned int ID;

   ID = GetCheckedRadioButton(IDC_FILEANDNUMBER_RADIO, IDC_FRAMENAME_RADIO);
	
   if (ID == IDC_FILEANDNUMBER_RADIO)
      NumberButChecked = true;
   else
      NumberButChecked = false;
	CDialog::OnOK();
}

BOOL CExportNamingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   CheckRadioButton( IDC_FILEANDNUMBER_RADIO, IDC_FRAMENAME_RADIO, IDC_FILEANDNUMBER_RADIO );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
