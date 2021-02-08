#include "stdafx.h"

#include <Math.h>

#include "MdlType.h"
#include "Undo3D.h"

CMoveUndoNode::CMoveUndoNode()
{
	M = NULL;
   VertSel = NULL;
   NumVerts = 0;
}

bool CMoveUndoNode::Reverse()
{
   dx = -dx;
   dy = -dy;
   dz = -dz;

   return true;
}

void CMoveUndoNode::SetNode(CMoveUndoNode *N)
{
   M = N->M;
   Name = N->Name;
   NumVerts = M->Head.num_xyz;
   MinFrame = N->MinFrame;
   MaxFrame = N->MaxFrame;
   dx = N->dx;
   dy = N->dy;
   dz = N->dz;

   VertSel = new bool[NumVerts];
   memcpy(VertSel, N->VertSel, sizeof(bool)*NumVerts);
}

void CMoveUndoNode::Set(CString N, TCompleteModel *Model, int Min, int Max,float x,float y,float z, SelectType Sel)
{
	M = Model;
   Name = N;
   NumVerts = M->Head.num_xyz;
   MinFrame = Min;
   MaxFrame = Max;
   dx = x;
   dy = y;
   dz = z;

   VertSel = new bool[NumVerts];

   if (Sel == stVertex)
      for (int i=0;i<NumVerts;i++)
      {
      	VertSel[i] = M->Verts[i].Selected && M->Verts[i].Visible;
      }
   if (Sel == stTriangle)
   {
      for (int i=0;i<NumVerts;i++)
         VertSel[i] = false;

      for (int t = 0;t<M->Head.num_tris;t++)
         if (M->Tris[t].Visible && M->Tris[t].Selected)
            {
               VertSel[ M->Tris[t].Vertices[0] ] = true;
               VertSel[ M->Tris[t].Vertices[1] ] = true;
               VertSel[ M->Tris[t].Vertices[2] ] = true;
            }
   }
}

CBaseUndoNode *CMoveUndoNode::CreateNew()
{
   CMoveUndoNode   *N;
   N = new CMoveUndoNode;
   N->SetNode(this);

   return N;
}

CMoveUndoNode::~CMoveUndoNode()
{
	if (VertSel)
   	delete[] VertSel;
}

int CMoveUndoNode::Execute()
{
	for (int i=0;i<NumVerts;i++)
   	if (VertSel[i])
      {
      	for (int f=MinFrame;f<=MaxFrame;f++)
         {
         	M->Frames[f][i].x += (float)dx;
         	M->Frames[f][i].y += dy;
         	M->Frames[f][i].z += dz;
         }

      }

   return 1;
}

int CMoveUndoNode::BytesUsed()
{
   int B=0;
   B+=sizeof(*this);
   B+=sizeof(bool)*NumVerts;

   return B;
}

//---------------------------------------------------------------------------
CRotateUndoNode::CRotateUndoNode()
{
	M = NULL;
   VertSel = NULL;
}

bool CRotateUndoNode::Reverse()
{
   dx = -dx;
   dy = -dy;
   dz = -dz;

   return true;
}

void CRotateUndoNode::SetNode(CRotateUndoNode *N)
{
   M = N->M;
   Name = N->Name;
   NumVerts = M->Head.num_xyz;
   MinFrame = N->MinFrame;
   MaxFrame = N->MaxFrame;
   px = N->px;
   py = N->py;
   pz = N->pz;
   dx = N->dx;
   dy = N->dy;
   dz = N->dz;

   VertSel = new bool[NumVerts];
   memcpy(VertSel, N->VertSel, sizeof(bool)*NumVerts);
}

