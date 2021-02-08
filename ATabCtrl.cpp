// ATabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ATabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoTabCtrl

CAutoTabCtrl::CAutoTabCtrl()
{
}

CAutoTabCtrl::~CAutoTabCtrl()
{
}

int CAutoTabCtrl::AddTab(char *Title, CDialog *Dlg, unsigned int IDTemplate)
{
	BOOL	Result;

	SetMinTabWidth(8);

	if (!Dlg || !IDTemplate || !Title)
			return -1;

	// Has the window already been created?
	if (Dlg->m_hWnd == NULL)
		Dlg->Create(IDTemplate, this);

	// Window creation failed
	if (Dlg->m_hWnd == NULL)
		return 0;

	Result = InsertItem(TCIF_TEXT|TCIF_PARAM, GetItemCount(), Title, 0, (LPARAM)Dlg);

   if (Result == 0)  // if this is the first tab being added
   {
      Dlg->ShowWindow(SW_SHOW);
   }

	return Result;
}



BEGIN_MESSAGE_MAP(CAutoTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CAutoTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelChanging)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoTabCtrl message handlers

// when the tab control is about to change tabs
void CAutoTabCtrl::OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TCITEM		TabItem;
	BOOL		Result;

	TabItem.mask = TCIF_PARAM;
	TabItem.lParam = NULL;
	Result = GetItem(GetCurSel(), &TabItem);

	if (Result == FALSE || TabItem.lParam == NULL)
		return;

	CDialog	*Dlg;

	Dlg = (CDialog*)TabItem.lParam;
	
	Dlg->ShowWindow(FALSE);

	*pResult = 0;
}

// after the tab control has changed tabs
void CAutoTabCtrl::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TCITEM		TabItem;
	BOOL		Result;

	TabItem.mask = TCIF_PARAM;
	TabItem.lParam = NULL;
	Result = GetItem(GetCurSel(), &TabItem);

	if (Result == FALSE || TabItem.lParam == NULL)
		return;

	CDialog	*Dlg;

	Dlg = (CDialog*)TabItem.lParam;

	ResizeChildDialog(Dlg);

	Dlg->ShowWindow(SW_SHOW);
	
	*pResult = 0;
}

// Recalculates the given dialogs window size to the given rect as the tab control size
// if lpRect is NULL, uses the actual tab control size
int CAutoTabCtrl::ResizeChildDialog(CDialog *Dlg, RECT *lpRect)
{
	RECT		Rect;


	if (lpRect == NULL)
		GetClientRect(&Rect);		// Get the tab controls client size.
	else
		Rect = *lpRect;

	AdjustRect(FALSE, &Rect);

	// Move the window to that position
	Dlg->MoveWindow(&Rect);	

	return 1;
}

// Gets the current dialog, if any, and resizes it.
int CAutoTabCtrl::ResizeCurrentDialog(RECT *lpRect)
{
	TCITEM		TabItem;
	BOOL		Result;

	TabItem.mask = TCIF_PARAM;
	TabItem.lParam = NULL;
	Result = GetItem(GetCurSel(), &TabItem);

	if (Result == FALSE || TabItem.lParam == NULL)
		return 0;

	CDialog	*Dlg;

	Dlg = (CDialog*)TabItem.lParam;

	ResizeChildDialog(Dlg, lpRect);

	return 1;
}

int CAutoTabCtrl::ResizeTabCtrl(int Width, int Height)
{
	RECT	Rect;

	Rect.left = 0;
	Rect.top = 0;
	Rect.bottom = Height-1;
	Rect.right = Width-1;

	ResizeCurrentDialog(&Rect);
	SetWindowPos(NULL, 0,0, Width, Height, SWP_NOZORDER | SWP_NOMOVE);

	return 1;
}

CWnd* CAutoTabCtrl::GetCurrentDlg()
{
	TCITEM		TabItem;
	BOOL		Result;

	TabItem.mask = TCIF_PARAM;
	TabItem.lParam = NULL;
	Result = GetItem(GetCurSel(), &TabItem);

	if (Result == FALSE || TabItem.lParam == NULL)
		return NULL;

	CDialog	*Dlg;

	Dlg = (CDialog*)TabItem.lParam;

   return Dlg;
}


int CAutoTabCtrl::ResizeControlToLargest()
{
   RECT        ThisRect, DlgRect;
   int         LargestWidth, i;
	TCITEM		TabItem;
	BOOL        Result;
   CDialog	   *Dlg;


   LargestWidth = 0;
   
   GetWindowRect(&ThisRect);
   ScreenToClient(&ThisRect);


   for (i = 0;i<GetItemCount();i++)
   {
	   TabItem.mask = TCIF_PARAM;
	   TabItem.lParam = NULL;
	   Result = GetItem(GetCurSel(), &TabItem);

	   if (Result == FALSE || TabItem.lParam == NULL)
		   continue;

	   Dlg = (CDialog*)TabItem.lParam;

      //Since the dialog is a child control with no border, the width can 
      // be retreived using GetClientRect

      Dlg->GetWindowRect(&DlgRect);
      if (DlgRect.right > LargestWidth )
         LargestWidth = DlgRect.right;
   }

   ResizeTabCtrl(LargestWidth, ThisRect.bottom);
   return LargestWidth;
}
