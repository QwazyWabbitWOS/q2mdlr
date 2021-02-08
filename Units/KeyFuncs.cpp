#include "stdafx.h"

#include "resource.h"


#include "MenuItem.h"
#include "Keyboard.h"

#include "MdlConst.h"
#include "Matrix.h"
#include "MDLType.h"
#include "Mainfunc.h"

#include "Undo.h"
#include "Undo3D.h"
#include "UndoSkin.h"

#include "MDLGlobs.h"
#include "Colors.h"

#include "MDLMsgs.h"

#include "HelperFuncs.h"

// dialogs

#include "DlgGlobs.h"   // Global dialog variables.

#include "ExTypDlg.h"   // Export type for exporting a frame sequence to 3ds ro asc files
#include "KeyCfgPg.h"   // The keyoard config page dialog class.
#include "DirCfgPg.h"   // the path config page dialog class
#include "ClrCfgPg.h"   // the coor configuration dlg class
#include "UndoCfg.h"    // The Undo config page dialog class
#include "PasteDlg.h"   // Two dialogs for pasting stuff in the 3D edtior
#include "MiscDlgs.h"   // Misc dialogs for the renaming frames, goto frames etc.

#include "KeyFuncs.h"

// Standard C includes
#include <fcntl.h>
#include <io.h>
#include <stdlib.h>

//---------------------------------------------------------------------------

CString    LastModelDir, LastMeshDir;

void SetFormVars(CWnd *Main, CWnd *SkinEdit, CWnd *Paint, CWnd *TypeIn)
{
	g_MainDlg = (CVCMDLDlg*)Main;
	g_SkinEditDlg = (CSkinEditorDlg*)SkinEdit;
	g_PaintDlg = (CPaintDlg*)Paint;
	g_TypeInDlg = (CTypeInDlg*)TypeIn;
}

void InitActionList()
{
	CActionListType *AL;
	AL = &g_ActionList;

	AL->Add("File New", "Main", KFileNew, g_MainDlg, ID_FILE_NEW);
	AL->Add("File Open", "Main", KFileOpen, g_MainDlg, ID_FILE_OPEN);
	AL->Add("File Save", "Main", KFileSave, g_MainDlg, ID_FILE_SAVE);
	AL->Add("File Save As", "Main", KFileSaveAs, g_MainDlg, ID_FILE_SAVEAS);
	AL->Add("File Merge", "Main", KFileMerge, g_MainDlg, ID_FILE_MERGE);
	AL->Add("Export MD2", "Main", KExportMD2, g_MainDlg, ID_FILE_EXPORT_EXPORTTOMD2);
	AL->Add("Export MDL", "Main", KExportMDL, g_MainDlg, ID_FILE_EXPORT_EXPORTTOMDL);
	AL->Add("Export Frame to ASC", "Main", KExportFrameASC, g_MainDlg, ID_FILE_EXPORT_EXPORTFRAMETOASC);
	AL->Add("Export Model to ASC", "Main", KExportModelASC, g_MainDlg, ID_FILE_EXPORT_EXPORTMODELTOASC);
	AL->Add("Export Frame to 3DS", "Main", KExportFrame3DS, g_MainDlg, ID_FILE_EXPORT_EXPORTFRAMETO3DS);
	AL->Add("Export Model to 3DS", "Main", KExportModel3DS, g_MainDlg, ID_FILE_EXPORT_EXPORTMODELTO3DS);
	AL->Add("Import Append Vertices", "Main", KImportAppendVerts, g_MainDlg, ID_FILE_IMPORT_IMPORTAPPENDVERTS);
	AL->Add("Import Append Frames", "Main", KImportAppendFrames, g_MainDlg, ID_FILE_IMPORT_IMPORTAPPENDFRAMES);
	AL->Add("Import Frame Names", "Main", KImportFrameNames, g_MainDlg, ID_FILE_IMPORT_FRAMENAMESFROMTEXT);
	AL->Add("File Exit", "Main", KFileExit, g_MainDlg, ID_FILE_EXIT);
	AL->Add("Choose Reference Model", "Main", KChooseReferenceModel, g_MainDlg, ID_FILE_CHOOSEREFERENCEMODEL);
	AL->Add("Clear Reference Model", "Main", KClearReferenceModel, g_MainDlg, ID_FILE_CLEARREFERENCEMODEL);
	AL->Add("Undo", "Main", KEditUndo, g_MainDlg, ID_EDIT_UNDO);
	AL->Add("Redo", "Main", KEditRedo, g_MainDlg, ID_EDIT_REDO);
	AL->Add("Copy", "Main", KEditCopy, g_MainDlg, ID_EDIT_COPYSELECTED);
	AL->Add("Paste", "Main", KEditPaste, g_MainDlg, ID_EDIT_PASTE);
	AL->Add("Paste Range", "Main", KEditPasteRange, g_MainDlg, ID_EDIT_PASTETORANGE);
	AL->Add("Change Frame Name", "Main", KEditRename, g_MainDlg, ID_EDIT_CHANGEFRAMENAME);
	AL->Add("Goto Frame", "Main", KEditGotoFrame, g_MainDlg, ID_EDIT_GOTOFRAME);
	AL->Add("Add New Frame", "Main", KEditAddFrame, g_MainDlg, ID_EDIT_ADDNEWFRAME);
	AL->Add("Delete Current Frame", "Main", KEditDeleteFrameCurrent, g_MainDlg, ID_EDIT_DELETECURRENTFRAME);
	AL->Add("Delete Frame Range", "Main", KEditDeleteFrameRange, g_MainDlg, ID_EDIT_DELETEFRAMES);
	AL->Add("Move Frames", "Main", KEditMoveFrame, g_MainDlg, ID_EDIT_MOVEFRAMES);
	AL->Add("Select All", "Main", KEditSelectAll, g_MainDlg, ID_EDIT_SELECTALL);
	AL->Add("Select None", "Main", KEditSelectNone, g_MainDlg, ID_EDIT_SELECTNONE);
	AL->Add("Select Inverse", "Main", KEditSelectInverse, g_MainDlg, ID_EDIT_SELECTINVERSE);
	AL->Add("Select Connected", "Main", KEditSelectConnected, g_MainDlg, ID_EDIT_SELECTCONNECTED);
	AL->Add("Select Touching", "Main", KEditSelectTouching, g_MainDlg, ID_EDIT_SELECTTOUCHING);
	AL->Add("Skin Editor", "Main", KViewSkins, g_MainDlg, ID_VIEW_SKINS);
	AL->Add("Skin Painter", "Main", KViewSkinPainter, g_MainDlg, ID_VIEW_SKINPAINTER);
	AL->Add("Numeric Type-In", "Main", KViewTypeIn, g_MainDlg, ID_TOOLS_NUMERICTYPEIN);
	AL->Add("Model Properties", "Main", KViewProperties, g_MainDlg, ID_VIEW_MODELPROPERTIES);
	AL->Add("Show Vertices as Ticks", "Main", KOptionsVertTicks, g_MainDlg, ID_OPTIONS_VERTICESTICKS);
	AL->Add("Show/Hide Origin", "Main", KOptionsShowOrigin, g_MainDlg, ID_OPTIONS_SHOWORIGIN);
	AL->Add("Show/Hide Grid", "Main", KOptionsShowGrid, g_MainDlg, ID_OPTIONS_SHOWGRID);
	AL->Add("Change Position of the 3D Grid", "Main", KOptionsGridPos, g_MainDlg, ID_OPTIONS_GROUNDPLANEPOSITION);
	AL->Add("Draw No Normals", "Main", KOptionsNormalsNone, g_MainDlg, ID_OPTIONS_SHOWNORMALS_NONE);
	AL->Add("Draw Normals on Selected Faces", "Main", KOptionsNormalsSelect, g_MainDlg, ID_OPTIONS_SHOWNORMALS_SELECTEDFACES);
	AL->Add("Draw All Normals", "Main", KOptionsNormalsAll, g_MainDlg, ID_OPTIONS_SHOWNORMALS_ALL);
	AL->Add("Cycle Normal Display", "Main", KOptionsNormalsCycle);
	AL->Add("Draw Backfaces in 2D", "Main", KOptions2DBackface, g_MainDlg, ID_OPTIONS_2DVIEW_DRAWBACKFACES);
	AL->Add("Play Top View with Camera", "Main", KOptionsCameraPlayXY, g_MainDlg, ID_OPTIONS_2DVIEW_PLAYCAMERAXY);
	AL->Add("Play Front View with Camera", "Main", KOptionsCameraPlayZY, g_MainDlg, ID_OPTIONS_2DVIEW_PLAYCAMERAZY);
	AL->Add("Play Side View with Camera", "Main", KOptionsCameraPlayXZ, g_MainDlg, ID_OPTIONS_2DVIEW_PLAYCAMERAXZ);
	AL->Add("Draw Wireframe in 3D", "Main", KOptions3DWireframe, g_MainDlg, ID_OPTIONS_3DVIEW_WIREFRAME);
	AL->Add("Draw Flat in 3D", "Main", KOptions3DFlat, g_MainDlg, ID_OPTIONS_3DVIEW_FLATSHADED);
	AL->Add("Draw Gourad in 3D", "Main", KOptions3DGourad, g_MainDlg, ID_OPTIONS_3DVIEW_GOURADSHADED);
	AL->Add("Draw Texture Mapped in 3D", "Main", KOptions3DTexture, g_MainDlg, ID_OPTIONS_3DVIEW_TEXTUREMAPPED);
	AL->Add("Draw Texture Gourad in 3D", "Main", KOptions3DTextureGourad, g_MainDlg, ID_OPTIONS_3DVIEW_TEXTUREGOURAD);
	AL->Add("Draw 3D Mode Cycle", "Main", KOptions3DDrawCycle);
	AL->Add("Draw Backfaces in Camera", "Main", KOptions3DBackface, g_MainDlg, ID_OPTIONS_3DVIEW_DRAWBACKFACES);
	AL->Add("Sync Skin Selection", "Main", KOptionsSyncSkinSelect, g_MainDlg, ID_OPTIONS_SYNCSKINSELECTION);
	AL->Add("Increase Grid Size", "Main", KOptionsIncreaseGrid, g_MainDlg, ID_OPTIONS_INCREASEGRIDSIZE);
	AL->Add("Decrease Grid Size", "Main", KOptionsDecreaseGrid, g_MainDlg, ID_OPTIONS_DECREASEGRIDSIZE);
	AL->Add("Zoom In", "Main", KOptionsZoomIn, g_MainDlg, ID_OPTIONS_ZOOMIN);
	AL->Add("Zoom Out", "Main", KOptionsZoomOut, g_MainDlg, ID_OPTIONS_ZOOMOUT);
	AL->Add("Configure", "Main", KOptionsConfigure, g_MainDlg, ID_OPTIONS_CONFIGURE);
	AL->Add("Generate Header File", "Main", KToolsGenerateHeader, g_MainDlg, ID_TOOLS_GENERATEHEADERFILE);
	AL->Add("Show Memo Window", "Main", KToolsMemoWindow, g_MainDlg, ID_TOOLS_MEMOWINDOW);
	AL->Add("Output Bounding Box", "Main", KToolsOutputBoundingBox, g_MainDlg, ID_TOOLS_OUTPUTBOUNDINGBOX);
	AL->Add("Help Contents", "Main", KHelpContents, g_MainDlg, ID_HELP_CONTENTS);
	AL->Add("Help Keyword Search", "Main", KHelpKeyword, g_MainDlg, ID_HELP_KEYWORDSEARCH);
	AL->Add("Help About", "Main", KHelpAbout, g_MainDlg, ID_HELP_ABOUT);
	AL->Add("Select Mode Vertex", "Main", KModeVertex);
	AL->Add("Select Mode Face", "Main", KModeFace);
	AL->Add("Select Mode Bones", "Main", KModeBones);
	AL->Add("Select Mode Toggle Object", "Main", KModeObject);
	AL->Add("Cycle Select Mode", "Main", KModeCycle);
	AL->Add("Restrict X", "Main", KControlLimitX);
	AL->Add("Restrict Y", "Main", KControlLimitY);
	AL->Add("Restrict Z", "Main", KControlLimitZ);
	AL->Add("Pan Tool", "Main", KToolPan);
	AL->Add("Select Tool", "Main", KToolSelect);
	AL->Add("Move Tool", "Main", KToolMove);
	AL->Add("Rotate Tool", "Main", KToolRotate);
	AL->Add("Scale Tool", "Main", KToolScale);
	AL->Add("Cycle Tools", "Main", KToolCycle);
	AL->Add("Mirror", "Main", KToolMirror);
	AL->Add("Delete Selected", "Main", KToolDelete);
	AL->Add("Flip Normals", "Main", KToolFlipNormal);
	AL->Add("Turn Edge", "Main", KToolTurn);
	AL->Add("Subdivide", "Main", KToolSubdivide);
	AL->Add("Extrude", "Main", KToolExtrude);
	AL->Add("Weld Verts", "Main", KToolWeld);

	AL->Add("Next Frame", "Main", KControlNextFrame);
	AL->Add("Previous Frame", "Main", KControlPrevFrame);
	AL->Add("Fit to All", "Main", KControlFitAll);
	AL->Add("Fit to Selected", "Main", KControlFitSelected);
	AL->Add("Play Camera", "Main", KControlPlayCamera);
	AL->Add("Hide Selected", "Main", KControlHideSelected);
	AL->Add("Hide Unselected", "Main", KControlHideUnSelected);
	AL->Add("Unide All", "Main", KControlUnhide);
	AL->Add("Create Vertex", "Main", KCreateVertex);
	AL->Add("Create Face", "Main", KCreateFace);
	AL->Add("Create Fan", "Main", KCreateFan);
	AL->Add("Create Strip", "Main", KCreateStrip);
	AL->Add("Toggle Frame Interpolation", "Main", KMainInterpolationToggle);
	AL->Add("Toggle Effect Frame Range", "Main", KEffectFrameRangeToggle);

	AL->Add("Import Skin Image", "Skin", KImportSkinImage, g_SkinEditDlg, ID_FILE_IMPORT_SKINIMAGE);
	AL->Add("Import Skin Mesh", "Skin", KImportSkinMesh, g_SkinEditDlg, ID_FILE_IMPORT_SKINMESHFROMASC);
	AL->Add("Export Skin to BMP", "Skin", KExportImageBMP, g_SkinEditDlg, ID_FILE_EXPORT_CURRENTIMAGE_BMP);
	AL->Add("Export Skin to PCX", "Skin", KExportImagePCX, g_SkinEditDlg, ID_FILE_EXPORT_CURRENTIMAGE_PCX);
	AL->Add("Export Skin Mesh", "Skin", KExportSkinMesh, g_SkinEditDlg, ID_FILE_EXPORT_SKINMESHTOASC);
	AL->Add("Exit Skin Editor", "Skin", KSkinEditorExit, g_SkinEditDlg, ID_FILE_CLOSE);
	AL->Add("Undo", "Skin", KSkinUndo, g_SkinEditDlg, ID_EDIT_UNDO);
	AL->Add("Redo", "Skin", KSkinRedo, g_SkinEditDlg, ID_EDIT_REDO);
	AL->Add("Copy", "Skin", KSkinCopy, g_SkinEditDlg, ID_EDIT_COPY);
	AL->Add("Paste", "Skin", KSkinPaste, g_SkinEditDlg, ID_EDIT_PASTE);
	AL->Add("Get Position From Model", "Skin", KGetPosition, g_SkinEditDlg, ID_EDIT_GETPOSITIONFROMMODEL);
	AL->Add("Add New Skin", "Skin", KAddSkin, g_SkinEditDlg, ID_EDIT_ADDNEWSKIN);
	AL->Add("Delete Skin", "Skin", KDeleteSkin, g_SkinEditDlg, ID_EDIT_DELETECURRENTSKIN);
	AL->Add("Clear Skin", "Skin", KClearSkin, g_SkinEditDlg, ID_EDIT_CLEARCURRENTSKIN);
	AL->Add("Resize Skins", "Skin", KResizeSkin, g_SkinEditDlg, ID_EDIT_RESIZESKIN);
	AL->Add("Select All", "Skin", KSkinSelectAll, g_SkinEditDlg, ID_EDIT_SELECTALL);
	AL->Add("Select None", "Skin", KSkinSelectNone, g_SkinEditDlg, ID_EDIT_SELECTNONE);
	AL->Add("Select Inverse", "Skin", KSkinSelectInverse, g_SkinEditDlg, ID_EDIT_SELECTINVERSE);
	AL->Add("Select Connected", "Skin", KSkinSelectConnected, g_SkinEditDlg, ID_EDIT_SELECTCONNECTED);
	AL->Add("Select Touching", "Skin", KSkinSelectTouching, g_SkinEditDlg, ID_EDIT_SELECTTOUCHING);
	AL->Add("Skin Painter", "Skin", KViewSkinPainter, g_MainDlg, ID_VIEW_SKINPAINTER);
	AL->Add("3D Editor", "Skin", KView3DEditor, g_MainDlg, ID_VIEW_3DEDITOR);
	AL->Add("Draw No Lines", "Skin", KSkinLinesNone, g_SkinEditDlg, ID_OPTIONS_LINES_DONTDRAW);
	AL->Add("Draw Normal Lines", "Skin", KSkinLinesNormal, g_SkinEditDlg, ID_OPTIONS_LINES_NORMAL);
	AL->Add("Cycle Line Drawing", "Skin", KSkinLinesCycle);
	AL->Add("Draw No Vertices", "Skin", KSkinVertsNone, g_SkinEditDlg, ID_OPTIONS_VERTICES_NONE);
	AL->Add("Draw Vertices as Dots", "Skin", KSkinVertsDots, g_SkinEditDlg, ID_OPTIONS_VERTICES_DOTS);
	AL->Add("Draw Vertices as Ticks", "Skin", KSkinVertsTicks, g_SkinEditDlg, ID_OPTIONS_VERTICES_TICKS);
	AL->Add("Cycle Vertex Drawing", "Skin", KSkinVertsCycle);
	AL->Add("Sync 3D Selection", "Skin", KSkinSyncSelection, g_SkinEditDlg, ID_OPTIONS_SYNC3DSELECTION);
	AL->Add("Zoom In", "Skin", KSkinZoomIn, g_SkinEditDlg, ID_OPTIONS_ZOOMIN);
	AL->Add("Zoom Out", "Skin", KSkinZoomOut, g_SkinEditDlg, ID_OPTIONS_ZOOMOUT);
	AL->Add("Keep Window On Top", "Skin", KSkinWindowOnTop, g_SkinEditDlg, ID_OPTIONS_KEEPWINDOWONTOP);
	AL->Add("Cycle Tools", "Skin", KSkinToolCycle);
	AL->Add("Select Tool", "Skin", KSkinToolSelect);
	AL->Add("Move Tool", "Skin", KSkinToolMove);
	AL->Add("Rotate Tool", "Skin", KSkinToolRotate);
	AL->Add("Scale Tool", "Skin", KSkinToolScale);
	AL->Add("Mirror", "Skin", KSkinToolMirror);
	AL->Add("Weld Verts", "Skin", KSkinToolWeldVerts);
	AL->Add("Detach", "Skin", KSkinToolDetach);
	AL->Add("Delete", "Skin", KSkinToolDelete);
	AL->Add("Next Skin", "Skin", KSkinNext);
	AL->Add("Previous Skin", "Skin", KSkinPrev);
	AL->Add("Select Vertex Mode", "Skin", KSkinVertexMode);
	AL->Add("Select Face Mode", "Skin", KSkinFaceMode);
	AL->Add("Select Active Area Mode", "Skin", KSkinActiveAreaMode);
	AL->Add("Cylce Selection Mode", "Skin", KSkinCycleMode);
	AL->Add("Update Skin Usage Dislay", "Skin", KSkinUpdateSkinUsage);

	AL->Add("Toggle Restrict X", "Skin", KSkinToggleX);
	AL->Add("Toggle Restrict Y", "Skin", KSkinToggleY);


	// Skin Painter Functions
	AL->Add("Pan Tool", "Paint", KPaintToolPan);
	AL->Add("Paint Tool", "Paint", KPaintToolPaint);
	AL->Add("Spray Tool", "Paint", KPaintToolSpray);
	AL->Add("Fill Tool", "Paint", KPaintToolFill);
	AL->Add("Swap Foreground/Background", "Paint", KPaintSwapFBColors);
	AL->Add("Undo", "Paint", KPaintUndo, g_PaintDlg, ID_EDIT_UNDO);
	AL->Add("Redo", "Paint", KPaintRedo, g_PaintDlg, ID_EDIT_REDO);

}

void DestroyActionList()
{
}


// file menu
int KFileNew()
{
	if (g_ChangesMade)
	{
		int MBRes;
		MBRes = MessageBox(NULL, "Do you wish to save your changes?", "Save Changes", MB_YESNOCANCEL);

		if (MBRes == IDCANCEL)
			return 0;
		if (MBRes == IDYES)
			if (KFileSaveAs() == 0)
				return 0;
	}

	g_CurMdl.DeallocateAll();

	g_CurMdl.FrameInfo = new TFrameInfo[1];
	g_CurMdl.Head.num_frames = 1;
	g_CurMdl.FrameInfo[0].Name[0] = 'F';
	g_CurMdl.FrameInfo[0].Name[1] = 'r';
	g_CurMdl.FrameInfo[0].Name[2] = 'a';
	g_CurMdl.FrameInfo[0].Name[3] = 'm';
	g_CurMdl.FrameInfo[0].Name[4] = 'e';
	g_CurMdl.FrameInfo[0].Name[5] = ' ';
	g_CurMdl.FrameInfo[0].Name[6] = '1';
	for (int i = 7; i < 15; i++)
		g_CurMdl.FrameInfo[0].Name[i] = '\x0';
	g_CurMdl.Head.num_skins = 1;
	g_CurMdl.Head.skinwidth = 320;
	g_CurMdl.Head.skinheight = 200;
	g_CurMdl.Head.num_xyz = 0;
	g_CurMdl.Head.num_st = 1;
	g_CurMdl.SkinVerts = new TRealSkinVertex[1];
	g_CurMdl.SkinVerts[0].s = 0;
	g_CurMdl.SkinVerts[0].t = 0;

	g_CurMdl.Head.num_tris = 0;
	g_CurMdl.Skins[0] = new unsigned char[g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight];
	memset(g_CurMdl.Skins[0], 0, g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight);
	g_CurMdl.Empty = false;
	g_CurMdl.Head.id = 0x4F504449;
	g_CurMdl.Head.version = 6;


	if (g_SkinEditDlg)
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATESKIN, 0, 0);

	g_MainUndo.ClearAllNodes();
	g_MainRedo.ClearAllNodes();
	g_SkinUndo.ClearAllNodes();
	g_SkinRedo.ClearAllNodes();

	g_CurFrame = 0;

	if (g_MainDlg)

		g_CurFileName = "Untitled";

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_FILENAMECHANGED, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	}

	if (g_SkinEditDlg)
		SendMessage(g_SkinEditDlg->m_hWnd, MM_PAINT, 0, 0);

	return 1;
}

