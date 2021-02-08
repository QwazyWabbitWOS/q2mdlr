// HelperFuncs.cpp
// -----------------------------------------
// Just some assorted functions or macros to assist in stuff
#include "stdafx.h"


#include "HelperFuncs.h"

void AddLineToEdit(HWND hWnd, CString &Str)
{
	AddLineToEdit(hWnd, (LPCTSTR)Str);
}

void AddLineToEdit(HWND hWnd, const char *Str)
{
	char *Buffer;
	int	Result, Length;

	Length = GetWindowTextLength(hWnd);
//	if (Length == 0)
//		return;

   // Extra 16 bytes for more info if necessary
	Length +=  strlen(Str) + 16;

	Buffer = new char[Length];

	Result = GetWindowText(hWnd, Buffer, Length-1);

	if (Result == 0)
	{
		DWORD err;
		err = GetLastError();
		if (err != 0)		// Only return if there is an error
			return;
	}

	strcat(Buffer, Str);
	strcat(Buffer, "\r\n");

	Result = SetWindowText(hWnd, CString(Buffer));
	if (Result == 0)
	{
		DWORD err;
		err = GetLastError();
	}

   int NumLines = SendMessage(hWnd, EM_GETLINECOUNT,0,0);
	SendMessage( hWnd, EM_LINESCROLL, 0 , NumLines);
//   SendMessage(
//	SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM << 8, NULL);
   SendMessage(hWnd, EM_SETSEL, Length-1, Length-1);

	delete[] Buffer;
}

void ThreadWaitFinish(HANDLE hThread)
{
	int		Result;
	DWORD	ExitCode;

	Result = 1;
	ExitCode = STILL_ACTIVE;
	while(ExitCode == STILL_ACTIVE && Result !=0)
	{
		Result = GetExitCodeThread(hThread, &ExitCode);
		Sleep(10);
	}

}


void SetCurrentResInstance(HINSTANCE hInst)
{
	g_CurResInstance = hInst;
}

CString LoadStr(unsigned int IDNum)
{
	int      Size = 256, SizeDone = 255;
	char     *Buf = NULL;
	CString  StrRet;

	while (SizeDone == Size-1)
	{
		if (Buf)
			delete[] Buf;
		Buf = new char[Size];
		SizeDone = LoadString(g_CurResInstance, IDNum, Buf, Size);
	}

	if (SizeDone == 0)
	{
		delete[] Buf;
		return CString("");
	}

	StrRet = Buf;

	if (Buf)
		delete[] Buf;
	return StrRet;
}


