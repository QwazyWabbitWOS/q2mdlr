#include "stdafx.h"

#include <fcntl.h>
#include <io.h>
#include <dos.h>
#include <math.h>
#include <ctype.h>
#include <sys\stat.h>
#include <errno.h>
#include <limits.h>
//#include <dir.h>

#include "LWOHeadr.h"
#include "MDLType.h"
#include "Matrix.h"
#include "MDLGlobs.h"

#include "OldMdlCn.h"

#define MAIN3DS			0x4D4D
#define VER3DS				0x0002
#define EDIT3DS			0x3D3D  // this is the start of the editor config
#define EDIT_CONFIG		0x3d3e
#define EDIT_CONFIG1		0x0100
#define EDIT_OBJECT		0x4000
#define OBJ_TRIMESH		0x4100
#define TRI_VERTEXL		0x4110
#define TRANS_MATRIX		0x4160
#define TRI_FACEL1		0x4120
#define FACE_SMOOTHING	0x4150
#define KF_DATA			0xb000
#define KF_HEAD			0xb00a
#define KF_SEG_INFO		0xb008
#define KF_CURTIME		0xb009
#define OBJECT_NODE_TAG	0xb002
#define NODE_ID			0xb030
#define NODE_HDR			0xb010
#define PIVOT				0xb013
#define POS_TRACK_TAG	0xb020
#define ROT_TRACK_TAG	0xb021
#define SCL_TRACK_TAG   0xb022
#define NO_PARENT			0xffff

#define writestring(H,S) write(H,S,strlen(S))
#define writeln(H) write(H,"\n", 1);
#define writeint(H, I)	  itoa(I,buf,10);write(H,buf,strlen(buf))

int vertsfound=0,trisfound=0,bytesread=0,oldverts=0, framenum;

unsigned long ReverseL(unsigned long V);
unsigned short ReverseW(unsigned short V);
float ReverseF(float F);


TCompleteModel::TCompleteModel()
{
   int i;
   
   Head.id = 0;
   Head.version = 0;
   Head.skinwidth = 0;
   Head.skinheight = 0;
   Head.framesize = 0;
   Head.num_skins = 0;
   Head.num_xyz = 0;
   Head.num_st = 0;
   Head.num_tris = 0;
   Head.num_glcmds = 0;
   Head.num_frames = 0;
   
   for (i=0;i<MAX_MD2SKINS;i++)
   {
      Skins[i] = NULL;
      memset(SkinNames[i],0,16);
   }
   for (i=0;i<MAX_FRAMES;i++)
   {
      Frames[i] = NULL;
   }
   
   SkinVerts = NULL;
   Tris = NULL;
   Verts = NULL;
   FrameInfo = NULL;
   
   Empty = 1;
}

TCompleteModel::TCompleteModel(const TCompleteModel &Src)
{
   memcpy(&Head,&Src.Head, sizeof(TMdlHeader));
   int i;
   
   
   for (i=0;i<MAX_MD2SKINS;i++)
      if (Src.Skins[i] != NULL)
      {
         Skins[i] = new BYTE[Head.skinwidth * Head.skinheight];
         memcpy(Src.Skins[i], Skins[i], Head.skinwidth*Head.skinheight);
      }
      else
         Skins[i] = NULL;
      
      if (Src.SkinVerts != NULL)
      {
         SkinVerts = new TRealSkinVertex[Head.num_st];
         memcpy(SkinVerts, Src.SkinVerts, sizeof(TRealSkinVertex)*Head.num_st);
      }
      else SkinVerts = NULL;
      
      if (Src.Tris != NULL)
      {
         Tris = new TRealTriangle[Head.num_tris];
         memcpy(Tris, Src.Tris, sizeof(TRealTriangle)*Head.num_tris);
      }
      else Tris = NULL;
      
      if (Src.Verts != NULL)
      {
         Verts = new TRealVertex[Head.num_xyz];
         memcpy(Verts, Src.Verts, sizeof(TRealVertex)*Head.num_xyz);
      }
      else Verts = NULL;
      
      for (i=0;i<MAX_FRAMES;i++)
         if (Src.Frames[i] != NULL)
         {
            Frames[i] = new TRealVertex[Head.num_xyz];
            memcpy(Frames[i],Src.Frames[i],sizeof(TRealTriangle)*Head.num_xyz);
         }
         else Frames[i] = NULL;
         
         if (FrameInfo != NULL)
         {
            FrameInfo = new TFrameInfo[Head.num_frames];
            memcpy(FrameInfo, Src.FrameInfo, sizeof(TFrameInfo)*Head.num_frames);
         }
         else FrameInfo = NULL;
         Empty = Src.Empty;
         
         // !!! add in bones copying here !!!
         Bones = Src.Bones;
}

TCompleteModel::~TCompleteModel()
{
   int i;
   for (i=0;i<MAX_MD2SKINS;i++)
   {
      if (Skins[i] != NULL)
         delete[] Skins[i];
      Skins[i] = NULL;
   }
   for (i=0;i<Head.num_frames;i++)
   {
      if (Frames[i] != NULL)
         delete[] Frames[i];
      
      Frames[i] = NULL;
   }
   
   if (SkinVerts != NULL)
      delete[] SkinVerts;
   if (Tris != NULL)
      delete[] Tris;
   if (Verts != NULL)
      delete[] Verts;
   if (FrameInfo != NULL)
      delete[] FrameInfo;
   
   SkinVerts = NULL;
   Tris = NULL;
   Verts = NULL;
   FrameInfo = NULL;
   
   Head.id = 0;
   Head.version = 0;
   Head.num_skins = 0;
   Head.skinwidth = 0;
   Head.skinheight;
   Head.num_xyz = 0;
   Head.num_st = 0;
   Head.num_tris = 0;
   Head.num_frames = 0;
   Head.framesize = 0;
   
   Empty = 1;
   
}

void TCompleteModel::DeallocateAll()
{
   Empty = 1;
   int i;
   
   for (i=0;i<MAX_MD2SKINS;i++)
   {
      if (Skins[i] != NULL)
         delete[] Skins[i];
      Skins[i] = NULL;
   }
   for (i=0;i<Head.num_frames;i++)
   {
      if (Frames[i] != NULL)
         delete[] Frames[i];
      
      Frames[i] = NULL;
   }
   
   if (SkinVerts != NULL)
      delete[] SkinVerts;
   if (Tris != NULL)
      delete[] Tris;
   if (Verts != NULL)
      delete[] Verts;
   if (FrameInfo != NULL)
      delete[] FrameInfo;
   
   SkinVerts = NULL;
   Tris = NULL;
   Verts = NULL;
   FrameInfo = NULL;
   
   
}

void TCompleteModel::AllocateAll(int Dealloc)
{
   int n;
   
   Empty = 0;
   if (Dealloc)
      DeallocateAll();
   Tris = new TRealTriangle[Head.num_tris];
   
   for (n=0;n<Head.num_tris;n++)
      Tris[n].Selected = false;
   
   for (n=0;n<Head.num_frames;n++)
   {
      Frames[n] = new TRealVertex[Head.num_xyz];
   }
   
   Verts = new TRealVertex[Head.num_xyz];
   
   for (n=0;n<Head.num_skins;n++)
      Skins[n] = new BYTE[(long)Head.skinwidth*Head.skinheight];
   
   SkinVerts = new TRealSkinVertex[Head.num_st];
   
   FrameInfo = new TFrameInfo[Head.num_frames];
}

int TCompleteModel::LoadFromBestGuess(CString FileName, bool Merge)
{
   char ext[_MAX_EXT];
   memset(ext, 0, _MAX_EXT);
   int result;

   _splitpath((LPCTSTR)FileName, NULL, NULL, NULL,ext);

   if (stricmp(ext, ".AQM") == 0)
   	if (Merge)
	      result = MergeFromFile(FileName,ftAQM);
      else
	      result = LoadFromAQM(FileName);

   if (stricmp(ext, ".MD2") == 0)
   	if (Merge)
	      result = MergeFromFile(FileName,ftMD2);
      else
	      result = LoadFromMD2(FileName);

   if (stricmp(ext, ".MDL") == 0)
   	if (Merge)
	      result = MergeFromFile(FileName,ftMDL);
      else
	      result = LoadFromMDL(FileName);

   return 1;
}

int TCompleteModel::LoadFromMD2(CString FileName)
{
   TCompleteModel	temp;
   TMdlHeader     tHead;

   int handle;
   
   handle = open((LPCTSTR)FileName,O_BINARY | O_RDONLY);
   
   read(handle, &tHead, sizeof(TMdlHeader));

   if (tHead.id !=IDALIASHEADER || tHead.version !=ALIAS_VERSION)
   {
      // MessageBox("Error, not an MD2 File","Error",MB_OK);
      close(handle);
      return 0;
   }
   
   temp.Head = tHead;
   if (temp.Head.num_skins>MAX_MD2SKINS)
   {
      // Application->MessageBox("Too Many Skins to load. File Open aborted.","Error", MB_OK);
      close(handle);
      return 0;
   }
   if (temp.Head.num_frames>MAX_FRAMES)
   {
      // Application->MessageBox("Too Many Frames to load. File Open aborted.","Error", MB_OK);
      close(handle);
      return 0;
   }
   
   temp.AllocateAll();
   
   int n;
   
   for (n=0;n<temp.Head.num_skins;n++)
   {
      read(handle, temp.SkinNames[n], MAX_SKINNAME);
   }
   
   for (n=0;n<temp.Head.num_skins;n++) // load the skins
   {
	   TDIBitmap8	tempbmp;

      if (tempbmp.LoadFromPCX(QuakeDataDir + CString(temp.SkinNames[n])) == 0)
         continue;
      
      if (tempbmp.Width != temp.Head.skinwidth || tempbmp.Width != temp.Head.skinwidth )
      {
   	   int   res = MessageBox(NULL, "The pcx's width and height are not the same as the current skin, Resize the skinmes?","Size difference",MB_OKCANCEL);
         int   i;

		   if (res == IDCANCEL)
            continue;

         float ratx, raty;

         ratx = (float)tempbmp.Width / temp.Head.skinwidth;
         raty = (float)tempbmp.Height / temp.Head.skinheight;

         for (i=0;i<temp.Head.num_st;i++)
         {
      	   temp.SkinVerts[i].s *= ratx;
      	   temp.SkinVerts[i].t *= raty;
         }
      }
      unsigned char *dest,*d,*s;

      dest = new unsigned char[tempbmp.Width * tempbmp.Height];

      int maxy,maxx;

      maxy = tempbmp.Height;
      maxx = tempbmp.Width;
      if (temp.Head.skinheight<maxy)
         maxy = temp.Head.skinheight;
      if (temp.Head.skinwidth<maxx)
         maxx = temp.Head.skinwidth;

      d = dest;
      s = tempbmp.Data;
      for (int y=0;y<maxy;y++)
      {
         memcpy(d,s,maxx);
         d+=tempbmp.Width;
//            d+=tempbmp.Width%4;
         s+=tempbmp.Width;
         s+=tempbmp.Width%4;
      }

      delete[] temp.Skins[n];
      temp.Skins[n] = dest;

      temp.Head.skinwidth = tempbmp.Width;
      temp.Head.skinheight = tempbmp.Height;
//	   Views[3].SetTexSize(Head.skinwidth, Head.skinheight);
//   	SkinPainterForm->View.SetTexSize(Head.skinwidth, Head.skinheight);
   }
   
   TFileSkinVertex	*tempskinverts;
   
   tempskinverts = new TFileSkinVertex[temp.Head.num_st];
   
   read(handle, tempskinverts, temp.Head.num_st*sizeof(TFileSkinVertex));
   
   int i;
   
   for (i=0;i<temp.Head.num_st;i++)
   {
      temp.SkinVerts[i].s = tempskinverts[i].s;
      temp.SkinVerts[i].t = tempskinverts[i].t;
      
      if (temp.SkinVerts[i].s < 0 )
         temp.SkinVerts[i].s = 0;
      if (temp.SkinVerts[i].s > temp.Head.skinwidth)
         temp.SkinVerts[i].s = 0;
      
      if (temp.SkinVerts[i].t < 0 )
         temp.SkinVerts[i].t = 0;
      if (temp.SkinVerts[i].t > temp.Head.skinheight)
         temp.SkinVerts[i].t = 0;
   }
   
   delete[] tempskinverts;
   
   
   TFileTri *temptris;
   
   temptris = new TFileTri[temp.Head.num_tris];
   read( handle, temptris, (long)temp.Head.num_tris*sizeof(TFileTri));
   
   for (i=0;i<temp.Head.num_tris;i++)
   {
      temp.Tris[i].Vertices[0] = temptris[i].vertices[0];
      temp.Tris[i].Vertices[1] = temptris[i].vertices[1];
      temp.Tris[i].Vertices[2] = temptris[i].vertices[2];
      temp.Tris[i].SkinVerts[0] = temptris[i].skinverts[0];
      temp.Tris[i].SkinVerts[1] = temptris[i].skinverts[1];
      temp.Tris[i].SkinVerts[2] = temptris[i].skinverts[2];
      temp.Tris[i].Selected = false;
      temp.Tris[i].SkinSelected = false;
      temp.Tris[i].Visible = true;
   }

   delete[] temptris;

   TFileAnimVertex		*tempFileFrame;
   TFileFrameInfo			tempFileFrameInfo;
   
   tempFileFrame = new TFileAnimVertex[temp.Head.num_xyz];
   
   for (n = 0;n<temp.Head.num_frames;n++)
   {
      read(handle, &tempFileFrameInfo, sizeof(TFileFrameInfo));
      memcpy(&temp.FrameInfo[n].Name, &tempFileFrameInfo.name, 16);
      
      read(handle, tempFileFrame, sizeof(TFileAnimVertex)*temp.Head.num_xyz);
      
      for (int v=0;v<temp.Head.num_xyz;v++)
      {
         float newx, newy, newz;
         
         newx = tempFileFrame[v].v[0];
         newy = tempFileFrame[v].v[1];
         newz = tempFileFrame[v].v[2];
         newx *= tempFileFrameInfo.scale[0];
         newy *= tempFileFrameInfo.scale[1];
         newz *= tempFileFrameInfo.scale[2];
         newx += tempFileFrameInfo.translate[0];
         newy += tempFileFrameInfo.translate[1];
         newz += tempFileFrameInfo.translate[2];
         
         temp.Frames[n][v].x = newx;
         temp.Frames[n][v].y = newy;
         temp.Frames[n][v].z = newz;
         
         temp.Frames[n][v].Selected = false;
         temp.Frames[n][v].Visible = true;
         
//         temp.Frames[n][v].Strength = 1;
//         temp.Frames[n][v].Parent = NULL;
      }
   }
   
   delete[] tempFileFrame;
   
   close(handle);
   
   DeallocateAll();
   TCompleteModel	*dest;
   dest = this;
   *dest = temp;
   
   return 1;
}

