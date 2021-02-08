//#define MainFuncs_CPP

//---------------------------------------------------------------------------
#include "stdafx.h"

#include "MdlConst.h"
#include "Bones.h"
#include "LWOHeadr.h"

#include "MDLType.h"
#include "Dibfuncs.h"
#include "Matrix.h"
#include "MainFunc.h"
//#include "SkinForm.h"

#include <Math.h>


#include "MDLGlobs.h"
#include "MdlConst.h"
#include "Matrix.h"

#include "Colors.h"
#include "light.h"

#include "dos.h"
#include "time.h"


#define TRANSX(X, Y, Z, M, T)  (M.m1.x*(X) + M.m1.y*(Y) + M.m1.z*(Z) + T.x)
#define TRANSY(X, Y, Z, M, T)  (M.m2.x*(X) + M.m2.y*(Y) + M.m2.z*(Z) + T.y)
#define TRANSZ(X, Y, Z, M, T)  (M.m3.x*(X) + M.m3.y*(Y) + M.m3.z*(Z) + T.z)

#define DECLARE_VWIDTHHEIGHT(B)        int vwidth = B.VisibleWidth, vwidth2 = vwidth >> 1, vheight = B.Height, vheight2 = vheight >> 1

void DrawGrid2D(TDIBitmap8 &Dest, float panx, float pany, ViewType V)
{
   DECLARE_VWIDTHHEIGHT(Dest);

   return;

	//if (MainForm->ShowGrid->Checked==false) return;
/*   float g_diffx=0, g_diffy=0;

   if (V == vtXY)
   {
//   	g_diffx = -g_CurMdl.Head.origin.y;
//      g_diffy = -g_CurMdl.Head.origin.x;
   }
   if (V == vtXZ)
   {
//   	g_diffx = g_CurMdl.Head.origin.x;
//      g_diffy = g_CurMdl.Head.origin.z;
   }
   if (V == vtZY)
   {
//   	g_diffx = -g_CurMdl.Head.origin.y;
//      g_diffy = g_CurMdl.Head.origin.z;
   }
   for (int i=0;i>=g_GridMin;i-=GridSize)
      Dest.Line(vwidth2+g_Zoom*(panx+i+g_diffx),vheight2+g_Zoom*(pany+128+g_diffy),
               vwidth2+g_Zoom*(panx+i+g_diffx),vheight2+g_Zoom*(pany-128+g_diffy),GRAY1);
   for (int i=0;i<=g_GridMax;i+=GridSize)
      Dest.Line(vwidth2+g_Zoom*(panx+i+g_diffx),vheight2+g_Zoom*(pany+128+g_diffy),
               vwidth2+g_Zoom*(panx+i+g_diffx),vheight2+g_Zoom*(pany-128+g_diffy),GRAY1);


   for (int j=0;j<=g_GridMax;j+=GridSize)
      Dest.Line(vwidth2+g_Zoom*(panx-128+g_diffx),vheight2+g_Zoom*((pany-j+g_diffy)),
               vwidth2+g_Zoom*(panx+128+g_diffx),vheight2+g_Zoom*((pany-j+g_diffy)),GRAY1);
   for (int j=0;j>=g_GridMin;j-=GridSize)
      Dest.Line(vwidth2+g_Zoom*(panx-128+g_diffx),vheight2+g_Zoom*((pany-j+g_diffy)),
               vwidth2+g_Zoom*(panx+128+g_diffx),vheight2+g_Zoom*((pany-j+g_diffy)),GRAY1);

   Dest.Line(vwidth2+g_Zoom*(panx+g_diffx),vheight2+g_Zoom*(pany+128+g_diffy),
            vwidth2+g_Zoom*(panx+g_diffx),vheight2+g_Zoom*(pany-128+g_diffy),GRAY2);

   Dest.Line(vwidth2+g_Zoom*(panx-128+g_diffx),vheight2+g_Zoom*((pany+g_diffy)),
            vwidth2+g_Zoom*(panx+128+g_diffx),vheight2+g_Zoom*((pany+g_diffy)),GRAY2);
            */
}

#define GridLine3D(S,T)				\
		if (S.Transform3D() && T.Transform3D())			\
      {                                               \
         Dest.Line_ZBuf((int)(vwidth2+S.nx), (int)(vheight2+S.ny), 1/S.nz,		\
                        (int)(vwidth2+T.nx), (int)(vheight2+T.ny), 1/T.nz,		\
                        GRID3D);												\
      }																\

void DrawGrid3D(TDIBitmap8 &Dest, CameraInfo *C)
{
   DECLARE_VWIDTHHEIGHT(Dest);

//	Vector3 corners[4];
//   vec2_t corn2d[4];
   Transform3DStruct S,T;

   float diff;
//   vec2_t start,end;

   diff = g_GridSize;

//   float cosa1,sina1,cosa2,sina2;

   S.Cam = T.Cam = C;

   S.cosa1 = T.cosa1 = (float)cos(RotX);
   S.sina1 = T.sina1 = (float)sin(RotX);
   S.cosa2 = T.cosa2 = (float)cos(RotY);
   S.sina2 = T.sina2 = (float)sin(RotY);

   // draw corners

   T.z = S.z = g_GroundHeight;

/*   S.x = g_GridMin;
   S.y = g_GridMin;
   T.x = g_GridMax;
   T.y = g_GridMin;

   GridLine3D(S,T);
   S.x = g_GridMax;
   S.y = g_GridMin;
   T.x = g_GridMax;
   T.y = g_GridMax;

   GridLine3D(S,T);
   S.x = g_GridMax;
   S.y = g_GridMax;
   T.x = g_GridMin;
   T.y = g_GridMax;

   GridLine3D(S,T);
   S.x = g_GridMin;
   S.y = g_GridMax;
   T.x = g_GridMin;
   T.y = g_GridMin;

   GridLine3D(S,T);*/

   T.y = g_GridMin;
   T.x = g_GridMin;

   while (T.y<g_GridMax)
   {
		S.y = T.y += diff;
	   S.x = g_GridMin;
      T.x = g_GridMin;
      while (T.x < g_GridMax)
	   {
      	T.x += diff;
   	   GridLine3D(S,T);
         S.x += diff;
	   }
   }

   T.x = g_GridMin;
   T.y = g_GridMin;

   while (T.x<g_GridMax)
   {
		S.x = T.x += diff;
	   S.y = g_GridMin;
      T.y = g_GridMin;
      while (T.y < g_GridMax)
	   {
      	T.y += diff;
   	   GridLine3D(S,T);
         S.y += diff;
	   }
   }

/*   if (T.Transform3D)

   for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
   {
   	float tempy;

      nx = (-g_CurMdl.Verts[n].x+PanX)*cosa1 + (-g_CurMdl.Verts[n].y+PanY)*sina1;
      tempy = (-g_CurMdl.Verts[n].y+PanY)*cosa1 - (-g_CurMdl.Verts[n].x+PanX)*sina1;

      nz = (g_CurMdl.Verts[n].z+PanZ)*cosa2 + (tempy)*sina2;
      ny = tempy*cosa2 - (g_CurMdl.Verts[n].z+PanZ)*sina2;

      nz+=Dist;
      if (nz > 1)
      {
         g_posz[n] = nz;
      	g_posx[n] = g_Persp*nx/nz;
         g_posy[n] = g_Persp*ny/nz;
         poszinv[n] = 1/nz;
			todraw[n] = true;
      }
      else todraw[n] = false;

   }

   delete[] start;
   delete[] end;*/
}

#undef GridLine3D

void DrawGridSolid3D(TDIBitmap8 &Dest, CameraInfo *C)
{
   Transform3DStruct Trans;

   Trans.Cam = C;

   Trans.cosa1 = (float)cos(C->m_RotX);
   Trans.sina1 = (float)sin(C->m_RotX);
   Trans.cosa2 = (float)cos(C->m_RotY);
   Trans.sina2 = (float)sin(C->m_RotY);

   double GS = (g_GridMax-g_GridMin)/16;
   TexVertex   V[16][16];
   int         TriCount = 0;
   bool        DrawVert[16][16];
   TexTriangle T[512], *TempTri;
   int         i;

   TempTri = T;

   Trans.z = g_GroundHeight;
   for (i=0;i<16;i++)
   {
      Trans.x = (float)(i*GS + g_GridMin);

      for (int j=0;j<16;j++)
      {
         Trans.z = (i+j) + g_GroundHeight;
         Trans.y = (float)(j*GS + g_GridMin);

         if (Trans.Transform3D())
         {
            V[i][j].sx = Trans.nx + (Dest.Width >> 1);
            V[i][j].sy = Trans.ny + (Dest.Height >> 1);
            V[i][j].sz = 1/Trans.nz;
            V[i][j].br = 32768*1024*(i+j)/2;
//            V[i][j].br = 32768*16384;

            DrawVert[i][j] = true;
         }
         else
            DrawVert[i][j] = false;
      }
   }

   for (i=0;i<512;i++)
      T[i].v[0] = T[i].v[1] = T[i].v[2] = NULL;

   for (i=0;i<15;i++)
      for (int j=0;j<15;j++)
      {
         if (DrawVert[i][j] && DrawVert[i+1][j] && DrawVert[i][j+1] && DrawVert[i+1][j+1])
         {
            TempTri->Tex = NULL;
            TempTri->v[0] = &V[i][j];
            TempTri->v[1] = &V[i][j+1];
            TempTri->v[2] = &V[i+1][j+1];

            TempTri++;
            TriCount++;

            TempTri->Tex = NULL;
            TempTri->v[1] = &V[i+1][j+1];
            TempTri->v[2] = &V[i+1][j];
            TempTri->v[0] = &V[i][j];

            TempTri++;
            TriCount++;
         }
      }

   {
      int i;
      for (i=0;i<512;i++)
         if (!(T[i].v[0] && T[i].v[1] && T[i].v[2]))
            break;

      TriCount = i-1;
   }


   for (i=0;i<TriCount;i++)
   {
      if (T[i].v[0] && T[i].v[1] && T[i].v[2])
         Dest.DrawGouradTri_ZBuf(T[i]);
   }

}

void inline DrawTick(TDIBitmap8 &Dest, int x, int y, unsigned char col)
{
	Dest.Line(x,y-2,x,y+2,col);
	Dest.Line(x-2,y,x+2,y,col);
}

