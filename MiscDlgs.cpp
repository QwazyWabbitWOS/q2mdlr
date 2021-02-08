// MiscDlgs.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"

#include "MdlGlobs.h"

#include "MiscDlgs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenameFrameDlg dialog


CRenameFrameDlg::CRenameFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRenameFrameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRenameFrameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRenameFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenameFrameDlg)
	DDX_Control(pDX, IDC_OLDNAMEEDIT, m_OldNameEdit);
	DDX_Control(pDX, IDC_NEWNAMEEDIT, m_NewNameEdit);
	DDX_Control(pDX, IDC_FRAMENUMEDIT, m_FrameNumEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRenameFrameDlg, CDialog)
	//{{AFX_MSG_MAP(CRenameFrameDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenameFrameDlg message handlers
/////////////////////////////////////////////////////////////////////////////
// CChooseFrameDlg dialog


CChooseFrameDlg::CChooseFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseFrameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChooseFrameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChooseFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChooseFrameDlg)
	DDX_Control(pDX, IDC_FRAMELIST, m_FrameList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChooseFrameDlg, CDialog)
	//{{AFX_MSG_MAP(CChooseFrameDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChooseFrameDlg message handlers

BOOL CChooseFrameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   UpdateFrameList();
   m_SelectedFrame = 0;
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChooseFrameDlg::UpdateFrameList()
{
   m_FrameList.DeleteAllItems();
   m_FrameList.DeleteColumn(1);
   m_FrameList.DeleteColumn(0);
   
   m_FrameList.InsertColumn(0, "Name", LVCFMT_LEFT, 75);
   m_FrameList.InsertColumn(1, "Num", LVCFMT_LEFT, 45, 1);

   for (int n=g_CurMdl.Head.num_frames-1 ; n>=0 ; n--)
   {
      LVITEM   Item;
      char     Text[128];

      Item.mask = LVIF_TEXT;
      Item.iItem = 0;
      Item.iSubItem = 0;
      Item.pszText = Text;
      Item.cchTextMax = 16;

      strcpy(Text, g_CurMdl.FrameInfo[n].Name);

      m_FrameList.InsertItem(&Item);

      Item.iItem = 0;
      Item.iSubItem = 1;
      sprintf(Text,"%i", n);
      Item.pszText = Text;

      m_FrameList.InsertItem(&Item);

   }
}

void CChooseFrameDlg::OnClose() 
{

	CDialog::OnClose();
}

