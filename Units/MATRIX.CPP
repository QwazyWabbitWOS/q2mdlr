//---------------------------------------------------------------------------
#include "stdafx.h"
#include "Matrix.h"
#pragma hdrstop

#include "Math.h"

/*******************************************************************************************
   Vector3 Class Functions
*******************************************************************************************/

// Get the dot product of two vectors.
float Vector3::DotProduct(Vector3& B) const
{
	return x*B.x + y*B.y + z*B.z;
}

// Get the cross product of two vectors.
Vector3 Vector3::CrossProduct(Vector3& B) const
{
   Vector3 temp;

   temp.x = y*B.z - z*B.y;
   temp.y = z*B.x - x*B.z;
   temp.z = x*B.y - y*B.x;

   return temp;
}

// Add two vectors together.
Vector3 Vector3::AddVector(Vector3& B) const
{
   Vector3 temp;

   temp.x = x + B.x;
   temp.y = y + B.y;
   temp.z = z + B.z;

   return temp;
}

// Subtract one vector from another.
Vector3 Vector3::SubVector(Vector3& B) const
{
   Vector3 temp;

   temp.x = x - B.x;
   temp.y = y - B.y;
   temp.z = z - B.z;

   return temp;
}

// Multiplies each dimension of the vector by a scalar number.
Vector3 Vector3::ScalarbyVector(float a) const
{
   Vector3 temp;

   temp.x = x * a;
   temp.y = y * a;
   temp.z = z * a;

   return temp;
}

// Make the vector into a unit vector.
void Vector3::Normalize()
{
   float l = Length();

   x /= l;
   y /= l;
   z /= l;
}

// Find the magnitude of the vector.
float Vector3::Length()
{
   return (float)(sqrt( x*x + y*y + z*z));
}


// This allows array-like access for our elements x, y and z.
float& Vector3::operator[] (int i)
{
   i = i%3;
   switch(i) {
     case 0:
       return x;
       break;
     case 1:
       return y;
       break;
     case 2:
       return z;
       break;
     default:
        return x;
   }
}


// rotates vector x, using u as an axis, by angle 'ang'.
void RotateWithVector(Vector3 &x, Vector3 &u, float ang, Vector3 &Res)
{
   Vector3 h,v,uxx;

   uxx = (u / x) * (float)sin(ang);

   h = u * (x * u);

   v = (x - h) * (float)cos(ang);

   Res = (h + v) + uxx;
}

void Vector3::between (Vector3& v1, Vector3& v2, Vector3& v, float pct, float wid)
{
  if (pct != -1)
    pct /= 100.;
  else
  {
    float dist = (float)(sqrt ((v1.x-v2.x)*(v1.x-v2.x)+(v1.y-v2.y)*(v1.y-v2.y)+(v1.z-v2.z)*(v1.z-v2.z)));
    pct = wid/dist;
  }
  v.x = pct*(v2.x-v1.x)+v1.x;
  v.y = pct*(v2.y-v1.y)+v1.y;
  v.z = pct*(v2.z-v1.z)+v1.z;
}

Vector3 operator* (float f, const Vector3 &V)
{
   return V.ScalarbyVector(f);
}

/*******************************************************************************************
   Matrix3 Class Functions
*******************************************************************************************/

void Matrix3::Set(Vector3& x, Vector3& y, Vector3& z)
{
  m1 = x;
  m2 = y;
  m3 = z;
}

void Matrix3::Set(float m11, float m12, float m13,
  	    	  float m21, float m22, float m23,
  	   	  float m31, float m32, float m33)
{
  m1.x = m11;
  m1.y = m12;
  m1.z = m13;
  m2.x = m21;
  m2.y = m22;
  m2.z = m23;
  m3.x = m31;
  m3.y = m32;
  m3.z = m33;
}

Matrix3& Matrix3::operator+= (Matrix3& m)
{
  m1.x += m.m1.x; m1.y += m.m1.y; m1.z += m.m1.z;
  m2.x += m.m2.x; m2.y += m.m2.y; m2.z += m.m2.z;
  m3.x += m.m3.x; m3.y += m.m3.y; m3.z += m.m3.z;
  return *this;
}

Matrix3& Matrix3::operator-= (Matrix3& m)
{
  m1.x -= m.m1.x; m1.y -= m.m1.y; m1.z -= m.m1.z;
  m2.x -= m.m2.x; m2.y -= m.m2.y; m2.z -= m.m2.z;
  m3.x -= m.m3.x; m3.y -= m.m3.y; m3.z -= m.m3.z;
  return *this;
}

