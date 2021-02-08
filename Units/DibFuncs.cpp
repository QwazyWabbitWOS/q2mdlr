//---------------------------------------------------------------------------
#include "stdafx.h"

#include "DibFuncs.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "colors.h"

#include "matrix.h"




#define LINECLIPPING2D	1
//---------------------------------------------------------------------------

typedef unsigned char byte;

TDIBitmap8::TDIBitmap8()
{
	Enabled = true;
	
	Width = Height = 0;
	PInfo = NULL;
	PInfoHeader = NULL;
	PPalette = NULL;
	Data = NULL;
	ZBuffer = NULL;
	
	PInfo = (BITMAPINFO*) new unsigned char[ sizeof(BITMAPINFOHEADER) + 1024];
	PInfoHeader = (BITMAPINFOHEADER*) PInfo;
	PPalette = &PInfo->bmiColors[0];
	
	PInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	PInfoHeader->biWidth = 0;
	PInfoHeader->biHeight = 0;
	PInfoHeader->biPlanes = 1;
	PInfoHeader->biBitCount = 8;
	PInfoHeader->biCompression = BI_RGB;
	PInfoHeader->biSizeImage = 0;
	PInfoHeader->biXPelsPerMeter = 72;
	PInfoHeader->biYPelsPerMeter = 72;
	PInfoHeader->biClrUsed = 256;
	PInfoHeader->biClrImportant = 100;
	
	OMinX = OMinY = OMaxX = OMaxY = MinX = MinY = MaxX = MaxY = 0;
	
	Handle = NULL;
	OfsY = NULL;
}

TDIBitmap8::TDIBitmap8(TDIBitmap8 &Src)
{
	Enabled = true;
}

TDIBitmap8::~TDIBitmap8()
{
	if (Handle!=NULL)
		DeleteObject(Handle);
	
	if (PInfo!=NULL)
		delete[] PInfo;
	PInfo = NULL;
	PInfoHeader = NULL;
	PPalette = NULL;
	
	Width = Height = 0;
	
	if (OfsY != NULL)
		delete[] OfsY;
	
	if (ZBuffer != NULL)
		delete[] ZBuffer;
	ZBuffer = NULL;
}

void  TDIBitmap8::SetSize(int w, int h)
{
   if (w<0)
      w = 0;
   if (h<0)
      h= 0;

	VisibleWidth = w;
	VisibleHeight = h;
	if (PInfo == NULL) return;
	
	if (w%4 != 0)
		w = w - w%4 + 4;
	
	Width = w;
	Height = h;
	
	PInfoHeader->biWidth = w;
	PInfoHeader->biHeight = h;
	PInfoHeader->biSizeImage = w*h;
	PInfoHeader->biHeight = h;
	
	if (Handle != NULL)
		DeleteObject(Handle);
	if (ZBuffer != NULL)
		delete[] ZBuffer;
	ZBuffer = NULL;
	
	// Used to get top most active form, and use its hdc. Use desktop instead

	HWND	hDesktopWnd;
	HDC		hDesktopDC;

	hDesktopWnd = GetDesktopWindow();
	if (hDesktopWnd == NULL)	// GetDesktopWindow failed?!?
		return;
	hDesktopDC = GetDC(hDesktopWnd);
	if (hDesktopDC == NULL)		// Get DC failed
		return;

	Handle = CreateDIBSection(hDesktopDC, PInfo, DIB_PAL_COLORS, (void**)&Data, NULL, 0);

   ReleaseDC(hDesktopWnd, hDesktopDC);
	ZBuffer = new float[Width*Height];

	ReleaseDC(hDesktopWnd, hDesktopDC);
}

void  TDIBitmap8::DrawToDC(HDC DestDC, int x, int y)
{
	if (PInfo != NULL)
		SetDIBitsToDevice(DestDC, x,y, VisibleWidth, VisibleHeight,0,0,0,Height,Data,PInfo,DIB_RGB_COLORS);
}
void  TDIBitmap8::StretchToDC(HDC DestDC, int x, int y, int w, int h)
{
	if (PInfo != NULL)
		StretchDIBits(DestDC,x,y,w,h,0,0,VisibleWidth, VisibleHeight,Data, PInfo,DIB_RGB_COLORS, SRCCOPY);
}

void  TDIBitmap8::StretchToDC(HDC DestDC, int xdest, int ydest, int wdest, int hdest, int xsrc, int ysrc, int wsrc, int hsrc)
{
	if (PInfo != NULL)
		StretchDIBits(DestDC,xdest,ydest,wdest,hdest,xsrc,ysrc,wsrc, hsrc,Data, PInfo,DIB_RGB_COLORS, SRCCOPY);
}



/*void  TDIBitmap8::StretchRectToDC(HDC DestDC, int x, int y, int w, int h, int srcw, srch)
{
if (PInfo != NULL)
StretchDIBits(DestDC,x,y,w,h,0,0,srcwidth, srcheight,Data, PInfo,DIB_RGB_COLORS, SRCCOPY);
} */
void  TDIBitmap8::PutPixel(int x1, int y1, unsigned char col)
{
	if (x1<0 || y1<0 || x1>=VisibleWidth || y1>=VisibleHeight)
		return;
	
	int diff;
	
	diff = PInfoHeader->biSizeImage - y1*Width + x1 - Width;
	unsigned char *dst = Data + diff;
	*dst = col;
}

void  TDIBitmap8::Line(int x1, int y1, int x2, int y2, unsigned char col)
{
	int dx,dy,sx,sy, sofsx, sofsy;
	unsigned char *dst;
	int d,numpix;
	
	if (!Enabled)
		return;
	
	if ( x1<0 && x2<0)
		return;
	if ( x1>=VisibleWidth && x2>=VisibleWidth )
		return;
	if ( y1<0 && y2<0)
		return;
	if ( y1>=Height && y2>=Height )
		return;
	
	if (x2<x1)
	{
		int t;
		t = x2;
		x2 = x1;
		x1 = t;
		
		t = y2;
		y2 = y1;
		y1 = t;
	}
	
	
#ifdef LINECLIPPING2D
	{
		if (x1 == x2) // vertical line
		{
			if (y1<0)
				y1 = 0;
			if (y2<0)
				y2 = 0;
			if (y1>=Height)
				y1 = Height-1;
			if (y2>=Height)
				y2 = Height;
		}
		else
			if (y1 == y2) // horizontal line
			{
				if (x1<0)
					x1 = 0;
				if (x2<0)
					x2 = 0;
				if (x1>=VisibleWidth)
					x1 = VisibleWidth-1;
				if (x2>=VisibleWidth)
					x2 = VisibleWidth-1;
			}
			else
			{
				double m = (float)(y2-y1)/(float)(x2-x1);
				double c = (float)y1 - (float)m*x1;
				
				if (m==0)
				{
					m=0;
				}
				
				try
				{
					if (x1<0)
					{
						x1 = 0;
						y1 = (int)c;
					}
					if (x2<0)
					{
						x2 = 0;
						y2 = (int)c;
					}
					if (x1>=VisibleWidth)
					{
						x1 = VisibleWidth-1;
						y1 = (int)(m*x1+c);
					}
					if (x2>=VisibleWidth)
					{
						x2 = VisibleWidth-1;
						y2 = (int)(m*x2+c);
					}
					
					if (y1<0)
					{
						y1 = 0;
						x1 = (int)((y1-c)/m);
					}
					if (y2<0)
					{
						y2 = 0;
						x2 = int((y2-c)/m);
					}
					if (y1>=Height)
					{
						y1 = Height-1;
						x1 = (int)((y1-c)/m);
					}
					if (y2>=Height)
					{
						y2 = Height-1;
						x2 = (int)((y2-c)/m);
					}
				}
				catch(...)
				{
				}
			}
	}
	
#endif
	
	dx=x2-x1;
	dy=y2-y1;
	
	
	//	if (dx) sx=(dx<0) ? -1 : 1; else sx=0;
	if (dy) sy=(dy<0) ? -1 : 1; else sy=0;
	if (dx) sx=1; else sx=0;
	
	//	if (dx) sofsx=(dx<0) ? -1 : 1; else sofsx=0;
	if (dy) sofsy=(dy<0) ? Width : -Width; else sofsy=0;
	if (dx) sofsx=1; else sofsx=0;
	
	
	//	if (dx<0) dx=-dx;
	if (dy<0) dy=-dy;
	
	dst = Data + (PInfoHeader->biSizeImage - y1*Width + x1 - Width);
	
	if (dx==0 && dy==0) return;
	
	if (dx>dy)
	{
		d = dx >> 1;
		numpix=dx;
		while (numpix--)
		{
			if (x1>=0 && x1<VisibleWidth)
				if (y1>=0 && y1<Height)
					*dst = col;
				d+=dy;
				x1+=sx;
				dst += sofsx;
				if (d>dx)
				{
					d-=dx;
					y1+=sy;
					dst += sofsy;
				}
		}
	}
	else
	{
		d = dy >> 1;
		numpix=dy;
		while (numpix--)
		{
			if (x1>=0 && x1<VisibleWidth)
				if (y1>=0 && y1<Height)
					*dst = col;
				d+=dx;
				y1+=sy;
				dst += sofsy;
				if (d>dy)
				{
					d-=dy;
					x1+=sx;
					dst += sofsx;
				}
		}
	}
}

void  TDIBitmap8::LineXOR(int x1, int y1, int x2, int y2, unsigned char col)
{
	int dx,dy,sx,sy, sofsx, sofsy;
	unsigned char *dst;
	int d,numpix;
	
	dx=x2-x1;
	dy=y2-y1;
	
	if (!Enabled)
		return;
	if ( (x1<0 || x1>=VisibleWidth) && (x2<0 || x2>=VisibleWidth) )
		return;
	if ( (y1<0 || y1>=Height) && (y2<0 || y2>=Height) )
		return;
	
	if (dx) sx=(dx<0) ? -1 : 1; else sx=0;
	if (dy) sy=(dy<0) ? -1 : 1; else sy=0;
	
	if (dx) sofsx=(dx<0) ? -1 : 1; else sofsx=0;
	if (dy) sofsy=(dy<0) ? Width : -Width; else sofsy=0;
	
	
	if (dx<0) dx=-dx;
	if (dy<0) dy=-dy;
	
	dst = Data + PInfoHeader->biSizeImage - y1*Width + x1 - Width;
	
	if (dx==0 && dy==0) return;
	
	if (dx>dy)
	{
		d=dx/2;
		numpix=dx;
		while (numpix--)
		{
			if (x1 >=0 && x1 < Width && y1>=0 && y1<Height)
				*dst = (unsigned char)(*dst - col);
			d+=dy;
			x1+=sx;
			dst += sofsx;
			if (d>dx)
			{
				d-=dx;
				y1+=sy;
				dst += sofsy;
			}
		}
	}
	else
	{
		d=dy/2;
		numpix=dy;
		while (numpix--)
		{
			if (x1 >=0 && x1 < Width && y1>=0 && y1<Height)
				*dst = (unsigned char)(*dst - col);
			d+=dx;
			y1+=sy;
			dst += sofsy;
			if (d>dy)
			{
				d-=dy;
				x1+=sx;
				dst += sofsx;
			}
		}
	}
}

