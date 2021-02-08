#if !defined(AFX_TYPEDLG_H__F974D311_DC01_11D2_83BD_0040332C7B2F__INCLUDED_)
#define AFX_TYPEDLG_H__F974D311_DC01_11D2_83BD_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTypeInDlg dialog
class CTypeInDlg;

#include "MdlConst.h"

enum  CTypeInTarget {tiModel, tiSkin};

class CTypeInDlg : public CDialog
{
// Construction
public:
	void EnableAll(BOOL State = TRUE);
   void EnableOrigin(BOOL State = TRUE);
	void UpdateSpinEdits();
	CTypeInDlg(CWnd* pParent = NULL);   // standard constructor

   void SetCurrentTool(ActionType Action, CTypeInTarget Target = tiModel);

   float m_X, m_Y, m_Z, m_OX, m_OY, m_OZ;
   ActionType     m_CurAction;
   CTypeInTarget  m_Target;

   CWnd     *m_MainDlg;

// Dialog Data
	//{{AFX_DATA(CTypeInDlg)
	enum { IDD = IDD_TYPEIN_POPUP };
	CButton	m_ApplyBut;
	CButton	m_SelectionCentreCheck;
	CButton	m_XFormGroup;
	CEdit	m_ZEdit;
	CEdit	m_YEdit;
	CEdit	m_XEdit;
	CEdit	m_OZEdit;
	CEdit	m_OYEdit;
	CEdit	m_OXEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTypeInDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTypeInDlg)
	afx_msg void OnApplybut();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusXedit();
	afx_msg void OnKillfocusYedit();
	afx_msg void OnKillfocusZedit();
	afx_msg void OnKillfocusOxedit();
	afx_msg void OnKillfocusOyedit();
	afx_msg void OnKillfocusOzedit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYPEDLG_H__F974D311_DC01_11D2_83BD_0040332C7B2F__INCLUDED_)
