//---------------------------------------------------------------------------
#include "stdafx.h"

#include "MdlConst.h"

#include "Matrix.h"

bool line_t::calcm()
{
	if (x1 == x2)
   	return false;

   m = (y1-y2)/(x1-x2);
   return true;
}

bool line_t::calcc()
{
	if (!calcm())
   	return false;

   c =  y1 - m*x1;

   return true;
}

line_t::line_t()
{
}

line_t::line_t(float nx1, float ny1, float nx2, float ny2)
{
	x1 = nx1;
   x2 = nx2;
   y1 = ny1;
   y2 = ny2;
}

bool line_t::Vertical()
{
	if (x1 == x2)
   	return true;

   return false;
}

bool line_t::Horionztal()
{
	if (y1 == y2)
   	return true;

   return false;
}

void ZeroVec(Vector3 &V)
{
	V.x = 0.0;
   V.y = 0.0;
   V.z = 0.0;
}

bool InRect(float x1, float y1, float x2, float y2, float a, float b)
{
	if (x1>x2)
   {
   	float temp = x1;
      x1 = x2;
      x2 = temp;
   }
	if (y1>y2)
   {
   	float temp = y1;
      y1 = y2;
      y2 = temp;
   }

   if (a>=x1 && a<=x2 && b>=y1 && b<=y2)
   	return true;
   else return false;

}

TRealVertex::TRealVertex()
{
	x = y = z = 0;
   Selected = false;
   Visible = true;

   Normal.Set(0,0,0);
/*   Strength = 1;
   Ox = Oy = Oz = 0;
   Parent = NULL;*/
};

bool Between(float a, float min, float max)
{
	if (min>max)
   {
   	float temp = max;
      max = min;
		min = temp;
   }

   if (a>=min && a <=max)
   	return true;
   else
   	return false;
}

bool InTri(	float x1,float y1, float x2,float y2, float x3,float y3, float xp,float yp)
{
	{
   	float minx,maxx,miny,maxy;

      miny = maxy = y1;
      minx = maxx = x1;

      if (x2<minx)
      	minx = x2;
      if (y2<miny)
      	miny = y2;

      if (x3<minx)
      	minx = x3;
      if (y3<miny)
      	miny = y3;

      if (x2>maxx)
      	maxx = x2;
      if (y2>maxy)
      	maxy = y2;

      if (x3>maxx)
      	maxx = x3;
      if (y3>maxy)
      	maxy = y3;

   	if (xp<minx || yp<miny || xp>maxx || yp>maxy)
      	return false;
   }

	float tx1,tx2,tx3,ty1,ty2,ty3;

   tx1 = (x2+x1)/2;
   tx2 = (x3+x2)/2;
   tx3 = (x1+x3)/2;
   ty1 = (y2+y1)/2;
   ty2 = (y3+y2)/2;
   ty3 = (y1+y3)/2;

	line_t	a(x1,y1,x2,y2),
				b(x2,y2,x3,y3),
				c(x3,y3,x1,y1),
            p1(xp,yp,tx1,ty1),
            p2(xp,yp,tx2,ty2),
            p3(xp,yp,tx3,ty3);

	bool res1, res2, res3;

   res1 = LinesIntersect(a,p2) || LinesIntersect(a,p3);
   res2 = LinesIntersect(b,p1) || LinesIntersect(b,p3);
   res3 = LinesIntersect(c,p1) || LinesIntersect(c,p2);
	if ( res1 || res2 || res3 )
   	return false;


   return true;
}

bool LinesIntersect(line_t a, line_t b)
{
   float ix, iy;

   if (a.Vertical() && b.Vertical())
   	return false;

   if (a.Vertical()) // first line is vertical
   {
		b.calcm();
      b.calcc();

      ix = a.x1;

   	if ( !Between(ix,b.x1,b.x2) )
      	return false;

      iy = b.m * ix + b.c;

      if ( Between(iy,b.y1,b.y2) && Between(iy, a.y1, a.y2) )
      	return true;

   	return false;
   }
   if (b.Vertical()) // first line is vertical
   {
		a.calcm();
      a.calcc();

      ix = b.x1;

   	if ( !Between(ix,a.x1,a.x2) )
      	return false;

      iy = a.m * ix + a.c;

      if ( Between(iy,a.y1,a.y2) && Between(iy, b.y1, b.y2) )
      	return true;

   	return false;
   }

/*    Refernce:
	y = m1*x + c1;
   y = m2*x + c2;

	m1*x = x2*x + c2 - c1;
   x*(m1-m2) = c2 - c1;
   x = (c2-c1)/(m1-m2)

   (y-c2)/m2 = (y-c1)/m1;
   m1*(y-c2) = m2*(y-c1)
   m1y - m1c2 = m2y - m2c1;
   m1y-m2y = m1c2-m2c1
   y = (m1c2-m2c1)/(m1-m2)
*/

	a.calcm();
   a.calcc();
   b.calcm();
   b.calcc();

   if (a.m == b.m) return false;

   ix = (b.c - a.c)/(a.m - b.m);
   iy = a.m * ix + a.c;

   if ( Between(ix,a.x1,a.x2) && Between(ix,b.x1,b.x2) &&
        Between(iy,a.y1,a.y2) && Between(iy,b.y1,b.y2) )
   return true;

   return false;
}


TLoadVertFace::TLoadVertFace()
{
   num_xyz = num_tris = 0;
   V = NULL;
   T = NULL;
}