void  TDIBitmap8::SetPalette(unsigned char SrcPal[256][3], bool DoFilter)
{
	for (int i=0;i<256;i++)
	{
		try {
			PInfo->bmiColors[i].rgbRed = SrcPal[i][0];
			PInfo->bmiColors[i].rgbGreen = SrcPal[i][1];
			PInfo->bmiColors[i].rgbBlue = SrcPal[i][2];
		}
		catch (...)
		{
		}
		
	}
	
	if (DoFilter)
		for (int i=0;i<256;i++)
		{
			double      Best;
			int      BestIndex;
			Vector3  VBest,Compare,Cur,DistV;
			double   Res;
			
			Best = 0;
			
			for (int br=0;br<256;br++)
			{
				Cur.x = (float)SrcPal[i][0];
				Cur.y = (float)SrcPal[i][1];
				Cur.z = (float)SrcPal[i][2];
				
				Cur.x *= (float)(br/128.0);
				Cur.y *= (float)(br/128.0);
				Cur.z *= (float)(br/128.0);
				
				VBest = Cur;
				BestIndex = 0;
				Best = 100000;
				
				for (int c=0;c<256;c++)
				{
					Compare.x = (float)SrcPal[c][0];
					Compare.y = (float)SrcPal[c][1];
					Compare.z = (float)SrcPal[c][2];
					
					DistV.x = Cur.x - Compare.x;
					DistV.y = Cur.y - Compare.y;
					DistV.z = Cur.z - Compare.z;
					
					Res = DistV*DistV;
					if (Res<Best)
					{
						BestIndex = c;
						Best = Res;
					}
				}
				Bright[i][br] = BestIndex;
			}
		}
}
void  TDIBitmap8::Clear(unsigned char Col)
{
	if (!Enabled)
		return;
	if (PInfo == NULL)
		return;
	memset(Data, Col, PInfoHeader->biSizeImage);
}

void  TDIBitmap8::Rect(int x1, int y1, int x2, int y2, unsigned char col)
{
	if (!Enabled)
		return;
	Line(x1,y1,x2,y1,col);
	Line(x1,y1,x1,y2,col);
	Line(x2,y1,x2,y2,col);
	Line(x1,y2,x2,y2,col);
}

void  TDIBitmap8::FilledRect(int x1, int y1, int x2, int y2, unsigned char col)
{
	if (!Enabled)
		return;
	
	int n;

	for (n=y1;n<=y2;n++)
		Line(x1,n,x2,n,col);
	for (n=y2;n<=y1;n++)
		Line(x1,n,x2,n,col);
}

void  TDIBitmap8::Poly(POINT *P, int num, unsigned char col)
{
	if (!Enabled)
		return;
	
	int i;
	for (i=num-1;i>0;i--)
	{
		Line(P[i].x, P[i].y, P[i-1].x, P[i-1].y, col);
	}
	Line(P[i].x, P[i].y, P[num-1].x, P[num-1].y, col);
}

void MinMaxLine(int x1,int y1,int x2, int y2, int *mins, int *maxs, int Width, int Height)
{
	int dx,dy,sx,sy;
	int d,numpix;
	
	int *pmins, *pmaxs ;
	dx=x2-x1;
	dy=y2-y1;
	
	if (dx) sx=(dx<0) ? -1 : 1; else sx=0;
	if (dy) sy=(dy<0) ? -1 : 1; else sy=0;
	
	if (dx<0) dx=-dx;
	if (dy<0) dy=-dy;
	
	if (dx==0 && dy==0) return;
	
	
	pmins = &mins[y1];
	pmaxs = &maxs[y1];
	
	if (dx>dy)
	{
		d=dx >> 1;
		numpix=dx;
		while (numpix--)
		{
			if (y1>=0 && y1<Height)
			{
				if (x1<*pmins) *pmins = x1;
				if (x1>*pmaxs) *pmaxs = x1;
			}
			d+=dy;
			x1+=sx;
			if (d>dx)
			{
				d-=dx;
				y1+=sy;
				pmins += sy;
				pmaxs += sy;
			}
		}
	}
	else
	{
		d=dy >> 1;
		numpix=dy;
		while (numpix--)
		{
			if (y1>=0 && y1<Height)
			{
				if (x1<*pmins) *pmins = x1;
				if (x1>*pmaxs) *pmaxs = x1;
			}
			d+=dx;
			y1+=sy;
			pmins += sy;
			pmaxs += sy;
			if (d>dy)
			{
				d-=dy;
				x1+=sx;
			}
		}
	}
}


void  TDIBitmap8::FillPoly(POINT *P, int num, unsigned char col)
{
	if (!Enabled)
		return;
	int		i,n;
	int 	miny,maxy;
	int 	*minx,*maxx;
	int 	*pminx, *pmaxx;
	POINT	*pnt;
	
	unsigned char *start, *dst;
	
	/*	for (int i=num-1;i>=0;i--)
	{
   	if (pnt->x<0 || pnt->x>Width || pnt->y<0 || pnt->y>Height)
	return;
	pnt++;
}*/
	
	pnt = P;
	miny = pnt->y;
	maxy = pnt->y;
	
	for (i=num-1;i>0;i--)
	{
		pnt++;
		if (pnt->y < miny)
			miny = pnt->y;
		if (pnt->y > maxy)
			maxy = pnt->y;
	}
	
	if (miny<0) miny=0;
	if (miny>=Height) miny=Height-1;
	if (maxy<0) miny=0;
	if (maxy>=Height) maxy=Height-1;
	
	minx = new int[Height];
	maxx = new int[Height];
	
	pminx = minx;
	pmaxx = maxx;
	for (n=maxy-miny-1;n>=0;n--)
	{
		*pminx = Width;
		*pmaxx = 0;
		pminx++;
		pmaxx++;
	}
	
	pnt = P;
	for (i=num-1;i>0;i--)
	{
		MinMaxLine(pnt->x,pnt->y-miny,(pnt+1)->x,(pnt+1)->y-miny,minx,maxx,Width, Height);
		pnt++;
	}
	MinMaxLine(pnt->x,pnt->y-miny,P->x,P->y-miny,minx,maxx, Width, Height);
	
	pminx = minx;
	pmaxx = maxx;
	for (n=maxy-miny-1;n>=0;n--)
	{
		if (*pminx<0)
			*pminx = 0;
		if (*pmaxx>=Width)
			*pmaxx = Width-1;
		pminx++;
		pmaxx++;
	}
	
	pminx = minx;
	pmaxx = maxx;
	start = Data + PInfoHeader->biSizeImage - miny*Width - Width;
	
	for (int y=maxy-miny-1;y>=0;y--)
	{
		dst = start + *pminx;
		for (int x=*pmaxx - *pminx -1 ;x>=0;x--)
		{
			dst++;
			*dst = col;
		}
		pminx ++;
		pmaxx ++;
		start -= Width;
	}
	
	delete[] minx;
	delete[] maxx;
	
}


void  TDIBitmap8::SetTexSize(int a,int b)
{
	if (!Enabled)
		return;
	TexWidth = a;
	TexHeight = b;
	if (OfsY != NULL)
		delete[] OfsY;
	
	OfsY = new int[b];
	for (int i=b-1;i>=0;i--)
	{
		OfsY[i] = i*a;
	}
}

//------------------------------------------------------------------------
// draws a scan line starting at the given offset.
void  TDIBitmap8::DrawAffineScanline(float x1, float x2, float u1, float u2,
									 float v1, float v2, unsigned char *targ,
									 unsigned char* tPtr)
{
	if(x2 < x1) {
		float tmp = x1; x1 = x2; x2 = tmp;
		      tmp = u1; u1 = u2; u2 = tmp;
			  tmp = v1; v1 = v2; v2 = tmp;
	}
	
	float width = x2 - x1;
	float du, dv;
	int u, v, x;
	
	if(width) {
		du = (u2 - u1)/width;
		dv = (v2 - v1)/width;
	}
	
	targ += int(x1);
	for (x = int(x1);x<int(x2);x++)
	{
		u = int(u1);
		v = int(v1);
		
		if (x>=0 && x<Width)
			*targ = tPtr[OfsY[v] + u];
		
		u1 += du;
		v1 += dv;
		targ ++;
	}
}

void  TDIBitmap8::DrawTexTri(const TexTriangle &tri)
{
	if (!Enabled)
		return;
	
	int top = 0;                       // index to top vertex
	int a, b;                          // other 2 vertices
	int	i;
	
	// interpolants
	
	float dx_A, 			// change in sx with respect to sy
		dx_B,
		du_A,			// change in u with respect to sy
		du_B,
		dv_A,			// change in v with respect to sy
		dv_B;
	
	for(i = 1; i < 3; i++) // find top vertex
	{
		if(tri.v[i]->sy < tri.v[top]->sy) top = i;
	}
	
	a = top + 1;
	b = top - 1;
	if(a > 2) a = 0;
	if(b < 0) b = 2;
	
	int    y = (int)(tri.v[top]->sy);
	float  x1 = tri.v[top]->sx;
	float  x2 = x1;
	float  u1 = tri.v[top]->u;
	float  u2 = u1;
	float  v1 = tri.v[top]->v;
	float  v2 = v1;
	
	int height_A = (int)(tri.v[a]->sy - tri.v[top]->sy);
	int height_B = (int)(tri.v[b]->sy - tri.v[top]->sy);
	
	if(height_A) // avoid divide by zero
	{
		// calculate the interpolants
		dx_A = (tri.v[a]->sx - tri.v[top]->sx)/ height_A;
		du_A = (tri.v[a]->u  - tri.v[top]->u)/ height_A;
		dv_A = (tri.v[a]->v  - tri.v[top]->v)/ height_A;
	}
	
	if(height_B)// avoid divide by zero
	{
		// calculate the interpolants
		dx_B = (tri.v[b]->sx - tri.v[top]->sx)/ height_B;
		du_B = (tri.v[b]->u  - tri.v[top]->u)/ height_B;
		dv_B = (tri.v[b]->v  - tri.v[top]->v)/ height_B;
		
	}
	
	// start drawing
	unsigned char *targ = Data + PInfoHeader->biSizeImage - Width - y*Width;
	
	for(i = 2; i > 0;) {
		
		while(height_A && height_B)
		{
			if (y>=0 && y<Height)
				DrawAffineScanline(x1, x2, u1, u2, v1, v2, targ, tri.Tex);
			
			targ-=Width;
			y++;
			height_A--;
			height_B--;
			
			x1 += dx_A;                     // add the interpolants
			x2 += dx_B;
			u1 += du_A;
			u2 += du_B;
			v1 += dv_A;
			v2 += dv_B;
		}
		
		if(!height_A)
		{
			// new edge
			
			int na = a + 1;
			if(na > 2) na = 0;
			
			height_A = (int)(tri.v[na]->sy - tri.v[a]->sy);
			
			if(height_A)  // avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_A = (tri.v[na]->sx - tri.v[a]->sx) / height_A;
				du_A = (tri.v[na]->u  - tri.v[a]->u)  / height_A;
				dv_A = (tri.v[na]->v  - tri.v[a]->v)  / height_A;
				
			}
			
			x1 = tri.v[a]->sx;
			u1 = tri.v[a]->u;
			v1 = tri.v[a]->v;
			
			i--;                               // one less vertex
			a = na;
			
		} // end if
		
		if(!height_B)
		{
			
			// new edge
			
			int nb = b - 1;
			if(nb < 0) nb = 2;
			
			height_B = (int)(tri.v[nb]->sy - tri.v[b]->sy);
			
			if(height_B)// avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_B = (tri.v[nb]->sx - tri.v[b]->sx) / height_B;
				du_B = (tri.v[nb]->u  - tri.v[b]->u)  / height_B;
				dv_B = (tri.v[nb]->v  - tri.v[b]->v)  / height_B;
			}
			
			x2 = tri.v[b]->sx;
			u2 = tri.v[b]->u;
			v2 = tri.v[b]->v;
			
			i--;                               // one less vertex
			b = nb;
			
		} // end if
		
	} // end for loop
	
} // end function

