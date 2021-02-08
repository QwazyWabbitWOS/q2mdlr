// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D4A35A6B_C189_11D2_8361_0040332C7B2F__INCLUDED_)
#define AFX_STDAFX_H__D4A35A6B_C189_11D2_8361_0040332C7B2F__INCLUDED_

#define _WIN32_WINNT 0x0502

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Whack in the memory leak tracking newing stuff. Cool!!!!
#ifdef _DEBUG
   #define new DEBUG_NEW
#endif



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D4A35A6B_C189_11D2_8361_0040332C7B2F__INCLUDED_)
