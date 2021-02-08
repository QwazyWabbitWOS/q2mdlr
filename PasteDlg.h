#if !defined(AFX_PASTEDLG_H__58EE9D53_CEC7_11D2_8394_0040332C7B2F__INCLUDED_)
#define AFX_PASTEDLG_H__58EE9D53_CEC7_11D2_8394_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPasteMethodDlg dialog

class CPasteMethodDlg : public CDialog
{
// Construction
public:
	CPasteMethodDlg(CWnd* pParent = NULL);   // standard constructor

   bool     m_ReplaceVerts;
   float    m_BlendFactor;

// Dialog Data
	//{{AFX_DATA(CPasteMethodDlg)
	enum { IDD = IDD_PASTEVERTS_DIALOG };
	CSpinButtonCtrl	m_BlendSpin;
	CEdit	m_BlendEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasteMethodDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasteMethodDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposBlendspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAppendvertsRadio();
	afx_msg void OnReplacevertsRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CPasteRangeDialog dialog

class CPasteRangeDialog : public CDialog
{
// Construction
public:
	void UpdateFrameLabels();
	CPasteRangeDialog(CWnd* pParent = NULL);   // standard constructor

   int      m_FrameStart, m_FrameEnd;
   float    m_BlendFactor;
// Dialog Data
	//{{AFX_DATA(CPasteRangeDialog)
	enum { IDD = IDD_PASTERANGE_DIALOG };
	CEdit	m_StartNumEdit;
	CEdit	m_StartNameEdit;
	CEdit	m_EndNameEdit;
	CEdit	m_EndNumEdit;
	CButton	m_ChooseStartBut;
	CButton	m_ChooseEndBut;
	CSpinButtonCtrl	m_BlendSpin;
	CEdit	m_BlendEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPasteRangeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPasteRangeDialog)
	afx_msg void OnChoosestartbut();
	afx_msg void OnChooseendbut();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposStartframespin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposEndframespin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposBlendspin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASTEDLG_H__58EE9D53_CEC7_11D2_8394_0040332C7B2F__INCLUDED_)
