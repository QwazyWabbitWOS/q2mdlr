//---------------------------------------------------------------------------
#include "stdafx.h"

#include "MdlConst.h"
#include "MdlType.h"
#include "Matrix.h"
#include "Bones.h"

#include <string.h>
#include <math.h>

#include "MdlConst.h"
#include "Matrix.h"
//---------------------------------------------------------------------------


TBone::TBone()
{
	Parent = Child = PrevSib = NextSib = NULL;

   NumVerts = 0;

	Pivot.Set(0,0,0);
	OriginalPivot.Set(0,0,0);

   for (int i=0;i<MAX_FRAMES;i++)
   {
		RotX[i] = RotY[i] = RotZ[i] = 0;
      ScaleX[i] = ScaleY[i] = ScaleZ[i] = 1;
   }
   TransX = TransY = TransZ = 0.0;

   memset(Name, 0,16);
}

TBone::TBone(float x, float y, float z)
{
	Parent = Child = PrevSib = NextSib = NULL;

   NumVerts = 0;

	Pivot.Set(x,y,z);
	OriginalPivot.Set(x,y,z);

   for (int i=0;i<MAX_FRAMES;i++)
   {
		RotX[i] = RotY[i] = RotZ[i] = 0;
      ScaleX[i] = ScaleY[i] = ScaleZ[i] = 1;
   }
	TransX = TransY = TransZ = 0.0;

   Selected = false;
   Active = false;

   memset(Name, 0,16);
}

TBone::TBone(char *N)
{
	Parent = Child = PrevSib = NextSib = NULL;

   NumVerts = 0;

	Pivot.Set(0,0,0);
	OriginalPivot.Set(0,0,0);

   for (int i=0;i<MAX_FRAMES;i++)
   {
		RotX[i] = RotY[i] = RotZ[i] = 0;
      ScaleX[i] = ScaleY[i] = ScaleZ[i] = 1;
   }
	TransX = TransY = TransZ = 0.0;

   Selected = false;
   Active = false;

   memset(Name, 0,16);

	strcpy(Name, N);
}

TBone::TBone(float x, float y, float z, char *N)
{
	Parent = Child = PrevSib = NextSib = NULL;

   NumVerts = 0;

	Pivot.Set(x,y,z);
	OriginalPivot.Set(x,y,z);

   for (int i=0;i<MAX_FRAMES;i++)
   {
		RotX[i] = RotY[i] = RotZ[i] = 0;
      ScaleX[i] = ScaleY[i] = ScaleZ[i] = 1;
   }
	TransX = TransY = TransZ = 0.0;
   Selected = false;
   Active = false;

	strcpy(Name, N);
}

TBoneList::TBoneList()
{
	Root = NULL;
}

void TBoneList::DeleteTree(TBone *B) // deletes self and all chidren nodes
{
   if (B == NULL) return;

	TBone	*Sib, *Next;

   Sib = B->Child;

   while (Sib != NULL)
   {
   	Next = Sib->NextSib;
      DeleteTree(Sib);

      Sib = Next;
   }

   delete B;
}

TBoneList::~TBoneList()
{
	// walk the tree and delete all nodes.
   TBone *B,*Next;

   B = Root;
   while(B)
	{
   	Next = B->NextSib;
	   DeleteTree(B);
      B = Next;
	}
   Root = NULL;
}

TBone *TBoneList::AddBone(float x, float y, float z, char *N)
{
	TBone *B = AddBone(x,y,z);
   strcpy(B->Name, N);
   return B;
}
TBone *TBoneList::AddBone(float x, float y, float z)
{
	TBone *NewBone;

   NewBone = new TBone(x,y,z);

   if (Root != NULL)
   {
   	Root->PrevSib = NewBone;
      NewBone->PrevSib = NULL;
      NewBone->NextSib = Root;
      NewBone->Parent = NULL;
      Root = NewBone;
   }
   else
   {
   	NewBone->PrevSib = NULL;
      NewBone->NextSib = NULL;
      NewBone->Parent = NULL;
      Root = NewBone;
   }

   return NewBone;
}

