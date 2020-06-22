#pragma once

typedef int (CALLBACK* INIT_MOTION_DLL)(int nReserved);

typedef int (CALLBACK* FREE_MOTION_DLL)(int nReserved);

typedef int (CALLBACK* REGISTER_MOTOR_CARD)(int nCardNo, int nStartAxis);

typedef int (CALLBACK* REGISTER_MOTORIO_CARD)(int nCardNo);

typedef int (CALLBACK* LOAD_MOTOR_CONFIG)(char* strFile);

typedef int (CALLBACK* RELEASE_MOTOR_CARD)(int nCardNo);

typedef int (CALLBACK* GET_AXIS_COUNT)();

typedef int (CALLBACK* INITIALIZE_AXIS)(int nAxis);

typedef int (CALLBACK* FREE_AXIS)(int nAxis, BOOL bShutdown);

typedef int (CALLBACK* SET_HOME_MODE)(int nAxis, int nHomeMode);

typedef int (CALLBACK* SET_SERVO_ON)(int nAxis, int nServoOn);

typedef int (CALLBACK* ABSOLUTE_MOVE)(int nAxis, 
									  int nPos, 
									  int nMaxVel, 
									  BOOL bWait);

typedef int (CALLBACK* RELATIVE_MOVE)(int nAxis, 
									  int nPos, 
									  int nMaxVel, 
									  BOOL bWait);

typedef int (CALLBACK* HOME_MOVE)(int nAxis, 
								  int nMode,
								  int nDir,
								  int nStartVel, 
								  int nMaxVel, 
								  BOOL bWait);

typedef int (CALLBACK* SET_WAIT_TIMEOUT)(int nWaitTimeOut);

typedef int (CALLBACK* WAIT_FOR_MOTION_DONE)(int nAxis, int nTimeOut, BOOL bIntMode);

typedef int (CALLBACK* WAIT_FOR_HOME_DONE)(int nAxis, int nTimeOut);

typedef int (CALLBACK* STOP_AXIS)(int nAxis, int nDecc);

typedef int (CALLBACK* GET_CURRENT_POSITION)(int nAxis, int* pnPosition);

typedef int (CALLBACK* SET_CURRENT_POSITION)(int nAxis, int nPosition);

typedef int (CALLBACK* ENABLE_INTERRUPT)(int nCardNo, int nEnable);

typedef int (CALLBACK* SET_INT_FACTOR)(int nAxis, UINT nIntFactor);

typedef int (CALLBACK* GET_IO_STATUS)(int nAxis, UINT* pnIoStatus);

typedef int (CALLBACK* GET_ALARM_STATUS)(int nAxis, BOOL* bAlarm, BOOL* bEl);

typedef int (CALLBACK* SET_ACCELERATION)(int nAxis, int nAcc, int nDecc);

typedef int (CALLBACK* EMERGENCY_STOP)(int nAxis);

typedef int (CALLBACK* SET_ALARM)(int nAxis);

typedef int (CALLBACK* RESET_ALARM)(int nCardNo, int nPortNo, int nAxis);

typedef int (CALLBACK* IO_IN_PORT_READ)(int nCardNo, int nPortNo, UINT* pData);

typedef int (CALLBACK* IO_IN_PORT_READ_BIT)(int nCardNo, int nPortNo,int nChannelNo, UINT* pData);

typedef int (CALLBACK* IO_OUTPORT_WRITE_BIT)(int nCardNo, int nPortNo,int nChannelNo, UINT pData);

typedef int (CALLBACK* IO_OUT_PORT_READ_BIT)(int nCardNo, int nPortNo,int nChannelNo, UINT* pData);

typedef int (CALLBACK* IO_OUT_PORT_READ)(int nCardNo, int nPortNo, UINT* pData);

typedef int (CALLBACK* IO_OUTPORT_WRITE)(int nCardNo, int nPortNo, UINT nData);

typedef int (CALLBACK* GET_SERVO_ALARM_STATUS)(int nAxis, long* lAlarmNo, long* lAlarmDetail);

typedef int (CALLBACK* ABSOLUTE_LINEAR_MOVE)(int nDimension, int* pAxisIDArray, int* pPositionArray, int nMaxLinearSpeed);

typedef int (CALLBACK* RELATIVE_LINEAR_MOVE)(int nDimension, int* pAxisIDArray, int* pDistanceArray, int nMaxLinearSpeed);

typedef int (CALLBACK* SET_SOFT_LIMITS)(int nAxis, int nNegLimit, int nPosLimit);

typedef int (CALLBACK* RESET_SOFT_LIMITS)(int nAxis);

	enum Home
	{
		HomeLow,
		HomeHigh
	};
	enum Origin
	{
		NoLatch,
		Latch
	};
	enum Index
	{
		IndexLow,
		IndexHigh
	};
	enum Alarm
	{
		AlarmLow,
		AlarmHigh
	};
	enum AlarmStop
	{
		Imd,
		Decel
	};
	enum Inp
	{
		InpLow,
		InpHigh
	};
	enum InpSignal
	{
		Disable,
		Enable
	};
	enum ServoOn
	{
		Low,
		High
	};
	enum PulseOutput
	{
		Dir,
		CCW
	};


struct MotorAxisParms
{
public:
	int nCardNo;
	int nAxis;

	int nHomeMode;
	bool bHomeHunt;

	int nOriginLogic;
	int nOrigin;
	int nIndex;
	int nAlarm;
	int nAlarmStop;
	int nInp;
	int nInpSignal;
	int nServoOn;
	int nPulseOutput;
	int nEzLogic;

