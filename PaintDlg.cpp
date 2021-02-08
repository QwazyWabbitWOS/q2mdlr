// PaintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"

#include "MdlGlobs.h"
#include "MdlMsgs.h"
#include "MainFunc.h"
#include "KeyFuncs.h"

#include "PaintDlg.h"



#include "Colors.h"

#include "Math.h"

#include "io.h"
#include "fcntl.h"
#include "sys/stat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define peNormal        0
#define peColor         1
#define peBrightness    2



/////////////////////////////////////////////////////////////////////////////
// CPaintDlg dialog

CPaintDlg::CPaintDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaintDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaintDlg)
	//}}AFX_DATA_INIT


   m_MainDlg = NULL;
   m_UVData = NULL;

   m_IntermediateSkin = NULL;
   m_IntermediateShade = NULL;
   m_XParent = NULL;

   m_PaintUndoNode = NULL;
}

CPaintDlg::~CPaintDlg()
{
   if (m_UVData)
      delete[] m_UVData;

   if (m_IntermediateSkin)
      delete [] m_IntermediateSkin;
   if (m_IntermediateShade)
      delete [] m_IntermediateShade;
   if (m_XParent)
      delete [] m_XParent;

   if (m_PaintUndoNode)
      delete m_PaintUndoNode;
}

void CPaintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaintDlg)
	DDX_Control(pDX, IDC_TOOLTAB, m_ToolTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaintDlg, CDialog)
	//{{AFX_MSG_MAP(CPaintDlg)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_VIEW_3DEDITOR, OnView3deditor)
	ON_COMMAND(ID_VIEW_SKINPAINTER, OnViewSkinpainter)
	ON_COMMAND(ID_VIEW_SKINEDITOR, OnViewSkineditor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaintDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CPaintBottomDlg dialog


CPaintBottomDlg::CPaintBottomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaintBottomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaintBottomDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPaintBottomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaintBottomDlg)
	DDX_Control(pDX, IDC_VEDIT, m_VEdit);
	DDX_Control(pDX, IDC_UEDIT, m_UEdit);
	DDX_Control(pDX, IDC_PANVIEWSBUT, m_PanViewsBut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaintBottomDlg, CDialog)
	//{{AFX_MSG_MAP(CPaintBottomDlg)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaintBottomDlg message handlers

BOOL CPaintDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_BottomToolDlg.Create(IDD_PAINT_TOOLDLG_BOTTOM, this);
   m_BottomToolDlg.ShowWindow(SW_SHOW);

   m_ToolTab.AddTab("Paint", &m_PaintTab, IDD_PAINT_TABDLG_PAINT);

   m_ToolTab.ResizeControlToLargest();

   AddWndStyle(m_PaintTab.m_DrawBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_PaintTab.m_FillBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_PaintTab.m_SprayBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   
   AddWndStyle(m_BottomToolDlg.m_PanViewsBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);

   m_CurTool = m_PaintTab.m_DrawBut.m_hWnd;
   m_LastTool = m_BottomToolDlg.m_PanViewsBut.m_hWnd;
   ::SendMessage(m_CurTool, BM_SETCHECK, 1,0);
   ::SendMessage(m_LastTool, BM_SETCHECK, 0,0);

   
   g_PaintUndo.RedoList = &g_PaintRedo;
   g_PaintUndo.DeleteRedoOnAdd = true;
   g_PaintUndo.MenuPrefix = "Undo ";
   g_PaintUndo.SetAssociatedMenu(GetMenu()->m_hMenu, ID_EDIT_UNDO);
   g_PaintUndo.UpdateMenuItem();
   g_PaintRedo.RedoList = &g_PaintUndo;
   g_PaintRedo.MenuPrefix = "Redo ";
   g_PaintRedo.SetAssociatedMenu(GetMenu()->m_hMenu, ID_EDIT_REDO);
   g_PaintRedo.UpdateMenuItem();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPaintDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanged(lpwndpos);
	
   RECT  ClientRect, BottomDlgRect, TabCtrlRect;

   GetClientRect(&ClientRect);

   m_ToolTab.GetWindowRect(&TabCtrlRect);
   m_ToolTab.ScreenToClient(&TabCtrlRect);

   if (m_BottomToolDlg.m_hWnd)
   {
      m_BottomToolDlg.GetWindowRect(&BottomDlgRect);
      m_BottomToolDlg.ScreenToClient(&BottomDlgRect);

      m_BottomToolDlg.MoveWindow(0,ClientRect.bottom-BottomDlgRect.bottom, ClientRect.right, BottomDlgRect.bottom);
   }

   DoResize();

   m_ToolTab.ResizeTabCtrl(TabCtrlRect.right, ClientRect.bottom - BottomDlgRect.bottom);	
   
}

void CPaintBottomDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanged(lpwndpos);
	
   RECT  PanViewsRect, ClientRect;

   // Move the Pan Views but to the right place.
   GetClientRect(&ClientRect);
   m_PanViewsBut.GetWindowRect(&PanViewsRect);
   m_PanViewsBut.ScreenToClient(&PanViewsRect);
   
   m_PanViewsBut.MoveWindow(ClientRect.right - PanViewsRect.right - 4, 4, PanViewsRect.right, PanViewsRect.bottom);

   
}