int KHelperOpenFile(CString Str)
{
	g_MainDlg->AddToMRUList(Str);
	//      AfxGetApp()->AddToRecentFileList(Str);

	HCURSOR     OldCursor;
	OldCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_STOPPLAY, 0, 0);

	if (!g_CurMdl.LoadFromBestGuess(Str, false))
	{
		::MessageBox(NULL, "There was an error loading the file", "Error", MB_OK);
		return 0;
	}

	g_CurFrame = 0;

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_CHECKSPINNERS, 0, 0);

	g_CurMdl.SetCurrentFrame(g_CurFrame);

	CalcAllNormals();
	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);

	//      g_MainDlg->AddToMRUList(FName);

	g_ChangesMade = 0;
	if (g_SkinEditDlg)
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATESKIN, 0, 0);
	Views[3].SetTexSize(g_CurMdl.Head.skinwidth, g_CurMdl.Head.skinheight);



	::SetCursor(OldCursor);
	{
		char Drive[4], Dir[265];
		_splitpath((LPCTSTR)Str, Drive, Dir, NULL, NULL);
		//         LastModelDir = CString(Drive) + CString(Dir);
	}

	g_CurFileName = Str;

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_FILENAMECHANGED, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_CALCVIEWPORTS, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_RESETSPINNERS, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_CHECKSPINNERS, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);
	}
	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATESKIN, 0, 0);
	}

	KControlFitAll();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	if (g_SkinEditDlg)
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);

	return 1;
}

int KFileOpen()
{
	if (g_ChangesMade)
	{
		int MBRes;
		MBRes = MessageBox(NULL, "Do you wish to save your changes?", "Save Changes", MB_YESNOCANCEL);

		if (MBRes == IDCANCEL)
			return 0;
		if (MBRes == IDYES)
			if (KFileSaveAs() == 0)
				return 0;
	}

	CFileDialog Dlg(TRUE, "", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Quake Models (*.MD2, *.AQM, *.MDL)|*.MD2;*.AQM;*.MDL||");

	if (LastModelDir == "")
		LastModelDir = QuakeDataDir;

	Dlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastModelDir;

	if (Dlg.DoModal() == IDOK)
	{
		// Add the file to the mru list
		KHelperOpenFile(Dlg.GetPathName());
	}


	return 1;
}

int KFileSave()
{
	KFileSaveAs();
	return 1;
}

int KFileSaveAs()
{
	CFileDialog SaveDlg(FALSE, "AQM", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "AQM File (*.AQM)|*.AQM||");

	if (LastModelDir == "")
		LastModelDir = QuakeDataDir;
	//	SaveDlg->InitialDir = LastModelDir;

	SaveDlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastModelDir;

	if (SaveDlg.DoModal() == IDCANCEL)
		return 0;

	{
		char Drive[4], Dir[265];
		_splitpath((LPCTSTR)SaveDlg.GetPathName(), Drive, Dir, NULL, NULL);
		LastModelDir = CString(Drive) + CString(Dir);
	}

	g_CurFileName = SaveDlg.GetPathName();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_FILENAMECHANGED, 0, 0);

	//	AddToMRUList(SaveDlg->FileName);

	//   if (PlayTimer->Enabled)
	//   	PlayBut->Click();

	if (g_Playing)
		for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
		{
			g_CurMdl.Frames[g_CurFrame][n].x = g_CurMdl.Verts[n].x;
			g_CurMdl.Frames[g_CurFrame][n].y = g_CurMdl.Verts[n].y;
			g_CurMdl.Frames[g_CurFrame][n].z = g_CurMdl.Verts[n].z;
		}

	g_CurMdl.SaveToAQM(g_CurFileName);
	g_ChangesMade = 0;

	return 1;
}

int KFileMerge()
{
	if (g_MainDlg)
		g_MainDlg->SendMessage(WM_COMMAND, ID_FILE_MERGE, 0);
	return 1;
}

