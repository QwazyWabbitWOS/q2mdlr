#if !defined(AFX_MODTAB_H__66175379_C248_11D2_8362_0040332C7B2F__INCLUDED_)
#define AFX_MODTAB_H__66175379_C248_11D2_8362_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModifyTab dialog

class CModifyTab : public CDialog
{
// Construction
public:
	void ResetSpinners();
	void CheckSpinners();
	CModifyTab(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModifyTab)
	enum { IDD = IDD_TABDLG_MODIFY };
	CButton	m_ZBut;
	CButton	m_YBut;
	CButton	m_XBut;
	CButton	m_EffectFrameRangeCheck;
	CButton	m_ExtrudeBut;
	CButton	m_FlipNormalsBut;
	CButton	m_MirrorBut;
	CButton	m_MoveBut;
	CButton	m_RotateBut;
	CButton	m_ScaleBut;
	CButton	m_SelectBut;
	CSpinButtonCtrl	m_ModToSpin;
	CEdit	m_ModToEdit;
	CSpinButtonCtrl	m_ModFromSpin;
	CEdit	m_ModFromEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModifyTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnFlipNormalsBut();
	afx_msg void OnWeldbut();
	afx_msg void OnMirrorbut();
	afx_msg void OnDelselectedbut();
	afx_msg void OnSubdividebut();
	afx_msg void OnExtudebut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODTAB_H__66175379_C248_11D2_8362_0040332C7B2F__INCLUDED_)
