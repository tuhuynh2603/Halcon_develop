// IOTestStatusDialog.cpp : implementation file
//

#include "stdafx.h"
#include "IOTestStatusDialog.h"
#include "afxdialogex.h"


// CIOTestStatusDialog dialog

IMPLEMENT_DYNAMIC(CIOTestStatusDialog, CDialogEx)

CIOTestStatusDialog::CIOTestStatusDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_HARDWARE_STATUS, pParent)
	, m_strIOHStatus(_T(""))
{
	
	m_pbrushStatus[0].CreateSolidBrush(RGB(0, 0, 153));
	m_pbrushStatus[1].CreateSolidBrush(RGB(230, 50, 50));
	m_pbrushStatus[2].CreateSolidBrush(RGB(255, 0, 255));
	m_pbrushStatus[3].CreateSolidBrush(RGB(50, 180, 50));
	m_pbrushStatus[4].CreateSolidBrush(RGB(255, 0, 0));

}

CIOTestStatusDialog::~CIOTestStatusDialog()
{

	m_pbrushStatus[0].DeleteObject();
	m_pbrushStatus[1].DeleteObject();
	m_pbrushStatus[2].DeleteObject();
	m_pbrushStatus[3].DeleteObject();
}

void CIOTestStatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_strIOHStatus);
}


BEGIN_MESSAGE_MAP(CIOTestStatusDialog, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CIOTestStatusDialog message handlers


BOOL CIOTestStatusDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rectDesktop, rectWnd, rectFrame;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rectDesktop, NULL);
	m_fontHead.CreateFont((int)(rectDesktop.Width() / 1280.0 * 16), 0, 0, 0, 350, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Segoe UI Semibold");

	// TODO:  Add extra initialization here
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}



HBRUSH CIOTestStatusDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	/*if (pWnd->GetDlgCtrlID() == IDC_STATIC_STATUS) {
		pDC->SetTextColor(RGB(0, 64, 255));
		pDC->SetBkMode(TRANSPARENT);
	}*/
	
	

	
	m_pBrush = &m_pbrushStatus[5];
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_STATUS) {
		m_pBrush = &m_pbrushStatus[m_nErrorCode];
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));

	}
	/*else if (pWnd->GetDlgCtrlID() == IDC_ONLINE_MACHINE_ID_STATIC) {
		m_pBrush = &m_pbrushStatus[4];
	}*/
	/*else if (pWnd->GetDlgCtrlID() == IDC_ONLINE_RECEIPE_STATUS_STATIC) {
		m_pBrush = &m_pbrushStatus[2];
	}*/
	
	
	hbr = *m_pBrush;

		return hbr;



	// TODO:  Return a different brush if the default is not desired
	
}
