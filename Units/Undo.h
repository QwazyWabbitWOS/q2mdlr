//---------------------------------------------------------------------------
#ifndef UndoH
#define UndoH
//---------------------------------------------------------------------------

#define BASEUNDONODE    1
#define MOVEUNDONODE    2

#include "MenuItem.h"

class CBaseUndoNode
{
public:
	CBaseUndoNode *Prev, *Next;
	TCompleteModel *M;
   CString Name;

   CBaseUndoNode() { M = NULL ;};
   virtual ~CBaseUndoNode() { };

   void SetNode(CBaseUndoNode *Node) { M = Node->M ; Name = Node->Name;};
   void Set(CString N,TCompleteModel *Model) { M = Model; Name = N;};
   virtual int Execute() { return 0; };
   virtual CBaseUndoNode* CreateNew();
   virtual bool Reverse() { return true;};
   virtual int BytesUsed();
};

class CCompleteUndoNode : public CBaseUndoNode
{
public:
   TCompleteModel ModelBuffer;

   CCompleteUndoNode();
   virtual ~CCompleteUndoNode();

   virtual bool Reverse();
	void Set(CString N, TCompleteModel *Model);
   void SetNode(CCompleteUndoNode *N);
   virtual int Execute();
   virtual CBaseUndoNode* CreateNew();
   virtual int BytesUsed();
};



class CUndoList
{
protected:
   CMenuItem      m_MenuItem;

public:
	int            NodeCount;
   int            MaxNodes,MinNodes,MaxSize;
   bool           DeleteRedoOnAdd;
	CBaseUndoNode  *Start;
   CUndoList      *RedoList;
   CString        MenuPrefix;

   CUndoList();
   ~CUndoList();

   void AddNode( CBaseUndoNode *N	);  // inserts a node at start.
	void RemoveNode(); // deletes node at start
   void RemoveNode(int i); // deletes the ith node;
   void UndoLast(); // exeutes the first node, and deletes it
   void RemoveEndNode(); // deletes the node most removed from the start
   CString GetName();  // gets the name of the most recent undo nide
   int NumNodes();
   void UpdateMenuItem(); // updates the associated menu, if any.
   void ClearAllNodes(); // clears the entire list up

   void SetAssociatedMenu(HMENU hMenu, unsigned int IDItem);
   
   int BytesUsed();
};

#endif
