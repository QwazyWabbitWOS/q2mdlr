//---------------------------------------------------------------------------
#ifndef KeyFuncsH
#define KeyFuncsH
//---------------------------------------------------------------------------

#define ACTION_COUNT    5


#ifndef KEYFUNCS_CPP
extern CActionListType  ActionList;
#endif

void SetFormVars(CWnd *Main, CWnd *SkinEdit, CWnd *Paint, CWnd *TypeIn);

void InitActionList();
void DestroyActionList();

int KHelperOpenFile(CString Str);

// file menu
int KFileNew();
int KFileOpen();
int KFileSave();
int KFileSaveAs();
int KFileMerge();
int KExportMD2();
int KExportMDL();
int KExportFrameASC();
int KExportModelASC();
int KExportFrame3DS();
int KExportModel3DS();
int KImportAppendVerts();
int KImportAppendFrames();
int KImportFrameNames();
int KFileExit();

int KChooseReferenceModel();
int KClearReferenceModel();
// Edit Menu
int KEditUndo();
int KEditRedo();
int KEditCopy();
int KEditPaste();
int KEditPasteRange();
int KEditRename();
int KEditGotoFrame();
int KEditAddFrame();
int KEditDeleteFrameCurrent();
int KEditDeleteFrameRange();
int KEditMoveFrame();
int KEditSelectAll();
int KEditSelectNone();
int KEditSelectInverse();
int KEditSelectConnected();
int KEditSelectTouching();

// View Menu

int KViewSkins();
int KView3DEditor();
int KViewSkinPainter();
int KViewTypeIn();
int KViewProperties();

// options menu

int KOptionsVertTicks();
int KOptionsShowOrigin();
int KOptionsShowGrid();
int KOptionsGridPos();
int KOptionsNormalsNone();
int KOptionsNormalsSelect();
int KOptionsNormalsAll();
int KOptionsNormalsCycle();
int KOptions2DBackface();
int KOptionsCameraPlayXY();
int KOptionsCameraPlayZY();
int KOptionsCameraPlayXZ();
int KOptions3DWireframe();
int KOptions3DFlat();
int KOptions3DGourad();
int KOptions3DTexture();
int KOptions3DTextureGourad();
int KOptions3DBackface();
int KOptions3DDrawCycle();
int KOptions3DShowReference();
int KOptionsSyncSkinSelect();
int KOptionsIncreaseGrid();
int KOptionsDecreaseGrid();
int KOptionsZoomIn();
int KOptionsZoomOut();
int KOptionsConfigure();

// Tools Menu

int KToolsGenerateHeader();
int KToolsMemoWindow();
int KToolsOutputBoundingBox();

// Help Menu

int KHelpContents();
int KHelpKeyword();
int KHelpAbout();

// Bottom Panel Commands

// Mode Commands
int KModeVertex();
int KModeFace();
int KModeBones();
int KModeObject();
int KModeCycle();
int KControlLimitX();
int KControlLimitY();
int KControlLimitZ();

// Tools
int KToolPan();
int KToolSelect();
int KToolMove();
int KToolRotate();
int KToolScale();
int KToolCycle();
int KToolMirror();
int KToolDelete();
int KToolFlipNormal();
int KToolTurn();
int KToolSubdivide();
int KToolExtrude();
int KToolWeld();

int KMainInterpolationToggle();
int KEffectFrameRangeToggle();

// view control
int KControlNextFrame();
int KControlPrevFrame();
int KControlLastFrame();
int KControlFirstFrame();
int KControlFitAll();
int KControlFitSelected();
int KControlPlayCamera();
int KControlHideSelected();
int KControlHideUnSelected();
int KControlUnhide();

// create
int KCreateVertex();
int KCreateFace();
int KCreateFan();
int KCreateStrip();


// SKIN EDITOR FUNCTIONS

int KImportSkinImage();
int KImportSkinMesh();
int KExportImageBMP();
int KExportImagePCX();
int KExportSkinMesh();
int KSkinEditorExit();

int KSkinUndo();
int KSkinRedo();
int KSkinCopy();
int KSkinPaste();
int KGetPosition();
int KAddSkin();
int KDeleteSkin();
int KClearSkin();
int KResizeSkin();

int KSkinSelectAll();
int KSkinSelectNone();
int KSkinSelectInverse();
int KSkinSelectConnected();
int KSkinSelectTouching();

int KSkinLinesNone();
int KSkinLinesNormal();
int KSkinLinesCycle();
int KSkinVertsNone();
int KSkinVertsDots();
int KSkinVertsTicks();
int KSkinVertsCycle();

int KSkinSyncSelection();
int KSkinZoomIn();
int KSkinZoomOut();
int KSkinWindowOnTop();

int KSkinToolCycle();
int KSkinToolSelect();
int KSkinToolMove();
int KSkinToolRotate();
int KSkinToolScale();
int KSkinToolMirror();
int KSkinToolWeldVerts();
int KSkinToolDetach();
int KSkinToolDelete();

int KSkinNext();
int KSkinPrev();

int KSkinVertexMode();
int KSkinFaceMode();
int KSkinActiveAreaMode();
int KSkinCycleMode();

int KSkinToggleX();
int KSkinToggleY();

int KSkinUpdateSkinUsage();



// Skin Painter functions

int KPaintToolPan();
int KPaintToolPaint();
int KPaintToolSpray();
int KPaintToolFill();
int KPaintSwapFBColors();

int KPaintUndo();
int KPaintRedo();

#endif
