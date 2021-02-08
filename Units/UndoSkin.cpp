//---------------------------------------------------------------------------
#include "stdafx.h"

#include "Math.h"
#include "MdlGlobs.h"
#include "UndoSkin.h"

CSkinMoveUndoNode::CSkinMoveUndoNode()
{
	M = NULL;
   VertSel = NULL;
   Num = 0;
}

bool CSkinMoveUndoNode::Reverse()
{
   dx = -dx;
   dy = -dy;
   return true;
}

void CSkinMoveUndoNode::SetNode(CSkinMoveUndoNode *N)
{
   M = N->M;
   Name = N->Name;
   dx = N->dx;
   dy = N->dy;

   Num = N->Num;
   VertSel = new bool[M->Head.num_st];
   memcpy(VertSel, N->VertSel, sizeof(bool)*M->Head.num_st);
}

void CSkinMoveUndoNode::Set(CString N, TCompleteModel *Model, double x,double y, SelectType Sel)
{
	M = Model;
   Name = N;
   dx = x;
   dy = y;

   Num = M->Head.num_st;

   VertSel = new bool[M->Head.num_st];

   if (Sel == stVertex)
      for (int i=0;i<M->Head.num_st;i++)
      {
      	VertSel[i] = M->SkinVerts[i].Selected;
      }
   if (Sel == stTriangle)
   {
      for (int i=0;i<M->Head.num_st;i++)
         VertSel[i] = false;

      for (int t = 0;t<M->Head.num_tris;t++)
         if (M->Tris[t].SkinSelected)
            {
               VertSel[ M->Tris[t].SkinVerts[0] ] = true;
               VertSel[ M->Tris[t].SkinVerts[1] ] = true;
               VertSel[ M->Tris[t].SkinVerts[2] ] = true;
            }
   }
}

CBaseUndoNode *CSkinMoveUndoNode::CreateNew()
{
   CSkinMoveUndoNode   *N;
   N = new CSkinMoveUndoNode;
   N->SetNode(this);

   return N;
}

CSkinMoveUndoNode::~CSkinMoveUndoNode()
{
	if (VertSel)
   	delete[] VertSel;
}

int CSkinMoveUndoNode::Execute()
{
	for (int i=0;i<Num;i++)
   	if (VertSel[i])
      {
        	M->SkinVerts[i].s += (float)dx;
        	M->SkinVerts[i].t += (float)dy;
      }

   return 1;
}

int CSkinMoveUndoNode::BytesUsed()
{
   int B = 0;
   B += sizeof(*this);
   B += sizeof(bool)*Num;

   return B;
}

//---------------------------------------------------------------------------
CSkinRotateUndoNode::CSkinRotateUndoNode()
{
	M = NULL;
   VertSel = NULL;
}

bool CSkinRotateUndoNode::Reverse()
{
   dx = -dx;
   return true;
}

void CSkinRotateUndoNode::SetNode(CSkinRotateUndoNode *N)
{
   M = N->M;
   Name = N->Name;
   px = N->px;
   py = N->py;
   dx = N->dx;
   dy = N->dy;
   Num = N->Num;

   VertSel = new bool[M->Head.num_st];
   memcpy(VertSel, N->VertSel, sizeof(bool)*M->Head.num_st);
}

void CSkinRotateUndoNode::Set(CString N, TCompleteModel *Model,
   									double ipx,double ipy,
												double x, SelectType Sel)
{
	M = Model;
   Name = N;
   dx = x;
   px = ipx;
   py = ipy;
   Num = M->Head.num_st;

   VertSel = new bool[M->Head.num_st];
   if (Sel == stVertex)
      for (int i=0;i<M->Head.num_st;i++)
      {
      	VertSel[i] = M->SkinVerts[i].Selected;
      }

   if (Sel == stTriangle)
   {
      for (int i=0;i<M->Head.num_st;i++)
         VertSel[i] = false;

      for (int t = 0;t<M->Head.num_tris;t++)
         if (M->Tris[t].SkinSelected)
            {
               VertSel[ M->Tris[t].SkinVerts[0] ] = true;
               VertSel[ M->Tris[t].SkinVerts[1] ] = true;
               VertSel[ M->Tris[t].SkinVerts[2] ] = true;
            }
   }
}