void CChooseFrameDlg::OnOK() 
{
   POSITION Pos;


   Pos = m_FrameList.GetFirstSelectedItemPosition();

   if (Pos != NULL)
   {
      m_SelectedFrame = m_FrameList.GetNextSelectedItem(Pos);
   }
   else
      m_SelectedFrame = 0;

	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CAddFrameDlg dialog


CAddFrameDlg::CAddFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddFrameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddFrameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddFrameDlg)
	DDX_Control(pDX, IDC_NEWFRAMENUMEDIT, m_NewFrameNumEdit);
	DDX_Control(pDX, IDC_NEWFRAMENAMEEDIT, m_NewFrameNameEdit);
	DDX_Control(pDX, IDC_FRAMENUMEDIT, m_FrameNumEdit);
	DDX_Control(pDX, IDC_FRAMENAMEEDIT, m_FrameNameEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddFrameDlg, CDialog)
	//{{AFX_MSG_MAP(CAddFrameDlg)
	ON_BN_CLICKED(IDC_CHOOSEBUT, OnChoosebut)
	ON_BN_CLICKED(IDC_AFTERFRAMECHECK, OnAfterframecheck)
	ON_EN_CHANGE(IDC_FRAMENUMEDIT, OnChangeFramenumedit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FRAMENUMSPIN, OnDeltaposFramenumspin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddFrameDlg message handlers

void CAddFrameDlg::OnOK() 
{
   CButton  *RadioCtrl;

   RadioCtrl = (CButton*)GetDlgItem(IDC_BEFOREFIRSTCHECK);
   if (RadioCtrl->GetCheck() == 1)
      m_FrameSource = FRAMESRC_BEFOREFIRST;

   RadioCtrl = (CButton*)GetDlgItem(IDC_AFTERFINALCHECK);
   if (RadioCtrl->GetCheck() == 1)
      m_FrameSource = FRAMESRC_AFTERFINAL;
   
   RadioCtrl = (CButton*)GetDlgItem(IDC_AFTERFRAMECHECK);
   if (RadioCtrl->GetCheck() == 1)
   {
      // m_FrameSource will already be set.
      m_NewFrameNameEdit.GetWindowText(m_NewName);
   }



   CDialog::OnOK();
}

void CAddFrameDlg::OnChoosebut() 
{
   CChooseFrameDlg   ChooseDlg;

   if (ChooseDlg.DoModal() == IDCANCEL)
   {
      return;
   }

   m_FrameSource = ChooseDlg.m_SelectedFrame;
   
}



void CAddFrameDlg::UpdateFrameLabels()
{
   CString  Str;

   if (m_FrameSource >= 0)
   {
      if (m_FrameSource >= g_CurMdl.Head.num_frames)
         m_FrameSource = g_CurMdl.Head.num_frames - 1;

      Str.Format("%i", m_FrameSource);
      m_FrameNumEdit.SetWindowText(Str);
      m_FrameNameEdit.SetWindowText(g_CurMdl.FrameInfo[m_FrameSource].Name);

      m_FrameTarget = m_FrameSource + 1;

   }
   else if (m_FrameSource == FRAMESRC_BEFOREFIRST)
   {
      m_FrameTarget = 0;

   }
   else if (m_FrameSource == FRAMESRC_AFTERFINAL)
   {
      m_FrameTarget = g_CurMdl.Head.num_frames;
   }

   Str.Format("%i", m_FrameTarget);
   m_NewFrameNumEdit.SetWindowText(Str);
}

void CAddFrameDlg::OnAfterframecheck() 
{
   char     Num[16];

   m_FrameNumEdit.GetWindowText(Num, 16);
   m_FrameSource = atoi(Num);

   UpdateFrameLabels();
}

void CAddFrameDlg::OnChangeFramenumedit() 
{

/*   char     Num[16];

   m_FrameNumEdit.GetWindowText(Num, 16);

   if (Num[0] = '\x0')
      strcpy(Num, "0");
   
   m_FrameSource = atoi(Num);

   UpdateFrameLabels();
*/	
}

void CAddFrameDlg::OnDeltaposFramenumspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   // Make sure that the spinner is allowed to change the values.
   if (m_FrameSource < 0)
      return;

   m_FrameSource -= pNMUpDown->iDelta;

   if (m_FrameSource < 0)
      m_FrameSource = 0;
   if (m_FrameSource >= g_CurMdl.Head.num_frames)
      m_FrameSource = g_CurMdl.Head.num_frames -1;
   UpdateFrameLabels();

	*pResult = 0;
}

BOOL CAddFrameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   m_FrameSource = FRAMESRC_BEFOREFIRST;
   UpdateFrameLabels();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CDeleteFrameRangeDlg dialog


