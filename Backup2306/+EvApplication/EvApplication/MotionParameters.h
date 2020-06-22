#pragma once

#include <vector>
#include "boost\array.hpp"
//#include "Application.h"
#include "motion_information.h"

class MotorInterface;

#define MAX_DIRECTION 2
#define MAX_FOV_TOP 5
//------ USER DEFINITIONS FOR MOTORS
enum Motors
{
	X_SERVO_MOTOR,
	WIDTH_ADJUSTMENT_STEPPER_MOTOR,
	Y_SERVO_MOTOR,
	PUSH_PULL_LINEAR_MOTOR,
	ST1_Z_SERVO_MOTOR,
	ST2_Z_SERVO_MOTOR,
};

enum Scales {
	X_SCALE_1,
	X_SCALE_2,
	X_SCALE_3,
	Y_SCALE_2
};

enum BiasClampOption
{
	BIASING_SEQUENCE,
	CLAMPING_SEQUENCE
};

class MotionParameters
{

public:
	
	MotionParameters();
	MotionParameters(const MotionParameters &motionParameters);
	~MotionParameters();
	CApplication* pApp;
	//Pass necessary InspectionParameter to Motion Movement class
	void SetMotionMovementParameters(const MotionParameters & motionParameters);
	const MotionParameters & operator=(const MotionParameters & motionParameters);

	void InitMotionCard();
	void InitScaleCard();
	void FreeMotionCard();

	int SetServoOn(int nAxisNo);
	int SetServoOff(int nAxisNo);
	int StopAxis(int nAxisNo);
	int EmergencyStop(int nAxisNo);
	int ResetAlarm(int nAxisNo);
	int HomeMove(int nAxisNo, bool bSafety = true, bool bHomeByPass = false);
	int HomeDone(int nAxisNo);
	int AbsoluteMove(int nAxisNo, int nPos, Speed speed, bool bSafety = true, bool bHomeByPass = false);
	int RelativeMove(int nAxisNo, int nPos, Speed speed, bool bSafety = true, bool bHomeByPass = false);
	int ChangeSpeedOnFly(int nAxisNo, int nNewMaxVel);
	int ContinuousMove(int nAxisNo, BOOL bForward, Speed speed, bool bSafety = true, bool bHomeByPass = false);

	int WaitForMotionDone(int nAxisNo, int nTimeOut);

	int IsMotionDone(int nAxisNo, BOOL & bDone);

	int getMotorStatus(int axisId, long & status);

	int GetCurrentMotorPosition(int nAxisNo, int & nPos);
	BOOL IsMotorActive(int nAxisNo);
	BOOL IsMotorHomed(int nAxisNo) const;
	BOOL IsMotorLimit(int nAxisNo);
	int GetMotorAlarmStatus(int nAxisNo, BOOL &bOn);
	int GetMotorIOStatus(int nAxisNo, long & nStatus);

	int GetMotorResolution(int nAxisNo, double & dResolution) const;
	int SetMotorResolution(int nAxisNo, double dResolution);

	int GetMotorSettlingTime(int nAxisNo, int & nTime) const;
	int SetMotorSettlingTime(int nAxisNo, int nTime);

	Speed getHomingSpeed(int axisId) const;
	Speed getScanningSpeed(int axisId) const;
	Speed getScanningSpeedTHR(int axisId) const;
	Speed getMovingSpeed(int axisId) const;

	int setHomingSpeed(int axisId, Speed speed);
	int setScanningSpeed(int axisId, Speed speed);		//Top speed
	int setScanningSpeedTHR(int axisId, Speed speed);
	int setMovingSpeed(int axisId, Speed speed);

	bool checkSoftLimitSetStatus();

	int GetMotorHomeOriginOffset(int nAxisNo, int & nOffset) const;
	int SetMotorHomeOriginOffset(int nAxisNo, int nOffset);

	int GetMotorHomeDirection(int nAxisNo, BOOL & bForward) const;
	int SetMotorHomeDirection(int nAxisNo, BOOL bForward);

	int GetMotorMovementDirection(int nAxisNo, BOOL & bForward) const;
	int SetMotorMovementDirection(int nAxisNo, BOOL bForward);

	int GetMotorMaximumSpeed(int nAxisNo, double & dSpeed) const;

