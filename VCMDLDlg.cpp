// VCMDLDlg.cpp : implementation file
//

#include "stdafx.h"

#include "MdlGlobs.h"
#include "MainFunc.h"
#include "Undo3D.h"

#include "RegCtrl.h"

#include "KeyFuncs.h"

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>

#include "VCMDL.h"
#include "VCMDLDlg.h"

#include "MdlMsgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_GETMINMAXINFO()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CVCMDLApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


//BOOL CAboutDlg::OnInitDialog() 
//{
//	CDialog::OnInitDialog();
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//	              // EXCEPTION: OCX Property Pages should return FALSE
//}


/////////////////////////////////////////////////////////////////////////////
// CVCMDLDlg dialog

CVCMDLDlg::CVCMDLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVCMDLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVCMDLDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVCMDLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVCMDLDlg)
	DDX_Control(pDX, IDC_TOOLTAB, m_ToolTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVCMDLDlg, CDialog)
	//{{AFX_MSG_MAP(CVCMDLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_TIMER()
	ON_COMMAND(ID_OPTIONS_3DVIEW_WIREFRAME, OnOptions3DViewWireframe)
	ON_COMMAND(ID_OPTIONS_3DVIEW_TEXTUREMAPPED, OnOptions3dviewTexturemapped)
	ON_COMMAND(ID_OPTIONS_3DVIEW_TEXTUREGOURAD, OnOptions3dviewTexturegourad)
	ON_COMMAND(ID_OPTIONS_3DVIEW_GOURADSHADED, OnOptions3dviewGouradshaded)
	ON_COMMAND(ID_OPTIONS_3DVIEW_FLATSHADED, OnOptions3dviewFlatshaded)
	ON_COMMAND(ID_OPTIONS_SHOWGRID, OnOptionsShowgrid)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_COMMAND(ID_EDIT_SELECTCONNECTED, OnEditSelectconnected)
	ON_COMMAND(ID_EDIT_SELECTINVERSE, OnEditSelectinverse)
	ON_COMMAND(ID_EDIT_SELECTNONE, OnEditSelectnone)
	ON_COMMAND(ID_EDIT_SELECTTOUCHING, OnEditSelecttouching)
	ON_WM_CLOSE()
	ON_COMMAND(ID_OPTIONS_2DVIEW_DRAWBACKFACES, OnOptions2dviewDrawbackfaces)
	ON_COMMAND(ID_OPTIONS_3DVIEW_DRAWBACKFACES, OnOptions3dviewDrawbackfaces)
	ON_COMMAND(ID_OPTIONS_VERTICESTICKS, OnOptionsVerticesticks)
	ON_COMMAND(ID_OPTIONS_SHOWORIGIN, OnOptionsShoworigin)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_IMPORT_IMPORTAPPENDVERTS, OnFileImportImportAppendVerts)
	ON_COMMAND(ID_FILE_IMPORT_IMPORTAPPENDFRAMES, OnFileImportImportAppendFrames)
	ON_WM_KEYUP()
	ON_COMMAND(ID_OPTIONS_2DVIEW_PLAYCAMERAXZ, OnOptions2dviewPlaycameraxz)
	ON_COMMAND(ID_OPTIONS_2DVIEW_PLAYCAMERAZY, OnOptions2dviewPlaycamerazy)
	ON_COMMAND(ID_OPTIONS_2DVIEW_PLAYCAMERAXY, OnOptions2dviewPlaycameraxy)
	ON_COMMAND(ID_OPTIONS_SHOWNORMALS_NONE, OnOptionsShownormalsNone)
	ON_COMMAND(ID_OPTIONS_SHOWNORMALS_ALL, OnOptionsShownormalsAll)
	ON_COMMAND(ID_OPTIONS_SHOWNORMALS_SELECTEDFACES, OnOptionsShownormalsSelectedfaces)
	ON_COMMAND(ID_FILE_EXPORT_EXPORTTOMD2, OnFileExportExporttomd2)
	ON_COMMAND(ID_FILE_EXPORT_EXPORTTOMDL, OnFileExportExporttomdl)
	ON_COMMAND(ID_FILE_EXPORT_EXPORTFRAMETO3DS, OnFileExportExportframeto3ds)
	ON_COMMAND(ID_FILE_EXPORT_EXPORTFRAMETOASC, OnFileExportExportframetoasc)
	ON_COMMAND(ID_FILE_EXPORT_EXPORTMODELTO3DS, OnFileExportExportmodelto3ds)
	ON_COMMAND(ID_FILE_EXPORT_EXPORTMODELTOASC, OnFileExportExportmodeltoasc)
	ON_COMMAND(ID_FILE_IMPORT_FRAMENAMESFROMTEXT, OnFileImportFramenamesfromtext)
	ON_COMMAND(ID_OPTIONS_DECREASEGRIDSIZE, OnOptionsDecreasegridsize)
	ON_COMMAND(ID_OPTIONS_INCREASEGRIDSIZE, OnOptionsIncreasegridsize)
	ON_COMMAND(ID_OPTIONS_ZOOMIN, OnOptionsZoomin)
	ON_COMMAND(ID_OPTIONS_ZOOMOUT, OnOptionsZoomout)
	ON_COMMAND(ID_OPTIONS_CONFIGURE, OnOptionsConfigure)
	ON_COMMAND(ID_VIEW_SKINS, OnViewSkins)
	ON_COMMAND(ID_EDIT_COPYSELECTED, OnEditCopyselected)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_PASTETORANGE, OnEditPastetorange)
	ON_COMMAND(ID_EDIT_CHANGEFRAMENAME, OnEditChangeframename)
	ON_COMMAND(ID_EDIT_GOTOFRAME, OnEditGotoframe)
	ON_COMMAND(ID_EDIT_ADDNEWFRAME, OnEditAddnewframe)
	ON_COMMAND(ID_EDIT_MOVEFRAMES, OnEditMoveframes)
	ON_COMMAND(ID_EDIT_DELETECURRENTFRAME, OnEditDeletecurrentframe)
	ON_COMMAND(ID_EDIT_DELETEFRAMES, OnEditDeleteframes)
	ON_COMMAND(ID_OPTIONS_SYNCSKINSELECTION, OnOptionsSyncskinselection)
	ON_COMMAND(ID_OPTIONS_GROUNDPLANEPOSITION, OnOptionsGroundplaneposition)
	ON_COMMAND(ID_TOOLS_MEMOWINDOW, OnToolsMemowindow)
	ON_COMMAND(ID_TOOLS_OUTPUTBOUNDINGBOX, OnToolsOutputboundingbox)
	ON_COMMAND(ID_VIEW_SKINPAINTER, OnViewSkinpainter)
	ON_COMMAND(ID_FILE_CHOOSEREFERENCEMODEL, OnFileChoosereferencemodel)
	ON_COMMAND(ID_FILE_CLEARREFERENCEMODEL, OnFileClearreferencemodel)
	ON_COMMAND(ID_OPTIONS_3DVIEW_SHOWREFERENCEMODEL, OnOptions3dviewShowreferencemodel)
	ON_COMMAND(ID_TOOLS_NUMERICTYPEIN, OnToolsNumerictypein)
	ON_COMMAND(ID_VIEW_MODELPROPERTIES, OnViewModelproperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCMDLDlg message handlers

LRESULT CVCMDLDlg::WindowProc(unsigned int message, WPARAM wParam, LPARAM lParam) 
{
   if (message == MM_TOOLBUTCLICK)
   {
      HWND  hBut;

      hBut = (HWND)lParam;

      ToolButClick(hBut);

      return 1;
   }
   if (message == MM_UPDATEVIEWPORTS)
   {
      m_BottomToolDlg.m_TimeScroll.SetScrollRange(0, g_CurMdl.Head.num_frames-1);
      if (g_CurFrame<=m_BottomToolDlg.m_TimeScroll.GetScrollLimit())
	      m_BottomToolDlg.m_TimeScroll.SetScrollPos(g_CurFrame);

      InitDrawInfo();

      if (wParam & MMC_UPDATE_XY)
	      ToDrawView[0] = true;
      if (wParam & MMC_UPDATE_XZ)
	      ToDrawView[1] = true;
      if (wParam & MMC_UPDATE_ZY)
	      ToDrawView[2] = true;
      if (wParam & MMC_UPDATE_CAM)
	      ToDrawView[3] = true;

	   if (ToDrawView[0])
		   DrawViewMatrix(Views[0], &g_CurMdl, &m_DrawInfo, vtXY);
	   if (ToDrawView[1])
		   DrawViewMatrix(Views[1], &g_CurMdl, &m_DrawInfo, vtZY);
	   if (ToDrawView[2])
		   DrawViewMatrix(Views[2], &g_CurMdl, &m_DrawInfo, vtXZ);
	   if (ToDrawView[3])
   	   Draw3DView(Views[3], &g_CurMdl, &m_DrawInfo);



      DrawAllViews();
      
   }
   if (message == MM_FILENAMECHANGED)
   {
      SetWindowText(CString("MED - ") + g_CurFileName);

   }
   if (message == MM_UPDATEFRAMELABEL)
   {
      m_SideToolDlg.m_FrameNameEdit.SetWindowText(g_CurMdl.FrameInfo[g_CurFrame].Name);
      CString  Str;
      Str.Format("%i", g_CurFrame);
      m_SideToolDlg.m_FrameNumEdit.SetWindowText(Str);

      if (wParam != MMC_NOSCROLL)
         m_BottomToolDlg.m_TimeScroll.SetScrollPos(g_CurFrame);

      m_BottomToolDlg.UpdateSelectionCounters();
   }
   if (message == MM_CHECKSPINNERS)
   {
      CheckSpinners();
   }
   if (message == MM_STOPPLAY)
   {
      if (g_Playing)
         m_ViewTab.m_PlayBut.SendMessage(BM_CLICK);
   }
   if (message == MM_UPDATEFEEDBACK)
   {
      if (wParam & MMC_XLABEL || wParam & MMC_YLABEL || wParam & MMC_ZLABEL)
      {
         m_BottomToolDlg.SetXYZLabels(g_curx, g_cury, g_curz);
      }
      if (wParam & MMC_GRIDLABEL)
      {
         m_BottomToolDlg.UpdateGridLabel();
      }
      if (wParam & MMC_SELECTCOUNTERS)
      {
         m_BottomToolDlg.UpdateSelectionCounters();
      }
   }
   if (message == MM_RESETSPINNERS)
   {
      m_ViewTab.ResetSpinners();
      m_ModifyTab.ResetSpinners();
   }
   if (message == MM_DOSYNC)
   {
      DoSync();
   }
   

	return CDialog::WindowProc(message, wParam, lParam);
}


BOOL CVCMDLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

   m_Ready = false;

// Create Side tool panel tabs
	m_ToolTab.AddTab("Create", &m_CreateTab, IDD_TABDLG_CREATE);
	m_ToolTab.AddTab("Modify", &m_ModifyTab, IDD_TABDLG_MODIFY);
	m_ToolTab.AddTab("Bones", &m_BonesTab, IDD_TABDLG_BONES);
	m_ToolTab.AddTab("View", &m_ViewTab, IDD_TABDLG_VIEW);
   m_SideToolDlg.Create(IDD_TOOLDLG_SIDE, this);
   m_SideToolDlg.ShowWindow(SW_SHOW);
   m_BottomToolDlg.Create(IDD_TOOLDLG_BOTTOM, this);
   m_BottomToolDlg.ShowWindow(SW_SHOW);
   
   SetFormVars(this, &m_SkinEditDlg, &m_PaintDlg, &m_TypeInDlg);
   m_SkinEditDlg.m_MainDlg = this;
   m_PaintDlg.m_MainDlg = this;
   m_ConsoleDlg.m_MainDlg = this;
   m_TypeInDlg.m_MainDlg = this;

   g_Playing = 0;
   m_PlayTimerID = SetTimer(2000, 10, NULL);

   // Set the styles forthe buttons so they are two-state buttons
   // only set for the tool buts.

   // bottom dialog 
   AddWndStyle(m_BottomToolDlg.m_VertexModeBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_BottomToolDlg.m_FaceModeBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_BottomToolDlg.m_BoneModeBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_BottomToolDlg.m_ObjectModeBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_BottomToolDlg.m_PanViewsBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);

   // Modify Tab dialog
   AddWndStyle(m_ModifyTab.m_SelectBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_MoveBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_RotateBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_ScaleBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_XBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_YBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ModifyTab.m_ZBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);

   // View Tab dialog
   AddWndStyle(m_ViewTab.m_PlayBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_ViewTab.m_InterpolateBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);

   // Create Dialog
   AddWndStyle(m_CreateTab.m_CreateVertexBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_CreateTab.m_BuildFaceBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_CreateTab.m_CreateFanBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);
   AddWndStyle(m_CreateTab.m_CreateStripBut.m_hWnd, BS_PUSHLIKE | BS_CHECKBOX);

   m_CurTool = m_ModifyTab.m_SelectBut.m_hWnd;
   m_LastTool = m_BottomToolDlg.m_PanViewsBut.m_hWnd;

   ::SendMessage(m_CurTool, BM_SETCHECK, 1,0);
   ::SendMessage(m_LastTool, BM_SETCHECK, 0,0);

   // Set the XYZ restriction buttons to all on.
   ::SendMessage(m_ModifyTab.m_XBut, BM_SETCHECK, 1,0);
   ::SendMessage(m_ModifyTab.m_YBut, BM_SETCHECK, 1,0);
   ::SendMessage(m_ModifyTab.m_ZBut, BM_SETCHECK, 1,0);

   ::SendMessage(m_ViewTab.m_InterpolateBut, BM_SETCHECK, 1,0);

// Set Default Menu Items
   GetMenu()->CheckMenuRadioItem(ID_OPTIONS_3DVIEW_WIREFRAME, 
                                 ID_OPTIONS_3DVIEW_TEXTUREGOURAD, 
                                 ID_OPTIONS_3DVIEW_WIREFRAME, 
                                 MF_BYCOMMAND);
   GetMenu()->CheckMenuItem(ID_OPTIONS_SHOWGRID, MF_CHECKED);
   GetMenu()->CheckMenuRadioItem(ID_OPTIONS_SHOWNORMALS_NONE, 
                                 ID_OPTIONS_SHOWNORMALS_ALL, 
                                 ID_OPTIONS_SHOWNORMALS_NONE, 
                                 MF_BYCOMMAND);

   GetMenu()->EnableMenuItem(ID_EDIT_PASTE, MF_GRAYED);
   GetMenu()->EnableMenuItem(ID_EDIT_PASTETORANGE, MF_GRAYED);



// Create the Skin Editor Dialog
   if (m_SkinEditDlg.m_hWnd == NULL)
      m_SkinEditDlg.Create(IDD_SKIN_MAIN_DIALOG, GetDesktopWindow());

// Create the Console Editor Dialog
   if (m_ConsoleDlg.m_hWnd == NULL)
      m_ConsoleDlg.Create(IDD_CONSOLE_DIALOG, GetDesktopWindow());

// Create the Console Editor Dialog
   if (m_PaintDlg.m_hWnd == NULL)
      m_PaintDlg.Create(IDD_PAINT_MAIN_DIALOG, GetDesktopWindow());

// Create the Console Editor Dialog
   if (m_TypeInDlg.m_hWnd == NULL)
      m_TypeInDlg.Create(IDD_TYPEIN_POPUP, GetDesktopWindow());

// From original Q2M
   g_ChangesMade = 0;
   g_CurFileName = "Untitled";
   SendMessage(MM_FILENAMECHANGED,0,0);

   m_MidpointX = 0.5;
   m_MidpointY = 0.5;

	g_CurMdl.FrameInfo = new TFrameInfo[1];
   g_CurMdl.Head.num_frames = 1;
   g_CurMdl.FrameInfo[0].Name[0] = 'F';
   g_CurMdl.FrameInfo[0].Name[1] = 'r';
   g_CurMdl.FrameInfo[0].Name[2] = 'a';
   g_CurMdl.FrameInfo[0].Name[3] = 'm';
   g_CurMdl.FrameInfo[0].Name[4] = 'e';
   g_CurMdl.FrameInfo[0].Name[5] = ' ';
   g_CurMdl.FrameInfo[0].Name[6] = '1';
   g_CurMdl.Head.num_skins = 1;
   g_CurMdl.Head.skinwidth = 256;
   g_CurMdl.Head.skinheight = 256;
   g_CurMdl.Skins[0] = new unsigned char[g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight];
   memset(g_CurMdl.Skins[0], 0, g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight);
   g_CurMdl.Empty = false;
   g_CurMdl.Head.id = 0x4F504449;
   g_CurMdl.Head.version = 6;
   g_CurMdl.Head.num_xyz = 0;
   g_CurMdl.Head.num_st = 1;
   g_CurMdl.SkinVerts = new TRealSkinVertex[1];
   g_CurMdl.SkinVerts[0].s = 0;
   g_CurMdl.SkinVerts[0].t = 0;
   g_CurMdl.SkinVerts[0].Selected = 0;
   g_CurMdl.Tris = NULL;
   Views[3].SetTexSize(g_CurMdl.Head.skinwidth, g_CurMdl.Head.skinheight);

   m_SideToolDlg.m_FrameNameEdit.SetWindowText(g_CurMdl.FrameInfo[0].Name);

   m_BottomToolDlg.UpdateGridLabel();
   g_CurFrame = 0;

   m_SideToolDlg.m_FitAllBut.SendMessage(BM_CLICK);   


//	DragAcceptFiles(Handle, true);
   m_Closing = false;

   g_MainUndo.RedoList = &g_MainRedo;
   g_MainUndo.DeleteRedoOnAdd = true;
   g_MainUndo.MenuPrefix = "Undo ";
   g_MainUndo.SetAssociatedMenu(GetMenu()->m_hMenu, ID_EDIT_UNDO);
   g_MainUndo.UpdateMenuItem();
   g_MainRedo.RedoList = &g_MainUndo;
   g_MainRedo.MenuPrefix = "Redo ";
   g_MainRedo.SetAssociatedMenu(GetMenu()->m_hMenu, ID_EDIT_REDO);
   g_MainRedo.UpdateMenuItem();

   m_BottomToolDlg.SetSelectionType(stVertex);
//   SelectVertexBut->Down = true;
//   SelectVertexBut->Click();

   PanX = PanY = PanZ = 0;
   g_Zoom = 1.0;




	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVCMDLDlg::OnSysCommand(unsigned int nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVCMDLDlg::OnPaint() 
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
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

	   if (!m_Ready)
   	   return;

      m_BottomToolDlg.m_TimeScroll.SetScrollRange(0, g_CurMdl.Head.num_frames-1);
      if (g_CurFrame<=m_BottomToolDlg.m_TimeScroll.GetScrollLimit())
	      m_BottomToolDlg.m_TimeScroll.SetScrollPos(g_CurFrame);

      m_BottomToolDlg.m_TimeScroll.SetScrollRange(0,g_CurMdl.Head.num_frames-1);
      InitDrawInfo();

      if (!m_ViewTab.m_PlayBut.GetCheck())
      {
	      if (ToDrawView[0])
		      DrawViewMatrix(Views[0], &g_CurMdl, &m_DrawInfo, vtXY);
	      if (ToDrawView[1])
		      DrawViewMatrix(Views[1], &g_CurMdl, &m_DrawInfo, vtZY);
	      if (ToDrawView[2])
		      DrawViewMatrix(Views[2], &g_CurMdl, &m_DrawInfo, vtXZ);
      }
   	if (ToDrawView[3])
      	Draw3DView(Views[3], &g_CurMdl, &m_DrawInfo);

      DrawAllViews();

      ToDrawView[0]=true;
      ToDrawView[1]=true;
      ToDrawView[2]=true;
      ToDrawView[3]=true;

	   g_Resizing = false;

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVCMDLDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CVCMDLDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanged(lpwndpos);
	
	RECT	ClientRect, TabCtrlRect, SideDlgRect, BotDlgRect;
   int   SideDlgHeight, BotDlgHeight, SideDlgWidth;

	
	GetClientRect(&ClientRect);
	m_ToolTab.GetWindowRect(&TabCtrlRect);	

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

// This is if you want the bottom bialog to extend all the way across the client window area

	m_ToolTab.ResizeTabCtrl(SideDlgWidth, ClientRect.bottom-SideDlgHeight-BotDlgHeight);	
	
   // Move the Other dialogs about the place.
   if (m_SideToolDlg.m_hWnd)
      m_SideToolDlg.MoveWindow(0,ClientRect.bottom-BotDlgHeight-SideDlgHeight,SideDlgRect.right-SideDlgRect.left, SideDlgHeight);

   if (m_BottomToolDlg.m_hWnd)
      m_BottomToolDlg.MoveWindow(0,ClientRect.bottom-BotDlgHeight,ClientRect.right, BotDlgHeight);

	// Resize all controls on the window here

// This is if you want the bottom dialog and the side too dialog side by side
/*
   ToolTab.ResizeTabCtrl(SideDlgWidth, ClientRect.bottom-SideDlgHeight);	
	
   // Move the Other dialogs about the place.
   if (m_SideToolDlg.m_hWnd)
      m_SideToolDlg.MoveWindow(0,ClientRect.bottom-SideDlgHeight,SideDlgRect.right-SideDlgRect.left, SideDlgHeight);

   if (m_BottomToolDlg.m_hWnd)
      m_BottomToolDlg.MoveWindow(SideDlgWidth,ClientRect.bottom-BotDlgHeight,ClientRect.right-SideDlgWidth, BotDlgHeight);
*/

  	DoResize();
}
   
BOOL CVCMDLDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
   
	return CDialog::OnCommand(wParam, lParam);
}

