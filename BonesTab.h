#if !defined(AFX_BONESTAB_H__6617537B_C248_11D2_8362_0040332C7B2F__INCLUDED_)
#define AFX_BONESTAB_H__6617537B_C248_11D2_8362_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BonesTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBonesTab dialog

class CBonesTab : public CDialog
{
// Construction
public:
	CBonesTab(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBonesTab)
	enum { IDD = IDD_TABDLG_BONES };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBonesTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBonesTab)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BONESTAB_H__6617537B_C248_11D2_8362_0040332C7B2F__INCLUDED_)