void Draw3DView(TDIBitmap8 &Dest, TCompleteModel *M, CDrawViewInfo *Info, TCompleteModel *Ref)
{
//   struct time b,a;

//   gettime(&a);

   int      i;

   DECLARE_VWIDTHHEIGHT(Dest);

   Transform3DStruct Trans;

   Trans.Cam = &Info->Cam;

   Trans.cosa1 = (float)cos(Info->Cam.m_RotX);
   Trans.sina1 = (float)sin(Info->Cam.m_RotX);
   Trans.cosa2 = (float)cos(Info->Cam.m_RotY);
   Trans.sina2 = (float)sin(Info->Cam.m_RotY);

  	if (Dest.PInfoHeader == NULL)
   	return;

	for (i = Dest.Width*Dest.Height-1;i>=0;i--)
   	Dest.ZBuffer[i] = 0.0;

//   if (M->Verts==NULL || M->Tris==NULL) return;
//	int *order;
   float *trisz,*poszinv;
   bool *todraw,*tridone, *drawtri;
  	float nx,ny,nz;
   float cosa1,sina1,cosa2,sina2;

   cosa1 = (float)cos(Info->Cam.m_RotX);
   sina1 = (float)sin(Info->Cam.m_RotX);
   cosa2 = (float)cos(Info->Cam.m_RotY);
   sina2 = (float)sin(Info->Cam.m_RotY);

   Dest.Clear(BACKGROUND3D);

   g_Drawing = true;

	if (Info->Grid)
//		DrawGridSolid3D(Dest, &Info->Cam);
      DrawGrid3D(Dest, &Info->Cam);

	if (Dest.TexWidth != g_CurMdl.Head.skinwidth ||
   	 Dest.TexHeight != g_CurMdl.Head.skinheight )
       Dest.SetTexSize(g_CurMdl.Head.skinwidth, g_CurMdl.Head.skinheight);

   if (g_pRef && Info->m_DrawRef)
   {
      int a,b;

      a = Dest.TexWidth;
      b = Dest.TexHeight;
      Dest.SetTexSize(g_pRef->Head.skinwidth, g_pRef->Head.skinheight);
      Draw3DModelOnly(Dest, g_pRef, Info);
      Dest.SetTexSize(a,b);
   }


   if (Info->Origin)
   {
		Vector3  P[6];
      float 	orig_px[6], orig_py[6], orig_pz[6];
      bool     Okay;

      ZeroVec(P[0]);
      ZeroVec(P[1]);
      ZeroVec(P[2]);
      ZeroVec(P[3]);
      ZeroVec(P[4]);
      ZeroVec(P[5]);

      P[0].x = 5;
      P[1].x = -5;
      P[2].y = 5;
      P[3].y = -5;
      P[4].z = 5;
      P[5].z = -5;

      Okay = true;

      for (int i=0;i<6;i++)
      {
         Trans.SetXYZ( -P[i].x, -P[i].y, P[i].z);
         if (Trans.Transform3D())
         {
            orig_px[i] = Trans.nx;
            orig_py[i] = Trans.ny;
            orig_pz[i] = 1/Trans.nz;
         }
         else
            Okay = false;
      }

      if (Okay)
      {
         Dest.Line_ZBuf((int)(vwidth2+orig_px[0]), (int)(vheight2+orig_py[0]), orig_pz[0],
      				      (int)(vwidth2+orig_px[1]), (int)(vheight2+orig_py[1]), orig_pz[1], ORIGIN);
         Dest.Line_ZBuf( 	(int)(vwidth2+orig_px[2]), (int)(vheight2+orig_py[2]), orig_pz[2],
      				   (int)(vwidth2+orig_px[3]), (int)(vheight2+orig_py[3]), orig_pz[3], ORIGIN);
         Dest.Line_ZBuf( 	(int)(vwidth2+orig_px[4]), (int)(vheight2+orig_py[4]), orig_pz[4],
      				   (int)(vwidth2+orig_px[5]), (int)(vheight2+orig_py[5]), orig_pz[5], ORIGIN);
      }
   }

   if (M->Head.num_xyz >0)
   {
   	if (g_posx != NULL)
         delete[] g_posx;
      if (g_posy != NULL)
         delete[] g_posy;
      if (g_posz != NULL)
         delete[] g_posz;

      g_posx = g_posy = NULL;
      g_posz = NULL;

		g_posx = new int[M->Head.num_xyz];
      g_posy = new int[M->Head.num_xyz];
//      order = new int[M->Head.num_tris];

   	g_posz = new float[M->Head.num_xyz];
      poszinv = new float[M->Head.num_xyz];
      trisz = new float[M->Head.num_tris];

      todraw = new bool[M->Head.num_xyz];
      tridone = new bool[M->Head.num_tris];
   }
   else
   {
   	if (Dest.PInfoHeader != NULL)
		   Dest.Rect(0,0,Dest.VisibleWidth-1,Dest.Height-1,BLACK);
   	return;
   }

   for (int n=0;n<M->Head.num_xyz;n++)
   {
      Trans.x = -M->Verts[n].x;
      Trans.y = -M->Verts[n].y;
      Trans.z = M->Verts[n].z;

      if (Trans.Transform3D())
      {
         g_posx[n] = (int)Trans.nx;
         g_posy[n] = (int)Trans.ny;
         g_posz[n] = Trans.nz;
         poszinv[n] = (float)(1.0 / Trans.nz);
         todraw[n] = true;
      }
      else
         todraw[n] = false;
   }

	drawtri = new bool[M->Head.num_tris];

   for (i=0;i<M->Head.num_tris;i++)
   {
      if ( todraw[M->Tris[i].Vertices[0]] &&
         todraw[M->Tris[i].Vertices[1]] &&
         todraw[M->Tris[i].Vertices[2]])
      {
      	float dotprod;

         dotprod = FaceDotProd3D(*M, i,cosa1,sina1,cosa2,sina2);

			if ((!Info->m_3DFaceCull) || dotprod>0)
	   		drawtri[i] = true;
         else
            drawtri[i] = false;

      }
      else
      	drawtri[i] = false;

      if (M->Tris[i].Visible == false)
      	drawtri[i] = false;
   }

   {
      unsigned int   *VertBright;

      VertBright = new unsigned int[M->Head.num_xyz];
      {
/*       Vector3  *VecVerts;
         int      *VecCount;

         VecVerts = new Vector3[M->Head.num_xyz];
         VecCount = new int[M->Head.num_xyz];

         memset(VecVerts, 0, sizeof(Vector3)*M->Head.num_xyz);
         memset(VecCount, 0, sizeof(int)*M->Head.num_xyz);

         for (int t=0;t<M->Head.num_tris;t++)
         {
            VecVerts[ M->Tris[t].Vertices[0] ].x += M->Tris[t].Normal.x;
            VecVerts[ M->Tris[t].Vertices[0] ].y += M->Tris[t].Normal.y;
            VecVerts[ M->Tris[t].Vertices[0] ].z += M->Tris[t].Normal.z;
            VecCount[ M->Tris[t].Vertices[0] ] ++;
            VecVerts[ M->Tris[t].Vertices[1] ].x += M->Tris[t].Normal.x;
            VecVerts[ M->Tris[t].Vertices[1] ].y += M->Tris[t].Normal.y;
            VecVerts[ M->Tris[t].Vertices[1] ].z += M->Tris[t].Normal.z;
            VecCount[ M->Tris[t].Vertices[1] ] ++;
            VecVerts[ M->Tris[t].Vertices[2] ].x += M->Tris[t].Normal.x;
            VecVerts[ M->Tris[t].Vertices[2] ].y += M->Tris[t].Normal.y;
            VecVerts[ M->Tris[t].Vertices[2] ].z += M->Tris[t].Normal.z;
            VecCount[ M->Tris[t].Vertices[2] ] ++;
         }
*/
         for (int v=0;v<M->Head.num_xyz;v++)
         {
/*
            if (VecCount[v] == 0)
               continue;

            VecVerts[v].Normalize();

            VecVerts[v].x /= (float)VecCount[v];
            VecVerts[v].y /= (float)VecCount[v];
            VecVerts[v].z /= (float)VecCount[v];*/

            double bright=0,diff;

            for (int i=0;i<NUM_LIGHTS;i++)
            {
               diff = Lights[i] * M->Verts[v].Normal;
               if (diff>0)
                  bright+=diff;
            }
            if (bright>1) bright = 1;
            bright *= (double)(32768);
            bright *= (double)(32768);

            VertBright[v] = (UINT)bright;
         }

//         delete[] VecVerts;
//         delete[] VecCount;

      }

      {
         TexVertex   *TexVerts,*pTexVerts;
         int         n;

         pTexVerts = TexVerts = new TexVertex[M->Head.num_xyz];

         for (n=0;n<M->Head.num_xyz;n++)
         {
            pTexVerts->sx = (float)(vwidth2+g_posx[n]+1);
            pTexVerts->sy = (float)(vheight2+g_posy[n]);
            pTexVerts->sz = poszinv[n];
            pTexVerts->br = VertBright[n];
            pTexVerts++;

         }

         for (n=0;n<M->Head.num_tris;n++)
            if (drawtri[n])
            {
               {
                  TexTriangle	T;
                  POINT	pnts[3];

                  pnts[0].x = vwidth2+g_posx[M->Tris[n].Vertices[0]];
                  pnts[0].y = vheight2+g_posy[M->Tris[n].Vertices[0]];
                  pnts[1].x = vwidth2+g_posx[M->Tris[n].Vertices[1]];
                  pnts[1].y = vheight2+g_posy[M->Tris[n].Vertices[1]];
                  pnts[2].x = vwidth2+g_posx[M->Tris[n].Vertices[2]];
                  pnts[2].y = vheight2+g_posy[M->Tris[n].Vertices[2]];

                  if (Info->Mode == stTriangle && M->Tris[n].Selected)
                  {
                     if (Info->Texture == ttWire)
                     {
                        Dest.Poly(pnts,3,SELFACE);
                     }
                     else
                     {
                        T.Tex = M->Skins[g_CurSkin];
                        T.v[0] = &TexVerts[ M->Tris[n].Vertices[0] ];
                        T.v[1] = &TexVerts[ M->Tris[n].Vertices[1] ];
                        T.v[2] = &TexVerts[ M->Tris[n].Vertices[2] ];

                        T.v[0]->u = M->SkinVerts[ M->Tris[n].SkinVerts[0] ].s;
                        T.v[0]->v = M->SkinVerts[ M->Tris[n].SkinVerts[0] ].t;
                        T.v[1]->u = M->SkinVerts[ M->Tris[n].SkinVerts[1] ].s;
                        T.v[1]->v = M->SkinVerts[ M->Tris[n].SkinVerts[1] ].t;
                        T.v[2]->u = M->SkinVerts[ M->Tris[n].SkinVerts[2] ].s;
                        T.v[2]->v = M->SkinVerts[ M->Tris[n].SkinVerts[2] ].t;


                        unsigned char Col;

                        TRealTriangle	*tri = &M->Tris[n];
                        Vector3 Norm(tri->Normal.x, tri->Normal.y, tri->Normal.z);

                        float bright=0,diff;

                        for (int i=0;i<NUM_LIGHTS;i++)
                        {
                           diff = Lights[i]*Norm;
                           if (diff>0)
                              bright+=diff;
                        }
                        if (bright>1) bright = 1;

                        Col = Dest.Bright[SELFACESHADMAX][(int)(bright*128)];

                        if (Info->Shading == shGourad)
                        {
                           unsigned char a,b;
                           a = FACESHADMIN;
                           b = FACESHADMAX;

                           FACESHADMIN = SELFACESHADMIN;
                           FACESHADMAX = SELFACESHADMAX;

                           Dest.DrawGouradTri_ZBuf(T);

                           FACESHADMIN = a;
                           FACESHADMAX = b;
                        }
                        else
                        if (Info->Shading == shFlat)
                           Dest.DrawTri_ZBuf(T, Col);

//                        if (Info->Texture == ttAffine)
//                        if (M->Head.num_skins>0)
//                           Dest.DrawTexTri_ZBufGreyScale(T, TEXSELFACE);
                     }
                  }
                  else
                  {
                     if (Info->Texture == ttWire)
                     {
                        Dest.Poly(pnts,3,WIREFRAME3D);
                     }
                     else
                     {
                        T.Tex = M->Skins[g_CurSkin];
                        T.v[0] = &TexVerts[ M->Tris[n].Vertices[0] ];
                        T.v[1] = &TexVerts[ M->Tris[n].Vertices[1] ];
                        T.v[2] = &TexVerts[ M->Tris[n].Vertices[2] ];

                        T.v[0]->u = M->SkinVerts[ M->Tris[n].SkinVerts[0] ].s;
                        T.v[0]->v = M->SkinVerts[ M->Tris[n].SkinVerts[0] ].t;
                        T.v[1]->u = M->SkinVerts[ M->Tris[n].SkinVerts[1] ].s;
                        T.v[1]->v = M->SkinVerts[ M->Tris[n].SkinVerts[1] ].t;
                        T.v[2]->u = M->SkinVerts[ M->Tris[n].SkinVerts[2] ].s;
                        T.v[2]->v = M->SkinVerts[ M->Tris[n].SkinVerts[2] ].t;

                        unsigned char Col;

                        TRealTriangle	*tri = &M->Tris[n];
                        Vector3 Norm(tri->Normal.x, tri->Normal.y, tri->Normal.z);

                        float bright=0,diff;

                        for (int i=0;i<NUM_LIGHTS;i++)
                        {
                           diff = Lights[i]*Norm;
                           if (diff>0)
                              bright+=diff;
                        }
                        if (bright>1) bright = 1;

                        Col = Dest.Bright[FACESHADMAX][(int)(bright*128)];

                        if (Info->Texture == ttNone)
                        {
                           if (Info->Shading == shGourad)
                              Dest.DrawGouradTri_ZBuf(T);
                           else
                           if (Info->Shading == shFlat)
                              Dest.DrawTri_ZBuf(T, Col);
                        }
                        else
                        if (Info->Texture == ttAffine && M->Head.num_skins>0)
                        {
                           if (Info->Shading == shGourad)
                              Dest.DrawTexTriGourad_ZBuf(T);
                           else
                           if (Info->Shading == shNone)
                              Dest.DrawTexTri_ZBuf(T);
                        }
                     }
                  }
               }

               float fx,fy,fz,tempy;
               int Cx,Cy,Nx,Ny;
               bool drawnorm=true;

               fx = M->Tris[n].Centre.x;
               fy = M->Tris[n].Centre.y;
               fz = M->Tris[n].Centre.z;

               nx = (float)((-fx+Info->Cam.m_PanX)*cosa1 + (-fy+Info->Cam.m_PanY)*sina1);
               tempy = float((-fy+Info->Cam.m_PanY)*cosa1 - (-fx+Info->Cam.m_PanX)*sina1);

               nz = (float)((fz+Info->Cam.m_PanZ)*cosa2 + (tempy)*sina2);
               ny = (float)(tempy*cosa2 - (fz+Info->Cam.m_PanZ)*sina2);

               nz+=Dist;
               if (nz > 1)
               {
                  Cx = (int)(g_Persp*nx/nz);
                  Cy = (int)(g_Persp*ny/nz);
               }
               else drawnorm = false;

               fx = M->Tris[n].Centre.x - 3*M->Tris[n].Normal.x;
               fy = M->Tris[n].Centre.y - 3*M->Tris[n].Normal.y;
               fz = M->Tris[n].Centre.z - 3*M->Tris[n].Normal.z;

               nx = (float)((-fx+Info->Cam.m_PanX)*cosa1 + (-fy+Info->Cam.m_PanY)*sina1);
               tempy = (float)((-fy+Info->Cam.m_PanY)*cosa1 - (-fx+Info->Cam.m_PanX)*sina1);

               nz = (float)((fz+Info->Cam.m_PanZ)*cosa2 + (tempy)*sina2);
               ny = (float)(tempy*cosa2 - (fz+Info->Cam.m_PanZ)*sina2);

               nz+=Dist;
               if (nz > 1)
               {
                  Nx = (int)(g_Persp*nx/nz - Cx);
                  Ny = (int)(g_Persp*ny/nz - Cy);
               }
               else drawnorm = false;

               if ((Info->Normals == ndAll) && drawnorm)
               {
                  Dest.Line(vwidth2+Cx,vheight2+Cy,vwidth2+Cx+(Nx>>1), vheight2+Cy+(Ny>>1), NORMHI);
                  Dest.Line(vwidth2+Cx+(Nx>>1),vheight2+Cy+(Ny>>1),vwidth2+Cx+Nx, vheight2+Cy+Ny, NORMLO);
               }
               if ((Info->Normals == ndSelected) && M->Tris[n].Selected && drawnorm)
               {
                  Dest.Line(vwidth2+Cx,vheight2+Cy,vwidth2+Cx+(Nx>>1), vheight2+Cy+(Ny>>1), NORMHI);
                  Dest.Line(vwidth2+Cx+(Nx>>1),vheight2+Cy+(Ny>>1),vwidth2+Cx+Nx, vheight2+Cy+Ny, NORMLO);
               }

            }

         delete[] TexVerts;
      }

      if (Info->Ticks)
      {
         for (int n=0;n<M->Head.num_xyz;n++)
         if (M->Verts[n].Visible)
         {
            if ( Info->Mode == stVertex && M->Verts[n].Selected)
               DrawTick(Dest,vwidth2+g_posx[n],vheight2+g_posy[n],YELLOW);
            else DrawTick(Dest,vwidth2+g_posx[n],vheight2+g_posy[n],TICKCOLOR);
         }
      }
      else
      for (int n=0;n<M->Head.num_xyz;n++)
      {
         if ( Info->Mode == stVertex && M->Verts[n].Selected)
         if (M->Verts[n].Visible)
            Dest.PutPixel(vwidth2+g_posx[n],vheight2+g_posy[n],SELVERT);
      }


      delete[] VertBright;
   }

   if (g_MainAction == atBuildingFace1 && vtCamera == g_BuildFaceView)
   {
      Dest.Line( (int)(vwidth2 + g_posx[CreateFace[0]]),
                 (int)(vheight2 + g_posy[CreateFace[0]]),
                 (int)(vwidth2 + g_downx),
                 (int)(vheight2 + g_downy),SELECTBOX);
   }
   if (g_MainAction == atBuildingFace2 && vtCamera == g_BuildFaceView)
   {
      Dest.Line( (int)(vwidth2 + g_posx[CreateFace[0]]),
                 (int)(vheight2 + g_posy[CreateFace[0]]),
                 (int)(vwidth2 + g_posx[CreateFace[1]]),
                 (int)(vheight2 + g_posy[CreateFace[1]]),SELECTBOX);

      Dest.Line( (int)(vwidth2 + g_posx[CreateFace[0]]),
                 (int)(vheight2 + g_posy[CreateFace[0]]),
                 (int)(vwidth2 + g_downx),
                 (int)(vheight2 + g_downy),SELECTBOX);
      Dest.Line( (int)(vwidth2 + g_posx[CreateFace[1]]),
                 (int)(vheight2 + g_posy[CreateFace[1]]),
                 (int)(vwidth2 + g_downx),
                 (int)(vheight2 + g_downy),SELECTBOX);
   }



   delete[] poszinv;
   delete[] trisz;
   delete[] todraw;
   delete[] drawtri;
   delete[] tridone;
//   delete[] order;

   if (g_MainAction == atSelectBox || g_MainAction == atDeselectBox)
   {
      Dest.Rect((int)(g_downx+vwidth2),(int)(g_downy+vheight2),(int)(g_upx+vwidth2),(int)(g_upy+vheight2), SELECTBOX);
   }

   Dest.Rect(0,0,Dest.VisibleWidth-1,Dest.VisibleHeight-1,BLACK);

   {
      Dest.MinX = Dest.VisibleWidth-1;
      Dest.MaxX = 0;
      Dest.MinY = Dest.Height-1;
      Dest.MaxY = 0;

      for (int i=0;i<M->Head.num_xyz;i++)
      {
         if (g_posx[i]<Dest.MinX)
         	Dest.MinX = g_posx[i];
         if (g_posx[i]>Dest.MaxX)
         	Dest.MaxX = g_posx[i];
         if (g_posy[i]<Dest.MinY)
         	Dest.MinY = g_posy[i];
         if (g_posy[i]>Dest.MaxY)
         	Dest.MaxY = g_posy[i];
      }

      if (Dest.MaxX > Dest.VisibleWidth-1)
      	Dest.MaxX = Dest.VisibleWidth-1;
      if (Dest.MaxX < 0)
      	Dest.MaxX = 0;
      if (Dest.MinX > Dest.VisibleWidth-1)
      	Dest.MinX = Dest.VisibleWidth-1;
      if (Dest.MinX < 0)
      	Dest.MinX = 0;

      if (Dest.MaxY > Dest.Height-1)
      	Dest.MaxY = Dest.Height-1;
      if (Dest.MaxY < 0)
      	Dest.MaxY = 0;
      if (Dest.MinY > Dest.Height-1)
      	Dest.MinY = Dest.Height-1;
      if (Dest.MinY < 0)
      	Dest.MinY = 0;
   }

//   gettime(&b);
//   MainForm->CounterLabel->Caption = b.ti_hund - a.ti_hund + 100*(b.ti_sec - a.ti_sec);

   g_Drawing = false;


}


