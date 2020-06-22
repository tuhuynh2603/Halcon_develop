#include "stdafx.h"
#include "Application.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include "GlobalBackgroundThread.h"
#include "DebugLog.h"
#include <windows.h>
#include <tlhelp32.h>
#include "CameraCode.h"
#include "ImageSelectionDlg.h"
#include "namespace_utilities_function.h"


namespace
{
	bool verifyAndLoadConfigurationWithPartId()
	{
		// 1. Retrieve all configuration name
		std::list< std::string > configurationList = getConfigurationFileNameList(theApp.m_strMainCfgDir, theApp.m_strMainCfgModeDir);

		// 2. loop through all configuration and check for the package id
		for (std::list< std::string >::iterator configurationIterator = configurationList.begin(); configurationIterator != configurationList.end(); ++configurationIterator) {
			//During online, only configuration inside production will be used
			std::string fileLocation = theApp.m_strMainCfgDir + "\\" + theApp.m_strMainCfgModeDir + "\\" + configurationIterator->c_str() + "\\Package ID.txt";

			// parse file information
			std::list < std::string > packageIdContainer = readWordsFromFile(fileLocation);
			CString strTemp = theApp.m_pImageSelectionDlg->lotInfo().panel.back().id.c_str();
			strTemp = strTemp.Left(4);
			// check if the package id exist on current configuration
			std::list < std::string >::iterator packageIdIterator = std::find_if(packageIdContainer.begin(), packageIdContainer.end(),
				[&](const std::string & str) {
				return str == strTemp.GetString();
			});

			if (packageIdIterator != packageIdContainer.end() && !packageIdContainer.empty()) {
				// No need to load the configuration, if the found configuration is the current configuration
				if (strcmp(configurationIterator->c_str(), theApp.m_strConfigFile) != 0) {

					theApp.m_strConfigFile = configurationIterator->c_str();

					theApp.LoadRecipes();
					theApp.SaveRecipeInfo();

					CString strMsg;
					strMsg.Format("%s Mode %s Recipe Loaded for Part ID:%s", theApp.m_strMainCfgModeDir, theApp.m_strConfigFile, theApp.m_pImageSelectionDlg->lotInfo().panel.back().id.c_str());
					theApp.AddLogEvent(strMsg);

				//	theApp.LoadConfigFile(theApp.m_strMainCfgModeDir, configurationIterator->c_str(), false);
				}

				return true;
			}
		}

		return false;
	}
}

CGlobalBackgroundThread::CGlobalBackgroundThread() : motionParameters(theApp.motionParameters), ioControl(theApp.ioControl)
{

	hardwareReady = FALSE;
	sequenceRunning = FALSE;
	moduleReplacement = FALSE;
	bSequenceStop = FALSE;
	bSequenceError = FALSE;
	bDryRunMode = FALSE;

//	waitInspection = false;
//	failedQrBarcodeSequence = false;

	_moduleRunMode = false;
	_transportRunning = false;

	enableMachineResetOperation(true);

	bEnableStepMode = FALSE;
	EventNextStep = CreateEventA(NULL, TRUE, FALSE, NULL);
	EventRetry = CreateEventA(NULL, TRUE, FALSE, NULL);
//	bLevelError = FALSE;
	
	SequenceLevelDestroyed = CreateEventA(NULL, TRUE, TRUE, NULL);

//	_transportRunning = false;

	XHomeDone = CreateEventA(NULL, TRUE, FALSE, NULL);
	ZHomeDone = CreateEventA(NULL, TRUE, FALSE, NULL);

	for(int i = 0; i < 4; i++) {
		nHomeError[i] = 0;
		InitMotion[i] = CreateEventA(NULL, TRUE, FALSE, NULL);
		
	}
	
	StartMotion[0] = CreateEventA(NULL, TRUE, FALSE, NULL);
	StartMotion[1] = CreateEventA(NULL, TRUE, FALSE, NULL);

	StartMotionTHR[0] = CreateEventA(NULL, TRUE, FALSE, NULL);
	StartMotionTHR[1] = CreateEventA(NULL, TRUE, FALSE, NULL);

	
	for(int i = 0; i<4; i++) {
		nInitMotionError[i] = 0;
	}
	for (int i = 0; i < 3; i++) {
		nStartMotionError[i] = 0;
		nStartMotionErrorTHR[i] = 0;
	}

	nCardNo = 0;
	nPortNo = 1;

	killGlobalThreads = FALSE;

	bAirPressureError = FALSE;
	bResetModeActivated = FALSE;
	bEmergencyStopActivated = FALSE;

	killThreads = FALSE;


	InterlockedExchange(&nGlobalThreadID, -1);
	InterlockedExchange(&nDestroyedGlobalThreadNo, 0);

	InterlockedExchange(&runningModuleCount, -1);
	InterlockedExchange(&destroyedModuleCount, 0);
	InterlockedExchange(&temporaryThreadCount, 0);


	InitializeCriticalSection(&CriticalSectionStepMode);
	InitializeCriticalSection(&CriticalSectionSequence);
	InitializeCriticalSection(&CriticalSectionUserCommand);
	InitializeCriticalSection(&CriticalSectionErrorHandling);

	pEventGlobalThreadClosed = NULL;
	bCycleStopPressed = FALSE;

	MoveYAxisScanTHR = CreateEventA(NULL, TRUE, FALSE, NULL);
	MoveYAxisScanDoneTHR = CreateEventA(NULL, TRUE, FALSE, NULL);
	bSequenceGrab = FALSE;
	bImmediateStopActivated = FALSE;
	bDryRun = FALSE;
	bWetRun = FALSE;
	bCloseSequence = FALSE;
	bReScanSequence = FALSE;
	_enableSaveImageLimit = FALSE;
	_imageSaveLimitPercentage;
	bCurtainMuteActivated = FALSE;
	loadSequenceOptionParameters();
}

CGlobalBackgroundThread::~CGlobalBackgroundThread()
{

	OutputDebugLogTo(7, TRUE, "in exit instance 4.0");

	RemoveSequence();

	OutputDebugLogTo(7, TRUE, "in exit instance 4.7");

	RemoveGlobalEvents();

	DeleteCriticalSection(&CriticalSectionSequence);

	DeleteCriticalSection(&CriticalSectionUserCommand);

	DeleteCriticalSection(&CriticalSectionStepMode);

	DeleteCriticalSection(&CriticalSectionErrorHandling);
	OutputDebugLogTo(7, TRUE, "in exit instance 4.8.1");

	// Close Event Handle
	CloseHandle(SequenceLevelDestroyed);
	CloseHandle(EventNextStep);
	CloseHandle(EventRetry);
	OutputDebugLogTo(7, TRUE, "in exit instance 4.8.2");

	for(int i = 0; i < 4; i++)
		CloseHandle(InitMotion[i]);
		
	OutputDebugLogTo(7, TRUE, "in exit instance 4.8.3");

	CloseHandle(StartMotion[0]);
	CloseHandle(StartMotion[1]);
	CloseHandle(StartMotionTHR[0]);
	CloseHandle(StartMotionTHR[1]);

	CloseHandle(MoveYAxisScanTHR);
	CloseHandle(MoveYAxisScanDoneTHR);
	CloseHandle(XHomeDone);
	CloseHandle(ZHomeDone);
	OutputDebugLogTo(7, TRUE, "in exit instance 4.8.4");


}
void CGlobalBackgroundThread::RemoveGlobalEvents()
{
	try {
		EnterCriticalSection(&CriticalSectionSequence);

		killGlobalThreads = TRUE;
		enableMachineResetOperation(true);

		const long nThreads = InterlockedExchangeAdd(&nGlobalThreadID, 0) + 1;

		if (nThreads > 0 && pEventGlobalThreadClosed != NULL) {

			OutputDebugLogTo(7, TRUE, "waiting for event close");
			::WaitForMultipleObjects(nThreads, pEventGlobalThreadClosed, TRUE, 40000);
			OutputDebugLogTo(7, TRUE, "waiting for event close done");

		}
		OutputDebugLogTo(7, TRUE, "in exit instance 4.8");

		if (pEventGlobalThreadClosed != NULL) {
			delete[] pEventGlobalThreadClosed;
			pEventGlobalThreadClosed = NULL;
		}
		OutputDebugLogTo(7, TRUE, "in exit instance 4.8.0");

		nGlobalEventNo = 0;

		const long nKilledThreadNo = InterlockedExchangeAdd(&nDestroyedGlobalThreadNo, 0);

		OutputDebugLogTo(7, TRUE, "in exit instance 4.8.0.0");

		killGlobalThreads = FALSE;

		LeaveCriticalSection(&CriticalSectionSequence);

		if (nKilledThreadNo > 0)
			OutputDebugLogTo(7, TRUE, "------- DESTROYED GLOBAL THREADS: %d------", nKilledThreadNo);
	}
	catch (std::exception & ex)
	{
		OutputDebugLogTo(0, FALSE, "%s exception occured in global destructor", ex.what());
	}
	catch (...)
	{
		OutputDebugLogTo(0, FALSE, "Unknown exception in global destructor");
	}
	return;
}

int CGlobalBackgroundThread::CreateBackgroundEvents(int nEventsNo)
{
	{
		if (nEventsNo < 1)
			return -1;

		RemoveGlobalEvents();

		//-- Do not run background threads if here is no motion card
		if (!theApp.m_MotorResource.isDllOpened())
			return 0;

		EnterCriticalSection(&CriticalSectionSequence);

		InterlockedExchange(&nGlobalThreadID, -1);

		nGlobalEventNo = nEventsNo;

		pEventGlobalThreadClosed = new HANDLE[nGlobalEventNo];
		for (int i = 0; i<nGlobalEventNo; i++)
			pEventGlobalThreadClosed[i] = CreateEventA(NULL, TRUE, FALSE, NULL);

		for (int i = 0; i<nGlobalEventNo; i++) {

			CWinThread * pWinThread = ::AfxBeginThread(GlobalBackgroundThread, this, THREAD_PRIORITY_NORMAL);

			if (pWinThread == NULL) {
				LeaveCriticalSection(&CriticalSectionSequence);
				OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING BACKGROUND EVENTS------");
				RemoveGlobalEvents();
				return -1;
			}
		}

		OutputDebugLogTo(7, TRUE, "------- RUNNING GLOBAL THREADS : %d------", nGlobalEventNo);

		LeaveCriticalSection(&CriticalSectionSequence);

		return 0;
	}
}

UINT CGlobalBackgroundThread::GlobalBackgroundThread(LPVOID pParam)
{
	CGlobalBackgroundThread * pGlobalBackgroundThread = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	const int threadId = InterlockedIncrement(&(pGlobalBackgroundThread->nGlobalThreadID));

	if (threadId >= pGlobalBackgroundThread->nGlobalEventNo) {
		OutputDebugLogTo(7, TRUE, "Machine sequence >>> Global background threads count is wrong");
		return 1;
	}

	pGlobalBackgroundThread->GlobalBackgroundThreadFunction(threadId);

	OutputDebugLogTo(7, TRUE, "Thread Id %d Closed", threadId);

	//To check the number of destroyed threads
	InterlockedIncrement(&(pGlobalBackgroundThread->nDestroyedGlobalThreadNo));

	SetEvent(pGlobalBackgroundThread->pEventGlobalThreadClosed[threadId]);

	return 0;
}

