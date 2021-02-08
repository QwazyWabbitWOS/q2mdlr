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

  // These functions create and set internal data.

  Vector3 () { x = y = z = 0; }
  Vector3 (float x, float y, float z) { Set(x, y, z);};
  Vector3 (Vector3 &Src) { x = Src.x; y = Src.y ; z = Src.z;};

  void Set (float x, float y, float z) { Vector3::x = x; Vector3::y = y; Vector3::z = z; }
  void Set (Vector3 &v) { Set(v.x, v.y, v.z); }
  // These are functions that perform math operations using Vector3

  float DotProduct(Vector3 &B) const;
  Vector3 CrossProduct(Vector3 &B) const;
  Vector3 AddVector(Vector3 &B) const;
  Vector3 SubVector(Vector3 &B) const;
  Vector3 ScalarbyVector(float B) const;
  void Normalize();
  float Length();
  static void between (Vector3 &v1, Vector3 &v2, Vector3 &v, float pct, float wid);

  // These operational overload functions hide the math functions from the rest of the code,
  // making the code more readable. Note that the Dot Product is represented by the
  // multiplication operator, and the Cross Product is represented by the division operator.
  
/*
  Vector3& operator=  (Vector3 v) { Set(v); return *this;};
  Vector3& operator+= (Vector3 &v) { Set(AddVector(v)); return *this;};
  Vector3& operator-= (Vector3 &v) { Set(SubVector(v)); return *this;};
  Vector3& operator*= (float a) { Set(ScalarbyVector(a)); return *this;};
  Vector3& operator/= (Vector3 &v) { Set(CrossProduct(v)); return *this;};

  Vector3 operator+  (Vector3 &v) { return AddVector(v); };
  Vector3 operator-  (Vector3 &v) { return SubVector(v); };
  float operator*     (Vector3 &v) { return DotProduct(v); };
  Vector3 operator* (float a) { return ScalarbyVector(a); };
  Vector3 operator/ (float a) { return ScalarbyVector(1/a); };
  Vector3 operator/  (Vector3 &v) { return CrossProduct(v); };
*/

  //Vector3& operator=  (Vector3 v) { x = v.x; y = v.y; z = v.z; return *this;};
  Vector3& operator+= (Vector3 &v) { x += v.x; y += v.y; z += v.z; return *this;};
  Vector3& operator-= (Vector3 &v) { x -= v.x; y -= v.y; z -= v.z; return *this;};
  Vector3& operator*= (float a) { x *= a; y *= a; z *= a; return *this;};
  Vector3& operator/= (Vector3 &v) { Set(CrossProduct(v)); return *this;};

  Vector3 operator+  (Vector3 &v) { return Vector3(x+v.x, y+v.y, z+v.z); };
  Vector3 operator-  (Vector3 &v) { return Vector3(x-v.x, y-v.y, z-v.z); };
  float operator*     (Vector3 &v) { return x*v.x + y*v.y + z*v.z; };
  Vector3 operator* (float a) { return Vector3(x*a, y*a, z*a);};
  Vector3 operator/ (float a) { return Vector3(x/a, y/a, z/a);};
  Vector3 operator/  (Vector3 &v) { return CrossProduct(v); };

  // In case some one want to use Vector3 as if it were the old TVector type, we use this
  // operational overload to simulate an array of float variables.
  float& operator[] (int i);

};

// Non-member Vector3 operators
Vector3 operator* (float f, const Vector3 &V);

class Matrix3
{
public:
  Vector3 m1;
  Vector3 m2;
  Vector3 m3;

public:
  Matrix3 () { };
  void Set(Vector3 &x, Vector3 &y, Vector3 &z);
  void Set(float m11, float m12, float m13,
  	   float m21, float m22, float m23,
  	   float m31, float m32, float m33);
  Matrix3 (float m11, float m12, float m13,
  	   float m21, float m22, float m23,
  	   float m31, float m32, float m33) { Set(m11,m12,m13,m21,m22,m23,m31,m32,m33); };
  Matrix3 (Vector3 &x, Vector3 &y, Vector3 &z) { Set(x,y,z); };
  ~Matrix3 () { };

  static void init () { }

  Matrix3& operator+= (Matrix3 &m);
  Matrix3& operator-= (Matrix3 &m);
  Matrix3& operator*= (Matrix3 &m);
  Matrix3& operator*= (float s);

  void Transpose ();
  void Inverse ();
  float Determinant ();
  void Transform (Vector3 &f);
  void Transform (Vector3 &Src, Vector3& Dest);
  void Transform (float x, float y, float z, Vector3 &t);
  void Identity ();

  void SplitHoriz(Vector3 &A, Vector3 &B, Vector3 &C);
  void SplitVert(Vector3 &A, Vector3 &B, Vector3 &C);

  void MakeHoriz(Vector3 &A, Vector3 &B, Vector3 &C);

  void MakeVert(Vector3 &A, Vector3 &B, Vector3 &C);

  Matrix3& MakeRotationMatrix(float X, float Y, float Z);
  Matrix3& MakeRotationMatrix(Vector3 &A);

  // In case some one want to use Vector3 as if it were the old TVector type, we use this
  // operational overload to simulate an array of float variables.

  Vector3& Matrix3::operator[] (int i);
};

typedef struct
{
	int x,y;
} TPoint2D;

#endif
