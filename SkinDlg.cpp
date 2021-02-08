// SkinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"

#include "MdlGlobs.h"
#include "MainFunc.h"
#include "UndoSkin.h"
#include "KeyFuncs.h"

#include "Colors.h"

#include "MdlMsgs.h"

#include "SkinDlg.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinEditorDlg dialog

float movex, movey,omovex,omovey;


CSkinEditorDlg::CSkinEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkinEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkinEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

   m_MainDlg = NULL;
}


void CSkinEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkinEditorDlg)
	DDX_Control(pDX, IDC_VERTSCROLL, m_VertScroll);
	DDX_Control(pDX, IDC_HORIZSCROLL, m_HorizScroll);
	DDX_Control(pDX, IDC_TOOLTAB, m_ToolTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkinEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CSkinEditorDlg)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_OPTIONS_LINES_DONTDRAW, OnOptionsLinesDontdraw)
	ON_COMMAND(ID_OPTIONS_LINES_NORMAL, OnOptionsLinesNormal)
	ON_COMMAND(ID_OPTIONS_VERTICES_NONE, OnOptionsVerticesNone)
	ON_COMMAND(ID_OPTIONS_VERTICES_DOTS, OnOptionsVerticesDots)
	ON_COMMAND(ID_OPTIONS_VERTICES_TICKS, OnOptionsVerticesTicks)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_COMMAND(ID_EDIT_SELECTCONNECTED, OnEditSelectconnected)
	ON_COMMAND(ID_EDIT_SELECTINVERSE, OnEditSelectinverse)
	ON_COMMAND(ID_EDIT_SELECTNONE, OnEditSelectnone)
	ON_COMMAND(ID_EDIT_SELECTTOUCHING, OnEditSelecttouching)
	ON_COMMAND(ID_OPTIONS_SYNC3DSELECTION, OnOptionsSync3dselection)
	ON_COMMAND(ID_FILE_IMPORT_SKINIMAGE, OnFileImportSkinimage)
	ON_COMMAND(ID_FILE_IMPORT_SKINMESHFROMASC, OnFileImportSkinmeshfromasc)
	ON_COMMAND(ID_FILE_EXPORT_CURRENTIMAGE_BMP, OnFileExportCurrentimageBmp)
	ON_COMMAND(ID_FILE_EXPORT_CURRENTIMAGE_PCX, OnFileExportCurrentimagePcx)
	ON_COMMAND(ID_FILE_EXPORT_SKINMESHTOASC, OnFileExportSkinmeshtoasc)
	ON_COMMAND(ID_EDIT_GETPOSITIONFROMMODEL, OnEditGetpositionfrommodel)
	ON_COMMAND(ID_EDIT_ADDNEWSKIN, OnEditAddnewskin)
	ON_COMMAND(ID_EDIT_DELETECURRENTSKIN, OnEditDeletecurrentskin)
	ON_COMMAND(ID_EDIT_CLEARCURRENTSKIN, OnEditClearcurrentskin)
	ON_COMMAND(ID_EDIT_RESIZESKIN, OnEditResizeskin)
	ON_COMMAND(ID_VIEW_SKINPAINTER, OnViewSkinpainter)
	ON_COMMAND(ID_VIEW_3DEDITOR, OnView3deditor)
	ON_COMMAND(ID_OPTIONS_KEEPWINDOWONTOP, OnOptionsKeepwindowontop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinEditorDlg message handlers
LRESULT CSkinEditorDlg::WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam) 
{
   if (message == MM_TOOLBUTCLICK)
   {
      HWND  hBut;

      hBut = (HWND)lParam;

      ToolButClick(hBut);

      return 1;
   }
   if (message == MM_UPDATESKIN)
   {
      UpdateSkin();
      return 1;
   }
   if (message == MM_UPDATEVIEWPORTS)
   {
      if (wParam & MMC_UPDATE_SKIN)
         SendMessage(WM_PAINT);

      if (lParam & MMC_UPDATE_DEPENDANTS)
      {
         m_MainDlg->SendMessage(MM_UPDATEVIEWPORTS, MMC_UPDATE_CAM,0);
      }
      return 1;
   }
   if (message == MM_SKIN_CHECKSCROLLBARS)
   {
      CheckScrollBars();
   }
   if (message == MM_DOSYNC)
   {
      DoSync();
   }
   if (message == MM_UPDATEFRAMELABEL)
   {
      m_SideToolDlg.UpdateFrameLabels();
   }

   
	return CDialog::WindowProc(message, wParam, lParam);
}


