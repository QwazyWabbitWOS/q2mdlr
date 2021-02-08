//---------------------------------------------------------------------------
#include "stdafx.h"

#include "MdlType.h"
#include "DibFuncs.h"
#include "undo.h"
#include "ProgSet.h"
#include "keyboard.h"

class TCompleteModel			g_CurMdl, *g_pRef = NULL;
TCompleteModel			CopyBuffer;
SelectType				CopyBufferMode;

RECT        			ViewRect[4];
int						CentreX[4],CentreY[4];
TDIBitmap8				Views[4];

bool						g_Resizing=false,g_Closing=false,g_Drawing = false;
int						g_CurFrame=0,g_CurSkin = 0;
float 					PanX=128,PanY=128,PanZ=128,g_Zoom=1,g_GridSize=16, g_GridMin=-128, g_GridMax=128;
float                g_GroundHeight = 0;
float						RotX=5.1f, RotY=1.8f, Dist=80, g_Persp = 200.0;
ViewType					g_CurView, g_BuildFaceView;
bool                 g_Playing;

bool						ToDrawView[4] = {true,true,true,true};
ActionType				g_MainAction, g_SkinAction, g_PaintAction;
SelectType				g_MainSelectMode, g_SkinSelectMode;
CAxisType				g_AxisType = atView;
Vector3					SelCentre(0,0,0);
Matrix3					CurAxis(1,0,0,0,1,0,0,0,1);

CUndoList				g_MainUndo,g_MainRedo, g_SkinUndo, g_SkinRedo, g_PaintUndo, g_PaintRedo;

float						g_downx,g_downy,g_downz,g_upx,g_upy,g_upz;
float                g_DownS, g_DownT, g_UpS, g_UpT;
float						g_diffx,g_diffy,g_diffz;
float						g_curx,g_cury,g_curz;
float						g_LastX, g_LastY, g_LastZ;
float						g_DeltaX, g_DeltaY, g_DeltaZ;
short						CreateFace[3];
int						g_ox,g_oy,*g_posx = NULL,*g_posy = NULL;
float						*g_posz = NULL;
int						g_ChangesMade = 1;
CString    				g_CurFileName;

int 						g_MRU_Count = 5;
CString              g_MRUList[5];

TPaletteCol				Palette[256];

TBone						*TempBone;

HPALETTE					HQuakePal = 0;
LOGPALETTE				*LogQuakePal;
PALETTEENTRY			EntryQuakePal[256];

float						InterFrame = 0;
int						PlayFrame = 0;

CString    				QuakeDataDir("e:/games/quake2/baseq2/");

CProgramSettings     g_ProgSet;

int            VWidthL, VWidthR, VHeightT, VHeightB;     // various window measurements
int            VWidthL2, VWidthR2, VHeightT2, VHeightB2; // same as above, but halved

int				      g_CreateCount=0, *g_CreateList = NULL;

CActionListType   g_ActionList;
CCommandList   g_KeyList;

float		   	g_SkinZoomChanged = 1.0, g_SkinZoomFactor = 1.0;


Vector3 NORMALS[162]=
{
#include "anorms.h"
};

//---------------------------------------------------------------------------





