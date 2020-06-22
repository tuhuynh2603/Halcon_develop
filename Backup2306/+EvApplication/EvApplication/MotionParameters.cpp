#include "StdAfx.h"
#include "Application.h"
#include "MotionParameters.h"
#include "IOParam.h"
#include "DebugLog.h"
#include "RegistryAccess.h"
#include "MsgCode.h"
#include "IniFile.h"
#include "operatorOverloading.h"
#include "TriangularSensor.h"
#include "ImageSelectionDlg.h"
#include <numeric>
#include <map>
#include <fstream>
#include <direct.h>

MotionParameters::MotionParameters() : motorResource( theApp.m_MotorResource )
{

	InitializeCriticalSection(&CriticalSectionCommands);
	pApp = (CApplication*) AfxGetApp();
	//Motor Names
	motorCount = maximumMotorCount;
	
	motorEngine[X_SERVO_MOTOR].setName("X Servo Motor");
	motorEngine[WIDTH_ADJUSTMENT_STEPPER_MOTOR].setName("Width-Adjustment Motor");
	motorEngine[Y_SERVO_MOTOR].setName("Y Servo Motor");
	motorEngine[PUSH_PULL_LINEAR_MOTOR].setName("Push-Pull Linear Motor");
	motorEngine[ST1_Z_SERVO_MOTOR].setName("ST1 Z Servo Motor");
	motorEngine[ST2_Z_SERVO_MOTOR].setName("ST2 Z Servo Motor");
	

	// Motor variables
	motorEngine[X_SERVO_MOTOR].setAxisId(1500); // 2-3D X Servo Motor
	motorEngine[WIDTH_ADJUSTMENT_STEPPER_MOTOR].setAxisId(1507); // Width-Adjustment Motor
	motorEngine[Y_SERVO_MOTOR].setAxisId(1501); // 2-3D Y Servo Motor
	motorEngine[PUSH_PULL_LINEAR_MOTOR].setAxisId(1506); // Push-Pull Linear Motor
	motorEngine[ST1_Z_SERVO_MOTOR].setAxisId(1502); // Station 1 Z Servo Motor
	motorEngine[ST2_Z_SERVO_MOTOR].setAxisId(1503); // Station 2 Z Servo Motor
	

	motorEngine[X_SERVO_MOTOR].setHomingMode(2);
	motorEngine[WIDTH_ADJUSTMENT_STEPPER_MOTOR].setHomingMode(0);
	motorEngine[Y_SERVO_MOTOR].setHomingMode(2);
	motorEngine[PUSH_PULL_LINEAR_MOTOR].setHomingMode(2);
	motorEngine[ST1_Z_SERVO_MOTOR].setHomingMode(2);
	motorEngine[ST2_Z_SERVO_MOTOR].setHomingMode(2);
	

	motorEngine[X_SERVO_MOTOR].setResolution(1638.4);
	motorEngine[WIDTH_ADJUSTMENT_STEPPER_MOTOR].setResolution(250);
	motorEngine[Y_SERVO_MOTOR].setResolution(1638.4);
	motorEngine[PUSH_PULL_LINEAR_MOTOR].setResolution(1000);
	motorEngine[ST1_Z_SERVO_MOTOR].setResolution(8192);
	motorEngine[ST2_Z_SERVO_MOTOR].setResolution(8192);
	
	motorEngine[X_SERVO_MOTOR].setHomePosition(0);
	motorEngine[WIDTH_ADJUSTMENT_STEPPER_MOTOR].setHomePosition(0);
	motorEngine[Y_SERVO_MOTOR].setHomePosition(0);
	motorEngine[PUSH_PULL_LINEAR_MOTOR].setHomePosition(0);
	motorEngine[ST1_Z_SERVO_MOTOR].setHomePosition(0);
	motorEngine[ST2_Z_SERVO_MOTOR].setHomePosition(0);


	// Scale variables
	scaleCount = maximumScaleCount;

	scaleEngine[X_SCALE_1].setAxisId(X_SCALE_1); // Linear Scale {1} for X-axis
	scaleEngine[X_SCALE_2].setAxisId(X_SCALE_2); // Linear Scale {2} for X-axis
	scaleEngine[X_SCALE_3].setAxisId(X_SCALE_3); // Linear Scale {3} for X-axis
	//scaleEngine[Y_SCALE_2].setAxisId(Y_SCALE_2); // Linear Scale {2} for Y-axis (top)

	scaleEngine[X_SCALE_1].setName("X Scale 1");
	scaleEngine[X_SCALE_2].setName("X Scale 2");
	scaleEngine[X_SCALE_3].setName("X Scale 3");
	//scaleEngine[Y_SCALE_2].setName("Y Scale 2");

	for (int i = 0; i < scaleCount; i++)
		scaleEngine[i].setResolution(5000);


	for(int i = 0; i < motorCount; i++) {
		_motorScanVelocity[i]._setPositionName("Scanning");
		_motorScanVelocityTHR[i]._setPositionName("Scanning_THR");
		_motorMoveVelocity[i]._setPositionName("Moving");

		_motorMoveVelocity[i].startSpeed = 1000;
		_motorScanVelocity[i].startSpeed = 1000;
		_motorScanVelocityTHR[i].startSpeed = 1000;
		_motorMoveVelocity[i].maximumSpeed = 10000;
		_motorScanVelocity[i].maximumSpeed = 10000;
		_motorScanVelocityTHR[i].maximumSpeed = 10000;
	}

	nTriggerInversePulse = 1;
	nTriggerPulseWidth = 100;//us

	for(int i = 0; i < motorCount; i++) {
		motorStartScanPos[i] = 0;
		motorInitScanPos[i] = 0;
		motorModuleUnloadPos[i] = 0;
		for (int k = 0; k < MAX_FOV_TOP; k++) {
			motorTOPStartScanPos[i][k] = 0;
		}
	}
	
	//Individual Z Sensor Teach Position
	for (int nDie = 0; nDie < MAX_DIES; nDie++) {
		for (int i = 0; i < motorCount; i++) {
			motorZSensorTeachPos[nDie][i] = 0;
		}
	}


	for (int i = 0; i < 4; i++)
	{for (int j = 0; j < 2; j++)
	{
		JigPosition[i][j] = 0;
		JigSensorValue[i]=0;

		}
	}


//	motorTHRStartScanPos[0] = 0;
//	motorTHRStartScanPos[1] = 0;
//	motorTHRStartScanPos[2] = 0;

//	motorZSensorStartScanPos[0] = 0;
//	motorZSensorStartScanPos[1] = 0;
//	motorZSensorStartScanPos[2] = 0;
	

	scanningReady = false;
							   // Scanning mode 1 (wafer mapping)
	m_nNoOfBlock[0] = 5;
	m_nNoOfBlock[1] = 2;
	m_dBlockPitch[0] = 10;
	m_dBlockPitch[1] = 10;
	m_nDeviceNoPerBlock[0] = 12;
	m_nDeviceNoPerBlock[1] = 3;
	m_dDevicePitch[0] = 5;
	m_dDevicePitch[1] = 5;
	nDieColumn = 1;
	nDieRow = 1;

	// Device Size
	dDeviceSizeInXMM = 2;
	dDeviceSizeInYMM = 2;

	
//	InitializeCriticalSection(&CriticalSectionData);

	nTotalImageFOVTHR = 0;

	// Z focusing levels for bond station
	// Triggering and movement for 2-3D station
	
	nScanColumnNoTHR = 0;
	nScanRowNoTHR = 0;

	nScanColumnNoTop = 0;
	nScanRowNoTop = 0;

	nScanColumnNoZSensor = 0;
	nScanRowNoZSensor = 0;

	motorYScanningPositionTHR.clear();
	motorYScanningPositionTop.clear();
	motorYScanningPositionTopEncap.clear();


	dieColumnNoTHR.clear();
	dieRowNoTHR.clear();
	dieNoTHR.clear();

	m_dDistanceZSensorToTop = 0;
	m_dDistanceTopToTHR = 0;
	dMotorXScanOffsetMM = 1;
	dForwardTriggerOffset = 0;
	dInverseTriggerOffset = 0;

	m_dZSensorTeachValue = 4.6188;
	for (int nDie = 0; nDie < MAX_DIES; nDie++) {
		m_dZSensorDieTeachValue[nDie] = 4.6188;
	}

	nNumberofZTriggerpoints = 30;
	m_dInitialOffset = 21.717;
	m_bEnablebarcodeReader = TRUE;
	m_bEnablebarcodeReaderWithCylinder = TRUE;
	m_bEnableIonizer	= TRUE;
	m_bEnableTHRScanning = TRUE;
	m_bEnableTopScanning = TRUE;
	
	m_dTopRowOffset[0] = 0;
	m_dTopRowOffset[1] = 0;
	m_dBottomRowOffset[0] = 0;
	m_dBottomRowOffset[1] = 0;
	m_dEncapOffsetX = 0;
	m_dEncapOffsetY = 0;

	

	dOffsetXScanTHRMicron.clear();
	dOffsetYScanTHRMicron.clear();

	InterlockedExchange(&nZSensorScanRow, - 1);

}

MotionParameters::MotionParameters(const MotionParameters & motionParameters) : motorResource(motionParameters.motorResource)
{
	InitializeCriticalSection(&CriticalSectionCommands);
	pApp = (CApplication*) AfxGetApp();

	motorEngine = motionParameters.motorEngine;

	// Scale variables
	scaleCount = motionParameters.scaleCount;
	scaleEngine = motionParameters.scaleEngine;

	_motorMoveVelocity = motionParameters._motorMoveVelocity;
	_motorScanVelocity = motionParameters._motorScanVelocity;
	_motorScanVelocityTHR = motionParameters._motorScanVelocityTHR;
	//Motor Names

	nTriggerInversePulse = motionParameters.nTriggerInversePulse;
	nTriggerPulseWidth = motionParameters.nTriggerPulseWidth;

	motorCount = motionParameters.motorCount;
	motorStartScanPos = motionParameters.motorStartScanPos;
	motorInitScanPos = motionParameters.motorInitScanPos;
	motorModuleUnloadPos = motionParameters.motorModuleUnloadPos;

	for (int nFov = 0; nFov < MAX_FOV_TOP; nFov++) {
		motorTOPStartScanPos[nFov] = motionParameters.motorTOPStartScanPos[nFov];
	}

	//Individual Z Sensor Teach Position
	for (int nDie = 0; nDie < MAX_DIES; nDie++) {
		motorZSensorTeachPos[nDie] = motionParameters.motorZSensorTeachPos[nDie];
	}


	scanningReady = motionParameters.scanningReady;
	// Scanning mode 1 (wafer mapping)
	m_nNoOfBlock[0] = motionParameters.m_nNoOfBlock[0];
	m_nNoOfBlock[1] = motionParameters.m_nNoOfBlock[1];
	m_dBlockPitch[0] = motionParameters.m_dBlockPitch[0];
	m_dBlockPitch[1] = motionParameters.m_dBlockPitch[1];
	m_nDeviceNoPerBlock[0] = motionParameters.m_nDeviceNoPerBlock[0];
	m_nDeviceNoPerBlock[1] = motionParameters.m_nDeviceNoPerBlock[1];
	m_dDevicePitch[0] = motionParameters.m_dDevicePitch[0];
	m_dDevicePitch[1] = motionParameters.m_dDevicePitch[1];
	nDieColumn = motionParameters.nDieColumn;
	nDieRow = motionParameters.nDieRow;

	// Device Size
	dDeviceSizeInXMM = motionParameters.dDeviceSizeInXMM;
	dDeviceSizeInYMM = motionParameters.dDeviceSizeInYMM;


	//	InitializeCriticalSection(&CriticalSectionData);

	nTotalImageFOVTHR = motionParameters.nTotalImageFOVTHR;
	nTotalImageFOVTOP = motionParameters.nTotalImageFOVTOP;

	// Z focusing levels for bond station
	// Triggering and movement for 2-3D station
	
	nScanColumnNoTHR = motionParameters.nScanColumnNoTHR;
	nScanRowNoTHR = motionParameters.nScanRowNoTHR;

	nScanColumnNoTop = motionParameters.nScanColumnNoTop;
	nScanRowNoTop = motionParameters.nScanRowNoTop;

	nScanColumnNoZSensor = motionParameters.nScanColumnNoZSensor;
	nScanRowNoZSensor = motionParameters.nScanRowNoZSensor;

	m_dDistanceZSensorToTop = motionParameters.m_dDistanceZSensorToTop;
	m_dDistanceTopToTHR = motionParameters.m_dDistanceTopToTHR;
	dMotorXScanOffsetMM = motionParameters.dMotorXScanOffsetMM;
	dForwardTriggerOffset = motionParameters.dForwardTriggerOffset;
	dInverseTriggerOffset = motionParameters.dInverseTriggerOffset;

	m_dZSensorTeachValue = motionParameters.m_dZSensorTeachValue;
	for(int nDie = 0; nDie < MAX_DIES; nDie++)
		m_dZSensorDieTeachValue[nDie] = motionParameters.m_dZSensorDieTeachValue[nDie];

	nNumberofZTriggerpoints = motionParameters.nNumberofZTriggerpoints;
	m_dInitialOffset = motionParameters.m_dInitialOffset;
	m_bEnablebarcodeReader = motionParameters.m_bEnablebarcodeReader;
	m_bEnablebarcodeReaderWithCylinder = motionParameters.m_bEnablebarcodeReaderWithCylinder;
	
	m_bEnableIonizer = motionParameters.m_bEnableIonizer;
	m_bEnableTopScanning = motionParameters.m_bEnableTopScanning;
	
	m_bEnableTHRScanning = motionParameters.m_bEnableTHRScanning;
	
	scanningImageComputerId = motionParameters.scanningImageComputerId;
	dOffsetXScanTHRMicron = motionParameters.dOffsetXScanTHRMicron;
	dOffsetYScanTHRMicron = motionParameters.dOffsetYScanTHRMicron;
	motorYScanningPositionTHR = motionParameters.motorYScanningPositionTHR;
	motorYScanningPositionTop = motionParameters.motorYScanningPositionTop;
	motorYScanningPositionTopEncap = motionParameters.motorYScanningPositionTopEncap;
	motorXStartScanningPositionTHR = motionParameters.motorXStartScanningPositionTHR;
	motorXEndScanningPositionTHR = motionParameters.motorXEndScanningPositionTHR;
	motorXStartScanningPositionTop = motionParameters.motorXEndScanningPositionTop;
	motorXStartScanningPositionTopEncap = motionParameters.motorXEndScanningPositionTopEncap;
	motorXEndScanningPositionTop = motionParameters.motorXEndScanningPositionTop;
	motorXStartScanningPositionZ = motionParameters.motorXStartScanningPositionZ;
	motorXEndScanningPositionZ = motionParameters.motorXEndScanningPositionZ;
	motorXStartScanningPositionTopandZ = motionParameters.motorXStartScanningPositionTopandZ;
	motorXEndScanningPositionTopandZ = motionParameters.motorXEndScanningPositionTopandZ;
	m_dTopRowOffset[0] = motionParameters.m_dTopRowOffset[0];
	m_dTopRowOffset[1] = motionParameters.m_dTopRowOffset[1];
	m_dBottomRowOffset[0] = motionParameters.m_dBottomRowOffset[0];
	m_dBottomRowOffset[1] = motionParameters.m_dBottomRowOffset[1];
	m_dEncapOffsetX = motionParameters.m_dEncapOffsetX;
	m_dEncapOffsetY = motionParameters.m_dEncapOffsetY;


}

const MotionParameters & MotionParameters::operator=(const MotionParameters & motionParameters)
{
	if(this == &motionParameters)
		return *this;

	motorEngine = motionParameters.motorEngine;

	// Scale variables
	scaleCount = motionParameters.scaleCount;
	scaleEngine = motionParameters.scaleEngine;

	// Motor variables
	motorCount = motionParameters.motorCount;

	_motorMoveVelocity = motionParameters._motorMoveVelocity;
	_motorScanVelocity = motionParameters._motorScanVelocity;
	_motorScanVelocityTHR = motionParameters._motorScanVelocityTHR;

	nTriggerInversePulse = motionParameters.nTriggerInversePulse;
	nTriggerPulseWidth = motionParameters.nTriggerPulseWidth;

	motorStartScanPos = motionParameters.motorStartScanPos;
	motorInitScanPos = motionParameters.motorInitScanPos;
	motorModuleUnloadPos = motionParameters.motorModuleUnloadPos;
	
	for (int nFov = 0; nFov < MAX_FOV_TOP; nFov++) {
		motorTOPStartScanPos[nFov] = motionParameters.motorTOPStartScanPos[nFov];
	}

	//Individual Z Sensor Teach Position
	for (int nDie = 0; nDie < MAX_DIES; nDie++) {
		motorZSensorTeachPos[nDie] = motionParameters.motorZSensorTeachPos[nDie];
	}


	motorDisplacement = motionParameters.motorDisplacement;

	// Scanning mode 1 (substrate mapping)
	m_nNoOfBlock[0] = motionParameters.m_nNoOfBlock[0];
	m_nNoOfBlock[1] = motionParameters.m_nNoOfBlock[1];
	m_dBlockPitch[0] = motionParameters.m_dBlockPitch[0];
	m_dBlockPitch[1] = motionParameters.m_dBlockPitch[1];
	m_nDeviceNoPerBlock[0] = motionParameters.m_nDeviceNoPerBlock[0];
	m_nDeviceNoPerBlock[1] = motionParameters.m_nDeviceNoPerBlock[1];
	m_dDevicePitch[0] = motionParameters.m_dDevicePitch[0];
	m_dDevicePitch[1] = motionParameters.m_dDevicePitch[1];
	nDieColumn = motionParameters.nDieColumn;
	nDieRow = motionParameters.nDieRow;

	// Merging for 2-3D station
//	InitializeCriticalSection(&CriticalSectionData);

// Device Size
	dDeviceSizeInXMM = motionParameters.dDeviceSizeInXMM;
	dDeviceSizeInYMM = motionParameters.dDeviceSizeInYMM;


	nTotalImageFOVTHR = motionParameters.nTotalImageFOVTHR;
	nTotalImageFOVTOP = motionParameters.nTotalImageFOVTOP;

	// Triggering and movement for 2-3D station
	nScanColumnNoTHR = motionParameters.nScanColumnNoTHR;
	nScanRowNoTHR = motionParameters.nScanRowNoTHR;

	nScanColumnNoTop = motionParameters.nScanColumnNoTop;
	nScanRowNoTop = motionParameters.nScanRowNoTop;

	nScanColumnNoZSensor = motionParameters.nScanColumnNoZSensor;
	nScanRowNoZSensor = motionParameters.nScanRowNoZSensor;

	m_dDistanceZSensorToTop = motionParameters.m_dDistanceZSensorToTop;
	m_dDistanceTopToTHR = motionParameters.m_dDistanceTopToTHR;
	dMotorXScanOffsetMM = motionParameters.dMotorXScanOffsetMM;
	dForwardTriggerOffset = motionParameters.dForwardTriggerOffset;
	dInverseTriggerOffset = motionParameters.dInverseTriggerOffset;

	m_dZSensorTeachValue = motionParameters.m_dZSensorTeachValue;
	for (int nDie = 0; nDie < MAX_DIES; nDie++)
		m_dZSensorDieTeachValue[nDie] = motionParameters.m_dZSensorDieTeachValue[nDie];

	nNumberofZTriggerpoints = motionParameters.nNumberofZTriggerpoints;
	m_dInitialOffset = motionParameters.m_dInitialOffset;
	m_bEnablebarcodeReaderWithCylinder = motionParameters.m_bEnablebarcodeReaderWithCylinder;
	
	m_bEnableIonizer = motionParameters.m_bEnableIonizer;
	m_bEnableTopScanning = motionParameters.m_bEnableTopScanning;
	
	m_bEnableTHRScanning = motionParameters.m_bEnableTHRScanning;
	
	scanningImageComputerId = motionParameters.scanningImageComputerId;
	dOffsetXScanTHRMicron = motionParameters.dOffsetXScanTHRMicron;
	dOffsetYScanTHRMicron = motionParameters.dOffsetYScanTHRMicron;
	motorYScanningPositionTHR = motionParameters.motorYScanningPositionTHR;
	motorYScanningPositionTop = motionParameters.motorYScanningPositionTop;
	motorYScanningPositionTopEncap = motionParameters.motorYScanningPositionTopEncap;
	motorXStartScanningPositionTHR = motionParameters.motorXStartScanningPositionTHR;
	motorXEndScanningPositionTHR = motionParameters.motorXEndScanningPositionTHR;
	motorXStartScanningPositionTop = motionParameters.motorXEndScanningPositionTop;
	motorXStartScanningPositionTopEncap = motionParameters.motorXEndScanningPositionTopEncap;
	motorXEndScanningPositionTop = motionParameters.motorXEndScanningPositionTop;
	motorXStartScanningPositionZ = motionParameters.motorXStartScanningPositionZ;
	motorXEndScanningPositionZ = motionParameters.motorXEndScanningPositionZ;
	motorXStartScanningPositionTopandZ = motionParameters.motorXStartScanningPositionTopandZ;
	motorXEndScanningPositionTopandZ = motionParameters.motorXEndScanningPositionTopandZ;
	m_dTopRowOffset[0] = motionParameters.m_dTopRowOffset[0];
	m_dTopRowOffset[1] = motionParameters.m_dTopRowOffset[1];
	m_dBottomRowOffset[0] = motionParameters.m_dBottomRowOffset[0];
	m_dBottomRowOffset[1] = motionParameters.m_dBottomRowOffset[1];

	m_dEncapOffsetX = motionParameters.m_dEncapOffsetX;
	m_dEncapOffsetY = motionParameters.m_dEncapOffsetY;

	return (*this);
}

MotionParameters::~MotionParameters()
{
	DeleteCriticalSection(&CriticalSectionCommands);
//	DeleteCriticalSection(&CriticalSectionData);
}

void MotionParameters::SetMotionMovementParameters(const MotionParameters & motionParameters)
{
	_motorMoveVelocity = motionParameters._motorMoveVelocity;
	_motorScanVelocity = motionParameters._motorScanVelocity;
	_motorScanVelocityTHR = motionParameters._motorScanVelocityTHR;

	for(int i = 0; i < motorCount; i++) {

		motorEngine[i].setForwardHoming(motionParameters.motorEngine[i].isForwardHoming());
		motorEngine[i].setForwardMovement(motionParameters.motorEngine[i].isForwardMovement());
		motorEngine[i].setHomeOriginOffset(motionParameters.motorEngine[i].getHomeOriginOffset());
		motorEngine[i].setSettlingTime(motionParameters.motorEngine[i].getSettlingTime());
		motorEngine[i].setHomingSpeed(motionParameters.motorEngine[i].getHomingSpeed());
	}
}

void MotionParameters::InitMotionCard()
{
	EnterCriticalSection(&CriticalSectionCommands);

	motorResource.EnableInterrupt(theApp.m_nMotorCardNo, 1);

	for(int i = 0; i < motorCount; i++)
		motorEngine[i].initialize();

//	OutputDebugLogTo(2, TRUE, "initialize fifo array, motor card %d, %d, %d ", theApp.m_nMotorCardNo, nTriggerPulseWidth, nTriggerInversePulse);
//	motorResource.InitializeFifoArray(theApp.m_nMotorCardNo, 0, 0, 0, 0, nTriggerPulseWidth, nTriggerInversePulse);		//Camera
//	motorResource.InitializeFifoArray(theApp.m_nMotorCardNo, 0, 3, 0, 3, nTriggerPulseWidth, nTriggerInversePulse);		//Z Sensor
//	//motorResource.InitializeFifoArray(theApp.m_nMotorCardNo, 0, 0, 0, 1, nTriggerPulseWidth, nTriggerInversePulse);
//
//	motorResource.EnableLatch(motorEngine[X_SERVO_MOTOR]._getAxisId());
//
//	motorResource.ResetTriggerCount(theApp.m_nMotorCardNo, 0, 0);
//	motorResource.ResetTriggerCount(theApp.m_nMotorCardNo, 0, 3);
////	motorResource.ResetTriggerCount(theApp.m_nMotorCardNo, 0, 1);

	LeaveCriticalSection(&CriticalSectionCommands);
}