BOOL CSkinEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_ToolTab.AddTab("Modify", &m_ModifyTab, IDD_SKIN_TABDLG_MODIFY);

   m_SideToolDlg.Create(IDD_SKIN_TOOLDLG_SIDE, this);
   m_SideToolDlg.ShowWindow(SW_SHOW);

   m_BottomToolDlg.Create(IDD_SKIN_TOOLDLG_BOTTOM, this);
   m_BottomToolDlg.ShowWindow(SW_SHOW);
   
   // bottom dialog
   AddWndStyle(m_BottomToolDlg.m_VertexModeBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_BottomToolDlg.m_FaceModeBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_BottomToolDlg.m_ObjectModeBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);

   // Modify Tab dialog
   AddWndStyle(m_ModifyTab.m_SelectBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_MoveBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_RotateBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_ScaleBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_XBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_YBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);


   // Set the default menu stuff
   GetMenu()->CheckMenuRadioItem(ID_OPTIONS_LINES_DONTDRAW, 
                                 ID_OPTIONS_LINES_NORMAL, 
                                 ID_OPTIONS_LINES_NORMAL, 
                                 MF_BYCOMMAND);
   GetMenu()->CheckMenuItem(ID_OPTIONS_SHOWGRID, MF_CHECKED);
   GetMenu()->CheckMenuRadioItem(ID_OPTIONS_VERTICES_NONE, 
                                 ID_OPTIONS_VERTICES_TICKS, 
                                 ID_OPTIONS_VERTICES_DOTS, 
                                 MF_BYCOMMAND);
   
   
   GetMenu()->EnableMenuItem(ID_EDIT_COPY, MF_GRAYED);
   GetMenu()->EnableMenuItem(ID_EDIT_PASTE, MF_GRAYED);

   m_CurTool = m_ModifyTab.m_SelectBut.m_hWnd;
   m_LastTool = m_ModifyTab.m_MoveBut.m_hWnd;

   ::SendMessage(m_CurTool, BM_SETCHECK, 1,0);
   ::SendMessage(m_LastTool, BM_SETCHECK, 0,0);

   // Set the XY restriction buttons to all on.
   ::SendMessage(m_ModifyTab.m_XBut, BM_SETCHECK, 1,0);
   ::SendMessage(m_ModifyTab.m_YBut, BM_SETCHECK, 1,0);


   m_PanX = 0;
   m_PanY = 0;

	g_CurSkin = 0;
   g_SkinAction = atNone;

   m_SkinPic.SetPalette(Palette);

   g_SkinUndo.RedoList = &g_SkinRedo;
   g_SkinUndo.DeleteRedoOnAdd = true;
   g_SkinUndo.MenuPrefix = "Undo ";
   g_SkinUndo.SetAssociatedMenu(GetMenu()->m_hMenu, ID_EDIT_UNDO);
   g_SkinUndo.UpdateMenuItem();
   g_SkinRedo.RedoList = &g_SkinUndo;
   g_SkinRedo.MenuPrefix = "Redo ";
   g_SkinRedo.SetAssociatedMenu(GetMenu()->m_hMenu, ID_EDIT_REDO);
   g_SkinRedo.UpdateMenuItem();

   m_BottomToolDlg.SetSelectionType(stVertex);

   m_SkinPic.SetPalette(Palette);

   UpdateSkin();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSkinEditorDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanged(lpwndpos);
	
	RECT	ClientRect, TabCtrlRect, SideDlgRect, BotDlgRect;
   int   SideDlgHeight, BotDlgHeight, SideDlgWidth;
   int   W, H;

	GetClientRect(&ClientRect);
	m_ToolTab.GetWindowRect(&TabCtrlRect);	

   W = ClientRect.right;
   H = ClientRect.bottom;
	


   if (m_SideToolDlg.m_hWnd)
      m_SideToolDlg.GetWindowRect(&SideDlgRect);

   if (m_BottomToolDlg.m_hWnd)
      m_BottomToolDlg.GetWindowRect(&BotDlgRect);


	TabCtrlRect.bottom = ClientRect.bottom;
	TabCtrlRect.left = ClientRect.left;
	TabCtrlRect.top = ClientRect.top;

   SideDlgHeight = SideDlgRect.bottom-SideDlgRect.top;
   SideDlgWidth = SideDlgRect.right-SideDlgRect.left;
   BotDlgHeight = BotDlgRect.bottom-BotDlgRect.top;

	// Resize all controls on the window here

	m_ToolTab.ResizeTabCtrl(SideDlgWidth, ClientRect.bottom-SideDlgHeight-BotDlgHeight);	
	
   // Move the Other dialogs about the place.
   if (m_SideToolDlg.m_hWnd)
      m_SideToolDlg.MoveWindow(0,ClientRect.bottom - BotDlgHeight-SideDlgHeight,SideDlgRect.right - SideDlgRect.left, SideDlgHeight);

   if (m_BottomToolDlg.m_hWnd)
      m_BottomToolDlg.MoveWindow(0,ClientRect.bottom-BotDlgHeight,ClientRect.right, BotDlgHeight);

   m_VertScroll.MoveWindow( W-16, 0, 16, H-16-BotDlgHeight);
   m_HorizScroll.MoveWindow( SideDlgWidth, H-16-BotDlgHeight, W - SideDlgWidth - 16, 16);

   CheckScrollBars();
	
}

