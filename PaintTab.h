#if !defined(AFX_PAINTTAB_H__ADF3E906_D68B_11D2_83B3_0040332C7B2F__INCLUDED_)
#define AFX_PAINTTAB_H__ADF3E906_D68B_11D2_83B3_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaintTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaintPaintTab dialog

#include "ColLstBx.h"
#include "ClrButn.h"

class CPaintPaintTab : public CDialog
{
// Construction
public:
	void UpdateSpinEdits();
	CPaintPaintTab(CWnd* pParent = NULL);   // standard constructor

   unsigned int     m_ForeColor, m_BackColor;
   int      m_DrawBrushSize;

// Dialog Data
	//{{AFX_DATA(CPaintPaintTab)
	enum { IDD = IDD_PAINT_TABDLG_PAINT };
	CSpinButtonCtrl	m_SoftSpin;
	CEdit	m_SoftEdit;
	CSpinButtonCtrl	m_SizeSpin;
	CEdit	m_SizeEdit;
	CSliderCtrl	m_OpacitySlider;
	CEdit	m_OpacityEdit;
	CComboBox	m_FadeToCombo;
	CSliderCtrl	m_FadeSlider;
	CEdit	m_FadeEdit;
	CComboBox	m_DrawEffectCombo;
	CButton	m_CompoundCheck;
	CButton	m_SprayBut;
	CButton	m_FillBut;
	CButton	m_DrawBut;
	CColorbutton	m_ForeBut;
	CColorbutton	m_BackBut;
	CColorListBoxCtrl	m_ColorListbox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaintPaintTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPaintPaintTab)
	afx_msg void OnMouseMove(unsigned int nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSelchangeColorlistbox();
	afx_msg void OnDeltaposSizespin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar);
	afx_msg void OnSwapcolsbut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAINTTAB_H__ADF3E906_D68B_11D2_83B3_0040332C7B2F__INCLUDED_)
