#if !defined(AFX_ATABCTRL_H__66175373_C248_11D2_8362_0040332C7B2F__INCLUDED_)
#define AFX_ATABCTRL_H__66175373_C248_11D2_8362_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ATabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoTabCtrl window

class CAutoTabCtrl : public CTabCtrl
{
// Construction
public:
	CAutoTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	int ResizeControlToLargest();
	CWnd* GetCurrentDlg();
	virtual ~CAutoTabCtrl();
	int AddTab(char *Title, CDialog *Dlg, unsigned int IDTemplate);
	int ResizeChildDialog(CDialog *Dlg, RECT *lpRect = NULL);
	int ResizeCurrentDialog(RECT *lpRect = NULL);
	int ResizeTabCtrl(int Width, int Height);
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CAutoTabCtrl)
	afx_msg void OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATABCTRL_H__66175373_C248_11D2_8362_0040332C7B2F__INCLUDED_)