Matrix3& Matrix3::operator*= (Matrix3& m)
{
  Matrix3 r;
  r.m1.x = m1.x*m.m1.x + m1.y*m.m2.x + m1.z*m.m3.x;
  r.m1.y = m1.x*m.m1.y + m1.y*m.m2.y + m1.z*m.m3.y;
  r.m1.z = m1.x*m.m1.z + m1.y*m.m2.z + m1.z*m.m3.z;
  r.m2.x = m2.x*m.m1.x + m2.y*m.m2.x + m2.z*m.m3.x;
  r.m2.y = m2.x*m.m1.y + m2.y*m.m2.y + m2.z*m.m3.y;
  r.m2.z = m2.x*m.m1.z + m2.y*m.m2.z + m2.z*m.m3.z;
  r.m3.x = m3.x*m.m1.x + m3.y*m.m2.x + m3.z*m.m3.x;
  r.m3.y = m3.x*m.m1.y + m3.y*m.m2.y + m3.z*m.m3.y;
  r.m3.z = m3.x*m.m1.z + m3.y*m.m2.z + m3.z*m.m3.z;
//  *this = r;
  return *this;
}

Matrix3& Matrix3::operator*= (float s)
{
  m1 *= s;
  m2 *= s;
  m3 *= s;
  return *this;
}

void Matrix3::Identity()
{
  m1.y = m1.z = 0;
  m2.x = m2.z = 0;
  m3.x = m3.y = 0;
  m1.x = m2.y = m3.z = 1;
}

void Matrix3::Transpose()
{
  float swap;
  swap = m1.y; m1.y = m2.x; m2.x = swap;
  swap = m1.z; m1.z = m3.x; m3.x = swap;
  swap = m2.z; m2.z = m3.y; m3.y = swap;
}

void Matrix3::Inverse()
{
  float s = (float)(1.0/Determinant());
  Matrix3 C;
  C.m1.x =  (m2.y*m3.z - m2.z*m3.y);
  C.m1.y = -(m2.x*m3.z - m2.z*m3.x);
  C.m1.z =  (m2.x*m3.y - m2.y*m3.x);
  C.m2.x = -(m1.y*m3.z - m1.z*m3.y);
  C.m2.y =  (m1.x*m3.z - m1.z*m3.x);
  C.m2.z = -(m1.x*m3.y - m1.y*m3.x);
  C.m3.x =  (m1.y*m2.z - m1.z*m2.y);
  C.m3.y = -(m1.x*m2.z - m1.z*m2.x);
  C.m3.z =  (m1.x*m2.y - m1.y*m2.x);
  C.Transpose();
//  *this = C;
  (*this) *= s;
}

float Matrix3::Determinant()
{
  return
    m1.x * (m2.y*m3.z - m2.z*m3.y)
    -m1.y * (m2.x*m3.z - m2.z*m3.x)
    +m1.z * (m2.x*m3.y - m2.y*m3.x);
}

void Matrix3::Transform (float x, float y, float z, Vector3 &t)
{
  t.x = m1.x*x+m1.y*y+m1.z*z;
  t.y = m2.x*x+m2.y*y+m2.z*z;
  t.z = m3.x*x+m3.y*y+m3.z*z;
}

void Matrix3::Transform (Vector3& f)
{
  Transform(f.x, f.y, f.z, f);
}

void Matrix3::Transform (Vector3 &Src, Vector3& Dest)
{
  Transform(Src.x, Src.y, Src.z, Dest);
}

Matrix3& Matrix3::MakeRotationMatrix(float X, float Y, float Z)
{
	static Matrix3 A,B,C;

   A.Identity();
   B.Identity();
   C.Identity();

	A.m1.x = (float)(cos(X));
	A.m1.y = (float)(sin(X));
	A.m2.x = (float)(-sin(X));
	A.m2.y = (float)(cos(X));

	B.m2.y = (float)(cos(-Y));
	B.m2.z = (float)(sin(-Y));
	B.m3.y = (float)(-sin(-Y));
	B.m3.z = (float)(cos(-Y));

	C.m1.x = (float)(cos(Z));
	C.m1.z = (float)(sin(Z));
	C.m3.x = (float)(-sin(Z));
	C.m3.z = (float)(cos(Z));

   B *= A;

//   *this = B;
   return *this;
}

Matrix3& Matrix3::MakeRotationMatrix(Vector3& A)
{
   return MakeRotationMatrix(A.x, A.y, A.z);
}

void Matrix3::SplitHoriz(Vector3& A, Vector3& B, Vector3& C)
{
	A = m1;
	B = m2;
	C = m3;
}

void Matrix3::SplitVert(Vector3& A, Vector3& B, Vector3& C)
{
   A.x = m1.x;
   A.y = m2.x;
   A.z = m3.x;
   B.x = m1.y;
   B.y = m2.y;
   B.z = m3.y;
   C.x = m1.z;
   C.y = m2.z;
   C.z = m3.z;
}

void Matrix3::MakeHoriz(Vector3& A, Vector3& B, Vector3& C)
{
   Set(A,B,C);
}

void Matrix3::MakeVert(Vector3& A, Vector3& B, Vector3& C)
{
   Set(A.x, B.x, C.x, A.y, B.y, C.y, A.z, B.z, C.z);
}

// This allows array-like access for our elements m1, m2 and m3.
Vector3& Matrix3::operator[] (int i)
{
   i = i%3;
   switch(i)
   {
      case 0:
         return m1;
         break;
      case 1:
         return m2;
         break;
      case 2:
         return m3;
         break;
      default:
         return m1;
         break;
   }
}

//---------------------------------------------------------------------------
