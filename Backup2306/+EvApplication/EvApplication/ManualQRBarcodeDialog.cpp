// ManualQRBarcodeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ManualQRBarcodeDialog.h"
#include "afxdialogex.h"


// CManualQRBarcodeDialog dialog

IMPLEMENT_DYNAMIC(CManualQRBarcodeDialog, CDialog)

CManualQRBarcodeDialog::CManualQRBarcodeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MANUAL_INPUT_QRBARCODE_DIALOG, pParent)
	, qrBarcodeID(_T(""))
{

}

CManualQRBarcodeDialog::~CManualQRBarcodeDialog()
{
}

void CManualQRBarcodeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_QRBARCODE_ID, qrBarcodeID);
}


BEGIN_MESSAGE_MAP(CManualQRBarcodeDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CManualQRBarcodeDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CManualQRBarcodeDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// ManualQRBarcodeDialog message handlers


void CManualQRBarcodeDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (qrBarcodeID.IsEmpty()) {
		AfxMessageBox("QR barcode cannot be empty. Please input a valid barcode and try again", MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_QRBARCODE_ID)->SetFocus();
	}
	else {
		CDialog::OnOK();
	}
}


void CManualQRBarcodeDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

CString CManualQRBarcodeDialog::getQRBarcodeID()
{
	return qrBarcodeID;
}