	void WaitMotorSettlingTime(int nAxisNo) const;

	int  GetMotorName(int nMotorID, CString & strName) const;

	// Multiple motor axis interraction
	int GetTotalMotorCount() const;
	int SetServoOnAllAxis();
	int SetServoOffAllAxis();
	int StopEveryAxis();
	int EmergencyStopEveryAxis();
	int ResetAlarmAllAxis();
	int HomeMoveEveryAxis(bool bSafety = true);
	int WaitForAnyMotionError(std::vector < int > axisId, int nTimeOut);
	void WaitForEveryMotionDone(std::vector < int > error, int nTimeOut = 1000);

	int MoveToStartScanPos(bool bSafety = true);//Move to X & Y Motor THR Start Scan Position
	int MoveToTOPStartScanPos(int nFov, bool bSafety = true);//Move to X & Y Motor TOP Start Scan Position
	int moveToZStartScanPosPos(bool bSafety = true);
	int moveToTOPZStartScanPosPos(int nFov, bool bSafety = true);
	int moveToZSensorTeachPos(bool bSafety = true);
	int moveToZSensorTeachPos(int nDie, bool bSafety = true);

	int MotionParameters::MoveJigLevelPos(int Pos);

	void SetStartScanPos();//THR Start Position X&Y Motor
	void SetTOPStartScanPos(int nFov);//TOP Start Position X&Y Motor
	void SetJigLevelPositions(int nAxis);
	void LoadJigLevelPositions();
	void SaveJigLevelPositions();
	void getJigLevelZSensorvalues(double *position);
	void setJigLevelZSensorvalues(double *position);

	void SetZStartScanPos();
	void SetTOPZStartScanPos(int nFov);
	void SetZSensorTeachPos(int nDie);//Set Z Sensor Teach Position Individual

	void SetReadyScanPos();
	void SetNestReplacementPos();

	int moveToReadyScanPos(bool bSafety = true);
	int moveToNestReplacementPos(bool bSafety = true);
	
	BOOL IsMachineHomed() const;

	int SetAlarm();

	// Scale interraction
	int GetTotalScaleCount() const;
	int GetCurrentScalePosition(int nAxisNo, int & nPos);

	int GetScaleResolution(int nAxisNo, double & dResolution) const;
	int SetScaleResolution(int nAxisNo, double dResolution);
	int ResetScaleTriggerCount(int nAxisNo);
	int SetScaleCompareTrigger(int nScanningRow);
	int SetScaleCompareTriggerTHR(int nScanningRow);
	int SetScaleCompareTriggerZSensor(int nScanningRow);

	int scaleManualTrigger(int axisId);


	int GetScaleName(int nAxisNo, CString & strName) const;

	// Motor interraction
	int ResetTriggerCount(int nTriggerSource = 0);

	int SetCompareTriggerTHR(int nScanningRow);
	int SetCompareTriggerTOP(int nScanningRow);
	int SetCompareTriggerTopEncap(int nScanningRow);
	int SetCompareTriggerZSensor(int nScanningRow);
	int SetCompareTriggerTopnZ(int nScanningRow);
	int GetZTriggerCountNoPerRow(int nScanningRow);

	int GetLatchValue(int nAxis, long & nPosValue, long nInitialLatchData);

	int GetInitialLatchXValue(int nValueID, int & nInitValue);

	void GetMappingParameters(int & nBlockX, int & nBlockY, double & dBlockPitchX, double & dBlockPitchY, int & nDieX, int & nDieY,
		double & dDiePitchX, double & dDiePitchY, int & nDieFOVX, int & nDieFOVY, double & dInitialOffset) const;

	void GetMappingParameters(int & nBlockX, int & nBlockY, int & nDieX, int & nDieY) const;

	void GetMappingParameters(int & nDieFOVX, int & nDieFOVY) const;

	void GetMappingParametersTOP(int & nDieTOPX, int & nDieTOPY) const;

	int SetMappingParametersTOP(int nDieTOPX, int nDieTOPY);