	MotorAxisParms()
	{
		nCardNo = -1;
		nAxis = -1;

		nOriginLogic = HomeHigh;
		nOrigin = NoLatch;
		nIndex = IndexLow;
		nAlarm = AlarmHigh;
		nAlarmStop = Imd;
		nInpSignal = Disable;
		nInp = InpLow;
		nServoOn = High;
		nPulseOutput = 4;

		nHomeMode = 9;
		bHomeHunt = true;

		nEzLogic = 0;
	}
};

class CMotorInterface
{
public :
	HINSTANCE			hDLL;

	INIT_MOTION_DLL			lpInitMotionDLL;
	FREE_MOTION_DLL			lpFreeMotionDLL;
	REGISTER_MOTOR_CARD		lpRegisterCard;
	RELEASE_MOTOR_CARD		lpReleaseCard;
	LOAD_MOTOR_CONFIG		lpLoadConfigFromFile;
	REGISTER_MOTORIO_CARD	lpRegisterIOCard;
	RELEASE_MOTOR_CARD		lpReleaseIOCard;
	GET_AXIS_COUNT			lpGetAxisCount;
	INITIALIZE_AXIS			lpInitializeAxis;
	FREE_AXIS				lpFreeAxis;
	SET_HOME_MODE			lpSetHomeMode;
	SET_SERVO_ON			lpSetServoOn;
	ABSOLUTE_MOVE			lpAbsoluteMove;
	RELATIVE_MOVE			lpRelativeMove;
	HOME_MOVE				lpHomeMove;
	SET_WAIT_TIMEOUT		lpSetWaitTimeOut;
	WAIT_FOR_MOTION_DONE	lpWaitForMotionDone;
	WAIT_FOR_HOME_DONE		lpWaitForHomeDone;
	STOP_AXIS				lpStopAxis;
	GET_CURRENT_POSITION	lpGetCurrentPosition;
	SET_CURRENT_POSITION	lpSetCurrentPosition;
	ENABLE_INTERRUPT		lpEnableInterrupt;
	FREE_AXIS				lpResetFieldBusInterrupt;
	SET_INT_FACTOR			lpSetIntFactor;
	GET_IO_STATUS			lpGetIOStatus;
	GET_ALARM_STATUS		lpGetAlarmStatus;
	SET_ACCELERATION		lpSetAcceleration;
	EMERGENCY_STOP			lpEmergencyStopAxis;
	SET_ALARM				lpSetAlarm;
	RESET_ALARM				lpResetAlarm;
	IO_IN_PORT_READ			lpInPortRead;
	IO_IN_PORT_READ_BIT		lpInPortReadBit;
	IO_OUTPORT_WRITE		lpOutPortWrite;
	IO_OUTPORT_WRITE_BIT	lpOutPortWriteBit;
	GET_SERVO_ALARM_STATUS	lpGetServoAlarmStatus;
	ABSOLUTE_LINEAR_MOVE	lpAbsoluteLinearMove;
	RELATIVE_LINEAR_MOVE	lpRelativeLinearMove;
	IO_OUT_PORT_READ_BIT    lpOutPortReadBit;
	SET_SOFT_LIMITS			lpSetSoftLimits;
	RESET_SOFT_LIMITS		lpResetSoftLimits;


	CMotorInterface();

	MotorAxisParms axisParms[8];

	int LoadMotorDll(CString strMotorCard);
	int RegisterCard(int nCardNo, int nStartAxis);
	int InitializeCard(char* strParmFile);
	int ReleaseCard(int nCardNo);
	int RegisterIOCard(int nCardNo);
	int ReleaseIOCard(int nCardNo);
	int InitializeAxis(int nAxis);
	int FreeAxis(int nAxis, BOOL bShutdown);
	int WaitForMotionDone(int nAxis, int nTimeOut, BOOL bIntMode=TRUE);
	int HomeMove(int nAxis, int nMode, int nDir, int nStartVelocity, int nMaxVelocity, BOOL bWait);
	int SetServoOn(int nAxis, BOOL bServoOn);
	int GetAlarmStatus(int nAxis, BOOL& bAlarm, BOOL& bEl);
	int SetAcceleration(int nAxis, int nAcc, int nDecc);

	int EmergencyStopAxis(int nAxis);
	int SetAlarm(int nAxis);
	int ResetAlarm(int nCardNo, int nPortNo, int nAxis);
	int OutPortRead(int nCardNo, int nPortNo, UINT* pnData);
	int InPortRead(int nCardNo, int nPortNo, UINT* pData);
	int InPortReadBit(int nCardNo, int nPortNo,int nChannelNo, UINT* pData);
	int OutPortWriteBit(int nCardNo, int nPortNo,int nChannelNo, UINT nData);
	int OutPortWrite(int nCardNo, int nPortNo, UINT nData);
	int OutPortReadBit(int nCardNo, int nPortNo,int nChannelNo, UINT* pData);
	int GetServoAlarmStatus(int nAxis, long* lAlarmNo, long* lAlarmDetail);
	int AbsoluteLinearMove(int nDimension, int* pAxisIDArray, int* pPositionArray, int nMaxLinearSpeed);
	int RelativeLinearMove(int nDimension, int* pAxisIDArray, int* pDistanceArray, int nMaxLinearSpeed);
	int GetIoStatus(int nAxis, UINT* nData);
	int EnableInterrupt(int nAxis);
	int SetSoftLimits(int nAxis, int nNegLimit, int nPosLimit);
	int ResetSoftLimits(int nAxis);
};
