// VCMDL.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxadv.h"

#include "MdlGlobs.h"

#include "VCMDL.h"
#include "VCMDLDlg.h"

#include "Keyfuncs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVCMDLApp

BEGIN_MESSAGE_MAP(CVCMDLApp, CWinApp)
	//{{AFX_MSG_MAP(CVCMDLApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCMDLApp construction

CVCMDLApp::CVCMDLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVCMDLApp object

CVCMDLApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVCMDLApp initialization

BOOL CVCMDLApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

   free((void*)m_pszRegistryKey);
   //Change the name of the registry key.
   //The CWinApp destructor will free the memory.
//m_pszRegistryKey=_tcsdup(_T("HKEY_CURRENT_USER\\Software\\MartinTools\\Modeller\\"));


   LoadStdProfileSettings(10);

	CVCMDLDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


void CVCMDLApp::UpdateMRUMenu(CCmdUI *pCmdUI)
{
}

BOOL CVCMDLApp::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
   if (nID >= ID_FILE_MRU_FILE1 && nID <= ID_FILE_MRU_FILE5)
   {

      if (m_pMainWnd)
      {
         CMenu *Menu;
         
         Menu = m_pMainWnd->GetMenu();

         CString Str;
         Menu->GetMenuString(nID, Str, MF_BYCOMMAND);

         if (nCode == -1)
            KHelperOpenFile(Str);
         return TRUE;
      }
   }
   
	return CWinApp::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