CBaseUndoNode *CSkinRotateUndoNode::CreateNew()
{
   CSkinRotateUndoNode   *N;
   N = new CSkinRotateUndoNode;
   N->SetNode(this);

   return N;
}

CSkinRotateUndoNode::~CSkinRotateUndoNode()
{
	if (VertSel)
   	delete[] VertSel;
}

int CSkinRotateUndoNode::Execute()
{
	double cosx,sinx,nx,ny;

   cosx = cos(dx);
   sinx = sin(dx);

	for (int i=0;i<M->Head.num_st;i++)
   	if (VertSel[i])
      {
         nx = (double)((M->SkinVerts[i].s - px)*cosx + (M->SkinVerts[i].t - py)*sinx);
         ny = (double)((M->SkinVerts[i].t - py)*cosx - (M->SkinVerts[i].s - px)*sinx);
         nx += px;
         ny += py;
         M->SkinVerts[i].s = (float)nx;
         M->SkinVerts[i].t = (float)ny;
      }

   return 1;
}

int CSkinRotateUndoNode::BytesUsed()
{
   int B = 0;
   B += sizeof(*this);
   B += sizeof(bool)*Num;

   return B;
}

//---------------------------------------------------------------------------
CSkinScaleUndoNode::CSkinScaleUndoNode()
{
	M = NULL;
   VertSel = NULL;
}

bool CSkinScaleUndoNode::Reverse()
{
   if (dx == 0 || dy == 0)
      return false;

   dx = 1.0 / dx;
   dy = 1.0 / dy;
   return true;
}

void CSkinScaleUndoNode::SetNode(CSkinScaleUndoNode *N)
{
   M = N->M;
   Name = N->Name;
   px = N->px;
   py = N->py;
   dx = N->dx;
   dy = N->dy;

   Num = N->Num;

   VertSel = new bool[M->Head.num_st];
   memcpy(VertSel, N->VertSel, sizeof(bool)*M->Head.num_st);
}

void CSkinScaleUndoNode::Set(CString N, TCompleteModel *Model,
   									double ipx,double ipy,
												double x,double y,SelectType Sel)
{
	M = Model;
   Name = N;
   dx = x;
   dy = y;
   px = ipx;
   py = ipy;

   Num = M->Head.num_st;

   VertSel = new bool[M->Head.num_st];
   if (Sel == stVertex)
      for (int i=0;i<M->Head.num_st;i++)
      {
      	VertSel[i] = M->SkinVerts[i].Selected;
      }

   if (Sel == stTriangle)
   {
      for (int i=0;i<M->Head.num_st;i++)
         VertSel[i] = false;

      for (int t = 0;t<M->Head.num_tris;t++)
         if (M->Tris[t].Selected)
            {
               VertSel[ M->Tris[t].SkinVerts[0] ] = true;
               VertSel[ M->Tris[t].SkinVerts[1] ] = true;
               VertSel[ M->Tris[t].SkinVerts[2] ] = true;
            }

   }
}

CBaseUndoNode *CSkinScaleUndoNode::CreateNew()
{
   CSkinScaleUndoNode   *N;
   N = new CSkinScaleUndoNode;
   N->SetNode(this);

   return N;
}

CSkinScaleUndoNode::~CSkinScaleUndoNode()
{
	if (VertSel)
   	delete[] VertSel;
}

int CSkinScaleUndoNode::Execute()
{
	double nx,ny;

	for (int i=0;i<M->Head.num_st;i++)
   	if (VertSel[i])
      {
         nx = (double)((M->SkinVerts[i].s - px)*dx);
         nx += px;
         ny = (double)((M->SkinVerts[i].t - py)*dy);
         ny += py;

         M->SkinVerts[i].s = (float)nx;
         M->SkinVerts[i].t = (float)ny;
      }

   return 1;
}

