// TerminalServiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TerminalServiceDlg.h"
#include "afxdialogex.h"


// TerminalServiceDlg dialog
#define COLOR_BACKROUND RGB(94,136,158)

IMPLEMENT_DYNAMIC(TerminalServiceDlg, CDialog)

TerminalServiceDlg::TerminalServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EQUIP_TERMINAL_SERVICE_DIALOG, pParent)
	, m_strClientMessage(_T(""))
	, m_strHostMessage(_T(""))
{
	m_brush.CreateSolidBrush(COLOR_BACKROUND);

}

TerminalServiceDlg::~TerminalServiceDlg()
{
	m_brush.DeleteObject();
}

void TerminalServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CLIENT_MESSAGE, m_strClientMessage);
	DDX_Text(pDX, IDC_EDIT_HOST_MESSAGE, m_strHostMessage);
}


BEGIN_MESSAGE_MAP(TerminalServiceDlg, CDialog)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, &TerminalServiceDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SEND_MSG, &TerminalServiceDlg::OnBnClickedButtonSendMsg)
END_MESSAGE_MAP()


// TerminalServiceDlg message handlers


BOOL TerminalServiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strHostMessage = theApp.m_strHostMessageRecvd;
	m_strClientMessage = "Test Client";

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void TerminalServiceDlg::OnBnClickedOk()
{
	theApp.m_bTerminalService = false;
	CDialog::OnOK();
}


void TerminalServiceDlg::OnBnClickedButtonSendMsg()
{
	UpdateData(TRUE);
	if (theApp.isSecsGemInitialized())
		theApp.getGEMController().SendTerminalMessageToHost(m_strClientMessage);
	else
	{
		AfxMessageBox("Failed to Initialize SECSGEM. Please ensure that Dongle is connected and relaunch the application.");
	}
}

HBRUSH TerminalServiceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pWnd == this)
		return m_brush;
	else {
		if (nCtlColor == CTLCOLOR_STATIC) {
			pDC->SetBkColor(COLOR_BACKROUND);
			return m_brush;
		}
		return NULL;
	}
}