//---------------------------------------------------------------------------
//#define colors_cpp
#include "stdafx.h"

#include "Colors.h"

//---------------------------------------------------------------------------

unsigned char
TICKCOLOR		= 56,
BONECOLOR		= 12,
TEXSELFACE		= 224,
BACKGROUND3D	= DARKGRAY,
BACKGROUND		= DARKGRAY,
WIREFRAME		= BLACK,
WIREFRAME3D		= BLACK,
GRID			= RED,
GRID3D			= RED,
SELVERT			= YELLOW,
SELFACE			= YELLOW,
SELBONE			= YELLOW,
NORMHI			= YELLOW,
NORMLO			= ORANGE,
SELECTBOX		= AQUA,
ORIGIN			= RED,
FACESHADMIN		= 0,
FACESHADMAX 	= 15,
SELFACESHADMIN	= 234,
SELFACESHADMAX	= 224,
SKINWIRE		= DARKGRAY,
SELSKINWIRE		= YELLOW,
SKINVERTEX		= WHITE,
SELSKINVERTEX	= YELLOW,
XAXIS			= RED,
YAXIS			= GREEN,
ZAXIS			= BLUE;

CString  COLORNAMES[] = 
{
	"Tick",
	"Bone",
	"Textured Selection",
	"Background",
	"3D Background ",
	"Wireframe ",
	"3D Wireframe",
	"Grid",
	"3D Grid",
	"Vertex Selection ",
	"Face Selection ",
	"Bone Selection ",
	"Normal Hilight",
	"Normal Lolight",
	"Select Box",
	"Origin",
	//"Darkest Face",
	"3D Face",
	//"Darkest Face Selection",
	"Face Selection",
	"Skin Wireframe",
	"Selected Skin Wireframe",
	"Skin Vertex",
	"Selected Skin Vertex",
	"X Axis",
	"Y Axis",
	"Z Axis"
};

unsigned char *COLORS[] =
{
	&TICKCOLOR,
	&BONECOLOR,
	&TEXSELFACE,
	&BACKGROUND,
	&BACKGROUND3D,
	&WIREFRAME,
	&WIREFRAME3D,
	&GRID,
	&GRID3D,
	&SELVERT,
	&SELFACE,
	&SELBONE,
	&NORMHI,
	&NORMLO,
	&SELECTBOX,
	&ORIGIN,
	//&FACESHADMIN,
	&FACESHADMAX,
	//&SELFACESHADMIN,
	&SELFACESHADMAX,
	&SKINWIRE			,
	&SELSKINWIRE		,
	&SKINVERTEX		,
	&SELSKINVERTEX,
	&XAXIS,
	&YAXIS,
	&ZAXIS
};

int NUM_COLORS = 22;
