// BarcodeReaderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "BarcodeReaderDialog.h"
#include "afxdialogex.h"



// CBarcodeReaderDialog dialog

IMPLEMENT_DYNAMIC(CBarcodeReaderDialog, CDialog)

CBarcodeReaderDialog::CBarcodeReaderDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBarcodeReaderDialog::IDD, pParent)
	, m_strMessage(_T(""))
{
}

CBarcodeReaderDialog::~CBarcodeReaderDialog()
{
}

void CBarcodeReaderDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MESSAGE_TO_BARCODEREADER, m_strMessage);
}


BEGIN_MESSAGE_MAP(CBarcodeReaderDialog, CDialog)
	ON_BN_CLICKED(IDC_BARCODE_SEND, &CBarcodeReaderDialog::OnBnClickedBarcodeSend)
	ON_BN_CLICKED(IDOK, &CBarcodeReaderDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BARCODE_LASERON, &CBarcodeReaderDialog::OnBnClickedButtonBarcodeLaseron)
	ON_BN_CLICKED(IDC_BUTTON_BARCODE_LASEROFF, &CBarcodeReaderDialog::OnBnClickedButtonBarcodeLaseroff)
	ON_BN_CLICKED(IDC_BUTTON_BARCODE_BUZZERON, &CBarcodeReaderDialog::OnBnClickedButtonBarcodeBuzzeron)
	ON_EN_CHANGE(IDC_MESSAGE_TO_BARCODEREADER, &CBarcodeReaderDialog::OnEnChangeMessageToBarcodereader)
	ON_BN_CLICKED(IDC_SAVE, &CBarcodeReaderDialog::OnBnClickedSave)
END_MESSAGE_MAP()


// CBarcodeReaderDialog message handlers


void CBarcodeReaderDialog::OnBnClickedBarcodeSend()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	theApp.BarcodeReader.SendMessageToServer(m_strMessage);
}


void CBarcodeReaderDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialog::OnOK();
}


void CBarcodeReaderDialog::OnBnClickedButtonBarcodeLaseron()
{
	// TODO: Add your control notification handler code here
	theApp.BarcodeReader.LaserON();
}


void CBarcodeReaderDialog::OnBnClickedButtonBarcodeLaseroff()
{
	// TODO: Add your control notification handler code here
	theApp.BarcodeReader.LaserOFF();
}


void CBarcodeReaderDialog::OnBnClickedButtonBarcodeBuzzeron()
{
	// TODO: Add your control notification handler code here
	theApp.BarcodeReader.BuzzerON();
}


BOOL CBarcodeReaderDialog::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}


void CBarcodeReaderDialog::OnEnChangeMessageToBarcodereader()
{
	UpdateData();
}


void CBarcodeReaderDialog::OnBnClickedSave()
{
	int nBarcodeLength = m_strMessage.GetLength();

	CString strTemp;
	if(nBarcodeLength > 0) {
		strTemp.Format("Is this barcode: %s correct? \nPress \"Yes\" to continue and \"No\" to Rescan Barcode", m_strMessage);
		if(AfxMessageBox(strTemp, MB_YESNO | MB_ICONQUESTION)==IDYES) {
			CDialog::OnOK();
		}
	}
	else {
		AfxMessageBox("Please enter a valid barcode to proceed!", MB_OK | MB_ICONERROR);
	}
}