void CVCMDLDlg::WasOnShowWindow()
{
   // Load program settings from registry
   LoadConfigFromReg();

	int handle;

// !!! old: from quake 1
//   handle = open("Palette.lmp",O_BINARY | O_RDONLY);
	CString PalFile;

	{
   	char drive[_MAX_DRIVE],dir[_MAX_DIR];
      char ExePath[256];

      ::GetModuleFileName(NULL, ExePath, 255);
		_splitpath(ExePath, drive,dir,NULL,NULL);
      PalFile = CString(drive)+CString(dir)+g_ProgSet.m_PALFile;
   }

   handle = open((LPCTSTR)PalFile,O_BINARY | O_RDONLY);
   if (handle == -1)
   {
   	MessageBox((LPCTSTR)CString(CString("Could not load \"") + PalFile + CString("\" - Exiting")),"Error",MB_OK);
      PostMessage(WM_CLOSE);
//      exit(1);
      return;
   }
   read(handle, Palette, 768);
   close(handle);

   for (int i=0;i<4;i++)
	   Views[i].SetPalette(Palette);

   Views[3].SetPalette(Palette,false);

   {
      int hBright;
      CString BrightFile;

      {
   	   char drive[_MAX_DRIVE],dir[_MAX_DIR];
         char ExePath[256];
   
         ::GetModuleFileName(NULL, ExePath, 255);
   		_splitpath(ExePath, drive,dir,NULL,NULL);
         BrightFile = CString(drive)+CString(dir)+g_ProgSet.m_LIGFile;
      }

      hBright = open((LPCTSTR)BrightFile, O_BINARY | O_RDONLY);
      if (hBright == -1)
      {
         //notify that not found, is being created
         MessageBox((LPCTSTR)CString(CString("Could not find \"") + g_ProgSet.m_LIGFile + CString("\". file Will be created")),"Error",MB_OK);
         Views[3].SetPalette(Palette, true);
         hBright = open((LPCTSTR)BrightFile, O_BINARY | O_WRONLY | O_TRUNC | O_CREAT, S_IREAD|S_IWRITE);
         if (hBright == -1)
         {
            // could not write file;
            MessageBox("Could not write Brightness file","Error", MB_OK);
         }
         else
            write(hBright, Views[3].Bright, 256*256);
      }
      else
      {
         read(hBright, Views[3].Bright, 256*256);
      }
      close(hBright);
   }

   // Tell the skinpainting dialog to load its data
   m_PaintDlg.SendMessage(MM_LOADPAINTDATA,0,0);


   m_BottomToolDlg.m_XEdit.SetWindowText("0");
   m_BottomToolDlg.m_YEdit.SetWindowText("0");
   m_BottomToolDlg.m_ZEdit.SetWindowText("0");


   m_Ready = true;


   //   Open1Click(MainForm);       // already remmed

   /*MRUMenuItems[0] = MRUItem1;
   MRUMenuItems[1] = MRUItem2;
   MRUMenuItems[2] = MRUItem3;
   MRUMenuItems[3] = MRUItem4;
   MRUMenuItems[4] = MRUItem5;*/

   // This loads any files specefied by the command line params

   UpdateMenuMRU();
   CCommandLineInfo   CmdLine;

   AfxGetApp()->ParseCommandLine(CmdLine);

   if (CmdLine.m_strFileName != "")
   {
      
   }

   /*
   if (ParamStr(1) != "")
   {
   	LoadFile(ParamStr(1));
      CurFileName = ParamStr(1);
      MainForm->Caption = CString("Quake Model Editor - ") + CurFileName;

      CalcViewportWidthHeight();

      NextBut->Enabled=true;
      PrevBut->Enabled=true;
      FrameLabel->Caption = g_CurFrame;
      NameLabel->Caption = g_CurMdl.FrameInfo[g_CurFrame].Name;
      FitButtonClick(Sender);

      SkinEditForm->FormPaint(this);
      SendMessage(WM_PAINT);
   }
   */

   /*
   if ( MRUCount() == 0)
	{
   	for (int i=0;i<MRU_Count;i++)
      	MRUMenuItems[i]->Visible = false;
      MRUBreak->Visible = false;
      return;
   }
   */

   /*
   MRUBreak->Visible = true;
	for (int i=0;i<MRU_Count;i++)
   	if ( !(MRU[i] == "") )
	   {
			MRUMenuItems[i]->Visible = true;
         MRUMenuItems[i]->Caption = CString("&") +
         									CString(i+1) +
                                    CString("  ") +
                                    CString(MRU[i]);
	   }
   */


   // This sets it up so the select tool is currently selected, and the pan tool was
   // previously selected

   /*
   SelectBut->Click();
   PanViewsBut->Click();
   SelectBut->Click();
   */

	m_BottomToolDlg.UpdateSelectionCounters();
   CheckSpinners();


}

void CVCMDLDlg::CheckSpinners()
{
   m_ViewTab.CheckSpinners();
   m_ModifyTab.CheckSpinners();
}