void  TDIBitmap8::DrawAffineScanlineGreyscale(float x1, float x2, float u1, float u2,
											  float v1, float v2, unsigned char *targ,
											  unsigned char* tPtr, unsigned char StartCol)
{
	
	// we are assuming the texture is 64 units wide, it can be any
	// size though.
	
	if(x2 < x1) {                            // swap coordinates so we can draw left-to-right
		float tmp = x1; x1 = x2; x2 = tmp;
		      tmp = u1; u1 = u2; u2 = tmp;
			  tmp = v1; v1 = v2; v2 = tmp;
	}
	
	float width = x2 - x1;
	float du, dv;                            // more interpolants
	int u, v;
	
	if(width) {                              // avoid divide by zero
		du = (u2 - u1)/width;
		dv = (v2 - v1)/width;
		
	}
	
	for(int x = int(x1); x < int(x2); x++) {
		
		u = int(u1);
		v = int(v1);
		
		if (x>=0 && x<Width)
		{
			int col;
			col = PPalette[tPtr[OfsY[v] + u]].rgbRed +
				PPalette[tPtr[OfsY[v] + u]].rgbGreen +
				PPalette[tPtr[OfsY[v] + u]].rgbBlue;
			col = col/3;
			col = col >> GREYSHIFT;
			targ[x] = (unsigned char)(StartCol + col);
		}
		
		u1 += du;
		v1 += dv;
	}
}

void  TDIBitmap8::DrawTexTriGreyscale(const TexTriangle &tri, unsigned char StartCol)
{
	if (!Enabled)
		return;
	// Affine texture map a triangle
	
	int top = 0;                       // index to top vertex
	int a, b;                          // other 2 vertices
	int	i;
	
	// interpolants
	
	float dx_A, 			// change in sx with respect to sy
		dx_B,
		du_A,			// change in u with respect to sy
		du_B,
		dv_A,			// change in v with respect to sy
		dv_B;
	
	for(i = 1; i < 3; i++) // find top vertex
	{
		if(tri.v[i]->sy < tri.v[top]->sy) top = i;
	}
	
	a = top + 1;
	b = top - 1;
	if(a > 2) a = 0;
	if(b < 0) b = 2;
	
	int    y = (int)(tri.v[top]->sy);
	float x1 = tri.v[top]->sx;
	float x2 = x1;
	float u1 = tri.v[top]->u;
	float u2 = u1;
	float v1 = tri.v[top]->v;
	float v2 = v1;
	
	int height_A = (int)(tri.v[a]->sy - tri.v[top]->sy);
	int height_B = (int)(tri.v[b]->sy - tri.v[top]->sy);
	
	if(height_A) // avoid divide by zero
	{
		// calculate the interpolants
		dx_A = (tri.v[a]->sx - tri.v[top]->sx)/ height_A;
		du_A = (tri.v[a]->u  - tri.v[top]->u)/ height_A;
		dv_A = (tri.v[a]->v  - tri.v[top]->v)/ height_A;
	}
	
	if(height_B)// avoid divide by zero
	{
		
		// calculate the interpolants
		dx_B = (tri.v[b]->sx - tri.v[top]->sx)/ height_B;
		du_B = (tri.v[b]->u  - tri.v[top]->u)/ height_B;
		dv_B = (tri.v[b]->v  - tri.v[top]->v)/ height_B;
		
	}
	
	// start drawing
	unsigned char *targ = Data + PInfoHeader->biSizeImage - Width - y*Width;
	
	for(i = 2; i > 0;) {
		
		while(height_A && height_B)
		{
			if (y>=0 && y<Height)
				DrawAffineScanlineGreyscale(x1, x2, u1, u2, v1, v2, targ, tri.Tex, StartCol);
			
			targ-=Width;
			y++;
			height_A--;
			height_B--;
			
			x1 += dx_A;                     // add the interpolants
			x2 += dx_B;
			u1 += du_A;
			u2 += du_B;
			v1 += dv_A;
			v2 += dv_B;
		}
		
		if(!height_A)
		{
			// new edge
			
			int na = a + 1;
			if(na > 2) na = 0;
			
			height_A = (int)(tri.v[na]->sy - tri.v[a]->sy);
			
			if(height_A)  // avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_A = (tri.v[na]->sx - tri.v[a]->sx) / height_A;
				du_A = (tri.v[na]->u  - tri.v[a]->u)  / height_A;
				dv_A = (tri.v[na]->v  - tri.v[a]->v)  / height_A;
				
			}
			
			x1 = tri.v[a]->sx;
			u1 = tri.v[a]->u;
			v1 = tri.v[a]->v;
			
			i--;                               // one less vertex
			a = na;
			
		} // end if
		
		if(!height_B)
		{
			
			// new edge
			
			int nb = b - 1;
			if(nb < 0) nb = 2;
			
			height_B = (int)(tri.v[nb]->sy - tri.v[b]->sy);
			
			if(height_B)// avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_B = (tri.v[nb]->sx - tri.v[b]->sx) / height_B;
				du_B = (tri.v[nb]->u  - tri.v[b]->u)  / height_B;
				dv_B = (tri.v[nb]->v  - tri.v[b]->v)  / height_B;
			}
			
			x2 = tri.v[b]->sx;
			u2 = tri.v[b]->u;
			v2 = tri.v[b]->v;
			
			i--;                               // one less vertex
			b = nb;
			
		} // end if
		
	} // end for loop
	
} // end function

int TDIBitmap8::LoadFromBMP(CString FName)
{
	BITMAPFILEHEADER	bmfh;
	BITMAPINFOHEADER	bmih;
	//   RGBQUAD				aColors[];
	//   BYTE					aBitmapBits[];
	
	int					handle;
	
	handle = open((LPCTSTR)FName, O_BINARY | O_RDONLY);
	
	read(handle, &bmfh, sizeof(BITMAPFILEHEADER));
	read(handle, &bmih, sizeof(BITMAPINFOHEADER));
	
	if (bmfh.bfType != 19778) // BM
	{
//		MessageBox(NULL, "Not a Windows Bitmap File.","Error",MB_OK);
		close(handle);
		return 0;
	}
	if ((int)bmfh.bfSize != filelength(handle))
	{
//		Application->MessageBox("Not a Windows Bitmap File.","Error",MB_OK);
		close(handle);
		return 0;
	}
	
	if (bmfh.bfReserved1 != 0)
	{
//		Application->MessageBox("Not a Windows Bitmap File.","Error",MB_OK);
		close(handle);
		return 0;
	}
	
	if (bmfh.bfReserved2 != 0)
	{
//		Application->MessageBox("Not a Windows Bitmap File.","Error",MB_OK);
		close(handle);
		return 0;
	}
	
	if (bmih.biBitCount != 8)
	{
//		Application->MessageBox("Bitmap must be 256 colors.","Error", MB_OK);
		close(handle);
		return 0;
	}
	
	SetSize(bmih.biWidth, bmih.biHeight);
	
	lseek(handle, bmfh.bfOffBits, SEEK_SET); // skip past the palette
											 /*   if (bmih.biClrUsed == 0)
											 {
											 lseek(handle, 256*4, SEEK_CUR); // skip past the palette
											 }
											 else
											 lseek(handle, 4*bmih.biClrUsed, SEEK_CUR); // skip past the palette
	*/
	if (bmih.biCompression == BI_RGB)
	{
		unsigned char *src,*dest,*origsrc;
		
		src = new unsigned char[Width*Height];
		
		origsrc = src;
		read(handle, src, Width*Height);
		
		dest = Data + Width*Height - Width;
		for (int i=0;i<Height;i++)
		{
			memcpy(dest,src,Width);
			dest -= Width;
			src += Width;
			src += Width%4;
		}
		
		delete[] origsrc;
	}
	
	if (bmih.biCompression == BI_RLE8)  // not done yet
	{
		//   	read(handle, Data, Width*Height);
	}
	
	
	close(handle);
	
	return 1;
}

int TDIBitmap8::LoadFromPCX(CString FName)
{
	int	handle;
	byte  *Buffer,*Buf;
	
	handle = open((LPCTSTR)FName,O_BINARY | O_RDONLY);
	
	if (handle == -1)
	{
		//   	Application->MessageBox("Could not open PCX file.","Error",MB_OK);
		return 0;
	}
	
	byte Header[128];
	short x1,y1,x2,y2;
	
	read(handle, Header, 128);
	
	if (Header[0] != 10)
	{
//		Application->MessageBox("Not a PCX file","Error",MB_OK);
		return 0;
	}
	if (Header[1] != 5)
	{
//		Application->MessageBox("Must be PCX version 3.0","Error",MB_OK);
		return 0;
	}
	if (Header[2] != 1)
	{
//		Application->MessageBox("Must be PCX run length encoding.","Error",MB_OK);
		return 0;
	}
	if (Header[3] != 8)
	{
//		Application->MessageBox("Must be an 8-bit PCX file","Error",MB_OK);
		return 0;
	}
	
	Buffer = new byte[filelength(handle)-128];
	
	read(handle, Buffer,filelength(handle)-128);
	close(handle);
	
	Buf = Buffer;
	memcpy(&x1,&Header[4],2);
	memcpy(&y1,&Header[6],2);
	memcpy(&x2,&Header[8],2);
	memcpy(&y2,&Header[10],2);
	
	int pwidth,pheight;
	
	pwidth = x2-x1 + 1;
	pheight = y2-y1 + 1;
	SetSize(pwidth,pheight);
	
	int x,y=0;
	byte B;
	
	while (y<Height)
	{
		x = 0;
		while (x<VisibleWidth)
		{
			B = *Buf;
			Buf ++;
			if (B >= 192)
			{
				int count;
				count = B & 0x3f;
				B = *Buf;
				Buf++;
				while (count--)
				{
					Data[y*Width + x] = B;
					x++;
				}
			}
			else
			{
				Data[y*Width + x] = B;
				x++;
			}
		}
		y++;
	}
	
	
	delete[] Buffer;
	return 1;
}