	int SetMappingParameters(int nBlockX, int nBlockY, double dBlockPitchX, double dBlockPitchY, int nDieX, int nDieY,
		double dDiePitchX, double dDiePitchY, int nDieFOVX, int nDieFOVY, double dInitialOffset);
	int SetDatumOffsetParameters(double dTopRowoffsetX, double dBottomRowOffsetX, double dTopRowoffsetY, double dBottomRowOffsetY);    //ravi
	void GetDatumOffsetParameters(double & dTopRowoffset, double & dBottomRowOffset, double & dTopRowoffsetY, double & dBottomRowOffsetY);	//ravi
	int SetSubstrateSelected(int nColumn, int nRow);				//Girish
	int GetSubstrateSelected(int & nColumn, int & nRow);

	void enableBarcodeStatus(BOOL bEnable);
	void enableBarcodeStatusWithCylinder(BOOL bEnable);
	
	void enableIonizerStatus(BOOL bEnable);
	void enableTopScanning(BOOL bEnable);

	void enableTHRScanning(BOOL bEnable);
	int setInspectionThreadId(int nImageId, int nThread);
	int setOffsetForTHR(double dOffsetXMicron, double dOffsetYMicron, int nDieNo);
	std::vector < int > getInspectionThreadId() const;
	std::vector < int > getInspectionTOPThreadId() const;

	int ClearComputerId();

	BOOL isBarcodeEnabled();
	BOOL isBarcodeWithCylinderEnabled();
	
	
	BOOL isIonizerEnabled();
	BOOL isTHRScanningEnabled();
	BOOL isTopScanningEnabled();
	
	int m_nSubstrateDieColumn;						//Girish
	int m_nSubstrateDieRow;

	
	
	


	// Merging interraction for 2-3D station
	int GetTotalImageGrabsTHR() const;
	int GetTotalImageGrabsTOP() const;

	void getDeviceSize(double & dWidthMM, double & dHeightMM) const;
	void setDeviceSize(double dWidthMM, double dHeightMM);

	// Merging interraction for Bond station

	void LoadZTeachPosition();
	void SaveZTeachPosition() const;

	void SaveScanningOptions();
	void loadScanningOptions();

	void SaveScanningParm(bool saveMappingData = true) const;
	void LoadScanningParm();

	void SaveDeviceParameters() const;
	void LoadDeviceParameters();

	void SaveMotionParm() const;
	void LoadMotionParm();

	int CreateScanPositions();

	int getMotorInitPosition(int nAxis) const;
	int getMotorModuleUnloadPosition(int nAxis)const;

	int GetCameraGrabIDTHR(int nImageID, int nRowID, int nColumnID, int nDieId, long & nCameraGrabID) const;
	int GetCameraGrabIDTHR(int nImageID, int nDieId, int nFov, long & nCameraGrabID) const;
	int GetCameraGrabIDTOP(int nImageID, int nRowID, int nColumnID, int nDieId, long & nCameraGrabID) const;
	int GetDeviceDetailsTHR(int nCameraGrabID, int &nRowID, int &nColumnID, int &nDieId, int &nDieFOV) const;
	int GetDeviceDetailsTOP(int nCameraGrabID, int &nRowID, int &nColumnID, int &nDieId, int &nDieFOV);
	int GetTopCameraGrabID(int nTHRCameraID, long &nTopCameraId);
	int getScanColumnNoTHR() const;
	int getScanRowNoTHR() const;
	int getScanColumnNoTop() const;
	int getScanRowNoTop() const;
	int getScanColumnNoZSensor() const;
	int getScanRowNoZSensor() const;

//	void SetStartScanPos();

	int GetXMotorStartPosition(int nRow, int & nXPos) const;
	int GetXMotorEndPosition(int nRow, int & nXPos) const;
	int GetYTopMotorPositionTHR(int nRow, int & nYPos) const;
	int GetYTopMotorPositionTop(int nRow, int & nYPos) const;
	//int GetYBottomMotorPosition(int nRow, int & nYPos) const;

	int MoveXMotorToStartPosTHR(int nRow, Speed speed = Speed(0, 0));
	int MoveXMotorToEndPosTHR(int nRow, Speed speed = Speed(0, 0));

	int MoveXMotorToStartPosTop(int nRow, Speed speed = Speed(0, 0));
	int MoveXMotorToEndPosTop(int nRow, Speed speed = Speed(0, 0));

	int MoveXMotorToStartPosTopEncap(int nRow, Speed speed = Speed(0, 0));
	int MoveXMotorToEndPosTopEncap(int nRow, Speed speed = Speed(0, 0));