void CVCMDLDlg::DrawAllViews()
{
   HDC   hDC;
   int   PanelWidth;

   PanelWidth = GetPanelWidth();

   // Get Device Context to draw on
   hDC = GetDC()->m_hDC;

   if (ToDrawView[0])
      Views[0].DrawToDC(hDC, PanelWidth,0);
	if (ToDrawView[1])
   	Views[1].DrawToDC(hDC, PanelWidth,VHeightT);
	if (ToDrawView[2])
   	Views[2].DrawToDC(hDC, PanelWidth+VWidthL,VHeightT);
	if (ToDrawView[3])
   	Views[3].DrawToDC(hDC, PanelWidth+VWidthL,0);

}

void CVCMDLDlg::DoResize()
{
   CalcViewportWidthHeight();
   DoViewPortSize();
   InitDrawInfo();

	if (ToDrawView[0])
		DrawViewMatrix(Views[0], &g_CurMdl, &m_DrawInfo, vtXY);
	if (ToDrawView[1])
		DrawViewMatrix(Views[1], &g_CurMdl, &m_DrawInfo, vtZY);
	if (ToDrawView[2])
		DrawViewMatrix(Views[2], &g_CurMdl, &m_DrawInfo, vtXZ);
	if (ToDrawView[3])
   	Draw3DView(Views[3], &g_CurMdl, &m_DrawInfo);

   DrawAllViews();

   ToDrawView[0]=true;
   ToDrawView[1]=true;
   ToDrawView[2]=true;
   ToDrawView[3]=true;

}

void CVCMDLDlg::CalcViewportWidthHeight()
{
   RECT  ClientRect;
   int   ClientWidth, ClientHeight, PanelWidth;

   GetClientRect(&ClientRect);
   ClientWidth = ClientRect.right;
   ClientHeight = ClientRect.bottom;
     
   PanelWidth = GetPanelWidth();

   VWidthL = (int)((ClientWidth-PanelWidth)*m_MidpointX);
   VWidthR = (ClientWidth-PanelWidth) - VWidthL;
   VHeightT = (int)((ClientHeight- m_BottomToolDlg.GetHeight())*m_MidpointY);
   VHeightB = (ClientHeight- m_BottomToolDlg.GetHeight()) - VHeightT;

   VWidthL2 = VWidthL/2;
   VWidthR2 = VWidthR/2;
   VHeightT2 = VHeightT/2;
   VHeightB2 = VHeightB/2;
}


void CVCMDLDlg::DoViewPortSize()
{
   if (!m_Ready)
         return;

   Views[0].SetSize(VWidthL,VHeightT);
   Views[1].SetSize(VWidthL,VHeightB);
   Views[2].SetSize(VWidthR,VHeightB);
   Views[3].SetSize(VWidthR,VHeightT);
}

void CVCMDLDlg::InitDrawInfo()
{
   CDrawViewInfo  *D;
   
   D = &m_DrawInfo;

   D->Shading = shNone;

   if (IsMenuChecked(ID_OPTIONS_3DVIEW_WIREFRAME))
      D->Texture = ttWire;
   else
      D->Texture = ttNone;

   if (IsMenuChecked(ID_OPTIONS_3DVIEW_FLATSHADED))
      D->Shading = shFlat;

   if (IsMenuChecked(ID_OPTIONS_3DVIEW_GOURADSHADED))
      D->Shading = shGourad;

   if (IsMenuChecked(ID_OPTIONS_3DVIEW_TEXTUREMAPPED))
      D->Texture = ttAffine;

   if (IsMenuChecked(ID_OPTIONS_3DVIEW_TEXTUREGOURAD))
   {
      D->Texture = ttAffine;
      D->Shading = shGourad;
   }

   D->Cam.Set(PanX, PanY, PanZ, RotX, RotY, Dist, g_Persp);

   if (IsMenuChecked(ID_OPTIONS_SHOWGRID))
      D->Grid = true;
   else
      D->Grid = false;

   if (IsMenuChecked(ID_OPTIONS_3DVIEW_SHOWREFERENCEMODEL))
      D->m_DrawRef = true;
   else
      D->m_DrawRef = false;

   if (IsMenuChecked(ID_OPTIONS_3DVIEW_DRAWBACKFACES))
      D->m_3DFaceCull = false;
   else
      D->m_3DFaceCull = true;

   if (IsMenuChecked(ID_OPTIONS_2DVIEW_DRAWBACKFACES))
      D->m_2DFaceCull = false;
   else
      D->m_2DFaceCull = true;

   if (IsMenuChecked(ID_OPTIONS_SHOWORIGIN))
      D->Origin = true;
   else
      D->Origin = false;

   D->Mode = g_MainSelectMode;

   if (IsMenuChecked(ID_OPTIONS_SHOWNORMALS_ALL))
      D->Normals = ndAll;
   else
   if (IsMenuChecked(ID_OPTIONS_SHOWNORMALS_SELECTEDFACES))
      D->Normals = ndSelected;
   else
      D->Normals = ndNone;

   if (IsMenuChecked(ID_OPTIONS_VERTICESTICKS))
      D->Ticks = true;
   else
      D->Ticks = false;

}

int CVCMDLDlg::GetPanelWidth()
{
   RECT  ToolPanelRect;
   int   PanelWidth;

   m_ToolTab.GetWindowRect(&ToolPanelRect);
   PanelWidth = ToolPanelRect.right-ToolPanelRect.left + 2;

   return PanelWidth;

}

void CVCMDLDlg::FillViewportWidthHeight(ViewType View, int &w, int &h, int &w2, int &h2)
{
   if (View == vtXY)
   {
      w = VWidthL;
      h = VHeightT;
      w2 = w >> 1;
      h2 = h >> 1;
   }
   if (View == vtZY)
   {
      w = VWidthL;
      h = VHeightB;
      w2 = w >> 1;
      h2 = h >> 1;
   }
   if (View == vtXZ)
   {
      w = VWidthR;
      h = VHeightB;
      w2 = w >> 1;
      h2 = h >> 1;
   }
   if (View == vtCamera)
   {
      w = VWidthR;
      h = VHeightT;
      w2 = w >> 1;
      h2 = h >> 1;
   }
   
}

void CVCMDLDlg::FillViewportWidthHeightOp(ViewType View, int &w, int &h, int &w2, int &h2)
{
   if (View == vtXY)
   {
      w = VWidthL;
      h = VHeightT;
      w2 = VWidthL >> 1;
      h2 = VHeightT >> 1;
   }
   if (View == vtZY)
   {
      w = VWidthL;
      h = VHeightT;
      w2 = VWidthL >> 1;
      h2 = VHeightB >> 1;
   }
   if (View == vtXZ)
   {
      w = VWidthL;
      h = VHeightT;
      w2 = VWidthR >> 1;
      h2 = VHeightB >> 1;
   }
   if (View == vtCamera)
   {
      w = VWidthL;
      h = VHeightT;
      w2 = VWidthR >> 1;
      h2 = VHeightT >> 1;
   }
   
}

void CVCMDLDlg::OnShowWindow(BOOL bShow, unsigned int nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   if (!m_Ready)
      WasOnShowWindow();
   
}

