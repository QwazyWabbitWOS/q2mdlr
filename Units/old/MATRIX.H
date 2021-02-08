//---------------------------------------------------------------------------
#ifndef MatrixH
#define MatrixH
//---------------------------------------------------------------------------
/*
	This is a matrix calc unit ype thing. it does NOT do 4x4 matricies
   Therefore translations must be done externally
*/

class Vector3
{
public:
  float x, y, z;

  Vector3 () { }
  Vector3 (float x, float y, float z) { Vector3::x = x; Vector3::y = y; Vector3::z = z; }

  Vector3& operator+= (Vector3& v);
  Vector3& operator+= (vec3_t& v);
  Vector3& operator-= (Vector3& v);
  Vector3& operator=  (Vector3 v);

  void Set (float x, float y, float z) { Vector3::x = x; Vector3::y = y; Vector3::z = z; }
  void Normalize();
  float Length();


  static void between (Vector3& v1, Vector3& v2, Vector3& v, float pct, float wid);
};

class Matrix3
{
public:
  float m11, m12, m13;
  float m21, m22, m23;
  float m31, m32, m33;

public:
  Matrix3 ();
  Matrix3 (float m11, float m12, float m13,
  	   float m21, float m22, float m23,
  	   float m31, float m32, float m33);
  ~Matrix3 ();

  static void init ();

  Matrix3& operator+= (Matrix3& m);
  Matrix3& operator-= (Matrix3& m);
  Matrix3& operator*= (Matrix3& m);
  Matrix3& operator*= (float s);

  void Set(float m11, float m12, float m13,
  	   float m21, float m22, float m23,
  	   float m31, float m32, float m33);

  void transpose ();
  void inverse ();
  float determinant ();
  void transform (Vector3 &f, Vector3 &t);
  void transform (float x, float y, float z, Vector3& t);
  void identity ();

  void SplitHoriz(Vector3 &A, Vector3 &B, Vector3 &C);
  void SplitVert(Vector3 &A, Vector3 &B, Vector3 &C);

  void MakeHoriz(Vector3 &A, Vector3 &B, Vector3 &C);

  void MakeVert(Vector3 &A, Vector3 &B, Vector3 &C);

};


typedef float TMatrix[3][3];
typedef float TVector[3];

typedef struct
{
	int x,y;
} TPoint2D;


void MatrixbyMatrix(TMatrix &A, TMatrix &B, TMatrix &Res);
void MatrixbyVector(TMatrix &A, TVector &B, TVector &Res);
void Normalize(TVector &A);
float Length(TVector &A);
float DotProduct(TVector &A, TVector &B);
void CrossProduct(TVector &A, TVector &B, TVector &Res);
void SetVector(TVector &A, float x, float y, float z);
void AddVector(TVector &A, TVector &B, TVector &Res);
void SubVector(TVector &A, TVector &B, TVector &Res);
void ScalarbyVector(TVector &A, float B, TVector &Res);

void MakeRotationMatrix(float X, float Y, float Z, Matrix3 &M);

float DotVec(Vector3& v1, Vector3& v2);
Vector3 CrossVec (Vector3& v1, Vector3& v2);

#endif