void MotionParameters::InitScaleCard()
{
	EnterCriticalSection(&CriticalSectionCommands);

	int nCounterSource = 0;
	scaleEngine[X_SCALE_1].initialize(X_SCALE_1, false, nTriggerPulseWidth * 1000, nCounterSource);
	scaleEngine[X_SCALE_2].initialize(X_SCALE_2, false, nTriggerPulseWidth * 1000, nCounterSource);
	scaleEngine[X_SCALE_3].initialize(X_SCALE_3, false, nTriggerPulseWidth * 1000, nCounterSource);
	//scaleEngine[Y_SCALE_2].initialize(Y_SCALE_2, false, nTriggerPulseWidth * 1000, Y_SCALE_2);

	scaleEngine[X_SCALE_1].enableLatching();
	//scaleEngine[Y_SCALE_1].enableLatching();

	scaleEngine[X_SCALE_1].resetTriggerCount();
	scaleEngine[X_SCALE_2].resetTriggerCount();
	scaleEngine[X_SCALE_3].resetTriggerCount();
	//scaleEngine[Y_SCALE_2].resetTriggerCount();


	LeaveCriticalSection(&CriticalSectionCommands);
}


void MotionParameters::FreeMotionCard()
{
	EnterCriticalSection(&CriticalSectionCommands);

	for(int i = 0; i < motorCount; i++) {
		motorEngine[i].free();
	}

	LeaveCriticalSection(&CriticalSectionCommands);
}

// Single motor axis interraction
int MotionParameters::CheckSafety(int nAxisNo, bool bSafety, bool bHomeByPass)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	int nCurpos;

	CString strTemp;

	
	if(bSafety) {

	}

	return 0;
}

int MotionParameters::CheckSoftwareLimits(int nAxisNo, int nPos)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	return motorEngine[nAxisNo].validatePosition(nPos);
}

bool MotionParameters::IsSoftwareLimits(int nAxisNo, int nPos)
{
	if (nAxisNo < 0 || nAxisNo >= motorCount)
		return true;

	return motorEngine[nAxisNo].isSoftwareLimits(nPos);
}

int MotionParameters::SetServoOn(int nAxisNo)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	EnterCriticalSection(&CriticalSectionCommands);

	int nError = motorEngine[nAxisNo].servoOn();

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;
}


int MotionParameters::SetServoOff(int nAxisNo)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	EnterCriticalSection(&CriticalSectionCommands);

	int nError = motorEngine[nAxisNo].servoOff();

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;
}

int MotionParameters::StopAxis(int nAxisNo)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	EnterCriticalSection(&CriticalSectionCommands);

	int nError = motorEngine[nAxisNo].stop();

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;
}

int MotionParameters::EmergencyStop(int nAxisNo)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	EnterCriticalSection(&CriticalSectionCommands);

	int nError = motorEngine[nAxisNo].emergencyStop();

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;
}

int MotionParameters::ResetAlarm(int nAxisNo)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	int nError;

	EnterCriticalSection(&CriticalSectionCommands);

	nError = motorEngine[nAxisNo].resetAlarm();

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;
}

int MotionParameters::HomeMove(int nAxisNo, bool bSafety, bool bHomeByPass)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount || CheckSafety(nAxisNo, bSafety, bHomeByPass) || CheckSoftwareLimits(nAxisNo, 0))
		return -1;

	//Scale Engine
	if (nAxisNo == X_SERVO_MOTOR) {
		scaleEngine[X_SCALE_1].startHoming();
		scaleEngine[X_SCALE_2].startHoming();
		scaleEngine[X_SCALE_3].startHoming();
	}
	
	EnterCriticalSection(&CriticalSectionCommands);

	int nError = motorEngine[nAxisNo].startHoming();

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;
}

int MotionParameters::HomeDone(const int nAxisNo)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	Sleep(motorEngine[nAxisNo].getSettlingTime() * 10);

	int nError;

	EnterCriticalSection(&CriticalSectionCommands);

	nError = motorEngine[nAxisNo].completeHoming();
	if(nError) {
		LeaveCriticalSection(&CriticalSectionCommands);
		return nError;
	}

	// Reset linear scale values
	if (nAxisNo == X_SERVO_MOTOR) {

		nError = scaleEngine[X_SCALE_1].completeHoming();
		if (nError) {
			LeaveCriticalSection(&CriticalSectionCommands);
			return nError;
		}

		nError = scaleEngine[X_SCALE_2].completeHoming();
		if (nError) {
			LeaveCriticalSection(&CriticalSectionCommands);
			return nError;
		}

		nError = scaleEngine[X_SCALE_3].completeHoming();
		if (nError) {
			LeaveCriticalSection(&CriticalSectionCommands);
			return nError;
		}

		/*nError = scaleEngine[Y_SCALE_2].completeHoming();
		if (nError) {
			LeaveCriticalSection(&CriticalSectionCommands);
			return nError;
		}*/
	}
	
	LeaveCriticalSection(&CriticalSectionCommands);

	return 0;
}

int MotionParameters::AbsoluteMove(int nAxisNo, int nPos, Speed speed, bool bSafety, bool bHomeByPass)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount || speed.startSpeed > speed.maximumSpeed || CheckSafety(nAxisNo, bSafety, bHomeByPass) || CheckSoftwareLimits(nAxisNo, nPos))
		return -1;

	int nError;

	EnterCriticalSection(&CriticalSectionCommands);

	nError = motorEngine[nAxisNo].absoluteMove(nPos, speed.startSpeed, speed.maximumSpeed);

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;
}

int MotionParameters::RelativeMove(int nAxisNo, int nPos, Speed speed, bool bSafety, bool bHomeByPass)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount || speed.startSpeed > speed.maximumSpeed || CheckSafety(nAxisNo, bSafety, bHomeByPass))
		return -1;

	int nError;

	EnterCriticalSection(&CriticalSectionCommands);

	nError = motorEngine[nAxisNo].relativeMove(nPos, speed.startSpeed, speed.maximumSpeed);

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;
}

int MotionParameters::ChangeSpeedOnFly(int nAxisNo, int nNewMaxVel)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	int nError;

	EnterCriticalSection(&CriticalSectionCommands);

	nError = motorEngine[nAxisNo].changeSpeed(nNewMaxVel);

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;
}

int MotionParameters::ContinuousMove(int nAxisNo, BOOL bForward, Speed speed, bool bSafety, bool bHomeByPass)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount || speed.startSpeed > speed.maximumSpeed || CheckSafety(nAxisNo, bSafety, bHomeByPass))
		return -1;

	int nError = StopAxis(nAxisNo);
	if(nError) return nError;

	EnterCriticalSection(&CriticalSectionCommands);

	nError = motorEngine[nAxisNo].continuousMove(abs(speed.startSpeed), abs(speed.maximumSpeed), bForward != FALSE);

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;
}

int MotionParameters::WaitForMotionDone(int nAxisNo, int nTimeOut)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	return motorEngine[nAxisNo].waitMotionCompletion(nTimeOut);
}

int MotionParameters::IsMotionDone(int nAxisNo, BOOL & bDone)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	bDone = motorEngine[nAxisNo].isMotionCompleted();

	return 0;
}

int MotionParameters::getMotorStatus(int axisId, long & status)
{
	if(axisId < 0 || axisId >= motorCount)
		return -1;

	return motorEngine[axisId].getMotorStatus(status);
}


int MotionParameters::GetCurrentMotorPosition(int nAxisNo, int & nPos)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	return motorEngine[nAxisNo].getCurrentPosition(nPos);
}

BOOL MotionParameters::IsMotorActive(int nAxisNo)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return FALSE;

	return motorEngine[nAxisNo].isActive();
}

BOOL MotionParameters::IsMotorHomed(int nAxisNo) const
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return FALSE;

	return motorEngine[nAxisNo].getHomedStatus();
}

BOOL MotionParameters::IsMotorLimit(int nAxisNo)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return TRUE;

	return motorEngine[nAxisNo].isLimit();
}

int MotionParameters::GetMotorAlarmStatus(int nAxisNo, BOOL &bOn)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	int nError = motorEngine[nAxisNo].getAlarmStatus(bOn);
	if(nError)
		return nError;

	return 0;
}

int MotionParameters::GetMotorIOStatus(int nAxisNo, long & nStatus)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	return motorEngine[nAxisNo].getIoStatus(nStatus);
}

int MotionParameters::GetMotorResolution(int nAxisNo, double & dResolution) const
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	dResolution = motorEngine[nAxisNo].getResolution();

	return 0;
}

int MotionParameters::SetMotorResolution(int nAxisNo, double dResolution)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	motorEngine[nAxisNo].setResolution(dResolution);

	return 0;
}

int MotionParameters::GetMotorSettlingTime(int nAxisNo, int & nTime) const
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;
	nTime = motorEngine[nAxisNo].getSettlingTime();

	return 0;
}

int MotionParameters::SetMotorSettlingTime(int nAxisNo, int nTime)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	motorEngine[nAxisNo].setSettlingTime(nTime);

	return 0;
}

Speed MotionParameters::getHomingSpeed(int axisId) const
{
	if(axisId < 0 || axisId >= motorCount)
		return Speed(0, 0);
	else
		return motorEngine[axisId].getHomingSpeed();
}

Speed MotionParameters::getScanningSpeed(int axisId) const
{
	if(axisId < 0 || axisId >= motorCount)
		return Speed(0, 0);
	else
		return _motorScanVelocity[axisId];
}

Speed MotionParameters::getScanningSpeedTHR(int axisId) const
{
	if(axisId < 0 || axisId >= motorCount)
		return Speed(0, 0);
	else
		return _motorScanVelocityTHR[axisId];
}


Speed MotionParameters::getMovingSpeed(int axisId) const
{
	if(axisId < 0 || axisId >= motorCount)
		return Speed(0, 0);
	else
		return _motorMoveVelocity[axisId];
}

int MotionParameters::setHomingSpeed(int axisId, Speed speed)
{
	if(axisId < 0 || axisId >= motorCount)
		return -1;

	motorEngine[axisId].setHomingSpeed(speed);

	return 0;
}

int MotionParameters::setScanningSpeed(int axisId, Speed speed)
{
	if(axisId < 0 || axisId >= motorCount)
		return -1;

	_motorScanVelocity[axisId] = speed;

	return 0;
}

int MotionParameters::setScanningSpeedTHR(int axisId, Speed speed)
{
	if(axisId < 0 || axisId >= motorCount)
		return -1;

	_motorScanVelocityTHR[axisId] = speed;

	return 0;
}

int MotionParameters::setMovingSpeed(int axisId, Speed speed)
{
	if(axisId < 0 || axisId >= motorCount)
		return -1;

	_motorMoveVelocity[axisId] = speed;

	return 0;
}


bool MotionParameters::checkSoftLimitSetStatus()
{
	bool set = true;

	for(int i = 0; i < motorCount; i++) {
		if(!motorEngine[i].isSoftwareLimitSet()) {
			set = false;
			break;
		}
	}

	return set;
}

int MotionParameters::GetMotorHomeOriginOffset(int nAxisNo, int & nOffset) const
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	nOffset = motorEngine[nAxisNo].getHomeOriginOffset();

	return 0;
}

int MotionParameters::SetMotorHomeOriginOffset(int nAxisNo, int nOffset)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	motorEngine[nAxisNo].setHomeOriginOffset(nOffset);

	return 0;
}

int MotionParameters::GetMotorHomeDirection(int nAxisNo, BOOL & bForward) const
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	bForward = motorEngine[nAxisNo].isForwardHoming();

	return 0;
}

int MotionParameters::SetMotorHomeDirection(int nAxisNo, BOOL bForward)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	motorEngine[nAxisNo].setForwardHoming(bForward != FALSE);

	return 0;
}

//BOOL MotionParameters::GetMotorHomeCheckByPass()
//{
//	return bEnableMotorHomeByPass;
//}

//void MotionParameters::SetMotorHomeCheckByPass(BOOL bHomeCheckByPass)
//{
//	bEnableMotorHomeByPass = bHomeCheckByPass;
//}

int MotionParameters::GetMotorMovementDirection(int nAxisNo, BOOL & bForward) const
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	bForward = motorEngine[nAxisNo].isForwardMovement();

	return 0;
}

int MotionParameters::SetMotorMovementDirection(int nAxisNo, BOOL bForward)
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	motorEngine[nAxisNo].setForwardMovement(bForward != FALSE);

	return 0;
}

int MotionParameters::GetMotorMaximumSpeed(int nAxisNo, double & dSpeed) const
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return -1;

	dSpeed = motorEngine[nAxisNo].getSpeedLimit();

	return 0;
}

void MotionParameters::WaitMotorSettlingTime(int nAxisNo) const
{
	if(nAxisNo < 0 || nAxisNo >= motorCount)
		return;

	Sleep(motorEngine[nAxisNo].getSettlingTime());
}

int MotionParameters::GetMotorName(int nMotorID, CString & strName) const
{
	if(nMotorID < 0 || nMotorID >= motorCount)
		return -1;

	strName = motorEngine[nMotorID].getName().c_str();

	return 0;
}

// Multiple motor axis interraction
int MotionParameters::GetTotalMotorCount() const
{
	return motorCount;
}

int MotionParameters::SetServoOnAllAxis()
{
	int nGlobalError = 0;

	for(int i = 0; i<motorCount; i++) {

		int nError = motorEngine[i].servoOn();
		if(nError)
			nGlobalError = nError;

		Sleep(100);
	}

	return nGlobalError;
}

int MotionParameters::SetServoOffAllAxis()
{
	int nGlobalError = 0;

	for(int i = 0; i<motorCount; i++) {

		int nError = motorEngine[i].servoOff();
		if(nError)
			nGlobalError = nError;
	}

	return nGlobalError;
}

int MotionParameters::StopEveryAxis()
{
	int nGlobalError = 0;

	for(int i = 0; i<motorCount; i++) {
		int nError = motorEngine[i].stop();
		if(nError)
			nGlobalError = nError;
	}

	return nGlobalError;
}

int MotionParameters::EmergencyStopEveryAxis()
{
	int nErrorTotal = 0;

	for(int i = 0; i<motorCount; i++) {
		int nError = motorEngine[i].emergencyStop();
		if(nError)
			nErrorTotal = nError;

	}

	return nErrorTotal;
}

int MotionParameters::ResetAlarmAllAxis()
{
	int nErrorGlobal = 0;

	for(int i = 0; i<motorCount; i++) {

		if(i == motorCount - 1)
			Sleep(1000);

		int nError = motorEngine[i].resetAlarm();
		if(nError)
			nErrorGlobal = nError;

		Sleep(100);
	}

	return nErrorGlobal;
}

int MotionParameters::HomeMoveEveryAxis(bool bSafety)
{
	int nErrorGlobal = 0;

	for(int i = 0; i<motorCount; i++) {
		int nError = HomeMove(i, bSafety);
		if(nError)
			nErrorGlobal = nError;
	}

	return nErrorGlobal;
}

int MotionParameters::WaitForAnyMotionError(std::vector < int > axisId, int nTimeOut)
{
	if(axisId.empty())
		return -1;

	std::vector < int > realAxis(axisId.size());

	for(size_t i = 0; i < axisId.size(); ++i) {

		if(axisId[i] < 0 || axisId[i] >= motorCount)
			return -1;

		realAxis[i] = motorEngine[i]._getAxisId();
	}

	return motorResource.WaitForAnyMotionError(axisId.size(), realAxis.data(), nTimeOut);
}

void MotionParameters::WaitForEveryMotionDone(std::vector < int > error, int nTimeOut)
{
	error.resize(motorCount);

	for(int i = 0; i<motorCount; i++)
		error[i] = WaitForMotionDone(i, nTimeOut);
}