void CVCMDLDlg::OnKeyDown(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) 
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

      if (g_KeyList.ExecuteKey(nChar,c,a,s,"Main"))
         return;
   }

	if ( !m_KeyState.m_Shift && !m_KeyState.m_Alt && !m_KeyState.m_Ctrl)
   {
      if (g_MainAction == atCreateVertex && nChar)  // Enter Key
      {
      	{
				g_CreateCount++;
   	      int *temp = new int[g_CreateCount];
            memcpy(temp,g_CreateList, 4*(g_CreateCount-1));
            delete[] g_CreateList;
            g_CreateList = temp;
      	}
         g_CreateList[g_CreateCount-1] = g_CurMdl.Head.num_xyz;

         int numverts = g_CurMdl.Head.num_xyz+1;
         int n, f;

         TRealVertex *tempFrames[MAX_FRAMES],*tempVerts;
         TRealSkinVertex *tempSkinVerts;

         for (n=0;n<g_CurMdl.Head.num_frames;n++)
         {
            tempFrames[n] = new TRealVertex[numverts];
         }

         tempVerts = new TRealVertex[numverts];

         tempSkinVerts = new TRealSkinVertex[numverts];

         for (n=0;n<g_CurMdl.Head.num_xyz;n++)
         {
            tempVerts[n] = g_CurMdl.Verts[n];
            for (int f=0;f<g_CurMdl.Head.num_frames;f++)
            {
               tempFrames[f][n] = g_CurMdl.Frames[f][n];
            }
         }

         tempVerts[g_CurMdl.Head.num_xyz].x = g_downx;
         tempVerts[g_CurMdl.Head.num_xyz].y = g_downy;
         tempVerts[g_CurMdl.Head.num_xyz].z = g_downz;
         tempVerts[g_CurMdl.Head.num_xyz].Selected = true;

         for (f=0;f<g_CurMdl.Head.num_frames;f++)
         {
            tempFrames[f][g_CurMdl.Head.num_xyz].x = g_downx;
            tempFrames[f][g_CurMdl.Head.num_xyz].y = g_downy;
            tempFrames[f][g_CurMdl.Head.num_xyz].z = g_downz;
            tempFrames[f][g_CurMdl.Head.num_xyz].Selected = true;
         }

         g_CurMdl.Head.num_xyz ++;

         for (f=0;f<g_CurMdl.Head.num_frames;f++)
         {
            delete[] g_CurMdl.Frames[f];

            g_CurMdl.Frames[f] = tempFrames[f];
         }

         delete[] g_CurMdl.Verts;
         g_CurMdl.Verts = tempVerts;

      }

      if (nChar == 120)	// F9
      {
      	CString toadd;
         char  XStr[32], YStr[32], ZStr[32];
         float g_curx, g_cury, g_curz;

         m_BottomToolDlg.m_XEdit.GetWindowText(XStr, 63);
         m_BottomToolDlg.m_YEdit.GetWindowText(YStr, 63);
         m_BottomToolDlg.m_ZEdit.GetWindowText(ZStr, 63);

         g_curx = (float)atof(XStr);
         g_cury = (float)atof(YStr);
         g_curz = (float)atof(ZStr);

         toadd.Format("%s\t%s\t%s\t%s\t%.8f\t%.8f\t%.8f", 
                        g_CurMdl.FrameInfo[g_CurFrame].Name,
                        XStr, YStr, ZStr, 
                        g_LastX - g_curx,
                        g_LastY - g_cury,
                        g_LastZ - g_curz);

         g_LastX = g_curx;
         g_LastY = g_cury;
         g_LastZ = g_curz;

//			MemoForm->Memo->Lines->Add(toadd);
      }
   }

   if (m_KeyState.m_Ctrl)
   {
   	if (nChar == 70 && g_MainSelectMode == stTriangle) // Ctrl + F
      {
         m_ModifyTab.m_FlipNormalsBut.SendMessage(BM_CLICK);
      }
   }

  	DoCursors(m_KeyState);

   
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CVCMDLDlg::MouseUp(CMouseState Button, CShiftState Shift, int X, int Y)
{  
   int      PanelWidth, ClientWidth, ClientHeight;
   RECT     ClientRect;

   GetWindowRect(&ClientRect);
   ClientWidth = ClientRect.right;
   ClientHeight = ClientRect.bottom;

   PanelWidth = GetPanelWidth();

   int vwidth, vheight, vwidth2, vheight2;
	bool Update = false;

   FillViewportWidthHeightOp(g_CurView, vwidth, vheight, vwidth2, vheight2);

   if ((Button.m_Left || Button.m_Right || Button.m_Mid) == false)
   {
      ReleaseCapture();
   }
   
   if (g_CurView == vtXY)
   {
   	g_upx = ((float)(Y - vheight2)/g_Zoom + PanX);
      g_upy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
      g_upz = g_downz;
   }
   if (g_CurView == vtXZ)
   {
      g_upx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
      g_upz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
      g_upy = g_downy;
   }
   if (g_CurView == vtZY)
   {
	   g_upy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
      g_upz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
      g_upx = g_downx;
   }
   if (g_CurView == vtCamera)
   {
      g_upx = (float)(X - PanelWidth - VWidthL - VWidthR2);
      g_upy = (float)(Y - VHeightT2);
      ToDrawView[3] = true;
   }

   {
      if (g_MainAction == atMove)
      {
         CMoveUndoNode *M;
         M = new CMoveUndoNode;

         int min,max;

         if (m_ModifyTab.m_EffectFrameRangeCheck.GetCheck())
         {
            min = m_ModifyTab.m_ModFromSpin.GetPos();
            max = m_ModifyTab.m_ModToSpin.GetPos();
         }
         else
         {
            min=max=g_CurFrame;
         }

         M->Set(CString("Move "), &g_CurMdl,min,max,-g_diffx,-g_diffy,-g_diffz, g_MainSelectMode);
         g_MainUndo.AddNode(M);
      }
      if (g_MainAction == atRotate)
      {
         CRotateUndoNode *R;
         R = new CRotateUndoNode;

         int min,max;

         if (m_ModifyTab.m_EffectFrameRangeCheck.GetCheck())
         {
            min = m_ModifyTab.m_ModFromSpin.GetPos();
            max = m_ModifyTab.m_ModToSpin.GetPos();
         }
         else
         {
            min=max=g_CurFrame;
         }

         R->Set(CString("Rotate "), &g_CurMdl,min,max,g_downx,g_downy,g_downz,-g_diffx,-g_diffy,-g_diffz, g_MainSelectMode);
         g_MainUndo.AddNode(R);
      }
      if (g_MainAction == atScale)
      {
         CScaleUndoNode *S;
         S = new CScaleUndoNode;

         int min,max;

         if (m_ModifyTab.m_EffectFrameRangeCheck.GetCheck())
         {
            min = m_ModifyTab.m_ModFromSpin.GetPos();
            max = m_ModifyTab.m_ModToSpin.GetPos();
         }
         else
         {
            min=max=g_CurFrame;
         }

         S->Set(CString("Scale "), &g_CurMdl,min,max,g_downx,g_downy,g_downz,1/g_diffx,1/g_diffy,1/g_diffz, g_MainSelectMode);
         g_MainUndo.AddNode(S);
      }
	}

   if (g_MainAction == atSelect || g_MainAction == atDeselect)
   {
      bool *NewSelect = NULL;

      if (g_MainSelectMode == stVertex)
      {
         NewSelect = new bool[g_CurMdl.Head.num_xyz];
         for (int i=g_CurMdl.Head.num_xyz-1;i>=0;i--)
            NewSelect[i] = false;

         if (g_CurView == vtCamera)
         {
         	int fx,fy;

            fx = X - PanelWidth-vwidth - vwidth2;
            fy = Y - vheight2;
            for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
		         if (g_CurMdl.Verts[n].Visible)
   	         	if (abs(g_posx[n]-fx)<3 && abs(g_posy[n]-fy)<3)
                     NewSelect[n] = true;

		      Update = true;
         }
         else
         {
            float fx,fy;
            if (g_CurView == vtXY)
            {
               fx = ((float)(Y - vheight2)/g_Zoom + PanX);
               fy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
               for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
               if (g_CurMdl.Verts[i].Visible)
               {
                  if (fabs(fx-g_CurMdl.Verts[i].x)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].y)<1.5/g_Zoom)
                     NewSelect[i] = true;
               }
            }
            if (g_CurView == vtXZ)
            {
               fx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
               fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
               for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
               if (g_CurMdl.Verts[i].Visible)
               {
                  if (fabs(fx-g_CurMdl.Verts[i].x)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].z)<1.5/g_Zoom)
                     NewSelect[i] = true;
               }
            }
            if (g_CurView == vtZY)
            {
               fx = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
               fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
               for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
               if (g_CurMdl.Verts[i].Visible)
               {
                  if (fabs(fx-g_CurMdl.Verts[i].y)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].z)<1.5/g_Zoom)
                     NewSelect[i] = true;
               }
            }
		      Update = true;
         }

         if (m_BottomToolDlg.m_ObjectModeBut.GetCheck())
            g_CurMdl.SelectConnected(NewSelect, g_MainSelectMode);


         {
            CSelectUndoNode      *N;
            bool                 *n,*o;
            int                  v;

            n = new bool[g_CurMdl.Head.num_xyz];
            o = new bool[g_CurMdl.Head.num_xyz];

            for (v=0;v<g_CurMdl.Head.num_xyz;v++)
               n[v] = g_CurMdl.Verts[v].Selected;


            if (g_MainAction == atSelect)
            {
               for (int v=g_CurMdl.Head.num_xyz-1;v>=0;v--)
                  g_CurMdl.Verts[v].Selected |= NewSelect[v];
            }
            if (g_MainAction == atDeselect)
            {
               for (int v=g_CurMdl.Head.num_xyz-1;v>=0;v--)
                  g_CurMdl.Verts[v].Selected &= !NewSelect[v];
            }

            for (v=0;v<g_CurMdl.Head.num_xyz;v++)
               o[v] = g_CurMdl.Verts[v].Selected;

            N = new CSelectUndoNode;

            N->Set("Select", &g_CurMdl, g_MainSelectMode, n);
            g_MainUndo.AddNode(N);

            delete[] n;
            delete[] o;
         }
      }

      if (g_MainSelectMode == stTriangle)
      {
         NewSelect = new bool[g_CurMdl.Head.num_tris];

         for (int i=g_CurMdl.Head.num_tris-1;i>=0;i--)
            NewSelect[i] = false;

         if (g_CurView == vtCamera)
         {
         	int fx,fy;
            float cosa1,cosa2,sina1,sina2;

            cosa1 = (float)cos(RotX);
            sina1 = (float)sin(RotX);
            cosa2 = (float)cos(RotY);
            sina2 = (float)sin(RotY);

            fx = X - PanelWidth-vwidth - vwidth2;
            fy = Y - vheight2;

            {
               float closef = 800000000;
               int	closet = -1;


               for (int n=0;n<g_CurMdl.Head.num_tris;n++)
                  if (g_CurMdl.Tris[n].Visible)
                  {
                     TRealTriangle *T = &g_CurMdl.Tris[n];

                     if ( InTri(
                                 (float)g_posx[T->Vertices[0]],(float)g_posy[T->Vertices[0]],
                                 (float)g_posx[T->Vertices[1]],(float)g_posy[T->Vertices[1]],
                                 (float)g_posx[T->Vertices[2]],(float)g_posy[T->Vertices[2]],
                                 (float)fx,(float)fy
                                  ) && FaceDotProd3D(g_CurMdl, n,cosa1,sina1,cosa2,sina2)>1 )
							{
                        float d = g_posz[T->Vertices[0]] + g_posz[T->Vertices[1]] + g_posz[T->Vertices[2]];
                        if (d<closef)
                        {
                        	closef = d;
                           closet = n;
                        }
                     }
                  }

               if (closet != -1)
                  if (g_CurMdl.Tris[closet].Visible)
                  	NewSelect[closet] = true;
            }

		      Update = true;
         }
         else
         {
            float fx,fy;

	         if (g_CurView == vtXY)
            {
	            fx = ((float)(Y - vheight2)/g_Zoom + PanX);
   	         fy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);

					for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
               {
                  if (fabs(fx-g_CurMdl.Verts[i].x)<0.1/g_Zoom && fabs(fy-g_CurMdl.Verts[i].y)<0.1/g_Zoom)
                  for (int f=0;f<g_CurMdl.Head.num_tris;f++)
		               if (g_CurMdl.Tris[f].Visible)
      	               if ( g_CurMdl.Tris[f].Vertices[0] == i ||
         	                 g_CurMdl.Tris[f].Vertices[1] == i ||
            	              g_CurMdl.Tris[f].Vertices[2] == i )
                           NewSelect[f] = true;
               }
         	}
            if (g_CurView == vtXZ)
            {
               fx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
               fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

               for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
                  if (fabs(fx-g_CurMdl.Verts[i].x)<0.1/g_Zoom && fabs(fy-g_CurMdl.Verts[i].z)<0.1/g_Zoom)
	                  for (int f=0;f<g_CurMdl.Head.num_tris;f++)
			               if (g_CurMdl.Tris[f].Visible)
	   	                  if ( g_CurMdl.Tris[f].Vertices[0] == i ||
   	   	                    g_CurMdl.Tris[f].Vertices[1] == i ||
      	   	                 g_CurMdl.Tris[f].Vertices[2] == i )
                              NewSelect[f] = true;
            }

            if (g_CurView == vtZY)
            {
               fx = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
               fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

               for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
                  if (fabs(fx-g_CurMdl.Verts[i].y)<0.1/g_Zoom && fabs(fy-g_CurMdl.Verts[i].z)<0.1/g_Zoom)
	                  for (int f=0;f<g_CurMdl.Head.num_tris;f++)
			               if (g_CurMdl.Tris[f].Visible)
	   	                  if ( g_CurMdl.Tris[f].Vertices[0] == i ||
   	   	                    g_CurMdl.Tris[f].Vertices[1] == i ||
      	   	                 g_CurMdl.Tris[f].Vertices[2] == i )
                              NewSelect[f] = true;
            }

		      Update = true;
         }

         if (m_BottomToolDlg.m_ObjectModeBut.GetCheck())
            g_CurMdl.SelectConnected(NewSelect, g_MainSelectMode);

         {
            CSelectUndoNode   *N;
            bool              *n,*o;
            int               v;

            n = new bool[g_CurMdl.Head.num_tris];
            o = new bool[g_CurMdl.Head.num_tris];

            for (v=0;v<g_CurMdl.Head.num_tris;v++)
               n[v] = g_CurMdl.Tris[v].Selected;


            if (g_MainAction == atSelect)
            {
               for (int t=g_CurMdl.Head.num_tris-1;t>=0;t--)
                  g_CurMdl.Tris[t].Selected |= NewSelect[t];
            }
            if (g_MainAction == atDeselect)
            {
               for (int t=g_CurMdl.Head.num_tris-1;t>=0;t--)
                  g_CurMdl.Tris[t].Selected &= !NewSelect[t];
            }

            for (v=0;v<g_CurMdl.Head.num_tris;v++)
               o[v] = g_CurMdl.Tris[v].Selected;

            N = new CSelectUndoNode;

            N->Set("Select", &g_CurMdl, g_MainSelectMode, n);
            g_MainUndo.AddNode(N);

            delete[] n;
            delete[] o;
         }
      }
      if (g_MainSelectMode == stBone)
      {
         bool target;
         if (g_MainAction == atSelect)
            target = true;
         if (g_MainAction == atDeselect)
            target = false;

         if (g_CurView == vtXY)
         {
            float fx,fy;

            fx = ((float)(Y - vheight2)/g_Zoom + PanX);
            fy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);


            g_CurMdl.Bones.SelectBonesNear(fx,fy,(float)0,(float)0.1, true, true, false, target);

		      Update = true;
         }
         if (g_CurView == vtXZ)
         {
            float fx,fy;

            fx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
            fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

            g_CurMdl.Bones.SelectBonesNear(fx,(float)0,fy,(float)0.1, true, false, true, target);

		      Update = true;
         }
         if (g_CurView == vtZY)
         {
            float fx,fy;
            fx = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
            fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
            g_CurMdl.Bones.SelectBonesNear((float)0,fx,fy,(float)0.1, false, true, true, target);

		      Update = true;
         }
	// !!! do camera here !!!
      }


      if (NewSelect)
         delete[] NewSelect;
   }

   if (g_MainAction == atSelectBox || g_MainAction == atDeselectBox)
   {
   	float minx, miny, maxx,maxy, minz, maxz;
      bool *NewSelect = NULL;

      if (g_downx<g_upx)
      {
      	minx=g_downx;
         maxx=g_upx;
      }
      else
      {
      	minx = g_upx;
         maxx=g_downx;
      }
      if (g_downy<g_upy)
      {
      	miny=g_downy;
         maxy=g_upy;
      }
      else
      {
      	miny = g_upy;
         maxy=g_downy;
      }
      if (g_downz<g_upz)
      {
      	minz=g_downz;
         maxz=g_upz;
      }
      else
      {
      	minz = g_upz;
         maxz=g_downz;
      }

		if (g_MainSelectMode == stVertex)
      {
         NewSelect = new bool[g_CurMdl.Head.num_xyz];
         for (int n=g_CurMdl.Head.num_xyz-1;n>=0;n--)
            NewSelect[n] = false;

         if (g_CurView == vtXY)
            for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
            if (g_CurMdl.Verts[n].Visible)
            {
               if (g_CurMdl.Verts[n].x >=minx && g_CurMdl.Verts[n].x<maxx &&
                   g_CurMdl.Verts[n].y >=miny && g_CurMdl.Verts[n].y<maxy)
                     NewSelect[n] = true;
            }
         if (g_CurView == vtXZ)
            for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
            if (g_CurMdl.Verts[n].Visible)
            {
               if (g_CurMdl.Verts[n].x >=minx && g_CurMdl.Verts[n].x<maxx &&
                   g_CurMdl.Verts[n].z >=minz && g_CurMdl.Verts[n].z<maxz)
                     NewSelect[n] = true;
            }
         if (g_CurView == vtZY)
            for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
            if (g_CurMdl.Verts[n].Visible)
            {
               if (g_CurMdl.Verts[n].z >=minz && g_CurMdl.Verts[n].z<maxz &&
                   g_CurMdl.Verts[n].y >=miny && g_CurMdl.Verts[n].y<maxy)
                     NewSelect[n] = true;
            }
         if (g_CurView == vtCamera)
            for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
               if (g_CurMdl.Verts[n].Visible)
               {
                  if (g_posx[n] >= minx && g_posx[n] <= maxx && g_posy[n] >= miny && g_posy[n] <= maxy)
                     NewSelect[n] = true;
               }

         if (m_BottomToolDlg.m_ObjectModeBut.GetCheck())
            g_CurMdl.SelectConnected(NewSelect, g_MainSelectMode);

         {
            CSelectUndoNode *N;
            bool *n,*o;

            n = new bool[g_CurMdl.Head.num_xyz];
            o = new bool[g_CurMdl.Head.num_xyz];

            g_CurMdl.LoadSelection(n, ssVertex);

            if (g_MainAction == atSelectBox)
            {
               for (int v=g_CurMdl.Head.num_xyz-1;v>=0;v--)
                  g_CurMdl.Verts[v].Selected |= NewSelect[v];
            }
            if (g_MainAction == atDeselectBox)
            {
               for (int v=g_CurMdl.Head.num_xyz-1;v>=0;v--)
                  g_CurMdl.Verts[v].Selected &= !NewSelect[v];
            }

            g_CurMdl.LoadSelection(o, ssVertex);

            N = new CSelectUndoNode;

            N->Set("Select", &g_CurMdl, g_MainSelectMode, n);
            g_MainUndo.AddNode(N);

            delete[] n;
            delete[] o;
         }
      }

		if (g_MainSelectMode == stTriangle)
      {
         NewSelect = new bool[g_CurMdl.Head.num_tris];
         for (int i=g_CurMdl.Head.num_tris-1;i>=0;i--)
            NewSelect[i] = false;

         if (g_CurView == vtXY)
            for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
            {
               if (g_CurMdl.Verts[n].x >=minx && g_CurMdl.Verts[n].x<maxx &&
                   g_CurMdl.Verts[n].y >=miny && g_CurMdl.Verts[n].y<maxy)
            for (int f=0;f<g_CurMdl.Head.num_tris;f++)
               if (g_CurMdl.Tris[f].Visible)
                  if ( g_CurMdl.Tris[f].Vertices[0] == n ||
                       g_CurMdl.Tris[f].Vertices[1] == n ||
                       g_CurMdl.Tris[f].Vertices[2] == n )
                     NewSelect[f] = true;
            }
         if (g_CurView == vtXZ)
            for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
            {
               if (g_CurMdl.Verts[n].x >=minx && g_CurMdl.Verts[n].x<maxx &&
                   g_CurMdl.Verts[n].z >=minz && g_CurMdl.Verts[n].z<maxz)
            for (int f=0;f<g_CurMdl.Head.num_tris;f++)
               if (g_CurMdl.Tris[f].Visible)
                  if ( g_CurMdl.Tris[f].Vertices[0] == n ||
                       g_CurMdl.Tris[f].Vertices[1] == n ||
                       g_CurMdl.Tris[f].Vertices[2] == n )
                     NewSelect[f] = true;
            }
         if (g_CurView == vtZY)
            for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
            {
               if (g_CurMdl.Verts[n].z >=minz && g_CurMdl.Verts[n].z<maxz &&
                   g_CurMdl.Verts[n].y >=miny && g_CurMdl.Verts[n].y<maxy)
                  for (int f=0;f<g_CurMdl.Head.num_tris;f++)
                     if (g_CurMdl.Tris[f].Visible)
                        if ( g_CurMdl.Tris[f].Vertices[0] == n ||
                             g_CurMdl.Tris[f].Vertices[1] == n ||
                             g_CurMdl.Tris[f].Vertices[2] == n )
                           NewSelect[f] = true;
            }
         if (g_CurView == vtCamera)
            for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
            {
               if (g_posx[n] >= minx && g_posx[n]<maxx &&
                   g_posy[n] >= miny && g_posy[n]<maxy)
                  for (int f=0;f<g_CurMdl.Head.num_tris;f++)
                     if (g_CurMdl.Tris[f].Visible)
                        if ( g_CurMdl.Tris[f].Vertices[0] == n ||
                             g_CurMdl.Tris[f].Vertices[1] == n ||
                             g_CurMdl.Tris[f].Vertices[2] == n )
                           NewSelect[f] = true;
            }

         if (m_BottomToolDlg.m_ObjectModeBut.GetCheck())
            g_CurMdl.SelectConnected(NewSelect, g_MainSelectMode);

         CSelectUndoNode   *N;
         bool              *n,*o;

         n = new bool[g_CurMdl.Head.num_tris];
         o = new bool[g_CurMdl.Head.num_tris];

         g_CurMdl.LoadSelection(n, ssTriangle);

         if (g_MainAction == atSelectBox)
         {
            for (int t=g_CurMdl.Head.num_tris-1;t>=0;t--)
               g_CurMdl.Tris[t].Selected |= NewSelect[t];
         }
         if (g_MainAction == atDeselectBox)
         {
            for (int t=g_CurMdl.Head.num_tris-1;t>=0;t--)
               g_CurMdl.Tris[t].Selected &= !NewSelect[t];
         }

         g_CurMdl.LoadSelection(o, ssTriangle);

         N = new CSelectUndoNode;

         N->Set("Select", &g_CurMdl, g_MainSelectMode, n);
         g_MainUndo.AddNode(N);

         delete[] n;
         delete[] o;
		}

      if (g_MainSelectMode == stBone)
      {
      	bool targ = (g_MainAction == atSelectBox);
      	if (g_CurView == vtXY)
	         g_CurMdl.Bones.SelectBonesIn(minx, miny, minz,
            								 maxx, maxy, maxz,
                                     true, true, false, targ);
      	if (g_CurView == vtXZ)
	         g_CurMdl.Bones.SelectBonesIn(minx, miny, minz,
            								 maxx, maxy, maxz,
                                     true, false, true, targ);
      	if (g_CurView == vtZY)
	         g_CurMdl.Bones.SelectBonesIn(minx, miny, minz,
            								 maxx, maxy, maxz,
                                     false, true, true, targ);

      }
      if (NewSelect)
         delete[] NewSelect;
	   Update = true;
	}

   if (	g_MainAction != atCreateVertex &&
   		g_MainAction != atBuildFace &&
   		g_MainAction != atBuildingFace1 &&
   		g_MainAction != atBuildingFace2 &&
   		g_MainAction != atBuildingFaceDone )
		g_MainAction = atNone;

   int numsel=0;
   for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
	   if (g_CurMdl.Verts[i].Visible)
			if (g_CurMdl.Verts[i].Selected)
      	numsel++;


	if (Update)
   {
      DoSync();
      SelCentre = GetSelectionCentre(g_CurMdl, g_MainSelectMode);
      CurAxis = GetSelectionAxis();
      SendMessage(WM_PAINT);
//      SendMessage(WM_PAINT);
   }

}

