#include "stdafx.h"
#include <direct.h>
#include "SequenceThread.h"
#include "Application.h"
#include "DebugLog.h"
#include "TrackHandler.h"
#include "ImageSelectionDlg.h"
#include "AppDoc.h"
#include "CameraCode.h"
#include "GlobalBackgroundThread.h"
#include "MathValidations.h"
//#include "ExternalFunctions.h"
#include "BitmapFunctions.h"
#include <numeric>
#include <queue>
#include "namespace_utilities_function.h"
#include "CalibrationSettings.h"
#include "PVIInspectionParameters.h"


UINT ScanThread(LPVOID)
{
//	try {
//
//		CApplication* pApp = (CApplication*) AfxGetApp();
//		MotionParameters & motionParameters = theApp.motionParameters;
//		IOControl & ioControl = theApp.ioControl;
//		int nError;
//		int nScanColumnsTHR;
//		int nScanRowsTHR;
//
//		int nScanColumnsTop;
//		int nScanRowsTop;
//
//		int nScanColumnsZ;
//		int nScanRowsZ;
//
//		while(1) {
//
//			theApp.m_bScanning = FALSE;
//
//			if(theApp.m_bScanningEnabled)
//				SetEvent(theApp.m_eventStartScan);
//
//			if(WaitForSingleObject(theApp.m_eventStartScan, INFINITE) != WAIT_OBJECT_0) {
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Thread manipulation, Scan Thread has been closed");
//				break;
//			}
//
//			if(theApp.m_bExitApp) break;
//
//			theApp.m_bScanning = TRUE;
//
//			OutputDebugLogTo(8, TRUE, "----- Step 1, Z sequence set");
//
//			if(motionParameters.CreateScanPositions()) {
//				theApp.m_bScanning = FALSE;
//			
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Failed to create scanning position");
//				continue;
//			}
//
//			OutputDebugLogTo(8, TRUE, "----- Step 2, Create scan positions set");
//
//			// 3. Create arrays for scanning
//			int nTotalImageNoTHR = motionParameters.GetTotalImageGrabsTHR();
//			int nTotalImageNoTOP = motionParameters.GetTotalImageGrabsTOP();
//
//			if(theApp.m_pTracks[STATION_THR].nTotalGrabNo != nTotalImageNoTHR) {
//				theApp.m_pTracks[STATION_THR].nTotalGrabNo = nTotalImageNoTHR;
//				theApp.m_pTracks[STATION_THR].imageGrab.clear();
//				theApp.m_pTracks[STATION_THR].imageGrab.resize(nTotalImageNoTHR);
//
//				CSize csImageFOV = theApp.m_pTracks[STATION_THR].m_pDoc[0]->m_Buffer.GetImgSize();
//
//				for(size_t j = 0; j < theApp.m_pTracks[STATION_THR].imageGrab.size(); j++)
//					theApp.m_pTracks[STATION_THR].imageGrab[j].Allocate(csImageFOV);
//			}
//
//
//			if(theApp.m_pTracks[STATION_TOP].nTotalGrabNo != nTotalImageNoTOP) {
//				theApp.m_pTracks[STATION_TOP].nTotalGrabNo = nTotalImageNoTOP;
//				theApp.m_pTracks[STATION_TOP].imageGrab.clear();
//				theApp.m_pTracks[STATION_TOP].imageGrab.resize(nTotalImageNoTOP);
//
//				CSize csImageFOV = theApp.m_pTracks[STATION_TOP].m_pDoc[0]->m_Buffer.GetImgSize();
//
//				for(size_t j = 0; j < theApp.m_pTracks[STATION_TOP].imageGrab.size(); j++)
//					theApp.m_pTracks[STATION_TOP].imageGrab[j].Allocate(csImageFOV);
//			}
//
//			//Biasing Sequence
//
//			if(!motionParameters.IsMotorHomed(ST1_Z_SERVO_MOTOR)) {
//
//				nError = motionParameters.HomeMove(ST1_Z_SERVO_MOTOR, false);
//				if(nError) {
//					OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Bond Z motor failed");
//					theApp.m_bScanning = FALSE;
//					for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//						ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//					}
//					theApp.m_bScanningEnabled = FALSE;
//					theApp.m_bScanningCompleted = TRUE;
//					continue;
//				}
//			}
//			else {
//				nError = motionParameters.AbsoluteMove(ST1_Z_SERVO_MOTOR, 0, motionParameters.getMovingSpeed(ST1_Z_SERVO_MOTOR), false);
//				if(nError) {
//					OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Bond Z motor failed");
//					theApp.m_bScanning = FALSE;
//					for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//						ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//					}
//					theApp.m_bScanningEnabled = FALSE;
//					theApp.m_bScanningCompleted = TRUE;
//					continue;
//				}
//			}
//		
//			if(!motionParameters.IsMotorHomed(X_SERVO_MOTOR)) {
//
//				nError = motionParameters.HomeMove(X_SERVO_MOTOR);
//				if(nError) {
//
//					theApp.m_bScanning = FALSE;
//					for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//						ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//					}
//					theApp.m_bScanningEnabled = FALSE;
//					theApp.m_bScanningCompleted = TRUE;
//					OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for X motor failed");
//					continue;
//				}
//			}
//			
//			if(!motionParameters.IsMotorHomed(Y_SERVO_MOTOR)) {
//
//				nError = motionParameters.HomeMove(Y_SERVO_MOTOR, false);
//				if(nError) {
//
//					theApp.m_bScanning = FALSE;
//					for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//						ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//					}
//					theApp.m_bScanningEnabled = FALSE;
//					theApp.m_bScanningCompleted = TRUE;
//					OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Y motor failed");
//					continue;
//				}
//
//			}
//
//
//			if(!motionParameters.IsMotorHomed(X_SERVO_MOTOR)) {
//
//				nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, INFINITE);
//				if(nError) {
//
//					theApp.m_bScanning = FALSE;
//					for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//						ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//					}
//					theApp.m_bScanningEnabled = FALSE;
//					theApp.m_bScanningCompleted = TRUE;
//					OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for X motor failed");
//					continue;
//				}
//
//				if(theApp.m_bExitApp) break;
//
//				nError = motionParameters.HomeDone(X_SERVO_MOTOR);
//				if(nError) {
//
//					theApp.m_bScanning = FALSE;
//					for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//						ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//					}
//					theApp.m_bScanningEnabled = FALSE;
//					theApp.m_bScanningCompleted = TRUE;
//					OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for X motor failed");
//					continue;
//				}
//
//			}
//
//
//			if(theApp.m_bExitApp) break;
//
//			if(!motionParameters.IsMotorHomed(Y_SERVO_MOTOR)) {
//
//				nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, INFINITE);
//				if(nError) {
//
//					theApp.m_bScanning = FALSE;
//					for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//						ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//					}
//					theApp.m_bScanningEnabled = FALSE;
//					theApp.m_bScanningCompleted = TRUE;
//					OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Y motor failed");
//					continue;
//				}
//
//				if(theApp.m_bExitApp) break;
//
//				nError = motionParameters.HomeDone(Y_SERVO_MOTOR);
//				if(nError) {
//
//					theApp.m_bScanning = FALSE;
//					for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//						ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//					}
//					theApp.m_bScanningEnabled = FALSE;
//					theApp.m_bScanningCompleted = TRUE;
//					OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Y motor failed");
//					continue;
//				}
//
//			}
//
//			nError = motionParameters.WaitForMotionDone(ST1_Z_SERVO_MOTOR, INFINITE);
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Bond Z motor failed");
//				continue;
//			}
//
//
//			if(theApp.m_bExitApp) break;
//
//			//Display the count of triggers per row
//			nScanColumnsTHR = motionParameters.getScanColumnNoTHR();
//			nScanRowsTHR = motionParameters.getScanRowNoTHR();
//
//			nScanColumnsZ = motionParameters.getScanColumnNoZSensor();
//			nScanRowsZ = motionParameters.getScanRowNoZSensor();
//
//			nScanColumnsTop = motionParameters.getScanColumnNoTop();;
//			nScanRowsTop = motionParameters.getScanRowNoTop();
//
//
//			int nFovNoPerY;
//			int nFovNoPerX;
//
//			int nDieNoPerX;
//			int nDIeNoPerY;
//			
//			motionParameters.GetMappingParameters(nDieNoPerX, nDIeNoPerY, nFovNoPerX, nFovNoPerY);
//
//		
//			for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//				theApp.m_pTracks[j].CancelSnap();
//			}
//
//			
//			nError = motionParameters.MoveXMotorToStartPosTopnZ(0);
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for X motor failed");
//				continue;
//			}
//
//			OutputDebugLogTo(8, TRUE, "----- Step 6, move Y to start pos");
//
//			nError = motionParameters.MoveYTopMotorTop(0);					// Y starts from row 1 for Z and Top Sequence
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for Y motor failed");
//				continue;
//			}
//
//			OutputDebugLogTo(8, TRUE, "----- Step 7, move z to start pos");
//
//			nError = motionParameters.MoveZMotorToReadyPos();
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for Z-Focusing motor failed");
//				continue;
//			}
//
//			if(theApp.m_bExitApp) break;
//
//
//			OutputDebugLogTo(8, TRUE, "----- Step 8, mwait for x");
//
//			nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, INFINITE);
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for X motor failed");
//				continue;
//			}
//
//			OutputDebugLogTo(8, TRUE, "----- Step 8, mwait for y");
//
//
//			if(theApp.m_bExitApp) break;
//
//			nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, INFINITE);
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for Top Y motor failed");
//				continue;
//			}
//
//			OutputDebugLogTo(8, TRUE, "----- Step 8, mwait for z");
//
//			if(theApp.m_bExitApp) break;
//
//			nError = motionParameters.WaitForMotionDone(ST1_Z_SERVO_MOTOR, INFINITE);
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for 23D-Z focusing motor failed");
//				continue;
//			}
//
//			if(theApp.m_bExitApp) break;
//			OutputDebugLogTo(8, TRUE, "----- Step 9,  wait for Y settling time");
//
//			motionParameters.WaitMotorSettlingTime(Y_SERVO_MOTOR);
//
//
//
//			InterlockedExchange(&theApp.nZSensorPositionId, 0);
//
//			theApp.nZSensorPositionCount = 2 * nScanColumnsZ;
//	//		OutputDebugLogTo(6, TRUE, "nZSensorPositionCount=  %d", theApp.nZSensorPositionCount);
//
//			for(int i = 0; i<theApp.m_nTotalTracks; i++) {
//				theApp.m_pTracks[i].CancelSnap();
//			}
//
//
//			// 9. Reset camera grab count
//			for(int i = 0; i<theApp.m_nTotalTracks; i++) {
//				InterlockedExchange(&theApp.m_pTracks[i].nCameraGrabNo, -1);	
//			}
//			
//			
//
//			theApp.ioControl.IonizerOn(TRUE);
//			theApp.ioControl.IonizerDistStop(TRUE);
//
//			//TOP and Z sensor
//			for(int i = 0; i < nScanRowsZ; i++) {
//
//
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					theApp.m_pTracks[j].CancelSnap();
//				}
//
//				motionParameters.SetCompareTriggerTopnZ(i);
//				motionParameters.ResetTriggerCount(0);
//
//			
//				SetTrigMode(&theApp.m_pTracks[STATION_TOP].m_FGResource, TRIG_HARD, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_TOP].m_nTrack]);
//				EnableOutputLine(&theApp.m_pTracks[STATION_THR].m_FGResource, false);
//
//				ResetEvent(theApp.m_eventZSensorTrigCount);
//				SetEvent(theApp.eventRunZSensorSequence);
//		
//				ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventTrigCount);			
//				SetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);	
//
//				OutputDebugLogTo(8, TRUE, "----- Step 9,  move x motor to end pos %d", i* nFovNoPerY);
//
//				nError = motionParameters.MoveXMotorToEndPosTopnZ(i);
//				if(nError) {
//					theApp.m_bScanningCompleted = TRUE;
//					OutputDebugLogTo(0, TRUE, "----- ERROR : X motor failed to start movement");
//				}
//
//				OutputDebugLogTo(8, TRUE, "----- Step 10,  waiting for Top trigger count");
//
//
//				int nError = 0;
//
//
//				//Wait for Grab and Z data.
//				std::vector < HANDLE > eventWait;
//
//				eventWait.push_back(theApp.m_eventZSensorTrigCount);
//				eventWait.push_back(theApp.m_pTracks[STATION_TOP].m_eventTrigCount);
//				//
//
//				nError = WaitForMultipleObjects(eventWait.size(), eventWait.data(),TRUE,  theApp.getMotionTimeOut() * 10);
//
//				
//				OutputDebugLogTo(8, TRUE, "----- Step 11,  waiting for Top trigger count done");
//
//				if(nError == WAIT_FAILED || nError == WAIT_TIMEOUT) {
//
//					theApp.ioControl.IonizerOn(FALSE);
//					theApp.ioControl.IonizerDistStop(FALSE);
//
//					if(theApp.m_bExitApp) break;
//
//					if(!theApp.m_bScanningEnabled) break;
//
//					CString strErrorMessage;
//					//CString strCameraErrorMessage;
//
//					if(AfxMessageBox("MotionTime Out. Grabs missing. Do you want to continue?", MB_YESNO | MB_ICONSTOP) == IDNO) break;
//
//				
//					ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
//					
//					SetTrigMode(&theApp.m_pTracks[STATION_TOP].m_FGResource, TRIG_SOFT, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_TOP].m_nTrack]);
//					EnableOutputLine(&theApp.m_pTracks[STATION_TOP].m_FGResource, true);
//					EnableOutputLine(&theApp.m_pTracks[STATION_THR].m_FGResource, true);
//
//
//					motionParameters.StopAxis(X_SERVO_MOTOR);
//
//					nError = motionParameters.MoveXMotorToStartPosTop(i);
//					if(nError) {
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : X motor failed to start movement");
//					}
//
//					nError = motionParameters.MoveYTopMotorTop(i);
//					if(nError) {
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : Y motor failed to start movement");
//					}
//
//					if(theApp.m_bExitApp) break;
//
//					nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
//					if(nError) {
//						theApp.m_bScanning = FALSE;
//
//						ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
//
//						theApp.m_bScanningEnabled = FALSE;
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for X motor failed");
//						continue;
//					}
//
//					nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
//					if(nError) {
//						theApp.m_bScanning = FALSE;
//						ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
//						theApp.m_bScanningEnabled = FALSE;
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for Top Y motor failed");
//						continue;
//					}
//
//					if(theApp.m_bExitApp) break;
//
//					motionParameters.WaitMotorSettlingTime(Y_SERVO_MOTOR);
//
//					i--;
//
//					//for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					//	if(theApp.m_StationHandler[j].getRealId() != STATION_BOND)
//					//		InterlockedExchange(&theApp.m_StationHandler[j].nCameraGrabNo, i*nScanColumns - 1);
//					//}
//
//					/*std::fill(theApp.cleanPreviousInspection23DResults.begin(),
//						theApp.cleanPreviousInspection23DResults.end(), true);
//					theApp.bServerSlaveCleanPreviousInspection23DResults = true;*/
//
//					//Set Light trigger steps
//					//nError = theApp.m_LightParameters.setTriggerMode(LIGHT_2D_3D);
//					//if(nError) {
//					//	theApp.m_bScanningCompleted = TRUE;
//					//	OutputDebugLogTo(7, TRUE, "-----ERROR: Cannot set trigger mode to 0");
//					//}
//
//					//nError = theApp.m_LightParameters.setSequenceSteps(LIGHT_2D_3D, motionParameters.getOneWayDirection23DScanning() ? false : !bForwardMovement);
//					//if(nError) {
//					//	//theApp.m_bScanningCompleted = TRUE;
//					//	OutputDebugLogTo(0, TRUE, "----- ERROR: Reverse Step setup is failed. Please check light condition");
//					//}
//
//					continue;
//				}
//
//				//OutputDebugLogTo(8, TRUE, "----- Step 10,  waiting for Top trigger count done");
//
//				ResetEvent(theApp.m_pTracks[STATION_TOP].m_eventGrabbing);
//
//
//				OutputDebugLogTo(8, TRUE, "----- Step 10,  waiting for Z trigger count done");
//
//				if(i < nScanRowsZ - 1) {
//
//
//					OutputDebugLogTo(8, TRUE, "----- Step 10,  move y to next row %d", (i + 1));
//
//					
//					nError = motionParameters.MoveYTopMotorTop(i + 1);
//					if(nError) {
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : Y motor failed to start movement");
//					}
//
//					OutputDebugLogTo(8, TRUE, "----- Step 11,  wait for y");
//
//
//					nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, INFINITE);
//					if(nError) {
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : Y motor waiting for motion done is failed");
//					}
//
//					if(theApp.m_bExitApp) break;
//
//					motionParameters.WaitMotorSettlingTime(Y_SERVO_MOTOR);
//
//				}
//
//				if(theApp.m_bExitApp) break;
//
//				/*for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//						ResetEvent(theApp.m_pTracks[j].m_eventG);
//				}*/
//
//				OutputDebugLogTo(8, TRUE, "----- Step 11,  wait for x");
//
//
//				nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
//				if(nError) {
//					theApp.m_bScanningCompleted = TRUE;
//					OutputDebugLogTo(0, TRUE, "----- ERROR : X motor waiting for motion done is failed");
//				}
//
//				OutputDebugLogTo(8, TRUE, "----- Step 11,  wait for x DOne");
//
//			}
//
//
//			theApp.ioControl.IonizerOn(FALSE);
//			theApp.ioControl.IonizerDistStop(FALSE);
//
//
//			for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//				theApp.m_pTracks[j].CancelSnap();
//			}
//
//			SetTrigMode(&theApp.m_pTracks[STATION_TOP].m_FGResource, TRIG_SOFT, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_TOP].m_nTrack]);
//			EnableOutputLine(&theApp.m_pTracks[STATION_TOP].m_FGResource, true);
//			EnableOutputLine(&theApp.m_pTracks[STATION_THR].m_FGResource, true);
//			/*long zCount = 0;
//
//			if((zCount = InterlockedExchangeAdd(&theApp.nZSensorPositionId, 0)) != 2 * nScanColumnsZ) {
//				CString strErrorMessage;
//
//				strErrorMessage.Format("----- ERROR : Z Scan count is %d instead of %d", zCount, 2 * nScanColumnsZ);
//
//				AfxMessageBox(strErrorMessage);
//			}*/
//
//			long latch = 0;
//
//			/*if((latch = InterlockedExchangeAdd(&theApp.nLatchedPositionId, 0)) != nScanRowsTop * nScanColumnsTop) {
//				CString strErrorMessage;
//
//				strErrorMessage.Format("----- ERROR : latch value count is %d instead of %d", latch, nScanRowsTop * nScanColumnsTop);
//
//				AfxMessageBox(strErrorMessage);
//			}*/
//
//			// ****** THR *******
//
//			motionParameters.CreateNewPositions();
//
//
//			nError = motionParameters.MoveXMotorToStartPosTHR(0);
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for X motor failed");
//				continue;
//			}
//
//			OutputDebugLogTo(8, TRUE, "----- Step 6, move Y to start pos");
//
//			nError = motionParameters.MoveYTopMotorTHR(0);					// Y starts from row 1 for Z and Top Sequence
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for Y motor failed");
//				continue;
//			}
//
//			OutputDebugLogTo(8, TRUE, "----- Step 7, move z to start pos");
//
//			nError = motionParameters.MoveZMotorToReadyPosTHR();
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for Z-Focusing motor failed");
//				continue;
//			}
//
//			if(theApp.m_bExitApp) break;
//
//
//			OutputDebugLogTo(8, TRUE, "----- Step 8, mwait for x");
//
//			nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, INFINITE);
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for X motor failed");
//				continue;
//			}
//
//			OutputDebugLogTo(8, TRUE, "----- Step 8, mwait for y");
//
//
//			if(theApp.m_bExitApp) break;
//
//			nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, INFINITE);
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for Top Y motor failed");
//				continue;
//			}
//
//			OutputDebugLogTo(8, TRUE, "----- Step 8, mwait for z");
//
//			if(theApp.m_bExitApp) break;
//
//			nError = motionParameters.WaitForMotionDone(ST1_Z_SERVO_MOTOR, INFINITE);
//			if(nError) {
//				theApp.m_bScanning = FALSE;
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for 23D-Z focusing motor failed");
//				continue;
//			}
//
//			if(theApp.m_bExitApp) break;
//			OutputDebugLogTo(8, TRUE, "----- Step 9,  wait for Y settling time");
//
//			motionParameters.WaitMotorSettlingTime(Y_SERVO_MOTOR);
//
//			for(int i = 0; i<theApp.m_nTotalTracks; i++) {
//				theApp.m_pTracks[i].CancelSnap();
//			}
//
//			//-- Reset values for latching
//			InterlockedExchange(&theApp.nLatchedPositionId, 0);
//
//			theApp.nLatchedPositionCount = nScanRowsTHR * nScanColumnsTHR;
//
//			theApp.latchedPositionX.resize(theApp.nLatchedPositionCount);
//			theApp.latchedPositionY.resize(theApp.nLatchedPositionCount);
//
//
//			//10. Display statistic window
//
//
//			//11. Set Light trigger steps				//Lohith will add
//			//Trigger mode,
//			//set sequence steps
////			theApp.m_pTracks[STATION_THR].RefreshLight(0);
//
//
//			OutputDebugLogTo(8, TRUE, "----- Create Z positions done");
//
//			//THR
//			for(int i = 0; i < nScanRowsTHR; i++) {
//
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					theApp.m_pTracks[j].CancelSnap();
//				}
//
//				motionParameters.ResetTriggerCount(0);
//				motionParameters.SetCompareTriggerTHR(i);
//
//				
//
//				SetTrigMode(&theApp.m_pTracks[STATION_THR].m_FGResource, TRIG_HARD, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_THR].m_nTrack]);
//				EnableOutputLine(&theApp.m_pTracks[STATION_TOP].m_FGResource, false);
////				EnableOutputLine(&theApp.m_pTracks[STATION_THR].m_FGResource, false);
//
//				ResetEvent(theApp.m_pTracks[STATION_THR].m_eventTrigCount);
//				SetEvent(theApp.m_pTracks[STATION_THR].m_eventGrabbing);
//
//
//				OutputDebugLogTo(8, TRUE, "----- Step 9,  move x motor to end pos %d", i* nFovNoPerY);
//
//				nError = motionParameters.MoveXMotorToEndPosTHR(i);
//				if(nError) {
//					theApp.m_bScanningCompleted = TRUE;
//					OutputDebugLogTo(0, TRUE, "----- ERROR : X motor failed to start movement");
//				}
//
//				OutputDebugLogTo(8, TRUE, "----- Step 10,  waiting for Z trigger count");
//
//				nError = WaitForSingleObject(theApp.m_pTracks[STATION_THR].m_eventTrigCount, theApp.getMotionTimeOut() * 10);
//
//				if(nError == WAIT_FAILED || nError == WAIT_TIMEOUT) {
//
//					if(theApp.m_bExitApp) break;
//
//					if(!theApp.m_bScanningEnabled) break;
//
//					CString strErrorMessage;
//					//CString strCameraErrorMessage;
//
//					if(AfxMessageBox("MotionTime Out. Grabs missing. Do you want to continue?", MB_YESNO | MB_ICONSTOP) == IDNO) break;
//
//
//					ResetEvent(theApp.m_pTracks[STATION_THR].m_eventGrabbing);
//
//					SetTrigMode(&theApp.m_pTracks[STATION_THR].m_FGResource, TRIG_SOFT, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_THR].m_nTrack]);
//					EnableOutputLine(&theApp.m_pTracks[STATION_TOP].m_FGResource, true);
//					EnableOutputLine(&theApp.m_pTracks[STATION_THR].m_FGResource, true);
//
//					motionParameters.StopAxis(X_SERVO_MOTOR);
//
//					nError = motionParameters.MoveXMotorToStartPosTHR(i);
//					if(nError) {
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : X motor failed to start movement");
//					}
//
//					nError = motionParameters.MoveYTopMotorTHR(i);
//					if(nError) {
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : Y motor failed to start movement");
//					}
//
//					if(theApp.m_bExitApp) break;
//
//					nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
//					if(nError) {
//						theApp.m_bScanning = FALSE;
//
//						ResetEvent(theApp.m_pTracks[STATION_THR].m_eventGrabbing);
//
//						theApp.m_bScanningEnabled = FALSE;
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for X motor failed");
//						continue;
//					}
//
//					nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
//					if(nError) {
//						theApp.m_bScanning = FALSE;
//						ResetEvent(theApp.m_pTracks[STATION_THR].m_eventGrabbing);
//						theApp.m_bScanningEnabled = FALSE;
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : Absolute move for Top Y motor failed");
//						continue;
//					}
//
//					if(theApp.m_bExitApp) break;
//
//					motionParameters.WaitMotorSettlingTime(Y_SERVO_MOTOR);
//
//					i--;
//
//					//for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//					//	if(theApp.m_StationHandler[j].getRealId() != STATION_BOND)
//					//		InterlockedExchange(&theApp.m_StationHandler[j].nCameraGrabNo, i*nScanColumns - 1);
//					//}
//
//					/*std::fill(theApp.cleanPreviousInspection23DResults.begin(),
//					theApp.cleanPreviousInspection23DResults.end(), true);
//					theApp.bServerSlaveCleanPreviousInspection23DResults = true;*/
//
//					//Set Light trigger steps
//					//nError = theApp.m_LightParameters.setTriggerMode(LIGHT_2D_3D);
//					//if(nError) {
//					//	theApp.m_bScanningCompleted = TRUE;
//					//	OutputDebugLogTo(7, TRUE, "-----ERROR: Cannot set trigger mode to 0");
//					//}
//
//					//nError = theApp.m_LightParameters.setSequenceSteps(LIGHT_2D_3D, motionParameters.getOneWayDirection23DScanning() ? false : !bForwardMovement);
//					//if(nError) {
//					//	//theApp.m_bScanningCompleted = TRUE;
//					//	OutputDebugLogTo(0, TRUE, "----- ERROR: Reverse Step setup is failed. Please check light condition");
//					//}
//
//					continue;
//				}
//
//				ResetEvent(theApp.m_pTracks[STATION_THR].m_eventGrabbing);
//
//
//				OutputDebugLogTo(8, TRUE, "----- Step 10,  waiting for Z trigger count done");
//
//				if(i < nScanRowsTHR - 1) {
//
//
//					OutputDebugLogTo(8, TRUE, "----- Step 10,  move y to next row %d", i);
//
//
//					nError = motionParameters.MoveYTopMotorTHR(i+1);
//					if(nError) {
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : Y motor failed to start movement");
//					}
//
//					OutputDebugLogTo(8, TRUE, "----- Step 11,  wait for y");
//
//
//					nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, INFINITE);
//					if(nError) {
//						theApp.m_bScanningCompleted = TRUE;
//						OutputDebugLogTo(0, TRUE, "----- ERROR : Y motor waiting for motion done is failed");
//					}
//
//					if(theApp.m_bExitApp) break;
//
//					motionParameters.WaitMotorSettlingTime(Y_SERVO_MOTOR);
//
//				}
//
//				if(theApp.m_bExitApp) break;
//
//				/*for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//				ResetEvent(theApp.m_pTracks[j].m_eventG);
//				}*/
//
//				OutputDebugLogTo(8, TRUE, "----- Step 11,  wait for x");
//
//
//				nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
//				if(nError) {
//					theApp.m_bScanningCompleted = TRUE;
//					OutputDebugLogTo(0, TRUE, "----- ERROR : X motor waiting for motion done is failed");
//				}
//
//				OutputDebugLogTo(8, TRUE, "----- Step 11,  wait for x DOne");
//
//			}
//
//			EnableOutputLine(&theApp.m_pTracks[STATION_TOP].m_FGResource, true);
//			EnableOutputLine(&theApp.m_pTracks[STATION_THR].m_FGResource, true);
//			SetTrigMode(&theApp.m_pTracks[STATION_TOP].m_FGResource, TRIG_SOFT, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_THR].m_nTrack]);
//			SetTrigMode(&theApp.m_pTracks[STATION_THR].m_FGResource, TRIG_SOFT, theApp.m_nCameraTrigDelay[theApp.m_pTracks[STATION_THR].m_nTrack]);
//
//	//		OutputDebugLogTo(8, TRUE, "Scanning time %.1f ms", timer_scan.GetElapsedMSec());
//
//
//			//Wait for inspection.
//			std::vector < HANDLE > eventInspection;
//
//			for (int i = 0; i < theApp.m_nTotalTracks; i++) {
//				for (int nThreadId = 0; nThreadId < theApp.m_pTracks[i].getInspectionThreadCount(); nThreadId++) {
//					eventInspection.push_back(theApp.m_pTracks[i].eventInspectionDone[nThreadId]);
//				}
//			}
//
//			/*for(int i = 0; i < theApp.m_nTotalTracks; i++) {
//				eventInspection.push_back(theApp.m_pTracks[i].eventInspectionDone);
//			}*/
//			
//			OutputDebugLogTo(8, TRUE, "----- Step 11,  waiting for inspection");
//
//			nError = WaitForMultipleObjects(eventInspection.size(), eventInspection.data(), TRUE, INFINITE);
//
//			OutputDebugLogTo(8, TRUE, "----- Step 11,  waiting for inspection Done");
//
//			if(nError == WAIT_FAILED || nError == WAIT_TIMEOUT) {
//				for(int j = 0; j<theApp.m_nTotalTracks; j++) {
//						ResetEvent(theApp.m_pTracks[j].m_eventGrabbing);
//				}
//				theApp.m_bScanningEnabled = FALSE;
//				theApp.m_bScanningCompleted = TRUE;
//				OutputDebugLogTo(0, TRUE, "-----ERROR: Image Inspection threads have been failed.");
//
//				continue;
//			}
//
//			OutputDebugLogTo(8, TRUE, "----- Step 11,  Loop completed");
//
////			OutputDebugLogTo(0, FALSE, "Inspection time %f ms", inspectionTime.GetElapsedMSec());
//
//
//			if(theApp.m_bExitApp) break;
//
////			theApp.m_bSingleRunScanning = FALSE; //Temp
//
//			if(theApp.m_bSingleRunScanning) {					//Girish
//				theApp.m_bScanningEnabled = FALSE;
//			}
//		}
//
//		OutputDebugLogTo(8, TRUE, "----- Step 11,  Out of  loop");
//
//
//		// We reached end of scan thread
//		theApp.m_bScanning = FALSE;
//		SetEvent(theApp.eventScanThreadKilled);
//
//	}
//	catch(std::exception & ex) {
//	OutputDebugLogTo(0, FALSE, "%s exception occured in scanning thread", ex.what());
//	}
//	catch(...) {
//	OutputDebugLogTo(0, FALSE, "Unknown exception occured in scanning thread");
//	}

return 0;
}

