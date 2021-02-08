//---------------------------------------------------------------------------
#ifndef ModelType_H
#define ModelType_H
//---------------------------------------------------------------------------

typedef unsigned char BYTE;

#include "Bones.h"

#define ascNoFile			-1
#define ascFileTooBig	-2
#define ascNotTriMesh	-3
#define ascError			-4
#define ascNoVerts		-5
#define ascNoTris			-6
#define ascVertTriDiff	-7

#define _3dsUEOF			-1
#define _3dsMultiVert	-2
#define _3dsMultiFace	-3
#define _3dsMaxVerts		-4
#define _3dsMaxTris		-5


#define AQMHeader			"AQN"
#define AQMOldHeader		"AQM"
#define AQMHeaderSize	3

#define CALCNORMS_ALL      1
#define CALCNORMS_SELECTED 2

enum TMode {imAppendVerts, imAppendFrame, imReplaceFrame};
enum CSelectionSource { ssVertex, ssTriangle, ssSkinVert, ssSkinTri };

class TImportMode
{
public:
   TMode	Mode;
   int 	Frame;	// for the two frame import modes

   TImportMode();
   TImportMode(TMode M, int f = 0);
} ;

class TCompleteModel
{
public:
   TMdlHeader  			Head;
   BYTE						*Skins[MAX_MD2SKINS];
   char						SkinNames[MAX_MD2SKINS][MAX_SKINNAME];
   TRealSkinVertex		*SkinVerts;
   TRealTriangle 			*Tris;
   TRealVertex 			*Verts;
   TRealVertex				*Frames[MAX_FRAMES];
   TFrameInfo				*FrameInfo;
   bool						Empty;

   TBoneList				Bones;

// glcmds related
   int					commands[65536];
   int					numcommands;
   int					numglverts;
   int					used[MAX_TRIANGLES];

   int					strip_xyz[128];
	int					strip_st[128];
	int					strip_tris[128];
	int					stripcount;
//

   TCompleteModel();
   TCompleteModel(const TCompleteModel &Src);

   ~TCompleteModel();
	void AllocateAll(int Dealloc = 0);
   void DeallocateAll();

   void UpdateFrame(int FrameNo);
   void SetCurrentFrame(double FrameNo, int Next = -1);

   int BytesUsed();

   // Selection related functions
   Vector3 GetSelectionCentre(SelectType S);
   void LoadSelection(bool *Dest, CSelectionSource S);
   bool AnythingSelected();
   bool SelectConnected(bool *Buf, SelectType S);

   // Generalised stuffs
   void CalcNormals(unsigned int Action=CALCNORMS_ALL, int FrameNo = -1);
   void DeleteUnusedSkinVerts();
   int GetSkinVerticesFrom3DVertices(SkinVertPos SVP, double Tolerance, float MinGetS, float MinGetT, float MaxGetS, float MaxGetT, bool Mirror);

   // Operators
   TCompleteModel& operator =(const TCompleteModel &Src);


// File Related Stuff
   int TCompleteModel::LoadFromBestGuess(CString FileName, bool Merge);

   int SaveToMD2(CString FileName);
   int LoadFromMD2(CString FileName);

   int SaveToMDL(CString FileName);
   int LoadFromMDL(CString FileName);

   int SaveToAQM(CString FileName);
   int LoadFromAQM(CString FileName);

	int LoadFromASC(CString FileName, TImportMode M);
   int ExportFrameToASC(CString FileName, int FrameNum);
	int ExportSkinMeshToASC(CString FileName);
   int LoadSkinMeshFromASC(CString FileName);

	int LoadFromSQF(CString FileName, TImportMode M);

   int ParseLWOChunk_Poly(BYTE *Chunk, unsigned long size, TLoadVertFace *L);
   int ParseLWOChunk_Points(BYTE *Chunk, unsigned long size, TLoadVertFace *L);
   int ParseLWOChunk(BYTE *Chunk, TLoadVertFace *L);
   CString LoadFromLWO(CString FileName, TImportMode M);

   CString LoadFrom3DS(CString FileName, TImportMode M);
	int ParseChunk3DS(int handle, TRealVertex* V, TRealTriangle* T, int &numverts, int &numtris);
   int ParseFaceL13DS(int handle,TRealTriangle* T, int &numtris, int &numverts);
	int ParseVertexL3DS(int handle, TRealVertex* V, int &numverts);

	int ExportFrameTo3DS(CString FileName, int FrameNum);
	int WriteChunk3DS(int handle, unsigned short tag);

	int MergeFromFile(CString FileName, TModelFileType Type);

   // Quake2 md2 related functions
	void BuildGlCmds ();
	int FanLength(int starttri, int startv);
	int StripLength(int starttri, int startv);
};

#endif
