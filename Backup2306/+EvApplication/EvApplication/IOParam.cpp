#include "stdafx.h"
#include "IOParam.h"
#include "Application.h"
#include "DebugLog.h"

#define IO_CONTROL_TIME_OUT_MS 1000

IOControl::IOControl()
{
	inverseLogic = false;

	InitializeCriticalSection(&accessSection);
}

IOControl::~IOControl()
{
	DeleteCriticalSection(&accessSection);
}

int IOControl::setOutput(UINT bitId, BOOL bOn, int moduleId)
{
	if (bitId > 15)
		return -1;

	return outputWriteBit(bitId, bOn ? 1 : 0, moduleId);
}

int IOControl::getOutput(UINT bitId, BOOL & bOn, int moduleId)
{
	if (bitId > 15)
		return -1;

	UINT value = 0;

	int error = outputReadBit(bitId, value, moduleId);
	if (error)
		return error;


	bOn = (value != 0);

	return 0;
}

int IOControl::getInput(UINT bitId, BOOL & bOn, int moduleId)
{
	if (bitId > 15)
		return -1;

	UINT value = 0;

	int error = inputReadBit(bitId, value, moduleId);
	if (error)
		return error;

	bOn = (value != 0);

	return 0;
}

int IOControl::getOutput(std::vector < BOOL > & status, int moduleId)
{
	UINT data;

	int error = outputRead(data, moduleId);
	if (error)
		return error;

	status.resize(16);

	for (size_t i = 0; i < 16; ++i)
		status[i] = data & (1 << i);

	return 0;
}

int IOControl::getInput(std::vector < BOOL > & status, int moduleId)
{
	UINT data;

	int error = inputRead(data, moduleId);
	if (error)
		return error;

	status.resize(16);

	for (size_t i = 0; i < 16; ++i)
		status[i] = data & (1 << i);

	return 0;
}

int IOControl::outputWrite(UINT data, int moduleId)
{
	int error;

	if (inverseLogic)
		data = ~data;

	EnterCriticalSection(&accessSection);
	error = theApp.m_MotorResource.OutPortWrite(theApp.m_nMotorCardNo, moduleId, data);
	LeaveCriticalSection(&accessSection);

	return error;
}

int IOControl::outputRead(UINT & data, int moduleId)
{
	int error;

	EnterCriticalSection(&accessSection);
	error = theApp.m_MotorResource.OutPortRead(theApp.m_nMotorCardNo, moduleId, &data);
	LeaveCriticalSection(&accessSection);

	if (inverseLogic)
		data = ~data;

	return error;
}

int IOControl::inputRead(UINT & data, int moduleId)
{
	int error;

	EnterCriticalSection(&accessSection);
	error = theApp.m_MotorResource.InPortRead(theApp.m_nMotorCardNo, moduleId, &data);
	LeaveCriticalSection(&accessSection);

	if (inverseLogic)
		data = ~data;

	return error;
}

int IOControl::outputWriteBit(UINT nBitNo, UINT nValue, int moduleId)
{
	int error;

	if (inverseLogic)
		nValue = ~nValue;

	EnterCriticalSection(&accessSection);
	error = theApp.m_MotorResource.OutPortWriteBit(theApp.m_nMotorCardNo, moduleId, nBitNo, nValue);
	LeaveCriticalSection(&accessSection);

	return error;
}

int IOControl::outputReadBit(UINT nBitNo, UINT & nValue, int moduleId)
{
	int error;

	EnterCriticalSection(&accessSection);
	error = theApp.m_MotorResource.OutPortReadBit(theApp.m_nMotorCardNo, moduleId, nBitNo, &nValue);
	LeaveCriticalSection(&accessSection);

	if (inverseLogic)
		nValue = ~nValue;

	return error;
}

int IOControl::inputReadBit(UINT nBitNo, UINT & nValue, int moduleId)
{
	int error;

	EnterCriticalSection(&accessSection);
	error = theApp.m_MotorResource.InPortReadBit(theApp.m_nMotorCardNo, moduleId, nBitNo, &nValue);
	LeaveCriticalSection(&accessSection);

	if (inverseLogic)
		nValue = ~nValue;

	return error;
}