CDeleteFrameRangeDlg::CDeleteFrameRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeleteFrameRangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeleteFrameRangeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDeleteFrameRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeleteFrameRangeDlg)
	DDX_Control(pDX, IDC_STARTNUMEDIT, m_StartNumEdit);
	DDX_Control(pDX, IDC_STARTNAMEEDIT, m_StartNameEdit);
	DDX_Control(pDX, IDC_ENDNUMEDIT, m_EndNumEdit);
	DDX_Control(pDX, IDC_ENDNAMEEDIT, m_EndNameEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeleteFrameRangeDlg, CDialog)
	//{{AFX_MSG_MAP(CDeleteFrameRangeDlg)
	ON_BN_CLICKED(IDC_CHOOSESTARTBUT, OnChoosestartbut)
	ON_BN_CLICKED(IDC_CHOOSEENDBUT, OnChooseendbut)
	ON_NOTIFY(UDN_DELTAPOS, IDC_STARTFRAMESPIN, OnDeltaposStartframespin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ENDFRAMESPIN, OnDeltaposEndframespin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeleteFrameRangeDlg message handlers

void CDeleteFrameRangeDlg::OnChoosestartbut() 
{
   CChooseFrameDlg   ChooseDlg;

   if (ChooseDlg.DoModal() == IDCANCEL)
      return;

   m_FrameEnd = ChooseDlg.m_SelectedFrame;
   if (m_FrameEnd < m_FrameStart)
      m_FrameStart = m_FrameEnd;

   UpdateFrameLabels();
}

void CDeleteFrameRangeDlg::OnChooseendbut() 
{
   CChooseFrameDlg   ChooseDlg;

   if (ChooseDlg.DoModal() == IDCANCEL)
      return;

   m_FrameEnd = ChooseDlg.m_SelectedFrame;
   if (m_FrameEnd < m_FrameStart)
      m_FrameStart = m_FrameEnd;

   UpdateFrameLabels();
}

void CDeleteFrameRangeDlg::UpdateFrameLabels()
{
   CString Str;

   if (m_FrameStart < 0)
      m_FrameStart = 0;
   if (m_FrameEnd >= g_CurMdl.Head.num_frames)
      m_FrameEnd = g_CurMdl.Head.num_frames-1;

   if (m_FrameEnd < 0)
      m_FrameEnd = 0;
   if (m_FrameStart >= g_CurMdl.Head.num_frames)
      m_FrameStart = g_CurMdl.Head.num_frames-1;

   if (m_FrameEnd < m_FrameStart)
      m_FrameEnd = m_FrameStart;

   m_StartNameEdit.SetWindowText(g_CurMdl.FrameInfo[m_FrameStart].Name);
   Str.Format("%i", m_FrameStart);
   m_StartNumEdit.SetWindowText(Str);

   m_EndNameEdit.SetWindowText(g_CurMdl.FrameInfo[m_FrameEnd].Name);
   Str.Format("%i", m_FrameEnd);
   m_EndNumEdit.SetWindowText(Str);

}

void CDeleteFrameRangeDlg::OnDeltaposStartframespin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   m_FrameStart -= pNMUpDown->iDelta;
   UpdateFrameLabels();
	
	*pResult = 0;
}

void CDeleteFrameRangeDlg::OnDeltaposEndframespin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   m_FrameEnd -= pNMUpDown->iDelta;
   UpdateFrameLabels();

	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////
// CMoveFramesDlg dialog


