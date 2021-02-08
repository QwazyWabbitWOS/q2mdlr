//---------------------------------------------------------------------------
#ifndef MainFuncsH
#define MainFuncsH
//---------------------------------------------------------------------------

#include "DIBFuncs.h"

class CXFormItemInfo
{
public:
   // viewport size where the operation is taking place.
   int   m_vwidth, m_vheight, m_vwidth2, m_vheight2;

   TCompleteModel *m_Mdl;   // Model object
   TRealVertex    *m_Verts;    // array of vertices
   bool           *m_Sel;  // arrat of selection bools

   // Parameters passed by xform function.
   ActionType  m_Action;
   ViewType    m_View;
   int         m_ox, m_X, m_oy, m_Y;

   bool        m_ConstrainX, m_ConstrainY, m_ConstrainZ;
};

void AddWndStyle(HWND hWnd, long NewStyle);

Vector3 GetSelectionCentre(TCompleteModel &M, SelectType S);
void DrawViewMatrix(TDIBitmap8 &Dest, TCompleteModel *M, CDrawViewInfo *Info, ViewType View);
void DrawView(TDIBitmap8 &Dest, ViewType V);
void Draw3DModelOnly(TDIBitmap8 &Dest, TCompleteModel *M, CDrawViewInfo *Info);
void Draw3DView(TDIBitmap8 &Dest, TCompleteModel *M, CDrawViewInfo *Info, TCompleteModel *Ref = NULL);
int DoXFormVertFaceBone(CXFormItemInfo *Info);
void CalcAllNormals();
void CalcAllNormals(int frameno);
void CalcSelectedNormals();
void ExtrudeSelected(float D = 10.0);
bool SamePair(int a1, int b1, int a2, int b2);
void CheckVisibleSelection();

Matrix3 GetSelectionAxis();
Matrix3 GetViewAxis();
Matrix3 GetViewAxis(ViewType View);

bool AnythingSelected();
void SetParentForBone(TBone *Bone);
void RotateBoneFunc(TBone *Bone);
void ScaleBoneFunc(TBone *Bone);
void MoveBoneFunc(TBone *Bone);
void MakeLogicalPalette();
float FaceDotProd3D(TCompleteModel &M, int i,float cosa1,float sina1,float cosa2,float sina2);

void Draw3DRaised(CDC *C, int x, int y, int x1, int y1);
void Draw3DSunken(CDC *C, int x, int y, int x1, int y1);

int GetCWndWidth(const CWnd &Wnd);
int GetCWndHeight(const CWnd &Wnd);

bool IsSkinVertUsedByUnselected(int n);
//bool ControlCheckChildren(TWinControl *Owner, TControl *Child);
//bool ControlHasChild(TWinControl *Owner, TControl *Child);

void CleanUpGlobals();

COLORREF RGBToColorRef(BYTE rgb[3]);
WORD RGBto16(BYTE r, BYTE g, BYTE b);
void Col16toRGB(WORD Col, BYTE &r,BYTE &g,BYTE &b);

int WrapMouseToScreen(HWND hWnd, int &x, int &y, int &ox, int &oy);

#ifndef MainFuncs_CPP

#endif


#endif
