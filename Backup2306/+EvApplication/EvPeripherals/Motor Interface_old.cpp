#include "stdafx.h"
#include "Motor_Interface.h"

#include "Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMotorInterface::CMotorInterface()
{
	hDLL = 0;
	lpInitMotionDLL			= 0;
	lpFreeMotionDLL			= 0;
	lpRegisterCard			= 0;
	lpReleaseCard			= 0;
	lpLoadConfigFromFile	= 0;
	lpRegisterIOCard		= 0;
	lpReleaseIOCard			= 0;
	lpGetAxisCount			= 0;
	lpInitializeAxis		= 0;
	lpFreeAxis				= 0;
	lpSetHomeMode			= 0;
	lpSetServoOn			= 0;
	lpAbsoluteMove			= 0;
	lpRelativeMove			= 0;
	lpHomeMove				= 0;
	lpSetWaitTimeOut		= 0;
	lpWaitForMotionDone		= 0;
	lpWaitForHomeDone		= 0;
	lpStopAxis				= 0;
	lpGetCurrentPosition	= 0;
	lpSetCurrentPosition	= 0;
	lpEnableInterrupt		= 0;
	lpResetFieldBusInterrupt= 0;
	lpSetIntFactor			= 0;
	lpGetIOStatus			= 0;
	lpGetAlarmStatus		= 0;
	lpSetAcceleration		= 0;
	lpEmergencyStopAxis		= 0;
	lpSetAlarm				= 0;
	lpResetAlarm			= 0;
	lpInPortRead			= 0;
	lpInPortReadBit			= 0;
	lpOutPortWrite			= 0;
	lpOutPortWriteBit		= 0;
	lpGetServoAlarmStatus	= 0;
	lpAbsoluteLinearMove	= 0;
	lpRelativeLinearMove	= 0;
	lpOutPortReadBit		= 0;
	lpSetSoftLimits			= 0;
	lpResetSoftLimits		= 0;
}

