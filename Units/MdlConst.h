//---------------------------------------------------------------------------
#ifndef MdlConstH
#define MdlConstH
//---------------------------------------------------------------------------

#include "Matrix.h"

#define IDALIASHEADER (('2'<<24)+('P'<<16)+('D'<<8)+'I')
#define ALIAS_VERSION 8

#define MAX_TRIANGLES	4096
#define MAX_VERTS			2048
#define MAX_FRAMES		2048
#define MAX_IDFRAMES    512
#define MAX_MD2SKINS		32
#define MAX_SKINNAME		64

#define REPLACEVERTS		0
#define APPENDVERTS		1

#define crArrowNorm		-21
#define crArrowMinus		-22
#define crArrowRotate	-23
#define crArrowScale		-24
#define crArrowMove		-25
#define crArrowPan		-26
#define crArrowSelect	-27
#define crArrowTri		-28
#define crArrowVert		-29
#define crArrowOrigin	-30
#define crArrowDraw		-31
#define crArrowDropper	-32

#define ShowMessage(a)	Application->MessageBox(a,"Message",MB_OK)

//defines for the Add new frame dialog and related stuff
#define FRAMESRC_BEFOREFIRST     -1
#define FRAMESRC_AFTERFINAL      -2

#include <memory.h>

enum ViewType { vtXY, vtXZ, vtZY, vtCamera };
enum ActionType {
	atNone, atSelect, atSelectBox, atDeselectBox,
	atDeselect, atPan, atZoom, atRotate, atMove, atScale,
	atCreateVertex, atCreatingVertex,
	atBuildFace, atBuildingFace1, atBuildingFace2, atBuildingFaceDone,
	atCreateBone, atAssignParent, atAssignVertToBone,
	atDraw, atMidPointMove, atEyeDropper
};
enum SelectType { stVertex, stTriangle, stBone };
enum CAxisType { atLocal, atView, atWorld };

enum SkinVertPos { svAll, svFront, svBack, svLeft, svRight, svTop, svBottom };
enum TModelFileType { ftAQM, ftMDL, ftMD2, ftASC };

enum ShadingType { shNone, shFlat, shGourad, shPhong };
enum TextureType { ttNone, ttWire, ttAffine, ttPers, ttBilinear };
enum NormalDType { ndNone, ndSelected, ndAll };

typedef struct
{
	float x, y;
} vec2_t;

class line_t
{
public:
	float x1, y1, x2, y2, m, c;

	line_t();
	line_t(float nx1, float ny1, float nx2, float ny2);

	bool calcm();
	bool calcc();

	bool Vertical();
	bool Horionztal();
};

typedef unsigned char byte;

class TMdlHeader
{
public:
	int id;		 // equals IDALIASHEADER
	int version;    // equals ALIAS_VERSION

	int skinwidth;
	int skinheight;
	int framesize; // byte size of each frame

	int num_skins;
	int num_xyz;
	int num_st; // greater than num_xyz for seams
	int num_tris;
	int num_glcmds; // dwords in strip/fan command list
	int num_frames;

	int ofs_skins; // each skin is a MAX_SKINNAME string
	int ofs_st; // byte offset from start for stverts
	int ofs_tris; // offset for dtriangles
	int ofs_frames; // offset for first frame
	int ofs_glcmds;
	int ofs_end; // end of file
};

typedef struct
{
	float s;						// horiz
	float t;						// vert
	bool	Selected;
} TRealSkinVertex;

typedef struct
{
	short s, t;
} TFileSkinVertex;

typedef struct
{
	short vertices[3];
	short skinverts[3];
} TFileTri;

class TBaseTriangle
{
public:
	short		Vertices[3];
	short    SkinVerts[3];
	Vector3  Centre;
	Vector3	Normal;
};

class TRealTriangle : public TBaseTriangle
{
public:
	bool	Selected, Visible, 	SkinSelected;

	TRealTriangle() { Visible = true; Selected = false; };
};

typedef struct
{
	byte v[3];    // X,Y,Z coordinate, packed on 0-255
	byte lightnormalindex;     // index of the vertex normal
} TFileAnimVertex;

class TRealVertex
{
public:
	float 	x, y, z;
	bool	Selected, Visible;

	Vector3  Normal;
	//	float 	Strength, Ox,Oy,Oz;
	// void		*Parent;

	TRealVertex();
};

class TOldAQMRealVertex
{
public:
	float 	x, y, z;
	bool		Selected, Visible;

	float 	Strength, Ox, Oy, Oz;
	void		*Parent;
};

class TFrameInfo
{
public:
	char	Name[16];

	TFrameInfo() { memset(Name, 0, 16); };
};

typedef struct
{
	float	scale[3];
	float translate[3];
	char	name[16];
} TFileFrameInfo;

typedef struct
{
	bool OnEdge[3];
} TExtrudeTri;

typedef unsigned char TPaletteCol[3];

class TLoadVertFace
{
public:
	int num_xyz, num_tris;

	TRealVertex *V;
	TRealTriangle *T;

	TLoadVertFace();
	~TLoadVertFace();

};

class CameraInfo
{
public:
	double   m_PanX, m_PanY, m_PanZ,
		m_RotX, m_RotY,
		m_Persp,
		m_Dist;

	CameraInfo();
	CameraInfo(double PX, double PY, double PZ, double RX, double RY, double D, double P);
	void Set(double PX, double PY, double PZ, double RX, double RY, double D, double P);
};

class CDrawViewInfo
{
public:
	ShadingType    Shading;
	TextureType    Texture;
	NormalDType    Normals;

	bool           Grid, Origin, Ticks, m_3DFaceCull, m_2DFaceCull, m_DrawRef;

	SelectType     Mode;

	CameraInfo     Cam;
	ViewType       View;

	CDrawViewInfo();
	CDrawViewInfo(ShadingType S, TextureType T, double PX, double PY, double PZ, double RX, double RY, double D, double P);
};

class Transform3DStruct
{
public:
	float       cosa1, sina1, cosa2, sina2;
	float       x, y, z;
	float       nx, ny, nz;
	CameraInfo  *Cam;

	Transform3DStruct();
	~Transform3DStruct();
	bool Transform3D();
	void SetXYZ(float X, float Y, float Z) { x = X; y = Y; z = Z; };
	void SetTrig(float c1, float s1, float c2, float s2) { cosa1 = c1; cosa2 = c2; sina1 = s1; sina2 = s2; };
};

typedef struct
{
	bool Limit;
	int MaxNodes, MinNodes, MaxSize;
	int NumNodes, CurSize;

} TempUndoInfo;

class CMouseState
{
public:
	bool  m_Left, m_Right, m_Mid;
	CMouseState();
};

class CShiftState
{
public:
	bool  m_Shift, m_Ctrl, m_Alt, m_Left, m_Right, m_Mid;

	CShiftState();
	void SetFromFlags(UINT nFlags);
	UINT SetIntoFlags();
};

void ZeroVec(Vector3 &V);
bool InRect(float x1, float y1, float x2, float y2, float a, float b);
bool Between(float a, float min, float max);
bool InTri(float x1, float y1, float x2, float y2, float x3, float y3, float a, float b);
bool LinesIntersect(line_t a, line_t b);

unsigned long ReverseL(unsigned long V);
unsigned short ReverseW(unsigned short V);
float ReverseF(float F);

#endif

