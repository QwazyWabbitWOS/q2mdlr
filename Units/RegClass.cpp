// Registry.cpp: implementation of the CRegistry class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "RegClass.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DUPCSTR(d,s) if (s) {if (d) delete[] d; d = new char[strlen(s)+1]; strcpy(d,s);}


CRegistry::CRegistry()
{
	m_hRootKey = HKEY_CURRENT_USER;
	m_hCurrentKey = NULL;
	m_CurrentKeyStr = NULL;

	m_KeyPrefix = NULL;
	m_KeySuffix = NULL;
}

CRegistry::CRegistry(HKEY RootKey)
{	
	m_hRootKey = HKEY_CURRENT_USER;
	m_hCurrentKey = NULL;
	m_CurrentKeyStr = NULL;

	m_KeyPrefix = NULL;
	m_KeySuffix = NULL;

	SetRootKey(RootKey);
}

CRegistry::~CRegistry()
{
	if (m_hCurrentKey)
	{
		RegCloseKey(m_hCurrentKey);
		if (m_hRootKey)
			RegCloseKey(m_hRootKey);
	}

	if (m_CurrentKeyStr)
		delete[] m_CurrentKeyStr;

	if (m_KeySuffix)
		delete[] m_KeySuffix;
	if (m_KeyPrefix)
		delete[] m_KeyPrefix;
}


void CRegistry::SetRootKey(HKEY RootKey)
{
	m_hRootKey = RootKey;
}

void CRegistry::SetKeyPrefix(char *Prefix)
{
	DUPCSTR(m_KeyPrefix, Prefix);
}

void CRegistry::SetKeySuffix(char *Suffix)
{
	DUPCSTR(m_KeySuffix, Suffix);
}


// Returns 0 on failure, 1 on successful opening, 2 on successful open and create.
int CRegistry::OpenKey(char *Key, REGSAM Access, bool AutoCreate)
{
	if (!m_hRootKey)
		return 0;

	if (!Key)	
		return 0;

	int		Length = 0;

	if (Key)
		Length += strlen(Key);
	if (m_KeyPrefix)
		Length += strlen(m_KeyPrefix);
	if (m_KeySuffix)
		Length += strlen(m_KeySuffix);
	
	if (m_CurrentKeyStr)
		delete[] m_CurrentKeyStr;

	m_CurrentKeyStr = new char[Length + 1];
	m_CurrentKeyStr[0] = '\x0';
	if (m_KeyPrefix)
		strcat(m_CurrentKeyStr, m_KeyPrefix);
	if (Key)
		strcat(m_CurrentKeyStr, Key);
	if (m_KeySuffix)
		strcat(m_CurrentKeyStr, m_KeySuffix);

	if (m_hCurrentKey)
		RegCloseKey(m_hCurrentKey);

	int		Result;
	DWORD	Disposition;

	if (AutoCreate)		// Use createkey, since it will open it if it is already there
	{
		Result = RegCreateKeyEx(m_hRootKey, m_CurrentKeyStr, 0, REG_NONE, REG_OPTION_NON_VOLATILE, Access, NULL, &m_hCurrentKey, &Disposition);
	}
	else
	{
		Result = RegOpenKeyEx(m_hRootKey, m_CurrentKeyStr, 0, Access, &m_hCurrentKey);
		Disposition = REG_OPENED_EXISTING_KEY;
	}

	if (Result != ERROR_SUCCESS)
		return 0;
	if (Disposition == REG_CREATED_NEW_KEY)
		return 2;

	return 1;
}

int CRegistry::CloseKey()
{
	if (!m_hCurrentKey)
		return 0;

	RegCloseKey(m_hCurrentKey);
	m_hCurrentKey = NULL;
	if (m_CurrentKeyStr)
		delete[] m_CurrentKeyStr;

	m_CurrentKeyStr = NULL;

	return 1;
}