void Draw3DModelOnly(TDIBitmap8 &Dest, TCompleteModel *M, CDrawViewInfo *Info)
{
   DECLARE_VWIDTHHEIGHT(Dest);

   Transform3DStruct Trans;
   int               i;

   Trans.Cam = &Info->Cam;

   Trans.cosa1 = (float)cos(Info->Cam.m_RotX);
   Trans.sina1 = (float)sin(Info->Cam.m_RotX);
   Trans.cosa2 = (float)cos(Info->Cam.m_RotY);
   Trans.sina2 = (float)sin(Info->Cam.m_RotY);

  	if (Dest.PInfoHeader == NULL)
   	return;

	for (i = Dest.Width*Dest.Height-1;i>=0;i--)
   	Dest.ZBuffer[i] = 0.0;

//   if (M->Verts==NULL || M->Tris==NULL) return;
//	int *order;
   float *trisz,*poszinv;
   bool *todraw,*tridone, *drawtri;
  	float nx,ny,nz;
   float cosa1,sina1,cosa2,sina2;

   cosa1 = (float)cos(Info->Cam.m_RotX);
   sina1 = (float)sin(Info->Cam.m_RotX);
   cosa2 = (float)cos(Info->Cam.m_RotY);
   sina2 = (float)sin(Info->Cam.m_RotY);

   g_Drawing = true;

   if (M->Head.num_xyz >0)
   {
   	if (g_posx != NULL) delete[] g_posx;
      if (g_posy != NULL) delete[] g_posy;
      if (g_posz != NULL) delete[] g_posz;
		g_posx = new int[M->Head.num_xyz];
      g_posy = new int[M->Head.num_xyz];
//      order = new int[M->Head.num_tris];

   	g_posz = new float[M->Head.num_xyz];
      poszinv = new float[M->Head.num_xyz];
      trisz = new float[M->Head.num_tris];

      todraw = new bool[M->Head.num_xyz];
      tridone = new bool[M->Head.num_tris];
   }
   else
   {
   	if (Dest.PInfoHeader != NULL)
		   Dest.Rect(0,0,Dest.VisibleWidth-1,Dest.Height-1,BLACK);
   	return;
   }

   for (int n=0;n<M->Head.num_xyz;n++)
   {

      Trans.x = -M->Verts[n].x;
      Trans.y = -M->Verts[n].y;
      Trans.z = M->Verts[n].z;

      if (Trans.Transform3D())
      {
         g_posx[n] = (int)Trans.nx;
         g_posy[n] = (int)Trans.ny;
         g_posz[n] = Trans.nz;
         poszinv[n] = (float)(1.0 / Trans.nz);
         todraw[n] = true;
      }
      else
         todraw[n] = false;
   }

	drawtri = new bool[M->Head.num_tris];

   for (i=0;i<M->Head.num_tris;i++)
   {
      if ( todraw[M->Tris[i].Vertices[0]] &&
         todraw[M->Tris[i].Vertices[1]] &&
         todraw[M->Tris[i].Vertices[2]])
      {
      	float dotprod;

         dotprod = FaceDotProd3D(*M, i,cosa1,sina1,cosa2,sina2);

			if ((!Info->m_3DFaceCull) || dotprod>0)
	   		drawtri[i] = true;
         else
            drawtri[i] = false;

      }
      else
      	drawtri[i] = false;

      if (M->Tris[i].Visible == false)
      	drawtri[i] = false;
   }

   {
      unsigned int   *VertBright;

      VertBright = new unsigned int[M->Head.num_xyz];
      {
         Vector3  *VecVerts;
         int      *VecCount;

         VecVerts = new Vector3[M->Head.num_xyz];
         VecCount = new int[M->Head.num_xyz];

         memset(VecVerts, 0, sizeof(Vector3)*M->Head.num_xyz);
         memset(VecCount, 0, sizeof(int)*M->Head.num_xyz);

         for (int t=0;t<M->Head.num_tris;t++)
         {
            VecVerts[ M->Tris[t].Vertices[0] ].x += M->Tris[t].Normal.x;
            VecVerts[ M->Tris[t].Vertices[0] ].y += M->Tris[t].Normal.y;
            VecVerts[ M->Tris[t].Vertices[0] ].z += M->Tris[t].Normal.z;
            VecCount[ M->Tris[t].Vertices[0] ] ++;
            VecVerts[ M->Tris[t].Vertices[1] ].x += M->Tris[t].Normal.x;
            VecVerts[ M->Tris[t].Vertices[1] ].y += M->Tris[t].Normal.y;
            VecVerts[ M->Tris[t].Vertices[1] ].z += M->Tris[t].Normal.z;
            VecCount[ M->Tris[t].Vertices[1] ] ++;
            VecVerts[ M->Tris[t].Vertices[2] ].x += M->Tris[t].Normal.x;
            VecVerts[ M->Tris[t].Vertices[2] ].y += M->Tris[t].Normal.y;
            VecVerts[ M->Tris[t].Vertices[2] ].z += M->Tris[t].Normal.z;
            VecCount[ M->Tris[t].Vertices[2] ] ++;
         }

         for (int v=0;v<M->Head.num_xyz;v++)
         {
            if (VecCount[v] == 0)
               continue;

            VecVerts[v].x /= (float)VecCount[v];
            VecVerts[v].y /= (float)VecCount[v];
            VecVerts[v].z /= (float)VecCount[v];

            double bright=0,diff;

            for (int i=0;i<NUM_LIGHTS;i++)
            {
               diff = Lights[i] * VecVerts[v];
               if (diff>0)
                  bright+=diff;
            }
            if (bright>1) bright = 1;
            bright *= (double)(32768);
            bright *= (double)(32768);

            VertBright[v] = (UINT)bright;
         }

         delete[] VecVerts;
         delete[] VecCount;

      }

      {
         TexVertex   *TexVerts,*pTexVerts;
         int         n;

         pTexVerts = TexVerts = new TexVertex[M->Head.num_xyz];

         for (n=0;n<M->Head.num_xyz;n++)
         {
            pTexVerts->sx = (float)(vwidth2+g_posx[n]+1);
            pTexVerts->sy = (float)(vheight2+g_posy[n]);
            pTexVerts->sz = poszinv[n];
            pTexVerts->u = M->SkinVerts[n].s;
            pTexVerts->v = M->SkinVerts[n].t;
            pTexVerts->br = VertBright[n];
            pTexVerts++;

         }

         for (n=0;n<M->Head.num_tris;n++)
         {
            if (drawtri[n])
            {
               {
                  TexTriangle	T;
                  POINT	pnts[3];
                  
                  pnts[0].x = vwidth2+g_posx[M->Tris[n].Vertices[0]];
                  pnts[0].y = vheight2+g_posy[M->Tris[n].Vertices[0]];
                  pnts[1].x = vwidth2+g_posx[M->Tris[n].Vertices[1]];
                  pnts[1].y = vheight2+g_posy[M->Tris[n].Vertices[1]];
                  pnts[2].x = vwidth2+g_posx[M->Tris[n].Vertices[2]];
                  pnts[2].y = vheight2+g_posy[M->Tris[n].Vertices[2]];
                  
                  if (Info->Mode == stTriangle && M->Tris[n].Selected)
                  {
                     if (Info->Texture == ttWire)
                     {
                        Dest.Poly(pnts,3,SELFACE);
                     }
                     else
                     {
                        T.Tex = M->Skins[g_CurSkin];
                        T.v[0] = &TexVerts[ M->Tris[n].Vertices[0] ];
                        T.v[1] = &TexVerts[ M->Tris[n].Vertices[1] ];
                        T.v[2] = &TexVerts[ M->Tris[n].Vertices[2] ];
                        
                        T.v[0]->u = M->SkinVerts[ M->Tris[n].SkinVerts[0] ].s;
                        T.v[0]->v = M->SkinVerts[ M->Tris[n].SkinVerts[0] ].t;
                        T.v[1]->u = M->SkinVerts[ M->Tris[n].SkinVerts[1] ].s;
                        T.v[1]->v = M->SkinVerts[ M->Tris[n].SkinVerts[1] ].t;
                        T.v[2]->u = M->SkinVerts[ M->Tris[n].SkinVerts[2] ].s;
                        T.v[2]->v = M->SkinVerts[ M->Tris[n].SkinVerts[2] ].t;
                        
                        
                        unsigned char Col;
                        
                        TRealTriangle	*tri = &M->Tris[n];
                        Vector3 Norm(tri->Normal.x, tri->Normal.y, tri->Normal.z);
                        
                        float bright=0,diff;
                        
                        for (int i=0;i<NUM_LIGHTS;i++)
                        {
                           diff = Lights[i]*Norm;
                           if (diff>0)
                              bright+=diff;
                        }
                        if (bright>1) bright = 1;
                        
                        Col = Dest.Bright[SELFACESHADMAX][(int)(bright*128)];
                        
                        if (Info->Shading == shGourad)
                        {
                           unsigned char a,b;
                           a = FACESHADMIN;
                           b = FACESHADMAX;
                           
                           FACESHADMIN = SELFACESHADMIN;
                           FACESHADMAX = SELFACESHADMAX;
                           
                           Dest.DrawGouradTri_ZBuf(T);
                           
                           FACESHADMIN = a;
                           FACESHADMAX = b;
                        }
                        else
                           if (Info->Shading == shFlat)
                              Dest.DrawTri_ZBuf(T, Col);
                           
                           if (Info->Texture == ttAffine)
                              if (M->Head.num_skins>0)
                                 Dest.DrawTexTri_ZBufGreyScale(T, TEXSELFACE);
                     }
                  }
                  else
                  {
                     if (Info->Texture == ttWire)
                     {
                        Dest.Poly(pnts,3,WIREFRAME3D);
                     }
                     else
                     {
                        T.Tex = M->Skins[g_CurSkin];
                        T.v[0] = &TexVerts[ M->Tris[n].Vertices[0] ];
                        T.v[1] = &TexVerts[ M->Tris[n].Vertices[1] ];
                        T.v[2] = &TexVerts[ M->Tris[n].Vertices[2] ];
                        
                        T.v[0]->u = M->SkinVerts[ M->Tris[n].SkinVerts[0] ].s;
                        T.v[0]->v = M->SkinVerts[ M->Tris[n].SkinVerts[0] ].t;
                        T.v[1]->u = M->SkinVerts[ M->Tris[n].SkinVerts[1] ].s;
                        T.v[1]->v = M->SkinVerts[ M->Tris[n].SkinVerts[1] ].t;
                        T.v[2]->u = M->SkinVerts[ M->Tris[n].SkinVerts[2] ].s;
                        T.v[2]->v = M->SkinVerts[ M->Tris[n].SkinVerts[2] ].t;
                        
                        unsigned char Col;
                        
                        TRealTriangle	*tri = &M->Tris[n];
                        Vector3 Norm(tri->Normal.x, tri->Normal.y, tri->Normal.z);
                        
                        float bright=0,diff;
                        
                        for (int i=0;i<NUM_LIGHTS;i++)
                        {
                           diff = Lights[i]*Norm;
                           if (diff>0)
                              bright+=diff;
                        }
                        if (bright>1) bright = 1;
                        
                        Col = Dest.Bright[FACESHADMAX][(int)(bright*128)];
                        
                        if (Info->Texture == ttNone)
                        {
                           if (Info->Shading == shGourad)
                              Dest.DrawGouradTri_ZBuf(T);
                           else
                              if (Info->Shading == shFlat)
                                 Dest.DrawTri_ZBuf(T, Col);
                        }
                        else
                           if (Info->Texture == ttAffine && M->Head.num_skins>0)
                           {
                              if (Info->Shading == shGourad)
                                 Dest.DrawTexTriGourad_ZBuf(T);
                              else
                                 if (Info->Shading == shNone)
                                    Dest.DrawTexTri_ZBuf(T);
                           }
                     }
                  }
               }
               
               float fx,fy,fz,tempy;
               int Cx,Cy,Nx,Ny;
               bool drawnorm=true;
               
               fx = M->Tris[n].Centre.x;
               fy = M->Tris[n].Centre.y;
               fz = M->Tris[n].Centre.z;
               
               nx = (float)((-fx+Info->Cam.m_PanX)*cosa1 + (-fy+Info->Cam.m_PanY)*sina1);
               tempy = (float)((-fy+Info->Cam.m_PanY)*cosa1 - (-fx+Info->Cam.m_PanX)*sina1);
               
               nz = (float)((fz+Info->Cam.m_PanZ)*cosa2 + (tempy)*sina2);
               ny = (float)(tempy*cosa2 - (fz+Info->Cam.m_PanZ)*sina2);
               
               nz+=Dist;
               if (nz > 1)
               {
                  Cx = (int)(g_Persp*nx/nz);
                  Cy = (int)(g_Persp*ny/nz);
               }
               else drawnorm = false;
               
               fx = M->Tris[n].Centre.x - 3*M->Tris[n].Normal.x;
               fy = M->Tris[n].Centre.y - 3*M->Tris[n].Normal.y;
               fz = M->Tris[n].Centre.z - 3*M->Tris[n].Normal.z;
               
               nx = (float)((-fx+Info->Cam.m_PanX)*cosa1 + (-fy+Info->Cam.m_PanY)*sina1);
               tempy = (float)((-fy+Info->Cam.m_PanY)*cosa1 - (-fx+Info->Cam.m_PanX)*sina1);
               
               nz = (float)((fz+Info->Cam.m_PanZ)*cosa2 + (tempy)*sina2);
               ny = (float)(tempy*cosa2 - (fz+Info->Cam.m_PanZ)*sina2);
               
               nz+=Dist;
               if (nz > 1)
               {
                  Nx = (int)(g_Persp*nx/nz - Cx);
                  Ny = (int)(g_Persp*ny/nz - Cy);
               }
               else drawnorm = false;
               
               if ((Info->Normals == ndAll) && drawnorm)
               {
                  Dest.Line(vwidth2+Cx,vheight2+Cy,vwidth2+Cx+(Nx>>1), vheight2+Cy+(Ny>>1), NORMHI);
                  Dest.Line(vwidth2+Cx+(Nx>>1),vheight2+Cy+(Ny>>1),vwidth2+Cx+Nx, vheight2+Cy+Ny, NORMLO);
               }
               if ((Info->Normals == ndSelected) && M->Tris[n].Selected && drawnorm)
               {
                  Dest.Line(vwidth2+Cx,vheight2+Cy,vwidth2+Cx+(Nx>>1), vheight2+Cy+(Ny>>1), NORMHI);
                  Dest.Line(vwidth2+Cx+(Nx>>1),vheight2+Cy+(Ny>>1),vwidth2+Cx+Nx, vheight2+Cy+Ny, NORMLO);
               }
               
            }
         }
            
         delete[] TexVerts;
      }

      if (Info->Ticks)
         for (int n=0;n<M->Head.num_xyz;n++)
         if (M->Verts[n].Visible)
         {
            if ( Info->Mode == stVertex && M->Verts[n].Selected)
               DrawTick(Dest,vwidth2+g_posx[n],vheight2+g_posy[n],YELLOW);
            else DrawTick(Dest,vwidth2+g_posx[n],vheight2+g_posy[n],TICKCOLOR);
         }
      else
      for (int n=0;n<M->Head.num_xyz;n++)
      {
         if ( Info->Mode == stVertex && M->Verts[n].Selected)
         if (M->Verts[n].Visible)
            Dest.PutPixel(vwidth2+g_posx[n],vheight2+g_posy[n],SELVERT);
      }


      delete[] VertBright;
   }



   delete[] poszinv;
   delete[] trisz;
   delete[] todraw;
   delete[] drawtri;
   delete[] tridone;
}