CMoveFramesDlg::CMoveFramesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMoveFramesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMoveFramesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMoveFramesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveFramesDlg)
	DDX_Control(pDX, IDC_TARGETNUMEDIT, m_TargetNumEdit);
	DDX_Control(pDX, IDC_TARGETNAMEEDIT, m_TargetNameEdit);
	DDX_Control(pDX, IDC_STARTNUMEDIT, m_StartNumEdit);
	DDX_Control(pDX, IDC_STARTNAMEEDIT, m_StartNameEdit);
	DDX_Control(pDX, IDC_ENDNUMEDIT, m_EndNumEdit);
	DDX_Control(pDX, IDC_ENDNAMEEDIT, m_EndNameEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveFramesDlg, CDialog)
	//{{AFX_MSG_MAP(CMoveFramesDlg)
	ON_BN_CLICKED(IDC_CHOOSESTARTBUT, OnChoosestartbut)
	ON_BN_CLICKED(IDC_CHOOSEENDBUT, OnChooseendbut)
	ON_BN_CLICKED(IDC_CHOOSETARGETBUT, OnChoosetargetbut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoveFramesDlg message handlers

void CMoveFramesDlg::OnChoosestartbut() 
{
   CChooseFrameDlg   ChooseDlg;

   if (ChooseDlg.DoModal() == IDCANCEL)
      return;

   m_FrameStart = ChooseDlg.m_SelectedFrame;
   UpdateFrameLabels();
}

void CMoveFramesDlg::OnChooseendbut() 
{
   CChooseFrameDlg   ChooseDlg;

   if (ChooseDlg.DoModal() == IDCANCEL)
      return;

   m_FrameEnd = ChooseDlg.m_SelectedFrame;
   UpdateFrameLabels();
}

void CMoveFramesDlg::OnChoosetargetbut() 
{
   CChooseFrameDlg   ChooseDlg;

   if (ChooseDlg.DoModal() == IDCANCEL)
      return;

   m_FrameTarget = ChooseDlg.m_SelectedFrame;
   UpdateFrameLabels();
}

void CMoveFramesDlg::UpdateFrameLabels()
{
   CString Str;

   if (m_FrameStart < 0)
      m_FrameStart = 0;
   if (m_FrameEnd >= g_CurMdl.Head.num_frames)
      m_FrameEnd = g_CurMdl.Head.num_frames-1;

   if (m_FrameEnd < 0)
      m_FrameEnd = 0;
   if (m_FrameStart >= g_CurMdl.Head.num_frames)
      m_FrameStart = g_CurMdl.Head.num_frames-1;

   if (m_FrameEnd < m_FrameStart)
      m_FrameEnd = m_FrameStart;

   if (m_FrameTarget < 0)
      m_FrameTarget = 0;
   if (m_FrameTarget >= g_CurMdl.Head.num_frames)
      m_FrameTarget = g_CurMdl.Head.num_frames-1;

   if (m_FrameTarget >= m_FrameStart && m_FrameTarget <= m_FrameEnd) // This shouldn't be possible
      m_FrameTarget = m_FrameEnd + 1;

   m_StartNameEdit.SetWindowText(g_CurMdl.FrameInfo[m_FrameStart].Name);
   Str.Format("%i", m_FrameStart);
   m_StartNumEdit.SetWindowText(Str);

   m_EndNameEdit.SetWindowText(g_CurMdl.FrameInfo[m_FrameEnd].Name);
   Str.Format("%i", m_FrameEnd);
   m_EndNumEdit.SetWindowText(Str);

   m_TargetNameEdit.SetWindowText(g_CurMdl.FrameInfo[m_FrameTarget].Name);
   Str.Format("%i", m_FrameTarget);
   m_TargetNumEdit.SetWindowText(Str);
}

void CMoveFramesDlg::OnOK() 
{
   CButton  *BeforeCheck;

   BeforeCheck = (CButton*)GetDlgItem(IDC_BEFOREFRAMECHECK);
   if (BeforeCheck->GetCheck() == 1)
      m_BeforeFrame = true;
   else
      m_BeforeFrame = false;
	
	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CGroundPlanePosDlg dialog


CGroundPlanePosDlg::CGroundPlanePosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroundPlanePosDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroundPlanePosDlg)
	//}}AFX_DATA_INIT

   m_GroundPlanePos = 0.0;
}


void CGroundPlanePosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroundPlanePosDlg)
	DDX_Control(pDX, IDC_GPPOSEDIT, m_GroundPlanePosEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroundPlanePosDlg, CDialog)
	//{{AFX_MSG_MAP(CGroundPlanePosDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroundPlanePosDlg message handlers

BOOL CGroundPlanePosDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

   CString  Text;
   Text.Format("%.1f", m_GroundPlanePos);
   m_GroundPlanePosEdit.SetWindowText(Text);
   

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGroundPlanePosDlg::OnOK() 
{
   char  Buf[128];
   m_GroundPlanePosEdit.GetWindowText(Buf,128);
   m_GroundPlanePos = atof(Buf);
   
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CExtrudeDistDlg dialog


CExtrudeDistDlg::CExtrudeDistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtrudeDistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtrudeDistDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

   m_ExtrudeDist = 0.0;
}


void CExtrudeDistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtrudeDistDlg)
	DDX_Control(pDX, IDC_EXTRUDEEDIT, m_ExtrudeEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtrudeDistDlg, CDialog)
	//{{AFX_MSG_MAP(CExtrudeDistDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtrudeDistDlg message handlers

