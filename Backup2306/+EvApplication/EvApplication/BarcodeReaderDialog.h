#pragma once
#include "resource.h"

// CBarcodeReaderDialog dialog

class CBarcodeReaderDialog : public CDialog
{
	DECLARE_DYNAMIC(CBarcodeReaderDialog)

public:
	CBarcodeReaderDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBarcodeReaderDialog();

private:
	CString m_strMessage;

// Dialog Data
	enum { IDD = IDD_BARCODE_READER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedBarcodeSend();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonBarcodeLaseron();
	afx_msg void OnBnClickedButtonBarcodeLaseroff();
	afx_msg void OnBnClickedButtonBarcodeBuzzeron();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChangeMessageToBarcodereader();
	afx_msg void OnBnClickedSave();
};
