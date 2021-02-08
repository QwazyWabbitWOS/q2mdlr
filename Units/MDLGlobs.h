//---------------------------------------------------------------------------
#ifndef GlobalsH
#define GlobalsH

#define MDLVERSION   0x90
#define CFGVERSION   0x10

#include "MdlConst.h"
#include "MdlType.h"
#include "DibFuncs.h"
#include "matrix.h"

#include "MenuItem.h"
#include "Keyboard.h"
#include "Undo.h"
#include "ProgSet.h"

#ifndef M_PI
#define M_PI      3.14159265358979323846
#endif


extern TCompleteModel		g_CurMdl, *g_pRef;
extern TCompleteModel		CopyBuffer;
extern SelectType				CopyBufferMode;

extern RECT    	         ViewRect[4];
extern int						CentreX[4],CentreY[4];
extern TDIBitmap8				Views[4];

extern bool						g_Resizing,g_Closing,g_Drawing;
extern int						g_CurFrame, g_CurSkin;
extern float 					PanX,PanY,PanZ,g_Zoom,g_GridSize, g_GridMin, g_GridMax;
extern float               g_GroundHeight;

extern float					RotX, RotY, Dist, g_Persp;
extern ViewType				g_CurView, g_BuildFaceView;
extern bool                g_Playing;

extern ActionType				g_MainAction, g_SkinAction,g_PaintAction;
extern SelectType				g_MainSelectMode, g_SkinSelectMode;
extern CAxisType				g_AxisType;
extern Vector3				   SelCentre;
extern Matrix3				   CurAxis;

extern CUndoList				g_MainUndo,g_MainRedo, g_SkinUndo, g_SkinRedo, g_PaintUndo, g_PaintRedo;

extern bool						ToDrawView[4];

extern float					g_downx,g_downy,g_downz,g_upx,g_upy,g_upz;
extern float               g_DownS, g_DownT, g_UpS, g_UpT;
extern float					g_diffx,g_diffy,g_diffz;
extern float					g_curx,g_cury,g_curz;
extern float					g_LastX, g_LastY, g_LastZ;
extern float					g_DeltaX, g_DeltaY, g_DeltaZ;
extern short						CreateFace[3];
extern int						g_ox,g_oy,*g_posx,*g_posy;
extern float					*g_posz;
extern int						g_ChangesMade;
extern CString      			g_CurFileName;

extern int						g_MRU_Count;
extern CString             g_MRUList[5];

extern Vector3             NORMALS[162];
extern TPaletteCol			Palette[256];
extern TDIBitmap8				View3d;

extern TBone					*TempBone;

extern HPALETTE				HQuakePal;
extern LOGPALETTE				*LogQuakePal;
extern PALETTEENTRY			EntryQuakePal[256];

extern float					InterFrame;
extern int						PlayFrame;

extern CString             QuakeDataDir;

extern CProgramSettings    g_ProgSet;

extern int                 VWidthL, VWidthR, VHeightT, VHeightB;     // various window measurements
extern int                 VWidthL2, VWidthR2, VHeightT2, VHeightB2; // same as above, but halved

extern int				      g_CreateCount, *g_CreateList;

extern CActionListType   g_ActionList;
extern CCommandList   g_KeyList;

extern float		   	g_SkinZoomChanged, g_SkinZoomFactor;


#endif   // globals H