void __fastcall DrawBoneTree( TBone *Bone, TDIBitmap8 &Dest,
						Matrix3 &XForm, Vector3 &Trans)
{
   DECLARE_VWIDTHHEIGHT(Dest);

	TBone *B;

   B = Bone->Child;
   while (B)
   {
   	DrawBoneTree(B, Dest, XForm, Trans);
      B = B->NextSib;
   }

   // Draw Bone

   Vector3	*P1,*P2;
   P2 = &Bone->Pivot;

   if (Bone->Parent)
	   P1 = &Bone->Parent->Pivot;
   else
   	P1 = P2;

   int x1,y1,x2,y2;

   x1 = (int)(TRANSX(P1->x, P1->y, P1->z, XForm, Trans));
   y1 = (int)(TRANSY(P1->x, P1->y, P1->z, XForm, Trans));
   x2 = (int)(TRANSX(P2->x, P2->y, P2->z, XForm, Trans));
   y2 = (int)(TRANSY(P2->x, P2->y, P2->z, XForm, Trans));

   Dest.Line( x1,y1,x2,y2,BONECOLOR);
   Dest.Line( x1-5,y1-5,x1+5,y1+5,BONECOLOR);
   Dest.Line( x1-5,y1+5,x1+5,y1-5,BONECOLOR);
	if (g_MainSelectMode == stBone)
	   if (Bone->Selected)
      {
         Dest.Line( x1,y1,x2,y2,SELBONE);
         Dest.Line( x1-5,y1-5,x1+5,y1+5,SELBONE);
         Dest.Line( x1-5,y1+5,x1+5,y1-5,SELBONE);
      }
}

