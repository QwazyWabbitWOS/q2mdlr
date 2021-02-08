#if !defined(AFX_CLRBUTN_H__63154736_D7A4_11D2_83B5_0040332C7B2F__INCLUDED_)
#define AFX_CLRBUTN_H__63154736_D7A4_11D2_83B5_0040332C7B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClrButn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorbutton window

class CColorbutton : public CButton
{
// Construction
public:
	CColorbutton();

// Attributes
public:

   COLORREF m_FaceColor;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorbutton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColor(COLORREF Col);
	virtual ~CColorbutton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorbutton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLRBUTN_H__63154736_D7A4_11D2_83B5_0040332C7B2F__INCLUDED_)
