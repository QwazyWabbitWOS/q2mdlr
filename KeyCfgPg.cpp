// KeyCfgPg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"

#include "mdlglobs.h"

#include "MenuItem.h"
#include "Keyboard.h"
#include "KeyCfgPg.h"

#include "KeyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyConfigPage property page

IMPLEMENT_DYNCREATE(CKeyConfigPage, CPropertyPage)

CKeyConfigPage::CKeyConfigPage() : CPropertyPage(CKeyConfigPage::IDD)
{
	//{{AFX_DATA_INIT(CKeyConfigPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKeyConfigPage::~CKeyConfigPage()
{
}

void CKeyConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyConfigPage)
	DDX_Control(pDX, IDC_ALTCHECK, m_AltCheck);
	DDX_Control(pDX, IDC_ASSIGNBUT, m_AssignBut);
	DDX_Control(pDX, IDC_CATEGORYCOMBO, m_CategoryCombo);
	DDX_Control(pDX, IDC_CURSHORTCUTEDIT, m_CurShortcutKeyEdit);
	DDX_Control(pDX, IDC_PRESSKEYBUT, m_PressKeyBut);
	DDX_Control(pDX, IDC_SHIFTCHECK, m_ShiftCheck);
	DDX_Control(pDX, IDC_USEDEDIT, m_UsedEdit);
	DDX_Control(pDX, IDC_KEYEDIT, m_KeyEdit);
	DDX_Control(pDX, IDC_CTRLCHECK, m_CtrlCheck);
	DDX_Control(pDX, IDC_COMMANDLIST, m_CommandList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyConfigPage, CPropertyPage)
	//{{AFX_MSG_MAP(CKeyConfigPage)
	ON_LBN_SELCHANGE(IDC_COMMANDLIST, OnSelchangeCommandlist)
	ON_BN_CLICKED(IDC_PRESSKEYBUT, OnPresskeybut)
	ON_BN_CLICKED(IDC_ASSIGNBUT, OnAssignbut)
	ON_CBN_EDITCHANGE(IDC_CATEGORYCOMBO, OnEditchangeCategorycombo)
	ON_CBN_CLOSEUP(IDC_CATEGORYCOMBO, OnCloseupCategorycombo)
	ON_CBN_SELCHANGE(IDC_CATEGORYCOMBO, OnSelchangeCategorycombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyConfigPage message handlers

int CALLBACK EnumCommandCategories(CAction *A, DWORD lParam)
{
   CKeyConfigPage *KeyConfigPage;
   CString     Category;
   int         Found;

   if (lParam == 0)
      return 0;

   KeyConfigPage = (CKeyConfigPage*)(lParam);

   Found = KeyConfigPage->m_CategoryCombo.FindStringExact(0, (LPCTSTR)A->m_Cat);
   if (Found == CB_ERR) 
   {
      KeyConfigPage->m_CategoryCombo.AddString(A->m_Cat);
   }

   return 1;
}


int CALLBACK EnumCommands(CAction *A, DWORD lParam)
{
   CKeyConfigPage    *KeyConfigPage;
   CString           Category;

   if (lParam == 0)
      return 0;

   KeyConfigPage = (CKeyConfigPage*)(lParam);
   KeyConfigPage->m_CategoryCombo.GetWindowText(Category);

   if (A->m_Cat.CompareNoCase(Category) == 0)
      KeyConfigPage->m_CommandList.AddString(A->m_Name);

   return 1;
}

BOOL CKeyConfigPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

   g_ActionList.EnumList(EnumCommandCategories, (DWORD)this);
   m_CategoryCombo.SetCurSel(0);
   m_CategoryCombo.GetWindowText(m_CurCategory);

   m_CommandList.ResetContent();
   g_ActionList.EnumList(EnumCommands, (DWORD)this);
   
   m_CurShortcutKeyEdit.SetWindowText("");
   m_UsedEdit.SetWindowText("");
   m_KeyEdit.SetWindowText("");
   m_AssignBut.EnableWindow(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKeyConfigPage::OnSelchangeCommandlist() 
{
   CString        N, Category;
   CKeystroke     *K;

   m_CommandList.GetText(m_CommandList.GetCurSel(), N);


   m_CategoryCombo.GetWindowText(Category);
  
   K = m_KeyListCopy.FindKey(N, Category);

   if (m_Key)
      m_AssignBut.EnableWindow(TRUE);
   else
      m_AssignBut.EnableWindow(FALSE);

   UpdateFromKeystroke(K);
}

void CKeyConfigPage::OnPresskeybut() 
{
   CPressKeyDlg   PressKeyDlg;


   PressKeyDlg.DoModal();

   m_AltCheck.SetCheck(PressKeyDlg.m_Alt);
   m_CtrlCheck.SetCheck(PressKeyDlg.m_Ctrl);
   m_ShiftCheck.SetCheck(PressKeyDlg.m_Shift);
   m_Key = PressKeyDlg.m_KeyPressed;

   bool A = (m_AltCheck.GetCheck() == 1);
   bool C = (m_CtrlCheck.GetCheck() == 1);
   bool S = (m_ShiftCheck.GetCheck() == 1);
   WORD K = m_Key;

   {
      char mapkey = MapVirtualKey(K,2);
      CString SC = "";

      if (A)
         SC += "Alt+";
      if (S)
         SC += "Shift+";
      if (C)
         SC += "Ctrl+";

//      if (mapkey)
//         SC += mapkey;
//      else
         SC += VKeyToString(K);

      m_KeyEdit.SetWindowText(SC);
   }


   if (m_CommandList.GetCurSel() != -1)
      m_AssignBut.EnableWindow(TRUE);
   else
      m_AssignBut.EnableWindow(FALSE);

   CKeystroke     *Ks;
   CString        CurCategory;

   m_CategoryCombo.GetWindowText(CurCategory);


   Ks = m_KeyListCopy.FindKey(K,CurCategory,C,A,S);
   if (Ks)
   {
      if (Ks->m_Act && (Ks->m_Act->m_Cat.CompareNoCase(CurCategory) == 0))
         m_UsedEdit.SetWindowText(Ks->m_Act->m_Name);
      else
         m_UsedEdit.SetWindowText("<None>");
   }
   else
      m_UsedEdit.SetWindowText("<None>");

	
}

void CKeyConfigPage::UpdateFromKeystroke(CKeystroke *K)
{
   if (K)
   {
      m_Key = K->m_Key;
      m_ShiftCheck.SetCheck(K->m_Shift);
      m_CtrlCheck.SetCheck(K->m_Ctrl);
      m_AltCheck.SetCheck(K->m_Alt);

      
      CString SC = "";

      if (K->m_Alt)
         SC += "Alt+";
      if (K->m_Shift)
         SC += "Shift+";
      if (K->m_Ctrl)
         SC += "Ctrl+";

//      if (mapkey)
//         SC += mapkey;
//      else
         SC += VKeyToString(K->m_Key);

      m_CurShortcutKeyEdit.SetWindowText(SC);
   }
   else
   {
      m_CurShortcutKeyEdit.SetWindowText("< undefined >");
   }

}

void CKeyConfigPage::OnAssignbut() 
{
   CString        CurCategory;
   m_CategoryCombo.GetWindowText(CurCategory);

   {
      bool A = (m_AltCheck.GetCheck() == 1);
      bool C = (m_CtrlCheck.GetCheck() == 1);
      bool S = (m_ShiftCheck.GetCheck() == 1);
      WORD K = m_Key;


      if (m_CommandList.GetCurSel() != -1)
         m_AssignBut.EnableWindow(TRUE);
      else
         m_AssignBut.EnableWindow(FALSE);

      CKeystroke   *Ks;

      Ks = m_KeyListCopy.FindKey(K,CurCategory, C,A,S);
      if (Ks)
      {
         if (Ks->m_Act)
            m_UsedEdit.SetWindowText(Ks->m_Act->m_Name);
         else
            m_UsedEdit.SetWindowText("");
      }
      else
         m_UsedEdit.SetWindowText("");

      if (Ks)
      {
         if (Ks->m_Act)
            Ks->m_Erase = Ks->m_Act->m_Menu;
            Ks->m_Act = NULL;
            Ks->m_Key = 0;
//         m_KeyListCopy.Remove(Ks);
      }

      m_UsedEdit.SetWindowText("");
   }

   CKeystroke   *K;
   CString  N;

   m_CommandList.GetText(m_CommandList.GetCurSel(), N);

   K = m_KeyListCopy.FindKey(N, CurCategory);
   bool A = (m_AltCheck.GetCheck() == 1);
   bool C = (m_CtrlCheck.GetCheck() == 1);
   bool S = (m_ShiftCheck.GetCheck() == 1);
   WORD Key = m_Key;


   if (K)
   {
      K->m_Key = Key;
      K->m_Alt = A;
      K->m_Ctrl = C;
      K->m_Shift = S;
   }
   else
   {
      m_KeyListCopy.Add(N,CurCategory, Key, C, A , S);
   }
   m_AssignBut.EnableWindow(FALSE);

   m_Key = 0;

   m_KeyEdit.SetWindowText("");
   m_ShiftCheck.SetCheck(0);
   m_AltCheck.SetCheck(0);
   m_CtrlCheck.SetCheck(0);

   OnSelchangeCommandlist();
}

void CKeyConfigPage::OnEditchangeCategorycombo() 
{
	
}

void CKeyConfigPage::OnCloseupCategorycombo() 
{
}

void CKeyConfigPage::OnSelchangeCategorycombo() 
{
   m_CommandList.ResetContent();
   int i = m_CategoryCombo.GetCurSel();
   m_CategoryCombo.SetCurSel(i);
   g_ActionList.EnumList(EnumCommands, (DWORD)this);
	
	
}