int KExportMD2()
{
	HCURSOR     OldCursor;

	if (g_Playing) // need to stop animation playing to save a file.
	{
		g_MainDlg->SendMessage(MM_STOPPLAY);
	}

	if (LastMeshDir == "")
		LastMeshDir = QuakeDataDir;

	CFileDialog SaveDlg(FALSE, "MD2", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Quake 2 Model Files (*.MD2)|*.MD2||");

	SaveDlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastMeshDir;

	if (SaveDlg.DoModal() == IDCANCEL)
		return 0;

	OldCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

	{
		char Drive[4], Dir[265];
		_splitpath(SaveDlg.GetPathName(), Drive, Dir, NULL, NULL);
		LastMeshDir = CString(Drive) + CString(Dir);
	}

	g_CurMdl.UpdateFrame(g_CurFrame);

	g_CurMdl.SaveToMD2(SaveDlg.GetPathName());

	//	AddToMRUList(SaveDlg->FileName);

	OldCursor = ::SetCursor(OldCursor);

	return 1;
}

int KExportMDL()
{
	HCURSOR     OldCursor;

	if (g_Playing) // need to stop animation playing to save a file.
	{
		g_MainDlg->SendMessage(MM_STOPPLAY);
	}

	if (LastMeshDir == "")
		LastMeshDir = QuakeDataDir;

	CFileDialog SaveDlg(FALSE, "MDL", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Quake 1 Model Files (*.MDL)|*.MDL||");

	SaveDlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastMeshDir;

	if (SaveDlg.DoModal() == IDCANCEL)
		return 0;

	OldCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

	{
		char Drive[4], Dir[265];
		_splitpath(SaveDlg.GetPathName(), Drive, Dir, NULL, NULL);
		LastMeshDir = CString(Drive) + CString(Dir);
	}

	g_CurMdl.UpdateFrame(g_CurFrame);

	g_CurMdl.SaveToMD2(SaveDlg.GetPathName());

	//	AddToMRUList(SaveDlg->FileName);

	OldCursor = ::SetCursor(OldCursor);

	return 1;
}

int KExportFrameASC()
{
	HCURSOR     OldCursor;

	if (g_Playing) // need to stop animation playing to save a file.
	{
		g_MainDlg->SendMessage(MM_STOPPLAY);
	}

	if (LastMeshDir == "")
		LastMeshDir = QuakeDataDir;

	CFileDialog SaveDlg(FALSE, "ASC", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Ascii Mesh Format (*.ASC)|*.ASC||");

	SaveDlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastMeshDir;

	if (SaveDlg.DoModal() == IDCANCEL)
		return 0;

	OldCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

	{
		char Drive[4], Dir[265];
		_splitpath(SaveDlg.GetPathName(), Drive, Dir, NULL, NULL);
		LastMeshDir = CString(Drive) + CString(Dir);
	}

	g_CurMdl.UpdateFrame(g_CurFrame);

	g_CurMdl.ExportFrameToASC(SaveDlg.GetPathName(), g_CurFrame);

	//	AddToMRUList(SaveDlg->FileName);

	OldCursor = ::SetCursor(OldCursor);

	return 1;
}

int KExportModelASC()
{
	HCURSOR     OldCursor;
	UINT        NamingType;

	if (g_Playing) // need to stop animation playing to save a file.
	{
		g_MainDlg->SendMessage(MM_STOPPLAY);
	}

	if (LastMeshDir == "")
		LastMeshDir = QuakeDataDir;

	{
		CExportNamingDlg  *TypeDlg;

		TypeDlg = new CExportNamingDlg;
		NamingType = TypeDlg->GetExportFilename();

		delete TypeDlg;

		if (NamingType == MOD_EXP_CANCEL)
			return 0;
	}


	CFileDialog SaveDlg(FALSE, "ASC", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Ascii Model Files (*.ASC)|*.ASC||");

	SaveDlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastMeshDir;

	if (SaveDlg.DoModal() == IDCANCEL)
		return 0;

	OldCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

	{
		char Drive[4], Dir[265];
		_splitpath(SaveDlg.GetPathName(), Drive, Dir, NULL, NULL);
		LastMeshDir = CString(Drive) + CString(Dir);
	}

	int maxdigits;
	if (g_CurMdl.Head.num_frames < 10000)
		maxdigits = 4;
	if (g_CurMdl.Head.num_frames < 1000)
		maxdigits = 3;
	if (g_CurMdl.Head.num_frames < 100)
		maxdigits = 2;
	if (g_CurMdl.Head.num_frames < 10)
		maxdigits = 1;


	CString  Filename;
	{
		char drive[_MAX_DRIVE], dir[_MAX_DIR], name[_MAX_FNAME], ext[_MAX_EXT];

		_splitpath(SaveDlg.GetPathName(), drive, dir, name, ext);

		if (NamingType == MOD_EXP_NUMBER)
		{
			Filename = CString(drive) + CString(dir) + CString(name);
		}
		else
		{
			Filename = CString(drive) + CString(dir);
		}
	}

	for (int f = 0; f < g_CurMdl.Head.num_frames; f++)
	{
		CString zeros, framenum;

		framenum = (char)f;

		if (f < 1000 && maxdigits >= 4)
			zeros += "0";
		if (f < 100 && maxdigits >= 3)
			zeros += "0";
		if (f < 10 && maxdigits >= 2)
			zeros += "0";

		char num[16];
		itoa(f, num, 10);
		framenum = zeros + CString(num);

		if (NamingType == MOD_EXP_NUMBER)
			g_CurMdl.ExportFrameToASC(Filename + framenum + CString(".ASC"), f);
		else
			g_CurMdl.ExportFrameToASC(Filename + CString(g_CurMdl.FrameInfo[f].Name) + CString(".ASC"), f);
	}

	OldCursor = ::SetCursor(OldCursor);

	return 1;
}

int KExportFrame3DS()
{
	HCURSOR     OldCursor;

	if (g_Playing) // need to stop animation playing to save a file.
	{
		g_MainDlg->SendMessage(MM_STOPPLAY);
	}

	if (LastMeshDir == "")
		LastMeshDir = QuakeDataDir;

	CFileDialog SaveDlg(FALSE, "3DS", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "3D Studio R4 files (*.3DS)|*.3DS||");

	SaveDlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastMeshDir;

	if (SaveDlg.DoModal() == IDCANCEL)
		return 0;

	OldCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

	{
		char Drive[4], Dir[265];
		_splitpath(SaveDlg.GetPathName(), Drive, Dir, NULL, NULL);
		LastMeshDir = CString(Drive) + CString(Dir);
	}

	g_CurMdl.UpdateFrame(g_CurFrame);

	g_CurMdl.ExportFrameTo3DS(SaveDlg.GetPathName(), g_CurFrame);

	//	AddToMRUList(SaveDlg->FileName);

	OldCursor = ::SetCursor(OldCursor);

	return 1;
	return 1;
}

int KExportModel3DS()
{
	HCURSOR     OldCursor;
	UINT        NamingType;

	if (g_Playing) // need to stop animation playing to save a file.
	{
		g_MainDlg->SendMessage(MM_STOPPLAY);
	}

	if (LastMeshDir == "")
		LastMeshDir = QuakeDataDir;

	{
		CExportNamingDlg  *TypeDlg;

		TypeDlg = new CExportNamingDlg;
		NamingType = TypeDlg->GetExportFilename();

		delete TypeDlg;

		if (NamingType == MOD_EXP_CANCEL)
			return 0;
	}


	CFileDialog SaveDlg(FALSE, "ASC", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Ascii Model Files (*.ASC)|*.ASC||");

	SaveDlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastMeshDir;

	if (SaveDlg.DoModal() == IDCANCEL)
		return 0;

	OldCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

	{
		char Drive[4], Dir[265];
		_splitpath(SaveDlg.GetPathName(), Drive, Dir, NULL, NULL);
		LastMeshDir = CString(Drive) + CString(Dir);
	}

	int maxdigits;
	if (g_CurMdl.Head.num_frames < 10000)
		maxdigits = 4;
	if (g_CurMdl.Head.num_frames < 1000)
		maxdigits = 3;
	if (g_CurMdl.Head.num_frames < 100)
		maxdigits = 2;
	if (g_CurMdl.Head.num_frames < 10)
		maxdigits = 1;


	CString  Filename;
	{
		char drive[_MAX_DRIVE], dir[_MAX_DIR], name[_MAX_FNAME], ext[_MAX_EXT];

		_splitpath(SaveDlg.GetPathName(), drive, dir, name, ext);

		if (NamingType == MOD_EXP_NUMBER)
		{
			Filename = CString(drive) + CString(dir) + CString(name);
		}
		else
		{
			Filename = CString(drive) + CString(dir);
		}
	}

	for (int f = 0; f < g_CurMdl.Head.num_frames; f++)
	{
		CString zeros, framenum;

		framenum = (char)f;

		if (f < 1000 && maxdigits >= 4)
			zeros += "0";
		if (f < 100 && maxdigits >= 3)
			zeros += "0";
		if (f < 10 && maxdigits >= 2)
			zeros += "0";

		char num[16];
		itoa(f, num, 10);
		framenum = zeros + CString(num);

		if (NamingType == MOD_EXP_NUMBER)
			g_CurMdl.ExportFrameTo3DS(Filename + framenum + CString(".3DS"), f);
		else
			g_CurMdl.ExportFrameTo3DS(Filename + CString(g_CurMdl.FrameInfo[f].Name) + CString(".3DS"), f);
	}

	OldCursor = ::SetCursor(OldCursor);

	return 1;
}


int KImportAppendVerts()
{
	CFileDialog Dlg(TRUE, "ASC", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Mesh Models (*.ASC, *.3DS)|*.ASC;*.3DS||");

	if (LastModelDir == "")
		LastModelDir = QuakeDataDir;

	Dlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastModelDir;

	if (Dlg.DoModal() == IDCANCEL)
		return 0;

	CString extstr;
	{
		char Drive[4], Dir[265];
		_splitpath(Dlg.GetPathName(), Drive, Dir, NULL, NULL);
		LastMeshDir = CString(Drive) + CString(Dir);
	}

	{
		char ext[_MAX_EXT];
		_splitpath(Dlg.GetPathName(), NULL, NULL, NULL, ext);
		extstr = ext;
	}

	if (extstr.CompareNoCase(".ASC") == 0)
		g_CurMdl.LoadFromASC(Dlg.GetPathName(), TImportMode(imAppendVerts));
	if (extstr.CompareNoCase(".3DS") == 0)
		g_CurMdl.LoadFrom3DS(Dlg.GetPathName(), TImportMode(imAppendVerts));
	if (extstr.CompareNoCase(".SQF") == 0)
		g_CurMdl.LoadFromSQF(Dlg.GetPathName(), TImportMode(imAppendVerts));
	if (extstr.CompareNoCase(".LWO") == 0)
		g_CurMdl.LoadFromLWO(Dlg.GetPathName(), TImportMode(imAppendVerts));

	g_ChangesMade = 1;

	CalcAllNormals();

	KControlFitAll();
	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KImportAppendFrames()
{
	HCURSOR     OldCursor;
	CFileDialog Dlg(TRUE, "ASC", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, "Mesh Models (*.ASC, *.3DS)|*.ASC;*.3DS||");

	if (LastModelDir == "")
		LastModelDir = QuakeDataDir;

	Dlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastModelDir;

	if (Dlg.DoModal() == IDCANCEL)
		return 0;

	{
		char Drive[4], Dir[265];
		_splitpath(Dlg.GetPathName(), Drive, Dir, NULL, NULL);
		LastMeshDir = CString(Drive) + CString(Dir);
	}

	OldCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));


	POSITION    FileNamePos;
	FileNamePos = Dlg.GetStartPosition();

	while (FileNamePos)
	{
		CString extstr, fn;
		fn = Dlg.GetNextPathName(FileNamePos);

		{
			char ext[_MAX_EXT];
			_splitpath(fn, NULL, NULL, NULL, ext);
			extstr = ext;
		}

		if (extstr.CompareNoCase(".ASC") == 0)
			g_CurMdl.LoadFromASC(fn, TImportMode(imAppendFrame));
		if (extstr.CompareNoCase(".3DS") == 0)
			g_CurMdl.LoadFrom3DS(fn, TImportMode(imAppendFrame));
		if (extstr.CompareNoCase(".SQF") == 0)
			g_CurMdl.LoadFromSQF(fn, TImportMode(imAppendFrame));
		if (extstr.CompareNoCase(".LWO") == 0)
			g_CurMdl.LoadFromLWO(fn, TImportMode(imAppendFrame));
	}

	::SetCursor(OldCursor);


	g_ChangesMade = 1;

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_CHECKSPINNERS, MMC_UPDATE_ALL, 0);

	CalcAllNormals();
	KControlFitAll();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KImportFrameNames()
{
	if (LastMeshDir == "")
		LastMeshDir = QuakeDataDir;

	CFileDialog Dlg(TRUE, "TXT", NULL, OFN_HIDEREADONLY, "Text Files (*.TXT)|*.TXT||");

	Dlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastMeshDir;

	if (Dlg.DoModal() == IDCANCEL)
		return 0;

	int handle;

	handle = open(Dlg.GetFileName(), O_BINARY | O_RDONLY);


	if (handle == -1)
	{
		MessageBox(NULL, "Could not open File. Aborting Import.", "Error", MB_OK);
		return 0;
	}

	char *buf, *b, tname[16];
	bool doing = false;

	int p, l, frame = 0, cc = 0;

	l = filelength(handle);

	buf = new char[l];

	read(handle, buf, l);
	close(handle);

	p = 0;

	b = buf;

	while (p < l && frame < g_CurMdl.Head.num_frames)
	{
		if (doing == false)
		{
			if ((*b) >= 32)
			{
				doing = true;
				cc = 0;
			}
			else
			{
				b++;
				p++;
			}

		}

		if (doing == true)
		{
			if ((*b) >= 32)
			{
				tname[cc] = *b;
				p++;
				cc++;
				b++;
			}
			else
			{
				if (cc < 15)
					tname[cc] = '\x0';
				memcpy(g_CurMdl.FrameInfo[frame].Name, tname, 16);
				memset(tname, 0, 16);
				p++;
				b++;
				cc = 0;
				frame++;
				doing = false;
			}
		}
	}


	delete[] buf;

	return 1;
}

int KFileExit()
{
	//   g_MainDlg->Exit->Click();
	return 1;
}


int KChooseReferenceModel()
{
	if (LastModelDir == "")
		LastModelDir = QuakeDataDir;

	CFileDialog Dlg(TRUE, "MD2", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Quake Models (*.MD2, *.AQM, *.MDL)|*.MD2;*.AQM;*.MDL||");

	Dlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastModelDir;

	if (Dlg.DoModal() == IDCANCEL)
	{
		return 0;
	}

	if (g_pRef)
		delete g_pRef;

	g_pRef = new TCompleteModel;
	g_pRef->LoadFromBestGuess(Dlg.GetPathName(), false);

	g_pRef->SetCurrentFrame(g_CurFrame);
	g_pRef->CalcNormals();


	CalcAllNormals();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	if (g_SkinEditDlg)
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);

	return 1;
}

int KClearReferenceModel()
{
	//   g_MainDlg->ClearReferenceModel->Click();
	return 1;
}


// Edit Menu
int KEditUndo()
{
	g_CurMdl.UpdateFrame(g_CurFrame);
	g_MainUndo.UndoLast();
	g_CurMdl.SetCurrentFrame(g_CurFrame);
	if (g_pRef)
		g_pRef->SetCurrentFrame(g_CurFrame);

	CalcAllNormals();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KEditRedo()
{
	g_CurMdl.UpdateFrame(g_CurFrame);
	g_MainRedo.UndoLast();
	g_CurMdl.SetCurrentFrame(g_CurFrame);
	if (g_pRef)
		g_pRef->SetCurrentFrame(g_CurFrame);

	CalcAllNormals();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KEditCopy()
{
	g_CurMdl.UpdateFrame(g_CurFrame);
	CopyBuffer = g_CurMdl;
	CopyBufferMode = g_MainSelectMode;


	::EnableMenuItem(g_MainDlg->GetMenu()->m_hMenu, ID_EDIT_PASTE, MF_BYCOMMAND | MF_ENABLED);
	::EnableMenuItem(g_MainDlg->GetMenu()->m_hMenu, ID_EDIT_PASTETORANGE, MF_BYCOMMAND | MF_ENABLED);

	return 1;
}

int KEditPaste()
{
	if (CopyBuffer.Empty)
		return 0;

	for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
	{
		g_CurMdl.Frames[g_CurFrame][n].x = g_CurMdl.Verts[n].x;
		g_CurMdl.Frames[g_CurFrame][n].y = g_CurMdl.Verts[n].y;
		g_CurMdl.Frames[g_CurFrame][n].z = g_CurMdl.Verts[n].z;
	}

	CPasteMethodDlg   PasteDlg;

	if (PasteDlg.DoModal() == IDCANCEL)
		return 0;

	float Blend = (float)(PasteDlg.m_BlendFactor / 100.0);

	if (PasteDlg.m_ReplaceVerts == true)
	{
		if (CopyBufferMode == stTriangle)
		{
			bool  *selverts;
			int   n, i;

			selverts = new bool[g_CurMdl.Head.num_xyz];

			for (i = 0; i < g_CurMdl.Head.num_xyz; i++)
				selverts[i] = false;

			for (n = 0; n < CopyBuffer.Head.num_tris; n++)
				if (CopyBuffer.Tris[n].Selected)
				{
					selverts[g_CurMdl.Tris[n].Vertices[0]] = true;
					selverts[g_CurMdl.Tris[n].Vertices[1]] = true;
					selverts[g_CurMdl.Tris[n].Vertices[2]] = true;
				}

			for (n = 0; n < CopyBuffer.Head.num_xyz; n++)
				if (selverts[n])
				{
					g_CurMdl.Verts[n].x += Blend*(CopyBuffer.Verts[n].x - g_CurMdl.Verts[n].x);
					g_CurMdl.Verts[n].y += Blend*(CopyBuffer.Verts[n].y - g_CurMdl.Verts[n].y);
					g_CurMdl.Verts[n].z += Blend*(CopyBuffer.Verts[n].z - g_CurMdl.Verts[n].z);
					g_CurMdl.Verts[n].Selected = true;
				}

			for (i = 0; i < g_CurMdl.Head.num_tris; i++)
				g_CurMdl.Tris[i].Selected = CopyBuffer.Tris[i].Selected;

			delete[] selverts;

		}


		if (CopyBufferMode == stVertex)
		{
			for (int n = 0; n < CopyBuffer.Head.num_xyz; n++)
				if (CopyBuffer.Verts[n].Selected)
				{
					g_CurMdl.Verts[n].x += Blend*(CopyBuffer.Verts[n].x - g_CurMdl.Verts[n].x);
					g_CurMdl.Verts[n].y += Blend*(CopyBuffer.Verts[n].y - g_CurMdl.Verts[n].y);
					g_CurMdl.Verts[n].z += Blend*(CopyBuffer.Verts[n].z - g_CurMdl.Verts[n].z);
					g_CurMdl.Verts[n].Selected = true;
				}
				else g_CurMdl.Verts[n].Selected = false;
		}
	}

	if (PasteDlg.m_ReplaceVerts == false)
	{
		bool			*vertsel;
		int			vertcopycount = 0, tricopycount = 0;
		short			*destverts, *desttris;
		TRealVertex	*tempverts;
		TRealTriangle	*temptris;
		int         n, t;

		destverts = new short[CopyBuffer.Head.num_xyz];
		desttris = new short[CopyBuffer.Head.num_tris];
		vertsel = new bool[CopyBuffer.Head.num_xyz];

		for (int i = CopyBuffer.Head.num_xyz - 1; i >= 0; i--)
			vertsel[i] = false;

		if (CopyBufferMode == stVertex)
		{
			for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
				g_CurMdl.Verts[n].Selected = false;
			for (n = 0; n < CopyBuffer.Head.num_xyz; n++)
				vertsel[n] = CopyBuffer.Verts[n].Selected;
		}

		if (CopyBufferMode == stTriangle)
		{
			for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
				g_CurMdl.Tris[n].Selected = false;
			for (n = 0; n < CopyBuffer.Head.num_tris; n++)
				if (CopyBuffer.Tris[n].Selected)
				{
					vertsel[CopyBuffer.Tris[n].Vertices[0]] = true;
					vertsel[CopyBuffer.Tris[n].Vertices[1]] = true;
					vertsel[CopyBuffer.Tris[n].Vertices[2]] = true;
				}

		}

		for (n = 0; n < CopyBuffer.Head.num_xyz; n++)
			if (vertsel[n])
			{
				destverts[n] = (short)(g_CurMdl.Head.num_xyz + vertcopycount);
				vertcopycount++;
			}
			else destverts[n] = -1;

			for (n = 0; n < g_CurMdl.Head.num_frames; n++)
			{
				tempverts = new TRealVertex[g_CurMdl.Head.num_xyz + vertcopycount];

				memcpy(tempverts, g_CurMdl.Frames[n], sizeof(TRealVertex)*g_CurMdl.Head.num_xyz);
				for (int v = 0; v < CopyBuffer.Head.num_xyz; v++)
					if (vertsel[v])
					{
						tempverts[destverts[v]] = CopyBuffer.Verts[v];
					}

				delete[] g_CurMdl.Frames[n];
				g_CurMdl.Frames[n] = tempverts;

			}

			delete[] g_CurMdl.Verts;
			g_CurMdl.Verts = new TRealVertex[g_CurMdl.Head.num_xyz + vertcopycount];
			memcpy(g_CurMdl.Verts, g_CurMdl.Frames[g_CurFrame], sizeof(TRealVertex)*(g_CurMdl.Head.num_xyz + vertcopycount));

			for (t = 0; t < CopyBuffer.Head.num_tris; t++)
				if (vertsel[CopyBuffer.Tris[t].Vertices[0]] &&
					vertsel[CopyBuffer.Tris[t].Vertices[1]] &&
					vertsel[CopyBuffer.Tris[t].Vertices[2]])
				{
					desttris[t] = (short)(g_CurMdl.Head.num_tris + tricopycount);
					tricopycount++;
				}
				else desttris[t] = -1;


				temptris = new TRealTriangle[g_CurMdl.Head.num_tris + tricopycount];
				memcpy(temptris, g_CurMdl.Tris, sizeof(TRealTriangle)*g_CurMdl.Head.num_tris);
				for (t = 0; t < CopyBuffer.Head.num_tris; t++)
				{
					if (desttris[t] != -1)
					{
						temptris[desttris[t]].Vertices[0] = destverts[CopyBuffer.Tris[t].Vertices[0]];
						temptris[desttris[t]].Vertices[1] = destverts[CopyBuffer.Tris[t].Vertices[1]];
						temptris[desttris[t]].Vertices[2] = destverts[CopyBuffer.Tris[t].Vertices[2]];
						temptris[desttris[t]].SkinVerts[0] = 0;
						temptris[desttris[t]].SkinVerts[1] = 0;
						temptris[desttris[t]].SkinVerts[2] = 0;
					}
				}

				delete[] g_CurMdl.Tris;
				g_CurMdl.Tris = temptris;

				g_CurMdl.Head.num_xyz += vertcopycount;
				g_CurMdl.Head.num_tris += tricopycount;

				if (CopyBufferMode == stTriangle)
				{
					for (int i = g_CurMdl.Head.num_tris - tricopycount - 1; i >= 0; i--)
						g_CurMdl.Tris[i].Selected = false;
					for (int i = g_CurMdl.Head.num_tris - tricopycount; i < g_CurMdl.Head.num_tris; i++)
						g_CurMdl.Tris[i].Selected = true;
				}
				else
					for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
						g_CurMdl.Tris[i].Selected = false;

				if (CopyBufferMode == stVertex)
				{
					for (int i = g_CurMdl.Head.num_xyz - vertcopycount - 1; i >= 0; i--)
						g_CurMdl.Verts[i].Selected = false;
					for (int i = g_CurMdl.Head.num_xyz - vertcopycount; i < g_CurMdl.Head.num_xyz; i++)
						g_CurMdl.Verts[i].Selected = true;
				}
				else
					for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
						g_CurMdl.Verts[i].Selected = false;

				int *targskinverts;
				bool *seltri, *selst;

				seltri = new bool[CopyBuffer.Head.num_tris];
				selst = new bool[CopyBuffer.Head.num_st];

				if (CopyBufferMode == stTriangle)
				{
					for (int i = 0; i < CopyBuffer.Head.num_tris; i++)
						seltri[i] = CopyBuffer.Tris[i].Selected;
				}
				else
				{
					for (int i = 0; i < CopyBuffer.Head.num_tris; i++)
						if (CopyBuffer.Verts[CopyBuffer.Tris[i].Vertices[0]].Selected &&
							CopyBuffer.Verts[CopyBuffer.Tris[i].Vertices[1]].Selected &&
							CopyBuffer.Verts[CopyBuffer.Tris[i].Vertices[2]].Selected)
							seltri[i] = true;
						else seltri[i] = false;

				}

				targskinverts = new int[CopyBuffer.Head.num_st];

				for (int i = 0; i < CopyBuffer.Head.num_st; i++)
				{
					targskinverts[i] = -1;
					selst[i] = false;
				}
				for (int i = 0; i < CopyBuffer.Head.num_tris; i++)
					if (seltri[i])
					{
						selst[CopyBuffer.Tris[i].SkinVerts[0]] = true;
						selst[CopyBuffer.Tris[i].SkinVerts[1]] = true;
						selst[CopyBuffer.Tris[i].SkinVerts[2]] = true;
					}

				int targ = g_CurMdl.Head.num_st;

				for (int i = 0; i < CopyBuffer.Head.num_st; i++)
					if (selst[i])
						targskinverts[i] = targ++;

				TRealSkinVertex	*newSkinVerts;

				newSkinVerts = new TRealSkinVertex[targ];
				memcpy(newSkinVerts, g_CurMdl.SkinVerts, sizeof(TRealSkinVertex)*g_CurMdl.Head.num_st);

				for (int i = 0; i < CopyBuffer.Head.num_st; i++)
				{
					if (selst[i])
					{
						newSkinVerts[targskinverts[i]].s = CopyBuffer.SkinVerts[i].s;
						newSkinVerts[targskinverts[i]].t = CopyBuffer.SkinVerts[i].t;
						newSkinVerts[targskinverts[i]].Selected = true;
					}
				}

				delete[] g_CurMdl.SkinVerts;
				g_CurMdl.SkinVerts = newSkinVerts;

				for (t = 0; t < CopyBuffer.Head.num_tris; t++)
				{
					if (desttris[t] != -1)
					{
						temptris[desttris[t]].SkinVerts[0] = (short)targskinverts[CopyBuffer.Tris[t].SkinVerts[0]];
						temptris[desttris[t]].SkinVerts[1] = (short)targskinverts[CopyBuffer.Tris[t].SkinVerts[1]];
						temptris[desttris[t]].SkinVerts[2] = (short)targskinverts[CopyBuffer.Tris[t].SkinVerts[2]];
					}
				}

				delete[] destverts;
				delete[] desttris;

				g_CurMdl.Head.num_st = targ;

				delete[] targskinverts;
				delete[] seltri;
				delete[] selst;
	}

	g_CurMdl.CalcNormals();
	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}

int KEditPasteRange()
{
	CPasteRangeDialog    PasteDlg;
	bool                 align;
	int                  i, n;

	for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
	{
		g_CurMdl.Frames[g_CurFrame][n].x = g_CurMdl.Verts[n].x;
		g_CurMdl.Frames[g_CurFrame][n].y = g_CurMdl.Verts[n].y;
		g_CurMdl.Frames[g_CurFrame][n].z = g_CurMdl.Verts[n].z;
	}

	if (PasteDlg.DoModal() == IDCANCEL)
		return 0;

	align = false;

	float Blend = (float)((PasteDlg.m_BlendFactor) / 100.0);
	bool	*seld = new bool[g_CurMdl.Head.num_xyz];

	if (CopyBufferMode == stTriangle)
	{
		for (i = 0; i < g_CurMdl.Head.num_xyz; i++)
			seld[i] = false;
		for (i = 0; i < g_CurMdl.Head.num_tris; i++)
			if (g_CurMdl.Tris[i].Selected)
			{
				seld[g_CurMdl.Tris[i].Vertices[0]] = true;
				seld[g_CurMdl.Tris[i].Vertices[1]] = true;
				seld[g_CurMdl.Tris[i].Vertices[2]] = true;
			}
	}
	else
		for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
			seld[i] = CopyBuffer.Verts[i].Selected;

	// !!! The paste alignment section goes here
	if (align)
	{
	}
	else
	{
		for (int f = PasteDlg.m_FrameStart; f <= PasteDlg.m_FrameEnd; f++)
		{
			for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
				if (seld[n])
				{
					g_CurMdl.Frames[f][n].x += Blend*(CopyBuffer.Verts[n].x - g_CurMdl.Frames[f][n].x);
					g_CurMdl.Frames[f][n].y += Blend*(CopyBuffer.Verts[n].y - g_CurMdl.Frames[f][n].y);
					g_CurMdl.Frames[f][n].z += Blend*(CopyBuffer.Verts[n].z - g_CurMdl.Frames[f][n].z);
					//               g_CurMdl.Frames[f][n].Parent = NULL;
				}
		}
	}

	for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
	{
		g_CurMdl.Verts[n].x = g_CurMdl.Frames[g_CurFrame][n].x;
		g_CurMdl.Verts[n].y = g_CurMdl.Frames[g_CurFrame][n].y;
		g_CurMdl.Verts[n].z = g_CurMdl.Frames[g_CurFrame][n].z;
	}

	g_CurMdl.CalcNormals();
	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}

int KEditRename()
{
	CRenameFrameDlg   RenameDlg;
	CString           Str;

	Str.Format("%i", g_CurFrame);
	RenameDlg.m_FrameNumEdit.SetWindowText(Str);
	RenameDlg.m_OldNameEdit.SetWindowText(g_CurMdl.FrameInfo[g_CurFrame].Name);
	RenameDlg.m_NewNameEdit.SetWindowText(g_CurMdl.FrameInfo[g_CurFrame].Name);

	if (RenameDlg.DoModal() == IDCANCEL)
		return 0;

	RenameDlg.m_NewNameEdit.GetWindowText(g_CurMdl.FrameInfo[g_CurFrame].Name, 16);

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);

	return 1;
}

int KEditGotoFrame()
{
	CChooseFrameDlg   ChooseDlg;
	int               n;

	if (ChooseDlg.DoModal() == IDCANCEL)
		return 0;

	for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
	{
		g_CurMdl.Frames[g_CurFrame][n].x = g_CurMdl.Verts[n].x;
		g_CurMdl.Frames[g_CurFrame][n].y = g_CurMdl.Verts[n].y;
		g_CurMdl.Frames[g_CurFrame][n].z = g_CurMdl.Verts[n].z;
	}


	g_CurFrame = ChooseDlg.m_SelectedFrame;

	if (g_CurFrame == g_CurMdl.Head.num_frames)
		g_CurFrame = 0;

	for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
	{
		g_CurMdl.Verts[n].x = g_CurMdl.Frames[g_CurFrame][n].x;
		g_CurMdl.Verts[n].y = g_CurMdl.Frames[g_CurFrame][n].y;
		g_CurMdl.Verts[n].z = g_CurMdl.Frames[g_CurFrame][n].z;
	}

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}

int KEditAddFrame()
{
	int				toframe;
	TFrameInfo		*TempFrameInfo;
	CAddFrameDlg   AddFrameDlg;

	if (AddFrameDlg.DoModal() == IDCANCEL)
		return 0;

	toframe = AddFrameDlg.m_FrameTarget;

	g_CurMdl.Frames[g_CurMdl.Head.num_frames] = new TRealVertex[g_CurMdl.Head.num_xyz];

	TempFrameInfo = new TFrameInfo[g_CurMdl.Head.num_frames + 1];

	if (toframe == g_CurMdl.Head.num_frames) // just chuck it on the end
	{
		memcpy(g_CurMdl.Frames[g_CurMdl.Head.num_frames], g_CurMdl.Frames[g_CurFrame], sizeof(TRealVertex)*g_CurMdl.Head.num_xyz);

		for (int n = 0; n < g_CurMdl.Head.num_frames; n++)
			TempFrameInfo[n] = g_CurMdl.FrameInfo[n];

		TempFrameInfo[g_CurMdl.Head.num_frames] = TempFrameInfo[g_CurFrame];

		char *dest;
		dest = TempFrameInfo[g_CurMdl.Head.num_frames].Name;
		AddFrameDlg.m_NewFrameNameEdit.GetWindowText(dest, 16);

		delete[] g_CurMdl.FrameInfo;
		g_CurMdl.FrameInfo = TempFrameInfo;
		g_CurMdl.Head.num_frames++;

		if (g_MainDlg)
			SendMessage(g_MainDlg->m_hWnd, MM_CHECKSPINNERS, 0, 0);
		return 0;
	}

	if (toframe == 0) // before everthing else
	{
		for (int n = g_CurMdl.Head.num_frames; n > 0; n--)
		{
			memcpy(g_CurMdl.Frames[n], g_CurMdl.Frames[n - 1], sizeof(TRealVertex)*g_CurMdl.Head.num_xyz);
			TempFrameInfo[n] = g_CurMdl.FrameInfo[n - 1];
		}

		g_CurFrame++;

		memcpy(g_CurMdl.Frames[0], g_CurMdl.Frames[g_CurFrame], sizeof(TRealVertex)*g_CurMdl.Head.num_xyz);
		TempFrameInfo[0] = g_CurMdl.FrameInfo[g_CurFrame];

		TempFrameInfo[0] = TempFrameInfo[g_CurFrame];
		char *dest;
		dest = TempFrameInfo[0].Name;
		AddFrameDlg.m_NewFrameNameEdit.GetWindowText(dest, 16);

		delete[] g_CurMdl.FrameInfo;
		g_CurMdl.FrameInfo = TempFrameInfo;
		g_CurMdl.Head.num_frames++;

		if (g_MainDlg)
			SendMessage(g_MainDlg->m_hWnd, MM_CHECKSPINNERS, 0, 0);
		return 0;
	}

	int n;

	if (g_CurFrame >= toframe)
		g_CurFrame++;

	for (n = 0; n < toframe; n++)
		TempFrameInfo[n] = g_CurMdl.FrameInfo[n];

	for (n = g_CurMdl.Head.num_frames; n > toframe; n--)
	{
		TempFrameInfo[n] = g_CurMdl.FrameInfo[n - 1];
		memcpy(g_CurMdl.Frames[n], g_CurMdl.Frames[n - 1], sizeof(TRealVertex)*g_CurMdl.Head.num_xyz);

	}

	TempFrameInfo[toframe] = g_CurMdl.FrameInfo[g_CurFrame];
	memcpy(g_CurMdl.Frames[toframe], g_CurMdl.Frames[g_CurFrame], sizeof(TRealVertex)*g_CurMdl.Head.num_xyz);

	char *dest;
	dest = TempFrameInfo[toframe].Name;

	strcpy(dest, AddFrameDlg.m_NewName);

	delete[] g_CurMdl.FrameInfo;
	g_CurMdl.FrameInfo = TempFrameInfo;
	g_CurMdl.Head.num_frames++;

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_CHECKSPINNERS, 0, 0);

	return 1;
}

int KEditDeleteFrameCurrent()
{
	TFrameInfo	*TempFrameInfo;
	int         destn, srcn, n;

	if (g_CurMdl.Head.num_frames == 1)
	{
		MessageBox(NULL, "You must have at least one frame", "Error", MB_OK);
		return 0;
	}

	if (MessageBox(NULL, "Are you sure you wish to delete this frame?", "Are you Sure", MB_OKCANCEL | MB_ICONQUESTION) == IDCANCEL)
		return 0;

	TempFrameInfo = new TFrameInfo[g_CurMdl.Head.num_frames - 1];

	destn = 0;
	for (n = 0; n < g_CurMdl.Head.num_frames; n++)
	{
		srcn = n;

		if (srcn != g_CurFrame)
		{
			memcpy(g_CurMdl.Frames[destn], g_CurMdl.Frames[srcn], sizeof(TRealVertex)*g_CurMdl.Head.num_xyz);
			memcpy(&TempFrameInfo[destn], &g_CurMdl.FrameInfo[srcn], sizeof(TFrameInfo));
			destn++;
		}
	}

	delete[] g_CurMdl.Frames[g_CurMdl.Head.num_frames - 1];
	delete[] g_CurMdl.FrameInfo;
	g_CurMdl.FrameInfo = TempFrameInfo;

	g_CurMdl.Head.num_frames--;

	if (g_CurFrame == g_CurMdl.Head.num_frames) g_CurFrame--;

	for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
	{
		g_CurMdl.Verts[n].x = g_CurMdl.Frames[g_CurFrame][n].x;
		g_CurMdl.Verts[n].y = g_CurMdl.Frames[g_CurFrame][n].y;
		g_CurMdl.Verts[n].z = g_CurMdl.Frames[g_CurFrame][n].z;
	}

	g_CurMdl.CalcNormals();

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_CHECKSPINNERS, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}

int KEditDeleteFrameRange()
{
	CDeleteFrameRangeDlg DeleteFramesDlg;
	if (DeleteFramesDlg.DoModal() == IDCANCEL)
		return 0;

	int            From, To, nDel;
	TRealVertex    **ToDelete;
	int            *TargFrame;
	TFrameInfo     *NewFrameInfo;
	int            temp = 0;

	From = DeleteFramesDlg.m_FrameStart;
	To = DeleteFramesDlg.m_FrameEnd;
	nDel = To - From + 1;

	if (nDel <= 0)
		return 0;

	if (From < 0 || From > To || From >= g_CurMdl.Head.num_frames)
		return 0;

	if (To < 0 || To < From || To >= g_CurMdl.Head.num_frames)
		return 0;

	TargFrame = new int[g_CurMdl.Head.num_frames];

	{
		for (int i = 0; i < g_CurMdl.Head.num_frames; i++)
		{
			if (i >= From && i <= To)
			{
				TargFrame[i] = -1;
			}
			else
			{
				TargFrame[i] = temp;
				temp++;
			}
		}
	}

	int NewFrameCount = temp, i;
	ToDelete = new TRealVertex*[nDel];

	NewFrameInfo = new TFrameInfo[NewFrameCount];

	int DelCount = 0;
	for (i = 0; i < g_CurMdl.Head.num_frames; i++)
	{
		if (TargFrame[i] == -1)
		{
			ToDelete[DelCount] = g_CurMdl.Frames[i];
			DelCount++;
		}
		else
		{
			g_CurMdl.Frames[TargFrame[i]] = g_CurMdl.Frames[i];
			memcpy(&NewFrameInfo[TargFrame[i]], &g_CurMdl.FrameInfo[i], sizeof(TFrameInfo));
		}
	}

	delete[] g_CurMdl.FrameInfo;

	g_CurMdl.FrameInfo = NewFrameInfo;
	g_CurMdl.Head.num_frames = NewFrameCount;

	for (i = 0; i < DelCount; i++)
		delete[] ToDelete[i];

	delete[] ToDelete;

	delete[] TargFrame;

	if (g_CurFrame >= g_CurMdl.Head.num_frames)
		g_CurFrame = g_CurMdl.Head.num_frames - 1;

	g_CurMdl.SetCurrentFrame(g_CurFrame);
	if (g_pRef)
		g_pRef->SetCurrentFrame(g_CurFrame);

	g_CurMdl.CalcNormals();

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_CHECKSPINNERS, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}

int KEditMoveFrame()
{
	CMoveFramesDlg    MoveFramesDlg;
	int               i;
	int               *TargFrames;
	int               srcbegin, srcend, targframe;
	int               TempNumFrames, MoveNumFrames, *MoveFrames;

	if (MoveFramesDlg.DoModal() == IDCANCEL)
		return 0;

	for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
	{
		g_CurMdl.Frames[g_CurFrame][n].x = g_CurMdl.Verts[n].x;
		g_CurMdl.Frames[g_CurFrame][n].y = g_CurMdl.Verts[n].y;
		g_CurMdl.Frames[g_CurFrame][n].z = g_CurMdl.Verts[n].z;
	}


	srcbegin = MoveFramesDlg.m_FrameStart;
	srcend = MoveFramesDlg.m_FrameEnd;
	targframe = MoveFramesDlg.m_FrameTarget;

	if (MoveFramesDlg.m_BeforeFrame == false)
		targframe++;

	// This is an array of numbers indicated where the ith frame will be copied to.
	TargFrames = new int[g_CurMdl.Head.num_frames];

	// Init all frames targets to -1, indicating they haven't been set
	for (i = 0; i < g_CurMdl.Head.num_frames; i++)
		TargFrames[i] = i;

	TempNumFrames = g_CurMdl.Head.num_frames;
	MoveNumFrames = srcend - srcbegin + 1;

	// Remove the Frames to be moved from the array.
	MoveFrames = new int[MoveNumFrames];

	memcpy(MoveFrames, &TargFrames[srcbegin], MoveNumFrames * sizeof(int));


	// Copy the frames after the move block so they are contigous with the frames
	// before the move block.
	{
		int   cnt;
		cnt = srcbegin;
		for (int i = srcend + 1; i < g_CurMdl.Head.num_frames; i++)
		{
			TargFrames[cnt] = TargFrames[i];
			TargFrames[i] = -1;
			cnt++;
		}
	}

	// Reduce the frame count by the number of move frames;
	TempNumFrames -= MoveNumFrames;

	// Adjust the targframe number so it is correct after the move block of frames has 
	// been removed
	if (targframe > srcend)
		targframe -= MoveNumFrames;


	// Now we get the move block, and insert it at the targframe position.

	// First, move the frames that will be after the move block to the end of the frame
	// array.

	// When the frames are inserted, they are inserted at targframe, and the frame that is
	// currently at targframe is moved to after the move block.
	int NumEndFrames;
	NumEndFrames = TempNumFrames - targframe;

	if (NumEndFrames > 0)
	{
		int   cnt;

		cnt = g_CurMdl.Head.num_frames - 1;

		for (int i = targframe + NumEndFrames - 1; i >= targframe; i--)
		{
			TargFrames[cnt] = TargFrames[i];
			cnt--;
		}
	}

	memcpy(&TargFrames[targframe], MoveFrames, sizeof(int)*MoveNumFrames);


	TRealVertex *NewFrames[MAX_FRAMES];
	TFrameInfo  *NewFrameInfo;


	NewFrameInfo = new TFrameInfo[g_CurMdl.Head.num_frames];

	for (i = 0; i < g_CurMdl.Head.num_frames; i++)
	{
		//      NewFrames[TargFrames[i]] = g_CurMdl.Frames[i];
		//      memcpy(&NewFrameInfo[TargFrames[i]], &g_CurMdl.FrameInfo[i], sizeof(TFrameInfo));
		NewFrames[i] = g_CurMdl.Frames[TargFrames[i]];
		memcpy(&NewFrameInfo[i], &g_CurMdl.FrameInfo[TargFrames[i]], sizeof(TFrameInfo));
	}

	delete[] g_CurMdl.FrameInfo;
	g_CurMdl.FrameInfo = NewFrameInfo;

	for (i = 0; i < g_CurMdl.Head.num_frames; i++)
	{
		g_CurMdl.Frames[i] = NewFrames[i];
	}

	g_CurMdl.SetCurrentFrame(g_CurFrame);
	if (g_pRef)
		g_pRef->SetCurrentFrame(g_CurFrame);

	g_CurMdl.CalcNormals();

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_CHECKSPINNERS, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	delete[] MoveFrames;
	delete[] TargFrames;

	return 1;
}

int KEditSelectAll()
{
	bool *OldSel, *NewSel;

	if (g_MainSelectMode == stVertex)
	{
		OldSel = new bool[g_CurMdl.Head.num_xyz];
		NewSel = new bool[g_CurMdl.Head.num_xyz];

		g_CurMdl.LoadSelection(NewSel, ssVertex);
		for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
			if (g_CurMdl.Verts[n].Visible)
				g_CurMdl.Verts[n].Selected = true;
		g_CurMdl.LoadSelection(OldSel, ssVertex);
	}
	if (g_MainSelectMode == stTriangle)
	{
		OldSel = new bool[g_CurMdl.Head.num_tris];
		NewSel = new bool[g_CurMdl.Head.num_tris];
		g_CurMdl.LoadSelection(NewSel, ssTriangle);
		for (int n = 0; n < g_CurMdl.Head.num_tris; n++)
			if (g_CurMdl.Tris[n].Visible)
				g_CurMdl.Tris[n].Selected = true;
		g_CurMdl.LoadSelection(OldSel, ssTriangle);
	}
	if (g_MainSelectMode == stBone)
		g_CurMdl.Bones.SelectBones(true);

	CSelectUndoNode *N;
	N = new CSelectUndoNode;
	N->Set("Select", &g_CurMdl, g_MainSelectMode, NewSel);
	g_MainUndo.AddNode(N);

	delete[] OldSel;
	delete[] NewSel;

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}

int KEditSelectNone()
{
	bool *OldSel, *NewSel;

	if (g_MainSelectMode == stVertex)
	{
		OldSel = new bool[g_CurMdl.Head.num_xyz];
		NewSel = new bool[g_CurMdl.Head.num_xyz];

		g_CurMdl.LoadSelection(NewSel, ssVertex);

		for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
			if (g_CurMdl.Verts[n].Visible)
				g_CurMdl.Verts[n].Selected = false;

		g_CurMdl.LoadSelection(OldSel, ssVertex);
	}
	if (g_MainSelectMode == stTriangle)
	{
		OldSel = new bool[g_CurMdl.Head.num_tris];
		NewSel = new bool[g_CurMdl.Head.num_tris];

		g_CurMdl.LoadSelection(OldSel, ssTriangle);
		for (int n = 0; n < g_CurMdl.Head.num_tris; n++)
			if (g_CurMdl.Tris[n].Visible)
				g_CurMdl.Tris[n].Selected = false;
		g_CurMdl.LoadSelection(NewSel, ssTriangle);
	}
	if (g_MainSelectMode == stBone)
		g_CurMdl.Bones.SelectBones(false);

	CSelectUndoNode *N;
	N = new CSelectUndoNode;
	N->Set("Select", &g_CurMdl, g_MainSelectMode, NewSel);
	g_MainUndo.AddNode(N);

	delete[] OldSel;
	delete[] NewSel;

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}

int KEditSelectInverse()
{
	bool *OldSel, *NewSel;

	if (g_MainSelectMode == stVertex)
	{
		OldSel = new bool[g_CurMdl.Head.num_xyz];
		NewSel = new bool[g_CurMdl.Head.num_xyz];

		g_CurMdl.LoadSelection(NewSel, ssVertex);

		for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
			if (g_CurMdl.Verts[n].Visible)
				g_CurMdl.Verts[n].Selected = !g_CurMdl.Verts[n].Selected;

		g_CurMdl.LoadSelection(OldSel, ssVertex);
	}
	if (g_MainSelectMode == stTriangle)
	{
		OldSel = new bool[g_CurMdl.Head.num_tris];
		NewSel = new bool[g_CurMdl.Head.num_tris];

		g_CurMdl.LoadSelection(NewSel, ssTriangle);

		for (int n = 0; n < g_CurMdl.Head.num_tris; n++)
			if (g_CurMdl.Tris[n].Visible)
				g_CurMdl.Tris[n].Selected = !g_CurMdl.Tris[n].Selected;

		g_CurMdl.LoadSelection(OldSel, ssTriangle);
	}

	CSelectUndoNode *N;
	N = new CSelectUndoNode;
	N->Set("Select", &g_CurMdl, g_MainSelectMode, NewSel);
	g_MainUndo.AddNode(N);

	delete[] OldSel;
	delete[] NewSel;

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}

int KEditSelectConnected()
{
	bool *Buf, *OldSel, *NewSel;

	if (g_MainSelectMode == stVertex)
	{
		Buf = new bool[g_CurMdl.Head.num_xyz];
		OldSel = new bool[g_CurMdl.Head.num_xyz];
		NewSel = new bool[g_CurMdl.Head.num_xyz];

		for (int v = 0; v < g_CurMdl.Head.num_xyz; v++)
			Buf[v] = g_CurMdl.Verts[v].Selected;

		memcpy(NewSel, Buf, sizeof(bool)*g_CurMdl.Head.num_xyz);
	}
	if (g_MainSelectMode == stTriangle)
	{
		Buf = new bool[g_CurMdl.Head.num_tris];
		OldSel = new bool[g_CurMdl.Head.num_tris];
		NewSel = new bool[g_CurMdl.Head.num_tris];

		for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
			Buf[t] = g_CurMdl.Tris[t].Selected;

		memcpy(NewSel, Buf, sizeof(bool)*g_CurMdl.Head.num_tris);
	}

	g_CurMdl.SelectConnected(Buf, g_MainSelectMode);

	if (g_MainSelectMode == stVertex)
	{
		memcpy(OldSel, Buf, sizeof(bool)*g_CurMdl.Head.num_xyz);
		for (int v = 0; v < g_CurMdl.Head.num_xyz; v++)
			g_CurMdl.Verts[v].Selected = Buf[v];
	}
	if (g_MainSelectMode == stTriangle)
	{
		memcpy(OldSel, Buf, sizeof(bool)*g_CurMdl.Head.num_tris);
		for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
			g_CurMdl.Tris[t].Selected = Buf[t];
	}

	CSelectUndoNode *N;
	N = new CSelectUndoNode;
	N->Set("Select", &g_CurMdl, g_MainSelectMode, NewSel);
	g_MainUndo.AddNode(N);

	delete[] OldSel;
	delete[] NewSel;
	delete[] Buf;

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}


int KEditSelectTouching()
{
	bool *seld, *OldSel, *NewSel;

	bool changed;
	if (g_MainSelectMode == stVertex)
	{
		seld = new bool[g_CurMdl.Head.num_xyz];
		OldSel = new bool[g_CurMdl.Head.num_xyz];
		NewSel = new bool[g_CurMdl.Head.num_xyz];

		g_CurMdl.LoadSelection(seld, ssVertex);
		g_CurMdl.LoadSelection(NewSel, ssVertex);

		memcpy(NewSel, seld, sizeof(bool)*g_CurMdl.Head.num_xyz);

		for (int n = 0; n < 1; n++)
		{
			changed = false;
			for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
				if (
					/*               !(g_CurMdl.Verts[g_CurMdl.Tris[t].Vertices[0]].Selected &&
									 g_CurMdl.Verts[g_CurMdl.Tris[t].Vertices[1]].Selected &&
									 g_CurMdl.Verts[g_CurMdl.Tris[t].Vertices[2]].Selected)
												  &&                                        */
					(g_CurMdl.Verts[g_CurMdl.Tris[t].Vertices[0]].Selected ||
						g_CurMdl.Verts[g_CurMdl.Tris[t].Vertices[1]].Selected ||
						g_CurMdl.Verts[g_CurMdl.Tris[t].Vertices[2]].Selected))
				{
					seld[g_CurMdl.Tris[t].Vertices[0]] = true;
					seld[g_CurMdl.Tris[t].Vertices[1]] = true;
					seld[g_CurMdl.Tris[t].Vertices[2]] = true;
					changed = true;
				}
			if (changed == false) n = 5000;
		}

		for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
			g_CurMdl.Verts[i].Selected = seld[i];

		g_CurMdl.LoadSelection(OldSel, ssVertex);

		delete[] seld;
	}
	if (g_MainSelectMode == stTriangle)
	{
		bool  *vertsel;
		int   n, i, t;

		vertsel = new bool[g_CurMdl.Head.num_xyz];
		seld = new bool[g_CurMdl.Head.num_xyz];
		OldSel = new bool[g_CurMdl.Head.num_tris];
		NewSel = new bool[g_CurMdl.Head.num_tris];

		g_CurMdl.LoadSelection(NewSel, ssTriangle);

		for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
			vertsel[n] = false;

		for (i = 0; i < g_CurMdl.Head.num_xyz; i++)
			seld[i] = false;

		for (t = 0; t < g_CurMdl.Head.num_tris; t++)
			if (g_CurMdl.Tris[t].Selected)
			{
				vertsel[g_CurMdl.Tris[t].Vertices[0]] = true;
				vertsel[g_CurMdl.Tris[t].Vertices[1]] = true;
				vertsel[g_CurMdl.Tris[t].Vertices[2]] = true;
			}


		for (n = 0; n < 1; n++)
		{
			changed = false;
			for (t = 0; t < g_CurMdl.Head.num_tris; t++)
				if (
					!(vertsel[g_CurMdl.Tris[t].Vertices[0]] &&
						vertsel[g_CurMdl.Tris[t].Vertices[1]] &&
						vertsel[g_CurMdl.Tris[t].Vertices[2]])
					&&
					(vertsel[g_CurMdl.Tris[t].Vertices[0]] ||
						vertsel[g_CurMdl.Tris[t].Vertices[1]] ||
						vertsel[g_CurMdl.Tris[t].Vertices[2]]))
				{
					seld[g_CurMdl.Tris[t].Vertices[0]] = true;
					seld[g_CurMdl.Tris[t].Vertices[1]] = true;
					seld[g_CurMdl.Tris[t].Vertices[2]] = true;
					changed = true;
				}
			if (changed == false) n = 5000;
		}

		for (t = 0; t < g_CurMdl.Head.num_tris; t++)
		{
			if (seld[g_CurMdl.Tris[t].Vertices[0]] &&
				seld[g_CurMdl.Tris[t].Vertices[1]] &&
				seld[g_CurMdl.Tris[t].Vertices[2]])
				g_CurMdl.Tris[t].Selected = true;
		}

		g_CurMdl.LoadSelection(OldSel, ssTriangle);
		delete[] vertsel;
	}

	CSelectUndoNode *N;
	N = new CSelectUndoNode;
	N->Set("Select", &g_CurMdl, g_MainSelectMode, NewSel);
	g_MainUndo.AddNode(N);

	delete[] OldSel;
	delete[] NewSel;

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}


// View Menu

int KViewSkins()
{
	if (g_SkinEditDlg)
	{
		g_SkinEditDlg->ShowWindow(SW_SHOW);
		if (g_SkinEditDlg->IsMenuChecked(ID_OPTIONS_KEEPWINDOWONTOP))
			g_SkinEditDlg->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			g_SkinEditDlg->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	return 1;
}

int KViewSkinPainter()
{
	if (g_PaintDlg)
	{
		g_PaintDlg->ShowWindow(SW_SHOW);
		g_PaintDlg->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	return 1;
}

int KView3DEditor()
{
	if (g_MainDlg)
	{
		g_MainDlg->ShowWindow(SW_SHOW);
		g_MainDlg->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	return 1;
}

int KViewTypeIn()
{
	if (g_TypeInDlg)
	{
		g_TypeInDlg->ShowWindow(SW_SHOW);
		g_TypeInDlg->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	return 1;
}

int KViewProperties()
{
	CString  Str;

	if (g_MainDlg)
	{
		g_MainDlg->AddLineToConsole("");
		g_MainDlg->AddLineToConsole(CString("File Name: ") + g_CurFileName);

		Str.Format("ID: %i", g_CurMdl.Head.id);
		g_MainDlg->AddLineToConsole(Str);

		Str.Format("Version: %i", g_CurMdl.Head.version);
		g_MainDlg->AddLineToConsole(Str);

		Str.Format("Number of Skins: %i", g_CurMdl.Head.num_skins);
		g_MainDlg->AddLineToConsole(Str);

		Str.Format("Skin Width: %i", g_CurMdl.Head.skinwidth);
		g_MainDlg->AddLineToConsole(Str);

		Str.Format("Skin Height: %i", g_CurMdl.Head.skinheight);
		g_MainDlg->AddLineToConsole(Str);

		Str.Format("Number of Vertices: %i", g_CurMdl.Head.num_xyz);
		g_MainDlg->AddLineToConsole(Str);

		Str.Format("Number of Skin Vertices: %i", g_CurMdl.Head.num_st);
		g_MainDlg->AddLineToConsole(Str);

		Str.Format("Number of Faces: %i", g_CurMdl.Head.num_tris);
		g_MainDlg->AddLineToConsole(Str);

		Str.Format("Number of Frames: %i", g_CurMdl.Head.num_frames);
		g_MainDlg->AddLineToConsole(Str);
		Str.Format("Average Size of Triangles: %i", g_CurMdl.Head.framesize);
		g_MainDlg->AddLineToConsole(Str);

		{
			RECT  Rect;
			g_MainDlg->GetWindowRect(&Rect);
			g_MainDlg->ScreenToClient(&Rect);

			Str.Format("MainWindow.x: %i", Rect.right);
			g_MainDlg->AddLineToConsole(Str);
			Str.Format("MainWindow.y: %i", Rect.bottom);
			g_MainDlg->AddLineToConsole(Str);
		}

	}
	KToolsMemoWindow();

	//g_MainDlg->ModelProperties->Click();
	return 1;
}


// options menu

int KOptionsVertTicks()
{
	if (g_MainDlg)
	{
		g_MainDlg->ToggleMenuChecked(ID_OPTIONS_VERTICESTICKS);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsShowOrigin()
{
	if (g_MainDlg)
	{
		g_MainDlg->ToggleMenuChecked(ID_OPTIONS_SHOWORIGIN);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsShowGrid()
{
	if (g_MainDlg)
	{
		g_MainDlg->ToggleMenuChecked(ID_OPTIONS_SHOWGRID);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsGridPos()
{
	CGroundPlanePosDlg   GroundDlg;

	GroundDlg.m_GroundPlanePos = g_GroundHeight;

	if (GroundDlg.DoModal() == IDCANCEL)
		return 0;

	g_GroundHeight = (float)GroundDlg.m_GroundPlanePos;

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsNormalsNone()
{
	if (g_MainDlg)
	{
		g_MainDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_SHOWNORMALS_NONE,
			ID_OPTIONS_SHOWNORMALS_ALL,
			ID_OPTIONS_SHOWNORMALS_NONE,
			MF_BYCOMMAND);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}

	return 1;
}

int KOptionsNormalsSelect()
{
	if (g_MainDlg)
	{
		g_MainDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_SHOWNORMALS_NONE,
			ID_OPTIONS_SHOWNORMALS_ALL,
			ID_OPTIONS_SHOWNORMALS_SELECTEDFACES,
			MF_BYCOMMAND);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsNormalsAll()
{
	if (g_MainDlg)
	{
		g_MainDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_SHOWNORMALS_NONE,
			ID_OPTIONS_SHOWNORMALS_ALL,
			ID_OPTIONS_SHOWNORMALS_ALL,
			MF_BYCOMMAND);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsNormalsCycle()
{
	if (g_MainDlg)
	{
		if (g_MainDlg->IsMenuChecked(ID_OPTIONS_SHOWNORMALS_NONE))
			KOptionsNormalsSelect();
		else
			if (g_MainDlg->IsMenuChecked(ID_OPTIONS_SHOWNORMALS_SELECTEDFACES))
				KOptionsNormalsAll();
			else
				if (g_MainDlg->IsMenuChecked(ID_OPTIONS_SHOWNORMALS_ALL))
					KOptionsNormalsNone();

		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptions2DBackface()
{
	if (g_MainDlg)
	{
		g_MainDlg->ToggleMenuChecked(ID_OPTIONS_2DVIEW_DRAWBACKFACES);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsCameraPlayXY()
{
	if (g_MainDlg)
	{
		g_MainDlg->ToggleMenuChecked(ID_OPTIONS_2DVIEW_PLAYCAMERAXY);
	}
	return 1;
}

int KOptionsCameraPlayZY()
{
	if (g_MainDlg)
	{
		g_MainDlg->ToggleMenuChecked(ID_OPTIONS_2DVIEW_PLAYCAMERAZY);
	}
	return 1;
}

int KOptionsCameraPlayXZ()
{
	if (g_MainDlg)
	{
		g_MainDlg->ToggleMenuChecked(ID_OPTIONS_2DVIEW_PLAYCAMERAXZ);
	}
	return 1;
}

int KOptions3DWireframe()
{
	g_MainDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_3DVIEW_WIREFRAME,
		ID_OPTIONS_3DVIEW_TEXTUREGOURAD,
		ID_OPTIONS_3DVIEW_WIREFRAME,
		MF_BYCOMMAND);
	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KOptions3DFlat()
{
	g_MainDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_3DVIEW_WIREFRAME,
		ID_OPTIONS_3DVIEW_TEXTUREGOURAD,
		ID_OPTIONS_3DVIEW_FLATSHADED,
		MF_BYCOMMAND);
	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	return 1;
}

int KOptions3DGourad()
{
	g_MainDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_3DVIEW_WIREFRAME,
		ID_OPTIONS_3DVIEW_TEXTUREGOURAD,
		ID_OPTIONS_3DVIEW_GOURADSHADED,
		MF_BYCOMMAND);

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	return 1;
}

int KOptions3DTexture()
{
	g_MainDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_3DVIEW_WIREFRAME,
		ID_OPTIONS_3DVIEW_TEXTUREGOURAD,
		ID_OPTIONS_3DVIEW_TEXTUREMAPPED,
		MF_BYCOMMAND);

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	return 1;
}

int KOptions3DTextureGourad()
{
	g_MainDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_3DVIEW_WIREFRAME,
		ID_OPTIONS_3DVIEW_TEXTUREGOURAD,
		ID_OPTIONS_3DVIEW_TEXTUREGOURAD,
		MF_BYCOMMAND);

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KOptions3DDrawCycle()
{
	if (g_MainDlg->IsMenuChecked(ID_OPTIONS_3DVIEW_WIREFRAME))
		KOptions3DFlat();
	else
		if (g_MainDlg->IsMenuChecked(ID_OPTIONS_3DVIEW_FLATSHADED))
			KOptions3DGourad();
		else
			if (g_MainDlg->IsMenuChecked(ID_OPTIONS_3DVIEW_GOURADSHADED))
				KOptions3DTexture();
			else
				if (g_MainDlg->IsMenuChecked(ID_OPTIONS_3DVIEW_TEXTUREMAPPED))
					KOptions3DTextureGourad();
				else
					if (g_MainDlg->IsMenuChecked(ID_OPTIONS_3DVIEW_TEXTUREGOURAD))
						KOptions3DWireframe();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	return 1;
}

int KOptions3DShowReference()
{
	if (g_MainDlg)
	{
		g_MainDlg->ToggleMenuChecked(ID_OPTIONS_3DVIEW_SHOWREFERENCEMODEL);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptions3DBackface()
{
	if (g_MainDlg)
	{
		g_MainDlg->ToggleMenuChecked(ID_OPTIONS_2DVIEW_DRAWBACKFACES);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsSyncSkinSelect()
{
	//g_MainDlg->SyncSkinSelection->Click();
	return 1;
}

int KOptionsIncreaseGrid()
{
	g_GridSize++;

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFEEDBACK, MMC_GRIDLABEL, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsDecreaseGrid()
{
	if (g_GridSize > 1)
		g_GridSize--;

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFEEDBACK, MMC_GRIDLABEL, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsZoomIn()
{
	g_Zoom *= (float)1.2;
	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsZoomOut()
{
	g_Zoom /= (float)1.2;
	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KOptionsConfigure()
{
	CPropertySheet    Sheet;
	CKeyConfigPage    KeyConfigPage;
	CDirConfigPage    DirConfigPage;
	CColorConfigPage  ColorConfigPage;
	CUndoConfigPage   UndoConfigPage;

	int               Result;
	BYTE              *NewColors, *OldColors;

	DirConfigPage.m_BasePath = QuakeDataDir;

	NewColors = new BYTE[NUM_COLORS];
	OldColors = new BYTE[NUM_COLORS];

	for (int i = 0; i < NUM_COLORS; i++)
	{
		NewColors[i] = *COLORS[i];
		OldColors[i] = *COLORS[i];
	}

	ColorConfigPage.SetOldNewColors(OldColors, NewColors);

	KeyConfigPage.m_KeyListCopy = g_KeyList;

	UndoConfigPage.m_MainInfo.GetFromUndoList(&g_MainUndo);
	UndoConfigPage.m_SkinInfo.GetFromUndoList(&g_SkinUndo);
	UndoConfigPage.m_PaintInfo.GetFromUndoList(&g_PaintUndo);

	Sheet.AddPage(&UndoConfigPage);
	Sheet.AddPage(&DirConfigPage);
	Sheet.AddPage(&KeyConfigPage);
	Sheet.AddPage(&ColorConfigPage);

	Sheet.SetTitle("Configuration");
	Result = Sheet.DoModal();

	if (Result == IDOK)     // User clicked ok, apply the changes.
	{
		g_KeyList = KeyConfigPage.m_KeyListCopy;
		g_KeyList.UpdateAllMenus();

		QuakeDataDir = DirConfigPage.m_BasePath;

		for (int i = 0; i < NUM_COLORS; i++)
		{
			*COLORS[i] = NewColors[i];
		}

		UndoConfigPage.m_MainInfo.SetToUndoList(&g_MainUndo);
		UndoConfigPage.m_SkinInfo.SetToUndoList(&g_SkinUndo);
		UndoConfigPage.m_PaintInfo.SetToUndoList(&g_PaintUndo);

		g_MainDlg->SaveConfigToReg();
	}

	if (NewColors)
		delete[] NewColors;
	if (OldColors)
		delete[] OldColors;

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	if (g_SkinEditDlg)
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);


	return 1;
}


// Tools Menu

int KToolsGenerateHeader()
{
	//g_MainDlg->GenerateHeaderFile->Click();
	return 1;
}

int KToolsMemoWindow()
{
	if (g_MainDlg)
	{
		g_MainDlg->m_ConsoleDlg.ShowWindow(SW_SHOW);
		g_MainDlg->m_ConsoleDlg.SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	return 1;
}

int KToolsOutputBoundingBox()
{
	float minx, miny, minz, maxx, maxy, maxz;
	CString toadd;

	TRealVertex *V = g_CurMdl.Verts;

	if (!V)
		return 0;

	minx = V[0].x;
	miny = V[0].y;
	minz = V[0].z;
	maxx = V[0].x;
	maxy = V[0].y;
	maxz = V[0].z;
	for (int v = 0; v < g_CurMdl.Head.num_xyz; v++)
	{
		if (V[v].x < minx)
			minx = V[v].x;
		if (V[v].y < miny)
			miny = V[v].y;
		if (V[v].z < minz)
			minz = V[v].z;

		if (V[v].x > maxx)
			maxx = V[v].x;
		if (V[v].y > maxy)
			maxy = V[v].y;
		if (V[v].z > maxz)
			maxz = V[v].z;
	}

	g_MainDlg->AddLineToConsole("");

	toadd.Format("BBox for Current Frame %i - (MinX, Miny, MinZ), (MaxX, MaxY, MaxZ):", g_CurFrame);
	g_MainDlg->AddLineToConsole(toadd);

	toadd.Format("\t(%.4f, %.4f, %.4f), (%.4f, %.4f, %.4f)", minx, miny, minz, maxx, maxy, maxz);
	g_MainDlg->AddLineToConsole(toadd);

	minx = V[0].x;
	miny = V[0].y;
	minz = V[0].z;
	maxx = V[0].x;
	maxy = V[0].y;
	maxz = V[0].z;

	int MinFrame, MaxFrame;
	MinFrame = g_MainDlg->m_ViewTab.m_FromAnimSpin.GetPos();
	MaxFrame = g_MainDlg->m_ViewTab.m_ToAnimSpin.GetPos();

	for (int f = MinFrame; f <= MaxFrame; f++)
	{
		V = g_CurMdl.Frames[f];
		for (int v = 0; v < g_CurMdl.Head.num_xyz; v++)
		{
			if (V[v].x < minx)
				minx = V[v].x;
			if (V[v].y < miny)
				miny = V[v].y;
			if (V[v].z < minz)
				minz = V[v].z;

			if (V[v].x > maxx)
				maxx = V[v].x;
			if (V[v].y > maxy)
				maxy = V[v].y;
			if (V[v].z > maxz)
				maxz = V[v].z;
		}
	}
	g_MainDlg->AddLineToConsole("");

	toadd.Format("BBox for Current Play Range %i - %i (MinX, Miny, MinZ), (MaxX, MaxY, MaxZ):",
		MinFrame, MaxFrame);

	g_MainDlg->AddLineToConsole(toadd);

	toadd.Format("\t(%.4f, %.4f, %.4f), (%.4f, %.4f, %.4f)", minx, miny, minz, maxx, maxy, maxz);
	g_MainDlg->AddLineToConsole(toadd);

	return 1;
}


// Help Menu

int KHelpContents()
{
	//g_MainDlg->Contents->Click();
	return 1;
}

int KHelpKeyword()
{
	//g_MainDlg->KeywordSearch->Click();
	return 1;

}

int KHelpAbout()
{
	//g_MainDlg->About->Click();
	return 1;
}


// Bottom Panel Commands

// Mode Commands
int KModeVertex()
{
	if (g_MainDlg)
	{
		g_MainDlg->m_BottomToolDlg.SetSelectionType(stVertex);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFEEDBACK, MMC_SELECTCOUNTERS, 0);
	}
	return 1;
}

int KModeFace()
{
	if (g_MainDlg)
	{
		g_MainDlg->m_BottomToolDlg.SetSelectionType(stTriangle);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFEEDBACK, MMC_SELECTCOUNTERS, 0);
	}
	return 1;
}

int KModeBones()
{
	if (g_MainDlg)
	{
		g_MainDlg->m_BottomToolDlg.SetSelectionType(stBone);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFEEDBACK, MMC_SELECTCOUNTERS, 0);
	}
	return 1;
}

int KModeCycle()
{
	if (g_MainSelectMode == stVertex)
		KModeFace();
	else
		if (g_MainSelectMode == stTriangle)
			KModeBones();
		else
			if (g_MainSelectMode == stBone)
				KModeVertex();

	return 1;
}

int KModeObject()
{
	if (g_MainDlg)
	{
		if (g_MainDlg->m_BottomToolDlg.m_ObjectModeBut.GetCheck() == 1)
			g_MainDlg->m_BottomToolDlg.m_ObjectModeBut.SetCheck(0);
		else
			g_MainDlg->m_BottomToolDlg.m_ObjectModeBut.SetCheck(1);

	}
	return 1;
}

int KControlLimitX()
{
	g_MainDlg->m_ModifyTab.m_XBut.SetCheck(!g_MainDlg->m_ModifyTab.m_XBut.GetCheck());
	return 1;
}

int KControlLimitY()
{
	g_MainDlg->m_ModifyTab.m_YBut.SetCheck(!g_MainDlg->m_ModifyTab.m_YBut.GetCheck());
	return 1;
}

int KControlLimitZ()
{
	g_MainDlg->m_ModifyTab.m_ZBut.SetCheck(!g_MainDlg->m_ModifyTab.m_ZBut.GetCheck());
	return 1;
}

// Tools
int KToolPan()
{
	g_MainDlg->m_BottomToolDlg.m_PanViewsBut.SendMessage(BM_CLICK);
	return 1;
}

int KToolSelect()
{
	g_MainDlg->m_ModifyTab.m_SelectBut.SendMessage(BM_CLICK);
	return 1;
}

int KToolMove()
{
	g_MainDlg->m_ModifyTab.m_MoveBut.SendMessage(BM_CLICK);
	return 1;
}

int KToolRotate()
{
	g_MainDlg->m_ModifyTab.m_RotateBut.SendMessage(BM_CLICK);
	return 1;
}

int KToolScale()
{
	g_MainDlg->m_ModifyTab.m_ScaleBut.SendMessage(BM_CLICK);
	return 1;
}

int KToolCycle()
{
	if (g_MainDlg->m_ModifyTab.m_SelectBut.GetCheck())
		KToolMove();
	else
		if (g_MainDlg->m_ModifyTab.m_MoveBut.GetCheck())
			KToolRotate();
		else
			if (g_MainDlg->m_ModifyTab.m_RotateBut.GetCheck())
				KToolScale();
			else
				if (g_MainDlg->m_ModifyTab.m_ScaleBut.GetCheck())
					KToolPan();
				else
					if (g_MainDlg->m_BottomToolDlg.m_PanViewsBut.GetCheck())
						KToolSelect();
					else
						return 0;

	return 1;
}

int KToolMirror()
{
	Vector3 centre;

	centre = GetSelectionCentre(g_CurMdl, g_MainSelectMode);

	if (g_MainSelectMode == stVertex)
	{
		for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
			if (g_CurMdl.Verts[n].Selected)
			{
				if (g_MainDlg->m_ModifyTab.m_ZBut.GetCheck())
					g_CurMdl.Verts[n].x = -g_CurMdl.Verts[n].x + centre.x + centre.x;
				if (g_MainDlg->m_ModifyTab.m_XBut.GetCheck())
					g_CurMdl.Verts[n].y = -g_CurMdl.Verts[n].y + centre.y + centre.y;
				if (g_MainDlg->m_ModifyTab.m_YBut.GetCheck())
					g_CurMdl.Verts[n].z = -g_CurMdl.Verts[n].z + centre.z + centre.z;
			}

		if (g_MainDlg->m_ModifyTab.m_EffectFrameRangeCheck.GetCheck())
			for (int f = g_MainDlg->m_ModifyTab.m_ModFromSpin.GetPos(); f < g_MainDlg->m_ModifyTab.m_ModToSpin.GetPos(); f++)
				for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
					if (g_CurMdl.Verts[n].Selected)
					{
						if (g_MainDlg->m_ModifyTab.m_ZBut.GetCheck())
							g_CurMdl.Frames[f][n].x = -g_CurMdl.Frames[f][n].x + centre.x + centre.x;
						if (g_MainDlg->m_ModifyTab.m_XBut.GetCheck())
							g_CurMdl.Frames[f][n].y = -g_CurMdl.Frames[f][n].y + centre.y + centre.y;
						if (g_MainDlg->m_ModifyTab.m_YBut.GetCheck())
							g_CurMdl.Frames[f][n].z = -g_CurMdl.Frames[f][n].z + centre.z + centre.z;
					}
	}
	if (g_MainSelectMode == stTriangle)
	{
		bool  *vertsel;
		int   i, n;

		vertsel = new bool[g_CurMdl.Head.num_xyz];

		for (i = 0; i < g_CurMdl.Head.num_xyz; i++)
			vertsel[i] = false;

		for (i = 0; i < g_CurMdl.Head.num_tris; i++)
			if (g_CurMdl.Tris[i].Selected)
			{
				vertsel[g_CurMdl.Tris[i].Vertices[0]] = true;
				vertsel[g_CurMdl.Tris[i].Vertices[1]] = true;
				vertsel[g_CurMdl.Tris[i].Vertices[2]] = true;
			}

		for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
			if (vertsel[n])
			{
				if (g_MainDlg->m_ModifyTab.m_ZBut.GetCheck())
					g_CurMdl.Verts[n].x = -g_CurMdl.Verts[n].x + centre.x + centre.x;
				if (g_MainDlg->m_ModifyTab.m_XBut.GetCheck())
					g_CurMdl.Verts[n].y = -g_CurMdl.Verts[n].y + centre.y + centre.y;
				if (g_MainDlg->m_ModifyTab.m_YBut.GetCheck())
					g_CurMdl.Verts[n].z = -g_CurMdl.Verts[n].z + centre.z + centre.z;
			}

		if (g_MainDlg->m_ModifyTab.m_EffectFrameRangeCheck.GetCheck())
			for (int f = g_MainDlg->m_ModifyTab.m_ModFromSpin.GetPos(); f < g_MainDlg->m_ModifyTab.m_ModToSpin.GetPos(); f++)
				for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
					if (vertsel[n])
					{
						if (g_MainDlg->m_ModifyTab.m_ZBut.GetCheck())
							g_CurMdl.Frames[f][n].x = -g_CurMdl.Frames[f][n].x + centre.x + centre.x;
						if (g_MainDlg->m_ModifyTab.m_XBut.GetCheck())
							g_CurMdl.Frames[f][n].y = -g_CurMdl.Frames[f][n].y + centre.y + centre.y;
						if (g_MainDlg->m_ModifyTab.m_YBut.GetCheck())
							g_CurMdl.Frames[f][n].z = -g_CurMdl.Frames[f][n].z + centre.z + centre.z;
					}

		delete[] vertsel;
	}
	CalcAllNormals();
	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

void DelBone(TBone *B)
{
	if (B)
		g_CurMdl.Bones.DeleteBone(B);
}

int KToolDelete()
{
	CCompleteUndoNode *Node;
	Node = new CCompleteUndoNode;

	Node->Set("Delete", &g_CurMdl);

	if (g_MainSelectMode == stVertex)
	{
		if (::MessageBox(NULL, "This will delete all the selected vertices and and faces using these. Are you Sure?", "Comfirmation", MB_YESNO) == IDNO)
			return 0;

		int   newverts = 0, newtris = 0, numverts = 0, numtris = 0;
		int   n;
		bool  *vertsdel, *trisdel;

		vertsdel = new bool[g_CurMdl.Head.num_xyz];
		trisdel = new bool[g_CurMdl.Head.num_tris];

		for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
		{
			if (g_CurMdl.Verts[n].Selected)
			{
				vertsdel[n] = true;
			}
			else
			{
				vertsdel[n] = false;
				numverts++;
			}
		}

		for (n = 0; n < g_CurMdl.Head.num_tris; n++)
			if (vertsdel[g_CurMdl.Tris[n].Vertices[0]] ||
				vertsdel[g_CurMdl.Tris[n].Vertices[1]] ||
				vertsdel[g_CurMdl.Tris[n].Vertices[2]])
				trisdel[n] = true;
			else
			{
				trisdel[n] = false;
				numtris++;
			}


		TRealTriangle *tempTris;
		TRealVertex *tempFrames[MAX_FRAMES], *tempVerts;


		tempTris = new TRealTriangle[numtris];

		for (n = 0; n < g_CurMdl.Head.num_frames; n++)
		{
			tempFrames[n] = new TRealVertex[numverts];
		}

		tempVerts = new TRealVertex[numverts];


		for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
		{
			if (vertsdel[n] == false)
			{
				tempVerts[newverts] = g_CurMdl.Verts[n];

				for (int f = 0; f < g_CurMdl.Head.num_frames; f++)
					tempFrames[f][newverts] = g_CurMdl.Frames[f][n];

				for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
				{
					if (g_CurMdl.Tris[t].Vertices[0] == n)
						g_CurMdl.Tris[t].Vertices[0] = (short)newverts;
					if (g_CurMdl.Tris[t].Vertices[1] == n)
						g_CurMdl.Tris[t].Vertices[1] = (short)newverts;
					if (g_CurMdl.Tris[t].Vertices[2] == n)
						g_CurMdl.Tris[t].Vertices[2] = (short)newverts;
				}

				newverts++;
			}
		}

		for (n = 0; n < g_CurMdl.Head.num_tris; n++)
		{
			if (trisdel[n] == false)
			{
				tempTris[newtris] = g_CurMdl.Tris[n];
				newtris++;
			}
		}


		if (g_CurMdl.Tris != NULL)
			delete[] g_CurMdl.Tris;
		g_CurMdl.Tris = NULL;


		if (g_CurMdl.Verts != NULL)
			delete[] g_CurMdl.Verts;
		g_CurMdl.Verts = NULL;

		for (n = 0; n < MAX_FRAMES; n++)
			if (g_CurMdl.Frames[n] != NULL)
			{
				delete[] g_CurMdl.Frames[n];
				g_CurMdl.Frames[n] = NULL;
			}

		g_CurMdl.Tris = tempTris;
		g_CurMdl.Verts = tempVerts;
		for (int f = 0; f < g_CurMdl.Head.num_frames; f++)
		{
			g_CurMdl.Frames[f] = tempFrames[f];
		}

		g_CurMdl.Head.num_xyz = numverts;
		g_CurMdl.Head.num_tris = numtris;

		if (g_MainDlg)
			SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

		if (vertsdel)
			delete[] vertsdel;
		if (trisdel)
			delete[] trisdel;

	}
	else
		if (g_MainSelectMode == stTriangle)
		{
			int DelIsolated = IDNO;

			if (::MessageBox(NULL, "This will delete all the selected faces. Are you Sure?", "Comfirmation", MB_YESNO) == IDNO)
				return 0;

			DelIsolated = ::MessageBox(NULL, "Do you want do delete any Isolated Vertcies?", "Delete Isolated", MB_YESNO);
			if (DelIsolated == IDNO)
				DelIsolated = 0;

			int   newtris, numtris = 0, numverts;
			int   n;
			bool  *trisdel;

			trisdel = new bool[g_CurMdl.Head.num_tris];

			for (n = 0; n < g_CurMdl.Head.num_tris; n++)
				if (g_CurMdl.Tris[n].Selected)
					trisdel[n] = true;
				else
				{
					trisdel[n] = false;
					numtris++;
				}

			TRealTriangle	*tempTris;
			TRealVertex		*tempVerts;
			short				*targVerts;

			tempTris = new TRealTriangle[numtris];

			newtris = 0;
			for (n = 0; n < g_CurMdl.Head.num_tris; n++)
			{
				if (trisdel[n] == false)
				{
					tempTris[newtris] = g_CurMdl.Tris[n];
					newtris++;
				}
			}

			if (g_CurMdl.Tris != NULL)
				delete[] g_CurMdl.Tris;

			g_CurMdl.Tris = tempTris;

			g_CurMdl.Head.num_tris = numtris;


			if (DelIsolated)
			{
				bool *vertsdel;

				vertsdel = new bool[g_CurMdl.Head.num_xyz];

				for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
					vertsdel[n] = true;

				for (n = 0; n < g_CurMdl.Head.num_tris; n++)
				{
					vertsdel[g_CurMdl.Tris[n].Vertices[0]] = false;
					vertsdel[g_CurMdl.Tris[n].Vertices[1]] = false;
					vertsdel[g_CurMdl.Tris[n].Vertices[2]] = false;
				}

				numverts = g_CurMdl.Head.num_xyz;

				for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
					if (vertsdel[n])
						numverts--;

				targVerts = new short[g_CurMdl.Head.num_xyz];

				{
					int t = 0;
					for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
						if (vertsdel[n] == false)
						{
							targVerts[n] = (short)t;
							t++;
						}
				}


				{
					int t = 0;

					tempVerts = new TRealVertex[numverts];

					for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
						if (vertsdel[n] == false)
						{
							tempVerts[t] = g_CurMdl.Verts[n];
							t++;
						}

					delete[] g_CurMdl.Verts;
					g_CurMdl.Verts = tempVerts;
				}

				for (int f = 0; f < g_CurMdl.Head.num_frames; f++)
				{
					int t = 0;

					tempVerts = new TRealVertex[numverts];

					for (int n = 0; n < g_CurMdl.Head.num_xyz; n++)
						if (vertsdel[n] == false)
						{
							tempVerts[t] = g_CurMdl.Frames[f][n];
							t++;
						}

					delete[] g_CurMdl.Frames[f];
					g_CurMdl.Frames[f] = tempVerts;
				}

				for (n = 0; n < g_CurMdl.Head.num_tris; n++)
				{
					tempTris[n].Vertices[0] = targVerts[g_CurMdl.Tris[n].Vertices[0]];
					tempTris[n].Vertices[1] = targVerts[g_CurMdl.Tris[n].Vertices[1]];
					tempTris[n].Vertices[2] = targVerts[g_CurMdl.Tris[n].Vertices[2]];
				}
				g_CurMdl.Head.num_xyz = numverts;

				delete[] targVerts;
				delete[] vertsdel;
			}

			if (g_MainDlg)
			{
				SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
				SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
			}

			if (trisdel)
				delete[] trisdel;
		}
		else
			if (g_MainSelectMode == stBone)
			{
				if (g_CurMdl.Bones.AnythingSelected())
				{
					g_CurMdl.Bones.ApplyFuncToSelected(DelBone);
					if (g_MainDlg)
					{
						SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
						SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
					}
				}
				else
				{
					delete Node;
					return 0;
				}
			}
			else
			{
				delete Node;
				return 0;
			}
	g_MainUndo.AddNode(Node);
	g_SkinUndo.ClearAllNodes();
	g_SkinRedo.ClearAllNodes();
	return 1;
}

int KToolFlipNormal()
{
	for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
		if (g_CurMdl.Tris[i].Selected)
		{
			short v0, v1, v2;

			v0 = g_CurMdl.Tris[i].Vertices[0];
			v1 = g_CurMdl.Tris[i].Vertices[1];
			v2 = g_CurMdl.Tris[i].Vertices[2];

			g_CurMdl.Tris[i].Vertices[0] = v2;
			g_CurMdl.Tris[i].Vertices[1] = v1;
			g_CurMdl.Tris[i].Vertices[2] = v0;

			v0 = g_CurMdl.Tris[i].SkinVerts[0];
			v1 = g_CurMdl.Tris[i].SkinVerts[1];
			v2 = g_CurMdl.Tris[i].SkinVerts[2];

			g_CurMdl.Tris[i].SkinVerts[0] = v2;
			g_CurMdl.Tris[i].SkinVerts[1] = v1;
			g_CurMdl.Tris[i].SkinVerts[2] = v0;
		}

	g_CurMdl.CalcNormals(CALCNORMS_SELECTED);

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	return 1;
}

int KToolTurn()
{
	//g_MainDlg->TurnEdgeBut->Click();
	return 1;
}

int KToolSubdivide()
{
	if (g_MainSelectMode != stTriangle)
		return 0;

	CCompleteUndoNode *Node;
	Node = new CCompleteUndoNode;

	Node->Set("Subdivide", &g_CurMdl);

	// do subdivision
	int   NumNewXYZ = 0, *TriToVert, *TriToST, NumNewST = 0;
	bool  *TriSel;
	bool  *DoSkin;  // this is always a subset of TriSel, indicating if the triangles contain a valid skin triangle or not.

	TriSel = new bool[g_CurMdl.Head.num_tris];
	DoSkin = new bool[g_CurMdl.Head.num_tris];
	TriToVert = new int[g_CurMdl.Head.num_tris];
	TriToST = new int[g_CurMdl.Head.num_tris];

	g_CurMdl.LoadSelection(TriSel, ssTriangle);

	for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
	{
		DoSkin[t] = false;
		if (TriSel[t])
			NumNewXYZ++;
		{
			if (g_CurMdl.Tris[t].SkinVerts[0] != g_CurMdl.Tris[t].SkinVerts[1] &&
				g_CurMdl.Tris[t].SkinVerts[1] != g_CurMdl.Tris[t].SkinVerts[2] &&
				g_CurMdl.Tris[t].SkinVerts[2] != g_CurMdl.Tris[t].SkinVerts[0])
				DoSkin[t] = true;
		}
	}

	{
		int Targ = 0;
		for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
			if (TriSel[t])
			{
				TriToVert[t] = g_CurMdl.Head.num_xyz + Targ;
				TriToST[t] = g_CurMdl.Head.num_st + Targ;
				Targ++;
			}
			else
			{
				TriToVert[t] = -1;
				TriToST[t] = -1;
			}
	}

	// Add in the vertices for all the frames
	// todo: add in skin vertices addition

	{
		for (int F = 0; F < g_CurMdl.Head.num_frames; F++)
		{
			TRealVertex *V;
			V = new TRealVertex[g_CurMdl.Head.num_xyz + NumNewXYZ];

			memcpy(V, g_CurMdl.Frames[F], sizeof(TRealVertex)*g_CurMdl.Head.num_xyz);

			for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
				if (TriSel[t])
				{
					double targx, targy, targz;
					targx = targy = targz = 0.0;

					targx += g_CurMdl.Frames[F][g_CurMdl.Tris[t].Vertices[0]].x;
					targx += g_CurMdl.Frames[F][g_CurMdl.Tris[t].Vertices[1]].x;
					targx += g_CurMdl.Frames[F][g_CurMdl.Tris[t].Vertices[2]].x;
					targy += g_CurMdl.Frames[F][g_CurMdl.Tris[t].Vertices[0]].y;
					targy += g_CurMdl.Frames[F][g_CurMdl.Tris[t].Vertices[1]].y;
					targy += g_CurMdl.Frames[F][g_CurMdl.Tris[t].Vertices[2]].y;
					targz += g_CurMdl.Frames[F][g_CurMdl.Tris[t].Vertices[0]].z;
					targz += g_CurMdl.Frames[F][g_CurMdl.Tris[t].Vertices[1]].z;
					targz += g_CurMdl.Frames[F][g_CurMdl.Tris[t].Vertices[2]].z;

					V[TriToVert[t]].x = (float)(targx / 3.0);
					V[TriToVert[t]].y = (float)(targy / 3.0);
					V[TriToVert[t]].z = (float)(targz / 3.0);
				}

			delete[] g_CurMdl.Frames[F];
			g_CurMdl.Frames[F] = V;
		}

		TRealVertex *V;
		V = new TRealVertex[g_CurMdl.Head.num_xyz + NumNewXYZ];

		memcpy(V, g_CurMdl.Frames[g_CurFrame], sizeof(TRealVertex)*(g_CurMdl.Head.num_xyz + NumNewXYZ));

		delete[] g_CurMdl.Verts;
		g_CurMdl.Verts = V;
	}

	// set up the skin vertices
	{
		int t;

		for (t = 0; t < g_CurMdl.Head.num_tris; t++)
			if (DoSkin[t])
				NumNewST++;

		TRealSkinVertex *V;
		V = new TRealSkinVertex[g_CurMdl.Head.num_st + NumNewST];

		memcpy(V, g_CurMdl.SkinVerts, sizeof(TRealSkinVertex)*g_CurMdl.Head.num_st);

		for (t = 0; t < g_CurMdl.Head.num_tris; t++)
			if (DoSkin[t])
			{
				double targs, targt;
				targs = targt = 0.0;

				targs += g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[0]].s;
				targs += g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[1]].s;
				targs += g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[2]].s;
				targt += g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[0]].t;
				targt += g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[1]].t;
				targt += g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[2]].t;

				V[TriToST[t]].s = (float)(targs / 3.0);
				V[TriToST[t]].t = (float)(targt / 3.0);
			}

		delete[] g_CurMdl.SkinVerts;
		g_CurMdl.SkinVerts = V;
	}

	// set up the new triangles, and reorganise the old ones.
	// todo: add in skin triangle set up

	{
		int TrisDone = 0;

		TRealTriangle  *T;

		T = new TRealTriangle[g_CurMdl.Head.num_tris + NumNewXYZ * 2];

		for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
			if (TriSel[t])
			{
				int NewTri1, NewTri2;
				NewTri1 = g_CurMdl.Head.num_tris + TrisDone++;
				NewTri2 = g_CurMdl.Head.num_tris + TrisDone++;

				T[t].Vertices[0] = g_CurMdl.Tris[t].Vertices[0];
				T[t].Vertices[1] = TriToVert[t];
				T[t].Vertices[2] = g_CurMdl.Tris[t].Vertices[2];

				T[NewTri1].Vertices[0] = g_CurMdl.Tris[t].Vertices[0];
				T[NewTri1].Vertices[1] = g_CurMdl.Tris[t].Vertices[1];
				T[NewTri1].Vertices[2] = TriToVert[t];

				T[NewTri2].Vertices[0] = TriToVert[t];
				T[NewTri2].Vertices[1] = g_CurMdl.Tris[t].Vertices[1];
				T[NewTri2].Vertices[2] = g_CurMdl.Tris[t].Vertices[2];

				T[t].SkinVerts[0] = g_CurMdl.Tris[t].SkinVerts[0];
				T[t].SkinVerts[1] = g_CurMdl.Tris[t].SkinVerts[1];
				T[t].SkinVerts[2] = g_CurMdl.Tris[t].SkinVerts[2];

				if (DoSkin[t])
				{
					T[t].SkinVerts[0] = g_CurMdl.Tris[t].SkinVerts[0];
					T[t].SkinVerts[1] = TriToST[t];
					T[t].SkinVerts[2] = g_CurMdl.Tris[t].SkinVerts[2];

					T[NewTri1].SkinVerts[0] = g_CurMdl.Tris[t].SkinVerts[0];
					T[NewTri1].SkinVerts[1] = g_CurMdl.Tris[t].SkinVerts[1];
					T[NewTri1].SkinVerts[2] = TriToST[t];

					T[NewTri2].SkinVerts[0] = TriToST[t];
					T[NewTri2].SkinVerts[1] = g_CurMdl.Tris[t].SkinVerts[1];
					T[NewTri2].SkinVerts[2] = g_CurMdl.Tris[t].SkinVerts[2];
				}
			}
			else
				T[t] = g_CurMdl.Tris[t];

		delete[] g_CurMdl.Tris;
		g_CurMdl.Tris = T;
	}


	g_CurMdl.Head.num_xyz += NumNewXYZ;
	g_CurMdl.Head.num_st += NumNewST;
	g_CurMdl.Head.num_tris += NumNewXYZ * 2;

	delete[] TriToVert;
	delete[] TriSel;
	delete[] DoSkin;
	delete[] TriToST;

	g_MainUndo.AddNode(Node);
	g_SkinUndo.ClearAllNodes();
	g_SkinRedo.ClearAllNodes();

	g_CurMdl.SetCurrentFrame(g_CurFrame);
	CalcAllNormals();
	SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
	SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);

	return 1;
}

int KToolExtrude()
{
	g_ChangesMade = true;
	CExtrudeDistDlg   ExtrudeDistDlg;

	if (ExtrudeDistDlg.DoModal() == IDCANCEL)
		return 0;

	if (AnythingSelected() == false)
		return 0;

	CCompleteUndoNode *Node;
	Node = new CCompleteUndoNode;

	Node->Set("Extrude", &g_CurMdl);

	ExtrudeSelected(ExtrudeDistDlg.m_ExtrudeDist);
	for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
		g_CurMdl.Verts[i].Selected = false;

	g_MainUndo.AddNode(Node);

	g_SkinUndo.ClearAllNodes();
	g_SkinRedo.ClearAllNodes();

	CalcAllNormals();

	SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
	SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);

	return 1;
}

int KToolWeld()
{
	if (g_MainSelectMode != stVertex)
		return 0;

	g_CurMdl.UpdateFrame(g_CurFrame);

	bool  *Sel;
	int   NumSel = 0, *TargVert, First = -1, NumNewVerts;
	double Newx = 0, Newy = 0, Newz = 0;

	Sel = new bool[g_CurMdl.Head.num_xyz];
	memset(Sel, 0, g_CurMdl.Head.num_xyz * sizeof(bool));

	g_CurMdl.LoadSelection(Sel, ssVertex);
	for (int v = 0; v < g_CurMdl.Head.num_xyz; v++)
	{
		if (Sel[v])
		{
			NumSel++;
			if (First == -1)
				First = v;

			Newx += g_CurMdl.Verts[v].x;
			Newy += g_CurMdl.Verts[v].y;
			Newz += g_CurMdl.Verts[v].z;
		}
	}

	if (NumSel == 0)
		return 0;

	Newx /= NumSel;
	Newy /= NumSel;
	Newz /= NumSel;

	TargVert = new int[g_CurMdl.Head.num_xyz];

	{
		int t = 0;
		for (int v = 0; v < g_CurMdl.Head.num_xyz; v++)
		{
			if (Sel[v] && v != First)
				TargVert[v] = First;
			else
			{
				TargVert[v] = t;
				t++;
			}
		}

		NumNewVerts = t;
	}

	CCompleteUndoNode *Node;
	Node = new CCompleteUndoNode;

	Node->Set("Weld Vertices", &g_CurMdl);

	for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
	{
		g_CurMdl.Tris[t].Vertices[0] = TargVert[g_CurMdl.Tris[t].Vertices[0]];
		g_CurMdl.Tris[t].Vertices[1] = TargVert[g_CurMdl.Tris[t].Vertices[1]];
		g_CurMdl.Tris[t].Vertices[2] = TargVert[g_CurMdl.Tris[t].Vertices[2]];
	}

	for (int F = 0; F < g_CurMdl.Head.num_frames; F++)
	{
		TRealVertex *V;

		V = new TRealVertex[NumNewVerts];

		Newx = Newy = Newz = 0;

		for (int v = 0; v < g_CurMdl.Head.num_xyz; v++)
		{
			V[TargVert[v]] = g_CurMdl.Frames[F][v];
			if (TargVert[v] == First)
			{
				Newx += g_CurMdl.Frames[F][v].x;
				Newy += g_CurMdl.Frames[F][v].y;
				Newz += g_CurMdl.Frames[F][v].z;
			}
		}
		Newx /= NumSel;
		Newy /= NumSel;
		Newz /= NumSel;

		V[First].x = (float)Newx;
		V[First].y = (float)Newy;
		V[First].z = (float)Newz;

		delete[] g_CurMdl.Frames[F];
		g_CurMdl.Frames[F] = V;
	}
	delete[] g_CurMdl.Verts;
	g_CurMdl.Verts = new TRealVertex[NumNewVerts];
	g_CurMdl.Head.num_xyz = NumNewVerts;

	delete[] TargVert;
	delete[] Sel;

	g_MainUndo.AddNode(Node);

	g_SkinUndo.ClearAllNodes();
	g_SkinRedo.ClearAllNodes();

	g_CurMdl.SetCurrentFrame(g_CurFrame);
	g_CurMdl.CalcNormals();

	g_CurMdl.Verts[First].Selected = true;

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KMainInterpolationToggle()
{
	//   g_MainDlg->InterpolationBut->Down = !g_MainDlg->InterpolationBut->Down;
	return 1;
}

int KEffectFrameRangeToggle()
{
	//   g_MainDlg->EffectFrameRange->Checked = !g_MainDlg->EffectFrameRange->Checked;
	return 1;
}

// view control
int KControlNextFrame()
{
	// Are the viewports currently playing?
	if (!g_Playing)
	{
		// Update the frame vertex buffer for the current frame
		g_CurMdl.UpdateFrame(g_CurFrame);
	}

	g_CurFrame++;
	if (g_CurFrame == g_CurMdl.Head.num_frames)
		g_CurFrame = 0;

	g_CurMdl.SetCurrentFrame(g_CurFrame);
	if (g_pRef)
		g_pRef->SetCurrentFrame(g_CurFrame);

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);


	CalcAllNormals();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KControlPrevFrame()
{

	// Are the viewports currently playing?
	if (!g_Playing)
	{
		// Update the frame vertex buffer for the current frame
		g_CurMdl.UpdateFrame(g_CurFrame);
	}

	g_CurFrame--;
	if (g_CurFrame == -1)
		g_CurFrame = g_CurMdl.Head.num_frames - 1;

	g_CurMdl.SetCurrentFrame(g_CurFrame);
	if (g_pRef)
		g_pRef->SetCurrentFrame(g_CurFrame);

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);


	CalcAllNormals();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KControlLastFrame()
{
	// Are the viewports currently playing?
	if (!g_Playing)
	{
		// Update the frame vertex buffer for the current frame
		g_CurMdl.UpdateFrame(g_CurFrame);
	}

	g_CurFrame = g_CurMdl.Head.num_frames - 1;

	g_CurMdl.SetCurrentFrame(g_CurFrame);
	if (g_pRef)
		g_pRef->SetCurrentFrame(g_CurFrame);

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);


	CalcAllNormals();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KControlFirstFrame()
{
	// Are the viewports currently playing?
	if (!g_Playing)
	{
		// Update the frame vertex buffer for the current frame
		g_CurMdl.UpdateFrame(g_CurFrame);
	}

	g_CurFrame = 0;

	g_CurMdl.SetCurrentFrame(g_CurFrame);
	if (g_pRef)
		g_pRef->SetCurrentFrame(g_CurFrame);

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFRAMELABEL, 0, 0);


	CalcAllNormals();

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KControlFitAll()
{
	int n, size;
	float minx, maxx, miny, maxy, minz, maxz, zoomx, zoomy, zoomz;
	bool *vis, NoSel = true;

	vis = new bool[g_CurMdl.Head.num_xyz];

	for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
		vis[i] = false;


	if (g_MainSelectMode == stVertex)
		for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
			vis[i] = g_CurMdl.Verts[i].Visible;

	if (g_MainSelectMode == stTriangle)
		for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
			if (g_CurMdl.Tris[i].Visible)
			{
				vis[g_CurMdl.Tris[i].Vertices[0]] = true;
				vis[g_CurMdl.Tris[i].Vertices[1]] = true;
				vis[g_CurMdl.Tris[i].Vertices[2]] = true;
			}


	minx = miny = minz = 100000;
	maxx = maxy = maxz = -100000;

	for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
		if (vis[n])
		{
			NoSel = false;
			if (g_CurMdl.Verts[n].x < minx) minx = g_CurMdl.Verts[n].x;
			if (g_CurMdl.Verts[n].x > maxx) maxx = g_CurMdl.Verts[n].x;
			if (g_CurMdl.Verts[n].y < miny) miny = g_CurMdl.Verts[n].y;
			if (g_CurMdl.Verts[n].y > maxy) maxy = g_CurMdl.Verts[n].y;
			if (g_CurMdl.Verts[n].z < minz) minz = g_CurMdl.Verts[n].z;
			if (g_CurMdl.Verts[n].z > maxz) maxz = g_CurMdl.Verts[n].z;
		}

	int vwidth, vheight;

	vwidth = VWidthL;
	if (VWidthR < vwidth)
		vwidth = VWidthR;

	vheight = VHeightT;
	if (VHeightB < vheight)
		vheight = VHeightB;


	if (vheight > vwidth) size = vwidth;
	else size = vheight;

	if (NoSel)
	{
		PanX = PanY = PanZ = 0;
		g_Zoom = (float)(size / (260));
	}
	else
	{
		PanX = (minx + maxx) / 2;
		PanY = (miny + maxy) / 2;
		PanZ = -(minz + maxz) / 2;

		if (maxz - minz == 0)
		{
			minz = -10;
			maxz = 10;
		}
		if (maxx - minx == 0)
		{
			minx = -10;
			maxx = 10;
		}
		if (maxy - miny == 0)
		{
			miny = -10;
			maxy = 10;
		}

		zoomx = (float)(0.8*size / (maxx - minx));
		zoomy = (float)(0.8*size / (maxy - miny));
		zoomz = (float)(0.8*size / (maxz - minz));
		g_Zoom = zoomx;
		if (zoomy < g_Zoom) g_Zoom = zoomy;
		if (zoomz < g_Zoom) g_Zoom = zoomz;
	}

	if (g_Zoom == 0)
		g_Zoom = 5;

	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	delete[] vis;

	return 1;
}

int KControlFitSelected()
{
	int n, size;
	float minx, maxx, miny, maxy, minz, maxz, zoomx, zoomy, zoomz;
	bool *vis, NoSel = true;


	vis = new bool[g_CurMdl.Head.num_xyz];
	for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
		vis[i] = false;

	if (g_MainSelectMode == stVertex)
		for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
			vis[i] = g_CurMdl.Verts[i].Visible && g_CurMdl.Verts[i].Selected;

	if (g_MainSelectMode == stTriangle)
		for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
			if (g_CurMdl.Tris[i].Visible && g_CurMdl.Tris[i].Selected)
			{
				vis[g_CurMdl.Tris[i].Vertices[0]] = true;
				vis[g_CurMdl.Tris[i].Vertices[1]] = true;
				vis[g_CurMdl.Tris[i].Vertices[2]] = true;
			}


	minx = miny = minz = 100000;
	maxx = maxy = maxz = -100000;

	for (n = 0; n < g_CurMdl.Head.num_xyz; n++)
		if (vis[n])
		{
			NoSel = false;
			if (g_CurMdl.Verts[n].x < minx) minx = g_CurMdl.Verts[n].x;
			if (g_CurMdl.Verts[n].x > maxx) maxx = g_CurMdl.Verts[n].x;
			if (g_CurMdl.Verts[n].y < miny) miny = g_CurMdl.Verts[n].y;
			if (g_CurMdl.Verts[n].y > maxy) maxy = g_CurMdl.Verts[n].y;
			if (g_CurMdl.Verts[n].z < minz) minz = g_CurMdl.Verts[n].z;
			if (g_CurMdl.Verts[n].z > maxz) maxz = g_CurMdl.Verts[n].z;
		}

	int vwidth, vheight;

	vwidth = VWidthL;
	if (VWidthR < vwidth)
		vwidth = VWidthR;

	vheight = VHeightT;
	if (VHeightB < vheight)
		vheight = VHeightB;


	if (vheight > vwidth) size = vwidth;
	else size = vheight;


	if (NoSel)
	{
		KControlFitAll();
	}
	else
	{
		PanX = (minx + maxx) / 2;
		PanY = (miny + maxy) / 2;
		PanZ = -(minz + maxz) / 2;

		zoomx = (float)(0.8*size / (maxx - minx));
		zoomy = (float)(0.8*size / (maxy - miny));
		zoomz = (float)(0.8*size / (maxz - minz));


		g_Zoom = zoomx;
		if (zoomy < g_Zoom) g_Zoom = zoomy;
		if (zoomz < g_Zoom) g_Zoom = zoomz;
	}
	if (g_MainDlg)
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KControlPlayCamera()
{
	//   g_MainDlg->PlayBut->Click();
	return 1;
}

int KControlHideSelected()
{
	if (g_MainSelectMode == stVertex)
	{
		for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
			if (g_CurMdl.Verts[i].Selected)
			{
				g_CurMdl.Verts[i].Visible = false;
				g_CurMdl.Verts[i].Selected = false;
			}
	}
	if (g_MainSelectMode == stTriangle)
	{
		for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
			if (g_CurMdl.Tris[i].Selected)
			{
				g_CurMdl.Tris[i].Visible = false;
				g_CurMdl.Tris[i].Selected = false;
			}
	}
	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFEEDBACK, MMC_SELECTCOUNTERS, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
	}

	return 1;
}

int KControlHideUnSelected()
{
	if (g_MainSelectMode == stVertex)
	{
		for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
			if (!g_CurMdl.Verts[i].Selected)
			{
				g_CurMdl.Verts[i].Visible = false;
				g_CurMdl.Verts[i].Selected = false;
			}
	}
	if (g_MainSelectMode == stTriangle)
	{
		for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
			if (!g_CurMdl.Tris[i].Selected)
			{
				g_CurMdl.Tris[i].Visible = false;
				g_CurMdl.Tris[i].Selected = false;
			}
	}
	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFEEDBACK, MMC_SELECTCOUNTERS, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
	}

	return 1;
}

int KControlUnhide()
{
	if (g_MainSelectMode == stVertex)
	{
		for (int i = 0; i < g_CurMdl.Head.num_xyz; i++)
			g_CurMdl.Verts[i].Visible = true;
	}
	if (g_MainSelectMode == stTriangle)
	{
		for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
			g_CurMdl.Tris[i].Visible = true;
	}
	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEFEEDBACK, MMC_SELECTCOUNTERS, 0);
		SendMessage(g_MainDlg->m_hWnd, MM_DOSYNC, 0, 0);
	}

	return 1;
}


// create
int KCreateVertex()
{
	//    g_MainDlg->CreateVertexBut->Click();
	return 1;
}

int KCreateFace()
{
	//   g_MainDlg->BuildFaceBut->Click();
	return 1;
}

int KCreateFan()
{
	//   g_MainDlg->CreateFanBut->Click();
	return 1;
}

int KCreateStrip()
{
	//   g_MainDlg->CreateStripBut->Click();
	return 1;
}

//--------------------------------------------------------------------------
//   SKIN EDITOR FUNCTIONS
//--------------------------------------------------------------------------

int KImportSkinImage()
{
	CFileDialog Dlg(TRUE, "PCX", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "PCX Files (*.PCX)|*.PCX||");

	if (LastModelDir == "")
		LastModelDir = QuakeDataDir;

	Dlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastModelDir;

	if (g_CurMdl.Head.num_skins == 0)
	{
		MessageBox(NULL, "There are no skins in this model.", "Error", MB_OK);
		return 0;
	}

	if (Dlg.DoModal() == IDCANCEL)
		return 0;

	{
		char Drive[4], Dir[265];
		_splitpath((LPCTSTR)Dlg.GetFileExt(), Drive, Dir, NULL, NULL);
		//      LastModelDir = AnsiString(Drive) + AnsiString(Dir);
	}

	TDIBitmap8	temp;

	if (temp.LoadFromPCX(Dlg.GetFileName()) == 0)
		return 0;

	if (temp.Width != g_CurMdl.Head.skinwidth || temp.Width != g_CurMdl.Head.skinwidth)
	{
		int   res = MessageBox(NULL, "Bitmap's width and height are not the same as the current skin, continue?", "Size difference", MB_OKCANCEL);
		int   i;
		if (res == IDCANCEL)
			return 0;

		for (i = 0; i < g_CurMdl.Head.num_skins; i++)
		{
			unsigned char *dest, *d, *s;

			dest = new unsigned char[temp.Width * temp.Height];

			int maxy, maxx;

			maxy = temp.Height;
			maxx = temp.Width;
			if (g_CurMdl.Head.skinheight < maxy)
				maxy = g_CurMdl.Head.skinheight;
			if (g_CurMdl.Head.skinwidth < maxx)
				maxx = g_CurMdl.Head.skinwidth;

			d = dest;
			s = g_CurMdl.Skins[i];
			for (int y = 0; y < maxy; y++)
			{
				memcpy(d, s, maxx);
				d += temp.Width;
				//            d+=temp.Width%4;
				s += g_CurMdl.Head.skinwidth;
			}

			delete[] g_CurMdl.Skins[i];
			g_CurMdl.Skins[i] = dest;
		}

		float ratx, raty;

		ratx = (float)temp.Width / g_CurMdl.Head.skinwidth;
		raty = (float)temp.Height / g_CurMdl.Head.skinheight;

		for (i = 0; i < g_CurMdl.Head.num_st; i++)
		{
			g_CurMdl.SkinVerts[i].s *= ratx;
			g_CurMdl.SkinVerts[i].t *= raty;
		}

		g_CurMdl.Head.skinwidth = temp.Width;
		g_CurMdl.Head.skinheight = temp.Height;
		//	   Views[3].SetTexSize(g_CurMdl.Head.skinwidth, g_CurMdl.Head.skinheight);
		//   	SkinPainterForm->View.SetTexSize(g_CurMdl.Head.skinwidth, g_CurMdl.Head.skinheight);
	}

	unsigned char *src, *dest;

	src = temp.Data;
	dest = g_CurMdl.Skins[g_CurSkin];
	for (int i = 0; i < g_CurMdl.Head.skinheight; i++)
	{
		memcpy(dest, src, g_CurMdl.Head.skinwidth);
		src += temp.Width;
		src += temp.Width % 4;
		dest += g_CurMdl.Head.skinwidth;
	}

	if (g_SkinEditDlg)
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATESKIN, 0, 0);

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_CAM, 0);
	}

	return 1;
}

int KImportSkinMesh()
{
	CFileDialog Dlg(TRUE, "PCX", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "PCX Files (*.PCX)|*.PCX||");

	if (LastModelDir == "")
		LastModelDir = QuakeDataDir;

	Dlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastModelDir;

	if (g_CurMdl.LoadSkinMeshFromASC(Dlg.GetFileName()) == 0)
		return 0;

	if (g_SkinEditDlg)
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATESKIN, 0, 0);

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_CAM, 0);
	}

	return 1;

}

int KExportImageBMP()
{
	CFileDialog SaveDlg(FALSE, "BMP", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap Files (*.BMP)|*.BMP||");

	if (LastModelDir == "")
		LastModelDir = QuakeDataDir;

	SaveDlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastModelDir;

	if (SaveDlg.DoModal() == IDCANCEL)
		return 0;

	g_SkinEditDlg->m_SkinPic.SaveToBMP(SaveDlg.GetFileName());

	return 1;
}

int KExportImagePCX()
{
	CFileDialog SaveDlg(FALSE, "PCX", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap Files (*.PCX)|*.PCX||");

	if (LastModelDir == "")
		LastModelDir = QuakeDataDir;
	SaveDlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastModelDir;

	if (SaveDlg.DoModal() == IDCANCEL)
		return 0;

	g_SkinEditDlg->m_SkinPic.SaveToPCX(SaveDlg.GetFileName());

	return 1;
}

int KExportSkinMesh()
{
	CFileDialog SaveDlg(FALSE, "ASC", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ASCII Mesh files (*.ASC)|*.ASC||");

	if (LastModelDir == "")
		LastModelDir = QuakeDataDir;
	SaveDlg.m_ofn.lpstrInitialDir = (LPCTSTR)LastModelDir;

	if (SaveDlg.DoModal() == IDCANCEL)
		return 0;

	g_CurMdl.ExportSkinMeshToASC(SaveDlg.GetFileName());

	return 1;
}

int KSkinEditorExit()
{
	//   g_SkinEditDlg->CloseSkinEdit->Click();
	return 1;
}


int KSkinUndo()
{
	g_SkinUndo.UndoLast();

	if (g_MainDlg)
	{
		if (g_MainDlg->IsMenuChecked(ID_OPTIONS_3DVIEW_TEXTUREMAPPED) ||
			g_MainDlg->IsMenuChecked(ID_OPTIONS_3DVIEW_TEXTUREGOURAD))
		{
			SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
		}
	}

	if (g_SkinEditDlg)
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);

	return 1;
}

int KSkinRedo()
{
	g_SkinRedo.UndoLast();

	if (g_MainDlg)
	{
		if (g_MainDlg->IsMenuChecked(ID_OPTIONS_3DVIEW_TEXTUREMAPPED) ||
			g_MainDlg->IsMenuChecked(ID_OPTIONS_3DVIEW_TEXTUREGOURAD))
		{
			SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
		}
	}

	if (g_SkinEditDlg)
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);

	return 1;
}

int KSkinCopy()
{
	//   g_SkinEditDlg->Copy->Click();
	return 0;
}

int KSkinPaste()
{
	//   g_SkinEditDlg->Paste->Click();
	return 0;
}

int KGetPosition()
{
	CSkinVertsFromProjectionDlg   GetPositionDlg;

	if (GetPositionDlg.DoModal() == IDCANCEL)
		return 0;

	CSkinVertsUndoNode *Node;
	Node = new CSkinVertsUndoNode;
	Node->Set("Get Position", &g_CurMdl);

	SkinVertPos	SVP;

	SVP = GetPositionDlg.m_ProjectionType;

	float    t, l, r, b;

	t = 10;
	l = 10;
	r = 100;
	b = 100;


	//	GetSkinVerticesFrom3DVertices(SVP, GetPositionForm->ToleranceUpDown->Position);
	if (g_CurMdl.Head.skinwidth < 105)
		r = (float)(g_CurMdl.Head.skinwidth - 10);
	if (g_CurMdl.Head.skinheight < b)
		b = (float)(g_CurMdl.Head.skinheight - 10);

	g_CurMdl.GetSkinVerticesFrom3DVertices(SVP, GetPositionDlg.m_Tolerance, l, t, r, b, GetPositionDlg.m_Mirror);


	g_ChangesMade = 1;

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}
	if (g_MainDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_CAM, 0);
	}

	g_SkinUndo.AddNode(Node);

	return 1;
}

int KAddSkin()
{
	if (g_CurMdl.Head.num_skins == MAX_MD2SKINS)
	{
		MessageBox(NULL, "Too many skins. Cannot add another.", "Error", MB_OK);
		return 0;
	}

	if (g_CurMdl.Head.num_skins == 0)
	{
		g_CurMdl.Head.num_skins = 1;
		g_CurMdl.Skins[0] = new unsigned char[g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight];
		memset(g_CurMdl.Skins[0], 0, g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight);
		g_CurSkin = 0;
		//      SkinLabel->Caption = "0";
		//      DeleteCurrentSkin->Enabled = true;
	}
	else
	{
		CInsertSkinDlg    InsertSkinDlg;

		InsertSkinDlg.m_MaxSkin = g_CurMdl.Head.num_skins - 1;
		InsertSkinDlg.m_MinSkin = 0;

		if (InsertSkinDlg.DoModal() == IDCANCEL)
			return 0;

		if (InsertSkinDlg.m_InsertAfter == false)
		{
			for (int i = g_CurMdl.Head.num_skins; i > 0; i--)
			{
				g_CurMdl.Skins[i] = g_CurMdl.Skins[i - 1];
			}

			g_CurMdl.Skins[0] = new unsigned char[g_CurMdl.Head.skinwidth * g_CurMdl.Head.skinheight];
			memcpy(g_CurMdl.Skins[0], g_CurMdl.Skins[g_CurSkin], g_CurMdl.Head.skinwidth * g_CurMdl.Head.skinheight);
			g_CurMdl.Head.num_skins++;
		}
		else
			if (InsertSkinDlg.m_InsertAfter && InsertSkinDlg.m_InsertSkin == g_CurMdl.Head.num_skins - 1)
			{
				g_CurMdl.Skins[g_CurMdl.Head.num_skins] = new unsigned char[g_CurMdl.Head.skinwidth * g_CurMdl.Head.skinheight];
				memcpy(g_CurMdl.Skins[g_CurMdl.Head.num_skins], g_CurMdl.Skins[g_CurSkin], g_CurMdl.Head.skinwidth * g_CurMdl.Head.skinheight);
				g_CurMdl.Head.num_skins++;
			}
			else
			{
				int afterskin;

				afterskin = InsertSkinDlg.m_InsertAfter;

				for (int i = g_CurMdl.Head.num_skins; i > afterskin + 1; i--)
				{
					g_CurMdl.Skins[i] = g_CurMdl.Skins[i - 1];
				}
				g_CurMdl.Skins[afterskin + 1] = new unsigned char[g_CurMdl.Head.skinwidth * g_CurMdl.Head.skinheight];
				memcpy(g_CurMdl.Skins[afterskin + 1], g_CurMdl.Skins[g_CurSkin], g_CurMdl.Head.skinwidth * g_CurMdl.Head.skinheight);

				if (g_CurSkin > afterskin)
					g_CurSkin++;

				g_CurMdl.Head.num_skins++;
			}

		//      if (g_CurMdl.Head.num_skins == MAX_MD2SKINS)
		//      	AddNewSkin->Enabled = false;
	}

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}
	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_CAM, 0);
	}

	return 1;
}

int KDeleteSkin()
{
	if (g_CurMdl.Head.num_skins == 0)
		return 0;

	int Res = MessageBox(NULL, "Are you Sure you want to delete the current Skin?", "Confirmation", MB_YESNO);

	if (Res == IDNO)
		return 0;

	if (g_CurMdl.Head.num_skins == 1)
	{
		delete[] g_CurMdl.Skins[0];
		g_CurMdl.Skins[0] = NULL;
		g_CurMdl.Head.num_skins = 0;
		//      SkinLabel->Caption = "-";
		//      DeleteCurrentSkin->Enabled = false;

		if (g_SkinEditDlg)
		{
			SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
		}

		return 0;
	}

	delete[] g_CurMdl.Skins[g_CurSkin];
	for (int i = g_CurSkin; i < g_CurMdl.Head.num_skins - 1; i++)
		g_CurMdl.Skins[i] = g_CurMdl.Skins[i + 1];

	g_CurMdl.Skins[g_CurMdl.Head.num_skins - 1] = NULL;
	g_CurMdl.Head.num_skins--;

	//   if (g_CurMdl.Head.num_skins < MAX_MD2SKINS)
	//		AddNewSkin->Enabled = true;

	if (g_CurSkin == g_CurMdl.Head.num_skins)
		g_CurSkin--;

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KClearSkin()
{
	memset(g_CurMdl.Skins[g_CurSkin], 0, g_CurMdl.Head.skinwidth*g_CurMdl.Head.skinheight);

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KResizeSkin()
{
	CResizeSkinDlg    ResizeDlg;

	ResizeDlg.m_NewHeight = ResizeDlg.m_OldHeight = g_CurMdl.Head.skinheight;
	ResizeDlg.m_NewWidth = ResizeDlg.m_OldWidth = g_CurMdl.Head.skinwidth;

	if (ResizeDlg.DoModal() == IDCANCEL)
		return 0;


	int w, h;
	double diffw, diffh;

	w = ResizeDlg.m_NewWidth;
	h = ResizeDlg.m_NewHeight;

	diffw = (float)w / (float)g_CurMdl.Head.skinwidth;
	diffh = (float)h / (float)g_CurMdl.Head.skinheight;

	BYTE  *skin;
	for (int s = 0; s < g_CurMdl.Head.num_skins; s++)
	{
		skin = new BYTE[w*h];
		memset(skin, 0, w*h);

		delete[] g_CurMdl.Skins[s];
		g_CurMdl.Skins[s] = skin;
	}

	for (int v = 0; v < g_CurMdl.Head.num_st; v++)
	{
		g_CurMdl.SkinVerts[v].s *= (float)(diffw);
		g_CurMdl.SkinVerts[v].t *= (float)(diffh);
	}

	g_CurMdl.Head.skinwidth = w;
	g_CurMdl.Head.skinheight = h;


	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATESKIN, 0, 0);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}
	return 1;
}


int KSkinSelectAll()
{
	bool *o, *n;

	if (g_SkinSelectMode == stVertex)
	{
		o = new bool[g_CurMdl.Head.num_st];
		n = new bool[g_CurMdl.Head.num_st];

		g_CurMdl.LoadSelection(n, ssSkinVert);

		for (int i = 0; i < g_CurMdl.Head.num_st; i++)
			g_CurMdl.SkinVerts[i].Selected = true;

		g_CurMdl.LoadSelection(o, ssSkinVert);
	}

	if (g_SkinSelectMode == stTriangle)
	{
		o = new bool[g_CurMdl.Head.num_tris];
		n = new bool[g_CurMdl.Head.num_tris];

		g_CurMdl.LoadSelection(n, ssSkinTri);

		for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
			g_CurMdl.Tris[i].SkinSelected = true;

		g_CurMdl.LoadSelection(n, ssSkinTri);
	}

	CSkinSelectUndoNode   *N;
	N = new CSkinSelectUndoNode;
	N->Set("Select", &g_CurMdl, g_SkinSelectMode, o, n);
	g_SkinUndo.AddNode(N);

	delete[] o;
	delete[] n;

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KSkinSelectNone()
{
	bool *o, *n;

	if (g_SkinSelectMode == stVertex)
	{
		o = new bool[g_CurMdl.Head.num_st];
		n = new bool[g_CurMdl.Head.num_st];

		g_CurMdl.LoadSelection(n, ssSkinVert);

		for (int i = 0; i < g_CurMdl.Head.num_st; i++)
			g_CurMdl.SkinVerts[i].Selected = false;

		g_CurMdl.LoadSelection(o, ssSkinVert);
	}

	if (g_SkinSelectMode == stTriangle)
	{
		o = new bool[g_CurMdl.Head.num_tris];
		n = new bool[g_CurMdl.Head.num_tris];

		g_CurMdl.LoadSelection(n, ssSkinTri);

		for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
			g_CurMdl.Tris[i].SkinSelected = false;

		g_CurMdl.LoadSelection(n, ssSkinTri);
	}

	CSkinSelectUndoNode   *N;
	N = new CSkinSelectUndoNode;
	N->Set("Select", &g_CurMdl, g_SkinSelectMode, o, n);
	g_SkinUndo.AddNode(N);

	delete[] o;
	delete[] n;

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}
	return 1;
}

int KSkinSelectInverse()
{
	bool *o, *n;

	if (g_SkinSelectMode == stVertex)
	{
		o = new bool[g_CurMdl.Head.num_st];
		n = new bool[g_CurMdl.Head.num_st];

		g_CurMdl.LoadSelection(n, ssSkinVert);

		for (int i = 0; i < g_CurMdl.Head.num_st; i++)
			g_CurMdl.SkinVerts[i].Selected = !g_CurMdl.SkinVerts[i].Selected;

		g_CurMdl.LoadSelection(o, ssSkinVert);
	}

	if (g_SkinSelectMode == stTriangle)
	{
		o = new bool[g_CurMdl.Head.num_tris];
		n = new bool[g_CurMdl.Head.num_tris];

		g_CurMdl.LoadSelection(n, ssSkinTri);

		for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
			g_CurMdl.Tris[i].SkinSelected = !g_CurMdl.Tris[i].SkinSelected;

		g_CurMdl.LoadSelection(n, ssSkinTri);
	}

	CSkinSelectUndoNode   *N;
	N = new CSkinSelectUndoNode;
	N->Set("Select", &g_CurMdl, g_SkinSelectMode, o, n);
	g_SkinUndo.AddNode(N);

	delete[] o;
	delete[] n;

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}
	return 1;
}

int KSkinSelectConnected()
{
	bool changed;
	bool *o, *n;


	if (g_SkinSelectMode == stVertex)
	{
		o = new bool[g_CurMdl.Head.num_st];
		n = new bool[g_CurMdl.Head.num_st];

		g_CurMdl.LoadSelection(n, ssSkinVert);

		for (int n = 0; n < 5000; n++)
		{
			changed = false;
			for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
				if (
					!(g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[0]].Selected &&
						g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[1]].Selected &&
						g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[2]].Selected)
					&&
					(g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[0]].Selected ||
						g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[1]].Selected ||
						g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[2]].Selected))
				{
					g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[0]].Selected = true;
					g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[1]].Selected = true;
					g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[2]].Selected = true;
					changed = true;
				}
			if (changed == false)
				break;
		}

		g_CurMdl.LoadSelection(o, ssSkinVert);
	}

	if (g_SkinSelectMode == stTriangle)
	{
		o = new bool[g_CurMdl.Head.num_tris];
		n = new bool[g_CurMdl.Head.num_tris];

		g_CurMdl.LoadSelection(n, ssSkinTri);

		bool  *seld;
		int   i;
		seld = new bool[g_CurMdl.Head.num_st];
		for (i = 0; i < g_CurMdl.Head.num_st; i++)
			seld[i] = false;

		for (i = 0; i < g_CurMdl.Head.num_tris; i++)
			if (g_CurMdl.Tris[i].SkinSelected)
			{
				seld[g_CurMdl.Tris[i].SkinVerts[0]] = true;
				seld[g_CurMdl.Tris[i].SkinVerts[1]] = true;
				seld[g_CurMdl.Tris[i].SkinVerts[2]] = true;
			}

		for (int n = 0; n < 5000; n++)
		{
			changed = false;
			for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
				if (
					!(seld[g_CurMdl.Tris[t].SkinVerts[0]] &&
						seld[g_CurMdl.Tris[t].SkinVerts[1]] &&
						seld[g_CurMdl.Tris[t].SkinVerts[2]])
					&&
					(seld[g_CurMdl.Tris[t].SkinVerts[0]] ||
						seld[g_CurMdl.Tris[t].SkinVerts[1]] ||
						seld[g_CurMdl.Tris[t].SkinVerts[2]]))
				{
					seld[g_CurMdl.Tris[t].SkinVerts[0]] = true;
					seld[g_CurMdl.Tris[t].SkinVerts[1]] = true;
					seld[g_CurMdl.Tris[t].SkinVerts[2]] = true;
					g_CurMdl.Tris[t].SkinSelected = true;
					changed = true;
				}
			if (changed == false) n = 5000;
		}
		for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
		{
			if (seld[g_CurMdl.Tris[t].SkinVerts[0]] &&
				seld[g_CurMdl.Tris[t].SkinVerts[1]] &&
				seld[g_CurMdl.Tris[t].SkinVerts[2]])
				g_CurMdl.Tris[t].SkinSelected = true;
		}
		delete[] seld;
		g_CurMdl.LoadSelection(o, ssSkinTri);
	}

	CSkinSelectUndoNode   *N;
	N = new CSkinSelectUndoNode;
	N->Set("Select", &g_CurMdl, g_SkinSelectMode, o, n);
	g_SkinUndo.AddNode(N);

	delete[] o;
	delete[] n;

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KSkinSelectTouching()
{
	bool  *seld, *vertsel;
	bool  *o, *n;
	int   i;

	bool changed;
	if (g_SkinSelectMode == stVertex)
	{
		o = new bool[g_CurMdl.Head.num_st];
		n = new bool[g_CurMdl.Head.num_st];

		g_CurMdl.LoadSelection(n, ssSkinVert);

		seld = new bool[g_CurMdl.Head.num_st];
		for (i = 0; i < g_CurMdl.Head.num_st; i++)
			seld[i] = g_CurMdl.SkinVerts[i].Selected;

		for (int n = 0; n < 1; n++)
		{
			changed = false;
			for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
				if (
					/*               !(g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[0]].Selected &&
									 g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[1]].Selected &&
									 g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[2]].Selected)
												  &&                                        */
					(g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[0]].Selected ||
						g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[1]].Selected ||
						g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[2]].Selected))
				{
					seld[g_CurMdl.Tris[t].SkinVerts[0]] = true;
					seld[g_CurMdl.Tris[t].SkinVerts[1]] = true;
					seld[g_CurMdl.Tris[t].SkinVerts[2]] = true;
					changed = true;
				}
			if (changed == false) n = 5000;
		}

		for (i = 0; i < g_CurMdl.Head.num_st; i++)
			g_CurMdl.SkinVerts[i].Selected = seld[i];

		g_CurMdl.LoadSelection(o, ssSkinVert);
		delete[] seld;
	}
	if (g_SkinSelectMode == stTriangle)
	{
		int i, t;
		o = new bool[g_CurMdl.Head.num_tris];
		n = new bool[g_CurMdl.Head.num_tris];

		g_CurMdl.LoadSelection(n, ssSkinTri);

		seld = new bool[g_CurMdl.Head.num_st];
		vertsel = new bool[g_CurMdl.Head.num_st];

		for (i = 0; i < g_CurMdl.Head.num_st; i++)
			vertsel[i] = false;
		for (i = 0; i < g_CurMdl.Head.num_st; i++)
			seld[i] = false;

		for (t = 0; t < g_CurMdl.Head.num_tris; t++)
			if (g_CurMdl.Tris[t].SkinSelected)
			{
				vertsel[g_CurMdl.Tris[t].SkinVerts[0]] = true;
				vertsel[g_CurMdl.Tris[t].SkinVerts[1]] = true;
				vertsel[g_CurMdl.Tris[t].SkinVerts[2]] = true;
			}

		for (t = 0; t < g_CurMdl.Head.num_tris; t++)
			if (g_CurMdl.Tris[t].SkinSelected)
			{
				seld[g_CurMdl.Tris[t].SkinVerts[0]] = true;
				seld[g_CurMdl.Tris[t].SkinVerts[1]] = true;
				seld[g_CurMdl.Tris[t].SkinVerts[2]] = true;
			}

		for (int n = 0; n < 2; n++)
		{
			changed = false;
			for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
				if (
					!(vertsel[g_CurMdl.Tris[t].SkinVerts[0]] &&
						vertsel[g_CurMdl.Tris[t].SkinVerts[1]] &&
						vertsel[g_CurMdl.Tris[t].SkinVerts[2]])
					&&
					(vertsel[g_CurMdl.Tris[t].SkinVerts[0]] ||
						vertsel[g_CurMdl.Tris[t].SkinVerts[1]] ||
						vertsel[g_CurMdl.Tris[t].SkinVerts[2]]))
				{
					seld[g_CurMdl.Tris[t].SkinVerts[0]] = true;
					seld[g_CurMdl.Tris[t].SkinVerts[1]] = true;
					seld[g_CurMdl.Tris[t].SkinVerts[2]] = true;
					changed = true;
				}
			if (changed == false) n = 5000;
		}

		for (i = 0; i < g_CurMdl.Head.num_tris; i++)
			if (seld[g_CurMdl.Tris[i].SkinVerts[0]] &&
				seld[g_CurMdl.Tris[i].SkinVerts[1]] &&
				seld[g_CurMdl.Tris[i].SkinVerts[2]])
				g_CurMdl.Tris[i].SkinSelected = true;
			else
				g_CurMdl.Tris[i].SkinSelected = false;

		delete[] seld;
		delete[] vertsel;
		g_CurMdl.LoadSelection(o, ssSkinTri);
	}

	CSkinSelectUndoNode   *N;
	N = new CSkinSelectUndoNode;
	N->Set("Select", &g_CurMdl, g_SkinSelectMode, o, n);
	g_SkinUndo.AddNode(N);

	delete[] o;
	delete[] n;

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_DOSYNC, 0, 0);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}
	return 1;
}

