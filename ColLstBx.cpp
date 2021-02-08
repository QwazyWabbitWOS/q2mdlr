// ColLstBx.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "ColLstBx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorListBoxCtrl

CColorListBoxCtrl::CColorListBoxCtrl()
{
   m_ItemWidth = 10;
   m_ItemHeight = 10;
}


CColorListBoxCtrl::~CColorListBoxCtrl()
{
   
}

BEGIN_MESSAGE_MAP(CColorListBoxCtrl, CListBox)
	//{{AFX_MSG_MAP(CColorListBoxCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorListBoxCtrl message handlers


void CColorListBoxCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
   int Offset = 2;                               // default offset

   CDC   *DC;
   RECT  *Rect;
   int   Index;

   DC = GetDC();

   Index = lpDrawItemStruct->itemID;
   Rect = &lpDrawItemStruct->rcItem;

   DC->FillSolidRect(Rect->left, Rect->top, m_ItemWidth, m_ItemHeight, ((m_Palette[Index][2] << 16) +
      (m_Palette[Index][1] << 8) +
      (m_Palette[Index][0] )));

   ReleaseDC(DC);
}

int CColorListBoxCtrl::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
	
   return -1;
//	return 0;
}

void CColorListBoxCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
   lpMeasureItemStruct->itemWidth = m_ItemWidth;
   lpMeasureItemStruct->itemHeight = m_ItemHeight;
   
}

void CColorListBoxCtrl::OnLButtonDown(unsigned int nFlags, CPoint point) 
{
   m_SettingFore = true;
	CListBox::OnLButtonDown(nFlags, point);
}

void CColorListBoxCtrl::OnRButtonDown(unsigned int nFlags, CPoint point) 
{
	CListBox::OnRButtonDown(nFlags, point);
}

void CColorListBoxCtrl::AutoAdjustItemWidthHeight()
{
   RECT  ClientRect;

   GetClientRect(&ClientRect);

   m_ItemWidth = (int)(ClientRect.right/16);
   m_ItemHeight = (int)((ClientRect.bottom)/16);

   SetItemHeight(0, m_ItemHeight);
   SetColumnWidth(m_ItemWidth);


}