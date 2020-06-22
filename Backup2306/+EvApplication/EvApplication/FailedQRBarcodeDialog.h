#pragma once
#include "resource.h"

// CFailedQRBarcodeDialog dialog

class CFailedQRBarcodeDialog : public CDialog
{
	DECLARE_DYNAMIC(CFailedQRBarcodeDialog)

public:
	CFailedQRBarcodeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFailedQRBarcodeDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FAILED_QRBARCODE_OPTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int getActionSelection();
	

	afx_msg void OnBnClickedInputBarcodeManually();
	afx_msg void OnBnClickedAbortUnloadStripManually();

private:
	int actionSelection;
public:
	afx_msg void OnBnClickedRetryBarcode();
};