	int MoveXMotorToStartPosZ(int nRow, Speed speed = Speed(0, 0));
	int MoveXMotorToEndPosZ(int nRow, Speed speed = Speed(0, 0));

	int MoveXMotorToStartPosTopnZ(int nRow, Speed speed = Speed(0, 0));
	int MoveXMotorToEndPosTopnZ(int nRow, Speed speed = Speed(0, 0));

	int MoveXMotorToZSensorTeachPos(int nDie, Speed speed = Speed(0, 0));
	
	int MoveXMotorToSafePos(long nPos);
	int MoveYMotorToSafePos(long nPos);
	int MoveZMotorToSafePos(long nPos);

	int MoveYTopMotorTHR(int nRow, Speed speed = Speed(0, 0));
	int MoveYTopMotorTop(int nRow, Speed speed = Speed(0, 0));
	int MoveYTopMotorTopEncap(int nRow, Speed speed = Speed(0, 0));

//	int MoveXMotorToPos(int nColumn, int nRow, Speed speed = Speed(0, 0));

	int MoveZMotorToReadyPos(Speed speed = Speed(0, 0));
	int MoveTOPZMotorToReadyPos(int nFov = 0, Speed speed = Speed(0, 0));//Move ST1 Z-Motor To First FOV Position
	int MoveZMotorToReadyPosTHR(Speed speed = Speed(0, 0));
	int MoveZMotorToFocussingPos(int nCameraGrabId, Speed speed = Speed(0, 0));
	int MoveYMotorDatumOffsetPos(int nCameraGrabId, Speed speed = Speed(0, 0));
	int CreateNewPositions();
	void ResetReScanStatus();
	bool GetReScanStatus(int nDieNo);
	bool GetReScanStatus();
	int SetReScanStatus(int nDieNo, bool bStatus);

	int IsCameraGrabIDLastOnDieRow(long nCameraGrabNo, bool & bLastGrabOnRow);
	int IsCameraGrabIDLastOnRowTHR(long nCameraGrabNo, bool & bLastGrabOnRow);
	int IsCameraGrabIDLastOnRowTop(long nCameraGrabNo, bool & bLastGrabOnRow);
	int IsCameraGrabIDLastOnRowZSensor(long nCameraGrabNo, bool & bLastGrabOnRow);
	int getRealGrabId(CString strImageFileName, int &nStationId, int &nDocumentId, int & nRealGrabID);
	int getFOVIndex(long nGrabId, int &nFOV);
	int getImageMappingDetails(int i, int &nRowId, int &nColId, int &nDieId, int &nFov);
	int getCameraGrabIdTHR(int nDieId, int &nCameraIdTHR);
	int getMappingDieIdTHR(int nCameraIdTHR, int &nDieId);
	int getMappingDieIdTOP(int nCameraIdTOP, int &nDieId);
	int getCameraGrabIdTop(int nDieId, long &nCameraIdTop);
	int getTopDieMappingId(int nImageId, std::vector <int> &dieNumbers);
	int getTopImageId(int nDieId, int &nImageId);
	int getInspectionStatus(int nTrack, int nFOV, bool &bInspectionStaus);
	int setInspectionStatus(int nTrack, int nFOV, bool bInspectionDone);
	int setInspectionEnableTHR(int nCameraGrabId, bool bEnable);
	int getInspectionEnableStatusTHR(int nCameraGrabId, bool &bEnable);
	int getInspectionWaitStatus(int nTrack, bool &bWaitforInspection);
	int resetInspection();
	void SetZSensortoTopDistance(double);
	double GetZSensortoTopDistance();
	bool nDisplayDieNo(int nMappingId, int &nDieId);
	bool GetDieMappingNo(int nUnitId, int &DieId);
	double GetMotorScanningOffset() const;
	void SetMotorScanningOffset(double dOffset);
	double GetForwardTriggerOffset() const;
	void SetForwardTriggerOffset(double dOffset);
	double GetInverseTriggerOffset() const;
	void SetInverseTriggerOffset(double dOffset);

	double GetEncapOffsetX() const;
	void SetEncapOffsetX(double dOffset);
	double GetEncapOffsetY() const;
	void SetEncapOffsetY(double dOffset);

	void SetTopToTHRDistance(double);
	double GetTopToTHRDistance();

