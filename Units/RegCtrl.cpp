//---------------------------------------------------------------------------
#include "stdafx.h"

#include "RegCtrl.h"
#include "RegClass.h"

#include <io.h>
#include <fcntl.h>
//#include <dir.h>
#include <sys/stat.h>


// All extra units
#include "MDLGlobs.h"
#include "Colors.h"
//#include "Settings.h"
#include "Keyfuncs.h"

#include "ProgSet.h"

// All dialogs and stiff go here
#include "VCMDL.h"
#include "DlgGlobs.h"

//---------------------------------------------------------------------------

int WriteFloat(int handle, float V)
{
   return write(handle, &V, sizeof(float));
}
int WriteDouble(int handle, double V)
{
   return write(handle, &V, sizeof(double));
}
int WriteLong(int handle, long V)
{
   return write(handle, &V, sizeof(long));
}
int WriteShort(int handle, short V)
{
   return write(handle, &V, sizeof(short));
}
int WriteChar(int handle, char V)
{
   return write(handle, &V, sizeof(char));
}
int WriteUChar(int handle, unsigned char V)
{
   return write(handle, &V, sizeof(unsigned char));
}
int WriteBool(int handle, bool V)
{
   return write(handle, &V, sizeof(bool));
}
int WriteString(int handle, const char *V)
{
   return write(handle, V, strlen(V)+1);
}

int WriteCFGVersion(int handle)
{
   int Size=0;
   Size += WriteLong(handle, CFGVersionTag);
   Size += WriteLong(handle, 4);
   Size += WriteLong(handle, CFGVERSION);

   return Size;
}

int WriteCFGWindowPos(int handle, CWnd *Window, unsigned int IDNumber)
{
// window pos organisation
//    char *Name = strlen(Name) + 1;
//    int Left, int Top, int Width, int Height, bool Maximized, bool OnTop
//    equals 18 bytes

/*   if (!Window)
      return 0;

   int Size=0, ChunkSize=18;
   bool Maxed = Window->WindowState == wsMaximized;

   ChunkSize += strlen(Window->GetRuntimeClass()->m_lpszClassName) + 1;

   TWindowState   Old = Window->WindowState;
   Window->WindowState = wsNormal;
   Size += WriteLong(handle, CFGWindowPosTag);
   Size += WriteLong(handle, ChunkSize);

   Size += WriteLong(handle, IDNumber);

   Size += WriteLong(handle, Window->Left);
   Size += WriteLong(handle, Window->Top);
   Size += WriteLong(handle, Window->Width);
   Size += WriteLong(handle, Window->Height);

   Size += WriteBool(handle, Maxed);
   Size += WriteBool(handle, Window->FormStyle == fsStayOnTop);

   Window->WindowState = Old;
   return Size;*/

   return 1;
}

int WriteCFGMRU(int handle, int nMRU, CString *MRUList)
{
   // int number of mru, then list of mru

   int Size = 0, SizePos;

   Size += WriteLong(handle, CFGMRUTag);
   SizePos = tell(handle);
   Size += WriteLong(handle, Size);
   Size += WriteLong(handle, nMRU);

   for (int i=0;i<nMRU;i++)
      Size += WriteString(handle, (LPCSTR)MRUList[i]);

   lseek(handle, SizePos, SEEK_SET);
   WriteLong(handle, Size-8);
   lseek(handle, Size-8, SEEK_CUR);

   return Size;
}

int WriteCFGColors(int handle, int nCols, unsigned char **Cols, CString *ColNames)
{
// int number of cols, then col name, then value, then repeat ncols times.

   int Size = 0, SizePos;

   Size += WriteLong(handle, CFGColorsTag);
   SizePos = tell(handle);
   Size += WriteLong(handle, Size);
   Size += WriteLong(handle, nCols);

   for (int i=0;i<nCols;i++)
   {
      Size += WriteString(handle, (LPCTSTR)ColNames[i]);
      Size += WriteCFGPalIndex(handle, *Cols[i]);
   }

   lseek(handle, SizePos, SEEK_SET);
   WriteLong(handle, Size-8);
   lseek(handle, Size-8, SEEK_CUR);

   return Size;
}

int WriteCFGInteger(int handle, char *Name, int V)
{
   int Size=0;

   Size += WriteLong(handle, CFGIntegerTag);
   Size += WriteLong(handle, strlen(Name)+1 + sizeof(V));

   Size += WriteString(handle, Name);
   Size += WriteLong(handle, V);

   return Size;
}