int CMotorInterface::LoadMotorDll(CString strMotionCard)
{
	OutputDebugLogTo(0, TRUE, "Registering Motion Card %s ...", strMotionCard);

	if(strMotionCard.CompareNoCase("none") != 0) {
		hDLL = AfxLoadLibrary(strMotionCard);

		if(hDLL==NULL) {
			AfxMessageBox("Error Loading Motion DLL");
			return -1;
		}

		OutputDebugLogTo(0, TRUE, "Registering Motion Card %s ...", strMotionCard);

		lpInitMotionDLL = (INIT_MOTION_DLL) GetProcAddress (hDLL,"InitMotionDLL");
		if(!lpInitMotionDLL)
			OutputDebugLogTo(0, TRUE, "Failed to register IO Card InitMotionDLL() Function");


		lpFreeMotionDLL = (FREE_MOTION_DLL) GetProcAddress (hDLL,"FreeMotionDLL");
		if(!lpFreeMotionDLL)
			OutputDebugLogTo(0, TRUE, "Failed to register IO Card FreeMotionDLL() Function");


		lpRegisterCard = (REGISTER_MOTOR_CARD) GetProcAddress (hDLL, "RegisterCard");
		if(!lpRegisterCard)
			OutputDebugLogTo(0, TRUE, "Failed to register RegisterCard() Function");

		lpLoadConfigFromFile = (LOAD_MOTOR_CONFIG) GetProcAddress (hDLL, "LoadConfigFromFile");
		if(!lpLoadConfigFromFile)
			OutputDebugLogTo(0, TRUE, "Failed to register LoadConfigFromFile() Function");

		lpReleaseCard = (RELEASE_MOTOR_CARD) GetProcAddress (hDLL, "ReleaseCard");
		if(!lpReleaseCard)
			OutputDebugLogTo(0, TRUE, "Failed to register ReleaseCard() Function");

		lpRegisterIOCard = (REGISTER_MOTORIO_CARD) GetProcAddress (hDLL, "RegisterIOCard");
		if(!lpRegisterIOCard)
			OutputDebugLogTo(0, TRUE, "Failed to register RegisterIOCard() Function");

		lpReleaseIOCard = (RELEASE_MOTOR_CARD) GetProcAddress (hDLL, "ReleaseIOCard");
		if(!lpReleaseCard)
			OutputDebugLogTo(0, TRUE, "Failed to register ReleaseIOCard() Function");

		lpGetAxisCount = (GET_AXIS_COUNT) GetProcAddress (hDLL, "GetAxisCount");
		if(!lpGetAxisCount)
			OutputDebugLogTo(0, TRUE, "Failed to register GetAxisCount() Function");


		lpInitializeAxis = (INITIALIZE_AXIS) GetProcAddress (hDLL, "InitializeAxis");
		if(!lpInitializeAxis)
			OutputDebugLogTo(0, TRUE, "Failed to register InitializeAxis() Function");


		lpFreeAxis = (FREE_AXIS) GetProcAddress (hDLL, "FreeAxis");
		if(!lpFreeAxis)
			OutputDebugLogTo(0, TRUE, "Failed to register FreeAxis() Function");

		lpSetHomeMode = (SET_HOME_MODE) GetProcAddress (hDLL, "SetHomeMode");
		if(!lpSetHomeMode)
			OutputDebugLogTo(0, TRUE, "Failed to register SetHomeMode() Function");

		lpSetServoOn = (SET_SERVO_ON) GetProcAddress (hDLL,"SetServoOn");
		if(!lpSetServoOn)
			OutputDebugLogTo(0, TRUE, "Failed to register ServoOn() Function");

		lpAbsoluteMove = (ABSOLUTE_MOVE) GetProcAddress (hDLL, "AbsoluteMove");
		if(!lpAbsoluteMove)
			OutputDebugLogTo(0, TRUE, "Failed to register AbsoluteMove() Function");

		lpRelativeMove = (RELATIVE_MOVE) GetProcAddress (hDLL, "RelativeMove");
		if(!lpRelativeMove)
			OutputDebugLogTo(0, TRUE, "Failed to register RelativeMove() Function");

		lpHomeMove = (HOME_MOVE) GetProcAddress (hDLL, "HomeMove");
		if(!lpHomeMove)
			OutputDebugLogTo(0, TRUE, "Failed to register HomeMove() Function");

		lpSetWaitTimeOut = (SET_WAIT_TIMEOUT) GetProcAddress (hDLL, "SetWaitTimeOut");
		if(!lpSetWaitTimeOut)
			OutputDebugLogTo(0, TRUE, "Failed to register SetWaitTimeOut() Function");

		lpWaitForMotionDone = (WAIT_FOR_MOTION_DONE) GetProcAddress (hDLL, "WaitForMotionDone");
		if(!lpWaitForMotionDone)
			OutputDebugLogTo(0, TRUE, "Failed to register WaitForMotionDone() Function");

		lpWaitForHomeDone = (WAIT_FOR_HOME_DONE) GetProcAddress (hDLL, "WaitForHomeDone");
		if(!lpWaitForHomeDone)
			OutputDebugLogTo(0, TRUE, "Failed to register WaitForHomeDone() Function");

		lpStopAxis = (STOP_AXIS) GetProcAddress (hDLL, "Stop");
		if(!lpStopAxis)
			OutputDebugLogTo(0, TRUE, "Failed to register StopAxis() Function");

		lpGetCurrentPosition = (GET_CURRENT_POSITION) GetProcAddress (hDLL, "GetCurrentPosition");
		if(!lpGetCurrentPosition)
			OutputDebugLogTo(0, TRUE, "Failed to register GetCurrentPosition() Function");

		lpSetCurrentPosition = (SET_CURRENT_POSITION) GetProcAddress (hDLL, "SetCurrentPosition");
		if(!lpSetCurrentPosition)
			OutputDebugLogTo(0, TRUE, "Failed to register SetCurrentPosition() Function");

		lpEnableInterrupt = (ENABLE_INTERRUPT) GetProcAddress (hDLL, "EnableInterrupt");
		if(!lpEnableInterrupt)
			OutputDebugLogTo(0, TRUE, "Failed to register EnableInterrupt() Function");

		lpResetFieldBusInterrupt = (FREE_AXIS) GetProcAddress (hDLL, "ResetFieldBusInterrupt");
		if(!lpResetFieldBusInterrupt)
			OutputDebugLogTo(0, TRUE, "Failed to register ResetFieldBusInterrupt() Function");

		lpSetIntFactor = (SET_INT_FACTOR) GetProcAddress (hDLL, "SetIntFactor");
		if(!lpSetIntFactor)
			OutputDebugLogTo(0, TRUE, "Failed to register SetIntFactor() Function");

		lpGetIOStatus = (GET_IO_STATUS) GetProcAddress (hDLL, "GetIOStatus");
		if(!lpGetIOStatus)
			OutputDebugLogTo(0, TRUE, "Failed to register GetIOStatus() Function");

		lpGetAlarmStatus = (GET_ALARM_STATUS) GetProcAddress (hDLL, "GetAlarmStatus");
		if(!lpGetAlarmStatus)
			OutputDebugLogTo(0, TRUE, "Failed to register GetAlarmStatus() Function");

		lpSetAcceleration = (SET_ACCELERATION) GetProcAddress (hDLL, "SetAcceleration");
		if(!lpSetAcceleration)
			OutputDebugLogTo(0, TRUE, "Failed to register SetAcceleration() Function");

		lpEmergencyStopAxis = (EMERGENCY_STOP) GetProcAddress (hDLL, "EmergencyStop");
		if(!lpEmergencyStopAxis)
			OutputDebugLogTo(0, TRUE, "Failed to register EmergencyStop() Function");

		lpSetAlarm = (SET_ALARM) GetProcAddress (hDLL, "SetAlarm");
		if (!lpSetAlarm)
			OutputDebugLog("Failed to register SetAlarm() Function");

		lpResetAlarm = (RESET_ALARM) GetProcAddress (hDLL, "ResetAlarm");
		if (!lpResetAlarm)
			OutputDebugLog("Failed to register ResetAlarm() Function");

		lpInPortRead = (IO_IN_PORT_READ) GetProcAddress (hDLL, "InPortRead");
		if (!lpInPortRead)
			OutputDebugLog("Failed to register InPortRead() Function");

		lpInPortReadBit = (IO_IN_PORT_READ_BIT)GetProcAddress (hDLL, "InPortReadBit");
		if (!lpInPortReadBit)
			OutputDebugLog("Failed to register InPortReadBit() Function");

		lpOutPortReadBit = (IO_OUT_PORT_READ_BIT)GetProcAddress (hDLL, "OutPortReadBit");
		if (!lpOutPortReadBit)
			OutputDebugLog("Failed to register OutPortReadBit() Function");

		lpOutPortWrite = (IO_OUTPORT_WRITE) GetProcAddress (hDLL, "OutPortWrite");
		if (!lpOutPortWrite)
			OutputDebugLog("Failed to register OutPortWrite() Function");

		lpOutPortWriteBit = (IO_OUTPORT_WRITE_BIT) GetProcAddress (hDLL, "OutPortWriteBit");
		if (!lpOutPortWriteBit)
			OutputDebugLog("Failed to register OutPortWriteBit() Function");

		lpGetServoAlarmStatus = (GET_SERVO_ALARM_STATUS) GetProcAddress (hDLL, "GetServoAlarmStatus");
		if (!lpGetServoAlarmStatus)
			OutputDebugLog("Failed to register GetServoAlarmStatus() Function");

		lpAbsoluteLinearMove = (ABSOLUTE_LINEAR_MOVE) GetProcAddress (hDLL, "AbsoluteLinearMove");
		if (!lpAbsoluteLinearMove)
			OutputDebugLog("Failed to register AbsoluteLinearMove() Function");

		lpRelativeLinearMove = (RELATIVE_LINEAR_MOVE) GetProcAddress (hDLL, "RelativeLinearMove");
		if (!lpRelativeLinearMove)
			OutputDebugLog("Failed to register RelativeLinearMove() Function");


		lpSetSoftLimits = (SET_SOFT_LIMITS) GetProcAddress (hDLL, "SetSoftLimits");
		if(!lpSetSoftLimits)
			OutputDebugLogTo(0, TRUE, "Failed to register SetSoftLimits() Function");

		lpResetSoftLimits = (RESET_SOFT_LIMITS) GetProcAddress (hDLL, "ResetSoftLimits");
		if(!lpResetSoftLimits)
			OutputDebugLogTo(0, TRUE, "Failed to register ResetSoftLimits() Function");

		if(lpInitMotionDLL)
			lpInitMotionDLL(0);
	}
	else {
		OutputDebugLogTo(0, TRUE, "No Motion Card");
	}

	return 0;
}

