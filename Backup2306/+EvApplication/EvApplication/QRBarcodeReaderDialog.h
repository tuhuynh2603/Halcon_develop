#pragma once
#include "resource.h"

// CQRBarcodeReaderDialog dialog

class CQRBarcodeReaderDialog : public CDialog
{

	DECLARE_DYNAMIC(CQRBarcodeReaderDialog)

public:
	CQRBarcodeReaderDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQRBarcodeReaderDialog();

// Dialog Data
	enum { IDD = IDD_QR_BARCODE_READER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:

	CString m_strMessage;
public:
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedButtonAllon();
	afx_msg void OnBnClickedButtonAlloff();
	afx_msg void OnBnClickedButtonQrReset();
	afx_msg void OnBnClickedButtonQrClear();
	afx_msg void OnBnClickedButtonLaseron();
	afx_msg void OnBnClickedButtonLaseroff();
	afx_msg void OnBnClickedButtonTriggeron();
	afx_msg void OnBnClickedButtonTriggeroff();
};
