#pragma once
#include "resource.h"
#include "TabPageSSL.h"
#include "afxwin.h"

// CMeasurementRate dialog

class CMeasurementRate : public CTabPageSSL
{
	//DECLARE_DYNAMIC(CMeasurementRate)

public:
	CMeasurementRate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMeasurementRate();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MEASUREMENT_RATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnCbnSelchangeComboMeasuringRate();
	CComboBox m_comboMeasuringRate;
	void InitializeMeasuringRate();
	void GetValues();

	afx_msg void OnBnClickedOk2();
};