int MotionParameters::moveToZSensorTeachPos(bool bSafety)
{
	int nError = StopAxis(X_SERVO_MOTOR);
	if (nError)
		return nError;

	nError = StopAxis(Y_SERVO_MOTOR);
	if (nError)
		return nError;

	nError = StopAxis(ST1_Z_SERVO_MOTOR);
	if(nError)
		return nError;

	if (!IsMotorHomed(X_SERVO_MOTOR)) {
		nError = HomeMove(X_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(Y_SERVO_MOTOR)) {
		nError = HomeMove(Y_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(ST1_Z_SERVO_MOTOR)) {
		nError = HomeMove(ST1_Z_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(X_SERVO_MOTOR)) {

		nError = WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(X_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(Y_SERVO_MOTOR)) {

		nError = WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(Y_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(ST1_Z_SERVO_MOTOR)) {

		nError = HomeMove(ST1_Z_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}
	int nDirectionX = 1;
	int nDirectionY = -1;
	
	int nMotorPositionX = (m_dDistanceZSensorToTop + m_dDistanceTopToTHR + 0.5)  * motorEngine[X_SERVO_MOTOR].getResolution() * nDirectionX + motorStartScanPos[X_SERVO_MOTOR];
	int nMotionPositonY = 0.5 * motorEngine[X_SERVO_MOTOR].getResolution() * nDirectionY + motorStartScanPos[Y_SERVO_MOTOR];

	nError = AbsoluteMove(X_SERVO_MOTOR, nMotorPositionX, getMovingSpeed(X_SERVO_MOTOR), false);
	if (nError)
		return nError;

	nError = AbsoluteMove(Y_SERVO_MOTOR, nMotionPositonY, getMovingSpeed(Y_SERVO_MOTOR), false);
	if (nError)
		return nError;

	nError = AbsoluteMove(ST1_Z_SERVO_MOTOR, motorStartScanPos[ST1_Z_SERVO_MOTOR], getMovingSpeed(ST1_Z_SERVO_MOTOR), false);
	if (nError)
		return nError;

	return 0;
}

int MotionParameters::moveToZSensorTeachPos(int nDie, bool bSafety)
{
	int nError = StopAxis(X_SERVO_MOTOR);
	if (nError)
		return nError;

	nError = StopAxis(Y_SERVO_MOTOR);
	if (nError)
		return nError;


	if (!IsMotorHomed(X_SERVO_MOTOR)) {
		nError = HomeMove(X_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(Y_SERVO_MOTOR)) {
		nError = HomeMove(Y_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}


	if (!IsMotorHomed(X_SERVO_MOTOR)) {

		nError = WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(X_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(Y_SERVO_MOTOR)) {

		nError = WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(Y_SERVO_MOTOR);
		if (nError)
			return nError;
	}


	nError = AbsoluteMove(X_SERVO_MOTOR, motorZSensorTeachPos[nDie][X_SERVO_MOTOR], getMovingSpeed(X_SERVO_MOTOR), false);
	if (nError)
		return nError;

	nError = AbsoluteMove(Y_SERVO_MOTOR, motorZSensorTeachPos[nDie][Y_SERVO_MOTOR], getMovingSpeed(Y_SERVO_MOTOR), false);
	if (nError)
		return nError;

	return 0;
}

void MotionParameters::SetZSensorTeachPos(int nDie)
{
	if (nDie < 0 || nDie >= MAX_DIES)
		return;

	scanningReady = false;

	motorEngine[X_SERVO_MOTOR].getCurrentPosition(motorZSensorTeachPos[nDie][X_SERVO_MOTOR]);
	motorEngine[Y_SERVO_MOTOR].getCurrentPosition(motorZSensorTeachPos[nDie][Y_SERVO_MOTOR]);
}

void MotionParameters::SetTOPZStartScanPos(int nFov)
{
	if (nFov >= 0 && nFov < MAX_FOV_TOP) {
		scanningReady = false;
		motorEngine[ST1_Z_SERVO_MOTOR].getCurrentPosition(motorTOPStartScanPos[nFov][ST1_Z_SERVO_MOTOR]);
	}
}

void MotionParameters::SetZStartScanPos()
{
	scanningReady = false;

	motorEngine[ST2_Z_SERVO_MOTOR].getCurrentPosition(motorStartScanPos[ST2_Z_SERVO_MOTOR]);

}

int MotionParameters::moveToTOPZStartScanPosPos(int nFov, bool bSafety)
{
	if (nFov < 0 || nFov >= MAX_FOV_TOP)
		return -1;

	int nError = StopAxis(ST1_Z_SERVO_MOTOR);
	if (nError)
		return nError;

	if (!IsMotorHomed(ST1_Z_SERVO_MOTOR)) {
		nError = HomeMove(ST1_Z_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(ST1_Z_SERVO_MOTOR)) {
		nError = WaitForMotionDone(ST1_Z_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(ST1_Z_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	nError = AbsoluteMove(ST1_Z_SERVO_MOTOR, motorTOPStartScanPos[nFov][ST1_Z_SERVO_MOTOR], getMovingSpeed(ST1_Z_SERVO_MOTOR), bSafety);
	if (nError)
		return nError;

	return 0;
}

int MotionParameters::moveToZStartScanPosPos(bool bSafety)
{
	
	int nError = StopAxis(ST2_Z_SERVO_MOTOR);
	if(nError)
		return nError;

	if(!IsMotorHomed(ST2_Z_SERVO_MOTOR)) {
		nError = HomeMove(ST2_Z_SERVO_MOTOR, false);
		if(nError)
			return nError;
	}

	if(!IsMotorHomed(ST2_Z_SERVO_MOTOR)) {
		nError = WaitForMotionDone(ST2_Z_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(ST2_Z_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	nError = AbsoluteMove(ST2_Z_SERVO_MOTOR, motorStartScanPos[ST2_Z_SERVO_MOTOR], getMovingSpeed(ST2_Z_SERVO_MOTOR), false);
	if(nError)
		return nError;

	return 0;
}

void MotionParameters::SetStartScanPos()
{
	scanningReady = false;

	motorEngine[X_SERVO_MOTOR].getCurrentPosition(motorStartScanPos[X_SERVO_MOTOR]);
	motorEngine[Y_SERVO_MOTOR].getCurrentPosition(motorStartScanPos[Y_SERVO_MOTOR]);
}

void MotionParameters::SetTOPStartScanPos(int nFov)
{
	scanningReady = false;

	motorEngine[X_SERVO_MOTOR].getCurrentPosition(motorTOPStartScanPos[nFov][X_SERVO_MOTOR]);
	motorEngine[Y_SERVO_MOTOR].getCurrentPosition(motorTOPStartScanPos[nFov][Y_SERVO_MOTOR]);
}

int MotionParameters::MoveToStartScanPos(bool bSafety)
{
	int nError = StopAxis(X_SERVO_MOTOR);
	if(nError)
		return nError;

	nError = StopAxis(Y_SERVO_MOTOR);
	if(nError)
		return nError;

	/*nError = StopAxis(ST1_Z_SERVO_MOTOR);
	if(nError)
	return nError;*/

	//if(CheckSafety(X_SERVO_MOTOR, bSafety) || CheckSafety(Y_SERVO_MOTOR, bSafety)/* || CheckSafety(ST1_Z_SERVO_MOTOR, bSafety)*/)
	//	return -1;

	if(!IsMotorHomed(X_SERVO_MOTOR)) {
		nError = HomeMove(X_SERVO_MOTOR, false);
		if(nError)
			return nError;
	}

	if(!IsMotorHomed(Y_SERVO_MOTOR)) {
		nError = HomeMove(Y_SERVO_MOTOR, false);
		if(nError)
			return nError;
	}

	if(!IsMotorHomed(X_SERVO_MOTOR)) {

		nError = WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
		if(nError)
			return nError;

		nError = HomeDone(X_SERVO_MOTOR);
		if(nError)
			return nError;
	}

	if(!IsMotorHomed(Y_SERVO_MOTOR)) {

		nError = WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
		if(nError)
			return nError;

		nError = HomeDone(Y_SERVO_MOTOR);
		if(nError)
			return nError;
	}

	nError = AbsoluteMove(X_SERVO_MOTOR, motorStartScanPos[X_SERVO_MOTOR], getMovingSpeed(X_SERVO_MOTOR), false);
	if(nError)
		return nError;

	nError = AbsoluteMove(Y_SERVO_MOTOR, motorStartScanPos[Y_SERVO_MOTOR], getMovingSpeed(Y_SERVO_MOTOR), false);
	if(nError)
		return nError;

	return 0;
}

int MotionParameters::MoveToTOPStartScanPos(int nFov, bool bSafety)
{
	if (nFov < 0 || nFov >= MAX_FOV_TOP)
		return -1;

	int nError = StopAxis(X_SERVO_MOTOR);
	if (nError)
		return nError;

	nError = StopAxis(Y_SERVO_MOTOR);
	if (nError)
		return nError;

	//if(CheckSafety(X_SERVO_MOTOR, bSafety) || CheckSafety(Y_SERVO_MOTOR, bSafety)/* || CheckSafety(ST1_Z_SERVO_MOTOR, bSafety)*/)
	//	return -1;

	if (!IsMotorHomed(X_SERVO_MOTOR)) {
		nError = HomeMove(X_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(Y_SERVO_MOTOR)) {
		nError = HomeMove(Y_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(X_SERVO_MOTOR)) {

		nError = WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(X_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(Y_SERVO_MOTOR)) {

		nError = WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(Y_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	nError = AbsoluteMove(X_SERVO_MOTOR, motorTOPStartScanPos[nFov][X_SERVO_MOTOR], getMovingSpeed(X_SERVO_MOTOR), false);
	if (nError)
		return nError;

	nError = AbsoluteMove(Y_SERVO_MOTOR, motorTOPStartScanPos[nFov][Y_SERVO_MOTOR], getMovingSpeed(Y_SERVO_MOTOR), false);
	if (nError)
		return nError;

	return 0;
}


void MotionParameters::SetReadyScanPos()
{
	scanningReady = false;
	for (int i = 0; i< motorCount; i++)
		motorEngine[i].getCurrentPosition(motorInitScanPos[i]);

}

int MotionParameters::moveToReadyScanPos(bool bSafety)
{
	int nError = StopAxis(X_SERVO_MOTOR);
	if (nError)
		return nError;

	nError = StopAxis(Y_SERVO_MOTOR);
	if (nError)
		return nError;

	nError = StopAxis(ST1_Z_SERVO_MOTOR);
	if (nError)
		return nError;

	nError = StopAxis(ST2_Z_SERVO_MOTOR);
	if (nError)
		return nError;

	if (!IsMotorHomed(X_SERVO_MOTOR)) {
		nError = HomeMove(X_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(Y_SERVO_MOTOR)) {
		nError = HomeMove(Y_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(ST1_Z_SERVO_MOTOR)) {
		nError = HomeMove(ST1_Z_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(ST2_Z_SERVO_MOTOR)) {
		nError = HomeMove(ST2_Z_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(X_SERVO_MOTOR)) {

		nError = WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(X_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(Y_SERVO_MOTOR)) {

		nError = WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(Y_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(ST1_Z_SERVO_MOTOR)) {

		nError = WaitForMotionDone(ST1_Z_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(ST1_Z_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(ST2_Z_SERVO_MOTOR)) {

		nError = WaitForMotionDone(ST2_Z_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(ST2_Z_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	nError = AbsoluteMove(X_SERVO_MOTOR, motorInitScanPos[X_SERVO_MOTOR], getMovingSpeed(X_SERVO_MOTOR), false);
	if (nError)
		return nError;

	nError = AbsoluteMove(Y_SERVO_MOTOR, motorInitScanPos[Y_SERVO_MOTOR], getMovingSpeed(Y_SERVO_MOTOR), false);
	if (nError)
		return nError;

	nError = AbsoluteMove(ST1_Z_SERVO_MOTOR, motorInitScanPos[ST1_Z_SERVO_MOTOR], getMovingSpeed(ST1_Z_SERVO_MOTOR), false);
	if (nError)
		return nError;

	nError = AbsoluteMove(ST2_Z_SERVO_MOTOR, motorInitScanPos[ST2_Z_SERVO_MOTOR], getMovingSpeed(ST2_Z_SERVO_MOTOR), false);
	if (nError)
		return nError;

	return 0;
}

void MotionParameters::SetNestReplacementPos()
{
	scanningReady = false;
	for (int i = 0; i< motorCount; i++)
		motorEngine[i].getCurrentPosition(motorModuleUnloadPos[i]);
}

int MotionParameters::moveToNestReplacementPos(bool bSafety)
{
	int nError = StopAxis(X_SERVO_MOTOR);
	if (nError)
		return nError;

	nError = StopAxis(Y_SERVO_MOTOR);
	if (nError)
		return nError;

	nError = StopAxis(ST1_Z_SERVO_MOTOR);
	if (nError)
		return nError;

	if (!IsMotorHomed(X_SERVO_MOTOR)) {
		nError = HomeMove(X_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(Y_SERVO_MOTOR)) {
		nError = HomeMove(Y_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(ST1_Z_SERVO_MOTOR)) {
		nError = HomeMove(ST1_Z_SERVO_MOTOR, false);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(X_SERVO_MOTOR)) {

		nError = WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(X_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(Y_SERVO_MOTOR)) {

		nError = WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(Y_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	if (!IsMotorHomed(ST1_Z_SERVO_MOTOR)) {

		nError = WaitForMotionDone(ST1_Z_SERVO_MOTOR, theApp.getMotionTimeOut());
		if (nError)
			return nError;

		nError = HomeDone(ST1_Z_SERVO_MOTOR);
		if (nError)
			return nError;
	}

	nError = AbsoluteMove(X_SERVO_MOTOR, motorModuleUnloadPos[X_SERVO_MOTOR], getMovingSpeed(X_SERVO_MOTOR), false);
	if (nError)
		return nError;

	nError = AbsoluteMove(Y_SERVO_MOTOR, motorModuleUnloadPos[Y_SERVO_MOTOR], getMovingSpeed(Y_SERVO_MOTOR), false);
	if (nError)
		return nError;

	nError = AbsoluteMove(ST1_Z_SERVO_MOTOR, motorModuleUnloadPos[ST1_Z_SERVO_MOTOR], getMovingSpeed(ST1_Z_SERVO_MOTOR), false);
	if (nError)
		return nError;
	return 0;
}


BOOL MotionParameters::IsMachineHomed() const
{
	bool bHomedOn = true;

	for(int i = 0; i<motorCount; i++)
		bHomedOn = bHomedOn & motorEngine[i].isHomed();

	for (int i = 0; i < scaleCount; i++)
		bHomedOn = bHomedOn & scaleEngine[i].isHomed();

	return bHomedOn;
}


void MotionParameters::SetZSensortoTopDistance(double dDist)
{
	m_dDistanceZSensorToTop = dDist;
}

double MotionParameters::GetZSensortoTopDistance()
{

	return m_dDistanceZSensorToTop;
}

void MotionParameters::SetTopToTHRDistance(double dDist)
{
	m_dDistanceTopToTHR = dDist;
}

double MotionParameters::GetTopToTHRDistance()
{
	return m_dDistanceTopToTHR;
}

int MotionParameters::SetAlarm()
{
	return motorResource.SetAlarm();
}

// Scale interraction
int MotionParameters::GetTotalScaleCount() const
{
	return scaleCount;
}

int MotionParameters::GetCurrentScalePosition(int nAxisNo, int & nPos)
{
	if (nAxisNo < 0 || nAxisNo >= scaleCount)
		return -1;

	return scaleEngine[nAxisNo].getCurrentPosition(nPos);
}


int MotionParameters::ResetScaleTriggerCount(int nAxisNo)
{
	int error = 0;

	EnterCriticalSection(&CriticalSectionCommands);

	error = scaleEngine[nAxisNo].resetTriggerCount();
		
	LeaveCriticalSection(&CriticalSectionCommands);

	return error;
}

int MotionParameters::SetScaleCompareTrigger(int nScanningRow)
{
	if (!scanningReady || nScanningRow < 0 || nScanningRow >= nScanRowNoTop || _cameraTriggerTOP.empty())
		return -1;

	int error = 0;

	EnterCriticalSection(&CriticalSectionCommands);

	error = scaleEngine[X_SCALE_1].setTriggerTable(&_cameraTriggerTOP.data()[nScanColumnNoTop * nScanningRow], triggerCountPerRowTOP[nScanningRow]);

	LeaveCriticalSection(&CriticalSectionCommands);

	return error;
}

int MotionParameters::SetScaleCompareTriggerTHR(int nScanningRow)
{
	if (!scanningReady || nScanningRow < 0 || nScanningRow >= nScanRowNoTHR || _cameraTriggerTHR.empty())
		return -1;

	int error = 0;

	EnterCriticalSection(&CriticalSectionCommands);

	error = scaleEngine[X_SCALE_3].setTriggerTable(&_cameraTriggerTHR.data()[nScanColumnNoTHR * nScanningRow], triggerCountPerRowTHR[nScanningRow]);

	LeaveCriticalSection(&CriticalSectionCommands);

	return error;
}

int MotionParameters::scaleManualTrigger(int axisId)
{
	if (axisId < 0 || axisId >= scaleCount)
		return -1;

	return scaleEngine[axisId].manualTrigger();
}

int MotionParameters::ResetTriggerCount(int nTriggerSource)
{
	int error = 0;

	EnterCriticalSection(&CriticalSectionCommands);

	error = theApp.m_MotorResource.ResetTriggerCount(theApp.m_nMotorCardNo, 0, nTriggerSource);
	
	LeaveCriticalSection(&CriticalSectionCommands);

	return error;
}


int MotionParameters::SetCompareTriggerTHR(int nScanningRow)
{
	if(!scanningReady ||   nScanningRow < 0 || nScanningRow >= nScanRowNoTHR || _cameraTriggerTHR.empty())
		return -1;

	int error = 0;

	EnterCriticalSection(&CriticalSectionCommands);

	error = theApp.m_MotorResource.SetFifoArray(0, 0, 0, &_cameraTriggerTHR.data()[nScanColumnNoTHR * nScanningRow], triggerCountPerRowTHR[nScanningRow]);

	LeaveCriticalSection(&CriticalSectionCommands);

	return error;
}

int MotionParameters::SetCompareTriggerTOP(int nScanningRow)
{
	if(!scanningReady ||   nScanningRow < 0 || nScanningRow >= nScanRowNoTop || _cameraTriggerTOP.empty())
		return -1;

	int error = 0;

	EnterCriticalSection(&CriticalSectionCommands);

	error = theApp.m_MotorResource.SetFifoArray(0, 0, 0, &_cameraTriggerTOP.data()[nScanColumnNoTop * nScanningRow], triggerCountPerRowTOP[nScanningRow]);

	LeaveCriticalSection(&CriticalSectionCommands);

	return error;
}

int MotionParameters::SetCompareTriggerTopEncap(int nScanningRow)
{
	if (!scanningReady || nScanningRow < 0 || nScanningRow >= nScanRowNoTop || _cameraTriggerTopEncap.empty())
		return -1;

	int error = 0;

	EnterCriticalSection(&CriticalSectionCommands);

	error = theApp.m_MotorResource.SetFifoArray(0, 0, 0, &_cameraTriggerTopEncap.data()[nScanColumnNoTop * nScanningRow], triggerCountPerRowTOP[nScanningRow]);

	LeaveCriticalSection(&CriticalSectionCommands);

	return error;
}

int MotionParameters::SetCompareTriggerZSensor(int nScanningRow)
{
	if(!scanningReady || nScanningRow < 0 || nScanningRow >= nScanRowNoZSensor || _zSensorTriggerStartPos.empty())
		return -1;

	int error = 0;

	EnterCriticalSection(&CriticalSectionCommands);

	error = theApp.m_MotorResource.SetFifoArray(0, 0, 3, &_zSensorTriggerStartPos.data()[nScanColumnNoZSensor * nScanningRow], _zSensortriggerCountPerRow[nScanningRow]);

	LeaveCriticalSection(&CriticalSectionCommands);

	return error;
}

int MotionParameters::SetScaleCompareTriggerZSensor(int nScanningRow)
{
	if (!scanningReady || nScanningRow < 0 || nScanningRow >= nScanRowNoZSensor || _zSensorTriggerStartPos.empty())
		return -1;

	int error = 0;

	EnterCriticalSection(&CriticalSectionCommands);

	error = scaleEngine[X_SCALE_2].setTriggerTable(&_zSensorTriggerStartPos.data()[nScanColumnNoZSensor * nScanningRow], _zSensortriggerCountPerRow[nScanningRow]);

	LeaveCriticalSection(&CriticalSectionCommands);

	return error;
}

int MotionParameters::SetCompareTriggerTopnZ(int nScanningRow)
{
	if(!scanningReady || nScanningRow < 0 || nScanningRow >= nScanRowNoZSensor || _zSensorTopTriggerPoints.empty())
		return -1;

	int error = 0;

	EnterCriticalSection(&CriticalSectionCommands);

	error = theApp.m_MotorResource.SetFifoArray(0, 0, 0, &_zSensorTopTriggerPoints.data()[_zTopTriggerCountPerRow[nScanningRow] * nScanningRow], _zTopTriggerCountPerRow[nScanningRow]);
	
	LeaveCriticalSection(&CriticalSectionCommands);

	return error;
}

int MotionParameters::GetZTriggerCountNoPerRow(int nScanningRow)
{
	if(!scanningReady || nScanningRow < 0 || nScanningRow >= nScanRowNoZSensor || _zSensorTopTriggerPoints.empty())
		return -1;

	return _zTopTriggerCountPerRow[nScanningRow];

}

int MotionParameters::GetLatchValue(int nAxis, long & nPosValue, long nInitialLatchData)
{
	if(!scanningReady) 
		return -1;
	EnterCriticalSection(&CriticalSectionCommands);

	int nError = 0;

//	motorEngine[nAxis]._getAxisId();
	nError = theApp.m_MotorResource.GetLatchValue(motorEngine[nAxis]._getAxisId(), &nPosValue, nInitialLatchData);

	LeaveCriticalSection(&CriticalSectionCommands);

	return nError;

}

int MotionParameters::GetInitialLatchXValue(int nValueID, int & nInitValue)
{
	if(!scanningReady || nValueID < 0 || nValueID >= nScanColumnNoTHR * nScanRowNoTHR)
		return -1;

	nInitValue = _zSensorTopTriggerPoints[nValueID];

	return 0;
}

//int MotionParameters::GetInitialLatchYValue(int nValueID, int & nInitValue)
//{
//	if(!scanningReady || nValueID < 0 || nValueID >= nScanColumnNoTHR * nScanRowNoTHR)
//		return -1;
//
////	nInitValue = motorYScanning23DPosition[nValueID / nScanColumnNo];
//
//	return 0;
//}

int MotionParameters::GetScaleResolution(int nAxisNo, double & dResolution) const
{
	if (nAxisNo < 0 || nAxisNo >= scaleCount)
		return -1;

	dResolution = scaleEngine[nAxisNo].getResolution();

	return 0;
}

int MotionParameters::SetScaleResolution(int nAxisNo, double dResolution)
{
	if (nAxisNo < 0 || nAxisNo >= scaleCount)
		return -1;

	scaleEngine[nAxisNo].setResolution(dResolution);


	return 0;
}

int MotionParameters::GetScaleName(int nAxisNo, CString & strName) const
{
	if (nAxisNo < 0 || nAxisNo >= scaleCount)
		return -1;

	strName = scaleEngine[nAxisNo].getName().c_str();

	return 0;
}


void MotionParameters::getDeviceSize(double & dWidthMM, double & dHeightMM) const
{
	dWidthMM = dDeviceSizeInXMM;
	dHeightMM = dDeviceSizeInYMM;
}

void MotionParameters::setDeviceSize(double dWidthMM, double dHeightMM)
{
	dDeviceSizeInXMM = dWidthMM;
	dDeviceSizeInYMM = dHeightMM;
}

void MotionParameters::GetMappingParameters(int & nBlockX, int & nBlockY, double & dBlockPitchX, double & dBlockPitchY, int & nDieX, int & nDieY,
	double & dDiePitchX, double & dDiePitchY, int & nDieFOVX, int & nDieFOVY, double & dInitialOffset) const
{
	nBlockX = m_nNoOfBlock[0];
	nBlockY = m_nNoOfBlock[1];
	dBlockPitchX = m_dBlockPitch[0];
	dBlockPitchY = m_dBlockPitch[1];
	nDieX = m_nDeviceNoPerBlock[0];
	nDieY = m_nDeviceNoPerBlock[1];
	dDiePitchX = m_dDevicePitch[0];
	dDiePitchY = m_dDevicePitch[1];
	nDieFOVX = nDieColumn;
	nDieFOVY = nDieRow;
	dInitialOffset = m_dInitialOffset;
}

void MotionParameters::GetDatumOffsetParameters(double &dTopRowoffsetX, double &dBottomRowOffsetX, double & dTopRowoffsetY, double & dBottomRowOffsetY)
{
	dTopRowoffsetX = m_dTopRowOffset[0];
	dBottomRowOffsetX = m_dBottomRowOffset[0];
	dTopRowoffsetY = m_dTopRowOffset[1];
	dBottomRowOffsetY = m_dBottomRowOffset[1];
	
}

int MotionParameters::SetSubstrateSelected(int nColumn, int nRow)
{
	m_nSubstrateDieColumn = nColumn;
	m_nSubstrateDieRow = nRow;
	return 0;
}

int MotionParameters::GetSubstrateSelected(int & nColumn, int & nRow)
{
	nColumn = m_nSubstrateDieColumn;
	nRow = m_nSubstrateDieRow;

	return 0;
}

void MotionParameters::GetMappingParameters(int & nBlockX, int & nBlockY, int & nDieX, int & nDieY) const
{
	nBlockX = m_nNoOfBlock[0];
	nBlockY = m_nNoOfBlock[1];
	nDieX = m_nDeviceNoPerBlock[0];
	nDieY = m_nDeviceNoPerBlock[1];
}

void MotionParameters::GetMappingParameters(int & nDieFOVX, int & nDieFOVY) const
{
	nDieFOVX = m_nDeviceNoPerBlock[0];
	nDieFOVY = m_nDeviceNoPerBlock[1];
}

void MotionParameters::GetMappingParametersTOP(int & nDieTOPX, int & nDieTOPY) const
{
	nDieTOPX = m_nDeviceNoPerBlockTOP[0];
	nDieTOPY = m_nDeviceNoPerBlockTOP[1];
}


int MotionParameters::SetMappingParametersTOP(int nDieTOPX, int nDieTOPY)
{
	if (nDieTOPX < 1 || nDieTOPY < 1)
		return -1;

	m_nDeviceNoPerBlockTOP[0] = nDieTOPX;
	m_nDeviceNoPerBlockTOP[1] = nDieTOPY;

	return 0;
}


int MotionParameters::SetMappingParameters(int nBlockX, int nBlockY, double dBlockPitchX, double dBlockPitchY, int nDieX, int nDieY, double dDiePitchX, double dDiePitchY, int nDieFOVX, int nDieFOVY, double dInitialOffset)
{
	if(nBlockX < 1 || nBlockY < 1 || dBlockPitchX < 0 || dBlockPitchY < 0 || nDieX < 1 || nDieY < 1 || dDiePitchX < 0 || dDiePitchY < 0 || nDieFOVX < 1 || nDieFOVY < 1 ||
		nDieFOVX > nDieX || nDieFOVY > nDieY /* ||((nDieX - 1) * dDiePitchX >= dBlockPitchX && nBlockX > 1) ||*/ /*((nDieY - 1) * dDiePitchY >= dBlockPitchY && nBlockY > 1)*/)
		return -1;

	m_nNoOfBlock[0] = nBlockX;
	m_nNoOfBlock[1] = nBlockY;
	m_dBlockPitch[0] = dBlockPitchX;
	m_dBlockPitch[1] = dBlockPitchY;
	m_nDeviceNoPerBlock[0] = nDieX;
	m_nDeviceNoPerBlock[1] = nDieY;
	m_dDevicePitch[0] = dDiePitchX;
	m_dDevicePitch[1] = dDiePitchY;
	nDieColumn = nDieFOVX;
	nDieRow = nDieFOVY;
	m_dInitialOffset = dInitialOffset;

	return 0;
}

int MotionParameters::SetDatumOffsetParameters(double dTopRowoffsetX, double dBottomRowOffsetX, double dTopRowoffsetY, double dBottomRowOffsetY)
{
	m_dTopRowOffset[0] = dTopRowoffsetX;
	m_dBottomRowOffset[0] = dBottomRowOffsetX;
	m_dTopRowOffset[1] = dTopRowoffsetY;
	m_dBottomRowOffset[1] = dBottomRowOffsetY;
	return 0;
}

// Merging interraction for 2-3D station
int MotionParameters::GetTotalImageGrabsTHR() const
{
	return nTotalImageFOVTHR;
}

int MotionParameters::GetTotalImageGrabsTOP() const
{
	return nTotalImageFOVTOP;
}

void MotionParameters::LoadZTeachPosition()
{
	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp;

	int nInt;

	strFile.Format("%s\\%s\\%s\\Triangular Sensor.info", pApp->m_strMainCfgDir, pApp->m_strMainCfgModeDir, pApp->m_strConfigFile);
	iniFile.SetPath(strFile);

	
	if(iniFile.ReadFile()) {
		strKey.Format("Z Sensor Data");
		m_dZSensorTeachValue = iniFile.GetDouble(strKey, "Triangular Sensor Teach Position (mm)", m_dZSensorTeachValue);
		for (int nDie = 0; nDie < MAX_DIES; nDie++) {
			CString strTempKey;
			strTempKey.Format("Triangular Sensor Teach Position Die %d (mm)", nDie);
			m_dZSensorDieTeachValue[nDie] = iniFile.GetDouble(strKey, strTempKey, m_dZSensorDieTeachValue[nDie]);
		}

		strKey.Format("Z Sensor Scan Position");
		for (int nDie = 0; nDie < MAX_DIES; nDie++) {
			for (int i = 0; i < motorCount; i++) {

				if (i != X_SERVO_MOTOR && i != Y_SERVO_MOTOR && i != ST1_Z_SERVO_MOTOR)
					continue;

				CString strTemp;
				strTemp.Format("%s Z Sensor Teach Position Die %d (pps)", motorEngine[i].getName().c_str(), nDie + 1);
				motorZSensorTeachPos[nDie][i] = iniFile.GetInteger(strKey, strTemp, motorZSensorTeachPos[nDie][i]);
			}
		}
	}
}

void MotionParameters::SaveZTeachPosition() const
{
	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp;

	strFile.Format("%s\\%s\\%s\\Triangular Sensor.info", pApp->m_strMainCfgDir, pApp->m_strMainCfgModeDir, pApp->m_strConfigFile);
	iniFile.SetPath(strFile);

	strKey.Format("Z Sensor Data");
	iniFile.SetDouble(strKey, "Triangular Sensor Teach Position (mm)", m_dZSensorTeachValue);
	for (int nDie = 0; nDie < MAX_DIES; nDie++) {
		CString strTempKey;
		strTempKey.Format("Triangular Sensor Teach Position Die %d (mm)", nDie);
		iniFile.SetDouble(strKey, strTempKey, m_dZSensorDieTeachValue[nDie]);
	}

	strKey.Format("Z Sensor Scan Position");
	for (int nDie = 0; nDie < MAX_DIES; nDie++) {
		for (int i = 0; i < motorCount; i++) {

			if (i != X_SERVO_MOTOR && i != Y_SERVO_MOTOR && i != ST1_Z_SERVO_MOTOR)
				continue;

			CString strTemp;
			strTemp.Format("%s Z Sensor Teach Position Die %d (pps)", motorEngine[i].getName().c_str(), nDie + 1);
			iniFile.SetInteger(strKey, strTemp, motorZSensorTeachPos[nDie][i]);
		}
	}

	iniFile.WriteFile();
}

void MotionParameters::SaveScanningOptions()
{
	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp;

	strFile.Format("%s\\%s\\%s\\Scanning Options.info", pApp->m_strMainCfgDir, pApp->m_strMainCfgModeDir, pApp->m_strConfigFile);
	iniFile.SetPath(strFile);

	strKey.Format("Scanning Options");

	iniFile.SetBoolean(strKey, "Enable Barcode Read Status", m_bEnablebarcodeReader);
	iniFile.SetBoolean(strKey, "Enable Barcode Read Status without Cylinder", m_bEnablebarcodeReaderWithCylinder);
	
	iniFile.SetBoolean(strKey, "Enable Ionizer", m_bEnableIonizer);
	iniFile.SetBoolean(strKey, "Enable Top Scanning", m_bEnableTopScanning);
	iniFile.SetBoolean(strKey, "Enable THR Scanning", m_bEnableTHRScanning);
	
	iniFile.WriteFile();
}

void MotionParameters::loadScanningOptions()
{
	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp;

	int nInt;

	strFile.Format("%s\\%s\\%s\\Scanning Options.info", pApp->m_strMainCfgDir, pApp->m_strMainCfgModeDir, pApp->m_strConfigFile);
	iniFile.SetPath(strFile);

	strKey.Format("Scanning Options");

	if (iniFile.ReadFile()) {
		m_bEnablebarcodeReader = iniFile.GetBoolean(strKey, "Enable Barcode Read Status", m_bEnablebarcodeReader);
		m_bEnablebarcodeReaderWithCylinder = iniFile.GetBoolean(strKey, "Enable Barcode Read Status without Cylinder", m_bEnablebarcodeReaderWithCylinder);
		
		m_bEnableIonizer = iniFile.GetBoolean(strKey, "Enable Ionizer", m_bEnableIonizer);
		m_bEnableTopScanning = iniFile.GetBoolean(strKey, "Enable Top Scanning", m_bEnableTopScanning);
		m_bEnableTHRScanning = iniFile.GetBoolean(strKey, "Enable THR Scanning", m_bEnableTHRScanning);
		
	}
}

void MotionParameters::SaveScanningParm(bool saveMappingData) const
{
	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp;

	strFile.Format("%s\\%s\\%s\\Scanning Parameters.info", pApp->m_strMainCfgDir, pApp->m_strMainCfgModeDir, pApp->m_strConfigFile);
	iniFile.SetPath(strFile);

	strKey.Format("Scanning Parameters");

	iniFile.SetInteger(strKey, "Trigger Inverse Pulse", nTriggerInversePulse);
	iniFile.SetInteger(strKey, "Trigger Pulse Width (us)", nTriggerPulseWidth);

	std::string str;

	for(int i = 0; i < motorCount; i++) {
		Speed ScanSpeed = getScanningSpeed(i);
		str = motorEngine[i].getName() + " Start Scanning Speed (pps/sec)";
		iniFile.SetInteger(strKey, str.c_str(), ScanSpeed.startSpeed);

		str = motorEngine[i].getName() + " Maximum Scanning Speed (pps/sec)";
		iniFile.SetInteger(strKey, str.c_str(), ScanSpeed.maximumSpeed);

		ScanSpeed = getScanningSpeedTHR(i);
		str = motorEngine[i].getName() + " Start Scanning Speed THR (pps/sec)";
		iniFile.SetInteger(strKey, str.c_str(), ScanSpeed.startSpeed);

		str = motorEngine[i].getName() + " Maximum Scanning Speed THR (pps/sec)";
		iniFile.SetInteger(strKey, str.c_str(), ScanSpeed.maximumSpeed);
	}

	for(int i = 0; i < motorCount; i++) {
		Speed ScanSpeed = getMovingSpeed(i);
		str = motorEngine[i].getName() + " Start Moving Speed (pps/sec)";
		iniFile.SetInteger(strKey, str.c_str(), ScanSpeed.startSpeed);

		str = motorEngine[i].getName() + " Maximum Moving Speed (pps/sec)";
		iniFile.SetInteger(strKey, str.c_str(), ScanSpeed.maximumSpeed);
	}

	/* THR Start Scan Position */
	strKey.Format("THR Start Scan Position");
	// X-Motor
	str = motorEngine[X_SERVO_MOTOR].getName() + " Start Scanning Position (pps)";
	iniFile.SetInteger(strKey, str.c_str(), motorStartScanPos[X_SERVO_MOTOR]);

	// Y-Motor
	str = motorEngine[Y_SERVO_MOTOR].getName() + " Start Scanning Position (pps)";
	iniFile.SetInteger(strKey, str.c_str(), motorStartScanPos[Y_SERVO_MOTOR]);

	// ST1-Z-Motor
	str = motorEngine[ST1_Z_SERVO_MOTOR].getName() + " Ready Position (pps)";
	iniFile.SetInteger(strKey, str.c_str(), motorStartScanPos[ST1_Z_SERVO_MOTOR]);
	// ST2-Z-Motor
	str = motorEngine[ST2_Z_SERVO_MOTOR].getName() + " Ready Position (pps)";
	iniFile.SetInteger(strKey, str.c_str(), motorStartScanPos[ST2_Z_SERVO_MOTOR]);

	/*TOP Start Scan Position*/
	strKey.Format("TOP Start Scan Position");
	for (int nFov = 0; nFov < MAX_FOV_TOP; nFov++) {
		CString strTemp;

		// X-Motor
		strTemp.Format("%s TOP Ready Position FOV %d (pps)", motorEngine[X_SERVO_MOTOR].getName().c_str(), nFov + 1);
		iniFile.SetInteger(strKey, strTemp, motorTOPStartScanPos[nFov][X_SERVO_MOTOR]);
		// Y-Motor
		strTemp.Format("%s TOP Ready Position FOV %d (pps)", motorEngine[Y_SERVO_MOTOR].getName().c_str(), nFov + 1);
		iniFile.SetInteger(strKey, strTemp, motorTOPStartScanPos[nFov][Y_SERVO_MOTOR]);
		// Z-Motor
		strTemp.Format("%s TOP Ready Position FOV %d (pps)", motorEngine[ST1_Z_SERVO_MOTOR].getName().c_str(), nFov + 1);
		iniFile.SetInteger(strKey, strTemp, motorTOPStartScanPos[nFov][ST1_Z_SERVO_MOTOR]);
	}


	iniFile.WriteFile();
}

void MotionParameters::LoadScanningParm()
{
	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp;

	int nInt;
	
	strFile.Format("%s\\%s\\%s\\Scanning Parameters.info", pApp->m_strMainCfgDir, pApp->m_strMainCfgModeDir, pApp->m_strConfigFile);
	iniFile.SetPath(strFile);

	strKey.Format("Scanning Parameters");

	if(iniFile.ReadFile()) {
		nTriggerInversePulse = iniFile.GetInteger(strKey, "Trigger Inverse Pulse", nTriggerInversePulse);
		nTriggerPulseWidth = iniFile.GetInteger(strKey, "Trigger Pulse Width (us)", nTriggerPulseWidth);

		std::string str;

		for(int i = 0; i < motorCount; i++) {
			Speed ScanSpeed;
			str = motorEngine[i].getName() + " Start Scanning Speed (pps/sec)";
			ScanSpeed.startSpeed = iniFile.GetInteger(strKey, str.c_str(), ScanSpeed.startSpeed);

			str = motorEngine[i].getName() + " Maximum Scanning Speed (pps/sec)";
			ScanSpeed.maximumSpeed = iniFile.GetInteger(strKey, str.c_str(), ScanSpeed.maximumSpeed);

			setScanningSpeed(i, ScanSpeed);

			str = motorEngine[i].getName() + " Start Scanning Speed THR (pps/sec)";
			ScanSpeed.startSpeed = iniFile.GetInteger(strKey, str.c_str(), ScanSpeed.startSpeed);

			str = motorEngine[i].getName() + " Maximum Scanning Speed THR (pps/sec)";
			ScanSpeed.maximumSpeed = iniFile.GetInteger(strKey, str.c_str(), ScanSpeed.maximumSpeed);

			setScanningSpeedTHR(i, ScanSpeed);
		}

		for(int i = 0; i < motorCount; i++) {
			Speed ScanSpeed;
			str = motorEngine[i].getName() + " Start Moving Speed (pps/sec)";
			ScanSpeed.startSpeed = iniFile.GetInteger(strKey, str.c_str(), ScanSpeed.startSpeed);

			str = motorEngine[i].getName() + " Maximum Moving Speed (pps/sec)";
			ScanSpeed.maximumSpeed = iniFile.GetInteger(strKey, str.c_str(), ScanSpeed.maximumSpeed);

			setMovingSpeed(i, ScanSpeed);
		}

		/* THR Start Scan Position */
		strKey.Format("THR Start Scan Position");
		//X-Motor
		str = motorEngine[X_SERVO_MOTOR].getName() + " Start Scanning Position (pps)";
		motorStartScanPos[X_SERVO_MOTOR] = iniFile.GetInteger(strKey, str.c_str(), motorStartScanPos[X_SERVO_MOTOR]);
		//Y-Motor
		str = motorEngine[Y_SERVO_MOTOR].getName() + " Start Scanning Position (pps)";
		motorStartScanPos[Y_SERVO_MOTOR] = iniFile.GetInteger(strKey, str.c_str(), motorStartScanPos[Y_SERVO_MOTOR]);
		//ST1-Z-Motor
		str = motorEngine[ST1_Z_SERVO_MOTOR].getName() + " Ready Position (pps)";
		motorStartScanPos[ST1_Z_SERVO_MOTOR] = iniFile.GetInteger(strKey, str.c_str(), motorStartScanPos[ST1_Z_SERVO_MOTOR]);
		//ST2-Z-Motor
		str = motorEngine[ST2_Z_SERVO_MOTOR].getName() + " Ready Position (pps)";
		motorStartScanPos[ST2_Z_SERVO_MOTOR] = iniFile.GetInteger(strKey, str.c_str(), motorStartScanPos[ST2_Z_SERVO_MOTOR]);

		/* TOP Start Scan Position */
		strKey.Format("TOP Start Scan Position");
		for (int nFov = 0; nFov < MAX_FOV_TOP; nFov++) {
			CString strTemp;

			//X-Motor
			strTemp.Format("%s TOP Ready Position FOV %d (pps)", motorEngine[X_SERVO_MOTOR].getName().c_str(), nFov + 1);
			motorTOPStartScanPos[nFov][X_SERVO_MOTOR] = iniFile.GetInteger(strKey, strTemp, motorTOPStartScanPos[nFov][X_SERVO_MOTOR]);
			//Y-Motor
			strTemp.Format("%s TOP Ready Position FOV %d (pps)", motorEngine[Y_SERVO_MOTOR].getName().c_str(), nFov + 1);
			motorTOPStartScanPos[nFov][Y_SERVO_MOTOR] = iniFile.GetInteger(strKey, strTemp, motorTOPStartScanPos[nFov][Y_SERVO_MOTOR]);
			//Z-Motor
			strTemp.Format("%s TOP Ready Position FOV %d (pps)", motorEngine[ST1_Z_SERVO_MOTOR].getName().c_str(), nFov + 1);
			motorTOPStartScanPos[nFov][ST1_Z_SERVO_MOTOR] = iniFile.GetInteger(strKey, strTemp, motorTOPStartScanPos[nFov][ST1_Z_SERVO_MOTOR]);
		}
	}
}

void MotionParameters::SaveDeviceParameters() const
{

	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp;

	//strFile.Format("%s\\DeviceConfiguration.dat", pApp->m_strConfigDir);
	strFile.Format("%s\\%s\\%s\\DeviceConfiguration.dat", pApp->m_strMainCfgDir, pApp->m_strMainCfgModeDir, pApp->m_strConfigFile);
	iniFile.SetPath(strFile);
	strKey.Format("Device Config");
	for(int b = 0; b<MAX_DIRECTION; b++) {
		strTemp.Format("No. Of Block[%c]", !b ? 'X' : 'Y');
		iniFile.SetInteger(strKey, strTemp, m_nNoOfBlock[b]);
		strTemp.Format("Block Pitch[%c]", !b ? 'X' : 'Y');
		iniFile.SetDouble(strKey, strTemp, m_dBlockPitch[b]);
		strTemp.Format("Device No. Per Block[%c]", !b ? 'X' : 'Y');
		iniFile.SetInteger(strKey, strTemp, m_nDeviceNoPerBlock[b]);
		strTemp.Format("Device Pitch[%c]", !b ? 'X' : 'Y');
		iniFile.SetDouble(strKey, strTemp, m_dDevicePitch[b]);
		strTemp.Format("Device Scan Area[%c]", !b ? 'X' : 'Y');
		iniFile.SetDouble(strKey, strTemp, m_dDeviceScanArea[b]);
		///strTemp.Format("Device Offset[%c]", !b ? 'X' : 'Y');
		//iniFile.SetDouble(strKey, strTemp, m_dDeviceOffset[b]);
		strTemp.Format("Device Overlap[%c]", !b ? 'X' : 'Y');
		iniFile.SetDouble(strKey, strTemp, m_dDeviceOverlap[b]);
		iniFile.SetDouble(strKey, "Device Initial Position", m_dInitialOffset);
		strTemp.Format("Datum Top Row Offset[%c]", !b ? 'X' : 'Y');
		iniFile.SetDouble(strKey, strTemp, m_dTopRowOffset[b]);
		strTemp.Format("Datum Bottom Row Offset[%c]", !b ? 'X' : 'Y');
		iniFile.SetDouble(strKey, strTemp, m_dBottomRowOffset[b]);

		strTemp.Format("Device No. Per Block[%c] TOP STATION", !b ? 'X' : 'Y');
		iniFile.SetInteger(strKey, strTemp, m_nDeviceNoPerBlockTOP[b]);
	}
	iniFile.WriteFile();
}


void MotionParameters::LoadDeviceParameters()
{
	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp;

	////strFile.Format("%s\\DeviceConfiguration.dat", pApp->m_strConfigDir);
	strFile.Format("%s\\%s\\%s\\DeviceConfiguration.dat", pApp->m_strMainCfgDir, pApp->m_strMainCfgModeDir, pApp->m_strConfigFile);
	iniFile.SetPath(strFile);

	strKey.Format("Device Config");
	if(iniFile.ReadFile()) {
		for(int a = 0; a<MAX_DIRECTION; a++) {
			strTemp.Format("No. Of Block[%c]", !a ? 'X' : 'Y');
			m_nNoOfBlock[a] = iniFile.GetInteger(strKey, strTemp, m_nNoOfBlock[a]);
			strTemp.Format("Block Pitch[%c]", !a ? 'X' : 'Y');
			m_dBlockPitch[a] = iniFile.GetDouble(strKey, strTemp, m_dBlockPitch[a]);
			strTemp.Format("Device No. Per Block[%c]", !a ? 'X' : 'Y');
			m_nDeviceNoPerBlock[a] = iniFile.GetInteger(strKey, strTemp, m_nDeviceNoPerBlock[a]);
			strTemp.Format("Device Pitch[%c]", !a ? 'X' : 'Y');
			m_dDevicePitch[a] = iniFile.GetDouble(strKey, strTemp, m_dDevicePitch[a]);
			strTemp.Format("Device Scan Area[%c]", !a ? 'X' : 'Y');
			m_dDeviceScanArea[a] = iniFile.GetDouble(strKey, strTemp, m_dDeviceScanArea[a]);
			//strTemp.Format("Device Offset[%c]", !a ? 'X' : 'Y');
			//m_dDeviceOffset[a] = iniFile.GetDouble(strKey, strTemp, m_dDeviceOffset[a]);
			strTemp.Format("Device Overlap[%c]", !a ? 'X' : 'Y');
			m_dDeviceOverlap[a] = iniFile.GetDouble(strKey, strTemp, m_dDeviceOverlap[a]);
			m_dInitialOffset = iniFile.GetDouble(strKey, "Device Initial Position", m_dInitialOffset);
			strTemp.Format("Datum Top Row Offset[%c]", !a ? 'X' : 'Y');
			m_dTopRowOffset[a] = iniFile.GetDouble(strKey, strTemp, m_dTopRowOffset[a]);
			strTemp.Format("Datum Bottom Row Offset[%c]", !a ? 'X' : 'Y');
			m_dBottomRowOffset[a] = iniFile.GetDouble(strKey, strTemp, m_dBottomRowOffset[a]);


			strTemp.Format("Device No. Per Block[%c] TOP STATION", !a ? 'X' : 'Y');
			m_nDeviceNoPerBlockTOP[a] = iniFile.GetInteger(strKey, strTemp, !a ? 1 : 5);
		}
	}
}

int MotionParameters::CreateScanPositions()
{
	int nDieIDX = 0;
	int nDieIDY = 0;
	int nImageIDPerDieX = 0;
	int nImageIDPerDieY = 0;

	int nXMotorPosition = 0;
	int nYMotorPosition = 0;

	double dStepX = 0;
	double dStepY = 0;

	int nDirectionX = -1;
	int nDirectionYTop = -1;

	nFOVPerDieinXTHR = m_nDeviceNoPerBlock[0];
	nFOVPerDieinYTHR = m_nDeviceNoPerBlock[1];

	nFOVPerDieinXTOP = m_nDeviceNoPerBlockTOP[0];			//Need to read from UI device configs
	nFOVPerDieinYTOP = m_nDeviceNoPerBlockTOP[1];

//	int nFOVPerDieinXTOP = 3;
//	int nFOVPerDieinYTOP = 1;

//	int nFOVPerDieinXTHR = 3;

	double dDiePerXPitchMM = m_dBlockPitch[0];
	double dFOVPerXPitchMM = m_dDevicePitch[0];

	double dDiePerYPitchMM = m_dBlockPitch[1];
	double dFOVPerYPitchMM = m_dDevicePitch[1];
	
	int nDieRowNo = m_nNoOfBlock[1];
	int nDieColNo = m_nNoOfBlock[0];	//Hardcode

	int nCameraGrabNoPerDevice = 1;  // In case of Multile Docs
	double dGrabOffsetMM = 2.0; 

	nScanRowNoTHR = nFOVPerDieinYTHR * m_nNoOfBlock[1];
	nScanColumnNoTHR = nFOVPerDieinXTHR  * m_nNoOfBlock[0] * nCameraGrabNoPerDevice;

	nScanRowNoTop = nFOVPerDieinYTOP * m_nNoOfBlock[1];				
	nScanColumnNoTop = nFOVPerDieinXTOP * m_nNoOfBlock[0] * nCameraGrabNoPerDevice;

	nScanRowNoZSensor = 1 * m_nNoOfBlock[1];
	nScanColumnNoZSensor = nFOVPerDieinXTHR  * m_nNoOfBlock[0];

	double dStartingOffsetX = m_dInitialOffset;
	motorYScanningPositionTHR.resize(nScanRowNoTHR);
	motorYScanningPositionTop.resize(nScanRowNoTop);
	motorYScanningPositionTopEncap.resize(nScanRowNoTop);

	dieColumnNoTHR.resize(nScanColumnNoTHR*nScanRowNoTHR);
	dieRowNoTHR.resize(nScanColumnNoTHR*nScanRowNoTHR);
	dieNoTHR.resize(nScanColumnNoTHR*nScanRowNoTHR);
	dieFOVTHR.resize(nScanColumnNoTHR*nScanRowNoTHR);
	
	bInspectionStatusTHR.resize(nScanColumnNoTHR*nScanRowNoTHR);
	enableInspectionTHR.resize(nScanColumnNoTHR*nScanRowNoTHR);
	scanningImageComputerId.resize(nScanColumnNoTHR*nScanRowNoTHR);

	scanningDiemappingId.resize(nScanColumnNoTop*nScanRowNoTop, -1);
	TopToTHRDieMapping.resize(nScanColumnNoTHR*nScanRowNoTHR);

	dieColumnNoTop.resize(nScanColumnNoTop*nScanRowNoTop);
	dieRowNoTop.resize(nScanColumnNoTop*nScanRowNoTop);
	dieNoTop.resize(nScanColumnNoTop*nScanRowNoTop);
	dieFOVTop.resize(nScanColumnNoTop*nScanRowNoTop);
	bInspectionStatusTop.resize(nScanColumnNoTop*nScanRowNoTop);

	scanningDiemappingIdTOP.resize(nScanColumnNoTop*nScanRowNoTop);


	dOffsetXScanTHRMicron.resize(m_nNoOfBlock[0] * m_nNoOfBlock[1]);
	dOffsetYScanTHRMicron.resize(m_nNoOfBlock[0] * m_nNoOfBlock[1]);

	nTotalImageFOVTHR = 0;
	nTotalImageFOVTOP = 0;
//	int nDieRow;

//	nDirectionYTop = 1; //for PCI_7856

//--. Create trigger points
	_cameraTriggerTHR.resize(nScanColumnNoTHR * nScanRowNoTHR);
	std::fill(_cameraTriggerTHR.begin(), _cameraTriggerTHR.end(), 0);

	nTotalImageFOVTHR = nScanColumnNoTHR * nScanRowNoTHR;

	triggerCountPerRowTHR.resize(nScanRowNoTHR);
	std::fill(triggerCountPerRowTHR.begin(), triggerCountPerRowTHR.end(), static_cast<short>(nScanColumnNoTHR));


	_cameraTriggerTOP.resize(nScanColumnNoTop * nScanRowNoTop);
	std::fill(_cameraTriggerTOP.begin(), _cameraTriggerTOP.end(), 0);

	nTotalImageFOVTOP = nScanColumnNoTop * nScanRowNoTop;

	triggerCountPerRowTOP.resize(nScanRowNoTop);
	std::fill(triggerCountPerRowTOP.begin(), triggerCountPerRowTOP.end(), static_cast<short>(nScanColumnNoTop));

	_cameraTriggerTopEncap.resize(nScanColumnNoTop * nScanRowNoTop);
	std::fill(_cameraTriggerTopEncap.begin(), _cameraTriggerTopEncap.end(), 0);

	_zSensorTriggerStartPos.resize(nScanColumnNoZSensor * nScanRowNoZSensor);
	std::fill(_zSensorTriggerStartPos.begin(), _zSensorTriggerStartPos.end(), 0);

	_zSensorTriggerEndPos.resize(nScanColumnNoZSensor * nScanRowNoZSensor);
	std::fill(_zSensorTriggerEndPos.begin(), _zSensorTriggerEndPos.end(), 0);

	_zSensortriggerCountPerRow.resize(nDieRowNo);
	std::fill(_zSensortriggerCountPerRow.begin(), _zSensortriggerCountPerRow.end(), static_cast<short>(nScanColumnNoTHR));
	

	
	_zSensorEachFovDistance.resize(nScanColumnNoZSensor * nScanRowNoZSensor);

	_zSensorMeasuredDistance.resize(nScanColumnNoZSensor * nScanRowNoZSensor);
	std::fill(_zSensorMeasuredDistance.begin(), _zSensorMeasuredDistance.end(), 0);

	/* Z Sensor Trigger Position */
	double dZScanningDiameterMM = 3;//Chicklet Width is around 4 mm
	double dLeftOffsetMM = 1.5;//Offset when scan from Left to Right
	double dRightOffsetMM = 2;//Offset when scan from Right to Left

	for(int i = 0; i < nScanRowNoZSensor; i++) {

		nDieIDY = i % nDieRowNo;
		nImageIDPerDieY = i % nFOVPerDieinYTHR;

		int m_nStartScanPosition = motorZSensorTeachPos[0][X_SERVO_MOTOR];
		int m_nEndStartScanPos = m_nStartScanPosition;


		//Scan direction: From Left to Right
		if (i % 2 == 0) {
			m_nStartScanPosition = m_nStartScanPosition - nDirectionX * dLeftOffsetMM * motorEngine[X_SERVO_MOTOR].getResolution();
		}
		//Scan direction: From Right to Left
		else {
			m_nStartScanPosition = m_nStartScanPosition + nDirectionX * dRightOffsetMM * motorEngine[X_SERVO_MOTOR].getResolution();
		}

		for(int j = 0; j < _zSensortriggerCountPerRow[i]; j++) {

			nDieIDX = j / nFOVPerDieinXTHR;
			nImageIDPerDieX = j % nFOVPerDieinXTHR;

			dStepX = (nDieIDX * dDiePerXPitchMM ) * motorEngine[X_SERVO_MOTOR].getResolution();

			if(i % 2 == 0) {
				int nIndex = i * nScanColumnNoTHR + j;
				_zSensorTriggerStartPos[nIndex] = Round(m_nStartScanPosition + nDirectionX * dStepX);

				if (j == 0)
					_zSensorTriggerStartPos[nIndex] = _zSensorTriggerStartPos[nIndex] + nDirectionX * 10;

				_zSensorTriggerEndPos[nIndex] = Round(_zSensorTriggerStartPos[nIndex] + nDirectionX * dZScanningDiameterMM * motorEngine[X_SERVO_MOTOR].getResolution());
				OutputDebugLogTo(2, TRUE, "_zSensorTriggerStartPos[%d]: %d", nIndex, _zSensorTriggerStartPos[nIndex]);
			}
			else {
				int nIndex = i * nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j);
				_zSensorTriggerStartPos[nIndex] = Round(m_nStartScanPosition + nDirectionX * dStepX);

				if (j == 0)
					_zSensorTriggerStartPos[nIndex] = _zSensorTriggerStartPos[nIndex] + nDirectionX * 10;


				_zSensorTriggerEndPos[nIndex] = Round(_zSensorTriggerStartPos[nIndex] + nDirectionX * dZScanningDiameterMM * motorEngine[X_SERVO_MOTOR].getResolution());
				OutputDebugLogTo(2, TRUE, "_zSensorTriggerStartPos[%d]: %d", nIndex, _zSensorTriggerStartPos[nIndex]);

			}

		}

		OutputDebugLogTo(2, TRUE, "--------------------------------------------------------------------------------");
	}

	/* TOP Scan Position */
	for (int i = 0; i < nScanRowNoTop; i++) {
		nDieIDY = i / nFOVPerDieinYTOP;
		nImageIDPerDieY = i % nFOVPerDieinYTOP;
		int nFov = nImageIDPerDieY;
		int nStartScanPosition = (nDieIDY ? -dStartingOffsetX : 0)  * motorEngine[X_SERVO_MOTOR].getResolution() * nDirectionX + motorTOPStartScanPos[nFov][X_SERVO_MOTOR];
		
		for (int j = 0; j < triggerCountPerRowTOP[i]; j++) {

			int nGrabOffsetNo = j % nCameraGrabNoPerDevice;
			nDieIDX = (j / nCameraGrabNoPerDevice) / nFOVPerDieinXTOP;
			nImageIDPerDieX = (j / nCameraGrabNoPerDevice) % nFOVPerDieinXTOP;

			dStepX = (nDieIDX * dDiePerXPitchMM + nImageIDPerDieX * dFOVPerXPitchMM + nGrabOffsetNo * dGrabOffsetMM) * motorEngine[X_SERVO_MOTOR].getResolution();

			int nIndex = 0;
			if (i % 2 == 0) {
				nIndex = i * nScanColumnNoTop + j;
				_cameraTriggerTOP[nIndex] = Round(nStartScanPosition + nDirectionX * dStepX + (nDirectionX * dForwardTriggerOffset * motorEngine[X_SERVO_MOTOR].getResolution()));
			}
			else {
				nIndex = i * nScanColumnNoTop + (nScanColumnNoTop - 1 - j);
				_cameraTriggerTOP[nIndex] = Round(nStartScanPosition + nDirectionX * dStepX + (nDirectionX * dInverseTriggerOffset * motorEngine[X_SERVO_MOTOR].getResolution()));
			}
			OutputDebugLogTo(8, TRUE, "_cameraTriggerTOP[%d]: %d", nIndex, _cameraTriggerTOP[nIndex]);
		}
	}
	OutputDebugLogTo(8, TRUE, "--------------------------------------------------------------------------------");

	/* THR Scan Position */
	for(int i = 0; i < nScanRowNoTHR; i++) {
		nDieIDY = i / nFOVPerDieinYTHR;
		nImageIDPerDieY = i % nFOVPerDieinYTHR;

		int nStartScanPosition = (nDieIDY ? -dStartingOffsetX : 0)  * motorEngine[X_SERVO_MOTOR].getResolution() * nDirectionX + motorStartScanPos[X_SERVO_MOTOR];
		int nTopToTHRDistance = m_dDistanceTopToTHR  * motorEngine[X_SERVO_MOTOR].getResolution() * nDirectionX;

		for(int j = 0; j < triggerCountPerRowTHR[i]; j++) {

			int nGrabOffsetNo = j % nCameraGrabNoPerDevice;
			nDieIDX = (j / nCameraGrabNoPerDevice) / nFOVPerDieinXTHR;
			nImageIDPerDieX = (j / nCameraGrabNoPerDevice) % nFOVPerDieinXTHR;

			dStepX = (nDieIDX * dDiePerXPitchMM + nGrabOffsetNo * dGrabOffsetMM ) * motorEngine[X_SERVO_MOTOR].getResolution();

			int nIndex = 0;
			if(i % 2 == 1) {
				nIndex = i * nScanColumnNoTHR + j;
				_cameraTriggerTHR[nIndex] = Round(nStartScanPosition + nDirectionX * dStepX + (nDirectionX * dForwardTriggerOffset * motorEngine[X_SERVO_MOTOR].getResolution()));
			}
			else {
				nIndex = i * nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j);
				_cameraTriggerTHR[nIndex] = Round(nStartScanPosition + nDirectionX * dStepX + (nDirectionX * dInverseTriggerOffset * motorEngine[X_SERVO_MOTOR].getResolution()));
			}
			OutputDebugLogTo(9, TRUE, "_cameraTriggerTHR[%d]: %d", nIndex, _cameraTriggerTHR[nIndex]);
		}
	}
	OutputDebugLogTo(9, TRUE, "--------------------------------------------------------------------------------");


	//Top and Z Points
	int nTriggerCountPerRow;
	std::map <long, short > Triggerpoints;	//1 -ZTrigger, 2- Top Trigger, 0 for both

	for(int row = 0; row < nScanRowNoZSensor; row++) {
		for(int i = 0; i < _zSensortriggerCountPerRow[row]; i++)
			Triggerpoints.insert(std::pair<long, short>(_zSensorTriggerStartPos[row*nScanColumnNoZSensor + i], 1));
		
		if(row % 2 == 0) {
			for (std::map<long, short>::reverse_iterator it = Triggerpoints.rbegin(); it != Triggerpoints.rend(); ++it) {
				_zSensorTopTriggerPoints.push_back(it->first);
				_zSensorTopTriggerIndicator.push_back(it->second);
			}
		}
		else {
			for (std::map<long, short>::iterator it = Triggerpoints.begin(); it != Triggerpoints.end(); ++it) {
				_zSensorTopTriggerPoints.push_back(it->first);
				_zSensorTopTriggerIndicator.push_back(it->second);
			}
		}
		
		_zTopTriggerCountPerRow.push_back(Triggerpoints.size());
		Triggerpoints.clear();
	}
	for (int i = 0; i < _zSensorTopTriggerPoints.size(); i++) {
		OutputDebugLogTo(7, TRUE, "_zSensorTopTriggerPoints[%d]: %d", i, _zSensorTopTriggerPoints[i]);
	}
	OutputDebugLogTo(7, TRUE, "--------------------------------------------------------------------------------");


	//THR Scan Start & End Position
	motorXStartScanningPositionTHR.resize(nScanRowNoTHR);
	motorXEndScanningPositionTHR.resize(nScanRowNoTHR);

	for(int i = 0; i<nScanRowNoTHR; i++) {
		if(i % 2 == 0) {
			motorXStartScanningPositionTHR[i] = Round((_cameraTriggerTHR[i*nScanColumnNoTHR]								/ motorEngine[X_SERVO_MOTOR].getResolution() + nDirectionX * dMotorXScanOffsetMM) * motorEngine[X_SERVO_MOTOR].getResolution());
			motorXEndScanningPositionTHR[i] = Round((_cameraTriggerTHR[i*nScanColumnNoTHR + triggerCountPerRowTHR[i] - 1]	/ motorEngine[X_SERVO_MOTOR].getResolution() - nDirectionX * (dMotorXScanOffsetMM)) * motorEngine[X_SERVO_MOTOR].getResolution());
		}
		else {
			motorXStartScanningPositionTHR[i] = Round((_cameraTriggerTHR[i*nScanColumnNoTHR]								/ motorEngine[X_SERVO_MOTOR].getResolution() - nDirectionX * (dMotorXScanOffsetMM)) * motorEngine[X_SERVO_MOTOR].getResolution());
			motorXEndScanningPositionTHR[i] = Round((_cameraTriggerTHR[i*nScanColumnNoTHR + triggerCountPerRowTHR[i] - 1]	/ motorEngine[X_SERVO_MOTOR].getResolution() + nDirectionX * (dMotorXScanOffsetMM)) * motorEngine[X_SERVO_MOTOR].getResolution());
		}
	}

	motorXStartScanningPositionTop.resize(nScanRowNoTop);
	motorXEndScanningPositionTop.resize(nScanRowNoTop);

	motorXStartScanningPositionTopEncap.resize(nScanRowNoTop);
	motorXEndScanningPositionTopEncap.resize(nScanRowNoTop);

	//Top position
	for(int i = 0; i<nScanRowNoTop; i++) {
		nDieIDY = i % nDieRowNo;
		if(i % 2 == 0) {
			motorXStartScanningPositionTop[i] = Round((_cameraTriggerTOP[i*nScanColumnNoTop]										/ motorEngine[X_SERVO_MOTOR].getResolution() - nDirectionX * (dMotorXScanOffsetMM)) * motorEngine[X_SERVO_MOTOR].getResolution());
			motorXEndScanningPositionTop[i] = Round((_cameraTriggerTOP[i*nScanColumnNoTop + triggerCountPerRowTOP[i] -1]			/ motorEngine[X_SERVO_MOTOR].getResolution() + nDirectionX * (dMotorXScanOffsetMM )) * motorEngine[X_SERVO_MOTOR].getResolution());
		}
		else {
			motorXStartScanningPositionTop[i] = Round((_cameraTriggerTOP[i*nScanColumnNoTop ]									/ motorEngine[X_SERVO_MOTOR].getResolution() + nDirectionX * (dMotorXScanOffsetMM)) * motorEngine[X_SERVO_MOTOR].getResolution());
			motorXEndScanningPositionTop[i] = Round((_cameraTriggerTOP[i*nScanColumnNoTop + triggerCountPerRowTOP[i] - 1]			/ motorEngine[X_SERVO_MOTOR].getResolution() - nDirectionX * (dMotorXScanOffsetMM)) * motorEngine[X_SERVO_MOTOR].getResolution());
		}
	}
	for (int i = 0; i < nScanRowNoTop; i++) {
		OutputDebugLogTo(6, TRUE, "Motor X start and end  Position Top[%d]: %d -> %d", i, motorXStartScanningPositionTop[i], motorXEndScanningPositionTop[i]);
	}

	OutputDebugLogTo(6, TRUE, "--------------------------------------------------------------------------------");
	motorXStartScanningPositionZ.resize(nScanRowNoZSensor);
	motorXEndScanningPositionZ.resize(nScanRowNoZSensor);

	// Z Sensor
	for(int i = 0; i<nScanRowNoZSensor; i++) {
		nDieIDY = i % nDieRowNo;
		if(i % 2 == 0) {
			motorXStartScanningPositionZ[i] = Round((_zSensorTriggerStartPos[i*nScanColumnNoZSensor] / motorEngine[X_SERVO_MOTOR].getResolution() - nDirectionX * (dMotorXScanOffsetMM)) * motorEngine[X_SERVO_MOTOR].getResolution());
			motorXEndScanningPositionZ[i] = Round((_zSensorTriggerStartPos[i*nScanColumnNoZSensor + _zSensortriggerCountPerRow[i] - 1] / motorEngine[X_SERVO_MOTOR].getResolution() + nDirectionX * (dMotorXScanOffsetMM )) * motorEngine[X_SERVO_MOTOR].getResolution());
			OutputDebugLogTo(6, TRUE, "Motor X start and end  Position ZSensor[%d]: %d -> %d", i, motorXStartScanningPositionTop[i], motorXEndScanningPositionTop[i]);
		}
		else {
			motorXStartScanningPositionZ[i] = Round((_zSensorTriggerStartPos[i*nScanColumnNoZSensor] / motorEngine[X_SERVO_MOTOR].getResolution() + nDirectionX * (dMotorXScanOffsetMM )) * motorEngine[X_SERVO_MOTOR].getResolution());
			motorXEndScanningPositionZ[i] = Round((_zSensorTriggerStartPos[i*nScanColumnNoZSensor + _zSensortriggerCountPerRow[i] - 1] / motorEngine[X_SERVO_MOTOR].getResolution() - nDirectionX * (dMotorXScanOffsetMM )) * motorEngine[X_SERVO_MOTOR].getResolution());
			OutputDebugLogTo(6, TRUE, "Motor X start and end  Position ZSensor[%d]: %d -> %d", i, motorXStartScanningPositionTop[i], motorXEndScanningPositionTop[i]);

		}
	}
	OutputDebugLogTo(6, TRUE, "--------------------------------------------------------------------------------");

	motorXStartScanningPositionTopandZ.resize(nScanRowNoTop);
	motorXEndScanningPositionTopandZ.resize(nScanRowNoTop);

	// Z Sensor and Top
	for(int i = 0; i< nScanRowNoTop; i++) {
		//Move Further For Z-Sensor Scanning
		if ((i == 1 || i == 7 ) && m_bEnableTHRScanning) {
			motorXStartScanningPositionTopandZ[i] = motorXStartScanningPositionTop[i];
			motorXEndScanningPositionTopandZ[i] = motorZSensorTeachPos[0][X_SERVO_MOTOR] - nDirectionX * dLeftOffsetMM * motorEngine[X_SERVO_MOTOR].getResolution();
		}
		else {
			motorXStartScanningPositionTopandZ[i] = motorXStartScanningPositionTop[i];
			motorXEndScanningPositionTopandZ[i] = motorXEndScanningPositionTop[i];
		}
	}
	//If THR scan is enable --> Last Row Scan of Top Scan --> Directly Move to First Scan Poistion Of THR 
	if (m_bEnableTHRScanning) {
		motorXEndScanningPositionTopandZ[nScanRowNoTop - 1] = motorXStartScanningPositionTHR[nScanRowNoTHR - 1];
	}

	for (int i = 0; i < nScanRowNoTop; i++) {
		OutputDebugLogTo(6, TRUE, "Motor X start and end  Position TopandZ[%d]: %d -> %d", i, motorXStartScanningPositionTopandZ[i], motorXEndScanningPositionTopandZ[i]);
	}
	OutputDebugLogTo(6, TRUE, "--------------------------------------------------------------------------------");

	motorYScanningPositionTHR.resize(nScanRowNoTHR);
	motorYScanningPositionTop.resize(nScanRowNoTop);
	motorYScanningPositionTopEncap.resize(nScanRowNoTop);

	//Y-Motor TOP Scan Position
	for (int i = 0; i < nScanRowNoTop; i++) {

		int nDieID = i / nFOVPerDieinYTOP;
		int nFOVIDPerDie = i % nFOVPerDieinYTOP;

		double dStepYTop = (nDieID * dDiePerYPitchMM /*+ nFOVIDPerDie * dFOVPerYPitchMM*/) * motorEngine[Y_SERVO_MOTOR].getResolution();

		motorYScanningPositionTop[i] = Round(motorTOPStartScanPos[nFOVIDPerDie][Y_SERVO_MOTOR] + nDirectionYTop * dStepYTop);
		OutputDebugLogTo(7, TRUE, "motorYScanningPositionTop[%d] = %d", i, motorYScanningPositionTop[i]);
	}
	OutputDebugLogTo(7, TRUE, "--------------------------------------------------------------------------------");

	//Y-Motor THR Scan Position
	for(int i = 0; i<nScanRowNoTHR; i++) {

		int nDieID = i / nFOVPerDieinYTHR;
		int nFOVIDPerDie = i % nFOVPerDieinYTHR;

		double dStepYTHR = (nDieID * dDiePerYPitchMM + nFOVIDPerDie * dFOVPerYPitchMM) * motorEngine[Y_SERVO_MOTOR].getResolution();
		
		motorYScanningPositionTHR[i] = Round(motorStartScanPos[Y_SERVO_MOTOR] + nDirectionYTop * dStepYTHR);
		OutputDebugLogTo(7, TRUE, "motorYScanningPositionTHR[%d] = %d", i, motorYScanningPositionTHR[i]);
	}
	OutputDebugLogTo(7, TRUE, "--------------------------------------------------------------------------------");

	int nGrabID = 0;

	for(int i = nScanRowNoTHR-1; i >=0; i--) {

		nDieIDY = i / nFOVPerDieinYTHR;
		if(nFOVPerDieinYTHR > 1)
			nImageIDPerDieY = i % nFOVPerDieinYTHR;
		else
			nImageIDPerDieY = i;

		for(int j = 0; j < nScanColumnNoTHR; j++, nGrabID++) {

			nDieIDX = j / nFOVPerDieinXTHR;

			if(nFOVPerDieinXTHR > 1)
				nImageIDPerDieX = j % nFOVPerDieinXTHR;
			else
				nImageIDPerDieX = j;

			int nColId = j % nFOVPerDieinXTHR;

			//Mapping Die Column ID, Die ID
			if(i % 2 == 1) {
				if (nFOVPerDieinXTHR > 1)
					dieColumnNoTHR[nGrabID] = j % nFOVPerDieinXTHR;
				else
					dieColumnNoTHR[nGrabID] = j;

				dieNoTHR[nGrabID] = nDieIDY * m_nNoOfBlock[0] + nImageIDPerDieX;
				
				if (nDieIDY == 0) {
					scanningDiemappingId[i*nScanColumnNoTHR + j] = nGrabID;
				}
				else {
					scanningDiemappingId[i*nScanColumnNoTHR + j + abs(nFOVPerDieinYTOP - nFOVPerDieinYTHR)* nScanColumnNoTHR] = nGrabID;
				}
			}
			else {
				if(nFOVPerDieinXTHR > 1)
					dieColumnNoTHR[nGrabID] = (nScanColumnNoTHR - 1 - j) % nFOVPerDieinXTHR;
				else
					dieColumnNoTHR[nGrabID] = (nScanColumnNoTHR - 1 - j);

				dieNoTHR[nGrabID] = nDieIDY * m_nNoOfBlock[0] + nScanColumnNoTHR - 1 - nImageIDPerDieX;

				if (nDieIDY == 0) {
					scanningDiemappingId[i*nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j)] = nGrabID;
				}
				else {
					scanningDiemappingId[i*nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j) + abs(nFOVPerDieinYTOP - nFOVPerDieinYTHR)* nScanColumnNoTHR] = nGrabID;
				}
			}

			//Mapping FOV ID
			
			if (nImageIDPerDieY % 2 == 0) {
				dieFOVTHR[nGrabID] = nImageIDPerDieY * nFOVPerDieinXTHR + nColId;
			}
			else {
				dieFOVTHR[nGrabID] = ((nImageIDPerDieY+1) * nFOVPerDieinXTHR - 1) - nColId;
			}

			enableInspectionTHR[nGrabID] = true;
			dieRowNoTHR[nGrabID] = nImageIDPerDieY;
			bInspectionStatusTHR[nGrabID] = false;
			scanningImageComputerId[nGrabID] = -1;
		}
	}

	//Mapping THR Grab ID to TOP Grab ID
	nGrabID = 0;
	for (int i = 0; i < nScanRowNoTHR; i++) {

		nDieIDY = i / nFOVPerDieinYTHR;
		nImageIDPerDieY = i % nFOVPerDieinYTHR;

		for (int j = 0; j < nScanColumnNoTHR; j++, nGrabID++) {

			if (nDieIDY % 2 == 0) {
				//OutputDebugLogTo(9, TRUE, "nGrabID %d, Index: %d, scanningDiemappingId: %d", nGrabID, i*nScanColumnNoTHR + j, scanningDiemappingId[nGrabID]);

				TopToTHRDieMapping[i*nScanColumnNoTHR + j] = nFOVPerDieinYTOP-1;
			}
			else {
				//OutputDebugLogTo(9, TRUE, "nGrabID %d, Index: %d, scanningDiemappingId: %d", nGrabID, i*nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j), scanningDiemappingId[nGrabID]);

				TopToTHRDieMapping[i*nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j)] = nFOVPerDieinYTOP - 1;
			}
		}
	}
	OutputDebugLogTo(9, TRUE, "--------------------------------------------------------------------------------");

	nGrabID = 0;
	for(int i = 0; i < nScanRowNoTop; i++) {

		nDieIDY = i / nFOVPerDieinYTOP;
		if (nFOVPerDieinYTOP > 1)
			nImageIDPerDieY = i % nFOVPerDieinYTOP;
		else
			nImageIDPerDieY = i;

		for(int j = 0; j < nScanColumnNoTop; j++, nGrabID++) {

			nDieIDX = j / nFOVPerDieinXTOP;
			if (nFOVPerDieinXTOP > 1)
				nImageIDPerDieX = j % nFOVPerDieinXTOP;
			else
				nImageIDPerDieX = j;

			int nColId = j % nFOVPerDieinXTOP;

			//Mapping Die Column ID, Die ID
			if(i % 2 == 0) {
				if (nFOVPerDieinXTOP > 1)
					dieColumnNoTop[nGrabID] = j % nFOVPerDieinXTOP;
				else
					dieColumnNoTop[nGrabID] = j;

				dieNoTop[nGrabID] = nDieIDY * m_nNoOfBlock[0] + nImageIDPerDieX;

				scanningDiemappingIdTOP[i*nScanColumnNoTop + j] = nGrabID;

				//Mapping FOV ID
				dieFOVTop[nGrabID] = nImageIDPerDieY * nFOVPerDieinXTOP + nColId;
			}
			else {
				if(nFOVPerDieinXTOP > 1)
					dieColumnNoTop[nGrabID] = (nScanColumnNoTop - 1 - j) % nFOVPerDieinXTOP;
				else
					dieColumnNoTop[nGrabID] = (nScanColumnNoTop - 1 - j);

				dieNoTop[nGrabID] = nDieIDY * m_nNoOfBlock[0] + nScanColumnNoTop - 1 - nImageIDPerDieX;

				scanningDiemappingIdTOP[i*nScanColumnNoTop + (nScanColumnNoTop - 1 - j)] = nGrabID;

				//Mapping FOV ID
				dieFOVTop[nGrabID] = ((nImageIDPerDieY + 1) * nFOVPerDieinXTOP - 1) - nColId;
			}


			dieRowNoTop[nGrabID] = nImageIDPerDieY;
			bInspectionStatusTop[nGrabID] = false;
		}
	}

	double frequency = 30;
	int nError = theApp.m_TriangularSensor.Get_SampleRate(theApp.m_TrigangualrsensorInstance, frequency);
	int SpeedinMM;

	if(!nError) {
		OutputDebugLogTo(6, TRUE, "Sample rate %f KHz", frequency);
		double dDeltaZScanningDiameterMM = dZScanningDiameterMM * 0.9;//Expect to ready 90% data point from Triangular sensor
		OutputDebugLogTo(6, TRUE, "Delta Scanning Z Sensor %f mm", dDeltaZScanningDiameterMM);
	
		SpeedinMM = Round(10 * _motorScanVelocity[X_SERVO_MOTOR].maximumSpeed / motorEngine[X_SERVO_MOTOR].getResolution()) / 10.;
		OutputDebugLogTo(6, TRUE, "Z readings speed %d mm/sec", SpeedinMM);

		nNumberofZTriggerpoints = (dDeltaZScanningDiameterMM * frequency*1000) / (SpeedinMM);
		OutputDebugLogTo(6, TRUE, "Trigger points %d", nNumberofZTriggerpoints);

		nError = theApp.m_TriangularSensor.Data_Avail_Event(theApp.m_TrigangualrsensorInstance, nNumberofZTriggerpoints);
		if(nError)
			OutputDebugLogTo(0, TRUE, "Failed to Set Data_Avail_Event");

		nError = theApp.m_TriangularSensor.Clear_Buffer(theApp.m_TrigangualrsensorInstance);
		if(nError)
			OutputDebugLogTo(0, TRUE, "Failed to Clear Buffer ERROR NO: %d", nError);
	}
	//Calculate speed to read N numbers 

	scanningReady = true;

	dReScanDieStatus.resize(getTotalDieCount());

//	CreateNewPositions();

	return 0;
}


int MotionParameters::CreateNewPositions()
{
	int nDieIDX = 0;
	int nDieIDY = 0;

	int nDirectionX = 1;
	int nDirectionYTop = -1;


	int nImageIDPerDieX = 0;
	int nImageIDPerDieY = 0;
	double dStartingOffsetX = m_dInitialOffset;

	int nCameraGrabNoPerDevice = 1;  // In case of Multile Docs
	double dGrabOffsetMM = 2.0;

	double dDiePerXPitchMM = m_dBlockPitch[0];
	double dFOVPerXPitchMM = m_dDevicePitch[0];

	double dStepX = 0;
	double dStepY = 0;

	double dDiePerYPitchMM = m_dBlockPitch[1];
	double dFOVPerYPitchMM = m_dDevicePitch[1];

	int nDeviceID;

	if (theApp.m_bEnableTopDatumPosToScanTHR) {
		int nGrabID = 0;

		double nDieOffsetX[2];
		double nDieOffsetY[2];

		GetDatumOffsetParameters(nDieOffsetX[0], nDieOffsetX[1], nDieOffsetY[0], nDieOffsetY[1]);


		for (int i = 0; i < nScanRowNoTHR; i++) {

			nDieIDY = i / nFOVPerDieinYTHR;
			nImageIDPerDieY = i % nFOVPerDieinYTHR;

			int nStartScanPosition = (nDieIDY ? -dStartingOffsetX : 0)  * motorEngine[X_SERVO_MOTOR].getResolution() * nDirectionX + motorStartScanPos[X_SERVO_MOTOR];
			int nTopToTHRDistance = m_dDistanceTopToTHR  * motorEngine[X_SERVO_MOTOR].getResolution() * nDirectionX;

			/*nTotalImageFOVTHR = nTotalImageFOVTHR + nScanColumnNoTHR;*/
//			double dOffsetExtra = 170;

//			if (dOffsetXScanTHRMicron[0] < dOffsetExtra)
//				dOffsetExtra = 0;
				
			for (int j = 0; j < triggerCountPerRowTHR[i]; j++, nGrabID++) {

				int nGrabOffsetNo = j % nCameraGrabNoPerDevice;
				nDieIDX = (j / nCameraGrabNoPerDevice) / nFOVPerDieinXTHR;
				nImageIDPerDieX = (j / nCameraGrabNoPerDevice) % nFOVPerDieinXTHR;
			//	int nDieNo = (i / nFOVPerDieinYTHR) ? j * 2 : j * 2 + 1;

				if (i % 2 == 0) {
					nDeviceID = dieNoTHR[nGrabID];
					dStepX = (nDieIDX * dDiePerXPitchMM + nImageIDPerDieX * dFOVPerXPitchMM + nGrabOffsetNo * dGrabOffsetMM - ((dOffsetXScanTHRMicron[nDeviceID]) / 1000) + (nDieOffsetX[nDieIDY]) + dForwardTriggerOffset ) * motorEngine[X_SERVO_MOTOR].getResolution();
	//				OutputDebugLogTo(8, TRUE, "THR,%d, %d", i*nScanColumnNoTHR + j, _cameraTriggerTHR[i*nScanColumnNoTHR + j]);
					_cameraTriggerTHR[i*nScanColumnNoTHR + j] = Round(nStartScanPosition + nDirectionX * dStepX);
//					OutputDebugLogTo(8, TRUE, "X valuse Chnaged %d to %d for Offset Diff %d = %f", i*nScanColumnNoTHR + j, _cameraTriggerTHR[i*nScanColumnNoTHR + j], nDeviceID, dOffsetXScanTHRMicron[nDeviceID]);

				}
				else {
//					OutputDebugLogTo(8, TRUE, "THR,%d, %d", i*nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j), _cameraTriggerTHR[i*nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j)]);
					nDeviceID = dieNoTHR[i*nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j)];
					dStepX = (nDieIDX * dDiePerXPitchMM + nImageIDPerDieX * dFOVPerXPitchMM + nGrabOffsetNo * dGrabOffsetMM - ((dOffsetXScanTHRMicron[nDeviceID]) / 1000) + (nDieOffsetX[nDieIDY]) + dInverseTriggerOffset ) * motorEngine[X_SERVO_MOTOR].getResolution();
					_cameraTriggerTHR[i*nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j)] = Round(nStartScanPosition + nDirectionX * dStepX);
//					OutputDebugLogTo(8, TRUE, "X valuse Chnaged %d to %d for Offset Diff %d = %f", i*nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j), _cameraTriggerTHR[i*nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j)], nDeviceID, dOffsetXScanTHRMicron[nDeviceID]);

				}
			}
		}

		motorYScanningPositionTHR.clear();
		motorYScanningPositionTHR.resize(nScanRowNoTHR);
		motorYScanningDatumPositiom.resize(nScanRowNoTHR * m_nNoOfBlock[0]);

		double dStepYTop = 0;
		int nTopIndex = 0;
		for (int i = 0; i < nScanRowNoTHR; i++) {
			int nDieID = i / nFOVPerDieinYTHR;
			int nFOVIDPerDie = i % nFOVPerDieinYTHR;
			dStepYTop = (nDieID * dDiePerYPitchMM + nFOVIDPerDie * dFOVPerYPitchMM - ((dOffsetYScanTHRMicron[0]) / 1000) + nDieOffsetY[nDieID] ) * motorEngine[Y_SERVO_MOTOR].getResolution();
			motorYScanningPositionTHR[i] = Round(motorStartScanPos[Y_SERVO_MOTOR] + nDirectionYTop * dStepYTop);
		}


//		nDieId = (nDeviceRow % 2 == 0) ? (nCol / nFOVPerDieinXTHR * 2 + 1) : (nCol / nFOVPerDieinXTHR * 2);
		
		for (int i = 0; i < nScanRowNoTHR; i++) {
			for (int j = 0; j < m_nNoOfBlock[0] ; j++) {
				int nDieNo = (i / nFOVPerDieinYTHR) ? j*2 : j*2 + 1;
				if (theApp.m_bZoneMapping == 1)
					nDieNo = m_nNoOfBlock[0] * m_nNoOfBlock[1] - nDieNo -1;

				int nDieID = i / nFOVPerDieinYTHR;
				int nFOVIDPerDie = i % nFOVPerDieinYTHR;
				dStepYTop = (nDieID * dDiePerYPitchMM + nFOVIDPerDie * dFOVPerYPitchMM - ((dOffsetYScanTHRMicron[nDieNo]) / 1000) + nDieOffsetY[nDieID] ) * motorEngine[Y_SERVO_MOTOR].getResolution();
				if (i % 2 == 0) {
					motorYScanningDatumPositiom[i * m_nNoOfBlock[0] + j] = Round(motorStartScanPos[Y_SERVO_MOTOR] + nDirectionYTop * dStepYTop);
//					OutputDebugLogTo(6, TRUE, "Die %d, Y value[%d] = %d, Previous Val %d", nDieNo, i * m_nNoOfBlock[0] + j, motorYScanningDatumPositiom[i * m_nNoOfBlock[0] + j], motorStartScanPos[Y_SERVO_MOTOR]);
				}
				else {
					motorYScanningDatumPositiom[i * m_nNoOfBlock[0] + (m_nNoOfBlock[0] - 1 - j)] = Round(motorStartScanPos[Y_SERVO_MOTOR] + nDirectionYTop * dStepYTop);
//					OutputDebugLogTo(6, TRUE, "nDieNo %d, Y value[%d] = %d Previous Val %d", nDieNo, i * m_nNoOfBlock[0] + (m_nNoOfBlock[0] - 1 - j),  motorYScanningDatumPositiom[i * m_nNoOfBlock[0] + (m_nNoOfBlock[0] - 1 - j)], motorStartScanPos[Y_SERVO_MOTOR]);
				}
			}
			motorYScanningPositionTHR[i] = motorYScanningDatumPositiom[i * m_nNoOfBlock[0]];
//			OutputDebugLogTo(7, TRUE, "Y valuse %d to %d", i * m_nNoOfBlock[0], motorYScanningDatumPositiom[i * m_nNoOfBlock[0]]);

		}
	}

	_zFocussingPositionTHR.resize(nScanColumnNoTHR * nScanRowNoTHR);
	std::fill(_zFocussingPositionTHR.begin(), _zFocussingPositionTHR.end(), 0);

	int nDirectionZ = 1;

	double value = 0.0;

	vector <double> zValues;

	_devicePresence.resize(m_nNoOfBlock[0] * m_nNoOfBlock[1]);
	std::fill(_devicePresence.begin(), _devicePresence.end(), true);
	std::fill(_zSensorMeasuredDistance.begin(), _zSensorMeasuredDistance.end(), 0);

	for(int i = 0; i < _zSensorEachFovDistance.size(); i++) {
		zValues = _zSensorEachFovDistance[i];

		zValues.erase(std::remove_if(zValues.begin(), zValues.end(), [](const double& x) { return x <= 0.0; }), zValues.end());



		if (zValues.size() > 0) {
			auto begin = zValues.begin();
			auto end = zValues.end();
			if (i%nFOVPerDieinXTHR == 0)
				begin += zValues.size() * 8 / 9;
			else if (i%nFOVPerDieinXTHR == nFOVPerDieinXTHR - 1)
				end -= zValues.size() * 5 / 6;

			_zSensorMeasuredDistance[i] = std::accumulate(begin, end, 0.0) / (end - begin);
			OutputDebugLogTo(5, TRUE,"PositionId %d -> Avg Value %3.06f", i, _zSensorMeasuredDistance[i]);
		}
		else {

			_zSensorMeasuredDistance[i] = 0;
		//	OutputDebugLogTo(5, TRUE, "PositionId %d->, Teach Value %3.06f ", i, _zSensorMeasuredDistance[i]);
		}
	}


	int	 nCount = 0;
	for(int i = 0; i < _zSensorMeasuredDistance.size(); i++) {

		if (i % nFOVPerDieinXTHR == 0)
			nCount = 0;

		double dZSensorDefaultTeachValue = m_dZSensorTeachValue;

		if (_zSensorMeasuredDistance[i] <= 0 || _zSensorMeasuredDistance[i] >= 10) {
			_zSensorMeasuredDistance[i] = dZSensorDefaultTeachValue;

			nCount++;
			if (nCount >= nFOVPerDieinXTHR - 1) {
				_devicePresence[dieNoTop[i / 4]] = false;
			}
			OutputDebugLogTo(5, TRUE, "PositionId %d: Invalid Z Sensor Measured Value", i);
		}
		
		if (i % nFOVPerDieinXTHR == 0) {
			if (i == _zSensorMeasuredDistance.size() - 1) {
				_zSensorMeasuredDistance[i] = dZSensorDefaultTeachValue;
			}
			else {
				_zSensorMeasuredDistance[i] = (_zSensorMeasuredDistance[i + 1] != 0) ? _zSensorMeasuredDistance[i + 1] : dZSensorDefaultTeachValue;
			}
		}

		if (i % nFOVPerDieinXTHR == (nFOVPerDieinXTHR - 1)) {
			if (i == 0) {
				_zSensorMeasuredDistance[i] = dZSensorDefaultTeachValue;
			}
			else {
				_zSensorMeasuredDistance[i] = (_zSensorMeasuredDistance[i - 1] != 0) ? _zSensorMeasuredDistance[i - 1] : dZSensorDefaultTeachValue;
			}
		}
		//OutputDebugLogTo(5, TRUE, "PositionId %d -> After Validated %3.06f ", i, _zSensorMeasuredDistance[i]);
	}
	std::vector <int > nZFocussingPositionTHRDieID;
	nZFocussingPositionTHRDieID.resize(nScanColumnNoTHR * nScanRowNoTHR);
	std::fill(nZFocussingPositionTHRDieID.begin(), nZFocussingPositionTHRDieID.end(), 0);
	int nGrabId = 0;
	for(int i = 0; i < nScanRowNoTHR; i++) {

		nDieIDY = i / nFOVPerDieinYTHR;
		nImageIDPerDieY = i % nFOVPerDieinYTHR;

		for(int j = 0; j < triggerCountPerRowTHR[i]; j++, nGrabId++) {
			
			nDieIDX = j / nFOVPerDieinXTHR;
			nImageIDPerDieX = j % nFOVPerDieinXTHR;

			int nIndex = 0;
			if ((nDieIDY % 2 == 0) ? (i % 2 == 0) : (i % 2 != 0)) {
				nIndex = i * nScanColumnNoTHR + j;
				_zFocussingPositionTHR[nIndex] = Round(motorStartScanPos[ST2_Z_SERVO_MOTOR] + (nDirectionZ * (m_dZSensorTeachValue - _zSensorMeasuredDistance[nDieIDY *nScanColumnNoTHR + j])) * motorEngine[ST2_Z_SERVO_MOTOR].getResolution());
			}
			else {
				nIndex = i * nScanColumnNoTHR + (nScanColumnNoTHR - 1 - j);
				_zFocussingPositionTHR[nIndex] = Round(motorStartScanPos[ST2_Z_SERVO_MOTOR] + (nDirectionZ *(m_dZSensorTeachValue - _zSensorMeasuredDistance[nDieIDY *nScanColumnNoTHR + j])) * motorEngine[ST2_Z_SERVO_MOTOR].getResolution());
			}
		}
	}

	//Validate soft/positive limits, if out of soft limits, just move to teach position.
	for (int i = 0; i < _zFocussingPositionTHR.size(); i++) {
		if (_zFocussingPositionTHR[i] >= motorEngine[ST2_Z_SERVO_MOTOR].getPositiveSoftLimit() || _zFocussingPositionTHR[i] <= motorEngine[ST2_Z_SERVO_MOTOR].getNegativeSoftLimit()) {
			_zFocussingPositionTHR[i] = motorStartScanPos[ST2_Z_SERVO_MOTOR];
		}
	}
	
	/*for (int nDevice = 0; nDevice < m_nNoOfBlock[0] * m_nNoOfBlock[1]; nDevice++) {
		if (_devicePresence[nDevice] == false) {
			for (int nRow = 0; nRow < nScanRowNoTHR; nRow++) {
				for (int nCol = 0; nCol < nScanColumnNoTHR; nCol++) {
					long nCameraId;
					int nDieId;
					GetCameraGrabIDTHR(0, nRow, nCol, nDevice, nCameraId);
					getCameraGrabIdTHR(nCameraId, nDieId);
					theApp.m_pImageSelectionDlg->SetDeviceStatus(nDieId, MISSING_DIE, false);
				}
			}
		}
	}*/


	//Debug
	CString strZLogPath = "Z Logs";
	_mkdir(strZLogPath);
	for (int nZPositionId = 0; nZPositionId < _zSensorEachFovDistance.size(); nZPositionId++) {
		CString strLogFileName;
		int nDieID = nZPositionId + 1;
		if ((nZPositionId / nScanColumnNoTHR) % 2 == 1) {
			nDieID = ((nZPositionId / nScanColumnNoTHR) + 1) * nScanColumnNoTHR - (nZPositionId - nScanColumnNoTHR);
		}
		strLogFileName.Format("%s\\ZPosition %d.log", strZLogPath, nDieID);
		std::ofstream zLogFile(strLogFileName);
		for (int i = 0; i < _zSensorEachFovDistance[nZPositionId].size(); i++) {
			zLogFile << _zSensorEachFovDistance[nZPositionId][i] << endl;
		}
		zLogFile.close();
	}
	//Debug
	
	return 0;
}


int MotionParameters::getImageMappingDetails(int unitId, int &nRowId, int &nColId, int &nDieId, int &nFov)
{
	if(unitId < 0 || unitId >= nScanColumnNoTop * nScanRowNoTop)
		return -1;

	int nRow, nCol;
	int nDeviceRow;

	nRow = unitId / nScanColumnNoTop;
	nCol = unitId % nScanColumnNoTop;

	nRowId = nRow % nFOVPerDieinYTOP;

	nColId = nCol;

	nFov = nRowId * nFOVPerDieinXTOP + (nCol % nFOVPerDieinXTOP);
	nDeviceRow = nRow / nFOVPerDieinYTOP;

	int nImageIDPerDieX = nCol / nFOVPerDieinXTOP;

	nDieId = nDeviceRow * m_nNoOfBlock[0] + nImageIDPerDieX;
	
	return 0;
}

bool MotionParameters::GetDieMappingNo(int nDeviceID, int &DieMappingId)
{
	if (nDeviceID < 0 || nDeviceID >= nScanColumnNoTHR * nScanRowNoTHR)
		return -1;

	int nRow, nCol;

	nRow = nDeviceID / m_nNoOfBlock[0];
	nCol = nDeviceID % m_nNoOfBlock[0];

	DieMappingId = (nRow % 2 == 0) ? (nCol * m_nNoOfBlock[1] + 1) : (nCol * m_nNoOfBlock[1]);
	if (theApp.m_bZoneMapping)
		DieMappingId = m_nNoOfBlock[0] * m_nNoOfBlock[1] - DieMappingId - 1;
	return true;
}


bool MotionParameters::nDisplayDieNo(int unitId, int &nDieId)
{
	if (unitId < 0 || unitId >= nScanColumnNoTop * nScanRowNoTop)
		return -1;

	int nRow, nCol;
	int nDeviceRow;

	/*nRow = unitId / nScanColumnNoTHR;
	nCol = unitId % nScanColumnNoTHR;*/

	nRow = unitId / nScanColumnNoTop;
	nCol = unitId % nScanColumnNoTop;

	nDeviceRow = nRow / nFOVPerDieinYTOP;

	if (unitId % nFOVPerDieinXTOP == 0 && nRow % nFOVPerDieinYTOP == 0) {
		int nImageIDPerDieX = nCol / nFOVPerDieinXTOP;

		nDieId = nDeviceRow * m_nNoOfBlock[0] + nImageIDPerDieX;

	
		return true;
	}
	
	return false;
}

int MotionParameters::getInspectionStatus(int nTrack, int nFOV, bool &bInspectionStaus)
{
	if(nTrack == STATION_TOP) {
		bInspectionStaus = bInspectionStatusTop[nFOV];
	}
	else if(nTrack == STATION_THR) {
		bInspectionStaus = bInspectionStatusTHR[nFOV];
	}
	return 0;
}

int MotionParameters::setInspectionStatus(int nTrack, int nFOV, bool bInspectionStaus)
{
	if(nTrack == STATION_TOP) {
		bInspectionStatusTop[nFOV] = bInspectionStaus;
	}
	else if(nTrack == STATION_THR) {
		bInspectionStatusTHR[nFOV] = bInspectionStaus;
	}
	return 0;
}

int MotionParameters::setInspectionEnableTHR(int nCameraGrabId, bool bEnable)
{
	if (nCameraGrabId < 0 || nCameraGrabId >= nTotalImageFOVTHR)
		return -1;

	enableInspectionTHR[nCameraGrabId] = bEnable;
	return 0;
}

int MotionParameters::getInspectionEnableStatusTHR(int nCameraGrabId, bool &bEnable)
{
	if (nCameraGrabId < 0 || nCameraGrabId >= nTotalImageFOVTHR)
		return -1;

	bEnable = enableInspectionTHR[nCameraGrabId];
	return 0;
}

int MotionParameters::getInspectionWaitStatus(int nTrack, bool &bWaitforInsp)
{
	if (nTrack == STATION_THR) {
		std::vector <bool> ::const_iterator v = std::find_if(enableInspectionTHR.begin(), enableInspectionTHR.end(), [](const bool &enable) {return enable == true;  });
		if (v != enableInspectionTHR.end()) {
			bWaitforInsp = true;
			return 0;
		}
	}
	/*else if (nTrack == STATION_TOP) {
		std::vector <bool> ::const_iterator v = std::find_if(enableInspectionTop.begin(), enableInspectionTHR.end(), true);
		if (v != enableInspectionTHR.end())
			bWaitforInsp = true;	
	}*/

	bWaitforInsp = false;
	return 0;
}

int MotionParameters::resetInspection()
{
	std::fill(enableInspectionTHR.begin(), enableInspectionTHR.end(), true);
	std::fill(bInspectionStatusTHR.begin(), bInspectionStatusTHR.end(), false);
	std::fill(bInspectionStatusTop.begin(), bInspectionStatusTop.end(), false);

	return 0;
}

int MotionParameters::getMappingDieIdTOP(int nCameraIdTOP, int &nDieId)
{
	for (int i = 0; i < scanningDiemappingIdTOP.size(); i++) {
		if (scanningDiemappingIdTOP[i] == nCameraIdTOP) {
			nDieId = i;
			return 0;
		}
	}
	return -1;
}

int MotionParameters::getMappingDieIdTHR(int nCameraIdTHR, int &nDieId)
{
	for (int i = 0; i < scanningDiemappingId.size(); i++) {
		if (scanningDiemappingId[i] == nCameraIdTHR) {
			nDieId = i;
			return 0;
		}
	}
	return -1;
}

int MotionParameters::getCameraGrabIdTHR(int nDieId, int &nCameraIdTHR)
{
	if (nDieId < 0 || nDieId >= scanningDiemappingId.size())
		return -1;

	nCameraIdTHR = scanningDiemappingId[nDieId];

	return 0;
}

int MotionParameters::getCameraGrabIdTop(int nDieId, long &nCameraIdTop)
{
	if (nDieId < 0 || nDieId >= nTotalImageFOVTOP)
		return -1;

	nCameraIdTop = scanningDiemappingIdTOP[nDieId];

	return 0;
}

int MotionParameters::getTopDieMappingId(int nImageId, std::vector <int> & dieNumbers)
{
	if (nImageId < 0 || nImageId > nTotalImageFOVTOP)
		return -1;
	int nCount = 0;
	for (int i = 0; i < nTotalImageFOVTHR; i++) {
		if (TopToTHRDieMapping[i] == nImageId) {
			dieNumbers.push_back(i);
			nCount++;
		}
		if (nCount > 12)
			break;
	}

	return 0;
}

int MotionParameters::getTopImageId(int nDieId, int &nImageId)
{
	if (nDieId < 0 || nDieId > TopToTHRDieMapping.size())
		return -1;

	nImageId = TopToTHRDieMapping[nDieId];

	return 0;
}

int MotionParameters::getScanColumnNoTHR() const
{
	return nScanColumnNoTHR;
}

int MotionParameters::getScanRowNoTHR() const
{
	return nScanRowNoTHR;
}

int MotionParameters::getScanColumnNoTop() const
{
	return nScanColumnNoTop;
}

int MotionParameters::getScanRowNoTop() const
{
	return nScanRowNoTop;
}

int MotionParameters::getScanColumnNoZSensor() const
{
	return nScanColumnNoZSensor;
}

int MotionParameters::getScanRowNoZSensor() const
{
	return nScanRowNoZSensor;
}

int MotionParameters::GetXMotorStartPosition(int nRow, int & nXPos) const
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTHR)
		return -1;

	nXPos = motorXStartScanningPositionTHR[nRow];

	return 0;
}

int MotionParameters::GetXMotorEndPosition(int nRow, int & nXPos) const
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTHR)
		return -1;

	nXPos = motorXEndScanningPositionTHR[nRow];

	return 0;
}


int MotionParameters::GetYTopMotorPositionTHR(int nRow, int & nYPos) const
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTHR)
		return -1;

	nYPos = motorYScanningPositionTHR[nRow];

	return 0;
}

int MotionParameters::GetYTopMotorPositionTop(int nRow, int & nYPos) const
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTop)
		return -1;

	nYPos = motorYScanningPositionTop[nRow];

	return 0;
}

int MotionParameters::MoveXMotorToSafePos(long nPos)
{
	return AbsoluteMove(X_SERVO_MOTOR, nPos, getMovingSpeed(X_SERVO_MOTOR), false);
}

int MotionParameters::MoveYMotorToSafePos(long nPos)
{
	return AbsoluteMove(Y_SERVO_MOTOR, nPos, getMovingSpeed(Y_SERVO_MOTOR), false);

}

int MotionParameters::MoveZMotorToSafePos(long nPos)
{
	return AbsoluteMove(ST1_Z_SERVO_MOTOR, nPos, getMovingSpeed(ST1_Z_SERVO_MOTOR), false);

}

int MotionParameters::MoveXMotorToStartPosTHR(int nRow, Speed speed)
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTHR)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0)
		return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTHR[nRow], speed, false);
	else
		return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTHR[nRow], (nRow == (nScanRowNoTHR - 1)) ? getMovingSpeed(X_SERVO_MOTOR) : getScanningSpeedTHR(X_SERVO_MOTOR), false);
}

int MotionParameters::MoveXMotorToEndPosTHR(int nRow, Speed speed)
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTHR)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0) {
		if(nRow == nScanRowNoTHR - 1) {
			return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTHR[nRow], speed, false);
		}
		else {
			if(abs(motorXEndScanningPositionTHR[nRow] - motorXStartScanningPositionTHR[nRow]) > abs(motorXStartScanningPositionTHR[nRow + 1] - motorXStartScanningPositionTHR[nRow])) {
				return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTHR[nRow], speed, false);
			}
			else {
				return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTHR[nRow + 1], speed, false);
			}
		}
	}
	else {

		if(nRow == nScanRowNoTHR - 1) {
			return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTHR[nRow], getScanningSpeedTHR(X_SERVO_MOTOR), false);
		}
		else {
			if(abs(motorXEndScanningPositionTHR[nRow] - motorXStartScanningPositionTHR[nRow]) > abs(motorXStartScanningPositionTHR[nRow + 1] - motorXStartScanningPositionTHR[nRow])) {
				return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTHR[nRow], getScanningSpeedTHR(X_SERVO_MOTOR), false);
			}
			else {
				return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTHR[nRow + 1], getScanningSpeedTHR(X_SERVO_MOTOR), false);
			}
		}

	}
}


int MotionParameters::MoveXMotorToStartPosTop(int nRow, Speed speed)
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTop)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0)
		return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTop[nRow], speed, false);
	else
		return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTop[nRow], (nRow == 0) ? getMovingSpeed(X_SERVO_MOTOR) : getScanningSpeed(X_SERVO_MOTOR), false);
}

int MotionParameters::MoveXMotorToEndPosTop(int nRow, Speed speed)
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTop)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0) {
		if(nRow == nScanRowNoTop - 1) {
			return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTop[nRow], speed, false);
		}
		else {
			if(abs(motorXEndScanningPositionTop[nRow] - motorXStartScanningPositionTop[nRow]) > abs(motorXStartScanningPositionTop[nRow + 1] - motorXStartScanningPositionTop[nRow])) {
				return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTop[nRow], speed, false);
			}
			else {
				return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTop[nRow + 1], speed, false);
			}
		}
	}
	else {

		if(nRow == nScanRowNoTop - 1) {
			return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTop[nRow], getScanningSpeed(X_SERVO_MOTOR), false);
		}
		else {
			if(abs(motorXEndScanningPositionTop[nRow] - motorXStartScanningPositionTop[nRow]) > abs(motorXStartScanningPositionTop[nRow + 1] - motorXStartScanningPositionTop[nRow])) {
				return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTop[nRow], getScanningSpeed(X_SERVO_MOTOR), false);
			}
			else {
				return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTop[nRow + 1], getScanningSpeed(X_SERVO_MOTOR), false);
			}
		}

	}

}

