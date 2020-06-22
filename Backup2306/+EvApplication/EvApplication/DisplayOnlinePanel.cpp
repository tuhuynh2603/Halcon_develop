// DisplayOnlinePanel.cpp : implementation file
//

#include "stdafx.h"
#include "DisplayOnlinePanel.h"
#include "Application.h"
#include "afxdialogex.h"
#include "Resource.h"

// CDisplayOnlinePanel dialog

IMPLEMENT_DYNAMIC(CDisplayOnlinePanel, CDialogEx)

CDisplayOnlinePanel::CDisplayOnlinePanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDC_ONLINE_DISPLAY_PANEL, pParent)
{
	m_pBrushTemp[0].CreateSolidBrush(RGB(0, 0, 153));
	m_pBrushTemp[1].CreateSolidBrush(RGB(230, 50, 50));
	m_pBrushTemp[2].CreateSolidBrush(RGB(255, 0, 255));
	m_pBrushTemp[3].CreateSolidBrush(RGB(50, 180, 50));
}

CDisplayOnlinePanel::~CDisplayOnlinePanel()
{
	m_pBrushTemp[0].DeleteObject();
	m_pBrushTemp[1].DeleteObject();
	m_pBrushTemp[2].DeleteObject();
	m_pBrushTemp[3].DeleteObject();
}

void CDisplayOnlinePanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ONLINE_RECEIPE_STATUS_STATIC, m_strRecipe);
	DDX_Text(pDX, IDC_ONLINE_LOT_ID_STATIC, m_strLotId);
	DDX_Text(pDX, IDC_ONLINE_MACHINE_ID_STATIC, m_strMachineId);
	DDX_Text(pDX, IDC_ONLINE_CURTAIN_STATUS_STATIC, m_strCurtainbypass);
}


BEGIN_MESSAGE_MAP(CDisplayOnlinePanel, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDisplayOnlinePanel message handlers


BOOL CDisplayOnlinePanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDisplayOnlinePanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	m_pBrush = &m_pBrushTemp[3];
	if (pWnd->GetDlgCtrlID() == IDC_ONLINE_LOT_ID_STATIC) {
		m_pBrush = &m_pBrushTemp[0];
	}
	else if (pWnd->GetDlgCtrlID() == IDD_DIALOG_HARDWARE_STATUS) {
		m_pBrush = &m_pBrushTemp[4];
	}
	else if (pWnd->GetDlgCtrlID() == IDC_ONLINE_RECEIPE_STATUS_STATIC) {
		m_pBrush = &m_pBrushTemp[2];
	}
	else if (pWnd->GetDlgCtrlID() == IDC_ONLINE_CURTAIN_STATUS_STATIC) {
		m_pBrush = &m_pBrushTemp[3];
	}
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));

	return *m_pBrush;
}