int WriteCFGString(int handle, char *Name, const char *V)
{
   int Size=0;
   Size += WriteLong(handle, CFGStringTag);
   Size += WriteLong(handle, strlen(Name)+1 + strlen(V)+1);

   Size += WriteString(handle, Name);
   Size += WriteString(handle, V);

   return Size;
}

int WriteCFGBool(int handle, char *Name, bool V)
{
   int Size=0;
   Size += WriteLong(handle, CFGBoolTag);
   Size += WriteLong(handle, strlen(Name)+1 + sizeof(V));

   Size += WriteString(handle, Name);
   Size += WriteBool(handle, V);

   return Size;
}

int WriteCFGChar(int handle, char *Name, char V)
{
   int Size=0;
   Size += WriteLong(handle, CFGCharTag);
   Size += WriteLong(handle, strlen(Name)+1 + sizeof(V));

   Size += WriteString(handle, Name);
   Size += WriteChar(handle, V);

   return Size;
}

int WriteCFGFloat(int handle, char *Name, float V)
{
   int Size=0;
   Size += WriteLong(handle, CFGFloatTag);
   Size += WriteLong(handle, strlen(Name)+1 + sizeof(V));

   Size += WriteString(handle, Name);
   Size += WriteFloat(handle, V);

   return Size;
}

int WriteCFGDouble(int handle, char *Name, double V)
{
   int Size=0;
   Size += WriteLong(handle, CFGDoubleTag);
   Size += WriteLong(handle, strlen(Name)+1 + sizeof(V));

   Size += WriteString(handle, Name);
   Size += WriteDouble(handle, V);

   return Size;
}


int WriteCFGPalIndex(int handle, unsigned char V)
{
//   int col, just a single byte

   int Size = 0;

   Size += WriteLong(handle, CFGPalIndexTag);
   Size += WriteLong(handle, 1);
   Size += WriteChar(handle, V);

   return Size;
}


int WriteConfigFile(const char *FileName)
{
   int hFile;

   hFile = open(FileName, O_BINARY | O_TRUNC | O_CREAT | O_WRONLY, S_IREAD|S_IWRITE);
   if (hFile == -1)
      return 0;

   WriteCFGVersion(hFile);

   /*
   WriteCFGWindowPos(hFile, MainForm);
   WriteCFGWindowPos(hFile, SkinEditForm);
   WriteCFGWindowPos(hFile, MemoForm);
   WriteCFGWindowPos(hFile, ModelPropForm);
   WriteCFGWindowPos(hFile, TypeInForm);
   WriteCFGWindowPos(hFile, SkinPainterForm);
   */

   WriteCFGMRU(hFile, g_MRU_Count, g_MRUList);
   

   WriteCFGColors(hFile, NUM_COLORS, COLORS, COLORNAMES);

/*   {
      char P;
      if (MainForm->ToolPageControl->ActivePage == MainForm->CreateSheet)
         P = 'C';
      if (MainForm->ToolPageControl->ActivePage == MainForm->ModifySheet)
         P = 'M';
      if (MainForm->ToolPageControl->ActivePage == MainForm->BonesSheet)
         P = 'B';
      if (MainForm->ToolPageControl->ActivePage == MainForm->ViewSheet)
         P = 'V';

      WriteCFGChar(hFile, "ToolPage", P);
   }
*/
   WriteCFGString(hFile, "BasePath", (LPCTSTR)QuakeDataDir);

   
   WriteCFGBool(hFile, "Grid", (g_MainDlg->GetMenu()->GetMenuState(ID_OPTIONS_SHOWGRID, MF_CHECKED) & MF_CHECKED) > 0);
   WriteCFGFloat(hFile, "GPos", g_GroundHeight);


   
   WriteCFGInteger(hFile, "UMaxNodes", g_MainUndo.MaxNodes);
   WriteCFGInteger(hFile, "UMinNodes", g_MainUndo.MinNodes);
   WriteCFGInteger(hFile, "UMaxSize", g_MainUndo.MaxSize);
   WriteCFGInteger(hFile, "SMaxNodes", g_SkinUndo.MaxNodes);
   WriteCFGInteger(hFile, "SMinNodes", g_SkinUndo.MinNodes);
   WriteCFGInteger(hFile, "SMaxSize", g_SkinUndo.MaxSize);
   WriteCFGInteger(hFile, "PMaxNodes", g_PaintUndo.MaxNodes);
   WriteCFGInteger(hFile, "PMinNodes", g_PaintUndo.MinNodes);
   WriteCFGInteger(hFile, "PMaxSize", g_PaintUndo.MaxSize);
//   WriteCFGInteger(hFile, "PanelWidth", PANELWIDTH);

   close(hFile);

   return 1;
}

