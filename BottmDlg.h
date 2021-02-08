#if !defined(AFX_BOTTMDLG_H__1F259414_C30F_11D2_8363_0040332C7B2F__INCLUDED_)
#define AFX_BOTTMDLG_H__1F259414_C30F_11D2_8363_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BottmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBottomDlg dialog

#define XYZ_ALL      0x07
#define XYZ_X        0x01
#define XYZ_Y        0x02
#define XYZ_Z        0x04
#define XYZ_XY       0x03
#define XYZ_XZ       0x05
#define XYZ_YZ       0x06

class CBottomDlg : public CDialog
{
// Construction
public:
	void UpdateGridLabel();
	void SetXYZLabels(float X, float Y, float Z, unsigned int nFlags = XYZ_ALL);
	int GetHeight();
	void UpdateSelectionCounters();
	BOOL SetSelectionType(SelectType SelType);
	CBottomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBottomDlg)
	enum { IDD = IDD_TOOLDLG_BOTTOM };
	CButton	m_PanViewsBut;
	CEdit	m_FeedBackEdit;
	CButton	m_VertexModeBut;
	CButton	m_ObjectModeBut;
	CButton	m_BoneModeBut;
	CButton	m_FaceModeBut;
	CScrollBar	m_TimeScroll;
	CEdit	m_GridSizeEdit;
	CEdit	m_ZEdit;
	CEdit	m_YEdit;
	CEdit	m_XEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBottomDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBottomDlg)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnVertexModeBut();
	afx_msg void OnFaceModeBut();
	afx_msg void OnBoneModeBut();
	afx_msg void OnHScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar);
	afx_msg void OnObjectmodebut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOTTMDLG_H__1F259414_C30F_11D2_8363_0040332C7B2F__INCLUDED_)