int CRegistry::CreateKey(char *Key, char *Class, REGSAM Access)
{
	char	*NewKey;

	NewKey = new char[strlen(Key)+strlen(m_KeyPrefix) + strlen(m_KeySuffix)+1];
	strcpy(NewKey, m_KeyPrefix);
	strcat(NewKey, Key);
	strcat(NewKey, m_KeySuffix);

	delete[] NewKey;

	return 0;
}

int CRegistry::WriteString(const char *Name, const char *Data)
{
	int Result;
	
	Result = RegSetValueEx(m_hCurrentKey, Name, 0, REG_SZ, (BYTE*)Data, strlen(Data)+1);
	if (Result == ERROR_SUCCESS)
		return 1;
	return 0;
}

int CRegistry::WriteDWORD(char *Name, DWORD Data)
{
	int Result;
	
	Result = RegSetValueEx(m_hCurrentKey, Name, 0, REG_DWORD, (BYTE*)&Data, sizeof(DWORD));
	if (Result == ERROR_SUCCESS)
		return 1;
	return 0;
}

int CRegistry::WriteBinary(char *Name, BYTE *Data, int Size)
{
	int Result;
	
	Result = RegSetValueEx(m_hCurrentKey, Name, 0, REG_BINARY, (BYTE*)Data, Size);
	if (Result == ERROR_SUCCESS)
		return 1;
	return 0;
}

int CRegistry::ReadString(char *Name, char *Data)
{
	int		Result;
	DWORD	Size, Type;
	BYTE	*Buffer;

	Result = RegQueryValueEx(m_hCurrentKey, Name, 0, &Type, NULL, &Size);
	if (Result != ERROR_SUCCESS)
		return 0;

	if (Type != REG_SZ)
		return 0;

	Buffer = new BYTE[Size];
	Result = RegQueryValueEx(m_hCurrentKey, Name, 0, &Type, Buffer, &Size);

	if (Result == ERROR_SUCCESS)
	{
		strcpy(Data, (char*)Buffer);
		delete[] Buffer;
		return 1;
	}

	delete[] Buffer;
	return 0;
}

int CRegistry::ReadString(char *Name, CString &Data)
{
   char  Buf[1024];

   if (ReadString(Name, Buf) == 0)
      return 0;

   Data = Buf;

   return 1;
}

CString CRegistry::ReadString(char *Name)
{
   char  Buf[1024];

   if (ReadString(Name, Buf) == 0)
      return CString();

   return CString(Buf);
}

int CRegistry::ReadDWORD(char *Name, DWORD &Data)
{
	int		Result;
	DWORD	Size, Type;
	BYTE	*Buffer;

	Result = RegQueryValueEx(m_hCurrentKey, Name, 0, &Type, NULL, &Size);
	if (Result != ERROR_SUCCESS)
		return 0;

	if (Type != REG_DWORD)
		return 0;

	Buffer = new BYTE[Size];
	Result = RegQueryValueEx(m_hCurrentKey, Name, 0, &Type, Buffer, &Size);

	if (Result == ERROR_SUCCESS)
	{
		memcpy(&Data, Buffer, Size);
		delete[] Buffer;
		return 1;
	}

	delete[] Buffer;
	return 0;
}

int CRegistry::ReadBinary(char *Name, BYTE *Data, int MaxSize)
{
	int		Result;
	DWORD	Size, Type;
	BYTE	*Buffer;

	Result = RegQueryValueEx(m_hCurrentKey, Name, 0, &Type, NULL, &Size);
	if (Result != ERROR_SUCCESS)
		return 0;

	if (Type != REG_BINARY)
		return 0;

	if (Size>(DWORD)MaxSize)
		return 0;

	Buffer = new BYTE[Size];
	Result = RegQueryValueEx(m_hCurrentKey, Name, 0, &Type, Buffer, &Size);

	if (Result == ERROR_SUCCESS)
	{
		memcpy(Data, Buffer, Size);
		delete[] Buffer;
		return 1;
	}

	delete[] Buffer;
	return 0;
}

