//---------------------------------------------------------------------------
#include "stdafx.h"

#include "Matrix.h"
#include "Math.h"

Matrix3::Matrix3 ()
{

}

Matrix3::Matrix3 (float m11, float m12, float m13,
  	    	  float m21, float m22, float m23,
  	   	  float m31, float m32, float m33)
{
  Matrix3::m11 = m11;
  Matrix3::m12 = m12;
  Matrix3::m13 = m13;
  Matrix3::m21 = m21;
  Matrix3::m22 = m22;
  Matrix3::m23 = m23;
  Matrix3::m31 = m31;
  Matrix3::m32 = m32;
  Matrix3::m33 = m33;
}

void Matrix3::Set(float m11, float m12, float m13,
  	    	  float m21, float m22, float m23,
  	   	  float m31, float m32, float m33)
{
  Matrix3::m11 = m11;
  Matrix3::m12 = m12;
  Matrix3::m13 = m13;
  Matrix3::m21 = m21;
  Matrix3::m22 = m22;
  Matrix3::m23 = m23;
  Matrix3::m31 = m31;
  Matrix3::m32 = m32;
  Matrix3::m33 = m33;
}

Matrix3::~Matrix3 ()
{
}

Matrix3& Matrix3::operator+= (Matrix3& m)
{
  m11 += m.m11; m12 += m.m12; m13 += m.m13;
  m21 += m.m21; m22 += m.m22; m23 += m.m23;
  m31 += m.m31; m32 += m.m32; m33 += m.m33;
  return *this;
}

Matrix3& Matrix3::operator-= (Matrix3& m)
{
  m11 -= m.m11; m12 -= m.m12; m13 -= m.m13;
  m21 -= m.m21; m22 -= m.m22; m23 -= m.m23;
  m31 -= m.m31; m32 -= m.m32; m33 -= m.m33;
  return *this;
}

Matrix3& Matrix3::operator*= (Matrix3& m)
{
  Matrix3 r;
  r.m11 = m11*m.m11 + m12*m.m21 + m13*m.m31;
  r.m12 = m11*m.m12 + m12*m.m22 + m13*m.m32;
  r.m13 = m11*m.m13 + m12*m.m23 + m13*m.m33;
  r.m21 = m21*m.m11 + m22*m.m21 + m23*m.m31;
  r.m22 = m21*m.m12 + m22*m.m22 + m23*m.m32;
  r.m23 = m21*m.m13 + m22*m.m23 + m23*m.m33;
  r.m31 = m31*m.m11 + m32*m.m21 + m33*m.m31;
  r.m32 = m31*m.m12 + m32*m.m22 + m33*m.m32;
  r.m33 = m31*m.m13 + m32*m.m23 + m33*m.m33;
  *this = r;
  return *this;
}

Matrix3& Matrix3::operator*= (float s)
{
  m11 *= s; m12 *= s; m13 *= s;
  m21 *= s; m22 *= s; m23 *= s;
  m31 *= s; m32 *= s; m33 *= s;
  return *this;
}

void Matrix3::identity ()
{
  m12 = m13 = 0;
  m21 = m23 = 0;
  m31 = m32 = 0;
  m11 = m22 = m33 = 1;
}

void Matrix3::transpose ()
{
  float swap;
  swap = m12; m12 = m21; m21 = swap;
  swap = m13; m13 = m31; m31 = swap;
  swap = m23; m23 = m32; m32 = swap;
}

void Matrix3::inverse ()
{
  float s = (float)(1.0/determinant ());
  Matrix3 C;
  C.m11 =  (m22*m33 - m23*m32);
  C.m12 = -(m21*m33 - m23*m31);
  C.m13 =  (m21*m32 - m22*m31);
  C.m21 = -(m12*m33 - m13*m32);
  C.m22 =  (m11*m33 - m13*m31);
  C.m23 = -(m11*m32 - m12*m31);
  C.m31 =  (m12*m23 - m13*m22);
  C.m32 = -(m11*m23 - m13*m21);
  C.m33 =  (m11*m22 - m12*m21);
  C.transpose ();
  *this = C;
  (*this) *= s;
}

float Matrix3::determinant ()
{
  return
    m11 * (m22*m33 - m23*m32)
   -m12 * (m21*m33 - m23*m31)
   +m13 * (m21*m32 - m22*m31);
}

void Matrix3::transform (float x, float y, float z, Vector3& t)
{
  t.x = m11*x+m12*y+m13*z;
  t.y = m21*x+m22*y+m23*z;
  t.z = m31*x+m32*y+m33*z;
}

void Matrix3::transform (Vector3& f, Vector3& t)
{
  t.x = m11*f.x + m12*f.y + m13*f.z;
  t.y = m21*f.x + m22*f.y + m23*f.z;
  t.z = m31*f.x + m32*f.y + m33*f.z;
}

void Matrix3::init ()
{
}
void Normalize(TVector &A)
{
	if (Length(A)==0) return;
	float L = 1 / Length(A);

   A[0] *= L;
   A[1] *= L;
   A[2] *= L;
}


float Length(TVector &A)
{
	return (float)(sqrt(A[0]*A[0] + A[1]*A[1] + A[2]*A[2]));
}

float DotProduct(TVector &A, TVector &B)
{
	return A[0]*B[0] + A[1]*B[1] + A[2]*B[2];
}

