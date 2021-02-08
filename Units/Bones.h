//---------------------------------------------------------------------------
#ifndef BonesH
#define BonesH
//---------------------------------------------------------------------------

class TBone;
class TBoneList;

#include "Matrix.h"
#include "MdlConst.h"


class TBone
{
public:
	TBone			*Parent, *Child, *PrevSib, *NextSib;
   bool 			Selected, Active;

	char			Name[16];
   int 			NumVerts;
   Vector3		Pivot, OriginalPivot;

   float 		RotX[MAX_FRAMES], RotY[MAX_FRAMES], RotZ[MAX_FRAMES],
   				ScaleX[MAX_FRAMES],ScaleY[MAX_FRAMES],ScaleZ[MAX_FRAMES],
   				TransX, TransY, TransZ;

   TBone();         
   TBone(float x, float y, float z);
   TBone(char *N);
   TBone(float x, float y, float z, char *N);

	TBone *AddChild(float x, float y, float z); // adds a bone as a child
	TBone *AddChild(float x, float y, float z, char *N); // adds a bone as a child
   void SetParent(TBone *P); // sets the parent bone to P

	void StepTransformChildrenWith(Matrix3 &R, Vector3 &Centre,
   									TRealVertex **V,
                              int numframes, int numverts, int Frame);
	void TransformChildrenWith(Matrix3 &R, Vector3 &Centre,
   									TRealVertex **V,
                              int numframes, int numverts, int Frame);

   bool IsChildOf(TBone *Bone);

	void ResetChildren();
};

class TBoneList
{
public:
	TBone			*Root;

	TBoneList();
   ~TBoneList();

   TBone *AddBone(float x, float y, float z);// adds a bone at the root level
   TBone *AddBone(float x, float y, float z, char *N);// adds a bone at the root level
   void DeleteBone(TBone *B); // deletes bone, and roots its children
   void DeleteTree(TBone *B);	// deletes Bone and all children

   void SelectBonesNear(float x, float y, float z, float Thresh,
   							bool dox, bool doy, bool doz, bool Targ);
   TBone *GetBoneNear(float x, float y, float z, float Thresh,
   							bool dox, bool doy, bool doz);
   void SelectChildrenNear(TBone *Bone, float Thresh,
   								float x, float y, float z,
                           bool dox, bool doy, bool doz, bool Targ);
   TBone *GetChildrenNear(TBone *Bone, float Thresh,
   								float x, float y, float z,
                           bool dox, bool doy, bool doz);

   void SelectBonesIn(float x1, float y1, float z1,
   						 float x2, float y2, float z2,
   							bool dox, bool doy, bool doz, bool Targ);
   void SelectChildrenIn(TBone *Bone,
   								float x1, float y1, float z1,
   								float x2, float y2, float z2,
                           bool dox, bool doy, bool doz, bool Targ);

   void SelectBones(bool Targ);
   void SelectChildren(TBone *Bone, bool Targ);

   void TransformAll(TRealVertex **V,int numframes, int numverts, int Frame);
   void TransformAllStep(TBone *Bone, TRealVertex **V,int numframes, int numverts, int Frame);

   void ResetAllBones();

   void ApplyFuncToSelected( void Func(TBone*));
   void ApplyFuncToSelectedStep( TBone *Bone,void Func(TBone*));

   bool AnythingSelected();

   void CheckRoot();

   TBoneList& operator =(const TBoneList &Src);
};


#endif
