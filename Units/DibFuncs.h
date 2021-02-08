//---------------------------------------------------------------------------
#ifndef DibFuncsH
#define DibFuncsH
//---------------------------------------------------------------------------
#define GREYSHIFT		4
#define PCXRLEFLAG	0xC0

#include <windows.h>

typedef unsigned char byte;

typedef struct
{
	int x, y;
	float	z;
} POINT3D;

typedef struct
{
	   float x, y, z;		// (x, y, z) coords. in 3d space
	   float u, v;			// (u, v) texture coordinates
	   float sx, sy, sz;	// (x, y, z) projected into screen space
	   float su, sv;		// (u, v) projected texture coords.
	   int br; 			// 8.24 int for brightness
} TexVertex;

typedef struct
{
	TexVertex		*v[3];
	unsigned char	*Tex;
} TexTriangle;

typedef struct
{
	byte	Manufacturer;	// = 10
	byte	Version;		//  5 = v3.0
	byte	Encoding;	// = 1, RLE
	byte	BitsPerPixel;
	short	XMin,YMin, XMax, YMax; //inclusive
	short	HRes;
	short	VRes;
	byte	ColorMap[48];
	byte	Reserved;
	byte	Planes;
	short	BytesPerLine;	//always even
	byte	PaletteType;	// 1 = color, 2 = greyscale
	byte	Filler[58];
} PCXHeader;

class TDIBitmap8
{
public:
   	int					VisibleWidth, VisibleHeight;
	int					Width, Height;
   	HBITMAP				Handle;
	BITMAPINFO			*PInfo;
	BITMAPINFOHEADER  *PInfoHeader;
	RGBQUAD				*PPalette;
	byte              Bright[256][256];
	
	int 					TexWidth,TexHeight;
	int					*OfsY;
	
	unsigned char		*Data;
	float					*ZBuffer;
	
	bool					Enabled;
	
	int 					OMinX, OMaxX,
		OMinY, OMaxY,
		MinX, MinY,
		MaxX, MaxY;
	
	TDIBitmap8();
	TDIBitmap8(TDIBitmap8 &Src);
	~TDIBitmap8();
	
	int LoadFromBMP(CString FName);
	int LoadFromPCX(CString FName);
	int SaveToBMP(CString FName);
	int SaveToPCX(CString FName);
	
	void  SetPalette(unsigned char SrcPal[256][3], bool DoFilter = false);
	void  SetSize(int w, int h);
	void  PutPixel(int x1, int y1, unsigned char col);
	void  Line(int x1, int y1, int x2, int y2, unsigned char col);
	void  LineXOR(int x1, int y1, int x2, int y2, unsigned char col);
	void  Rect(int x1, int y1, int x2, int y2, unsigned char col);
	void  FilledRect(int x1, int y1, int x2, int y2, unsigned char col);
	void  DrawToDC(HDC destDC, int x, int y);
	//      void  StretchRectToDC(HDC DestDC, int x, int y, int w, int h, int srcw, srch);
	void  StretchToDC(HDC DestDC, int x, int y, int w, int h);
	void  StretchToDC(HDC DestDC, int xdest, int ydest, int wdest, int hdest, int xsrc, int ysrc, int wsrc, int hsrc);
	//      void  StretchToDC(HDC destDC, int x, int y, int w, int h);
	void  Clear(unsigned char col);
	void  Poly(POINT *P, int num, unsigned char col);
	void  FillPoly(POINT *P, int num, unsigned char col);
	
	void  SetTexSize(int a,int b);
	
	
	void  DrawTri_ZBuf(const TexTriangle &tri, unsigned char col);
	void  DrawScanline_ZBuf(float x1, float x2,
		float z1, float z2,unsigned char col,
		unsigned char *targ, float *ZPtr);
	
	void  DrawTexTri(const TexTriangle &tri);
	void  DrawAffineScanline(float x1, float x2, float u1, float u2,
		float v1, float v2, unsigned char *targ,
		unsigned char* tPtr);
	
	void  DrawTexTriGreyscale(const TexTriangle &tri, unsigned char StartCol);
	void  DrawAffineScanlineGreyscale(float x1, float x2, float u1, float u2,
		float v1, float v2, unsigned char *targ,
		unsigned char* tPtr, unsigned char StartCol);
	
	void  DrawTexTri_ZBuf(const TexTriangle &tri);
	void  DrawAffineScanline_ZBuf(float x1, float x2, float u1, float u2,
		float v1, float v2, float z1, float z2, unsigned char *targ,
		unsigned char* tPtr, float *ZBuf);
	
	void  DrawTexTri_ZBuf_UV(const TexTriangle &tri, short* UData, short *VData);
	void  DrawAffineScanline_ZBuf_UV(float x1, float x2, float u1, float u2,
		float v1, float v2, float z1, float z2, unsigned char *targ,
		unsigned char* tPtr, float *ZBuf, short *UD, short *VD);
	
	void  DrawTexTri_ZBuf_UV(const TexTriangle &tri, unsigned char **UVData);
	void  DrawAffineScanline_ZBuf_UV(float x1, float x2, float u1, float u2,
		float v1, float v2, float z1, float z2, unsigned char *targ,
		unsigned char* tPtr, float *ZBuf, unsigned char **UVD);
	
	
	void  Line_ZBuf(int x1, int y1, float z1, int x2, int y2, float z2, unsigned char col);
	
	
	void  DrawTexTri_ZBufGreyScale(const TexTriangle &tri, unsigned char StartCol);
	void  DrawAffineScanline_ZBufGreyScale(float x1, float x2, float u1, float u2,
		float v1, float v2, float z1, float z2, unsigned char *targ,
		unsigned char* tPtr, float *ZPtr, unsigned char StartCol);
	
	void  TDIBitmap8::DrawGouradScanline_ZBuf(float x1, float x2,
		float z1, float z2,  int b1, int b2, unsigned char *targ,
		float *ZBuf);
	void  TDIBitmap8::DrawGouradTri_ZBuf(const TexTriangle &tri);
	
	void  DrawTexTriGourad_ZBuf(const TexTriangle &tri);
	void  DrawAffineGouradScanline_ZBuf(float x1, float x2, float u1, float u2,
		float v1, float v2, float z1, float z2, int b1, int b2, unsigned char *targ,
		unsigned char* tPtr, float *ZBuf);
	
};

#endif