UINT ZSensorSequence(LPVOID)
{
	try {

		MotionParameters & motionParameters = theApp.motionParameters;
	
		std::vector <double> ZValues;

		volatile int nZPositionId;
		int nError = 0;
		int nTriggerCount = 0;
		int nScalePos = 0;
		int nPreviousCount = 0;
		CPerformanceTimer readingDataTime;

		while(1) {

			//Step 1. Wait for event
			if(WaitForSingleObject(theApp.eventRunZSensorSequence, INFINITE) != WAIT_OBJECT_0) {
				OutputDebugLogTo(0, TRUE, "-----ERROR : X Latching thread synchronization has been failed. Closing thread...");
				break;
			}

			if(theApp.m_bExitApp)
				return 0;

			int nReadingCount = motionParameters.GetZSensorReadingCount();

			OutputDebugLogTo(3, TRUE, "ZSensorSequence: nReadingCount %d", nReadingCount);


			nZPositionId = InterlockedExchangeAdd(&theApp.nZSensorPositionId, 0);
			int nScanRow = InterlockedExchangeAdd(&theApp.motionParameters.nZSensorScanRow, 0); 

			theApp.m_TriangularSensor.Clear_Buffer(theApp.m_TrigangualrsensorInstance);

			while(1){

				//theApp.m_MotorResource.GetTriggerCount(theApp.m_nMotorCardNo, 0, 3, 3, &nTriggerCount, &nTrigCountRemain, &nTrigEnCount);
				nError = theApp.m_ScaleResource.GetScaleIntCount(theApp.m_nMotorCardNo, X_SCALE_2, &nTriggerCount);
				theApp.motionParameters.GetCurrentScalePosition(X_SCALE_2, nScalePos);

				if(theApp.m_bExitApp)
					return 0;

				if(nTriggerCount == 0) {				
					Sleep(2);
					continue;
				}
					
				if(nPreviousCount != nTriggerCount) {
					readingDataTime.Reset();
					ZValues.clear();

					theApp.m_TriangularSensor.Clear_Buffer(theApp.m_TrigangualrsensorInstance);
					theApp.m_TriangularSensor.ReadBlockData(theApp.m_TrigangualrsensorInstance, nReadingCount, ZValues);

					OutputDebugLogTo(3, TRUE, "nTriggerCount %d, nZPositionId %d, ReadBlockData Time %.2f ms, nScalePos %d", nTriggerCount, nZPositionId, readingDataTime.GetElapsedMSec(), nScalePos);

					int nZIndex = nZPositionId;
					/*if (nScanRow == 3) {
						nZIndex = 29 - nZPositionId;
					}
					else if (nScanRow == 8) {
						nZIndex = 89 - nZPositionId;
					}*/
					nError = motionParameters.SetZSensorEachFovMultipleDistance(nZIndex, ZValues);
					//OutputDebugLogTo(3, TRUE, "SetZSensorEachFovMultipleDistance For Z Index %d", nZIndex);


					if(nZPositionId < 0 || nZPositionId >= theApp.nZSensorPositionCount) {
						OutputDebugLogTo(0, TRUE, "----- ERROR : Z Position ID is incorrect %d", nZPositionId);
						break;
					}

					nZPositionId = InterlockedIncrement(&theApp.nZSensorPositionId);
					nPreviousCount = nTriggerCount;
					break;
				}
				
			}

			bool lastOnRow = false;
			if(motionParameters.IsCameraGrabIDLastOnRowZSensor(nZPositionId, lastOnRow)) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : Can't get information about zPosition %d", nZPositionId);
				continue;
			}

			if(!lastOnRow) {
				SetEvent(theApp.eventRunZSensorSequence);
			}
			else {
				SetEvent(theApp.m_eventZSensorTrigCount);
				OutputDebugLogTo(3, TRUE, "Last on row zPosition %d, nTriggerCount %d", nZPositionId, nTriggerCount);
				OutputDebugLogTo(3, TRUE, "------------------------------------------");
			}
		}

		SetEvent(theApp.m_eventZSensorTrigCount);
	}
	catch(std::exception & ex) {
		OutputDebugLogTo(0, FALSE, "%s exception occured in Z Sensor thread", ex.what());
	}
	catch(...) {
		OutputDebugLogTo(0, FALSE, "Unknown exception occured in Z Sensor thread");
	}

	return 0;
}


