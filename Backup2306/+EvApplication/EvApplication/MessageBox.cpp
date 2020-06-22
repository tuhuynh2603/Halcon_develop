// MessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MessageBox.h"
#include "afxdialogex.h"

#define TIMER 1
// CMessageBox dialog

IMPLEMENT_DYNAMIC(CMessageBox, CDialog)

CMessageBox::CMessageBox(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_HAWKEYE_MSG_BOX, pParent)
	, m_strMsg(_T("Here is the defauly message at constructor"))
{
	nDuration = 0;
	bTimerOn = false;
}

CMessageBox::~CMessageBox()
{
	if(bTimerOn) {
		KillTimer(TIMER);
		Sleep(nDuration * 2);
	}
}

void CMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HAWKEYEMSG_LBL, m_strMsg);

}


BEGIN_MESSAGE_MAP(CMessageBox, CDialog)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CMessageBox::Msg(CString _strMsg, int _nDuration)
{
	m_strMsg = _strMsg;
	nDuration = _nDuration;
	ShowWindow(SW_SHOW);
}


BOOL CMessageBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Get application size
	CRect rcClientAppication;
	CRect rcClientAppicationRelative;

	AfxGetMainWnd()->GetClientRect(&rcClientAppication);
	rcClientAppicationRelative = rcClientAppication;
	ClientToScreen(&rcClientAppicationRelative);


	//Change window size and set proper location of objects
	const int nWindowWidth = rcClientAppicationRelative.Width() - 10;
	const int nWindowHeight = rcClientAppicationRelative.Height() / 3;

	SetWindowPos(NULL, rcClientAppicationRelative.left, rcClientAppicationRelative.top + rcClientAppication.Height() / 4, nWindowWidth, nWindowHeight, SWP_HIDEWINDOW);

	//Position Lbl
	GetDlgItem(IDC_HAWKEYEMSG_LBL)->SetWindowPos(NULL,
		0,
		40,
		nWindowWidth - 10,
		nWindowHeight / 2,
		SWP_NOZORDER);

	//Font
	font.CreateFont(nWindowWidth / 20, 0, 0, 0, 700, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Arial");

	GetDlgItem(IDC_HAWKEYEMSG_LBL)->SetFont(&font);

	return TRUE;
}


void CMessageBox::OnTimer(UINT_PTR nIDEvent)
{
	bTimerOn = true;

	if(nIDEvent == TIMER)
		ShowWindow(SW_HIDE);

	CDialog::OnTimer(nIDEvent);
}


void CMessageBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	UpdateData(FALSE);

	if(bShow && nDuration > 0) {

		SetTimer(TIMER, nDuration, NULL);

	}
	else {

		KillTimer(TIMER);
		bTimerOn = false;

	}

}
// CMessageBox message handlers