void  TDIBitmap8::DrawAffineScanline_ZBuf(float x1, float x2, float u1, float u2,
										  float v1, float v2, float z1, float z2, unsigned char *targ,
										  unsigned char* tPtr, float *ZBuf)
{
	if ((x1<0) && (x2<0))
		return;
	if ((x1>=VisibleWidth) && (x2>=VisibleWidth))
		return;
	
	static float width;
	static float du, dv, dz;
	//   static int u, v;
	static int dui, dvi,ui,vi;
	
	if(x2 < x1) {
		static float tmp;
		
		tmp = x1; x1 = x2; x2 = tmp;
		tmp = u1; u1 = u2; u2 = tmp;
		tmp = v1; v1 = v2; v2 = tmp;
		tmp = z1; z1 = z2; z2 = tmp;
	}
	
	width = x2 - x1;
	
	if(width) {
		du = (u2 - u1)/width;
		dv = (v2 - v1)/width;
		dz = (z2 - z1)/width;
	}
	
	if (x1<0)
	{
		u1 -= (float)x1*du;
		v1 -= (float)x1*dv;
		z1 -= (float)x1*dz;
		x1= 0 ;
	}
	if (x2<0)
		x2 = 0;
	if (x2>=VisibleWidth)
		x2 = (float)(VisibleWidth-1);
	
	targ += int(x1);
	ZBuf += int(x1);
	
	dui = (int)(du*65536);
	dvi = (int)(dv*65536);
	ui = (int)(u1 * 65536);
	vi = (int)(v1 * 65536);
	
	int x = int(x2-x1);
	while (x-- >=0)
	{
		
#ifdef DEBUG
		try
		{
			if (z1 > *ZBuf)
			{
				*targ = tPtr[OfsY[vi>>16] + (ui>>16)];
				*ZBuf = z1;
			}
		}
		catch(...)
		{
		}
#else
		if (z1 > *ZBuf)
		{
			*targ = tPtr[OfsY[vi>>16] + (ui>>16)];
			*ZBuf = z1;
		}
#endif
		
		ui += dui;
		vi += dvi;
		
		
		z1 += dz;
		targ ++;
		ZBuf ++;
		
	}
	
	/*
	if(x2 < x1) {
	float tmp;
	
      tmp = x1; x1 = x2; x2 = tmp;
	  tmp = u1; u1 = u2; u2 = tmp;
	  tmp = v1; v1 = v2; v2 = tmp;
      tmp = z1; z1 = z2; z2 = tmp;
	  }
	  
		float width = x2 - x1;
		float du, dv, dz;
		int u, v;
		
		  if(width) {
		  du = (u2 - u1)/width;
		  dv = (v2 - v1)/width;
		  dz = (z2 - z1)/width;
		  }
		  
			targ += int(x1);
			ZBuf += int(x1);
			int x = int(width);
			for (x = int(x1);x<int(x2);x++)
			{
			u = int(u1);
			v = int(v1);
			
			  if (x>=0 && x<Width)
			  if (z1 > *ZBuf)
			  {
			  *targ = tPtr[OfsY[v] + u];
			  *ZBuf = z1;
			  }
			  
				u1 += du;
				v1 += dv;
				z1 += dz;
				targ ++;
				ZBuf++;
				}
	*/
}

void  TDIBitmap8::DrawTexTri_ZBuf(const TexTriangle &tri)
{
	if (!Enabled)
		return;
	
	int top = 0;                       // index to top vertex
	int a, b;                          // other 2 vertices
	int	i;
	
	// interpolants
	
	float dx_A, 			// change in sx with respect to sy
		dx_B,
		du_A,			// change in u with respect to sy
		du_B,
		dv_A,			// change in v with respect to sy
		dv_B,
		dz_A,
		dz_B;
	
	for(i = 1; i < 3; i++) // find top vertex
	{
		if(tri.v[i]->sy < tri.v[top]->sy) top = i;
	}
	
	a = top + 1;
	b = top - 1;
	if(a > 2) a = 0;
	if(b < 0) b = 2;
	
	int   y = (int)(tri.v[top]->sy);
	float x1 = tri.v[top]->sx;
	float x2 = x1;
	float u1 = tri.v[top]->u;
	float u2 = u1;
	float v1 = tri.v[top]->v;
	float	v2 = v1;
	float	z1 = tri.v[top]->sz;
	float z2 = z1;
	
	int height_A = (int)(tri.v[a]->sy - tri.v[top]->sy);
	int height_B = (int)(tri.v[b]->sy - tri.v[top]->sy);
	
	if(height_A) // avoid divide by zero
	{
		// calculate the interpolants
		dx_A = (tri.v[a]->sx - tri.v[top]->sx)/ height_A;
		du_A = (tri.v[a]->u  - tri.v[top]->u)/ height_A;
		dv_A = (tri.v[a]->v  - tri.v[top]->v)/ height_A;
		dz_A = (tri.v[a]->sz  - tri.v[top]->sz)/ height_A;
	}
	
	if(height_B)// avoid divide by zero
	{
		// calculate the interpolants
		dx_B = (tri.v[b]->sx - tri.v[top]->sx)/ height_B;
		du_B = (tri.v[b]->u  - tri.v[top]->u)/ height_B;
		dv_B = (tri.v[b]->v  - tri.v[top]->v)/ height_B;
		dz_B = (tri.v[b]->sz  - tri.v[top]->sz)/ height_B;
	}
	
	// start drawing
	unsigned char *targ = Data + PInfoHeader->biSizeImage - Width - y*Width;
	float *Ztarg = ZBuffer + y*Width;
	
	for(i = 2; i > 0;) {
		
		while(height_A && height_B)
		{
			if (y>=0 && y<Height)
				DrawAffineScanline_ZBuf(x1, x2, u1, u2, v1, v2, z1, z2, targ, tri.Tex, Ztarg);
			
			Ztarg += Width;
			targ-=Width;
			y++;
			height_A--;
			height_B--;
			
			x1 += dx_A;                     // add the interpolants
			x2 += dx_B;
			u1 += du_A;
			u2 += du_B;
			v1 += dv_A;
			v2 += dv_B;
			z1 += dz_A;
			z2 += dz_B;
		}
		
		if(!height_A)
		{
			// new edge
			
			int na = a + 1;
			if(na > 2) na = 0;
			
			height_A = (int)(tri.v[na]->sy - tri.v[a]->sy);
			
			if(height_A)  // avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_A = (tri.v[na]->sx - tri.v[a]->sx) / height_A;
				du_A = (tri.v[na]->u  - tri.v[a]->u)  / height_A;
				dv_A = (tri.v[na]->v  - tri.v[a]->v)  / height_A;
				dz_A = (tri.v[na]->sz - tri.v[a]->sz) / height_A;
			}
			
			x1 = tri.v[a]->sx;
			u1 = tri.v[a]->u;
			v1 = tri.v[a]->v;
			z1 = tri.v[a]->sz;
			
			i--;                               // one less vertex
			a = na;
			
		} // end if
		
		if(!height_B)
		{
			
			// new edge
			
			int nb = b - 1;
			if(nb < 0) nb = 2;
			
			height_B = (int)(tri.v[nb]->sy - tri.v[b]->sy);
			
			if(height_B)// avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_B = (tri.v[nb]->sx - tri.v[b]->sx) / height_B;
				du_B = (tri.v[nb]->u  - tri.v[b]->u)  / height_B;
				dv_B = (tri.v[nb]->v  - tri.v[b]->v)  / height_B;
				dz_B = (tri.v[nb]->sz - tri.v[b]->sz) / height_B;
			}
			
			x2 = tri.v[b]->sx;
			u2 = tri.v[b]->u;
			v2 = tri.v[b]->v;
			z2 = tri.v[b]->sz;
			
			i--;                               // one less vertex
			b = nb;
			
		} // end if
		
	} // end for loop
	
} // end function

void  TDIBitmap8::DrawAffineScanline_ZBufGreyScale(float x1, float x2, float u1, float u2,
												   float v1, float v2, float z1, float z2, unsigned char *targ,
												   unsigned char* tPtr, float *ZBuf, unsigned char StartCol)
{
	if(x2 < x1) {
		float tmp;
		
		tmp = x1; x1 = x2; x2 = tmp;
		tmp = u1; u1 = u2; u2 = tmp;
		tmp = v1; v1 = v2; v2 = tmp;
		tmp = z1; z1 = z2; z2 = tmp;
	}
	
	float width = x2 - x1;
	float du, dv, dz;
	int u, v;
	
	if(width) {
		du = (u2 - u1)/width;
		dv = (v2 - v1)/width;
		dz = (z2 - z1)/width;
	}
	
	targ += int(x1);
	ZBuf += int(x1);
	int x;
	for (x = int(x1);x<int(x2);x++)
	{
		u = int(u1);
		v = int(v1);
		
		if (x>=0 && x<Width)
			if (z1 > *ZBuf)
			{
				int col;
				col = PPalette[tPtr[OfsY[v] + u]].rgbRed +
					PPalette[tPtr[OfsY[v] + u]].rgbGreen +
					PPalette[tPtr[OfsY[v] + u]].rgbBlue;
				col = col/3;
				col = (int)col >> GREYSHIFT;
				col+=StartCol;
				//            targ[x] = StartCol + col;
				*targ = (unsigned char)col;
				*ZBuf = z1;
			}
			
			u1 += du;
			v1 += dv;
			z1 += dz;
			targ ++;
			ZBuf++;
	}
}

