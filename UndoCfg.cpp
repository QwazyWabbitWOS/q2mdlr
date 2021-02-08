// UndoCfg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"

#include "MdlGlobs.h"
#include "Undo.h"

#include "UndoCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUndoConfigPage property page

IMPLEMENT_DYNCREATE(CUndoConfigPage, CPropertyPage)

CUndoConfigPage::CUndoConfigPage() : CPropertyPage(CUndoConfigPage::IDD)
{
	//{{AFX_DATA_INIT(CUndoConfigPage)
	//}}AFX_DATA_INIT
}

CUndoConfigPage::~CUndoConfigPage()
{
}

void CUndoConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUndoConfigPage)
	DDX_Control(pDX, IDC_UNDOSIZEEDIT, m_UndoSizeEdit);
	DDX_Control(pDX, IDC_UNDOLEVELSEDIT, m_UndoLevelsEdit);
	DDX_Control(pDX, IDC_MINUNDOSEDIT, m_MinUndosEdit);
	DDX_Control(pDX, IDC_MAXUNDOSIZEEDIT, m_MaxUndoSizeEdit);
	DDX_Control(pDX, IDC_MAXUNDOSEDIT, m_MaxUndosEdit);
	DDX_Control(pDX, IDC_UNDOMODE_TAB, m_UndoModeTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUndoConfigPage, CPropertyPage)
	//{{AFX_MSG_MAP(CUndoConfigPage)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MINUNDOSSPIN, OnDeltaposMinundosspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MAXUNDOSSPIN, OnDeltaposMaxundosspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MAXUNDOSIZESPIN, OnDeltaposMaxundosizespin)
	ON_NOTIFY(TCN_SELCHANGE, IDC_UNDOMODE_TAB, OnSelchangeUndomodeTab)
	ON_BN_CLICKED(IDC_LIMITUNDOSCHECK, OnLimitundoscheck)
	ON_BN_CLICKED(IDC_MINUNDOSCHECK, OnMinundoscheck)
	ON_BN_CLICKED(IDC_MAXUNDOSRADIO, OnMaxundosradio)
	ON_BN_CLICKED(IDC_MAXUNDOSIZERADIO, OnMaxundosizeradio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUndoConfigPage message handlers

BOOL CUndoConfigPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
   m_UndoModeTab.InsertItem(0, "Main");
   m_UndoModeTab.InsertItem(1, "Skins");
   m_UndoModeTab.InsertItem(2, "Painter");

   UpdateSpinEdits();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
class CUndoLimitInfo
{
public:
	int   m_NodeCount;
   int   m_MaxNodes,m_MinNodes,m_MaxSize;
   bool  m_DeleteRedoOnAdd;
};
*/
void CUndoConfigPage::UpdateSpinEdits()
{
   CString        Str;

   GetCurrentUndoMode();

   CButton  *LimitCheck;
   LimitCheck = (CButton*)GetDlgItem(IDC_LIMITUNDOSCHECK);

   if (!m_CurInfo->m_Limit)     // No max's set, therefore infinite
   {
      LimitCheck->SetCheck(0);
   }
   else
   {
      LimitCheck->SetCheck(1);

      CButton  *MinLevels, *MaxLevels, *MaxSize;

      MinLevels = (CButton*)GetDlgItem(IDC_MINUNDOSCHECK);
      MaxLevels = (CButton*)GetDlgItem(IDC_MAXUNDOSRADIO);
      MaxSize = (CButton*)GetDlgItem(IDC_MAXUNDOSIZERADIO);

      if (!m_CurInfo->m_LimitMinNodes)
      {
         MinLevels->SetCheck(0);
         m_MinUndosEdit.SetWindowText("-");
      }
      else
      {
         MinLevels->SetCheck(1);
         Str.Format("%i", m_CurInfo->m_MinNodes);
         m_MinUndosEdit.SetWindowText(Str);
      }

      if (!m_CurInfo->m_LimitMaxNodes)
      {
         MaxLevels->SetCheck(0);
         m_MaxUndosEdit.SetWindowText("-");
      }
      else
      {
         MaxLevels->SetCheck(1);
         Str.Format("%i", m_CurInfo->m_MaxNodes);
         m_MaxUndosEdit.SetWindowText(Str);
      }

      if (!m_CurInfo->m_LimitMaxSize)
      {
         MaxSize->SetCheck(0);
         m_MaxUndoSizeEdit.SetWindowText("-");
      }
      else
      {
         MaxSize->SetCheck(1);
         Str.Format("%i", m_CurInfo->m_MaxSize);
         m_MaxUndoSizeEdit.SetWindowText(Str);
      }
   }

   Str.Format("%i", m_CurInfo->m_NodeCount);
   m_UndoLevelsEdit.SetWindowText(Str);
   Str.Format("%i kb", m_CurInfo->m_NodeSize);
   m_UndoSizeEdit.SetWindowText(Str);
   
}

void CUndoConfigPage::OnDeltaposMinundosspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   GetCurrentUndoMode();
   if (m_CurInfo->m_MinNodes != -1)
   {
      m_CurInfo->m_MinNodes -= pNMUpDown->iDelta;
      UpdateSpinEdits();
   }
	
	*pResult = 0;
}

