#pragma once

class CGlobalBackgroundThread 
	
{
public:

	CGlobalBackgroundThread();
	~CGlobalBackgroundThread();
		//User commands
	int Run();
	void Stop();
	//	void NextStep();

	int Retry(BOOL bReScan = FALSE);

	int RunBackgroundEvents();

//	BOOL GetDryRunMode() const;
//	void SetDryRunMode(BOOL bOn = FALSE);

	BOOL IsHardwareReady() const;
	BOOL IsRunning() const;
	BOOL IsModuleReplacement() const;
	BOOL IsPaused() const;
	BOOL IsReScanEnabled() const;

	int RunSequence();

	//	void FAULT_Light(BOOL bOn = TRUE);
	void CYCLE_STOP_Light(BOOL bOn = TRUE);
	void START_Light(BOOL bOn = TRUE);
	void STOP_Light(BOOL bOn = TRUE);
	void ACTION_Light(BOOL bOn = TRUE);

	void CURTAIN_BYPASS(BOOL bOn = TRUE);
	void RESET_Light(BOOL bOn = TRUE);
//	void IDLE_Light(BOOL bOn = TRUE);
//	void ACTION_Light(BOOL bOn = TRUE);

	void RemoveSequence();
	void CameraLiveOff();
	void RemoveGlobalEvents();


	int nCardNo;
	int nPortNo;

//	CPerformanceTimer timer_sequence;
//	CPerformanceTimer timerUPH;

	IOControl & ioControl;
	MotionParameters & motionParameters;

	BOOL hardwareReady;

	BOOL sequenceRunning;
	BOOL moduleReplacement;
	BOOL bSequenceStop;
	BOOL bSequenceError;
	BOOL bDryRunMode;

	BOOL bAirPressureError;
	BOOL bResetModeActivated;
	BOOL bEmergencyStopActivated;
	BOOL bCycleStopPressed;
	BOOL bDryRun;
	BOOL bWetRun;
	BOOL bCloseSequence;
	CPerformanceTimer SequenceTime;
	CPerformanceTimer NetSequenceTime;

	BOOL killThreads;

	CRITICAL_SECTION CriticalSectionSequence; //For proper data cleaning

	CRITICAL_SECTION CriticalSectionUserCommand; //For Hardware Button interraction

	//												 //Global background events
		BOOL killGlobalThreads;
	    HANDLE * pEventGlobalThreadClosed;
		int nGlobalEventNo;

		volatile long nGlobalThreadID;
		volatile long nDestroyedGlobalThreadNo;
		bool _enableMachineResetOperation;

	//	//Proper thread manipulation
		volatile long runningModuleCount;
		volatile long destroyedModuleCount;
		HANDLE SequenceLevelDestroyed;

	//	//Check temporary thread destroying
		volatile long temporaryThreadCount;

		//Step mode
		BOOL bEnableStepMode;
		HANDLE EventNextStep;
		CRITICAL_SECTION CriticalSectionStepMode;

	//	// Modular Run Mode (Individual Module)
		volatile bool _moduleRunMode;
		volatile bool _transportRunning;

	//	//Error handling
		HANDLE EventRetry;
		BOOL bReScanSequence;
//		BOOL bLevelError;
		CRITICAL_SECTION CriticalSectionErrorHandling;

		HANDLE XHomeDone;
		HANDLE ZHomeDone;
		int nHomeError[4];//X, Y, ST1-Z and ST2-Z


		HANDLE InitMotion[4];
		HANDLE StartMotion[2];
		HANDLE StartMotionTHR[2];
		int nInitMotionError[4];//X, Y, ST1-Z and ST2-Z

		int nStartMotionError[3];//X, Y, ST1-Z
		int nStartMotionErrorTHR[3];//X, Y, ST2-Z

		int CreateBackgroundEvents(int nEventsNo); //	<---- For user interraction (buttons) and check statuses

		static UINT GlobalBackgroundThread(LPVOID pParam);
		void GlobalBackgroundThreadFunction(const int nEventID);

		static UINT SequenceRun(LPVOID pParam);

		void SequenceFunction();

		int StepMode();
		int ErrorHandling(CString strError = "", BOOL bError= TRUE);

		static UINT XServoMotor(LPVOID pParam);
		static UINT ZServoMotor(LPVOID pParam);

		static UINT InitXMotor(LPVOID pParam);
		static UINT InitYTopMotor(LPVOID pParam);
		static UINT InitZMotor(LPVOID pParam);
		static UINT InitZMotorTHR(LPVOID pParam);

		static UINT StartXMotor(LPVOID pParam);
		static UINT StartXMotorEncap(LPVOID pParam);
		static UINT StartYTopMotor(LPVOID pParam);
		static UINT StartYTopMotorEncap(LPVOID pParam);

		static UINT StartXMotorTHR(LPVOID pParam);
		static UINT StartYTopMotorTHR(LPVOID pParam);

		static UINT ScanYTopMotorTHR(LPVOID pParam);

		HANDLE MoveYAxisScanTHR;
		HANDLE MoveYAxisScanDoneTHR;
		int nMotorYAxisScanPosTHR;
		BOOL bScanModeTHR;
		BOOL bEmptyScanTHR;
		int nScanYAxisErrorTHR;
		BOOL bSequenceGrab;
		BOOL bImmediateStopActivated;
		BOOL bCurtainMuteActivated;

		int HomeMoveFunction(int nMotorID);

	//	void WidthMotorFunction();

	int InitXMotorFunction();
	int InitYTopMotorFunction();
	int InitZMotorFunction();
	int InitZMotorFunctionTHR();


	int MoveXMotorToStartPosition();
	int MoveXMotorToStartPositionEncap();
	int MoveYMotorToStartPosition();
	int MoveYMotorToStartPositionEncap();
	int MoveZMotorToStartPosition();

	int MoveXMotorToStartPositionTHR();
	int MoveYMotorToStartPositionTHR();
	int MoveZMotorToStartPositionTHR();

	int ScanXMotorFunctionTHR(int nPos);
	void ScanYTopMotorFunctionTHR();

	void BuzzerOn(int nBeepNo);

	void enableMachineResetOperation(bool enable);
	bool getEnableMachineResetOperationStatus() const;


	int saveEndLotInformation();
	int getSaveImageLimitPercentage() const;
	BOOL getEnableSaveImageLimitStatus() const;
	BOOL _enableSaveImageLimit;
	int  _imageSaveLimitPercentage;
	void setSaveImageLimitParameters(BOOL enable, int percentage);
	int saveSequenceOptionParameters() const;
	int loadSequenceOptionParameters();
	bool IsCurtainMuteActivated();
};