void  TDIBitmap8::DrawTexTri_ZBufGreyScale(const TexTriangle &tri, unsigned char StartCol)
{
	if (!Enabled)
		return;
	
	int top = 0;                       // index to top vertex
	int a, b;                          // other 2 vertices
	int i;
	
	// interpolants
	
	float dx_A, 			// change in sx with respect to sy
		dx_B,
		du_A,			// change in u with respect to sy
		du_B,
		dv_A,			// change in v with respect to sy
		dv_B,
		dz_A,
		dz_B;
	
	for(i = 1; i < 3; i++) // find top vertex
	{
		if(tri.v[i]->sy < tri.v[top]->sy) top = i;
	}
	
	a = top + 1;
	b = top - 1;
	if(a > 2) a = 0;
	if(b < 0) b = 2;
	
	int   y = (int)(tri.v[top]->sy);
	float x1 = tri.v[top]->sx;
	float x2 = x1;
	float u1 = tri.v[top]->u;
	float u2 = u1;
	float v1 = tri.v[top]->v;
	float	v2 = v1;
	float	z1 = tri.v[top]->sz;
	float z2 = z1;
	
	int height_A = (int)(tri.v[a]->sy - tri.v[top]->sy);
	int height_B = (int)(tri.v[b]->sy - tri.v[top]->sy);
	
	if(height_A) // avoid divide by zero
	{
		// calculate the interpolants
		dx_A = (tri.v[a]->sx - tri.v[top]->sx)/ height_A;
		du_A = (tri.v[a]->u  - tri.v[top]->u)/ height_A;
		dv_A = (tri.v[a]->v  - tri.v[top]->v)/ height_A;
		dz_A = (tri.v[a]->sz  - tri.v[top]->sz)/ height_A;
	}
	
	if(height_B)// avoid divide by zero
	{
		// calculate the interpolants
		dx_B = (tri.v[b]->sx - tri.v[top]->sx)/ height_B;
		du_B = (tri.v[b]->u  - tri.v[top]->u)/ height_B;
		dv_B = (tri.v[b]->v  - tri.v[top]->v)/ height_B;
		dz_B = (tri.v[b]->sz  - tri.v[top]->sz)/ height_B;
	}
	
	// start drawing
	unsigned char *targ = Data + PInfoHeader->biSizeImage - Width - y*Width;
	float *Ztarg = ZBuffer + y*Width;
	
	for(i = 2; i > 0;) {
		
		while(height_A && height_B)
		{
			if (y>=0 && y<Height)
				DrawAffineScanline_ZBufGreyScale(x1, x2, u1, u2, v1, v2, z1, z2, targ, tri.Tex, Ztarg,StartCol);
			
			Ztarg += Width;
			targ-=Width;
			y++;
			height_A--;
			height_B--;
			
			x1 += dx_A;                     // add the interpolants
			x2 += dx_B;
			u1 += du_A;
			u2 += du_B;
			v1 += dv_A;
			v2 += dv_B;
			z1 += dz_A;
			z2 += dz_B;
		}
		
		if(!height_A)
		{
			// new edge
			
			int na = a + 1;
			if(na > 2) na = 0;
			
			height_A = (int)(tri.v[na]->sy - tri.v[a]->sy);
			
			if(height_A)  // avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_A = (tri.v[na]->sx - tri.v[a]->sx) / height_A;
				du_A = (tri.v[na]->u  - tri.v[a]->u)  / height_A;
				dv_A = (tri.v[na]->v  - tri.v[a]->v)  / height_A;
				dz_A = (tri.v[na]->sz - tri.v[a]->sz) / height_A;
			}
			
			x1 = tri.v[a]->sx;
			u1 = tri.v[a]->u;
			v1 = tri.v[a]->v;
			z1 = tri.v[a]->sz;
			
			i--;                               // one less vertex
			a = na;
			
		} // end if
		
		if(!height_B)
		{
			
			// new edge
			
			int nb = b - 1;
			if(nb < 0) nb = 2;
			
			height_B = (int)(tri.v[nb]->sy - tri.v[b]->sy);
			
			if(height_B)// avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_B = (tri.v[nb]->sx - tri.v[b]->sx) / height_B;
				du_B = (tri.v[nb]->u  - tri.v[b]->u)  / height_B;
				dv_B = (tri.v[nb]->v  - tri.v[b]->v)  / height_B;
				dz_B = (tri.v[nb]->sz - tri.v[b]->sz) / height_B;
			}
			
			x2 = tri.v[b]->sx;
			u2 = tri.v[b]->u;
			v2 = tri.v[b]->v;
			z2 = tri.v[b]->sz;
			
			i--;                               // one less vertex
			b = nb;
			
		} // end if
		
	} // end for loop
	
}



void  TDIBitmap8::DrawScanline_ZBuf(float x1, float x2,
									float z1, float z2, unsigned char Col, unsigned char *targ,
									float *ZBuf)
{
	if ((x1<0) && (x2<0))
		return;
	if ((x1>=VisibleWidth) && (x2>=VisibleWidth))
		return;
	
	static float width;
	static float dz;
	
	if(x2 < x1) {
		static float tmp;
		
		tmp = x1; x1 = x2; x2 = tmp;
		tmp = z1; z1 = z2; z2 = tmp;
	}
	
	width = x2 - x1;
	
	if(width) {
		dz = (z2 - z1)/width;
	}
	
	if (x1<0)
	{
		z1 -= (float)x1*dz;
		x1= 0 ;
	}
	if (x2<0)
		x2 = 0;
	if (x2>=VisibleWidth)
		x2 = (float)(VisibleWidth-1);
	
	targ += int(x1);
	ZBuf += int(x1);
	
	int x = int(x2-x1);
	while (x-- >=0)
	{
		if (z1 > *ZBuf)
		{
			*targ = Col;
			*ZBuf = z1;
		}
		z1 += dz;
		targ ++;
		ZBuf ++;
		
	}
	
}

void  TDIBitmap8::DrawTri_ZBuf(const TexTriangle &tri, unsigned char Col)
{
	if (!Enabled)
		return;
	
	int top = 0;                       // index to top vertex
	int a, b;                          // other 2 vertices
	int i;
	
	// interpolants
	
	float dx_A, 			// change in sx with respect to sy
		dx_B,
		dz_A,
		dz_B;
	
	for(i = 1; i < 3; i++) // find top vertex
	{
		if(tri.v[i]->sy < tri.v[top]->sy) top = i;
	}
	
	a = top + 1;
	b = top - 1;
	if(a > 2) a = 0;
	if(b < 0) b = 2;
	
	int   y = (int)(tri.v[top]->sy);
	float x1 = tri.v[top]->sx;
	float x2 = x1;
	float	z1 = tri.v[top]->sz;
	float z2 = z1;
	
	int height_A = (int)(tri.v[a]->sy - tri.v[top]->sy);
	int height_B = (int)(tri.v[b]->sy - tri.v[top]->sy);
	
	if(height_A) // avoid divide by zero
	{
		// calculate the interpolants
		dx_A = (tri.v[a]->sx - tri.v[top]->sx)/ height_A;
		dz_A = (tri.v[a]->sz  - tri.v[top]->sz)/ height_A;
	}
	
	if(height_B)// avoid divide by zero
	{
		// calculate the interpolants
		dx_B = (tri.v[b]->sx - tri.v[top]->sx)/ height_B;
		dz_B = (tri.v[b]->sz  - tri.v[top]->sz)/ height_B;
	}
	
	// start drawing
	unsigned char *targ = Data + PInfoHeader->biSizeImage - Width - y*Width;
	float *Ztarg = ZBuffer + y*Width;
	
	for(i = 2; i > 0;) {
		
		while(height_A && height_B)
		{
			if (y>=0 && y<Height)
				DrawScanline_ZBuf(x1, x2, z1, z2, Col, targ, Ztarg);
			
			Ztarg += Width;
			targ -= Width;
			y++;
			height_A--;
			height_B--;
			
			x1 += dx_A;                     // add the interpolants
			x2 += dx_B;
			z1 += dz_A;
			z2 += dz_B;
		}
		
		if(!height_A)
		{
			// new edge
			
			int na = a + 1;
			if(na > 2) na = 0;
			
			height_A = (int)(tri.v[na]->sy - tri.v[a]->sy);
			
			if(height_A)  // avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_A = (tri.v[na]->sx - tri.v[a]->sx) / height_A;
				dz_A = (tri.v[na]->sz - tri.v[a]->sz) / height_A;
			}
			
			x1 = tri.v[a]->sx;
			z1 = tri.v[a]->sz;
			
			i--;                               // one less vertex
			a = na;
			
		} // end if
		
		if(!height_B)
		{
			
			// new edge
			
			int nb = b - 1;
			if(nb < 0) nb = 2;
			
			height_B = (int)(tri.v[nb]->sy - tri.v[b]->sy);
			
			if(height_B)// avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_B = (tri.v[nb]->sx - tri.v[b]->sx) / height_B;
				dz_B = (tri.v[nb]->sz - tri.v[b]->sz) / height_B;
			}
			
			x2 = tri.v[b]->sx;
			z2 = tri.v[b]->sz;
			
			i--;                               // one less vertex
			b = nb;
			
		} // end if
		
	} // end for loop
	
} // end function

void  TDIBitmap8::Line_ZBuf(int x1, int y1, float z1, int x2, int y2, float z2, unsigned char col)
{
	int dx,dy,sx,sy, sofsx, sofsy;
	unsigned char *dst;
	int d,numpix;
	float dz;
	
	dx=x2-x1;
	dy=y2-y1;
	dz = z2-z1;
	
	if (!Enabled)
		return;
	
	if ( x1<0 && x2<0)
		return;
	if ( x1>=VisibleWidth && x2>=VisibleWidth )
		return;
	if ( y1<0 && y2<0)
		return;
	if ( y1>=Height && y2>=Height )
		return;
	
	if (dx) sx=(dx<0) ? -1 : 1; else sx=0;
	if (dy) sy=(dy<0) ? -1 : 1; else sy=0;
	
	if (dx) sofsx=(dx<0) ? -1 : 1; else sofsx=0;
	if (dy) sofsy=(dy<0) ? Width : -Width; else sofsy=0;
	
	
	if (dx<0) dx=-dx;
	if (dy<0) dy=-dy;
	
	dst = Data + (PInfoHeader->biSizeImage - y1*Width + x1 - Width);
	float *ZBuf = ZBuffer + y1*Width + x1;
	
	if (dx==0 && dy==0) return;
	
	if (dx>dy)
	{
		dz /= dx;
		d = dx >> 1;
		numpix=dx;
		while (numpix--)
		{
			if (x1>=0 && x1<VisibleWidth)
				if (y1>=0 && y1<Height)
					if (z1>*ZBuf)
					{
						*dst = col;
						*ZBuf = z1;
					}
					d+=dy;
					x1+=sx;
					dst += sofsx;
					ZBuf+= sofsx;
					if (d>dx)
					{
						z1 += dz;
						d-=dx;
						y1+=sy;
						dst += sofsy;
						ZBuf+= -sofsy;
					}
		}
	}
	else
	{
		dz /= dy;
		d = dy >> 1;
		numpix=dy;
		while (numpix--)
		{
			if (x1>=0 && x1<VisibleWidth)
				if (y1>=0 && y1<Height)
					if (z1>*ZBuf)
					{
						*dst = col;
						*ZBuf = z1;
					}
					d+=dx;
					y1+=sy;
					dst += sofsy;
					ZBuf+= -sofsy;
					if (d>dy)
					{
						z1 += dz;
						d-=dy;
						x1+=sx;
						dst += sofsx;
						ZBuf+= sofsx;
					}
		}
	}
}