//int TCompleteModel::SaveToMD2(CString FileName, TProgressBar *P, TLabel *L)
int TCompleteModel::SaveToMD2(CString FileName)
{
   int handle;
   
   // do gl commands here
   BuildGlCmds();
   Head.num_glcmds = numcommands;
   
   if (Head.num_skins>MAX_MD2SKINS)
   {
      //		Application->MessageBox("Warning: Too Many Skins to Save. File Save aborted.","Error", MB_OK);
      return 0;
   }
   if (Head.num_frames>MAX_FRAMES)
   {
      //		Application->MessageBox("Warning: Too Many Frames to Save. File Save aborted.","Error", MB_OK);
      return 0;
   }
   if (Head.num_frames>MAX_IDFRAMES)
   {
      //		Application->MessageBox("Warning: Too Many Frames for Quake 2 to Handle.","Error", MB_OK);
   }
   
   {
      CFileDialog    ChooseSkinDlg(TRUE, "PCX", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "PCX Files (*.PCX)|*.PCX");
      
      ChooseSkinDlg.m_ofn.lpstrInitialDir = (LPCTSTR)QuakeDataDir;

      int   BaseDirLen = QuakeDataDir.GetLength();
      int   SkinNameLen;
      
      for (int i=0;i<Head.num_skins;i++)
      {
         int fp;
         
         if (ChooseSkinDlg.DoModal()==IDCANCEL)
            return 0;
         
         
         CString SkinName;
         
         SkinName = ChooseSkinDlg.GetPathName();
         
         SkinNameLen = SkinName.GetLength();
         
         if (BaseDirLen > SkinNameLen) // BaseDir should always be smaller
            // continue
            
            for (fp = 0; fp<BaseDirLen; fp++)
            {
               if (QuakeDataDir[fp] != SkinName[fp])
                  break;
            }
            
            if (fp < BaseDirLen)    // the loop stopped before comparison ended
               continue;
            
            strcpy(SkinNames[i], (LPCTSTR)SkinName.Right(SkinNameLen-BaseDirLen));
      }
   }
   
   handle = open((LPCTSTR)FileName,O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD|S_IWRITE);
   
   if (handle == -1)
   {
      //   	int e = errno;
      return 0;
   }
   
   Head.framesize = sizeof(TFileAnimVertex)*Head.num_xyz + sizeof(TFileFrameInfo);
   {
      int pos = 0;
      
      pos += sizeof(TMdlHeader);
      Head.ofs_skins = pos;
      pos += MAX_SKINNAME*Head.num_skins;
      Head.ofs_st = pos;
      pos += Head.num_st*sizeof(TFileSkinVertex);
      Head.ofs_tris = pos;
      pos += Head.num_tris*sizeof(TFileTri);
      Head.ofs_frames = pos;
      pos += Head.framesize*Head.num_frames;
      Head.ofs_glcmds = pos;
      pos += numcommands*4;
      Head.ofs_end = pos;
   }
   
   
   Head.id = IDALIASHEADER;
   Head.version = ALIAS_VERSION;
   
   int n;
   for (n=0;n<Head.num_skins;n++)
   {
      int i;
      for (i=0;i<MAX_SKINNAME;i++)
      {
         SkinNames[n][i] = (char)tolower(SkinNames[n][i]);
         if (SkinNames[n][i] == '\\')
            SkinNames[n][i] = '/';
      }
      for (i=strlen(SkinNames[n]);i<MAX_SKINNAME;i++)
         SkinNames[n][i] = '\x0';
   }
   
   
   write(handle, &Head, sizeof(TMdlHeader));
   
   
   
   for (n=0;n<Head.num_skins;n++)
   {
      write(handle, SkinNames[n], MAX_SKINNAME);
   }
   
   TFileSkinVertex	*tempskinverts;
   
   tempskinverts = new TFileSkinVertex[Head.num_st];
   
   int i;
   for (i=0;i<Head.num_st;i++)
   {
      if (SkinVerts[i].s < 0 )
         SkinVerts[i].s = 0;
      if (SkinVerts[i].s > Head.skinwidth)
         SkinVerts[i].s = 0;
      
      if (SkinVerts[i].t < 0 )
         SkinVerts[i].t = 0;
      if (SkinVerts[i].t > Head.skinheight)
         SkinVerts[i].t = 0;
      
      tempskinverts[i].s = (short)SkinVerts[i].s;
      tempskinverts[i].t = (short)SkinVerts[i].t;
   }
   
   write(handle, tempskinverts, Head.num_st*sizeof(TFileSkinVertex));
   
   delete[] tempskinverts;
   
   
   TFileTri *temptris;
   
   temptris = new TFileTri[Head.num_tris];
   
   for (i=0;i<Head.num_tris;i++)
   {
      temptris[i].vertices[0] = Tris[i].Vertices[0];
      temptris[i].vertices[1] = Tris[i].Vertices[1];
      temptris[i].vertices[2] = Tris[i].Vertices[2];
      temptris[i].skinverts[0] = Tris[i].SkinVerts[0];
      temptris[i].skinverts[1] = Tris[i].SkinVerts[1];
      temptris[i].skinverts[2] = Tris[i].SkinVerts[2];
   }
   
   write( handle, temptris, (long)Head.num_tris*sizeof(TFileTri));

   delete[] temptris;
   
   TFileAnimVertex		*tempFileFrame;
   TFileFrameInfo			tempFileFrameInfo;
   
   
   tempFileFrame = new TFileAnimVertex[Head.num_xyz];
   for (int f = 0;f<Head.num_frames;f++)
   {
      // Progress bar update
      /*      if (L != NULL)
      {
      L->Caption = FrameInfo[f].Name;
      L->Update();
      }
      if (P != NULL)
      {
      P->Position = (float)100.0*f/Head.num_frames;
      P->Update();
      }
      */    
      memcpy(&tempFileFrameInfo.name, &FrameInfo[f].Name, 16);
      
      Vector3	vmin(100000, 100000, 100000), vmax(-100000,-100000,-100000);
      
      int v;
      for (v=0;v<Head.num_xyz;v++)
      {
         if (Frames[f][v].x < vmin.x)
            vmin.x = Frames[f][v].x;
         if (Frames[f][v].y < vmin.y)
            vmin.y = Frames[f][v].y;
         if (Frames[f][v].z < vmin.z)
            vmin.z = Frames[f][v].z;
         if (Frames[f][v].x > vmax.x)
            vmax.x = Frames[f][v].x;
         if (Frames[f][v].y > vmax.y)
            vmax.y = Frames[f][v].y;
         if (Frames[f][v].z > vmax.z)
            vmax.z = Frames[f][v].z;
      }
      
      Vector3 *trinormals;
      trinormals = new Vector3[Head.num_tris];
      
      Vector3 va,vb,vc,vd,ve,vn;
      
      int n;
      for (n=0;n<Head.num_tris;n++)
      {
         va.Set(Frames[f][Tris[n].Vertices[0]].x,
            Frames[f][Tris[n].Vertices[0]].y,
            Frames[f][Tris[n].Vertices[0]].z);
         vb.Set(Frames[f][Tris[n].Vertices[1]].x,
            Frames[f][Tris[n].Vertices[1]].y,
            Frames[f][Tris[n].Vertices[1]].z);
         vc.Set(Frames[f][Tris[n].Vertices[2]].x,
            Frames[f][Tris[n].Vertices[2]].y,
            Frames[f][Tris[n].Vertices[2]].z);
         
         vb = va-vd;
         vc = vb-ve;
         
         vd = ve/vn;
         
         vn.Normalize();
         trinormals[n][0] = vn[0];
         trinormals[n][1] = vn[1];
         trinormals[n][2] = vn[2];
      }
      
      for (n=0;n<Head.num_xyz;n++)
      {
         Vector3 vn1,vn;
         vn.Set(0,0,0);
         for (int i=0;i<Head.num_tris;i++)
         {
            if( Tris[i].Vertices[0] == n || Tris[i].Vertices[1] == n || Tris[i].Vertices[2] == n )
                  vn += trinormals[i];
         }
         vn.Normalize();
         
         int best=0;
         float bestf=1,newf;
         for (int norm=0;norm<162;norm++)
         {
            vn1.Set(NORMALS[norm].x,NORMALS[norm].y,NORMALS[norm].z);
            
            newf = vn*vn1;
            if (newf<bestf)
            {
               bestf = newf;
               best = norm;
            }
         }
         tempFileFrame[n].lightnormalindex = (BYTE)best;
      }
      
      tempFileFrameInfo.scale[0] = (float)((vmax.x-vmin.x) / 255.0);
      tempFileFrameInfo.scale[1] = (float)((vmax.y-vmin.y) / 255.0);
      tempFileFrameInfo.scale[2] = (float)((vmax.z-vmin.z) / 255.0);
      
      if (tempFileFrameInfo.scale[0] == 0)
         tempFileFrameInfo.scale[0] = 1.0;
      if (tempFileFrameInfo.scale[1] == 0)
         tempFileFrameInfo.scale[1] = 1.0;
      if (tempFileFrameInfo.scale[2] == 0)
         tempFileFrameInfo.scale[2] = 1.0;
      
      tempFileFrameInfo.translate[0] = vmin.x;
      tempFileFrameInfo.translate[1] = vmin.y;
      tempFileFrameInfo.translate[2] = vmin.z;
      
      for (v=0;v<Head.num_xyz;v++)
      {
         float fx,fy,fz;
         
         fx = (float)((Frames[f][v].x-vmin.x)/tempFileFrameInfo.scale[0]+0.5);
         fy = (float)((Frames[f][v].y-vmin.y)/tempFileFrameInfo.scale[1]+0.5);
         fz = (float)((Frames[f][v].z-vmin.z)/tempFileFrameInfo.scale[2]+0.5);
         
         if (fx>255.0)
            fx = 255.0;
         if (fy>255.0)
            fy = 255.0;
         if (fz>255.0)
            fz = 255.0;
         if (fx<0)
            fx = 0;
         if (fy<0)
            fy = 0;
         if (fz<0)
            fz = 0;
         
         tempFileFrame[v].v[0] = (BYTE)fx;
         tempFileFrame[v].v[1] = (BYTE)fy;
         tempFileFrame[v].v[2] = (BYTE)fz;
         
      }
      
      write(handle, &tempFileFrameInfo, sizeof(TFileFrameInfo));
      write(handle, tempFileFrame, sizeof(TFileAnimVertex)*Head.num_xyz);

      delete[] trinormals;
   }
   
   delete[] tempFileFrame;
   
   write(handle, commands, numcommands*4);
   
   close(handle);
   
   return 1;
}

TCompleteModel& TCompleteModel::operator =(const TCompleteModel &Src)
{
   DeallocateAll();
   
   Head = Src.Head;
   
   Tris = new TRealTriangle[Head.num_tris];
   memcpy(Tris,Src.Tris,sizeof(TRealTriangle)*Head.num_tris);
   
   int n;
   
   for (n=0;n<Head.num_frames;n++)
   {
      Frames[n] = new TRealVertex[Head.num_xyz];
      memcpy(Frames[n],Src.Frames[n],sizeof(TRealVertex)*Head.num_xyz);
   }
   
   Verts = new TRealVertex[Head.num_xyz];
   memcpy(Verts,Src.Verts,sizeof(TRealVertex)*Head.num_xyz);
   
   for (n=0;n<Head.num_skins;n++)
   {
      Skins[n] = new BYTE[(long)Head.skinwidth*Head.skinheight];
      memcpy(Skins[n],Src.Skins[n],(long)Head.skinwidth*Head.skinheight);
      memcpy(SkinNames[n], Src.SkinNames[n], MAX_SKINNAME);
   }
   
   SkinVerts = new TRealSkinVertex[Head.num_st];
   memcpy(SkinVerts,Src.SkinVerts,sizeof(TRealSkinVertex)*Head.num_st);
   
   FrameInfo = new TFrameInfo[Head.num_frames];
   memcpy(FrameInfo,Src.FrameInfo,sizeof(TFrameInfo)*Head.num_frames);
   
   Empty = Src.Empty;
   
   return *this;
}


TImportMode::TImportMode()
{
   Mode = imAppendVerts;
}
TImportMode::TImportMode(TMode M, int f)
{
   Mode = M;
   Frame = f;
}

#define CheckPointer(P,E)		if (P == NULL) 	\
{                 	\
   error = E;		\
   break;			\
}

int TCompleteModel::LoadFromASC(CString FileName, TImportMode M)
{
   int handle;
   unsigned long filesize;
   
   handle = open((LPCTSTR)FileName,O_BINARY | O_RDONLY);
   
   if (handle == -1)
   {
      return ascNoFile;
   }
   
   filesize = filelength(handle);
   if (filesize > UINT_MAX-1)
   {
      close(handle);
      return ascFileTooBig;
   }
   
   char 	*file, *text, *res, *pos, vertextag[8], facetag[6];
   int 	numverts, numtris,
      error=1,
      total_numxyz=0, total_numtris=0,
      step_xyz = 0, step_tris = 0;
   
   
   TRealTriangle	*readtris = NULL;
   TRealVertex		*readverts = NULL;
   
   strcpy(vertextag, "vertex ");
   strcpy(facetag, "face ");
   
   file = new char[filesize];
   
   read(handle, file, filesize);
   close(handle);
   
   text = file;
   
   {
      char *txtfile;
      txtfile = file;
      for (unsigned long f=0;f<filesize;f++)
      {
         *txtfile = tolower(*txtfile);
         txtfile++;
      }
   }
   
   while (text != NULL)
   {
      res = strstr(text, "named object:");
      CheckPointer(res,1);
      
      res = strstr(text, "tri-mesh");
      CheckPointer(res,ascNotTriMesh);
      pos = res;
      
      res = strstr(pos, "vertices:");
      CheckPointer(res,ascError);
      
      text = pos = res + 9;
      numverts = atoi(text);
      total_numxyz += numverts;
      
      res = strstr(pos, "faces:");
      CheckPointer(res, ascError);
      
      text = pos = res + 6;
      numtris = atoi(text);
      total_numtris += numtris;
      
      if (numverts == 0)
      {
         continue;
      }
      if (numtris == 0)
      {
         continue;
      }
      
      {
         TRealVertex *tempverts;
         TRealTriangle * temptris;
         
         temptris = new TRealTriangle[total_numtris];
         tempverts = new TRealVertex[total_numxyz];
         
         if (readverts != NULL)
            memcpy(tempverts, readverts, sizeof(TRealVertex)*(total_numxyz-numverts));
         if (readtris != NULL)
            memcpy(temptris, readtris, sizeof(TRealTriangle)*(total_numtris-numtris));
         
         delete[] readtris;
         delete[] readverts;
         
         readverts = tempverts;
         readtris = temptris;
         
         step_xyz = total_numxyz - numverts;
         step_tris = total_numtris - numtris;
      }
      
      {
         char tag[16], *numdest, *d1;
         
         strcpy(tag, vertextag);
         numdest = tag + 7;
         
         for (int i=0;i<numverts;i++)
         {
            itoa(i, numdest, 10);
            d1 = numdest + strlen(numdest);
            *d1 = ':';
            *(d1+1) = '\x0';
            
            res = strstr(pos, tag);
            CheckPointer(res, ascError);
            
            text = res + strlen(tag);
            res = strchr(text, 'x');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readverts[i+step_xyz].x = (float)atof(text);
            
            res = strchr(text, 'y');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readverts[i+step_xyz].y = (float)atof(text);
            
            res = strchr(text, 'z');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readverts[i+step_xyz].z = (float)atof(text);
            
         }
      }
      
      {
         char tag[16], *numdest, *d1;
         
         strcpy(tag, facetag);
         numdest = tag + 5;
         
         for (int i=0;i<numtris;i++)
         {
            itoa(i, numdest, 10);
            d1 = numdest + strlen(numdest);
            *d1 = ':';
            *(d1+1) = '\x0';
            
            res = strstr(pos, tag);
            CheckPointer(res, ascError);
            
            text = res + strlen(tag);
            res = strchr(text, 'a');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readtris[i+step_tris].Vertices[2] = (short)(step_xyz+atoi(text));
            
            res = strchr(text, 'b');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readtris[i+step_tris].Vertices[1] = (short)(step_xyz+atoi(text));
            
            res = strchr(text, 'c');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readtris[i+step_tris].Vertices[0] = (short)(step_xyz+atoi(text));
            
            readtris[i+step_tris].SkinVerts[0] = 0;
            readtris[i+step_tris].SkinVerts[1] = 0;
            readtris[i+step_tris].SkinVerts[2] = 0;
         }
      }
   }
   
   numverts = total_numxyz;
   numtris = total_numtris;
   
   if (M.Mode == imAppendFrame && (numtris != Head.num_tris || numverts != Head.num_xyz))
      error = ascVertTriDiff;
   
   if (error!=1)
   {
      delete[] file;
      delete[] readtris;
      delete[] readverts;
      
      return error;
   }
   
   if (M.Mode == imReplaceFrame)
   {
      if (Head.num_xyz != numverts || Head.num_tris != numtris)
      {
         delete[] readtris;
         delete[] readverts;
         return ascVertTriDiff;
      }
      
      for (int i=0;i<numverts;i++)
      {
         Verts[i] = readverts[i];
      }
      
      char name[256];
      
      _splitpath((LPCTSTR)FileName,NULL,NULL,name,NULL);
      if (strlen(name)>15)
      {
         memcpy(FrameInfo[0].Name, name,15);
         FrameInfo[0].Name[15] = '\x0';
      }
      else
         strcpy(FrameInfo[0].Name, name);
   }
   
   if (M.Mode == imAppendFrame)
   {
      if (Head.num_xyz != numverts || Head.num_tris != numtris)
      {
         delete[] readtris;
         delete[] readverts;
         return ascVertTriDiff;
      }
      
      Frames[Head.num_frames] = new TRealVertex[numverts];
      for (int i=0;i<numverts;i++)
      {
         Frames[Head.num_frames][i] = readverts[i];
      }
      
      TFrameInfo	*tempframeinfo;
      
      tempframeinfo = new TFrameInfo[Head.num_frames+1];
      for (int n=0;n<Head.num_frames;n++)
         memcpy(tempframeinfo[n].Name, FrameInfo[n].Name, 16);
      
      delete[] FrameInfo;
      FrameInfo = tempframeinfo;
      
      char name[256];
      _splitpath((LPCTSTR)FileName,NULL,NULL,name,NULL);
      if (strlen(name)>15)
      {
         memcpy(FrameInfo[Head.num_frames].Name, name,15);
         FrameInfo[Head.num_frames].Name[15] = '\x0';
      }
      else
         strcpy(FrameInfo[Head.num_frames].Name, name);
      
      Head.num_frames ++;
      //      g_CurFrame = =
   }
   
   if (M.Mode == imAppendVerts)
   {
      TRealTriangle	*temptris;
      TRealVertex		*tempverts;
      
      delete[] Verts;
      
      temptris = new TRealTriangle[Head.num_tris + numtris];
      
      if (Head.num_tris != 0)
         memcpy(temptris,Tris, Head.num_tris*sizeof(TRealTriangle));
      
      for (int i=0;i<numtris;i++)
      {
         readtris[i].Vertices[0] += (short)Head.num_xyz;
         readtris[i].Vertices[1] += (short)Head.num_xyz;
         readtris[i].Vertices[2] += (short)Head.num_xyz;
         readtris[i].SkinVerts[0] = 0;
         readtris[i].SkinVerts[1] = 0;
         readtris[i].SkinVerts[2] = 0;
         temptris[Head.num_tris+i] = readtris[i];
      }
      
      delete[] Tris;
      Tris = temptris;
      
      for (int f=0;f<Head.num_frames;f++)
      {
         tempverts = new TRealVertex[Head.num_xyz + numverts];
         memcpy(tempverts, Frames[f], Head.num_xyz*sizeof(TRealVertex));
         for (int i=0;i<numverts;i++)
            tempverts[Head.num_xyz + i] = readverts[i];
         
         Frames[f] = tempverts;
      }
      
      
      Head.num_xyz += numverts;
      Head.num_tris += numtris;
      
      Verts = new TRealVertex[Head.num_xyz];
      memcpy(Verts, Frames[g_CurFrame], Head.num_xyz*sizeof(TRealVertex));
   }
   
   delete[] readtris;
   delete[] readverts;
   
   delete[] file;
   
   return 0;
}

