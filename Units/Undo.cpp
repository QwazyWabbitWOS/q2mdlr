//---------------------------------------------------------------------------
#include "stdafx.h"
#include <math.h>

#include "MdlType.h"
#include "MenuItem.h"
#include "Undo.h"
//---------------------------------------------------------------------------

CBaseUndoNode *CBaseUndoNode::CreateNew()
{
   CBaseUndoNode   *N;

   N = new CBaseUndoNode;
   N->SetNode(this);

   return N;
}

int CBaseUndoNode::BytesUsed()
{
   return sizeof(*this);
}


CUndoList::CUndoList()
{
   Start = NULL;
   MaxNodes = -1;
   MinNodes=-1;
   MaxSize=-1;
   NodeCount = 0;
   RedoList = NULL;
   DeleteRedoOnAdd = false;
}
CUndoList::~CUndoList()
{
	CBaseUndoNode *B, *N;

   B = Start;

   while (B)
   {
		N = B->Next;

      delete B;

      B = N;
   }
}


void CUndoList::SetAssociatedMenu(HMENU hMenu, unsigned int IDItem)
{
   m_MenuItem.SetItem(hMenu, IDItem);
}


void CUndoList::AddNode( CBaseUndoNode *N)
{
   if (DeleteRedoOnAdd && RedoList)
      RedoList->ClearAllNodes();

	if (Start == NULL)  // this is the firt and only node
   {
   	Start = N;
      N->Next = NULL;
      N->Prev = NULL;

      UpdateMenuItem();
      return;
   }

   Start->Prev = N;
   N->Next = Start;
	N->Prev = NULL;
   Start = N;

   NodeCount++;

   if (MaxNodes != -1)
   {
      int Count=0;
      CBaseUndoNode *B = Start;
      while (B)
      {
         Count++;
         B = B->Next;
      }
      if (Count>MaxNodes)
         RemoveEndNode();
   }
   if (MaxSize != -1)
   {
      int Size=0,Count=0;
      CBaseUndoNode *B = Start;
      while (B)
      {
         Count++;
         Size += B->BytesUsed();
         B = B->Next;
      }
      if (Count>MinNodes && Size>MaxSize)
         RemoveEndNode();
   }

   if (NodeCount > MaxNodes && MaxNodes != -1)
   {
   	CBaseUndoNode *B = Start;
      if (Start == NULL)
      	return;

      while (B->Next)
      {
         B = B->Next;
      }
      B->Prev->Next = NULL;
      delete B;
      NodeCount--;
   }

   UpdateMenuItem();
}

void CUndoList::RemoveEndNode()
{
   CBaseUndoNode *B = Start;
   if (Start == NULL)
      return;

   while (B->Next)
      B = B->Next;

   if (B->Prev)
      B->Prev->Next = NULL;
   else
      Start = NULL;
   delete B;
   NodeCount--;
}

void CUndoList::RemoveNode()
{
   CBaseUndoNode *N;

	if (Start == NULL)
   	return;

   N = Start->Next;
   delete Start;
   Start = N;
   if (N)
   	N->Prev = NULL;

   NodeCount--;
}

void CUndoList::RemoveNode(int i)
{
	CBaseUndoNode *N = Start;
	for (int n=0;n<i;n++)
   {
   	if (N == NULL)
      	return;
      else
			N = N->Next;
   }

   if (N->Prev)
	   N->Prev->Next = N->Next;
   if (N->Next)
   	N->Next->Prev = N->Prev;

   delete N;
   NodeCount--;
}

void CUndoList::UndoLast()
{
	if (Start == NULL)
   	return;


   if (RedoList)
   {
      bool OldDelete = RedoList->DeleteRedoOnAdd;
      RedoList->DeleteRedoOnAdd = false;

      CBaseUndoNode   *N;
      N = Start->CreateNew();
      N->Reverse();
      RedoList->AddNode(N);

      RedoList->DeleteRedoOnAdd = OldDelete;
   }
	Start->Execute();
   RemoveNode();
   NodeCount--;


   UpdateMenuItem();
}

CString CUndoList::GetName()
{
	if (Start == NULL)
   	return "";

   return Start->Name;
}

void CUndoList::UpdateMenuItem()
{
   if (Start == NULL)
   {
      m_MenuItem.SetCaption(MenuPrefix);
      m_MenuItem.SetEnabled(false);
      return;
   }

   m_MenuItem.SetEnabled(true);
   m_MenuItem.SetCaption(MenuPrefix + GetName());
}

int CUndoList::NumNodes()
{
   int            num=0;
   CBaseUndoNode   *N;

   N = Start;

   while (N)
   {
      N = N->Next;
      num++;
   }

   return num;
}

int CUndoList::BytesUsed()
{
   int            num=0;
   CBaseUndoNode   *N;

   N = Start;

   while (N)
   {
      num += N->BytesUsed();
      N = N->Next;
   }

   return num;
}

void CUndoList::ClearAllNodes()
{
	CBaseUndoNode *B, *N;

   B = Start;
   while (B)
   {
		N = B->Next;
      delete B;
      B = N;
   }

   Start = NULL;

   UpdateMenuItem();
}

//------------------------------------------------------------------------

CCompleteUndoNode::CCompleteUndoNode()
{

}

CCompleteUndoNode::~CCompleteUndoNode()
{
}

bool CCompleteUndoNode::Reverse()
{
   ModelBuffer = *M;
   return true;
}

void CCompleteUndoNode::Set(CString N, TCompleteModel *Model)
{
   M = Model;
   Name = N;

   ModelBuffer = *M;
}

void CCompleteUndoNode::SetNode(CCompleteUndoNode *N)
{
   Name = N->Name;
   M = N->M;

   ModelBuffer = N->ModelBuffer;
}

int CCompleteUndoNode::Execute()
{
   *M = ModelBuffer;
   return 1;
}

CBaseUndoNode *CCompleteUndoNode::CreateNew()
{
   CCompleteUndoNode *New;

   New = new CCompleteUndoNode;
   New->SetNode(this);
   return New;
}

int CCompleteUndoNode::BytesUsed()
{
   return sizeof(CCompleteUndoNode) - sizeof(ModelBuffer) + ModelBuffer.BytesUsed();
}