int KSkinLinesNone()
{
	if (g_SkinEditDlg)
	{
		g_SkinEditDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_LINES_DONTDRAW,
			ID_OPTIONS_LINES_NORMAL,
			ID_OPTIONS_LINES_DONTDRAW,
			MF_BYCOMMAND);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KSkinLinesNormal()
{
	if (g_SkinEditDlg)
	{
		g_SkinEditDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_LINES_DONTDRAW,
			ID_OPTIONS_LINES_NORMAL,
			ID_OPTIONS_LINES_NORMAL,
			MF_BYCOMMAND);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KSkinLinesCycle()
{
	if (g_SkinEditDlg)
	{
		if (g_SkinEditDlg->IsMenuChecked(ID_OPTIONS_LINES_DONTDRAW))
			KSkinLinesNormal();
		else
			if (g_SkinEditDlg->IsMenuChecked(ID_OPTIONS_LINES_NORMAL))
				KSkinLinesNone();

		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}
	return 1;
	return 1;

}

int KSkinVertsNone()
{
	if (g_SkinEditDlg)
	{
		g_SkinEditDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_VERTICES_NONE,
			ID_OPTIONS_VERTICES_TICKS,
			ID_OPTIONS_VERTICES_NONE,
			MF_BYCOMMAND);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KSkinVertsDots()
{
	if (g_SkinEditDlg)
	{
		g_SkinEditDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_VERTICES_NONE,
			ID_OPTIONS_VERTICES_TICKS,
			ID_OPTIONS_VERTICES_DOTS,
			MF_BYCOMMAND);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KSkinVertsTicks()
{
	if (g_SkinEditDlg)
	{
		g_SkinEditDlg->GetMenu()->CheckMenuRadioItem(ID_OPTIONS_VERTICES_NONE,
			ID_OPTIONS_VERTICES_TICKS,
			ID_OPTIONS_VERTICES_TICKS,
			MF_BYCOMMAND);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KSkinVertsCycle()
{
	if (g_SkinEditDlg)
	{
		if (g_SkinEditDlg->IsMenuChecked(ID_OPTIONS_VERTICES_NONE))
			KSkinVertsDots();
		else
			if (g_SkinEditDlg->IsMenuChecked(ID_OPTIONS_VERTICES_DOTS))
				KSkinVertsTicks();
			else
				if (g_SkinEditDlg->IsMenuChecked(ID_OPTIONS_VERTICES_TICKS))
					KSkinVertsNone();

		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);
	}
	return 1;
}

int KSkinSyncSelection()
{
	//   g_SkinEditDlg->Sync3DSelection->Click();
	return 1;
}

int KSkinZoomIn()
{
	g_SkinZoomChanged = g_SkinZoomFactor;
	g_SkinZoomFactor *= (float)1.1;

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_SKIN_CHECKSCROLLBARS, 0, 0);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KSkinZoomOut()
{
	g_SkinZoomChanged = g_SkinZoomFactor;
	g_SkinZoomFactor /= (float)1.1;

	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_SKIN_CHECKSCROLLBARS, 0, 0);
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	return 1;
}

int KSkinWindowOnTop()
{
	//   g_SkinEditDlg->KeepWindowOnTop->Click();
	return 1;
}

int KSkinToolSelect()
{
	g_SkinEditDlg->m_ModifyTab.m_SelectBut.SendMessage(BM_CLICK);
	return 1;
}

int KSkinToolMove()
{
	g_SkinEditDlg->m_ModifyTab.m_MoveBut.SendMessage(BM_CLICK);
	return 1;
}

int KSkinToolRotate()
{
	g_SkinEditDlg->m_ModifyTab.m_RotateBut.SendMessage(BM_CLICK);
	return 1;
}

int KSkinToolScale()
{
	g_SkinEditDlg->m_ModifyTab.m_ScaleBut.SendMessage(BM_CLICK);
	return 1;
}

int KSkinToolCycle()
{
	if (g_SkinEditDlg->m_ModifyTab.m_SelectBut.GetCheck())
		KToolMove();
	else
		if (g_SkinEditDlg->m_ModifyTab.m_MoveBut.GetCheck())
			KToolRotate();
		else
			if (g_SkinEditDlg->m_ModifyTab.m_RotateBut.GetCheck())
				KToolScale();
			else
				if (g_SkinEditDlg->m_ModifyTab.m_ScaleBut.GetCheck())
					KToolSelect();
				else
					return 0;

	return 1;
}

int KSkinToolMirror()
{
	bool  *seld, cando;
	int   i;

	seld = new bool[g_CurMdl.Head.num_st];
	for (i = 0; i < g_CurMdl.Head.num_st; i++)
		seld[i] = false;

	if (g_SkinSelectMode == stVertex)
		for (int i = 0; i < g_CurMdl.Head.num_st; i++)
		{
			seld[i] = g_CurMdl.SkinVerts[i].Selected;
		}
	else
		if (g_SkinSelectMode == stTriangle)
		{
			for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
				if (g_CurMdl.Tris[i].SkinSelected)
				{
					seld[g_CurMdl.Tris[i].SkinVerts[0]] = true;
					seld[g_CurMdl.Tris[i].SkinVerts[1]] = true;
					seld[g_CurMdl.Tris[i].SkinVerts[2]] = true;
				}
		}


	float centrex = 0, centrey = 0;
	int num = 0;

	for (i = 0; i < g_CurMdl.Head.num_st; i++)
		if (seld[i])
		{
			centrex += g_CurMdl.SkinVerts[i].s;
			centrey += g_CurMdl.SkinVerts[i].t;
			num++;
		}

	if (num == 0)
	{
		delete[] seld;
		return 0;
	}

	centrex /= num;
	centrey /= num;

	cando = true;
	if (g_SkinEditDlg->m_ModifyTab.m_XBut.GetCheck())
	{
		for (int i = 0; i < g_CurMdl.Head.num_st; i++)
			if (seld[i])
			{
				float s = (float)(2.0*centrex - g_CurMdl.SkinVerts[i].s);
				if (s < 0 || s >= g_CurMdl.Head.skinwidth)
					cando = false;
			}
		if (cando)
			for (int i = 0; i < g_CurMdl.Head.num_st; i++)
				if (seld[i])
				{
					float s = (float)(2.0*centrex - g_CurMdl.SkinVerts[i].s);
					g_CurMdl.SkinVerts[i].s = s;
				}
	}

	cando = true;

	if (g_SkinEditDlg->m_ModifyTab.m_YBut.GetCheck())
	{
		for (int i = 0; i < g_CurMdl.Head.num_st; i++)
			if (seld[i])
			{
				float t = (float)(2.0*centrey - g_CurMdl.SkinVerts[i].t);
				if (t < 0 || t >= g_CurMdl.Head.skinheight)
					cando = false;
			}
		if (cando)
			for (int i = 0; i < g_CurMdl.Head.num_st; i++)
				if (seld[i])
				{
					float t = (float)(2.0*centrey - g_CurMdl.SkinVerts[i].t);
					g_CurMdl.SkinVerts[i].t = t;
				}
	}

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_CAM, 0);
	}
	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	delete[] seld;

	return 1;
}

int KSkinToolWeldVerts()
{

	int WeldTarget;

	if (g_SkinSelectMode == stTriangle)
		return 0;

	CSkinVertsUndoNode *Node;
	Node = new CSkinVertsUndoNode;
	Node->Set("Weld", &g_CurMdl);

	for (int i = 0; i < g_CurMdl.Head.num_st; i++)
		if (g_CurMdl.SkinVerts[i].Selected)
		{
			WeldTarget = i;
			break;
		}

	for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
	{
		for (int i = 0; i < 3; i++)
			if (g_CurMdl.SkinVerts[g_CurMdl.Tris[t].SkinVerts[i]].Selected)
				g_CurMdl.Tris[t].SkinVerts[i] = (short)WeldTarget;
	}

	g_CurMdl.DeleteUnusedSkinVerts();


	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}
	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_CAM, 0);
	}

	g_SkinUndo.AddNode(Node);

	return 1;
}