UINT GrabThread(LPVOID pParam)
{
	try{

		CTrackHandler* pTrack = static_cast <CTrackHandler*> (pParam);
		//	CCameraInterface &  camera = pTrack->m_camera;
		CApplication* pApp = (CApplication*) AfxGetApp();
		CAppDoc* pDoc = NULL;
		int nError;
		int nTriggerCount = 0;
		int nScalePos = 0;
		int nPreviousCount = 0;
		CImageBuffer * grabbedBuffer = new CImageBuffer();

		while (1) {
			if (WaitForSingleObject(pTrack->m_eventGrabbing, INFINITE) != WAIT_OBJECT_0) {
				OutputDebugLogTo(0, TRUE, "----- RESTART APPLICATION! Hardware grabbing thread is destoyed by ERROR; [%s] Camera");
				break;
			}

			int nDoc = theApp.getSequenceIntensityId();

			if (pApp->m_bExitApp)
				break;

			nError = pTrack->EnqueueAsyncGrab();

			if (nError) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : EnqueueAsyncGrab [Track %d] Camera", pTrack->m_nTrack);
				continue;
			}

			int nXSize = pTrack->m_pDoc[0]->m_Buffer.GetImgSize().cx;
			int nYSize = pTrack->m_pDoc[0]->m_Buffer.GetImgSize().cy;
			CRect rcFOV = CRect(0, 0, nXSize, nYSize);

			grabbedBuffer->Allocate(rcFOV.Size());

			CPerformanceTimer timer;
			timer.Reset();

			if (pTrack->WaitForCompletion(&grabbedBuffer)) {
				OutputDebugLogTo(3 + pTrack->m_nTrack, TRUE, "[Track%d] === Wait for Snap Completion Error {%5.1fms}", pTrack->m_nTrack + 1, timer.GetElapsedMSec());
				continue;
			}

			const long nCameraGrabNo = InterlockedIncrement(&pTrack->m_pDoc[nDoc]->nCameraGrabNo);

			theApp.m_ScaleResource.GetScaleIntCount(theApp.m_nMotorCardNo, X_SCALE_1, &nTriggerCount);

			OutputDebugLogTo(6, TRUE, "Grab count %d, Trigger count %d: time {%5.1fms}", nCameraGrabNo, nTriggerCount, timer.GetElapsedMSec());

			if (nCameraGrabNo < 0) {
				InterlockedDecrement(&pTrack->m_pDoc[nDoc]->nCameraGrabNo);
				OutputDebugLogTo(0, TRUE, "----- ERROR : Camera grab incorrect ID %d", nCameraGrabNo);
				continue;
			}

			int nDieColNo, nDieRowNo;
			theApp.motionParameters.GetMappingParameters(nDieColNo, nDieRowNo);

			if (pTrack->m_pDoc[nDoc]->imageGrab.empty()) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : Image buffer is not allocated for grabbing");
				continue;
			}
			if (!pTrack->m_pDoc[nDoc]->imageGrab[nCameraGrabNo].IsAllocated()) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : Image buffer is not allocated for grabbing");
				continue;
			}

			if (ImgCopy(grabbedBuffer, &rcFOV, &pTrack->m_pDoc[nDoc]->imageGrab[nCameraGrabNo], &rcFOV)) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : Copy image from camera is failed");
				continue;
			}

			bool bLastGrabOnRow = false;

			if (pApp->motionParameters.IsCameraGrabIDLastOnRowTop(nCameraGrabNo + 1, bLastGrabOnRow)) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : Can't get information about camera grab [%d]", nCameraGrabNo);
				continue;
			}

			if (bLastGrabOnRow /* || theApp.pGlobalBackgroundThread->killThreads*/) {
				ResetEvent(pTrack->m_eventGrabbing);
				SetEvent(pTrack->m_eventTrigCount);

				OutputDebugLogTo(0, TRUE, "---------- TOP STATION LAST ON ROW %d ----------", nCameraGrabNo + 1);

			}
			int threadId = 0;


			int nDieRow, nDieColumn, nDieFOV, nDieNo;

			pApp->motionParameters.GetDeviceDetailsTOP(nCameraGrabNo, nDieRow, nDieColumn, nDieNo, nDieFOV);

			if (theApp.pGlobalBackgroundThread->IsReScanEnabled() && theApp.m_bEnableReScanOption) {
				if (!theApp.motionParameters.GetReScanStatus(nDieNo)) {
					OutputDebugLogTo(8, TRUE, "In Sequence thread Die Id= %d, Status = False", nDieNo);
					continue;
				}
			}



			bool bStatus = true;
			nError = pTrack->m_pDoc[nDoc]->EnableInspection(nDieFOV, nDieNo, bStatus);
			if (nError) {
				OutputDebugLogTo(8, TRUE, "TOP Station: Failed to get Inspection status for Die Id= %d, Fov Id %d", nDieNo, nDieFOV);
				bStatus = true;
			}

			if (!bStatus) {
				OutputDebugLogTo(8, TRUE, "TOP Station: Doc %d, Grab ID %d, Fov %d, Die %d", nDoc, nCameraGrabNo, nDieFOV, nDieNo);
				continue;
			}

			OutputDebugLogTo(6, TRUE, "TOP Station: Set Event Do Inspection In Sequence DieId= %d, Status = true", nDieNo);


			if (pTrack->getInspectionThreadCount() > 0) {

				EnterCriticalSection(&pTrack->CriticalSectionScannedImage);
				std::vector < size_t > pendingImageCount(pTrack->getInspectionThreadCount());

				for (size_t id = 0; id < pendingImageCount.size(); ++id)
					pendingImageCount[id] = pTrack->m_pDoc[nDoc]->grabbedImageId[id].size();

				threadId = nDieNo % pTrack->getInspectionThreadCount();
				pTrack->m_pDoc[nDoc]->grabbedImageId[threadId].push(nCameraGrabNo);
				LeaveCriticalSection(&pTrack->CriticalSectionScannedImage);
				SetEvent(pTrack->eventDoInspection[threadId]);
			}
			else {
				EnterCriticalSection(&pTrack->CriticalSectionScannedImage);
				pTrack->m_pDoc[nDoc]->grabbedImageId[0].push(nCameraGrabNo);
				LeaveCriticalSection(&pTrack->CriticalSectionScannedImage);
				SetEvent(pTrack->eventDoInspection[0]);
			}
		}

		SetEvent(pTrack->m_eventTrigCount);
		SetEvent(pTrack->eventHardwareGrabThreadKilled);

	}
	catch (std::exception & ex){
		OutputDebugLogTo(0, FALSE, "%s exception occured in GrabThread thread", ex.what());
	}
	catch (...){
		OutputDebugLogTo(0, FALSE, "Unknown exception occured in GrabThread thread");
	}

	return 0;
}