void DrawViewMatrix(TDIBitmap8 &Dest, TCompleteModel *M, CDrawViewInfo *Info, ViewType View)
{
   DECLARE_VWIDTHHEIGHT(Dest);

	Dest.Clear(BACKGROUND);

   if (!M)
      return;

  	if (Dest.PInfoHeader == NULL)
   	return;
   if (M->Verts==NULL || M->Tris==NULL)
   {
   	if (Dest.PInfoHeader != NULL)
		   Dest.Rect(0,0,Dest.VisibleWidth-1,Dest.VisibleHeight-1,BACKGROUND);
   }

	Matrix3	XForm;
   Vector3  Trans;

// Set up the transformation matricies
	if (View == vtXY)
   {
   	Trans.Set(vwidth2 - g_Zoom*PanY, vheight2 - g_Zoom*PanX, 0);

      XForm.Set( 	0, 	g_Zoom, 0,
                  g_Zoom, 0, 	0,
                  0,    0, 	g_Zoom);
   }
	if (View == vtXZ)
   {
   	Trans.Set(vwidth2 + g_Zoom*PanX, vheight2 - g_Zoom*PanZ, 0);

      XForm.Set( 	-g_Zoom,0, 	0,
                  0, 	0, 	-g_Zoom,
                  0,    g_Zoom, 	0);
   }
   if (View == vtZY)
   {
   	Trans.Set(vwidth2 - g_Zoom*PanY, vheight2 - g_Zoom*PanZ, 0);

      XForm.Set( 	0, 	g_Zoom, 0,
                  0, 	0, 	-g_Zoom,
                  g_Zoom,    0, 	0);
   }

// transform all the vertices
   TPoint2D			*ScrVerts;
   ScrVerts = new TPoint2D[M->Head.num_xyz];
	{
	   TRealVertex		*V;
      TPoint2D			*P;

      V = M->Verts;
      P = ScrVerts;

      for (int n=M->Head.num_xyz-1;n>=0;n--)
      {
      	try
         {
      	P->x = (int)(XForm.m1.x*V->x + XForm.m1.y*V->y + XForm.m1.z*V->z + Trans.x);
      	P->y = (int)(XForm.m2.x*V->x + XForm.m2.y*V->y + XForm.m2.z*V->z + Trans.y);
      	P++;
         V++;
         }
         catch(...)
         {
         }
      }

   }


   {
   	TRealTriangle	*T;
      TRealVertex		*V;
      TPoint2D			*P;

      T = M->Tris;
      V = M->Verts;
      for (int n=M->Head.num_tris-1;n>=0;n--)
      {
      	float normz;
         POINT	pnts[3];

         pnts[0].x = ScrVerts[T->Vertices[0]].x;
         pnts[0].y = ScrVerts[T->Vertices[0]].y;
         pnts[1].x = ScrVerts[T->Vertices[1]].x;
         pnts[1].y = ScrVerts[T->Vertices[1]].y;
         pnts[2].x = ScrVerts[T->Vertices[2]].x;
         pnts[2].y = ScrVerts[T->Vertices[2]].y;

			if (Info->m_2DFaceCull)
				normz = TRANSZ(T->Normal.x,
                           T->Normal.y,
                           T->Normal.z, XForm, Trans);
         else
         normz = 0;


         if (T->Visible)
				if (!Info->m_2DFaceCull|| normz < 0)
		         Dest.Poly(pnts,3,WIREFRAME);
      	T++;
      }

      if (Info->Mode == stTriangle)
      {
         T = &M->Tris[0];
         for (int n=M->Head.num_tris-1;n>=0;n--)
         {
            if (T->Selected && T->Visible)
            {
               POINT	pnts[3];

               pnts[0].x = ScrVerts[T->Vertices[0]].x;
               pnts[0].y = ScrVerts[T->Vertices[0]].y;
               pnts[1].x = ScrVerts[T->Vertices[1]].x;
               pnts[1].y = ScrVerts[T->Vertices[1]].y;
               pnts[2].x = ScrVerts[T->Vertices[2]].x;
               pnts[2].y = ScrVerts[T->Vertices[2]].y;

               Dest.Poly(pnts,3,SELFACE);
            }
            T++;
         }
      }

      if (Info->Ticks)
      {
         P = ScrVerts;
         M->Verts;

        	for (int n=M->Head.num_xyz-1;n>=0;n--)
         {
            if (V->Visible)
            {
               if (V->Selected && g_MainSelectMode == stVertex)
                  DrawTick(Dest, P->x, P->y, YELLOW);
               else
                  DrawTick(Dest, P->x, P->y, TICKCOLOR);
            }
            V++;
            P++;
         }
      }
      else
      {
         P = ScrVerts;
         V = M->Verts;

      	if (g_MainSelectMode == stVertex)
         	for (int n=M->Head.num_xyz-1;n>=0;n--)
            {
            	if (V->Visible)
	               if (V->Selected)
							Dest.PutPixel(P->x, P->y, SELVERT);
               V++;
               P++;
            }
      }

      if (Info->Origin)
      {
      	Vector3  *O = new Vector3;

         O->x = 0;
         O->y = 0;
         O->z = 0;

         Dest.Line(	(int)TRANSX( -O->x+5, -O->y, -O->z, XForm, Trans),
                     (int)TRANSY( -O->x+5, -O->y, -O->z, XForm, Trans),
                     (int)TRANSX( -O->x-5, -O->y, -O->z, XForm, Trans),
                     (int)TRANSY( -O->x-5, -O->y, -O->z, XForm, Trans), ORIGIN);

         Dest.Line(	(int)TRANSX( -O->x, -O->y+5, -O->z, XForm, Trans),
                     (int)TRANSY( -O->x, -O->y+5, -O->z, XForm, Trans),
                     (int)TRANSX( -O->x, -O->y-5, -O->z, XForm, Trans),
                     (int)TRANSY( -O->x, -O->y-5, -O->z, XForm, Trans), ORIGIN);

         Dest.Line(	(int)TRANSX( -O->x, -O->y, -O->z+5, XForm, Trans),
                     (int)TRANSY( -O->x, -O->y, -O->z+5, XForm, Trans),
                     (int)TRANSX( -O->x, -O->y, -O->z-5, XForm, Trans),
                     (int)TRANSY( -O->x, -O->y, -O->z-5, XForm, Trans), ORIGIN);

         delete O;
      }

      if (Info->Normals == ndSelected)
      {
         T = M->Tris;
         for (int n=M->Head.num_tris-1;n>=0;n--)
         {
            if (T->Selected && T->Visible)
            {
               TPoint2D	a,b,c;

               a.x = (int)TRANSX( T->Centre.x, T->Centre.y, T->Centre.z, XForm, Trans);
               a.y = (int)TRANSY( T->Centre.x, T->Centre.y, T->Centre.z, XForm, Trans);
               b.x = (int)TRANSX(T->Centre.x - 1.5*T->Normal.x,
                            T->Centre.y - 1.5*T->Normal.y,
                            T->Centre.z - 1.5*T->Normal.z, XForm, Trans);
               b.y = (int)TRANSY(T->Centre.x - 1.5*T->Normal.x,
                            T->Centre.y - 1.5*T->Normal.y,
                            T->Centre.z - 1.5*T->Normal.z, XForm, Trans);
               c.x = (int)TRANSX(T->Centre.x - 3*T->Normal.x,
                            T->Centre.y - 3*T->Normal.y,
                            T->Centre.z - 3*T->Normal.z, XForm, Trans);
               c.y = (int)TRANSY(T->Centre.x - 3*T->Normal.x,
                            T->Centre.y - 3*T->Normal.y,
                            T->Centre.z - 3*T->Normal.z, XForm, Trans);
               Dest.Line( a.x, a.y, b.x, b.y, NORMHI);
               Dest.Line( b.x, b.y, c.x, c.y, NORMLO);
            }
            T++;
         }

      }
      if (Info->Normals == ndAll)
      {
         T = M->Tris;
         for (int n=M->Head.num_tris-1;n>=0;n--)
         {
            TPoint2D	a,b,c;

            if (T->Visible)
            {
               a.x = (int)TRANSX( T->Centre.x, T->Centre.y, T->Centre.z, XForm, Trans);
               a.y = (int)TRANSY( T->Centre.x, T->Centre.y, T->Centre.z, XForm, Trans);
               b.x = (int)TRANSX(T->Centre.x - 1.5*T->Normal.x,
                            T->Centre.y - 1.5*T->Normal.y,
                            T->Centre.z - 1.5*T->Normal.z, XForm, Trans);
               b.y = (int)TRANSY(T->Centre.x - 1.5*T->Normal.x,
                            T->Centre.y - 1.5*T->Normal.y,
                            T->Centre.z - 1.5*T->Normal.z, XForm, Trans);
               c.x = (int)TRANSX(T->Centre.x - 3*T->Normal.x,
                            T->Centre.y - 3*T->Normal.y,
                            T->Centre.z - 3*T->Normal.z, XForm, Trans);
               c.y = (int)TRANSY(T->Centre.x - 3*T->Normal.x,
                            T->Centre.y - 3*T->Normal.y,
                            T->Centre.z - 3*T->Normal.z, XForm, Trans);

               Dest.Line( a.x, a.y, b.x, b.y, NORMHI);
               Dest.Line( b.x, b.y, c.x, c.y, NORMLO);
            }
            T++;
         }

      }

      if ((g_MainAction == atSelectBox || g_MainAction == atDeselectBox) && g_CurView == View)
      {
/*      	Dest.Line( TRANSX(g_downx, g_downy, g_downz, XForm, Trans),
                    TRANSY(g_downx, g_downy, g_downz, XForm, Trans),
                    TRANSX(g_upx, g_downy, g_upz, XForm, Trans),
                    TRANSY(g_upx, g_downy, g_upz, XForm, Trans),
                    AQUA);

      	Dest.Line( TRANSX(g_downx, g_upy, g_downz, XForm, Trans),
                    TRANSY(g_downx, g_upy, g_downz, XForm, Trans),
                    TRANSX(g_upx, g_upy, g_upz, XForm, Trans),
                    TRANSY(g_upx, g_upy, g_upz, XForm, Trans),
                    AQUA);

      	Dest.Line( TRANSX(g_downx, g_downy, g_downz, XForm, Trans),
                    TRANSY(g_downx, g_downy, g_downz, XForm, Trans),
                    TRANSX(g_downx, g_upy, g_upz, XForm, Trans),
                    TRANSY(g_downx, g_upy, g_upz, XForm, Trans),
                    AQUA);*/

      	Dest.Rect( (int)TRANSX(g_downx, g_downy, g_downz, XForm, Trans),
                    (int)TRANSY(g_downx, g_downy, g_downz, XForm, Trans),
                    (int)TRANSX(g_upx, g_upy, g_upz, XForm, Trans),
                    (int)TRANSY(g_upx, g_upy, g_upz, XForm, Trans),
                    SELECTBOX);

      }

      if (g_MainAction == atCreateVertex)
      {
         Dest.Line( (int)TRANSX(g_downx+100, g_downy, g_downz, XForm, Trans),
                    (int)TRANSY(g_downx+100, g_downy, g_downz, XForm, Trans),
                    (int)TRANSX(g_downx-100, g_downy, g_downz, XForm, Trans),
                    (int)TRANSY(g_downx-100, g_downy, g_downz, XForm, Trans),
                    SELECTBOX);
         Dest.Line( (int)TRANSX(g_downx, g_downy, g_downz+100, XForm, Trans),
                    (int)TRANSY(g_downx, g_downy, g_downz+100, XForm, Trans),
                    (int)TRANSX(g_downx, g_downy, g_downz-100, XForm, Trans),
                    (int)TRANSY(g_downx, g_downy, g_downz-100, XForm, Trans),
                    SELECTBOX);
         Dest.Line( (int)TRANSX(g_downx, g_downy+100, g_downz, XForm, Trans),
                    (int)TRANSY(g_downx, g_downy+100, g_downz, XForm, Trans),
                    (int)TRANSX(g_downx, g_downy-100, g_downz, XForm, Trans),
                    (int)TRANSY(g_downx, g_downy-100, g_downz, XForm, Trans),
                    SELECTBOX);
      }

      if (g_MainAction == atBuildingFace1 && View == g_BuildFaceView)
      {
      	Dest.Line( (int)TRANSX(g_downx, g_downy, g_downz, XForm, Trans),
                    (int)TRANSY(g_downx, g_downy, g_downz, XForm, Trans),
                    (int)ScrVerts[CreateFace[0]].x,
                    (int)ScrVerts[CreateFace[0]].y,
                    SELECTBOX);
      }
      if (g_MainAction == atBuildingFace2 && View == g_BuildFaceView)
      {
      	Dest.Line( (int)TRANSX(g_downx, g_downy, g_downz, XForm, Trans),
                    (int)TRANSY(g_downx, g_downy, g_downz, XForm, Trans),
                    ScrVerts[CreateFace[0]].x,
                    ScrVerts[CreateFace[0]].y,
                    SELECTBOX);
      	Dest.Line( (int)TRANSX(g_downx, g_downy, g_downz, XForm, Trans),
                    (int)TRANSY(g_downx, g_downy, g_downz, XForm, Trans),
                    ScrVerts[CreateFace[1]].x,
                    ScrVerts[CreateFace[1]].y,
                    SELECTBOX);
      	Dest.Line( ScrVerts[CreateFace[0]].x,
                    ScrVerts[CreateFace[0]].y,
                    ScrVerts[CreateFace[1]].x,
                    ScrVerts[CreateFace[1]].y,
                    SELECTBOX);
      }

      // bone drawing

//      if (MainForm->ToolPageControl->ActivePage == MainForm->BonesSheet)
      {
         if (M->Bones.Root)
        {
           TBone *B;
           B = M->Bones.Root;
            while (B)
            {
               DrawBoneTree(B, Dest,XForm, Trans);
               B = B->NextSib;
            }
         }
      }

/*
	// Axis Drawing
      if (AnythingSelected())
      {
      	Vector3 A,B,C,O;
         Matrix3 M;
         int OX,OY;

      	O = SelCentre = GetSelectionCentre(CurMdl, MainSelectMode);
         M = CurAxis;
         M *= 35.0/g_Zoom;
         M.SplitVert(A,B,C);

         A+=O;
         B+=O;
         C+=O;

         OX = TRANSX(O.x,O.y,O.z,XForm,Trans);
         OY = TRANSY(O.x,O.y,O.z,XForm,Trans);
         Dest.Line( OX,OY,
                    TRANSX(A.x,A.y,A.z,XForm,Trans),
                    TRANSY(A.x,A.y,A.z,XForm,Trans),
         			  MainForm->XBut->Down ? RED : LIGHTGRAY);
         Dest.Line( OX,OY,
                    TRANSX(B.x,B.y,B.z,XForm,Trans),
                    TRANSY(B.x,B.y,B.z,XForm,Trans),
         			  MainForm->YBut->Down ? RED : LIGHTGRAY);
         Dest.Line( OX,OY,
                    TRANSX(C.x,C.y,C.z,XForm,Trans),
                    TRANSY(C.x,C.y,C.z,XForm,Trans),
                    MainForm->ZBut->Down ? RED : LIGHTGRAY);
      }
   */
   }


// Do the banner

//	Dest.FilledRect(0,0, Dest.VisibleWidth-1, 16,0);

/*	{
   	static int i;
   	HDC		Ban;
      HBITMAP  OldBitmap;
      HFONT		OldFont;
      HBRUSH	OldBrush;
      HPEN		OldPen;

      MainForm->FontBox->Canvas->Font->Color = rand();
      MainForm->FontBox->Canvas->Brush->Style = bsSolid;
      MainForm->FontBox->Canvas->Brush->Color = i;
      MainForm->FontBox->Canvas->Pen->Color = clWhite;

      Ban = CreateCompatibleDC(MainForm->Canvas->Handle);

      OldBitmap = SelectObject(Ban, Dest.Handle);
		OldFont = SelectObject(Ban, MainForm->FontBox->Canvas->Font->Handle);
		OldBrush = SelectObject(Ban, MainForm->FontBox->Canvas->Brush->Handle);
		OldPen = SelectObject(Ban, MainForm->FontBox->Canvas->Pen->Handle);

      TextOut(Ban,1,1,"Testing", 7);

      SelectObject(Ban, OldPen);
      SelectObject(Ban, OldBrush);
      SelectObject(Ban, OldFont);
      SelectObject(Ban, OldBitmap);

   }*/


   Dest.Rect(0,0,Dest.VisibleWidth-1,Dest.VisibleHeight-1,BLACK);

   delete[] ScrVerts;
}


