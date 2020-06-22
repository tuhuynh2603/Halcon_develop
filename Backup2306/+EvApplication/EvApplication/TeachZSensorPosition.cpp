// TeachZSensorPosition.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "TeachZSensorPosition.h"
#include "afxdialogex.h"
#include "CalibrationSettings.h"

static const UINT_PTR SENSOR_TIMER = 54;


// CTeachZSensorPosition dialog

IMPLEMENT_DYNAMIC(CTeachZSensorPosition, CDialogEx)

CTeachZSensorPosition::CTeachZSensorPosition(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TEACH_Z_SENSOR_POS, pParent)
	, m_strOldTriReading(_T(""))
	, _motorId(0)
	, _enableRelativeMove(TRUE)		// enable relative motion by default
	, _relativeMoveMm(0)
	, _continuousMove(false)
	, _id(0)
	, _dieId(0)
	, _motorMoving(false)
	, dZSensorTeachValue(0)
{

}

CTeachZSensorPosition::~CTeachZSensorPosition()
{
}

void CTeachZSensorPosition::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RELATIVE_MOVE2, _relativeMoveMm);
	DDX_Control(pDX, IDC_MOTOR_ID_COMBO, _motorIdCombobox);
	DDX_CBIndex(pDX, IDC_MOTOR_ID_COMBO, _motorId);
	DDX_Check(pDX, IDC_ENABLE_RELATIVE_MOVE, _enableRelativeMove);
	DDX_Control(pDX, IDC_NEG_MOVEMENT, _negativeMovementButton);
	DDX_Control(pDX, IDC_POS_MOVEMENT2, _positiveMovementButton);
	DDX_Control(pDX, IDC_CURR_READING, ctlrSensorData);
	DDX_Text(pDX, IDC_CURR_Z_TEACH, dZSensorTeachValue);
	DDX_Control(pDX, IDC_DIE_ID_COMBO, _dieIdCombobox);
}


BEGIN_MESSAGE_MAP(CTeachZSensorPosition, CDialogEx)
	ON_BN_CLICKED(IDC_SET_POSITION, &CTeachZSensorPosition::OnBnClickedSetPosition)
	ON_BN_CLICKED(IDOK, &CTeachZSensorPosition::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SAVE_VALUE, &CTeachZSensorPosition::OnBnClickedSaveValue)
	ON_BN_CLICKED(IDC_MOVE_POSITION, &CTeachZSensorPosition::OnBnClickedMovePosition)
	ON_CBN_SELCHANGE(IDC_MOTOR_ID_COMBO, &CTeachZSensorPosition::OnSelchangeMotorIdCombo)
	ON_CBN_SELCHANGE(IDC_DIE_ID_COMBO, &CTeachZSensorPosition::OnSelchangeDieIdCombo)
	ON_BN_CLICKED(IDC_ENABLE_RELATIVE_MOVE, &CTeachZSensorPosition::OnBnClickedEnableRelativeMove)
	ON_BN_CLICKED(IDC_NEG_MOVEMENT, &CTeachZSensorPosition::OnBnClickedNegMovement)
	ON_BN_CLICKED(IDC_POS_MOVEMENT2, &CTeachZSensorPosition::OnBnClickedPosMovement)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CTeachZSensorPosition::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CTeachZSensorPosition message handlers

BOOL CTeachZSensorPosition::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	theApp.motionParameters.LoadJigLevelPositions();

	m_strOldTriReading.Format("%0.4f", 0);

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

	_dieIdCombobox.ResetContent();
	int nDeviceNoPerSubstrateX, nDeviceNoPerSubstrateY, nDieNoPerDeviceX, nDieNoPerDeviceY;
	theApp.motionParameters.GetMappingParameters(nDeviceNoPerSubstrateX, nDeviceNoPerSubstrateY, nDieNoPerDeviceX, nDieNoPerDeviceY);

	int nTotalDie = nDeviceNoPerSubstrateX * nDeviceNoPerSubstrateY;
	for (int nDie = 0; nDie < nTotalDie; nDie++) {
		CString strDieID;
		strDieID.Format("Die %2d", nDie + 1);
		_dieIdCombobox.AddString(strDieID);
	}
	_dieIdCombobox.SetCurSel(0);

	dZSensorTeachValue = theApp.motionParameters.GetZSensorTeachValue(0);
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeachZSensorPosition::OnBnClickedSetPosition()
{
	int nCurrentSel = _dieIdCombobox.GetCurSel();
	CString strTemp;
	strTemp.Format("Do you want save current (X, Y) position as Z Teach Position for Die %d", nCurrentSel + 1);
	if (AfxMessageBox(strTemp, MB_YESNO) == IDYES) {
		theApp.motionParameters.SetZSensorTeachPos(nCurrentSel);
		theApp.motionParameters.SaveZTeachPosition();
	}
	
}


