#if !defined(AFX_SKINDLG_H__99B94126_CBD7_11D2_8385_0040332C7B2F__INCLUDED_)
#define AFX_SKINDLG_H__99B94126_CBD7_11D2_8385_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinDlg.h : header file
//

#include "ATabCtrl.h"
#include "SModTab.h"
#include "SBotmDlg.h"
#include "SToolDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSkinEditorDlg dialog

class CSkinEditorDlg : public CDialog
{
// Construction
public:
   void MouseDown(CMouseState Button, CShiftState Shift, int X, int Y);
   void MouseUp(CMouseState Button, CShiftState Shift, int X, int Y);
	CSkinEditorDlg(CWnd* pParent = NULL);   // standard constructor
   unsigned int ToggleMenuChecked(unsigned int IDItem);

   CWnd  *m_MainDlg;

// Dialog Data
	//{{AFX_DATA(CSkinEditorDlg)
	enum { IDD = IDD_SKIN_MAIN_DIALOG };
	CScrollBar	m_VertScroll;
	CScrollBar	m_HorizScroll;
	CAutoTabCtrl	m_ToolTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinEditorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetViewportHeight();
	void ToolButClick(HWND Wnd);
	int GetPanelWidth();
	void DoCursors(CShiftState Shift);
	void DoSync();
	void CheckScrollBars();
	void UpdateSkin();
	unsigned int IsMenuChecked(unsigned int IDItem);

   CSkinModifyTab       m_ModifyTab;
   CSkinBottomToolDlg   m_BottomToolDlg;
   CSkinSideToolDlg     m_SideToolDlg;
   CShiftState    m_KeyState;

   CMouseState    m_ButtonState;
   CShiftState    m_ShiftState;
   HWND       	   m_CurTool, m_LastTool;

   TDIBitmap8     m_SkinPic;
               
   double         m_SkinDiffAng, m_SkinDiffX, m_SkinDiffY;

   int            m_PanX, m_PanY;
protected:


   // Generated message map functions
	//{{AFX_MSG(CSkinEditorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar);
	afx_msg void OnOptionsLinesDontdraw();
	afx_msg void OnOptionsLinesNormal();
	afx_msg void OnOptionsVerticesNone();
	afx_msg void OnOptionsVerticesDots();
	afx_msg void OnOptionsVerticesTicks();
	afx_msg void OnLButtonDown(unsigned int nFlags, CPoint point);
	afx_msg void OnLButtonUp(unsigned int nFlags, CPoint point);
	afx_msg void OnRButtonDown(unsigned int nFlags, CPoint point);
	afx_msg void OnRButtonUp(unsigned int nFlags, CPoint point);
	afx_msg void OnMButtonDown(unsigned int nFlags, CPoint point);
	afx_msg void OnMButtonUp(unsigned int nFlags, CPoint point);
	afx_msg void OnMouseMove(unsigned int nFlags, CPoint point);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnKeyDown(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
	afx_msg void OnKeyUp(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
	afx_msg void OnEditSelectall();
	afx_msg void OnEditSelectconnected();
	afx_msg void OnEditSelectinverse();
	afx_msg void OnEditSelectnone();
	afx_msg void OnEditSelecttouching();
	afx_msg void OnOptionsSync3dselection();
	afx_msg void OnFileImportSkinimage();
	afx_msg void OnFileImportSkinmeshfromasc();
	afx_msg void OnFileExportCurrentimageBmp();
	afx_msg void OnFileExportCurrentimagePcx();
	afx_msg void OnFileExportSkinmeshtoasc();
	afx_msg void OnEditGetpositionfrommodel();
	afx_msg void OnEditAddnewskin();
	afx_msg void OnEditDeletecurrentskin();
	afx_msg void OnEditClearcurrentskin();
	afx_msg void OnEditResizeskin();
	afx_msg void OnViewSkinpainter();
	afx_msg void OnView3deditor();
	afx_msg void OnOptionsKeepwindowontop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINDLG_H__99B94126_CBD7_11D2_8385_0040332C7B2F__INCLUDED_)
