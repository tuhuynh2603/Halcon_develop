#pragma once
#include "afxwin.h"

class MotionParameters;
// ScanningTeachDialog dialog

class ScanningTeachDialog : public CDialog
{
	DECLARE_DYNAMIC(ScanningTeachDialog)

public:
	ScanningTeachDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ScanningTeachDialog();
//	CApplication* m_pApp;

	MotionParameters * pMotionParms;
	MotionParameters * pMotionParmsInitial;

	CComboBox m_comboZStartPosFOVSetting;
	CComboBox m_comboZStartPosFOVSettingXY;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCANNING_TEACH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetStartPos();
	afx_msg void OnBnClickedMoveToStartPos();

	void SetMotionParameters(MotionParameters * pMotionParmsIn, MotionParameters * pMotionParmsInitialIn);
	afx_msg void OnBnClickedSetZreadyPos();
	afx_msg void OnBnClickedMoveToZreadyPos();
private:
	double m_dDistanceZSensorToTop;
	double m_dDistanceTopToTHR;
public:
	afx_msg void OnEnChangeEditZSensorToTopDistance();
	afx_msg void OnEnChangeEditTopToThrDistance();
private:
	double m_dMotorXScanOffset;
public:
	afx_msg void OnEnChangeEditXTriggerOffset();
	afx_msg void OnBnClickedButtonMoveToZSensorPos();
	afx_msg void OnBnClickedButtonReadyPosSet();
	afx_msg void OnBnClickedButtonReadyPosMov();
	afx_msg void OnBnClickedButtonNestPosMov();
	afx_msg void OnBnClickedButtonNestPosSet();
	double m_dForwardTriggerOffset;
	double m_dInverseTriggerOffset;
	afx_msg void OnEnChangeEditXForwardOffset();
	afx_msg void OnEnChangeEditXReverseOffset();
	

	afx_msg void OnBnClickedSetTopStartPos();
	afx_msg void OnBnClickedMoveToTopStartPos();
	afx_msg void OnBnClickedSetTopZreadyPos();
	afx_msg void OnBnClickedMoveToTopZreadyPos();
	afx_msg void OnBnClickedButtonApply();
};