int TCompleteModel::LoadSkinMeshFromASC(CString FileName)
{
   int handle;
   unsigned long filesize;
   
   handle = open((LPCTSTR)FileName,O_BINARY | O_RDONLY);
   
   if (handle == -1)
   {
      return ascNoFile;
   }
   
   filesize = filelength(handle);
   if (filesize > UINT_MAX-1)
   {
      close(handle);
      return ascFileTooBig;
   }
   
   char 	*file, *text, *res, *pos, vertextag[8], facetag[6];
   int 	numverts, numtris,
      error=1,
      total_numst=0, total_numtris=0,
      step_st = 0, step_tris = 0;
   
   
   TRealTriangle	*readtris = NULL;
   TRealSkinVertex		*readverts = NULL;
   
   strcpy(vertextag, "vertex ");
   strcpy(facetag, "face ");
   
   file = new char[filesize];
   
   read(handle, file, filesize);
   close(handle);
   
   text = file;
   
   {
      char *txtfile;
      txtfile = file;
      for (unsigned long f=0;f<filesize;f++)
      {
         *txtfile = tolower(*txtfile);
         txtfile++;
      }
   }
   
   
   while (text != NULL)
   {
      res = strstr(text, "named object:");
      CheckPointer(res,1);
      
      res = strstr(text, "tri-mesh");
      CheckPointer(res,ascNotTriMesh);
      pos = res;
      
      res = strstr(pos, "vertices:");
      CheckPointer(res,ascError);
      
      text = pos = res + 9;
      numverts = atoi(text);
      total_numst += numverts;
      
      res = strstr(pos, "faces:");
      CheckPointer(res, ascError);
      
      text = pos = res + 6;
      numtris = atoi(text);
      total_numtris += numtris;
      
      if (numverts == 0)
      {
         continue;
      }
      if (numtris == 0)
      {
         continue;
      }
      
      {
         TRealSkinVertex *tempverts;
         TRealTriangle * temptris;
         
         temptris = new TRealTriangle[total_numtris];
         tempverts = new TRealSkinVertex[total_numst];
         
         if (readverts != NULL)
            memcpy(tempverts, readverts, sizeof(TRealSkinVertex)*(total_numst-numverts));
         if (readtris != NULL)
            memcpy(temptris, readtris, sizeof(TRealTriangle)*(total_numtris-numtris));
         
         delete[] readtris;
         delete[] readverts;
         
         readverts = tempverts;
         readtris = temptris;
         
         step_st = total_numst - numverts;
         step_tris = total_numtris - numtris;
      }
      
      {
         char tag[16], *numdest, *d1;
         
         strcpy(tag, vertextag);
         numdest = tag + 7;
         
         for (int i=0;i<numverts;i++)
         {
            itoa(i, numdest, 10);
            d1 = numdest + strlen(numdest);
            *d1 = ':';
            *(d1+1) = '\x0';
            
            res = strstr(pos, tag);
            CheckPointer(res, ascError);
            
            text = res + strlen(tag);
            res = strchr(text, 'x');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readverts[i+step_st].s = (float)atof(text);
            
            res = strchr(text, 'y');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readverts[i+step_st].t = (float)atof(text);
            
            res = strchr(text, 'z');
            CheckPointer(res, ascError);
            
            text = res + 2;
            //            readverts[i+step_st].z = atof(text);
            
         }
      }
      
      {
         char tag[16], *numdest, *d1;
         
         strcpy(tag, facetag);
         numdest = tag + 5;
         
         for (int i=0;i<numtris;i++)
         {
            itoa(i, numdest, 10);
            d1 = numdest + strlen(numdest);
            *d1 = ':';
            *(d1+1) = '\x0';
            
            res = strstr(pos, tag);
            CheckPointer(res, ascError);
            
            text = res + strlen(tag);
            res = strchr(text, 'a');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readtris[i+step_tris].SkinVerts[0] = (short)(step_st+atoi(text));
            
            res = strchr(text, 'b');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readtris[i+step_tris].SkinVerts[1] = (short)(step_st+atoi(text));
            
            res = strchr(text, 'c');
            CheckPointer(res, ascError);
            
            text = res + 2;
            readtris[i+step_tris].SkinVerts[2] = (short)(step_st+atoi(text));
         }
      }
   }
   
   numverts = total_numst;
   numtris = total_numtris;
   
   if (Head.num_tris != numtris)
      error = ascVertTriDiff;
   
   if (error!=1)
   {
      delete[] file;
      delete[] readtris;
      delete[] readverts;
      
      return error;
   }
   
   for (int f=0;f<Head.num_tris;f++)
   {
      Tris[f].SkinVerts[0] = readtris[f].SkinVerts[0];
      Tris[f].SkinVerts[1] = readtris[f].SkinVerts[1];
      Tris[f].SkinVerts[2] = readtris[f].SkinVerts[2];
   }
   
   Head.num_st = numverts;
   delete[] SkinVerts;
   SkinVerts = readverts;
   
   delete[] readtris;
   
   delete[] file;
   
   return 0;
}

#undef CheckPointer

int TCompleteModel::LoadFromAQM(CString FileName)
{
   TCompleteModel temp;
   
   int handle;
   char header[AQMHeaderSize];
   int   IsOldHeader = 0;
   
   handle = open((LPCTSTR)FileName, O_BINARY | O_RDONLY);
   if (handle == -1)
      return 0;
   
   read(handle, header, AQMHeaderSize);
   
   if (memcmp(header, AQMHeader, AQMHeaderSize) != 0)
   {
      if (memcmp(header, AQMOldHeader, AQMHeaderSize) != 0)
         return 0;

      IsOldHeader = 1;
   }
   
   read(handle, &temp.Head, sizeof(TMdlHeader));
   
   temp.AllocateAll();
   
   int i;
   
   for (i=0;i<temp.Head.num_skins;i++)
   {
      if (temp.Skins[i] != NULL)
      {
         read(handle, temp.Skins[i], temp.Head.skinwidth*temp.Head.skinheight);
      }
   }
   
   for (i=0;i<temp.Head.num_skins;i++)
      read(handle, temp.SkinNames[i], MAX_SKINNAME);
   
   read(handle, temp.SkinVerts, sizeof(TRealSkinVertex)*temp.Head.num_st);
   
   read(handle, temp.Tris, sizeof(TRealTriangle) * temp.Head.num_tris);
   
   read(handle, temp.FrameInfo, sizeof(TFrameInfo) * temp.Head.num_frames);

// The only thing that has changed is the vertex structure. Bone info has been removed.
   if (IsOldHeader == 0)
   {
      for (i=0;i<temp.Head.num_frames;i++)
         read(handle, temp.Frames[i], sizeof(TRealVertex)*temp.Head.num_xyz);
   }
   else
   {
      TOldAQMRealVertex *ReadVerts;

      ReadVerts = new TOldAQMRealVertex[temp.Head.num_xyz];
      for (i=0;i<temp.Head.num_frames;i++)
      {
         read(handle, ReadVerts, sizeof(TOldAQMRealVertex)*temp.Head.num_xyz);

         for (int v=0;v<temp.Head.num_xyz;v++)
         {
            temp.Frames[i][v].x = ReadVerts[v].x;
            temp.Frames[i][v].y = ReadVerts[v].y;
            temp.Frames[i][v].z = ReadVerts[v].z;
            temp.Frames[i][v].Visible = ReadVerts[v].Visible;
            temp.Frames[i][v].Selected = ReadVerts[v].Selected;
         }
      }
   }
   
   // add in bone reading here
   
   close(handle);
   
   *this = temp;
   
   return 1;
}

int TCompleteModel::SaveToAQM(CString FileName)
{
   int handle;
   
   handle = open((LPCTSTR)FileName, O_BINARY | O_CREAT | O_TRUNC | O_WRONLY, S_IREAD | S_IWRITE);
   if (handle == -1)
      return 0;
   
   write(handle, AQMHeader, AQMHeaderSize);
   write(handle, &Head, sizeof(TMdlHeader));
   
   int i;

   for (i=0;i<Head.num_skins;i++)
      if (Skins[i] != NULL)
         write(handle, Skins[i], Head.skinwidth*Head.skinheight);
      
      for (i=0;i<Head.num_skins;i++)
         write(handle, SkinNames[i], MAX_SKINNAME);
      
      if (SkinVerts != NULL)
         write(handle, SkinVerts, sizeof(TRealSkinVertex)*Head.num_st);
      
      if (Tris != NULL)
         write(handle, Tris, sizeof(TRealTriangle) * Head.num_tris);
      
      if (FrameInfo != NULL)
         write(handle, FrameInfo, sizeof(TFrameInfo) * Head.num_frames);
      
      for (i=0;i<Head.num_frames;i++)
         if (Frames[i] != NULL)
            write(handle, Frames[i], sizeof(TRealVertex)*Head.num_xyz);
         
         // add in bone writing here
         
         close(handle);
         
         return 1;
}

int TCompleteModel::LoadFromMDL(CString FileName)
{
   TOldFileAnimVertex		*tempFileFrame;
   
   TCompleteModel	temp;
   TOldMdlHeader	head;
   int ActualSkinCount, ActualFrameCount;
   int handle;
   long group=0;
   int SkinsRead = 0;
   bool *facesfront;
   
   handle = open((LPCTSTR)FileName,O_BINARY | O_RDONLY);
   
   read(handle, &head, sizeof(TOldMdlHeader));
   if (head.id !=0x4F504449 || head.version !=6)
   {
//      Application->MessageBox("Error, not an MDL File","Error",MB_OK);
      close(handle);
      return 0;
   }
   
   if (head.numskins>MAXSKINS)
   {
//      Application->MessageBox("Too Many Skins to load. File Open aborted.","Error", MB_OK);
      close(handle);
      return 0;
   }
   if (head.numframes>MAX_FRAMES)
   {
//      Application->MessageBox("Too Many Frames to load. File Open aborted.","Error", MB_OK);
      close(handle);
      return 0;
   }
   
   temp.Head.skinwidth = head.skinwidth;
   temp.Head.skinheight = head.skinheight;
   temp.Head.num_skins = head.numskins;
   temp.Head.num_xyz = head.numverts;
   temp.Head.num_st = 0;
   temp.Head.num_tris = head.numtris;
   temp.Head.num_frames = head.numframes;
   
   temp.AllocateAll();
   
   ActualSkinCount = head.numskins;
   
   int N;
   for (N=0;N<ActualSkinCount;N++)
   {
      read(handle,&group,sizeof(long));
      
      if (group == 0)
      {
         temp.Skins[SkinsRead] = new unsigned char[temp.Head.skinwidth*temp.Head.skinheight];
         read(handle, temp.Skins[SkinsRead], (long)temp.Head.skinwidth*temp.Head.skinheight);
         SkinsRead++;
      }
      
      
      if (group == 1)
      {
         long nb;
         
         read(handle,&nb,sizeof(long));
         
         if (SkinsRead+nb > MAXSKINS)
         {
//            Application->MessageBox("Too Many Skins to load. File Open aborted.","Error", MB_OK);
            temp.DeallocateAll();
            close(handle);
            return 0;
         }
         
         int i;
         for (i=0;i<nb;i++)
         {
            lseek(handle, sizeof(float), SEEK_CUR);
            //	         read(handle,&temp.SkinGroupInfo[SkinsRead+i],sizeof(float));
         }
         
         for (i=0;i<nb;i++)
         {
            temp.Skins[SkinsRead] = new unsigned char[ temp.Head.skinwidth*temp.Head.skinheight ];
            read(handle,temp.Skins[SkinsRead++],(long)temp.Head.skinwidth*temp.Head.skinheight);
         }
      }
   }
   
   temp.Head.num_skins = SkinsRead;
   
   TOldFileSkinVertex	*tempskinverts;
   TOldRealSkinVertex	*tempsverts;
   
   tempskinverts = new TOldFileSkinVertex[head.numverts];
   tempsverts = new TOldRealSkinVertex[head.numverts];
   
   read(handle, tempskinverts, head.numverts*sizeof(TOldFileSkinVertex));

   int i;
   for (i=0;i<head.numverts;i++)
   {
      tempsverts[i].s = (float)tempskinverts[i].s;
      tempsverts[i].t = (float)tempskinverts[i].t;
      
      if (tempsverts[i].s < 0 )
         tempsverts[i].s = 0;
      if (tempsverts[i].s > temp.Head.skinwidth)
         tempsverts[i].s = 0;
      
      if (tempsverts[i].t < 0 )
         tempsverts[i].t = 0;
      if (tempsverts[i].t > temp.Head.skinheight)
         tempsverts[i].t = 0;
      
      tempsverts[i].onseam = tempskinverts[i].onseam;
   }
   
   delete[] tempskinverts;
   
   TOldFileTri *temptris;
   
   facesfront = new bool[head.numtris];
   temptris = new TOldFileTri[head.numtris];
   read( handle, temptris, (long)head.numtris*sizeof(TOldFileTri));
   
   for (i=0;i<head.numtris;i++)
   {
      if (temptris[i].facesfront)
         facesfront[i] = true;
      else facesfront[i] = false;
      temp.Tris[i].Vertices[0] = (short)temptris[i].vertices[0];
      temp.Tris[i].Vertices[1] = (short)temptris[i].vertices[1];
      temp.Tris[i].Vertices[2] = (short)temptris[i].vertices[2];
      temp.Tris[i].Selected = false;
   }
   
   
   ActualFrameCount = head.numframes;
   
   tempFileFrame = new TOldFileAnimVertex[head.numverts];
   
   
   
   int FramesRead = 0;
   for (N=0;N<ActualFrameCount;N++)
   {
      read (handle,&group,sizeof(long));
      
      if (group == 0)
      {
         TOldFileFrameInfo tempframeinfo;
         
         read(handle, &tempframeinfo,sizeof(TOldFileFrameInfo));
         
         read(handle,tempFileFrame,(long)sizeof(TOldFileAnimVertex)*head.numverts);
         
         memcpy(temp.FrameInfo[FramesRead].Name,tempframeinfo.Name,16);
         
         temp.Frames[FramesRead] = new TRealVertex[temp.Head.num_xyz];
         
         for (int v=0;v<head.numverts;v++)
         {
            temp.Frames[FramesRead][v].x = (float)(tempFileFrame[v].packedposition[0])*head.scale.x;
            temp.Frames[FramesRead][v].y = (float)(tempFileFrame[v].packedposition[1])*head.scale.y;
            temp.Frames[FramesRead][v].z = (float)(tempFileFrame[v].packedposition[2])*head.scale.z;
            temp.Frames[FramesRead][v].Selected = false;
         }
         FramesRead ++;
      }
      else
      {
         read (handle,&group,sizeof(long));
         if (group>1)
         {
            TFrameInfo		*tempframeinfo;
            TOldFileFrameInfo tempfileinfo;
            
            tempframeinfo = new TFrameInfo[head.numframes + group -1 ];
            memcpy(tempframeinfo,temp.FrameInfo,sizeof(TFrameInfo)*FramesRead);
            memcpy(&tempframeinfo[FramesRead+group-1],&temp.FrameInfo[FramesRead],sizeof(TFrameInfo)*(head.numframes-FramesRead));
            
            delete[] temp.FrameInfo;
            
            temp.FrameInfo = tempframeinfo;
            

            int i;

            for (i=head.numframes+group-1;i>=FramesRead+group-1;i--)
            {
               temp.Frames[i] = temp.Frames[i-group+1];
            }
            for (i=FramesRead;i<FramesRead+group-1;i++)
            {
               temp.Frames[i] = new TRealVertex[head.numverts];
            }
            
            lseek(handle,2*sizeof(TFileAnimVertex),SEEK_CUR); // skip past min and max
            
            for (i=0;i<group;i++)
            {
               lseek(handle, sizeof(float), SEEK_CUR);
               //            	read(handle, &temp.FrameInfo[FramesRead + i].time, sizeof(float));
            }
            
            for (i=0;i<group;i++)
            {
               read(handle, &tempfileinfo,sizeof(TOldFileFrameInfo));
               
               read(handle,tempFileFrame,(long)sizeof(TOldFileAnimVertex)*head.numverts);
               
               memcpy(temp.FrameInfo[FramesRead].Name,tempfileinfo.Name,16);
               
               temp.Frames[FramesRead] = new TRealVertex[temp.Head.num_xyz];
               
               for (int v=0;v<head.numverts;v++)
               {
                  temp.Frames[FramesRead][v].x = (float)(tempFileFrame[v].packedposition[0])*head.scale.x;
                  temp.Frames[FramesRead][v].y = (float)(tempFileFrame[v].packedposition[1])*head.scale.y;
                  temp.Frames[FramesRead][v].z = (float)(tempFileFrame[v].packedposition[2])*head.scale.z;
                  temp.Frames[FramesRead][v].Selected = false;
               }
               FramesRead++;
               
            }
            head.numframes += group-1;
         }
         else
         {
            lseek(handle,2*sizeof(TFileAnimVertex),SEEK_CUR); // skip past min and max
            lseek(handle,sizeof(float),SEEK_CUR);
            
            TOldFileFrameInfo tempfileinfo;
            
            read(handle, &tempfileinfo,sizeof(TOldFileFrameInfo));
            read(handle,tempFileFrame,(long)sizeof(TOldFileAnimVertex)*head.numverts);
            
            memcpy(temp.FrameInfo[FramesRead].Name,tempfileinfo.Name,16);
            
            for (int v=0;v<head.numverts;v++)
            {
               temp.Frames[FramesRead][v].x = (float)(tempFileFrame[v].packedposition[0])*head.scale.x;
               temp.Frames[FramesRead][v].y = (float)(tempFileFrame[v].packedposition[1])*head.scale.y;
               temp.Frames[FramesRead][v].z = (float)(tempFileFrame[v].packedposition[2])*head.scale.z;
               temp.Frames[FramesRead][v].Selected = false;
            }
            
         }
      }
   }
   
   close(handle);
   
   
   temp.Head.num_frames = FramesRead;
   
   for (int f=0;f<temp.Head.num_frames;f++)
      for (int v=0;v<temp.Head.num_xyz;v++)
      {
         temp.Frames[f][v].x += head.origin.x;
         temp.Frames[f][v].y += head.origin.y;
         temp.Frames[f][v].z += head.origin.z;
      }
      
      
      int *toseam;
      int seamcount = head.numverts;
      
      toseam = new int[head.numverts];
      
      temp.Head.num_st = head.numverts;
      for (i=0;i<head.numverts;i++)
      {
         if (tempsverts[i].onseam)
         {
            toseam[i] = seamcount;
            seamcount++;
            temp.Head.num_st ++;
         }
         else
            toseam[i] = 0;
      }
      
      temp.SkinVerts = new TRealSkinVertex[temp.Head.num_st];
      
      for (i=0;i<head.numtris;i++)
      {
         temp.SkinVerts[temp.Tris[i].Vertices[0]].s = tempsverts[temp.Tris[i].Vertices[0]].s;
         temp.SkinVerts[temp.Tris[i].Vertices[0]].t = tempsverts[temp.Tris[i].Vertices[0]].t;
         temp.SkinVerts[temp.Tris[i].Vertices[1]].s = tempsverts[temp.Tris[i].Vertices[1]].s;
         temp.SkinVerts[temp.Tris[i].Vertices[1]].t = tempsverts[temp.Tris[i].Vertices[1]].t;
         temp.SkinVerts[temp.Tris[i].Vertices[2]].s = tempsverts[temp.Tris[i].Vertices[2]].s;
         temp.SkinVerts[temp.Tris[i].Vertices[2]].t = tempsverts[temp.Tris[i].Vertices[2]].t;
         if (facesfront[i])
         {
            temp.Tris[i].SkinVerts[0] = temp.Tris[i].Vertices[0];
            temp.Tris[i].SkinVerts[1] = temp.Tris[i].Vertices[1];
            temp.Tris[i].SkinVerts[2] = temp.Tris[i].Vertices[2];
         }
         else
         {
            if (tempsverts[temp.Tris[i].Vertices[0]].onseam)
               temp.Tris[i].SkinVerts[0] = (short)toseam[temp.Tris[i].Vertices[0]];
            else
               temp.Tris[i].SkinVerts[0] = (short)temp.Tris[i].Vertices[0];
            
            if (tempsverts[temp.Tris[i].Vertices[1]].onseam)
               temp.Tris[i].SkinVerts[1] = (short)toseam[temp.Tris[i].Vertices[1]];
            else
               temp.Tris[i].SkinVerts[1] = (short)temp.Tris[i].Vertices[1];
            
            if (tempsverts[temp.Tris[i].Vertices[2]].onseam)
               temp.Tris[i].SkinVerts[2] = (short)toseam[temp.Tris[i].Vertices[2]];
            else
               temp.Tris[i].SkinVerts[2] = (short)temp.Tris[i].Vertices[2];
            
         }
         
      }
      
      for (i=0;i<head.numverts;i++)
      {
         if (tempsverts[i].onseam)
         {
            temp.SkinVerts[toseam[i]].s = tempsverts[i].s + head.skinwidth/2;
            temp.SkinVerts[toseam[i]].t = tempsverts[i].t;
         }
      }
      
      delete[] toseam;
      
      DeallocateAll();
      TCompleteModel	*dest;
      dest = this;
      *dest = temp;
      
      delete[] tempFileFrame;
      
      delete[] facesfront;
      delete[] tempsverts;
      
      return 1;
}