Vector3 GetSelectionCentre(TCompleteModel &M, SelectType S)
{
   int 		ns=0;
   Vector3	V;

   V.x = 0;
   V.y = 0;
   V.z = 0;

   if (S == stVertex)
      for (int n=0;n<M.Head.num_xyz;n++)
	      if (M.Verts[n].Visible)
         if (M.Verts[n].Selected)
         {
            V.x += M.Verts[n].x;
            V.y += M.Verts[n].y;
            V.z += M.Verts[n].z;
            ns++;
         }

   if (S == stTriangle)
   {
      bool  *vertsel;
      int   n;

      vertsel = new bool[M.Head.num_xyz];

      for (n=0;n<M.Head.num_xyz;n++)
         vertsel[n] = false;

      for (n=0;n<M.Head.num_tris;n++)
      if (M.Tris[n].Visible)
         if (M.Tris[n].Selected)
         {
            vertsel[ M.Tris[n].Vertices[0] ] = true;
            vertsel[ M.Tris[n].Vertices[1] ] = true;
            vertsel[ M.Tris[n].Vertices[2] ] = true;
         }

      for (n=0;n<M.Head.num_xyz;n++)
      if (M.Verts[n].Visible)
         if (vertsel[n])
         {
            V.x += M.Verts[n].x;
            V.y += M.Verts[n].y;
            V.z += M.Verts[n].z;
            ns++;
         }
      delete[] vertsel;
   }
   if (ns>0)
   {
		V.x /= ns;
		V.y /= ns;
		V.z /= ns;
   }

   return V;
}

int DoXFormVertFaceBone( CXFormItemInfo *Info)
{
   if (!Info)
      return 0;

	TRealVertex    *V;
   bool           *Sel;
   ActionType     A;
   ViewType       View;
   int            ox, X, oy, Y;
   int            vheight, vwidth, vheight2, vwidth2;
                     
   CheckVisibleSelection();

   V = Info->m_Verts;
   Sel = Info->m_Sel;
   A = Info->m_Action;
   View = Info->m_View;

   ox = Info->m_ox;
   oy = Info->m_oy;
   X = Info->m_X;
   Y = Info->m_Y;

   vwidth = Info->m_vwidth;
   vheight = Info->m_vheight;
   vwidth2 = Info->m_vwidth2;
   vheight2 = Info->m_vheight2;

   if (g_MainSelectMode == stVertex || g_MainSelectMode == stTriangle)
   {
      if (g_MainAction == atMove)
      {
         float altx,alty;

         Vector3 T,N;

         if (Info->m_ConstrainX)
            altx = (-ox+X)/g_Zoom;
         else altx = 0;
         if (Info->m_ConstrainY)
            alty = (oy-Y)/g_Zoom;
         else alty = 0;

         T.x = altx;
         T.y = alty;
         T.z = 0;

         CurAxis.Transform(T,N);

         for (int i=0;i<Info->m_Mdl->Head.num_xyz;i++)
         {
            if (Sel[i])
            {
               V[i].x+=N.x;
               V[i].y+=N.y;
               V[i].z+=N.z;
            }
         }

         g_diffx += N.x;
         g_diffy += N.y;
         g_diffz += N.z;

         CalcAllNormals();
         return 1;
      }
      if (g_MainAction == atRotate)
      {
         float ang,nx,ny,nz;

         ang = (float)2.0*(oy-Y)/vheight;
        	float cosa = (float)cos(ang),sina = (float)sin(ang);

         if (g_CurView == vtXY)
         {
            for (int i = 0;i<Info->m_Mdl->Head.num_xyz;i++)
            if (Info->m_Mdl->Verts[i].Visible)
               if (Sel[i])
               {
                  nx = (float)(V[i].x - g_downx)*cosa + (V[i].y - g_downy)*sina;
                  ny = (float)(V[i].y - g_downy)*cosa - (V[i].x - g_downx)*sina;
                  nx+=g_downx;
                  ny+=g_downy;

                  V[i].x = nx;
                  V[i].y = ny;
               }

            g_diffx += ang;
         }
         if (g_CurView == vtXZ)
         {
            for (int i = 0;i<Info->m_Mdl->Head.num_xyz;i++)
            if (Info->m_Mdl->Verts[i].Visible)
               if (Sel[i])
               {
                  nx = (float)(V[i].x - g_downx)*cosa + (V[i].z - g_downz)*sina;
                  nz = (float)(V[i].z - g_downz)*cosa - (V[i].x - g_downx)*sina;
                  nx+=g_downx;
                  nz+=g_downz;

                  V[i].x = nx;
                  V[i].z = nz;
               }

            g_diffy += ang;
         }
         if (g_CurView == vtZY)
         {
            for (int i = 0;i<Info->m_Mdl->Head.num_xyz;i++)
            if (Info->m_Mdl->Verts[i].Visible)
               if (Sel[i])
               {
                  nz = (float)(V[i].z - g_downz)*cosa + (V[i].y - g_downy)*sina;
                  ny = (float)(V[i].y - g_downy)*cosa - (V[i].z - g_downz)*sina;
                  nz+=g_downz;
                  ny+=g_downy;

                  V[i].z = nz;
                  V[i].y = ny;
               }

            g_diffz += ang;
         }
         CalcAllNormals();
         return 1;
      }

      if (g_MainAction == atScale)
      {
         float fact,nx,ny,nz;

         fact = (float)5*(oy-Y)/vheight + 1;
         if (fact<0) fact = 1/(-fact);

         if (Info->m_ConstrainX)
         	g_diffy *= fact;
         if (Info->m_ConstrainY)
         	g_diffx *= fact;
         if (Info->m_ConstrainZ)
         	g_diffz *= fact;

         for (int i = 0;i<Info->m_Mdl->Head.num_xyz;i++)
         if (Info->m_Mdl->Verts[i].Visible)
            if (Sel[i])
            {
               nx = (float)(V[i].x - g_downx)*fact;
               ny = (float)(V[i].y - g_downy)*fact;
               nz = (float)(V[i].z - g_downz)*fact;
               nx+=g_downx;
               ny+=g_downy;
               nz+=g_downz;

               if (Info->m_ConstrainX)
                  V[i].y = ny;
               if (Info->m_ConstrainY)
                  V[i].x = nx;
               if (Info->m_ConstrainZ)
                  V[i].z = nz;
            }
         CalcAllNormals();
         return 1;
      }
	}
   if (g_MainSelectMode == stBone)
   {
   	if (g_MainAction == atMove)
      {
         float altx,alty;

         Vector3 T,N;

         if (Info->m_ConstrainX)
            altx = (-ox+X)/g_Zoom;
         else altx = 0;
         if (Info->m_ConstrainY)
            alty = (oy-Y)/g_Zoom;
         else alty = 0;

         T.x = altx;
         T.y = alty;

         CurAxis.Transform(T,N);

         g_DeltaX = N.x;
         g_DeltaY = N.y;
         g_DeltaZ = N.z;

         Info->m_Mdl->Bones.ApplyFuncToSelected(MoveBoneFunc);
			Info->m_Mdl->Bones.ResetAllBones();
         Info->m_Mdl->Bones.TransformAll(Info->m_Mdl->Frames, Info->m_Mdl->Head.num_frames, Info->m_Mdl->Head.num_xyz, g_CurFrame);

         CalcAllNormals();
         return 1;
      }
   	if (g_MainAction == atRotate)
      {
         float ang;

         ang = (float)2.0*(oy-Y)/vheight;

         if (g_CurView == vtXY)
         {
         	g_DeltaX = ang;
            g_DeltaZ = 0;
            g_DeltaY = 0;
         }
         if (g_CurView == vtXZ)
         {
         	g_DeltaY = ang;
            g_DeltaZ = 0;
            g_DeltaX = 0;
         }
         if (g_CurView == vtZY)
         {
         	g_DeltaZ = ang;
            g_DeltaX = 0;
            g_DeltaY = 0;
         }

         Info->m_Mdl->Bones.ApplyFuncToSelected(RotateBoneFunc);
			Info->m_Mdl->Bones.ResetAllBones();
         Info->m_Mdl->Bones.TransformAll(Info->m_Mdl->Frames, Info->m_Mdl->Head.num_frames, Info->m_Mdl->Head.num_xyz, g_CurFrame);
         CalcAllNormals();
         return 1;
      }
   	if (g_MainAction == atScale)
      {

      	return 1;
      }
   }

   return 0;
}

void CalcAllNormals()
{
   g_CurMdl.CalcNormals();
   if (g_pRef)
      g_pRef->CalcNormals();

}

void CalcAllNormals(int frameno)
{
   g_CurMdl.CalcNormals(CALCNORMS_ALL, frameno);
   if (g_pRef)
      g_pRef->CalcNormals(CALCNORMS_ALL, frameno);
}

void CalcSelectedNormals()
{
   g_CurMdl.CalcNormals(CALCNORMS_SELECTED);
}

bool SamePair(int a1, int b1, int a2, int b2)
{
	if ((a1 == a2 || a1 == b2) && (b1 == a2 || b1 == b2))
   	return true;
   else return false;
}

void CheckVisibleSelection()
{
   int i;

	for (i=0;i<g_CurMdl.Head.num_tris;i++)
   if (g_CurMdl.Tris[i].Visible==false)
   	g_CurMdl.Tris[i].Selected = false;
	for (i=0;i<g_CurMdl.Head.num_xyz;i++)
   if (g_CurMdl.Verts[i].Visible==false)
   	g_CurMdl.Verts[i].Selected = false;
}

