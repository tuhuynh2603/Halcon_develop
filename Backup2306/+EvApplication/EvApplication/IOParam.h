#pragma once

#include <vector>

class IOControl
{
public:
	IOControl();
	~IOControl();

	// I\O User functions
	int GripperUp();
	int GripperDown(BOOL bSafety = TRUE);

	int GripperClampOpen();
	int GripperClampClose();

	
	int TrackClampsUp2();		// Machine ver 1
	int TrackClampsDown2();		// Machine ver 1

	//Sherlock Machine
	int TrackClampsUp();
	int TrackClampsDown();
	int TrackClamps2Up();
	int TrackClamps2Down();
	int ClampingSequence();//Track Clamp 1 & 2 Down together
	int UnClampingSequence();//Track Clamp 1 & 2 Up together

	int VacuumOn();
	int VacuumOff();

	int VacuumUp(BOOL bSafety = TRUE);
	int VacuumDown();

	int VacuumPurgeOn();
	int VacuumPurgeOff();


	//Tower Light
	int SetTowerBlueLightOn();
	int SetTowerBlueLightOff();
	int SetTowerGreenLightOn();
	int SetTowerGreenLightOff();
	int SetTowerOrangeLightOn();
	int SetTowerOrangeLightOff();
	int SetTowerRedLightOn();
	int SetTowerRedLightOff();
	int SetBuzzerOn();
	int SetBuzzerOff();

	//Transport IO
	int GetGripperStatus(int & nPos);
	int GetPullPresentSensorState(BOOL & bOn);
	int GetPushJamSensorState(BOOL & bOn);
	int GetSubstratePresence(BOOL & bOn);

	int GetVacuumSwitchOnStatus(BOOL & bOn);
	int GetVacuumUpStatus(BOOL & bOn);
	int GetWidthAdjustmentCrashStatus(BOOL & bOn);

	//Other IO
	int GetAirPressureStatus(BOOL & bOn);
	int GetEmergencyStatus(BOOL & bOn);
	int GetDoorSwitchStatus(BOOL & bOn);
	int GetGuardHealthyStatus(BOOL & bOn);
	int GetGuardBypassStatus(BOOL & bOn);
	int GetIncomingAirPressureStatus(BOOL & bOn);
	int SetGuardDoorLockOn();
	int SetGuardDoorLockOff();
	
	//Push Button LED
	int GetStartButtonStatus(BOOL & bOn);
	int GetStopButtonStatus(BOOL & bOn);
	int GetCycleStopButtonStatus(BOOL & bOn);
	int GetResetButtonStatus(BOOL & bOn);

	int ResetButtonLampOn();
	int ResetButtonLampOff();
	int StartButtonLampOn();
	int StartButtonLampOff();
	int CycleStopButtonLampOn();
	int CycleStopButtonLampOff();
	int StopButtonLampOn();
	int StopButtonLampOff();

	//Ionizer
	int SetIonizerDischargeStopOn();
	int SetIonizerDischargeStopOff();
	int SetIonizerMaintenanceSignalOn();
	int SetIonizerMaintenanceSignalOff();
	int GetIonizerErrorStatus(BOOL & bOn);
	int SetIonizerValveOn();
	int SetIonizerValveOff();

	//Loader
	int SetLoaderReadyToReceiveOn();
	int SetLoaderReadyToReceiveOff();
	int SetLoaderTransferSuccessOn();
	int SetLoaderTransferSuccessOff();
	int GetLoaderPanelAvailableStatus(BOOL & bOn);
	int GetLoaderPanelInStatus(BOOL & bOn);

	//Unloader
	int SetUnloaderPanelAvailableOn();
	int SetUnloaderPanelAvailableOff();
	int SetUnloaderPanelPushOutOn();
	int SetUnloaderPanelPushOutOff();
	int GetUnloaderReadyToReceiveStatus(BOOL & bOn);
	int GetUnloaderTransferSuccessStatus(BOOL & bOn);

	//Contactors
	int SetK1K2On();
	int SetK1K2Off();
	int SetK3K4On();
	int SetK3K4Off();


	int GetInputStatus(UINT& nData, int moduleId = 1);

	int getInputStatus(const std::vector < int > & ioId, std::vector < BOOL > & ioStatus, int moduleId);

	//*******
	int outputWrite(UINT nData, int moduleId);
	int outputRead(UINT & nData, int moduleId);
	int inputRead(UINT & nData, int moduleId);

	int outputWriteBit(UINT nBitNo, UINT nValue, int moduleId);
	int outputReadBit(UINT nBitNo, UINT & nValue, int moduleId);
	int inputReadBit(UINT nBitNo, UINT & nValue, int moduleId);

	void ResetIO();

private:

	bool inverseLogic;

	CRITICAL_SECTION accessSection;

	int setOutput(UINT bitId, BOOL bOn, int moduleId);
	int getOutput(UINT bitId, BOOL & bOn, int moduleId);
	int getInput(UINT bitId, BOOL & bOn, int moduleId);

	int getOutput(std::vector < BOOL > & status, int moduleId);
	int getInput(std::vector < BOOL > & status, int moduleId);
};