void CrossProduct(TVector &A, TVector &B, TVector &Res)
{
	Res[0] = A[1]*B[2] - A[2]*B[1];
	Res[1] = A[2]*B[0] - A[0]*B[2];
	Res[2] = A[0]*B[1] - A[1]*B[0];
}

void SetVector(TVector &A, float x, float y, float z)
{
	A[0] = x;
   A[1] = y;
   A[2] = z;
}

void AddVector(TVector &A, TVector &B, TVector &Res)
{
	Res[0] = A[0]+B[0];
	Res[1] = A[1]+B[1];
	Res[2] = A[2]+B[2];
}
void SubVector(TVector &A, TVector &B, TVector &Res)
{
	Res[0] = A[0]-B[0];
	Res[1] = A[1]-B[1];
	Res[2] = A[2]-B[2];
}

void ScalarbyVector(TVector &A, float B, TVector &Res)
{
   Res[0] = A[0]*B;
   Res[1] = A[1]*B;
   Res[2] = A[2]*B;
}

// rotates vector x, using u as an axis, by angle 'ang'.
void RotateWithVector(TVector &x, TVector &u, float ang, TVector &Res)
{
	TVector h,v,uxx,t;

   CrossProduct(u,x,uxx);
   ScalarbyVector(uxx,(float)sin(ang),uxx);

   ScalarbyVector(u,DotProduct(x,u),h);

   SubVector(x,h,v);
   ScalarbyVector(v,(float)cos(ang),v);


   AddVector(h,v,t);
   AddVector(t,uxx,Res);
}

void Vector3::between (Vector3& v1, Vector3& v2, Vector3& v, float pct, float wid)
{
  if (pct != -1)
    pct /= 100.;
  else
  {
    float dist = (float)sqrt ((v1.x-v2.x)*(v1.x-v2.x)+(v1.y-v2.y)*(v1.y-v2.y)+(v1.z-v2.z)*(v1.z-v2.z));
    pct = wid/dist;
  }
  v.x = pct*(v2.x-v1.x)+v1.x;
  v.y = pct*(v2.y-v1.y)+v1.y;
  v.z = pct*(v2.z-v1.z)+v1.z;
}

Vector3& Vector3::operator+= (Vector3& v)
{
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}
Vector3& Vector3::operator+= (vec3_t& v)
{
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector3& Vector3::operator-= (Vector3& v)
{
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector3& Vector3::operator= (Vector3 v)
{
  x = v.x;
  y = v.y;
  z = v.z;
  return *this;
}

float DotVec (Vector3& v1, Vector3& v2)
{
	return (v1.x*v2.x) + (v1.y*v2.y) + (v1.z*v2.z);
}

Vector3 CrossVec (Vector3& A, Vector3& B)
{
	Vector3 V;

   V.x = A.y*B.z - A.z*B.y;
   V.y = A.z*B.x - A.x*B.z;
   V.z = A.x*B.y - A.y*B.x;

	return V;
}

void MakeRotationMatrix(float X, float Y, float Z, Matrix3 &M)
{
	static Matrix3 A,B,C;

   A.identity();
   B.identity();
   C.identity();

	A.m11 = (float)(cos(X));
	A.m12 = (float)(sin(X));
	A.m21 = (float)(-sin(X));
	A.m22 = (float)(cos(X));

	B.m22 = (float)(cos(-Y));
	B.m23 = (float)(sin(-Y));
	B.m32 = (float)(-sin(-Y));
	B.m33 = (float)(cos(-Y));

	C.m11 = (float)(cos(Z));
	C.m13 = (float)(sin(Z));
	C.m31 = (float)(-sin(Z));
	C.m33 = (float)(cos(Z));

   B *= A;

   M = B;
}

void Matrix3::SplitHoriz(Vector3 &A, Vector3 &B, Vector3 &C)
{
	A.x = m11;
   A.y = m12;
   A.z = m13;
	B.x = m21;
   B.y = m22;
   B.z = m23;
	C.x = m31;
   C.y = m32;
   C.z = m33;
}

void Matrix3::SplitVert(Vector3 &A, Vector3 &B, Vector3 &C)

{

	A.x = m11;

   A.y = m21;
   A.z = m31;
	B.x = m12;
   B.y = m22;
   B.z = m32;
	C.x = m13;
   C.y = m23;
   C.z = m33;
}

void Matrix3::MakeHoriz(Vector3 &A, Vector3 &B, Vector3 &C)
{
   m11 = A.x;
   m12 = A.y;
   m13 = A.z;
   m21 = B.x;
   m22 = B.y;
   m23 = B.z;
   m31 = C.x;
   m32 = C.y;
   m33 = C.z;
}

void Matrix3::MakeVert(Vector3 &A, Vector3 &B, Vector3 &C)

{

	m11 = A.x;

   m21 = A.y;
   m31 = A.z;
	m12 = B.x;
   m22 = B.y;
   m32 = B.z;
	m13 = C.x;
   m23 = C.y;
   m33 = C.z;
}

void Vector3::Normalize()
{
	float l = Length();
   x /= l;
   y /= l;
   z /= l;
}

float Vector3::Length()
{
   return (float)sqrt( x*x + y*y + z*z);
}
//---------------------------------------------------------------------------