UINT GrabThreadTHR(LPVOID pParam)
{
	try {
		CTrackHandler* pTrack = static_cast <CTrackHandler*> (pParam);
		//	CCameraInterface &  camera = pTrack->m_camera;
		CApplication* pApp = (CApplication*)AfxGetApp();
		CAppDoc* pDoc = NULL;
		int nError;
		int nTriggerCount = 0;
		int nScalePos = 0;
		int nPreviousCount = 0;
		CImageBuffer * grabbedBuffer = new CImageBuffer();
		int threadId = 0;

		while (1) {
			if (WaitForSingleObject(pTrack->m_eventGrabbing, INFINITE) != WAIT_OBJECT_0) {
				OutputDebugLogTo(0, TRUE, "----- RESTART APPLICATION! Hardware grabbing thread is destoyed by ERROR; [%s] Camera");
				break;
			}
			OutputDebugLogTo(6, TRUE, "----------------------------------------------------");
			OutputDebugLogTo(6, TRUE, "pTrack->m_eventGrabbing....");

			if (pApp->m_bExitApp)
				break;

			int nDoc = 0;

			OutputDebugLogTo(7, TRUE, "----------------------------------------------------", nTriggerCount);
			while (1) {
				nError = theApp.m_ScaleResource.GetScaleIntCount(theApp.m_nMotorCardNo, X_SCALE_3, &nTriggerCount);
				theApp.motionParameters.GetCurrentScalePosition(X_SCALE_3, nScalePos);

				if (theApp.m_bExitApp)
					return 0;

				if (nTriggerCount == 0) {
					Sleep(2);
					continue;
				}

				if (nPreviousCount != nTriggerCount) {
					//1. Waiting for captured image
					CPerformanceTimer timer;
					timer.Reset();

					nError = pTrack->EnqueueAsyncGrab();

					if (nError) {
						OutputDebugLogTo(0, TRUE, "----- ERROR : EnqueueAsyncGrab [Track %d] Camera", pTrack->m_nTrack);
						break;
					}

					int nXSize = pTrack->m_pDoc[0]->m_Buffer.GetImgSize().cx;
					int nYSize = pTrack->m_pDoc[0]->m_Buffer.GetImgSize().cy;
					CRect rcFOV = CRect(0, 0, nXSize, nYSize);
					grabbedBuffer->Allocate(rcFOV.Size());

					OutputDebugLogTo(6, TRUE, "WaitForCompletionTHR...{%5.1fms}", timer.GetElapsedMSec());
					timer.Reset();
					if (pTrack->WaitForCompletionTHR(&grabbedBuffer)) {
						OutputDebugLogTo(3 + pTrack->m_nTrack, TRUE, "[Track%d] === Wait for Snap Completion Error {%5.1fms}", pTrack->m_nTrack + 1, timer.GetElapsedMSec());
						break;
					}

					//2. Proceed Captured Image
					const long nCameraGrabNo = InterlockedIncrement(&pTrack->m_pDoc[nDoc]->nCameraGrabNo);

					OutputDebugLogTo(6, TRUE, "Grab count %d, Trigger count %d, Scalse Pos %d: time {%5.1fms}", nCameraGrabNo, nTriggerCount, nScalePos, timer.GetElapsedMSec());

					if (nCameraGrabNo < 0) {
						InterlockedDecrement(&pTrack->m_pDoc[nDoc]->nCameraGrabNo);
						OutputDebugLogTo(6, TRUE, "----- ERROR : Camera grab incorrect ID %d", nCameraGrabNo);
						break;
					}

					int nDieColNo, nDieRowNo;
					theApp.motionParameters.GetMappingParameters(nDieColNo, nDieRowNo);

					//3. Immediately move to next Z position
					if (pTrack->m_nTrack == STATION_THR) {
						SetEvent(theApp.m_eventZAxizFocus);
					}

					//4. Validate grabber buffer data before copy captured image to it.
					if (pTrack->m_pDoc[nDoc]->imageGrab.empty()) {
						OutputDebugLogTo(6, TRUE, "----- ERROR : Image buffer is not allocated for grabbing");
						break;
					}
					if (!pTrack->m_pDoc[nDoc]->imageGrab[nCameraGrabNo].IsAllocated()) {
						OutputDebugLogTo(6, TRUE, "----- ERROR : Image buffer is not allocated for grabbing");
						break;
					}

					if (ImgCopy(grabbedBuffer, &rcFOV, &pTrack->m_pDoc[nDoc]->imageGrab[nCameraGrabNo], &rcFOV)) {
						OutputDebugLogTo(6, TRUE, "----- ERROR : Copy image from camera is failed grabbedBuffer(%d, %d), imageGrab(%d, %d)",
							grabbedBuffer->GetImgSize().cx, grabbedBuffer->GetImgSize().cy,
							pTrack->m_pDoc[nDoc]->imageGrab[nCameraGrabNo].GetImgSize().cx, pTrack->m_pDoc[nDoc]->imageGrab[nCameraGrabNo].GetImgSize().cy);
						break;
					}

					//5. Break the loop to next grabbing
					//End Loop Checking Trigger Count
					nPreviousCount = nTriggerCount;
					break;
				}
				//End Checking Trigger Count
			}


			//6. Check Last Grab Image On Row
			volatile int nCameraGrabNo = InterlockedExchangeAdd(&pTrack->m_pDoc[nDoc]->nCameraGrabNo, 0);
			bool bLastGrabOnRow = false;
			if (pApp->motionParameters.IsCameraGrabIDLastOnRowTHR(nCameraGrabNo + 1, bLastGrabOnRow)) {
				OutputDebugLogTo(6, TRUE, "----- ERROR : Can't get information about camera grab [%d]", nCameraGrabNo);
				continue;
			}

			if (bLastGrabOnRow /*|| theApp.pGlobalBackgroundThread->killThreads*/) {
				ResetEvent(pTrack->m_eventGrabbing);
				SetEvent(pTrack->m_eventTrigCount);
				OutputDebugLogTo(6, TRUE, "---------- THR STATION LAST ON ROW %d ----------", nCameraGrabNo + 1);
			}

			if (theApp.m_bEnableTopDatumPosToScanTHR) {
				bool bLastGrabOnDieRow = false;
				if (pApp->motionParameters.IsCameraGrabIDLastOnDieRow(nCameraGrabNo, bLastGrabOnDieRow)) {
					OutputDebugLogTo(6, TRUE, "----- ERROR : Can't get information about camera grab [%d]", nCameraGrabNo);
					continue;
				}

				if (bLastGrabOnDieRow && !bLastGrabOnRow) {
					OutputDebugLogTo(6, TRUE, "Lost on die row %d", nCameraGrabNo);
					SetEvent(theApp.m_eventYAxisDatumOffset);
				}
			}

			//7. Checking Sampling-Inspection Or Rescan Inspection(Based on TOP Inspection result) (To Trigger Inspection Event Or Not)
			int nDieRow, nDieColumn, nDieFOV, nDieNo;
			bool bEnableInspection;
			pApp->motionParameters.GetDeviceDetailsTHR(nCameraGrabNo, nDieRow, nDieColumn, nDieNo, nDieFOV);
			pApp->motionParameters.getInspectionEnableStatusTHR(nCameraGrabNo, bEnableInspection);

			//A. Inspection Flag From TOP Scan Inspection Result
			if (!bEnableInspection)
				continue;

			//B. ReScan Option in registry
			if (theApp.pGlobalBackgroundThread->IsReScanEnabled() && theApp.m_bEnableReScanOption) {
				if (!theApp.motionParameters.GetReScanStatus(nDieNo)) {
					OutputDebugLogTo(8, TRUE, "In Sequence thread Die Id= %d, Status = false", nDieNo);
					continue;
				}
			}

			//C. Sampling Inspection
			bool bStatus;
			nError = pTrack->m_pDoc[nDoc]->EnableInspection(nDieFOV, nDieNo, bStatus);
			if (nError) {
				OutputDebugLogTo(6, TRUE, "THR Station: Failed to get Inspection status for Die Id= %d, Fov Id %d", nDieNo, nDieFOV);
				bStatus = true;
			}

			if (!bStatus) {
				OutputDebugLogTo(6, TRUE, "THR Station: Doc %d, Grab ID %d, Fov %d, Die %d", nDoc, nCameraGrabNo, nDieFOV, nDieNo);
				continue;
			}

			OutputDebugLogTo(6, TRUE, "THR Station: Set Event Do Inspection In Sequence DieId= %d, Status = true", nDieNo);

			
			if (pTrack->getInspectionThreadCount() > 0) {

				EnterCriticalSection(&pTrack->CriticalSectionScannedImage);
				std::vector < size_t > pendingImageCount(pTrack->getInspectionThreadCount());

				for (size_t id = 0; id < pendingImageCount.size(); ++id)
					pendingImageCount[id] = pTrack->m_pDoc[nDoc]->grabbedImageId[id].size();


				threadId = nDieNo % pTrack->getInspectionThreadCount();
				pApp->motionParameters.setInspectionThreadId(nCameraGrabNo, threadId);

				pTrack->m_pDoc[nDoc]->grabbedImageId[threadId].push(nCameraGrabNo);
				LeaveCriticalSection(&pTrack->CriticalSectionScannedImage);
				SetEvent(pTrack->eventDoInspection[threadId]);
			}
			else {
				EnterCriticalSection(&pTrack->CriticalSectionScannedImage);
				pApp->motionParameters.setInspectionThreadId(nCameraGrabNo, 0);
				pTrack->m_pDoc[nDoc]->grabbedImageId[0].push(nCameraGrabNo);
				LeaveCriticalSection(&pTrack->CriticalSectionScannedImage);
				SetEvent(pTrack->eventDoInspection[0]);
			}
		}

		SetEvent(pTrack->m_eventTrigCount);
		SetEvent(pTrack->eventHardwareGrabThreadKilled);

	}
	catch (std::exception & ex){
		OutputDebugLogTo(0, FALSE, "%s exception occured in GrabThreadTHR thread", ex.what());
	}
	catch (...){
		OutputDebugLogTo(0, FALSE, "Unknown exception occured in GrabThreadTHR thread");
	}

	return 0;
}

