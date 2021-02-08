// Clrbutn.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "ClrButn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorbutton

CColorbutton::CColorbutton()
{
}

CColorbutton::~CColorbutton()
{
}


BEGIN_MESSAGE_MAP(CColorbutton, CButton)
	//{{AFX_MSG_MAP(CColorbutton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorbutton message handlers

void CColorbutton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
   // do stuff
   HBRUSH   hBrush;
   RECT     Rect;

   hBrush = ::CreateSolidBrush(m_FaceColor);

   GetWindowRect(&Rect);
   ScreenToClient(&Rect);

   ::SelectObject(lpDrawItemStruct->hDC, hBrush);
   ::Rectangle(lpDrawItemStruct->hDC, 
               lpDrawItemStruct->rcItem.left, 
               lpDrawItemStruct->rcItem.top,
               lpDrawItemStruct->rcItem.right,
               lpDrawItemStruct->rcItem.bottom);

   ::DeleteObject(hBrush);
}

void CColorbutton::SetColor(COLORREF Col)
{
   if (m_FaceColor != Col)
   {
      m_FaceColor = Col;
//      SendMessage(WM_PAINT);
      Invalidate(FALSE);
   }
}
