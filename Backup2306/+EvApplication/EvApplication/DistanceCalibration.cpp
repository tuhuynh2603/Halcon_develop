// DistanceCalibration.cpp : implementation file
//

#include "stdafx.h"
#include "DistanceCalibration.h"
#include "afxdialogex.h"
#include "CalibrationSettings.h"
#include "Application.h"
#include "TrackHandler.h"
#include "AppDoc.h"


static const UINT_PTR STATION_OFFSET_TIMER = 54;


// CDistanceCalibration dialog

IMPLEMENT_DYNAMIC(CDistanceCalibration, CDialogEx)

CDistanceCalibration::CDistanceCalibration(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CALIB_DISTANCE, pParent)
	, _descriptions(_T(""))
	, _cameraId(0)
	, _calibrationType(0)
	, _motorId(0)
	, _enableRelativeMove(TRUE)		// enable relative motion by default
	, _relativeMoveMm(0)
	, _continuousMove(false)
	, _id(0)
	, _cameraLive(false)
	, _motorMoving(false)
{

}

CDistanceCalibration::~CDistanceCalibration()
{
	_fontDescription.DeleteObject();
}

void CDistanceCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Text(pDX, IDC_DESCRIPTION_STATIC, _descriptions);
	DDX_Radio(pDX, IDC_RADIO_TOP_CAMERA, _cameraId);
	DDX_Radio(pDX, IDC_RADIO_TOP_TRAINGULAR_SENSOR_DISTANCE, _calibrationType);
	DDX_Control(pDX, IDC_MOTOR_ID_COMBO, _motorIdCombobox);
	DDX_CBIndex(pDX, IDC_MOTOR_ID_COMBO, _motorId);
	DDX_Check(pDX, IDC_ENABLE_RELATIVE_MOVE, _enableRelativeMove);
	DDX_Text(pDX, IDC_RELATIVE_MOVE, _relativeMoveMm);
	DDX_Control(pDX, IDC_NEG_MOVEMENT, _negativeMovementButton);
	DDX_Control(pDX, IDC_POS_MOVEMENT, _positiveMovementButton);
	DDX_Control(pDX, IDC_EDIT_MOTOR_CURRENT_POSITION, ctrlMotorPos);
	DDX_Control(pDX, IDC_EDIT_SENSOR_POS, ctrlTSensorPos);
}


BEGIN_MESSAGE_MAP(CDistanceCalibration, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDistanceCalibration::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDistanceCalibration::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_NEG_MOVEMENT, &CDistanceCalibration::OnBnClickedNegMovement)
	ON_BN_CLICKED(IDC_POS_MOVEMENT, &CDistanceCalibration::OnBnClickedPosMovement)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_GRAB, &CDistanceCalibration::OnBnClickedButtonCameraGrab)
	ON_BN_CLICKED(IDC_BUTTON_CAMERA_LIVE, &CDistanceCalibration::OnBnClickedButtonCameraLive)
	ON_BN_CLICKED(IDC_RADIO_TOP_CAMERA, &CDistanceCalibration::OnBnClickedRadioTopCamera)
	ON_BN_CLICKED(IDC_RADIO_THR_CAMERA, &CDistanceCalibration::OnBnClickedRadioThrCamera)
	ON_BN_CLICKED(IDC_ENABLE_RELATIVE_MOVE, &CDistanceCalibration::OnBnClickedEnableRelativeMove)
	ON_CBN_SELCHANGE(IDC_MOTOR_ID_COMBO, &CDistanceCalibration::OnSelchangeMotorIdCombo)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO_TOP_TRAINGULAR_SENSOR_DISTANCE, &CDistanceCalibration::OnBnClickedRadioTopTraingularSensorDistance)
	ON_BN_CLICKED(IDC_RADIO_TOP_THR_DISTANCE, &CDistanceCalibration::OnBnClickedRadioTopThrDistance)
	ON_BN_CLICKED(IDC_BUTTON_MEASURE_DISTANCE, &CDistanceCalibration::OnBnClickedButtonMeasureDistance)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_DATA, &CDistanceCalibration::OnBnClickedButtonSaveData)
	ON_BN_CLICKED(IDC_BUTTON_GRID, &CDistanceCalibration::OnBnClickedButtonGrid)