void CUndoConfigPage::OnDeltaposMaxundosspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   GetCurrentUndoMode();
   if (m_CurInfo->m_MaxNodes != -1)
   {
      m_CurInfo->m_MaxNodes -= pNMUpDown->iDelta;
      UpdateSpinEdits();
   }
	
	*pResult = 0;
}

void CUndoConfigPage::OnDeltaposMaxundosizespin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
   
   GetCurrentUndoMode();
   if (m_CurInfo->m_MaxSize != -1)
   {
      m_CurInfo->m_MaxSize -= 64*pNMUpDown->iDelta;
      UpdateSpinEdits();
   }

	*pResult = 0;
}

void CUndoConfigPage::GetCurrentUndoMode()
{
   m_CurInfo = NULL;
   if (m_UndoModeTab.GetCurSel() == 0)    // Main Undo m_CurInfo
   {
      m_CurInfo = &m_MainInfo;
   }
   if (m_UndoModeTab.GetCurSel() == 1)    // Skin Undo m_CurInfo
   {
      m_CurInfo = &m_SkinInfo;
   }
   if (m_UndoModeTab.GetCurSel() == 2)    // Paint Undo m_CurInfo
   {
      m_CurInfo = &m_PaintInfo;
   }
   ASSERT(m_CurInfo);   
}


void CUndoLimitInfo::GetFromUndoList(CUndoList *List)
{
   if (!List)
      return;
   m_MaxNodes = List->MaxNodes;
   m_MinNodes = List->MinNodes;
   m_MaxSize = List->MaxSize;

   m_NodeCount = List->NumNodes();
   m_NodeSize = List->BytesUsed();

   m_LimitMaxNodes = true;
   m_LimitMinNodes = true;
   m_LimitMaxSize = true;

   if (m_MaxNodes == -1)
   {
      m_LimitMaxNodes = false;
      m_MaxNodes = 15;
   }

   if (m_MinNodes == -1)
   {
      m_LimitMinNodes = false;
      m_MinNodes = 0;
   }

   if (m_MaxSize == -1)
   {
      m_LimitMaxSize = false;
      m_MaxSize = 4096;
   }

   if (m_LimitMaxSize || m_LimitMinNodes || m_LimitMaxNodes)
      m_Limit = true;
   else
      m_Limit = false;
}

void CUndoLimitInfo::SetToUndoList(CUndoList *List)
{
   if (!List)
      return;

   if (m_LimitMaxNodes)
      List->MaxNodes = m_MaxNodes;
   else
      List->MaxNodes = -1;

   if (m_LimitMinNodes)
      List->MinNodes = m_MinNodes;
   else
      List->MinNodes = -1;

   if (m_LimitMaxSize)
      List->MaxSize = m_MaxSize;
   else
      List->MaxSize = -1;
}

void CUndoConfigPage::OnSelchangeUndomodeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
   UpdateSpinEdits();   
   
	*pResult = 0;
}

void CUndoConfigPage::OnLimitundoscheck() 
{
   GetCurrentUndoMode();

   m_CurInfo->m_Limit = !m_CurInfo->m_Limit;

   if (m_CurInfo->m_Limit)
   {
      m_CurInfo->m_LimitMaxNodes = true;
      m_CurInfo->m_LimitMaxSize = false;
      m_CurInfo->m_LimitMinNodes = false;
   }
   else
   {
      m_CurInfo->m_LimitMaxNodes = false;
      m_CurInfo->m_LimitMaxSize = false;
      m_CurInfo->m_LimitMinNodes = false;
   }

   UpdateSpinEdits();
}

void CUndoConfigPage::OnMinundoscheck() 
{
   GetCurrentUndoMode();
   m_CurInfo->m_LimitMinNodes = !m_CurInfo->m_LimitMinNodes;
	UpdateSpinEdits();
}

void CUndoConfigPage::OnMaxundosradio() 
{
   GetCurrentUndoMode();
   m_CurInfo->m_LimitMaxNodes = true;
   m_CurInfo->m_LimitMaxSize = false;
	UpdateSpinEdits();
}

void CUndoConfigPage::OnMaxundosizeradio() 
{
   GetCurrentUndoMode();
   m_CurInfo->m_LimitMaxNodes = false;
   m_CurInfo->m_LimitMaxSize = true;
	UpdateSpinEdits();
}
