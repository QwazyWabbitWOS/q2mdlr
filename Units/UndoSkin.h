//---------------------------------------------------------------------------
#ifndef SkinUndoH
#define SkinUndoH

#include "Undo.h"

class CSkinMoveUndoNode : public CBaseUndoNode
{
public:
	bool *VertSel;
   double dx,dy;
   int Num;

   CSkinMoveUndoNode();
   virtual ~CSkinMoveUndoNode();

   virtual bool Reverse();
   void SetNode(CSkinMoveUndoNode *N);
	void Set(CString N, TCompleteModel *Model, double x,double y, SelectType Sel);
   virtual int Execute();
   virtual int BytesUsed();
   CBaseUndoNode *CreateNew();
};

class CSkinRotateUndoNode : public CBaseUndoNode
{
public:
	bool *VertSel;
   double dx,dy;
   double px,py;
   int Num;

   CSkinRotateUndoNode();
   virtual ~CSkinRotateUndoNode();

   virtual bool Reverse();
   void SetNode(CSkinRotateUndoNode *N);
	void Set(CString N, TCompleteModel *Model,
   									double ipx,double ipy,
   									double x, SelectType Sel);
   virtual int Execute();
   virtual int BytesUsed();
   CBaseUndoNode *CreateNew();
};

class CSkinScaleUndoNode : public CBaseUndoNode
{
public:
	bool *VertSel;
   double dx,dy;
   double px,py;
   int Num;

   CSkinScaleUndoNode();
   virtual ~CSkinScaleUndoNode();

   virtual bool Reverse();
   void SetNode(CSkinScaleUndoNode *N);
	void Set(CString N, TCompleteModel *Model,
   									double ipx,double ipy,
   									double x,double y, SelectType Sel);
   virtual int Execute();
   virtual int BytesUsed();
   CBaseUndoNode *CreateNew();
};

class CSkinSelectUndoNode : public CBaseUndoNode
{
public:
   bool        *NewSel, *OldSel;
   SelectType  Type;
   int         Count;

   CSkinSelectUndoNode();
   virtual ~CSkinSelectUndoNode();

   virtual bool Reverse();
   void Set(CString N, TCompleteModel *Model, SelectType T, bool *Old, bool *New);
   void SetNode(CSkinSelectUndoNode *N);
   virtual int Execute();
   virtual int BytesUsed();
   virtual CBaseUndoNode *CreateNew();
};

class CSkinVertsUndoNode : public CBaseUndoNode
{
public:
   TRealSkinVertex   *STData;
   TRealTriangle      *Tris;
   int               NumST,NumTris;

   CSkinVertsUndoNode();
   virtual ~CSkinVertsUndoNode();

   virtual bool Reverse();
   void Set(CString N, TCompleteModel *Model);
   void SetNode(CSkinVertsUndoNode *N);
   virtual int Execute();
   virtual int BytesUsed();
   virtual CBaseUndoNode *CreateNew();
};

class CSkinPaintUndoNode : public CBaseUndoNode
{
public:
   int   SkinNo, Width,Height;
   BYTE  *SkinData;

   CSkinPaintUndoNode();
   virtual ~CSkinPaintUndoNode();

   virtual bool Reverse();
   void Set(CString N, TCompleteModel *Model, int nSkin);
   void SetNode(CSkinPaintUndoNode *N);
   virtual int Execute();
   virtual int BytesUsed();
   virtual CBaseUndoNode *CreateNew();
};

//---------------------------------------------------------------------------
#endif
