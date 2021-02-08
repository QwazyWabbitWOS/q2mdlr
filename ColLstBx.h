#if !defined(AFX_COLLSTBX_H__ADF3E908_D68B_11D2_83B3_0040332C7B2F__INCLUDED_)
#define AFX_COLLSTBX_H__ADF3E908_D68B_11D2_83B3_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColLstBx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorListBoxCtrl window

class CColorListBoxCtrl : public CListBox
{
// Construction
public:
	CColorListBoxCtrl();

// Attributes
public:
   BYTE  (*m_Palette)[3];
   bool  m_SettingFore;
   int   m_ItemWidth, m_ItemHeight;

// Operations
public:
   void AutoAdjustItemWidthHeight();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorListBoxCtrl)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorListBoxCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorListBoxCtrl)
	afx_msg void OnLButtonDown(unsigned int nFlags, CPoint point);
	afx_msg void OnRButtonDown(unsigned int nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLLSTBX_H__ADF3E908_D68B_11D2_83B3_0040332C7B2F__INCLUDED_)
