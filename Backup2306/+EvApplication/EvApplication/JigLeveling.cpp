// JigLeveling.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "JigLeveling.h"
#include "afxdialogex.h"
#include "CalibrationSettings.h"

static const UINT_PTR SENSOR_TIMER = 54;
// CJigLeveling dialog

IMPLEMENT_DYNAMIC(CJigLeveling, CDialogEx)

CJigLeveling::CJigLeveling(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_JIG_LEVELING, pParent)
	, m_strOldTriReading(_T(""))
	, _motorId(0)
	, _enableRelativeMove(TRUE)		// enable relative motion by default
	, _relativeMoveMm(0)
	, _continuousMove(false)
	, _id(0)
	, _motorMoving(false)
	, _nPosition(0)
{
	m_pApp = (CApplication*)AfxGetApp();
	pMotionParmsInitial = &theApp.motionParameters;
	for (int i = 0; i < 4; i++)
		_dDataPosition[i] = 0;
}

CJigLeveling::~CJigLeveling()
{
}

void CJigLeveling::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RELATIVE_MOVE2, _relativeMoveMm);
	DDX_Text(pDX, IDC_EDIT_POSITION_01, _dDataPosition[0]);
	DDX_Text(pDX, IDC_EDIT_POSITION_2, _dDataPosition[1]);
	DDX_Text(pDX, IDC_EDIT_POSITION_3, _dDataPosition[2]);
	DDX_Text(pDX, IDC_EDIT_POSITION_4, _dDataPosition[3]);
	DDX_Control(pDX, IDC_MOTOR_ID_COMBO, _motorIdCombobox);
	DDX_CBIndex(pDX, IDC_MOTOR_ID_COMBO, _motorId);
	DDX_Check(pDX, IDC_ENABLE_RELATIVE_MOVE, _enableRelativeMove);
	DDX_Control(pDX, IDC_NEG_MOVEMENT, _negativeMovementButton);
	DDX_Control(pDX, IDC_POS_MOVEMENT2, _positiveMovementButton);
	DDX_Control(pDX, IDC_CURR_READING, ctlrSensorData);
	DDX_Radio(pDX, IDC_RADIO_POSITION1, _nPosition);
}


BEGIN_MESSAGE_MAP(CJigLeveling, CDialogEx)
	ON_BN_CLICKED(IDC_SET_POSITION, &CJigLeveling::OnBnClickedSetPosition)
	ON_BN_CLICKED(IDOK, &CJigLeveling::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SAVE_VALUE, &CJigLeveling::OnBnClickedSaveValue)
	ON_BN_CLICKED(IDC_RADIO_POSITION1, &CJigLeveling::OnBnClickedRadioPosition)
	ON_BN_CLICKED(IDC_RADIO_POSITION2, &CJigLeveling::OnBnClickedRadioPosition)
	ON_BN_CLICKED(IDC_RADIO_POSITION3, &CJigLeveling::OnBnClickedRadioPosition)
	ON_BN_CLICKED(IDC_RADIO_POSITION4, &CJigLeveling::OnBnClickedRadioPosition)
	ON_BN_CLICKED(IDC_MOVE_POSITION, &CJigLeveling::OnBnClickedMovePosition)
	ON_CBN_SELCHANGE(IDC_MOTOR_ID_COMBO, &CJigLeveling::OnSelchangeMotorIdCombo)
	ON_BN_CLICKED(IDC_ENABLE_RELATIVE_MOVE, &CJigLeveling::OnBnClickedEnableRelativeMove)
	ON_BN_CLICKED(IDC_NEG_MOVEMENT, &CJigLeveling::OnBnClickedNegMovement)
	ON_BN_CLICKED(IDC_POS_MOVEMENT2, &CJigLeveling::OnBnClickedPosMovement)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CJigLeveling::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_JIG_LEVEL, &CJigLeveling::OnBnClickedButtonSaveJigLevel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CJigLeveling message handlers

BOOL CJigLeveling::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	theApp.motionParameters.LoadJigLevelPositions();
	_nPosition = 0;
	m_strOldTriReading.Format("%0.4f",0);

	_positiveMovementButton.LoadBitmaps(IDB_ARROWRIGHT, IDB_ARROWRIGHT_CLICKED);
	_negativeMovementButton.LoadBitmaps(IDB_ARROWLEFT, IDB_ARROWLEFT_CLICKED);

	//Initialize combo box items
	_motorIdCombobox.ResetContent();

	std::vector< int > axisId;
	axisId.push_back(X_SERVO_MOTOR);
	axisId.push_back(Y_SERVO_MOTOR);
	axisId.push_back(ST1_Z_SERVO_MOTOR);

	CString name;
	for (size_t id = 0; id < axisId.size(); ++id) {
		theApp.motionParameters.GetMotorName(axisId[id], name);
		_motorIdCombobox.AddString(name);
	}

	//Set default selection as first item
	_motorIdCombobox.SetCurSel(0);
	OnSelchangeMotorIdCombo();
	theApp.motionParameters.getJigLevelZSensorvalues(_dDataPosition);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CJigLeveling::OnBnClickedSetPosition()
{
	if (_nPosition >= 0)
		theApp.motionParameters.SetJigLevelPositions(_nPosition);
	else
		AfxMessageBox("Please Select Position To Set Motor Position");
}


void CJigLeveling::OnBnClickedOk()
{
	KillTimer(SENSOR_TIMER);

	if (!theApp.m_pCalibrationDlg->IsWindowVisible())
		theApp.m_pCalibrationDlg->ShowWindow(TRUE);

	ShowWindow(SW_HIDE);

	CDialogEx::OnOK();
}

