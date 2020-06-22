#pragma once
#include "Resource.h"
#include "TabPageSSL.h"
#include "TabCtrlSSL.h"
#include "TriggerModeDLg.h"
#include "MeasurementMode.h"
#include "MeasurementRate.h"
#include "afxlinkctrl.h"
#include "afxwin.h"
//#include "TabControlGe3f.h"


// CTriangularSensroDlg dialog
class CApplication;
class CTriangularSensroDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTriangularSensroDlg)

public:
	CTriangularSensroDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTriangularSensroDlg();
	int m_nActiveTab;
	CDialog *m_pPage[4];
	CTabCtrl* pTabCtrl;
	//CTabControlGe3f	m_TabCtrl;
	CFont m_fontList;
	CApplication* m_pApp;
	CTriggerModeDLg m_tabTriggerModeDlg;
	CMeasurementMode m_tabMeasurementModeDlg;
	CMeasurementRate m_tabMeasurementRateDlg;
	CBrush* m_pBrush;
	CBrush m_pBrushTemp[2];
	BOOL m_bConnected;
	BOOL m_bPause;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRIANGULAR_SENSRO_DLG };
#endif

private:
	double m_dZSensorTeachValue;
	volatile double m_dCurrentZValue;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void Initailize();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	CString m_strReadSensorData;
	afx_msg void OnBnClickedCancel();
	CTabCtrlSSL m_tabCtrl;
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_strIpAddress;
	CString m_strMasterValueSet;
	afx_msg void OnBnClickedMfclink1();
	CMFCLinkCtrl m_btnMfcLinkControl;
	afx_msg void OnBnClickedButtonConnectSensor();
	afx_msg void OnBnClickedButtonDisconnectSensor();
	afx_msg void OnBnClickedButtonSetMasterValue();
	afx_msg void OnBnClickedButtonResetMasterValue();
//	afx_msg void OnBnClickedPasuePlay();
//	CMFCButton m_butPlayPause;
	afx_msg void OnBnClickedStopPlay();
	CStatic m_ctrlSensorDistance;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonSetZTeachPos();

	CComboBox m_comboDieIDTeach;

};