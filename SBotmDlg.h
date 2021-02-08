#if !defined(AFX_SBOTMDLG_H__99B94128_CBD7_11D2_8385_0040332C7B2F__INCLUDED_)
#define AFX_SBOTMDLG_H__99B94128_CBD7_11D2_8385_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SBotmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinBottomToolDlg dialog

class CSkinBottomToolDlg : public CDialog
{
// Construction
public:
	int GetHeight();
   BOOL SetSelectionType(SelectType SelType);
	CSkinBottomToolDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSkinBottomToolDlg)
	enum { IDD = IDD_SKIN_TOOLDLG_BOTTOM };
	CEdit	m_YEdit;
	CEdit	m_XEdit;
	CButton	m_VertexModeBut;
	CButton	m_ObjectModeBut;
	CButton	m_FaceModeBut;
	CEdit	m_FeebackEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinBottomToolDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSkinBottomToolDlg)
	afx_msg void OnVertexmodebut();
	afx_msg void OnFacemodebut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SBOTMDLG_H__99B94128_CBD7_11D2_8385_0040332C7B2F__INCLUDED_)
