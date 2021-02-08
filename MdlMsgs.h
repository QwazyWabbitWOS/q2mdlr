#ifndef MDLMESSAGES_H
#define MDLMESSAGES_H

// constants for the MM_UPDATEVIEWPORTS message
#define MMC_UPDATE_XY   0x0001
#define MMC_UPDATE_XZ   0x0002
#define MMC_UPDATE_ZY   0x0004
#define MMC_UPDATE_XYZ  0x0007
#define MMC_UPDATE_CAM  0x0008
#define MMC_UPDATE_SKIN 0x0010

#define MMC_UPDATE_ALL  0xFFFF

#define MMC_UPDATE_DEPENDANTS 0x0001

#define MMC_NOSCROLL    0x01


#define MMC_XLABEL           0x01
#define MMC_YLABEL           0x02
#define MMC_ZLABEL           0x04
#define MMC_GRIDLABEL        0x08
#define MMC_SELECTCOUNTERS   0x10

//-------------------------------------------------------------
// VC Modeller Messages
//-------------------------------------------------------------


#define MM_UPDATESKIN            WM_USER+8001

#define MM_UPDATEFRAMELABEL      WM_USER+8002
// Indicates that to the main window, or any other window, that the current frame
// has been changed, and any frame-related screen data should be updated, 
// eg Frame name labels.
// wparam: 0   -  updatse the label, and the bottom time scroller
//       : MMC_NOSCROLL - does not update the bottom time scroller
// lparam: none


#define MM_FILENAMECHANGED       WM_USER+8003
#define MM_PAINT                 WM_USER+8004
#define MM_STOPPLAY              WM_USER+8005
#define MM_STARTPLAY             WM_USER+8006
#define MM_DOSYNC                WM_USER+8007
// Tells the main dialog to update the skin and main editor selection, if they are
// synchornous right now.

#define MM_CHECKSPINNERS         WM_USER+8008
// Tells the main dialog to call teh CheckSpinners() function to ensure
// that the Spiiners are within valid ranges

#define MM_CALCVIEWPORTS         WM_USER+8009

#define MM_RESETSPINNERS         WM_USER+8010
// this will set all spinners int eh modify and view tabs and reset their values
// to 0, adn their minimums and mximums to the current number of frames


#define MM_UPDATEFRAMES          WM_USER+8011
#define MM_LOADPAINTDATA         WM_USER+8012

#define MM_TOOLBUTCLICK          WM_USER+8013
// Message saying that a tool button has been pushed. All tools are modal, ie. Only
// One tool button can be pushed at a time. The main window handle sthe despressing
// and undepressing of tool buttons, so it needs to know about it.
// wparam: NULL
// lparam: hWnd of button being pushed

#define MM_UPDATEVIEWPORTS       WM_USER+8014
// Sent to a window to indicate that some or all of the view ports need updating
// wparam:  MMC_UPDATE_XY     updates the XY view
//          MMC_UPDATE_XZ     updates the XZ view
//          MMC_UPDATE_ZY     updates the ZY view
//          MMC_UPDATE_CAM    updates the Camera view.
//          MMC_UPDATE_SKIN   updates the skin if aplicable (For Skin Editor)
// lparam: MMC_UPDATE_DEPENDANTS    updates the skin, or 3d view, or painter view if required

#define MM_UPDATEFEEDBACK        WM_USER+8016
// This updates all or some of the bottom bits of data on the bottom dialog. Like
// XYZ positiuon, GirdSize, ScrollBar Pos etc.
// wParam:  MMC_XLABEL           updates the X feedback
//          MMC_YLABEL           updates the Y feedback
//          MMC_ZLABEL           updates the Z feedback
//          MMC_GRIDLABEL        updates the Gird feedback
//          MMC_SELECTCOUNTERS   updates the selection counters.

#define MM_SKIN_CHECKSCROLLBARS  WM_USER+8017
// This is sent to the skin editor dialog to tell it to check its scroll
// bars for valid values.
// wParam: none
// lParam: none


#define MM_PAINT_UPDATECOLORS    WM_USER+8018
// Tells the paint window to update the current color indicators
// wParam: none
// lParam: none

#endif