void ExtrudeSelected(float D)
{
	CheckVisibleSelection();

	if (g_MainSelectMode != stTriangle)
   {
   	MessageBox(NULL, "Must be on Triangle mode","Error", MB_OK);
      return;
   }

	Vector3		dir,ofs;
   int      	num;
   short			*targvert;
   TExtrudeTri	*extTris;
   bool			*vertneeded;
   int         i,f;

//   TRealSkinVertex		*tempSkinVerts;
   TRealTriangle 			*tempTris;
   TRealVertex 			*tempVerts;
   TRealVertex				*tempFrames[MAX_FRAMES];

   int 			numnewverts, numnewtris, curtri;

	// copy frame buffer back into frames
   for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
   {
      g_CurMdl.Frames[g_CurFrame][n].x = g_CurMdl.Verts[n].x;
      g_CurMdl.Frames[g_CurFrame][n].y = g_CurMdl.Verts[n].y;
      g_CurMdl.Frames[g_CurFrame][n].z = g_CurMdl.Verts[n].z;
   }

   vertneeded = new bool[g_CurMdl.Head.num_xyz];
   extTris = new TExtrudeTri[g_CurMdl.Head.num_tris];
   targvert = new short[g_CurMdl.Head.num_xyz];

   dir[0] = 0;
   dir[1] = 0;
   dir[2] = 0;

   num = 0;
   numnewtris=0;
   for (i=0;i<g_CurMdl.Head.num_tris;i++)
     if (g_CurMdl.Tris[i].Visible)
   	if (g_CurMdl.Tris[i].Selected)
      {
      	dir[0] -= g_CurMdl.Tris[i].Normal.x;
      	dir[1] -= g_CurMdl.Tris[i].Normal.y;
      	dir[2] -= g_CurMdl.Tris[i].Normal.z;
         num++;
      }

   if (num == 0)
   {
   	MessageBox(NULL, "No Faces are Selected.","Error", MB_OK);
      return;
   }

   dir[0] /= num;
   dir[1] /= num;
   dir[2] /= num;

   dir.Normalize();

   ofs[0] = D*dir[0];
   ofs[1] = D*dir[1];
   ofs[2] = D*dir[2];

   for (i=0;i<g_CurMdl.Head.num_tris;i++)
   	if (g_CurMdl.Tris[i].Selected && g_CurMdl.Tris[i].Visible)
      {
         extTris[i].OnEdge[0] = true;
         extTris[i].OnEdge[1] = true;
         extTris[i].OnEdge[2] = true;
      }
      else
      {
         extTris[i].OnEdge[0] = false;
         extTris[i].OnEdge[1] = false;
         extTris[i].OnEdge[2] = false;
      }

   for (int t1=0;t1<g_CurMdl.Head.num_tris-1;t1++)
	   for (int t2=t1+1;t2<g_CurMdl.Head.num_tris;t2++)
      if (g_CurMdl.Tris[t1].Selected && g_CurMdl.Tris[t2].Selected)
         if (g_CurMdl.Tris[t1].Visible && g_CurMdl.Tris[t2].Visible)
         {
            if (SamePair( g_CurMdl.Tris[t1].Vertices[0], g_CurMdl.Tris[t1].Vertices[1],
                          g_CurMdl.Tris[t2].Vertices[0], g_CurMdl.Tris[t2].Vertices[1]))
            {
               extTris[t1].OnEdge[0] = false;
               extTris[t2].OnEdge[0] = false;
            }
            if (SamePair( g_CurMdl.Tris[t1].Vertices[0], g_CurMdl.Tris[t1].Vertices[1],
                          g_CurMdl.Tris[t2].Vertices[1], g_CurMdl.Tris[t2].Vertices[2]))
            {
               extTris[t1].OnEdge[0] = false;
               extTris[t2].OnEdge[1] = false;
            }
            if (SamePair( g_CurMdl.Tris[t1].Vertices[0], g_CurMdl.Tris[t1].Vertices[1],
                          g_CurMdl.Tris[t2].Vertices[2], g_CurMdl.Tris[t2].Vertices[0]))
            {
               extTris[t1].OnEdge[0] = false;
               extTris[t2].OnEdge[2] = false;
            }
            if (SamePair( g_CurMdl.Tris[t1].Vertices[1], g_CurMdl.Tris[t1].Vertices[2],
                          g_CurMdl.Tris[t2].Vertices[0], g_CurMdl.Tris[t2].Vertices[1]))
            {
               extTris[t1].OnEdge[1] = false;
               extTris[t2].OnEdge[0] = false;
            }
            if (SamePair( g_CurMdl.Tris[t1].Vertices[1], g_CurMdl.Tris[t1].Vertices[2],
                          g_CurMdl.Tris[t2].Vertices[1], g_CurMdl.Tris[t2].Vertices[2]))
            {
               extTris[t1].OnEdge[1] = false;
               extTris[t2].OnEdge[1] = false;
            }
            if (SamePair( g_CurMdl.Tris[t1].Vertices[1], g_CurMdl.Tris[t1].Vertices[2],
                          g_CurMdl.Tris[t2].Vertices[2], g_CurMdl.Tris[t2].Vertices[0]))
            {
               extTris[t1].OnEdge[1] = false;
               extTris[t2].OnEdge[2] = false;
            }
            if (SamePair( g_CurMdl.Tris[t1].Vertices[2], g_CurMdl.Tris[t1].Vertices[0],
                          g_CurMdl.Tris[t2].Vertices[0], g_CurMdl.Tris[t2].Vertices[1]))
            {
               extTris[t1].OnEdge[2] = false;
               extTris[t2].OnEdge[0] = false;
            }
            if (SamePair( g_CurMdl.Tris[t1].Vertices[2], g_CurMdl.Tris[t1].Vertices[0],
                          g_CurMdl.Tris[t2].Vertices[1], g_CurMdl.Tris[t2].Vertices[2]))
            {
               extTris[t1].OnEdge[2] = false;
               extTris[t2].OnEdge[1] = false;
            }
            if (SamePair( g_CurMdl.Tris[t1].Vertices[2], g_CurMdl.Tris[t1].Vertices[0],
                          g_CurMdl.Tris[t2].Vertices[2], g_CurMdl.Tris[t2].Vertices[0]))
            {
               extTris[t1].OnEdge[2] = false;
               extTris[t2].OnEdge[2] = false;
            }
         }

   for (i=0;i<g_CurMdl.Head.num_xyz;i++)
   	vertneeded[i] = false;

   for (i=0;i<g_CurMdl.Head.num_tris;i++)
	   if (g_CurMdl.Tris[i].Visible)
         if (g_CurMdl.Tris[i].Selected)
         {
            vertneeded[ g_CurMdl.Tris[i].Vertices[0]] = true;
            vertneeded[ g_CurMdl.Tris[i].Vertices[1]] = true;
            vertneeded[ g_CurMdl.Tris[i].Vertices[2]] = true;
         }

   numnewverts = 0;
   for (i=0;i<g_CurMdl.Head.num_xyz;i++)
   	if (vertneeded[i])
      {
      	targvert[i] = (short)(numnewverts + g_CurMdl.Head.num_xyz);
      	numnewverts++;
      }
      else targvert[i] = 0;

   for (i=0;i<g_CurMdl.Head.num_tris;i++)
   if (g_CurMdl.Tris[i].Selected)
   {
   	if (extTris[i].OnEdge[0])
      	numnewtris +=2;
   	if (extTris[i].OnEdge[1])
      	numnewtris +=2;
   	if (extTris[i].OnEdge[2])
      	numnewtris +=2;
	}

//   tempSkinVerts = new TRealSkinVertex[g_CurMdl.Head.num_st + numnewverts];
//   memcpy(tempSkinVerts, g_CurMdl.SkinVerts, sizeof(TRealSkinVertex)*g_CurMdl.Head.num_st);
   tempTris = new TRealTriangle[g_CurMdl.Head.num_tris + numnewtris];
   memcpy(tempTris, g_CurMdl.Tris, sizeof(TRealTriangle)*g_CurMdl.Head.num_tris);
   tempVerts = new TRealVertex[g_CurMdl.Head.num_xyz + numnewverts];
   memcpy(tempVerts, g_CurMdl.Verts, sizeof(TRealVertex)*g_CurMdl.Head.num_xyz);

   for (f=0;f<g_CurMdl.Head.num_frames;f++)
   {
	   tempFrames[f] = new TRealVertex[g_CurMdl.Head.num_xyz + numnewverts];
      memcpy(tempFrames[f], g_CurMdl.Frames[f], sizeof(TRealVertex)*g_CurMdl.Head.num_xyz);
   }

	for (i=0;i<g_CurMdl.Head.num_xyz;i++)
   	if (vertneeded[i])
      {
      	tempVerts[ targvert[i] ].x = g_CurMdl.Verts[i].x + ofs[0];
      	tempVerts[ targvert[i] ].y = g_CurMdl.Verts[i].y + ofs[1];
      	tempVerts[ targvert[i] ].z = g_CurMdl.Verts[i].z + ofs[2];
      }

   for (f=0;f<g_CurMdl.Head.num_frames;f++)
   {
      
   	dir[0] = 0;
      dir[1] = 0;
      dir[2] = 0;

      CalcAllNormals(f);

      for (i=0;i<g_CurMdl.Head.num_tris;i++)
         if (g_CurMdl.Tris[i].Selected)
         {
            dir[0] -= g_CurMdl.Tris[i].Normal.x;
            dir[1] -= g_CurMdl.Tris[i].Normal.y;
            dir[2] -= g_CurMdl.Tris[i].Normal.z;
         }

      dir[0] /= num;
      dir[1] /= num;
      dir[2] /= num;

      dir.Normalize();

      ofs[0] = D*dir[0];
      ofs[1] = D*dir[1];
      ofs[2] = D*dir[2];

      for (i=0;i<g_CurMdl.Head.num_xyz;i++)
         if (vertneeded[i])
         {
            tempFrames[f][ targvert[i] ].x = g_CurMdl.Frames[f][i].x + ofs[0];
            tempFrames[f][ targvert[i] ].y = g_CurMdl.Frames[f][i].y + ofs[1];
            tempFrames[f][ targvert[i] ].z = g_CurMdl.Frames[f][i].z + ofs[2];
         }
   }

	for (i=0;i<g_CurMdl.Head.num_tris;i++)
   	if (g_CurMdl.Tris[i].Selected)
      {
      	tempTris[i].Vertices[0] = (short)targvert[g_CurMdl.Tris[i].Vertices[0]];
      	tempTris[i].Vertices[1] = targvert[g_CurMdl.Tris[i].Vertices[1]];
      	tempTris[i].Vertices[2] = targvert[g_CurMdl.Tris[i].Vertices[2]];
      }


   curtri = g_CurMdl.Head.num_tris;
	for (i=0;i<g_CurMdl.Head.num_tris;i++)
   {
   	if (extTris[i].OnEdge[0])
      {
			tempTris[curtri].Vertices[0] = g_CurMdl.Tris[i].Vertices[0];
			tempTris[curtri].Vertices[1] = g_CurMdl.Tris[i].Vertices[1];
			tempTris[curtri].Vertices[2] = targvert[g_CurMdl.Tris[i].Vertices[1]];
      	curtri++;

			tempTris[curtri].Vertices[0] = targvert[g_CurMdl.Tris[i].Vertices[1]];
			tempTris[curtri].Vertices[1] = targvert[g_CurMdl.Tris[i].Vertices[0]];
			tempTris[curtri].Vertices[2] = g_CurMdl.Tris[i].Vertices[0];
      	curtri++;
      }
   	if (extTris[i].OnEdge[1])
      {
			tempTris[curtri].Vertices[1] = g_CurMdl.Tris[i].Vertices[1];
			tempTris[curtri].Vertices[2] = g_CurMdl.Tris[i].Vertices[2];
			tempTris[curtri].Vertices[0] = targvert[g_CurMdl.Tris[i].Vertices[2]];
      	curtri++;

			tempTris[curtri].Vertices[1] = targvert[g_CurMdl.Tris[i].Vertices[2]];
			tempTris[curtri].Vertices[2] = targvert[g_CurMdl.Tris[i].Vertices[1]];
			tempTris[curtri].Vertices[0] = g_CurMdl.Tris[i].Vertices[1];
      	curtri++;
      }
   	if (extTris[i].OnEdge[2])
      {
			tempTris[curtri].Vertices[2] = g_CurMdl.Tris[i].Vertices[2];
			tempTris[curtri].Vertices[0] = g_CurMdl.Tris[i].Vertices[0];
			tempTris[curtri].Vertices[1] = targvert[g_CurMdl.Tris[i].Vertices[0]];
      	curtri++;

			tempTris[curtri].Vertices[2] = targvert[g_CurMdl.Tris[i].Vertices[0]];
			tempTris[curtri].Vertices[0] = targvert[g_CurMdl.Tris[i].Vertices[2]];
			tempTris[curtri].Vertices[1] = g_CurMdl.Tris[i].Vertices[2];
      	curtri++;
      }
   }

   delete[] g_CurMdl.Tris;
   g_CurMdl.Tris = tempTris;

//	delete[] g_CurMdl.SkinVerts;
//   g_CurMdl.SkinVerts = tempSkinVerts;

   for (f=0;f<g_CurMdl.Head.num_frames;f++)
   {
   	delete[] g_CurMdl.Frames[f];
      g_CurMdl.Frames[f] = tempFrames[f];
   }

   delete[] g_CurMdl.Verts;
   g_CurMdl.Verts = tempVerts;

//   for (int i=0;i<g_CurMdl.Head.num_tris;i++)
//   	g_CurMdl.Tris[i].Selected = false;

   for (i=g_CurMdl.Head.num_tris;i<g_CurMdl.Head.num_tris+numnewtris;i++)
   	g_CurMdl.Tris[i].Selected = false;

//   for (int i=0;i<g_CurMdl.Head.num_st;i++)
//   {
//   	g_CurMdl.SkinVerts[targvert[i]].s = g_CurMdl.SkinVerts[i].s;
//   	g_CurMdl.SkinVerts[targvert[i]].t = g_CurMdl.SkinVerts[i].t;
//   }

   for (i=0;i<numnewtris;i++)
   {
   	g_CurMdl.Tris[i+g_CurMdl.Head.num_tris].SkinVerts[0] = 0;
   	g_CurMdl.Tris[i+g_CurMdl.Head.num_tris].SkinVerts[1] = 0;
   	g_CurMdl.Tris[i+g_CurMdl.Head.num_tris].SkinVerts[2] = 0;
   }


   g_CurMdl.Head.num_xyz += numnewverts;
   g_CurMdl.Head.num_tris += numnewtris;


   delete[] extTris;
   delete[] vertneeded;
   delete[] targvert;

	CalcAllNormals();
}