//int TCompleteModel::SaveToMDL(CString FileName, TProgressBar *P, TLabel *L)
int TCompleteModel::SaveToMDL(CString FileName)
{
   TOldFileAnimVertex		*FileFrames[MAXFRAMES];
   TOldMdlHeader				OldHead;
   TOldFrameInfo				*OldFrameInfo;
   
   OldHead.id = 0x4F504449;
   OldHead.version = 6;
   OldHead.numskins = Head.num_skins;
   OldHead.skinwidth = Head.skinwidth;
   OldHead.skinheight = Head.skinheight;
   OldHead.numverts = Head.num_xyz;
   OldHead.numtris = Head.num_tris;
   OldHead.numframes = Head.num_frames;
   OldHead.synctype = 0;
   OldHead.flags = 0;
   
   OldFrameInfo = new TOldFrameInfo[Head.num_frames];
   
   int i,n;
   
   for (n=0;n<Head.num_frames;n++)
      memcpy(OldFrameInfo[n].Name, FrameInfo[n].Name,16);
   
   
   for (i = 0;i<OldHead.numframes;i++)
      FileFrames[i] = new TOldFileAnimVertex[OldHead.numverts];
   
   if (OldHead.numframes == 0)
   {
      if (MessageBox(NULL, "The Current Model has no Frames. Continue saving?","No Frames", MB_YESNO) == IDNO)
         return 0;
   }
   
   if (OldHead.numskins == 0)
   {
      if (MessageBox(NULL, "The Current Model has no Skins. Continue saving?","No Skins", MB_YESNO) == IDNO)
         return 0;
   }
   
   if (OldHead.numverts == 0)
   {
      if (MessageBox(NULL, "The Current Model has no Vertices. Continue saving?","No Vertices", MB_YESNO) == IDNO)
         return 0;
   }
   
   if (OldHead.numtris == 0)
   {
      if (MessageBox(NULL, "The Current Model has no Faces defined. Continue saving?","No Faces", MB_YESNO) == IDNO)
         return 0;
   }
   
   Vector3 va,vb,vc,vd,ve,vn;
   Vector3 *trinormals;
   float minx,maxx,miny,maxy,minz,maxz;
   
   trinormals = new Vector3[OldHead.numtris];
   
   //               if (P != NULL)
   //                  P->Position = 0;
   int f;
   for (f=0;f<OldHead.numframes;f++)
   {
   /*                  if (L != NULL)
   {
   L->Caption = FrameInfo[f].Name;
   L->Update();
   }
   if (P != NULL)
   {
   P->Position = (float)100.0*f/Head.num_frames;
   P->Update();
   }
      */                 
      int n;
      for (n=0;n<OldHead.numtris;n++)
      {
         va.Set(Frames[f][Tris[n].Vertices[0]].x,
            Frames[f][Tris[n].Vertices[0]].y,
            Frames[f][Tris[n].Vertices[0]].z);
         vb.Set(Frames[f][Tris[n].Vertices[1]].x,
            Frames[f][Tris[n].Vertices[1]].y,
            Frames[f][Tris[n].Vertices[1]].z);
         vc.Set(Frames[f][Tris[n].Vertices[2]].x,
            Frames[f][Tris[n].Vertices[2]].y,
            Frames[f][Tris[n].Vertices[2]].z);
         
         vd = vb-va;
         ve = vc-vb;
         
         //         Normalize(vd);
         //         Normalize(ve);
         vn = vd/ve;
         
         vn.Normalize();
         trinormals[n] = vn;
         
      }
      for (n=0;n<OldHead.numverts;n++)
      {
         Vector3 vn1,vn;
         vn.Set(0,0,0);
         for (int i=0;i<OldHead.numtris;i++)
         {
            if(	Tris[i].Vertices[0] == n ||
               Tris[i].Vertices[1] == n ||
               Tris[i].Vertices[2] == n )
               vn += trinormals[i];
            
         }
         vn.Normalize();
         
         int best=0;
         float bestf=-1,newf;
         for (int norm=0;norm<162;norm++)
         {
            vn1.Set(NORMALS[norm].x,
               NORMALS[norm].y,
               NORMALS[norm].z);
            
            newf = vn*vn1;
            if (newf>bestf)
            {
               bestf = newf;
               best = norm;
            }
         }
         FileFrames[f][n].lightnormalindex = (BYTE)best;
      }
      
   }
   
   OldHead.origin.x = 0;
   OldHead.origin.y = 0;
   OldHead.origin.z = 0;
   
   {
      minx=Verts[0].x;
      maxx=Verts[0].x;
      miny=Verts[0].y;
      maxy=Verts[0].y;
      minz=Verts[0].z;
      maxz=Verts[0].z;
      
      for (int f=0;f<OldHead.numframes;f++)
         for (int n=0;n<OldHead.numverts;n++)
         {
            if (Frames[f][n].x<minx) minx = Frames[f][n].x;
            if (Frames[f][n].x>maxx) maxx = Frames[f][n].x;
            if (Frames[f][n].y<miny) miny = Frames[f][n].y;
            if (Frames[f][n].y>maxy) maxy = Frames[f][n].y;
            if (Frames[f][n].z<minz) minz = Frames[f][n].z;
            if (Frames[f][n].z>maxz) maxz = Frames[f][n].z;
         }
         
   }
   
   OldHead.scale.x = (float)((maxx-minx)/255.0);
   OldHead.scale.y = (float)((maxy-miny)/255.0);
   OldHead.scale.z = (float)((maxz-minz)/255.0);
   
   float radius=0,tr;
   for (f=0;f<OldHead.numframes;f++)
   {
      for (int n=0;n<OldHead.numverts;n++)
      {
         int t;
         t = (int)(float)((Frames[f][n].x-minx)/OldHead.scale.x + 0.5);
         if (t<0)
            t = 0;
         if (t>255)
            t =255;
         FileFrames[f][n].packedposition[0] = (BYTE)t;
         t = (int)(float)((Frames[f][n].y-miny)/OldHead.scale.y + 0.5);
         if (t<0)
            t = 0;
         if (t>255)
            t =255;
         FileFrames[f][n].packedposition[1] = (BYTE)t;
         t = (int)(float)((Frames[f][n].z-minz)/OldHead.scale.z + 0.5);
         if (t<0)
            t = 0;
         if (t>255)
            t =255;
         FileFrames[f][n].packedposition[2] = (BYTE)t;
      }
      
      for (f=0;f<OldHead.numframes;f++)
      {
         for (int n=0;n<OldHead.numverts;n++)
         {
            tr = (Frames[f][n].x-OldHead.origin.x)*(Frames[f][n].x-OldHead.origin.x);
            tr+=(Frames[f][n].y-OldHead.origin.y)*(Frames[f][n].y-OldHead.origin.y);
            tr+=(Frames[f][n].z-OldHead.origin.z)*(Frames[f][n].z-OldHead.origin.z);
            tr = (float)sqrt(tr);
            if (tr>radius) radius = tr;
         }
      }
   }
   
   
   for (f=0;f<OldHead.numframes;f++)
   {
      OldFrameInfo[f].max.packedposition[0] = 0;
      OldFrameInfo[f].max.packedposition[1] = 0;
      OldFrameInfo[f].max.packedposition[2] = 0;
      OldFrameInfo[f].min.packedposition[0] = 255;
      OldFrameInfo[f].min.packedposition[1] = 255;
      OldFrameInfo[f].min.packedposition[2] = 255;
      for (int n=0;n<OldHead.numverts;n++)
      {
         if (FileFrames[f][n].packedposition[0] < OldFrameInfo[f].min.packedposition[0])
            OldFrameInfo[f].min.packedposition[0] = FileFrames[f][n].packedposition[0];
         if (FileFrames[f][n].packedposition[1] < OldFrameInfo[f].min.packedposition[1])
            OldFrameInfo[f].min.packedposition[1] = FileFrames[f][n].packedposition[1];
         if (FileFrames[f][n].packedposition[2] < OldFrameInfo[f].min.packedposition[2])
            OldFrameInfo[f].min.packedposition[2] = FileFrames[f][n].packedposition[2];
         
         if (FileFrames[f][n].packedposition[0] > OldFrameInfo[f].max.packedposition[0])
            OldFrameInfo[f].max.packedposition[0] = FileFrames[f][n].packedposition[0];
         if (FileFrames[f][n].packedposition[1] > OldFrameInfo[f].max.packedposition[1])
            OldFrameInfo[f].max.packedposition[1] = FileFrames[f][n].packedposition[1];
         if (FileFrames[f][n].packedposition[2] > OldFrameInfo[f].max.packedposition[2])
            OldFrameInfo[f].max.packedposition[2] = FileFrames[f][n].packedposition[2];
      }
   }
   
   OldHead.radius = radius;
   
   int handle;
   long group=0;
   
   handle = open((LPCTSTR)FileName,O_BINARY | O_WRONLY | O_TRUNC | O_CREAT,S_IREAD|S_IWRITE);
   
   int ActualFrameCount = 0;
   
   int InGroup=0;
   for (i=0;i<OldHead.numframes;i++)
   {
      if (InGroup == 0)
      {
         ActualFrameCount ++;
         if (OldFrameInfo[i].StartGroup)
            InGroup = 1;
      }
      if (OldFrameInfo[i].EndGroup)
         InGroup = 0;
   }
   
   InGroup = 0;
   
   //   long tempframecount = OldHead.numframes,
   //   	  tempskincount = OldHead.numskins;
   
   //   OldHead.numframes = ActualFrameCount;
   //   OldHead.numskins = ActualSkinCount;
   write(handle, &OldHead, sizeof(TOldMdlHeader));
   //   OldHead.numframes = tempframecount;
   //   OldHead.numskins = tempskincount;
   
   for (n=0;n<OldHead.numskins;n++)
   {
      group=0;
      write(handle, &group,sizeof(long));
      
      write(handle, Skins[n], (long)OldHead.skinwidth*OldHead.skinheight);
   }
   
   TOldFileSkinVertex	*fileskinverts;
   
   fileskinverts = new TOldFileSkinVertex[OldHead.numverts];
   
   for (i=0;i<OldHead.numverts;i++)
   {
      fileskinverts[i].onseam = 0;
      fileskinverts[i].s = 0;
      fileskinverts[i].t = 0;
   }
   
   write(handle, fileskinverts, OldHead.numverts*sizeof(TOldFileSkinVertex));
   
   delete[] fileskinverts;
   
   TOldFileTri *temptris;
   
   temptris = new TOldFileTri[OldHead.numtris];
   
   for (i=0;i<OldHead.numtris;i++)
   {
      //   	temptris[i].facesfront = Tris[i].facesfront;
      temptris[i].facesfront = 0;
      temptris[i].vertices[0] = Tris[i].Vertices[0];
      temptris[i].vertices[1] = Tris[i].Vertices[1];
      temptris[i].vertices[2] = Tris[i].Vertices[2];
   }
   
   write( handle, temptris, (long)OldHead.numtris*sizeof(TOldFileTri));
   
   delete[] temptris;
   
   InGroup = 0;
   for (n=0;n<OldHead.numframes;n++)
   {
   /*   	if (OldFrameInfo[n].StartGroup)
   {
   TOldFileAnimVertex gmin,gmax;
   gmin.packedposition[0] = 255;
   gmin.packedposition[1] = 255;
   gmin.packedposition[2] = 255;
   gmax.packedposition[0] = 0;
   gmax.packedposition[1] = 0;
   gmax.packedposition[2] = 0;
   
     
       InGroup = 1;
       group = 0;
       for (int fc=n;fc<OldHead.numframes;fc++)
       {
       if (OldFrameInfo[fc].min.packedposition[0] < gmin.packedposition[0] )
       gmin.packedposition[0] = OldFrameInfo[fc].min.packedposition[0];
       if (OldFrameInfo[fc].min.packedposition[1] < gmin.packedposition[1] )
       gmin.packedposition[1] = OldFrameInfo[fc].min.packedposition[1];
       if (OldFrameInfo[fc].min.packedposition[2] < gmin.packedposition[2] )
       gmin.packedposition[2] = OldFrameInfo[fc].min.packedposition[2];
       if (OldFrameInfo[fc].max.packedposition[0] > gmax.packedposition[0] )
       gmax.packedposition[0] = OldFrameInfo[fc].max.packedposition[0];
       if (OldFrameInfo[fc].max.packedposition[1] > gmax.packedposition[1] )
       gmax.packedposition[1] = OldFrameInfo[fc].max.packedposition[1];
       if (OldFrameInfo[fc].max.packedposition[2] > gmax.packedposition[2] )
       gmax.packedposition[2] = OldFrameInfo[fc].max.packedposition[2];
       group++;
       
         if (OldFrameInfo[fc].EndGroup)
         fc = OldHead.numframes;
         }
         write (handle,&InGroup,sizeof(long));
         write (handle,&group,sizeof(long)); // num frames in group
         write (handle,&gmin,sizeof(TOldFileAnimVertex));
         write (handle,&gmax,sizeof(TOldFileAnimVertex));
         
           for (int fc=n;fc<OldHead.numframes;fc++)
           {
           write (handle,&OldFrameInfo[fc].time,sizeof(float));
           if (OldFrameInfo[fc].EndGroup)
           fc = OldHead.numframes;
           }
           }
           
             if (InGroup == 0)
             {
             write (handle,&InGroup,sizeof(long));
   }*/
      
      InGroup = 0;
      write (handle,&InGroup,sizeof(long));
      
      TOldFileFrameInfo	tempOldFrameInfo;
      
      tempOldFrameInfo.min = OldFrameInfo[n].min;
      tempOldFrameInfo.max = OldFrameInfo[n].max;
      memcpy(&tempOldFrameInfo.Name,&OldFrameInfo[n].Name,16);
      
      write(handle, &tempOldFrameInfo,sizeof(TOldFileFrameInfo));
      write(handle,FileFrames[n],(long)sizeof(TOldFileAnimVertex)*OldHead.numverts);
      
      /*      if (OldFrameInfo[n].EndGroup)
      InGroup = 0;*/
   }
   
   close(handle);
   delete[] trinormals;
   
   for (i = 0;i<OldHead.numframes;i++)
      delete[] FileFrames[i];
   
   delete[] OldFrameInfo;
   
   return 1;
}