UINT FocusZAxis(LPVOID pParam)
{
try{

	CApplication* pApp = (CApplication*) AfxGetApp();
	MotionParameters & motionParameters = theApp.motionParameters;

	while(1) {
		if(WaitForSingleObject(theApp.m_eventZAxizFocus, INFINITE) != WAIT_OBJECT_0) {
			OutputDebugLogTo(0, TRUE, "-----ERROR : X Latching thread synchronization has been failed. Closing thread...");
			break;
		}

		if(theApp.m_bExitApp)
			break;

		const long nCameraGrabNo = InterlockedExchangeAdd(&theApp.m_pTracks[STATION_THR].m_pDoc[0]->nCameraGrabNo, 0);

//		OutputDebugLogTo(9, TRUE, "Moving to in Grab ", nCameraGrabNo + 1);

//		OutputDebugLogTo(9, TRUE, "Focus Move Id %d", nCameraGrabNo +1);

		motionParameters.MoveZMotorToFocussingPos(nCameraGrabNo + 1);

	}

}
catch (std::exception & ex)
{
	OutputDebugLogTo(0, FALSE, "%s exception occured in image Z focussing move thread", ex.what());
}
catch (...)
{
	OutputDebugLogTo(0, FALSE, "Unknown exception occured Z focussing move thread");
}
	return 0;
}

UINT YMotorToDatumOffset(LPVOID pParam)
{
	MotionParameters & motionParameters = theApp.motionParameters;

	while (1) {
		if (WaitForSingleObject(theApp.m_eventYAxisDatumOffset, INFINITE) != WAIT_OBJECT_0) {
			OutputDebugLogTo(0, TRUE, "-----ERROR : Y motor move on Datum thread synchronization has been failed. Closing thread...");
			break;
		}

		if (theApp.m_bExitApp)
			break;

		const long nCameraGrabNo = InterlockedExchangeAdd(&theApp.m_pTracks[STATION_THR].m_pDoc[0]->nCameraGrabNo, 0);

		motionParameters.MoveYMotorDatumOffsetPos(nCameraGrabNo + 1);
	}

	return 0;
}

