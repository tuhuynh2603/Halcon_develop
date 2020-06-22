#pragma once
#include "resource.h"
#include "afxwin.h"

// CDistanceCalibration dialog

enum
{
	SENSOR_TO_TOP,
	TOP_TO_THR,
};

class CDistanceCalibration : public CDialogEx
{
	DECLARE_DYNAMIC(CDistanceCalibration)

public:
	CDistanceCalibration(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDistanceCalibration();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIB_DISTANCE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	int			  _id;
	int			  _motorId;
	int			  _cameraId;
	int			  _calibrationType;
	bool		  _cameraLive;
	bool		  _motorMoving;
	bool		  _continuousMove;
	BOOL		  _enableRelativeMove;
	double		  _relativeMoveMm;
	CFont		  _fontDescription;
	CString		  _descriptions;
	CComboBox	  _motorIdCombobox;
	CBitmapButton _negativeMovementButton;
	CBitmapButton _positiveMovementButton;
	int			  _nStartingMotorPosition;
	int           _nEndMotorPosition;

	void updateDescriptions();
	void Move(int axisId, int direction);
	void RelativeMove(int axisId, int direction);
	void RedrawPositions();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedNegMovement();
	afx_msg void OnBnClickedPosMovement();
	afx_msg void OnBnClickedButtonCameraGrab();
	afx_msg void OnBnClickedButtonCameraLive();
	afx_msg void OnBnClickedRadioTopCamera();
	afx_msg void OnBnClickedRadioThrCamera();
	afx_msg void OnBnClickedEnableRelativeMove();
	afx_msg void OnSelchangeMotorIdCombo();

	CEdit ctrlMotorPos;
	CEdit ctrlTSensorPos;
	afx_msg void OnBnClickedRadioTopTraingularSensorDistance();
	afx_msg void OnBnClickedRadioTopThrDistance();
	afx_msg void OnBnClickedButtonMeasureDistance();
	afx_msg void OnBnClickedButtonSaveData();
	afx_msg void OnBnClickedButtonGrid();
};
