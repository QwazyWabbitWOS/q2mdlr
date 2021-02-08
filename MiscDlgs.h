#if !defined(AFX_MISCDLGS_H__5374DE57_CEE2_11D2_8395_0040332C7B2F__INCLUDED_)
#define AFX_MISCDLGS_H__5374DE57_CEE2_11D2_8395_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiscDlgs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRenameFrameDlg dialog

class CRenameFrameDlg : public CDialog
{
// Construction
public:
	CRenameFrameDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CRenameFrameDlg)
	enum { IDD = IDD_MISC_RENAME_DIALOG };
	CEdit	m_OldNameEdit;
	CEdit	m_NewNameEdit;
	CEdit	m_FrameNumEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenameFrameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRenameFrameDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CChooseFrameDlg dialog

class CChooseFrameDlg : public CDialog
{
// Construction
public:
	void UpdateFrameList();
	CChooseFrameDlg(CWnd* pParent = NULL);   // standard constructor

   int   m_SelectedFrame;
// Dialog Data
	//{{AFX_DATA(CChooseFrameDlg)
	enum { IDD = IDD_GOTOFRAME_DIALOG };
	CListCtrl	m_FrameList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChooseFrameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChooseFrameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CAddFrameDlg dialog

class CAddFrameDlg : public CDialog
{
// Construction
public:
	void UpdateFrameLabels();
	CAddFrameDlg(CWnd* pParent = NULL);   // standard constructor

   int      m_FrameSource, m_FrameTarget;
   CString  m_NewName;

// Dialog Data
	//{{AFX_DATA(CAddFrameDlg)
	enum { IDD = IDD_ADDFRAME_DIALOG };
	CEdit	m_NewFrameNumEdit;
	CEdit	m_NewFrameNameEdit;
	CEdit	m_FrameNumEdit;
	CEdit	m_FrameNameEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddFrameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddFrameDlg)
	virtual void OnOK();
	afx_msg void OnChoosebut();
	afx_msg void OnAfterframecheck();
	afx_msg void OnChangeFramenumedit();
	afx_msg void OnDeltaposFramenumspin(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDeleteFrameRangeDlg dialog

class CDeleteFrameRangeDlg : public CDialog
{
// Construction
public:
	CDeleteFrameRangeDlg(CWnd* pParent = NULL);   // standard constructor

   void UpdateFrameLabels();

   int      m_FrameStart, m_FrameEnd;

// Dialog Data
	//{{AFX_DATA(CDeleteFrameRangeDlg)
	enum { IDD = IDD_DELETEFRAMES_DIALOG };
	CEdit	m_StartNumEdit;
	CEdit	m_StartNameEdit;
	CEdit	m_EndNumEdit;
	CEdit	m_EndNameEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeleteFrameRangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeleteFrameRangeDlg)
	afx_msg void OnChoosestartbut();
	afx_msg void OnChooseendbut();
	afx_msg void OnOutofmemoryStartframespin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposStartframespin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposEndframespin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CMoveFramesDlg dialog

class CMoveFramesDlg : public CDialog
{
// Construction
public:
	CMoveFramesDlg(CWnd* pParent = NULL);   // standard constructor
   void UpdateFrameLabels();

   int      m_FrameStart, m_FrameEnd, m_FrameTarget;
   bool     m_BeforeFrame;

// Dialog Data
	//{{AFX_DATA(CMoveFramesDlg)
	enum { IDD = IDD_MOVEFRAMES_DIALOG };
	CEdit	m_TargetNumEdit;
	CEdit	m_TargetNameEdit;
	CEdit	m_StartNumEdit;
	CEdit	m_StartNameEdit;
	CEdit	m_EndNumEdit;
	CEdit	m_EndNameEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoveFramesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMoveFramesDlg)
	afx_msg void OnChoosestartbut();
	afx_msg void OnChooseendbut();
	afx_msg void OnChoosetargetbut();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CGroundPlanePosDlg dialog

class CGroundPlanePosDlg : public CDialog
{
// Construction
public:
	CGroundPlanePosDlg(CWnd* pParent = NULL);   // standard constructor

   double   m_GroundPlanePos;
// Dialog Data
	//{{AFX_DATA(CGroundPlanePosDlg)
	enum { IDD = IDD_GROUNDPOS_DIALOG };
	CEdit	m_GroundPlanePosEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroundPlanePosDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroundPlanePosDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CExtrudeDistDlg dialog

class CExtrudeDistDlg : public CDialog
{
// Construction
public:
	CExtrudeDistDlg(CWnd* pParent = NULL);   // standard constructor

   float    m_ExtrudeDist;
// Dialog Data
	//{{AFX_DATA(CExtrudeDistDlg)
	enum { IDD = IDD_EXTRUDEDIST_DIALOG };
	CEdit	m_ExtrudeEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtrudeDistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExtrudeDistDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CSkinVertsFromProjectionDlg dialog

class CSkinVertsFromProjectionDlg : public CDialog
{
// Construction
public:
	void UpdateSpinEdits();
	CSkinVertsFromProjectionDlg(CWnd* pParent = NULL);   // standard constructor

   SkinVertPos    m_ProjectionType;
   float          m_Tolerance;
   bool           m_Mirror;

// Dialog Data
	//{{AFX_DATA(CSkinVertsFromProjectionDlg)
	enum { IDD = IDD_GETSKINVERTSFROMMODEL };
	CSpinButtonCtrl	m_ToleranceSpin;
	CEdit	m_ToleranceEdit;
	CButton	m_MirrorCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinVertsFromProjectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSkinVertsFromProjectionDlg)
	afx_msg void OnFrontradio();
	afx_msg void OnRightradio();
	afx_msg void OnTopradio();
	afx_msg void OnBackradio();
	afx_msg void OnLeftradio();
	afx_msg void OnBottomradio();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposTolerancespin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusToleranceedit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CResizeSkinDlg dialog

class CResizeSkinDlg : public CDialog
{
// Construction
public:
	CResizeSkinDlg(CWnd* pParent = NULL);   // standard constructor

   int   m_OldWidth, m_OldHeight, m_NewWidth, m_NewHeight;

// Dialog Data
	//{{AFX_DATA(CResizeSkinDlg)
	enum { IDD = IDD_RESIZESKIN_DIALOG };
	CEdit	m_OldWidthEdit;
	CEdit	m_OldHeightEdit;
	CEdit	m_NewWidthEdit;
	CEdit	m_NewHeightEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizeSkinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResizeSkinDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CInsertSkinDlg dialog

class CInsertSkinDlg : public CDialog
{
// Construction
public:
	void UpdateSpinEdits();
	CInsertSkinDlg(CWnd* pParent = NULL);   // standard constructor

   bool  m_InsertAfter;
   int   m_InsertSkin, m_MinSkin, m_MaxSkin;
// Dialog Data
	//{{AFX_DATA(CInsertSkinDlg)
	enum { IDD = IDD_INSERTNEWSKIN_DIALOG };
	CSpinButtonCtrl	m_InsertAfterSpin;
	CEdit	m_InsertAfterEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsertSkinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInsertSkinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposInsertafterspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeforefirstradio();
	afx_msg void OnAfterlastradio();
	afx_msg void OnAfterskinradio();
	afx_msg void OnKillfocusInsertafteredit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISCDLGS_H__5374DE57_CEE2_11D2_8395_0040332C7B2F__INCLUDED_)
