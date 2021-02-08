// DirCfgPg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "DirCfgPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirConfigPage dialog

IMPLEMENT_DYNCREATE(CDirConfigPage, CPropertyPage)

CDirConfigPage::CDirConfigPage() : CPropertyPage(CDirConfigPage::IDD)
{
	//{{AFX_DATA_INIT(CDirConfigPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDirConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirConfigPage)
	DDX_Control(pDX, IDC_PATHEDIT, m_PathEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirConfigPage, CDialog)
	//{{AFX_MSG_MAP(CDirConfigPage)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_PATHTREE, OnItemexpandingPathtree)
	ON_NOTIFY(TVN_DELETEITEM, IDC_PATHTREE, OnDeleteitemPathtree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PATHTREE, OnSelchangedPathtree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirConfigPage message handlers

BOOL CDirConfigPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_PathTree = (CShellTree*)GetDlgItem(IDC_PATHTREE);

   ASSERT(m_PathTree);

   m_PathTree->EnableImages();
   m_PathTree->PopulateTree();

//   m_BasePath.SetAt(m_BasePath.GetLength()-1, '\x0');
   m_BasePath.Replace("/", "\\");

   m_PathTree->TunnelTree(m_BasePath);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDirConfigPage::OnItemexpandingPathtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

   m_PathTree->OnFolderExpanding(pNMHDR, pResult);

   *pResult = 0;
}

void CDirConfigPage::OnDeleteitemPathtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

   m_PathTree->OnDeleteShellItem(pNMHDR,pResult);

	*pResult = 0;
}

void CDirConfigPage::OnSelchangedPathtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

   if (m_PathTree->OnFolderSelected(pNMHDR,pResult,m_BasePath))
   {
      m_BasePath += '\\';
      m_PathEdit.SetWindowText(m_BasePath);
   }

	*pResult = 0;
}