void CExtrudeDistDlg::OnOK() 
{
   char  Str[64];

   m_ExtrudeEdit.GetWindowText(Str,64);
   m_ExtrudeDist = (float)atof(Str);
      
	CDialog::OnOK();
}

BOOL CExtrudeDistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   CString  Str;
   Str.Format("%f", m_ExtrudeDist);
   m_ExtrudeEdit.SetWindowText(Str);
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
// CSkinVertsFromProjectionDlg dialog


CSkinVertsFromProjectionDlg::CSkinVertsFromProjectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkinVertsFromProjectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkinVertsFromProjectionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSkinVertsFromProjectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkinVertsFromProjectionDlg)
	DDX_Control(pDX, IDC_TOLERANCESPIN, m_ToleranceSpin);
	DDX_Control(pDX, IDC_TOLERANCEEDIT, m_ToleranceEdit);
	DDX_Control(pDX, IDC_MIRRORCHECK, m_MirrorCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkinVertsFromProjectionDlg, CDialog)
	//{{AFX_MSG_MAP(CSkinVertsFromProjectionDlg)
	ON_BN_CLICKED(IDC_FRONTRADIO, OnFrontradio)
	ON_BN_CLICKED(IDC_RIGHTRADIO, OnRightradio)
	ON_BN_CLICKED(IDC_TOPRADIO, OnTopradio)
	ON_BN_CLICKED(IDC_BACKRADIO, OnBackradio)
	ON_BN_CLICKED(IDC_LEFTRADIO, OnLeftradio)
	ON_BN_CLICKED(IDC_BOTTOMRADIO, OnBottomradio)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TOLERANCESPIN, OnDeltaposTolerancespin)
	ON_EN_KILLFOCUS(IDC_TOLERANCEEDIT, OnKillfocusToleranceedit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinVertsFromProjectionDlg message handlers

void CSkinVertsFromProjectionDlg::OnFrontradio() 
{
   m_ProjectionType = svFront;
	
}

void CSkinVertsFromProjectionDlg::OnRightradio() 
{
   m_ProjectionType = svRight;
	
}

void CSkinVertsFromProjectionDlg::OnTopradio() 
{
   m_ProjectionType = svRight;
	
}

void CSkinVertsFromProjectionDlg::OnBackradio() 
{
   m_ProjectionType = svBack;
	
}

void CSkinVertsFromProjectionDlg::OnLeftradio() 
{
   m_ProjectionType = svLeft;
	
}

void CSkinVertsFromProjectionDlg::OnBottomradio() 
{
   m_ProjectionType = svBottom;
	
}

BOOL CSkinVertsFromProjectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   CButton  *FrontRadio;
   FrontRadio = (CButton*)GetDlgItem(IDC_FRONTRADIO);
   if (FrontRadio)
   {
      FrontRadio->SetCheck(1);
   }
   m_MirrorCheck.SetCheck(1);

   m_ProjectionType = svFront;
   m_Tolerance = 90.0;
   m_Mirror = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSkinVertsFromProjectionDlg::UpdateSpinEdits()
{
   CString  Str;

   if (m_Mirror)
      m_MirrorCheck.SetCheck(1);
   else
      m_MirrorCheck.SetCheck(0);

   Str.Format("%.1f", m_Tolerance);
   m_ToleranceEdit.SetWindowText(Str);
}

void CSkinVertsFromProjectionDlg::OnDeltaposTolerancespin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   m_Tolerance -= pNMUpDown->iDelta;
   UpdateSpinEdits();

	*pResult = 0;
}

void CSkinVertsFromProjectionDlg::OnKillfocusToleranceedit() 
{
   char  Str[16];

   m_ToleranceEdit.GetWindowText(Str, 16);
   m_Tolerance = (float)atof(Str);
   UpdateSpinEdits();
   
}
/////////////////////////////////////////////////////////////////////////////
// CResizeSkinDlg dialog