TBone *TBone::AddChild(float x, float y, float z, char *N)
{
	TBone *B;
   B = AddChild(x,y,z);
   strcpy(B->Name, N);
   return B;
}
TBone *TBone::AddChild(float x, float y, float z)
{
	TBone	*temp;

   temp = new TBone(x,y,z);

	if (Child == NULL)   // Bone has not children
   {
   	temp->PrevSib = NULL;
      temp->NextSib = NULL;
      temp->Parent = this;

      Child = temp;
   }
   else		// Bone has some children, put new bone in 1st position.
   {
   	Child->PrevSib = temp;
      temp->PrevSib = NULL;
      temp->NextSib = Child;
      temp->Parent = this;

      Child = temp;
   }

   return temp;
}

void TBone::SetParent(TBone *P) // sets the parent bone to P
{
	// trying to assign self as parent

	if (this == P)
   	return;
	if (P == NULL)	// set tbone to root node one removed
   {
      TBone *Root;
      Root = this;

      // find out abosulte root node
      while (Root->Parent)
      	Root = Root->Parent;
      while (Root->PrevSib)
      	Root = Root->PrevSib;

      if (Root == this)	//  is this already root?
      	return;

		// break link
   	if (NextSib)
      	NextSib->PrevSib = PrevSib;
   	if (PrevSib)
      	PrevSib->NextSib = NextSib;

      // place this bone

      Parent = NULL;
      PrevSib = Root;
      NextSib = Root->NextSib;
      if (NextSib)
      	NextSib->PrevSib = this;

      Root->NextSib = this;

      return;
   }

   // parent is actual node
   if (NextSib)
      NextSib->PrevSib = PrevSib;
   if (PrevSib)
      PrevSib->NextSib = NextSib;

   Parent = P;
   PrevSib = NULL;
   NextSib = P->Child;
   if (P->Child != NULL)
	   P->Child->PrevSib = this;
   P->Child = this;
}

void TBoneList::DeleteBone(TBone *B) // deletes bone, and roots its children
{
	if (B == NULL)
   	return;

	TBone	*Prev,*Next;

   if (B->Child != NULL)	// Bone has some children
   {
   	TBone *Last = Root;

      while (Last->NextSib)
      	Last = Last->NextSib; // find last Root bone

		Last->NextSib = B->Child;
      B->Child->PrevSib = Last;

      Last = B->Child;
      while (Last)
      {
      	Last->Parent = NULL;
      	Last = Last->NextSib;
      }

   }

   Prev = B->PrevSib;
   Next = B->NextSib;

   if (Prev)
		Prev->NextSib = Next;
   if (Next)
   	Next->PrevSib = Prev;

   if (Prev == NULL) // bone 'B' is the first child of a bone
   	B->Parent->Child = Next;

	delete B;
}



void TBoneList::SelectChildrenNear(TBone *Bone, float x, float y, float z,float Thresh,
											bool dox, bool doy, bool doz, bool Targ)
{
   TBone *B;
   B = Bone->Child;
   while (B)
   {
   	SelectChildrenNear(B, x,y,z, Thresh, dox,doy,doz,Targ);
   	B = B->NextSib;
   }

   if (dox)
   {
   	if ( fabs(Bone->Pivot.x - x) > Thresh)
         return;
   }

   if (doy)
   {
   	if ( fabs(Bone->Pivot.y - y) > Thresh)
         return;
   }

   if (doz)
   {
   	if ( fabs(Bone->Pivot.z - z) > Thresh)
         return;
   }

   Bone->Selected = Targ;
}

void TBoneList::SelectBonesNear(float x, float y, float z,float Thresh,
											bool dox, bool doy, bool doz, bool Targ)
{
	TBone *B;
   B = Root;
   while (B)
   {
   	SelectChildrenNear(B,x,y,z,Thresh,dox,doy,doz, Targ);
   	B = B->NextSib;
   }
}