UINT LatchThread(LPVOID pParam)
{
	CTrackHandler* pTrack = (CTrackHandler*) pParam;
	MotionParameters & motionParameters = theApp.motionParameters;


	long latchValueX = 0, latchValueY = 0;
	int nError;

	long latchId = 0;

	while(1) {

		if(WaitForSingleObject(theApp.eventRunLatching, INFINITE) != WAIT_OBJECT_0) {
			OutputDebugLogTo(0, TRUE, "-----ERROR : X Latching thread synchronization has been failed. Closing thread...");
			break;
		}

		if(theApp.m_bExitApp)
			return 0;

		nError = motionParameters.GetLatchValue(X_SERVO_MOTOR, latchValueX, 1);
		if(nError) {
			OutputDebugLogTo(0, TRUE, "------ ERROR : X Latching has received an error");
			continue;
		}

		//nError = motionParameters.GetLatchValue(Y_SERVO_MOTOR, latchValueY, 1);
		//if(nError) {
		//	OutputDebugLogTo(0, TRUE, "------ ERROR : Y Latching has received an error");
		//	continue;
		//}

		if(theApp.m_bExitApp)
			return 0;

		latchId = InterlockedExchangeAdd(&theApp.nLatchedPositionId, 0);

		if(latchId < 0 || latchId >= theApp.nLatchedPositionCount) {
			OutputDebugLogTo(0, TRUE, "----- ERROR : Latch ID is incorrect %d", latchId);
			continue;
		}

		theApp.latchedPositionX[latchId] = latchValueX;
		theApp.latchedPositionY[latchId] = latchValueY;


		latchId = InterlockedIncrement(&theApp.nLatchedPositionId);

		bool lastGrabOnRow = false;
		
		if(pTrack->m_nTrack == STATION_THR) {

			if(motionParameters.IsCameraGrabIDLastOnRowTHR(latchId, lastGrabOnRow)) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : Can't get information about latch %d", latchId);
				continue;
			}
		}
		else if(pTrack->m_nTrack == STATION_TOP) {
			if(motionParameters.IsCameraGrabIDLastOnRowTop(latchId, lastGrabOnRow)) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : Can't get information about latch %d", latchId);
				continue;
			}	
		}

		if(!lastGrabOnRow) {
			SetEvent(theApp.eventRunLatching);
		}
	}
}

UINT InspectOnlineThread(LPVOID pParam)
{
	try {

		//	CCameraInterface &  camera = pTrack->m_camera;
		CApplication* pApp = (CApplication*) AfxGetApp();
		CTrackHandler* pTrack = (CTrackHandler*) pParam;
		CAppDoc* pDoc = pTrack->m_pDoc[0];
		int nTrackIndex = pTrack->m_nTrack;
		
		const unsigned char threadId = static_cast<unsigned char>(pTrack->getInspectionThreadId());
		
		
		LOGPEN TextColorRed, TextColorGreen, TextColor, TextColorYellow;
		int nErrInsp[5];
		
		TextColorRed.lopnColor = RGB(230, 50, 50);
		TextColorRed.lopnStyle = PS_SOLID;
		TextColorRed.lopnWidth.x = 1;
		TextColorYellow.lopnColor = RGB(230, 230, 0);
		TextColorYellow.lopnStyle = PS_SOLID;
		TextColorYellow.lopnWidth.x = 1;
		TextColorGreen.lopnColor = RGB(50, 230, 50);
		TextColorGreen.lopnStyle = PS_SOLID;
		TextColorGreen.lopnWidth.x = 1;

		int nXSize = pTrack->m_pDoc[0]->m_Buffer.GetImgSize().cx;
		int nYSize = pTrack->m_pDoc[0]->m_Buffer.GetImgSize().cy;
		CRect rcFOV = CRect(0, 0, nXSize, nYSize);

		long nScannedImageID = -1;
		std::vector <int> inspectionResult;
		std::vector <int> inspectionBlockColumn;
		std::vector <int> inspectionBlockRow;
		std::vector <int> inspectionDieColumn;
		std::vector <int> inspectionDieRow;
		std::vector <int> inspectionDieNo;
		std::vector <int> inspectionFovNo;
		//	std::vector <int> inspectionReturnValues;
		bool bUpdateMapping = false;
		int inspectionReturnValue;
		BOOL bBreak = FALSE;
		int nDocumentId = 0;
	
		while(1) {
			if(pApp->m_bExitApp)
				break;

			if(WaitForSingleObject(pTrack->eventDoInspection[threadId], INFINITE) != WAIT_OBJECT_0) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : Thread manipulation, Inspection Thread has been closed");
				break;
			}

			if(pApp->m_bExitApp)
				break;

			ResetEvent(pTrack->eventInspectionDone[threadId]);

			bool bScannedQueueEmpty = false;

			while (!bScannedQueueEmpty) {

				if (pApp->m_bExitApp)
					break;

				EnterCriticalSection(&pTrack->CriticalSectionScannedImage);
				
				for (int nDoc = 0; nDoc < pTrack->m_nTotalDocs; nDoc++) {
					bScannedQueueEmpty = pTrack->m_pDoc[nDoc]->grabbedImageId[threadId].empty();
					
					if (!bScannedQueueEmpty) {
						nDocumentId = nDoc;
						break;
					}
				}

				if (bScannedQueueEmpty) {
					SetEvent(pTrack->eventInspectionDone[threadId]);
					ResetEvent(pTrack->eventDoInspection[threadId]);

					if (bUpdateMapping) {
						theApp.m_pImageSelectionDlg->setDieStatus(inspectionResult, inspectionDieColumn, inspectionDieRow, inspectionDieNo, inspectionFovNo, pTrack->m_nTrack, nDocumentId);
						inspectionResult.clear();
						inspectionDieColumn.clear();
						inspectionDieRow.clear();
						inspectionDieNo.clear();
						inspectionFovNo.clear();
					}

					CPerformanceTimer timer;
					timer.Reset();

					//Clear Memory
					CInspectionHandler* pInspHandler = &pApp->m_pTracks[0].m_pDoc[0]->inspection(0);
					pInspHandler->m_bInitiateClearCache = TRUE;
					pInspHandler->ReleaseMemory();

					LeaveCriticalSection(&pTrack->CriticalSectionScannedImage);

					break;
				}

				nScannedImageID = pTrack->m_pDoc[nDocumentId]->grabbedImageId[threadId].front();
				pTrack->m_pDoc[nDocumentId]->grabbedImageId[threadId].pop();

				bool bInspectionDone;
				pApp->motionParameters.getInspectionStatus(pTrack->m_nTrack, nScannedImageID, bInspectionDone);

				LeaveCriticalSection(&pTrack->CriticalSectionScannedImage);

				if (bInspectionDone && nDocumentId == 0)
					continue;


				if (theApp.m_bExitApp)
					break;


				if (pTrack->m_pDoc[nDocumentId]->imageGrab.empty()) {
					OutputDebugLogTo(0, TRUE, "----- ERROR : An Array for Scanned images is not allocated");
					break;
				}

				if (!pTrack->m_pDoc[nDocumentId]->imageGrab[nScannedImageID].IsAllocated()) {
					OutputDebugLogTo(0, TRUE, "----- ERROR : Image %d in scanned image array is not allocated", nScannedImageID);
					break;
				}

				int nDieRow, nDieColumn, nDieFOV, nDieNo;
				
				if (pTrack->m_nTrack == STATION_TOP)
					pApp->motionParameters.GetDeviceDetailsTOP(nScannedImageID, nDieRow, nDieColumn, nDieNo, nDieFOV);
				else if (pTrack->m_nTrack == STATION_THR)
					pApp->motionParameters.GetDeviceDetailsTHR(nScannedImageID, nDieRow, nDieColumn, nDieNo, nDieFOV);

				pTrack->m_nFOVIndex = nDieFOV;
				pTrack->m_nDieIndex = nDieNo;
				inspectionReturnValue = nErrInsp[0] = GOOD_UNIT;

				//1. Call Inspection Function
				if (!theApp.pGlobalBackgroundThread->bDryRun) {
					
					if(pTrack->m_nTrack != STATION_THR)
						inspectionReturnValue = nErrInsp[nDocumentId] = pTrack->m_pDoc[0]->InspectAllDocOnline(nDieFOV, nDocumentId, nScannedImageID);
					

					//--- Start of get the shifted information TOP Die Scan ---
					EnterCriticalSection(&theApp.m_CriticalSectionTOPDieShiftedInfo);
					
					//Get the shifted information from first die, first FOV
					if(pTrack->m_nTrack == STATION_TOP && nDieFOV == 0) {
						//In case we can not found the pattern --> we assume that have not any shifted
						theApp.motionParameters.setOffsetForTHR(0, 0, nDieNo);

						HTuple hInspectShiftAlongXMicron, hInspectShiftAlongYMicron;
						hInspectShiftAlongXMicron = pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->inspection(nDieFOV).m_Inspection.m_InspectionData.hInspectShiftAlongXMicron;
						hInspectShiftAlongYMicron = pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->inspection(nDieFOV).m_Inspection.m_InspectionData.hInspectShiftAlongYMicron;

						if(hInspectShiftAlongXMicron.TupleLength() > 0 && hInspectShiftAlongYMicron.TupleLength() > 0) {
							theApp.motionParameters.setOffsetForTHR(hInspectShiftAlongXMicron.D(), hInspectShiftAlongYMicron.D(), nDieNo);
							//OutputDebugLogTo(7, false, "TOP Die Scan Shifted Info: (%.2f, %.2f)", hInspectShiftAlongXMicron.D(), hInspectShiftAlongYMicron.D());
						}
					}

					LeaveCriticalSection(&theApp.m_CriticalSectionTOPDieShiftedInfo);
					//--- End of get the shifted information TOP Die Scan ---

					//Temp to show only half of images
					if ((nScannedImageID % 3 == 0) && pApp->m_RuntimeDisplayEnable) {				
						pTrack->m_pDoc[nDocumentId]->CopyGrabbedBuffer(&pTrack->m_pDoc[nDocumentId]->imageGrab[nScannedImageID]);
						DrwUpdate(&pTrack->m_pDoc[nDocumentId]->m_ImageViewInfo);

						CString strDefectMsg;
						DrwDel(&pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->m_ImageViewInfo);
						if (nErrInsp[nDocumentId] < GOOD_UNIT) {
							if (nErrInsp[nDocumentId] > -ERROR_CODES || nErrInsp[nDocumentId] == -PROCESS_ERROR) {
								strDefectMsg.Format("%s ", pApp->GetFailedStr(nErrInsp[nDocumentId], FALSE));
								DrwStr(&pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->m_ImageViewInfo, &strDefectMsg, &CPoint(240, 20), &TextColorRed, 80);
							}
							else {
								CString strTemp("PVI Defect");
								std::map<int, CString> mapPVIDefect(pApp->m_pTracks[pTrack->m_nTrack].m_mapPviDefectsStatsGeneral);
								if (mapPVIDefect.find(nErrInsp[nDocumentId]) != mapPVIDefect.end()) {
									strTemp.Format("%s", mapPVIDefect[nErrInsp[nDocumentId]]);
								}
								DrwStr(&pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->m_ImageViewInfo, &strTemp, &CPoint(240, 20), &TextColorRed, 80);
							}

						}
						else {
							strDefectMsg = "PASS";
							DrwStr(&pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->m_ImageViewInfo, &strDefectMsg, &CPoint(240, 20), &TextColorGreen, 80);
						}

						CRect rect = CRect(CPoint(80, 80), CSize(60, 60));
						BOOL bFill = pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->m_nInspCounter % 2;
						if (bFill)
							rect.DeflateRect(6, 6);
						else
							TextColorYellow.lopnWidth.x = 10;
						DrwRect(&pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->m_ImageViewInfo, &rect, &TextColorYellow);
						DrwUpdate(&pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->m_ImageViewInfo);

						pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->m_nInspCounter++;
						if (pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->m_nInspCounter == 100)
							pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[nDocumentId]->m_nInspCounter = 0;
					}
				}

				pApp->motionParameters.setInspectionStatus(pTrack->m_nTrack, nScannedImageID, true);

				if (nErrInsp[nDocumentId] == -NO_IMAGE)
					pApp->m_nErrorCode[pTrack->m_nDieIndex][pTrack->m_nFOVIndex][nDocumentId] = 0;
				else
					pApp->m_nErrorCode[pTrack->m_nDieIndex][pTrack->m_nFOVIndex][nDocumentId] = nErrInsp[nDocumentId];

				pApp->UpdateTotalLotSummary(pTrack->m_nTrack, nErrInsp[nDocumentId], !pApp->m_bOnline);

				pApp->UpdateStatisticsWindow();

				inspectionResult.push_back(inspectionReturnValue);
				inspectionDieColumn.push_back(nDieColumn);
				inspectionDieRow.push_back(nDieRow);
				inspectionDieNo.push_back(nDieNo);
				inspectionFovNo.push_back(nDieFOV);
				bUpdateMapping = true;

				EnterCriticalSection(&theApp.m_CriticalSectionSaveImage);	

				CString strStripID;
				//if (theApp.bEnableBarcode)

				//2. Saving Images
				if (((theApp.m_bSaveOnlinePassImages &&  inspectionReturnValue == 0) || (theApp.m_bSaveOnlineFailImages && inspectionReturnValue!= 0)) && !theApp.m_bVerificationMode) {

					if (theApp.strInspectDeviceBarcodeID != "")
						strStripID = theApp.strInspectDeviceBarcodeID;
					else
						strStripID = "1";

					SaveImageInfo info;

					info.image = new CImageBuffer();
					info.image->Allocate(pTrack->m_pDoc[0]->m_Buffer.GetImgSize());

					int nXSize = __min(pApp->m_pTracks[nTrackIndex].m_pDoc[nDocumentId]->imageGrab[nScannedImageID].GetImgSize().cx, info.image->GetImgSize().cx);
					int nYSize = __min(pApp->m_pTracks[nTrackIndex].m_pDoc[nDocumentId]->imageGrab[nScannedImageID].GetImgSize().cy, info.image->GetImgSize().cy);
					rcFOV = CRect(0, 0, nXSize, nYSize);
					
					int nError = ImgCopy(&pApp->m_pTracks[nTrackIndex].m_pDoc[nDocumentId]->imageGrab[nScannedImageID], &rcFOV, info.image, &rcFOV);
					if (nError) {
						OutputDebugLogTo(0, TRUE, "----- ERROR : Copy image from camera is failed");
					}
					else {
						info.lotId = theApp.m_strLotInfo.GetString();
						info.stripId = strStripID;
						info.BarcodeScanTime = theApp.strInspectDeviceScanTimeStamp;
						info.cameraId = nTrackIndex;
						info.nDocumentId = nDocumentId;
						info.grabId = nScannedImageID;
						info.dieColumnId = nDieColumn;
						info.dieRowId = nDieRow;
						info.nDieNo = nDieNo;
						info.nFov = nDieFOV;
						info.defectId = inspectionReturnValue;
						info.saveAll = theApp.m_bSaveOnlinePassImages && theApp.m_bSaveOnlineFailImages;
						info.savePass = theApp.m_bSaveOnlinePassImages;
						info.saveDefect = theApp.m_bSaveOnlineFailImages;
						theApp.saveImageQueue.push(info);

						SetEvent(theApp.eventSaveImage);
					}
				}

				LeaveCriticalSection(&theApp.m_CriticalSectionSaveImage);

				EnterCriticalSection(&theApp.m_CriticalSectionDeepLearnSaveImage);

			
				if (/*theApp.m_bDeepLearningSaveImage*/FALSE) {

					if (theApp.strInspectDeviceBarcodeID != "")
						strStripID = theApp.strInspectDeviceBarcodeID;
					else
						strStripID = "1";

					SaveImageInfo info;

					info.image = new CImageBuffer();
					info.image->Allocate(pTrack->m_pDoc[0]->m_Buffer.GetImgSize());
					if (ImgCopy(&pApp->m_pTracks[nTrackIndex].m_pDoc[nDocumentId]->imageGrab[nScannedImageID], &rcFOV, info.image, &rcFOV)) {
						OutputDebugLogTo(0, TRUE, "----- ERROR : Copy image from camera is failed");
						continue;
					}

					info.lotId = theApp.m_strLotInfo.GetString();
					info.stripId = strStripID;
					info.BarcodeScanTime = theApp.strInspectDeviceScanTimeStamp;
					info.cameraId = nTrackIndex;
					info.nDocumentId = nDocumentId;
					info.grabId = nScannedImageID;
					info.dieColumnId = nDieColumn;
					info.dieRowId = nDieRow;
					info.nDieNo = nDieNo;
					info.nFov = nDieFOV;
					info.defectId = inspectionReturnValue;
					info.saveAll = theApp.m_bSaveOnlinePassImages && theApp.m_bSaveOnlineFailImages;
					info.savePass = theApp.m_bSaveOnlinePassImages;
					info.saveDefect = theApp.m_bSaveOnlineFailImages;
					theApp.saveDeepLearnImageQueue.push(info);
					SetEvent(theApp.eventDeepLearnSaveImage);

				}

				LeaveCriticalSection(&theApp.m_CriticalSectionDeepLearnSaveImage);
			}

			SetEvent(pTrack->eventInspectionDone[threadId]);
		}
		SetEvent(pTrack->eventInspectionDone[threadId]);
	}
	
	catch(...) {
		OutputDebugLogTo(0, FALSE, "Unknown exception occured in InspectOnlineThread() thread");
	}
	return 0;
}