int ReadConfigFile(const char *FileName)
{
   int hFile;

   hFile = open(FileName, O_BINARY | O_RDONLY);
   if (hFile==-1)
      return 0;

   int            Length = filelength(hFile),Pos=0;
   unsigned char  *Buffer;

   Buffer = new unsigned char[Length];

   read(hFile, Buffer, Length);
   close(hFile);

   while (Pos < Length)
   {
      int Ret;
      Ret = ParseCFGChunk(&Buffer[Pos]);
      if (Ret == -1)
         return 0;

      Pos += Ret;
   }


   delete[] Buffer;

   return 1;
}



int ParseCFGChunk(unsigned char *Buf)
{
   int Type, Length, Diff=0;

   Type = *(long*)Buf;
   Buf+=4;
   Diff+=4;
   Length = *(long*)Buf;
   Buf+=4;
   Diff+=4;

   switch(Type)
   {
      case CFGVersionTag:
         if (ReadCFGVersion(Buf)==-1)
            return 0;
         break;
      case CFGWindowPosTag:
         ReadCFGWindowPos(Buf);
         break;
      case CFGMRUTag:
         ReadCFGMRU(Buf);
         break;
      case CFGColorsTag:
         ReadCFGColors(Buf);
         break;
      case CFGRGBTag:
         break;
      case CFGPalIndexTag:
         break;
      case CFGFloatTag:
         ReadCFGFloat(Buf);
         break;
      case CFGDoubleTag:
         ReadCFGDouble(Buf);
         break;
      case CFGIntegerTag:
         ReadCFGInteger(Buf);
         break;
      case CFGStringTag:
         ReadCFGString(Buf);
         break;
      case CFGBoolTag:
         ReadCFGBool(Buf);
         break;
      case CFGCharTag:
         ReadCFGChar(Buf);
         break;
   }
   Buf += Length;
   Diff += Length;

   return Diff;
}

int ReadCFGVersion(unsigned char *Buf)
{
   int Version = *(long*)Buf;

   if (Version != CFGVERSION)
      return -1;

   return 1;
}

int ReadCFGWindowPos(unsigned char *Buf)
{
// window pos organisation
//    char *Name = strlen(Name) + 1;
//    int Left, int Top, int Width, int Height, bool Maximized, bool OnTop
//    equals 18 bytes

/*
   int NameLen, Left, Top, Width, Height;
   bool Maxed, OnTop;
   char *Name;

   NameLen = strlen((char*)Buf)+1;
   Name = new char[NameLen];

   strcpy(Name, (char*)Buf);
   Buf += NameLen;

   Left = *(long*)Buf; Buf += 4;
   Top = *(long*)Buf; Buf += 4;
   Width = *(long*)Buf; Buf += 4;
   Height = *(long*)Buf;  Buf += 4;
   Maxed = *(bool*)Buf; Buf += 1;
   OnTop = *(bool*)Buf; Buf += 1;



   for(int i=0;i<Application->ComponentCount;i++)
      if (Application->Components[i]->Name.AnsiCompareIC(Name) == 0)
      {
         CWnd *Form = dynamic_cast<TForm*>(Application->Components[i]);
         if (Form == 0)
            continue;

         Form->WindowState = wsNormal;
         Form->Left = Left;
         Form->Top = Top;
         Form->Width = Width;
         Form->Height = Height;
         Form->FormStyle = OnTop ? fsStayOnTop : fsNormal;
         Form->WindowState = Maxed ? wsMaximized : wsNormal;
      }

   delete[] Name;
*/
   return 1;

}

int ReadCFGMRU(unsigned char *Buf)
{
   // int number of mru, then list of mru

   int nMRU;

   nMRU = *(long*)Buf;
   Buf += 4;

   g_MRU_Count = nMRU;

   for (int i=0;i<nMRU;i++)
   {
      g_MRUList[i] = (char*)Buf;
      Buf += strlen((char*)Buf) + 1;
   }

   return 1;
}

int ReadCFGColors(unsigned char *Buf)
{
// int number of cols, then col name, then value, then repeat ncols times.
   int nCols;

   nCols = *(long*)Buf;
   Buf +=4;

   for (int i=0;i<nCols;i++)
   {
      for (int c=0;c<nCols;c++)
      {
         if (COLORNAMES[i].CompareNoCase((char*)Buf) == 0)
         {
            Buf += strlen((char*)Buf)+1;
            Buf += 8;
            *COLORS[i] = *Buf;
            Buf += 1;
         }
      }
   }

   return 1;
}