void CGlobalBackgroundThread::GlobalBackgroundThreadFunction(const int nEventID)
{
	try{

		//Button light displaying
		if (nEventID == 0) {
			BOOL bCycleStartStatus = FALSE;
			BOOL bCycleStopStatus = FALSE;
			BOOL bResetButtonStatus = FALSE;
			BOOL bImmidiateStopStatus = FALSE;
			
			int nStartButtonRepeatNo = 0;
			int nStopButtonRepeatNo = 0;
			int nCycleStopButtonRepeatNo = 0;
			int nResetButtonRepeatNo = 0;
			int nActionRequiredRepeatNo = 0;
			int nAirPressureRepeatNo = 0;

			BOOL bCurtainBypassLights = FALSE;
			BOOL bStartLight = FALSE;
			BOOL bStopLight = FALSE;
			BOOL bCycleStopLight = FALSE;
			BOOL bResetLight = FALSE;

			BOOL bActionLight = FALSE;
			BOOL bActionTriggered = FALSE;
			BOOL GuardHealthy = TRUE;
			BOOL bPrevbDoorSwitch = TRUE;

			UINT nIOStatus = 0;

			while (1) {
				if (killGlobalThreads) return;

				int nError = 0; 
				
				nError = ioControl.GetStartButtonStatus(bCycleStartStatus);
				nError = ioControl.GetCycleStopButtonStatus(bCycleStopStatus);
				nError = ioControl.GetStopButtonStatus(bImmidiateStopStatus);
				nError = ioControl.GetResetButtonStatus(bResetButtonStatus); 
				nError = ioControl.GetGuardHealthyStatus(GuardHealthy);

				if (nError) {
					OutputDebugLogTo(7, TRUE, "----- ERROR in I/O sensors");
					theApp.AddLogEvent("I/O input signal can't be readable. Check I/O status");
					//	setGemAlarm("SENSOR-STATUS-ERROR");
					Sleep(500);
					continue;
				}

				if (killGlobalThreads) return;

				//Start button light
				if (bCycleStartStatus) {
					bStartLight = TRUE;
					START_Light(bStartLight);
				}
				else {
					if (hardwareReady) {
						if (IsRunning()) {
							if (bSequenceStop) {
								nStartButtonRepeatNo++;
								if (nStartButtonRepeatNo > 5) {
									nStartButtonRepeatNo = 0;
									bStartLight = !bStartLight;
									START_Light(bStartLight);
								}
							}
							else if (IsModuleReplacement()) {
								nStartButtonRepeatNo++;
								if (nStartButtonRepeatNo > 3) {
									nStartButtonRepeatNo = 0;
									bStartLight = !bStartLight;
									START_Light(bStartLight);
								}
							}
							else {
								nStartButtonRepeatNo = 0;
								bStartLight = TRUE;
								START_Light(bStartLight);
							}
						}
						else {
							if (bAirPressureError) {
								nAirPressureRepeatNo++;
								if (nAirPressureRepeatNo > 5) {
									nAirPressureRepeatNo = 0;
									bActionLight = !bActionLight;
									//	FAULT_Light(bActionLight);
								}
							}
							else {
								nStartButtonRepeatNo++;
								if (nStartButtonRepeatNo > 5) {
									nStartButtonRepeatNo = 0;
									bStartLight = !bStartLight;
									START_Light(bStartLight);

								}
							}
						}
					}
					else {
						if (bStartLight) {
							bStartLight = FALSE;
							START_Light(bStartLight);
						}
					}
				}

				if (killGlobalThreads || theApp.m_bExitApp) return;

				//Cycle stop light
				if (bCycleStopStatus) {
					bCycleStopLight = TRUE;
					CYCLE_STOP_Light(bCycleStopLight);
				}
				else {

					if (hardwareReady) {
						if (IsRunning() && bCycleStopPressed) {
							nCycleStopButtonRepeatNo++;
							if (nCycleStopButtonRepeatNo > 5) {
								nCycleStopButtonRepeatNo = 0;
								bCycleStopLight = !bCycleStopLight;
								CYCLE_STOP_Light(bCycleStopLight);
							}
						}
						else {
							if (bCycleStopLight) {
								bCycleStopLight = FALSE;
								CYCLE_STOP_Light(bCycleStopLight);
							}
						}
					}
					else {
						if (bCycleStopLight) {
							bCycleStopLight = FALSE;
							CYCLE_STOP_Light(bCycleStopLight);
						}
					}
				}

				if (killGlobalThreads || theApp.m_bExitApp) return;

				//Stop button light
				if (bImmidiateStopStatus) {
					bStopLight = TRUE;
					STOP_Light(bStopLight);
				}
				else {
					if (hardwareReady) {
						if (IsRunning()) {
							if (bSequenceError) {
								nStopButtonRepeatNo++;
								if (nStopButtonRepeatNo > 5) {
									nStopButtonRepeatNo = 0;
									bStopLight = !bStopLight;

									STOP_Light(bStopLight);

								}
							}
							else if (bSequenceStop) {
								if (!bStopLight) {
									bStopLight = TRUE;
									STOP_Light(bStopLight);
								}
							}
							else {
								nStopButtonRepeatNo = 0;
								if (bStopLight) {
									bStopLight = FALSE;
									STOP_Light(bStopLight);
								}
							}
						}
						else {
							if (bStopLight) {
								bStopLight = FALSE;
								STOP_Light(bStopLight);
							}
						}
					}
					else {
						if (!bAirPressureError) {
							nStopButtonRepeatNo = 0;
							if (bStopLight) {
								bStopLight = FALSE;
								STOP_Light(bStopLight);

							}
						}
						else {
							nStopButtonRepeatNo++;
							if (nStopButtonRepeatNo > 5) {
								nStopButtonRepeatNo = 0;
								bStopLight = !bStopLight;
								STOP_Light(bStopLight);
							}
						}
					}
				}

				if (killGlobalThreads) return;

				//Reset button light
				if (bResetButtonStatus) {
					bResetLight = TRUE;
					RESET_Light(bResetLight);
				}
				else {
					if (bResetModeActivated || !hardwareReady) {
						if (!bResetLight)
							bResetLight = TRUE;
						RESET_Light(bResetLight);
					}
					else {
						nResetButtonRepeatNo = 0;
						if (bResetLight) {
							bResetLight = FALSE;
							RESET_Light(bResetLight);

						}
					}
				}
				Sleep(100);
			}
		}

		//START, STOP, RESET button interraction and AIR pressure status
		if (nEventID == 1) {
			BOOL bCycleStartStatus = FALSE;
			BOOL bCycleStopStatus = FALSE;
			BOOL bImmidiateStopStatus = FALSE;
			BOOL bResetButtonStatus = FALSE;
			BOOL bLightCurtain = FALSE;
			BOOL bDoorStatus = FALSE;
			
			BOOL bAirPressureStatus = FALSE;

			BOOL bStartButtonPrevStatus = FALSE;
			BOOL bStopButtonPrevStatus = FALSE;
			BOOL bCycleStopButtonPrevStatus = FALSE;
			BOOL bResetButtonPrevStatus = FALSE;

			BOOL bEmergencyStatus = TRUE;

			int nStopButtonRepeatNo = 0;
			int nStartButtonRepeatNo = 0;

			BOOL bAllowAction = TRUE;

			UINT nIOStatus;

			while (1) {

				if (killGlobalThreads)
					return;

				int nError = 0;

				nError = ioControl.GetStartButtonStatus(bCycleStartStatus);
				//nError = ioControl.GetCycleStopButtonStatus(bCycleStopStatus);
				nError = ioControl.GetStopButtonStatus(bImmidiateStopStatus);
				nError = ioControl.GetResetButtonStatus(bResetButtonStatus);

				nError = ioControl.GetGuardHealthyStatus(bDoorStatus);
				nError = ioControl.GetIncomingAirPressureStatus(bAirPressureStatus);
				nError = ioControl.GetEmergencyStatus(bEmergencyStatus);

				if (nError) {
					OutputDebugLogTo(7, TRUE, "----- ERROR in I/O sensors");
					bAirPressureStatus = FALSE;
					theApp.AddLogEvent("I/O input signal can't be readable. Check I/O status");
					Sleep(500);
					continue;
				}

				if (killGlobalThreads)
					return;
			
				//Reset button
				if (bResetButtonStatus && !bResetButtonPrevStatus && getEnableMachineResetOperationStatus()) {

					OutputDebugLogTo(8, TRUE, "RESET SETP1 Reset pressed. Check all conditions");
					if (killGlobalThreads)
						return;

					//1. Check E Stop
					if (!bEmergencyStatus) {
						OutputDebugLogTo(7, TRUE, "-----EMERGENCY STATUS IS OFF. CLOSE SEQUENCE...");
						enableMachineResetOperation(true);
						AfxMessageBox("Emergency Button has been pressed. Release button and press RESET to continue.", MB_OK | MB_ICONERROR);
						continue;
					}

					//2. Check Door Status
					if (!bDoorStatus) {
						OutputDebugLogTo(7, TRUE, "-----DOOR STATUS IS OFF. CLOSE SEQUENCE...");
						AfxMessageBox("Guard healthy is OFF", MB_OK | MB_ICONERROR);
						continue;
					}

					//3. Check Air Input
					if (!bAirPressureStatus) {
						OutputDebugLogTo(7, TRUE, "-----AIR PRESSURE IS OFF. CLOSE SEQUENCE...");
						theApp.AddLogEvent("No Air Pressure");
						Stop();

						BuzzerOn(3);

						AfxMessageBox("No Air Pressure Supply To Machine, Please Turn On Or Connect Air To Reset Machine.", MB_OK | MB_ICONERROR);
						continue;
					}

					/* IF 3 Conditions above is passed --> Turn on Contactors*/
					nError = ioControl.SetK1K2On();
					if (nError) {
						OutputDebugLogTo(7, TRUE, "----- ERROR in Turn ON K1-K2 I/O");
						bAirPressureStatus = FALSE;
						theApp.AddLogEvent("I/O output signal can't be writable. Check I/O");
						Sleep(500);
						continue;
					}
					Sleep(1000);

					nError = ioControl.SetK3K4On();
					if (nError) {
						OutputDebugLogTo(7, TRUE, "----- ERROR in Turn ON K3-K4 I/O");
						bAirPressureStatus = FALSE;
						theApp.AddLogEvent("I/O output signal can't be writable. Check I/O");
						Sleep(500);
						continue;
					}

					
					///*If everything smooth --> Buzzer once and Flash green tower light*/
					//hardwareReady = TRUE;
					//bResetModeActivated = FALSE;
					//enableMachineResetOperation(false);


					//BuzzerOn(1);
					//bResetButtonPrevStatus = bResetButtonStatus;
					////Don't allow to execute other button simultaneously
					//bAllowAction = FALSE;
					

					//RESETING MOTOR...... !bEmergencyStatus && !IsRunning()
					/* ------------------- RESETING MOTOR ------------------- */
					if (bEmergencyStatus && !IsRunning()) {

						CProgressDlg *pProgressDlg;
						pProgressDlg = new CProgressDlg();

						if (!pProgressDlg->Create(IDD_PROGRESS_DLG, AfxGetMainWnd())) {
							delete pProgressDlg;
							pProgressDlg = NULL;
						}
						else {
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
								pProgressDlg->CenterWindow();
								pProgressDlg->UpdateWindow();
								pProgressDlg->SetStatus("Machine Resetting....", 0);
							}
						}

						EnterCriticalSection(&CriticalSectionUserCommand);

						hardwareReady = FALSE;
						bResetModeActivated = TRUE;
						enableMachineResetOperation(false);

						if (pProgressDlg != NULL) {
							pProgressDlg->SetStatus("Machine Resetting....", 1);
							pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
						}

						RemoveSequence();

						if (killGlobalThreads) {
							bResetModeActivated = FALSE;
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							return;
						}

						if (bEmergencyStopActivated) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							bResetButtonPrevStatus = bResetButtonStatus;
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}
						Sleep(1000);

						if (pProgressDlg != NULL) {
							pProgressDlg->SetStatus("Machine Resetting....", 5);
							pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
						}

						if (killGlobalThreads) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							return;
						}

						OutputDebugLogTo(8, TRUE, "RESET Check for . bEmergencyStopActivated status %d", bEmergencyStopActivated);

						if (bEmergencyStopActivated) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							bResetButtonPrevStatus = bResetButtonStatus;
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}
						Sleep(1000);

						if (pProgressDlg != NULL) {
							pProgressDlg->SetStatus("Machine Resetting....", 10);
						}

						if (bEmergencyStopActivated) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							bResetButtonPrevStatus = bResetButtonStatus;
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}

						//Need to wait 5s around for Alarm Status
						Sleep(5000);
						//Check motor status
						BOOL bAlarm = TRUE;

						OutputDebugLogTo(8, TRUE, "RESET get X servo motor alarm status . bEmergencyStopActivated %d", bEmergencyStopActivated);

						motionParameters.GetMotorAlarmStatus(X_SERVO_MOTOR, bAlarm);
						if (bAlarm) {
							OutputDebugLogTo(7, TRUE, "----- X SERVO MOTOR ALARM. PLEASE CHECK MOTOR POWER SUPPLY");
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}
						if (pProgressDlg != NULL) {
							pProgressDlg->SetStatus("Machine Resetting....", 20);
							pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
						}

						OutputDebugLogTo(8, TRUE, "RESET get Y servo motor alarm status . bEmergencyStopActivated %d", bEmergencyStopActivated);

						motionParameters.GetMotorAlarmStatus(Y_SERVO_MOTOR, bAlarm);
						if (bAlarm) {
							OutputDebugLogTo(7, TRUE, "----- Y TOP SERVO MOTOR ALARM. PLEASE CHECK MOTOR POWER SUPPLY");
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}

						OutputDebugLogTo(8, TRUE, "RESET get ST1-Z servo motor alarm status . bEmergencyStopActivated %d", bEmergencyStopActivated);

						motionParameters.GetMotorAlarmStatus(ST1_Z_SERVO_MOTOR, bAlarm);
						if (bAlarm) {
							OutputDebugLogTo(7, TRUE, "----- ST1 Z SERVO MOTOR ALARM. PLEASE CHECK MOTOR POWER SUPPLY");
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}

						OutputDebugLogTo(8, TRUE, "RESET get ST2-Z servo motor alarm status . bEmergencyStopActivated %d", bEmergencyStopActivated);

						motionParameters.GetMotorAlarmStatus(ST2_Z_SERVO_MOTOR, bAlarm);
						if (bAlarm) {
							OutputDebugLogTo(7, TRUE, "----- ST2 Z SERVO MOTOR ALARM. PLEASE CHECK MOTOR POWER SUPPLY");
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}


						OutputDebugLogTo(8, TRUE, "RESET Check for . bEmergencyStopActivated status %d", bEmergencyStopActivated);

						if (bEmergencyStopActivated) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							bResetButtonPrevStatus = bResetButtonStatus;
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}

						//Wait for 1 second
						for (int i = 0; i < 10; i++) {
							Sleep(100);

							if (killGlobalThreads) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								return;
							}
							if (bEmergencyStopActivated)
								break;

							if (pProgressDlg != NULL) {
								pProgressDlg->SetStatus("Machine Resetting....", 20 + i / 5);
								pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
							}
						}

						if (bEmergencyStopActivated) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							bResetButtonPrevStatus = bResetButtonStatus;
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//			theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}

						OutputDebugLogTo(8, TRUE, "RESET servo on all axis start");

						nError = motionParameters.SetServoOnAllAxis();
						if (nError) {
							OutputDebugLogTo(7, TRUE, "----- ERROR IN SERVO ON EVERY AXIS");
							bResetModeActivated = FALSE;
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}

						OutputDebugLogTo(8, TRUE, "RESET servo on all axis done");

						if (killGlobalThreads) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							return;
						}
						if (bEmergencyStopActivated) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							bResetButtonPrevStatus = bResetButtonStatus;
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							continue;
						}

						if (pProgressDlg != NULL) {
							pProgressDlg->SetStatus("Machine Resetting....", 30);
							pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
						}


						if (!motionParameters.IsMotorHomed(X_SERVO_MOTOR)) {

							nError = motionParameters.HomeMove(X_SERVO_MOTOR);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for X motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}
						}

						if (pProgressDlg != NULL) {
							pProgressDlg->SetStatus("Machine Resetting....", 35);
							pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
						}

						if (!motionParameters.IsMotorHomed(Y_SERVO_MOTOR)) {

							nError = motionParameters.HomeMove(Y_SERVO_MOTOR);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Y motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}
						}

						if (pProgressDlg != NULL) {
							pProgressDlg->SetStatus("Machine Resetting....", 45);
							pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
						}

						if (killGlobalThreads) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							return;
						}

						if (!motionParameters.IsMotorHomed(ST1_Z_SERVO_MOTOR)) {

							nError = motionParameters.HomeMove(ST1_Z_SERVO_MOTOR);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for ST1 Z motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}
						}

						if (pProgressDlg != NULL) {
							pProgressDlg->SetStatus("Machine Resetting....", 55);
							pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
						}

						if (!motionParameters.IsMotorHomed(ST2_Z_SERVO_MOTOR)) {

							nError = motionParameters.HomeMove(ST2_Z_SERVO_MOTOR);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for ST2 Z motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}
						}

						if (pProgressDlg != NULL) {
							pProgressDlg->SetStatus("Machine Resetting....", 65);
							pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
						}

						if (killGlobalThreads) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							return;
						}

						if (!motionParameters.IsMotorHomed(X_SERVO_MOTOR)) {

							nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, INFINITE);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for X motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}

							if (theApp.m_bExitApp) break;

							if (pProgressDlg != NULL) {
								pProgressDlg->SetStatus("Machine Resetting....", 70);
								pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
							}

							nError = motionParameters.HomeDone(X_SERVO_MOTOR);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for X motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}

						}

						if (killGlobalThreads) {
							bResetModeActivated = FALSE;
							enableMachineResetOperation(true);
							LeaveCriticalSection(&CriticalSectionUserCommand);
							if (pProgressDlg != NULL) {
								pProgressDlg->ShowWindow(FALSE);
								//		theApp.m_bProgressDlgOpen = FALSE;
								pProgressDlg->DestroyWindow();
								delete pProgressDlg;
								pProgressDlg = NULL;
							}
							return;
						}

						if (!motionParameters.IsMotorHomed(Y_SERVO_MOTOR)) {

							nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, INFINITE);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Y motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}

							if (theApp.m_bExitApp) break;

							if (pProgressDlg != NULL) {
								pProgressDlg->SetStatus("Machine Resetting....", 80);
								pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
							}

							nError = motionParameters.HomeDone(Y_SERVO_MOTOR);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Y motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}

						}

						if (!motionParameters.IsMotorHomed(ST1_Z_SERVO_MOTOR)) {

							nError = motionParameters.WaitForMotionDone(ST1_Z_SERVO_MOTOR, INFINITE);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for ST1 Z motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}

							if (theApp.m_bExitApp) break;

							if (pProgressDlg != NULL) {
								pProgressDlg->SetStatus("Machine Resetting....", 90);
								pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
							}

							nError = motionParameters.HomeDone(ST1_Z_SERVO_MOTOR);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for ST1 Z motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}

						}

						if (!motionParameters.IsMotorHomed(ST2_Z_SERVO_MOTOR)) {

							nError = motionParameters.WaitForMotionDone(ST2_Z_SERVO_MOTOR, INFINITE);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for ST2 Z motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}

							if (theApp.m_bExitApp) break;

							if (pProgressDlg != NULL) {
								pProgressDlg->SetStatus("Machine Resetting....", 95);
								pProgressDlg->ShowWindow(TRUE && !theApp.m_bProgressDlgOpen);
							}

							nError = motionParameters.HomeDone(ST2_Z_SERVO_MOTOR);
							if (nError) {
								bResetModeActivated = FALSE;
								enableMachineResetOperation(true);
								LeaveCriticalSection(&CriticalSectionUserCommand);
								OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for ST2 Z motor failed");
								if (pProgressDlg != NULL) {
									pProgressDlg->ShowWindow(FALSE);
									//		theApp.m_bProgressDlgOpen = FALSE;
									pProgressDlg->DestroyWindow();
									delete pProgressDlg;
									pProgressDlg = NULL;
								}
								continue;
							}

						}

						OutputDebugLogTo(7, TRUE, "----- Machine RESET Completed");
						theApp.AddLogEvent("Machine RESET Completed");

						if (pProgressDlg != NULL) {
							pProgressDlg->SetStatus("Machine RESET Completed...", 100);
							//		theApp.m_bProgressDlgOpen = FALSE;
							pProgressDlg->ShowWindow(FALSE);
							pProgressDlg->DestroyWindow();
							delete pProgressDlg;
							pProgressDlg = NULL;
						}

						hardwareReady = TRUE;
						bResetModeActivated = FALSE;
						enableMachineResetOperation(false);

						BuzzerOn(1);
						bResetButtonPrevStatus = bResetButtonStatus;
						//Don't allow to execute other button simultaneously
						bAllowAction = FALSE;

						LeaveCriticalSection(&CriticalSectionUserCommand);
					}
					else if (IsRunning()) {
						OutputDebugLogTo(7, TRUE, "-----MACHINE SEQUENCE IS RUNNING...");
						theApp.AddLogEvent("Machine Sequence Is Running!");
					}
				}
				else {
					bResetButtonPrevStatus = bResetButtonStatus;
					bAllowAction = TRUE;
				}

				if (killGlobalThreads) return;

				if (hardwareReady) {

					//Stop button
					if (bAllowAction) {

						if (!bStopButtonPrevStatus && bImmidiateStopStatus && IsRunning()) {
							theApp.AddLogEvent("PAUSE Button Pressed");


							if (!bSequenceGrab)
								motionParameters.StopEveryAxis();

							bImmediateStopActivated = TRUE;

							Stop();
							theApp.m_bScanning = FALSE;
							theApp.AddLogEvent("Machine Sequence Paused");
							nStopButtonRepeatNo = 0;

							//Don't allow to execute other button simultaneously
							bAllowAction = FALSE;
						}
						else {

							if (bStopButtonPrevStatus && bImmidiateStopStatus && IsRunning()) {
								nStopButtonRepeatNo++;
								if (nStopButtonRepeatNo > 25) {

									OutputDebugLogTo(7, TRUE, "Long Stop Pressed in GlobalBackgroundThread");

									if (AfxMessageBox("Click \'Yes\' to End Lot and Click \'No\' to Continue Sequence?", MB_YESNO | MB_ICONQUESTION) == IDYES) {
										nStopButtonRepeatNo = 0;
										theApp.AddLogEvent("STOP Button Pressed");

										//									theApp.pSubstrateInspectionResultsWnd->updateInspectionResults();

										saveEndLotInformation();
										RemoveSequence();

										theApp.AddLogEvent("Machine Sequence Stopped");
										bAllowAction = FALSE;

										if (theApp.m_pMsgBox != NULL)
											theApp.m_pMsgBox->Msg("Machine Sequence Stopped", 3500);

									}
									else {

										if (!IsModuleReplacement())
											nError = Run();
										else
											bImmediateStopActivated = FALSE;

										theApp.AddLogEvent("Machine Sequence Started");

										theApp.m_bScanning = TRUE;

										if (nError) {
											OutputDebugLogTo(7, TRUE, "----- ERROR IN STARTING SEQUENCE");
											RemoveSequence();
											continue;
										}

										nStopButtonRepeatNo = 0;
									}
								}
							}
							else {
								nStopButtonRepeatNo = 0;
							}
						}

						bStopButtonPrevStatus = bImmidiateStopStatus;
					}
					if (killGlobalThreads) return;

					//Start button
					if (bAllowAction) {
						if (!bStartButtonPrevStatus && bCycleStartStatus) {

							/*BOOL bDoorStatus;
							int nError = theApp.ioControl.GetGuardDoorHealthyStatus(bDoorStatus);
							if (nError)
								OutputDebugLogTo(7, TRUE, "Failed to Read Status of Guard Door Healthy");

							if (bDoorStatus) {
								AfxMessageBox("Guard Door is Open, Please close the Door and Press Start to Continue Sequence");
								continue;
							}
							else
								ioControl.GuardDoorLock(true);*/

							theApp.AddLogEvent("START Button Pressed");
				
							nError = Run();

							theApp.AddLogEvent("Machine Sequence Started");
						}
						bStartButtonPrevStatus = bCycleStartStatus;
					}

					if (killGlobalThreads) return;
					//Cycle stop button

					if (bAllowAction) {

						if (!bCycleStopButtonPrevStatus && bCycleStopStatus) {
							theApp.AddLogEvent("CYCLE STOP Button Pressed");
							bCycleStopPressed = TRUE;
							bCycleStopButtonPrevStatus = bCycleStopStatus;

						}
						bCycleStopButtonPrevStatus = bCycleStopStatus;
					}

				}

				Sleep(75);
			}
		}

		//EMERGENCY status
		if (nEventID == 2) {

			BOOL bEmergencyStatus = FALSE;
			BOOL bPreviousEmergencyStatus = FALSE;

			BOOL bDoorStatus = FALSE;
			BOOL bPreviousDoorStatus = FALSE;

			BOOL bPusherJamStatus = TRUE;
			BOOL bPreviousPusherJamStatus = FALSE;

			BOOL bWidthAdjustmentCrashStatus = TRUE;
			BOOL bPreviousWidthAjustmentCrashStatus = FALSE;

			BOOL bKickerJamStatus = TRUE;
			BOOL bPreviousKickerJamStatus = FALSE;

			UINT nIOStatus = 0;

			while (1) {

				if (killGlobalThreads)
					return;

				int nError = ioControl.GetInputStatus(nIOStatus);
				if (nError) {
					OutputDebugLogTo(7, TRUE, "----- ERROR in I/O sensors");
					theApp.AddLogEvent("I/O input signal can't be readable. Check I/O status");
					//setGemAlarm("SENSOR-STATUS-ERROR");
					Sleep(1000);
					continue;
				}

				bEmergencyStatus = (nIOStatus >> 10) & 1;
				bDoorStatus = (nIOStatus >> 7) & 1;

				if (killGlobalThreads)
					return;

				/*if (bDoorOpenOnError && !bDoorStatus && bPreviousDoorStatus) {
					OutputDebugLogTo(7, TRUE, "-----1 DOOR HAS BEEN OPENED!");
					PostError("Machine Door Opened");
					bIsDoorClosed = bPreviousDoorStatus = bDoorStatus;
				}*/

				
				if (((!bEmergencyStatus && bPreviousEmergencyStatus) || (!bDoorStatus && bPreviousDoorStatus))) {

					bEmergencyStopActivated = TRUE;
					hardwareReady = FALSE;
					enableMachineResetOperation(true);

					//Set all output signal to off
					theApp.ioControl.ResetIO();

					nError = motionParameters.EmergencyStopEveryAxis();
					if (nError) {
						OutputDebugLogTo(7, TRUE, "----- ERROR IN EMERGENCY STOP EVERY AXIS");
						//setGemAlarm("EMERGENCY-STOP-EVERY-AXIS-ERROR");
						continue;
					}

					if (killGlobalThreads)
						return;

					EnterCriticalSection(&CriticalSectionUserCommand);

					hardwareReady = FALSE;
					bEmergencyStopActivated = FALSE;

					RemoveSequence();

					nError = motionParameters.SetServoOffAllAxis();
					if (nError) {
						OutputDebugLogTo(7, TRUE, "----- ERROR IN SERVO OFF EVERY AXIS");
						//setGemAlarm("SERVO-OFF-ALL-AXIS-ERROR");
						LeaveCriticalSection(&CriticalSectionUserCommand);
						continue;
					}

					//theApp.getGEMController().setStatusVariablesValue("ProcessState", getValueInString(IDLE));		//Girish Krishna
					OutputDebugLogTo(9, TRUE, "Set Machine ProcessState to IDLE2");
					//				theApp.sendGemEvent("ProcessingStateChanged");

					LeaveCriticalSection(&CriticalSectionUserCommand);

					if (killGlobalThreads)
						return;

					if (!bDoorStatus) {
						OutputDebugLogTo(7, TRUE, "-----2 DOOR HAS BEEN OPENED!");
						theApp.AddLogEvent("Machine Door Opened");

						BuzzerOn(3);

						//setGemAlarm("DOOR-OPEN");

						//int error = ioControl.SetMaintenanceLightOn();
						//if(error) {
						//	setGemAlarm("MAINTENANCE-LIGHT-ON-ERROR");
						//	OutputDebugLogTo(7,TRUE,"----- Maintenance light failed to turn on, please check wiring connection");
						//}

						AfxMessageBox("Door has been Opened. Close door and press RESET to continue.", MB_OK | MB_ICONERROR);
					}

					if (!bEmergencyStatus) {
						OutputDebugLogTo(7, TRUE, "----- EMERGENCY BUTTON HAS BEEN PRESSED!");
						theApp.AddLogEvent("Emergency Stop Button Pressed");
						enableMachineResetOperation(true);
						BuzzerOn(3);

					/*	if (theApp.m_bDefaultTowerLightConfig)
							BuzzerOn(3);
						else if (theApp.UserTowerLightOptions[0].bEnableBuzzer)
							BuzzerOn(3);*/

						//setGemAlarm("EMERGENCY-STOP");
						AfxMessageBox("Emergency Button has been pressed. Release button and press RESET to continue.", MB_OK | MB_ICONERROR);
					}

					bPreviousEmergencyStatus = bEmergencyStatus;
					bPreviousDoorStatus = bDoorStatus;
				}
				else {

					bPreviousEmergencyStatus = bEmergencyStatus;
					bPreviousDoorStatus = bDoorStatus;
					//bIsDoorClosed = bDoorStatus;

					// Start Check for Pusher Jam
					nError = ioControl.GetPushJamSensorState(bPusherJamStatus);
					if (nError) {
						OutputDebugLogTo(7, TRUE, "----- ERROR IN PUSHER JAM STATUS");
						//setGemAlarm("PUSHER-JAM-SENSOR-ERROR");
						Sleep(1000);
						continue;
					}

					/*if (bPusherJamStatus && !bPreviousPusherJamStatus && !bResetModeActivated) {

						nError = motionParameters.StopAxis(PUSH_PULL_LINEAR_MOTOR);
						if (nError) {
							OutputDebugLogTo(0, TRUE, "----- ERROR : Cannot stop push-pull motor");
							setGemAlarm("TRANSPORT-PUSHPULL-STOP-ERROR");
							continue;
						}

						if (killGlobalThreads)
							return;

						OutputDebugLogTo(0, TRUE, "----- ERROR : PUSHER JAM");
						PostError("Pusher Jam");
						setGemAlarm("TRANSPORT-PUSHJAM-ERROR");
						if (theApp.m_bDefaultTowerLightConfig)
							BuzzerOn(3);

						AfxMessageBox("Pusher is Jammed. Push-pull motor has been stopped.", MB_OK | MB_ICONERROR);
					}*/
					// End Pusher Jam Check

					// Start Check for Kicker Jam
					/*nError = ioControl.GetKickerJamSensorState(bKickerJamStatus);
					if (nError) {
						OutputDebugLogTo(7, TRUE, "----- ERROR IN KICKER JAM STATUS");
						setGemAlarm("KICKER-JAM-SENSOR-ERROR");
						Sleep(1000);
						continue;
					}

					if (bKickerJamStatus && !bPreviousKickerJamStatus && !bResetModeActivated) {

						if (killGlobalThreads)
							return;

						OutputDebugLogTo(7, TRUE, "----- ERROR : KICKER JAM");
						PostError("Kicker Jam");
						setGemAlarm("KICKER-JAM-SENSOR-ERROR");
						if (theApp.m_bDefaultTowerLightConfig)
							BuzzerOn(3);

						AfxMessageBox("Kicker is Jammed. Please clear Jam manually.", MB_OK | MB_ICONERROR);
					}*/
					// End Kicker Jam Check

					/*nError = ioControl.GetWidthAdjustmentCrashStatus(bWidthAdjustmentCrashStatus);
					if (nError) {
						OutputDebugLogTo(7, TRUE, "----- ERROR IN WIDTH ADJUSTMENT CRASH STATUS");
						setGemAlarm("TRANSPORT-WIDTH-CRASH-SENSOR-ERROR");
						Sleep(1000);
						continue;
					}

					if (bWidthAdjustmentCrashStatus && !bPreviousWidthAjustmentCrashStatus && !bResetModeActivated) {

						nError = motionParameters.StopAxis(WIDTH_ADJUSTMENT_STEPPER_MOTOR);
						if (nError) {
							OutputDebugLogTo(0, TRUE, "----- ERROR : Cannot stop width motor");
							setGemAlarm("TRANSPORT-WIDTH-STOP-ERROR");
							continue;
						}

						if (killGlobalThreads)
							return;

						OutputDebugLogTo(0, TRUE, "----- ERROR : WIDTH ADJUSTMENT MOTOR CRASH");
						PostError("Width Adjustment Motor Crashed");
						setGemAlarm("TRANSPORT-WIDTH-CRASH-ERROR");
						if (theApp.m_bDefaultTowerLightConfig)
							BuzzerOn(3);
						AfxMessageBox("Width Motor Crashed Sensor ON. Width Adjustment Motor Has Been Stopped. Width Motor will be homing now", MB_OK | MB_ICONERROR);

						int nError = motionParameters.HomeMove(WIDTH_ADJUSTMENT_STEPPER_MOTOR, false);
						if (nError) {
							motionParameters.StopAxis(WIDTH_ADJUSTMENT_STEPPER_MOTOR);
							setGemAlarm("TRANSPORT-WIDTH-HOME-ERROR");
							OutputDebugLogTo(0, TRUE, "----- ERROR : Width motor home move failed, please check move condition.");
							continue;
						}

						nError = motionParameters.WaitForMotionDone(WIDTH_ADJUSTMENT_STEPPER_MOTOR, theApp.getMotionTimeOut());
						if (nError) {
							motionParameters.StopAxis(WIDTH_ADJUSTMENT_STEPPER_MOTOR);
							setGemAlarm("TRANSPORT-WIDTH-HOME-ERROR");
							OutputDebugLogTo(0, TRUE, "----- ERROR : Width motor home move failed, please check move condition.");
							continue;
						}

						nError = motionParameters.HomeDone(WIDTH_ADJUSTMENT_STEPPER_MOTOR);
						if (nError) {
							motionParameters.StopAxis(WIDTH_ADJUSTMENT_STEPPER_MOTOR);
							setGemAlarm("TRANSPORT-WIDTH-HOME-ERROR");
							OutputDebugLogTo(0, TRUE, "----- ERROR : Width motor home move failed, please check move condition.");
							continue;
						}
					}*/
				}

				bPreviousPusherJamStatus = bPusherJamStatus;
				bPreviousWidthAjustmentCrashStatus = bWidthAdjustmentCrashStatus;
				bPreviousKickerJamStatus = bKickerJamStatus;

				Sleep(50);
			}
		}

	
		//AXIS ALARM AND OUTPUT Reset
		if (nEventID == 3) {

			const int motorCount = motionParameters.GetTotalMotorCount();

			if (motorCount > 0) {

				std::vector < int > motorId(motorCount);

				for (int i = 0; i < motorCount; i++)
					motorId[i] = i;

				while (1) {
					if (killGlobalThreads)
						return;

					if (hardwareReady) {

						int nError = motionParameters.WaitForAnyMotionError(motorId, 10000);

						if (killGlobalThreads)
							return;

						if (hardwareReady) {
							if (nError != 0 && nError != 1) {

								hardwareReady = FALSE;

								//Check motor Alarm status
								BOOL bStatus = TRUE;

								CString strMotorName;

								for (int i = 0; i < motorCount; i++) {
									motionParameters.GetMotorAlarmStatus(motorId[i], bStatus);
									if (bStatus) {
										motionParameters.GetMotorName(motorId[i], strMotorName);
										OutputDebugLogTo(7, TRUE, "----- %s Alarm. Please check motor power supply", strMotorName);
									}
								}
								motionParameters.EmergencyStopEveryAxis();

								_enableMachineResetOperation = true;

								OutputDebugLogTo(7, TRUE, "ALARM IN MOTORS. SEQUENCE STOPPED!");
								RemoveSequence();

							}
							else {
								continue;
							}
						}
						else {
							continue;
						}
					}

					Sleep(50);
				}

			}
		}

	}
	catch (std::exception & ex){
		OutputDebugLogTo(0, FALSE, "%s exception occured in Global background thread Id %d", ex.what(), nEventID);
	}
	catch (...){
		OutputDebugLogTo(0, FALSE, "Unknown exception in Global background thread Id %d", nEventID);
	}
	return;
}