int TDIBitmap8::SaveToBMP(CString FName)
{
	BITMAPFILEHEADER	bmfh;
	//   RGBQUAD				aColors[];
	//   BYTE					aBitmapBits[];
	
	int					handle;
	
	handle = open((LPCTSTR)FName,O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD|S_IWRITE);
	
	if (handle == -1)
		return 0;
	
	bmfh.bfType = 19778;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPINFOHEADER) + 1024 + sizeof(bmfh) + Width*Height;
	bmfh.bfOffBits = sizeof(BITMAPINFOHEADER) + 1024 + sizeof(bmfh);
	
	write(handle, &bmfh, sizeof(BITMAPFILEHEADER));
	write(handle, PInfo, sizeof(BITMAPINFOHEADER) + 1024);
	
	unsigned char *temp = new unsigned char[Width*Height];
	
	memcpy(temp, Data, Width*Height);
	/*
	dest = temp + Width*Height - Width;
	src = Data;
	for (int i=0;i<Height;i++)
	{
	memcpy(dest,src,Width);
	dest -= Width;
	src += Width;
	}
	*/
	
	write(handle, temp, Width*Height);
	delete[] temp;
	
	close(handle);
	
	return 1;
}

int TDIBitmap8::SaveToPCX(CString FName)
{
	int handle;
	
	handle = open((LPCTSTR)FName, O_BINARY|O_WRONLY|O_CREAT|O_TRUNC, S_IREAD|S_IWRITE);
	if (handle == -1)
		return 0;
	
	PCXHeader	pcx;
	unsigned char *comp;
	int count=0;
	
	comp = new unsigned char[VisibleWidth*Height*2];
	
	pcx.Manufacturer = 10;
	pcx.Version = 5;
	pcx.Encoding = 1;
	pcx.BitsPerPixel = 8;
	pcx.XMin = pcx.YMin = 0;
	pcx.XMax = VisibleWidth-1;
	pcx.YMax = Height-1;
	pcx.HRes = (short)VisibleWidth;
	pcx.VRes = (short)Height;
	memset(pcx.ColorMap,0,48);
	pcx.Planes = 1;
	pcx.BytesPerLine = (short)VisibleWidth + VisibleWidth%2;
	
	unsigned char *scanline,*targ = comp;
	scanline = Data + (PInfoHeader->biSizeImage - Width);
	
	bool counting = false;
	int repc=0;
	unsigned char ref;
	
	for (int j=0;j<Height;j++)
	{
		for (int i=0;i<VisibleWidth;i++)
		{
			if (!counting)
			{
				if (scanline[i]<PCXRLEFLAG)
				{
					if (i<VisibleWidth-1)
					{
						if (scanline[i] == scanline[i+1])
						{
							counting = true;
							repc = 1;
							ref = scanline[i];
						}
					}
					
					if (!counting)
					{
						*targ = scanline[i];
						targ++;
						count++;
					}
				}
				else
				{
					if (i<VisibleWidth-1)
					{
						if (scanline[i] == scanline[i+1])
						{
							counting = true;
							repc = 1;
							ref = scanline[i];
						}
					}
					
					if (!counting)
					{
						*targ = PCXRLEFLAG+1;
						targ++;
						count++;
						*targ = scanline[i];
						targ++;
						count++;
					}
				}
			}
			else
			{
				if (i==VisibleWidth-1 || scanline[i] != ref || repc == 63) // write changes
				{
					*targ = PCXRLEFLAG+repc;
					targ++;
					count++;
					*targ = ref;
					targ++;
					count++;
					
					counting = false;
					repc=0;
					ref = 0;
					i--;
				}
				else
					repc++;
			}
			
		}
		scanline -= Width;
	}
	
	*targ = 12;
	targ++;
	count++;
	
	unsigned char pal[768], *p = &pal[0];
	
	for (int i=0;i<256;i++)
	{
		*p++ = PPalette[i].rgbRed;
		*p++ = PPalette[i].rgbGreen;
		*p++ = PPalette[i].rgbBlue;
	}
	
	write(handle, &pcx, sizeof(PCXHeader));
	write(handle, comp, count);
	write(handle, pal, 768);
	
	close(handle);
	
	delete[] comp;
	return 1;
}

void  TDIBitmap8::DrawGouradScanline_ZBuf(float x1, float x2,
										  float z1, float z2,  int b1, int b2, unsigned char *targ,
										  float *ZBuf)
{
	if ((x1<0) && (x2<0))
		return;
	if ((x1>=VisibleWidth) && (x2>=VisibleWidth))
		return;
	
	static float width;
	static float dz;
	static int db;
	
	
	if(x2 < x1) {
		static float tmp;
		static int t;
		
		tmp = x1; x1 = x2; x2 = tmp;
		tmp = z1; z1 = z2; z2 = tmp;
		t = b1 ; b1 = b2; b2 = t;
	}
	
	width = x2 - x1;
	
	if(width) {
		dz = (z2 - z1)/width;
		db = (int)((b2 - b1)/width);
	}
	
	if (x1<0)
	{
		z1 -= (float)x1*dz;
		b1 -= (int)(x1*db);
		x1= 0 ;
	}
	if (x2<0)
		x2 = 0;
	if (x2>=VisibleWidth)
		x2 = (float)(VisibleWidth-1);
	
	targ += int(x1);
	ZBuf += int(x1);
	
	int x = int(x2-x1);
	
	
	unsigned char *BrightShad = Bright[FACESHADMAX];
	
	while (x-- >=0)
	{
		//      try
		//      {
		if (z1 > *ZBuf)
		{
			//         float base;
			
			//         *targ = ((((b1>>15)*(FACESHADMAX-FACESHADMIN)) >> 15) + FACESHADMIN);
			//         *targ = Bright[FACESHADMAX][b1 >> 23];
			*targ = BrightShad[b1 >> 23];
			*ZBuf = z1;
		}
		//      }
		//      catch(...)
		//      {
		//      }
		
		z1 += dz;
		b1 += db;
		targ ++;
		ZBuf ++;
	}
	
}

void  TDIBitmap8::DrawGouradTri_ZBuf(const TexTriangle &tri)
{
	if (!Enabled)
		return;
	
	int top = 0;                       // index to top vertex
	int a, b;                          // other 2 vertices
	int i;
	
	// interpolants
	
	float dx_A, 			// change in sx with respect to sy
		dx_B,
		dz_A,
		dz_B;
	
	int   db_A,
		db_B;
	
	
	for(i = 1; i < 3; i++) // find top vertex
	{
		try{
			if(tri.v[i]->sy < tri.v[top]->sy) top = i;
		}
		catch(...)
		{
		}
	}
	
	a = top + 1;
	b = top - 1;
	if(a > 2) a = 0;
	if(b < 0) b = 2;
	
	int   y = (int)(tri.v[top]->sy);
	float x1 = tri.v[top]->sx;
	float x2 = x1;
	float	z1 = tri.v[top]->sz;
	float z2 = z1;
	
	int b1 = tri.v[top]->br;
	int b2 = b1;
	
	int height_A = (int)(tri.v[a]->sy - tri.v[top]->sy);
	int height_B = (int)(tri.v[b]->sy - tri.v[top]->sy);
	
	if(height_A) // avoid divide by zero
	{
		// calculate the interpolants
		dx_A = (tri.v[a]->sx - tri.v[top]->sx)/ height_A;
		dz_A = (tri.v[a]->sz  - tri.v[top]->sz)/ height_A;
		db_A = (tri.v[a]->br  - tri.v[top]->br)/ height_A;
	}
	
	if(height_B)// avoid divide by zero
	{
		// calculate the interpolants
		dx_B = (tri.v[b]->sx - tri.v[top]->sx)/ height_B;
		dz_B = (tri.v[b]->sz  - tri.v[top]->sz)/ height_B;
		db_B = (tri.v[b]->br  - tri.v[top]->br)/ height_B;
	}
	
	// start drawing
	unsigned char *targ = Data + PInfoHeader->biSizeImage - Width - y*Width;
	float *Ztarg = ZBuffer + y*Width;
	
	for(i = 2; i > 0;) {
		
		while(height_A && height_B)
		{
			if (b1 >= 1073741824)
				b1 = 1073741823;
			
			if (b2 >= 1073741824)
				b2 = 1073741823;
			
			if (y>=0 && y<Height)
				DrawGouradScanline_ZBuf(x1, x2, z1, z2, b1,b2,targ,Ztarg);
			
			Ztarg += Width;
			targ-=Width;
			y++;
			height_A--;
			height_B--;
			
			x1 += dx_A;                     // add the interpolants
			x2 += dx_B;
			z1 += dz_A;
			z2 += dz_B;
			b1 += db_A;
			b2 += db_B;
		}
		
		if(!height_A)
		{
			// new edge
			
			int na = a + 1;
			if(na > 2) na = 0;
			
			height_A = (int)(tri.v[na]->sy - tri.v[a]->sy);
			
			if(height_A)  // avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				dx_A = (tri.v[na]->sx - tri.v[a]->sx) / height_A;
				dz_A = (tri.v[na]->sz - tri.v[a]->sz) / height_A;
				db_A = (tri.v[na]->br - tri.v[a]->br) / height_A;
			}
			
			x1 = tri.v[a]->sx;
			b1 = tri.v[a]->br;
			z1 = tri.v[a]->sz;
			
			i--;                               // one less vertex
			a = na;
			
		} // end if
		
		
		if(!height_B)
		{
			
			// new edge
			
			int nb = b - 1;
			if(nb < 0) nb = 2;
			
			height_B = (int)(tri.v[nb]->sy - tri.v[b]->sy);
			
			if(height_B)// avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_B = (tri.v[nb]->sx - tri.v[b]->sx) / height_B;
				dz_B = (tri.v[nb]->sz - tri.v[b]->sz) / height_B;
				db_B = (tri.v[nb]->br - tri.v[b]->br) / height_B;
			}
			
			x2 = tri.v[b]->sx;
			z2 = tri.v[b]->sz;
			b2 = tri.v[b]->br;
			
			i--;                               // one less vertex
			b = nb;
			
		} // end if
		
	} // end for loop
	
} // end function