void CVCMDLDlg::MouseDown(CMouseState Button, CShiftState Shift, int X, int Y)
{
   SetCapture();

   int      PanelWidth, ClientWidth, ClientHeight;
   RECT     ClientRect;

   GetWindowRect(&ClientRect);
   ClientWidth = ClientRect.right;
   ClientHeight = ClientRect.bottom;

   PanelWidth = GetPanelWidth();

   if ( (Button.m_Right && Shift.m_Ctrl) ||
         (abs(X-VWidthL - m_SideToolDlg.GetWidth())<2 && abs(Y-VHeightT<2)))
   {
      g_MainAction = atMidPointMove;
      m_MidpointX = (float)(X-PanelWidth)/(float)(ClientWidth-PanelWidth);
      m_MidpointY = (float)(Y)/(float)(ClientHeight-m_BottomToolDlg.GetHeight());
      DoResize();
      return;
   }

   int vwidth, vheight, vwidth2, vheight2;

	g_diffx = g_diffy = g_diffz = 0;

//	ActiveControl = NULL;

	if (m_BottomToolDlg.m_VertexModeBut.GetCheck())
   	g_MainSelectMode = stVertex;
	if (m_BottomToolDlg.m_FaceModeBut.GetCheck())
   	g_MainSelectMode = stTriangle;
	if (m_BottomToolDlg.m_BoneModeBut.GetCheck())
   	g_MainSelectMode = stBone;

   g_CurView = GetViewFromCursor(X, Y);
   FillViewportWidthHeightOp(g_CurView, vwidth, vheight, vwidth2, vheight2);

   if ((Button.m_Left || Button.m_Right || Button.m_Mid) && (m_BottomToolDlg.m_PanViewsBut.GetCheck() || Shift.m_Shift))
		g_MainAction = atPan;

   if (Button.m_Mid)
      g_MainAction = atPan;

	if (Button.m_Left && g_MainAction == atNone)
	{
		if (m_ToolTab.GetCurrentDlg() == &m_ModifyTab ||
      	  m_ToolTab.GetCurrentDlg() == &m_ViewTab)
      {
         if (m_ModifyTab.m_SelectBut.GetCheck())
         {
            if (Shift.m_Alt)
               g_MainAction = atDeselect;
            else
               g_MainAction = atSelect;
         }
         if (m_ModifyTab.m_MoveBut.GetCheck())
         	g_MainAction = atMove;
         if (m_ModifyTab.m_RotateBut.GetCheck())
         	g_MainAction = atRotate;
         if (m_ModifyTab.m_ScaleBut.GetCheck())
         	g_MainAction = atScale;

         if (g_MainAction == atScale)
         {
            g_diffx = g_diffy = g_diffz = 1;
         }

      }
		if (m_ToolTab.GetCurrentDlg() == &m_BonesTab)
      {
         if (m_ModifyTab.m_SelectBut.GetCheck())
         {
            if (Shift.m_Alt)
               g_MainAction = atDeselect;
            else
               g_MainAction = atSelect;
         }
/*			if (CreateBoneBut->Down)
         	g_MainAction = atCreateBone;
			if (AssignBoneBut->Down)
         	g_MainAction = atAssignParent;
         if (AssignVertBut->Down)
         	g_MainAction = atAssignVertToBone;

         if (MoveBut->Down)
            g_MainAction = atMove;
         if (RotateBut->Down)
            g_MainAction = atRotate;
         if (ScaleBut->Down)
            g_MainAction = atScale;*/
      }
      if (m_ToolTab.GetCurrentDlg() == &m_CreateTab)
      {
      	if (m_CreateTab.m_CreateVertexBut.GetCheck())
         	g_MainAction = atCreatingVertex;
         if (m_CreateTab.m_BuildFaceBut.GetCheck())
         	g_MainAction = atBuildFace;
      }
   }

   if (g_CurView == vtXY)
   {
   	g_downx = ((float)(Y - vheight2)/g_Zoom + PanX);
      g_downy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);

      if (g_MainAction == atScale)
      	g_downz = g_CurMdl.GetSelectionCentre(g_MainSelectMode).z;
      else if (g_MainAction != atCreateVertex)
      	g_downz = PanZ;
   }
   if (g_CurView == vtXZ)
   {
      g_downx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
      g_downz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

      if (g_MainAction == atScale)
      	g_downy = g_CurMdl.GetSelectionCentre(g_MainSelectMode).y;
      else if (g_MainAction != atCreateVertex)
         g_downy = PanY;

   }
   if (g_CurView == vtZY)
   {
	   g_downy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
      g_downz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

      if (g_MainAction == atScale)
      	g_downx = g_CurMdl.GetSelectionCentre(g_MainSelectMode).x;
      else if (g_MainAction != atCreateVertex)
         g_downx = PanX;
   }
   if (g_CurView == vtCamera)
   {
      g_downx = (float)(X - PanelWidth - VWidthL - VWidthR2);
      g_downy = (float)(Y - VHeightT2);
      ToDrawView[3] = true;
   }

   if (g_MainAction == atMove || g_MainAction == atRotate || g_MainAction == atScale
       || g_MainAction == atAssignParent)
   {

   	bool seld=false;

      seld = g_CurMdl.AnythingSelected();

      if (Shift.m_Alt)
         g_MainAction = atDeselect;
      else
      if (Shift.m_Ctrl)
         g_MainAction = atSelect;
      else
      if (seld==false)
      	g_MainAction = atSelect;
   }



   if (g_MainAction == atCreatingVertex)
   	g_MainAction = atCreateVertex;
   if (g_MainAction == atCreateVertex)
   	SendMessage(WM_PAINT);

	if (g_MainAction == atBuildFace)
   {
      if (g_CurView == vtXY)
      {
         float fx,fy;

         fx = ((float)(Y - vheight2)/g_Zoom + PanX);
         fy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);


         for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
         if (g_CurMdl.Verts[i].Visible)
         {
            if (fabs(fx-g_CurMdl.Verts[i].x)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].y)<1.5/g_Zoom)
            {
            	CreateFace[0] = (short)i;
               g_MainAction = atBuildingFace1;
               g_BuildFaceView = vtXY;
               break;
            }
         }

         SendMessage(WM_PAINT);
      }
      if (g_CurView == vtXZ)
      {
         float fx,fy;

         fx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
         fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

         for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
         if (g_CurMdl.Verts[i].Visible)
         {
            if (fabs(fx-g_CurMdl.Verts[i].x)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].z)<1.5/g_Zoom)
         	{
            	CreateFace[0] = (short)i;
               g_MainAction = atBuildingFace1;
               g_BuildFaceView = vtXZ;
               break;
            }
         }

         SendMessage(WM_PAINT);
      }
      if (g_CurView == vtZY)
      {
         float fx,fy;

         fx = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
         fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

         for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
         if (g_CurMdl.Verts[i].Visible)
         {
            if (fabs(fx-g_CurMdl.Verts[i].y)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].z)<1.5/g_Zoom)
				{
            	CreateFace[0] = (short)i;
               g_MainAction = atBuildingFace1;
               g_BuildFaceView = vtZY;
               break;
            }
         }

         SendMessage(WM_PAINT);
      }
      if (g_CurView == vtCamera)
      {
         int fx,fy;

         fx = X - PanelWidth-vwidth - vwidth2;
         fy = Y - vheight2;
         for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
         if (g_CurMdl.Verts[n].Visible)
            if (abs(g_posx[n]-fx)<3 && abs(g_posy[n]-fy)<3)
            {
            	CreateFace[0] = (short)n;
               g_MainAction = atBuildingFace1;
               g_BuildFaceView = vtCamera;
               break;
            }

         SendMessage(WM_PAINT);
      }

      return;
   }
	if (g_MainAction == atBuildingFace1)
   {
   	if (Button.m_Right)
      {
      	g_MainAction = atBuildFace;
         SendMessage(WM_PAINT);
         return;
      }

      if (g_CurView == vtXY)
      {
         float fx,fy;

         fx = ((float)(Y - vheight2)/g_Zoom + PanX);
         fy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);


         for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
         if (g_CurMdl.Verts[i].Visible)
         {
            if (fabs(fx-g_CurMdl.Verts[i].x)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].y)<1.5/g_Zoom
	            	&& i != CreateFace[0])
            {
            	CreateFace[1] = (short)i;
               g_MainAction = atBuildingFace2;
               break;
            }
         }
      }
      if (g_CurView == vtXZ)
      {
         float fx,fy;

         fx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
         fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

         for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
         if (g_CurMdl.Verts[i].Visible)
         {
            if (fabs(fx-g_CurMdl.Verts[i].x)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].z)<1.5/g_Zoom
		            && i != CreateFace[0])
         	{
            	CreateFace[1] = (short)i;
               g_MainAction = atBuildingFace2;
               break;
            }
         }
      }
      if (g_CurView == vtZY)
      {
         float fx,fy;

         fx = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
         fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

         for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
         if (g_CurMdl.Verts[i].Visible)
         {
            if (fabs(fx-g_CurMdl.Verts[i].y)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].z)<1.5/g_Zoom
            		&& i != CreateFace[0])
				{
            	CreateFace[1] = (short)i;
               g_MainAction = atBuildingFace2;
               break;
            }
         }
      }
      if (g_CurView == vtCamera)
      {
         int fx,fy;

         fx = X - PanelWidth-vwidth - vwidth2;
         fy = Y - vheight2;
         for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
         if (g_CurMdl.Verts[n].Visible)
            if (abs(g_posx[n]-fx)<3 && abs(g_posy[n]-fy)<3
            	&& n != CreateFace[0])
            {
            	CreateFace[1] = (short)n;
               g_MainAction = atBuildingFace2;
               break;
            }
      }

      SendMessage(WM_PAINT);
		return;
   }
	if (g_MainAction == atBuildingFace2)
   {
   	if (Button.m_Right)
      {
      	g_MainAction = atBuildFace;
         SendMessage(WM_PAINT);
         return;
      }
      if (g_CurView == vtXY)
      {
         float fx,fy;

         fx = ((float)(Y - vheight2)/g_Zoom + PanX);
         fy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);


         for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
         if (g_CurMdl.Verts[i].Visible)
         {
            if (fabs(fx-g_CurMdl.Verts[i].x)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].y)<1.5/g_Zoom
            	&& (i != CreateFace[0] && i!= CreateFace[1]))
            {
            	CreateFace[2] = (short)i;
               g_MainAction = atBuildingFaceDone;
               break;
            }
         }
      }
      if (g_CurView == vtXZ)
      {
         float fx,fy;

         fx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
         fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

         for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
         if (g_CurMdl.Verts[i].Visible)
         {
            if (fabs(fx-g_CurMdl.Verts[i].x)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].z)<1.5/g_Zoom
            		&& (i != CreateFace[0] && i!= CreateFace[1]))
         	{
            	CreateFace[2] = (short)i;
               g_MainAction = atBuildingFaceDone;
               break;
            }
         }
      }
      if (g_CurView == vtZY)
      {
         float fx,fy;

         fx = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
         fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

         for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
         if (g_CurMdl.Verts[i].Visible)
         {
            if (fabs(fx-g_CurMdl.Verts[i].y)<1.5/g_Zoom && fabs(fy-g_CurMdl.Verts[i].z)<1.5/g_Zoom
            		&& (i != CreateFace[0] && i!= CreateFace[1]))
				{
            	CreateFace[2] = (short)i;
               g_MainAction = atBuildingFaceDone;
               break;
            }
         }
      }
      if (g_CurView == vtCamera)
      {
         int fx,fy;

         fx = X - PanelWidth-vwidth - vwidth2;
         fy = Y - vheight2;
         for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
         if (g_CurMdl.Verts[n].Visible)
            if (abs(g_posx[n]-fx)<3 && abs(g_posy[n]-fy)<3
            		&& (n != CreateFace[0] && n!= CreateFace[1]))
            {
            	CreateFace[2] = (short)n;
               g_MainAction = atBuildingFaceDone;
               break;
            }
      }
      SendMessage(WM_PAINT);
   }

   if (g_MainAction == atCreateBone)
   {
      g_CurMdl.Bones.AddBone(g_downx,g_downy,g_downz);
      SendMessage(WM_PAINT);
   }
   if (g_MainAction == atBuildingFaceDone)
   {
   	TRealTriangle *tempTris;

      tempTris = new TRealTriangle[g_CurMdl.Head.num_tris+1];

      memcpy(tempTris, g_CurMdl.Tris, g_CurMdl.Head.num_tris * sizeof(TRealTriangle));

      tempTris[g_CurMdl.Head.num_tris].Vertices[0] = CreateFace[0];
      tempTris[g_CurMdl.Head.num_tris].Vertices[1] = CreateFace[1];
      tempTris[g_CurMdl.Head.num_tris].Vertices[2] = CreateFace[2];

      tempTris[g_CurMdl.Head.num_tris].SkinVerts[0] = 0;
      tempTris[g_CurMdl.Head.num_tris].SkinVerts[1] = 0;
      tempTris[g_CurMdl.Head.num_tris].SkinVerts[2] = 0;

      tempTris[g_CurMdl.Head.num_tris].Selected = true;

      g_CurMdl.Head.num_tris++;

      delete[] g_CurMdl.Tris;
      g_CurMdl.Tris = tempTris;

      g_MainAction = atBuildFace;

   	CalcAllNormals();
      SendMessage(WM_PAINT);
   }


   if (g_MainAction == atAssignParent)
   {
      TempBone = NULL;

      if (g_CurView == vtXY)
      {
         float fx,fy;

         fx = ((float)(Y - vheight2)/g_Zoom + PanX);
         fy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);


         TempBone = g_CurMdl.Bones.GetBoneNear(fx,fy,(float)0,(float)0.1, true, true, false);
      }
      if (g_CurView == vtXZ)
      {
         float fx,fy;

         fx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
         fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

         TempBone = g_CurMdl.Bones.GetBoneNear(fx,(float)0,fy,(float)0.1, true, false, true);
      }
      if (g_CurView == vtZY)
      {
         float fx,fy;
         fx = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
         fy = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);

         TempBone = g_CurMdl.Bones.GetBoneNear((float)0,fx,fy,(float)0.1, false, true, true);
      }

      g_CurMdl.Bones.ApplyFuncToSelected(SetParentForBone);
      g_CurMdl.Bones.CheckRoot();

      SendMessage(WM_PAINT);
   }

	g_ox=X;
   g_oy=Y;

   CurAxis = GetSelectionAxis();

}