int CGlobalBackgroundThread::RunBackgroundEvents()
{
	return CreateBackgroundEvents(4);
}

void CGlobalBackgroundThread::CURTAIN_BYPASS(BOOL bOn)
{
	CString strStatus;
	if (theApp.m_bSaveOnlinePassImages || theApp.m_bSaveOnlinePassImages)
		strStatus = "Saving Images";
	else if (theApp.m_bDryRun)
		strStatus = "Dry Run";
	else if(theApp.m_bWetRun)
		strStatus = "Wet Run";
	else
		strStatus = "";

	if (bOn) 
		theApp.m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_CURTAIN_STATUS_STATIC)->SetWindowTextA("Guard Bypass");	
	else 
		theApp.m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_CURTAIN_STATUS_STATIC)->SetWindowTextA(strStatus.GetString());
}

void CGlobalBackgroundThread::START_Light(BOOL bOn)
{
	if (killGlobalThreads) return;

		if (bOn) {
			if (ioControl.SetTowerGreenLightOn()) {
				OutputDebugLogTo(7, TRUE, "----- ERROR IN Green Light (Tower) on");
			
			}

			if (killGlobalThreads) return;

			if (ioControl.StartButtonLampOn()) {
				OutputDebugLogTo(7, TRUE, "----- ERROR IN Green Light (Button) on");
				
			}
		}
		else {
			if (ioControl.SetTowerGreenLightOff()) {
				OutputDebugLogTo(7, TRUE, "----- ERROR IN Green Light (Tower) off");
				
			}
			
			if (killGlobalThreads) return;

			if (ioControl.StartButtonLampOff()) {
				OutputDebugLogTo(7, TRUE, "----- ERROR IN Green Light (Button) off");
			
			}
		}
	
}

