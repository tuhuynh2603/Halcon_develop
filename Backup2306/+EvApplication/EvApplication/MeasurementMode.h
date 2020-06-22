#pragma once
#include "resource.h"
#include "TabPageSSL.h"
#include "afxwin.h"

// CMeasurementMode dialog

class CMeasurementMode : public CTabPageSSL
{
	//DECLARE_DYNAMIC(CMeasurementMode)

public:
	CMeasurementMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMeasurementMode();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MEASUREMENT_MODE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboMeasureArrangements;
	CComboBox m_comboPeakToBeMeasured;
	CComboBox m_comboLaserPower;
	afx_msg void OnBnClickedOk();
	void InitializeComoboBox();
	void GetValues();
	void SetValues();
};