void CSkinEditorDlg::OnPaint() 
{
   if (IsIconic())
   {
      CPaintDC dc(this); // device context for painting
      
      SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
      
      // Center icon in client rectangle
      int cxIcon = GetSystemMetrics(SM_CXICON);
      int cyIcon = GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x = (rect.Width() - cxIcon + 1) / 2;
      int y = (rect.Height() - cyIcon + 1) / 2;
      
      // Draw the icon
      //		dc.DrawIcon(x, y, m_hIcon);
   }
   else
   {
      if (g_CurMdl.Head.num_skins == 0)
      {
         CDialog::OnPaint();
         return;
      }
      
      if (g_CurMdl.Skins[g_CurSkin] == NULL)
      {
         CDialog::OnPaint();
         return;
      }
      if (m_SkinPic.Data == NULL)
      {
         CDialog::OnPaint();
         return;
      }
      
      unsigned char *d = m_SkinPic.Data, *s = g_CurMdl.Skins[g_CurSkin] + m_SkinPic.PInfoHeader->biSizeImage;
      for (int y=0;y<g_CurMdl.Head.skinheight;y++)
      {
         s-=g_CurMdl.Head.skinwidth;
         memcpy(d, s, g_CurMdl.Head.skinwidth);
         d+=m_SkinPic.Width;
      }
      
      TRealTriangle	*T = &g_CurMdl.Tris[0];
      int            i;
      
      for (i=0;i<g_CurMdl.Head.num_tris;i++, T++)
      {
         int x1,x2,x3,y1,y2,y3;
         
         x1 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[0] ].s;
         x2 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[1] ].s;
         x3 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[2] ].s;
         
         y1 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[0] ].t;
         y2 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[1] ].t;
         y3 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[2] ].t;
         
         
         if (!(T->SkinSelected && g_SkinSelectMode == stTriangle))
         {
            if (IsMenuChecked(ID_OPTIONS_LINES_NORMAL))
            {
               m_SkinPic.Line( x1,y1,x2,y2,SKINWIRE);
               m_SkinPic.Line( x2,y2,x3,y3,SKINWIRE);
               m_SkinPic.Line( x3,y3,x1,y1,SKINWIRE);
            }
         }
         
      }
      
      T = &g_CurMdl.Tris[0];
      for (i=0;i<g_CurMdl.Head.num_tris;i++, T++)
      {
         int x1,x2,x3,y1,y2,y3;
         
         x1 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[0] ].s;
         x2 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[1] ].s;
         x3 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[2] ].s;
         
         y1 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[0] ].t;
         y2 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[1] ].t;
         y3 = (int)g_CurMdl.SkinVerts[ T->SkinVerts[2] ].t;
         
         
         if (T->SkinSelected && g_SkinSelectMode == stTriangle)
         {
            m_SkinPic.Line( x1,y1,x2,y2,SELSKINWIRE);
            m_SkinPic.Line( x2,y2,x3,y3,SELSKINWIRE);
            m_SkinPic.Line( x3,y3,x1,y1,SELSKINWIRE);
         }
         
      }
      
      if (IsMenuChecked(ID_OPTIONS_VERTICES_TICKS))
      {
         for (int i=0;i<g_CurMdl.Head.num_st;i++)
            if (g_CurMdl.SkinVerts[i].Selected && g_SkinSelectMode == stVertex)
            {
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s), (int)(g_CurMdl.SkinVerts[i].t), SELSKINVERTEX);
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s-1), (int)(g_CurMdl.SkinVerts[i].t), SELSKINVERTEX);
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s+1), (int)(g_CurMdl.SkinVerts[i].t), SELSKINVERTEX);
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s), (int)(g_CurMdl.SkinVerts[i].t-1), SELSKINVERTEX);
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s), (int)(g_CurMdl.SkinVerts[i].t+1), SELSKINVERTEX);
            }
            else
            {
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s), (int)(g_CurMdl.SkinVerts[i].t), SKINVERTEX);
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s-1), (int)(g_CurMdl.SkinVerts[i].t), SKINVERTEX);
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s+1), (int)(g_CurMdl.SkinVerts[i].t), SKINVERTEX);
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s), (int)(g_CurMdl.SkinVerts[i].t-1), SKINVERTEX);
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s), (int)(g_CurMdl.SkinVerts[i].t+1), SKINVERTEX);
            }
      }
      
      if (IsMenuChecked(ID_OPTIONS_VERTICES_DOTS))
      {
         for (int i=0;i<g_CurMdl.Head.num_st;i++)
            if (g_CurMdl.SkinVerts[i].Selected && g_SkinSelectMode == stVertex)
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s), (int)(g_CurMdl.SkinVerts[i].t), SELSKINVERTEX);
            else
               m_SkinPic.PutPixel( (int)(g_CurMdl.SkinVerts[i].s), (int)(g_CurMdl.SkinVerts[i].t), SKINVERTEX);
      }
      
      if (g_SkinAction == atSelectBox || g_SkinAction == atDeselectBox)
      {
         m_SkinPic.Rect( (int)g_DownS,(int)g_DownT, (int)movex, (int)movey, SELECTBOX);
      }
      
      
      {
         m_SkinPic.SetPalette(Palette);
         
         int srcx,srcy,srcheight, srcwidth, destx, desty, destwidth, destheight;
         
         int VisHeight, VisWidth;
         int ClientWidth, ClientHeight;
         RECT     ClientRect;
         
         GetClientRect(&ClientRect);
         ClientWidth = ClientRect.right;
         ClientHeight = ClientRect.bottom;
         
         
         VisWidth = ClientWidth - m_SideToolDlg.GetWidth();
         VisHeight = ClientHeight -  m_BottomToolDlg.GetHeight();
         
         if (m_VertScroll.IsWindowVisible())
            VisWidth -= GetCWndWidth(m_VertScroll);
         if (m_HorizScroll.IsWindowVisible())
            VisHeight -= GetCWndHeight(m_HorizScroll);
         
         srcx = (int)(m_PanX/g_SkinZoomFactor);
         srcy = (int)(m_PanY/g_SkinZoomFactor);
         srcwidth = (int)(1 + (VisWidth + GetCWndWidth(m_VertScroll)) / g_SkinZoomFactor);
         srcheight = (int)(1 + (VisHeight + GetCWndHeight(m_HorizScroll)) / g_SkinZoomFactor);
         
         
         if (srcx<0)
            srcx = 0;
         if (srcy<0)
            srcy = 0;
         if (srcx>=g_CurMdl.Head.skinwidth)
            srcx = g_CurMdl.Head.skinwidth-1;
         if (srcy>=g_CurMdl.Head.skinheight)
            srcy = g_CurMdl.Head.skinheight-1;
         
         if (srcwidth+srcx >= g_CurMdl.Head.skinwidth)
            srcwidth = g_CurMdl.Head.skinwidth-srcx-1;
         if (srcheight+srcy >= g_CurMdl.Head.skinheight)
            srcheight = g_CurMdl.Head.skinheight-srcy-1;
         
         //      srcy = g_CurMdl.Head.skinheight-srcy-(ClientHeight-BottomPanel->Height)/g_SkinZoomFactor;
         srcy = -srcy;
         srcy += g_CurMdl.Head.skinheight-srcheight;
         destx = m_SideToolDlg.GetWidth();
         desty = 0;
         
/*         destwidth = VisWidth;
         destheight = VisHeight;
         
         destwidth = VisWidth + GetCWndWidth(m_VertScroll)+1;
         destheight = VisHeight + GetCWndHeight(m_HorizScroll)+1;*/
         
//         if (srcwidth * g_SkinZoomFactor < destwidth)
            destwidth = (int)(srcwidth * g_SkinZoomFactor);
//         if (srcheight * g_SkinZoomFactor < destheight)
            destheight = (int)(srcheight * g_SkinZoomFactor);
         
         
         //         m_SkinPic.DrawToDC(GetDC()->m_hDC, destx, desty);
         CDC   *DC;
         DC = GetDC();

         m_SkinPic.StretchToDC(DC->m_hDC, destx, desty, destwidth, destheight, srcx, srcy, srcwidth, srcheight);

         ReleaseDC(DC);
         /*
         m_SkinPic.StretchToDC(Canvas->Handle,
   						SidePanel->Width-m_HorizScroll.GetScrollPos(),-m_VertScroll.GetScrollPos(),
                     g_SkinZoomFactor*g_CurMdl.Head.skinwidth, g_SkinZoomFactor*g_CurMdl.Head.skinheight);
         */
      }
      
      if (g_SkinZoomChanged > g_SkinZoomFactor)
      {
         // This clears the extra bits from the view wehn the skin is zoomed out
         // so that it doesn't fill the entire view.
         
         /*         Canvas->Brush->Style = bsSolid;
         Canvas->Brush->Color = clBtnFace;
         Canvas->Pen->Color = clBtnFace;
         
           Canvas->Rectangle(SidePanel->Width+g_SkinZoomFactor*g_CurMdl.Head.skinwidth-m_HorizScroll.GetScrollPos(),
           0,
           SidePanel->Width+g_SkinZoomChanged*g_CurMdl.Head.skinwidth-m_HorizScroll.GetScrollPos(),
           g_SkinZoomChanged*g_CurMdl.Head.skinheight-m_HorizScroll.GetScrollPos());
           Canvas->Rectangle( SidePanel->Width,
           g_SkinZoomFactor*g_CurMdl.Head.skinheight-m_HorizScroll.GetScrollPos(),
           SidePanel->Width+g_SkinZoomFactor*g_CurMdl.Head.skinwidth-m_HorizScroll.GetScrollPos(),
         g_SkinZoomChanged*g_CurMdl.Head.skinheight-m_HorizScroll.GetScrollPos());*/
      }
      
      g_SkinZoomChanged = 0;
      
      
      
      CDialog::OnPaint();
   }
}

unsigned int CSkinEditorDlg::IsMenuChecked(unsigned int IDItem)
{
   return GetMenu()->GetMenuState(IDItem, MF_CHECKED) & MF_CHECKED;
}

void CSkinEditorDlg::UpdateSkin()
{
	if (g_CurMdl.Empty)
   	return;

/*
   Canvas->Brush->Style = bsSolid;
   Canvas->Brush->Color = clBtnFace;
   Canvas->Pen->Color = clBtnFace;

   Canvas->Rectangle(100,0,ClientWidth,ClientHeight);
*/

	m_SkinPic.SetSize(g_CurMdl.Head.skinwidth,g_CurMdl.Head.skinheight);

   if (g_CurMdl.Head.num_skins==0)
   {
   	m_SideToolDlg.m_SkinNumEdit.SetWindowText("-");
//      DeleteCurrentSkin->Enabled = false;
   	return;
   }

   if (m_SkinPic.Data == NULL)
   	return;

   if (g_CurSkin < 0)
      g_CurSkin = 0;
   if (g_CurSkin >= g_CurMdl.Head.num_skins)
      g_CurSkin = g_CurMdl.Head.num_skins - 1;

   unsigned char *d = m_SkinPic.Data, *s = g_CurMdl.Skins[g_CurSkin] + m_SkinPic.PInfoHeader->biSizeImage;
   for (int y=0;y<g_CurMdl.Head.skinheight;y++)
   {
      s-=g_CurMdl.Head.skinwidth;
	   memcpy(d, s, g_CurMdl.Head.skinwidth);
      d+=g_CurMdl.Head.skinwidth;
   }
//   ViewPort->Width = g_SkinZoomFactor*g_CurMdl.Head.skinwidth;
//  ViewPort->Height = g_SkinZoomFactor*g_CurMdl.Head.skinheight;
	CheckScrollBars();
}