void CPaintDlg::OnKeyDown(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) 
{
   SetFocus();
   // Set up the ctrl, shift, and alt members of the m_ShiftState object.
    
   if (nChar == VK_CONTROL)  // control key
   {
      m_ShiftState.m_Ctrl = true;
      CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
      return;
      
   }
   if (nChar == VK_SHIFT)  // shift key
   {
      m_ShiftState.m_Shift = true;
      CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
      return;
   }

   if (nFlags & (0x01 << 13))
   {
      m_ShiftState.m_Alt = true;
   }
   else
   {
      m_ShiftState.m_Alt = false;
   }

   {
      bool s,a,c;
      s = m_ShiftState.m_Shift;
      a = m_ShiftState.m_Alt;
      c = m_ShiftState.m_Ctrl;

      if (g_KeyList.ExecuteKey(nChar,c,a,s,"Paint"))
         return;
   }
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPaintDlg::OnKeyUp(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) 
{
   SetFocus();
   if (nChar == VK_CONTROL)  // control key
   {
      m_ShiftState.m_Ctrl = false;
      CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
      return;
      
   }
   if (nChar == VK_SHIFT)  // shift key
   {
      m_ShiftState.m_Shift = false;
      CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
      return;
   }
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CPaintBottomDlg::PreTranslateMessage(MSG* pMsg) 
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

LRESULT CPaintDlg::WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam) 
{
   if (message == MM_PAINT_UPDATECOLORS)
   {
      UpdateColors();
      m_PaintTab.m_ForeBut.SetColor(RGBToColorRef(Palette[m_PaintTab.m_ForeColor]));
      m_PaintTab.m_BackBut.SetColor(RGBToColorRef(Palette[m_PaintTab.m_BackColor]));

      return 1;
   }
   if (message == MM_LOADPAINTDATA)
   {
      LoadPaintData();
   }
   if (message == MM_TOOLBUTCLICK)
   {
      HWND  hBut;

      hBut = (HWND)lParam;

      ToolButClick(hBut);

      return 1;
   }
   if (message == MM_UPDATEVIEWPORTS)
   {
      OnPaint();
   }
   
   
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPaintDlg::PreTranslateMessage(MSG* pMsg) 
{
   /*
   if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
   {
      LONG  WndStyle;

      WndStyle = GetWindowLong(pMsg->hwnd, GWL_STYLE);
      if (WndStyle & ES_AUTOHSCROLL)
      {
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
      return GetParentOwner()->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
   }
*/	
   // Only let the main window handle key down messages
   if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
   {

      if (::IsChild(m_hWnd, pMsg->hwnd))
      {
         return PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
      }
   }

	return CDialog::PreTranslateMessage(pMsg);
}

void CPaintDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

   if (IsIconic())
   {
      return;
   }

   int   SideTabWidth, BottomDlgHeight, Width, Height;
   RECT  SideTabRect, BottomDlgRect, ClientRect;

   m_ToolTab.GetWindowRect(&SideTabRect);
   m_ToolTab.ScreenToClient(&SideTabRect);
   SideTabWidth = SideTabRect.right;

   m_BottomToolDlg.GetWindowRect(&BottomDlgRect);
   m_BottomToolDlg.ScreenToClient(&BottomDlgRect);
   BottomDlgHeight = BottomDlgRect.bottom;

   GetClientRect(&ClientRect);

   m_CurExtents.left = 0;
   m_CurExtents.right = ClientRect.right - SideTabWidth;
   m_CurExtents.top = 0;
   m_CurExtents.bottom = ClientRect.bottom - BottomDlgHeight;

   DrawSkinPaint3DView(m_View);

   Width = m_DrawExtents.right-m_DrawExtents.left;
   Height = m_DrawExtents.bottom-m_DrawExtents.top;

//   m_View.StretchToDC(SkinPainterForm->Canvas->Handle,SideTabWidth,0,w,h,0,0,w,h);

   CDC   *DC;
   DC = GetDC();

   m_View.StretchToDC( DC->m_hDC,
                     SideTabWidth + m_DrawExtents.left, m_DrawExtents.top, Width, Height,
                     m_DrawExtents.left, m_View.Height - m_DrawExtents.bottom,Width,Height);

   ReleaseDC(DC);
//   m_View.DrawToDC(SkinPainterForm->Canvas->Handle, SideTabWidth,0);
	
	// Do not call CDialog::OnPaint() for painting messages
}


void CPaintDlg::DrawSkinPaint3DView(TDIBitmap8 &Dest)
{
	if (g_CurMdl.Head.num_skins == 0)
	   return;
	if (g_CurMdl.Skins[g_CurSkin] == NULL)
   	return;
   if (g_CurMdl.Head.skinwidth == 0 || g_CurMdl.Head.skinheight == 0)
   	return;
   if (Dest.Data == NULL)
   	return;

	if (Dest.TexWidth != g_CurMdl.Head.skinwidth ||
   	 Dest.TexHeight != g_CurMdl.Head.skinheight )
       Dest.SetTexSize(g_CurMdl.Head.skinwidth, g_CurMdl.Head.skinheight);

//   if (g_CurMdl.Verts==NULL || g_CurMdl.Tris==NULL) return;
//	int *order;
   float *trisz,*poszinv;
   bool *todraw,*tridone, *drawtri;


  	float nx,ny,nz;

  	if (Dest.PInfoHeader == NULL)
   	return;

   Dest.Clear(BACKGROUND3D);

   m_DrawExtents = m_CurExtents;

   if (g_CurMdl.Head.num_xyz >0)
   {
   	if (g_posx != NULL)
         delete[] g_posx;
      if (g_posy != NULL)
         delete[] g_posy;
      if (g_posz != NULL)
         delete[] g_posz;

		g_posx = new int[g_CurMdl.Head.num_xyz];
      g_posy = new int[g_CurMdl.Head.num_xyz];
//      order = new int[g_CurMdl.Head.num_tris];

   	g_posz = new float[g_CurMdl.Head.num_xyz];
      poszinv = new float[g_CurMdl.Head.num_xyz];
      trisz = new float[g_CurMdl.Head.num_tris];

      todraw = new bool[g_CurMdl.Head.num_xyz];
      tridone = new bool[g_CurMdl.Head.num_tris];
   }
   else
   {
   	if (Dest.PInfoHeader != NULL)
		   Dest.Rect(0,0,Dest.VisibleWidth-1,Dest.Height-1,BLACK);
   	return;
   }

   float cosa1,sina1,cosa2,sina2;

   cosa1 = (float)cos(RotX);
   sina1 = (float)sin(RotX);
   cosa2 = (float)cos(RotY);
   sina2 = (float)sin(RotY);

   for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
   {
   	float tempy;

      nx = (-g_CurMdl.Verts[n].x+PanX)*cosa1 + (-g_CurMdl.Verts[n].y+PanY)*sina1;
      tempy = (-g_CurMdl.Verts[n].y+PanY)*cosa1 - (-g_CurMdl.Verts[n].x+PanX)*sina1;

      nz = (g_CurMdl.Verts[n].z+PanZ)*cosa2 + (tempy)*sina2;
      ny = tempy*cosa2 - (g_CurMdl.Verts[n].z+PanZ)*sina2;

      nz+=Dist;
      if (nz > 1)
      {
         g_posz[n] = nz;
      	g_posx[n] = (int)(g_Persp*nx/nz);
         g_posy[n] = (int)(g_Persp*ny/nz);
         poszinv[n] = 1/nz;
			todraw[n] = true;

//         if (!ForceClip)
//         {

            if (g_posx[n]+m_ViewWidth2 < m_CurExtents.left)
               m_CurExtents.left = g_posx[n]+m_ViewWidth2;
            if (g_posy[n]+m_ViewHeight2 < m_CurExtents.top)
               m_CurExtents.top = g_posy[n]+m_ViewHeight2;
            if (g_posx[n]+2+m_ViewWidth2 > m_CurExtents.right)
               m_CurExtents.right = g_posx[n]+2+m_ViewWidth2;
            if (g_posy[n]+2+m_ViewHeight2 > m_CurExtents.bottom)
               m_CurExtents.bottom = g_posy[n]+2+m_ViewHeight2;
//         }
      }
      else todraw[n] = false;

   }

   if (m_CurExtents.left < 0)
      m_CurExtents.left = 0;
   if (m_CurExtents.top < 0)
      m_CurExtents.top = 0;

   if (m_CurExtents.right >= Dest.VisibleWidth)
      m_CurExtents.right = Dest.VisibleWidth-1;
   if (m_CurExtents.bottom >= Dest.Height)
      m_CurExtents.bottom = Dest.Height-1;

   m_DrawExtents = m_LastExtents;

   if (m_CurExtents.left < m_DrawExtents.left)
      m_CurExtents.left = m_DrawExtents.left;
   if (m_CurExtents.top < m_DrawExtents.top)
      m_CurExtents.top = m_DrawExtents.top;
   if (m_CurExtents.right < m_DrawExtents.right)
      m_CurExtents.right = m_DrawExtents.right;
   if (m_CurExtents.bottom < m_DrawExtents.bottom)
      m_CurExtents.bottom = m_DrawExtents.bottom;

//	for (int i = Dest.Width*Dest.Height-1;i>=0;i--)
//   	Dest.ZBuffer[i] = 0.0;


   {
      float *Z = Dest.ZBuffer;
      Z += m_DrawExtents.left + m_DrawExtents.top*Dest.Width;
      for (int i = m_DrawExtents.top;i<m_DrawExtents.bottom;i++)
      {
         memset(Z, 0 ,sizeof(float)*(m_DrawExtents.right-m_DrawExtents.left));

         Z+=Dest.Width;
      }
   }

//   return;
/*   int a,b;
   a = DMaxY;
   b = DMinY;
   DMinY = Dest.Height - b;
   DMaxY = Dest.Height - a;*/

	drawtri = new bool[g_CurMdl.Head.num_tris];

   for (int i=0;i<g_CurMdl.Head.num_tris;i++)
   {
      if ( todraw[g_CurMdl.Tris[i].Vertices[0]] &&
         todraw[g_CurMdl.Tris[i].Vertices[1]] &&
         todraw[g_CurMdl.Tris[i].Vertices[2]])
      {
      	float dotprod;

         dotprod = FaceDotProd3D(g_CurMdl, i,cosa1,sina1,cosa2,sina2);

			if (dotprod>0)
	   		drawtri[i] = true;
         else drawtri[i] = false;

      }
      else
      	drawtri[i] = false;

      if (g_CurMdl.Tris[i].Visible == false)
      	drawtri[i] = false;
   }


   {
      TexVertex   *TexVerts,*pTexVerts;
      int         n;

      TexVerts = new TexVertex[g_CurMdl.Head.num_xyz];
      pTexVerts = TexVerts;

      for (n=0;n<g_CurMdl.Head.num_xyz;n++)
      {
         pTexVerts->sx = (float)(m_ViewWidth2+ g_posx[n]+1);
         pTexVerts->sy = (float)(m_ViewHeight2+ g_posy[n]);
         pTexVerts->sz = poszinv[n];
//         pTexVerts->u = g_CurMdl.SkinVerts[n].s;
//         pTexVerts->v = g_CurMdl.SkinVerts[n].t;
         pTexVerts++;
      }

      TCompleteModel *M = &g_CurMdl;

      for (n=0;n<g_CurMdl.Head.num_tris;n++)
         if (drawtri[n])
         {
            TexTriangle	T;

            TexVerts[ M->Tris[n].Vertices[0] ].u = M->SkinVerts[ M->Tris[n].SkinVerts[0] ].s;
            TexVerts[ M->Tris[n].Vertices[0] ].v = M->SkinVerts[ M->Tris[n].SkinVerts[0] ].t;
            TexVerts[ M->Tris[n].Vertices[1] ].u = M->SkinVerts[ M->Tris[n].SkinVerts[1] ].s;
            TexVerts[ M->Tris[n].Vertices[1] ].v = M->SkinVerts[ M->Tris[n].SkinVerts[1] ].t;
            TexVerts[ M->Tris[n].Vertices[2] ].u = M->SkinVerts[ M->Tris[n].SkinVerts[2] ].s;
            TexVerts[ M->Tris[n].Vertices[2] ].v = M->SkinVerts[ M->Tris[n].SkinVerts[2] ].t;


            T.Tex = g_CurMdl.Skins[g_CurSkin];
            T.Tex = g_CurMdl.Skins[g_CurSkin];
            T.v[0] = &TexVerts[ g_CurMdl.Tris[n].Vertices[0] ];
            T.v[1] = &TexVerts[ g_CurMdl.Tris[n].Vertices[1] ];
            T.v[2] = &TexVerts[ g_CurMdl.Tris[n].Vertices[2] ];
            T.v[0]->u = g_CurMdl.SkinVerts[ g_CurMdl.Tris[n].SkinVerts[0] ].s;
            T.v[0]->v = g_CurMdl.SkinVerts[ g_CurMdl.Tris[n].SkinVerts[0] ].t;
            T.v[1]->u = g_CurMdl.SkinVerts[ g_CurMdl.Tris[n].SkinVerts[1] ].s;
            T.v[1]->v = g_CurMdl.SkinVerts[ g_CurMdl.Tris[n].SkinVerts[1] ].t;
            T.v[2]->u = g_CurMdl.SkinVerts[ g_CurMdl.Tris[n].SkinVerts[2] ].s;
            T.v[2]->v = g_CurMdl.SkinVerts[ g_CurMdl.Tris[n].SkinVerts[2] ].t;

            TRealTriangle	*tri = &g_CurMdl.Tris[n];
            Vector3 Norm(tri->Normal.x, tri->Normal.y, tri->Normal.z);

            if (g_CurMdl.Head.num_skins>0)
               Dest.DrawTexTri_ZBuf_UV(T, this->m_UVData);
//               Dest.DrawTexTri_ZBuf_UV(T,UData,VData);


         }
      delete[] TexVerts;
   }

   delete[] poszinv;
   delete[] trisz;
   delete[] todraw;
   delete[] drawtri;
   delete[] tridone;
//   delete[] order;


//	if (MainForm->ShowGrid->Checked)
//		DrawGrid3D(Dest);


   Dest.Rect(0,0,Dest.VisibleWidth-1,Dest.VisibleHeight-1,BLACK);

   {
      Dest.MinX = Dest.VisibleWidth-1;
      Dest.MaxX = 0;
      Dest.MinY = Dest.Height-1;
      Dest.MaxY = 0;

      for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
      {
         if (g_posx[i]<Dest.MinX)
         	Dest.MinX = g_posx[i];
         if (g_posx[i]>Dest.MaxX)
         	Dest.MaxX = g_posx[i];
         if (g_posy[i]<Dest.MinY)
         	Dest.MinY = g_posy[i];
         if (g_posy[i]>Dest.MaxY)
         	Dest.MaxY = g_posy[i];
      }

      if (Dest.MaxX > Dest.VisibleWidth-1)
      	Dest.MaxX = Dest.VisibleWidth-1;
      if (Dest.MaxX < 0)
      	Dest.MaxX = 0;
      if (Dest.MinX > Dest.VisibleWidth-1)
      	Dest.MinX = Dest.VisibleWidth-1;
      if (Dest.MinX < 0)
      	Dest.MinX = 0;

      if (Dest.MaxY > Dest.Height-1)
      	Dest.MaxY = Dest.Height-1;
      if (Dest.MaxY < 0)
      	Dest.MaxY = 0;
      if (Dest.MinY > Dest.Height-1)
      	Dest.MinY = Dest.Height-1;
      if (Dest.MinY < 0)
      	Dest.MinY = 0;
   }

   m_LastExtents = m_CurExtents;

}

int CPaintDlg::SetUVDataSize(int x, int y)
{

   if (m_UVData)
      delete[] m_UVData;

   m_UVData = new BYTE*[x*y];

   return x*y;

}

void CPaintDlg::DoResize()
{
   int   SideTabWidth, BottomDlgHeight;
   RECT  SideTabRect, BottomDlgRect, ClientRect;

   m_ToolTab.GetWindowRect(&SideTabRect);
   m_ToolTab.ScreenToClient(&SideTabRect);
   SideTabWidth = SideTabRect.right;

   m_BottomToolDlg.GetWindowRect(&BottomDlgRect);
   m_BottomToolDlg.ScreenToClient(&BottomDlgRect);
   BottomDlgHeight = BottomDlgRect.bottom;

   GetClientRect(&ClientRect);

   m_ViewWidth = ClientRect.right - SideTabWidth;
   m_ViewHeight = ClientRect.bottom - BottomDlgHeight;

   m_ViewWidth2 = m_ViewWidth >> 1;
   m_ViewHeight2 = m_ViewHeight >> 1;

   m_View.SetSize(m_ViewWidth, m_ViewHeight);
   SetUVDataSize(m_View.Width , m_ViewHeight);

   m_LastExtents.left = 0;
   m_LastExtents.top = 0;
   m_LastExtents.right = m_ViewWidth;
   m_LastExtents.bottom = m_ViewHeight;

   m_CurExtents = m_LastExtents;

   DrawSkinPaint3DView(m_View);


   CDC   *DC;
   DC = GetDC();

   m_View.StretchToDC( DC->m_hDC,
                     SideTabWidth + m_DrawExtents.left, m_DrawExtents.top, m_ViewWidth, m_ViewHeight,
                     m_DrawExtents.left, m_View.Height - m_DrawExtents.bottom,m_ViewWidth,m_ViewHeight);

   ReleaseDC(DC);

}

void CPaintDlg::LoadPaintData()
{
	m_View.SetPalette(Palette);
   g_PaintAction = atNone;

//   DrawEffectCombo->ItemIndex = 0;
//   FadeToCombo->ItemIndex = 0;
//   FadeToCombo->Enabled = false;

//   OpacityEdit->Text = 100;
//   FadeEdit->Text = 0;

   {
      CString PaintFile;

      m_XParent = new BYTE[32768+256];


      {
         int hPaint;
         CString PaintFile;

         {
   	      char drive[_MAX_DRIVE],dir[_MAX_DIR];
            char ExePath[256];
   
            ::GetModuleFileName(NULL, ExePath, 255);
   		   _splitpath(ExePath, drive,dir,NULL,NULL);
            PaintFile = CString(drive)+CString(dir)+g_ProgSet.m_PAIFile;
         }

         hPaint = open((LPCTSTR)PaintFile, O_BINARY | O_RDONLY);
         if (hPaint == -1)
         {
            //notify that not found, is being created
            MessageBox((LPCTSTR)CString(CString("Could not find \"") + g_ProgSet.m_PAIFile + CString("\". file Will be created")),"Error",MB_OK);

            MakePaintingData();

            hPaint = open((LPCTSTR)PaintFile, O_BINARY | O_WRONLY | O_TRUNC | O_CREAT, S_IREAD|S_IWRITE);

            if (hPaint == -1)
            {
               // could not write file;
               MessageBox("Could not write Paintness file","Error", MB_OK);
            }
            else
               write(hPaint, m_XParent, 32768+256);
         }
         else
         {
            read(hPaint, m_XParent, 32768+256);
         }
         close(hPaint);
      }

   }
   
}


void CPaintDlg::MakePaintingData()
{
   {
      double      Best;
      int      BestIndex, Col;
      Vector3  VBest,Compare,Cur,DistV;
      double   Res;


      Best = 0;

      for (Col=32768;Col<32768+256;Col++)
      {
         m_XParent[Col] = Col & 255;
      }

      for (Col=32767;Col>=0;Col--)
      {
         BYTE r,g,b;

         Col16toRGB(Col, r,g,b);
         Cur.x = r;
         Cur.y = g;
         Cur.z = b;

         VBest = Cur;
         BestIndex = 0;
         Best = 10000000000;

         for (int c=0;c<256;c++)
         {
            Compare.x = (float)Palette[c][0];
            Compare.y = (float)Palette[c][1];
            Compare.z = (float)Palette[c][2];

            DistV.x = Cur.x - Compare.x;
            DistV.y = Cur.y - Compare.y;
            DistV.z = Cur.z - Compare.z;

            Res = DistV * DistV;
            if (Res<Best)
            {
               BestIndex = c;
               Best = Res;
            }
         }
         m_XParent[Col] = BestIndex;
      }
   }

   m_IntermediateSkin = NULL;
   m_IntermediateShade = NULL;

}

void CPaintDlg::MouseDown(CShiftState Shift, int X, int Y)
{
   SetCapture();

   if (this->m_PaintTab.m_DrawBut.GetCheck())
   	g_PaintAction = atDraw;

   m_DrawTransparency = m_PaintTab.m_OpacitySlider.GetPos() / 100.0;

   if ((Shift.m_Left || Shift.m_Right || Shift.m_Mid) && (m_BottomToolDlg.m_PanViewsBut.GetCheck() || Shift.m_Shift))
		g_PaintAction = atPan;
   
   g_ox = X;
   g_oy = Y;

   if (m_IntermediateSkin)
   {
      delete[] m_IntermediateSkin;
      m_IntermediateSkin = NULL;
   }

   if (m_IntermediateShade)
   {
      delete[] m_IntermediateShade;
      m_IntermediateShade = NULL;
   }

   if (!m_PaintTab.m_CompoundCheck.GetCheck())
   {
      m_IntermediateSkin = new BYTE[g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight];
      m_IntermediateShade = new BYTE[g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight];

      memcpy(m_IntermediateSkin, g_CurMdl.Skins[g_CurSkin], sizeof(BYTE)*g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight);
      memset(m_IntermediateShade, 0,sizeof(BYTE)*g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight);
   }

   if (m_PaintUndoNode)
      delete m_PaintUndoNode;
   m_PaintUndoNode = new CSkinPaintUndoNode;
   m_PaintUndoNode->Set("Paint", &g_CurMdl, g_CurSkin);

   m_FadeToBack = false;
   m_FadeToXParent = false;

   if (Shift.m_Alt)
      g_PaintAction = atEyeDropper;
   else
   {
      if (Shift.m_Left)
         m_FlipCol = false;

/*      if (Shift.m_Right)
      {
         FlipCol = true;
         BYTE t;
         t = m_PaintTab.m_ForeColor;
         m_PaintTab.m_ForeColor = m_PaintTab.m_BackColor;
         m_PaintTab.m_BackColor = t;
     }*/
   }

   if (m_PaintTab.m_FadeSlider.GetPos()>0)
   {
      m_DrawTransStep = 1.0 / m_PaintTab.m_FadeSlider.GetPos();
      if (m_PaintTab.m_FadeToCombo.GetCurSel() >= 1)  // fade to background color
      {
         m_InterRed = Palette[m_PaintTab.m_ForeColor][0];
         m_InterGreen = Palette[m_PaintTab.m_ForeColor][1];
         m_InterBlue = Palette[m_PaintTab.m_ForeColor][2];
         m_RStep = Palette[m_PaintTab.m_BackColor][0] - m_InterRed;
         m_GStep = Palette[m_PaintTab.m_BackColor][1] - m_InterGreen;
         m_BStep = Palette[m_PaintTab.m_BackColor][2] - m_InterBlue;
         m_RStep /= m_PaintTab.m_FadeSlider.GetPos();
         m_GStep /= m_PaintTab.m_FadeSlider.GetPos();
         m_BStep /= m_PaintTab.m_FadeSlider.GetPos();
         m_FadeToBack = true;
      }
      else
         m_FadeToXParent = true;

      if (m_PaintTab.m_FadeToCombo.GetCurSel() == 2)
         m_FadeToXParent = true;
   }


   MouseMove(Shift, X, Y);
//   FormMouseMove(Sender, Shift,X,Y);

}

void CPaintDlg::MouseUp(CShiftState Shift, int X, int Y)
{
   /*
   if (FlipCol)
   {
      FlipCol = true;
      BYTE t;
      t = m_PaintTab.m_ForeColor;
      m_PaintTab.m_ForeColor = m_PaintTab.m_BackColor;
      m_PaintTab.m_BackColor = t;
   }*/


   if ((Shift.m_Left || Shift.m_Right || Shift.m_Mid) == false)
   {
      ReleaseCapture();
   }


   if (g_PaintAction == atDraw)
   {
      g_PaintUndo.AddNode(m_PaintUndoNode);
      m_PaintUndoNode = NULL;
   }

   if (m_PaintUndoNode)
   {
      delete m_PaintUndoNode;
      m_PaintUndoNode = NULL;
   }

	g_PaintAction = atNone;

}

void CPaintDlg::OnMouseMove(unsigned int nFlags, CPoint point) 
{
   CShiftState    Shift;

   Shift.SetFromFlags(nFlags);

   MouseMove(Shift, point.x, point.y);
	
	CDialog::OnMouseMove(nFlags, point);
}

void CPaintDlg::OnLButtonDown(unsigned int nFlags, CPoint point) 
{
// First determine if the point is inside the for viewports or not
   if (InClientRect(point))
   {
   	CDialog::OnLButtonDown(nFlags, point);
      return;
   }

   m_ShiftState.m_Left = true;
   m_ShiftState.SetFromFlags(nFlags);

   MouseDown(m_ShiftState, point.x, point.y); 
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CPaintDlg::OnLButtonUp(unsigned int nFlags, CPoint point) 
{
   m_ShiftState.m_Left = false;
   m_ShiftState.SetFromFlags(nFlags);

   MouseUp(m_ShiftState, point.x, point.y); 
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CPaintDlg::OnMButtonDown(unsigned int nFlags, CPoint point) 
{
// First determine if the point is inside the for viewports or not
   if (InClientRect(point))
   {
   	CDialog::OnLButtonDown(nFlags, point);
      return;
   }

   m_ShiftState.m_Mid = true;
   m_ShiftState.SetFromFlags(nFlags);

   MouseDown(m_ShiftState, point.x, point.y); 
	
	CDialog::OnMButtonDown(nFlags, point);
}

void CPaintDlg::OnMButtonUp(unsigned int nFlags, CPoint point) 
{
   m_ShiftState.m_Mid = false;
   m_ShiftState.SetFromFlags(nFlags);

   MouseUp(m_ShiftState, point.x, point.y); 
	
	CDialog::OnMButtonUp(nFlags, point);
}

void CPaintDlg::OnRButtonDown(unsigned int nFlags, CPoint point) 
{
// First determine if the point is inside the for viewports or not
   if (InClientRect(point))
   {
   	CDialog::OnLButtonDown(nFlags, point);
      return;
   }

   m_ShiftState.m_Right = true;
   m_ShiftState.SetFromFlags(nFlags);

   MouseDown(m_ShiftState, point.x, point.y); 
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CPaintDlg::OnRButtonUp(unsigned int nFlags, CPoint point) 
{
   m_ShiftState.m_Right = false;
   m_ShiftState.SetFromFlags(nFlags);

   MouseUp(m_ShiftState, point.x, point.y); 
	
	CDialog::OnRButtonUp(nFlags, point);
}

void CPaintDlg::UpdateColors()
{
      m_PaintTab.m_ForeBut.SetColor(RGBToColorRef(Palette[m_PaintTab.m_ForeColor]));
      m_PaintTab.m_BackBut.SetColor(RGBToColorRef(Palette[m_PaintTab.m_BackColor]));


}

void CPaintDlg::DoCursors(CShiftState Shift)
{
   static unsigned int    LastCursor = NULL;
   unsigned int           targ = IDC_CURSORARROW;

	if (m_BottomToolDlg.m_PanViewsBut.GetCheck())
      targ = IDC_CURSORPAN;
   else
   {
      if (m_PaintTab.m_DrawBut.GetCheck())
      {
         if (Shift.m_Alt)
         {
            targ = IDC_CURSORDROPPER;
         }
         else
         targ = IDC_CURSORDRAW;
      }
   }
   
   if (LastCursor != targ)
   {
      SetCursor(AfxGetApp()->LoadCursor(targ));
      LastCursor = targ;
   }
}


void CPaintDlg::SkinDot(int x, int y, unsigned char col, float XParent, bool UseIntermediate, int Style)
{
   int   SideTabWidth, BottomDlgHeight, ClientWidth, ClientHeight;
   RECT  SideTabRect, BottomDlgRect, ClientRect;

   m_ToolTab.GetWindowRect(&SideTabRect);
   m_ToolTab.ScreenToClient(&SideTabRect);
   SideTabWidth = SideTabRect.right;

   m_BottomToolDlg.GetWindowRect(&BottomDlgRect);
   m_BottomToolDlg.ScreenToClient(&BottomDlgRect);
   BottomDlgHeight = BottomDlgRect.bottom;

   GetClientRect(&ClientRect);
   ClientWidth = ClientRect.right;
   ClientHeight = ClientRect.bottom;

   int   UVPos;

   if ((x>=SideTabWidth && x<ClientWidth && y>=0 && y<ClientHeight-BottomDlgHeight) == false)
      return;

   if (x-SideTabWidth<m_CurExtents.left || x-SideTabWidth>m_CurExtents.right || y<m_CurExtents.top || y>m_CurExtents.bottom)
      return;

   if (m_View.ZBuffer[y*m_View.Width + x-SideTabWidth]<=0)
      return;

//   int u, v;
//      u = UData[y*m_View.Width + x-SideTabWidth];
//      v = VData[y*m_View.Width + x-SideTabWidth];

   unsigned char *UV;
   UV = m_UVData[y*m_View.Width + x-SideTabWidth];

   BYTE OldCol8, or, og, ob,nr,ng,nb, NewCol;
   WORD NewCol16;

//   UVPos = v*g_CurMdl.Head.skinwidth + u;
   UVPos = UV - g_CurMdl.Skins[g_CurSkin];

//   if (UseIntermediate)
//     OldCol8 = IntermediateSkin[UVPos];
//   else
//      OldCol8 = g_CurMdl.Skins[SkinEditForm->CurSkin][UVPos];
   OldCol8 = *UV;

   or = Palette[OldCol8][0];
   og = Palette[OldCol8][1];
   ob = Palette[OldCol8][2];
   nr = Palette[col][0];
   ng = Palette[col][1];
   nb = Palette[col][2];

   if (UseIntermediate)
   {
      float ShadeLeft;
      unsigned char NewShade;

      ShadeLeft = (float)((255-m_IntermediateShade[UVPos])/255.0);
      if (XParent > ShadeLeft)
         XParent = ShadeLeft;

      NewShade = (BYTE)(255 - (float)(ShadeLeft - XParent)*255.0);
      NewShade = 255;
      m_IntermediateShade[UVPos] = NewShade;
   }

   if (Style == peNormal)
   {
      NewCol16 = RGBto16(or,og,ob);

      or += (BYTE)(XParent*(nr-or));
      og += (BYTE)(XParent*(ng-og));
      ob += (BYTE)(XParent*(nb-ob));
      NewCol16 = RGBto16(or,og,ob);

      NewCol = m_XParent[NewCol16];
      *UV = NewCol;
//      g_CurMdl.Skins[SkinEditForm->CurSkin][v*g_CurMdl.Head.skinwidth + u] = NewCol;
   }
   if (Style == peColor)
   {
      double OldValue,NewValue;

      OldValue = (or+og+ob);
      or += (BYTE)(XParent*(nr-or));
      og += (BYTE)(XParent*(ng-og));
      ob += (BYTE)(XParent*(nb-ob));

      if (or+og+ob == 0)
         NewValue = 0;
      else
         NewValue = (double)OldValue/(or+og+ob);

      if (NewValue*or > 255)
         NewValue = (double)255.0/or;
      if (NewValue*og > 255)
         NewValue = (double)255.0/og;
      if (NewValue*ob > 255)
         NewValue = (double)255.0/ob;

      or *= (BYTE)(NewValue);
      og *= (BYTE)(NewValue);
      ob *= (BYTE)(NewValue);


      NewCol16 = RGBto16(or,og,ob);

      NewCol = m_XParent[NewCol16];
      *UV = NewCol;
//      g_CurMdl.Skins[SkinEditForm->CurSkin][v*g_CurMdl.Head.skinwidth + u] = NewCol;
   }
   if (Style == peBrightness)
   {
      double OldValue,NewValue;

      OldValue = (or+og+ob);
      NewValue = (nr+ng+nb);

      NewValue -= XParent*(NewValue-OldValue);
      if (OldValue == 0)
      {
         or = og = ob  =1;
         OldValue = 3;
      }
      NewValue /= OldValue;

      if (NewValue*or > 255)
         NewValue = (double)255.0/or;
      if (NewValue*og > 255)
         NewValue = (double)255.0/og;
      if (NewValue*ob > 255)
         NewValue = (double)255.0/ob;

      or *= (BYTE)(NewValue);
      og *= (BYTE)(NewValue);
      ob *= (BYTE)(NewValue);


      NewCol16 = RGBto16(or,og,ob);
      NewCol = m_XParent[NewCol16];

      *UV = NewCol;
//      g_CurMdl.Skins[SkinEditForm->CurSkin][v*g_CurMdl.Head.skinwidth + u] = NewCol;
   }
}

void CPaintDlg::PaintJustUVs()
{
   int   SideTabWidth, BottomDlgHeight;
   RECT  SideTabRect, BottomDlgRect, ClientRect;

   m_ToolTab.GetWindowRect(&SideTabRect);
   m_ToolTab.ScreenToClient(&SideTabRect);
   SideTabWidth = SideTabRect.right;

   m_BottomToolDlg.GetWindowRect(&BottomDlgRect);
   m_BottomToolDlg.ScreenToClient(&BottomDlgRect);
   BottomDlgHeight = BottomDlgRect.bottom;

   GetClientRect(&ClientRect);

   {
      BYTE  *d;
      float *f;

      d = m_View.Data + m_View.Height*m_View.Width - m_View.Width;;
      f = m_View.ZBuffer;

      d-= m_View.Width*m_LastExtents.top;
      f+= m_View.Width*m_LastExtents.top;

      for (int i=m_LastExtents.top;i<m_LastExtents.bottom;i++)
      {
         for (int j=m_LastExtents.left;j<m_LastExtents.right;j++)
         {
            if (f[j]>0)
            {
//               u = UData[m_View.Width*i + j];
//               v = VData[m_View.Width*i + j];

//               d[j] = CurMdl.Skins[SkinEditForm->CurSkin][ CurMdl.Head.skinwidth*v + u];
               d[j] = *m_UVData[m_View.Width*i + j];
            }
         }

         d-=m_View.Width;
         f+=m_View.Width;
      }
   }

   int w,h;

   w = m_DrawExtents.right - m_DrawExtents.left;
   h = m_DrawExtents.bottom - m_DrawExtents.top;

//   m_View.StretchToDC(SkinPainterForm->Canvas->Handle,SideTabWidth,0,w,h,0,0,w,h);
   CDC   *DC;
   DC = GetDC();

   m_View.StretchToDC( DC->m_hDC,
                     SideTabWidth + m_DrawExtents.left, m_DrawExtents.top, w, h,
                     m_DrawExtents.left, m_View.Height - m_DrawExtents.bottom,w,h);

   ReleaseDC(DC);
}

bool CPaintDlg::InClientRect(CPoint point)
{
   int   SideTabWidth, BottomDlgHeight;
   RECT  SideTabRect, BottomDlgRect, ClientRect;

   m_ToolTab.GetWindowRect(&SideTabRect);
   m_ToolTab.ScreenToClient(&SideTabRect);
   SideTabWidth = SideTabRect.right;

   m_BottomToolDlg.GetWindowRect(&BottomDlgRect);
   m_BottomToolDlg.ScreenToClient(&BottomDlgRect);
   BottomDlgHeight = BottomDlgRect.bottom;

   GetClientRect(&ClientRect);

   if (point.x < SideTabWidth || point.y>ClientRect.bottom-BottomDlgHeight)
      return true;

   return false;

}

BOOL CPaintBottomDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	unsigned int  CmdID, Msg;
   HWND  hWnd;
   CWnd  *Parent;

   Parent = GetParentOwner();
   CmdID = LOWORD(wParam);
   Msg = HIWORD(wParam);
   hWnd = (HWND)lParam;

   if (Msg == BN_CLICKED)
   {
      switch(CmdID)
      {
      case IDC_PANVIEWSBUT:
         Parent->PostMessage(MM_TOOLBUTCLICK, 0, (long)hWnd);
      }
   }
	
	return CDialog::OnCommand(wParam, lParam);
}

void CPaintDlg::ToolButClick(HWND But)
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

void CPaintDlg::OnEditUndo() 
{
   KPaintUndo();
	
}

void CPaintDlg::OnEditRedo() 
{
   KPaintRedo();
	
}

void CPaintDlg::MouseMove(CShiftState Shift, int X, int Y)
{
   int   ClientWidth, ClientHeight;
   RECT  ClientRect;

   GetClientRect(&ClientRect);
   ClientWidth = ClientRect.right;
   ClientHeight = ClientRect.bottom;

   int   SideTabWidth, BottomDlgHeight;
   RECT  SideTabRect, BottomDlgRect;

   m_ToolTab.GetWindowRect(&SideTabRect);
   m_ToolTab.ScreenToClient(&SideTabRect);
   SideTabWidth = SideTabRect.right;

   m_BottomToolDlg.GetWindowRect(&BottomDlgRect);
   m_BottomToolDlg.ScreenToClient(&BottomDlgRect);
   BottomDlgHeight = BottomDlgRect.bottom;

	DoCursors(Shift);

   if (g_MainAction != atNone)
   {
      WrapMouseToScreen(m_hWnd, X,Y,g_ox, g_oy);
   }

	bool Update = false;
   static short oldu=0, oldv=0;

   if (g_PaintAction == atPan)
   {
      if (Shift.m_Left && Shift.m_Right)
      {
         double OldPersp = g_Persp;
         g_Persp += (float)(g_oy-Y)/10;

         Dist *= (float)(g_Persp/OldPersp);
      }
      else
      if (Shift.m_Left)
      {
         RotX += (float)(g_ox-X)/100;
         RotY -= (float)(g_oy-Y)/100;

      }
      else
      if (Shift.m_Right)
      {
         Dist -= (float)(g_oy-Y)/2;
      }
      else
      if (Shift.m_Mid)
      {
         Vector3 T,N;
      
      
         T.x = (float)(g_ox - X);
         T.y = (float)(g_oy - Y);
         T.z = 0;
      
         GetViewAxis(vtCamera).Transform(T,N);
      
         PanX += Dist/g_Persp*N.x;
         PanY += Dist/g_Persp*N.y;
         PanZ += Dist/g_Persp*N.z;
      }

      Update = true;
   }

   bool IsDrawable = (X >= SideTabWidth)  && (X < ClientWidth) && (Y >= 0) && (Y < ClientHeight-BottomDlgHeight);

   if (Shift.m_Alt && IsDrawable && g_PaintAction == atEyeDropper)
   {
      BYTE  *UV;
      UV = m_UVData[Y*m_View.Width + X-SideTabWidth];

      if (m_View.ZBuffer[Y*m_View.Width + X-SideTabWidth]>0)
      {

         if (Shift.m_Left)
            m_PaintTab.m_ForeColor = *UV;
         if (Shift.m_Right)
            m_PaintTab.m_BackColor = *UV;

         UpdateColors();
      }

      return;
   }


   if ((g_PaintAction == atDraw) && IsDrawable)
		if (g_ox>=SideTabWidth && g_ox<ClientWidth && g_oy>=0 && g_oy<m_ViewHeight)
      {
         {
            unsigned char ActualCol = m_PaintTab.m_ForeColor;
            {
               if (m_FadeToBack)
               {
                  WORD rgb16;
                  rgb16 = RGBto16((BYTE)m_InterRed, (BYTE)m_InterGreen, (BYTE)m_InterBlue);
                  ActualCol = m_XParent[rgb16];
                  m_InterRed += m_RStep;
                  m_InterGreen += m_GStep;
                  m_InterBlue += m_BStep;
                  if (m_RStep>0)
                     if (m_InterRed>Palette[m_PaintTab.m_BackColor][0])
                        m_InterRed = Palette[m_PaintTab.m_BackColor][0];
                  if (m_RStep<0)
                     if (m_InterRed<Palette[m_PaintTab.m_BackColor][0])
                        m_InterRed = Palette[m_PaintTab.m_BackColor][0];
                  if (m_GStep>0)
                     if (m_InterGreen>Palette[m_PaintTab.m_BackColor][1])
                        m_InterGreen = Palette[m_PaintTab.m_BackColor][1];
                  if (m_GStep<0)
                     if (m_InterGreen<Palette[m_PaintTab.m_BackColor][1])
                        m_InterGreen = Palette[m_PaintTab.m_BackColor][1];
                  if (m_BStep>0)
                     if (m_InterBlue>Palette[m_PaintTab.m_BackColor][2])
                        m_InterBlue = Palette[m_PaintTab.m_BackColor][2];
                  if (m_BStep<0)
                     if (m_InterBlue<Palette[m_PaintTab.m_BackColor][2])
                        m_InterBlue = Palette[m_PaintTab.m_BackColor][2];
               }
            }

            float S = (float)(0.5*(m_PaintTab.m_DrawBrushSize + 1));

//            if (DrawEffectComb

            static float dist = 0;

/*            dist += sqrt(((X-g_ox)*(X-g_ox) + (Y-g_oy)*(Y-g_oy)));
            if ( dist > S)
            {
               float aX, aY, dX, dY;

               aX = g_ox;
               aY = g_oy;
               dX = (X-g_ox)/(dist*S/5);
               dY = (Y-g_oy)/(dist*S/5);

               for (int l = 0;l<dist*S/5;l++)
               {
                  aX += dX;
                  aY += dY;
*/
               for (int i = (int)(-S+1);i<S;i++)
                  for (int j = (int)(-S+1);j<S;j++)
                     if (i*i+j*j <= (S*S))
//                        SkinDot(i+aX, j+aY, ActualCol,DrawTransparency, !CompoundCheck->Checked, DrawEffectCombo->ItemIndex);
                        SkinDot(i+X, j+Y, ActualCol,(float)m_DrawTransparency, !m_PaintTab.m_CompoundCheck.GetCheck(), m_PaintTab.m_DrawEffectCombo.GetCurSel());

               if (m_FadeToXParent)
               {
                  m_DrawTransparency -= (double)m_DrawTransStep;
                  if (m_DrawTransparency<0)
                     m_DrawTransparency = 0;
               }
//               }
//               dist = 0;
//            }

   	      Update = true;
         }
      }

   if (IsDrawable)
   {
      if (m_View.ZBuffer[Y*m_View.Width + X-SideTabWidth]>0)
      {
      }
      else
      {
         m_BottomToolDlg.m_UEdit.SetWindowText("-");
         m_BottomToolDlg.m_VEdit.SetWindowText("-");
      }
	}
   else
   {
      m_BottomToolDlg.m_UEdit.SetWindowText("-");
      m_BottomToolDlg.m_VEdit.SetWindowText("-");
   }

   if (Update)
   {
      if (g_PaintAction == atDraw)
      {
         PaintJustUVs();
      }
      else
   		SendMessage(WM_PAINT);

/*      if (UpdateMainView->Checked)
      {
      	Draw3DView(Views[3], &g_CurMdl, &MainForm->DrawInfo);
         Views[3].DrawToDC(MainForm->Canvas->Handle, PANELWIDTH+MainForm->VWidthL,0);
      }
      SendMessage(SkinEditForm->Handle, MM_PAINT,0,0);
//      SkinEditForm->FormPaint(this);*/
   }

   g_ox = X;
   g_oy = Y;
}

void CPaintDlg::OnView3deditor() 
{
   KView3DEditor();	
	
}

void CPaintDlg::OnViewSkinpainter() 
{
	// TODO: Add your command handler code here
	
}

void CPaintDlg::OnViewSkineditor() 
{
   KViewSkins();
	
}
