#pragma once


// CTeachZSensorPosition dialog

class CTeachZSensorPosition : public CDialogEx
{
	DECLARE_DYNAMIC(CTeachZSensorPosition)

public:
	CTeachZSensorPosition(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTeachZSensorPosition();

	
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
	afx_msg void OnBnClickedMovePosition();
private:
	int			  _id;
	int			  _dieId;
	int			  _motorId;
	bool		  _motorMoving;
	bool		  _continuousMove;
	BOOL		  _enableRelativeMove;
	double		  _relativeMoveMm;
	CComboBox	  _motorIdCombobox;
	CComboBox	  _dieIdCombobox;
	CBitmapButton _negativeMovementButton;
	CBitmapButton _positiveMovementButton;
	int			  _nStartingMotorPosition;
	int           _nEndMotorPosition;

	void Move(int axisId, int direction);
	void RelativeMove(int axisId, int direction);
	void RedrawPositions();

	double dZSensorTeachValue;

public:
	afx_msg void OnSelchangeMotorIdCombo();
	afx_msg void OnSelchangeDieIdCombo();
	afx_msg void OnBnClickedEnableRelativeMove();
	afx_msg void OnBnClickedNegMovement();
	afx_msg void OnBnClickedPosMovement();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit ctlrSensorData;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
