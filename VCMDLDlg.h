// VCMDLDlg.h : header file
//

#if !defined(AFX_VCMDLDLG_H__D4A35A69_C189_11D2_8361_0040332C7B2F__INCLUDED_)
#define AFX_VCMDLDLG_H__D4A35A69_C189_11D2_8361_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ATabCtrl.h"
#include "CreatTab.h"
#include "ViewTab.h"
#include "BonesTab.h"
#include "ModTab.h"
#include "SideDlg.h"
#include "BottmDlg.h"  

#include "ConDlg.h"        // Console Output
#include "SkinDlg.h"       // Skin Edtior
#include "PaintDlg.h"      // Skin painter
#include "TypeDlg.h"       // Numeric Typerinierer

#include "UNITS\MdlConst.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CVCMDLDlg dialog

class CVCMDLDlg : public CDialog
{
// Construction
public:
	unsigned int IsMenuChecked(unsigned int IDItem);
   unsigned int ToggleMenuChecked(unsigned int IDItem);
	void ToolButClick(HWND But);
	void DoSync();
	void DoCursors(CShiftState Shift);
	ViewType GetViewFromCursor(int X, int Y);
	void MouseDown(CMouseState Button, CShiftState Shift, int X, int Y);
   void MouseUp(CMouseState Button, CShiftState Shift, int X, int Y);
	void SaveConfigToReg();
	void LoadConfigFromReg();
	void FillViewportWidthHeight(ViewType View, int &w, int &h, int &w2, int &h2);
   void FillViewportWidthHeightOp(ViewType View, int &w, int &h, int &w2, int &h2);
	int GetPanelWidth();
	void InitDrawInfo();
	void DoViewPortSize();
	void CalcViewportWidthHeight();
	void DoResize();
	void DrawAllViews();
	void CheckSpinners();
	void WasOnShowWindow();
	CVCMDLDlg(CWnd* pParent = NULL);	// standard constructor

   void AddLineToConsole(const char *Text);

// 
   bool m_Ready,m_Closing;

// Dialog Data
	//{{AFX_DATA(CVCMDLDlg)
	enum { IDD = IDD_VCMDL_DIALOG };
	CAutoTabCtrl	m_ToolTab;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVCMDLDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

public:
	void UpdateMenuMRU();
	void AddToMRUList(CString Str);
	HICON	         m_hIcon;
	CBonesTab	   m_BonesTab;
	CCreateTab	   m_CreateTab;
	CViewTab       m_ViewTab;
	CModifyTab	   m_ModifyTab;
   CSideToolDlg   m_SideToolDlg;
   CBottomDlg     m_BottomToolDlg;

   float          m_MidpointX, m_MidpointY;                 // midpoint, as fraction of 1.0

   CDrawViewInfo  m_DrawInfo;
   CMouseState    m_ButtonState;
   CShiftState    m_ShiftState;
   HWND       	   m_CurTool, m_LastTool;

   unsigned int           m_PlayTimerID;
   CShiftState    m_KeyState;

   CSkinEditorDlg m_SkinEditDlg;
   CPaintDlg      m_PaintDlg;
   CConsoleDlg    m_ConsoleDlg;
   CTypeInDlg    m_TypeInDlg;

	// Generated message map functions
	//{{AFX_MSG(CVCMDLDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(unsigned int nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnShowWindow(BOOL bShow, unsigned int nStatus);
	afx_msg void OnLButtonDown(unsigned int nFlags, CPoint point);
	afx_msg void OnKeyDown(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
	afx_msg void OnLButtonUp(unsigned int nFlags, CPoint point);
	afx_msg void OnMButtonDown(unsigned int nFlags, CPoint point);
	afx_msg void OnMButtonUp(unsigned int nFlags, CPoint point);
	afx_msg void OnRButtonDown(unsigned int nFlags, CPoint point);
	afx_msg void OnRButtonUp(unsigned int nFlags, CPoint point);
	afx_msg void OnMouseMove(unsigned int nFlags, CPoint point);
	afx_msg void OnFileOpen();
	afx_msg void OnTimer(unsigned int nIDEvent);
	afx_msg void OnOptions3DViewWireframe();
	afx_msg void OnOptions3dviewTexturemapped();
	afx_msg void OnOptions3dviewTexturegourad();
	afx_msg void OnOptions3dviewGouradshaded();
	afx_msg void OnOptions3dviewFlatshaded();
	afx_msg void OnOptionsShowgrid();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditSelectall();
	afx_msg void OnEditSelectconnected();
	afx_msg void OnEditSelectinverse();
	afx_msg void OnEditSelectnone();
	afx_msg void OnEditSelecttouching();
	afx_msg void OnClose();
	afx_msg void OnOptions2dviewDrawbackfaces();
	afx_msg void OnOptions3dviewDrawbackfaces();
	afx_msg void OnOptionsVerticesticks();
	afx_msg void OnOptionsShoworigin();
	afx_msg void OnFileNew();
	afx_msg void OnFileSaveas();
	afx_msg void OnFileSave();
	afx_msg void OnFileImportImportAppendVerts();
	afx_msg void OnFileImportImportAppendFrames();
	afx_msg void OnKeyUp(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
	afx_msg void OnOptions2dviewPlaycameraxz();
	afx_msg void OnOptions2dviewPlaycamerazy();
	afx_msg void OnOptions2dviewPlaycameraxy();
	afx_msg void OnOptionsShownormalsNone();
	afx_msg void OnOptionsShownormalsAll();
	afx_msg void OnOptionsShownormalsSelectedfaces();
	afx_msg void OnFileExportExporttomd2();
	afx_msg void OnFileExportExporttomdl();
	afx_msg void OnFileExportExportframeto3ds();
	afx_msg void OnFileExportExportframetoasc();
	afx_msg void OnFileExportExportmodelto3ds();
	afx_msg void OnFileExportExportmodeltoasc();
	afx_msg void OnFileImportFramenamesfromtext();
	afx_msg void OnOptionsDecreasegridsize();
	afx_msg void OnOptionsIncreasegridsize();
	afx_msg void OnOptionsZoomin();
	afx_msg void OnOptionsZoomout();
	afx_msg void OnOptionsConfigure();
	afx_msg void OnViewSkins();
	afx_msg void OnEditCopyselected();
	afx_msg void OnEditPaste();
	afx_msg void OnEditPastetorange();
	afx_msg void OnEditChangeframename();
	afx_msg void OnEditGotoframe();
	afx_msg void OnEditAddnewframe();
	afx_msg void OnEditMoveframes();
	afx_msg void OnEditDeletecurrentframe();
	afx_msg void OnEditDeleteframes();
	afx_msg void OnOptionsSyncskinselection();
	afx_msg void OnOptionsGroundplaneposition();
	afx_msg void OnToolsMemowindow();
	afx_msg void OnToolsOutputboundingbox();
	afx_msg void OnViewSkinpainter();
	afx_msg void OnFileChoosereferencemodel();
	afx_msg void OnFileClearreferencemodel();
	afx_msg void OnOptions3dviewShowreferencemodel();
	afx_msg void OnToolsNumerictypein();
	afx_msg void OnViewModelproperties();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCMDLDLG_H__D4A35A69_C189_11D2_8361_0040332C7B2F__INCLUDED_)