int ReadCFGInteger(unsigned char *Buf)
{
   int Value;
   char *Name;

   Name = new char[strlen((char*)Buf)+1];
   strcpy(Name, (char*)Buf);
   Buf += strlen((char*)Buf)+1;
   Value = *(int*)Buf;

   if (strcmpi(Name, "UMaxNodes") == 0)
      g_MainUndo.MaxNodes = Value;
   if (strcmpi(Name, "UMinNodes") == 0)
      g_MainUndo.MinNodes = Value;
   if (strcmpi(Name, "UMaxSize") == 0)
      g_MainUndo.MaxSize = Value;

   if (strcmpi(Name, "SMaxNodes") == 0)
      g_SkinUndo.MaxNodes = Value;
   if (strcmpi(Name, "SMinNodes") == 0)
      g_SkinUndo.MinNodes = Value;
   if (strcmpi(Name, "SMaxSize") == 0)
      g_SkinUndo.MaxSize = Value;

   if (strcmpi(Name, "PMaxNodes") == 0)
      g_PaintUndo.MaxNodes = Value;
   if (strcmpi(Name, "PMinNodes") == 0)
      g_PaintUndo.MinNodes = Value;
   if (strcmpi(Name, "PMaxSize") == 0)
      g_PaintUndo.MaxSize = Value;

//   if (strcmpi(Name, "PanelWidth") == 0)
//      PANELWIDTH = Value;

   delete[] Name;
   return Value;
}

int ReadCFGString(unsigned char *Buf)
{
   char *Name, *Value;

   Name = new char[strlen((char*)Buf)+1];
   strcpy(Name, (char*)Buf);
   Buf += strlen((char*)Buf)+1;

   Value = new char[strlen((char*)Buf)+1];
   strcpy(Value, (char*)Buf);
   Buf += strlen((char*)Buf)+1;

   if (strcmpi(Name, "BasePath") == 0)
      QuakeDataDir = Value;


   delete[] Name;
   delete[] Value;
   return 1;
}

int ReadCFGBool(unsigned char *Buf)
{
   bool Value;
   char *Name;

   Name = new char[strlen((char*)Buf)+1];
   strcpy(Name, (char*)Buf);
   Buf += strlen((char*)Buf)+1;
   Value = *(bool*)Buf;

//   if (strcmpi(Name, "Grid") == 0)
//      MainForm->ShowGrid->Checked = Value;


   delete[] Name;
   return 1;
}

int ReadCFGChar(unsigned char *Buf)
{
   char Value;
   char *Name;

   Name = new char[strlen((char*)Buf)+1];
   strcpy(Name, (char*)Buf);
   Buf += strlen((char*)Buf)+1;
   Value = *(char*)Buf;

   if (strcmpi(Name,"ToolPage") == 0)
   {
/*      if (Value == 'C')
         MainForm->ToolPageControl->ActivePage == MainForm->CreateSheet;
      if (Value == 'M')
         MainForm->ToolPageControl->ActivePage == MainForm->ModifySheet;
      if (Value == 'B')
         MainForm->ToolPageControl->ActivePage == MainForm->BonesSheet;
      if (Value == 'V')
         MainForm->ToolPageControl->ActivePage == MainForm->ViewSheet;*/
   }

   delete[] Name;

   return 1;
}

int ReadCFGFloat(unsigned char *Buf)
{
   float Value;
   char *Name;

   Name = new char[strlen((char*)Buf)+1];
   strcpy(Name, (char*)Buf);
   Buf += strlen((char*)Buf)+1;
   Value = *(float*)Buf;

   if (strcmpi(Name, "GPos") == 0)
   {
      g_GroundHeight = Value;
   }



   delete[] Name;

   return 1;
}

int ReadCFGDouble(unsigned char *Buf)
{
   double Value;
   char *Name;

   Name = new char[strlen((char*)Buf)+1];
   strcpy(Name, (char*)Buf);
   Buf += strlen((char*)Buf)+1;
   Value = *(double*)Buf;

   delete[] Name;

   return 1;
}

/*
int ReadCFGPalIndex(int handle, unsigned char V)
{
//   int col, just a single byte

   int Size = 0;

   Size += WriteLong(handle, CFGPalIndexTag);
   Size += WriteLong(handle, 1);
   Size += WriteChar(handle, V);

   return Size;
}*/