int MotionParameters::MoveXMotorToStartPosTopEncap(int nRow, Speed speed)
{
	if (!scanningReady || nRow < 0 || nRow >= nScanRowNoTop)
		return -1;

	if (speed.startSpeed > 0 && speed.maximumSpeed > 0)
		return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTopEncap[nRow], speed, false);
	else
		return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTopEncap[nRow], (nRow == 0) ? getMovingSpeed(X_SERVO_MOTOR) : getScanningSpeed(X_SERVO_MOTOR), false);
}

int MotionParameters::MoveXMotorToEndPosTopEncap(int nRow, Speed speed)
{
	if (!scanningReady || nRow < 0 || nRow >= nScanRowNoTop)
		return -1;

	if (speed.startSpeed > 0 && speed.maximumSpeed > 0) {
		if (nRow == nScanRowNoTop - 1) {
			return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTopEncap[nRow], speed, false);
		}
		else {
			if (abs(motorXEndScanningPositionTopEncap[nRow] - motorXStartScanningPositionTopEncap[nRow]) > abs(motorXStartScanningPositionTopEncap[nRow + 1] - motorXStartScanningPositionTopEncap[nRow])) {
				return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTopEncap[nRow], speed, false);
			}
			else {
				return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTopEncap[nRow + 1], speed, false);
			}
		}
	}
	else {

		if (nRow == nScanRowNoTop - 1) {
			return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTopEncap[nRow], getScanningSpeed(X_SERVO_MOTOR), false);
		}
		else {
			if (abs(motorXEndScanningPositionTopEncap[nRow] - motorXStartScanningPositionTopEncap[nRow]) > abs(motorXStartScanningPositionTopEncap[nRow + 1] - motorXStartScanningPositionTopEncap[nRow])) {
				return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTopEncap[nRow], getScanningSpeed(X_SERVO_MOTOR), false);
			}
			else {
				return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTopEncap[nRow + 1], getScanningSpeed(X_SERVO_MOTOR), false);
			}
		}

	}
}


