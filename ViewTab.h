#if !defined(AFX_VIEWTAB_H__66175378_C248_11D2_8362_0040332C7B2F__INCLUDED_)
#define AFX_VIEWTAB_H__66175378_C248_11D2_8362_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewTab dialog

class CViewTab : public CDialog
{
// Construction
public:
	void ResetSpinners();
	void CheckSpinners();
	CViewTab(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewTab)
	enum { IDD = IDD_TABDLG_VIEW };
	CButton	m_InterpolateBut;
	CButton	m_PlayBut;
	CEdit	m_FPSEdit;
	CSpinButtonCtrl	m_FPSSpin;
	CSpinButtonCtrl	m_ToAnimSpin;
	CEdit	m_ToAnimEdit;
	CSpinButtonCtrl	m_FromAnimSpin;
	CEdit	m_FromAnimEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnPlayCamBut();
	afx_msg void OnInterpolateBut();
	afx_msg void OnDeltaposViewFpsspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposViewFromspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposViewTospin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHideselectedbut();
	afx_msg void OnHideunselectedbut();
	afx_msg void OnUnhideallbut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWTAB_H__66175378_C248_11D2_8362_0040332C7B2F__INCLUDED_)
