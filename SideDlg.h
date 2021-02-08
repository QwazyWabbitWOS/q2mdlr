#if !defined(AFX_SIDEDLG_H__1F259411_C30F_11D2_8363_0040332C7B2F__INCLUDED_)
#define AFX_SIDEDLG_H__1F259411_C30F_11D2_8363_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SideDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSideToolDlg dialog

class CSideToolDlg : public CDialog
{
// Construction
public:
	int GetWidth();
	CSideToolDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSideToolDlg)
	enum { IDD = IDD_TOOLDLG_SIDE };
	CButton	m_PrevFrameBut;
	CButton	m_NextFrameBut;
	CEdit	m_FrameNumEdit;
	CEdit	m_FrameNameEdit;
	CButton	m_FitSelectedBut;
	CButton	m_FitAllBut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSideToolDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSideToolDlg)
	afx_msg void OnFitAllBut();
	afx_msg void OnFitSelectedBut();
	afx_msg void OnNextFrameBut();
	afx_msg void OnPrevFrameBut();
	afx_msg void OnDoubleclickedNextframebut();
	afx_msg void OnDoubleclickedPrevframebut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIDEDLG_H__1F259411_C30F_11D2_8363_0040332C7B2F__INCLUDED_)