UINT DuplicateInspectionThread(LPVOID pParam)
{
	try {
		CApplication* pApp = (CApplication*)AfxGetApp();
		CTrackHandler* pTrack = (CTrackHandler*)pParam;
		CAppDoc* pDoc = pTrack->m_pDoc[0];
		int nTrackIndex = pTrack->m_nTrack;

		const unsigned char threadId = static_cast<unsigned char>(pTrack->getDuplicateInspectionThreadId());


		while (1) {
			if (pApp->m_bExitApp)
				break;

			if (WaitForSingleObject(pTrack->eventDuplicateInspection[threadId], INFINITE) != WAIT_OBJECT_0) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : Thread manipulation, Inspection Thread has been closed");
				break;
			}

			if (pApp->m_bExitApp)
				break;

			ResetEvent(pTrack->eventDuplicateInspectionDone[threadId]);

			pTrack->m_pDoc[0]->DuplicateInspectionOnline(threadId);

			
			SetEvent(pTrack->eventDuplicateInspectionDone[threadId]);
			ResetEvent(pTrack->eventDuplicateInspection[threadId]);
		}
		SetEvent(pTrack->eventDuplicateInspectionDone[threadId]);
	}

	catch (...) {
		OutputDebugLogTo(0, FALSE, "Unknown exception occured in DuplicateInspectionThread thread");
	}

	return 0;
}


//UINT ClearInspectionData(LPVOID)
//{
//	CApplication* pApp = (CApplication*)AfxGetApp();
//
//	while (1) {
//		if (WaitForSingleObject(pApp->eventClearMemory, INFINITE) != WAIT_OBJECT_0) {
//			OutputDebugLogTo(0, TRUE, "-----ERROR : Failed, Clear Inspection Thread had Synchronisation Error. Closing thread...");
//			break;
//		}
//
//		ResetEvent(pApp->eventClearMemory);
//
//		if (pApp->m_bExitApp) break;
//
//		CPerformanceTimer timer;
//		timer.Reset();
//		//Clear Moemory
//		CInspectionHandler* pInspHandler = &pApp->m_pTracks[0].m_pDoc[0]->inspection(0);
//		pInspHandler->m_bClearThreadCache = TRUE;
//		pInspHandler->ReleaseMemory();
//		OutputDebugLogTo(7, TRUE,  "Time taken to clear cache memory %3.06f", timer.GetElapsedMSec());
//		SetEvent(pApp->eventClearMemoryDone);
//	}
//	return 0;
//}

