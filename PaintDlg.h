#if !defined(AFX_PAINTDLG_H__380A51F3_D4C6_11D2_83B1_0040332C7B2F__INCLUDED_)
#define AFX_PAINTDLG_H__380A51F3_D4C6_11D2_83B1_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaintDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaintDlg dialog

#include "ATabCtrl.h"
#include "PaintTab.h"
#include "DIBFuncs.h"

#include "MdlConst.h"

#include "UndoSkin.h"
/////////////////////////////////////////////////////////////////////////////
// CPaintBottomDlg dialog

class CPaintBottomDlg : public CDialog
{
// Construction
public:
	CPaintBottomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPaintBottomDlg)
	enum { IDD = IDD_PAINT_TOOLDLG_BOTTOM };
	CEdit	m_VEdit;
	CEdit	m_UEdit;
	CButton	m_PanViewsBut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaintBottomDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPaintBottomDlg)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CPaintDlg : public CDialog
{
// Construction
public:
	void MouseMove(CShiftState Shift, int X, int Y);
	void ToolButClick(HWND But);
	bool InClientRect(CPoint point);
   void PaintJustUVs();
	void SkinDot(int x, int y, unsigned char col, float XParent, bool UseIntermediate, int Style);
   void DoCursors(CShiftState Shift);
	void UpdateColors();
	void MouseUp(CShiftState Shift, int X, int Y);
	void MouseDown(CShiftState Shift, int X, int Y);
	void MakePaintingData();
	void LoadPaintData();
	void DoResize();
	int SetUVDataSize(int x, int y);
	void DrawSkinPaint3DView(TDIBitmap8 &Dest);
	CPaintDlg(CWnd* pParent = NULL);   // standard constructor
   ~CPaintDlg();

   CWnd              *m_MainDlg;
   CPaintBottomDlg   m_BottomToolDlg;
   CPaintPaintTab    m_PaintTab;

   CShiftState       m_ShiftState;
   HWND       	      m_CurTool, m_LastTool;

   RECT              m_DrawExtents, m_LastExtents, m_CurExtents;
   TDIBitmap8        m_View;
   BYTE              **m_UVData;

   int               m_ViewWidth, m_ViewHeight, m_ViewWidth2, m_ViewHeight2;

   BYTE              *m_IntermediateSkin, *m_IntermediateShade, *m_XParent;
   double            m_DrawTransparency, m_DrawTransStep;
   bool              m_FadeToBack, m_FadeToXParent, m_FlipCol;

   double            m_InterRed, m_InterGreen,m_InterBlue, m_RStep, m_GStep, m_BStep,m_InterValue,m_VStep;

// Dialog Data
	//{{AFX_DATA(CPaintDlg)
	enum { IDD = IDD_PAINT_MAIN_DIALOG };
	CAutoTabCtrl	m_ToolTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaintDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
   CSkinPaintUndoNode   *m_PaintUndoNode;

	// Generated message map functions
	//{{AFX_MSG(CPaintDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnKeyDown(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
	afx_msg void OnKeyUp(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(unsigned int nFlags, CPoint point);
	afx_msg void OnLButtonDown(unsigned int nFlags, CPoint point);
	afx_msg void OnLButtonUp(unsigned int nFlags, CPoint point);
	afx_msg void OnMButtonDown(unsigned int nFlags, CPoint point);
	afx_msg void OnMButtonUp(unsigned int nFlags, CPoint point);
	afx_msg void OnRButtonDown(unsigned int nFlags, CPoint point);
	afx_msg void OnRButtonUp(unsigned int nFlags, CPoint point);
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnView3deditor();
	afx_msg void OnViewSkinpainter();
	afx_msg void OnViewSkineditor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_PAINTDLG_H__380A51F3_D4C6_11D2_83B1_0040332C7B2F__INCLUDED_)
