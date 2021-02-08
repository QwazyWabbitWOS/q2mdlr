// HelperFuncs.h
// -----------------------------------------
// Just some assorted functions or macros to assist in stuff


#if !defined HelperFuncs_H
#define HelperFuncs_H

void SetCurrentResInstance(HINSTANCE hInst);


#define DUPCSTR(d,s) if (s) {if (d) delete[] d; d = new char[strlen(s)+1]; strcpy(d,s);}

void AddLineToEdit(HWND hWnd, CString &Str);
void AddLineToEdit(HWND hWnd, const char *Str);

// String loading and resource loading
CString LoadStr(unsigned int IDNum);
static HINSTANCE	g_CurResInstance;


// Not actually used yet, but probably will be after the app has been multi threaded.
void ThreadWaitFinish(HANDLE hThread);

#endif