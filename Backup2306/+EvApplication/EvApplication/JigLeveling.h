#pragma once
#include "resource.h"
#include "afxwin.h"

// CJigLeveling dialog

class CJigLeveling : public CDialogEx
{
	DECLARE_DYNAMIC(CJigLeveling)

public:
	CJigLeveling(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJigLeveling();
	CApplication *m_pApp;
	MotionParameters * pMotionParmsInitial;
	void MoveMotor(int Position);

	int m_nPosition;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_JIG_LEVELING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetPosition();

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_strOldTriReading;
	afx_msg void OnBnClickedSaveValue();
	afx_msg void OnBnClickedRadioPosition();
	afx_msg void OnBnClickedMovePosition();
private:
	int			  _id;
	int			  _motorId;
	bool		  _motorMoving;
	bool		  _continuousMove;
	BOOL		  _enableRelativeMove;
	double		  _relativeMoveMm;
	double		  _dDataPosition[4];
	CComboBox	  _motorIdCombobox;
	CBitmapButton _negativeMovementButton;
	CBitmapButton _positiveMovementButton;
	int			  _nStartingMotorPosition;
	int           _nEndMotorPosition;

	void Move(int axisId, int direction);
	void RelativeMove(int axisId, int direction);
	void RedrawPositions();

public:
	afx_msg void OnSelchangeMotorIdCombo();
	afx_msg void OnBnClickedEnableRelativeMove();
	afx_msg void OnBnClickedNegMovement();
	afx_msg void OnBnClickedPosMovement();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit ctlrSensorData;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSaveJigLevel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	int _nPosition;
};
