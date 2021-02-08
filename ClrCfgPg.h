#if !defined(AFX_CLRCGFPG_H__FD0F7811_DBA6_11D2_83BC_0040332C7B2F__INCLUDED_)
#define AFX_CLRCGFPG_H__FD0F7811_DBA6_11D2_83BC_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClrCgfPg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorConfigPage dialog

#include "ClrButn.h"
#include "ColLstBx.h"

class CColorConfigPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CColorConfigPage)

// Construction
public:
	void UpdateColorButtons();
	CColorConfigPage();
	~CColorConfigPage();

   BYTE  *m_OldColors, *m_NewColors;

   void SetOldNewColors(BYTE *Old, BYTE *New);

// Dialog Data
	//{{AFX_DATA(CColorConfigPage)
	enum { IDD = IDD_CONFIG_COLOR_PAGE };
	CColorbutton	m_OldColorBut;
	CColorbutton	m_NewColorBut;
	CListBox	m_ColorItemList;
	CColorListBoxCtrl	m_PaletteList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CColorConfigPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CColorConfigPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeColoritemlist();
	afx_msg void OnSelchangePalettelist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLRCGFPG_H__FD0F7811_DBA6_11D2_83BC_0040332C7B2F__INCLUDED_)
