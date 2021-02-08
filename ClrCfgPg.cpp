// ClrCgfPg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "ClrCfgPg.h"

#include "mdlglobs.h"
#include "Colors.h"
#include "MainFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorConfigPage property page

IMPLEMENT_DYNCREATE(CColorConfigPage, CPropertyPage)

CColorConfigPage::CColorConfigPage() : CPropertyPage(CColorConfigPage::IDD)
{
	//{{AFX_DATA_INIT(CColorConfigPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

   m_OldColors = NULL;
   m_NewColors = NULL;
}

CColorConfigPage::~CColorConfigPage()
{
}

void CColorConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorConfigPage)
	DDX_Control(pDX, IDC_OLDCOLORBUT, m_OldColorBut);
	DDX_Control(pDX, IDC_NEWCOLORBUT, m_NewColorBut);
	DDX_Control(pDX, IDC_COLORITEMLIST, m_ColorItemList);
	DDX_Control(pDX, IDC_PALETTELIST, m_PaletteList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorConfigPage, CPropertyPage)
	//{{AFX_MSG_MAP(CColorConfigPage)
	ON_LBN_SELCHANGE(IDC_COLORITEMLIST, OnSelchangeColoritemlist)
	ON_LBN_SELCHANGE(IDC_PALETTELIST, OnSelchangePalettelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorConfigPage message handlers

BOOL CColorConfigPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
   m_PaletteList.AutoAdjustItemWidthHeight();
   {
      int      i;
      m_PaletteList.m_Palette = Palette;
      for (i=0; i<256; i++)
      {
         char  Str[16];

         itoa(i, Str, 10);
         m_PaletteList.AddString(Str);
      }
   }
   m_ColorItemList.SetCurSel(0);
	
	for (int i=0;i<NUM_COLORS;i++)
   {
      m_ColorItemList.AddString(COLORNAMES[i]);
   }



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorConfigPage::UpdateColorButtons()
{
   m_OldColorBut.SetColor(RGBToColorRef(Palette[m_OldColors[m_ColorItemList.GetCurSel()]]));
   m_NewColorBut.SetColor(RGBToColorRef(Palette[m_NewColors[m_ColorItemList.GetCurSel()]]));
}

void CColorConfigPage::SetOldNewColors(BYTE *Old, BYTE *New)
{
   m_OldColors = Old;
   m_NewColors = New;
   
}

void CColorConfigPage::OnSelchangeColoritemlist() 
{
   UpdateColorButtons();	
}

void CColorConfigPage::OnSelchangePalettelist() 
{
   m_NewColors[m_ColorItemList.GetCurSel()] = m_PaletteList.GetCurSel();	
   UpdateColorButtons();
}
