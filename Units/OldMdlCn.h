//---------------------------------------------------------------------------
#ifndef OldMdlConstH
#define OldMdlConstH
//---------------------------------------------------------------------------

#define PANELWIDTH		100
#define MAXFRAMES			1000
#define MAXSKINS			500
#define REPLACEVERTS		0
#define APPENDVERTS		1

#define crArrowNorm		-21
#define crArrowMinus		-22
#define crArrowRotate	-23
#define crArrowScale		-24
#define crArrowMove		-25
#define crArrowPan		-26
#define crArrowSelect	-27
#define crArrowTri		-28
#define crArrowVert		-29
#define crArrowOrigin	-30

typedef float scalar_t;
typedef unsigned char u_char;

class TOldMdlHeader
{
public:
   long id;                     // 0x4F504449 = "IDPO" for IDPOLYGON
   long version;                // Version = 6
   Vector3 scale;                // Model scale factors.
   Vector3 origin;               // Model origin.
   scalar_t radius;             // Model bounding radius.
   Vector3 offsets;              // Eye position (useless?)
   long numskins ;              // the number of skin textures
   long skinwidth;              // Width of skin texture
                               //           must be multiple of 8
   long skinheight;             // Height of skin texture
                               //           must be multiple of 8
   long numverts;               // Number of vertices
   long numtris;                // Number of triangles surfaces
   long numframes;              // Number of frames
   long synctype;               // 0= synchron, 1= random
   long flags;                  // 0 (see Alias models)
   scalar_t size;               // average size of triangles
};

typedef struct
{
	long onseam;				// 0 or 0x20
  	float s;						// horiz
	float t;						// vert
	bool	Selected;
} TOldRealSkinVertex;

typedef struct
{ long onseam;                 // 0 or 0x20
  long s;
  long t;
} TOldFileSkinVertex;


typedef struct
{ long facesfront;
  long vertices[3];

} TOldFileTri;

class TOldBaseTriangle
{
public:
	long		vertices[3];
   Vector3	Centre;
   Vector3	Normal;
};

class TOldRealTriangle: public TOldBaseTriangle
{
public:
   bool		facesfront,
   			Selected,
            Visible,
            SkinSelected;
};

typedef struct
{ u_char packedposition[3];    // X,Y,Z coordinate, packed on 0-255
  u_char lightnormalindex;     // index of the vertex normal
} TOldFileAnimVertex;

typedef struct
{
	float 	x,y,z;
   bool		Selected,Visible;
} TOldRealVertex;

typedef struct
{
   TOldFileAnimVertex min,max;
   char		  	Name[16];
   float			time;
   bool			StartGroup,EndGroup;
} TOldFrameInfo;

typedef struct
{
   TOldFileAnimVertex min,max;
   char		  	Name[16];
} TOldFileFrameInfo;

typedef struct
{
	bool	StartGroup,EndGroup;
   float	Time;

} TOldGroupInfo;

#endif