int CMotorInterface::RegisterCard(int nCardNo, int nStartAxis)
{
	int nError;

	if(lpRegisterCard) {
		nError = lpRegisterCard(nCardNo, nStartAxis);
		if(nError) {
			AfxMessageBox("Failed to Register Motor Card");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Registered Motor Card");
	}
	else
		OutputDebugLogTo(0, TRUE, "No RegisterCard() Function");

	return 0;
}

int CMotorInterface::InitializeCard(char* strParmFile)
{
	int nError;

	if(lpLoadConfigFromFile) {
		nError = lpLoadConfigFromFile(strParmFile);
		if(nError) {
			///AfxMessageBox("Failed to load motion config from file");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Loaded motion config from file");
	}
	else
		OutputDebugLogTo(0, TRUE, "No LoadConfigFromFile() Function");



	///if(lpSetServoOn != NULL)
	///	lpSetServoOn(nAxis, pAxisParm->nServoOn);
	
	///HomeMove(pMM, nAxis, -1000, -1000, -10000, 100, 100, true);
	///OutputDebugLogTo(0, TRUE, "HomeMove done");

	return 0;
}

int CMotorInterface::ReleaseCard(int nCardNo)
{
	int nError;

	if(lpReleaseCard) {
		nError = lpReleaseCard(nCardNo);
		if(nError) {
			AfxMessageBox("Failed to Release Motor Card");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Released Motor Card");
	}
	else 
		OutputDebugLogTo(0, TRUE, "No ReleaseCard() Function");

	return 0;
}

int CMotorInterface::RegisterIOCard(int nCardNo)
{
	int nError;

	if(lpRegisterIOCard) {
		nError = lpRegisterIOCard(nCardNo);
		if(nError) {
			AfxMessageBox("Failed to Register Motor I/O Card");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Registered Motor I/O Card");
	}
	else
		OutputDebugLogTo(0, TRUE, "No RegisterIOCard() Function");

	return 0;
}

int CMotorInterface::ReleaseIOCard(int nCardNo)
{
	int nError;

	if(lpReleaseIOCard) {
		nError = lpReleaseIOCard(nCardNo);
		if(nError) {
			AfxMessageBox("Failed to Release Motor I/O Card");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Released Motor I/O Card");
	}
	else 
		OutputDebugLogTo(0, TRUE, "No ReleaseIOCard() Function");

	return 0;
}

int CMotorInterface::InitializeAxis(int nAxis)
{
	int nError;

	if(lpInitializeAxis) {
		nError = lpInitializeAxis(nAxis);
		if(nError) {
			AfxMessageBox("Failed to Initialize Motor Axis");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Initialized Motor Axis %d", nAxis);
	}
	else 
		OutputDebugLogTo(0, TRUE, "No InitializeAxis() Function");


	return 0;
}

int CMotorInterface::FreeAxis(int nAxis, BOOL bShutdown)
{
	int nError;

	if(lpFreeAxis) {
		nError = lpFreeAxis(nAxis, bShutdown);
		if(nError) {
			//AfxMessageBox("Failed to Free Motor Axis");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Freed Motor Axis");
	}
	else 
		OutputDebugLogTo(0, TRUE, "No FreeAxis() Function");

	//if(lpSetServoOn != NULL)
	//	lpSetServoOn(nAxis, 0);
		//lpSetServoOn(nAxis, !axisParms[0].nServoOn);

	return 0;
}

int CMotorInterface::WaitForMotionDone(int nAxis, int nTimeOut, BOOL bIntMode)
{
	if(lpWaitForMotionDone==NULL) {
		OutputDebugLogTo(1, TRUE, "[Axis%d] No WaitForMotionDone function", nAxis);
		return -1;
	}


	return lpWaitForMotionDone(nAxis, nTimeOut, bIntMode);
}

int CMotorInterface::HomeMove(int nAxis,  int nMode, int nDir, int nStartVelocity, int nMaxVelocity, BOOL bWait)
{
	if(lpHomeMove==NULL)
		return -1;

	return lpHomeMove(nAxis, nMode, nDir, nStartVelocity, nMaxVelocity, bWait);
}

int CMotorInterface::SetServoOn(int nAxis, BOOL bServoOn)
{
	if(lpSetServoOn != NULL)
		lpSetServoOn(nAxis, bServoOn);
		//lpSetServoOn(nAxis, bServoOn ? axisParms[nAxis].nServoOn : !axisParms[nAxis].nServoOn);

	return 0;
}

int CMotorInterface::GetAlarmStatus(int nAxis, BOOL& bAlarm, BOOL& bEl)
{
	if(lpGetAlarmStatus != NULL)
		lpGetAlarmStatus(nAxis, &bAlarm, &bEl);

	return 0;
}

int CMotorInterface::SetAcceleration(int nAxis, int nAcc, int nDecc)
{
	if(lpSetAcceleration != NULL)
		lpSetAcceleration(nAxis, nAcc, nDecc);

	return 0;
}

int CMotorInterface::EmergencyStopAxis(int nAxis)
{
	if(lpEmergencyStopAxis != NULL)
		lpEmergencyStopAxis(nAxis);

	return 0;
}

int CMotorInterface::SetAlarm(int nAxis)
{
	if(lpSetAlarm != NULL)
		lpSetAlarm(nAxis);

	return 0;
}

int CMotorInterface::ResetAlarm(int nCardNo, int nPortNo, int nAxis)
{
	if(lpResetAlarm != NULL)
		lpResetAlarm(nCardNo, nPortNo, nAxis);

	return 0;
}

int CMotorInterface::InPortRead(int nCardNo, int nPortNo, UINT* pData)
{
	if(lpInPortRead != NULL)
		lpInPortRead(nCardNo, nPortNo, pData);

	return 0;
}
int CMotorInterface::OutPortRead(int nCardNo, int nPortNo, UINT *pnData)
{
	if (lpInPortRead)
		return lpInPortRead(nCardNo, nPortNo, pnData);

	return 0;
}

int CMotorInterface::InPortReadBit(int nCardNo, int nPortNo,int nChannelNo, UINT* pData)
{
	if(lpInPortReadBit != NULL)
		lpInPortReadBit(nCardNo, nPortNo,nChannelNo, pData);

	return 0;
}

int CMotorInterface::OutPortReadBit(int nCardNo, int nPortNo,int nChannelNo, UINT* pData)
{
	if(lpOutPortReadBit != NULL)
		lpOutPortReadBit(nCardNo, nPortNo,nChannelNo, pData);

	return 0;
}


int CMotorInterface::OutPortWrite(int nCardNo, int nPortNo, UINT nData)
{
	if (lpOutPortWrite != NULL)
	{
	//	OutputDebugLogTo(8, TRUE, "Unable To Write  %d %d %d ", nCardNo, nPortNo,  nData);
		lpOutPortWrite(nCardNo, nPortNo, nData);
	}

	return 0;
}

int CMotorInterface::OutPortWriteBit(int nCardNo, int nPortNo,int nChannelNo,UINT nData)
{
	if(lpOutPortWriteBit != NULL)
	{
//		OutputDebugLogTo(8, TRUE, "Write %d %d %d %d Bit", nCardNo, nPortNo, nChannelNo, nData);
		lpOutPortWriteBit(nCardNo, nPortNo,nChannelNo,nData);
		}

	return 0;
}

int CMotorInterface::GetServoAlarmStatus(int nAxis, long* lAlarmNo, long* lAlarmDetail)
{
	if(lpGetServoAlarmStatus != NULL)
		lpGetServoAlarmStatus(nAxis, lAlarmNo, lAlarmDetail);

	return 0;
}

int CMotorInterface::AbsoluteLinearMove(int nDimension, int* pAxisIDArray, int* pPositionArray, int nMaxLinearSpeed)
{
	if(lpAbsoluteLinearMove != NULL)
		lpAbsoluteLinearMove(nDimension, pAxisIDArray, pPositionArray, nMaxLinearSpeed);

	return 0;
}

int CMotorInterface::RelativeLinearMove(int nDimension, int* pAxisIDArray, int* pDistanceArray, int nMaxLinearSpeed)
{
	if(lpRelativeLinearMove != NULL)
		lpRelativeLinearMove(nDimension, pAxisIDArray, pDistanceArray, nMaxLinearSpeed);

	return 0;
}


int CMotorInterface::EnableInterrupt(int nAxis)
{
	if(lpEnableInterrupt != NULL)
		lpEnableInterrupt(0,nAxis);

	return 0;
}


int CMotorInterface::GetIoStatus(int nAxis, UINT* nData)
{
	if(lpGetIOStatus != NULL)
		lpGetIOStatus(nAxis,nData);

	return 0;
}

int CMotorInterface::SetSoftLimits(int nAxis, int nNegLimit, int nPosLimit)
{
	if(lpSetSoftLimits)
		return lpSetSoftLimits(nAxis, nNegLimit, nPosLimit);

	return 0;
}

int CMotorInterface::ResetSoftLimits(int nAxis)
{
	if(lpResetSoftLimits)
		return lpResetSoftLimits(nAxis);

	return 0;
}