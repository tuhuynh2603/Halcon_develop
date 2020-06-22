// DeviceConfig.cpp : implementation file
//

#include "stdafx.h"
#include "DeviceConfig.h"
#include "afxdialogex.h"
#include "Application.h"
#include "DatumOffset.h"

// CDeviceConfig dialog

IMPLEMENT_DYNAMIC(CDeviceConfig, CDialogEx)

CDeviceConfig::CDeviceConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceConfig::IDD, pParent)
{
	pApp = (CApplication*) AfxGetApp();
	pMotionParms = &pApp->motionParameters;

	for(int i=0; i<MAX_DIRECTION; i++) {
		m_nNoOfBlock[i] = 0;
		m_dBlockPitch[i] = 0;
		m_nDeviceNoPerBlock[i] = 0;
		m_dDevicePitch[i] = 0;
	//	m_dDeviceScanArea[i] = 0;
		m_nDeviceOffset[i] = 0;
		m_dInitialOffset = 0;

	}
}

CDeviceConfig::~CDeviceConfig()
{
}

void CDeviceConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for(int i=0; i<MAX_DIRECTION; i++) {
		DDX_Text(pDX, IDC_DC_NO_OF_BLOCK_X_EDIT+i, m_nNoOfBlock[i]);
		DDX_Text(pDX, IDC_DC_BLOCK_PITCH_X_EDIT+i, m_dBlockPitch[i]);
		DDX_Text(pDX, IDC_DC_DEVICE_NO_PER_BLOCK_X_EDIT+i, m_nDeviceNoPerBlock[i]);
		DDX_Text(pDX, IDC_DC_DEVICE_NO_PER_BLOCK_X_EDIT_TOP_STATION + i, m_nDeviceNoPerBlockTOP[i]);
		DDX_Text(pDX, IDC_DC_DEVICE_PITCH_X_EDIT+i, m_dDevicePitch[i]);
	}
	DDX_Text(pDX, IDC_DC_DEVICE_INITIAL_OFFSET, m_dInitialOffset);
}


BEGIN_MESSAGE_MAP(CDeviceConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDeviceConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDeviceConfig::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DATUM_OFFSET, &CDeviceConfig::OnBnClickedDatumOffset)
END_MESSAGE_MAP()


// CDeviceConfig message handlers


BOOL CDeviceConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	UpdateData(TRUE);
	GetDlgItem(IDC_DATUM_OFFSET)->EnableWindow(theApp.m_bEnableTopDatumPosToScanTHR);
	LoadData();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDeviceConfig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	double dTempFOVX, dTempFOVY, dTempDie;

	SaveData();
	
	UpdateData(FALSE);
	CDialogEx::OnOK();
}

void CDeviceConfig::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateData(FALSE);

	CDialogEx::OnCancel();
}

void CDeviceConfig::LoadData()
{
	UpdateData(TRUE);
	pMotionParms->getDeviceSize(m_dBlockPitch[0], m_dBlockPitch[1]);

	int m_nDieXPerImage;
	int m_nDieYPerImage;

	pMotionParms->GetMappingParameters(m_nNoOfBlock[0], m_nNoOfBlock[1], m_dBlockPitch[0], m_dBlockPitch[1], m_nDeviceNoPerBlock[0], m_nDeviceNoPerBlock[1],
		m_dDevicePitch[0], m_dDevicePitch[1], m_nDieXPerImage, m_nDieYPerImage, m_dInitialOffset);

	pMotionParms->GetMappingParametersTOP(m_nDeviceNoPerBlockTOP[0], m_nDeviceNoPerBlockTOP[1]);

	UpdateData(FALSE);

}

void CDeviceConfig::SaveData()
{
	UpdateData(TRUE);
	int m_nDieXPerImage = 1;
	int m_nDieYPerImage = 1;

	//Get the old mapping info
	int nFovNoPerY, nFovNoPerX, nDieNoPerX, nDIeNoPerY,
		nFovNoPerXTOP, nFovNoPerYTOP;
	pMotionParms->GetMappingParameters(nDieNoPerX, nDIeNoPerY, nFovNoPerX, nFovNoPerY);
	pMotionParms->GetMappingParametersTOP(nFovNoPerXTOP, nFovNoPerYTOP);

	
	//Save Device Config Info
	pMotionParms->setDeviceSize(m_dBlockPitch[0], m_dBlockPitch[1]);
	pMotionParms->SetMappingParameters(m_nNoOfBlock[0], m_nNoOfBlock[1], m_dBlockPitch[0], m_dBlockPitch[1], m_nDeviceNoPerBlock[0], m_nDeviceNoPerBlock[1],
		m_dDevicePitch[0], m_dDevicePitch[1], m_nDieXPerImage, m_nDieYPerImage, m_dInitialOffset);

	pMotionParms->SetMappingParametersTOP(m_nDeviceNoPerBlockTOP[0], m_nDeviceNoPerBlockTOP[1]);
	//pMotionParms->SetDatumOffsetParameters(m_dTopRowOffsetX,m_dBottomRowOffsetX, m_dTopRowOffsetY, m_dBottomRowOffsetY);

	pMotionParms->SaveDeviceParameters();

	//If Mapping Info is changed --> Need to Load Recipe Again
	if (nDieNoPerX != m_nNoOfBlock[0] || nDIeNoPerY != m_nNoOfBlock[1] ||
		nFovNoPerX != m_nDeviceNoPerBlock[0] || nFovNoPerY != m_nDeviceNoPerBlock[1] ||
		nFovNoPerXTOP != m_nDeviceNoPerBlockTOP[0] || nFovNoPerYTOP != m_nDeviceNoPerBlockTOP[1]) {
		theApp.m_pMainWnd->SendMessage(WM_LOAD_RECIPE);
	}
	else {
		pMotionParms->CreateScanPositions();
		pMotionParms->CreateNewPositions();
	}
}


void CDeviceConfig::OnBnClickedDatumOffset()
{
	CDatumOffset Dlg;
	Dlg.DoModal();
}