int MotionParameters::MoveXMotorToStartPosZ(int nRow, Speed speed)
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoZSensor)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0)
		return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionZ[nRow], speed, false);
	else
		return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionZ[nRow], (nRow == 0) ? getMovingSpeed(X_SERVO_MOTOR) : getScanningSpeed(X_SERVO_MOTOR), false);
}

int MotionParameters::MoveXMotorToStartPosTopnZ(int nRow, Speed speed)
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoZSensor)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0)
		return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTopandZ[nRow], speed, false);
	else
		return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTopandZ[nRow], (nRow == 0) ? getScanningSpeed(X_SERVO_MOTOR) : getScanningSpeed(X_SERVO_MOTOR), false);
}




int MotionParameters::MoveXMotorToEndPosZ(int nRow, Speed speed)
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoZSensor)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0) {
		if(nRow == nScanRowNoZSensor - 1) {
			return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionZ[nRow], speed, false);
		}
		else {
			if(abs(motorXEndScanningPositionZ[nRow] - motorXStartScanningPositionZ[nRow]) > abs(motorXStartScanningPositionZ[nRow + 1] - motorXStartScanningPositionZ[nRow])) {
				return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionZ[nRow], speed, false);
			}
			else {
				return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionZ[nRow + 1], speed, false);
			}
		}
	}
	else {

		if(nRow == nScanRowNoZSensor - 1) {
			return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionZ[nRow], getScanningSpeed(X_SERVO_MOTOR), false);
		}
		else {
			if(abs(motorXEndScanningPositionZ[nRow] - motorXStartScanningPositionZ[nRow]) > abs(motorXStartScanningPositionZ[nRow + 1] - motorXStartScanningPositionZ[nRow])) {
				return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionZ[nRow], getScanningSpeed(X_SERVO_MOTOR), false);
			}
			else {
				return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionZ[nRow + 1], getScanningSpeed(X_SERVO_MOTOR), false);
			}
		}

	}

}