void CSkinEditorDlg::CheckScrollBars()
{
   int      ClientWidth, ClientHeight;
   RECT     ClientRect;
   int      HMin, HMax, VMin, VMax;

   m_HorizScroll.GetScrollRange(&HMin, &HMax);
   m_VertScroll.GetScrollRange(&VMin, &VMax);
   
   GetClientRect(&ClientRect);
   ClientWidth = ClientRect.right;
   ClientHeight = ClientRect.bottom;

   int VisWidth, VisHeight;

	for (int i=0;i<2;i++)
   {
      VisWidth = ClientWidth - m_SideToolDlg.GetWidth();

      if (m_VertScroll.IsWindowVisible())
         VisWidth -= GetCWndWidth(m_VertScroll);
      VisHeight = ClientHeight - m_BottomToolDlg.GetHeight();

      if (m_HorizScroll.IsWindowVisible())
         VisHeight -= GetCWndHeight(m_HorizScroll);

      if (g_SkinZoomFactor*g_CurMdl.Head.skinwidth < VisWidth)
         m_HorizScroll.ShowWindow(SW_HIDE);
      else
         m_HorizScroll.ShowWindow(SW_SHOW);

      if (g_SkinZoomFactor*g_CurMdl.Head.skinheight < VisHeight)
         m_VertScroll.ShowWindow(SW_HIDE);
      else
         m_VertScroll.ShowWindow(SW_SHOW);

	}
   if (m_HorizScroll.IsWindowVisible())
   {

      float OldPos = (float)(m_PanX)/(HMax);
   	SCROLLINFO	si;

      int max = (int)(g_SkinZoomFactor*g_CurMdl.Head.skinwidth+20);

      m_HorizScroll.SetScrollRange(0,max);
      m_HorizScroll.SetScrollPos((int)(OldPos * max));
      m_PanX = (int)(OldPos * max);

      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_PAGE;
      si.nPage = VisWidth;

      m_HorizScroll.SetScrollInfo(&si, TRUE);
   }
   else 
   {
      m_HorizScroll.SetScrollPos(0);
      m_PanX = 0;
   }


   if (m_VertScroll.IsWindowVisible())
   {
      float OldPos = (float)(m_PanY)/(VMax);
   	SCROLLINFO	si;
      int max = (int)(g_SkinZoomFactor*g_CurMdl.Head.skinheight+31);

      m_VertScroll.SetScrollRange(0, max);
      m_VertScroll.SetScrollPos((int)(OldPos*max));
      m_PanY = (int)(OldPos*max);

      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_PAGE;
      si.nPage = VisHeight;

      m_VertScroll.SetScrollInfo(&si, TRUE);
   }
   else 
   {
      m_VertScroll.SetScrollPos(0);
      m_PanY = 0;
   }

/*   if (m_VertScroll.IsWindowVisible() == false && m_HorizScroll.IsWindowVisible() == false)
   {
   	CornerPanel->Visible = false;
   }
   else CornerPanel->Visible = true;*/

}

