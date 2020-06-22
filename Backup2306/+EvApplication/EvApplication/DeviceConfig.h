#pragma once
#include "resource.h"
#include "Application.h"

#define MAX_DIRECTION		2

// CDeviceConfig dialog

class CDeviceConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceConfig)

public:
	CDeviceConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDeviceConfig();

	CApplication* pApp;
	MotionParameters * pMotionParms;

	int m_nNoOfBlock[MAX_DIRECTION];
	double m_dBlockPitch[MAX_DIRECTION];
	int m_nDeviceNoPerBlock[MAX_DIRECTION];
	int m_nDeviceNoPerBlockTOP[MAX_DIRECTION];
	double m_dDevicePitch[MAX_DIRECTION];
	int m_nDeviceOffset[MAX_DIRECTION];
	double m_dInitialOffset;

	//void DeviceConfigFile(BOOL bSave);

// Dialog Data
	enum { IDD = IDD_DEVICE_CONFIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void LoadData();
	void SaveData();
	afx_msg void OnBnClickedDatumOffset();
};
