// SequenceOptions.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "SequenceOptions.h"
#include "GlobalBackgroundThread.h"
#include "afxdialogex.h"
	

// SequenceOptions dialog

IMPLEMENT_DYNAMIC(SequenceOptions, CDialogEx)

SequenceOptions::SequenceOptions(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SEQUENCE_OPTIONS, pParent)
	, m_bEnableBarcodeReader(TRUE)
	, m_bEnableIonizer(TRUE)
	, m_bDryRun(FALSE)
	, m_bWetRun(FALSE)
	, m_bEnableTopScanning(TRUE)
	, m_bEnableTHRScanning(TRUE)
	, m_bEnableDatumOffset(FALSE)
	, m_bEnableBarcodeReaderWithCylinder(FALSE)
{
	pSequence = theApp.pGlobalBackgroundThread;
	pMotionParms = &theApp.motionParameters;
}

SequenceOptions::~SequenceOptions()
{
}

void SequenceOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ENABLE_BARCODE, m_bEnableBarcodeReader);
	
	DDX_Check(pDX, IDC_CHECK_ENABLE_IONIZER, m_bEnableIonizer);
	DDX_Check(pDX, IDC_CHECK_DRY_RUN, m_bDryRun);
	DDX_Check(pDX, IDC_CHECK_WET_RUN, m_bWetRun);
	DDX_Check(pDX, IDC_CHECK_ENABLE_TOP_SCANNING, m_bEnableTopScanning);
	DDX_Check(pDX, IDC_CHECK_ENABLE_THR_SCANNING, m_bEnableTHRScanning);
	
	DDX_Check(pDX, IDC_ENABLE_DATUM_OFFSET, m_bEnableDatumOffset);
	DDX_Check(pDX, IDC_CHECK_ENABLE_BARCODE_WTO_CYLINDER, m_bEnableBarcodeReaderWithCylinder);
	
}


BEGIN_MESSAGE_MAP(SequenceOptions, CDialogEx)
	ON_BN_CLICKED(IDOK, &SequenceOptions::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_DRY_RUN, &SequenceOptions::OnBnClickedCheckDryRun)
	ON_BN_CLICKED(IDC_CHECK_WET_RUN, &SequenceOptions::OnBnClickedCheckWetRun)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// SequenceOptions message handlers


void SequenceOptions::OnBnClickedOk()
{
	UpdateData(TRUE);
	SaveData();
	CDialogEx::OnOK();
}

void SequenceOptions::LoadData(void)
{
	m_bEnableBarcodeReader = pMotionParms->isBarcodeEnabled();
	m_bEnableBarcodeReaderWithCylinder = pMotionParms->isBarcodeWithCylinderEnabled();
	
	
	m_bEnableIonizer = pMotionParms->isIonizerEnabled();
	m_bEnableTHRScanning = pMotionParms->isTHRScanningEnabled();
	m_bEnableTopScanning = pMotionParms->isTopScanningEnabled();
	
	
	m_bEnableDatumOffset = theApp.m_bEnableTopDatumPosToScanTHR;
	if (pSequence != NULL) {
		m_bDryRun = pSequence->bDryRun;
		m_bWetRun = pSequence->bWetRun;
	}


}

void SequenceOptions::SaveData() const
{
	pMotionParms->enableBarcodeStatus(m_bEnableBarcodeReader);
	pMotionParms->enableBarcodeStatusWithCylinder(m_bEnableBarcodeReaderWithCylinder);
	
	pMotionParms->enableIonizerStatus(m_bEnableIonizer);
	pMotionParms->enableTHRScanning(m_bEnableTHRScanning);
	pMotionParms->enableTopScanning(m_bEnableTopScanning);
	
	
	theApp.m_bEnableTopDatumPosToScanTHR= m_bEnableDatumOffset;

	if (pSequence != NULL) {
		pSequence->bDryRun = m_bDryRun;
		pSequence->bWetRun = m_bWetRun;
	}
//	pMotionParms->enableDryRun(m_bDryRun);

	pMotionParms->SaveScanningOptions();
	PutRegistryEntry(WRITE_REGPATH + "Preferences", "Use Top Station Datum Location to Scan THR", theApp.m_bEnableTopDatumPosToScanTHR);

}

void SequenceOptions::OnBnClickedCheckDryRun()
{
	UpdateData(TRUE);
	pSequence->bDryRun = m_bDryRun;
	if (m_bDryRun)
		pSequence->bWetRun = m_bWetRun = !m_bDryRun;
	UpdateData(FALSE);
}


void SequenceOptions::OnBnClickedCheckWetRun()
{
	UpdateData(TRUE);
	pSequence->bWetRun = m_bWetRun;
	if(m_bWetRun)
		pSequence->bDryRun = m_bDryRun = !m_bWetRun;
	UpdateData(FALSE);
}

void SequenceOptions::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	LoadData();
	UpdateData(FALSE);
}

