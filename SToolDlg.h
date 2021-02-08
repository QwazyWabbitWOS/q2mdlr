#if !defined(AFX_STOOLDLG_H__99B94127_CBD7_11D2_8385_0040332C7B2F__INCLUDED_)
#define AFX_STOOLDLG_H__99B94127_CBD7_11D2_8385_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SToolDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinSideToolDlg dialog

class CSkinSideToolDlg : public CDialog
{
// Construction
public:
	void UpdateFrameLabels();
	int GetWidth();
   CSkinSideToolDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSkinSideToolDlg)
	enum { IDD = IDD_SKIN_TOOLDLG_SIDE };
	CButton	m_ZoomOutBut;
	CButton	m_ZoomInBut;
	CButton	m_PrevSkinBut;
	CButton	m_NextSkinBut;
	CEdit	m_SkinNumEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinSideToolDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSkinSideToolDlg)
	afx_msg void OnZoominbut();
	afx_msg void OnZoomoutbut();
	afx_msg void OnNextskinbut();
	afx_msg void OnPrevskinbut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOOLDLG_H__99B94127_CBD7_11D2_8385_0040332C7B2F__INCLUDED_)