CResizeSkinDlg::CResizeSkinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResizeSkinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResizeSkinDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CResizeSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResizeSkinDlg)
	DDX_Control(pDX, IDC_OLDWIDTHEDIT, m_OldWidthEdit);
	DDX_Control(pDX, IDC_OLDHEIGHTEDIT, m_OldHeightEdit);
	DDX_Control(pDX, IDC_NEWWIDTHEDIT, m_NewWidthEdit);
	DDX_Control(pDX, IDC_NEWHEIGHTEDIT, m_NewHeightEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResizeSkinDlg, CDialog)
	//{{AFX_MSG_MAP(CResizeSkinDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizeSkinDlg message handlers

BOOL CResizeSkinDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   char     Str[16];

   itoa(m_NewWidth, Str, 10);
   m_NewWidthEdit.SetWindowText(Str);
   itoa(m_NewHeight, Str, 10);
   m_NewHeightEdit.SetWindowText(Str);
   itoa(m_OldWidth, Str, 10);
   m_OldWidthEdit.SetWindowText(Str);
   itoa(m_OldHeight, Str, 10);
   m_OldHeightEdit.SetWindowText(Str);

   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResizeSkinDlg::OnOK() 
{
   char  Str[16];

   m_NewWidthEdit.GetWindowText(Str,16);
   m_NewWidth = atoi(Str);
   m_NewHeightEdit.GetWindowText(Str,16);
   m_NewHeight = atoi(Str);
   m_OldWidthEdit.GetWindowText(Str,16);
   m_OldWidth = atoi(Str);
   m_OldHeightEdit.GetWindowText(Str,16);
   m_OldHeight = atoi(Str);


	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CInsertSkinDlg dialog


CInsertSkinDlg::CInsertSkinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsertSkinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsertSkinDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInsertSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertSkinDlg)
	DDX_Control(pDX, IDC_INSERTAFTERSPIN, m_InsertAfterSpin);
	DDX_Control(pDX, IDC_INSERTAFTEREDIT, m_InsertAfterEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsertSkinDlg, CDialog)
	//{{AFX_MSG_MAP(CInsertSkinDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_INSERTAFTERSPIN, OnDeltaposInsertafterspin)
	ON_BN_CLICKED(IDC_BEFOREFIRSTRADIO, OnBeforefirstradio)
	ON_BN_CLICKED(IDC_AFTERLASTRADIO, OnAfterlastradio)
	ON_BN_CLICKED(IDC_AFTERSKINRADIO, OnAfterskinradio)
	ON_EN_KILLFOCUS(IDC_INSERTAFTEREDIT, OnKillfocusInsertafteredit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertSkinDlg message handlers

BOOL CInsertSkinDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   CButton   *Radio;

   Radio = (CButton*)this->GetDlgItem(IDC_AFTERLASTRADIO);
   if (Radio)
   {
      Radio->SetCheck(1);
   }

   m_InsertSkin = m_MaxSkin;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInsertSkinDlg::UpdateSpinEdits()
{
   char     Str[16];

   itoa(m_InsertSkin, Str,10);
   m_InsertAfterEdit.SetWindowText(Str);
}

void CInsertSkinDlg::OnDeltaposInsertafterspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   m_InsertSkin -= pNMUpDown->iDelta;
   
   if (m_InsertSkin < m_MinSkin)
      m_InsertSkin = m_MinSkin;
   if (m_InsertSkin > m_MaxSkin)
      m_InsertSkin = m_MaxSkin;

   UpdateSpinEdits();

	*pResult = 0;
}

void CInsertSkinDlg::OnBeforefirstradio() 
{
   m_InsertAfter = false;
   
}

void CInsertSkinDlg::OnAfterlastradio() 
{
   m_InsertAfter = true;
   m_InsertSkin = m_MaxSkin;

   UpdateSpinEdits();
}

void CInsertSkinDlg::OnAfterskinradio() 
{
   m_InsertAfter = true;
   UpdateSpinEdits();
	
}

void CInsertSkinDlg::OnKillfocusInsertafteredit() 
{
   char     Str[16];

   m_InsertAfterEdit.GetWindowText(Str, 16);
   m_InsertSkin = atoi(Str);

   UpdateSpinEdits();
   
}
