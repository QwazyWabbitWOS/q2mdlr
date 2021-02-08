// PaintTab.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "PaintTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MdlGlobs.h"
#include "MainFunc.h"
#include "KeyFuncs.h"

#include "MdlMsgs.h"

/////////////////////////////////////////////////////////////////////////////
// CPaintPaintTab dialog


CPaintPaintTab::CPaintPaintTab(CWnd* pParent /*=NULL*/)
	: CDialog(CPaintPaintTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaintPaintTab)
	//}}AFX_DATA_INIT
}


void CPaintPaintTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaintPaintTab)
	DDX_Control(pDX, IDC_SOFTSPIN, m_SoftSpin);
	DDX_Control(pDX, IDC_SOFTEDIT, m_SoftEdit);
	DDX_Control(pDX, IDC_SIZESPIN, m_SizeSpin);
	DDX_Control(pDX, IDC_SIZEEDIT, m_SizeEdit);
	DDX_Control(pDX, IDC_OPACITYSLIDER, m_OpacitySlider);
	DDX_Control(pDX, IDC_OPACITYEDIT, m_OpacityEdit);
	DDX_Control(pDX, IDC_FADETOCOMBO, m_FadeToCombo);
	DDX_Control(pDX, IDC_FADESLIDER, m_FadeSlider);
	DDX_Control(pDX, IDC_FADEEDIT, m_FadeEdit);
	DDX_Control(pDX, IDC_DRAWEFFECTCOMBO, m_DrawEffectCombo);
	DDX_Control(pDX, IDC_COMPOUNDCHECK, m_CompoundCheck);
	DDX_Control(pDX, IDC_SPRAYBUT, m_SprayBut);
	DDX_Control(pDX, IDC_FILLBUT, m_FillBut);
	DDX_Control(pDX, IDC_DRAWBUT, m_DrawBut);
	DDX_Control(pDX, IDC_FOREBUT, m_ForeBut);
	DDX_Control(pDX, IDC_BACKBUT, m_BackBut);
	DDX_Control(pDX, IDC_COLORLISTBOX, m_ColorListbox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaintPaintTab, CDialog)
	//{{AFX_MSG_MAP(CPaintPaintTab)
	ON_WM_MOUSEMOVE()
	ON_WM_DRAWITEM()
	ON_LBN_SELCHANGE(IDC_COLORLISTBOX, OnSelchangeColorlistbox)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SIZESPIN, OnDeltaposSizespin)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SWAPCOLSBUT, OnSwapcolsbut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaintPaintTab message handlers

void CPaintPaintTab::OnMouseMove(unsigned int nFlags, CPoint point) 
{
}

BOOL CPaintPaintTab::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

   m_ColorListbox.AutoAdjustItemWidthHeight();
   {
      int      i;
      m_ColorListbox.m_Palette = Palette;
      for (i=0; i<256; i++)
      {
         char  Str[16];

         itoa(i, Str, 10);
         m_ColorListbox.AddString(Str);
      }
   }
   
   m_ForeColor = 0;
   m_BackColor = 115;

   m_ForeBut.SetColor(RGBToColorRef(Palette[m_ForeColor]));
   m_BackBut.SetColor(RGBToColorRef(Palette[m_BackColor]));

   m_DrawBrushSize = 3;

   m_DrawEffectCombo.SetCurSel(0);
   m_FadeToCombo.SetCurSel(0);
   
   m_OpacitySlider.SetRange(0,100);
   m_OpacitySlider.SetPos(100);
   m_OpacitySlider.SetTicFreq(10);

   m_FadeSlider.SetRange(0,100);
   m_FadeSlider.SetPos(0);
   m_FadeSlider.SetTicFreq(10);

   UpdateSpinEdits();

   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPaintPaintTab::WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam) 
{
	return CDialog::WindowProc(message, wParam, lParam);
}

void CPaintPaintTab::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CPaintPaintTab::OnSelchangeColorlistbox() 
{
   {
      m_ForeColor = m_ColorListbox.GetCurSel();
      m_ForeBut.SetColor(RGBToColorRef(Palette[m_ForeColor]));
         
   }
}

BOOL CPaintPaintTab::PreTranslateMessage(MSG* pMsg) 
{
   if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
   {
      LONG  WndStyle;

      WndStyle = GetWindowLong(pMsg->hwnd, GWL_STYLE);
      if (WndStyle & ES_AUTOHSCROLL)
      {
         // we have an edit box
         if (pMsg->message == WM_KEYDOWN)
         {
            if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
            {
               GetParentOwner()->SetFocus();
               return 1;
            }
         }
      }
      else
      {
         return GetParentOwner()->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
      }
   }
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPaintPaintTab::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	unsigned int     CmdID, Msg;
   HWND     hWnd;
   CWnd     *Parent;

   Parent = GetParentOwner();
   CmdID = LOWORD(wParam);
   Msg = HIWORD(wParam);
   hWnd = (HWND)lParam;

   if (Msg == BN_CLICKED)
   {
      switch(CmdID)
      {
      case IDC_DRAWBUT:
      case IDC_FILLBUT:
      case IDC_SPRAYBUT:
         Parent->PostMessage(MM_TOOLBUTCLICK, 0, (long)hWnd);
         break;
      }
   }
	
	
	return CDialog::OnCommand(wParam, lParam);
}

void CPaintPaintTab::OnDeltaposSizespin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   m_DrawBrushSize += pNMUpDown->iDelta;

   char  Str[16];
   itoa(m_DrawBrushSize, Str, 10);
   m_SizeEdit.SetWindowText(Str);

   UpdateSpinEdits();

	*pResult = 0;
}

void CPaintPaintTab::UpdateSpinEdits()
{
   char  Str[16];

   itoa(m_DrawBrushSize, Str, 10);
   m_SizeEdit.SetWindowText(Str);

   itoa(m_OpacitySlider.GetPos(), Str, 10);
   m_OpacityEdit.SetWindowText(Str);

   itoa(m_FadeSlider.GetPos(), Str, 10);
   m_FadeEdit.SetWindowText(Str);
}

void CPaintPaintTab::OnHScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) 
{
   UpdateSpinEdits();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPaintPaintTab::OnSwapcolsbut() 
{
   KPaintSwapFBColors();	
}