TBone *TBoneList::GetChildrenNear(TBone *Bone, float x, float y, float z,float Thresh,
											bool dox, bool doy, bool doz)
{
   TBone *B, *Res;
   B = Bone->Child;
   while (B)
   {
   	Res = GetChildrenNear(B, x,y,z, Thresh, dox,doy,doz);
      if (Res)
      	return Res;
   	B = B->NextSib;
   }

   if (dox)
   {
   	if ( fabs(Bone->Pivot.x - x) > Thresh)
         return NULL;
   }

   if (doy)
   {
   	if ( fabs(Bone->Pivot.y - y) > Thresh)
         return NULL;
   }

   if (doz)
   {
   	if ( fabs(Bone->Pivot.z - z) > Thresh)
         return NULL;
   }

   return Bone;
}

TBone *TBoneList::GetBoneNear(float x, float y, float z,float Thresh,
											bool dox, bool doy, bool doz)
{
	TBone *B,*Res;
   B = Root;
   while (B)
   {
   	Res = GetChildrenNear(B,x,y,z,Thresh,dox,doy,doz);
      if (Res)
      	return Res;
   	B = B->NextSib;
   }
   return NULL;
}

void TBoneList::SelectBones(bool Targ)
{
	TBone *B = Root;
   while (B)
   {
		SelectChildren(B, Targ);
   	B = B->NextSib;
   }
}

void TBoneList::SelectChildren(TBone *Bone, bool Targ)
{
	TBone *B;

   B = Bone->Child;
   while (B)
   {
   	SelectChildren(B, Targ);
   	B = B->NextSib;
   }

	Bone->Selected = Targ;
}

void TBoneList::SelectBonesIn(float x1, float y1, float z1,
   						 float x2, float y2, float z2,
   							bool dox, bool doy, bool doz, bool Targ)
{
	TBone *B;
   B = Root;
   while (B)
   {
   	SelectChildrenIn(B,x1,y1,z1,x2,y2,z2,dox,doy,doz, Targ);
   	B = B->NextSib;
   }
}
void TBoneList::SelectChildrenIn(TBone *Bone,
   								float x1, float y1, float z1,
   								float x2, float y2, float z2,
                           bool dox, bool doy, bool doz, bool Targ)
{
   TBone *B;
   B = Bone->Child;
   while (B)
   {
   	SelectChildrenIn(B, x1,y1,z1, x2,y2,z2,dox,doy,doz, Targ);
   	B = B->NextSib;
   }

   if (dox)
   {
   	if ( !Between(Bone->Pivot.x,x1,x2) )
         return;
   }

   if (doy)
   {
   	if ( !Between(Bone->Pivot.y,y1,y2) )
         return;
   }

   if (doz)
   {
   	if ( !Between(Bone->Pivot.z,z1,z2) )
         return;
   }

   Bone->Selected = Targ;
}

bool TBone::IsChildOf(TBone *Bone)
{
   if (Bone == NULL)
      return false;

   bool  Res=false;
   TBone *B;

   B = Bone;

   while (B)
   {
      if (IsChildOf(B->Child))
         return true;

      B = B->NextSib;
   }

   return false;
}

// Do an iteration of the transforming child thing.
void TBone::StepTransformChildrenWith(Matrix3 &R, Vector3 &Centre,
   									TRealVertex **V,
                              int numframes, int numverts, int Frame)
{
	TBone	*B;

   B = Child;

   while (B)
   {
   	B->StepTransformChildrenWith(R,Centre, V, numframes,
      										numverts, Frame);

   	B = B->NextSib;
   }

   Pivot -= Centre;
   Vector3 Vec;
   R.Transform(Pivot,Vec);
   Pivot = Vec;
   Pivot += Centre;
}

// Transofrm the children of the bone with the supplied matrix
void TBone::TransformChildrenWith(Matrix3 &R, Vector3 &Centre,
   									TRealVertex **V,
                              int numframes, int numverts, int Frame)
{
   TBone *B;

   B = Child;
   while (B)
   {
      B->StepTransformChildrenWith(R,Centre, V, numframes,
      									  numverts, Frame);
   	B = B->NextSib;
   }
}