// from models.c in the Quake 2 source.
void TCompleteModel::BuildGlCmds()
{
   int		i, j, k;
   int		startv;
   float		s, t;
   int		len, bestlen, besttype;
   int		best_xyz[1024];
   int		best_st[1024];
   int		best_tris[1024];
   int		type;
   
   //
   // build tristrips
   //
   numcommands = 0;
   numglverts = 0;
   memset (used, 0, sizeof(used));
   for (i=0 ; i<Head.num_tris ; i++)
   {
      // pick an unused triangle and start the trifan
      if (used[i])
         continue;
      
      bestlen = 0;
      for (type = 0 ; type < 2 ; type++)
         //	type = 1;
      {
         for (startv =0 ; startv < 3 ; startv++)
         {
            if (type == 1)
               len = StripLength (i, startv);
            else
               len = FanLength (i, startv);
            if (len > bestlen)
            {
               besttype = type;
               bestlen = len;
               for (j=0 ; j<bestlen+2 ; j++)
               {
                  best_st[j] = strip_st[j];
                  best_xyz[j] = strip_xyz[j];
               }
               for (j=0 ; j<bestlen ; j++)
                  best_tris[j] = strip_tris[j];
            }
         }
      }
      
      // mark the tris on the best strip/fan as used
      for (j=0 ; j<bestlen ; j++)
         used[best_tris[j]] = 1;
      
      if (besttype == 1)
         commands[numcommands++] = (bestlen+2);
      else
         commands[numcommands++] = -(bestlen+2);
      
      numglverts += bestlen+2;
      
      for (j=0 ; j<bestlen+2 ; j++)
      {
         // emit a vertex into the reorder buffer
         k = best_st[j];
         
         // emit s/t coords into the commands stream
         s = SkinVerts[k].s;
         t = SkinVerts[k].t;
         
         s = (float)((s + 0.5) / Head.skinwidth);
         t = (float)((t + 0.5) / Head.skinheight);
         
         if(numcommands>16510)
         {
            int x = 25;
            
            x += numcommands-4 *10;
            //this is shit
            k = best_st[j];
            //            numcommands = x++;
            
            //            numcommands++;
         }
         
         try {
            *(float *)&commands[numcommands++] = s;
            *(float *)&commands[numcommands++] = t;
            *(int *)&commands[numcommands++] = best_xyz[j];
         }
         catch (...)
         {
         }
      }
   }
   
   commands[numcommands++] = 0;		// end of list marker
}

int	TCompleteModel::StripLength (int starttri, int startv)
{
   int				m1, m2;
   int				st1, st2;
   int				j;
   TRealTriangle	*last, *check;
   int				k;
   
   used[starttri] = 2;
   
   last = &Tris[starttri];
   
   strip_xyz[0] = last->Vertices[(startv)%3];
   strip_xyz[1] = last->Vertices[(startv+1)%3];
   strip_xyz[2] = last->Vertices[(startv+2)%3];
   strip_st[0] = last->SkinVerts[(startv)%3];
   strip_st[1] = last->SkinVerts[(startv+1)%3];
   strip_st[2] = last->SkinVerts[(startv+2)%3];
   
   strip_tris[0] = starttri;
   stripcount = 1;
   
   m1 = last->Vertices[(startv+2)%3];
   st1 = last->SkinVerts[(startv+2)%3];
   m2 = last->Vertices[(startv+1)%3];
   st2 = last->SkinVerts[(startv+1)%3];
   
   // look for a matching triangle
nexttri:
   for (j=starttri+1, check=&Tris[starttri+1]
      ; j<Head.num_tris ; j++, check++)
   {
      for (k=0 ; k<3 ; k++)
      {
         if (check->Vertices[k] != m1)
            continue;
         if (check->SkinVerts[k] != st1)
            continue;
         if (check->Vertices[ (k+1)%3 ] != m2)
            continue;
         if (check->SkinVerts[ (k+1)%3 ] != st2)
            continue;
         
         // this is the next part of the fan
         
         // if we can't use this triangle, this tristrip is done
         if (used[j])
            goto done;
         
         // the new edge
         if (stripcount & 1)
         {
            m2 = check->Vertices[ (k+2)%3 ];
            st2 = check->SkinVerts[ (k+2)%3 ];
         }
         else
         {
            m1 = check->Vertices[ (k+2)%3 ];
            st1 = check->SkinVerts[ (k+2)%3 ];
         }
         
         strip_xyz[stripcount+2] = check->Vertices[ (k+2)%3 ];
         strip_st[stripcount+2] = check->SkinVerts[ (k+2)%3 ];
         strip_tris[stripcount] = j;
         stripcount++;
         
         used[j] = 2;
         goto nexttri;
      }
   }
done:
   
   // clear the temp used flags
   for (j=starttri+1 ; j<Head.num_tris ; j++)
      if (used[j] == 2)
         used[j] = 0;
      
      return stripcount;
}


/*
===========
FanLength
===========
*/
int	TCompleteModel::FanLength (int starttri, int startv)
{
   int		m1, m2;
   int		st1, st2;
   int		j;
   TRealTriangle	*last, *check;
   int		k;
   
   used[starttri] = 2;
   
   last = &Tris[starttri];
   
   strip_xyz[0] = last->Vertices[(startv)%3];
   strip_xyz[1] = last->Vertices[(startv+1)%3];
   strip_xyz[2] = last->Vertices[(startv+2)%3];
   strip_st[0] = last->SkinVerts[(startv)%3];
   strip_st[1] = last->SkinVerts[(startv+1)%3];
   strip_st[2] = last->SkinVerts[(startv+2)%3];
   
   strip_tris[0] = starttri;
   stripcount = 1;
   
   m1 = last->Vertices[(startv+0)%3];
   st1 = last->SkinVerts[(startv+0)%3];
   m2 = last->Vertices[(startv+2)%3];
   st2 = last->SkinVerts[(startv+2)%3];
   
   
   // look for a matching triangle
nexttri:
   for (j=starttri+1, check=&Tris[starttri+1] 
      ; j<Head.num_tris ; j++, check++)
   {
      for (k=0 ; k<3 ; k++)
      {
         if (check->Vertices[k] != m1)
            continue;
         if (check->SkinVerts[k] != st1)
            continue;
         if (check->Vertices[ (k+1)%3 ] != m2)
            continue;
         if (check->SkinVerts[ (k+1)%3 ] != st2)
            continue;
         
         // this is the next part of the fan
         
         // if we can't use this triangle, this tristrip is done
         if (used[j])
            goto done;
         
         // the new edge
         m2 = check->Vertices[ (k+2)%3 ];
         st2 = check->SkinVerts[ (k+2)%3 ];
         
         strip_xyz[stripcount+2] = m2;
         strip_st[stripcount+2] = st2;
         strip_tris[stripcount] = j;
         stripcount++;
         
         used[j] = 2;
         goto nexttri;
      }
   }
done:
   
   // clear the temp used flags
   for (j=starttri+1 ; j<Head.num_tris ; j++)
      if (used[j] == 2)
         used[j] = 0;
      
      return stripcount;
}

void TCompleteModel::DeleteUnusedSkinVerts()
{
   int	num_st_new=Head.num_st;
   short *st_to_new, targ;
   bool	*to_delete;
   int   v,t;
   TRealSkinVertex	*tempskinverts;
   
   st_to_new = new short[Head.num_st];
   to_delete = new bool[Head.num_st];
   
   for (v=0;v<Head.num_st;v++)
      to_delete[v] = true;
   
   for (t=0;t<Head.num_tris;t++)
   {
      to_delete[ Tris[t].SkinVerts[0] ] = false;
      to_delete[ Tris[t].SkinVerts[1] ] = false;
      to_delete[ Tris[t].SkinVerts[2] ] = false;
   }
   
   targ = 0;
   for (v=0;v<Head.num_st; v++)
   {
      if (to_delete[v])
         num_st_new--;
      else
         st_to_new[v] = targ++;
   }
   
   tempskinverts = new TRealSkinVertex[num_st_new];
   for (t=0;t<Head.num_tris;t++)
   {
      Tris[t].SkinVerts[0] = st_to_new[Tris[t].SkinVerts[0]];
      Tris[t].SkinVerts[1] = st_to_new[Tris[t].SkinVerts[1]];
      Tris[t].SkinVerts[2] = st_to_new[Tris[t].SkinVerts[2]];
   }
   
   for (v=0;v<Head.num_st;v++)
      if (to_delete[v]==false)
         tempskinverts[ st_to_new[v] ] = SkinVerts[v];
      
      delete[] SkinVerts;
      SkinVerts = tempskinverts;
      
      Head.num_st = num_st_new;
      
      delete[] st_to_new;
      delete[] to_delete;
}

int TCompleteModel::MergeFromFile(CString FileName, TModelFileType Type)
{
   TCompleteModel temp;
   
   if (Type == ftAQM)
      temp.LoadFromAQM(FileName);
   if (Type == ftMD2)
      temp.LoadFromMD2(FileName);
   if (Type == ftMDL)
      temp.LoadFromMDL(FileName);
   
   {
      float xfact,yfact;
      int st;
      
      xfact = (float)Head.skinwidth/(float)temp.Head.skinwidth;
      yfact = (float)Head.skinheight/(float)temp.Head.skinheight;
      
      for (st=0;st<temp.Head.num_st;st++)
      {
         temp.SkinVerts[st].s *= xfact;
         temp.SkinVerts[st].t *= yfact;
         temp.SkinVerts[st].Selected = true;
      }
      
      for (st=0;st<Head.num_st;st++)
         SkinVerts[st].Selected = false;
      
      TRealSkinVertex *tempstverts;
      
      tempstverts = new TRealSkinVertex[Head.num_st+temp.Head.num_st];
      memcpy(tempstverts,SkinVerts, sizeof(TRealSkinVertex)*Head.num_st);
      memcpy(&tempstverts[Head.num_st], temp.SkinVerts, sizeof(TRealSkinVertex)*temp.Head.num_st);
      
      delete[] SkinVerts;
      SkinVerts = tempstverts;
   }
   
   {
      int num_newxyz = Head.num_xyz + temp.Head.num_xyz,f;
      
      delete[] Verts;
      Verts = new TRealVertex[num_newxyz];
      
      for (f=0;f<temp.Head.num_frames;f++)
      {
         for (int v=0;v<temp.Head.num_xyz;v++)
            temp.Frames[f][v].Selected = true;
      }
      for (f=0;f<Head.num_frames;f++)
      {
         for (int v=0;v<Head.num_xyz;v++)
            Frames[f][v].Selected = false;
      }
      
      
      if (Head.num_frames < temp.Head.num_frames)
      {
         for (int f=0;f<temp.Head.num_frames;f++)
         {
            TRealVertex *tempframe;
            
            tempframe = new TRealVertex[num_newxyz];
            
            if (f<Head.num_frames)
               memcpy(tempframe, Frames[f], sizeof(TRealVertex)*Head.num_xyz);
            else
               memcpy(tempframe, Frames[Head.num_frames-1], sizeof(TRealVertex)*Head.num_xyz);
            
            memcpy(&tempframe[Head.num_xyz], temp.Frames[f], sizeof(TRealVertex)*temp.Head.num_xyz);
            
            delete[] Frames[f];
            Frames[f] = tempframe;
         }
      }
      else if (Head.num_frames > temp.Head.num_frames)
      {
         for (int f=0;f<Head.num_frames;f++)
         {
            TRealVertex *tempframe;
            
            tempframe = new TRealVertex[num_newxyz];
            memcpy(tempframe, Frames[f], sizeof(TRealVertex)*Head.num_xyz);
            
            if (f<temp.Head.num_frames)
               memcpy(&tempframe[Head.num_xyz], temp.Frames[f], sizeof(TRealVertex)*temp.Head.num_xyz);
            else
               memcpy(&tempframe[Head.num_xyz], temp.Frames[temp.Head.num_frames-1], sizeof(TRealVertex)*temp.Head.num_xyz);
            
            delete[] Frames[f];
            Frames[f] = tempframe;
         }
      }
      else
      {
         for (int f=0;f<Head.num_frames;f++)
         {
            TRealVertex *tempframe;
            
            tempframe = new TRealVertex[num_newxyz];
            memcpy(tempframe, Frames[f], sizeof(TRealVertex)*Head.num_xyz);
            memcpy(&tempframe[Head.num_xyz], temp.Frames[f], sizeof(TRealVertex)*temp.Head.num_xyz);
            
            delete[] Frames[f];
            Frames[f] = tempframe;
         }
      }
      
      memcpy(Verts,Frames[0],sizeof(TRealSkinVertex)*num_newxyz);
   }
   
   {
      if (temp.Head.num_frames > Head.num_frames)
      {
         TFrameInfo	*tempframeinfo;
         
         tempframeinfo = new TFrameInfo[temp.Head.num_frames];
         memcpy(tempframeinfo, FrameInfo, sizeof(TFrameInfo)*Head.num_frames);
         memcpy(&tempframeinfo[Head.num_frames],&temp.FrameInfo[Head.num_frames], sizeof(TFrameInfo)*(temp.Head.num_frames-Head.num_frames));
         
         delete[] FrameInfo;
         FrameInfo = tempframeinfo;
      }
   }
   
   {
      TRealTriangle *temptris;
      
      int t;
      for (t=0;t<Head.num_tris;t++)
         Tris[t].Selected = false;
      for (t=0;t<temp.Head.num_tris;t++)
         temp.Tris[t].Selected = true;
      
      
      temptris = new TRealTriangle[Head.num_tris+temp.Head.num_tris];
      memcpy(temptris,Tris, sizeof(TRealTriangle)*Head.num_tris);
      memcpy(&temptris[Head.num_tris], temp.Tris, sizeof(TRealTriangle)*temp.Head.num_tris);
      for (t=Head.num_tris;t<Head.num_tris+temp.Head.num_tris;t++)
      {
         temptris[t].Vertices[0] += (short)Head.num_xyz;
         temptris[t].Vertices[1] += (short)Head.num_xyz;
         temptris[t].Vertices[2] += (short)Head.num_xyz;
         temptris[t].SkinVerts[0] += (short)Head.num_st;
         temptris[t].SkinVerts[1] += (short)Head.num_st;
         temptris[t].SkinVerts[2] += (short)Head.num_st;
      }
      
      delete[] Tris;
      Tris = temptris;
   }
   
   Head.num_xyz += temp.Head.num_xyz;
   Head.num_st += temp.Head.num_st;
   Head.num_tris += temp.Head.num_tris;
   if (Head.num_frames < temp.Head.num_frames)
      Head.num_frames = temp.Head.num_frames;
   
   return 1;
}


int TCompleteModel::ExportFrameToASC(CString FileName, int FrameNum)
{
   int handle;
   char buf[30];
   CString floatt;
   memset(buf,0,30);
   
   handle = open((LPCTSTR)FileName, O_TEXT | O_CREAT | O_TRUNC | O_WRONLY, S_IREAD | S_IWRITE);
   
   if (handle == -1)
      return 0;
   
   writestring(handle, "Named Object: \"");
   writestring(handle, FrameInfo[FrameNum].Name);
   writestring(handle, "\"");
   writeln(handle);
   writestring(handle, "Tri-mesh, Vertices: ");
   writeint(handle, Head.num_xyz);
   writestring(handle, "    Faces: ");
   writeint(handle, Head.num_tris);
   writeln(handle);
   
   {
      writestring(handle, "Vertex List:\n");
      TRealVertex *V = Frames[FrameNum];
      
      for (int v=0;v<Head.num_xyz;v++)
      {
         writestring(handle, "Vertex ");
         writeint(handle, v);
         writestring(handle, ": ");
         
         writestring(handle, "X: ");
         floatt.Format("%.8f",V[v].x);
         writestring(handle, (LPCTSTR)floatt);
         
         writestring(handle, "    ");
         writestring(handle, "Y: ");
         floatt.Format("%.8f",V[v].y);
         writestring(handle, (LPCTSTR)floatt);
         
         writestring(handle, "    ");
         writestring(handle, "Z: ");
         floatt.Format("%.8f",V[v].z);
         writestring(handle, (LPCTSTR)floatt);
         
         writeln(handle);
      }
   }
   
   {
      writestring(handle, "Face list:");
      for (int f=0;f<Head.num_tris;f++)
      {
         writestring(handle, "Face ");
         writeint(handle,f);
         writestring(handle,":   ");
         
         writestring(handle, " A:");
         writeint(handle, Tris[f].Vertices[0]);
         writestring(handle, " B:");
         writeint(handle, Tris[f].Vertices[1]);
         writestring(handle, " C:");
         writeint(handle, Tris[f].Vertices[2]);
         
         writestring(handle, " AB:1 BC:1 CA:1");
         
         writeln(handle);
         writestring(handle, "Smoothing: 1");
         writeln(handle);
      }
   }
   
   close(handle);
   return 1;
}

