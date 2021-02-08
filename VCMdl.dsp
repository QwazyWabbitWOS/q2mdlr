# Microsoft Developer Studio Project File - Name="VCMDL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VCMDL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VCMdl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VCMdl.mak" CFG="VCMDL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VCMDL - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VCMDL - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VCMDL - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\Units .\ShlBrows" /I ".\\" /I ".\Units" /I ".\ShlBrows" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\Units" /I ".\ShlBrows" /I ".\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0xc09 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "VCMDL - Win32 Release"
# Name "VCMDL - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ArrowDraw.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowDropper.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowMinus.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowMove.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowNorm.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowOrigin.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowPan.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowRotate.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowScale.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowSelect.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowTri.cur
# End Source File
# Begin Source File

SOURCE=.\res\ArrowVert.cur
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\res\VCMDL.ico
# End Source File
# Begin Source File

SOURCE=.\VCMdl.rc
# End Source File
# Begin Source File

SOURCE=.\res\VCMDL.rc2
# End Source File
# End Group
# Begin Group "Units"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Units\Bones.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\Bones.h
# End Source File
# Begin Source File

SOURCE=.\Units\Colors.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\Colors.h
# End Source File
# Begin Source File

SOURCE=.\Units\DibFuncs.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\DibFuncs.h
# End Source File
# Begin Source File

SOURCE=.\Units\Keyboard.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\Keyboard.h
# End Source File
# Begin Source File

SOURCE=.\Units\KeyFuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\Units\KeyFuncs.h
# End Source File
# Begin Source File

SOURCE=.\Units\Light.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\Light.h
# End Source File
# Begin Source File

SOURCE=.\Units\LWOHeadr.h
# End Source File
# Begin Source File

SOURCE=.\Units\MainFunc.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\MainFunc.h
# End Source File
# Begin Source File

SOURCE=.\Units\MATRIX.CPP

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\MATRIX.H
# End Source File
# Begin Source File

SOURCE=.\Units\MdlConst.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\MdlConst.h
# End Source File
# Begin Source File

SOURCE=.\Units\MDLGlobs.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\MDLGlobs.h
# End Source File
# Begin Source File

SOURCE=.\Units\MDLType.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\MDLType.h
# End Source File
# Begin Source File

SOURCE=.\Units\MenuItem.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\MenuItem.h
# End Source File
# Begin Source File

SOURCE=.\Units\OldMdlCn.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\OldMdlCn.h
# End Source File
# Begin Source File

SOURCE=.\Units\ProgSet.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\ProgSet.h
# End Source File
# Begin Source File

SOURCE=.\Units\RegClass.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\RegClass.h
# End Source File
# Begin Source File

SOURCE=.\Units\RegCtrl.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\RegCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Units\Undo.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\Undo.h
# End Source File
# Begin Source File

SOURCE=.\Units\Undo3D.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Units\Undo3D.h
# End Source File
# Begin Source File

SOURCE=.\Units\UndoSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\Units\UndoSkin.h
# End Source File
# End Group
# Begin Group "Dialogs And Controls"

# PROP Default_Filter ""
# Begin Group "MainEditor"

# PROP Default_Filter ""
# Begin Group "CPP Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VCMDLDlg.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BonesTab.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BottmDlg.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CreatTab.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ExTypDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModTab.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SideDlg.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ViewTab.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# End Group
# Begin Group "H Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VCMDLDlg.h
# End Source File
# Begin Source File

SOURCE=.\BonesTab.h
# End Source File
# Begin Source File

SOURCE=.\BottmDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreatTab.h
# End Source File
# Begin Source File

SOURCE=.\ExTypDlg.h
# End Source File
# Begin Source File

SOURCE=.\ModTab.h
# End Source File
# Begin Source File

SOURCE=.\SideDlg.h
# End Source File
# Begin Source File

SOURCE=.\ViewTab.h
# End Source File
# End Group
# End Group
# Begin Group "SkinEditor"

# PROP Default_Filter ""
# Begin Group "CPP Skin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SBotmDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SModTab.cpp
# End Source File
# Begin Source File

SOURCE=.\SToolDlg.cpp
# End Source File
# End Group
# Begin Group "H Skin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SBotmDlg.h
# End Source File
# Begin Source File

SOURCE=.\SkinDlg.h
# End Source File
# Begin Source File

SOURCE=.\SModTab.h
# End Source File
# Begin Source File

SOURCE=.\SToolDlg.h
# End Source File
# End Group
# End Group
# Begin Group "Config"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClrCfgPg.cpp
# End Source File
# Begin Source File

SOURCE=.\ClrCfgPg.h
# End Source File
# Begin Source File

SOURCE=.\DirCfgPg.cpp
# End Source File
# Begin Source File

SOURCE=.\DirCfgPg.h
# End Source File
# Begin Source File

SOURCE=.\KeyCfgPg.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyCfgPg.h
# End Source File
# Begin Source File

SOURCE=.\KeyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyDlg.h
# End Source File
# End Group
# Begin Group "PaintEditor"

# PROP Default_Filter ""
# Begin Group "CPP Paint"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Clrbutn.cpp
# End Source File
# Begin Source File

SOURCE=.\ColLstBx.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintTab.cpp
# End Source File
# End Group
# Begin Group "H Paint"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClrButn.h
# End Source File
# Begin Source File

SOURCE=.\ColLstBx.h
# End Source File
# Begin Source File

SOURCE=.\PaintDlg.h
# End Source File
# Begin Source File

SOURCE=.\PaintTab.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\ATabCtrl.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ATabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ConDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConDlg.h
# End Source File
# Begin Source File

SOURCE=.\MiscDlgs.cpp
# End Source File
# Begin Source File

SOURCE=.\MiscDlgs.h
# End Source File
# Begin Source File

SOURCE=.\PasteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PasteDlg.h
# End Source File
# Begin Source File

SOURCE=.\TypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TypeDlg.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DlgGlobs.cpp
# End Source File
# Begin Source File

SOURCE=.\Units\HelperFuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "VCMDL - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "VCMDL - Win32 Debug"

# ADD CPP /Yc"StdAfx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UndoCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\VCMdl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Units\ANORMS.H
# End Source File
# Begin Source File

SOURCE=.\DlgGlobs.h
# End Source File
# Begin Source File

SOURCE=.\Units\HelperFuncs.h
# End Source File
# Begin Source File

SOURCE=.\Units\old\MATRIX.H
# End Source File
# Begin Source File

SOURCE=.\MdlMsgs.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UndoCfg.h
# End Source File
# Begin Source File

SOURCE=.\VCMdl.h
# End Source File
# End Group
# Begin Group "Shell Browser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ShlBrows\SHFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ShlBrows\SHFile.h
# End Source File
# Begin Source File

SOURCE=.\ShlBrows\SHPidl.cpp
# End Source File
# Begin Source File

SOURCE=.\ShlBrows\SHPidl.h
# End Source File
# Begin Source File

SOURCE=.\ShlBrows\SHTree.cpp
# End Source File
# Begin Source File

SOURCE=.\ShlBrows\SHTree.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