int CSkinScaleUndoNode::BytesUsed()
{
   int B = 0;
   B += sizeof(*this);
   B += sizeof(bool)*Num;

   return B;
}

//---------------------------------------------------------------------------

CSkinSelectUndoNode::CSkinSelectUndoNode()
{
   M = NULL;
   NewSel = OldSel = NULL;
   Count = 0;
}

CSkinSelectUndoNode::~CSkinSelectUndoNode()
{
   if (NewSel)
      delete[] NewSel;

   if (OldSel)
      delete[] OldSel;
}

bool CSkinSelectUndoNode::Reverse()
{
   bool *B;
   B = new bool[Count];
   memcpy(B, OldSel, sizeof(bool)*Count);
   memcpy(OldSel, NewSel, sizeof(bool)*Count);
   if (Type == stVertex)
      M->LoadSelection(NewSel, ssSkinVert);
   if (Type == stTriangle)
      M->LoadSelection(NewSel, ssSkinTri);

   delete[] B;
   
   return true;
}

void CSkinSelectUndoNode::Set(CString N, TCompleteModel *Model, SelectType T, bool *Old, bool *New)
{
   M = Model;
   Name = N;
   Type = T;
   if (Type == stVertex)
      Count = M->Head.num_st;
   if (Type == stTriangle)
      Count = M->Head.num_tris;

   OldSel = new bool[Count];
   NewSel = new bool[Count];
   memcpy(OldSel, Old, sizeof(bool)*Count);
   memcpy(NewSel, New, sizeof(bool)*Count);
}

void CSkinSelectUndoNode::SetNode(CSkinSelectUndoNode *N)
{
   M = N->M;
   Name = N->Name;
   Type = N->Type;

   Count = N->Count;

   OldSel = new bool[Count];
   NewSel = new bool[Count];
   memcpy(OldSel, N->OldSel, sizeof(bool)*Count);
   memcpy(NewSel, N->NewSel, sizeof(bool)*Count);
}

int CSkinSelectUndoNode::Execute()
{
   if (OldSel==NULL || NewSel == NULL || M == NULL)
      return 0;

   if (Type == stVertex)
   {
      for (int v=0;v<M->Head.num_st;v++)
         M->SkinVerts[v].Selected = NewSel[v];
   }
   if (Type == stTriangle)
   {
      for (int t=0;t<M->Head.num_tris;t++)
         M->Tris[t].SkinSelected = NewSel[t];
   }

   return 1;
}

CBaseUndoNode *CSkinSelectUndoNode::CreateNew()
{
   CSkinSelectUndoNode   *N;
   N = new CSkinSelectUndoNode;
   N->SetNode(this);

   return N;
}

int CSkinSelectUndoNode::BytesUsed()
{
   int B = 0;
   B += sizeof(*this);
   B += sizeof(bool)*Count*2;

   return B;
}

//---------------------------------------------------------------------------


CSkinVertsUndoNode::CSkinVertsUndoNode()
{
   M = NULL;
   Name = "";
   STData = NULL;
   Tris = NULL;
   NumST = 0;
}

CSkinVertsUndoNode::~CSkinVertsUndoNode()
{
   if (STData)
      delete[] STData;
   if (Tris)
      delete[] Tris;
}


bool CSkinVertsUndoNode::Reverse()
{
   NumST = M->Head.num_st;
   NumTris = M->Head.num_tris;

   if (STData)
      delete[] STData;
   if (Tris)
      delete[] Tris;

   STData = new TRealSkinVertex[NumST];
   Tris = new TRealTriangle[NumTris];

   memcpy(STData, M->SkinVerts, sizeof(TRealSkinVertex)*NumST);
   memcpy(Tris, M->Tris, sizeof(TRealTriangle)*NumTris);

   return true;
}