int MotionParameters::MoveXMotorToEndPosTopnZ(int nRow, Speed speed)
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTop)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0) {
		if(nRow == nScanRowNoTop - 1) {
			return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTopandZ[nRow], speed, false);
		}
		else {
			if(abs(motorXEndScanningPositionTopandZ[nRow] - motorXStartScanningPositionTopandZ[nRow]) > abs(motorXStartScanningPositionTopandZ[nRow + 1] - motorXStartScanningPositionTopandZ[nRow])) {
				return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTopandZ[nRow], speed, false);
			}
			else {
				return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTopandZ[nRow + 1], speed, false);
			}
		}
	}
	else {
		if(nRow == nScanRowNoTop - 1) {
			return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTopandZ[nRow], getScanningSpeed(X_SERVO_MOTOR), false);
		}
		else {
			if(abs(motorXEndScanningPositionTopandZ[nRow] - motorXStartScanningPositionTopandZ[nRow]) > abs(motorXStartScanningPositionTopandZ[nRow + 1] - motorXStartScanningPositionTopandZ[nRow])) {
				return AbsoluteMove(X_SERVO_MOTOR, motorXEndScanningPositionTopandZ[nRow], getScanningSpeed(X_SERVO_MOTOR), false);
			}
			else {
				return AbsoluteMove(X_SERVO_MOTOR, motorXStartScanningPositionTopandZ[nRow + 1], getScanningSpeed(X_SERVO_MOTOR), false);
			}
		}

	}

}