void TBone::ResetChildren()
{
   TBone *B;
   B = Child;

   while (B)
   {
   	B->ResetChildren();
   	B = B->NextSib;
   }
   Pivot = OriginalPivot;
}

void TBoneList::ResetAllBones()
{
	TBone *B;

   B = Root;
   while (B)
   {
      B->Pivot = B->OriginalPivot;
      B->ResetChildren();

   	B = B->NextSib;
   }
}

void TBoneList::TransformAll(TRealVertex **V,int numframes,
										int numverts, int Frame)
{
	ResetAllBones();

	TBone *B;
   B = Root;
   while (B)
   {
      Vector3   O(B->Pivot.x,B->Pivot.y,B->Pivot.z);
      Matrix3 M;

      M.Identity();

      M.MakeRotationMatrix(B->RotX[Frame],B->RotY[Frame],B->RotZ[Frame]);
      M.m1.x *= B->ScaleX[Frame];
      M.m2.y *= B->ScaleY[Frame];
      M.m3.z *= B->ScaleZ[Frame];

      B->TransformChildrenWith(M,O,V,numframes,numverts,Frame);
		TransformAllStep(B->Child,V,numframes,numverts,Frame);
   	B = B->NextSib;
   }
}

void TBoneList::TransformAllStep(TBone *Bone,TRealVertex **V,int numframes,
										int numverts, int Frame)
{
	TBone *B;
   B = Bone;
   while (B)
   {
      Vector3   O(B->Pivot.x,B->Pivot.y,B->Pivot.z);
      static Matrix3   M;

      M.Identity();

      M.MakeRotationMatrix(B->RotX[Frame],B->RotY[Frame],B->RotZ[Frame]);
      M.m1.x *= B->ScaleX[Frame];
      M.m2.y *= B->ScaleY[Frame];
      M.m3.z *= B->ScaleZ[Frame];

      B->TransformChildrenWith(M,O,V,numframes,numverts,Frame);
		TransformAllStep(B->Child,V,numframes,numverts,Frame);

   	B = B->NextSib;
   }
}

bool AnythingSelectedStep(TBone *Bone)
{
	TBone *B;

   B = Bone->Child;
	while (B)
   {
   	if (AnythingSelectedStep(B))
      	return true;
   	B = B->NextSib;
   }
   if (Bone->Selected)
   	return true;
   else
   	return false;
}


bool TBoneList::AnythingSelected()
{
	TBone *B;

   B = Root;
   while (B)
   {
   	if (AnythingSelectedStep(B))
      	return true;
      B = B->NextSib;
   }

   return false;
}

TBoneList& TBoneList::operator =(const TBoneList &Src)
{
	// clear the current bone structure
   {
      TBone *B,*Next;

      B = Root;
      while(B)
      {
         Next = B->NextSib;
         DeleteTree(B);
         B = Next;
      }
      Root = NULL;
	}

//   TBone *B;

   return *this;
}

void TBoneList::ApplyFuncToSelectedStep( TBone* Bone,void Func(TBone*))
{
	TBone *B,*Next;

   if (!Bone)
   	return;

	B = Bone->Child;

   while (B)
   {
   	Next = B->NextSib;
      ApplyFuncToSelectedStep(B, Func);

      B = Next;
   }
   if (Bone->Selected)
   	Func(Bone);
}

void TBoneList::ApplyFuncToSelected( void Func(TBone*))
{
	TBone *B,*Next;

   B = Root;
   while (B)
   {
		Next = B->NextSib;
		ApplyFuncToSelectedStep(B, Func);

      B = Next;
   }
}

void TBoneList::CheckRoot() // updates root node
{
   TBone *NewRoot;

   NewRoot = Root;

	while (NewRoot->Parent)
		NewRoot = NewRoot->Parent;
   while (NewRoot->PrevSib)
   	NewRoot = NewRoot->PrevSib;

   Root = NewRoot;
}
