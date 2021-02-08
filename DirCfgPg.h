#if !defined(AFX_DIRCFGPG_H__CF1B36F8_D9A6_11D2_83B7_0040332C7B2F__INCLUDED_)
#define AFX_DIRCFGPG_H__CF1B36F8_D9A6_11D2_83B7_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirCfgPg.h : header file
//

#include "SHTree.h"

/////////////////////////////////////////////////////////////////////////////
// CDirConfigPage dialog

class CDirConfigPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDirConfigPage)

      // Construction
public:
	CDirConfigPage();   // standard constructor

   CShellTree     *m_PathTree;
   CString        m_BasePath;

// Dialog Data
	//{{AFX_DATA(CDirConfigPage)
	enum { IDD = IDD_CONFIG_DIR_PAGE };
	CEdit	m_PathEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirConfigPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDirConfigPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemexpandingPathtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitemPathtree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedPathtree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRCFGPG_H__CF1B36F8_D9A6_11D2_83B7_0040332C7B2F__INCLUDED_)
