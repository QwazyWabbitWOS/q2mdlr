#if !defined(AFX_EXTYPDLG_H__72191484_C723_11D2_8371_0040332C7B2F__INCLUDED_)
#define AFX_EXTYPDLG_H__72191484_C723_11D2_8371_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExTypDlg.h : header file
//

#define MOD_EXP_CANCEL     0
#define MOD_EXP_NUMBER     1
#define MOD_EXP_FRAMENAME  2

/////////////////////////////////////////////////////////////////////////////
// CExportNamingDlg dialog

class CExportNamingDlg : public CDialog
{
// Construction
public:
	unsigned int GetExportFilename();
	CExportNamingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExportNamingDlg)
	enum { IDD = IDD_EXPORTNAMINGDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportNamingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportNamingDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTYPDLG_H__72191484_C723_11D2_8371_0040332C7B2F__INCLUDED_)
