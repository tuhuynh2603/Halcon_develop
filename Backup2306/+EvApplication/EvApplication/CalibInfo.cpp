// CalibInfo.cpp : implementation file
//

#include "stdafx.h"
#include "CalibInfo.h"
#include "afxdialogex.h"
#include "Application.h"
#include "CalibrationSettings.h"
// CCalibInfo dialog

IMPLEMENT_DYNAMIC(CCalibInfo, CDialogEx)

CCalibInfo::CCalibInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalibInfo::IDD, pParent)
{

	m_nCalibGreyValue = 0;
	m_nCalibValueY = 500;
	m_bDotCalib = FALSE;
	for (int i = 0; i < 2; i++)
		m_nCalibGreyVal[i] = 0;

	m_nMinRadiusDotTarget = 10;
}

CCalibInfo::~CCalibInfo()
{
	m_fontHead.DeleteObject();
}

void CCalibInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CALIB_GREY_VALUE, m_nCalibGreyValue);
	DDX_Text(pDX, IDC_EDIT_CALIB_GREY_VALUE_Y, m_nCalibValueY);
	DDX_Text(pDX, IDC_EDIT_CALIB_RADIUS_DOT_TARGET, m_nMinRadiusDotTarget);


	//DDV_MinMaxInt(pDX, m_nCalibGreyValue, 0, 255);
}


BEGIN_MESSAGE_MAP(CCalibInfo, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCalibInfo::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_START_POS, &CCalibInfo::OnBnClickedButtonStartPos)
END_MESSAGE_MAP()


// CCalibInfo message handlers


void CCalibInfo::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	CDialogEx::OnOK();
}


BOOL CCalibInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rectDesktop, rectWnd, rectFrame;
	//if(m_fontHead)
	m_fontHead.DeleteObject();
	m_fontHead.CreateFont((int)(rectDesktop.Width() / 1280.0 * 16), 0, 0, 0, 350, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Segoe UI Semibold");

	// TODO:  Add extra initialization here
	CString str;
	str.Format("1)Please place %s on track", !m_bDotCalib ? "Target" : "calib plate");
	GetDlgItem(IDC_STATIC_MSG_1)->SetWindowTextA(str);
	str.Format("2)Please make sure %s covers whole FOV area", !m_bDotCalib ? "Target" : "calib plate");
	GetDlgItem(IDC_STATIC_MSG_2)->SetWindowTextA(str);
	str.Format("3)%s", !m_bDotCalib ? "Please enter grey value for light correction" : "Please enter pixel distance along X and Y axes(microns)");
	GetDlgItem(IDC_STATIC_MSG_3)->SetWindowTextA(str);
	
//	str.Format("%d", !m_bDotCalib ? 0 : 500);
	/*str.Format("%d", m_nCalibGreyVal[m_bDotCalib]);
	GetDlgItem(IDC_EDIT_CALIB_GREY_VALUE)->SetWindowTextA(str);*/
	GetDlgItem(IDC_STATIC_CALIBRATION_NAME)->SetFont(&m_fontHead);
	GetDlgItem(IDC_EDIT_CALIB_GREY_VALUE_Y)->ShowWindow(m_bDotCalib);

	GetDlgItem(IDC_STATIC_CALIBRATION_NAME)->SetWindowText(strCalibrationName);

	//Minimum radius of dot target in pixel
	GetDlgItem(IDC_EDIT_CALIB_RADIUS_DOT_TARGET)->ShowWindow(m_bDotCalib);
	if(m_bDotCalib)
		GetDlgItem(IDC_STATIC_MSG_4)->SetWindowTextA("4) Please enter minimum radius of the dot target(pixel)");
	else
		GetDlgItem(IDC_STATIC_MSG_4)->SetWindowTextA("");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalibInfo::OnBnClickedButtonStartPos()
{
	CApplication *pApp = (CApplication*)AfxGetApp();
	// TODO: Add your control notification handler code here
	
}