void CSkinEditorDlg::OnHScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) 
{
   if (nSBCode == SB_ENDSCROLL)
   {
   	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
      return;
   }

   if (nSBCode != SB_THUMBPOSITION && nSBCode != SB_THUMBTRACK)
      nPos = pScrollBar->GetScrollPos();
   if (nSBCode == SB_LINELEFT)
      nPos --;
   if (nSBCode == SB_LINERIGHT)
      nPos ++;
   if (nSBCode == SB_PAGELEFT)
      nPos -= 10;
   if (nSBCode == SB_PAGERIGHT)
      nPos += 10;

   
   int ClientWidth, ClientHeight;
   RECT     ClientRect;

   GetClientRect(&ClientRect);
   ClientWidth = ClientRect.right;
   ClientHeight = ClientRect.bottom;

   double
   	vp = nPos,
      cp= g_SkinZoomFactor*g_CurMdl.Head.skinwidth-ClientWidth+100+20;

	if (vp > cp)
	   nPos = (unsigned int)cp;

   m_PanX = nPos;

	SendMessage(WM_PAINT);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSkinEditorDlg::OnVScroll(unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) 
{
   if (nSBCode == SB_ENDSCROLL)
   {
   	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
      return;
   }
   if (nSBCode != SB_THUMBPOSITION && nSBCode != SB_THUMBTRACK)
      nPos = pScrollBar->GetScrollPos();
   if (nSBCode == SB_LINELEFT)
      nPos --;
   if (nSBCode == SB_LINERIGHT)
      nPos ++;
   if (nSBCode == SB_PAGELEFT)
      nPos -= 10;
   if (nSBCode == SB_PAGERIGHT)
      nPos += 10;

   
   int ClientWidth, ClientHeight;
   RECT     ClientRect;

   GetClientRect(&ClientRect);
   ClientWidth = ClientRect.right;
   ClientHeight = ClientRect.bottom;

	double
   	vp = nPos,
      cp = g_SkinZoomFactor*g_CurMdl.Head.skinheight+20;

	if (vp > cp)
      nPos = (unsigned int)cp;

   m_PanY = nPos;
	
   SendMessage(WM_PAINT);

   CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSkinEditorDlg::OnOptionsLinesDontdraw() 
{
   GetMenu()->CheckMenuRadioItem(ID_OPTIONS_LINES_DONTDRAW, 
                                 ID_OPTIONS_LINES_NORMAL, 
                                 ID_OPTIONS_LINES_DONTDRAW, 
                                 MF_BYCOMMAND);
   SendMessage(WM_PAINT);
}

void CSkinEditorDlg::OnOptionsLinesNormal() 
{
   GetMenu()->CheckMenuRadioItem(ID_OPTIONS_LINES_DONTDRAW, 
                                 ID_OPTIONS_LINES_NORMAL, 
                                 ID_OPTIONS_LINES_NORMAL, 
                                 MF_BYCOMMAND);
	
   SendMessage(WM_PAINT);
}

void CSkinEditorDlg::OnOptionsVerticesNone() 
{
   GetMenu()->CheckMenuRadioItem(ID_OPTIONS_VERTICES_NONE, 
                                 ID_OPTIONS_VERTICES_TICKS, 
                                 ID_OPTIONS_VERTICES_NONE, 
                                 MF_BYCOMMAND);
	
   SendMessage(WM_PAINT);
}

void CSkinEditorDlg::OnOptionsVerticesDots() 
{
   GetMenu()->CheckMenuRadioItem(ID_OPTIONS_VERTICES_NONE, 
                                 ID_OPTIONS_VERTICES_TICKS, 
                                 ID_OPTIONS_VERTICES_DOTS, 
                                 MF_BYCOMMAND);
	
   SendMessage(WM_PAINT);
}

void CSkinEditorDlg::OnOptionsVerticesTicks() 
{
   GetMenu()->CheckMenuRadioItem(ID_OPTIONS_VERTICES_NONE, 
                                 ID_OPTIONS_VERTICES_TICKS, 
                                 ID_OPTIONS_VERTICES_TICKS, 
                                 MF_BYCOMMAND);
	
   SendMessage(WM_PAINT);
}

void CSkinEditorDlg::OnLButtonDown(unsigned int nFlags, CPoint point) 
{
   if (point.x < m_SideToolDlg.GetWidth() || point.y>GetViewportHeight())
   {
   	CDialog::OnLButtonDown(nFlags, point);
      return;
   }

   m_ButtonState.m_Left = true;
   m_ShiftState.SetFromFlags(nFlags);

   MouseDown(m_ButtonState, m_ShiftState, point.x, point.y); 
   
  	CDialog::OnLButtonDown(nFlags, point);
}

void CSkinEditorDlg::OnLButtonUp(unsigned int nFlags, CPoint point) 
{
   m_ButtonState.m_Left = false;
   m_ShiftState.SetFromFlags(nFlags);

   MouseUp(m_ButtonState, m_ShiftState, point.x, point.y); 
   
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CSkinEditorDlg::OnRButtonDown(unsigned int nFlags, CPoint point) 
{
// First determine if the point is inside the for viewports or not
   if (point.x < m_SideToolDlg.GetWidth() || point.y>GetViewportHeight())
   {
   	CDialog::OnLButtonDown(nFlags, point);
      return;
   }

   m_ButtonState.m_Right = true;
   m_ShiftState.SetFromFlags(nFlags);

   MouseDown(m_ButtonState, m_ShiftState, point.x, point.y); 
   
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CSkinEditorDlg::OnRButtonUp(unsigned int nFlags, CPoint point) 
{
   m_ButtonState.m_Right = false;
   m_ShiftState.SetFromFlags(nFlags);

   MouseUp(m_ButtonState, m_ShiftState, point.x, point.y); 
   
	
	CDialog::OnRButtonUp(nFlags, point);
}

void CSkinEditorDlg::OnMButtonDown(unsigned int nFlags, CPoint point) 
{
// First determine if the point is inside the for viewports or not
   if (point.x < m_SideToolDlg.GetWidth() || point.y>GetViewportHeight())
   {
   	CDialog::OnLButtonDown(nFlags, point);
      return;
   }

   m_ButtonState.m_Mid = true;
   m_ShiftState.SetFromFlags(nFlags);

   MouseDown(m_ButtonState, m_ShiftState, point.x, point.y); 
   
	
	CDialog::OnMButtonDown(nFlags, point);
}

void CSkinEditorDlg::OnMButtonUp(unsigned int nFlags, CPoint point) 
{
   m_ButtonState.m_Mid = false;
   m_ShiftState.SetFromFlags(nFlags);

   MouseUp(m_ButtonState, m_ShiftState, point.x, point.y); 
   
	
	CDialog::OnMButtonUp(nFlags, point);
}

void CSkinEditorDlg::MouseDown(CMouseState Button, CShiftState Shift, int X, int Y)
{
   SetCapture();
   
   g_DownS = (X - m_SideToolDlg.GetWidth() + m_HorizScroll.GetScrollPos())/g_SkinZoomFactor;
   g_DownT = (Y + m_VertScroll.GetScrollPos())/g_SkinZoomFactor;

	if (m_ModifyTab.m_SelectBut.GetCheck())
   	if (Shift.m_Alt)
      	g_SkinAction = atDeselect;
      else
	   	g_SkinAction = atSelect;
   if (m_ModifyTab.m_MoveBut.GetCheck())
   	g_SkinAction = atMove;
   if (m_ModifyTab.m_RotateBut.GetCheck())
   	g_SkinAction = atRotate;
   if (m_ModifyTab.m_ScaleBut.GetCheck())
   	g_SkinAction = atScale;

   m_SkinDiffAng = 0;
   if (g_SkinAction == atScale)
      m_SkinDiffX = m_SkinDiffY = 1;
   else
      m_SkinDiffX = m_SkinDiffY = 0;

   if (g_SkinAction == atMove || g_SkinAction == atRotate || g_SkinAction == atScale)
   {
   	bool seld=false;
   	if (g_SkinSelectMode == stVertex)
      {
	      for(int i=0;i<g_CurMdl.Head.num_st;i++)
				if (g_CurMdl.SkinVerts[i].Selected)
            {
            	seld = true;
               i=g_CurMdl.Head.num_st;
            }
      }
   	if (g_SkinSelectMode == stTriangle)
      {
	      for(int i=0;i<g_CurMdl.Head.num_tris;i++)
				if (g_CurMdl.Tris[i].SkinSelected)
            {
            	seld = true;
               i=g_CurMdl.Head.num_tris;
            }
      }
      if (Shift.m_Alt)
         g_SkinAction = atDeselect;
      else
      if (Shift.m_Ctrl)
         g_SkinAction = atSelect;
      else
      if (seld==false)
      	g_SkinAction = atSelect;
	}
}

void CSkinEditorDlg::MouseUp(CMouseState Button, CShiftState Shift, int X, int Y)
{
   g_UpS = (X - m_SideToolDlg.GetWidth() + m_HorizScroll.GetScrollPos())/g_SkinZoomFactor;
   g_UpT = (Y + m_VertScroll.GetScrollPos())/g_SkinZoomFactor;

   if ((Button.m_Left || Button.m_Right || Button.m_Mid) == false)
   {
      ReleaseCapture();
   }
   
	if (g_SkinAction == atSelect || g_SkinAction == atDeselect)
   {
      bool target,changed = false;
      bool *o,*n;

      if (g_SkinAction == atSelect)
         target = true;
      else
         target = false;

   	if (g_SkinSelectMode == stVertex)
      {
         o = new bool[g_CurMdl.Head.num_st];
         n = new bool[g_CurMdl.Head.num_st];

         g_CurMdl.LoadSelection(n, ssSkinVert);

         for (int i=0;i<g_CurMdl.Head.num_st;i++)
         {
            if ( fabs(g_CurMdl.SkinVerts[i].s - g_UpS) < 1 &&
                 fabs(g_CurMdl.SkinVerts[i].t - g_UpT) < 1)
            {
               if (g_CurMdl.SkinVerts[i].Selected != target)
                  changed = true;
               g_CurMdl.SkinVerts[i].Selected = target;
            }
         }

         g_CurMdl.LoadSelection(o,ssSkinVert);
      }
      else
      if (g_SkinSelectMode == stTriangle)
      {
         o = new bool[g_CurMdl.Head.num_tris];
         n = new bool[g_CurMdl.Head.num_tris];

         g_CurMdl.LoadSelection(n, ssSkinTri);


      	for (int t=0;t<g_CurMdl.Head.num_tris;t++)
         {
            if (InTri(  g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[0]].s, g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[0]].t,
                        g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[1]].s, g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[1]].t,
                        g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[2]].s, g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[2]].t,
                        g_UpS, g_UpT))
            {
               if (g_CurMdl.Tris[t].SkinSelected != target)
                  changed = true;
               g_CurMdl.Tris[t].SkinSelected = target;
            }
         }
         g_CurMdl.LoadSelection(o,ssSkinTri);
      }
      if (changed)
      {
         CSkinSelectUndoNode   *N;
         N = new CSkinSelectUndoNode;
         N->Set("Select", &g_CurMdl, g_SkinSelectMode, o, n);
         g_SkinUndo.AddNode(N);
      }
      delete[] o;
      delete[] n;
	   DoSync();
   }

	if (g_SkinAction == atSelectBox || g_SkinAction == atDeselectBox)
   {
      bool  *NewSelect;
      int   i;

   	if (g_SkinSelectMode == stVertex)
      {
         NewSelect = new bool[g_CurMdl.Head.num_st];
         for (i=0;i<g_CurMdl.Head.num_st;i++)
            NewSelect[i] = false;

         for (i=0;i<g_CurMdl.Head.num_st;i++)
         {
            if (InRect(g_DownS,g_DownT,g_UpS,g_UpT,g_CurMdl.SkinVerts[i].s,g_CurMdl.SkinVerts[i].t))
               NewSelect[i] = true;
         }

         bool  *n,*o;

         n = new bool[g_CurMdl.Head.num_st];
         o = new bool[g_CurMdl.Head.num_st];

         g_CurMdl.LoadSelection(n, ssSkinVert);

         if (g_SkinAction == atSelectBox)
            for (int v=0;v<g_CurMdl.Head.num_st;v++)
               g_CurMdl.SkinVerts[v].Selected |= NewSelect[v];
         if (g_SkinAction == atDeselectBox)
            for (int v=0;v<g_CurMdl.Head.num_st;v++)
               g_CurMdl.SkinVerts[v].Selected &= !NewSelect[v];

         g_CurMdl.LoadSelection(o, ssSkinVert);

         bool changed = false;

         for (i=0;i<g_CurMdl.Head.num_st;i++)
            if (n[i] != o[i])
            {
               changed = true;
               break;
            }

         if (changed)
         {
            CSkinSelectUndoNode   *N;
            N = new CSkinSelectUndoNode;
            N->Set("Select", &g_CurMdl, g_SkinSelectMode, o, n);
            g_SkinUndo.AddNode(N);
         }
         delete[] o;
         delete[] n;
      }
      else
      if (g_SkinSelectMode == stTriangle)
      {
         int      t;
         NewSelect = new bool[g_CurMdl.Head.num_tris];
         for (t=0;t<g_CurMdl.Head.num_tris;t++)
            NewSelect[t] = false;

      	for (t=0;t<g_CurMdl.Head.num_tris;t++)
         {
         	int v;

            for (int i=0;i<3;i++)
            {
	            v = g_CurMdl.Tris[t].SkinVerts[i];
            if (InRect(g_DownS,g_DownT,g_UpS,g_UpT,g_CurMdl.SkinVerts[v].s,g_CurMdl.SkinVerts[v].t))
                  NewSelect[t] = true;
            }
         }

         bool                 *n, *o;

         n = new bool[g_CurMdl.Head.num_tris];
         o = new bool[g_CurMdl.Head.num_tris];

         g_CurMdl.LoadSelection(n, ssSkinTri);

         if (g_SkinAction == atSelectBox)
            for (int t=0;t<g_CurMdl.Head.num_tris;t++)
               g_CurMdl.Tris[t].SkinSelected |= NewSelect[t];

         if (g_SkinAction == atDeselectBox)
            for (int t=0;t<g_CurMdl.Head.num_tris;t++)
               g_CurMdl.Tris[t].SkinSelected &= !NewSelect[t];

         g_CurMdl.LoadSelection(o, ssSkinTri);
         bool changed = false;
         for (int i=0;i<g_CurMdl.Head.num_st;i++)
            if (n[i] != o[i])
            {
               changed = true;
               break;
            }

         if (changed)
         {
            CSkinSelectUndoNode   *N;
            N = new CSkinSelectUndoNode;
            N->Set("Select", &g_CurMdl, g_SkinSelectMode, o, n);
            g_SkinUndo.AddNode(N);
         }

         delete[] o;
         delete[] n;
      }


      delete[] NewSelect;
	   DoSync();
   }

   if (g_SkinAction == atMove)
   {
      CSkinMoveUndoNode  *M;

      M = new CSkinMoveUndoNode;

      M->Set("Move ", &g_CurMdl, -m_SkinDiffX, -m_SkinDiffY, g_SkinSelectMode);
      g_SkinUndo.AddNode(M);
   }
   if (g_SkinAction == atRotate)
   {
      CSkinRotateUndoNode   *M;

      M = new CSkinRotateUndoNode;

      M->Set("Rotate ", &g_CurMdl, g_DownS, g_DownT, -m_SkinDiffAng,g_SkinSelectMode);

      g_SkinUndo.AddNode(M);
   }
   if (g_SkinAction == atScale)
   {
      CSkinScaleUndoNode  *M;

      M = new CSkinScaleUndoNode;

      if (m_SkinDiffX == 0)
         m_SkinDiffX = 0.001;
      if (m_SkinDiffY == 0)
         m_SkinDiffY = 0.001;
      M->Set("Scale ", &g_CurMdl, g_DownS, g_DownT, 1/m_SkinDiffX, 1/m_SkinDiffY, g_SkinSelectMode);
      g_SkinUndo.AddNode(M);
   }

	g_SkinAction = atNone;

   DoSync();
   SendMessage(WM_PAINT);
}