int KSkinToolDetach()
{
	if (g_SkinSelectMode != stTriangle)
		return 0;

	{
		bool Anything = false;
		for (int i = 0; i < g_CurMdl.Head.num_tris; i++)
			if (g_CurMdl.Tris[i].SkinSelected)
				Anything = true;

		if (!Anything)
			return 0;
	}

	CSkinVertsUndoNode *Node;
	Node = new CSkinVertsUndoNode;
	Node->Set("Weld", &g_CurMdl);

	int               *ToVerts, VertTop, t;
	TRealSkinVertex   *NewVerts;

	ToVerts = new int[g_CurMdl.Head.num_st];

	for (int i = 0; i < g_CurMdl.Head.num_st; i++)
		ToVerts[i] = -1;

	VertTop = g_CurMdl.Head.num_st;

	for (t = 0; t < g_CurMdl.Head.num_tris; t++)
	{
		if (g_CurMdl.Tris[t].SkinSelected)
		{
			if (IsSkinVertUsedByUnselected(g_CurMdl.Tris[t].SkinVerts[0]))
				if (ToVerts[g_CurMdl.Tris[t].SkinVerts[0]] == -1)
					ToVerts[g_CurMdl.Tris[t].SkinVerts[0]] = VertTop++;
			if (IsSkinVertUsedByUnselected(g_CurMdl.Tris[t].SkinVerts[1]))
				if (ToVerts[g_CurMdl.Tris[t].SkinVerts[1]] == -1)
					ToVerts[g_CurMdl.Tris[t].SkinVerts[1]] = VertTop++;
			if (IsSkinVertUsedByUnselected(g_CurMdl.Tris[t].SkinVerts[2]))
				if (ToVerts[g_CurMdl.Tris[t].SkinVerts[2]] == -1)
					ToVerts[g_CurMdl.Tris[t].SkinVerts[2]] = VertTop++;
		}
	}

	if (VertTop == g_CurMdl.Head.num_st)
		return 0;

	NewVerts = new TRealSkinVertex[VertTop];
	memcpy(NewVerts, g_CurMdl.SkinVerts, sizeof(TRealSkinVertex)*g_CurMdl.Head.num_st);

	for (int v = 0; v < g_CurMdl.Head.num_st; v++)
	{
		if (ToVerts[v] != -1)
		{
			NewVerts[ToVerts[v]].s = NewVerts[v].s;
			NewVerts[ToVerts[v]].t = NewVerts[v].t;
			NewVerts[ToVerts[v]].Selected = false;
		}
	}

	for (t = 0; t < g_CurMdl.Head.num_tris; t++)
	{
		if (g_CurMdl.Tris[t].SkinSelected)
		{
			if (ToVerts[g_CurMdl.Tris[t].SkinVerts[0]] != -1)
				g_CurMdl.Tris[t].SkinVerts[0] = ToVerts[g_CurMdl.Tris[t].SkinVerts[0]];
			if (ToVerts[g_CurMdl.Tris[t].SkinVerts[1]] != -1)
				g_CurMdl.Tris[t].SkinVerts[1] = ToVerts[g_CurMdl.Tris[t].SkinVerts[1]];
			if (ToVerts[g_CurMdl.Tris[t].SkinVerts[2]] != -1)
				g_CurMdl.Tris[t].SkinVerts[2] = ToVerts[g_CurMdl.Tris[t].SkinVerts[2]];
		}
	}

	g_CurMdl.Head.num_st = VertTop;
	delete[] g_CurMdl.SkinVerts;
	g_CurMdl.SkinVerts = NewVerts;


	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_CAM, 0);
	}
	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}

	g_SkinUndo.AddNode(Node);

	// Doesn't need updating since visually, nothing has changed, just some triangles have
	// been made, but not moved.

	return 1;
}

