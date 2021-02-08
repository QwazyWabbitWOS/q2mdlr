// Registry.h: interface for the CRegistry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRY_H__AE6460F4_539A_11D2_A01D_0040332C7B2F__INCLUDED_)
#define AFX_REGISTRY_H__AE6460F4_539A_11D2_A01D_0040332C7B2F__INCLUDED_


// The aim of this class is to encapsulate registry functions into one class
// aint with having a ltitle more flexibility and useability.

class CRegistry
{
	HKEY		m_hRootKey;

	HKEY		m_hCurrentKey;
	char		*m_CurrentKeyStr;
	char		*m_KeyPrefix, *m_KeySuffix;

public:
	CRegistry();
	CRegistry(HKEY RootKey);
	virtual ~CRegistry();

	void SetRootKey(HKEY RootKey);
	void SetKeyPrefix(char *Prefix);
	void SetKeySuffix(char *Suffix);

	int OpenKey(char *Key, REGSAM Access = KEY_READ | KEY_WRITE, bool AutoCreate = true);
	int CloseKey();	// Closes the current key
	int CreateKey(char *Key, char *Class = REG_NONE, REGSAM Access = KEY_READ | KEY_WRITE);
	int WriteString(const char *Name, const char *Data);
	int WriteDWORD(char *Name, DWORD Data);
	int WriteBinary(char *Name, BYTE *Data, int Size);
	CString ReadString(char *Name);
   int ReadString(char *Name, CString &Data);
   int ReadString(char *Name, char *Data);
	int ReadDWORD(char *Name, DWORD &Data);
	int ReadBinary(char *Name, BYTE *Data, int MaxSize);

};

#endif // !defined(AFX_REGISTRY_H__AE6460F4_539A_11D2_A01D_0040332C7B2F__INCLUDED_)