void CJigLeveling::MoveMotor(int Position)
{
	pMotionParmsInitial->MoveJigLevelPos(Position);
	UpdateData(FALSE);
}

void CJigLeveling::OnBnClickedSaveValue()
{
	UpdateData(TRUE);

	if (_nPosition >= 0)
		_dDataPosition[_nPosition] = pMotionParmsInitial->GetZSensorCurrentDistance();
	else
		AfxMessageBox("Please Select Position to Save Data");

	UpdateData(FALSE);
	
}


void CJigLeveling::OnBnClickedRadioPosition()
{
	int nId = GetCheckedRadioButton(IDC_RADIO_POSITION1, IDC_RADIO_POSITION4);	// first,last ID
	_nPosition = nId - IDC_RADIO_POSITION1;
}


void CJigLeveling::OnBnClickedMovePosition()
{
	if (_nPosition >= 0)
		MoveMotor(_nPosition);
	else
		AfxMessageBox("Please Select Position to Move Motor");
}

void CJigLeveling::Move(int axisId, int direction)
{
	if (!_continuousMove) {
		// 1. Stop current movement
		// 2. Move

		int nError = theApp.motionParameters.StopAxis(axisId);
		if (nError) return;

		Sleep(50);

		nError = theApp.motionParameters.ContinuousMove(axisId, direction == 1 ? TRUE : FALSE, theApp.motionParameters.getMovingSpeed(axisId), true);
		if (nError) return;

		_continuousMove = TRUE;
	}
	else {
		// 1. Stop current movement
		int nError = theApp.motionParameters.StopAxis(axisId);
		if (nError) return;

		Sleep(50);

		_continuousMove = FALSE;
	}
}

void CJigLeveling::RelativeMove(int axisId, int direction)
{
	// 1. Stop current movement
	// 2. Move

	double motorResolution = 0.0;

	theApp.motionParameters.GetMotorResolution(axisId, motorResolution);
	UpdateData(TRUE);

	int motorMoveRelativePos = Round(_relativeMoveMm * motorResolution);

	UpdateData(FALSE);

	int nError = theApp.motionParameters.StopAxis(axisId);
	if (nError)
		return;

	Sleep(50);

	_motorMoving = true;

	nError = theApp.motionParameters.RelativeMove(axisId, direction * motorMoveRelativePos, theApp.motionParameters.getMovingSpeed(axisId), true);
	if (nError) {
		_motorMoving = false;
		return;
	}

	_motorMoving = false;
}

void CJigLeveling::RedrawPositions()
{
	CString strSensorPos;
	int nDataAvail = 1;
	double dData = 0.0;
	vector <double> zValues;

	strSensorPos = "0";
		//		theApp.m_TriangularSensor.ProcessInstance(theApp.m_TrigangualrsensorInstance, dData, zValues, nDataAvail);
	theApp.m_TriangularSensor.ProcessInstancePoll(theApp.m_TrigangualrsensorInstance, dData);
	strSensorPos.Format("%3.06f", dData);
	
	ctlrSensorData.SetWindowText(strSensorPos);
}

void CJigLeveling::OnSelchangeMotorIdCombo()
{
	UpdateData(TRUE);

	switch (_motorId) {
	case 0:
		_id = X_SERVO_MOTOR;
		_positiveMovementButton.LoadBitmaps(IDB_ARROWRIGHT, IDB_ARROWRIGHT_CLICKED);
		_negativeMovementButton.LoadBitmaps(IDB_ARROWLEFT, IDB_ARROWLEFT_CLICKED);

		break;

	case 1:
		_id = Y_SERVO_MOTOR;
		_positiveMovementButton.LoadBitmaps(IDB_ARROWUP, IDB_ARROWUP_CLICKED);
		_negativeMovementButton.LoadBitmaps(IDB_ARROWDOWN, IDB_ARROWDOWN_CLICKED);

		break;

	case 2:
		_id = ST1_Z_SERVO_MOTOR;
		_positiveMovementButton.LoadBitmaps(IDB_ARROWUP, IDB_ARROWUP_CLICKED);
		_negativeMovementButton.LoadBitmaps(IDB_ARROWDOWN, IDB_ARROWDOWN_CLICKED);
		break;
	default:
		break;
	}

	CWnd::Invalidate();
}


void CJigLeveling::OnBnClickedEnableRelativeMove()
{
	_enableRelativeMove = !_enableRelativeMove;
}


void CJigLeveling::OnBnClickedNegMovement()
{
	if (_enableRelativeMove)
		RelativeMove(_id, -1);
	else
		Move(_id, -1);
}


void CJigLeveling::OnBnClickedPosMovement()
{
	if (_enableRelativeMove)
		RelativeMove(_id, 1);
	else
		Move(_id, 1);
}


void CJigLeveling::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == SENSOR_TIMER)
		RedrawPositions();

	CDialogEx::OnTimer(nIDEvent);
}


void CJigLeveling::OnBnClickedCancel()
{
	KillTimer(SENSOR_TIMER);

	if (!theApp.m_pCalibrationDlg->IsWindowVisible())
		theApp.m_pCalibrationDlg->ShowWindow(TRUE);

	CDialogEx::OnCancel();
}


void CJigLeveling::OnBnClickedButtonSaveJigLevel()
{
	pMotionParmsInitial->setJigLevelZSensorvalues(_dDataPosition);
	pMotionParmsInitial->SaveJigLevelPositions();
	pMotionParmsInitial->LoadJigLevelPositions();
}


void CJigLeveling::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	SetTimer(SENSOR_TIMER, 50, NULL);

	// TODO: Add your message handler code here
}
