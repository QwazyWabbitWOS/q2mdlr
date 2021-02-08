#if !defined(AFX_KEYDLG_H__BBD2B131_CB2D_11D2_8382_0040332C7B2F__INCLUDED_)
#define AFX_KEYDLG_H__BBD2B131_CB2D_11D2_8382_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPressKeyDlg dialog

class CPressKeyDlg : public CDialog
{
// Construction
public:
	CPressKeyDlg(CWnd* pParent = NULL);   // standard constructor

   unsigned int  m_KeyPressed;
   bool  m_Ctrl, m_Alt, m_Shift;

// Dialog Data
	//{{AFX_DATA(CPressKeyDlg)
	enum { IDD = IDD_PRESSKEY_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPressKeyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPressKeyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYDLG_H__BBD2B131_CB2D_11D2_8382_0040332C7B2F__INCLUDED_)
