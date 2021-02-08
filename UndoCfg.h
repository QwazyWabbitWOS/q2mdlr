#if !defined(AFX_UNDOCFG_H__E1055743_DE84_11D2_83C3_0040332C7B2F__INCLUDED_)
#define AFX_UNDOCFG_H__E1055743_DE84_11D2_83C3_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UndoCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUndoConfigPage dialog

class CUndoLimitInfo
{
public:
	int   m_NodeCount, m_NodeSize;
   int   m_MaxNodes,m_MinNodes,m_MaxSize;
   bool  m_Limit, m_LimitMaxSize, m_LimitMinNodes, m_LimitMaxNodes;

   void GetFromUndoList(CUndoList *List);
   void SetToUndoList(CUndoList *List);
};

class CUndoConfigPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CUndoConfigPage)

// Construction
public:
	void GetCurrentUndoMode();
	void UpdateSpinEdits();
	CUndoConfigPage();
	~CUndoConfigPage();

   CUndoLimitInfo    m_MainInfo, m_SkinInfo, m_PaintInfo, *m_CurInfo;

// Dialog Data
	//{{AFX_DATA(CUndoConfigPage)
	enum { IDD = IDD_CONFIG_UNDO_PAGE };
	CEdit	m_UndoSizeEdit;
	CEdit	m_UndoLevelsEdit;
	CEdit	m_MinUndosEdit;
	CEdit	m_MaxUndoSizeEdit;
	CEdit	m_MaxUndosEdit;
	CTabCtrl	m_UndoModeTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUndoConfigPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUndoConfigPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposMinundosspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposMaxundosspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposMaxundosizespin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeUndomodeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLimitundoscheck();
	afx_msg void OnMinundoscheck();
	afx_msg void OnMaxundosradio();
	afx_msg void OnMaxundosizeradio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNDOCFG_H__E1055743_DE84_11D2_83C3_0040332C7B2F__INCLUDED_)
