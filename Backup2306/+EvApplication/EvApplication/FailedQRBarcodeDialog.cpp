// FailedQRBarcodeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FailedQRBarcodeDialog.h"
#include "afxdialogex.h"
#include "Application.h"
#include "ManualQRBarcodeDialog.h"


// CFailedQRBarcodeDialog dialog

IMPLEMENT_DYNAMIC(CFailedQRBarcodeDialog, CDialog)

CFailedQRBarcodeDialog::CFailedQRBarcodeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FAILED_QRBARCODE_OPTIONS, pParent)
	, actionSelection(-1)
{

}

CFailedQRBarcodeDialog::~CFailedQRBarcodeDialog()
{
}

void CFailedQRBarcodeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFailedQRBarcodeDialog, CDialog)
	
	ON_BN_CLICKED(IDC_INPUT_BARCODE_MANUALLY, &CFailedQRBarcodeDialog::OnBnClickedInputBarcodeManually)
	ON_BN_CLICKED(IDC_ABORT_UNLOAD_STRIP_MANUALLY, &CFailedQRBarcodeDialog::OnBnClickedAbortUnloadStripManually)
	ON_BN_CLICKED(IDC_RETRY_BARCODE, &CFailedQRBarcodeDialog::OnBnClickedRetryBarcode)
END_MESSAGE_MAP()


// FailedQRBarcodeDialog message handlers


void CFailedQRBarcodeDialog::OnBnClickedInputBarcodeManually()
{
	

	////Let user to input the QR manually

	CManualQRBarcodeDialog manualQRBarcodeDialog;
	if (manualQRBarcodeDialog.DoModal() == IDOK) {
		/*theApp.pSubstrateInspectionResultsWnd->SetBarcodeID(manualQRBarcodeDialog.getQRBarcodeID());*/
		theApp.strInspectDeviceBarcodeID = manualQRBarcodeDialog.getQRBarcodeID();
		//CString strDeviveBarcodeID;
		//strDeviveBarcodeID = manualQRBarcodeDialog.getQRBarcodeID();
		////theApp.pSubstrateInspectionResultsWnd->compareStripId(strDeviveBarcodeID.GetString());		//Girish

		////theApp.pSubstrateInspectionResultsWnd->SetBarcodeID(strDeviveBarcodeID);
		//theApp.strInspectDeviceBarcodeID = manualQRBarcodeDialog.getQRBarcodeID();

		actionSelection = CONTINUE_SEQUENCE;

		CDialog::OnOK();
	}
	else {
		actionSelection = ABORT_INPUT_BARCODE_SEQUENCE;
	}
}


void CFailedQRBarcodeDialog::OnBnClickedAbortUnloadStripManually()
{
	//// Run unload strip sequence
	//theApp.OnSequenceRununloadstripsequence();

	//BOOL substratePresence = FALSE;

	//int error = theApp.ioControl.GripperClampOpen();
	//if (error) {
	//	AfxMessageBox("Unable to open gripper clamp. Please check io status");
	//	actionSelection = STOP_MACHINE_SEQUENCE;
	//	return;
	//}

	//do {

	//	int error = theApp.ioControl.GetSubstratePresence(substratePresence);
	//	if (error) {
	//		AfxMessageBox("Unable to retrieve substrate presence status. Please check io status");
	//		actionSelection = STOP_MACHINE_SEQUENCE;
	//		return;
	//	}

	//	if (substratePresence) {
	//		AfxMessageBox("Please unload strip on the track before continue and try again.", MB_OK | MB_ICONERROR);
	//	}

	//} while (substratePresence);

	//actionSelection = AFTER_UNLOAD_STRIP_SEQUENCE;

	//CDialog::OnOK();
}

int CFailedQRBarcodeDialog::getActionSelection()
{
	return actionSelection;
}


void CFailedQRBarcodeDialog::OnBnClickedRetryBarcode()
{
	BOOL bAllowScan = TRUE;
	CString strTeachDeviceBarcodeID;
	CString strInspectDeviceBarcodeID;

	int nError = theApp.QRBarcodeReader.GetDeviceScanningStatus(strTeachDeviceBarcodeID, bAllowScan);

	//	int nError = theApp.QRBarcodeReader.TriggerInputON();

	//Get Scanned Barcode ID
	//If scanned barcode is not valid, we need to create a different name, as a file/folder with that name cannot be created
	if (!nError) {
		nError = theApp.QRBarcodeReader.GetDeviceInfo(strInspectDeviceBarcodeID);
		

		/*theApp.pSubstrateInspectionResultsWnd->SetBarcodeID(strInspectDeviceBarcodeID);*/
		theApp.strInspectDeviceBarcodeID = strInspectDeviceBarcodeID;

		actionSelection = CONTINUE_SEQUENCE;

		CDialog::OnOK();
	}
	else
	{

	}
}

