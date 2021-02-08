#if !defined(AFX_SMODTAB_H__99B94129_CBD7_11D2_8385_0040332C7B2F__INCLUDED_)
#define AFX_SMODTAB_H__99B94129_CBD7_11D2_8385_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SModTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinModifyTab dialog

class CSkinModifyTab : public CDialog
{
// Construction
public:
	void ToolButClick();
	CSkinModifyTab(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSkinModifyTab)
	enum { IDD = IDD_SKIN_TABDLG_MODIFY };
	CButton	m_YBut;
	CButton	m_XBut;
	CButton	m_WeldVertsBut;
	CButton	m_SelectBut;
	CButton	m_ScaleBut;
	CButton	m_RotateBut;
	CButton	m_MoveBut;
	CButton	m_MirrorBut;
	CButton	m_DetachBut;
	CButton	m_DeleteSelBut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinModifyTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSkinModifyTab)
	afx_msg void OnMirrorbut();
	afx_msg void OnWeldvertsbut();
	afx_msg void OnDetachbut();
	afx_msg void OnDeleteselbut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMODTAB_H__99B94129_CBD7_11D2_8385_0040332C7B2F__INCLUDED_)