void IOControl::ResetIO()
{
	outputWrite(0, IoSignal::MODULE_1);
	outputWrite(0, IoSignal::MODULE_2);
	outputWrite(0, IoSignal::MODULE_3);
}

// I\O User functions
int IOControl::GripperUp()
{
	int error = setOutput(IoSignal::GRIPPER_UP, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	error = setOutput(IoSignal::GRIPPER_DOWN, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	BOOL status[2];
	int repeatCount = 1000;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::GRIPPER_DOWN_STATUS, status[0], IoSignal::MODULE_1);
		if (error)
			return error;

		error = getInput(IoSignal::GRIPPED_UP_STATUS, status[1], IoSignal::MODULE_1);
		if (error)
			return error;

		if (!status[0] && status[1])
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
		return -1;

	return 0;
}

int IOControl::GripperDown(BOOL safetyValidation)
{
	//If Vacuum Table is up, do not move gripper down
	if (safetyValidation) {

		BOOL vacuumUpStatus = FALSE;

		int error = getInput(IoSignal::VACUUM_UP_STATUS, vacuumUpStatus, IoSignal::MODULE_1);
		if (error)
			return error;

		if (vacuumUpStatus) {
			theApp.AddLogEvent("Cannot move Gripper Down. Vacuum Table is Up");
			return 1;
		}
	}

	int error = setOutput(IoSignal::GRIPPER_UP, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	error = setOutput(IoSignal::GRIPPER_DOWN, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	BOOL status[2];
	int repeatCount = 1000;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::GRIPPER_DOWN_STATUS, status[0], IoSignal::MODULE_1);
		if (error)
			return error;

		error = getInput(IoSignal::GRIPPED_UP_STATUS, status[1], IoSignal::MODULE_1);
		if (error)
			return error;

		if (status[0] || !status[1])
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
		return -1;

	return 0;
}

int IOControl::ClampingSequence()
{
	//1. Track Clamp 1 Down
	int error = setOutput(IoSignal::CLAMP1_UP, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	error = setOutput(IoSignal::CLAMP1_DOWN, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	//2. Track Clamp 2 Down
	error = setOutput(IoSignal::CLAMP2_UP, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	error = setOutput(IoSignal::CLAMP2_DOWN, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	//3. Check Status of Track Clamp 1 & 2
	BOOL status[2];
	int repeatCount = 1000;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::CLAMP1_SENSOR_STATUS, status[0], IoSignal::MODULE_1);
		if (error)
			return error;

		error = getInput(IoSignal::CLAMP2_SENSOR_STATUS, status[1], IoSignal::MODULE_1);
		if (error)
			return error;

		if (status[0] && status[0])
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
		return -1;

	return 0;
}

int IOControl::UnClampingSequence()
{
	//1. Track Clamp 1 Up
	int error = setOutput(IoSignal::CLAMP1_UP, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	error = setOutput(IoSignal::CLAMP1_DOWN, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	//2. Track Clamp 2 Up
	error = setOutput(IoSignal::CLAMP2_UP, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	error = setOutput(IoSignal::CLAMP2_DOWN, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	//3. Check Status of Track Clamp 1 & 2
	BOOL status[2];
	int repeatCount = 1000;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::CLAMP1_SENSOR_STATUS, status[0], IoSignal::MODULE_1);
		if (error)
			return error;

		error = getInput(IoSignal::CLAMP2_SENSOR_STATUS, status[1], IoSignal::MODULE_1);
		if (error)
			return error;

		if (!status[0] && !status[1])
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
		return -1;

	return 0;
}


int IOControl::TrackClampsUp()
{
	int error = setOutput(IoSignal::CLAMP1_UP, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	error = setOutput(IoSignal::CLAMP1_DOWN, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	BOOL status[2];
	int repeatCount = 1000;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::CLAMP1_SENSOR_STATUS, status[0], IoSignal::MODULE_1);
		if (error)
			return error;

		if (!status[0])
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
		return -1;

	return 0;
}

int IOControl::TrackClampsDown()
{
	int error = setOutput(IoSignal::CLAMP1_UP, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	error = setOutput(IoSignal::CLAMP1_DOWN, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	BOOL status[2];
	int repeatCount = 1000;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::CLAMP1_SENSOR_STATUS, status[0], IoSignal::MODULE_1);
		if (error)
			return error;

		if (status[0])
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
		return -1;

	return 0;
}

int IOControl::TrackClamps2Up()
{
	int error = setOutput(IoSignal::CLAMP2_UP, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	error = setOutput(IoSignal::CLAMP2_DOWN, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	BOOL status[2];
	int repeatCount = 1000;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::CLAMP2_SENSOR_STATUS, status[0], IoSignal::MODULE_1);
		if (error)
			return error;

		if (!status[0])
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
		return -1;

	return 0;
}

int IOControl::TrackClamps2Down()
{
	int error = setOutput(IoSignal::CLAMP2_UP, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	error = setOutput(IoSignal::CLAMP2_DOWN, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	BOOL status[2];
	int repeatCount = 1000;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::CLAMP2_SENSOR_STATUS, status[0], IoSignal::MODULE_1);
		if (error)
			return error;

		if (status[0])
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
		return -1;

	return 0;
}


// Machine Version1
int IOControl::TrackClampsUp2()
{
	OutputDebugLogTo(0, TRUE, "Start : Track Clamps UP");

	int error = setOutput(IoSignal::CLAMP1_UP, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
	{
		OutputDebugLogTo(0, TRUE, "----- ERROR : IoSignal::CLAMP1_UP_ON  - %d", error);
		return error;
	}

	error = setOutput(IoSignal::CLAMP2_UP, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
	{
		OutputDebugLogTo(0, TRUE, "----- ERROR : IoSignal::CLAMP2_UP_ON  - %d", error);
		return error;
	}

	error = setOutput(IoSignal::CLAMP1_DOWN, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
	{
		OutputDebugLogTo(0, TRUE, "----- ERROR : IoSignal::CLAMP1_DOWN_ON  - %d", error);
		return error;
	}

	error = setOutput(IoSignal::CLAMP2_DOWN, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
	{
		OutputDebugLogTo(0, TRUE, "----- ERROR : IoSignal::CLAMP2_DOWN_ON  - %d", error);
		return error;
	}

	BOOL status[2];

	int repeatCount = 1000;

	/*	do
		{
			error = getInput(IoSignal::CLAMP1_SENSOR_STATUS, status[0], IoSignal::MODULE_1);
			if (error)
			{
				OutputDebugLogTo(0, TRUE, "TrackClampsUp Error : IoSignal::CLAMP1_SENSOR_STATUS  - %d", error);
				return error;
			}

			error = getInput(IoSignal::CLAMP2_SENSOR_STATUS, status[1], IoSignal::MODULE_1);
			if (error)
			{
				OutputDebugLogTo(0, TRUE, "TrackClampsUp Error : IoSignal::CLAMP2_SENSOR_STATUS  - %d", error);
				return error;
			}

			if(!status[0] && !status[1])
				break;

			Sleep( 1 );

		} while((--repeatCount) > 0);

		if (repeatCount == 0)
		{
			OutputDebugLogTo(0, TRUE, "----- ERROR : TrackClampsUp Repeat Count is Zero");
			return -1;
		}
	*/
	return 0;
}

// Machine Version1
int IOControl::TrackClampsDown2()
{
	OutputDebugLogTo(0, TRUE, "Start : Track Clamps DOWN");

	int error = setOutput(IoSignal::CLAMP1_UP, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
	{
		OutputDebugLogTo(0, TRUE, "----- ERROR : IoSignal::CLAMP1_UP_OFF  - %d", error);
		return error;
	}

	error = setOutput(IoSignal::CLAMP2_UP, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
	{
		OutputDebugLogTo(0, TRUE, "----- ERROR : IoSignal::CLAMP2_UP_OFF  - %d", error);
		return error;
	}

	error = setOutput(IoSignal::CLAMP1_DOWN, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
	{
		OutputDebugLogTo(0, TRUE, "----- ERROR : IoSignal::CLAMP1_DOWN_OFF  - %d", error);
		return error;
	}

	error = setOutput(IoSignal::CLAMP2_DOWN, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
	{
		OutputDebugLogTo(0, TRUE, "----- ERROR : IoSignal::CLAMP2_DOWN_OFF  - %d", error);
		return error;
	}

	BOOL status[2];
	int repeatCount = 1000;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::CLAMP1_SENSOR_STATUS, status[0], IoSignal::MODULE_1);
		if (error)
		{
			OutputDebugLogTo(0, TRUE, "TrackClampsDown Error : IoSignal::CLAMP1_SENSOR_STATUS  - %d", error);
			return error;
		}

		error = getInput(IoSignal::CLAMP2_SENSOR_STATUS, status[1], IoSignal::MODULE_1);
		if (error)
		{
			OutputDebugLogTo(0, TRUE, "TrackClampsDown Error : IoSignal::CLAMP2_SENSOR_STATUS  - %d", error);
			return error;
		}

		if (status[0] && status[1])
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
	{
		OutputDebugLogTo(0, TRUE, "----- ERROR : TrackClampsDown Repeat Count is Zero");
		return -1;
	}

	return 0;
}

int IOControl::GripperClampOpen()
{
	return setOutput(IoSignal::GRIPPER_CLAMP, IoSignal::OFF, IoSignal::MODULE_1);
}

int IOControl::GripperClampClose()
{
	return setOutput(IoSignal::GRIPPER_CLAMP, IoSignal::ON, IoSignal::MODULE_1);
}

int IOControl::VacuumUp(BOOL safetyValidation)
{
	//Check if Gripper is Down before Vacuum Up
	if (safetyValidation) {

		BOOL status[2];

		int error = getInput(IoSignal::GRIPPER_DOWN_STATUS, status[0], IoSignal::MODULE_1);
		if (error)
			return error;

		error = getInput(IoSignal::GRIPPED_UP_STATUS, status[1], IoSignal::MODULE_1);
		if (error)
			return error;

		int nPos;

		if (status[0] && !status[1])
			nPos = -1;
		else if (!status[0] && status[1])
			nPos = 1;
		else
			nPos = 0;

		if (nPos != 1)
			return 1;

	}

	int error = setOutput(IoSignal::VACUUM_UP, IoSignal::ON, IoSignal::MODULE_1);
	if (error)
		return error;

	int repeatCount = 3000;
	BOOL status;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::VACUUM_UP_STATUS, status, IoSignal::MODULE_1);
		if (error)
			return error;

		if (status)
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
		return -1;

	return 0;
}

int IOControl::VacuumDown()
{
	int error = setOutput(IoSignal::VACUUM_UP, IoSignal::OFF, IoSignal::MODULE_1);
	if (error)
		return error;

	int repeatCount = 3000;
	BOOL status;

	CPerformanceTimer timer;
	timer.Reset();
	do
	{
		error = getInput(IoSignal::VACUUM_UP_STATUS, status, IoSignal::MODULE_1);
		if (error)
			return error;

		if (!status)
			break;

		Sleep(1);

		if (timer.GetElapsedMSec() > IO_CONTROL_TIME_OUT_MS) {
			repeatCount = 0;
			break;
		}

	} while ((--repeatCount) > 0);

	if (repeatCount == 0)
		return -1;

	return 0;
}

int IOControl::VacuumOn()
{
	return setOutput(IoSignal::VACUUM_ON, IoSignal::ON, IoSignal::MODULE_1);
}

int IOControl::VacuumOff()
{
	return setOutput(IoSignal::VACUUM_ON, IoSignal::OFF, IoSignal::MODULE_1);
}

int IOControl::VacuumPurgeOn()
{
	return setOutput(IoSignal::PURGE_ON, IoSignal::ON, IoSignal::MODULE_1);
}

int IOControl::VacuumPurgeOff()
{
	return setOutput(IoSignal::PURGE_ON, IoSignal::OFF, IoSignal::MODULE_1);
}

int IOControl::SetIonizerDischargeStopOn()
{
	return setOutput(IoSignal::IONIZER_DIS_STOP, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetIonizerDischargeStopOff()
{
	return setOutput(IoSignal::IONIZER_DIS_STOP, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::SetIonizerMaintenanceSignalOn()
{
	return setOutput(IoSignal::IONIZER_MAINTENANCE_SIGNAL, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetIonizerMaintenanceSignalOff()
{
	return setOutput(IoSignal::IONIZER_MAINTENANCE_SIGNAL, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::GetIonizerErrorStatus(BOOL & bOn)
{
	return getInput(IoSignal::IONIZER_ERROR_STATUS, bOn, IoSignal::MODULE_2);
}

int IOControl::SetIonizerValveOn()
{
	return setOutput(IoSignal::IONIZER_VALVE_ON, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetIonizerValveOff()
{
	return setOutput(IoSignal::IONIZER_VALVE_ON, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::SetLoaderReadyToReceiveOn()
{
	return setOutput(IoSignal::LOADER_READY_TO_RECEIVE, IoSignal::ON, IoSignal::MODULE_1);
}

int IOControl::SetLoaderReadyToReceiveOff()
{
	return setOutput(IoSignal::LOADER_READY_TO_RECEIVE, IoSignal::OFF, IoSignal::MODULE_1);
}

int IOControl::SetLoaderTransferSuccessOn()
{
	return setOutput(IoSignal::LOADER_TRANSFER_SUCCESS, IoSignal::ON, IoSignal::MODULE_1);
}

int IOControl::SetLoaderTransferSuccessOff()
{
	return setOutput(IoSignal::LOADER_TRANSFER_SUCCESS, IoSignal::OFF, IoSignal::MODULE_1);
}

int IOControl::GetLoaderPanelAvailableStatus(BOOL & bOn)
{
	return getInput(IoSignal::LOADER_PANEL_AVAILABLE_STATUS, bOn, IoSignal::MODULE_2);
}

int IOControl::GetLoaderPanelInStatus(BOOL & bOn)
{
	return getInput(IoSignal::LOADER_PANEL_IN_STATUS, bOn, IoSignal::MODULE_2);
}

int IOControl::SetUnloaderPanelAvailableOn()
{
	return setOutput(IoSignal::UNLOADER_PANEL_AVAILABLE, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetUnloaderPanelAvailableOff()
{
	return setOutput(IoSignal::UNLOADER_PANEL_AVAILABLE, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::SetUnloaderPanelPushOutOn()
{
	return setOutput(IoSignal::UNLOADER_PANEL_PUSH_OUT_ON, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetUnloaderPanelPushOutOff()
{
	return setOutput(IoSignal::UNLOADER_PANEL_PUSH_OUT_ON, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::GetUnloaderReadyToReceiveStatus(BOOL & bOn)
{
	return getInput(IoSignal::UNLOADER_READY_TO_RECEIVE_STATUS, bOn, IoSignal::MODULE_2);
}

int IOControl::GetUnloaderTransferSuccessStatus(BOOL & bOn)
{
	return getInput(IoSignal::UNLOADER_TRANSFER_SUCCESS_STATUS, bOn, IoSignal::MODULE_2);
}

int IOControl::StartButtonLampOn()
{
	return setOutput(IoSignal::START_LAMP_ON, IoSignal::ON, IoSignal::MODULE_1);
}

int IOControl::StartButtonLampOff()
{
	return setOutput(IoSignal::START_LAMP_ON, IoSignal::OFF, IoSignal::MODULE_1);
}

int IOControl::CycleStopButtonLampOn()
{
	return setOutput(IoSignal::CYCLE_STOP_LAMP_ON, IoSignal::ON, IoSignal::MODULE_1);
}

int IOControl::CycleStopButtonLampOff()
{
	return setOutput(IoSignal::CYCLE_STOP_LAMP_ON, IoSignal::OFF, IoSignal::MODULE_1);
}


int IOControl::StopButtonLampOn()
{
	return setOutput(IoSignal::STOP_LAMP_ON, IoSignal::ON, IoSignal::MODULE_1);
}

int IOControl::StopButtonLampOff()
{
	return setOutput(IoSignal::STOP_LAMP_ON, IoSignal::OFF, IoSignal::MODULE_1);
}

int IOControl::ResetButtonLampOn()
{
	return setOutput(IoSignal::RESET_LAMP_ON, IoSignal::ON, IoSignal::MODULE_1);
}

int IOControl::ResetButtonLampOff()
{
	return setOutput(IoSignal::RESET_LAMP_ON, IoSignal::OFF, IoSignal::MODULE_1);
}


int IOControl::SetTowerBlueLightOn()
{
	return setOutput(IoSignal::TOWER_BLUE_LIGHT_ON, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetTowerBlueLightOff()
{
	return setOutput(IoSignal::TOWER_BLUE_LIGHT_ON, IoSignal::OFF, IoSignal::MODULE_2);
}


int IOControl::SetTowerGreenLightOn()
{
	return setOutput(IoSignal::TOWER_GREEN_LIGHT_ON, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetTowerGreenLightOff()
{
	return setOutput(IoSignal::TOWER_GREEN_LIGHT_ON, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::SetTowerOrangeLightOn()
{
	return setOutput(IoSignal::TOWER_ORANGE_LIGHT_ON, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetTowerOrangeLightOff()
{
	return setOutput(IoSignal::TOWER_ORANGE_LIGHT_ON, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::SetTowerRedLightOn()
{
	return setOutput(IoSignal::TOWER_RED_LIGHT_ON, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetTowerRedLightOff()
{
	return setOutput(IoSignal::TOWER_RED_LIGHT_ON, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::SetBuzzerOn()
{
	return setOutput(IoSignal::SWITCH_ON_BUZZER, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetBuzzerOff()
{
	return setOutput(IoSignal::SWITCH_ON_BUZZER, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::GetGripperStatus(int & nPos)
{
	BOOL status[2];

	/*int error = getInput(IoSignal::GRIPPER_DOWN_STATUS, status[0], IoSignal::MODULE_1);
	if (error)
		return error;

	error = getInput(IoSignal::GRIPPED_UP_STATUS, status[1], IoSignal::MODULE_1);
	if (error)
		return error;*/

	UINT nIOStatus = 0;
	int nError = theApp.ioControl.GetInputStatus(nIOStatus, IoSignal::MODULE_1);
	status[0] = (nIOStatus >> IoSignal::GRIPPER_DOWN_STATUS) & 1;
	status[1] = (nIOStatus >> IoSignal::GRIPPED_UP_STATUS) & 1;
	
	if (status[0] && !status[1])
		nPos = -1;
	else if (!status[0] && status[1])
		nPos = 1;
	else
		nPos = 0;

	return nError;
}

int IOControl::GetPullPresentSensorState(BOOL & bOn)
{
	//return getInput(IoSignal::PULL_SENSOR_STATUS, bOn, IoSignal::MODULE_1);
	UINT nIOStatus = 0;
	int nError = theApp.ioControl.GetInputStatus(nIOStatus, IoSignal::MODULE_1);
	bOn = (nIOStatus >> IoSignal::PULL_SENSOR_STATUS) & 1;
	return nError;
}

int IOControl::GetPushJamSensorState(BOOL & bOn)
{
	/*int error = getInput(IoSignal::JAM_SENSOR_STATUS, bOn, IoSignal::MODULE_1);
	if (error)
		return error;*/
	UINT nIOStatus = 0;
	int nError = theApp.ioControl.GetInputStatus(nIOStatus, IoSignal::MODULE_1);
	bOn = (nIOStatus >> IoSignal::JAM_SENSOR_STATUS) & 1;

	bOn = !bOn;

	return nError;
}

int IOControl::GetSubstratePresence(BOOL & bOn)
{
	//return getInput(IoSignal::PRESENT_SENSOR_STATUS, bOn, IoSignal::MODULE_1);
	UINT nIOStatus = 0;
	int nError = theApp.ioControl.GetInputStatus(nIOStatus, IoSignal::MODULE_1);
	bOn = (nIOStatus >> IoSignal::PRESENT_SENSOR_STATUS) & 1;
	return nError;
}

int IOControl::GetVacuumSwitchOnStatus(BOOL & bOn)
{
	//return getInput(IoSignal::VACUUM_SWITCH_ON_STATUS, bOn, IoSignal::MODULE_1);
	UINT nIOStatus = 0;
	int nError = theApp.ioControl.GetInputStatus(nIOStatus, IoSignal::MODULE_1);
	bOn = (nIOStatus >> IoSignal::VACUUM_SWITCH_ON_STATUS) & 1;
	return nError;
}

int IOControl::GetVacuumUpStatus(BOOL & bOn)
{
	//return getInput(IoSignal::VACUUM_UP_STATUS, bOn, IoSignal::MODULE_1);
	UINT nIOStatus = 0;
	int nError = theApp.ioControl.GetInputStatus(nIOStatus, IoSignal::MODULE_1);
	bOn = (nIOStatus >> IoSignal::VACUUM_UP_STATUS) & 1;
	return nError;
}

int IOControl::GetAirPressureStatus(BOOL & bOn)
{
	return getInput(IoSignal::AIR_PRESSURE_STATUS, bOn, IoSignal::MODULE_1);
}

int IOControl::GetEmergencyStatus(BOOL & bOn)
{
	return getInput(IoSignal::EMERGENCY_STATUS, bOn, IoSignal::MODULE_1);
}

int IOControl::GetDoorSwitchStatus(BOOL & bOn)
{
	return getInput(IoSignal::DOOR_SWITCH_STATUS, bOn, IoSignal::MODULE_1);
}

int IOControl::GetGuardHealthyStatus(BOOL & bOn)
{
	return getInput(IoSignal::GUARD_HEALTHY_STATUS, bOn, IoSignal::MODULE_2);
}

int IOControl::GetGuardBypassStatus(BOOL & bOn)
{
	return getInput(IoSignal::GUARD_BYPASS_STATUS, bOn, IoSignal::MODULE_2);
}

int IOControl::GetIncomingAirPressureStatus(BOOL & bOn)
{
	return getInput(IoSignal::AIR_PRESSURE_STATUS, bOn, IoSignal::MODULE_1);
}

int IOControl::SetGuardDoorLockOn()
{
	return setOutput(IoSignal::GUARD_DOOR_LOCK, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetGuardDoorLockOff()
{
	return setOutput(IoSignal::GUARD_DOOR_LOCK, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::GetStartButtonStatus(BOOL & bOn)
{
	return getInput(IoSignal::START_BUTTON_STATUS, bOn, IoSignal::MODULE_1);
}

int IOControl::GetStopButtonStatus(BOOL & bOn)
{
	return getInput(IoSignal::STOP_BUTTON_STATUS, bOn, IoSignal::MODULE_1);
}

int IOControl::GetCycleStopButtonStatus(BOOL & bOn)
{
	return getInput(IoSignal::CYCLE_STOP_BUTTON_STATUS, bOn, IoSignal::MODULE_1);
}

int IOControl::GetResetButtonStatus(BOOL & bOn)
{
	return getInput(IoSignal::RESET_BUTTON_STATUS, bOn, IoSignal::MODULE_2);
}


int IOControl::GetWidthAdjustmentCrashStatus(BOOL & bOn)
{
	return getInput(IoSignal::WIDTH_ADJUSTMENT_CRASH_STATUS, bOn, IoSignal::MODULE_1);
}

int IOControl::GetInputStatus(UINT & data, int moduleId)
{
	return inputRead(data, moduleId);
}

int IOControl::getInputStatus(const std::vector < int > & ioId, std::vector < BOOL > & ioStatus, int moduleId)
{
	if (ioId.empty())
		return -1;

	for (std::vector < int > ::const_iterator io = ioId.cbegin(); io != ioId.cend(); ++io) {
		if (*io < 0 || *io > 15)
			return -1;
	}

	UINT data;

	int error = inputRead(data, moduleId);
	if (error)
		return error;

	ioStatus.resize(ioId.size());

	std::vector < BOOL > ::iterator status = ioStatus.begin();

	for (std::vector < int > ::const_iterator io = ioId.cbegin(); io != ioId.cend(); ++io, ++status)
		*status = (data >> (*io)) & 1;

	return 0;
}

int IOControl::SetK1K2On()
{
	return setOutput(IoSignal::K1_K2_CTRL, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetK1K2Off()
{
	return setOutput(IoSignal::K1_K2_CTRL, IoSignal::OFF, IoSignal::MODULE_2);
}

int IOControl::SetK3K4On()
{
	return setOutput(IoSignal::K3_K4_CTRL, IoSignal::ON, IoSignal::MODULE_2);
}

int IOControl::SetK3K4Off()
{
	return setOutput(IoSignal::K3_K4_CTRL, IoSignal::OFF, IoSignal::MODULE_2);
}