void CRotateUndoNode::Set(CString N, TCompleteModel *Model, int Min, int Max,
   									float ipx,float ipy,float ipz,
												float x,float y,float z, SelectType Sel)
{
	M = Model;
   Name = N;
   NumVerts = M->Head.num_xyz;
   MinFrame = Min;
   MaxFrame = Max;
   dx = x;
   dy = y;
   dz = z;
   px = ipx;
   py = ipy;
   pz = ipz;

   VertSel = new bool[NumVerts];
   if (Sel == stVertex)
      for (int i=0;i<NumVerts;i++)
      {
      	VertSel[i] = M->Verts[i].Selected && M->Verts[i].Visible;
      }

   if (Sel == stTriangle)
   {
      for (int i=0;i<NumVerts;i++)
         VertSel[i] = false;

      for (int t = 0;t<M->Head.num_tris;t++)
         if (M->Tris[t].Visible && M->Tris[t].Selected)
            {
               VertSel[ M->Tris[t].Vertices[0] ] = true;
               VertSel[ M->Tris[t].Vertices[1] ] = true;
               VertSel[ M->Tris[t].Vertices[2] ] = true;
            }
   }
}

CBaseUndoNode *CRotateUndoNode::CreateNew()
{
   CRotateUndoNode   *N;
   N = new CRotateUndoNode;
   N->SetNode(this);

   return N;
}

CRotateUndoNode::~CRotateUndoNode()
{
	if (VertSel)
   	delete[] VertSel;
}

int CRotateUndoNode::Execute()
{
	float cosx,cosy,cosz,sinx,siny,sinz,nx,ny,nz;

   cosx = (float)cos(dx);
   cosy = (float)cos(dy);
   cosz = (float)cos(dz);
   sinx = (float)sin(dx);
   siny = (float)sin(dy);
   sinz = (float)sin(dz);

	for (int i=0;i<NumVerts;i++)
   	if (VertSel[i])
      {
      	for (int f=MinFrame;f<=MaxFrame;f++)
         {
            nx = (float)(M->Frames[f][i].x - px)*cosx + (M->Frames[f][i].y - py)*sinx;
            ny = (float)(M->Frames[f][i].y - py)*cosx - (M->Frames[f][i].x - px)*sinx;
            nx += px;
            ny += py;
            M->Frames[f][i].x = nx;
            M->Frames[f][i].y = ny;

            nx = (float)(M->Frames[f][i].x - px)*cosy + (M->Frames[f][i].z - pz)*siny;
            nz = (float)(M->Frames[f][i].z - pz)*cosy - (M->Frames[f][i].x - px)*siny;
            nx += px;
            nz += pz;
            M->Frames[f][i].x = nx;
            M->Frames[f][i].z = nz;

            nz = (float)(M->Frames[f][i].z - pz)*cosz + (M->Frames[f][i].y - py)*sinz;
            ny = (float)(M->Frames[f][i].y - py)*cosz - (M->Frames[f][i].z - pz)*sinz;
            nz += pz;
            ny += py;
            M->Frames[f][i].z = nz;
            M->Frames[f][i].y = ny;
         }

      }

   return 1;
}

int CRotateUndoNode::BytesUsed()
{
   int B=0;
   B+=sizeof(*this);
   B+=sizeof(bool)*NumVerts;

   return B;
}

//---------------------------------------------------------------------------
CScaleUndoNode::CScaleUndoNode()
{
	M = NULL;
   VertSel = NULL;
}

bool CScaleUndoNode::Reverse()
{
   if (dx == 0 || dy == 0 || dz == 0)
      return false;

   dx = (float)(1.0 / dx);
   dy = (float)(1.0 / dy);
   dz = (float)(1.0 / dz);

   return true;
}

void CScaleUndoNode::SetNode(CScaleUndoNode *N)
{
   M = N->M;
   Name = N->Name;
   NumVerts = M->Head.num_xyz;
   MinFrame = N->MinFrame;
   MaxFrame = N->MaxFrame;
   px = N->px;
   py = N->py;
   pz = N->pz;
   dx = N->dx;
   dy = N->dy;
   dz = N->dz;

   VertSel = new bool[NumVerts];
   memcpy(VertSel, N->VertSel, sizeof(bool)*NumVerts);
}