void CSkinEditorDlg::DoSync()
{
   if (this->IsMenuChecked(ID_OPTIONS_SYNC3DSELECTION))
   {
      for (int i=0;i<g_CurMdl.Head.num_tris;i++)
      {
        	g_CurMdl.Tris[i].Selected = g_CurMdl.Tris[i].SkinSelected;
      }

      if (g_MainSelectMode == stTriangle)
         m_MainDlg->SendMessage(MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL);
   }
   m_MainDlg->SendMessage(MM_UPDATEFEEDBACK,MMC_SELECTCOUNTERS,0);
}

void CSkinEditorDlg::OnMouseMove(unsigned int nFlags, CPoint point) 
{
   int   X,Y;
   int   ClientWidth, ClientHeight, PanelWidth;
   RECT  ClientRect;
   CShiftState Shift;

   Shift.SetFromFlags(nFlags);

   GetClientRect(&ClientRect);
   ClientWidth = ClientRect.right;
   ClientHeight = ClientRect.bottom;

   X = point.x;
   Y = point.y;
   
   PanelWidth = GetPanelWidth();


   DoCursors(Shift);

   bool 	*selected,Update = false;
   selected = new bool[g_CurMdl.Head.num_st];
   for (int i=0;i<g_CurMdl.Head.num_st;i++)
      selected[i] = false;

   if (g_SkinAction != atNone)
   {
      WrapMouseToScreen(m_hWnd, X,Y,g_ox, g_oy);
   }

   movex = (X - PanelWidth + m_HorizScroll.GetScrollPos())/g_SkinZoomFactor;
   movey = (Y + m_VertScroll.GetScrollPos())/g_SkinZoomFactor;

   if (g_SkinSelectMode == stVertex)
   {
   	for (int i=0;i<g_CurMdl.Head.num_st;i++)
      	selected[i] = g_CurMdl.SkinVerts[i].Selected;
   }
	else
   if (g_SkinSelectMode == stTriangle)
   {
   	for (int i=0;i<g_CurMdl.Head.num_tris;i++)
      	if (g_CurMdl.Tris[i].SkinSelected)
         {
         	selected[g_CurMdl.Tris[i].SkinVerts[0]] = true;
         	selected[g_CurMdl.Tris[i].SkinVerts[1]] = true;
         	selected[g_CurMdl.Tris[i].SkinVerts[2]] = true;
         }
   }

	if (g_SkinAction == atSelect)
   	g_SkinAction = atSelectBox;
   if (g_SkinAction == atDeselect)
   	g_SkinAction = atDeselectBox;

   if (g_SkinAction == atMove)
   {
      double dx, dy;
      bool cando = true;

      if (m_ModifyTab.m_XBut.GetCheck())
         dx = movex - omovex;
      else
         dx = 0;

      if (m_ModifyTab.m_YBut.GetCheck())
         dy = movey - omovey;
      else
         dy = 0;

      for (int i=0;i<g_CurMdl.Head.num_st;i++)
         if (selected[i])
         {
            float s,t;
            s = (float)(g_CurMdl.SkinVerts[i].s + dx);
	         t = (float)(g_CurMdl.SkinVerts[i].t + dy);

            if (s<0 || s>=g_CurMdl.Head.skinwidth || t<0 || t>=g_CurMdl.Head.skinheight)
            {
               cando = false;
               i = g_CurMdl.Head.num_st;
            }
         }

      if (cando)
      {
         for (int i=0;i<g_CurMdl.Head.num_st;i++)
            if (selected[i])
            {
            	if (m_ModifyTab.m_XBut.GetCheck())
	               g_CurMdl.SkinVerts[i].s += (float)dx;

               if (m_ModifyTab.m_YBut.GetCheck())
	               g_CurMdl.SkinVerts[i].t += (float)dy;
            }

         m_SkinDiffX += dx;
         m_SkinDiffY += dy;

         Update = true;
         g_ChangesMade = 1;
      }
   }

   if (g_SkinAction == atScale)
   {
      bool cando = true;

      double ScaleX, ScaleY;

      if (m_ModifyTab.m_XBut.GetCheck())
         ScaleX = (1 + 0.01*(-movey + omovey));
      else ScaleX = 1;
      if (m_ModifyTab.m_YBut.GetCheck())
         ScaleY = (1 + 0.01*(-movey + omovey));
      else ScaleY = 1;

      for (int i=0;i<g_CurMdl.Head.num_st;i++)
         if (selected[i])
         {
            float s,t;

            s = (float)((g_CurMdl.SkinVerts[i].s - g_DownS)*ScaleX + g_DownS);
            t = (float)((g_CurMdl.SkinVerts[i].t - g_DownT)*ScaleY + g_DownT);

            if (s<0 || s>=g_CurMdl.Head.skinwidth || t<0 || t>=g_CurMdl.Head.skinheight)
            {
               cando = false;
               i = g_CurMdl.Head.num_st;
            }
         }

      if (cando)
      {
         for (int i=0;i<g_CurMdl.Head.num_st;i++)
            if (selected[i])
            {
               float s,t;
               s = (float)((g_CurMdl.SkinVerts[i].s - g_DownS)*ScaleX + g_DownS);
               t = (float)((g_CurMdl.SkinVerts[i].t - g_DownT)*ScaleY + g_DownT);

               g_CurMdl.SkinVerts[i].s = s;
               g_CurMdl.SkinVerts[i].t = t;
            }

         m_SkinDiffX *= ScaleX;
         m_SkinDiffY *= ScaleY;

   		Update = true;
         g_ChangesMade = 1;
      }
   }

   if (g_SkinAction == atRotate)
   {
      bool cando = true;
      float ang,nx,ny;

      ang = (float)(M_PI/360.0*(omovey-movey)/g_SkinZoomFactor);

      for (int i = 0;i<g_CurMdl.Head.num_st;i++)
         if (selected[i])
         {
            nx = (float)((g_CurMdl.SkinVerts[i].s - g_DownS)*cos(ang) + (g_CurMdl.SkinVerts[i].t - g_DownT)*sin(ang));
            ny = (float)((g_CurMdl.SkinVerts[i].t - g_DownT)*cos(ang) - (g_CurMdl.SkinVerts[i].s - g_DownS)*sin(ang));
            nx+=g_DownS;
            ny+=g_DownT;

            if (nx<0 || nx>=g_CurMdl.Head.skinwidth || ny<0 || ny>=g_CurMdl.Head.skinheight)
               cando = false;
         }

      if (cando)
      {
         for (int i = 0;i<g_CurMdl.Head.num_st;i++)
            if (selected[i])
            {
               nx = (float)((g_CurMdl.SkinVerts[i].s - g_DownS)*cos(ang) + (g_CurMdl.SkinVerts[i].t - g_DownT)*sin(ang));
               ny = (float)((g_CurMdl.SkinVerts[i].t - g_DownT)*cos(ang) - (g_CurMdl.SkinVerts[i].s - g_DownS)*sin(ang));
               nx+=g_DownS;
               ny+=g_DownT;

               g_CurMdl.SkinVerts[i].s = nx;
               g_CurMdl.SkinVerts[i].t = ny;
            }

         m_SkinDiffAng += ang;
         Update = true;
   		g_ChangesMade = 1;
      }
   }

   if (g_SkinAction != atNone)
      SendMessage(MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN,0);

   if (Update)
   {
      if (m_MainDlg)
         m_MainDlg->SendMessage(MM_UPDATEVIEWPORTS, MMC_UPDATE_CAM,0);
   }

   omovex = movex;
   omovey = movey;

   delete[] selected;
	
	CDialog::OnMouseMove(nFlags, point);
}