	double GetZSensorCurrentDistance(int nReadData, vector <double> & zValues);
	double GetZSensorCurrentDistance();
	int GetZSensorDataAvail(int &Count);

	int SetZSensorDistance(int nPos, double nDistance);
	double GetZSensorDistance(int nPos);


	int SetZSensorEachFovMultipleDistance(int nPos, vector <double> nDistance);
	vector <double> GetZSensorEachFovMultipleDistance(int nPos);

	int GetZSensorTriggerPoints(int nPos, int &nStartPosition, int &nEndPosition);
	int GetZTopTriggerIndicator(int nTriggerCount);

	int GetZSensorReadingCount();
	//motor functions
//	int GetMotorAlarmStatus(int nAxisNo, BOOL &bOn);
//	int SetServoOnAllAxis();
//	int SetServoOffAllAxis();
//	int EmergencyStopEveryAxis();

	int CheckSafety(int nAxisNo, bool bSafety, bool bHomeByPass = false);
	int CheckSoftwareLimits(int nAxisNo, int nPos);
	bool IsSoftwareLimits(int nAxisNo, int nPos);

	//Set Common Z Sensor Teach Value
	void SetZSensorTeachValue(double nZTeachValue);
	//Get Common Z Sensor Teach Value
	double GetZSensorTeachValue() const;

	//Set Individual Die Z Sensor Teach Value
	void SetZSensorTeachValue(int nDieID, double nZTeachValue);
	//Get Individual Die Z Sensor Teach Value
	double GetZSensorTeachValue(int nDieID) const;

	int GetTotalFOVPerDieTHR();
	int GetTotalFOVPerDieTop();

	int getTotalDieCount() const;

private:

	const static size_t maximumMotorCount = 6;
	const static size_t maximumScaleCount = 3;

	// Hardware variables
	CRITICAL_SECTION CriticalSectionCommands;

	MotorInterface & motorResource;

	// Scale variables
	int scaleCount;

// Motor variables
	int motorCount;
	

	boost::array < MotorEngine, maximumMotorCount > motorEngine;
	boost::array < ScaleEngine, maximumScaleCount > scaleEngine;

	boost::array < Speed, maximumMotorCount > _motorMoveVelocity;
	boost::array < Speed, maximumMotorCount > _motorScanVelocity;
	boost::array < Speed, maximumMotorCount > _motorScanVelocityTHR;

	int nTriggerInversePulse;
	int nTriggerPulseWidth;

	boost::array <int, 6		> motorStartScanPos;
	boost::array <int, 6		> motorTOPStartScanPos[MAX_FOV_TOP];
	boost::array <int, 6		> motorInitScanPos;		//initial position
	boost::array <int, 6		> motorModuleUnloadPos;		//Module unload position
//	boost::array <int, 3		> motorTHRStartScanPos;
//	boost::array <int, 3		> motorZSensorStartScanPos;
	boost::array <int, 6		> motorZSensorTeachPos[MAX_DIES];

	bool scanningReady;
	boost::array <double, 2> motorDisplacement;
	
	double m_dDistanceZSensorToTop;
	double m_dDistanceTopToTHR;
	double dMotorXScanOffsetMM;
	double m_dZSensorTeachValue;
	double m_dZSensorDieTeachValue[MAX_DIES];

	double dForwardTriggerOffset;
	double dInverseTriggerOffset;

	double m_dEncapOffsetX;
	double m_dEncapOffsetY;

	int m_nNoOfBlock[MAX_DIRECTION];
	double m_dBlockPitch[MAX_DIRECTION];
	int m_nDeviceNoPerBlock[MAX_DIRECTION];
	int m_nDeviceNoPerBlockTOP[MAX_DIRECTION];
	int m_nDeviceOffset[MAX_DIRECTION];
	double m_dInitialOffset;
	double m_dTopRowOffset[2];
	double m_dBottomRowOffset[2];
public:
	double m_dDevicePitch[MAX_DIRECTION];
	double m_dDeviceScanArea[MAX_DIRECTION];
	double m_dDeviceOverlap[MAX_DIRECTION];

//	int nNumberofDiesPerRow[MAX_DEVICE_ROW];
	int nFOVPerDieinXTHR;
	int nFOVPerDieinYTHR;