Matrix3 GetSelectionAxis()
{
	Matrix3 M;

	if (g_AxisType == atView)
   {
		if (g_CurView == vtXY)
      	M.Set( 0,-1,0,
         		 1,0,0,
                0,0,1);
		if (g_CurView == vtXZ)
      	M.Set( -1,0,0,
         		 0,0,1,
                0,1,0);
		if (g_CurView == vtZY)
      	M.Set( 0,0,-1,
         		 1,0,0,
                0,1,0);
      if (g_CurView == vtCamera)
      {
         Matrix3  Temp;
         M.MakeRotationMatrix((float)(RotX+M_PI), -RotY, 0);
         M.Inverse();
      }
   }

   if (g_AxisType == atLocal) // !!! must be fixed !!!
   {
      M.Identity();

      if (g_MainSelectMode == stTriangle)
      {
         int n, NumSel = 0;
         Vector3  N;

         N.Set(0,0,0);

         for (n=0;n<g_CurMdl.Head.num_tris;n++)
         {
            if (g_CurMdl.Tris[n].Selected)
            {
               N += g_CurMdl.Tris[n].Normal;
            }
         }
         if (NumSel == 0)
            return M;

         N.Normalize();
//         N /= (float)NumSel;
//         M
      }
   }

   if (g_AxisType == atWorld)
   {
		if (g_CurView == vtXY)
      	M.Set( 0,-1,0,
         		 1,0,0,
                0,0,-1);
		if (g_CurView == vtXZ)
      	M.Set( -1,0,0,
         		 0,0,1,
                0,-1,0);
		if (g_CurView == vtZY)
      	M.Set( 0,0,-1,
         		 1,0,0,
                0,-1,0);
   }

   return M;
}

Matrix3 GetViewAxis()
{
	Matrix3 M;

	if (g_CurView == vtXY)
      M.Set( 0,-1,0,
         	 1,0,0,
             0,0,1);
	if (g_CurView == vtXZ)
      M.Set( -1,0,0,
         	 0,0,1,
             0,1,0);
	if (g_CurView == vtZY)
      M.Set( 0,0,-1,
         	 1,0,0,
             0,1,0);
   if (g_CurView == vtCamera)
   {
      Matrix3  Temp;
      M.MakeRotationMatrix((float)(RotX+M_PI), -RotY, 0);
      M.Inverse();
   }
   return M;
}

Matrix3 GetViewAxis(ViewType View)
{
	Matrix3 M;

	if (View == vtXY)
      M.Set( 0,-1,0,
         	 1,0,0,
             0,0,1);
	if (View == vtXZ)
      M.Set( -1,0,0,
         	 0,0,1,
             0,1,0);
	if (View == vtZY)
      M.Set( 0,0,-1,
         	 1,0,0,
             0,1,0);
   if (View == vtCamera)
   {
      Matrix3  Temp;
      M.MakeRotationMatrix((float)(RotX+M_PI), -RotY, 0);
      M.Inverse();
   }
   return M;
}


bool AnythingSelected()
{
	if (g_MainSelectMode == stVertex)
   {
   	for (int i=0;i<g_CurMdl.Head.num_xyz;i++)
			if (g_CurMdl.Verts[i].Selected)
         	return true;

      return false;
   }

   if (g_MainSelectMode == stTriangle)
   {
   	for (int i=0;i<g_CurMdl.Head.num_tris;i++)
			if (g_CurMdl.Tris[i].Selected)
         	return true;

      return false;
   }

   if (g_MainSelectMode == stBone)
   {
		return g_CurMdl.Bones.AnythingSelected();
   }
   return false;
}

void SetParentForBone(TBone *Bone)
{
	Bone->SetParent(TempBone);
   if (TempBone)
	   Bone->Selected = false;
}

void RotateBoneFunc(TBone *Bone)
{
	Bone->RotX[g_CurFrame] += g_DeltaX;
	Bone->RotY[g_CurFrame] += g_DeltaY;
	Bone->RotZ[g_CurFrame] += g_DeltaZ;
}

void ScaleBoneFunc(TBone *Bone)
{
	Bone->ScaleX[g_CurFrame] += g_DeltaX;
	Bone->ScaleY[g_CurFrame] += g_DeltaY;
	Bone->ScaleZ[g_CurFrame] += g_DeltaZ;
}

void MoveBoneFunc(TBone *Bone)
{
	Bone->OriginalPivot.x += g_DeltaX;
	Bone->OriginalPivot.y += g_DeltaY;
	Bone->OriginalPivot.z += g_DeltaZ;
}

void MakeLogicalPalette()
{
	for (int i=0;i<256;i++)
   {
   	EntryQuakePal[i].peRed = Palette[i][0];
   	EntryQuakePal[i].peGreen = Palette[i][1];
   	EntryQuakePal[i].peBlue = Palette[i][2];
   	EntryQuakePal[i].peFlags = PC_NOCOLLAPSE;
   }

   LogQuakePal = (LOGPALETTE*) new char[1028];

	memcpy(LogQuakePal + 4, EntryQuakePal, 1024);

   LogQuakePal->palVersion = 0x300;
   LogQuakePal->palNumEntries = 256;

	HQuakePal = CreatePalette(LogQuakePal);

	if (HQuakePal == NULL)
   	MessageBox(NULL, "Could not make Palette","Error",MB_OK);
}


void Draw3DRaised(CDC *C, int x, int y, int x1, int y1)
{
/*   C->Pen->Style = psClear;
   C->Brush->Color = clBtnFace;
   C->Rectangle(x+1,y,x1-1, y1-1);

   C->MoveTo(x, y);
   C->Pen->Style = psSolid;

   C->Pen->Color = clBtnHighlight;
   C->LineTo(x1, y);
   C->Pen->Color = cl3DDkShadow;
   C->LineTo(x1, y1);
   C->LineTo(x, y1);
   C->Pen->Color = clBtnHighlight;
   C->LineTo(x, y);*/
}

void Draw3DSunken(CDC *C, int x, int y, int x1, int y1)
{
   /*
   C->Pen->Style = psClear;
   C->Brush->Color = clBtnFace;
   C->Rectangle(x+1,y,x1-1, y1-1);

   C->MoveTo(x, y);
   C->Pen->Style = psSolid;

   C->Pen->Color = cl3DDkShadow;
   C->LineTo(x1, y);
   C->Pen->Color = clBtnHighlight;
   C->LineTo(x1, y1);
   C->LineTo(x, y1);
   C->Pen->Color = cl3DDkShadow;
   C->LineTo(x, y);*/
}

/*
bool ControlCheckChildren(TWinControl *Owner, TControl *Child)
{
	TWinControl *t;
	for (int i=0;i<Owner->ControlCount;i++)
   {
   	t = dynamic_cast<TWinControl*>(Owner->Controls[i]);
      if (t!=NULL)
   	if ( ControlCheckChildren(t, Child) )
      	return true;

	   if (Owner->Controls[i] == Child)
   		return true;
   }

   if (Owner == Child)
   	return true;
	else
   	return false;
}

bool ControlHasChild(TWinControl *Owner, TControl *Child)
{
	TWinControl *t;
	for (int i=0;i<Owner->ControlCount;i++)
   {
   	t = dynamic_cast<TWinControl*>(Owner->Controls[i]);
      if (t!=NULL)
   	if ( ControlCheckChildren(t, Child) )
      	return true;
   }

   return false;
}
*/

float FaceDotProd3D(TCompleteModel &M, int i,float cosa1,float sina1,float cosa2,float sina2)
{
   float tempy,nx,ny,nz;
   Vector3	A,B;


   nx = (-M.Tris[i].Normal.x)*cosa1 + (-M.Tris[i].Normal.y)*sina1;
   tempy = (-M.Tris[i].Normal.y)*cosa1 -
            (-M.Tris[i].Normal.x)*sina1;

   nz = (M.Tris[i].Normal.z)*cosa2 + (tempy)*sina2;
   ny = tempy*cosa2 - (M.Tris[i].Normal.z)*sina2;

   A[0] = nx;
   A[1] = ny;
   A[2] = nz;

   nx = (-M.Tris[i].Centre.x+PanX)*cosa1 + (-M.Tris[i].Centre.y+PanY)*sina1;
   tempy = (-M.Tris[i].Centre.y+PanY)*cosa1 - (-M.Tris[i].Centre.x+PanX)*sina1;

   nz = (M.Tris[i].Centre.z+PanZ)*cosa2 + (tempy)*sina2;
   ny = tempy*cosa2 - (M.Tris[i].Centre.z+PanZ)*sina2;

   nz+=Dist;

   B[0] = nx;
   B[1] = ny;
   B[2] = nz;

   return A*B;
}


void AddWndStyle(HWND hWnd, long NewStyle)
{
   long l = ::GetWindowLong(hWnd, GWL_STYLE);
   ::SetWindowLong(hWnd, GWL_STYLE, l | NewStyle);
}

int GetCWndWidth(const CWnd &Wnd)
{
   RECT  Rect;
   Wnd.GetWindowRect(&Rect);
   return Rect.right-Rect.left;
}

int GetCWndHeight(const CWnd &Wnd)
{
   RECT  Rect;
   Wnd.GetWindowRect(&Rect);
   return Rect.bottom-Rect.top;
}

bool IsSkinVertUsedByUnselected(int n)
{
   for (int t=0;t<g_CurMdl.Head.num_tris;t++)
   {
      if (g_CurMdl.Tris[t].SkinSelected == false)
      {
         if (g_CurMdl.Tris[t].SkinVerts[0] == n)
            return true;
         if (g_CurMdl.Tris[t].SkinVerts[1] == n)
            return true;
         if (g_CurMdl.Tris[t].SkinVerts[2] == n)
            return true;
      }
   }

   return false;
}

void CleanUpGlobals()
{
   TRACE("Cleaning Up Globals.");
   if (g_posx)
      delete[] g_posx;
   g_posx = NULL;

   if (g_posy)
      delete[] g_posy;
   g_posy = NULL;

   if (g_posz)
      delete[] g_posz;
   g_posz = NULL;

   if (g_pRef)
      delete g_pRef;
   g_pRef = NULL;

}


COLORREF RGBToColorRef(BYTE rgb[3])
{
   return RGB(rgb[0], rgb[1], rgb[2]);
}


// 16 bits in r5g5b6
WORD RGBto16(BYTE r, BYTE g, BYTE b)
{
   WORD res;

   int found=-1;

   for(int col=0;col<256;col++)
   {
      if (Palette[col][0] == r)
         if (Palette[col][1] == g)
            if (Palette[col][2] == b)
            {
               found = col;
               break;
            }
   }

   r = r >> 3;
   g = g >> 3;
   b = b >> 3;

   res = (r << 10) + (g << 5) + b;


   if (found != -1)
   {
      res = 32768 + found;
   }

   return res;
}

void Col16toRGB(WORD Col, BYTE &r,BYTE &g,BYTE &b)
{
   long i,j,k;

//|        |        |
// Nrrrrrgg gggbbbbb

   if (Col & 32768)
   {
      int col = Col & 255;
      r = Palette[col][0];
      g = Palette[col][1];
      b = Palette[col][2];
   }
   else
   {
      i = Col & 0xFC00;
      j = Col & 0x03E0;
      k = Col & 0x001F;

      i = i >> 10;
      j = j >> 5;

      r = (BYTE)(i << 3);
      g = (BYTE)j << 3;
      b = (BYTE)k << 3;
   }
}

int WrapMouseToScreen(HWND hWnd, int &x, int &y, int &ox, int &oy)
{
   int      targX, targY;
   bool     changed=false;
   POINT    C,S;
   int      ScreenWidth, ScreenHeight;
   
   ScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
   ScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
   
   C.x = x;
   C.y = y;
   S = C;
   
   ClientToScreen(hWnd, &S);
   
   targX = (int)(65535.0*(0.5+S.x)/ScreenWidth);
   targY = (int)(65535.0*(0.5+S.y)/ScreenHeight);
   
   if (S.x == ScreenWidth-1)
   {
      targX = (int)(2*(65535.0/ScreenWidth));
      ox += 1-ScreenWidth;
      x += 1-ScreenWidth;
      changed = true;
   }
   else
   {
      if (S.y == ScreenHeight-1)
      {
         targY = (int)(2*(65535.0/ScreenHeight));
         oy += 1-ScreenHeight;
         y += 1-ScreenHeight;
         changed = true;
      }
      else
      {
         if (S.x == 0)
         {
            targX = (int)(65535 - 2*(65535.0/ScreenWidth));
            ox -= 1-ScreenWidth;
            x -= 1-ScreenWidth;;
            changed = true;
         }
         else
         {
            if (S.y == 0)
            {
               targY = (int)(65535 - 2*(65535.0/ScreenHeight));
               oy -= 1-ScreenHeight;
               y -= 1-ScreenHeight;
               changed = true;
            }
         }
      }
   }
   
   if (changed)
   {
      mouse_event(MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE, targX,targY,0,0);
      return 1;
   }
   return 0;
}