void CSkinEditorDlg::DoCursors(CShiftState Shift)
{
   static unsigned int    LastCursor = NULL;
   unsigned int     targ = IDC_CURSORARROW;

   if (m_ModifyTab.m_MoveBut.GetCheck())
      targ = IDC_CURSORMOVE;
   else
   if (m_ModifyTab.m_RotateBut.GetCheck())
      targ = IDC_CURSORROTATE;
   else
   if (m_ModifyTab.m_ScaleBut.GetCheck())
      targ = IDC_CURSORSCALE;
   else
   if (m_ModifyTab.m_SelectBut.GetCheck())
   {
      if (Shift.m_Alt)
      {
         targ = IDC_CURSORMINUS;
      }
      else
      targ = IDC_CURSORARROW;
   }

   if (LastCursor != targ)
   {
      SetCursor(AfxGetApp()->LoadCursor(targ));
      LastCursor = targ;
   }

}

int CSkinEditorDlg::GetPanelWidth()
{
   RECT  ToolPanelRect;
   int   PanelWidth;

   m_ToolTab.GetWindowRect(&ToolPanelRect);
   PanelWidth = ToolPanelRect.right-ToolPanelRect.left + 2;

   return PanelWidth;
}

void CSkinEditorDlg::ToolButClick(HWND But)
{
	if (g_Closing)
   	return;

	if (But == m_CurTool)
   {
      if (m_LastTool)
      {
         ::SendMessage(m_LastTool, BM_CLICK,0,0);
         return;
      }
   }

	m_LastTool = m_CurTool;
  	m_CurTool = But;

   ::SendMessage(m_CurTool, BM_SETCHECK, 1,0);
   ::SendMessage(m_LastTool, BM_SETCHECK, 0,0);

   g_MainAction = atNone;
   DoCursors(CShiftState());

   SendMessage(WM_PAINT);
}