int KSkinToolDelete()
{
	if (g_CurMdl.Head.num_st == 1)
		return 0;

	CSkinVertsUndoNode *Node;
	Node = new CSkinVertsUndoNode;
	Node->Set("Delete", &g_CurMdl);

	if (g_SkinSelectMode == stVertex)
	{
		bool  *del;
		int   v;

		del = new bool[g_CurMdl.Head.num_st];

		for (v = 0; v < g_CurMdl.Head.num_st; v++)
		{
			del[v] = g_CurMdl.SkinVerts[v].Selected;
		}

		int *targ, tg = 0;

		targ = new int[g_CurMdl.Head.num_st];

		for (v = 0; v < g_CurMdl.Head.num_st; v++)
		{
			if (!del[v])
			{
				targ[v] = tg;
				tg++;
			}
			else targ[v] = -1;
		}

		for (int t = 0; t < g_CurMdl.Head.num_tris; t++)
		{
			bool res = false;
			for (int i = 0; i < 3; i++)
				if (targ[g_CurMdl.Tris[t].SkinVerts[i]] == -1)
					res = true;

			if (res)
			{
				for (int i = 0; i < 3; i++)
					g_CurMdl.Tris[t].SkinVerts[i] = 0;
			}
			else
			{
				g_CurMdl.Tris[t].SkinVerts[0] = targ[g_CurMdl.Tris[t].SkinVerts[0]];
				g_CurMdl.Tris[t].SkinVerts[1] = targ[g_CurMdl.Tris[t].SkinVerts[1]];
				g_CurMdl.Tris[t].SkinVerts[2] = targ[g_CurMdl.Tris[t].SkinVerts[2]];
			}
		}

		TRealSkinVertex *newskinverts;
		newskinverts = new TRealSkinVertex[tg];

		tg = 0;
		for (v = 0; v < g_CurMdl.Head.num_st; v++)
		{
			if (!del[v])
			{
				newskinverts[tg] = g_CurMdl.SkinVerts[v];
				tg++;
			}
		}

		delete[] g_CurMdl.SkinVerts;
		g_CurMdl.SkinVerts = newskinverts;

		g_CurMdl.Head.num_st = tg;

		delete[] targ;
		delete[] del;
	}

	if (g_MainDlg)
	{
		SendMessage(g_MainDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_CAM, 0);
	}
	if (g_SkinEditDlg)
	{
		SendMessage(g_SkinEditDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_SKIN, 0);
	}
	g_SkinUndo.AddNode(Node);

	return 1;
}

