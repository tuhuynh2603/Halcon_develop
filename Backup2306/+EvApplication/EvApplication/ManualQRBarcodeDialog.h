#pragma once
#include "resource.h"

// CManualQRBarcodeDialog dialog

class CManualQRBarcodeDialog : public CDialog
{
	DECLARE_DYNAMIC(CManualQRBarcodeDialog)

public:
	CManualQRBarcodeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManualQRBarcodeDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANUAL_INPUT_QRBARCODE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString getQRBarcodeID();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	CString qrBarcodeID;
};
