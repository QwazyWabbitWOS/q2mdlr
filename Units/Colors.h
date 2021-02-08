//---------------------------------------------------------------------------
#ifndef ColorsH
#define ColorsH
//---------------------------------------------------------------------------

#define WHITE     	(unsigned char)254
#define YELLOW  		(unsigned char)221
#define DARKGRAY     (unsigned char)8
#define GRAY1			(unsigned char)6
#define GRAY2			(unsigned char)10
#define LIGHTGRAY		(unsigned char)12
#define AQUA			(unsigned char)112
#define BLACK			(unsigned char)0
#define RED				(unsigned char)251
#define BEIGE			(unsigned char)56
#define ORANGE			(unsigned char)234
#define BLUE         (unsigned char)176
#define PURPLE			(unsigned char)128
#define GREEN        (unsigned char)208

#define MAXCOLORS		128

#ifndef colors_cpp
extern
	unsigned char
		TICKCOLOR,
		BONECOLOR,
		TEXSELFACE,
      BACKGROUND3D,
      BACKGROUND,
      WIREFRAME,
      WIREFRAME3D,
      GRID,
      GRID3D,
      SELVERT,
      SELFACE,
      SELBONE,
      NORMHI,
      NORMLO,
      SELECTBOX,
      ORIGIN,
      FACESHADMIN,
      FACESHADMAX,
      SELFACESHADMIN,
      SELFACESHADMAX,
      SKINWIRE			,
      SELSKINWIRE		,
      SKINVERTEX		,
      SELSKINVERTEX,
      XAXIS,
      YAXIS,
      ZAXIS;

extern unsigned char *COLORS[];
extern int NUM_COLORS;
extern CString COLORNAMES[];




#endif

#endif