int KSkinNext()
{
	//   g_SkinEditDlg->NextBut->Click();
	return 1;
}

int KSkinPrev()
{
	//   g_SkinEditDlg->PrevBut->Click();
	return 1;
}


int KSkinVertexMode()
{
	if (g_SkinEditDlg)
		g_SkinEditDlg->m_BottomToolDlg.SetSelectionType(stVertex);
	return 1;
}

int KSkinFaceMode()
{
	if (g_SkinEditDlg)
		g_SkinEditDlg->m_BottomToolDlg.SetSelectionType(stTriangle);
	return 1;
}

int KSkinActiveAreaMode()
{
	//   g_SkinEditDlg->ActiveAreaBut->Down = true;
	//   g_SkinEditDlg->ActiveAreaBut->Click();
	return 1;
}

int KSkinCycleMode()
{
	if (g_SkinEditDlg->m_BottomToolDlg.m_VertexModeBut.GetCheck())
		KSkinFaceMode();
	else
		if (g_SkinEditDlg->m_BottomToolDlg.m_FaceModeBut.GetCheck())
			KSkinVertexMode();
		else
			KSkinVertexMode();

	return 1;
}

int KSkinUpdateSkinUsage()
{
	//   g_SkinEditDlg->SpaceLabelClick(NULL);
	return 1;
}