ViewType CVCMDLDlg::GetViewFromCursor(int X, int Y)
{
   int PanelWidth;
   PanelWidth = GetPanelWidth();

   if (X>PanelWidth && X<PanelWidth+VWidthL && Y>0 && Y<VHeightT)
      return vtXY;
   if (X>PanelWidth && X<PanelWidth+VWidthL && Y>VHeightT && Y<VHeightT+VHeightB)
      return vtZY;
   if (X>PanelWidth+VWidthL && X<PanelWidth+VWidthL+VWidthR && Y>VHeightT && Y<VHeightT+VHeightB)
      return vtXZ;
   if (X>PanelWidth+VWidthL && X<PanelWidth+VWidthL+VWidthR && Y<VHeightT)
   	return vtCamera;

   return vtCamera;

}

void CVCMDLDlg::OnLButtonDown(unsigned int nFlags, CPoint point) 
{
// First determine if the point is inside the for viewports or not
   if (point.x < m_SideToolDlg.GetWidth() || point.y>VHeightT+VHeightB)
   {
   	CDialog::OnLButtonDown(nFlags, point);
      return;
   }

   m_ButtonState.m_Left = true;
   m_ShiftState.SetFromFlags(nFlags);

   MouseDown(m_ButtonState, m_ShiftState, point.x, point.y); 
   
  	CDialog::OnLButtonDown(nFlags, point);
}

