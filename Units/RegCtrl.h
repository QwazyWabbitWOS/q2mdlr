//---------------------------------------------------------------------------
#ifndef RegControlH
#define RegControlH
//---------------------------------------------------------------------------


#define CFGVersionTag      0x63666732     // CFG2

#define CFGWindowPosTag    0x21
#define CFGMRUTag          0x22
#define CFGColorsTag       0x23
#define CFGRGBTag          0x24
#define CFGPalIndexTag     0x25

#define CFGFloatTag        0xA0
#define CFGDoubleTag       0xA1
#define CFGIntegerTag      0xA2
#define CFGStringTag       0xA3
#define CFGBoolTag         0xA4
#define CFGCharTag         0xA5

int WriteFloat(int handle, float V);
int WriteDouble(int handle, double V);
int WriteLong(int handle, long V);
int WriteShort(int handle, short V);
int WriteChar(int handle, char V);
int WriteUChar(int handle, unsigned char V);
int WriteBool(int handle, bool V);
int WriteString(int handle, const char *V);

int WriteCFGVersion(int handle);
int WriteCFGWindowPos(int handle, CWnd *Window);
int WriteCFGMRU(int handle, int nMRU, char **MRUList);
int WriteCFGColors(int handle, int nCols, unsigned char **Cols, CString *ColNames);
int WriteCFGInteger(int handle, char *Name, int V);
int WriteCFGString(int handle, char *Name, const char *V);
int WriteCFGBool(int handle, char *Name, bool V);
int WriteCFGChar(int handle, char *Name, char V);
int WriteCFGFloat(int handle, char *Name, float V);
int WriteCFGDouble(int handle, char *Name, double V);
int WriteCFGPalIndex(int handle, unsigned char V);

int WriteConfigFile(const char *FileName);
int ReadConfigFile(const char *FileName);
int ParseCFGChunk(unsigned char *Buf);

int ReadCFGVersion(unsigned char *Buf);
int ReadCFGWindowPos(unsigned char *Buf);
int ReadCFGMRU(unsigned char *Buf);
int ReadCFGColors(unsigned char *Buf);
int ReadCFGInteger(unsigned char *Buf);
int ReadCFGString(unsigned char *Buf);
int ReadCFGBool(unsigned char *Buf);
int ReadCFGChar(unsigned char *Buf);
int ReadCFGFloat(unsigned char *Buf);
int ReadCFGDouble(unsigned char *Buf);

#endif