void  TDIBitmap8::DrawAffineScanline_ZBuf_UV(float x1, float x2, float u1, float u2,
											 float v1, float v2, float z1, float z2, unsigned char *targ,
											 unsigned char* tPtr, float *ZBuf, short *UD, short *VD)
{
	if ((x1<0) && (x2<0))
		return;
	if ((x1>=VisibleWidth) && (x2>=VisibleWidth))
		return;
	
	static float width;
	static float du, dv, dz;
	//   static int u, v;
	static int dui, dvi,ui,vi;
	
	if(x2 < x1) {
		static float tmp;
		
		tmp = x1; x1 = x2; x2 = tmp;
		tmp = u1; u1 = u2; u2 = tmp;
		tmp = v1; v1 = v2; v2 = tmp;
		tmp = z1; z1 = z2; z2 = tmp;
	}
	
	width = x2 - x1;
	
	if(width) {
		du = (u2 - u1)/width;
		dv = (v2 - v1)/width;
		dz = (z2 - z1)/width;
	}
	
	if (x1<0)
	{
		u1 -= (float)x1*du;
		v1 -= (float)x1*dv;
		z1 -= (float)x1*dz;
		x1= 0 ;
	}
	if (x2<0)
		x2 = 0;
	if (x2>=VisibleWidth)
		x2 = (float)(VisibleWidth-1);
	
	targ += int(x1);
	ZBuf += int(x1);
	UD += int(x1);
	VD += int(x1);
	
	dui = (int)(du*65536);
	dvi = (int)(dv*65536);
	ui = (int)(u1 * 65536);
	vi = (int)(v1 * 65536);
	
	int x = int(x2-x1);
	while (x-- >=0)
	{
		
		try
		{
			if (z1 > *ZBuf)
			{
				*targ = tPtr[OfsY[vi>>16] + (ui>>16)];
				*ZBuf = z1;
				*UD = ui>>16;
				*VD = vi>>16;
			}
		}
		catch(...)
		{
		}
		
		ui += dui;
		vi += dvi;
		
		
		z1 += dz;
		targ ++;
		ZBuf ++;
		UD++;
		VD++;
		
	}
}

void  TDIBitmap8::DrawTexTri_ZBuf_UV(const TexTriangle &tri, short* UData, short *VData)
{
	if (!Enabled)
		return;
	
	int top = 0;                       // index to top vertex
	int a, b;                          // other 2 vertices
	int i;
	
	// interpolants
	
	float dx_A, 			// change in sx with respect to sy
		dx_B,
		du_A,			// change in u with respect to sy
		du_B,
		dv_A,			// change in v with respect to sy
		dv_B,
		dz_A,
		dz_B;
	
	for(i = 1; i < 3; i++) // find top vertex
	{
		if(tri.v[i]->sy < tri.v[top]->sy) top = i;
	}
	
	a = top + 1;
	b = top - 1;
	if(a > 2) a = 0;
	if(b < 0) b = 2;
	
	int   y = (int)(tri.v[top]->sy);
	float x1 = tri.v[top]->sx;
	float x2 = x1;
	float u1 = tri.v[top]->u;
	float u2 = u1;
	float v1 = tri.v[top]->v;
	float	v2 = v1;
	float	z1 = tri.v[top]->sz;
	float z2 = z1;
	
	int height_A = (int)(tri.v[a]->sy - tri.v[top]->sy);
	int height_B = (int)(tri.v[b]->sy - tri.v[top]->sy);
	
	if(height_A) // avoid divide by zero
	{
		// calculate the interpolants
		dx_A = (tri.v[a]->sx - tri.v[top]->sx)/ height_A;
		du_A = (tri.v[a]->u  - tri.v[top]->u)/ height_A;
		dv_A = (tri.v[a]->v  - tri.v[top]->v)/ height_A;
		dz_A = (tri.v[a]->sz  - tri.v[top]->sz)/ height_A;
	}
	
	if(height_B)// avoid divide by zero
	{
		// calculate the interpolants
		dx_B = (tri.v[b]->sx - tri.v[top]->sx)/ height_B;
		du_B = (tri.v[b]->u  - tri.v[top]->u)/ height_B;
		dv_B = (tri.v[b]->v  - tri.v[top]->v)/ height_B;
		dz_B = (tri.v[b]->sz  - tri.v[top]->sz)/ height_B;
	}
	
	// start drawing
	unsigned char *targ = Data + PInfoHeader->biSizeImage - Width - y*Width;
	float *Ztarg = ZBuffer + y*Width;
	short *UD = UData + y*Width,*VD = VData + y*Width;
	
	for(i = 2; i > 0;) {
		
		while(height_A && height_B)
		{
			if (y>=0 && y<Height)
				DrawAffineScanline_ZBuf_UV(x1, x2, u1, u2, v1, v2, z1, z2, targ, tri.Tex, Ztarg,UD,VD);
			
			Ztarg += Width;
			UD += Width;
			VD += Width;
			targ-=Width;
			y++;
			height_A--;
			height_B--;
			
			x1 += dx_A;                     // add the interpolants
			x2 += dx_B;
			u1 += du_A;
			u2 += du_B;
			v1 += dv_A;
			v2 += dv_B;
			z1 += dz_A;
			z2 += dz_B;
		}
		
		if(!height_A)
		{
			// new edge
			
			int na = a + 1;
			if(na > 2) na = 0;
			
			height_A = (int)(tri.v[na]->sy - tri.v[a]->sy);
			
			if(height_A)  // avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_A = (tri.v[na]->sx - tri.v[a]->sx) / height_A;
				du_A = (tri.v[na]->u  - tri.v[a]->u)  / height_A;
				dv_A = (tri.v[na]->v  - tri.v[a]->v)  / height_A;
				dz_A = (tri.v[na]->sz - tri.v[a]->sz) / height_A;
			}
			
			x1 = tri.v[a]->sx;
			u1 = tri.v[a]->u;
			v1 = tri.v[a]->v;
			z1 = tri.v[a]->sz;
			
			i--;                               // one less vertex
			a = na;
			
		} // end if
		
		if(!height_B)
		{
			
			// new edge
			
			int nb = b - 1;
			if(nb < 0) nb = 2;
			
			height_B = (int)(tri.v[nb]->sy - tri.v[b]->sy);
			
			if(height_B)// avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_B = (tri.v[nb]->sx - tri.v[b]->sx) / height_B;
				du_B = (tri.v[nb]->u  - tri.v[b]->u)  / height_B;
				dv_B = (tri.v[nb]->v  - tri.v[b]->v)  / height_B;
				dz_B = (tri.v[nb]->sz - tri.v[b]->sz) / height_B;
			}
			
			x2 = tri.v[b]->sx;
			u2 = tri.v[b]->u;
			v2 = tri.v[b]->v;
			z2 = tri.v[b]->sz;
			
			i--;                               // one less vertex
			b = nb;
			
		} // end if
		
	} // end for loop
	
} // end function






void  TDIBitmap8::DrawAffineScanline_ZBuf_UV(float x1, float x2, float u1, float u2,
											 float v1, float v2, float z1, float z2, unsigned char *targ,
											 unsigned char* tPtr, float *ZBuf, unsigned char **UVD)
{
	if ((x1<0) && (x2<0))
		return;
	if ((x1>=VisibleWidth) && (x2>=VisibleWidth))
		return;
	
	static float width;
	static float du, dv, dz;
	//   static int u, v;
	static int dui, dvi,ui,vi;
	
	if(x2 < x1) {
		static float tmp;
		
		tmp = x1; x1 = x2; x2 = tmp;
		tmp = u1; u1 = u2; u2 = tmp;
		tmp = v1; v1 = v2; v2 = tmp;
		tmp = z1; z1 = z2; z2 = tmp;
	}
	
	width = x2 - x1;
	
	if(width) {
		du = (u2 - u1)/width;
		dv = (v2 - v1)/width;
		dz = (z2 - z1)/width;
	}
	
	if (x1<0)
	{
		u1 -= (float)x1*du;
		v1 -= (float)x1*dv;
		z1 -= (float)x1*dz;
		x1= 0 ;
	}
	if (x2<0)
		x2 = 0;
	if (x2>=VisibleWidth)
		x2 = (float)(VisibleWidth-1);
	
	targ += int(x1);
	ZBuf += int(x1);
	UVD += int(x1);
	
	dui = (int)(du*65536);
	dvi = (int)(dv*65536);
	ui = (int)(u1 * 65536);
	vi = (int)(v1 * 65536);
	
	int x = int(x2-x1);
	while (x-- >=0)
	{
		
		try
		{
			if (z1 > *ZBuf)
			{
				*targ = tPtr[OfsY[vi>>16] + (ui>>16)];
				*ZBuf = z1;
				*UVD = &tPtr[OfsY[vi>>16] + (ui>>16)];
			}
		}
		catch(...)
		{
		}
		
		ui += dui;
		vi += dvi;
		
		z1 += dz;
		targ ++;
		ZBuf ++;
		UVD ++;
		//      UD++;
		//      VD++;
		
	}
}

void  TDIBitmap8::DrawTexTri_ZBuf_UV(const TexTriangle &tri, unsigned char **UVData)
{
	if (!Enabled)
		return;
	
	int top = 0;                       // index to top vertex
	int a, b;                          // other 2 vertices
	int i;
	
	// interpolants
	
	float dx_A, 			// change in sx with respect to sy
		dx_B,
		du_A,			// change in u with respect to sy
		du_B,
		dv_A,			// change in v with respect to sy
		dv_B,
		dz_A,
		dz_B;
	
	for(i = 1; i < 3; i++) // find top vertex
	{
		if(tri.v[i]->sy < tri.v[top]->sy) top = i;
	}
	
	a = top + 1;
	b = top - 1;
	if(a > 2) a = 0;
	if(b < 0) b = 2;
	
	int   y = (int)(tri.v[top]->sy);
	float x1 = tri.v[top]->sx;
	float x2 = x1;
	float u1 = tri.v[top]->u;
	float u2 = u1;
	float v1 = tri.v[top]->v;
	float	v2 = v1;
	float	z1 = tri.v[top]->sz;
	float z2 = z1;
	
	int height_A = (int)(tri.v[a]->sy - tri.v[top]->sy);
	int height_B = (int)(tri.v[b]->sy - tri.v[top]->sy);
	
	if(height_A) // avoid divide by zero
	{
		// calculate the interpolants
		dx_A = (tri.v[a]->sx - tri.v[top]->sx)/ height_A;
		du_A = (tri.v[a]->u  - tri.v[top]->u)/ height_A;
		dv_A = (tri.v[a]->v  - tri.v[top]->v)/ height_A;
		dz_A = (tri.v[a]->sz  - tri.v[top]->sz)/ height_A;
	}
	
	if(height_B)// avoid divide by zero
	{
		// calculate the interpolants
		dx_B = (tri.v[b]->sx - tri.v[top]->sx)/ height_B;
		du_B = (tri.v[b]->u  - tri.v[top]->u)/ height_B;
		dv_B = (tri.v[b]->v  - tri.v[top]->v)/ height_B;
		dz_B = (tri.v[b]->sz  - tri.v[top]->sz)/ height_B;
	}
	
	// start drawing
	unsigned char *targ = Data + PInfoHeader->biSizeImage - Width - y*Width;
	float *Ztarg = ZBuffer + y*Width;
	//   short *UD = UData + y*Width,*VD = VData + y*Width;
	unsigned char **UVD = UVData + y*Width ;
	
	for(i = 2; i > 0;) {
		
		while(height_A && height_B)
		{
			if (y>=0 && y<Height)
				DrawAffineScanline_ZBuf_UV(x1, x2, u1, u2, v1, v2, z1, z2, targ, tri.Tex, Ztarg,UVD);
			
			Ztarg += Width;
			UVD += Width;
			//         UD += Width;
			//         VD += Width;
			targ-=Width;
			y++;
			height_A--;
			height_B--;
			
			x1 += dx_A;                     // add the interpolants
			x2 += dx_B;
			u1 += du_A;
			u2 += du_B;
			v1 += dv_A;
			v2 += dv_B;
			z1 += dz_A;
			z2 += dz_B;
		}
		
		if(!height_A)
		{
			// new edge
			
			int na = a + 1;
			if(na > 2) na = 0;
			
			height_A = (int)(tri.v[na]->sy - tri.v[a]->sy);
			
			if(height_A)  // avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_A = (tri.v[na]->sx - tri.v[a]->sx) / height_A;
				du_A = (tri.v[na]->u  - tri.v[a]->u)  / height_A;
				dv_A = (tri.v[na]->v  - tri.v[a]->v)  / height_A;
				dz_A = (tri.v[na]->sz - tri.v[a]->sz) / height_A;
			}
			
			x1 = tri.v[a]->sx;
			u1 = tri.v[a]->u;
			v1 = tri.v[a]->v;
			z1 = tri.v[a]->sz;
			
			i--;                               // one less vertex
			a = na;
			
		} // end if
		
		if(!height_B)
		{
			
			// new edge
			
			int nb = b - 1;
			if(nb < 0) nb = 2;
			
			height_B = (int)(tri.v[nb]->sy - tri.v[b]->sy);
			
			if(height_B)// avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_B = (tri.v[nb]->sx - tri.v[b]->sx) / height_B;
				du_B = (tri.v[nb]->u  - tri.v[b]->u)  / height_B;
				dv_B = (tri.v[nb]->v  - tri.v[b]->v)  / height_B;
				dz_B = (tri.v[nb]->sz - tri.v[b]->sz) / height_B;
			}
			
			x2 = tri.v[b]->sx;
			u2 = tri.v[b]->u;
			v2 = tri.v[b]->v;
			z2 = tri.v[b]->sz;
			
			i--;                               // one less vertex
			b = nb;
			
		} // end if
		
	} // end for loop
	
} // end function