void CVCMDLDlg::OnLButtonUp(unsigned int nFlags, CPoint point) 
{
   m_ButtonState.m_Left = false;
   m_ShiftState.SetFromFlags(nFlags);

   MouseUp(m_ButtonState, m_ShiftState, point.x, point.y); 
   
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CVCMDLDlg::OnMButtonDown(unsigned int nFlags, CPoint point) 
{
// First determine if the point is inside the for viewports or not
   if (point.x < m_SideToolDlg.GetWidth() || point.y>VHeightT+VHeightB)
   {
   	CDialog::OnLButtonDown(nFlags, point);
      return;
   }

   m_ButtonState.m_Mid = true;
   m_ShiftState.SetFromFlags(nFlags);

   MouseDown(m_ButtonState, m_ShiftState, point.x, point.y); 
   
	
	CDialog::OnMButtonDown(nFlags, point);
}

void CVCMDLDlg::OnMButtonUp(unsigned int nFlags, CPoint point) 
{
   m_ButtonState.m_Mid = false;
   m_ShiftState.SetFromFlags(nFlags);

   MouseUp(m_ButtonState, m_ShiftState, point.x, point.y); 
   
	
	CDialog::OnMButtonUp(nFlags, point);
}

void CVCMDLDlg::OnRButtonDown(unsigned int nFlags, CPoint point) 
{
// First determine if the point is inside the for viewports or not
   if (point.x < m_SideToolDlg.GetWidth() || point.y>VHeightT+VHeightB)
   {
   	CDialog::OnLButtonDown(nFlags, point);
      return;
   }

   m_ButtonState.m_Right = true;
   m_ShiftState.SetFromFlags(nFlags);

   MouseDown(m_ButtonState, m_ShiftState, point.x, point.y); 
   
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CVCMDLDlg::OnRButtonUp(unsigned int nFlags, CPoint point) 
{
   m_ButtonState.m_Right = false;
   m_ShiftState.SetFromFlags(nFlags);

   if (g_MainAction != atNone)
      MouseUp(m_ButtonState, m_ShiftState, point.x, point.y); 
   else
      CDialog::OnRButtonUp(nFlags, point);
}

void CVCMDLDlg::OnMouseMove(unsigned int nFlags, CPoint point) 
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

   if ( g_MainAction == atMidPointMove)
   {
      m_MidpointX = (float)(X-PanelWidth)/(float)(ClientWidth-PanelWidth);
      m_MidpointY = (float)Y/(float)(ClientHeight-m_BottomToolDlg.GetHeight());
      DoResize();
      return;
   }

	bool Update = false;
   int vwidth, vheight, vwidth2, vheight2;

  	DoCursors(Shift);

   FillViewportWidthHeightOp(g_CurView, vwidth, vheight, vwidth2, vheight2);

   if (g_MainAction != atNone)
   {
      WrapMouseToScreen(m_hWnd, X,Y,g_ox, g_oy);
   }

   bool *vertsel;
   vertsel = new bool[g_CurMdl.Head.num_xyz];

	if (g_MainSelectMode == stVertex)
	{
      for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
      	vertsel[n] = g_CurMdl.Verts[n].Selected && g_CurMdl.Verts[n].Visible;
   }

	if (g_MainSelectMode == stTriangle)
	{
      int n;
      for (n=0;n<g_CurMdl.Head.num_xyz;n++)
      	vertsel[n] = false;

      for (n=0;n<g_CurMdl.Head.num_tris;n++)
      if (g_CurMdl.Tris[n].Selected)
         if (g_CurMdl.Tris[n].Visible)
	         {
		   		vertsel[ g_CurMdl.Tris[n].Vertices[0] ] = true;
			   	vertsel[ g_CurMdl.Tris[n].Vertices[1] ] = true;
   				vertsel[ g_CurMdl.Tris[n].Vertices[2] ] = true;
	         }
   }

   if (g_MainAction == atMove || g_MainAction == atRotate ||
   	 g_MainAction == atScale )
   {
      CXFormItemInfo    XFInfo;

      XFInfo.m_Mdl = &g_CurMdl;
      XFInfo.m_Verts = g_CurMdl.Verts;
      XFInfo.m_Sel = vertsel;
      XFInfo.m_View = g_CurView;
      XFInfo.m_Action = g_MainAction;
      XFInfo.m_ConstrainX = this->m_ModifyTab.m_XBut.GetCheck() == 1;
      XFInfo.m_ConstrainY = this->m_ModifyTab.m_YBut.GetCheck() == 1;
      XFInfo.m_ConstrainZ = this->m_ModifyTab.m_ZBut.GetCheck() == 1;
      XFInfo.m_X = X;
      XFInfo.m_Y = Y;
      XFInfo.m_ox = g_ox;
      XFInfo.m_oy = g_oy;
      FillViewportWidthHeight(g_CurView, XFInfo.m_vwidth, XFInfo.m_vheight, XFInfo.m_vwidth2, XFInfo.m_vheight2);

	   if (m_ModifyTab.m_EffectFrameRangeCheck.GetCheck())
      {
      	int               fmin = m_ModifyTab.m_ModFromSpin.GetPos();
         int               fmax = m_ModifyTab.m_ModToSpin.GetPos();


      	if (g_CurFrame>=fmin && g_CurFrame<=fmax)
         {
            XFInfo.m_Verts = g_CurMdl.Verts;
	      	DoXFormVertFaceBone(&XFInfo);
         }
         else
         {
            XFInfo.m_Verts = g_CurMdl.Frames[fmin];
		      DoXFormVertFaceBone(&XFInfo);
   	      fmin++;
      	}

         float odx = g_diffx,ody = g_diffy ,odz = g_diffz;

   		for (int f=fmin;f<=fmax;f++)
         {
            XFInfo.m_Verts = g_CurMdl.Frames[f];
	   	   if (DoXFormVertFaceBone(&XFInfo))
   	   	{
	      	   g_ChangesMade = 1;
   	      	Update = true;
	   	   }
         }

         g_diffx = odx;
         g_diffy = ody;
         g_diffz = odz;
      }
      else
      {
         XFInfo.m_Verts = g_CurMdl.Verts;
         if (DoXFormVertFaceBone(&XFInfo))
         {
            g_ChangesMade = 1;
            Update = true;
         }
      }
  	}

   delete[] vertsel;

   if (g_MainAction == atPan)
   {
   	if (Shift.m_Left)
      {
         if (g_CurView == vtXY)
         {
            PanY += (g_ox-X)/g_Zoom;
            PanX += (g_oy-Y)/g_Zoom;
				Update = true;
         }
         if (g_CurView == vtXZ)
         {
            PanX += (-g_ox+X)/g_Zoom;
            PanZ += (g_oy-Y)/g_Zoom;
				Update = true;
         }
         if (g_CurView == vtZY)
         {
            PanY += (g_ox-X)/g_Zoom;
            PanZ += (g_oy-Y)/g_Zoom;
				Update = true;
         }
      }
   	if (Shift.m_Right && g_CurView != vtCamera)
      {
         if (g_oy>Y)
         	g_Zoom *= (float)(0.02 * (g_oy-Y) + 1);
         else
         	g_Zoom /= (float)(0.02 * (Y-g_oy) + 1);

			Update = true;
      }
      if (g_CurView == vtCamera)
      {
         ToDrawView[0] = false;
         ToDrawView[1] = false;
         ToDrawView[2] = false;
         if (Shift.m_Left&&Shift.m_Right)
         {
            double OldPersp = g_Persp;
            g_Persp += (float)(g_oy-Y)/10;
            
            Dist *= (float)(g_Persp/OldPersp);
            
         }
         else
         {
            if (Shift.m_Left)
            {
               RotX += (float)(g_ox-X)/100;
               RotY -= (float)(g_oy-Y)/100;
               
            }
            else
            {
               if (Shift.m_Right)
               {
                  Dist -= (float)(g_oy-Y)/2;
               }
               
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

                  ToDrawView[0] = true;
                  ToDrawView[1] = true;
                  ToDrawView[2] = true;

               }
            }
         }
         
			Update = true;
      }

   }
   if (g_MainAction == atSelect && (X!=g_ox || Y!=g_oy))
		g_MainAction = atSelectBox;
   if (g_MainAction == atDeselect && (X!=g_ox || Y!=g_oy))
		g_MainAction = atDeselectBox;

   if (g_MainAction == atSelectBox || g_MainAction == atDeselectBox)
   {
   	ToDrawView[0] = false;
   	ToDrawView[1] = false;
   	ToDrawView[2] = false;
   	ToDrawView[3] = false;
      if (g_CurView == vtXY)
      {
         g_upx = ((float)(Y - vheight2)/g_Zoom + PanX);
         g_upy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
         ToDrawView[0] = true;
      }
      if (g_CurView == vtXZ)
      {
         g_upx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
         g_upz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
         g_upy = g_downy;
         ToDrawView[2] = true;
      }
      if (g_CurView == vtZY)
      {
         g_upy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
         g_upz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
         g_upx = g_downx;
         ToDrawView[1] = true;
      }
      if (g_CurView == vtCamera)
      {
         g_upx = (float)(X - PanelWidth - VWidthL - VWidthR2);
         g_upy = (float)(Y - VHeightT2);
         ToDrawView[3] = true;
      }
		Update = true;
   }


   if (g_MainAction == atCreateVertex && Shift.m_Left)
   {
      if (g_CurView == vtXY)
      {
         g_downx = ((float)(Y - vheight2)/g_Zoom + PanX);
         g_downy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
      }
      if (g_CurView == vtXZ)
      {
         g_downx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
         g_downz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
      }
      if (g_CurView == vtZY)
      {
         g_downy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
         g_downz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
      }
		Update = true;
   }

   if (g_MainAction == atBuildingFace1 || g_MainAction == atBuildingFace2)
   {
      if (g_CurView == vtXY)
      {
         g_downx = ((float)(Y - vheight2)/g_Zoom + PanX);
         g_downy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
      }
      if (g_CurView == vtXZ)
      {
         g_downx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
         g_downz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
      }
      if (g_CurView == vtZY)
      {
         g_downy = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
         g_downz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
      }
      if (g_CurView == vtCamera)
      {
      	g_downx = (float)(X - vwidth - PanelWidth - vwidth2);
      	g_downy = (float)(Y - vheight2);
      }
		Update = true;
   }

   g_ox = X;
   g_oy = Y;

   {
   	if (g_MainAction == atNone)
      {
         if (X>PanelWidth && X<PanelWidth+vwidth && Y>0 && Y<vheight)
         {
            g_curx = ((float)(Y - vheight2)/g_Zoom + PanX);
            g_cury = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
         }
         if (X>PanelWidth+vwidth && X<PanelWidth+vwidth+vwidth && Y>vheight && Y<vheight+vheight)
         {
            g_curx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
            g_curz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
         }
         if (X>PanelWidth && X<PanelWidth+vwidth && Y>vheight && Y<vheight+vheight)
         {
            g_cury = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
            g_curz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
         }
      }
      else
      {
         if (g_CurView == vtXY)
         {
            g_curx = ((float)(Y - vheight2)/g_Zoom + PanX);
            g_cury = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
         }
         if (g_CurView == vtXZ)
         {
            g_curx = -((float)(X - vwidth2 - vwidth - PanelWidth)/g_Zoom - PanX);
            g_curz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
         }
         if (g_CurView == vtZY)
         {
            g_cury = ((float)(X - vwidth2 - PanelWidth)/g_Zoom + PanY);
            g_curz = -((float)(Y - vheight2 - vheight)/g_Zoom + PanZ);
         }
      }

   	if (g_MainAction == atMove)
         m_BottomToolDlg.SetXYZLabels(g_diffx, g_diffy, g_diffz);

   	if (g_MainAction == atRotate)
         m_BottomToolDlg.SetXYZLabels((float)(g_diffx*180.0/M_PI), (float)(g_diffy*180.0/M_PI), (float)(g_diffz*180.0/M_PI));

      if (g_MainAction == atScale)
         m_BottomToolDlg.SetXYZLabels((float)(100.0*g_diffx), (float)(100.0*g_diffy), (float)(100.0*g_diffz));

   	if (g_MainAction == atNone)
      {
/*      	XLabel->Font->Color = clWindowText;
      	YLabel->Font->Color = clWindowText;
      	ZLabel->Font->Color = clWindowText;*/
         if (X>PanelWidth && X<PanelWidth+vwidth && Y>0 && Y<vheight)
         {
            m_BottomToolDlg.SetXYZLabels(g_curx, g_cury, 0.0, XYZ_XY);
         }
         if (X>PanelWidth+vwidth && X<PanelWidth+vwidth+vwidth && Y>vheight && Y<vheight+vheight)
         {
            m_BottomToolDlg.SetXYZLabels(g_curx, 0.0, g_curz, XYZ_XZ);
         }
         if (X>PanelWidth && X<PanelWidth+vwidth && Y>vheight && Y<vheight+vheight)
         {
            m_BottomToolDlg.SetXYZLabels(0.0, g_cury, g_curz, XYZ_YZ);
         }
      }
      else
      {
/*      	XLabel->Font->Color = clRed;
      	YLabel->Font->Color = clRed;
      	ZLabel->Font->Color = clRed;*/
      }
   }

	if (Update)
   {
      SelCentre = g_CurMdl.GetSelectionCentre(g_MainSelectMode);

      // Only send a Paint Message if the camera is not playing
      if (g_Playing == false)
      {
         DoSync();
         SendMessage(WM_PAINT);
      }
   }
	
	CDialog::OnMouseMove(nFlags, point);
}

void CVCMDLDlg::DoCursors(CShiftState Shift)
{
   static unsigned int    LastCursor = NULL;
   unsigned int     targ = IDC_CURSORARROW;


	if (m_BottomToolDlg.m_PanViewsBut.GetCheck())
      targ = IDC_CURSORPAN;
   else
   {
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
      else
      if (m_CreateTab.m_CreateVertexBut.GetCheck())
         targ = IDC_CURSORVERTEX;
      else
      if (m_CreateTab.m_BuildFaceBut.GetCheck())
         targ = IDC_CURSORTRIANGLE;
   }

   HCURSOR  CurCurs;
   
   CurCurs = ::GetCursor();

   if (LastCursor != targ || CurCurs != SetCursor(AfxGetApp()->LoadCursor(targ)))
   {
      SetCursor(AfxGetApp()->LoadCursor(targ));
      LastCursor = targ;
   }
}

void CVCMDLDlg::DoSync()
{
   if (IsMenuChecked(ID_OPTIONS_SYNCSKINSELECTION))
   {
      for (int i=0;i<g_CurMdl.Head.num_tris;i++)
      {
        	g_CurMdl.Tris[i].SkinSelected = g_CurMdl.Tris[i].Selected;
      }
      m_SkinEditDlg.PostMessage(MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
   }

   m_BottomToolDlg.UpdateSelectionCounters();

}

void CVCMDLDlg::ToolButClick(HWND But)
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

   if (m_CurTool == m_ModifyTab.m_MoveBut.m_hWnd)
      m_TypeInDlg.SetCurrentTool(atMove, tiModel);
   else
   if (m_CurTool == m_ModifyTab.m_RotateBut.m_hWnd)
      m_TypeInDlg.SetCurrentTool(atRotate, tiModel);
   else
   if (m_CurTool == m_ModifyTab.m_ScaleBut.m_hWnd)
      m_TypeInDlg.SetCurrentTool(atScale, tiModel);
   else
      m_TypeInDlg.SetCurrentTool(atNone, tiModel);
   /*
	if (m_CurTool == MoveBut)
   {
		TypeInForm->EnableAll();
		TypeInForm->XFormGroup->Caption = "Move";
      TypeInForm->XEdit->Text  = "0";
      TypeInForm->YEdit->Text  = "0";
      TypeInForm->ZEdit->Text  = "0";
      TypeInForm->DisableOrigin();
   }
   else
	if (m_CurTool == RotateBut)
   {
		TypeInForm->EnableAll();
		TypeInForm->XFormGroup->Caption = "Rotate";
      TypeInForm->XEdit->Text  = "0";
      TypeInForm->YEdit->Text  = "0";
      TypeInForm->ZEdit->Text  = "0";
      TypeInForm->EnableOrigin();
   }
   else
	if (m_CurTool == ScaleBut)
   {
		TypeInForm->EnableAll();
		TypeInForm->XFormGroup->Caption = "Scale";
      TypeInForm->XEdit->Text  = "100";
      TypeInForm->YEdit->Text  = "100";
      TypeInForm->ZEdit->Text  = "100";
      TypeInForm->EnableOrigin();
   }
   else
   {
		TypeInForm->DisableAll();
   }
*/

   // find a way to bring the tab page that contains the given tool button to the front.
   

   g_MainAction = atNone;
   DoCursors(CShiftState());

   SendMessage(WM_PAINT);

}

void CVCMDLDlg::OnFileOpen() 
{
   KFileOpen();	
}

