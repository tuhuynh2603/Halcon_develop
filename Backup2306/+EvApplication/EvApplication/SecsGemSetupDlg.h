#pragma once

#include "Application.h"
#include "resource.h"
#include "afxwin.h"

// SecsGemSetupDlg dialog

class SecsGemSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(SecsGemSetupDlg)

public:
	SecsGemSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SecsGemSetupDlg();
	CBrush m_brush;

	void initializeSetup();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SECSGEM_SETUP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_bEnableMultiOpenTransaction;
	int m_nMaxMultiOpenTransactions;
	int m_nEquipDeviceID;
	CString m_strEquipLocalAddress;
	CString m_strEquipRemoteAddress;
	int m_nEquipLocalPortID;
	int m_nEquipRemotePortID;
	int m_nEquipConnectMode;
	virtual BOOL OnInitDialog();
	//afx_msg void OnCbnSelchangeComboEquipMaxOpenTransaction();
	CComboBox m_cmbMaxOpenTransactions;
	CComboBox m_cmbConnectionMode;
	int m_nHSMST3Timeout;
	int m_nHSMST5Timeout;
	int m_nHSMST6Timeout;
	int m_nHSMST7Timeout;
	int m_nHSMST8Timeout;
	afx_msg void OnSelchangeComboEquipConnectMode();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedOk();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