int MotionParameters::MoveXMotorToZSensorTeachPos(int nDie, Speed speed)
{
	if (!scanningReady || nDie < 0 || nDie >= MAX_DIES)
		return -1;

	if (speed.startSpeed > 0 && speed.maximumSpeed > 0) {
		return AbsoluteMove(X_SERVO_MOTOR, motorZSensorTeachPos[nDie][X_SERVO_MOTOR], speed, false);
	}
	else {
		return AbsoluteMove(X_SERVO_MOTOR, motorZSensorTeachPos[nDie][X_SERVO_MOTOR], getScanningSpeed(X_SERVO_MOTOR), false);
	}
}


int MotionParameters::MoveTOPZMotorToReadyPos(int nFov, Speed speed)
{
	if (nFov < 0 || nFov >= MAX_FOV_TOP)
		return -1;

	if (speed.startSpeed > 0 && speed.maximumSpeed > 0)
		return AbsoluteMove(ST1_Z_SERVO_MOTOR, motorTOPStartScanPos[nFov][ST1_Z_SERVO_MOTOR], speed, false);
	else
		return AbsoluteMove(ST1_Z_SERVO_MOTOR, motorTOPStartScanPos[nFov][ST1_Z_SERVO_MOTOR], getScanningSpeed(ST1_Z_SERVO_MOTOR), false);
}


int MotionParameters::MoveZMotorToReadyPos(Speed speed)
{

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0)
		return AbsoluteMove(ST2_Z_SERVO_MOTOR, motorStartScanPos[ST2_Z_SERVO_MOTOR], speed, false);
	else
		return AbsoluteMove(ST2_Z_SERVO_MOTOR, motorStartScanPos[ST2_Z_SERVO_MOTOR], getScanningSpeed(ST2_Z_SERVO_MOTOR), false);
}


int MotionParameters::MoveZMotorToReadyPosTHR(Speed speed)
{

	if(_zFocussingPositionTHR[0] != 0) {
		if(speed.startSpeed > 0 && speed.maximumSpeed > 0)
			return AbsoluteMove(ST2_Z_SERVO_MOTOR, _zFocussingPositionTHR[0], speed, false);
		else
			return AbsoluteMove(ST2_Z_SERVO_MOTOR, _zFocussingPositionTHR[0], getScanningSpeedTHR(ST2_Z_SERVO_MOTOR), false);
	}
	else {
		if(speed.startSpeed > 0 && speed.maximumSpeed > 0)
			return AbsoluteMove(ST2_Z_SERVO_MOTOR, motorStartScanPos[ST2_Z_SERVO_MOTOR], speed, false);
		else
			return AbsoluteMove(ST2_Z_SERVO_MOTOR, motorStartScanPos[ST2_Z_SERVO_MOTOR], getScanningSpeed(ST2_Z_SERVO_MOTOR), false);

	}
}

int MotionParameters::MoveZMotorToFocussingPos(int nCameraGrabId, Speed speed)
{
	if(!scanningReady || nCameraGrabId < 0 || nCameraGrabId >= nScanRowNoTHR * nScanColumnNoTHR)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0)
		return AbsoluteMove(ST2_Z_SERVO_MOTOR, _zFocussingPositionTHR[nCameraGrabId], speed, false);
	else
		return AbsoluteMove(ST2_Z_SERVO_MOTOR, _zFocussingPositionTHR[nCameraGrabId], getScanningSpeedTHR(ST2_Z_SERVO_MOTOR), false);
}

int MotionParameters::MoveYMotorDatumOffsetPos(int nCameraGrabId, Speed speed)
{
	if (!scanningReady || nCameraGrabId < 0 || nCameraGrabId >= nScanRowNoTHR * nScanColumnNoTHR)
		return -1;

	OutputDebugLogTo(7, TRUE, "Y valuse %d to %d", nCameraGrabId / nFOVPerDieinXTHR, motorYScanningDatumPositiom[nCameraGrabId/ nFOVPerDieinXTHR]);

	if (speed.startSpeed > 0 && speed.maximumSpeed > 0)
		return AbsoluteMove(Y_SERVO_MOTOR, motorYScanningDatumPositiom[nCameraGrabId / nFOVPerDieinXTHR], speed, false);
	else
		return AbsoluteMove(Y_SERVO_MOTOR, motorYScanningDatumPositiom[nCameraGrabId / nFOVPerDieinXTHR], getScanningSpeed(Y_SERVO_MOTOR), false);

}

int MotionParameters::MoveYTopMotorTHR(int nRow, Speed speed)
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTHR)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0) {
		return AbsoluteMove(Y_SERVO_MOTOR, motorYScanningPositionTHR[nRow], speed, false);
	}
	else {
		return AbsoluteMove(Y_SERVO_MOTOR, motorYScanningPositionTHR[nRow], (nRow == (nScanRowNoTHR - 1)) ? getMovingSpeed(Y_SERVO_MOTOR) : getScanningSpeed(Y_SERVO_MOTOR), false);
	}
}

int MotionParameters::MoveYTopMotorTop(int nRow, Speed speed)
{
	if(!scanningReady || nRow < 0 || nRow >= nScanRowNoTop)
		return -1;

	if(speed.startSpeed > 0 && speed.maximumSpeed > 0) {
		return AbsoluteMove(Y_SERVO_MOTOR, motorYScanningPositionTop[nRow], speed, false);
	}
	else {
		return AbsoluteMove(Y_SERVO_MOTOR, motorYScanningPositionTop[nRow], (nRow == 0) ? getMovingSpeed(Y_SERVO_MOTOR) : getScanningSpeed(Y_SERVO_MOTOR), false);
	}
}

int MotionParameters::MoveYTopMotorTopEncap(int nRow, Speed speed)
{
	if (!scanningReady || nRow < 0 || nRow >= nScanRowNoTop)
		return -1;

	if (speed.startSpeed > 0 && speed.maximumSpeed > 0) {
		return AbsoluteMove(Y_SERVO_MOTOR, motorYScanningPositionTopEncap[nRow], speed, false);
	}
	else {
		return AbsoluteMove(Y_SERVO_MOTOR, motorYScanningPositionTopEncap[nRow], (nRow == 0) ? getMovingSpeed(Y_SERVO_MOTOR) : getScanningSpeed(Y_SERVO_MOTOR), false);
	}
}

int MotionParameters::getMotorInitPosition(int nAxis) const
{
	return motorInitScanPos[nAxis];
}

int MotionParameters::getMotorModuleUnloadPosition(int nAxis) const
{
	return motorModuleUnloadPos[nAxis];
}

int MotionParameters::GetCameraGrabIDTHR(int nImageID, int nRowID, int nColumnID, int nDieId, long & nCameraGrabID) const
{
	//int nDeviceCol = nDieId % 2 ? 0 : 1;
	for(int i = 0; i<nTotalImageFOVTHR; i++) {
		if(/*scanning2D3DImageId[i] == nImageID &&*/dieNoTHR[i] == nDieId && dieColumnNoTHR[i] == nColumnID && dieRowNoTHR[i] == nRowID) {
			nCameraGrabID = i;
			return 0;
		}
	}
	return 0;
}

int MotionParameters::GetCameraGrabIDTHR(int nImageID, int nDieId, int nFov, long & nCameraGrabID) const
{
	//int nDeviceCol = nDieId % 2 ? 0 : 1;
	for (int i = 0; i<nTotalImageFOVTHR; i++) {
		if (/*scanning2D3DImageId[i] == nImageID &&*/dieNoTHR[i] == nDieId && dieFOVTHR[i] == nFov) {
			nCameraGrabID = i;
			return 0;
		}
	}
	return 0;
}


int MotionParameters::GetDeviceDetailsTHR(int nCameraGrabID, int &nRowID, int &nColumnID, int &nDieId, int &nDieFOV) const
{
	if(nCameraGrabID >= nTotalImageFOVTHR || nCameraGrabID < 0)
		return -1;

	nRowID = dieRowNoTHR[nCameraGrabID];
	nColumnID = dieColumnNoTHR[nCameraGrabID];
	nDieId = dieNoTHR[nCameraGrabID];
	nDieFOV = dieFOVTHR[nCameraGrabID];

	return 0;
}

int MotionParameters::GetCameraGrabIDTOP(int nImageID, int nRowID, int nColumnID, int nDieId, long & nCameraGrabID) const
{
	//int nDeviceCol = nDieId % 2 ? 0 : 1;
	for(int i = 0; i<nTotalImageFOVTOP; i++) {
		if(/*scanning2D3DImageId[i] == nImageID &&*/dieNoTop[i] == nDieId && dieColumnNoTop[i] == nColumnID && dieRowNoTop[i] == nRowID) {
			nCameraGrabID = i;
			return 0;
		}
	}
	return 0;
}

int MotionParameters::GetDeviceDetailsTOP(int nCameraGrabID, int &nRowID, int &nColumnID, int &nDieId, int &nDieFOV)
{
	if(nCameraGrabID >= nTotalImageFOVTOP || nCameraGrabID < 0)
		return -1;

	nRowID = dieRowNoTop[nCameraGrabID];
	nColumnID = dieColumnNoTop[nCameraGrabID];
	nDieId = dieNoTop[nCameraGrabID];
	nDieFOV = dieFOVTop[nCameraGrabID];

	return 0;
}

int MotionParameters::GetTopCameraGrabID(int nCameraGrabID, long &nTopGarbId)
{
	if (nCameraGrabID >= nTotalImageFOVTHR || nCameraGrabID < 0)
		return -1;
	

	int nColNo, nRowNo, nDieNo, nFov;
	int nError = GetDeviceDetailsTHR(nCameraGrabID, nRowNo, nColNo, nDieNo, nFov);
	if (nError)
		return nError;

	nError = GetCameraGrabIDTOP(0, nRowNo / nFOVPerDieinYTHR, (nColNo/ (nFOVPerDieinXTHR / nFOVPerDieinXTOP)), nDieNo, nTopGarbId);

	/*int nDeviceY = (nDieNo / m_nNoOfBlock[0]);     nFOVPerDieinXTOP;

	nTopGarbId = (nDeviceY %2 == 0 )? nDieNo * nFOVPerDieinXTOP + nColNo / (nFOVPerDieinXTHR / nFOVPerDieinXTOP):
		(nScanColumnNoTop + (nScanColumnNoTop - nDieNo * nFOVPerDieinXTOP) + nColNo / (nFOVPerDieinXTHR / nFOVPerDieinXTOP) - 1);*/

	return 0;
}

int MotionParameters::IsCameraGrabIDLastOnDieRow(long nCameraGrabNo, bool &bLast)
{
	if(nCameraGrabNo < 0)
		return -1;

	long nTotalGrabs = 0;

	bLast = (((nCameraGrabNo + 1) % nFOVPerDieinXTHR == 0)) && (nCameraGrabNo > 0);

	return 0;

}


int MotionParameters::IsCameraGrabIDLastOnRowTHR(long nCameraGrabNo, bool& bLast)
{
	if(!scanningReady || nCameraGrabNo < 0)
		return -1;

	long nTotalGrabs = 0;

	for(int i = 0; i<nScanRowNoTHR; i++) {
		if(nCameraGrabNo > nTotalGrabs)
			nTotalGrabs = nTotalGrabs + triggerCountPerRowTHR[i];
		else
			break;
	}

	bLast = (nCameraGrabNo == nTotalGrabs) && (nCameraGrabNo > 0);

	return 0;
}


int MotionParameters::IsCameraGrabIDLastOnRowTop(long nCameraGrabNo, bool &bLast)
{
	if(!scanningReady || nCameraGrabNo < 0)
		return -1;

	long nTotalGrabs = 0;

	for(int i = 0; i<nScanRowNoTop; i++) {
		if(nCameraGrabNo > nTotalGrabs)
			nTotalGrabs = nTotalGrabs + triggerCountPerRowTOP[i];
		else
			break;
	}

	bLast = (nCameraGrabNo == nTotalGrabs) && (nCameraGrabNo > 0);

	return 0;
}

int MotionParameters::IsCameraGrabIDLastOnRowZSensor(long nCameraGrabNo, bool &bLast)
{
	if(!scanningReady || nCameraGrabNo < 0)
		return -1;

	long nTotalGrabs = 0;

	for(int i = 0; i<nScanRowNoZSensor; i++) {
		if(nCameraGrabNo > nTotalGrabs)
			nTotalGrabs = nTotalGrabs + _zSensortriggerCountPerRow[i];
		else
			break;
	}

	bLast = (nCameraGrabNo == nTotalGrabs) && (nCameraGrabNo > 0);
	return 0;
}

int MotionParameters::getFOVIndex(long nGrabId, int &nFOV)
{
	
	return 0;
}

int MotionParameters::getRealGrabId( CString strImageFileName, int &nStation, int &nDocumentID, int & nRealGrabID)
{
	int nColumnID, nRowID, nDieID, nFOV;
	long RealID;
	bool bForward;
	CString strFileName, strTemp = "";

	int nPos = strImageFileName.Find('.');
	strFileName = strImageFileName.Left(nPos);

	if(strFileName.Find("TOP") >= 0)
		nStation = 0;
	else if(strFileName.Find("THR") >= 0)
		nStation = 1;
	else
		return -1;

	nPos = strFileName.Find("Doc");
	strTemp = "";
	for (int i = 0; i < 2; i++) {
		strTemp.AppendChar(strFileName.GetAt(nPos + 3 + i));
	}
	nDocumentID = atoi(strTemp);

	nPos = strFileName.Find("Col");
	strTemp = "";
	for(int i = 0; i < 3; i++) {
		strTemp.AppendChar(strFileName.GetAt(nPos + 3 + i));
	}
	nColumnID = atoi(strTemp);

	nPos = strFileName.Find("Row");
	strTemp = "";
	for(int i = 0; i < 3; i++) {
		strTemp.AppendChar(strFileName.GetAt(nPos + 3 + i));
	}
	nRowID = atoi(strTemp);

	nPos = strFileName.Find("DieNo");
	strTemp = "";
	for (int i = 0; i < 3; i++) {
		strTemp.AppendChar(strFileName.GetAt(nPos + 5 + i));
	}
	nDieID = atoi(strTemp);

	nPos = strFileName.Find("FOV");
	strTemp = "";
	for (int i = 0; i < 3; i++) {
		strTemp.AppendChar(strFileName.GetAt(nPos + 3 + i));
	}
	nFOV = atoi(strTemp);

	int nError = 0;
	if(nStation == STATION_TOP){
		nError = GetCameraGrabIDTOP(nStation,  nRowID , nColumnID , nDieID , RealID);
	}
	else if (nStation == STATION_THR) {
		nDieID = nColumnID;
		nError = GetCameraGrabIDTHR(nStation, nRowID, nColumnID, nDieID, RealID);
	}

	if(nError)
		return nError;

	nRealGrabID = RealID;

	return 0;
}