void CScaleUndoNode::Set(CString N, TCompleteModel *Model, int Min, int Max,
   									float ipx,float ipy,float ipz,
												float x,float y,float z, SelectType Sel)
{
	M = Model;
   Name = N;
   NumVerts = M->Head.num_xyz;
   MinFrame = Min;
   MaxFrame = Max;
   dx = x;
   dy = y;
   dz = z;
   px = ipx;
   py = ipy;
   pz = ipz;

   VertSel = new bool[NumVerts];
   if (Sel == stVertex)
      for (int i=0;i<NumVerts;i++)
      {
      	VertSel[i] = M->Verts[i].Selected && M->Verts[i].Visible;
      }

   if (Sel == stTriangle)
   {
      for (int i=0;i<NumVerts;i++)
         VertSel[i] = false;

      for (int t = 0;t<M->Head.num_tris;t++)
         if (M->Tris[t].Visible && M->Tris[t].Selected)
            {
               VertSel[ M->Tris[t].Vertices[0] ] = true;
               VertSel[ M->Tris[t].Vertices[1] ] = true;
               VertSel[ M->Tris[t].Vertices[2] ] = true;
            }

   }
}

CBaseUndoNode *CScaleUndoNode::CreateNew()
{
   CScaleUndoNode   *N;
   N = new CScaleUndoNode;
   N->SetNode(this);

   return N;
}

CScaleUndoNode::~CScaleUndoNode()
{
	if (VertSel)
   	delete[] VertSel;
}

int CScaleUndoNode::Execute()
{
	float nx,ny,nz;

	for (int i=0;i<NumVerts;i++)
   	if (VertSel[i])
      {
      	for (int f=MinFrame;f<=MaxFrame;f++)
         {
         	nx = (float)(M->Frames[f][i].x - px)*dx;
            nx += px;
         	ny = (float)(M->Frames[f][i].y - py)*dy;
            ny += py;
         	nz = (float)(M->Frames[f][i].z - pz)*dz;
            nz += pz;

				M->Frames[f][i].x = nx;
				M->Frames[f][i].y = ny;
				M->Frames[f][i].z = nz;
         }

      }

   return 1;
}

int CScaleUndoNode::BytesUsed()
{
   int B=0;
   B+=sizeof(*this);
   B+=sizeof(bool)*NumVerts;

   return B;
}

//---------------------------------------------------------------------------

CSelectUndoNode::CSelectUndoNode()
{
   M = NULL;
   NewSel = NULL;
   Count = 0;
}

CSelectUndoNode::~CSelectUndoNode()
{
   if (NewSel)
      delete[] NewSel;
}

bool CSelectUndoNode::Reverse()
{
   if (Type == stVertex)
      M->LoadSelection(NewSel, ssVertex);
   else
   if (Type == stTriangle)
      M->LoadSelection(NewSel, ssTriangle);
   else
      return false;

   return true;
}

void CSelectUndoNode::Set(CString N, TCompleteModel *Model, SelectType T, bool *New)
{
   M = Model;
   Name = N;
   Type = T;
   if (Type == stVertex)
      Count = M->Head.num_xyz;
   if (Type == stTriangle)
      Count = M->Head.num_tris;

   NewSel = new bool[Count];
   memcpy(NewSel, New, sizeof(bool)*Count);
}

void CSelectUndoNode::SetNode(CSelectUndoNode *N)
{
   M = N->M;
   Name = N->Name;
   Type = N->Type;

   Count = N->Count;

   NewSel = new bool[Count];
   memcpy(NewSel, N->NewSel, sizeof(bool)*Count);
}

int CSelectUndoNode::Execute()
{
   if (NewSel == NULL || M == NULL)
      return 0;

   if (Type == stVertex)
   {
      for (int v=0;v<M->Head.num_xyz;v++)
         M->Verts[v].Selected = NewSel[v];
   }
   if (Type == stTriangle)
   {
      for (int t=0;t<M->Head.num_tris;t++)
         M->Tris[t].Selected = NewSel[t];
   }

   return 1;
}

CBaseUndoNode *CSelectUndoNode::CreateNew()
{
   CSelectUndoNode   *N;
   N = new CSelectUndoNode;
   N->SetNode(this);

   return N;
}

int CSelectUndoNode::BytesUsed()
{
   int B=0;
   B+=sizeof(*this);
   B+=sizeof(bool)*Count;
   B+=sizeof(bool)*Count;

   return B;
}



//---------------------------------------------------------------------------