void  TDIBitmap8::DrawAffineGouradScanline_ZBuf(float x1, float x2, float u1, float u2,
												float v1, float v2, float z1, float z2, int b1, int b2, unsigned char *targ,
												unsigned char* tPtr, float *ZBuf)
{
	if ((x1<0) && (x2<0))
		return;
	if ((x1>=VisibleWidth) && (x2>=VisibleWidth))
		return;
	
	static float width;
	static float du, dv, dz;
	//   static int u, v;
	static int dui, dvi,ui,vi;
	static int db;
	
	if(x2 < x1) {
		static float tmp;
		static int t;
		
		tmp = x1; x1 = x2; x2 = tmp;
		tmp = u1; u1 = u2; u2 = tmp;
		tmp = v1; v1 = v2; v2 = tmp;
		tmp = z1; z1 = z2; z2 = tmp;
		t = b1 ; b1 = b2; b2 = t;
	}
	
	width = x2 - x1;
	
	if(width) {
		du = (u2 - u1)/width;
		dv = (v2 - v1)/width;
		dz = (z2 - z1)/width;
		db = (int)((b2 - b1)/width);
	}
	
	if (x1<0)
	{
		u1 -= (float)x1*du;
		v1 -= (float)x1*dv;
		z1 -= (float)x1*dz;
		x1= 0 ;
		db = (int)((b2 - b1)/width);
	}
	if (x2<0)
		x2 = 0;
	if (x2>=VisibleWidth)
		x2 = (float)(VisibleWidth-1);
	
	targ += int(x1);
	ZBuf += int(x1);
	
	dui = (int)(du*65536);
	dvi = (int)(dv*65536);
	ui = (int)(u1 * 65536);
	vi = (int)(v1 * 65536);
	
	int x = int(x2-x1);
	while (x-- >=0)
	{
		
		try
		{
			if (z1 > *ZBuf)
			{
				*targ = Bright[tPtr[OfsY[vi>>16] + (ui>>16)]][b1>>22];
				*ZBuf = z1;
			}
		}
		catch(...)
		{
		}
		/*
		if (z1 > *ZBuf)
		{
		*targ = Bright[tPtr[OfsY[vi>>16] + (ui>>16)]][b1>>22];
		*ZBuf = z1;
		}
		*/
		ui += dui;
		vi += dvi;
		b1 += db;
		
		z1 += dz;
		targ ++;
		ZBuf ++;
		
	}
	
	/*
	if(x2 < x1) {
	float tmp;
	
      tmp = x1; x1 = x2; x2 = tmp;
	  tmp = u1; u1 = u2; u2 = tmp;
	  tmp = v1; v1 = v2; v2 = tmp;
      tmp = z1; z1 = z2; z2 = tmp;
	  }
	  
		float width = x2 - x1;
		float du, dv, dz;
		int u, v;
		
		  if(width) {
		  du = (u2 - u1)/width;
		  dv = (v2 - v1)/width;
		  dz = (z2 - z1)/width;
		  }
		  
			targ += int(x1);
			ZBuf += int(x1);
			int x = int(width);
			for (x = int(x1);x<int(x2);x++)
			{
			u = int(u1);
			v = int(v1);
			
			  if (x>=0 && x<Width)
			  if (z1 > *ZBuf)
			  {
			  *targ = tPtr[OfsY[v] + u];
			  *ZBuf = z1;
			  }
			  
				u1 += du;
				v1 += dv;
				z1 += dz;
				targ ++;
				ZBuf++;
				}
	*/
}

void  TDIBitmap8::DrawTexTriGourad_ZBuf(const TexTriangle &tri)
{
	if (!Enabled)
		return;
	
	int top = 0;                       // index to top vertex
	int a, b;                          // other 2 vertices
	int i;
	
	// interpolants
	
	float dx_A, 			// change in sx with respect to sy
		dx_B,
		du_A,			// change in u with respect to sy
		du_B,
		dv_A,			// change in v with respect to sy
		dv_B,
		dz_A,
		dz_B;
	
	int   db_A,
		db_B;
	
	
	for(i = 1; i < 3; i++) // find top vertex
	{
		if(tri.v[i]->sy < tri.v[top]->sy) top = i;
	}
	
	a = top + 1;
	b = top - 1;
	if(a > 2) a = 0;
	if(b < 0) b = 2;
	
	int   y = (int)(tri.v[top]->sy);
	float x1 = tri.v[top]->sx;
	float x2 = x1;
	float u1 = tri.v[top]->u;
	float u2 = u1;
	float v1 = tri.v[top]->v;
	float	v2 = v1;
	float	z1 = tri.v[top]->sz;
	float z2 = z1;
	
	int b1 = tri.v[top]->br;
	int b2 = b1;
	
	int height_A = (int)(tri.v[a]->sy - tri.v[top]->sy);
	int height_B = (int)(tri.v[b]->sy - tri.v[top]->sy);
	
	if(height_A) // avoid divide by zero
	{
		// calculate the interpolants
		dx_A = (tri.v[a]->sx - tri.v[top]->sx)/ height_A;
		du_A = (tri.v[a]->u  - tri.v[top]->u)/ height_A;
		dv_A = (tri.v[a]->v  - tri.v[top]->v)/ height_A;
		dz_A = (tri.v[a]->sz  - tri.v[top]->sz)/ height_A;
		db_A = (tri.v[a]->br  - tri.v[top]->br)/ height_A;
	}
	
	if(height_B)// avoid divide by zero
	{
		// calculate the interpolants
		dx_B = (tri.v[b]->sx - tri.v[top]->sx)/ height_B;
		du_B = (tri.v[b]->u  - tri.v[top]->u)/ height_B;
		dv_B = (tri.v[b]->v  - tri.v[top]->v)/ height_B;
		dz_B = (tri.v[b]->sz  - tri.v[top]->sz)/ height_B;
		db_B = (tri.v[b]->br  - tri.v[top]->br)/ height_B;
	}
	
	// start drawing
	unsigned char *targ = Data + PInfoHeader->biSizeImage - Width - y*Width;
	float *Ztarg = ZBuffer + y*Width;
	
	for(i = 2; i > 0;) {
		
		while(height_A && height_B)
		{
			if (b1 >= 1073741824)
				b1 = 1073741823;
			
			if (b2 >= 1073741824)
				b2 = 1073741823;
			
			if (y>=0 && y<Height)
				DrawAffineGouradScanline_ZBuf(x1, x2, u1, u2, v1, v2, z1, z2, b1, b2, targ, tri.Tex, Ztarg);
			
			Ztarg += Width;
			targ-=Width;
			y++;
			height_A--;
			height_B--;
			
			x1 += dx_A;                     // add the interpolants
			x2 += dx_B;
			u1 += du_A;
			u2 += du_B;
			v1 += dv_A;
			v2 += dv_B;
			z1 += dz_A;
			z2 += dz_B;
			b1 += db_A;
			b2 += db_B;
		}
		
		if(!height_A)
		{
			// new edge
			
			int na = a + 1;
			if(na > 2) na = 0;
			
			height_A = (int)(tri.v[na]->sy - tri.v[a]->sy);
			
			if(height_A)  // avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_A = (tri.v[na]->sx - tri.v[a]->sx) / height_A;
				du_A = (tri.v[na]->u  - tri.v[a]->u)  / height_A;
				dv_A = (tri.v[na]->v  - tri.v[a]->v)  / height_A;
				dz_A = (tri.v[na]->sz - tri.v[a]->sz) / height_A;
				db_A = (tri.v[na]->br - tri.v[a]->br) / height_A;
			}
			
			x1 = tri.v[a]->sx;
			u1 = tri.v[a]->u;
			v1 = tri.v[a]->v;
			z1 = tri.v[a]->sz;
			b1 = tri.v[a]->br;
			
			i--;                               // one less vertex
			a = na;
			
		} // end if
		
		if(!height_B)
		{
			
			// new edge
			
			int nb = b - 1;
			if(nb < 0) nb = 2;
			
			height_B = (int)(tri.v[nb]->sy - tri.v[b]->sy);
			
			if(height_B)// avoid divide by zero
			{
				// recalculate the interpolants for the new edge
				
				dx_B = (tri.v[nb]->sx - tri.v[b]->sx) / height_B;
				du_B = (tri.v[nb]->u  - tri.v[b]->u)  / height_B;
				dv_B = (tri.v[nb]->v  - tri.v[b]->v)  / height_B;
				dz_B = (tri.v[nb]->sz - tri.v[b]->sz) / height_B;
				db_B = (tri.v[nb]->br - tri.v[b]->br) / height_B;
			}
			
			x2 = tri.v[b]->sx;
			u2 = tri.v[b]->u;
			v2 = tri.v[b]->v;
			z2 = tri.v[b]->sz;
			b2 = tri.v[b]->br;
			
			i--;                               // one less vertex
			b = nb;
			
		} // end if
		
	} // end for loop
	
} // end function