void CSkinVertsUndoNode::Set(CString N, TCompleteModel *Model)
{
   Name = N;
   M = Model;
   NumST = M->Head.num_st;
   NumTris = M->Head.num_tris;

   if (STData)
      delete[] STData;
   if (Tris)
      delete[] Tris;

   STData = new TRealSkinVertex[NumST];
   Tris = new TRealTriangle[NumTris];
   memcpy(STData, M->SkinVerts, sizeof(TRealSkinVertex)*NumST);
   memcpy(Tris, M->Tris , sizeof(TRealTriangle)*NumTris);
}

void CSkinVertsUndoNode::SetNode(CSkinVertsUndoNode *N)
{
   Name = N->Name;
   M = N->M;
   NumST = N->NumST;
   NumTris = N->NumTris;

   if (STData)
      delete[] STData;
   if (Tris)
      delete[] Tris;

   STData = new TRealSkinVertex[NumST];
   Tris = new TRealTriangle[NumTris];
   memcpy(STData, N->STData, sizeof(TRealSkinVertex)*NumST);
   memcpy(Tris, N->Tris, sizeof(TRealTriangle)*NumTris);
}

int CSkinVertsUndoNode::Execute()
{
   if (STData == NULL)
      return 0;

   if (M->SkinVerts)
      delete[] M->SkinVerts;
   if (M->Tris)
      delete[] M->Tris;

   M->SkinVerts = new TRealSkinVertex[NumST];
   memcpy(M->SkinVerts, STData, sizeof(TRealSkinVertex)*NumST);
   M->Head.num_st = NumST;

   M->Tris = new TRealTriangle[NumTris];
   memcpy(M->Tris, Tris, sizeof(TRealTriangle)*NumTris);
   M->Head.num_tris = NumTris;
   return 1;
}

CBaseUndoNode *CSkinVertsUndoNode::CreateNew()
{
   CSkinVertsUndoNode *N;
   N = new CSkinVertsUndoNode;

   N->SetNode(this);

   return N;
}

int CSkinVertsUndoNode::BytesUsed()
{
   int B = 0;
   B += sizeof(*this);
   B += sizeof(TRealSkinVertex)*NumST;
   B += sizeof(TRealTriangle)*NumTris;

   return B;
}

//---------------------------------------------------------------------------

CSkinPaintUndoNode::CSkinPaintUndoNode()
{
   SkinData = NULL;
   SkinNo = -1;
   Width = Height = 0;
}

CSkinPaintUndoNode::~CSkinPaintUndoNode()
{
   if (SkinData)
      delete[] SkinData;
}

bool CSkinPaintUndoNode::Reverse()
{
   if (!SkinData)
      return false;

   memcpy(SkinData, M->Skins[SkinNo], Width*Height);
   return true;
}

void CSkinPaintUndoNode::Set(CString N, TCompleteModel *Model, int nSkin)
{
   M = Model;
   Name = N;
   SkinNo = nSkin;
   Width = M->Head.skinwidth;
   Height = M->Head.skinheight;

   if (SkinData)
      delete[] SkinData;

   SkinData = new BYTE[Width*Height];

   memcpy(SkinData, M->Skins[SkinNo], Width*Height);
}

void CSkinPaintUndoNode::SetNode(CSkinPaintUndoNode *N)
{
   if (N == NULL)
      return;

   M = N->M;
   Name = N->Name;
   SkinNo = N->SkinNo;
   Width = N->Width;
   Height = N->Height;

   if (SkinData)
      delete[] SkinData;

   SkinData = new BYTE[Width*Height];
   memcpy(SkinData, N->SkinData, Width*Height);

}

int CSkinPaintUndoNode::Execute()
{
   memcpy(M->Skins[SkinNo], SkinData, Width*Height);
   return 1;
}

CBaseUndoNode *CSkinPaintUndoNode::CreateNew()
{
   CSkinPaintUndoNode *Node;

   Node = new CSkinPaintUndoNode;

   Node->SetNode(this);

   return Node;
}

int CSkinPaintUndoNode::BytesUsed()
{
   int B = 0;
   B += sizeof(*this);
   B += sizeof(BYTE)*Width*Height;

   return B;
}

