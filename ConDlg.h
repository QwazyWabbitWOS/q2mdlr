#if !defined(AFX_CONDLG_H__3EAE9113_D406_11D2_83B0_0040332C7B2F__INCLUDED_)
#define AFX_CONDLG_H__3EAE9113_D406_11D2_83B0_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConsoleDlg dialog

class CConsoleDlg : public CDialog
{
// Construction
public:
	CConsoleDlg(CWnd* pParent = NULL);   // standard constructor
   void AddLineToConsole(const char *Text);

   CWnd  *m_MainDlg;

// Dialog Data
	//{{AFX_DATA(CConsoleDlg)
	enum { IDD = IDD_CONSOLE_DIALOG };
	CEdit	m_OutputEdit;
	CEdit	m_InputEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConsoleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConsoleDlg)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDLG_H__3EAE9113_D406_11D2_83B0_0040332C7B2F__INCLUDED_)