UINT SaveImageThread(LPVOID)
{
	ResetEvent(theApp.eventSaveImageKilled);

	CImageBuffer ImgColour;

	CString strSavePath;
	CString strSaveImageName;
	CString strTemp;
	CString strCurStripId = "";

	bool bImagePresence = false;
	bool bCloseApplication = true;

	const COLORREF nDefectColour = RGB(255, 0, 0);

	SaveImageInfo info;

	while (1) {
		if (WaitForSingleObject(theApp.eventSaveImage, INFINITE) != WAIT_OBJECT_0) {
			OutputDebugLogTo(0, TRUE, "-----ERROR : Failed Image saving thread has synchronisation error. Closing thread...");
			break;
		}

		if (theApp.m_bExitApp) break;

		bImagePresence = false;

		do {
			EnterCriticalSection(&theApp.m_CriticalSectionSaveImage);

			if (!theApp.saveImageQueue.empty()) {

				info = theApp.saveImageQueue.front();
				theApp.saveImageQueue.pop();

				bImagePresence = true;
			}
			else
			{
				bImagePresence = false;
			}

			LeaveCriticalSection(&theApp.m_CriticalSectionSaveImage);

			
			CString strType[] = { "TOP_STATION", "THR_STATION" , "ENCAP_STATION" };

			if (bImagePresence) {
				// Create Defect Save folder
				if (info.BarcodeScanTime.IsEmpty())
					info.BarcodeScanTime = "1";
				strSavePath.Format("%s\\%s\\%s\\%s\\Doc %02d\\",
					theApp.m_ImageSaveFolder,
					theApp.m_strConfigFile,
					info.stripId +"_"+ info.BarcodeScanTime ,
					strType[info.cameraId],
					info.nDocumentId);

				CreateDirectoryTree(strSavePath);

			//	_mkdir(strSavePath);

				std::string fileExtension = theApp.m_bSaveOnlineBMPImages ? "bmp" : "jpg";

				if (info.saveAll || info.savePass || info.saveDefect) {
					
					strSaveImageName.Format("%s\\%s Doc%02d DieNo%03d Row%03d Col%03d FOV%03d.%s", strSavePath, strType[info.cameraId], info.nDocumentId,
						info.nDieNo + 1, info.dieRowId + 1, info.dieColumnId + 1, info.nFov + 1, fileExtension.c_str());
					
					int error;

					if (theApp.m_bSaveOnlineJpegImages)
						error = SaveJPGFile(strSaveImageName, *info.image, theApp.m_nSaveJpegQualityImages);
					else
						error = SaveGrayScaleBMPFile(strSaveImageName, *info.image);

					delete info.image;
					info.image = NULL;

					if (error)
						OutputDebugLogTo(0, TRUE, "-----ERROR : Failed to save image %s", strSaveImageName);
				}
			}

			//If image saving is not completed when application is closing
			if (theApp.m_bExitApp && bCloseApplication) {
				size_t nUnsavedImages = 0;

				EnterCriticalSection(&theApp.m_CriticalSectionSaveImage);

				nUnsavedImages = theApp.saveImageQueue.size();

				LeaveCriticalSection(&theApp.m_CriticalSectionSaveImage);

				if (nUnsavedImages > 0) {

					CString strClosingMessage;

				//	strClosingMessage.Format("%u images are unsaved.\nDo you want to close application immediately?", nUnsavedImages);
					strClosingMessage.Format("%u images are unsaved", nUnsavedImages);

					if (AfxMessageBox(strClosingMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
						bImagePresence = false;

					bCloseApplication = false;
				}
				break;
			}

		} while (bImagePresence);

		if (theApp.m_bExitApp) break;
	}

	SetEvent(theApp.eventSaveImageKilled);

	return 0;
}
UINT SaveDeepLeanrningImageThread(LPVOID)
{
	bool bImagePresence;
	ResetEvent(theApp.eventDeepLearnSaveImageKilled);
	CString strType[] = { "TOP_STATION", "THR_STATION" };
	std::string fileExtension = theApp.m_bSaveOnlineBMPImages ? "bmp" : "jpg";
	SaveImageInfo info;
	CString Strimage;
	bImagePresence = false;
	bool bCloseApplication = true;
	
	while (1) {
		if (WaitForSingleObject(theApp.eventDeepLearnSaveImage, INFINITE) != WAIT_OBJECT_0) {
			OutputDebugLogTo(0, TRUE, "-----ERROR : Failed Image saving thread has synchronisation error. Closing thread...");
			break;
		}
		if (theApp.m_bExitApp) break;

		do {
			EnterCriticalSection(&theApp.m_CriticalSectionDeepLearnSaveImage);
			if (!theApp.saveDeepLearnImageQueue.empty()) {
				info = theApp.saveDeepLearnImageQueue.front();
				theApp.saveDeepLearnImageQueue.pop();
				bImagePresence = true;
			}
			else
			{
				bImagePresence = false;
			}

			LeaveCriticalSection(&theApp.m_CriticalSectionDeepLearnSaveImage);
			int nErrorCode = info.defectId;
			if (bImagePresence) {
				CString strDstFileDir;
				strDstFileDir = theApp.m_ImageSaveFolder + "\\" + theApp.m_strConfigFile + "\\";
				CreateDirectoryTree(strDstFileDir);
				SYSTEMTIME sysTime;
				GetLocalTime(&sysTime);
				CString str;
				str.Format("%04u%02u%02u%02u%02u%02u%03u",sysTime.wYear,sysTime.wMonth,sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
		
				if (nErrorCode) {
					strDstFileDir.Append("Fail\\");
					_mkdir(strDstFileDir);
					strDstFileDir.Append(strType[info.cameraId] + "\\");
					_mkdir(strDstFileDir);
					CString	strErrorName = theApp.m_pImageSelectionDlg->GetDefectName(nErrorCode);

					strDstFileDir.Append(strErrorName);

					_mkdir(strDstFileDir);


					Strimage.Format(" FOV%03d %s.%s", info.nFov, str, fileExtension.c_str());

					strDstFileDir = strDstFileDir + "\\" + strType[info.cameraId] + Strimage;
					int error;
					if (theApp.m_bSaveOnlineJpegImages)
						error = SaveJPGFile(strDstFileDir, *info.image, theApp.m_nSaveJpegQualityImages);
					else
						error = SaveGrayScaleBMPFile(strDstFileDir, *info.image);


					if (error)
						OutputDebugLogTo(0, TRUE, "-----ERROR : Failed to save Raw image %s", strDstFileDir);

				}
				else if (!nErrorCode ) {

					strDstFileDir.Append("Pass\\");
					_mkdir(strDstFileDir);
					strDstFileDir.Append(strType[info.cameraId] + "\\");
					_mkdir(strDstFileDir);
					Strimage.Format(" FOV%03d %s.%s", info.nFov, str, fileExtension.c_str());

					strDstFileDir = strDstFileDir + "\\" + strType[info.cameraId] + Strimage;
					int error;
					if (theApp.m_bSaveOnlineJpegImages)
						error = SaveJPGFile(strDstFileDir, *info.image, theApp.m_nSaveJpegQualityImages);
					else
						error = SaveGrayScaleBMPFile(strDstFileDir, *info.image);


					if (error)
						OutputDebugLogTo(0, TRUE, "-----ERROR : Failed to save Raw image %s", strDstFileDir);
				}

				delete info.image;
				info.image = NULL;
			}

			//If image saving is not completed when application is closing
			if (theApp.m_bExitApp && bCloseApplication) {
				size_t nUnsavedImages = 0;

				EnterCriticalSection(&theApp.m_CriticalSectionDeepLearnSaveImage);

				nUnsavedImages = theApp.saveDeepLearnImageQueue.size();

				LeaveCriticalSection(&theApp.m_CriticalSectionDeepLearnSaveImage);

				if (nUnsavedImages > 0) {

					CString strClosingMessage;

					strClosingMessage.Format("%u failed images are unsaved.\nDo you want to close application immediately?", nUnsavedImages);

					if (AfxMessageBox(strClosingMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
						bImagePresence = false;

					bCloseApplication = false;
				}
			}
		}
		while (bImagePresence);

		if (theApp.m_bExitApp) break;

	}
	SetEvent(theApp.eventDeepLearnSaveImageKilled);

	return 0;
}

//UINT BarcodeTriggerThread(LPVOID )
//{
//	
//
//	try {
//
//		BOOL ON = TRUE;
//		BOOL OFF = FALSE;
//		UINT nIOStatus = 0;
//		BOOL bScannerCylinder01EXT = 0;
//		BOOL bScannerCylinder02EXT = 0;
//		BOOL bScannerCylinder01RET = 0;
//		MotionParameters & motionParameters = theApp.motionParameters;
//		IOControl & ioControl = theApp.ioControl;
//
//		while (1) {
//
//			//			theApp.m_bScanning = FALSE;
//
//			if (WaitForSingleObject(theApp.EventStartBarcodeTrigger, INFINITE) != WAIT_OBJECT_0) {
//				OutputDebugLogTo(0, TRUE, "----- ERROR : Thread manipulation, Scan BARcode Thread has been closed");
//				break;
//			}
//
//			if (theApp.m_bExitApp) break;
//
//			while(!bScannerCylinder02EXT) {
//				int nError = theApp.ioControl.SetscannerCylinder02Extension();
//				if(nError/* == -1*/) {
//					AfxMessageBox("Unable to set Scanner Extension 02 Please Retry To continue ", MB_RETRYCANCEL);
//					bScannerCylinder02EXT = FALSE;
//				}
//				else
//					bScannerCylinder02EXT = TRUE;
//
//			}
//			while (!bScannerCylinder01EXT) {
//				int  nError = theApp.ioControl.SetscannerCylinder01Extension();
//			
//				if (nError /*== -1*/)
//				{
//					AfxMessageBox("Unable to set Scanner Extension 01 Please Retry To continue ", MB_RETRYCANCEL);
//					bScannerCylinder01EXT = FALSE;
//				}
//				else
//					bScannerCylinder01EXT = TRUE;
//				
//				if (nError) {
//					OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_SCANNER_CYLINDER_EXT);
//						}
//			
//			}
//			CString strDeviceID;
//			BOOL bAllowScan = TRUE;
//			//Trigger Barcode Reader 
//			int nError = theApp.QRBarcodeReader.GetDeviceScanningStatus(strDeviceID, bAllowScan);
//	
//			if (!nError) {
//
//				theApp.QRBarcodeReader.GetDeviceInfo(strDeviceID);
//				//	m_strTeachBarcodeID = strTeachBarcodeID;
//				OutputDebugLogTo(6, TRUE, "Final ID: %s", strDeviceID);
//				OutputDebugLogTo(9, TRUE, "ret");
//				//	nError = theApp.ioControl.SetscannerCylinder01Retension();
//				if (nError) {
//					OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_SCANNER_CYLINDER_RET);
//				}
//			
//			}
//			else
//			{
//				theApp.runQRBarcodeFailSequence();
//				OutputDebugLogTo(6, TRUE, "Final ID: %s", theApp.strInspectDeviceBarcodeID);
//			//	nError = theApp.ioControl.SetscannerCylinder01Retension();
//				
//			}
//			while (!bScannerCylinder01RET) {
//				nError = theApp.ioControl.SetscannerCylinder01Retension();
//
//				if (nError /*== -1*/)
//				{
//					AfxMessageBox("Unable to set Scanner Retension 01 Please Retry To continue ", MB_RETRYCANCEL);
//					bScannerCylinder01RET = FALSE;
//				}
//				else
//					bScannerCylinder01RET = TRUE;
//			}
//
//			Sleep(100);
//
//			bScannerCylinder01EXT = FALSE;
//			bScannerCylinder02EXT = FALSE;
//			bScannerCylinder01RET = FALSE;
//			SetEvent(theApp.m_eventBarcodeTriggerDone);
//
//		}
//
//		theApp.m_bBiasingSequenceEnabled = FALSE;
//
//	}
//	catch (std::exception & ex) {
//		OutputDebugLogTo(0, FALSE, "%s exception occured in Biasing thread", ex.what());
//	}
//	catch (...) {
//		OutputDebugLogTo(0, FALSE, "Unknown exception occured in Biasing thread");
//	}
//	
//	return 0;
//	
//
//
//}