void CGlobalBackgroundThread::STOP_Light(BOOL bOn)
{
	if (killGlobalThreads) return;

	if (bOn) {
		if (ioControl.SetTowerRedLightOn()) {
			OutputDebugLogTo(7, TRUE, "----- ERROR IN Red Light (Tower) on");
			
		}

		if (killGlobalThreads) return;

		if (ioControl.StopButtonLampOn()) {
			OutputDebugLogTo(7, TRUE, "----- ERROR IN Red Light (Button) on");
		
		}
	}
	else {
		if (ioControl.SetTowerRedLightOff()) {
			OutputDebugLogTo(7, TRUE, "----- ERROR IN Red Light (Tower) off");
			
		}

		if (killGlobalThreads) return;

		if (ioControl.StopButtonLampOff()) {
			OutputDebugLogTo(7, TRUE, "----- ERROR IN Red Light (Button) off");
		
		}
	}
}

void CGlobalBackgroundThread::CYCLE_STOP_Light(BOOL bOn)
{
	if (killGlobalThreads) return;

	//if (bOn) {
	//	int error;
	//	error = theApp.m_MotorResource.OutPortWriteBit(nCardNo, MODULE_1, OUT_TOWER_LIGHT_YELLOW, bOn);
	//	if (error) {
	//		OutputDebugLogTo(7, TRUE, "----- ERROR IN Red Light (Tower) on");
	//		//setGemAlarm("TOWER-RED-ON-ERROR");
	//	}

	//		error;
	//	error = theApp.m_MotorResource.OutPortWriteBit(nCardNo, MODULE_1, OUT_CYCLE_STOP_LAMP, bOn);
	//	if (error) {
	//		OutputDebugLogTo(7, TRUE, "----- ERROR IN Red Light (Button) on");
	//		//	setGemAlarm("STOP-BUTTON-LIGHT-ON-ERROR");
	//	}
	//}
	//else {
	//	int error;
	//	error = theApp.m_MotorResource.OutPortWriteBit(nCardNo, MODULE_1, OUT_TOWER_LIGHT_YELLOW, bOn);
	//	if (error) {
	//		OutputDebugLogTo(7, TRUE, "----- ERROR IN Red Light (Tower) off");
	//		//setGemAlarm("TOWER-RED-OFF-ERROR");
	//	}


	//		error;
	//	error = theApp.m_MotorResource.OutPortWriteBit(nCardNo, MODULE_1, OUT_CYCLE_STOP_LAMP, bOn);
	//	if (error) {
	//		OutputDebugLogTo(7, TRUE, "----- ERROR IN Red Light (Button) off");
	//		//	setGemAlarm("STOP-BUTTON-LIGHT-OFF-ERROR");
	//	}
	//}
}
void CGlobalBackgroundThread::RESET_Light(BOOL bOn)
{
	if (bOn) {
		if (ioControl.SetTowerOrangeLightOn()) {
			OutputDebugLogTo(7, TRUE, "----- ERROR IN Orange Light (Tower) on");
			//setGemAlarm("TOWER-ORANGE-ON-ERROR");
		}

		if (ioControl.ResetButtonLampOn()) {
			OutputDebugLogTo(7, TRUE, "----- ERROR IN Orange Light (Button) on");
			//setGemAlarm("RESET-BUTTON-LIGHT-ON-ERROR");
		}
	}
	else {
		if (ioControl.SetTowerOrangeLightOff()) {
			OutputDebugLogTo(7, TRUE, "----- ERROR IN Orange Light (Tower) off");
			//setGemAlarm("TOWER-ORANGE-OFF-ERROR");
		}

		if (ioControl.ResetButtonLampOff()) {
			OutputDebugLogTo(7, TRUE, "----- ERROR IN Orange Light (Button) off");
			//setGemAlarm("RESET-BUTTON-LIGHT-OFF-ERROR");
		}
	}
}

void CGlobalBackgroundThread::ACTION_Light(BOOL bOn)
{
	int nSelColor = 0;/* theApp.UserTowerLightOptions[2].nLightColor;*/
	//	bool blink = theApp.UserTowerLightOptions[2].bEnableBlink;
	//	bool buzzer = theApp.UserTowerLightOptions[2].bEnableBuzzer;

	if (bOn) {

		//		if (buzzer)
		//			BuzzerOn(3);

		switch (nSelColor)
		{
		case 0:
			ioControl.SetTowerGreenLightOn();
			ioControl.SetTowerOrangeLightOff();
			ioControl.SetTowerRedLightOff();
			break;
		case 1:
			ioControl.SetTowerOrangeLightOn();
			ioControl.SetTowerGreenLightOff();
			ioControl.SetTowerRedLightOff();
			break;
		case 2:
			ioControl.SetTowerRedLightOn();
			ioControl.SetTowerOrangeLightOff();
			ioControl.SetTowerGreenLightOff();
			break;
		}
	}
	else {
		switch (nSelColor)
		{
		case 0:
			ioControl.SetTowerGreenLightOff();
			break;
		case 1:
			ioControl.SetTowerOrangeLightOff();
			break;
		case 2:
			ioControl.SetTowerRedLightOff();
			break;
		}
	}
}

void CGlobalBackgroundThread::BuzzerOn(int nBeepNo)
{
	for (int i = 0; i < nBeepNo; i++) {
		int nError = ioControl.SetBuzzerOn();
		if (nError) {
			theApp.AddLogEvent("Buzzer ON failed.");
			return;
		}

		Sleep(800);

		nError = ioControl.SetBuzzerOff();
		if (nError) {
			theApp.AddLogEvent("Buzzer OFF failed.");
			return;
		}

		Sleep(500);
	}
}

void CGlobalBackgroundThread::enableMachineResetOperation(bool enable)
{
	_enableMachineResetOperation = enable;
}
bool CGlobalBackgroundThread::getEnableMachineResetOperationStatus() const
{
	return _enableMachineResetOperation;
}

BOOL CGlobalBackgroundThread::IsHardwareReady() const
{
	return hardwareReady;
}

BOOL CGlobalBackgroundThread::IsRunning() const
{
	return sequenceRunning;
}

BOOL CGlobalBackgroundThread::IsModuleReplacement() const
{
	return moduleReplacement;
}

BOOL CGlobalBackgroundThread::IsReScanEnabled() const
{
	return bReScanSequence;
}

BOOL CGlobalBackgroundThread::IsPaused() const
{
	return bSequenceStop;
}

int CGlobalBackgroundThread::RunSequence()
{
	//Lot management
	CameraLiveOff();
	RemoveSequence();
	BuzzerOn(1);

	InterlockedExchange(&runningModuleCount, -1);

//	theApp.SetVisOnLineState(TRUE);
	theApp.OnVisionOnline();
	BOOL bDoorStatus = TRUE;
	
//	while (bDoorStatus) {
//
//		int nError = theApp.ioControl.GetGuardDoorHealthyStatus(bDoorStatus);
//		if (nError)
//			OutputDebugLogTo(7, TRUE, "Failed to Read Status of Guard Door Healthy");
//		
//		if (bDoorStatus) {
////			if (ErrorHandling("Guard Door is Open, Please Close the Door and Continue to Start Machine Sequence"))
////				return -1;
//		}
//		else
//			ioControl.GuardDoorLock(true);
//		bDoorStatus = FALSE;
//	}

	//if(nError)
	//	OutputDebugLogTo(7, TRUE, "------- ERROR Failed to Read Guard Door Healthy Status------");

	//if(!bOn)
	//	ioControl.GuardDoorLock(true);

	//	OutputDebugLogTo(7, TRUE, "----- after vision online");

	CWinThread* pWinThread = ::AfxBeginThread(SequenceRun, this, THREAD_PRIORITY_ABOVE_NORMAL);

	if(pWinThread == NULL) {
		OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING THREAD------");
		return -1;
	}

	sequenceRunning = TRUE;
//	OutputDebugLogTo(7, TRUE, "----- after thread  Sequence running %d", sequenceRunning);
//	theApp.bSequenceRun = TRUE;

	theApp.AddLogEvent("Started Machine Sequence");
	return 0;
}


UINT CGlobalBackgroundThread::SequenceRun(LPVOID pParam)
{
	OutputDebugLogTo(7, TRUE, "START SEQUENCE");

	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->runningModuleCount);

	ResetEvent(pMachineSequence->SequenceLevelDestroyed);

	pMachineSequence->_transportRunning = true;

	pMachineSequence->SequenceFunction();

	pMachineSequence->_transportRunning = false;

	InterlockedIncrement(&pMachineSequence->destroyedModuleCount);

	OutputDebugLogTo(7, TRUE, "END SEQUENCE");

	SetEvent(pMachineSequence->SequenceLevelDestroyed);

	if(pMachineSequence->bCloseSequence) {
		if(theApp.m_pMsgBox != NULL)
			theApp.m_pMsgBox->Msg("Machine Sequence Stopped", 3500);

		pMachineSequence->saveEndLotInformation();
		pMachineSequence->RemoveSequence();
		pMachineSequence->bCloseSequence = FALSE;
		theApp.AddLogEvent("Machine Sequence Stopped");
	}

	if (theApp.m_bScanningEnabled) {
		theApp.m_bScanningEnabled = !theApp.m_bScanningEnabled;
	}

	theApp.m_bScanning = FALSE;
	pMachineSequence->bSequenceGrab = FALSE;

	return 0;
}