int TCompleteModel::ExportSkinMeshToASC(CString FileName)
{
   int handle;
   char buf[30];
   CString floatt;
   memset(buf,0,30);
   
   handle = open((LPCTSTR)FileName, O_TEXT | O_CREAT | O_TRUNC | O_WRONLY, S_IREAD | S_IWRITE);
   
   if (handle == -1)
      return 0;
   
   writestring(handle, "Named Object: \"");
   writestring(handle, "SkinMesh");
   writestring(handle, "\"");
   writeln(handle);
   writestring(handle, "Tri-mesh, Vertices: ");
   writeint(handle, Head.num_st);
   writestring(handle, "    Faces: ");
   writeint(handle, Head.num_tris);
   writeln(handle);
   
   {
      writestring(handle, "Vertex List:\n");
      TRealSkinVertex *V = SkinVerts;
      
      for (int v=0;v<Head.num_st;v++)
      {
         writestring(handle, "Vertex ");
         writeint(handle, v);
         writestring(handle, ": ");
         
         writestring(handle, "X: ");
         floatt.Format("%.8f",V[v].s);
         writestring(handle, (LPCTSTR)floatt);
         
         writestring(handle, "    ");
         writestring(handle, "Y: ");
         floatt.Format("%.8f",V[v].t);
         writestring(handle, (LPCTSTR)floatt);
         
         writestring(handle, "    ");
         writestring(handle, "Z: ");
         floatt.Format("%.8f",0.0);
         writestring(handle, (LPCTSTR)floatt);
         
         writeln(handle);
      }
   }
   
   {
      writestring(handle, "Face list:\n");
      for (int f=0;f<Head.num_tris;f++)
      {
         writestring(handle, "Face ");
         writeint(handle,f);
         writestring(handle,":   ");
         
         writestring(handle, " A:");
         writeint(handle, Tris[f].SkinVerts[0]);
         writestring(handle, " B:");
         writeint(handle, Tris[f].SkinVerts[1]);
         writestring(handle, " C:");
         writeint(handle, Tris[f].SkinVerts[2]);
         
         writestring(handle, " AB:1 BC:1 CA:1");
         
         writeln(handle);
         writestring(handle, "Smoothing: 1");
         writeln(handle);
      }
   }
   
   close(handle);
   return 1;
}


//  3ds files importing


int TCompleteModel::ParseFaceL13DS(int handle, TRealTriangle* T, int &numtris, int &numverts)
{
   int				i, j, startbytesread, oldtris;
   unsigned short	tshort;
   
   
   //	if (trisfound)
   //   	return _3dsMultiFace;
   
   trisfound = 1;
   startbytesread = bytesread;
   
   if (eof(handle))
      return _3dsUEOF;
   
   read(handle, &tshort, sizeof(tshort));
   bytesread += sizeof(tshort);
   
   oldtris = numtris;
   numtris += (int)tshort;
   
   if (numtris > MAX_TRIANGLES)
      return _3dsMaxTris;
   
   for (i=oldtris ; i<numtris ; i++)
   {
      for (j=3 ; j>=0 ; j--)
         //		for (j=0 ; j<4 ; j++)
      {
         if (eof(handle))
            return _3dsUEOF;
         
         read(handle, &tshort, sizeof(tshort));
         bytesread += sizeof(tshort);
         if (j!=0)
            T[i].Vertices[j-1] = (short)tshort + oldverts;
      }
   }
   
   oldverts = numverts;
   
   return bytesread - startbytesread;
}

int TCompleteModel::ParseVertexL3DS(int handle, TRealVertex* V, int &numverts)
{
   int				i, startbytesread,oldverts;
   unsigned short	tshort;
   
   //	if (vertsfound)
   //	   return _3dsMultiVert;
   
   vertsfound = 1;
   startbytesread = bytesread;
   
   if (eof(handle))
      return _3dsUEOF;
   
   read(handle, &tshort, sizeof(tshort));
   bytesread += sizeof(tshort);
   
   oldverts = numverts;
   numverts += (int)tshort;
   
   if (numverts > MAX_VERTS)
      return _3dsMaxVerts;
   
   for (i=oldverts ; i<numverts ; i++)
   {
      if (eof(handle))
         return _3dsUEOF;
      
      read(handle, &V[i].x, sizeof(float));
      bytesread += sizeof(float);
      
      if (eof(handle))
         return _3dsUEOF;
      
      read(handle, &V[i].y, sizeof(float));
      bytesread += sizeof(float);
      
      if (eof(handle))
         return _3dsUEOF;
      
      read(handle, &V[i].z, sizeof(float));
      bytesread += sizeof(float);
   }
   
   return bytesread - startbytesread;
}

int TCompleteModel::ParseChunk3DS (int handle, TRealVertex* V, TRealTriangle* T, int &numverts, int &numtris)
{
#define BLOCK_SIZE	4096
   char			temp[BLOCK_SIZE];
   unsigned short	type;
   int				i, length, w, t, retval;
   
   retval = 0;
   
   if (eof(handle))
      return -1;
   
   read(handle, &type, sizeof(type));
   bytesread += sizeof(type);
   
   if (eof(handle))
      return 0;
   
   read(handle, &length, sizeof(length));
   bytesread += sizeof(length);
   w = length - 6;
   
   // process chunk if we care about it, otherwise skip it
   switch (type)
   {
      int res;
   case TRI_VERTEXL:
      res = ParseVertexL3DS (handle, V, numverts);
      if (res <0)
         return res;
      w -= res;
      goto ParseSubchunk;
      
   case TRI_FACEL1:
      res = ParseFaceL13DS (handle, T, numtris,numverts);
      if (res<0)
         return res;
      w -= res;
      goto ParseSubchunk;
      
   case EDIT_OBJECT:
      // read the name
      i = 0;
      
      do
      {
         if (eof(handle))
            return _3dsUEOF;
         
         read(handle, &temp[i], 1);
         i++;
         w--;
         bytesread++;
      } while (temp[i-1]);
      
   case MAIN3DS:
   case OBJ_TRIMESH:
   case EDIT3DS:
      // parse through subchunks
ParseSubchunk:
      while (w > 0)
      {
         res = ParseChunk3DS (handle,V,T,numverts, numtris);
         if (res<0)
            return res;
         w -= res;
      }
      
      retval = length;
      goto Done;
      
   default:
      // skip other chunks
      while (w > 0)
      {
         t = w;
         
         if (t > BLOCK_SIZE)
            t = BLOCK_SIZE;
         
         if (eof(handle))
            return _3dsUEOF;
         
         read(handle, &temp, t);
         bytesread += t;
         
         w -= t;
      }
      
      retval = length;
      goto Done;
   }
   
Done:
   //	level--;
   return retval;
}

CString TCompleteModel::LoadFrom3DS(CString FileName, TImportMode M)
{
   TRealVertex		*readxyz;
   TRealTriangle	*readtris;
   
   int handle,numxyz=0,numtris=0;
   short int tshort;
   
   
   handle = open((LPCTSTR)FileName,O_BINARY|O_RDONLY);
   if (handle == -1)
      return CString("Could not open file ") + FileName;
   
   read(handle, &tshort, sizeof(short int));
   
   if ((tshort != MAIN3DS) && (tshort != EDIT3DS))
   {
      close(handle);
      return CString("Not a 3DS file.");
   }
   
   lseek(handle, 0, SEEK_SET);
   
   readxyz = new TRealVertex[MAX_VERTS];
   readtris = new TRealTriangle[MAX_TRIANGLES];
   
   int res = ParseChunk3DS(handle, readxyz,readtris,numxyz,numtris);
   
   close(handle);
   
   oldverts = 0;
   
   if (res<=0)
   {
      delete[] readxyz;
      delete[] readtris;
   }
   
   if (res == _3dsUEOF)
      return CString("Unexpected End of file.");
   if (res == _3dsMultiVert)
      return CString("Multiple Vertex chunks encountered");
   if (res == _3dsMultiFace)
      return CString("Multiple Triangle chunks encountered");
   if (res == _3dsMaxVerts)
      return CString("file exceed maximum number of vertices");
   if (res == _3dsMaxTris)
      return CString("file exceeds maximum number of triangles");
   
   
   if (M.Mode == imAppendFrame)
   {
      if (numxyz != Head.num_xyz || numtris != Head.num_tris)
      {
         delete[] readxyz;
         delete[] readtris;
         return CString("Incorrect Number of vertices or Triangles");
      }
      Frames[Head.num_frames] = new TRealVertex[Head.num_xyz];
      memcpy(Frames[Head.num_frames],readxyz, sizeof(TRealVertex)*Head.num_xyz);
      
      TFrameInfo *tempframeinfo;
      
      tempframeinfo = new TFrameInfo[Head.num_frames+1];
      memcpy(tempframeinfo, FrameInfo, sizeof(TFrameInfo)*Head.num_frames);
      
      delete[] FrameInfo;
      FrameInfo = tempframeinfo;
      
      char name[256];
      _splitpath((LPCTSTR)FileName,NULL,NULL,name,NULL);
      if (strlen(name)>15)
      {
         memcpy(FrameInfo[Head.num_frames].Name, name,15);
         FrameInfo[Head.num_frames].Name[15] = '\x0';
      }
      else
         strcpy(FrameInfo[Head.num_frames].Name, name);
      
      
      Head.num_frames++;
   }
   
   if (M.Mode == imAppendVerts)
   {
      TRealTriangle	*temptris;
      TRealVertex		*tempverts;
      
      delete[] Verts;
      
      temptris = new TRealTriangle[Head.num_tris + numtris];
      
      if (Head.num_tris != 0)
         memcpy(temptris,Tris, Head.num_tris*sizeof(TRealTriangle));
      
      for (int i=0;i<numtris;i++)
      {
         readtris[i].Vertices[0] += (short)Head.num_xyz;
         readtris[i].Vertices[1] += (short)Head.num_xyz;
         readtris[i].Vertices[2] += (short)Head.num_xyz;
         readtris[i].SkinVerts[0] = 0;
         readtris[i].SkinVerts[1] = 0;
         readtris[i].SkinVerts[2] = 0;
         temptris[Head.num_tris+i] = readtris[i];
      }
      
      delete[] Tris;
      Tris = temptris;
      
      for (int f=0;f<Head.num_frames;f++)
      {
         tempverts = new TRealVertex[Head.num_xyz + numxyz];
         memcpy(tempverts, Frames[f], Head.num_xyz*sizeof(TRealVertex));
         for (int i=0;i<numxyz;i++)
            tempverts[Head.num_xyz + i] = readxyz[i];
         
         Frames[f] = tempverts;
      }
      
      
      Head.num_xyz += numxyz;
      Head.num_tris += numtris;
      
      Verts = new TRealVertex[Head.num_xyz];
      memcpy(Verts, Frames[g_CurFrame], Head.num_xyz*sizeof(TRealVertex));
   }
   
   
   delete[] readxyz;
   delete[] readtris;
   return CString("");
}


void writelong(int handle, int i)
{
   write(handle, &i, sizeof(i));
}

void writeshort(int handle, short i)
{
   write(handle, &i, sizeof(i));
}

void writefloat3(int handle, float a, float b, float c)
{
   write(handle, &a, 4);
   write(handle, &b, 4);
   write(handle, &c, 4);
}

void writefloat4(int handle, float a, float b, float c, float d)
{
   write(handle, &a, 4);
   write(handle, &b, 4);
   write(handle, &c, 4);
   write(handle, &d, 4);
}

int TCompleteModel::WriteChunk3DS(int handle, unsigned short tag)
{
   int				sizepos,v,t;
   int				size=0;
   int 				templ;
   unsigned short				tempw;
   float				tempf;
   
   write(handle, &tag, 2);
   size+=2;
   sizepos = tell(handle);
   
   templ = 0;
   write(handle,&templ,4);
   size+=4;
   
   
   
   switch (tag)
   {
   case MAIN3DS:
      size+=WriteChunk3DS(handle, VER3DS);
      size+=WriteChunk3DS(handle, EDIT3DS);
      size+=WriteChunk3DS(handle, KF_DATA);
      break;
      
   case VER3DS:
      templ = 3;
      write(handle, &templ, 4);
      size += 4;
      break;
      
      
   case EDIT3DS:
      size+=WriteChunk3DS(handle, EDIT_CONFIG);
      size+=WriteChunk3DS(handle, EDIT_CONFIG1);
      size+=WriteChunk3DS(handle, EDIT_OBJECT);
      break;
      
   case EDIT_CONFIG:
      templ = 3;
      write(handle,&templ, 4);
      size+=4;
      break;
      
   case EDIT_CONFIG1:
      templ = 0x3d800000;
      write(handle,&templ, 4);
      size+=4;
      break;
      
   case EDIT_OBJECT:
      write(handle, FrameInfo[framenum].Name, strlen(FrameInfo[framenum].Name)+1);
      size += strlen(FrameInfo[framenum].Name)+1;
      size += WriteChunk3DS(handle, OBJ_TRIMESH);
      break;
      
   case OBJ_TRIMESH:
      size += WriteChunk3DS(handle, TRI_VERTEXL);
      size += WriteChunk3DS(handle, TRANS_MATRIX);
      size += WriteChunk3DS(handle, TRI_FACEL1);
      break;
      
   case TRI_VERTEXL:
      tempw = (unsigned short)Head.num_xyz;
      write(handle, &tempw, 2);
      size += 2;
      for (v=0;v<Head.num_xyz;v++)
      {
         tempf = Frames[framenum][v].x;
         write(handle, &tempf, 4);
         size += 4;
         tempf = Frames[framenum][v].y;
         write(handle, &tempf, 4);
         size += 4;
         tempf = Frames[framenum][v].z;
         write(handle, &tempf, 4);
         size += 4;
      }
      break;
      
   case TRANS_MATRIX:
      writefloat3(handle, 1.0/16,0,0);
      size += 12;
      writefloat3(handle, 0,1.0/16,0);
      size += 12;
      writefloat3(handle, 0,0,1.0/16);
      size += 12;
      writefloat3(handle, 0,0,0);
      size += 12;
      break;
      
   case TRI_FACEL1:
      tempw = (unsigned short) Head.num_tris;
      write(handle, &tempw, 2);
      size += 2;
      for (t=0;t<Head.num_tris;t++)
      {
         write(handle, &Tris[t].Vertices[2],2);
         size += 2;
         write(handle, &Tris[t].Vertices[1],2);
         size += 2;
         write(handle, &Tris[t].Vertices[0],2);
         size += 2;
         tempw = 7;
         write(handle, &tempw, 2);
         size += 2;
      }
      size += WriteChunk3DS(handle, FACE_SMOOTHING);
      break;
      
   case FACE_SMOOTHING:
      for (t=0;t<Head.num_tris;t++)
      {
         templ = 1;
         write(handle,&templ, 4);
         size += 4;
      }
      break;
      
   case KF_DATA:
      size += WriteChunk3DS(handle, KF_HEAD);
      size += WriteChunk3DS(handle, KF_SEG_INFO);
      size += WriteChunk3DS(handle, KF_CURTIME);
      size += WriteChunk3DS(handle, OBJECT_NODE_TAG);
      break;
      
   case KF_HEAD:
      tempw = 5;
      write(handle, &tempw, 2);
      size += 2;
      {
         char name[9];
         memcpy(name, "MDLSCENE",9);
         write(handle, name, 9);
         size += 9;
      }
      templ = 10;
      write(handle, &templ, 4);
      size+=4;
      break;
      
   case KF_SEG_INFO:
      templ = 0;
      write(handle, &templ, 4);
      size += 4;
      templ = 10;
      write(handle, &templ, 4);
      size += 4;
      break;
      
   case KF_CURTIME:
      templ = 0;
      write(handle, &templ, 4);
      size += 4;
      break;
      
   case OBJECT_NODE_TAG:
      size += WriteChunk3DS(handle, NODE_ID);
      size += WriteChunk3DS(handle, NODE_HDR);
      size += WriteChunk3DS(handle, PIVOT);
      size += WriteChunk3DS(handle, POS_TRACK_TAG);
      size += WriteChunk3DS(handle, ROT_TRACK_TAG);
      size += WriteChunk3DS(handle, SCL_TRACK_TAG);
      break;
      
   case NODE_ID:
      tempw = 0;
      write(handle, &tempw, 2);
      size += 2;
      break;
      
   case NODE_HDR:
      write(handle, FrameInfo[framenum].Name, strlen(FrameInfo[framenum].Name)+1);
      size += strlen(FrameInfo[framenum].Name)+1;
      tempw = 0;
      write(handle, &tempw, 2);
      size += 2;
      tempw = 0;
      write(handle, &tempw, 2);
      size += 2;
      tempw = NO_PARENT;
      write(handle, &tempw, 2);
      size += 2;
      break;
      
   case PIVOT:
      writefloat3(handle, 0,0,0);
      size += 4*3;
      break;
      
   case POS_TRACK_TAG:
      tempw = 0; // track flags
      write(handle, &tempw, 2);
      size += 2;
      templ = 0; // trackmin
      write(handle, &templ, 4);
      size += 4;
      templ = 0; // tracmax
      write(handle, &templ, 4);
      size += 4;
      templ = 1; // nkeys
      write(handle, &templ, 4);
      size += 4;
      templ = 0; // keytime
      write(handle, &templ, 4);
      size += 4;
      tempw = 0; // tension cont. etc.
      write(handle, &tempw, 2);
      size += 2;
      
      writefloat3(handle, 0,0,0);
      size += 3*4;
      break;
      
   case ROT_TRACK_TAG:
      tempw = 0; // track flags
      write(handle, &tempw, 2);
      size += 2;
      templ = 0; // trackmin
      write(handle, &templ, 4);
      size += 4;
      templ = 0; // tracmax
      write(handle, &templ, 4);
      size += 4;
      templ = 1; // nkeys
      write(handle, &templ, 4);
      size += 4;
      templ = 0; // keytime
      write(handle, &templ, 4);
      size += 4;
      tempw = 0; // tension cont. etc.
      write(handle, &tempw, 2);
      size += 2;
      
      writefloat4(handle, 0,0,0,0);
      size += 4*4;
      break;
      
   case SCL_TRACK_TAG:
      tempw = 0; // track flags
      write(handle, &tempw, 2);
      size += 2;
      templ = 0; // trackmin
      write(handle, &templ, 4);
      size += 4;
      templ = 0; // tracmax
      write(handle, &templ, 4);
      size += 4;
      templ = 1; // nkeys
      write(handle, &templ, 4);
      size += 4;
      templ = 0; // keytime
      write(handle, &templ, 4);
      size += 4;
      tempw = 0; // tension cont. etc.
      write(handle, &tempw, 2);
      size += 2;
      
      writefloat3(handle, 1,1,1);
      size += 3*4;
      break;
      
   }
   
   lseek(handle, sizepos, SEEK_SET);
   write(handle, &size, 4);
   lseek(handle, 0,SEEK_END);
   
   return size;
}