void CVCMDLDlg::OnTimer(unsigned int nIDEvent) 
{
   if (nIDEvent == 2000)   // the playback animation timer
   {
      if (g_Playing)    // is the camera playing
      {
         static int  clocktime;
         static int  oldclock;
         int         MinAnim, MaxAnim;
         CString     Str;

         MinAnim = m_ViewTab.m_FromAnimSpin.GetPos();
         MaxAnim = m_ViewTab.m_ToAnimSpin.GetPos();

         if (InterFrame == -8192)
         {
            oldclock = clocktime = clock();
            InterFrame  = 0;
         }
         else
         {
            oldclock = clocktime;
            clocktime = clock();
         }
         
         InterFrame += (float)m_ViewTab.m_FPSSpin.GetPos() * (clocktime - oldclock)/CLK_TCK;
         //   InterFrame += 0.2;
         
         if (InterFrame>1)
         {
            CString  Str;
            int      d = int(InterFrame);

            g_CurFrame += d;
            InterFrame -= d;
            g_CurFrame -= MinAnim;
            g_CurFrame = g_CurFrame % (1+MaxAnim-MinAnim);
            g_CurFrame += MinAnim;
         }
         if (!g_Drawing)
         {
            int cur = g_CurFrame, next = (cur+1);

            next -= MinAnim;
            next = next % (1+MaxAnim-MinAnim);
            next += MinAnim;
            
            
            TRealVertex **v;
            
            v = g_CurMdl.Frames;
            
            for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
            {
               g_CurMdl.Verts[n].x = (v[next][n].x-v[cur][n].x)*InterFrame + v[cur][n].x;
               g_CurMdl.Verts[n].y = (v[next][n].y-v[cur][n].y)*InterFrame + v[cur][n].y;
               g_CurMdl.Verts[n].z = (v[next][n].z-v[cur][n].z)*InterFrame + v[cur][n].z;
            }
            
            if (m_ViewTab.m_InterpolateBut.GetCheck())
            {
               g_CurMdl.SetCurrentFrame((float)cur+InterFrame,next);
               if (g_pRef)
                  g_pRef->SetCurrentFrame((float)cur+InterFrame,next);
            }
            else
            {
               g_CurMdl.SetCurrentFrame(g_CurFrame);
               if (g_pRef)
                  g_pRef->SetCurrentFrame(g_CurFrame);
            }
            
            CalcAllNormals();
            InitDrawInfo();

            if (IsMenuChecked(ID_OPTIONS_2DVIEW_PLAYCAMERAXY))
               DrawViewMatrix(Views[0], &g_CurMdl, &m_DrawInfo, vtXY);
            if (IsMenuChecked(ID_OPTIONS_2DVIEW_PLAYCAMERAZY))
               DrawViewMatrix(Views[1], &g_CurMdl, &m_DrawInfo, vtZY);
            if (IsMenuChecked(ID_OPTIONS_2DVIEW_PLAYCAMERAXZ))
               DrawViewMatrix(Views[2], &g_CurMdl, &m_DrawInfo, vtXZ);

         	Draw3DView(Views[3], &g_CurMdl, &m_DrawInfo);

            DrawAllViews();
         }

         m_BottomToolDlg.m_TimeScroll.SetScrollPos(g_CurFrame);
      }
   }

	CDialog::OnTimer(nIDEvent);
}

unsigned int CVCMDLDlg::IsMenuChecked(unsigned int IDItem)
{
   return GetMenu()->GetMenuState(IDItem, MF_CHECKED) & MF_CHECKED;
}

unsigned int CVCMDLDlg::ToggleMenuChecked(unsigned int IDItem)
{
   if (IsMenuChecked(IDItem))
      return GetMenu()->CheckMenuItem(IDItem, MF_UNCHECKED);
   else
      return GetMenu()->CheckMenuItem(IDItem, MF_CHECKED);
}

void CVCMDLDlg::OnOptions3DViewWireframe() 
{
   KOptions3DWireframe();
}

void CVCMDLDlg::OnOptions3dviewTexturemapped() 
{
   KOptions3DTexture();
}

void CVCMDLDlg::OnOptions3dviewTexturegourad() 
{
   KOptions3DTextureGourad();
}

void CVCMDLDlg::OnOptions3dviewGouradshaded() 
{
   KOptions3DGourad();	
}

void CVCMDLDlg::OnOptions3dviewFlatshaded() 
{
   KOptions3DFlat();
}

void CVCMDLDlg::OnOptionsShowgrid() 
{
   KOptionsShowGrid();
}

void CVCMDLDlg::OnEditUndo() 
{
   KEditUndo();
}

void CVCMDLDlg::OnEditRedo() 
{
   KEditRedo();
}

void CVCMDLDlg::OnEditSelectall() 
{
   KEditSelectAll();	
}

void CVCMDLDlg::OnEditSelectconnected() 
{
   KEditSelectConnected();
}

void CVCMDLDlg::OnEditSelectinverse() 
{
   KEditSelectInverse();
}

void CVCMDLDlg::OnEditSelectnone() 
{
   KEditSelectNone();
}

void CVCMDLDlg::OnEditSelecttouching() 
{
   KEditSelectTouching();
}

void CVCMDLDlg::OnClose() 
{
   KillTimer(m_PlayTimerID);
	SaveConfigToReg();
	
   CleanUpGlobals();

	CDialog::OnClose();
}
void CVCMDLDlg::LoadConfigFromReg()
{
   g_ProgSet.LoadFromRegistry("Software\\MartinTools\\Modeller\\");

//   SettingsForm->PrefEdit->Text = CFGFile;
//   SettingsForm->KeyboardEdit->Text = KeyFile;

   char drive[_MAX_DRIVE],dir[_MAX_DIR];
   char ExePath[256];

   ::GetModuleFileName(NULL, ExePath, 255);
   _splitpath(ExePath, drive,dir,NULL,NULL);

   ReadConfigFile((LPCTSTR)CString(CString(drive)+CString(dir)+g_ProgSet.m_CFGFile));

   InitActionList();

   g_ActionList.Sort();
   g_KeyList.m_Actions = &g_ActionList;

   g_KeyList.LoadFromFile(CString(drive) + CString(dir) + g_ProgSet.m_KEYFile);

   g_KeyList.UpdateAllMenus();


   return;

}

void CVCMDLDlg::SaveConfigToReg()
{
   g_ProgSet.SaveToRegistry("Software\\MartinTools\\Modeller\\");

   char drive[_MAX_DRIVE],dir[_MAX_DIR];
   char ExePath[256];

   ::GetModuleFileName(NULL, ExePath, 255);
   _splitpath(ExePath, drive,dir,NULL,NULL);

   WriteConfigFile((LPCTSTR)CString(CString(drive)+CString(dir)+g_ProgSet.m_CFGFile));
   g_KeyList.SaveToFile((LPCTSTR)CString(CString(drive)+CString(dir)+g_ProgSet.m_KEYFile));
}


void CVCMDLDlg::OnOptions2dviewDrawbackfaces() 
{
   KOptions2DBackface();
}

void CVCMDLDlg::OnOptions3dviewDrawbackfaces() 
{
   KOptions3DBackface();
}

void CVCMDLDlg::OnOptionsVerticesticks() 
{
   KOptionsVertTicks();
}

void CVCMDLDlg::OnOptionsShoworigin() 
{
   KOptionsShowOrigin();
}

void CVCMDLDlg::OnFileNew() 
{
   KFileNew();
}

void CVCMDLDlg::OnFileSaveas() 
{
   KFileSaveAs();	
}

void CVCMDLDlg::OnFileSave() 
{
   KFileSave();	
}

void CVCMDLDlg::OnFileImportImportAppendVerts() 
{
   KImportAppendVerts();
}

void CVCMDLDlg::OnFileImportImportAppendFrames() 
{
   KImportAppendFrames();	
}

void CVCMDLDlg::OnOptions2dviewPlaycameraxz() 
{
   KOptionsCameraPlayXZ();
}

void CVCMDLDlg::OnOptions2dviewPlaycamerazy() 
{
   KOptionsCameraPlayZY();
}

void CVCMDLDlg::OnOptions2dviewPlaycameraxy() 
{
   KOptionsCameraPlayXY();
}

void CVCMDLDlg::OnOptionsShownormalsNone() 
{
   KOptionsNormalsNone();
}

void CVCMDLDlg::OnOptionsShownormalsAll() 
{
   KOptionsNormalsAll();
	
}

void CVCMDLDlg::OnOptionsShownormalsSelectedfaces() 
{
   KOptionsNormalsSelect();
}

void CVCMDLDlg::OnFileExportExporttomd2() 
{
   KExportMD2();
   
}

void CVCMDLDlg::OnFileExportExporttomdl() 
{
   KExportMDL();
	
}

void CVCMDLDlg::OnFileExportExportframeto3ds() 
{
   KExportFrame3DS();
	
}

void CVCMDLDlg::OnFileExportExportframetoasc() 
{
   KExportFrameASC();
	
}

void CVCMDLDlg::OnFileExportExportmodelto3ds() 
{
   KExportModel3DS();
	
}

void CVCMDLDlg::OnFileExportExportmodeltoasc() 
{
   KExportModelASC();
	
}

void CVCMDLDlg::OnFileImportFramenamesfromtext() 
{
   KImportFrameNames();
	   
}

void CVCMDLDlg::OnOptionsDecreasegridsize() 
{
   KOptionsIncreaseGrid();

}

void CVCMDLDlg::OnOptionsIncreasegridsize() 
{
   KOptionsDecreaseGrid();
	
}

void CVCMDLDlg::OnOptionsZoomin() 
{
   KOptionsZoomIn();
	
}

void CVCMDLDlg::OnOptionsZoomout() 
{
   KOptionsZoomOut();
}

void CVCMDLDlg::OnOptionsConfigure() 
{
   KOptionsConfigure();	
}

void CVCMDLDlg::OnViewSkins() 
{
   m_SkinEditDlg.ShowWindow(SW_SHOW);
}

BOOL CVCMDLDlg::PreTranslateMessage(MSG* pMsg) 
{
   // Only let the main window handle key down messages
   if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
   {
      if (::IsChild(m_hWnd, pMsg->hwnd))
         return PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
   }

	return CDialog::PreTranslateMessage(pMsg);
}

void CVCMDLDlg::OnKeyUp(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) 
{
   SetFocus();
   if (nChar == VK_CONTROL)  // control key
   {
      m_KeyState.m_Ctrl = false;
      CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
      return;
      
   }
   if (nChar == VK_SHIFT)  // shift key
   {
      m_KeyState.m_Shift = false;
      CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
      return;
   }
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CVCMDLDlg::OnEditCopyselected() 
{
   KEditCopy();	
}

void CVCMDLDlg::OnEditPaste() 
{
   KEditPaste();
   
}

void CVCMDLDlg::OnEditPastetorange() 
{
   KEditPasteRange();
   
}

void CVCMDLDlg::OnEditChangeframename() 
{
   KEditRename();
}

void CVCMDLDlg::OnEditGotoframe() 
{
   KEditGotoFrame();
}

void CVCMDLDlg::OnEditAddnewframe() 
{
   KEditAddFrame();	
}

void CVCMDLDlg::OnEditMoveframes() 
{
   KEditMoveFrame();	
}

void CVCMDLDlg::OnEditDeletecurrentframe() 
{
   KEditDeleteFrameCurrent();
}

void CVCMDLDlg::OnEditDeleteframes() 
{
   KEditDeleteFrameRange();
}

void CVCMDLDlg::OnOptionsSyncskinselection() 
{
   ToggleMenuChecked(ID_OPTIONS_SYNCSKINSELECTION);
   m_SkinEditDlg.GetMenu()->CheckMenuItem(ID_OPTIONS_SYNC3DSELECTION, IsMenuChecked(ID_OPTIONS_SYNCSKINSELECTION));

   DoSync();
}

void CVCMDLDlg::OnOptionsGroundplaneposition() 
{
   KOptionsGridPos();
	
}

void CVCMDLDlg::OnToolsMemowindow() 
{
   KToolsMemoWindow();
}

void CVCMDLDlg::AddLineToConsole(const char *Text)
{
   m_ConsoleDlg.AddLineToConsole(Text);
}

void CVCMDLDlg::OnToolsOutputboundingbox() 
{
   KToolsOutputBoundingBox();
   
}

void CVCMDLDlg::OnViewSkinpainter() 
{
   KViewSkinPainter();	
}

void CVCMDLDlg::OnFileChoosereferencemodel() 
{
   KChooseReferenceModel();	
}

void CVCMDLDlg::OnFileClearreferencemodel() 
{
   KClearReferenceModel();	
}

void CVCMDLDlg::OnOptions3dviewShowreferencemodel() 
{
   KOptions3DShowReference();
   
}

void CVCMDLDlg::OnToolsNumerictypein() 
{
   KViewTypeIn();
}

void CVCMDLDlg::OnViewModelproperties() 
{
   KViewProperties();
}

void CAboutDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
//   lpMMI->ptMinTrackSize.x = 
   
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CVCMDLDlg::AddToMRUList(CString Str)
{
	int There = -1;
	for (int i=0;i<g_MRU_Count;i++)
   {
   	if (g_MRUList[i] == Str)
      {
      	There = i;
         break;
      }
   }
   if (There == -1)
   {
		for (int i=g_MRU_Count-1;i>0;i--)
      	g_MRUList[i] = g_MRUList[i-1];
   }
	else
		for (int i=There;i>0;i--)
   		g_MRUList[i] = g_MRUList[i-1];

   g_MRUList[0] = Str;

   UpdateMenuMRU();
}

void CVCMDLDlg::UpdateMenuMRU()
{
   CMenu    *FileMenu;
   int      InsertPos, n;

   FileMenu = GetMenu()->GetSubMenu(0);
   if (!FileMenu)
      return;

   FileMenu->DeleteMenu(ID_FILE_MRU_FILE1, MF_BYCOMMAND);
   FileMenu->DeleteMenu(ID_FILE_MRU_FILE2, MF_BYCOMMAND);
   FileMenu->DeleteMenu(ID_FILE_MRU_FILE3, MF_BYCOMMAND);
   FileMenu->DeleteMenu(ID_FILE_MRU_FILE4, MF_BYCOMMAND);
   FileMenu->DeleteMenu(ID_FILE_MRU_FILE5, MF_BYCOMMAND);

   InsertPos = FileMenu->GetMenuItemCount() - 2;

   for (n=g_MRU_Count-1;n>=0;n--)
   {
      if (g_MRUList[n] != "")
         FileMenu->InsertMenu(InsertPos, MF_BYPOSITION, ID_FILE_MRU_FILE1 + n, g_MRUList[n]);
   }

}

int CAboutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}