void CGlobalBackgroundThread::SequenceFunction()
{

	const int nModuleID = 1;

	DWORD dwError = 0;
	BOOL bSingnalOn = FALSE;
	BOOL bIsFirstScan = TRUE;

	BOOL bStartOver = FALSE;

	BOOL bRepeatOperation = FALSE;
	BOOL bEndSequence = FALSE;
	BOOL bCycleStopActivated = FALSE;
	BOOL bPreTrayRepeat = FALSE;
	double bMoudleReplaceTime, bInspectionSequenceTime;
	
	int nError;

	int nScanColumnsTHR = motionParameters.getScanColumnNoTHR();
	int nScanRowsTHR = motionParameters.getScanRowNoTHR();

	int nScanColumnsZ = motionParameters.getScanColumnNoZSensor();
	int nScanRowsZ = motionParameters.getScanRowNoZSensor();

	int nScanColumnsTop = motionParameters.getScanColumnNoTop();;
	int nScanRowsTop = motionParameters.getScanRowNoTop();

	int nFovNoPerY;
	int nFovNoPerX;

	int nFovNoPerYTOP;
	int nFovNoPerXTOP;

	int nDieNoPerX;
	int nDIeNoPerY;
	int nPrintBarCount = 0;
	CWinThread * pWinThread;

	bool bLotFound = false;
	CString filePath;
	CString strPostMessage;
	filePath.Format("%s\\Summary\\%s\\%s\\", theApp.m_LotSummaryRootDir.GetString(), theApp.m_strConfigFile.GetString(), getDate2().c_str());
	theApp.m_pImageSelectionDlg->ReadLotSummayFile(filePath.GetString(), bLotFound);
	if (bLotFound) {
		theApp.m_pImageSelectionDlg->ReadStripSummary(filePath.GetString());
	}

	theApp.m_pImageSelectionDlg->setup(theApp.m_strMachineID.GetString(), bLotFound);

	if(!bLotFound)
		theApp.m_pImageSelectionDlg->setLotStartTime();

	motionParameters.GetMappingParameters(nDieNoPerX, nDIeNoPerY, nFovNoPerX, nFovNoPerY);
	motionParameters.GetMappingParametersTOP(nFovNoPerXTOP, nFovNoPerYTOP);

	if (!motionParameters.isTHRScanningEnabled() && !motionParameters.isTopScanningEnabled()) {
		AfxMessageBox("Both Top and THR Station Scanning is Disabled. Closing Sequence");
		theApp.AddLogEvent("Both Top and THR Station Scanning is Disabled. Closing Sequence");
		bCloseSequence = TRUE;
		return;
	}
	
	if(motionParameters.CreateScanPositions()) {
		for(int j = 0; j<theApp.m_nTotalTracks; j++) {
			ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
		}
		OutputDebugLogTo(7, TRUE, "----- ERROR : Failed to create scanning position");
	}
	if (getEnableSaveImageLimitStatus()) {

		if (theApp.m_pResourceCheckDlg != NULL) {
			if (!theApp.m_pResourceCheckDlg->m_bDiskOk) {
				CString message;
				message.Format("%s\\ drive is low in memory! Please remove file to avoid unexpected software behavior happened",
					theApp.m_LotSummaryRootDir.Left(theApp.m_LotSummaryRootDir.Find(':') + 1));
				AfxMessageBox(message, MB_OK | MB_ICONERROR);

	
			}
		}
	}

	//Make Sure back door is locked 
	theApp.m_pIoHardwareDlg->SafeDoorLock();

	bStartOver = FALSE;

	//Step 1.0: Preapre Inspection
	OutputDebugLogTo(8, FALSE, "----- Step 0: Trigger Prepare Inspection...");
	for (int nThreadDuplicate = 0; nThreadDuplicate < theApp.m_pTracks[STATION_TOP].getDuplicateInspectionThreadCount(); nThreadDuplicate++) {
		SetEvent(theApp.m_pTracks[STATION_TOP].eventDuplicateInspection[nThreadDuplicate]);
	}
	std::vector < HANDLE > eventDuplicateInspection;
	for (int nThreadId = 0; nThreadId < theApp.m_pTracks[STATION_TOP].getDuplicateInspectionThreadCount(); nThreadId++) {
		eventDuplicateInspection.push_back(theApp.m_pTracks[STATION_TOP].eventDuplicateInspectionDone[nThreadId]);
	}

	while (1) {
		//Reset event before waiting for it
		NetSequenceTime.Reset();
		SequenceTime.Reset();


		if (StepMode()) return;

		//1. Move Motor to READY Position

		//	timer_sequence.Reset();
		//	timerUPH.Reset();

		
		ResetEvent(InitMotion[0]);
		ResetEvent(InitMotion[1]);
		ResetEvent(InitMotion[2]);
		ResetEvent(InitMotion[3]);

		// X-Motor Ready Position
		pWinThread = ::AfxBeginThread(InitXMotor, this, THREAD_PRIORITY_ABOVE_NORMAL);

		if(pWinThread == NULL) {
			OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING InitXMotor THREAD------");
			return;
		}

		// Y-Motor Ready Position
		pWinThread = ::AfxBeginThread(InitYTopMotor, this, THREAD_PRIORITY_ABOVE_NORMAL);

		if(pWinThread == NULL) {
			OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING InitYTopMotor THREAD------");
			return;
		}

		// ST1-Z-Motor Ready Position
		pWinThread = ::AfxBeginThread(InitZMotor, this, THREAD_PRIORITY_ABOVE_NORMAL);

		if(pWinThread == NULL) {
			OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING InitZMotor THREAD------");
			return;
		}

		// ST2-Z-Motor Ready Position
		pWinThread = ::AfxBeginThread(InitZMotorTHR, this, THREAD_PRIORITY_ABOVE_NORMAL);

		if (pWinThread == NULL) {
			OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING InitZMotorTHR THREAD------");
			return;
		}


		// 3. Create arrays for scanning
		int nTotalImageNoTHR = motionParameters.GetTotalImageGrabsTHR();
		int nTotalImageNoTOP = motionParameters.GetTotalImageGrabsTOP();
		
		for (int nDoc = 0; nDoc < theApp.m_pTracks[STATION_THR].m_nTotalDocs; nDoc++) {
			if (theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->nTotalGrabNo != nTotalImageNoTHR) {
				theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->nTotalGrabNo = nTotalImageNoTHR;
				theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->imageGrab.clear();
				theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->imageGrab.resize(nTotalImageNoTHR);

				CSize csImageFOV = theApp.m_pTracks[STATION_THR].m_pDoc[0]->m_Buffer.GetImgSize();

				for (size_t j = 0; j < theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->imageGrab.size(); j++)
					theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->imageGrab[j].Allocate(csImageFOV);
			}
		}

		for (int nDoc = 0; nDoc < theApp.m_pTracks[STATION_TOP].m_nTotalDocs; nDoc++) {
			if (theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->nTotalGrabNo != nTotalImageNoTOP) {
				theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->nTotalGrabNo = nTotalImageNoTOP;
				theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->imageGrab.clear();
				theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->imageGrab.resize(nTotalImageNoTOP);

				CSize csImageFOV = theApp.m_pTracks[STATION_TOP].m_pDoc[0]->m_Buffer.GetImgSize();

				for (size_t j = 0; j < theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->imageGrab.size(); j++)
					theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->imageGrab[j].Allocate(csImageFOV);
			}
		}

		dwError = WaitForMultipleObjects(4, InitMotion, TRUE, INFINITE);
		if(dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {
			OutputDebugLogTo(7, TRUE, "------- ERROR IN THREAD MANIPULATION------");
			return;
		}
		if(killThreads) return;

		if(nInitMotionError[0] != 0 || nInitMotionError[1] != 0 || nInitMotionError[2] != 0 || nInitMotionError[3] != 0) {
			if(nInitMotionError[0] != 0) {
				OutputDebugLogTo(7, TRUE, "X Motor Ready Position Error = %d", nInitMotionError[0]);
			}
			if(nInitMotionError[1] != 0) {
				OutputDebugLogTo(7, TRUE, "Y Motor Ready Position Error = %d", nInitMotionError[1]);
			}
			if(nInitMotionError[2] != 0) {
				OutputDebugLogTo(7, TRUE, "ST1 Z Motor Ready Position Error = %d", nInitMotionError[2]);
			}
			if (nInitMotionError[3] != 0) {
				OutputDebugLogTo(7, TRUE, "ST2 Z Motor Ready Position Error = %d", nInitMotionError[3]);
			}

			if(ErrorHandling("Motors failed in motion. Please check motor condition"))
				return;
			continue;
		}

		if(bEndSequence)
			return;

		if(bCycleStopActivated) {
			bEnableStepMode = TRUE;
			bCycleStopPressed = FALSE;
			if(StepMode()) return;
			bCycleStopActivated = FALSE;
		}
				
		if(StepMode()) return;

		bCurtainMuteActivated = TRUE;

		bSingnalOn = FALSE;
		int maximumRetry = 4;
		int retryCount = 0;
		BOOL bUnloadOld = false;

		if(StepMode()) return;
		
		moduleReplacement = TRUE;
		bMoudleReplaceTime = SequenceTime.GetElapsedMSec();

		bSingnalOn = TRUE;
			
		SequenceTime.Reset();
			
		moduleReplacement = FALSE;

		if(StepMode()) return;
		bCurtainMuteActivated = FALSE;

		if (!bIsFirstScan && !theApp.m_bInspSeqLoop) {
			if (ErrorHandling("Start Scan Panel?"))
				return;
		}
		bIsFirstScan = FALSE;

		OutputDebugLogTo(9, TRUE, "--- Start Sequence Run ---");
		
	
		theApp.m_bScanning = TRUE;

		nPrintBarCount;
		theApp.strInspectDeviceBarcodeID.Format("%d", nPrintBarCount);

		
		bPreTrayRepeat = FALSE;
		if (theApp.m_bEnableRepeatScanOption)
			bPreTrayRepeat = theApp.m_pImageSelectionDlg->IsPreTrayRepeat(theApp.strInspectDeviceBarcodeID);

		if (!bDryRun && !bWetRun) {
//				theApp.m_pImageSelectionDlg->compareStripId(theApp.strInspectDeviceBarcodeID.GetString());
			theApp.m_pImageSelectionDlg->SetBarcodeID(theApp.strInspectDeviceBarcodeID.GetString());
		}

		if(StepMode()) return;

		//Lot display
		//theApp.m_strLotInfo = theApp.strInspectDeviceBarcodeID.Mid(0, 4);
		theApp.m_strLotInfo = theApp.strInspectDeviceBarcodeID;
		theApp.m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_LOT_ID_STATIC)->SetWindowTextA(theApp.m_strLotInfo);

		theApp.m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_RECEIPE_STATUS_STATIC)->SetWindowTextA(theApp.m_strConfigFile);

		if(StepMode()) return;

		//2. Move Motor to Start Position

		ResetEvent(StartMotion[0]);
		ResetEvent(StartMotion[1]);
		//ResetEvent(StartMotion[2]);

		// X-Motor Start Position
		pWinThread = ::AfxBeginThread(StartXMotor, this, THREAD_PRIORITY_ABOVE_NORMAL);

		if(pWinThread == NULL) {
			OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING THREAD------");
			return;
		}

		// YTop-Motor Start Position
		pWinThread = ::AfxBeginThread(StartYTopMotor, this, THREAD_PRIORITY_ABOVE_NORMAL);
			
		if(pWinThread == NULL) {
			OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING THREAD------");
			return;
		}

		//// ST1 Z-Motor Start Position For TOP Station
		nStartMotionError[2] = MoveZMotorToStartPosition();

		if(killThreads)
			return;

		dwError = WaitForMultipleObjects(2, StartMotion, TRUE, INFINITE);
		if(dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {
			OutputDebugLogTo(7, TRUE, "------- ERROR IN THREAD MANIPULATION------");
			return;
		}

		if(nStartMotionError[0] != 0 || nStartMotionError[1] != 0 || nStartMotionError[2] != 0) {
			if(nInitMotionError[0] != 0) {
				OutputDebugLogTo(7, TRUE, "X Motor Start Position Error = %d", nStartMotionError[0]);

			}
			if(nInitMotionError[1] != 0) {
				OutputDebugLogTo(7, TRUE, "Y Motor Start Position Error = %d", nStartMotionError[1]);
			}
			if(nInitMotionError[2] != 0) {
				OutputDebugLogTo(7, TRUE, "ST1 Z Motor Start Position Error = %d", nStartMotionError[2]);
			}

			if(ErrorHandling("Motors failed in motion. Please check motor condition"))
				return;
			continue;
		}
		if(StepMode()) return;


		bSingnalOn = FALSE;
//			bSingnalOn = FALSE;;
		maximumRetry = 3;
		retryCount = 0;

			
		InterlockedExchange(&theApp.nZSensorPositionId, 0);
		for(int i = 0; i<theApp.m_nTotalTracks; i++) {
			for (int j = 0; j < theApp.m_pTracks[i].m_nTotalDocs; j++) {
				InterlockedExchange(&theApp.m_pTracks[i].m_pDoc[j]->nCameraGrabNo, -1);
				//				InterlockedExchange(&theApp.m_pTracks[i].nCameraDummyGrabNo, -1);			//Only for Top
			}
		}

		theApp.nZSensorPositionCount = nScanRowsZ * nScanColumnsZ;
//		OutputDebugLogTo(6, TRUE, "nZSensorPositionCount=  %d", theApp.nZSensorPositionCount);

		for(int i = 0; i<theApp.m_nTotalTracks; i++) {
			theApp.m_pTracks[i].CancelSnap();
		}

//			ResetEvent(theApp.m_pTracks[STATION_TOP].eventInspectionDone);

		//Prepare Inspection
		CPerformanceTimer prepareInspectionTimer;
		DWORD dwError = WaitForMultipleObjects(eventDuplicateInspection.size(), eventDuplicateInspection.data(), TRUE, INFINITE);
		if (dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {

			OutputDebugLogTo(7, TRUE, "-----ERROR: Duplicate Inspection threads Waiting have been failed.");

			if (ErrorHandling("Duplicate Inspection threads Waiting have been failed"))
				return;
			continue;
		}
		OutputDebugLogTo(8, FALSE, "----- Step 0: Prepare Inspection : %.3f ms", prepareInspectionTimer.GetElapsedMSec());

		bSequenceGrab = TRUE;

		theApp.m_pImageSelectionDlg->ResetDieStates();
		motionParameters.resetInspection();
		motionParameters.ClearComputerId();

		/* 1. TOP Die Scanning */

		if (/*motionParameters.isTHRScanningEnabled() ||*/ motionParameters.isTopScanningEnabled()) {
			OutputDebugLogTo(8, TRUE, "----- Starting Scan TOP Station...");

			for (int i = 0; i < nScanRowsTop; i++) {
				OutputDebugLogTo(8, TRUE, "----- Step 1: Cancel All Snap, Refresh Light and Reset Trigger Count...");

				for (int j = 0; j < theApp.m_nTotalTracks; j++) {
					theApp.m_pTracks[j].CancelSnap();
				}

				int nFov = i % nFovNoPerYTOP;
				theApp.m_pTracks[STATION_TOP].RefreshLightFOV(0, nFov);			//Intensity Top Die
				theApp.SetSequenceIntensityId(0);

				if (motionParameters.isTopScanningEnabled())
					nError = motionParameters.ResetScaleTriggerCount(X_SCALE_1);

				if (nError) {
					if (ErrorHandling("Reset X_SCALE_1 Trigger Count is failed. Please check trigger source wire"))
						return;
					bStartOver = TRUE; break;
				}

				if(motionParameters.isTHRScanningEnabled() && nFov == 2)
					nError = motionParameters.ResetScaleTriggerCount(X_SCALE_2);

				
				if (nError) {
					if (ErrorHandling("Reset X_SCALE_2 Trigger Count is failed. Please check trigger source wire"))
						return;
					bStartOver = TRUE; break;
				}

				OutputDebugLogTo(8, TRUE, "----- Step 2: Set Compare Trigger...");


				InterlockedExchange(&motionParameters.nZSensorScanRow, i);

				if (motionParameters.isTopScanningEnabled()) {
					motionParameters.SetScaleCompareTrigger(i);
					ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventTrigCount);
				}

				int nScanRowZSensor = i / nFovNoPerYTOP;
				if (motionParameters.isTHRScanningEnabled() && nFov == 2) {
					motionParameters.SetScaleCompareTriggerZSensor(nScanRowZSensor);
				}

				OutputDebugLogTo(8, TRUE, "----- Step 3: SetTrigMode For Camera...");

				if (motionParameters.isTopScanningEnabled()) {
					nError = SetTrigMode(&theApp.m_pTracks[STATION_TOP].m_FGResource, TRIG_HARD, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_TOP].m_nTrack]);
					if (nError) {
						OutputDebugLogTo(7, TRUE, "-----ERROR: [%d] Camera Set Trigger Mode is failed. Please check camera condition", theApp.m_pTracks[STATION_TOP].m_nTrack);
					}
					//EnableOutputLine(&theApp.m_pTracks[STATION_THR].m_FGResource, false);

					OutputDebugLogTo(8, TRUE, "----- Step 3.1: SetEvent Image Grabbing...");

					SetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
				}
					

				if (killThreads) return;

				if (motionParameters.isTHRScanningEnabled() && nFov == 2) {
					ResetEvent(theApp.m_eventZSensorTrigCount);
					SetEvent(theApp.eventRunZSensorSequence);
				}

				if (StepMode()) return;
				
				//Move To Z Sensor Position
				OutputDebugLogTo(8, TRUE, "----- Step 4: Move X-Motor To End Position Of Row %d...", i);

				nError = motionParameters.MoveXMotorToEndPosTopnZ(i);
				if (nError) {
					if (ErrorHandling("Scanning movement failed by X motor. Check motor status")) return;
					bStartOver = TRUE; break;
				}
				

				OutputDebugLogTo(8, TRUE, "----- Step 5: Waiting for Top and Z-Sensor trigger count...");

				//Wait for Grab and Z data.
				std::vector < HANDLE > eventWait;

				if(motionParameters.isTHRScanningEnabled() && nFov == 2)
					eventWait.push_back(theApp.m_eventZSensorTrigCount);

				if(motionParameters.isTopScanningEnabled())
					eventWait.push_back(theApp.m_pTracks[STATION_TOP].m_eventTrigCount);

				dwError = WaitForMultipleObjects(eventWait.size(), eventWait.data(), TRUE, /*theApp.getMotionTimeOut()*/10000);

				if (killThreads) return;

				OutputDebugLogTo(8, TRUE, "----- Step 6: Waiting for Top and Z-Sensor trigger count: done");

				if (dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {

					if (theApp.m_bExitApp) break;
					OutputDebugLogTo(8, TRUE, "----- Step 6: Waiting for Top and Z-Sensor trigger count failed");

					const long nCameraGrabNo = InterlockedExchangeAdd(&theApp.m_pTracks[STATION_TOP].m_pDoc[0]->nCameraGrabNo, 0);

					bool bLastGrabOnRow = false;

					if (theApp.motionParameters.IsCameraGrabIDLastOnRowTop(nCameraGrabNo + 1, bLastGrabOnRow)) {
						OutputDebugLogTo(0, TRUE, "----- ERROR : Can't get information about camera grab [%d]", nCameraGrabNo);
						continue;
					}

					OutputDebugLogTo(8, TRUE, "Camera ID %d, last %d", nCameraGrabNo, bLastGrabOnRow);


					if (!bLastGrabOnRow) {
						if (ErrorHandling("Scanning movement failed by camera triggering. Repeat last row scan.")) return;
					}
					else {
						if (ErrorHandling("Scanning movement failed by triangular Sensor. Please check sensor condition")) return;
					}

					OutputDebugLogTo(8, TRUE, "After error");

					if (motionParameters.isTopScanningEnabled()) {
						InterlockedExchange(&theApp.m_pTracks[STATION_TOP].m_pDoc[0]->nCameraGrabNo, -1);
				//		SetTrigMode(&theApp.m_pTracks[STATION_TOP].m_FGResource, TRIG_SOFT, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_TOP].m_nTrack]);
					}



					if (motionParameters.isTHRScanningEnabled() && nFov == 2) {
						InterlockedExchange(&theApp.nZSensorPositionId, 0);
						ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
					}

					if (StepMode()) return;

					motionParameters.StopAxis(X_SERVO_MOTOR);

					nError = motionParameters.MoveYTopMotorTop(0);		//Top row 1 and 4
					if (nError) {
						if (ErrorHandling("Scanning movement failed by Y motor. Check motor status")) return;
						break;
					}

					if (theApp.m_bExitApp) break;

					nError = MoveXMotorToStartPosition();
					if (nError) {
						if (ErrorHandling("Scanning movement failed by X motor. Check motor status")) return;
						break;
					}

					nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
					if (nError) {
						ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
						OutputDebugLogTo(7, TRUE, "----- ERROR : Absolute move for X motor failed");
						if (ErrorHandling("Motors failed in motion. Please check motor condition"))
							return;
						bStartOver = TRUE; break;
					}

					nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
					if (nError) {

						ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
						if (ErrorHandling("Motors failed in motion. Please check motor condition")) {
							return;
						}
						bStartOver = TRUE; break;
					}

					if (theApp.m_bExitApp) break;

					motionParameters.WaitMotorSettlingTime(Y_SERVO_MOTOR);

					i = -1;
					continue;
				}

				if (bStartOver)
					break;

				if (killThreads) return;

				OutputDebugLogTo(8, TRUE, "----- Step 7: ResetEvent Image Grabbing...");

				if(motionParameters.isTopScanningEnabled())
					ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);


				if (i < nScanRowsTop - 1) {
					//Move Y-Motor to next row
					OutputDebugLogTo(8, TRUE, "----- Step 8: Move Y-Motor To Start Scan Position Of Row %d...", i+1);

					nError = motionParameters.MoveYTopMotorTop((i + 1));
					if (nError) {
						if (ErrorHandling("Scanning movement failed by Y motor. Check motor status")) return;
						bStartOver = TRUE; break;
					}

					//Move ST1 Z-Motor to next Fov Position
					int nNextFov = (i + 1) % nFovNoPerYTOP;
					OutputDebugLogTo(8, TRUE, "----- Step 8.1: Move ST1 Z-Motor To Start Scan Position Of Row %d Fov %d...", i + 1, nNextFov);
					
					nError = motionParameters.MoveTOPZMotorToReadyPos(nNextFov);
					if (nError) {
						if (ErrorHandling("Scanning movement failed by ST1 Z motor. Check motor status")) return;
						bStartOver = TRUE; break;
					}

					//Wait For Y-Motor
					nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, INFINITE);
					if (nError) {
						ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
						OutputDebugLogTo(7, TRUE, "----- ERROR : Y motor waiting for motion done is failed");

						if (ErrorHandling("Y Motor failed in motion. Please check motor condition"))
							return;
						bStartOver = TRUE; break;
					}

					if (theApp.m_bExitApp) break;

					motionParameters.WaitMotorSettlingTime(Y_SERVO_MOTOR);
					OutputDebugLogTo(8, TRUE, "----- Step 8: Move Y-Motor To Start Scan Position Of Row %d: Done", i + 1);

					//Wait For ST1 Z-Motor
					nError = motionParameters.WaitForMotionDone(ST1_Z_SERVO_MOTOR, INFINITE);
					if (nError) {
						ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
						OutputDebugLogTo(7, TRUE, "----- ERROR : ST1 Z motor waiting for motion done is failed");

						if (ErrorHandling("ST1 Z Motor failed in motion. Please check motor condition"))
							return;
						bStartOver = TRUE; break;
					}
					if (theApp.m_bExitApp) break;

					motionParameters.WaitMotorSettlingTime(ST1_Z_SERVO_MOTOR);
					OutputDebugLogTo(8, TRUE, "----- Step 8.1: Move ST1 Z-Motor To Start Scan Position Of Row %d: Done", i + 1);
				}

				if (theApp.m_bExitApp) break;

				OutputDebugLogTo(8, TRUE, "----- Step 9: Wait For X-Motor Complete...");


				nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
				if (nError) {
					ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
					OutputDebugLogTo(7, TRUE, "----- ERROR : X motor waiting for motion done is failed");

					if (ErrorHandling("Motors failed in motion. Please check motor condition"))
						return;
					bStartOver = TRUE; break;
				}

				OutputDebugLogTo(8, TRUE, "----- Step 9: Wait For X-Motor Complete: Done");
				OutputDebugLogTo(8, TRUE, "--------------------------------------------------");
			}
		}
		
		if (bStartOver)
			continue;

		//Set Back Trigger Soft for Camera To able Single Snap or Live Stream
		SetTrigMode(&theApp.m_pTracks[STATION_TOP].m_FGResource, TRIG_SOFT, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_TOP].m_nTrack]);
			



		/* 2. THR Die Scanning */

		if (motionParameters.isTHRScanningEnabled()) {

			theApp.m_pTracks[STATION_THR].RefreshLight(0);			//Intensity Top Die
			theApp.SetSequenceIntensityId(0);

			for (int j = 0; j < theApp.m_nTotalTracks; j++) {
				theApp.m_pTracks[j].CancelSnap();
			}

			//			EnableOutputLine(&theApp.m_pTracks[STATION_TOP].m_FGResource, true);
			//			EnableOutputLine(&theApp.m_pTracks[STATION_THR].m_FGResource, true);

			nError = motionParameters.CreateNewPositions();
			if (nError) {
				OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING Z POSITIONS");
			}

			//THR Scan
			ResetEvent(StartMotionTHR[0]);
			ResetEvent(StartMotionTHR[1]);


			pWinThread = ::AfxBeginThread(StartXMotorTHR, this, THREAD_PRIORITY_ABOVE_NORMAL);
			if (pWinThread == NULL) {
				OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING THREAD------");
				return;
			}

			// Y-Motor - Start Scan Postion (Use values from MotionParams)
			pWinThread = ::AfxBeginThread(StartYTopMotorTHR, this, THREAD_PRIORITY_ABOVE_NORMAL);
			if (pWinThread == NULL) {
				OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING THREAD------");
				return;
			}

			nStartMotionErrorTHR[2] = MoveZMotorToStartPositionTHR();

			dwError = WaitForMultipleObjects(2, StartMotionTHR, TRUE, INFINITE);
			if (dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {
				OutputDebugLogTo(7, TRUE, "------- ERROR IN THREAD MANIPULATION------");
				return;
			}

			if (nStartMotionErrorTHR[0] != 0 || nStartMotionErrorTHR[1] != 0 || nStartMotionErrorTHR[2] != 0) {
				if (ErrorHandling("Start scan movement is failed. Please check motor condition"))
					return;
				break;
			}

			if (StepMode()) return;

			InterlockedExchange(&theApp.m_pTracks[STATION_THR].m_pDoc[0]->nCameraGrabNo, -1);

			for (int j = 0; j < theApp.m_nTotalTracks; j++) {
				theApp.m_pTracks[j].CancelSnap();
			}

			ResetEvent(MoveYAxisScanTHR);
			ResetEvent(MoveYAxisScanDoneTHR);

			bScanModeTHR = TRUE;

			//Debug
			if (!motionParameters.isTopScanningEnabled()) {
				if (ErrorHandling("Start THR Scan?"))
					return;
			}

			pWinThread = ::AfxBeginThread(ScanYTopMotorTHR, this, THREAD_PRIORITY_ABOVE_NORMAL);

			if (pWinThread == NULL) {
				OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING THREAD------");
				return;
			}

			for (int i = nScanRowsTHR - 1; i >= 0; i--) {
				OutputDebugLogTo(7, TRUE, "----- Step 1: Cancel All Snap, Refresh Light and Reset Trigger Count...");

				for (int j = 0; j < theApp.m_nTotalTracks; j++) {
					theApp.m_pTracks[j].CancelSnap();
				}

				if (killThreads) return;

				nError = motionParameters.ResetScaleTriggerCount(X_SCALE_3);
				if (nError) {
					if (ErrorHandling("Reset X_SCALE_3 Trigger Count is failed. Please check trigger source wire"))
						return;
					bStartOver = TRUE; break;
				}

				nError = motionParameters.SetScaleCompareTriggerTHR(i);
				if (nError) {
					if (ErrorHandling("Set Compare X_SCALE_3 Trigger is failed. Please check trigger source wire")) return;
					bStartOver = TRUE; break;
				}

				if (killThreads) return;

				OutputDebugLogTo(7, TRUE, "----- Step 2: SetTrigMode For THR Camera...");

				SetTrigMode(&theApp.m_pTracks[STATION_THR].m_FGResource, TRIG_HARD, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_THR].m_nTrack]);
				//EnableOutputLine(&theApp.m_pTracks[STATION_TOP].m_FGResource, false);

				OutputDebugLogTo(7, TRUE, "----- Step 3: SetEvent THR Camera Grabbing...");
				ResetEvent(theApp.m_pTracks[STATION_THR].m_eventTrigCount);
				SetEvent(theApp.m_pTracks[STATION_THR].m_eventGrabbing);

				if (killThreads) return;

				//Move motors
				bEmptyScanTHR = FALSE;
				nMotorYAxisScanPosTHR = i;

				OutputDebugLogTo(7, TRUE, "----- Step 4: ScanXMotorFunctionTHR at row %d...", i);

				//Reset event before waiting for it
				ResetEvent(MoveYAxisScanDoneTHR);
				SetEvent(MoveYAxisScanTHR);

				int nScanXAxisError = ScanXMotorFunctionTHR(i);
				if (WaitForSingleObject(MoveYAxisScanDoneTHR, INFINITE) != WAIT_OBJECT_0) {
					OutputDebugLogTo(7, TRUE, "------- ERROR IN THREAD MANIPULATION------");
					return;
				}

				if (killThreads) return;

				ResetEvent(theApp.m_pTracks[STATION_THR].m_eventGrabbing);

				//	SetTrigMode(&theApp.m_pTracks[STATION_THR].m_FGResource, TRIG_SOFT, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_THR].m_nTrack]);
				//	EnableOutputLine(&theApp.m_pTracks[STATION_TOP].m_FGResource, true);

				if (killThreads) return;

				if (nScanXAxisError != 0) {
					if (ErrorHandling("Scanning movement failed by X motor. Check motor status")) return;
					bStartOver = TRUE; break;
				}

				if (killThreads) return;

				if (nScanYAxisErrorTHR == -100) {

					if (StepMode()) return;

					i = -1;

					bEmptyScanTHR = TRUE;

					//THR Scan
					ResetEvent(StartMotionTHR[0]);
					ResetEvent(StartMotionTHR[1]);

					pWinThread = ::AfxBeginThread(StartXMotorTHR, this, THREAD_PRIORITY_ABOVE_NORMAL);
					if (pWinThread == NULL) {
						OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING THREAD------");
						return;
					}

					// Y-Motor - Start Scan Postion (Use values from MotionParams)
					pWinThread = ::AfxBeginThread(StartYTopMotorTHR, this, THREAD_PRIORITY_ABOVE_NORMAL);
					if (pWinThread == NULL) {
						OutputDebugLogTo(7, TRUE, "------- ERROR IN CREATING THREAD------");
						return;
					}

					nStartMotionErrorTHR[2] = MoveZMotorToStartPositionTHR();


					dwError = WaitForMultipleObjects(2, StartMotionTHR, TRUE, INFINITE);
					if (dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {
						OutputDebugLogTo(7, TRUE, "------- ERROR IN THREAD MANIPULATION------");
						return;
					}

					if (nStartMotionErrorTHR[0] != 0 || nStartMotionErrorTHR[1] != 0 || nStartMotionErrorTHR[2] != 0) {
						if (ErrorHandling("Start scan movement is failed. Please check motor condition")) {
							return;
						}
					}

					if (StepMode()) return;
					InterlockedExchange(&theApp.m_pTracks[STATION_THR].m_pDoc[0]->nCameraGrabNo, -1);
					continue;
				}

				if (bStartOver)
					break;

				if (nScanYAxisErrorTHR != 0) {
					if (ErrorHandling("Scanning movement failed by Y (Top) motor. Check motor status")) return;
					bStartOver = TRUE; break;
				}

				if (StepMode()) return;

				OutputDebugLogTo(7, TRUE, "----- Step 5: Finish scan at row %d...", i);
			}

			if (bStartOver)
				continue;


			bEmptyScanTHR = TRUE;
			bScanModeTHR = FALSE;


			//Reset event before waiting for it
			ResetEvent(MoveYAxisScanDoneTHR);
			SetEvent(MoveYAxisScanTHR);

			if (WaitForSingleObject(MoveYAxisScanDoneTHR, INFINITE) != WAIT_OBJECT_0) {
				OutputDebugLogTo(7, TRUE, "------- ERROR IN THREAD MANIPULATION------");
				return;
			}
		}

		if (bStartOver)
			continue;

		for (int nTrack = 0; nTrack < theApp.m_nTotalTracks; nTrack++) {
			SetTrigMode(&theApp.m_pTracks[nTrack].m_FGResource, TRIG_SOFT, theApp.m_nCameraTrigDelay[theApp.m_pTracks[nTrack].m_nTrack]);
		}
		bSequenceGrab = FALSE;

		//Wait for inspection.
		std::vector < HANDLE > eventInspection;
		bool bWaitforInsp;

		//TOP Station Event Inspection
		if (motionParameters.isTopScanningEnabled()) {
			for (int nThreadId = 0; nThreadId < theApp.m_pTracks[STATION_TOP].getInspectionThreadCount(); nThreadId++) {
				eventInspection.push_back(theApp.m_pTracks[STATION_TOP].eventInspectionDone[nThreadId]);
			}
		}

		//TOP Station Event Inspection
		if (motionParameters.isTHRScanningEnabled()) {
			for (int nThreadId = 0; nThreadId < theApp.m_pTracks[STATION_THR].getInspectionThreadCount(); nThreadId++) {
				eventInspection.push_back(theApp.m_pTracks[STATION_THR].eventInspectionDone[nThreadId]);
			}
		}

		//THR Station Event Inspection
		/*std::vector < int > threadId = motionParameters.getInspectionThreadId();
		std::for_each(threadId.begin(), threadId.end(), [&](int id) {eventInspection.push_back(theApp.m_pTracks[STATION_THR].eventInspectionDone[id]);});*/


		
		OutputDebugLogTo(8, TRUE, "----- Step 9: Waiting for inspection (Size: %d, Timeout: 10 seconds)", eventInspection.size());

		nError = WaitForMultipleObjects(eventInspection.size(), eventInspection.data(), TRUE, 10000);

		OutputDebugLogTo(8, TRUE, "----- Step 10: Waiting for inspection Done");

		if(nError == WAIT_FAILED || nError == WAIT_TIMEOUT) {
			for(int j = 0; j<theApp.m_nTotalTracks; j++) {
				ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
			}
			OutputDebugLogTo(8, TRUE, "-----ERROR: Image Inspection threads have been failed.");
			break;
		}
		//SetEvent(theApp.eventClearMemory);
		motionParameters.ClearComputerId();

		theApp.m_bScanning = FALSE;

		if (!bDryRun && !bWetRun) {

			if (theApp.m_bVerificationMode) {
				theApp.m_pVerificationMode->BackUpPreviousMapping();
				theApp.m_pVerificationMode->ShowWindow(TRUE);
				bEnableStepMode = TRUE;
				if (StepMode()) return;
				theApp.m_pVerificationMode->ShowWindow(FALSE);
				theApp.m_bVerificationMode = FALSE;
				for (int i = 0; i < theApp.m_nTotalTracks; i++) {
					theApp.m_pTracks[i].AddImageToBufferQueue();
				}
			}

			theApp.m_pImageSelectionDlg->setStripEndTime();
			theApp.UpdatePrintBar(theApp.m_pImageSelectionDlg->badDeviceCount());
			theApp.UpdateStatistics(theApp.m_pImageSelectionDlg->goodDieCount(), theApp.m_pImageSelectionDlg->badDieCount());
			if (bPreTrayRepeat && theApp.m_bEnableRepeatScanOption) {
				theApp.UpdateStatisticsOnRepeatScan(theApp.m_pImageSelectionDlg->nRepeatGoodDieCount, theApp.m_pImageSelectionDlg->nRepeatBadDieCount);
				theApp.UpdatePrintBarOnRepeatScan(theApp.m_pImageSelectionDlg->nRepeatBadDieCount);
				theApp.m_pImageSelectionDlg->UpdateStatsOnRepeatScan();
			}
			nError = theApp.m_pImageSelectionDlg->SaveMappingData();
			nError = theApp.m_pImageSelectionDlg->SavePartSummary();
			theApp.m_pImageSelectionDlg->savePrintBarSummaryLogFile();
			theApp.m_pImageSelectionDlg->Invalidate();
		}

		bInspectionSequenceTime = SequenceTime.GetElapsedMSec();
		theApp.UpdateUPH(bInspectionSequenceTime + bMoudleReplaceTime, NetSequenceTime.GetElapsedMSec(),  nDieNoPerX * nDIeNoPerY);
		OutputDebugLogTo(9, TRUE, "Net Sequence Time %3.03f ms, UPH %2.02f", NetSequenceTime.GetElapsedMSec(), static_cast<double>(60000/ NetSequenceTime.GetElapsedMSec()));
		OutputDebugLogTo(9, TRUE, "--- End Sequence Run ---");

		if (bReScanSequence && theApp.m_bEnableReScanOption) {
			bReScanSequence = FALSE;
			//theApp.motionParameters.ResetReScanStatus();
		}
			
		if(bCycleStopPressed) {
			//bEndSequence = TRUE;
			bCycleStopActivated = TRUE;
		}
	}	
}