TLoadVertFace::~TLoadVertFace()
{
   try {
   if (V)
      delete V;
   if (T)
      delete T;
   }
   catch (...)
   {
   }
}

CameraInfo::CameraInfo()
{
   m_PanX = m_PanY = m_PanZ = 0.0;
   m_RotX = 5.1;
   m_RotY = 1.8;
   m_Dist = 80.0;
   m_Persp = 200.0;
}

CameraInfo::CameraInfo(double PX, double PY, double PZ, double RX, double RY, double D, double P)
{
   m_PanX = PX;
   m_PanY = PY;
   m_PanZ = PZ;
   m_RotX = RX;
   m_RotY = RY;
   m_Dist = D;
   m_Persp = P;
}

void CameraInfo::Set(double PX, double PY, double PZ, double RX, double RY, double D, double P)
{
   m_PanX = PX;
   m_PanY = PY;
   m_PanZ = PZ;
   m_RotX = RX;
   m_RotY = RY;
   m_Dist = D;
   m_Persp = P;
}


CDrawViewInfo::CDrawViewInfo()
{
   Shading = shNone;
   Texture = ttWire;
   Grid = false;
   Origin = false;
   m_3DFaceCull = false;
   m_2DFaceCull = false;
}

CDrawViewInfo::CDrawViewInfo(ShadingType S, TextureType T, double PX, double PY, double PZ, double RX, double RY, double D, double P)
{
   Shading = S;
   Texture = T;
   Cam.Set(PX,PY,PZ,RX,RY,D,P);
}

Transform3DStruct::Transform3DStruct()
{
   Cam = NULL;
}

Transform3DStruct::~Transform3DStruct()
{
}

bool Transform3DStruct::Transform3D()
{
   float tempy;


   nx = (float)((x+Cam->m_PanX)*cosa1 + (y+Cam->m_PanY)*sina1);
   tempy = (float)((y+Cam->m_PanY)*cosa1 - (x+Cam->m_PanX)*sina1);

   nz = (float)((z+Cam->m_PanZ)*cosa2 + (tempy)*sina2);
   ny = (float)(tempy*cosa2 - (z+Cam->m_PanZ)*sina2);

   nz+=(float)Cam->m_Dist;

/*
   {
      Matrix3  M;
      Vector3  V,T;

      MakeRotationMatrix(Cam->RotX, Cam->RotY, 0, M);

      V.Set(x+Cam->m_PanX,y+Cam->m_PanY,z+Cam->m_PanZ);

      M.transform(V,T);
      nx = T.x;
      ny = T.y;
      nz = T.z;

      nz += Cam->Dist;
   }
*/
   if (nz > 1)
   {
      nx = (float)(Cam->m_Persp*nx/nz);
      ny = (float)(Cam->m_Persp*ny/nz);
   	return true;
   }

	return false;
}

// This splits the path up into the given sections
// the extenstion is all the stuff after the first dot encountered for the file name
// the dir is the stuff afte rthe drive, and before the file name

unsigned long ReverseL(unsigned long V)
{
   BYTE a,b,c,d;

   a = (BYTE)(V & 0xff);
   b = (BYTE)((V & 0xff00) >> 8);
   c = (BYTE)((V & 0xff0000) >> 16);
   d = (BYTE)((V & 0xff000000) >> 24);

   return (a << 24) + (b << 16) + (c << 8) + (d);
}

float ReverseF(float F)
{
   BYTE a,b,c,d;

   unsigned long  V;
   float f;

   V = *((unsigned long *)(&F));

   a = (BYTE)(V & 0xff);
   b = (BYTE)((V & 0xff00) >> 8);
   c = (BYTE)((V & 0xff0000) >> 16);
   d = (BYTE)((V & 0xff000000) >> 24);

   V = (a << 24) + (b << 16) + (c << 8) + (d);
   f = *((float*)(&V));

   return f;
}

unsigned short ReverseW(unsigned short V)
{
   BYTE a,b;

   a = (V & 0xff);
   b = (V & 0xff00) >> 8;

   return (a << 8) + (b);
}

CShiftState::CShiftState()
{
   m_Shift = false;
   m_Ctrl = false;
   m_Alt = false;
   m_Left = false;
   m_Right = false;
   m_Mid = false;
}

void CShiftState::SetFromFlags(unsigned int nFlags)
{
   m_Ctrl = (nFlags & MK_CONTROL) > 0;
   m_Shift = (nFlags & MK_SHIFT) > 0;
   m_Left = (nFlags & MK_LBUTTON) > 0;
   m_Right = (nFlags & MK_RBUTTON) > 0;
   m_Mid = (nFlags & MK_MBUTTON) > 0;

   short r,l;
   r=  GetKeyState(VK_RMENU);
   l = GetKeyState(VK_LMENU);

   if (r <0 || l < 0)
      m_Alt = true;
   else 
      m_Alt = false;
}

unsigned int CShiftState::SetIntoFlags()
{
   unsigned int  nFlags;
   nFlags = 0;

   if (m_Ctrl)
      nFlags &= MK_CONTROL;
   if (m_Shift)
      nFlags &= MK_SHIFT;
   if (m_Left)
      nFlags &= MK_LBUTTON;
   if (m_Mid)
      nFlags &= MK_MBUTTON;
   if (m_Right)
      nFlags &= MK_RBUTTON;

   return nFlags;
}

CMouseState::CMouseState()
{
   m_Left = false;
   m_Mid = false;
   m_Right = false;
}