void CSkinEditorDlg::OnEditUndo() 
{
   KSkinUndo();
   
}

void CSkinEditorDlg::OnEditRedo() 
{
   KSkinRedo();
   
}


int CSkinEditorDlg::GetViewportHeight()
{
   int ClientWidth, ClientHeight;
   RECT     ClientRect;
   
   GetClientRect(&ClientRect);
   ClientWidth = ClientRect.right;
   ClientHeight = ClientRect.bottom;
   
   return ClientHeight-m_BottomToolDlg.GetHeight();
}

void CSkinEditorDlg::OnKeyDown(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) 
{
   SetFocus();
   // Set up the ctrl, shift, and alt members of the m_KeyState object.
    
   if (nChar == VK_CONTROL)  // control key
   {
      m_KeyState.m_Ctrl = true;
      CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
      return;
      
   }
   if (nChar == VK_SHIFT)  // shift key
   {
      m_KeyState.m_Shift = true;
      CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
      return;
   }

   if (nFlags & (0x01 << 13))
   {
      m_KeyState.m_Alt = true;
   }
   else
   {
      m_KeyState.m_Alt = false;
   }

   {
      bool s,a,c;
      s = m_KeyState.m_Shift;
      a = m_KeyState.m_Alt;
      c = m_KeyState.m_Ctrl;

      if (g_KeyList.ExecuteKey(nChar,c,a,s,"Skin"))
         return;
   }
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSkinEditorDlg::OnKeyUp(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) 
{
   SetFocus();
   if (nChar == VK_CONTROL)  // control key
   {
      m_KeyState.m_Ctrl = false;
      CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
      return;
      
   }
   if (nChar == VK_SHIFT)  // shift key
   {
      m_KeyState.m_Shift = false;
      CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
      return;
   }
	
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CSkinEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
   // Only let the main window handle key down messages
   if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
   {
      if (::IsChild(m_hWnd, pMsg->hwnd))
         return PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
   }

	return CDialog::PreTranslateMessage(pMsg);
}

void CSkinEditorDlg::OnEditSelectall() 
{
   KSkinSelectAll();
   
}

void CSkinEditorDlg::OnEditSelectconnected() 
{
   KSkinSelectConnected();
	
}

void CSkinEditorDlg::OnEditSelectinverse() 
{
   KSkinSelectInverse();
}

void CSkinEditorDlg::OnEditSelectnone() 
{
   KSkinSelectNone();
	
}

void CSkinEditorDlg::OnEditSelecttouching() 
{
   KSkinSelectTouching();
	
}

void CSkinEditorDlg::OnOptionsSync3dselection() 
{
   ToggleMenuChecked(ID_OPTIONS_SYNC3DSELECTION);
   m_MainDlg->GetMenu()->CheckMenuItem(ID_OPTIONS_SYNCSKINSELECTION, IsMenuChecked(ID_OPTIONS_SYNCSKINSELECTION));

   DoSync();
	
}

unsigned int CSkinEditorDlg::ToggleMenuChecked(unsigned int IDItem)
{
   if (IsMenuChecked(IDItem))
      return GetMenu()->CheckMenuItem(IDItem, MF_UNCHECKED);
   else
      return GetMenu()->CheckMenuItem(IDItem, MF_CHECKED);
}

void CSkinEditorDlg::OnFileImportSkinimage() 
{
   KImportSkinImage();
}

void CSkinEditorDlg::OnFileImportSkinmeshfromasc() 
{
   KImportSkinMesh();	
}

void CSkinEditorDlg::OnFileExportCurrentimageBmp() 
{
   KExportImageBMP();
   
}

void CSkinEditorDlg::OnFileExportCurrentimagePcx() 
{
   KExportImagePCX();
	
}

void CSkinEditorDlg::OnFileExportSkinmeshtoasc() 
{
   KExportSkinMesh();
	
}

void CSkinEditorDlg::OnEditGetpositionfrommodel() 
{
   KGetPosition();	
}

void CSkinEditorDlg::OnEditAddnewskin() 
{
   KAddSkin();	
}

void CSkinEditorDlg::OnEditDeletecurrentskin() 
{
   KDeleteSkin();
	
}

void CSkinEditorDlg::OnEditClearcurrentskin() 
{
   KClearSkin();
	
}

void CSkinEditorDlg::OnEditResizeskin() 
{
   KResizeSkin();
	
}

void CSkinEditorDlg::OnViewSkinpainter() 
{
   KViewSkinPainter();
   
}

void CSkinEditorDlg::OnView3deditor() 
{
   KView3DEditor();	
}

void CSkinEditorDlg::OnOptionsKeepwindowontop() 
{
   if (IsMenuChecked(ID_OPTIONS_KEEPWINDOWONTOP))
      GetMenu()->CheckMenuItem(ID_OPTIONS_KEEPWINDOWONTOP, MF_UNCHECKED);
   else
      GetMenu()->CheckMenuItem(ID_OPTIONS_KEEPWINDOWONTOP, MF_CHECKED);
   
   KViewSkins();
}
