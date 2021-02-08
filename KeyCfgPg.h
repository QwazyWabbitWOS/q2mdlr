#if !defined(AFX_KEYCFGPG_H__B94D0E61_C81B_11D2_8374_0040332C7B2F__INCLUDED_)
#define AFX_KEYCFGPG_H__B94D0E61_C81B_11D2_8374_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyCfgPg.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CKeyConfigPage dialog

class CKeyConfigPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CKeyConfigPage)

// Construction
public:
	void UpdateFromKeystroke(CKeystroke *K);
	CKeyConfigPage();
	~CKeyConfigPage();

   CCommandList   m_KeyListCopy;
   unsigned int           m_Key;
// Dialog Data
	//{{AFX_DATA(CKeyConfigPage)
	enum { IDD = IDD_CONFIG_KEY_PAGE };
	CButton	m_AltCheck;
	CButton	m_AssignBut;
	CComboBox	m_CategoryCombo;
	CEdit	m_CurShortcutKeyEdit;
	CButton	m_PressKeyBut;
	CButton	m_ShiftCheck;
	CEdit	m_UsedEdit;
	CEdit	m_KeyEdit;
	CButton	m_CtrlCheck;
	CListBox	m_CommandList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CKeyConfigPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   CString  m_CurCategory;
   
   
   // Generated message map functions
	//{{AFX_MSG(CKeyConfigPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCommandlist();
	afx_msg void OnPresskeybut();
	afx_msg void OnAssignbut();
	afx_msg void OnEditchangeCategorycombo();
	afx_msg void OnCloseupCategorycombo();
	afx_msg void OnSelchangeCategorycombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYCFGPG_H__B94D0E61_C81B_11D2_8374_0040332C7B2F__INCLUDED_)
