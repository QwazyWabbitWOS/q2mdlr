// PasteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VCMDL.h"
#include "PasteDlg.h"

#include "MdlGlobs.h"
#include "MiscDlgs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasteMethodDlg dialog


CPasteMethodDlg::CPasteMethodDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasteMethodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasteMethodDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPasteMethodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasteMethodDlg)
	DDX_Control(pDX, IDC_BLENDSPIN, m_BlendSpin);
	DDX_Control(pDX, IDC_BLENDEDIT, m_BlendEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasteMethodDlg, CDialog)
	//{{AFX_MSG_MAP(CPasteMethodDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_BLENDSPIN, OnDeltaposBlendspin)
	ON_BN_CLICKED(IDC_APPENDVERTS_RADIO, OnAppendvertsRadio)
	ON_BN_CLICKED(IDC_REPLACEVERTS_RADIO, OnReplacevertsRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasteMethodDlg message handlers

BOOL CPasteMethodDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   CString  BlendText;
	
   m_ReplaceVerts = true;
   m_BlendFactor = (float)100.0;

   BlendText.Format("%f", m_BlendFactor);
   m_BlendEdit.SetWindowText(BlendText);

   
   {
      CButton  *But;
      But = (CButton*)this->GetDlgItem(IDC_REPLACEVERTS_RADIO);

      But->SetCheck(1);
   }


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPasteMethodDlg::OnDeltaposBlendspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   CString  BlendText;

   m_BlendFactor -= pNMUpDown->iDelta;

   if (m_BlendFactor > 100.0)
      m_BlendFactor = 100.0;

   if (m_BlendFactor < 0.)
      m_BlendFactor = 0.0;

   BlendText.Format("%f", m_BlendFactor);

   m_BlendEdit.SetWindowText(BlendText);

	*pResult = 0;
}

void CPasteMethodDlg::OnAppendvertsRadio() 
{
   m_ReplaceVerts = false;
   
}

void CPasteMethodDlg::OnReplacevertsRadio() 
{
	m_ReplaceVerts = true;
}
/////////////////////////////////////////////////////////////////////////////
// CPasteRangeDialog dialog


CPasteRangeDialog::CPasteRangeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPasteRangeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasteRangeDialog)
	//}}AFX_DATA_INIT
}


void CPasteRangeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasteRangeDialog)
	DDX_Control(pDX, IDC_STARTNUMEDIT, m_StartNumEdit);
	DDX_Control(pDX, IDC_STARTNAMEEDIT, m_StartNameEdit);
	DDX_Control(pDX, IDC_ENDNAMEEDIT, m_EndNameEdit);
	DDX_Control(pDX, IDC_ENDNUMEDIT, m_EndNumEdit);
	DDX_Control(pDX, IDC_CHOOSESTARTBUT, m_ChooseStartBut);
	DDX_Control(pDX, IDC_CHOOSEENDBUT, m_ChooseEndBut);
	DDX_Control(pDX, IDC_BLENDSPIN, m_BlendSpin);
	DDX_Control(pDX, IDC_BLENDEDIT, m_BlendEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasteRangeDialog, CDialog)
	//{{AFX_MSG_MAP(CPasteRangeDialog)
	ON_BN_CLICKED(IDC_CHOOSESTARTBUT, OnChoosestartbut)
	ON_BN_CLICKED(IDC_CHOOSEENDBUT, OnChooseendbut)
	ON_NOTIFY(UDN_DELTAPOS, IDC_STARTFRAMESPIN, OnDeltaposStartframespin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ENDFRAMESPIN, OnDeltaposEndframespin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_BLENDSPIN, OnDeltaposBlendspin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasteRangeDialog message handlers

BOOL CPasteRangeDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_FrameStart = 0;
   m_FrameEnd = g_CurMdl.Head.num_frames;

   m_BlendFactor = 100.0;
   
   UpdateFrameLabels();
   
   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPasteRangeDialog::OnChoosestartbut() 
{
   CChooseFrameDlg   ChooseDlg;

   if (ChooseDlg.DoModal() == IDCANCEL)
      return;

   m_FrameStart = ChooseDlg.m_SelectedFrame;
   if (m_FrameEnd < m_FrameStart)
      m_FrameEnd = m_FrameStart;

   UpdateFrameLabels();
   
}

void CPasteRangeDialog::OnChooseendbut() 
{
   CChooseFrameDlg   ChooseDlg;

   if (ChooseDlg.DoModal() == IDCANCEL)
      return;

   m_FrameEnd = ChooseDlg.m_SelectedFrame;
   if (m_FrameEnd < m_FrameStart)
      m_FrameStart = m_FrameEnd;

   UpdateFrameLabels();
}


void CPasteRangeDialog::UpdateFrameLabels()
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

void CPasteRangeDialog::OnDeltaposStartframespin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   m_FrameStart -= pNMUpDown->iDelta;
   UpdateFrameLabels();

	*pResult = 0;
}

void CPasteRangeDialog::OnDeltaposEndframespin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   m_FrameEnd -= pNMUpDown->iDelta;
   UpdateFrameLabels();
	
	*pResult = 0;
}

void CPasteRangeDialog::OnDeltaposBlendspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

   CString  BlendText;

   m_BlendFactor -= pNMUpDown->iDelta;

   if (m_BlendFactor > 100.0)
      m_BlendFactor = 100.0;

   if (m_BlendFactor < 0.)
      m_BlendFactor = 0.0;

   BlendText.Format("%f", m_BlendFactor);

   m_BlendEdit.SetWindowText(BlendText);

	*pResult = 0;
}