int CGlobalBackgroundThread::StepMode()
{

	if(killThreads) return 1;

	if(bEnableStepMode || bAirPressureError) {

		do {
			ResetEvent(EventNextStep);

			EnterCriticalSection(&CriticalSectionStepMode);

			bSequenceStop = TRUE;

			LeaveCriticalSection(&CriticalSectionStepMode);

			if(WaitForSingleObject(EventNextStep, INFINITE) != WAIT_OBJECT_0) {
				OutputDebugLogTo(7, TRUE, "-----ERROR IN STEP MODE. CLOSING SEQUENCE...");
				return -1;
			}

			if (bImmediateStopActivated)
				bImmediateStopActivated = FALSE;
		//		OutputDebugLogTo(4, TRUE, "Immediate Stop Deactivated, %d", bImmediateStopActivated);

			if(killThreads) return 1;

		} while(bAirPressureError);

		if (bImmediateStopActivated)
			bImmediateStopActivated = FALSE;

		EnterCriticalSection(&CriticalSectionStepMode);
		
		bSequenceStop = FALSE;
//			theApp.m_pMainWnd->SendMessage(WM_ONLINE_ON);
		
		LeaveCriticalSection(&CriticalSectionStepMode);

		if(killThreads) return 1;
	}

	return 0;
}