END_MESSAGE_MAP()


// CDistanceCalibration message handlers
BOOL CDistanceCalibration::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Initialize font and disply descriptions
	_fontDescription.CreateFont(15, 0, 0, 0, 560, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Arial");

//	GetDlgItem(IDC_DESCRIPTION_STATIC)->SetFont(&_fontDescription);

//	updateDescriptions();

	//Initalize bitmaps buttons
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

	return TRUE;
}

void CDistanceCalibration::updateDescriptions()
{
	_descriptions.Append("1.Select \"Top Camera to Z-Sensor Distance\" from the Measurement Selection options\n");
	_descriptions.Append("2.Get Dot targets left corner edge to center of Top camera using the X-Y-Z axis(use setup grid to get exact center position)\n");
	_descriptions.Append("3.After ensuring the camera center position click \'Start\' and move X axis to measure distance for triangular sensor (when the reading reaches to the dot target edge will be approximately 4-6 mm)\n");
	_descriptions.Append("4.Once get triangular positions Press \'Save\' which saves the distance between two\n");
	_descriptions.Append("5.Select \'TOP to THR Distance\' from the Measurement Selection options\n");
	_descriptions.Append("6.Get the dot targets left corner first dot target circle to center of Top camera using the XYZ axis (use setup grid to get exact center position) After ensuring the camera center position click \'Start\'\n");
	_descriptions.Append("7.Get the dot targets left corner first dot target circle to center of THR camera using the XYZ axis (use setup grid to get exact center position). Once get THR camera center positions Press \'Save\' which saves the distance between two.\n");

	UpdateData(FALSE);
}


void CDistanceCalibration::Move(int axisId, int direction)
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

void CDistanceCalibration::RelativeMove(int axisId, int direction)
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


void CDistanceCalibration::RedrawPositions()
{
	int nMotorPos;
	double dMotorResolution;
	CString strMotorPos, strSensorPos;
	int nDataAvail = 1;
	double dData = 0.0;
	vector <double> zValues;

	theApp.motionParameters.GetCurrentMotorPosition(_motorId, nMotorPos);
	theApp.motionParameters.GetMotorResolution(_motorId, dMotorResolution);

	strMotorPos.Format("%d", nMotorPos);
	ctrlMotorPos.SetWindowText(strMotorPos);
	ctrlMotorPos.UpdateData(FALSE);

	strSensorPos = "0";
	if (_calibrationType == SENSOR_TO_TOP) {
		theApp.m_TriangularSensor.ProcessInstancePoll(theApp.m_TrigangualrsensorInstance, dData);
		if (dData > 0) {
			strSensorPos.Format("%3.06f", dData);
		}
		else if (dData < 0) {
			strSensorPos = "Out of range";
		}
		else {
			strSensorPos = "Z Sensor not connected";
		}
	}
	ctrlTSensorPos.SetWindowText(strSensorPos);

}


void CDistanceCalibration::OnBnClickedOk()
{
	KillTimer(STATION_OFFSET_TIMER);

	if (!theApp.m_pCalibrationDlg->IsWindowVisible())
		theApp.m_pCalibrationDlg->ShowWindow(TRUE);

	ShowWindow(SW_HIDE);

	CDialogEx::OnOK();
}