int TCompleteModel::ExportFrameTo3DS(CString FileName, int FrameNum)
{
   int handle;
   
   framenum = FrameNum;
   handle = open((LPCTSTR)FileName, O_BINARY|O_TRUNC|O_CREAT|O_WRONLY,S_IREAD|S_IWRITE);
   
   WriteChunk3DS(handle, MAIN3DS);
   
   close(handle);
   return 0;
}


int TCompleteModel::LoadFromSQF(CString FileName, TImportMode M)
{
   int handle;
   unsigned long filesize;
   
   handle = open((LPCTSTR)FileName,O_BINARY | O_RDONLY);
   
   if (handle == -1)
   {
      return ascNoFile;
   }
   
   filesize = filelength(handle);
   if (filesize > UINT_MAX-1)
   {
      close(handle);
      return ascFileTooBig;
   }
   
   char 	*file, *pos, vertextag[8], facetag[6];
   int 	numverts, numtris,
      error=1,
      total_numxyz=0, total_numtris=0,
      step_xyz = 0, step_tris = 0;
   
   
   TRealTriangle	*readtris = NULL;
   TRealVertex		*readverts = NULL;
   
   strcpy(vertextag, "vertex ");
   strcpy(facetag, "face ");
   
   file = new char[filesize];
   
   read(handle, file, filesize);
   close(handle);
   
   char H[3];
   memcpy(H,file,3);
   if (H[0] != 'S' || H[1] != 'Q' || H[2] != 'F')
      return -1;
   
   pos = file;
   pos+=3;
   
   numverts = *(int *)pos;
   pos+=4;
   numtris = *(int *)pos;
   pos+=4;
   
   numverts = numverts;
   numtris = numtris;
   readverts = new TRealVertex[numverts];
   readtris = new TRealTriangle[numtris];
   
   for (int v=0;v<numverts;v++)
   {
      readverts[v].Selected = true;
      readverts[v].Visible = true;
      readverts[v].x = *((float*)pos);
      pos += 4;
      readverts[v].y = *((float*)pos);
      pos += 4;
      readverts[v].z = *((float*)pos);
      pos += 4;
   }
   for (int t=0;t<numtris;t++)
   {
      readtris[t].Selected = true;
      readtris[t].Visible = true;
      readtris[t].Vertices[0] = (*((short*)pos));
      pos += 2;
      readtris[t].Vertices[1] = (*((short*)pos));
      pos += 2;
      readtris[t].Vertices[2] = (*((short*)pos));
      pos += 2;
      readtris[t].SkinVerts[0] = 0;
      readtris[t].SkinVerts[1] = 0;
      readtris[t].SkinVerts[2] = 0;
   }
   
   
   if (M.Mode == imAppendFrame && (numtris != Head.num_tris || numverts != Head.num_xyz))
      error = ascVertTriDiff;
   
   if (error!=1)
   {
      delete[] file;
      delete[] readtris;
      delete[] readverts;
      
      return error;
   }
   
   if (M.Mode == imReplaceFrame)
   {
      if (Head.num_xyz != numverts || Head.num_tris != numtris)
      {
         delete[] readtris;
         delete[] readverts;
         return ascVertTriDiff;
      }
      
      for (int i=0;i<numverts;i++)
      {
         Verts[i] = readverts[i];
      }
      
      char name[256];
      _splitpath((LPCTSTR)FileName,NULL,NULL,name,NULL);
      if (strlen(name)>15)
      {
         memcpy(FrameInfo[0].Name, name,15);
         FrameInfo[0].Name[15] = '\x0';
      }
      else
         strcpy(FrameInfo[0].Name, name);
   }
   
   if (M.Mode == imAppendFrame)
   {
      if (Head.num_xyz != numverts || Head.num_tris != numtris)
      {
         delete[] readtris;
         delete[] readverts;
         return ascVertTriDiff;
      }
      
      Frames[Head.num_frames] = new TRealVertex[numverts];
      int i;
      for (i=0;i<numverts;i++)
      {
         Frames[Head.num_frames][i] = readverts[i];
      }
      
      TFrameInfo	*tempframeinfo;
      
      tempframeinfo = new TFrameInfo[Head.num_frames+1];
      for (int n=0;n<Head.num_frames;n++)
         memcpy(tempframeinfo[n].Name, FrameInfo[n].Name, 16);
      
      delete[] FrameInfo;
      FrameInfo = tempframeinfo;
      
      char name[256];
      _splitpath((LPCTSTR)FileName,NULL,NULL,name,NULL);
      if (strlen(name)>15)
      {
         memcpy(FrameInfo[Head.num_frames].Name, name,15);
         FrameInfo[Head.num_frames].Name[15] = '\x0';
      }
      else
         strcpy(FrameInfo[Head.num_frames].Name, name);
      
      
      float nx, ny;
      
      for (i = 0;i<g_CurMdl.Head.num_xyz;i++)
      {
         nx = -(Frames[Head.num_frames][i].y);
         ny = +(Frames[Head.num_frames][i].x);
         
         Frames[Head.num_frames][i].x = nx;
         Frames[Head.num_frames][i].y = ny;
      }
      
      
      
      Head.num_frames ++;
      //      g_CurFrame = =
   }
   
   if (M.Mode == imAppendVerts)
   {
      TRealTriangle	*temptris;
      TRealVertex		*tempverts;
      
      delete[] Verts;
      
      temptris = new TRealTriangle[Head.num_tris + numtris];
      
      if (Head.num_tris != 0)
         memcpy(temptris,Tris, Head.num_tris*sizeof(TRealTriangle));
      
      for (int i=0;i<numtris;i++)
      {
         readtris[i].Vertices[0] += (short)Head.num_xyz;
         readtris[i].Vertices[1] += (short)Head.num_xyz;
         readtris[i].Vertices[2] += (short)Head.num_xyz;
         readtris[i].SkinVerts[0] = 0;
         readtris[i].SkinVerts[1] = 0;
         readtris[i].SkinVerts[2] = 0;
         temptris[Head.num_tris+i] = readtris[i];
      }
      
      delete[] Tris;
      Tris = temptris;
      
      int f;
      for (f=0;f<Head.num_frames;f++)
      {
         tempverts = new TRealVertex[Head.num_xyz + numverts];
         memcpy(tempverts, Frames[f], Head.num_xyz*sizeof(TRealVertex));
         for (int i=0;i<numverts;i++)
            tempverts[Head.num_xyz + i] = readverts[i];
         
         Frames[f] = tempverts;
      }
      
      float nx, ny;
      
      for (f=0;f<g_CurMdl.Head.num_frames;f++)
         for (int i = g_CurMdl.Head.num_xyz;i<g_CurMdl.Head.num_xyz+numverts;i++)
         {
            nx = -(Frames[f][i].y);
            ny = +(Frames[f][i].x);
            
            Frames[f][i].x = nx;
            Frames[f][i].y = ny;
         }
         
         Head.num_xyz += numverts;
         Head.num_tris += numtris;
         
         Verts = new TRealVertex[Head.num_xyz];
         memcpy(Verts, Frames[g_CurFrame], Head.num_xyz*sizeof(TRealVertex));
   }
   
   delete[] readtris;
   delete[] readverts;
   
   delete[] file;
   
   return 0;
}

int TCompleteModel::ParseLWOChunk_Poly(BYTE *Chunk, unsigned long size, TLoadVertFace *L)
{
   unsigned short n, n_sides,cur_tri=0,Cnt=0;
   BYTE           *C;
   
   L->num_tris = 0;
   
   //   num_polys = ReverseW(*(unsigned short*)Chunk);
   //   Chunk+=2;
   
   C = Chunk;
   
   n = 0;
   while (n<size)
   {
      n_sides = ReverseW(*(unsigned short*)C);
      
      C += 2;
      n += 2;
      
      
      if (n_sides > 4 || n_sides < 3)
      {
         C += n_sides * 2;
         n += n_sides * 2;
         C += 2;
         n += 2;
         continue;
      }
      
      if (n_sides == 3)
      {
         L->num_tris ++;
         C += 6;
         n += 6;
         
         C += 2;
         n += 2;
      }
      else
      {
         L->num_tris +=2;
         C += 8;
         n += 8;
         
         C += 2;
         n += 2;
      }
      
   }
   
   L->T = new TRealTriangle[L->num_tris];
   
   C = Chunk;
   
   cur_tri = 0;
//   n = num_polys;
   while (Cnt<size)
   {
      n_sides = ReverseW(*(unsigned short*)C);
      
      C += 2;
      Cnt += 2;
      
      if (n_sides > 4 || n_sides < 3)
      {
         C += n_sides * 2;
         Cnt += n_sides * 2;
         C += 2;
         Cnt += 2;
         continue;
      }
      
      if (n_sides == 3)
      {
         unsigned short p1,p2,p3;
         
         p1 = ReverseW(*(unsigned short*)C);
         C+=2;
         Cnt+=2;
         p2 = ReverseW(*(unsigned short*)C);
         C+=2;
         Cnt+=2;
         p3 = ReverseW(*(unsigned short*)C);
         C+=2;
         Cnt+=2;
         
         L->T[cur_tri].Vertices[0] = p3;
         L->T[cur_tri].Vertices[1] = p2;
         L->T[cur_tri].Vertices[2] = p1;
         
         L->T[cur_tri].SkinVerts[0] = 0;
         L->T[cur_tri].SkinVerts[1] = 0;
         L->T[cur_tri].SkinVerts[2] = 0;
         
         cur_tri ++;
      }
      else
      {
         unsigned short p1,p2,p3,p4;
         
         p1 = ReverseW(*(unsigned short*)C);
         C+=2;
         Cnt+=2;
         p2 = ReverseW(*(unsigned short*)C);
         C+=2;
         Cnt+=2;
         p3 = ReverseW(*(unsigned short*)C);
         C+=2;
         Cnt+=2;
         p4 = ReverseW(*(unsigned short*)C);
         C+=2;
         Cnt+=2;
         
         L->T[cur_tri].Vertices[0] = p3;
         L->T[cur_tri].Vertices[1] = p2;
         L->T[cur_tri].Vertices[2] = p1;
         
         L->T[cur_tri].SkinVerts[0] = 0;
         L->T[cur_tri].SkinVerts[1] = 0;
         L->T[cur_tri].SkinVerts[2] = 0;
         
         cur_tri ++;
         
         L->T[cur_tri].Vertices[0] = p4;
         L->T[cur_tri].Vertices[1] = p3;
         L->T[cur_tri].Vertices[2] = p1;
         
         L->T[cur_tri].SkinVerts[0] = 0;
         L->T[cur_tri].SkinVerts[1] = 0;
         L->T[cur_tri].SkinVerts[2] = 0;
         
         cur_tri ++;
      }
      
      C += 2;
      Cnt += 2;
      
   }
   
   
   return Cnt;
}

int TCompleteModel::ParseLWOChunk_Points(BYTE *Chunk, unsigned long size, TLoadVertFace *L)
{
   L->num_xyz = size / 12;
   
   L->V = new TRealVertex[L->num_xyz];
   
   for (int i=0;i<L->num_xyz;i++)
   {
      L->V[i].x = ReverseF(*((float*)Chunk));
      Chunk += 4;
      L->V[i].y = ReverseF(*((float*)Chunk));
      Chunk += 4;
      L->V[i].z = ReverseF(*((float*)Chunk));
      Chunk += 4;
   }
   
   return size;
}


DWORD LWO_Size;

int TCompleteModel::ParseLWOChunk(BYTE *Chunk, TLoadVertFace *L)
{
   unsigned long  type, size, count=0;
   
   type = *((unsigned long*)Chunk);
   Chunk+=4;
   count += 4;
   if (type != LWOB)
   {
      size = ReverseL(*((unsigned long*)Chunk));
      Chunk+=4;
      count += 4;
   }
   
   switch (type)
   {
   case FORM:
      count+=ParseLWOChunk(Chunk,L);
      break;
      
   case LWOB:
      while (count < LWO_Size)
      {
         int i = ParseLWOChunk(Chunk, L);
         count += i;
         Chunk += i;
      }
      //         Chunk+=size;
      break;
   case PNTS:
      count+=ParseLWOChunk_Points(Chunk, size, L);
      Chunk+=size;
      break;
   case SRFS:
      count+=size;
      Chunk+=size;
      break;
   case POLS:
      count+=ParseLWOChunk_Poly(Chunk, size, L);
      Chunk+=size;
      break;
   case CRVS:
      count+=size;
      Chunk+=size;
      break;
   case PCHS:
      count+=size;
      Chunk+=size;
      break;
   case SURF:
      count+=size;
      Chunk+=size;
      break;
   }
   
   return size + 8;
}


CString TCompleteModel::LoadFromLWO(CString FileName, TImportMode M)
{
   BYTE           *file;
   int            hFile, nFileLength;
   TLoadVertFace  L;
   
   hFile = open((LPCTSTR)FileName, O_BINARY|O_RDONLY);
   
   if (hFile == -1)
      return "";
   
   nFileLength = filelength(hFile);
   
   file = new BYTE[nFileLength];
   
   read(hFile, file, nFileLength);
   
   close(hFile);
   
   LWO_Size = nFileLength;
   ParseLWOChunk(file, &L);
   
   delete[] file;
   
   if (M.Mode == imAppendVerts)
   {
      TRealVertex *V;
      
      V = new TRealVertex[Head.num_xyz + L.num_xyz];
      memcpy(V, Verts, Head.num_xyz*sizeof(TRealVertex));
      
      for (int i=0;i<L.num_xyz;i++)
      {
         V[i+Head.num_xyz].x = L.V[i].x;
         V[i+Head.num_xyz].y = L.V[i].y;
         V[i+Head.num_xyz].z = L.V[i].z;
         
         V[i+Head.num_xyz].Selected = false;
//         V[i+Head.num_xyz].Strength = 1;
//         V[i+Head.num_xyz].Parent = NULL;
      }
      
      delete Verts;
      Verts = V;
      
      for (int f=0;f<Head.num_frames;f++)
      {
         TRealVertex *V;
         
         V = new TRealVertex[Head.num_xyz + L.num_xyz];
         memcpy(V, Frames[f], Head.num_xyz*sizeof(TRealVertex));
         
         for (int i=0;i<L.num_xyz;i++)
         {
            V[i+Head.num_xyz].x = L.V[i].x;
            V[i+Head.num_xyz].y = L.V[i].y;
            V[i+Head.num_xyz].z = L.V[i].z;
            
            V[i+Head.num_xyz].Selected = false;
//            V[i+Head.num_xyz].Strength = 1;
//            V[i+Head.num_xyz].Parent = NULL;
         }
         
         delete Frames[f];
         Frames[f] = V;
      }
      
      
      TRealTriangle *T;
      T = new TRealTriangle[Head.num_tris + L.num_tris];
      
      memcpy(T, Tris, Head.num_tris*sizeof(TRealTriangle));
      
      for (int t=0;t<L.num_tris;t++)
      {
         T[t+Head.num_tris].Vertices[0] = L.T[t].Vertices[0] + Head.num_xyz;
         T[t+Head.num_tris].Vertices[1] = L.T[t].Vertices[1] + Head.num_xyz;
         T[t+Head.num_tris].Vertices[2] = L.T[t].Vertices[2] + Head.num_xyz;
         T[t+Head.num_tris].SkinVerts[0] = 0;
         T[t+Head.num_tris].SkinVerts[1] = 0;
         T[t+Head.num_tris].SkinVerts[2] = 0;
         T[t+Head.num_tris].Selected = false;
         T[t+Head.num_tris].SkinSelected = false;
      }
      
      
      delete Tris;
      Tris = T;
      
      Head.num_tris += L.num_tris;
      Head.num_xyz += L.num_xyz;
   }
   
   if (M.Mode == imAppendFrame)
   {
      if (L.num_tris != Head.num_tris)
         return CString("Number of Triangles different in file");
      
      if (L.num_xyz != Head.num_xyz)
         return CString("Numver of Vertices different in File");
      
      Frames[Head.num_frames] = new TRealVertex[Head.num_xyz];
      memcpy(Frames[Head.num_frames], Frames[Head.num_frames-1], sizeof(TRealVertex)*Head.num_xyz);
      for (int v=0;v<Head.num_xyz;v++)
      {
         Frames[Head.num_frames][v].x = L.V[v].x;
         Frames[Head.num_frames][v].y = L.V[v].y;
         Frames[Head.num_frames][v].z = L.V[v].z;
         Frames[Head.num_frames][v].Selected = false;
      }
      
      TFrameInfo  *FI;
      FI = new TFrameInfo[Head.num_frames+1];
      memcpy(FI, FrameInfo, sizeof(TFrameInfo)*Head.num_frames);
      
      char name[256];
      _splitpath((LPCTSTR)FileName,NULL,NULL,name,NULL);
      if (strlen(name)>15)
      {
         memcpy(FI[Head.num_frames].Name, name,15);
         FI[Head.num_frames].Name[15] = '\x0';
      }
      else
         strcpy(FI[Head.num_frames].Name, name);
      
      delete[] FrameInfo;
      FrameInfo = FI;
      
      Head.num_frames ++;
   }
   
   return CString("");
}