int KSkinToggleX()
{
	g_SkinEditDlg->m_ModifyTab.m_XBut.SetCheck(!g_SkinEditDlg->m_ModifyTab.m_XBut.GetCheck());
	return 1;
}
int KSkinToggleY()
{
	g_SkinEditDlg->m_ModifyTab.m_YBut.SetCheck(!g_SkinEditDlg->m_ModifyTab.m_YBut.GetCheck());
	return 1;
}



// Skin Painter Fucntions
int KPaintToolPan()
{
	g_PaintDlg->m_BottomToolDlg.m_PanViewsBut.SendMessage(BM_CLICK);
	return 1;
}

int KPaintToolPaint()
{
	g_PaintDlg->m_PaintTab.m_DrawBut.SendMessage(BM_CLICK);
	return 1;
}

int KPaintToolSpray()
{
	g_PaintDlg->m_PaintTab.m_SprayBut.SendMessage(BM_CLICK);
	return 1;
}

int KPaintToolFill()
{
	g_PaintDlg->m_PaintTab.m_FillBut.SendMessage(BM_CLICK);
	return 1;
}


int KPaintSwapFBColors()
{
	if (g_PaintDlg)
	{
		UINT  temp;
		temp = g_PaintDlg->m_PaintTab.m_ForeColor;
		g_PaintDlg->m_PaintTab.m_ForeColor = g_PaintDlg->m_PaintTab.m_BackColor;
		g_PaintDlg->m_PaintTab.m_BackColor = temp;

		g_PaintDlg->SendMessage(MM_PAINT_UPDATECOLORS, 0, 0);

		return 1;
	}
	return 0;
}

int KPaintUndo()
{
	g_PaintUndo.UndoLast();

	if (g_PaintDlg)
		SendMessage(g_PaintDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}

int KPaintRedo()
{
	g_PaintRedo.UndoLast();

	if (g_PaintDlg)
		SendMessage(g_PaintDlg->m_hWnd, MM_UPDATEVIEWPORTS, MMC_UPDATE_ALL, 0);

	return 1;
}
