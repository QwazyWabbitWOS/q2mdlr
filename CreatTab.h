#if !defined(AFX_CREATTAB_H__6617537A_C248_11D2_8362_0040332C7B2F__INCLUDED_)
#define AFX_CREATTAB_H__6617537A_C248_11D2_8362_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreatTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreateTab dialog

class CCreateTab : public CDialog
{
// Construction
public:
	CCreateTab(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateTab)
	enum { IDD = IDD_TABDLG_CREATE };
	CButton	m_CreateVertexBut;
	CButton	m_CreateFanBut;
	CButton	m_CreateStripBut;
	CButton	m_BuildFaceBut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateTab)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATTAB_H__6617537A_C248_11D2_8362_0040332C7B2F__INCLUDED_)