void CTeachZSensorPosition::OnBnClickedOk()
{
	KillTimer(SENSOR_TIMER);

	if (!theApp.m_pCalibrationDlg->IsWindowVisible())
		theApp.m_pCalibrationDlg->ShowWindow(TRUE);

	ShowWindow(SW_HIDE);

	CDialogEx::OnOK();
}

void CTeachZSensorPosition::OnBnClickedSaveValue()
{
	UpdateData(TRUE);
	CString str;
	CString strDieID;

	int nCurrentSel = _dieIdCombobox.GetCurSel();
	strDieID.Format("Die %d", nCurrentSel + 1);

	double dCurrentZValue = theApp.motionParameters.GetZSensorCurrentDistance();
	if (dCurrentZValue < 0) {
		AfxMessageBox("Out of range", MB_ICONWARNING);
	}
	else if (dCurrentZValue > 0) {
		str.Format("Do you want save %3.06f as Teach Position for %s", dCurrentZValue, strDieID);
		if (AfxMessageBox(str, MB_YESNO) == IDYES) {
			theApp.motionParameters.SetZSensorTeachValue(nCurrentSel, dCurrentZValue);
			theApp.motionParameters.SaveZTeachPosition();

			int nDieSelection = _dieIdCombobox.GetCurSel();
			dZSensorTeachValue = theApp.motionParameters.GetZSensorTeachValue(nDieSelection);
		}
	}
	else {
		AfxMessageBox("Failed To Get Z Data", MB_ICONWARNING);
	}

	UpdateData(FALSE);

}


void CTeachZSensorPosition::OnBnClickedMovePosition()
{
	int nCurrentSel = _dieIdCombobox.GetCurSel();
	theApp.motionParameters.moveToZSensorTeachPos(nCurrentSel);
}

void CTeachZSensorPosition::Move(int axisId, int direction)
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

void CTeachZSensorPosition::RelativeMove(int axisId, int direction)
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

void CTeachZSensorPosition::RedrawPositions()
{
	CString strSensorPos;
	int nDataAvail = 1;
	double dData = 0.0;
	vector <double> zValues;

	strSensorPos = "0";
	//theApp.m_TriangularSensor.ProcessInstance(theApp.m_TrigangualrsensorInstance, dData, zValues, nDataAvail);
	theApp.m_TriangularSensor.ProcessInstancePoll(theApp.m_TrigangualrsensorInstance, dData);
	if (dData < 0) {
		strSensorPos = "OutOfRange";
	}
	else if (dData > 0) {
		strSensorPos.Format("%3.06f", dData);
	}
	else {
		strSensorPos.Format("NoData");
		int nDataVail = 0;
		theApp.m_TriangularSensor.ProcessDataAvail(theApp.m_TrigangualrsensorInstance, nDataVail);
		OutputDebugLogTo(1, TRUE, "Data Avail: %d", nDataAvail);
	}

	ctlrSensorData.SetWindowText(strSensorPos);
}

void CTeachZSensorPosition::OnSelchangeMotorIdCombo()
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

void CTeachZSensorPosition::OnSelchangeDieIdCombo()
{
	int nDieSelection = _dieIdCombobox.GetCurSel();
	dZSensorTeachValue = theApp.motionParameters.GetZSensorTeachValue(nDieSelection);

	UpdateData(FALSE);
}

void CTeachZSensorPosition::OnBnClickedEnableRelativeMove()
{
	_enableRelativeMove = !_enableRelativeMove;
}


void CTeachZSensorPosition::OnBnClickedNegMovement()
{
	if (_enableRelativeMove)
		RelativeMove(_id, -1);
	else
		Move(_id, -1);
}


void CTeachZSensorPosition::OnBnClickedPosMovement()
{
	if (_enableRelativeMove)
		RelativeMove(_id, 1);
	else
		Move(_id, 1);
}


void CTeachZSensorPosition::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == SENSOR_TIMER)
		RedrawPositions();

	CDialogEx::OnTimer(nIDEvent);
}


void CTeachZSensorPosition::OnBnClickedCancel()
{
	KillTimer(SENSOR_TIMER);

	if (!theApp.m_pCalibrationDlg->IsWindowVisible())
		theApp.m_pCalibrationDlg->ShowWindow(TRUE);

	CDialogEx::OnCancel();
}

void CTeachZSensorPosition::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	SetTimer(SENSOR_TIMER, 50, NULL);

	// TODO: Add your message handler code here
}