void CGlobalBackgroundThread::CameraLiveOff()
{
	//Live cam off in case if on
	for (int ntrack = 0; ntrack < theApp.m_nTotalTracks; ntrack++) {
		for (int nDoc = 0; nDoc < theApp.m_pTracks[ntrack].m_nTotalDocs; nDoc++) {
			if (theApp.m_pTracks[ntrack].m_pDoc[nDoc]->m_bLiveImageSingle) {
				SetEvent(theApp.m_pTracks[ntrack].m_pDoc[nDoc]->m_EventKillLiveSingle);
				theApp.m_pTracks[ntrack].m_pDoc[nDoc]->m_bLiveImageSingle = FALSE;
				Sleep(100);
			}
		}
		if (theApp.m_pTracks[ntrack].m_bLiveImage) {
			SetEvent(theApp.m_pTracks[ntrack].m_EventKillLive);
			theApp.m_pTracks[ntrack].m_bLiveImage = FALSE;
			Sleep(100);
		}
	}
}

void CGlobalBackgroundThread::RemoveSequence()
{
	EnterCriticalSection(&CriticalSectionSequence);

	killThreads = TRUE;
	theApp.OnGooffline();

	BOOL bPrevRunStatus = IsRunning();
	sequenceRunning = FALSE;
//	theApp.bSequenceRun = FALSE;
	bSequenceStop = FALSE;
	bSequenceError = FALSE;
	bSequenceGrab = FALSE;
	bImmediateStopActivated = FALSE;
	//Stop all motors

	for(int i = 0; i<theApp.m_nTotalTracks; i++)
		SetEvent(theApp.m_pTracks[i].m_eventTrigCount);

	SetEvent(theApp.m_eventZSensorTrigCount);

	for(int i = 0; i<theApp.m_nTotalTracks; i++) {
		theApp.m_pTracks[i].CancelSnap();
	}

	for(int i = 0; i < theApp.m_nTotalTracks; i++) {
		SetTrigMode(&theApp.m_pTracks[i].m_FGResource, TRIG_SOFT, theApp.m_nCameraTrigDelay[theApp.m_pTracks[i].m_nTrack]);
	}

	for(int i = 0; i < theApp.m_nTotalTracks; i++) {
		std::for_each(theApp.m_pTracks[i].eventInspectionDone.begin(), theApp.m_pTracks[i].eventInspectionDone.end(),
			[](HANDLE & handle) { SetEvent(handle); });

//		SetEvent(theApp.m_pTracks[i].eventInspectionDone);
	}

	if(bPrevRunStatus) {

	}
//	motionParameters.StopEveryAxis();

	SetEvent(EventNextStep);
	SetEvent(EventRetry);
	SetEvent(MoveYAxisScanTHR);

	const long nModuleNo = InterlockedExchangeAdd(&runningModuleCount, 0) + 1;

	if(nModuleNo > 0) {
		OutputDebugLogTo(7, TRUE, "-----WAITING FOR DESTOYING SEQUENCE");

		DWORD dwError = WaitForSingleObject(SequenceLevelDestroyed, 10000);
		if(dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {
			OutputDebugLogTo(7, TRUE, "-----ERROR IN MAIN THREAD DESTROYING");
		}
	}
	
	for(int i = 0; i<theApp.m_nTotalTracks; i++)
		ResetEvent(theApp.m_pTracks[i].m_eventTrigCount);

	ResetEvent(theApp.m_eventZSensorTrigCount);

//	bLevelError = FALSE;
	bEnableStepMode = FALSE;

	/*SetEvent(MoveYAxisScan);*/

	ResetEvent(EventNextStep);
	ResetEvent(EventRetry);
	ResetEvent(MoveYAxisScanTHR);
//	ResetEvent(EventSelectBranch);

	const long nDestroyedModules = InterlockedExchangeAdd(&destroyedModuleCount, 0);
	const long nTempThreads = InterlockedExchangeAdd(&temporaryThreadCount, 0);

	if(nDestroyedModules > 0)
		OutputDebugLogTo(7, TRUE, "-----DESTROYED MAIN THREADS %d of %d", nDestroyedModules, nModuleNo);

	if(nTempThreads > 0)
		OutputDebugLogTo(7, TRUE, "-----TEMP THREADS ARE STILL RUNNING: %d", nTempThreads);

	killThreads = FALSE;

	InterlockedExchange(&destroyedModuleCount, 0);

	LeaveCriticalSection(&CriticalSectionSequence);

	if(!theApp.m_bExitApp && bPrevRunStatus)
		theApp.AddLogEvent("Stopped Machine Sequence");

	//reset any errors in errordlg
	if(theApp.pErrorMsgDlg != NULL)
		theApp.pErrorMsgDlg->ResetState();

	//Close Sequence Brainching and Error Message Dialog, if they are open
	if(!theApp.m_bExitApp && bPrevRunStatus) {
		if (theApp.pErrorMsgDlg != NULL)
			theApp.pErrorMsgDlg->ShowWindow(SW_HIDE);
	}
	OutputDebugLogTo(7, TRUE, "in exit instance 4.6");

}

int CGlobalBackgroundThread::Run()
{

	/*if(theApp.m_bScanningEnabled)
		return 0;*/

	if(TryEnterCriticalSection(&CriticalSectionUserCommand)) {

		if(hardwareReady) {

			if(IsRunning()) {
				if(bEnableStepMode) {
					bEnableStepMode = FALSE;
					SetEvent(EventNextStep);
				}
				theApp.Goonline();

				if(IsModuleReplacement())
					SetEvent(EventRetry);				
			}
			else {

				int nError = RunSequence();

				if(nError) {
					OutputDebugLogTo(7, TRUE, "----- ERROR IN CREATING SEQUENCE");
					RemoveSequence();
					LeaveCriticalSection(&CriticalSectionUserCommand);
					return nError;
				}
			}
		}

		LeaveCriticalSection(&CriticalSectionUserCommand);
	}

	return 0;
}

void CGlobalBackgroundThread::Stop()
{
	EnterCriticalSection(&CriticalSectionUserCommand);

	if(IsRunning()) {

		bEnableStepMode = TRUE;

		//<ERROR_LOG>
		theApp.AddLogEvent("Paused Machine Sequence");
	}

	LeaveCriticalSection(&CriticalSectionUserCommand);
}


int CGlobalBackgroundThread::Retry(BOOL bReScan)
{

	if(TryEnterCriticalSection(&CriticalSectionUserCommand)) {

		if(IsRunning()) {

			bReScanSequence = bReScan;
			SetEvent(EventRetry);
			theApp.Goonline();
			
			if (bEnableStepMode /*&& IsModuleReplacement()*/) {
				bEnableStepMode = FALSE;
				SetEvent(EventNextStep);
			}
		}
		LeaveCriticalSection(&CriticalSectionUserCommand);
	}

	return 0;
}

//void CGlobalBackgroundThread::NextStep()
//{
//	if(TryEnterCriticalSection(&CriticalSectionUserCommand)) {
//
//		if(IsRunning()) {
//			
//			SetEvent(EventNextStep);
//			
//		}
//
//		LeaveCriticalSection(&CriticalSectionUserCommand);
//	}
//}

int CGlobalBackgroundThread::ErrorHandling(CString strError, BOOL bError)
{
	
	if(killThreads)
		return 1;

	OutputDebugLogTo(7, TRUE, "-----ERROR: %s", strError);

	ResetEvent(EventRetry);

	EnterCriticalSection(&CriticalSectionErrorHandling);

	//bLevelError = TRUE;

	if(bError)
		bSequenceError = TRUE;

	LeaveCriticalSection(&CriticalSectionErrorHandling);

	if(theApp.pErrorMsgDlg != NULL)
		theApp.pErrorMsgDlg->PostModuleStatus(true, strError);

	theApp.AddLogEvent(strError);

	if(WaitForSingleObject(EventRetry, INFINITE) != WAIT_OBJECT_0) {
		OutputDebugLogTo(7, TRUE, "-----ERROR IN ERROR HANDLING FUNCTION. CLOSING SEQUENCE...");
		return -1;
	}

	if(killThreads)
		return 1;

	EnterCriticalSection(&CriticalSectionErrorHandling);

//	bLevelError = FALSE;

	if(bSequenceError)
		bSequenceError = FALSE;

	LeaveCriticalSection(&CriticalSectionErrorHandling);

	if(theApp.pErrorMsgDlg != NULL)
		theApp.pErrorMsgDlg->PostModuleStatus(false, "Running...");

	if(killThreads)
		return 1;

	return 0;
}

int CGlobalBackgroundThread::HomeMoveFunction(int nMotorID)
{
	if(killThreads)
		return 0;

	int nError = motionParameters.HomeMove(nMotorID, false);
	if(nError) {
		motionParameters.StopAxis(nMotorID);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(nMotorID, INFINITE);
	if(nError) {
		motionParameters.StopAxis(nMotorID);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.HomeDone(nMotorID);
	if(nError) {
		motionParameters.StopAxis(nMotorID);
		return nError;
	}

	return 0;
}

UINT CGlobalBackgroundThread::XServoMotor(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nHomeError[0] = pMachineSequence->HomeMoveFunction(X_SERVO_MOTOR);

	SetEvent(pMachineSequence->XHomeDone);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

UINT CGlobalBackgroundThread::ZServoMotor(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nHomeError[2] = pMachineSequence->HomeMoveFunction(ST1_Z_SERVO_MOTOR);

	SetEvent(pMachineSequence->ZHomeDone);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

int CGlobalBackgroundThread::InitXMotorFunction()
{
	if(killThreads)
		return 0;

	// X-Motor Ready Position
L1:	if (StepMode()) return -1;

	int nError = motionParameters.AbsoluteMove(X_SERVO_MOTOR, motionParameters.getMotorInitPosition(X_SERVO_MOTOR), motionParameters.getMovingSpeed(X_SERVO_MOTOR), false);

	if(nError) {
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return nError;
	}

	if (bImmediateStopActivated)
		goto L1;

	return 0;
}

int CGlobalBackgroundThread::InitYTopMotorFunction()
{
	if(killThreads)
		return 0;

	// YTop-Motor Ready Position
L1:	if (StepMode()) return -1;

	int nError = motionParameters.AbsoluteMove(Y_SERVO_MOTOR, motionParameters.getMotorInitPosition(Y_SERVO_MOTOR) , motionParameters.getMovingSpeed(Y_SERVO_MOTOR), false);

	if(nError) {
		motionParameters.StopAxis(Y_SERVO_MOTOR);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {	
		motionParameters.StopAxis(Y_SERVO_MOTOR);
		return nError;
	}
	if (bImmediateStopActivated)
		goto L1;

	return 0;
}

int CGlobalBackgroundThread::InitZMotorFunction()
{
	if(killThreads)
		return 0;

L1:	if (StepMode()) return -1;
	int nError = motionParameters.AbsoluteMove(ST1_Z_SERVO_MOTOR, motionParameters.getMotorInitPosition(ST1_Z_SERVO_MOTOR), motionParameters.getMovingSpeed(ST1_Z_SERVO_MOTOR), false);

	if(nError) {
		motionParameters.StopAxis(ST1_Z_SERVO_MOTOR);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(ST1_Z_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {	
		OutputDebugLogTo(7, TRUE, "Error in ST1-Z WaitForMotionDone Timeout = %d", nError);
		motionParameters.StopAxis(ST1_Z_SERVO_MOTOR);
		return nError;
	}
	if (bImmediateStopActivated)
		goto L1;

	return 0;
}

int CGlobalBackgroundThread::InitZMotorFunctionTHR()
{
	if (killThreads)
		return 0;

L1:	if (StepMode()) return -1;
	int nError = motionParameters.AbsoluteMove(ST2_Z_SERVO_MOTOR, motionParameters.getMotorInitPosition(ST2_Z_SERVO_MOTOR), motionParameters.getMovingSpeed(ST2_Z_SERVO_MOTOR), false);

	if (nError) {
		motionParameters.StopAxis(ST2_Z_SERVO_MOTOR);
		return nError;
	}

	if (killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(ST2_Z_SERVO_MOTOR, theApp.getMotionTimeOut());
	if (nError) {
		OutputDebugLogTo(7, TRUE, "Error in ST2-Z WaitForMotionDone Timeout = %d", nError);
		motionParameters.StopAxis(ST2_Z_SERVO_MOTOR);
		return nError;
	}
	if (bImmediateStopActivated)
		goto L1;

	return 0;
}

int CGlobalBackgroundThread::MoveXMotorToStartPosition()
{
	int nError;

	if(killThreads)
		return 0;;

L1:	if (StepMode()) return -1;
	nError = motionParameters.MoveXMotorToStartPosTopnZ(0);
	if(nError) {
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {
		OutputDebugLogTo(7, TRUE, "Error in X WaitForMotionDone Timeout = %d", nError);	
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return nError;
	}
	if (bImmediateStopActivated)
		goto L1;
	return 0;
}

int CGlobalBackgroundThread::MoveXMotorToStartPositionEncap()
{
	int nError;

	if (killThreads)
		return 0;;

L1:	if (StepMode()) return -1;
	nError = motionParameters.MoveXMotorToStartPosTopEncap(0);
	if (nError) {
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return nError;
	}

	if (killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
	if (nError) {
		OutputDebugLogTo(7, TRUE, "Error in X WaitForMotionDone Timeout = %d", nError);
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return nError;
	}
	if (bImmediateStopActivated)
		goto L1;
	return 0;
}

int CGlobalBackgroundThread::MoveYMotorToStartPosition()
{

L1:	if (StepMode()) return -1;
	int	nError = motionParameters.MoveYTopMotorTop(0);
	if(nError) {
		motionParameters.StopAxis(Y_SERVO_MOTOR);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {	
		OutputDebugLogTo(7, TRUE, "Error in Y WaitForMotionDone Timeout = %d", nError);
		motionParameters.StopAxis(Y_SERVO_MOTOR);
		return nError;
	}
	if (bImmediateStopActivated)
		goto L1;

	return 0;
}

int CGlobalBackgroundThread::MoveYMotorToStartPositionEncap()
{

L1:	if (StepMode()) return -1;
	int	nError = motionParameters.MoveYTopMotorTopEncap(0);
	if (nError) {
		motionParameters.StopAxis(Y_SERVO_MOTOR);
		return nError;
	}

	if (killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
	if (nError) {
		OutputDebugLogTo(7, TRUE, "Error in Y WaitForMotionDone Timeout = %d", nError);
		motionParameters.StopAxis(Y_SERVO_MOTOR);
		return nError;
	}
	if (bImmediateStopActivated)
		goto L1;

	return 0;
}


int CGlobalBackgroundThread::MoveZMotorToStartPosition()
{

L1:	if (StepMode()) return -1;
	int	nError = motionParameters.MoveTOPZMotorToReadyPos();
	if(nError) {
		motionParameters.StopAxis(ST1_Z_SERVO_MOTOR);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(ST1_Z_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {	
		OutputDebugLogTo(7, TRUE, "Error in Z WaitForMotionDone Timeout = %d", nError);
		motionParameters.StopAxis(ST1_Z_SERVO_MOTOR);
		return nError;
	}

	if (bImmediateStopActivated)
		goto L1;
	return 0;
}

UINT CGlobalBackgroundThread::InitXMotor(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nInitMotionError[0] = pMachineSequence->InitXMotorFunction();

	SetEvent(pMachineSequence->InitMotion[0]);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

UINT CGlobalBackgroundThread::InitYTopMotor(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nInitMotionError[1] = pMachineSequence->InitYTopMotorFunction();

	SetEvent(pMachineSequence->InitMotion[1]);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

UINT CGlobalBackgroundThread::InitZMotor(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nInitMotionError[2] = pMachineSequence->InitZMotorFunction();

	SetEvent(pMachineSequence->InitMotion[2]);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

UINT CGlobalBackgroundThread::InitZMotorTHR(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nInitMotionError[3] = pMachineSequence->InitZMotorFunctionTHR();

	SetEvent(pMachineSequence->InitMotion[3]);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

UINT CGlobalBackgroundThread::StartXMotor(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nStartMotionError[0] = pMachineSequence->MoveXMotorToStartPosition();

	SetEvent(pMachineSequence->StartMotion[0]);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

UINT CGlobalBackgroundThread::StartXMotorEncap(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nStartMotionError[0] = pMachineSequence->MoveXMotorToStartPositionEncap();

	SetEvent(pMachineSequence->StartMotion[0]);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

UINT CGlobalBackgroundThread::StartYTopMotor(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nStartMotionError[1] = pMachineSequence->MoveYMotorToStartPosition();

	SetEvent(pMachineSequence->StartMotion[1]);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

UINT CGlobalBackgroundThread::StartYTopMotorEncap(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nStartMotionError[1] = pMachineSequence->MoveYMotorToStartPositionEncap();

	SetEvent(pMachineSequence->StartMotion[1]);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

UINT CGlobalBackgroundThread::StartXMotorTHR(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nStartMotionErrorTHR[0] = pMachineSequence->MoveXMotorToStartPositionTHR();

	SetEvent(pMachineSequence->StartMotionTHR[0]);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}

UINT CGlobalBackgroundThread::StartYTopMotorTHR(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->nStartMotionErrorTHR[1] = pMachineSequence->MoveYMotorToStartPositionTHR();

	SetEvent(pMachineSequence->StartMotionTHR[1]);

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}


UINT CGlobalBackgroundThread::ScanYTopMotorTHR(LPVOID pParam)
{
	CGlobalBackgroundThread * pMachineSequence = reinterpret_cast<CGlobalBackgroundThread *>(pParam);

	InterlockedIncrement(&pMachineSequence->temporaryThreadCount);

	pMachineSequence->ScanYTopMotorFunctionTHR();

	InterlockedDecrement(&pMachineSequence->temporaryThreadCount);

	return 0;
}


int CGlobalBackgroundThread::MoveXMotorToStartPositionTHR()
{
	int nError;

	if(killThreads)
		return 0;

L1:	if (StepMode()) return -1;

	int nScanRowNoTHR = motionParameters.getScanRowNoTHR();
	nError = motionParameters.MoveXMotorToStartPosTHR(nScanRowNoTHR - 1);
	if(nError) {
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {
		OutputDebugLogTo(7, TRUE, "Error in X WaitForMotionDone Timeout = %d", nError);
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return nError;
	}
	if (bImmediateStopActivated)
		goto L1;

	return 0;
}

int CGlobalBackgroundThread::MoveYMotorToStartPositionTHR()
{
	int nError;

	if(killThreads)
		return 0;

L1:	if (StepMode()) return -1;

	int nScanRowNoTHR = motionParameters.getScanRowNoTHR();
	nError = motionParameters.MoveYTopMotorTHR(nScanRowNoTHR - 1);
	if(nError) {
		motionParameters.StopAxis(Y_SERVO_MOTOR);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {
		OutputDebugLogTo(7, TRUE, "Error in X WaitForMotionDone Timeout = %d", nError);
		motionParameters.StopAxis(Y_SERVO_MOTOR);
		return nError;
	}
	if (bImmediateStopActivated)
		goto L1;

	return 0;
}

int CGlobalBackgroundThread::MoveZMotorToStartPositionTHR()
{
	int nError;

	if(killThreads)
		return 0;

L1:	if (StepMode()) return -1;

	nError = motionParameters.MoveZMotorToReadyPosTHR();
	if(nError) {
		motionParameters.StopAxis(ST2_Z_SERVO_MOTOR);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(ST2_Z_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {
		OutputDebugLogTo(7, TRUE, "Error in X WaitForMotionDone Timeout = %d", nError);
		motionParameters.StopAxis(ST2_Z_SERVO_MOTOR);
		return nError;
	}
	if (bImmediateStopActivated)
		goto L1;
	return 0;
}

void CGlobalBackgroundThread::ScanYTopMotorFunctionTHR()
{
	int nTrackID2DStation = -1;
	int nWaitingTrackNo = 0;

	while(1) {

		nScanYAxisErrorTHR = 0;


		if(WaitForSingleObject(MoveYAxisScanTHR, INFINITE) != WAIT_OBJECT_0) {
			OutputDebugLogTo(7, TRUE, "-----ERROR IN SEQUENCE SYNCHRONIZATION");

			OutputDebugLogTo(0, TRUE, "Motion Time Out. Camera grabs are missing");
			nScanYAxisErrorTHR = -1;
			goto StopThread;
		}

		if(killThreads || !bScanModeTHR)
			goto StopThread;

		ResetEvent(MoveYAxisScanTHR);
		ResetEvent(MoveYAxisScanDoneTHR);

		if(!bEmptyScanTHR) {

			DWORD dwError = WaitForSingleObject( theApp.m_pTracks[STATION_THR].m_eventTrigCount, theApp.getMotionTimeOut());

			if(dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {
				OutputDebugLogTo(7, TRUE, "-----ERROR: TimeOut for image grabs");
				nScanYAxisErrorTHR = -100;
				SetEvent(MoveYAxisScanDoneTHR);
				continue;
			}

			if(killThreads)
				goto StopThread;
		}

		OutputDebugLogTo(6, TRUE, "ScanYTopMotorFunctionTHR: nMotorYAxisScanPosTHR %d", nMotorYAxisScanPosTHR);
		if(nMotorYAxisScanPosTHR > 0) {

			if(nMotorYAxisScanPosTHR >= motionParameters.getScanRowNoTHR()) {
				motionParameters.StopAxis(Y_SERVO_MOTOR);
				nScanYAxisErrorTHR = -1;
				goto StopThread;
			}

			int nError = motionParameters.MoveYTopMotorTHR(nMotorYAxisScanPosTHR - 1);

			if(nError) {
				motionParameters.StopAxis(Y_SERVO_MOTOR);
				nScanYAxisErrorTHR = -1;
				goto StopThread;
			}

			if(killThreads)
				goto StopThread;

			nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
			if(nError) {
				motionParameters.StopAxis(Y_SERVO_MOTOR);
				nScanYAxisErrorTHR = -1;
				goto StopThread;
			}

			if(killThreads)
				goto StopThread;

			motionParameters.WaitMotorSettlingTime(Y_SERVO_MOTOR);

		}


		SetEvent(MoveYAxisScanDoneTHR);
	}

StopThread:
	SetEvent(MoveYAxisScanDoneTHR);

}

int CGlobalBackgroundThread::ScanXMotorFunctionTHR(int nPos)
{
	if(killThreads)
		return 0;

	if(nPos < 0)
		return -1;

	int nError = motionParameters.MoveXMotorToEndPosTHR(nPos);

	if(nError) {
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return nError;
	}

	if(killThreads)
		return 0;

	nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return nError;
	}

	return 0;
}

int CGlobalBackgroundThread::saveEndLotInformation()
{
	// Update Lot End Time
	theApp.m_pImageSelectionDlg->setLotEndTime();

	// Save Lot File
	// Save Lot summary
	CString filePath;
	filePath.Format("%s\\Summary\\%s\\%s\\", theApp.m_LotSummaryRootDir, theApp.m_strConfigFile, theApp.m_pImageSelectionDlg->lotInfo().lotId.c_str());
	CreateDirectoryTree(filePath);
	theApp.m_pImageSelectionDlg->saveLotSummaryFile(filePath.GetString());
	theApp.m_pImageSelectionDlg->savePrintBarSummaryLogFile();
	theApp.m_pImageSelectionDlg->SaveMappingData();
	theApp.m_pImageSelectionDlg->SavePartSummary(true);

	return 0;
}
BOOL CGlobalBackgroundThread::getEnableSaveImageLimitStatus() const
{
	return _enableSaveImageLimit;
}
int CGlobalBackgroundThread::getSaveImageLimitPercentage() const
{
	return _imageSaveLimitPercentage;
}
void CGlobalBackgroundThread::setSaveImageLimitParameters(BOOL enable, int percentage)
{
	_enableSaveImageLimit = enable;
	_imageSaveLimitPercentage = percentage;
}
int CGlobalBackgroundThread::saveSequenceOptionParameters() const
{
	
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Image Saving Limit Parameter", _imageSaveLimitPercentage);
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Image Saving Limit Parameter check", _enableSaveImageLimit);

	return 0;
}
int CGlobalBackgroundThread::loadSequenceOptionParameters()
{
	_imageSaveLimitPercentage = GetRegistryEntry(READ_REGPATH + "Preferences", "Image Saving Limit Parameter", 20);
	_enableSaveImageLimit = GetRegistryEntry(READ_REGPATH + "Preferences", "Image Saving Limit Parameter check", false);

	return 0;
}

bool CGlobalBackgroundThread::IsCurtainMuteActivated()
{
	return bCurtainMuteActivated;
}
