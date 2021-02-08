//---------------------------------------------------------------------------
#ifndef _3DUndoH
#define _3DUndoH
//---------------------------------------------------------------------------

#include "Undo.h"


class CMoveUndoNode : public CBaseUndoNode
{
public:
	bool *VertSel;
   int NumVerts;
   int MinFrame, MaxFrame;
   float dx,dy,dz;

   CMoveUndoNode();
   virtual ~CMoveUndoNode();

   bool Reverse();
	void Set(CString N, TCompleteModel *Model, int Min, int Max,float x,float y,float z, SelectType Sel);
   void SetNode(CMoveUndoNode *N);
   virtual int Execute();
   virtual CBaseUndoNode *CreateNew();
   virtual int BytesUsed();
};

class CRotateUndoNode : public CBaseUndoNode
{
public:
	bool *VertSel;
   int NumVerts;
   int MinFrame, MaxFrame;
   float dx,dy,dz;
   float px,py,pz;

   CRotateUndoNode();
   virtual ~CRotateUndoNode();

   virtual bool Reverse();
	void Set(CString N, TCompleteModel *Model, int Min, int Max,
   									float ipx,float ipy,float ipz,
   									float x,float y,float z, SelectType Sel);
   void SetNode(CRotateUndoNode *N);
   virtual int Execute();
   virtual CBaseUndoNode *CreateNew();
   virtual int BytesUsed();
};

class CScaleUndoNode : public CBaseUndoNode
{
public:
	bool *VertSel;
   int NumVerts;
   int MinFrame, MaxFrame;
   float dx,dy,dz;
   float px,py,pz;

   CScaleUndoNode();
   virtual ~CScaleUndoNode();

   virtual bool Reverse();
	void Set(CString N, TCompleteModel *Model, int Min, int Max,
   									float ipx,float ipy,float ipz,
   									float x,float y,float z, SelectType Sel);
   void SetNode(CScaleUndoNode *N);
   virtual int Execute();
   virtual CBaseUndoNode *CreateNew();
   virtual int BytesUsed();
};

class CSelectUndoNode : public CBaseUndoNode
{
public:
   bool        *NewSel;
   SelectType  Type;
   int         Count;

   CSelectUndoNode();
   virtual ~CSelectUndoNode();

   virtual bool Reverse();
   void Set(CString N, TCompleteModel *Model, SelectType T, bool *New);
   void SetNode(CSelectUndoNode *N);
   virtual int Execute();
   virtual CBaseUndoNode *CreateNew();
   virtual int BytesUsed();
};


#endif
