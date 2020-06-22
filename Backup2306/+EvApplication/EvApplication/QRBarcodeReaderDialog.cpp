// QRBarcodeReaderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "QRBarcodeReaderDialog.h"
#include "DebugLog.h"

// CQRBarcodeReaderDialog dialog

IMPLEMENT_DYNAMIC(CQRBarcodeReaderDialog, CDialog)

CQRBarcodeReaderDialog::CQRBarcodeReaderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQRBarcodeReaderDialog::IDD, pParent)
	, m_strMessage(_T(""))
{
}

CQRBarcodeReaderDialog::~CQRBarcodeReaderDialog()
{
}

void CQRBarcodeReaderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MESSAGE_TO_SEND, m_strMessage);
}


BEGIN_MESSAGE_MAP(CQRBarcodeReaderDialog, CDialog)
	ON_BN_CLICKED(IDC_SEND, &CQRBarcodeReaderDialog::OnBnClickedSend)
	ON_BN_CLICKED(IDC_BUTTON_ALLON, &CQRBarcodeReaderDialog::OnBnClickedButtonAllon)
	ON_BN_CLICKED(IDC_BUTTON_ALLOFF, &CQRBarcodeReaderDialog::OnBnClickedButtonAlloff)
	ON_BN_CLICKED(IDC_BUTTON_QR_RESET, &CQRBarcodeReaderDialog::OnBnClickedButtonQrReset)
	ON_BN_CLICKED(IDC_BUTTON_QR_CLEAR, &CQRBarcodeReaderDialog::OnBnClickedButtonQrClear)
	ON_BN_CLICKED(IDC_BUTTON_LASERON, &CQRBarcodeReaderDialog::OnBnClickedButtonLaseron)
	ON_BN_CLICKED(IDC_BUTTON_LASEROFF, &CQRBarcodeReaderDialog::OnBnClickedButtonLaseroff)
	ON_BN_CLICKED(IDC_BUTTON_TRIGGERON, &CQRBarcodeReaderDialog::OnBnClickedButtonTriggeron)
	ON_BN_CLICKED(IDC_BUTTON_TRIGGEROFF, &CQRBarcodeReaderDialog::OnBnClickedButtonTriggeroff)
END_MESSAGE_MAP()


// CQRBarcodeReaderDialog message handlers


void CQRBarcodeReaderDialog::OnBnClickedSend()
{
	UpdateData(TRUE);

	theApp.QRBarcodeReader.SendMessageToServer(m_strMessage);
}


void CQRBarcodeReaderDialog::OnBnClickedButtonAllon()
{
	// TODO: Add your control notification handler code here
	theApp.QRBarcodeReader.AllOutputsON();
}


void CQRBarcodeReaderDialog::OnBnClickedButtonAlloff()
{
	// TODO: Add your control notification handler code here
	theApp.QRBarcodeReader.AllOutputsOFF();
}


void CQRBarcodeReaderDialog::OnBnClickedButtonQrReset()
{
	// TODO: Add your control notification handler code here
	theApp.QRBarcodeReader.Reset();
}


void CQRBarcodeReaderDialog::OnBnClickedButtonQrClear()
{
	// TODO: Add your control notification handler code here
	theApp.QRBarcodeReader.ClearBuffer();
}


void CQRBarcodeReaderDialog::OnBnClickedButtonLaseron()
{
	// TODO: Add your control notification handler code here
	theApp.QRBarcodeReader.LaserON();
}


void CQRBarcodeReaderDialog::OnBnClickedButtonLaseroff()
{
	// TODO: Add your control notification handler code here
	theApp.QRBarcodeReader.LaserOFF();
}


void CQRBarcodeReaderDialog::OnBnClickedButtonTriggeron()
{
	// TODO: Add your control notification handler code here
	//system("C:\\Program Files(x86)\\KEYENCE Applications\\AutoID\\MultiMonitor_EN\\MultiMonitor.exe");
	//ShellExecute(NULL,L"Open",L"\"C:\\Program Files(x86)\\KEYENCE Applications\\AutoID\\MultiMonitor_EN\\MultiMonitor.exe\"",NULL,NULL,SW_SHOWDEFAULT);
	//ShellExecute(NULL, "open", "C:\\Program Files(x86)\\KEYENCE Applications\\AutoID\\MultiMonitor_EN\\MultiMonitor.exe", NULL, NULL, SW_SHOWDEFAULT);
	ShellExecute(NULL, "open", theApp.strMultiMonitorPath.GetString(), NULL, NULL, SW_SHOWDEFAULT);
	CString strDeviceID;
	int nError = 0;
	nError = theApp.QRBarcodeReader.TriggerInputON();
	
	if(!nError) {
		theApp.QRBarcodeReader.GetDeviceInfo(strDeviceID);
		OutputDebugLogTo(4,TRUE,"Final ID: %s",strDeviceID);
	}
	else
		OutputDebugLogTo(4,TRUE,"Error ID: %d",nError);

	
}


void CQRBarcodeReaderDialog::OnBnClickedButtonTriggeroff()
{
	// TODO: Add your control notification handler code here
	theApp.QRBarcodeReader.TriggerInputOFF();
}