void MotionParameters::SaveMotionParm() const
{
	CString strFile;
	strFile.Format("%s\\Motor.info", theApp.m_strMainCfgDir);

	CIniFile iniFile;
	CIniFile iniMotionPara;
	CFileException e;

	CString strKey, strTemp;
	std::string str;
	//strFile.Format("%s\\DeviceConfiguration.dat", pApp->m_strConfigDir);
	iniFile.SetPath(strFile);

	for(int i = 0; i < motorCount; i++) {
		
		iniFile.SetInteger(motorEngine[i].getName().c_str(), "Axis ID", motorEngine[i]._getAxisId());
		iniFile.SetInteger(motorEngine[i].getName().c_str(), "Homing mode", motorEngine[i]._getHomingMode());
		iniFile.SetInteger(motorEngine[i].getName().c_str(), "Negative Soft Limit", motorEngine[i].getNegativeSoftLimit());
		iniFile.SetInteger(motorEngine[i].getName().c_str(), "Positive Soft Limit", motorEngine[i].getPositiveSoftLimit());
		iniFile.SetBoolean(motorEngine[i].getName().c_str(), "Forward movement", motorEngine[i].isForwardMovement());
		iniFile.SetBoolean(motorEngine[i].getName().c_str(), "Forward homing", motorEngine[i].isForwardHoming());
		iniFile.SetInteger(motorEngine[i].getName().c_str(), "Home origin offset (pps)", motorEngine[i].getHomeOriginOffset());
		iniFile.SetInteger(motorEngine[i].getName().c_str(), "Settling time (ms)", motorEngine[i].getSettlingTime());
		iniFile.SetInteger(motorEngine[i].getName().c_str(), "Speed limit (mm/sec)", motorEngine[i].getSpeedLimit());
		iniFile.SetInteger(motorEngine[i].getName().c_str(), "Start Homing speed (mm/sec)", motorEngine[i].getHomingSpeed().startSpeed);
		iniFile.SetInteger(motorEngine[i].getName().c_str(), "Maximum Homing speed (mm/sec)", motorEngine[i].getHomingSpeed().maximumSpeed);
	}	
	iniFile.WriteFile();

	strFile.Format("%s\\Motion Parameters.info", theApp.m_strMainCfgDir);
	strKey.Format("Motor predefined positions");
	iniMotionPara.SetPath(strFile);

	str = motorEngine[X_SERVO_MOTOR].getName() + " Initial Position (pps)";
	iniMotionPara.SetInteger(strKey, str.c_str(), motorInitScanPos[X_SERVO_MOTOR]);

	// Y-Motor
	str = motorEngine[Y_SERVO_MOTOR].getName() + " Initial Position (pps)";
	iniMotionPara.SetInteger(strKey, str.c_str(), motorInitScanPos[Y_SERVO_MOTOR]);

	// ST1-Z-Motor
	str = motorEngine[ST1_Z_SERVO_MOTOR].getName() + " Initial Position (pps)";
	iniMotionPara.SetInteger(strKey, str.c_str(), motorInitScanPos[ST1_Z_SERVO_MOTOR]);

	// ST2-Z-Motor
	str = motorEngine[ST2_Z_SERVO_MOTOR].getName() + " Initial Position (pps)";
	iniMotionPara.SetInteger(strKey, str.c_str(), motorInitScanPos[ST2_Z_SERVO_MOTOR]);

	
	str = motorEngine[X_SERVO_MOTOR].getName() + " Module Unload Position (pps)";
	iniMotionPara.SetInteger(strKey, str.c_str(), motorModuleUnloadPos[X_SERVO_MOTOR]);

	str = motorEngine[Y_SERVO_MOTOR].getName() + " Module Unload Position (pps)";
	iniMotionPara.SetInteger(strKey, str.c_str(), motorModuleUnloadPos[Y_SERVO_MOTOR]);

	str = motorEngine[ST1_Z_SERVO_MOTOR].getName() + " Module Unload Position (pps)";
	iniMotionPara.SetInteger(strKey, str.c_str(), motorModuleUnloadPos[ST1_Z_SERVO_MOTOR]);

	str = motorEngine[ST2_Z_SERVO_MOTOR].getName() + " Module Unload Position (pps)";
	iniMotionPara.SetInteger(strKey, str.c_str(), motorModuleUnloadPos[ST2_Z_SERVO_MOTOR]);

	// X-Motor
	strKey.Format("Motor predefined measurements");
	str = motorEngine[X_SERVO_MOTOR].getName() + " Scanning Offset (mm)";
	iniMotionPara.SetDouble(strKey, str.c_str(), dMotorXScanOffsetMM);

	strKey.Format("Motor predefined measurements");
	str = motorEngine[X_SERVO_MOTOR].getName() + " Forward Trigger Offset (mm)";
	iniMotionPara.SetDouble(strKey, str.c_str(), dForwardTriggerOffset);

	strKey.Format("Motor predefined measurements");
	str = motorEngine[X_SERVO_MOTOR].getName() + " Inverse Trigger Offset (mm)";
	iniMotionPara.SetDouble(strKey, str.c_str(), dInverseTriggerOffset);

	strKey.Format("Motor predefined measurements");
	str = motorEngine[X_SERVO_MOTOR].getName() + " Top Die to Encap Offset (mm)";
	iniMotionPara.SetDouble(strKey, str.c_str(), m_dEncapOffsetX);

	strKey.Format("Motor predefined measurements");
	str = motorEngine[Y_SERVO_MOTOR].getName() + " Top Die to Encap Offset (mm)";
	iniMotionPara.SetDouble(strKey, str.c_str(), m_dEncapOffsetY);

	iniMotionPara.SetDouble(strKey, "Z Sensor to TOP Camera Distance (mm)", m_dDistanceZSensorToTop);
	iniMotionPara.SetDouble(strKey, "TOP Camera to THR Camera Distance (mm)", m_dDistanceTopToTHR);

	iniMotionPara.WriteFile();

}
void MotionParameters::LoadMotionParm()
{
	CIniFile iniFile;
	CIniFile iniMotionPara;

	CFileException e;

	CString strFile, strKey, strTemp;

	int nTemp = 0, nTemp2 = 0;
	bool bTemp = false;

	strFile.Format("%s\\Motor.info", theApp.m_strMainCfgDir);
	iniFile.SetPath(strFile);

	if(iniFile.ReadFile()) {
		for(int i = 0; i < motorCount; i++) {
			nTemp = iniFile.GetInteger(motorEngine[i].getName().c_str(), "Axis ID", nTemp);
			motorEngine[i].setAxisId(nTemp);

			nTemp = iniFile.GetInteger(motorEngine[i].getName().c_str(), "Homing mode", nTemp);
			motorEngine[i].setHomingMode(nTemp);

			nTemp = iniFile.GetInteger(motorEngine[i].getName().c_str(), "Negative Soft Limit", nTemp);
			motorEngine[i].setNegativeSoftLimit(nTemp);

			nTemp = iniFile.GetInteger(motorEngine[i].getName().c_str(), "Positive Soft Limit", nTemp);
			motorEngine[i].setPositiveSoftLimit(nTemp);

			bTemp = iniFile.GetBoolean(motorEngine[i].getName().c_str(), "Forward movement", bTemp);
			motorEngine[i].setForwardMovement(bTemp);

			bTemp = iniFile.GetBoolean(motorEngine[i].getName().c_str(), "Forward homing", bTemp);
			motorEngine[i].setForwardHoming(bTemp);

			nTemp = iniFile.GetInteger(motorEngine[i].getName().c_str(), "Home origin offset (pps)", nTemp);
			motorEngine[i].setHomeOriginOffset(nTemp);

			nTemp = iniFile.GetInteger(motorEngine[i].getName().c_str(), "Settling time (ms)", nTemp);
			motorEngine[i].setSettlingTime(nTemp);

			nTemp = iniFile.GetInteger(motorEngine[i].getName().c_str(), "Speed limit (mm/sec)", nTemp);
			motorEngine[i].setSpeedLimit(nTemp);

			nTemp = iniFile.GetInteger(motorEngine[i].getName().c_str(), "Start Homing speed (mm/sec)", nTemp);
			nTemp2 = iniFile.GetInteger(motorEngine[i].getName().c_str(), "Maximum Homing speed (mm/sec)", nTemp2);

			motorEngine[i].setHomingSpeed(nTemp, nTemp2);
		}
	}

	std::string str;
	strFile.Format("%s\\Motion Parameters.info", theApp.m_strMainCfgDir);
	strKey.Format("Motor predefined positions");
	iniMotionPara.SetPath(strFile);
	if(iniMotionPara.ReadFile()) {
		//X-Motor
		str = motorEngine[X_SERVO_MOTOR].getName() + " Initial Position (pps)";
		motorInitScanPos[X_SERVO_MOTOR] = iniMotionPara.GetInteger(strKey, str.c_str(), motorInitScanPos[X_SERVO_MOTOR]);

		str = motorEngine[Y_SERVO_MOTOR].getName() + " Initial Position (pps)";
		motorInitScanPos[Y_SERVO_MOTOR] = iniMotionPara.GetInteger(strKey, str.c_str(), motorInitScanPos[Y_SERVO_MOTOR]);

		str = motorEngine[ST1_Z_SERVO_MOTOR].getName() + " Initial Position (pps)";
		motorInitScanPos[ST1_Z_SERVO_MOTOR] = iniMotionPara.GetInteger(strKey, str.c_str(), motorInitScanPos[ST1_Z_SERVO_MOTOR]);

		str = motorEngine[ST2_Z_SERVO_MOTOR].getName() + " Initial Position (pps)";
		motorInitScanPos[ST2_Z_SERVO_MOTOR] = iniMotionPara.GetInteger(strKey, str.c_str(), motorInitScanPos[ST2_Z_SERVO_MOTOR]);

		str = motorEngine[X_SERVO_MOTOR].getName() + " Module Unload Position (pps)";
		motorModuleUnloadPos[X_SERVO_MOTOR] = iniMotionPara.GetInteger(strKey, str.c_str(), motorModuleUnloadPos[X_SERVO_MOTOR]);
	
		str = motorEngine[Y_SERVO_MOTOR].getName() + " Module Unload Position (pps)";
		motorModuleUnloadPos[Y_SERVO_MOTOR] = iniMotionPara.GetInteger(strKey, str.c_str(), motorModuleUnloadPos[Y_SERVO_MOTOR]);
	
		str = motorEngine[ST1_Z_SERVO_MOTOR].getName() + " Module Unload Position (pps)";
		motorModuleUnloadPos[ST1_Z_SERVO_MOTOR] = iniMotionPara.GetInteger(strKey, str.c_str(), motorModuleUnloadPos[ST1_Z_SERVO_MOTOR]);

		str = motorEngine[ST2_Z_SERVO_MOTOR].getName() + " Module Unload Position (pps)";
		motorModuleUnloadPos[ST2_Z_SERVO_MOTOR] = iniMotionPara.GetInteger(strKey, str.c_str(), motorModuleUnloadPos[ST2_Z_SERVO_MOTOR]);

		//X-Motor
 		strKey.Format("Motor predefined measurements");
		str = motorEngine[X_SERVO_MOTOR].getName() + " Scanning Offset (mm)";
		dMotorXScanOffsetMM = iniMotionPara.GetDouble(strKey, str.c_str(), dMotorXScanOffsetMM);
		if (dMotorXScanOffsetMM <= 0) {
			dMotorXScanOffsetMM = 1;
		}

		strKey.Format("Motor predefined measurements");
		str = motorEngine[X_SERVO_MOTOR].getName() + " Forward Trigger Offset (mm)";
		dForwardTriggerOffset = iniMotionPara.GetDouble(strKey, str.c_str(), dForwardTriggerOffset);

		strKey.Format("Motor predefined measurements");
		str = motorEngine[X_SERVO_MOTOR].getName() + " Inverse Trigger Offset (mm)";
		dInverseTriggerOffset = iniMotionPara.GetDouble(strKey, str.c_str(), dInverseTriggerOffset);

		strKey.Format("Motor predefined measurements");
		str = motorEngine[X_SERVO_MOTOR].getName() + " Top Die to Encap Offset (mm)";
		m_dEncapOffsetX = iniMotionPara.GetDouble(strKey, str.c_str(), m_dEncapOffsetX);

		strKey.Format("Motor predefined measurements");
		str = motorEngine[Y_SERVO_MOTOR].getName() + " Top Die to Encap Offset (mm)";
		m_dEncapOffsetY = iniMotionPara.GetDouble(strKey, str.c_str(), m_dEncapOffsetY);

		m_dDistanceZSensorToTop = iniMotionPara.GetDouble(strKey, "Z Sensor to TOP Camera Distance (mm)", m_dDistanceZSensorToTop);
		m_dDistanceTopToTHR = iniMotionPara.GetDouble(strKey, "TOP Camera to THR Camera Distance (mm)", m_dDistanceTopToTHR);
	}

}

double MotionParameters::GetMotorScanningOffset() const
{
	return dMotorXScanOffsetMM;
}

void MotionParameters::SetMotorScanningOffset(double dOffset)
{
	if(dOffset <= 0)
		return;

	dMotorXScanOffsetMM = dOffset;
}

double MotionParameters::GetForwardTriggerOffset() const
{
	return dForwardTriggerOffset;
}

void MotionParameters::SetForwardTriggerOffset(double dOffset)
{
	dForwardTriggerOffset = dOffset;
}

double MotionParameters::GetInverseTriggerOffset() const
{
	return dInverseTriggerOffset;
}

void MotionParameters::SetInverseTriggerOffset(double dOffset)
{
	dInverseTriggerOffset = dOffset;
}

double MotionParameters::GetEncapOffsetX() const
{
	return m_dEncapOffsetX;
}

void MotionParameters::SetEncapOffsetX(double dOffset)
{
	m_dEncapOffsetX = dOffset;
}

double MotionParameters::GetEncapOffsetY() const
{
	return m_dEncapOffsetY;
}

void MotionParameters::SetEncapOffsetY(double dOffset)
{
	m_dEncapOffsetY = dOffset;
}

double MotionParameters::GetZSensorCurrentDistance(int nReadData, vector <double > &ZValues)
{
	double nZSensorData = 0.0;
//	theApp.m_TriangularSensor.ProcessInstancePoll(theApp.m_TrigangualrsensorInstance, nZSensorData);
	theApp.m_TriangularSensor.ProcessInstance(theApp.m_TrigangualrsensorInstance, nZSensorData, ZValues, nReadData);

	return nZSensorData;
}

double MotionParameters::GetZSensorCurrentDistance()
{
	double nZSensorData = 0.0;
	theApp.m_TriangularSensor.ProcessInstancePoll(theApp.m_TrigangualrsensorInstance, nZSensorData);
	//	theApp.m_TriangularSensor.ProcessInstance(theApp.m_TrigangualrsensorInstance, nZSensorData, ZValues, nReadData);

	return nZSensorData;
}

int MotionParameters::GetZSensorDataAvail(int &nReadDatas)
{
	
	theApp.m_TriangularSensor.ProcessDataAvail(theApp.m_TrigangualrsensorInstance, nReadDatas);

	return 0;
}


int MotionParameters::SetZSensorDistance(int nPos, double nDistance)
{
	if(nPos < 0 || nPos > _zSensorMeasuredDistance.size() - 1)
		return -1;

	_zSensorMeasuredDistance[nPos] = nDistance;
	return 0;
}

double MotionParameters::GetZSensorDistance(int nPos)
{
	if(nPos < 0 || nPos > _zSensorMeasuredDistance.size())
		return -1;

	return _zSensorMeasuredDistance[nPos];

}


int MotionParameters::SetZSensorEachFovMultipleDistance(int nPos, vector <double> nDistance)
{
	if(nPos < 0 || nPos > _zSensorEachFovDistance.size() - 1)
		return -1;

	_zSensorEachFovDistance[nPos] = nDistance;

	return 0;
}

vector <double> MotionParameters::GetZSensorEachFovMultipleDistance(int nPos)
{
	/*if(nPos < 0 || nPos > _zSensorEachFovDistance.size() - 1)
		return;*/

	return _zSensorEachFovDistance[nPos];

}

int MotionParameters::GetZSensorTriggerPoints(int nPos, int &nStartPosition, int &nEndPosition)
{
	if(nPos < 0 || nPos > _zSensorTriggerStartPos.size() || nPos > _zSensorTriggerEndPos.size())
		return -1;

	nStartPosition = _zSensorTriggerStartPos[nPos];
	nEndPosition = _zSensorTriggerEndPos[nPos];

	return 0;
}

int MotionParameters::GetZTopTriggerIndicator(int nTriggerCount)
{
	if(nTriggerCount < 0 || nTriggerCount > _zSensorTopTriggerIndicator.size())
		return -1;

	return _zSensorTopTriggerIndicator[nTriggerCount];

}

int MotionParameters::GetZSensorReadingCount() {

	return nNumberofZTriggerpoints;
}


void MotionParameters::SetZSensorTeachValue(double dZTeachValue)
{
	if(dZTeachValue < 0 || dZTeachValue > 10.0)
		return;

	m_dZSensorTeachValue = dZTeachValue;
	OutputDebugLogTo(0, TRUE, "Z Sensor Teach Value Common Set to %3.06f", m_dZSensorTeachValue);

}

double MotionParameters::GetZSensorTeachValue() const
{
	return m_dZSensorTeachValue;
}

void MotionParameters::SetZSensorTeachValue(int nDieID, double dZTeachValue)
{
	if (dZTeachValue < 0 || dZTeachValue > 10.0 || nDieID < 0 || nDieID >= MAX_DIES)
		return;

	m_dZSensorDieTeachValue[nDieID] = dZTeachValue;
	OutputDebugLogTo(0, TRUE, "Z Sensor Teach Value Die %d Set to %3.06f", nDieID, m_dZSensorDieTeachValue[nDieID]);

}

double MotionParameters::GetZSensorTeachValue(int nDieID) const
{
	if (nDieID < 0 || nDieID >= MAX_DIES)
		return 0;

	return m_dZSensorDieTeachValue[nDieID];
}

int MotionParameters::getTotalDieCount() const
{
	return (m_nNoOfBlock[0] * m_nNoOfBlock[1]);
}

int MotionParameters::GetTotalFOVPerDieTHR()
{
	return nFOVPerDieinXTHR * nFOVPerDieinYTHR;
}

int MotionParameters::GetTotalFOVPerDieTop()
{
	return nFOVPerDieinXTOP * nFOVPerDieinYTOP;
}

void MotionParameters::enableBarcodeStatus(BOOL bEnable)
{
	m_bEnablebarcodeReader = bEnable;
}

void MotionParameters::enableBarcodeStatusWithCylinder(BOOL bEnable)
{
	m_bEnablebarcodeReaderWithCylinder = bEnable;
}

void MotionParameters::enableIonizerStatus(BOOL bEnable)
{
	m_bEnableIonizer = bEnable;
}

void MotionParameters::enableTopScanning(BOOL bEnable)
{
	m_bEnableTopScanning = bEnable;
}


void MotionParameters::enableTHRScanning(BOOL bEnable)
{
	m_bEnableTHRScanning = bEnable;
}

BOOL MotionParameters::isBarcodeEnabled()
{
	return m_bEnablebarcodeReader;
}

BOOL MotionParameters::isBarcodeWithCylinderEnabled()
{
	return m_bEnablebarcodeReaderWithCylinder;
}


BOOL MotionParameters::isIonizerEnabled()
{
	return m_bEnableIonizer;
}

BOOL MotionParameters::isTHRScanningEnabled()
{
	return m_bEnableTHRScanning;
}

BOOL MotionParameters::isTopScanningEnabled()
{
	return m_bEnableTopScanning;
}

int MotionParameters::ClearComputerId()
{
	int nGrabID = 0;
	for (int i = 0; i<nScanRowNoTHR; i++) {
		for (int j = 0; j<triggerCountPerRowTHR[i]; j++, nGrabID++) {
			scanningImageComputerId[nGrabID] = -1;
		}
	}
	return 0;
}

int MotionParameters::setInspectionThreadId(int nImageId, int nThread)
{
	if (nImageId < 0 && nImageId > nTotalImageFOVTHR)
		return -1;

	scanningImageComputerId[nImageId] = nThread;

	return 0;
}

int MotionParameters::setOffsetForTHR(double dOffsetXMicron, double dOffsetYMicron, int nDieNo)
{
	if (nDieNo < 0 && nDieNo > getTotalDieCount())
		return -1;

	dOffsetXScanTHRMicron[nDieNo] = dOffsetXMicron;
	dOffsetYScanTHRMicron[nDieNo] = dOffsetYMicron;

	//OutputDebugLogTo(7, TRUE, "Die %d, Offset %f, %f", nDieNo, dOffsetXScanTHRMicron[nDieNo], dOffsetYScanTHRMicron[nDieNo]);
	return 0;
}

std::vector < int > MotionParameters::getInspectionThreadId() const
{
	std::vector < int > id;

	for (long i = 0; i < nTotalImageFOVTHR; ++i) {
		int nColumnID, nRowID;
		bool bForward;

		int computerId = scanningImageComputerId[i];

		if (computerId >= 0 && computerId < 99) // 99 is slave PC
			id.push_back(computerId);
	}

	std::sort(id.begin(), id.end());
	id.erase(std::unique(id.begin(), id.end()), id.end());

	return id;
}
std::vector < int > MotionParameters::getInspectionTOPThreadId() const
{
	std::vector < int > id;

	for (long i = 0; i < nTotalImageFOVTOP; ++i) {
		int nColumnID, nRowID;
		bool bForward;

		int computerId = scanningImageComputerId[i];

		if (computerId >= 0 && computerId < 99) // 99 is slave PC
			id.push_back(computerId);
	}

	std::sort(id.begin(), id.end());
	id.erase(std::unique(id.begin(), id.end()), id.end());

	return id;
}

void MotionParameters::SetJigLevelPositions(int nPosition)
{
	motorEngine[X_SERVO_MOTOR].getCurrentPosition(JigPosition[nPosition][0]);
	motorEngine[Y_SERVO_MOTOR].getCurrentPosition(JigPosition[nPosition][1]);
//	JigSensorValue[Pos] = GetZSensorCurrentDistance();
}

void MotionParameters::getJigLevelZSensorvalues(double *positions)
{
	for (int i = 0; i<4; i++)
		positions[i] = JigSensorValue[i];
}

void MotionParameters::setJigLevelZSensorvalues(double *positions)
{
	for(int i=0; i<4; i++)
		JigSensorValue[i] = positions[i];
}

void MotionParameters::SaveJigLevelPositions()
{
	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp, str;

	strFile.Format("%s\\Jig Leveling Parameters.info", pApp->m_strMainCfgDir);
	iniFile.SetPath(strFile);

	
	for (int i = 0; i < 4; i++)
	{
		strKey.Format("Positin %d", i + 1);
		str.Format("Scanning Position of X");
		iniFile.SetInteger(strKey, str, JigPosition[i][0]);
		str.Format("Scanning Position of Y");
		iniFile.SetInteger(strKey, str, JigPosition[i][1]);
		str.Format("Z Sensor Value");
		iniFile.SetDouble(strKey, str, JigSensorValue[i]);
	}
	iniFile.WriteFile();
}

void MotionParameters::LoadJigLevelPositions()
{

	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp,str;

	int nInt;

	strFile.Format("%s\\Jig Leveling Parameters.info", pApp->m_strMainCfgDir);
	iniFile.SetPath(strFile);

	if (iniFile.ReadFile()) {
		for (int i = 0; i < 4; i++){
			strKey.Format("Positin %d", i + 1);
			str.Format("Scanning Position of X");
			JigPosition[i][0] = iniFile.GetInteger(strKey, str, JigPosition[i][0]);
			str.Format("Scanning Position of Y");
			JigPosition[i][1] = iniFile.GetInteger(strKey, str, JigPosition[i][1]);
			str.Format("Z Sensor Value");
			JigSensorValue[i] = iniFile.GetDouble(strKey, str, JigSensorValue[i]);
		}
	}

}
int MotionParameters::MoveJigLevelPos(int Pos)
{
	int nError = StopAxis(X_SERVO_MOTOR);
	if (nError)
		return nError;

	nError = StopAxis(Y_SERVO_MOTOR);
	if (nError)
		return nError;


	

	nError = AbsoluteMove(X_SERVO_MOTOR, JigPosition[Pos][0], getMovingSpeed(X_SERVO_MOTOR), false);
	if (nError)
		return nError;

	nError = AbsoluteMove(Y_SERVO_MOTOR, JigPosition[Pos][1], getMovingSpeed(Y_SERVO_MOTOR), false);
	if (nError)
		return nError;


	nError = WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
	if (nError) {
		StopAxis(X_SERVO_MOTOR);
		return nError;
	}

	nError = WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
	if (nError) {
			StopAxis(Y_SERVO_MOTOR);
		return nError;
	}
	return 0;
}

void MotionParameters::ResetReScanStatus()
{
	dReScanDieStatus.resize(getTotalDieCount());
	std::fill(dReScanDieStatus.begin(), dReScanDieStatus.end(), false);
}

bool MotionParameters::GetReScanStatus(int nDieNo)
{
	if (nDieNo < 0 || nDieNo >= getTotalDieCount())
		return false;

	return dReScanDieStatus[nDieNo];	
}

bool MotionParameters::GetReScanStatus()
{
	bool bVal = true;
	std::vector < bool >::const_iterator v = std::find_if(dReScanDieStatus.begin(), dReScanDieStatus.end(),
		[bVal](const bool & item) { return bVal == item; });

	if (v != dReScanDieStatus.end())
		return true;
	else
		return false;
}

int MotionParameters::SetReScanStatus(int nDieNo, bool bStatus)
{
	if (nDieNo < 0 || nDieNo >= getTotalDieCount())
		return -1;

	dReScanDieStatus[nDieNo] = bStatus;
	OutputDebugLogTo(8, TRUE, "dReScanDieStatus[%d] = %d", nDieNo, dReScanDieStatus[nDieNo]);

	return 0;
}