void TCompleteModel::SetCurrentFrame(double FrameNo, int Next)
{
   double   Inter;
   int      nextframe;
   TRealVertex *V;
   
   if (FrameNo<0)
      FrameNo = 0;
   
   if (FrameNo>=Head.num_frames)
      FrameNo = Head.num_frames - 1;
   
   if (FrameNo < 0)
      return;   // failure, there is no frames to work with.
   
   if (Next == -1)
      nextframe = (int)FrameNo % Head.num_frames;
   else nextframe = Next;
   
   Inter = FrameNo - int(FrameNo);
   
   V = Frames[(int)FrameNo];
   
   if (nextframe >= Head.num_frames)
      nextframe = Head.num_frames - 1;
   if (nextframe < 0)
      nextframe = 0;

   for (int v=0;v<Head.num_xyz;v++)
   {
      Verts[v].x = (float)((Frames[nextframe][v].x - V[v].x)*Inter + V[v].x);
      Verts[v].y = (float)((Frames[nextframe][v].y - V[v].y)*Inter + V[v].y);
      Verts[v].z = (float)((Frames[nextframe][v].z - V[v].z)*Inter + V[v].z);
   }
}

void TCompleteModel::LoadSelection(bool *Dest, CSelectionSource S)
{
   if (S == ssVertex)
   {
      for (int i=0;i<Head.num_xyz;i++)
         *Dest++ = Verts[i].Selected;
      return;
   }
   if (S == ssTriangle)
   {
      for (int i=0;i<Head.num_tris;i++)
         *Dest++ = Tris[i].Selected;
      return;
   }
   if (S == ssSkinVert)
   {
      for (int i=0;i<Head.num_st;i++)
         *Dest++ = SkinVerts[i].Selected;
      return;
   }
   if (S == ssSkinTri)
   {
      for (int i=0;i<Head.num_tris;i++)
         *Dest++ = Tris[i].SkinSelected;
      return;
   }
}

int TCompleteModel::BytesUsed()
{
   int Size=0;
   Size += sizeof(*this);
   
   Size += sizeof(BYTE)*Head.num_skins*Head.skinwidth*Head.skinheight;
   Size += sizeof(TRealSkinVertex)*Head.num_st;
   Size += sizeof(TRealVertex)*Head.num_xyz*(Head.num_frames+1);
   Size += sizeof(TRealTriangle)*Head.num_tris;
   Size += sizeof(TFrameInfo)*Head.num_frames;
   
   return Size;
}

void TCompleteModel::UpdateFrame(int FrameNo)
{
   if (g_CurFrame >= g_CurMdl.Head.num_frames)
      g_CurFrame = g_CurMdl.Head.num_frames-1;

   if (g_CurFrame < 0)
      g_CurFrame = 0;
   for (int n=0;n<g_CurMdl.Head.num_xyz;n++)
   {
      g_CurMdl.Frames[g_CurFrame][n].x = g_CurMdl.Verts[n].x;
      g_CurMdl.Frames[g_CurFrame][n].y = g_CurMdl.Verts[n].y;
      g_CurMdl.Frames[g_CurFrame][n].z = g_CurMdl.Verts[n].z;
   }
}

Vector3 TCompleteModel::GetSelectionCentre(SelectType S)
{
   int 		ns=0;
   Vector3	V;

   V.x = 0;
   V.y = 0;
   V.z = 0;

   if (S == stVertex)
      for (int n=0;n<Head.num_xyz;n++)
	      if (Verts[n].Visible)
         if (Verts[n].Selected)
         {
            V.x += Verts[n].x;
            V.y += Verts[n].y;
            V.z += Verts[n].z;
            ns++;
         }

   if (S == stTriangle)
   {
      bool  *vertsel;
      int   n;

      vertsel = new bool[Head.num_xyz];

      for (n=0;n<Head.num_xyz;n++)
         vertsel[n] = false;

      for (n=0;n<Head.num_tris;n++)
      if (Tris[n].Visible)
         if (Tris[n].Selected)
         {
            vertsel[ Tris[n].Vertices[0] ] = true;
            vertsel[ Tris[n].Vertices[1] ] = true;
            vertsel[ Tris[n].Vertices[2] ] = true;
         }

      for (n=0;n<Head.num_xyz;n++)
      if (Verts[n].Visible)
         if (vertsel[n])
         {
            V.x += Verts[n].x;
            V.y += Verts[n].y;
            V.z += Verts[n].z;
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


bool TCompleteModel::AnythingSelected()
{
	if (g_MainSelectMode == stVertex)
   {
   	for (int i=0;i<Head.num_xyz;i++)
			if (Verts[i].Selected)
         	return true;

      return false;
   }

   if (g_MainSelectMode == stTriangle)
   {
   	for (int i=0;i<Head.num_tris;i++)
			if (Tris[i].Selected)
         	return true;

      return false;
   }

   if (g_MainSelectMode == stBone)
   {
		return Bones.AnythingSelected();
   }
   return false;
}


void TCompleteModel::CalcNormals(UINT Action, int FrameNo)
{
   TRealVertex *V;
   
   if (FrameNo == -1)
      V = Verts;
   else
   {
      if (FrameNo>0 && FrameNo<Head.num_frames)
         V = Frames[FrameNo];
      else
         return;
   }
   
   if (Action == CALCNORMS_ALL)
   {
      for (int n=0;n<Head.num_xyz;n++)
         V[n].Normal.Set(0,0,0);
   }

   Vector3	va,vb,vc,vd,ve,vn;
   for (int n=0;n<Head.num_tris;n++)
   {
      if (Action == CALCNORMS_ALL || Tris[n].Selected)
      {
         va.Set(V[Tris[n].Vertices[0]].x,V[Tris[n].Vertices[0]].y,V[Tris[n].Vertices[0]].z);
         vb.Set(V[Tris[n].Vertices[1]].x,V[Tris[n].Vertices[1]].y,V[Tris[n].Vertices[1]].z);
         vc.Set(V[Tris[n].Vertices[2]].x,V[Tris[n].Vertices[2]].y,V[Tris[n].Vertices[2]].z);
         
         vd = vb-va;
         ve = vc-vb;
         
         Tris[n].Normal = vd/ve;
         Tris[n].Normal.Normalize();
         Tris[n].Centre = (va + vb + vc)*(float)(1/3.0);

         V[Tris[n].Vertices[0]].Normal += Tris[n].Normal;
         V[Tris[n].Vertices[1]].Normal += Tris[n].Normal;
         V[Tris[n].Vertices[2]].Normal += Tris[n].Normal;
      }
   }

   if (Action == CALCNORMS_ALL)
   {
      for (int n=0;n<Head.num_xyz;n++)
         V[n].Normal.Normalize();
   }
}

bool TCompleteModel::SelectConnected(bool *Buf, SelectType S)
{
   bool changed = false;

   if (S == stVertex)
      for (int n=0;n<5000;n++)
      {
         changed = false;
         for (int t=0;t<Head.num_tris;t++)
            if (!(Buf[Tris[t].Vertices[0]] && Buf[Tris[t].Vertices[1]] && Buf[Tris[t].Vertices[2]])
                                   &&
                 (Buf[Tris[t].Vertices[0]] || Buf[Tris[t].Vertices[1]] || Buf[Tris[t].Vertices[2]] )  )
               if (Verts[t].Visible)
               {
                  Buf[Tris[t].Vertices[0]] = true;
                  Buf[Tris[t].Vertices[1]] = true;
                  Buf[Tris[t].Vertices[2]] = true;
                  changed = true;
               }
         if (changed == false) n=5000;
      }

   if (S == stTriangle)
   {
   	bool  *vertsel;
      int   t,n;

      vertsel = new bool[Head.num_xyz];

      for (n=0;n<Head.num_xyz;n++)
      	vertsel[n] = false;

      for (t=0;t<Head.num_tris;t++)
	      if (Buf[t])
   	   {
				vertsel[ Tris[t].Vertices[0] ] = true;
				vertsel[ Tris[t].Vertices[1] ] = true;
				vertsel[ Tris[t].Vertices[2] ] = true;
   	   }


      for (n=0;n<5000;n++)
      {
         changed = false;
         for (t=0;t<Head.num_tris;t++)
            if (
               !(vertsel[Tris[t].Vertices[0]] &&
                 vertsel[Tris[t].Vertices[1]] &&
                 vertsel[Tris[t].Vertices[2]])
                              &&
               (vertsel[Tris[t].Vertices[0]] ||
                 vertsel[Tris[t].Vertices[1]] ||
                 vertsel[Tris[t].Vertices[2]])  )
            {
               vertsel[Tris[t].Vertices[0]]=true;
               vertsel[Tris[t].Vertices[1]]=true;
               vertsel[Tris[t].Vertices[2]]=true;
               changed = true;
            }
         if (changed == false) n=5000;
      }

      for (t=0;t<Head.num_tris;t++)
      {
         if ( vertsel[Tris[t].Vertices[0]] &&
         	  vertsel[Tris[t].Vertices[1]] &&
              vertsel[Tris[t].Vertices[2]] )
            if (Tris[t].Visible)
               Buf[t]= true;
      }

      delete[] vertsel;
   }

   return changed;
}


int TCompleteModel::GetSkinVerticesFrom3DVertices(SkinVertPos SVP, double Tolerance, float MinGetS, float MinGetT, float MaxGetS, float MaxGetT, bool Mirror)
{
	this->CalcNormals();

/*	if (SkinSelectMode == stVertex)
   	for (int i=0;i<Head.num_st;i++)
      	SkinVerts[i].Selected = false;

   if (SkinSelectMode == stTriangle)
   	for (int i=0;i<Head.num_tris;i++)
      	Tris[i].SkinSelected = Tris[i].Selected;
*/
	{
      int               t,v;

      bool 	            *vertsel,*trisel;
      int	            num_st_toadd=0, num_st_new;
      short             *xyz_to_st;
      TRealSkinVertex	*tempskinverts;

      Vector3           ProjVect, UpVect, RightVect;
      double            CosTol;

      CosTol = cos(M_PI*Tolerance/180.0);
      if (SVP == svFront)
      {
         ProjVect.Set(-1,0,0);
         RightVect.Set(0,1,0);
         UpVect.Set(0,0,-1);
      }
      if (SVP == svBack)
      {
         ProjVect.Set(1,0,0);
         if (Mirror)
            RightVect.Set(0,1,0);
         else
            RightVect.Set(0,-1,0);
         UpVect.Set(0,0,-1);
      }
      if (SVP == svRight)
      {
         ProjVect.Set(0,-1,0);
         RightVect.Set(1,0,0);
         UpVect.Set(0,0,-1);
      }
      if (SVP == svLeft)
      {
         ProjVect.Set(0,1,0);
         if (Mirror)
            RightVect.Set(1,0,0);
         else
            RightVect.Set(-1,0,0);
         UpVect.Set(0,0,-1);
      }
      if (SVP == svTop)
      {
         ProjVect.Set(0,0,-1);
         RightVect.Set(0,1,0);
         UpVect.Set(1,0,0);
      }
      if (SVP == svBottom)
      {
         ProjVect.Set(0,0,1);
         if (Mirror)
            RightVect.Set(0,-1,0);
         else
            RightVect.Set(0,1,0);
         UpVect.Set(-1,0,0);
      }
// add others here


      vertsel = new bool[Head.num_xyz];

      for (v=0;v<Head.num_xyz;v++)
         vertsel[v] = false;

      trisel = new bool[Head.num_tris];

      for (t=0;t<Head.num_tris;t++)
		{
         Vector3 Normal;
         double product;

         Normal.Set(Tris[t].Normal.x,Tris[t].Normal.y,Tris[t].Normal.z);
         product = ProjVect * Normal;

      	trisel[t] = Tris[t].Selected;

         if (product < CosTol)
            trisel[t] = false;
      }


      for (t=0;t<Head.num_tris;t++)
      {
         if (trisel[t])
         {
            vertsel[Tris[t].Vertices[0]] = true;
            vertsel[Tris[t].Vertices[1]] = true;
            vertsel[Tris[t].Vertices[2]] = true;
         }

      }
      for (v = 0;v<Head.num_xyz;v++)
         if (vertsel[v])
            num_st_toadd++;

      num_st_new = Head.num_st + num_st_toadd;

      tempskinverts = new TRealSkinVertex[num_st_new];
      memcpy(tempskinverts, SkinVerts, sizeof(TRealSkinVertex)*Head.num_st);

      xyz_to_st = new short[Head.num_xyz];

      {
         short temp=(short)Head.num_st;
         for (int v=0;v<Head.num_xyz;v++)
            if (vertsel[v])
               xyz_to_st[v] = temp++;
      }

      for (t=0;t<Head.num_tris;t++)
         if (trisel[t])
         {
            Tris[t].SkinVerts[0] = xyz_to_st[Tris[t].Vertices[0]];
            Tris[t].SkinVerts[1] = xyz_to_st[Tris[t].Vertices[1]];
            Tris[t].SkinVerts[2] = xyz_to_st[Tris[t].Vertices[2]];
         }

      float minx=100000,miny=100000,
            maxx=-100000,maxy=-100000;

      for (v=0;v<Head.num_xyz;v++)
         if (vertsel[v])
         {
            Vector3 VertVect;

            VertVect.Set(Verts[v].x,Verts[v].y,Verts[v].z);


            tempskinverts[xyz_to_st[v]].s = VertVect * RightVect;
            tempskinverts[xyz_to_st[v]].t = VertVect * UpVect;
            tempskinverts[xyz_to_st[v]].Selected = true;
         }


      for (v=0;v<Head.num_xyz;v++)
         if (vertsel[v])
         {
            if (tempskinverts[xyz_to_st[v]].s < minx)
               minx = tempskinverts[xyz_to_st[v]].s;
            if (tempskinverts[xyz_to_st[v]].s > maxx)
               maxx = tempskinverts[xyz_to_st[v]].s;
            if (tempskinverts[xyz_to_st[v]].t < miny)
               miny = tempskinverts[xyz_to_st[v]].t;
            if (tempskinverts[xyz_to_st[v]].t > maxy)
               maxy = tempskinverts[xyz_to_st[v]].t;
         }

      {
         double divS, divT, mulS, mulT;
         divS = maxx-minx;
         divT = maxy-miny;
         mulS = MaxGetS - MinGetS;
         mulT = MaxGetT - MinGetT;

         for (int v=0;v<Head.num_xyz;v++)
            if (vertsel[v])
            {
               double s,t;
               s = tempskinverts[xyz_to_st[v]].s;
               t = tempskinverts[xyz_to_st[v]].t;

               s = mulS*(s - minx)/divS  +  MinGetS;
               t = mulT*(t - miny)/divT  +  MinGetT;

               tempskinverts[xyz_to_st[v]].s = (float)s;
               tempskinverts[xyz_to_st[v]].t = (float)t;
            }
      }

      delete[] SkinVerts;
      SkinVerts = tempskinverts;
      Head.num_st = num_st_new;

//      if (SkinSelectMode == stTriangle)
//         for (int i=0;i<Head.num_tris;i++)
//            Tris[i].SkinSelected = trisel[i];


      delete[] xyz_to_st;
      delete[] vertsel;
      delete[] trisel;
	}

	DeleteUnusedSkinVerts();

   return 1;
}