void CDistanceCalibration::OnBnClickedCancel()
{

	KillTimer(STATION_OFFSET_TIMER);

	if (!theApp.m_pCalibrationDlg->IsWindowVisible())
		theApp.m_pCalibrationDlg->ShowWindow(TRUE);

	ShowWindow(SW_HIDE);

	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CDistanceCalibration::OnBnClickedNegMovement()
{
	if (_enableRelativeMove)
		RelativeMove(_id, -1);
	else
		Move(_id, -1);
}


void CDistanceCalibration::OnBnClickedPosMovement()
{
	if (_enableRelativeMove)
		RelativeMove(_id, 1);
	else
		Move(_id, 1);
}


void CDistanceCalibration::OnBnClickedButtonCameraGrab()
{
	theApp.m_pTracks[_cameraId].m_pDoc[0]->CameraGrab();
}


void CDistanceCalibration::OnBnClickedButtonCameraLive()
{
	theApp.m_pTracks[_cameraId].m_pDoc[0]->CameraLive();
	_cameraLive = theApp.m_pTracks[_cameraId].m_bLiveImage;
}


void CDistanceCalibration::OnBnClickedRadioTopCamera()
{
	_cameraId = STATION_TOP;
}


void CDistanceCalibration::OnBnClickedRadioThrCamera()
{
	_cameraId = STATION_THR;
}


void CDistanceCalibration::OnBnClickedEnableRelativeMove()
{
	_enableRelativeMove = !_enableRelativeMove;
}


void CDistanceCalibration::OnSelchangeMotorIdCombo()
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

void CDistanceCalibration::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == STATION_OFFSET_TIMER) {

		bool enableControl = true;

		if (_motorMoving || _continuousMove || _cameraLive)
			enableControl = false;

		GetDlgItem(IDC_RADIO_TOP_CAMERA)->EnableWindow(enableControl);
		GetDlgItem(IDC_RADIO_THR_CAMERA)->EnableWindow(enableControl);
		GetDlgItem(IDC_BUTTON_CAMERA_GRAB)->EnableWindow(enableControl);
//		GetDlgItem(IDC_CALIBRATE_STATION_SHIFT_BUTTON)->EnableWindow(enableControl);
		GetDlgItem(IDOK)->EnableWindow(enableControl);
		RedrawPositions();
	}

	CDialog::OnTimer(nIDEvent);
}

void CDistanceCalibration::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//Initialize timer
	SetTimer(STATION_OFFSET_TIMER, 50, NULL);
}


void CDistanceCalibration::OnBnClickedRadioTopTraingularSensorDistance()
{
	_calibrationType = SENSOR_TO_TOP;
}


void CDistanceCalibration::OnBnClickedRadioTopThrDistance()
{
	_calibrationType = TOP_TO_THR;
}


void CDistanceCalibration::OnBnClickedButtonMeasureDistance()
{

	int nError = theApp.motionParameters.GetCurrentMotorPosition(X_SERVO_MOTOR, _nStartingMotorPosition);
	if (nError)
		AfxMessageBox("Failed to get current motor position");

	// TODO: Add your control notification handler code here
}


void CDistanceCalibration::OnBnClickedButtonSaveData()
{
	double dTotalDistance;
	double dMotorResolution;
	int nError = theApp.motionParameters.GetCurrentMotorPosition(X_SERVO_MOTOR, _nEndMotorPosition);
	if (nError) {
	
	}
	theApp.motionParameters.GetMotorResolution(_motorId, dMotorResolution);

	dTotalDistance = abs(double(_nEndMotorPosition/ dMotorResolution)- double(_nStartingMotorPosition/ dMotorResolution));
	CString str;
	if(_calibrationType == SENSOR_TO_TOP)
		str.Format("Measured distance between Trinagular Sensor To Top Camera is %f mm, do you wish to save ?", dTotalDistance);
	else
		str.Format("Measured distance between Top Camera to THR Camera is %f mm, do you wish to save ?", dTotalDistance);

	if (AfxMessageBox(str, MB_YESNO) == IDYES) {
		if (_calibrationType == SENSOR_TO_TOP)
			theApp.motionParameters.SetZSensortoTopDistance(dTotalDistance);
		else
			theApp.motionParameters.SetTopToTHRDistance(dTotalDistance);

		theApp.motionParameters.SaveMotionParm();
	}
}


void CDistanceCalibration::OnBnClickedButtonGrid()
{
	theApp.m_pTracks[_cameraId].m_pDoc[0]->OnViewOptionsSetupGrid();
}