	int nFOVPerDieinXTOP;
	int nFOVPerDieinYTOP;

	volatile long nZSensorScanRow;

	std::vector <int> deviceRowId;
	BOOL m_bEnablebarcodeReader;
	BOOL m_bEnablebarcodeReaderWithCylinder;
	
	BOOL m_bEnableIonizer;
//	BOOL m_bDryRun;
	BOOL m_bEnableTopScanning;
	
	BOOL m_bEnableTHRScanning;

	int JigPosition[4][2];
	double JigSensorValue[4];
	

	// Scanning mode 1 (substrate mapping)
	/*int nBlockPerXNo;
	int nBlockPerYNo;
	double dBlockPerXPitchMM;
	double dBlockPerYPitchMM;
	int nDiePerBlockInX;
	int nDiePerBlockInY;
	double dDiePerXPitchMM;
	double dDiePerYPitchMM;*/
private:
	int nDieColumn;
	int nDieRow;

	// Device Size
	double dDeviceSizeInXMM;
	double dDeviceSizeInYMM;

	// Merging for 2-3D station
//	CRITICAL_SECTION CriticalSectionData;

//	MotorInterface  & motorResource;

	int nTotalImageFOVTHR;
	int nTotalImageFOVTOP;
	
	int nNumberofDie; 

	int nScanColumnNoTHR;
	int nScanRowNoTHR;

	int nScanColumnNoTop;
	int nScanRowNoTop;

	int nScanColumnNoZSensor;
	int nScanRowNoZSensor;

	std::vector <int> motorXStartScanningPositionTHR;
	std::vector <int> motorXEndScanningPositionTHR;

	std::vector <int> motorXStartScanningPositionTop;
	std::vector <int> motorXEndScanningPositionTop;

	std::vector <int> motorXStartScanningPositionTopEncap;
	std::vector <int> motorXEndScanningPositionTopEncap;

	std::vector <int> motorXStartScanningPositionZ;
	std::vector <int> motorXEndScanningPositionZ;

	std::vector <int> motorXStartScanningPositionTopandZ;
	std::vector <int> motorXEndScanningPositionTopandZ;


	std::vector <int> motorYScanningPositionTHR;
	std::vector <int> motorYScanningPositionTop;
	std::vector <int> motorYScanningPositionTopEncap;
	std::vector <int> motorYScanningDatumPositiom;

	std::vector < long > _cameraTriggerTHR;
	std::vector <short> triggerCountPerRowTHR;

	std::vector <long > _zFocussingPositionTHR;

	std::vector < long > _cameraTriggerTOP;
	std::vector <short> triggerCountPerRowTOP;

	std::vector < long > _cameraTriggerTopEncap;
	
	std::vector < long > _zSensorTriggerStartPos;
	std::vector < long > _zSensorTriggerEndPos;
	std::vector <short> _zSensortriggerCountPerRow;


	std::vector < long > _zSensorTopTriggerPoints;
	std::vector < short > _zSensorTopTriggerIndicator;		//2 for Top, 1 for Z, 0 for both;
	std::vector <short> _zTopTriggerCountPerRow;

	std::vector <  double> _zSensorMeasuredDistance;
	std::vector < std::vector <  double>> _zSensorEachFovDistance;


	std::vector < int> dieColumnNoTHR;
	std::vector < int> dieRowNoTHR;
	std::vector < int> dieNoTHR;
	std::vector < int> dieFOVTHR;

	std::vector < int> dieColumnNoTop;
	std::vector < int> dieRowNoTop;
	std::vector < int> dieNoTop;
	std::vector < int> dieFOVTop;
	std::vector < int> scanningDiemappingId;
	std::vector < int> scanningDiemappingIdTOP;
	std::vector < int>  TopToTHRDieMapping;
	std::vector < bool> enableInspectionTHR;

	std::vector <double> motorEngineResolution;
	std::vector <bool> bInspectionStatusTop;
	std::vector <bool> bInspectionStatusTHR;
	std::vector < int > scanningImageComputerId;
	int nNumberofZTriggerpoints;

	std::vector <double > dOffsetXScanTHRMicron;
	std::vector <double > dOffsetYScanTHRMicron;

	std::vector <bool > dReScanDieStatus;
	std::vector <bool > _devicePresence;
};
