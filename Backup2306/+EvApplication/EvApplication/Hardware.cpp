#include "StdAfx.h"
#include "Hardware.h"
#include "Application.h"
#include "TrackHandler.h"
#include "AppView.h"
#include "AppDoc.h"
#include <direct.h>
#include <algorithm>
#include "StatisticsView.h"
#include "ScanBarcode.h"
#include "CameraCode.h"
#include "ImageSelectionDlg.h"
#include "MainFrame.h"
#include "MotionControlDialog.h"
#include "namespace_utilities_function.h"
#include "GlobalBackgroundThread.h"

int Grab(CAppDoc *pDoc, BOOL bDisplay) 
{
	CApplication* pApp = (CApplication*) AfxGetApp();
	CTrackHandler* pTrack = pDoc->m_pTrack;

	if(pTrack->m_bCamEnable) {
		CPerformanceTimer timer;
		if(pDoc->m_pApp->m_bLightCtrlDlgOpen)
			pTrack->RefreshLight(pDoc->m_nDocIdx);
		CImageBuffer* pGrabbedBuffer;
		if(pTrack->m_FGResource.lpImageGrab(pTrack->m_FGResource.pCamModule, &pGrabbedBuffer))
			return -1;
		pDoc->CopyGrabbedBuffer(pGrabbedBuffer);

	///if(pDoc->m_lDebugFlag&DEBUG_TIME_EXT||pDoc->m_lDebugFlag&DEBUG_TIME)
		OutputDebugLogTo(2+pTrack->m_nTrack/5, TRUE, "[Doc%d] Total Snap Time: %5.1fms", pDoc->m_nDocIdx+1, timer.GetElapsedMSec());
		if(bDisplay)
			pDoc->m_ImageViewInfo.pImageView->RedrawWindow();
	}
	else {
		OutputDebugLogTo(2+pTrack->m_nTrack/5, TRUE, "[Doc%d] Snap not available", pDoc->m_nDocIdx+1);
		//CImageBuffer buf;
		//CString str;
		//str.Format("C:\\Users\\JY\\Desktop\\NewBio\\Test\\a (%d).bmp", pDoc->m_nDocIdx+1);
		//LoadGrayScaleBMPFile(str, buf);
		//pDoc->CopyGrabbedBuffer(&buf);

		//if(bDisplay)
		//	pDoc->m_ImageViewInfo.pImageView->RedrawWindow();
	}
	
	return 0;
}

UINT TrackLiveAll(LPVOID pParam)
{
	CApplication* pApp = (CApplication*) pParam;
	int nError;
	BOOL bHome = FALSE;

	while(WaitForSingleObject(pApp->m_pTracks[0].m_EventKillLive, 100) == WAIT_TIMEOUT) {
		
		if(!pApp->m_bStream) {
			pApp->m_bStream = TRUE;
		
//			nError = pApp->InspectionScanSequenceOffline(bHome);
			bHome = TRUE;
			if(nError) {
				OutputDebugLogTo(9,TRUE,"TrackLiveAll Error");//// for testing
				//break;
			}
		}
	}

	for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++)
		pApp->m_pTracks[nTrack].m_bLiveImage = FALSE;
	pApp->m_bStream = FALSE;

	return 0;
}

UINT TrackLiveImage(LPVOID pParam)
{
	CApplication* pApp = (CApplication*) AfxGetApp();
	int nError=-1;
	CAppDoc* pDoc = (CAppDoc*)pParam;
	CTrackHandler* pTrack = pDoc->m_pTrack;

	while(WaitForSingleObject(pDoc->m_pTrack->m_EventKillLive, 30) == WAIT_TIMEOUT) {
		//nError = pDoc->CameraGrab();
		if(!pApp->m_bSnap) {
			pApp->m_bSnap = TRUE;
			nError = pDoc->CameraGrabUsingExposure();

			if(nError=0) {
				pDoc->m_pTrack->m_bLiveImage = FALSE;
				break;
			}
		}
	}
	pTrack->m_bLiveImage = FALSE;
	
	return 0;
}

UINT TrackLiveImageSingle(LPVOID pParam)
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	int nError = -1;
	CAppDoc* pDoc = (CAppDoc*)pParam;
	CTrackHandler* pTrack = pDoc->m_pTrack;

	while (WaitForSingleObject(pDoc->m_EventKillLiveSingle, 30) == WAIT_TIMEOUT) {
		if (!pApp->m_bSnap) {
			pApp->m_bSnap = TRUE;
			nError = pDoc->CameraGrabSingle();

			if (nError = 0) {
				pDoc->m_bLiveImageSingle = FALSE;
				break;
			}
		}
	}

	pDoc->m_bLiveImageSingle = FALSE;
	if (pApp->m_bSnap)
		pApp->m_bSnap = FALSE;

	return 0;
}

UINT TrackLightCalibrationLiveImage(LPVOID pParam)
{
	CApplication* pApp = (CApplication*) AfxGetApp();
	int nError=-1;
	CPerformanceTimer m_Time,m_Time1;
	CAppDoc* pDoc = (CAppDoc*)pParam;
	int nDoc = pDoc->m_nDocIdx;
	CTrackHandler* pTrack = pDoc->m_pTrack;
	OutputDebugLogTo(7,TRUE,"TrackLightCalibrationLiveImage Started");
	while(WaitForSingleObject(pDoc->m_pTrack->m_EventKillLiveCalibration, 500) == WAIT_TIMEOUT) {
		//m_bCalib
		//nError = pDoc->CameraGrab();
		OutputDebugLogTo(7,TRUE,"TrackLightCalibrationLiveImage inside 1 bCalib = %d",pApp->m_bCalib);
		if(!pApp->m_bCalib) {
			pApp->m_bCalib = TRUE;
			OutputDebugLogTo(7,TRUE,"TrackLightCalibrationLiveImage inside 2 bCalib = %d",pApp->m_bCalib);
			nError = pDoc->CameraGrab();	
			OutputDebugLogTo(7,TRUE,"TrackLightCalibrationLiveImage inside 3 bCalib = %d",pApp->m_bCalib);
			if(nError) {
				pDoc->m_pTrack->m_bLiveCalibration = FALSE;
				//
				break;
			}
		}
	}
	pTrack->m_bLiveCalibration = FALSE;
	
	return 0;
}


UINT DocInspectCycle(LPVOID pParam)
{
	CApplication* pApp = (CApplication*) AfxGetApp();
	CAppDoc* pDoc = (CAppDoc*) pParam;

	int nTrackIndex = -1;
	if(GetKeyState(VK_SHIFT) & 0x8000) {
		pApp->m_LotSummary.InitDisplaySummary();
		nTrackIndex = pDoc->m_pTrack->m_nTrack;
	}

	int nError;
	BOOL bCameraOn = pDoc->m_pTrack->m_bCamEnable && pDoc->m_bCameraActive;

	while (WaitForSingleObject(pDoc->m_pTrack->m_EventKillInspCycle, 10) == WAIT_TIMEOUT) {
		if(bCameraOn) {
			//nError = pApp->m_bSynchronousGrabEnable ? pApp->SynchronousCameraSnap(FALSE) : 
			//	pDoc->CameraGrab(FALSE);
			nError = pDoc->CameraGrabUsingExposure();
			if(nError) {
				pDoc->m_pTrack->m_bContInsp = FALSE;
				break;
			}
		}
		
		//nError = pDoc->Inspect(0);
		nError = pDoc->InspectAllDoc(0);
	
		/*if(bCameraOn) {
			CAppDoc* pDoc1;
			for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++) {
				pDoc1 = pApp->m_pTracks[0].m_pDoc[0];
				if(pDoc1->m_pRemoteInspDoc)
					DrwUpdate(&pDoc1->m_ImageViewInfo);
			}
		}*/

		if(nTrackIndex >= 0) {
			//pApp->UpdateLotSummary(nTrackIndex, nError);
			pApp->UpdateStatisticsWindow();
		}
	}

	return 0;
}

UINT TrackInspectCycle(LPVOID pParam)
{
	///Udupa; TODO
	//CPerformanceTimer m_Time,m_Time1;
	//CTrackHandler* pTrack = (CTrackHandler*) pParam;
	//CEvTabView* pTabView = (CEvTabView*) (((CApplication*)AfxGetApp())->m_pFrames[pTrack->m_nTrack]->GetActiveView());
	//int nError;

	//while (WaitForSingleObject(pTrack->m_EventKillInspCycle, 100) == WAIT_TIMEOUT) {
	//	for(int nDoc=0; nDoc<pTrack->m_nTotalDocs; nDoc++) {
	//		if(WaitForSingleObject(pTrack->m_EventKillInspCycle, 10)==WAIT_OBJECT_0)
	//			return 0;
	//		if(pTrack->m_pDoc[nDoc]->m_lDebugFlag&DEBUG_PRINT)
	//			OutputDebugLog("====================================================");
	//		nError = 0;
	//		if(pTrack->m_bCamEnable) {
	//			nError = pTrack->m_pDoc[nDoc]->CameraGrab(pTrack->m_pDoc[nDoc]->m_lDebugFlag!=0);
	//			if(nError) {
	//				pTrack->m_bTrackContInsp = FALSE;
	//				return 0;
	//			}
	//		}
	//		
	//		if(((CApplication*)AfxGetApp())->m_bTrackProcess)
	//			pTabView->ActivatePage(nDoc);
	//		nError = pTrack->m_pDoc[nDoc]->Inspect(nError);
	//		if(nError && !((CApplication*)AfxGetApp())->m_bTrackProcess)
	//			pTabView->ActivatePage(nDoc);
	//	}
	//}
	return 0;
}


UINT TrackAutoRunThread(LPVOID pParam)
{
	
	CTrackHandler* pTrack = (CTrackHandler*) pParam;
	CAppDoc* pDoc = (CAppDoc*) pParam;
	
	int nDoc;

	ResetEvent(pTrack->m_EventKillAutoRun);
	while (WaitForSingleObject(pDoc->m_pTrack->m_EventKillAutoRun, pDoc->m_pTrack->m_nAutoRunDelay) != WAIT_OBJECT_0) {
		for(nDoc=0; nDoc<pDoc->m_pTrack->m_nTotalDocs; nDoc++) {
			if(pDoc->m_pTrack->m_pDoc[nDoc]->m_bWaiting) {
				pDoc->m_pTrack->m_pDoc[nDoc]->Continue();
			}
		}
	}
	return 0;
}


UINT TrackCommandThread(LPVOID pParam)
{
	CTrackHandler* pTrack = (CTrackHandler*)pParam;
	CApplication* pApp = (CApplication*)AfxGetApp();
	CAppDoc* pDoc=NULL;
	int nError;

	while(1) {
		
		if(WaitForSingleObject(pApp->m_EventRun, INFINITE) != WAIT_OBJECT_0) {
			OutputDebugLogTo(0, TRUE, "----- ERROR : Track Command Thread Synchronization. Thread is closing...");
			return 0;
		}

		if(pApp->m_bExitApp)
			return 0;

		if(pApp->m_nActiveTrackCommandIdx < 0 && pApp->m_nActiveTrackCommandIdx >= pApp->m_nTotalTracks) {
			ResetEvent(pApp->m_EventRun);
			continue;
		}

		pTrack = &pApp->m_pTracks[pApp->m_nActiveTrackCommandIdx];

		pDoc = (CAppDoc*)pTrack->m_pActiveDoc;
		if(!pDoc) {
			OutputDebugLog("TrackCommandThread closed");
			return 0;
		}

		if(pDoc->m_lDebugFlag&DEBUG_PRINT)
			OutputDebugLog("====================================================");

		switch (pApp->m_nCommand) {
		case TEACH :
			nError = 0;
			pApp->m_bTeaching = TRUE;
			pDoc->Teach();
			pApp->m_bTeaching = FALSE;
			//pApp->m_pMainWnd->SendMessage(WM_LOAD_RECIPE); // update teach value in teach params, safe?
			for(size_t i = 0; i < pDoc->m_nFovCompletedTeach.size(); i++) {
				pApp->m_pMainWnd->SendMessage(WM_LOAD_SINGLE_FOV_RECIPE, (WPARAM) pTrack->m_nTrack, (LPARAM) pDoc->m_nFovCompletedTeach[i]);
			}
			pDoc->m_nFovCompletedTeach.clear();	
			break;

		case INSPECT :
			nError = 0;
			pApp->m_bInspecting = TRUE;
			pDoc->m_bInspecting = TRUE;
			pApp->m_pTracks[pTrack->m_nTrack].m_pDoc[pDoc->m_nDocIdx]->InspectAllDoc(nError);
			pApp->m_bInspecting = FALSE;
			pDoc->m_bInspecting = FALSE;
			SetEvent(pDoc->m_EventInspEnd);
			break;

		case INSPECT_ALL:
			nError = 0;
			pApp->m_bInspecting = TRUE;
			pDoc->m_bInspecting = TRUE;
			
			for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++)
				pApp->m_pTracks[nTrack].m_pDoc[0]->InspectAllDoc(nError);
			pApp->m_bInspecting = FALSE;
			pDoc->m_bInspecting = FALSE;
			pApp->m_bInspectionAll = !pApp->m_bInspectionAll;
			SetEvent(pDoc->m_EventInspEnd);
			break;
		
		case CALIBRATE :
			nError = 0;
			pDoc->SetDotCalibration();
			break;
		case CALIBRATE_LIGHT :
			pDoc->SetLightCalibration();
			
			break;

		case CLAMP_SEQUENCE:
			pApp->m_bClampingSequenceEnabled = TRUE;

			//1. Vaccum Up
			pApp->ioControl.VacuumUp();

			//2. Vaccum On
			pApp->ioControl.VacuumOn();

			//3. Track Clamping
			pApp->ioControl.ClampingSequence();

			pApp->m_bClampingSequenceEnabled = FALSE;
			break;

		case UNCLAMP_SEQUENCE:
			pApp->m_bUnClampingSequenceEnabled = TRUE;

			//1. Track Unclamp
			pApp->ioControl.UnClampingSequence();

			//2. Vaccum Off
			pApp->ioControl.VacuumOff();

			//3. Vaccum Down
			pApp->ioControl.VacuumDown();

			pApp->m_bUnClampingSequenceEnabled = FALSE;
			break;

		case BARCODE_SEQUENCE:
			pApp->m_bBarcodeSequenceEnabled = TRUE;
			pApp->BarcodeSequence();
			pApp->m_bBarcodeSequenceEnabled = FALSE;
			break;

		case SAVE_ALL_IMAGES:
			pApp->SaveAllImages();
			break;

		case KILL_CMD_THREAD :
			return 0;
		}
	}
	return 0;
}

UINT VisionResetThread(LPVOID pParam) // Changed to End lot Thread
{
	CApplication *pApp = (CApplication*)AfxGetApp();
	if (pApp->m_lCOMMInCardNo<0 || pApp->m_lCOMMInPortID<0) {
		ASSERT(!"m_lCOMMInCardNo or m_lCOMMInPortID not initialised!!!"); 
		return -1;
	}

	while(1) {
		WaitForSingleObject(pApp->m_eventOnline, INFINITE);
		if(pApp->m_bExitApp) {
			OutputDebugLog("VisionResetThread (End Lot) closed");
			return 0;
		}
		if(pApp->m_strIOInterfaceCard != "none")
		{		
			if(pApp->m_CommModule.WaitForActiveDIInterrupt(pApp->m_lCOMMInCardNo, pApp->m_lCOMMInPortID, INFINITE, 1) == 0)
			{
				OutputDebugLog("VisionResetThread:ClearActiveDIInterrupt (End Lot) done");
				pApp->m_CommModule.ClearActiveDIInterrupt(1);
				
				// Record Lot Stats
				//pApp->RecordLotStats();

				// Reset Counter
				pApp->m_nGoodACounter = 0;
				pApp->m_nGoodBCounter = 0;
				pApp->m_nFailCounter = 0;

				//Reset Statistic
				pApp->m_LotSummary.InitDisplaySummary();
				pApp->UpdateStatisticsWindow();

			}
		}
	}
}


///Udupa; New version
///Improvements:
/// 1. Sort the files in ascending order of the filename (earlier, it was arbitrary, as dictated by the SDK, depending on the hardware)
/// 2. Fix an issue of erratic behavior of inspection results when executing parallely for multiple tracks
/// 3. Enable parallel inspection of different folders in multiple tracks
/// 4. Got rid of lot of redundancies
//DWORD WINAPI TrackInspectImages(LPVOID pParam)
UINT TrackInspectImages(LPVOID pParam)
{
	CApplication* pApp = (CApplication*) AfxGetApp();
	CAppDoc* pDoc = (CAppDoc*) pParam;
	CTrackHandler* pTrack = (CTrackHandler*) pDoc->m_pTrack;
	int nDieStats;
	pApp->m_nStepTrack = pTrack->m_nTrack;
	pApp->m_nStepDoc = pDoc->m_nDocIdx;

	CStringArray arrayFilePaths;
	CStringArray arrayFileFolders;
	CString strFolder, strFileName, strFilePath;
	CArray <CString, CString> strFileNames;
	CArray <CString, CString> strFilePaths;
	CString strType[] = { "TOP", "THR"};

	theApp.m_bScanning = TRUE;
	CFileFind FileFind;
	strFolder = pTrack->m_strStoredImagesPath.Trim();
	if(strFolder.Right(1) != "\\")
		strFolder.Append("\\");
	strFilePath = strFolder + "*.bmp";

	BOOL bNextFile = FileFind.FindFile(strFilePath.GetBuffer(0));
	strFilePath.ReleaseBuffer();
	// 3. Create arrays for scanning

	int nTotalImageNo;
	bool bLotFound;

	theApp.m_pImageSelectionDlg->compareStripId("3056301000492362");
	theApp.m_pImageSelectionDlg->setLotStartTime();
	theApp.m_pImageSelectionDlg->SetBarcodeID("3056301000492362");

	for (int nTrack = 0; nTrack < theApp.m_nTotalTracks; nTrack++) {

		if (nTrack == STATION_THR)
			nTotalImageNo = theApp.motionParameters.GetTotalImageGrabsTHR();
		else if (nTrack == STATION_TOP)
			nTotalImageNo = theApp.motionParameters.GetTotalImageGrabsTOP();

		for (int nDoc = 0; nDoc < theApp.m_pTracks[nTrack].m_nTotalDocs; nDoc++) {
			if (theApp.m_pTracks[nTrack].m_pDoc[nDoc]->nTotalGrabNo != nTotalImageNo) {
				theApp.m_pTracks[nTrack].m_pDoc[nDoc]->nTotalGrabNo = nTotalImageNo;

				theApp.m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab.clear();
				theApp.m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab.resize(nTotalImageNo);

				CSize csImageFOV = theApp.m_pTracks[nTrack].m_pDoc[0]->m_Buffer.GetImgSize();

				for (size_t j = 0; j < theApp.m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab.size(); j++)
					theApp.m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab[j].Allocate(csImageFOV);
			}
		}
	}

	theApp.m_pImageSelectionDlg->ResetDieStates();

	if (!pApp->m_bSetForAllTracks)
		GetImageFilePaths(strFolder, arrayFilePaths, arrayFileFolders, TRUE, strType[pTrack->m_nTrack]);
	else
		GetImageFilePaths(strFolder, arrayFilePaths, arrayFileFolders, TRUE,"");

	int nCount = arrayFileFolders.GetCount();
	///int nTotalCount = arrayFileNames.GetSize();
	INT_PTR nTotalCount = arrayFilePaths.GetSize();
	int nTrackIndex = pTrack->m_nTrack;
	
	OutputDebugLogTo(nTrackIndex+1, FALSE, " Stored Images Inspection"); 
	OutputDebugLogTo(nTrackIndex+1, FALSE, "=========================="); 
	OutputDebugLogTo(nTrackIndex+1, FALSE, "Number of files found for inspection: %d\n", nTotalCount); 

	if(nTotalCount > 0) {
		CString* fileNames = new CString[nTotalCount];
		for(int k = 0; k < nTotalCount; k++)
			fileNames[k] = arrayFilePaths.GetAt(k);
		arrayFilePaths.RemoveAll();

		CString strStatus;
		int nError = 0;
		int nPassCount = 0;
		int nFailCount = 0;

		pApp->InitializeLotStats();

		int i;
		BOOL bFovFail;
		BOOL bBreak = FALSE;
		int nPos = -1;
		BOOL waitInspection = FALSE;
		CPerformanceTimer inspectionTime;
		int nDoc;

		int nTotalLoopCount = 0;
		CPerformanceTimer inspectionTimeTotal;

		inspectionTimeTotal.Reset();
		//pTrack->m_pDoc[0]->DuplicateInspectionOnline();
		for (int nThreadDuplicate = 0; nThreadDuplicate < pTrack->getDuplicateInspectionThreadCount(); nThreadDuplicate++) {
			SetEvent(pTrack->eventDuplicateInspection[nThreadDuplicate]);
		}
		std::vector < HANDLE > eventDuplicateInspection;
		for (int nThreadId = 0; nThreadId < pTrack->getDuplicateInspectionThreadCount(); nThreadId++) {
			eventDuplicateInspection.push_back(pTrack->eventDuplicateInspectionDone[nThreadId]);
		}

		DWORD dwError = WaitForMultipleObjects(eventDuplicateInspection.size(), eventDuplicateInspection.data(), TRUE, INFINITE);
		if (dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {

			OutputDebugLogTo(7, TRUE, "-----ERROR: Duplicate Inspection threads Waiting have been failed.");
			return 0;
		}

		OutputDebugLogTo(nTrackIndex + 1, FALSE, " =============== Prepare Inspection : %.3f ms ===============", inspectionTimeTotal.GetElapsedMSec());

		inspectionTimeTotal.Reset();
		OutputDebugLogTo(nTrackIndex + 1, FALSE, " ===============         Start of Stored Images Inspection    ===============");

		do {
			inspectionTime.Reset();

			std::vector <BOOL> waitEventInspection;
			waitEventInspection.resize(pTrack->getInspectionThreadCount(), FALSE);

			CString strType[] = { "TOP_STATION", "THR_STATION" , "ENCAP_STATION" };
			int nStationId = pTrack->m_nTrack;
			int nDocumentId = 0;
			int nRealGrabID = 0;
			int nScanRow = nStationId == 0 ? pApp->motionParameters.getScanRowNoTop() : pApp->motionParameters.getScanRowNoTHR();
			int nScanCol = nStationId == 0 ? pApp->motionParameters.getScanColumnNoTop() : pApp->motionParameters.getScanColumnNoTHR();
			nTotalCount = pApp->m_pTracks[nStationId].m_pDoc[nDocumentId]->imageGrab.size();
			
			nPos = fileNames[0].ReverseFind('\\') + 1;
			strFolder = fileNames[0].Left(nPos);
			int nDieRow =0 , nDieColumn =0, nDieFOV=0, nDieNo=0;

			for (int i = 0; i < nScanRow; i++) {
				for (int j = 0; j < nScanCol; j++, nRealGrabID++) {
					pDoc->m_bAccurateResize = TRUE; 
					if (i % 2 == 0) {
						nDieNo = (j + (i / (nScanRow / 2))*nScanCol + 1);
						nDieColumn = j + 1;
					}
					else {
						nDieNo = (nScanCol-j + (i / (nScanRow / 2))*nScanCol );
						nDieColumn = nScanCol -j;
					}
					nDieRow = i% (nScanRow/2) + 1;
					nDieFOV = i% (nScanRow/2) + 1;
					strFilePath.Format("%s\\%s Doc%02d DieNo%03d Row%03d Col%03d FOV%03d.bmp", strFolder, strType[nStationId], nDocumentId, nDieNo, nDieRow, nDieColumn, nDieFOV);
					bFovFail = FALSE;

					nDoc = nDocumentId;
					
					bool bEnableInspection = true;

					if (pTrack->m_nTrack == STATION_TOP)
						pApp->motionParameters.GetDeviceDetailsTOP(nRealGrabID, nDieRow, nDieColumn, nDieNo, nDieFOV);
					else if (pTrack->m_nTrack == STATION_THR)
						pApp->motionParameters.GetDeviceDetailsTHR(nRealGrabID, nDieRow, nDieColumn, nDieNo, nDieFOV);


					//	if (!bEnableInspection)
					//		continue;

					bool bStatus = true;
					nError = pTrack->m_pDoc[nDocumentId]->EnableInspection(nDieFOV, nDieNo, bStatus);
					if (nError) {
						OutputDebugLogTo(8, TRUE, "TOP Station: Failed to get Inspection status for Die Id= %d, Fov Id %d", nDieNo, nDieFOV);
						bStatus = true;
					}

					if (nStationId != pTrack->m_nTrack || !bStatus)
						continue;

					nError = LoadGrayScaleBMPFile(strFilePath, pApp->m_pTracks[nStationId].m_pDoc[nDocumentId]->imageGrab[nRealGrabID]);
					Sleep(pApp->m_nInSpectionDelay);//To simulate Grabing Time

					if (nError)
						continue;

					int threadId;
					if (pTrack->getInspectionThreadCount() > 0) {

						EnterCriticalSection(&pTrack->CriticalSectionScannedImage);
						std::vector < size_t > pendingImageCount(pTrack->getInspectionThreadCount());

						for (size_t id = 0; id < pendingImageCount.size(); ++id)
							pendingImageCount[id] = pTrack->m_pDoc[nDocumentId]->grabbedImageId[id].size();

						threadId = nDieNo % pTrack->getInspectionThreadCount();
						pTrack->m_pDoc[nDocumentId]->grabbedImageId[threadId].push(nRealGrabID);
						LeaveCriticalSection(&pTrack->CriticalSectionScannedImage);
						SetEvent(pTrack->eventDoInspection[threadId]);
						Sleep(pApp->m_nInSpectionDelay);

						waitEventInspection[threadId] = TRUE;
					}
					else {
						EnterCriticalSection(&pTrack->CriticalSectionScannedImage);
						pTrack->m_pDoc[nDocumentId]->grabbedImageId[0].push(nRealGrabID);
						LeaveCriticalSection(&pTrack->CriticalSectionScannedImage);
						SetEvent(pTrack->eventDoInspection[0]);

						waitEventInspection[0] = TRUE;
					}

					waitInspection = TRUE;
				}
				//pApp->motionParameters.getRealGrabId(strFileName, nStationId, nDocumentId, nRealGrabID);
			}

			if (waitInspection) {
				//--. Wait for inspection completion

				std::vector < HANDLE > eventInspection;

				if (pTrack->getInspectionThreadCount() > 0) {
					for (int nThreadId = 0; nThreadId < pTrack->getInspectionThreadCount(); nThreadId++) {
						if (waitEventInspection[nThreadId])
							eventInspection.push_back(pTrack->eventInspectionDone[nThreadId]);
					}

					DWORD dwError = WaitForMultipleObjects(eventInspection.size(), eventInspection.data(), TRUE, INFINITE);
					if (dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {

						OutputDebugLogTo(7, TRUE, "-----ERROR: Image Inspection threads Waiting have been failed.");
						return 0;
					}
					
					eventInspection.clear();			
				}
				else {
					
					DWORD dwError = WaitForSingleObject(pTrack->eventInspectionDone[0], INFINITE);
					if (dwError == WAIT_FAILED || dwError == WAIT_TIMEOUT) {

						OutputDebugLogTo(7, TRUE, "-----ERROR: Image Inspection threads Waiting have been failed.");
						return 0;
					}
				
				}

				theApp.m_pImageSelectionDlg->setStripEndTime();
				theApp.pGlobalBackgroundThread->saveEndLotInformation();

				theApp.motionParameters.resetInspection();

				if (pTrack->m_nTrack == STATION_THR) {
					int nDieNoPerX, nDIeNoPerY, nFovNoPerX, nFovNoPerY;
					theApp.motionParameters.GetMappingParameters(nDieNoPerX, nDIeNoPerY, nFovNoPerX, nFovNoPerY);
		//			theApp.m_pImageSelectionDlg->UpdateDieCount();
					theApp.UpdateUPH(inspectionTime.GetElapsedMSec(), inspectionTime.GetElapsedMSec(), nDieNoPerX * nFovNoPerX * nDIeNoPerY * nFovNoPerY);
					theApp.UpdatePrintBar(theApp.m_pImageSelectionDlg->badDeviceCount());
					theApp.UpdateStatistics(theApp.m_pImageSelectionDlg->goodDieCount(), theApp.m_pImageSelectionDlg->badDieCount());

				}
			}


			if(theApp.m_bStoredImagesLoop)
				theApp.m_pImageSelectionDlg->ResetDieStates();

			nTotalLoopCount++;
		} while (theApp.m_bStoredImagesLoop);

		double dAutoRunTime = inspectionTimeTotal.GetElapsedMSec() / 1000;
		OutputDebugLogTo(nTrackIndex + 1, FALSE, " ==== End of Stored Images Inspection (Time: %.3f second - Loop Count: %d) ===", dAutoRunTime, nTotalLoopCount);

		//Update mapping to SECS/GEM mapping
		/*theApp.getGEMController().mapData.inspectionResultMap = theApp.m_pImageSelectionDlg->getDefectCode();

		theApp.getGEMController().mapData.nSubstrateColumnCount = 1;
		theApp.getGEMController().mapData.nSubstrateRowCount = 2;

		theApp.getGEMController().mapData.nUnitColumnCount = 30;
		theApp.getGEMController().mapData.nUnitRowCount = 5;
		theApp.getGEMController().UpdateSubstrateResults2("12345678");*/
	}

	if (pApp->m_bSetForAllTracks)
	{
		for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++) {
			for(int nDoc = 0;nDoc<pApp->m_nTotalViewDocs[nTrack];nDoc++)
				pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspSavedImage = FALSE;
			pApp->m_bInspectionAll = FALSE;
		}
	}
	else
		pDoc->m_bInspSavedImage = FALSE;
	pApp->m_bInspecting = FALSE;

	ResetEvent(pTrack->m_EventKillInspectImages);
	SetEvent(pTrack->m_hInspectImagesKilled);
	theApp.m_bScanning = FALSE;

	return 0;
}

UINT TrackUpdateThread(LPVOID pParam)
{
	CTrackHandler* pTrack = (CTrackHandler*) pParam;
	CApplication* pApp = (CApplication*)AfxGetApp();
	CAppDoc* pDoc=NULL;
//	int nError;
	bool bLotChangedAck = true;
	while(1) {
		WaitForSingleObject(pTrack->m_EventUpdate, INFINITE);
		pDoc = (CAppDoc*)pTrack->m_pActiveUpdateDoc;
		if(!pDoc) {
			OutputDebugLog("TrackUpdateThread closed");
			return 0;
		}

		if (pDoc->m_lDebugFlag&DEBUG_PRINT)
			OutputDebugLog("====================================================");
	}
}

//UINT TrackImageUpdate(LPVOID pParam)
//{
//
//	CApplication* pApp = (CApplication*)AfxGetApp();
//	//CTrackHandler* pTrack = (CTrackHandler*) pParam;
//	CAppDoc* pDoc=(CAppDoc*) pParam;;
//	//CMainFrame *pMainFrame;
//	//int cstring float
//
//	while(TRUE)
//	{
//		WaitForSingleObject(pDoc->m_EventImageUpdate,INFINITE);
//		if (pApp->m_bExitApp) {
//			OutputDebugLog("TrackImageUpdate closed");
//			return 0;
//		}
//		OutputDebugLogTo(8, TRUE, "Copy time: Before");
//		
//		//((CMainFrame*) pApp->m_pMainWnd)->m_dlgPane->m_wndDlg.CopyImagesToStats(pDoc[0].bufImageUpdate);
//		OutputDebugLogTo(8, TRUE, "Copy time After");
//	}
//
//   return 0;
//}

UINT TrackUpdateImage(LPVOID pParam)
{
	CApplication* pApp = (CApplication*) AfxGetApp();
	CTrackHandler* pTrack = (CTrackHandler*) pParam;
	CAppDoc* pDoc = NULL;
	BOOL bDisplay;
	int nError[3];

	while(TRUE) {
		WaitForSingleObject(pTrack->m_hTrackEventUpdateImage, INFINITE);
		pDoc = (CAppDoc*) pTrack->m_pActiveUpdateDoc;
		
		if(!pDoc) {
			OutputDebugLog("TrackUpdateImage closed");
			return 0;
		}

		bDisplay = TRUE;
		if(pApp->m_bOnline)
			bDisplay = FALSE;

		nError[pTrack->m_nTrack] = pTrack->WaitForSnapCompletion(bDisplay, pDoc->m_nDocIdx, TRUE);
		SetEvent(pTrack->m_EventGrabEnd);
		OutputDebugLogTo(2+pTrack->m_nTrack, TRUE, "Track%d [Doc%d] GrabEnd Event Set", pTrack->m_nTrack+1, pDoc->m_nDocIdx+1);
		
		if(!pTrack->m_nTrack && pApp->m_bStream /*&& (pDoc->m_nDocIdx==pApp->m_nTotalViewDocs[pTrack->m_nTrack])*/)
			pApp->m_bStream = FALSE;
		if(pApp->m_bSnap /*&& (pDoc->m_nDocIdx==pApp->m_nTotalViewDocs[pTrack->m_nTrack])*/)
			pApp->m_bSnap = FALSE;

		if(nError[pTrack->m_nTrack])		//// return if WaitForSnapCompletion error
			pTrack->m_nOpticsRecalcMode = 0;
	}
	
	return 0;
}

UINT RefreshLightThread(LPVOID pParam)
{
	CApplication* pApp = (CApplication*) pParam;

	while(TRUE) {
		WaitForSingleObject(pApp->m_EventRefreshLight, INFINITE);

		if(pApp->m_bExitApp) {
			OutputDebugLog("RefreshLightThread closed");
			return 0;
		}

		pApp->m_LightHandler.RefreshLight();
		pApp->m_LightHandler.ResetLightSteps();

		ResetEvent(pApp->m_EventRefreshLight);//// Reset Event Once Refresh Light has done
	}

	return 0;
}

void GetImageFilePaths(CString strStartFolder, CStringArray& arrayFilePaths, CStringArray& arrayFileFolders, BOOL bParseSub,CString strChar)
{
	CString strFilePath;
	strFilePath.Format("%s%s", strStartFolder, bParseSub ? "*.*" : "*.bmp");
	CString strFolder, strFile;

	CFileFind FileFind;
	BOOL bNextFile = FileFind.FindFile(strFilePath);
	while (bNextFile) {
		bNextFile = FileFind.FindNextFile();
		if (FileFind.IsDots())
			continue;

		strFile = FileFind.GetFileName();
		if (FileFind.IsDirectory()) {
			strFolder.Format("%s%s\\", strStartFolder, strFile);
			arrayFileFolders.Add(strStartFolder);
			GetImageFilePaths(strFolder, arrayFilePaths, arrayFileFolders, bParseSub,strChar);
		}
		else if (!strFile.Right(3).CompareNoCase("bmp"))
		{
			if (!strChar.IsEmpty()) {

				if (strFile.Find(strChar) >= 0) {
					arrayFilePaths.Add(strStartFolder + strFile);
				}
			}
			else
				arrayFilePaths.Add(strStartFolder + strFile);

		}

	}
}

UINT LogInspectionData(LPVOID pParam)
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	CAppDoc *pDoc = (CAppDoc*)pParam;;
	int nError;

	while (1) {
		WaitForSingleObject(pDoc->m_EventLogInspectionData, INFINITE);
		if (pDoc->m_bExitLogEnry) {
			OutputDebugLogTo(7, TRUE, "LogInspectionData closed");
			return 0;
		}
		for (int nCount = 0; nCount < pDoc->m_strLogInspectionData.GetCount(); nCount++) {
			pApp->AddLogEvent(pDoc->m_strLogInspectionData.GetAt(nCount), 2,FALSE);
		}
		pDoc->m_strLogInspectionData.RemoveAll();
		ResetEvent(pDoc->m_EventLogInspectionData);
	}
	return 0;
}

UINT TrackAutoFocusing(LPVOID pParam)
{
	int nLoadedDataPercentage = 0;
	if (theApp.m_pProgressDlg != NULL) {
		theApp.m_pProgressDlg->ShowWindow(SW_SHOWNA);
		theApp.m_bProgressDlgOpen = TRUE;
		theApp.m_pProgressDlg->SetStatus("Auto Focusing Process...", nLoadedDataPercentage);
	}


	int nTrack = 0, nDoc = 0, nFov = 0;
	COpticsDlg* pFocusingToolDlg = (COpticsDlg*)pParam;
	nTrack = pFocusingToolDlg->m_nCurrentTrack;
	nDoc = pFocusingToolDlg->m_nCurrentDoc;
	nFov = pFocusingToolDlg->m_nFOVSetting;
	int nMotorAxiz = (nTrack == STATION_TOP) ? ST1_Z_SERVO_MOTOR : ST2_Z_SERVO_MOTOR;


	int nLoadedDataPercentageStepSize = 100 / (2 * pFocusingToolDlg->m_nAutoFocusStepNo);
	int nCurrentZPos = 0;
	theApp.motionParameters.GetCurrentMotorPosition(nMotorAxiz, nCurrentZPos);

	//Move up/down m_nAutoFocusStepNo times
	double dMotorResolution = 0;
	theApp.motionParameters.GetMotorResolution(nMotorAxiz, dMotorResolution);

	int nError = 0;
	std::map<int, double> mapZFocus;

	//Get Focusing Value of current position
	int nGrabCount = 1, nFocusRoiCount = 1;
	int nDummyT, nDummyB;
	pFocusingToolDlg->GetStats(nFocusRoiCount, nGrabCount, nDummyT, nDummyB);

	OutputDebugLogTo(8, TRUE, "TrackAutoFocusing For Track %d, Doc %d, nGrabCount %d, nFocusRoiCount %d", nTrack, nDoc, nGrabCount, nFocusRoiCount);

	if (theApp.m_pTracks[nTrack].m_pDoc[nDoc]->m_bLiveImageSingle) {
		theApp.m_pTracks[nTrack].m_pDoc[nDoc]->CamLiveSingle();
	}

	//Refresh Light For Single Step Light Mode
	if (nTrack == STATION_TOP) {
		theApp.m_pTracks[nTrack].RefreshLightFOV(nDoc, nFov);
	}
	else {
		theApp.m_pTracks[nTrack].RefreshLight(nDoc);
	}
	

	int nGrabSuccessCount = 0;
	double dAverageFocusGrabbed = 0;
	for (int nGrab = 0; nGrab < nGrabCount; nGrab++) {

		theApp.m_pTracks[nTrack].m_nOpticsRecalcMode = 1;
		nError = theApp.m_pTracks[nTrack].EnqueueAsyncGrab();
		if (!nError) {
			theApp.m_pTracks[nTrack].UpdateGrabImage(nDoc);

			double dMean, dNoise, dFocus;
			double dAvgFocus = 0;
			for (int nFocusRoi = 0; nFocusRoi < nFocusRoiCount; nFocusRoi++) {
				OpticsCharacteristics(&theApp.m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer, &theApp.m_pTracks[nTrack].m_rectOptics[nFocusRoi], dMean, dNoise, dFocus);
				dAvgFocus += dFocus;
			}
			dAvgFocus = dAvgFocus / nFocusRoiCount;

			dAverageFocusGrabbed += dAvgFocus;
			nGrabSuccessCount++;
		}
	}
	if (nGrabSuccessCount > 0)
		dAverageFocusGrabbed = dAverageFocusGrabbed / nGrabSuccessCount;

	dAverageFocusGrabbed = dAverageFocusGrabbed * 100;
	OutputDebugLogTo(8, TRUE, "Auto Focusing - Z: %d, Focus: %.5f - Optic Focus: %.5f", nCurrentZPos, dAverageFocusGrabbed, pFocusingToolDlg->m_dOpticsFocus[0]);
	mapZFocus.insert(std::pair<int, double>(nCurrentZPos, dAverageFocusGrabbed));

	//Move Up
	for (int nStep = 0; nStep < pFocusingToolDlg->m_nAutoFocusStepNo; nStep++) {
		int nNextZPos = nCurrentZPos + (nStep + 1)*pFocusingToolDlg->m_dAutoFocusStepSizeMM * dMotorResolution;

		//Validate Z Position before moving (without warning message box)
		if (theApp.motionParameters.IsSoftwareLimits(nMotorAxiz, nNextZPos)) {
			OutputDebugLogTo(8, TRUE, "Auto Focusing - Z Position %d is reach soft limit", nNextZPos);
			break;
		}

		nError = theApp.motionParameters.AbsoluteMove(nMotorAxiz, nNextZPos, theApp.motionParameters.getMovingSpeed(nMotorAxiz));
		if (nError) {
			OutputDebugLogTo(8, TRUE, "Auto Focusing - Failed to move motor axis %d Error %d", nMotorAxiz);
			break;
		}

		nError = theApp.motionParameters.WaitForMotionDone(nMotorAxiz, theApp.getMotionTimeOut());
		if (nError) {
			OutputDebugLogTo(8, TRUE, "Auto Focusing - CMotionControlDialog::Movement[Absolute] Axis%d Error %d", nMotorAxiz, nError);
			break;
		}
		theApp.motionParameters.WaitMotorSettlingTime(nMotorAxiz);

		//Get Focusing Value
		int nGrabSuccessCount = 0;
		double dAverageFocusGrabbed = 0;
		for (int nGrab = 0; nGrab < nGrabCount; nGrab++) {
			theApp.m_pTracks[nTrack].m_nOpticsRecalcMode = 1;
			nError = theApp.m_pTracks[nTrack].EnqueueAsyncGrab();
			if (!nError) {
				theApp.m_pTracks[nTrack].UpdateGrabImage(nDoc);

				double dMean, dNoise, dFocus;
				double dAvgFocus = 0;
				for (int nFocusRoi = 0; nFocusRoi < nFocusRoiCount; nFocusRoi++) {
					OpticsCharacteristics(&theApp.m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer, &theApp.m_pTracks[nTrack].m_rectOptics[nFocusRoi], dMean, dNoise, dFocus);
					dAvgFocus += dFocus;
				}
				dAvgFocus = dAvgFocus / nFocusRoiCount;

				dAverageFocusGrabbed += dAvgFocus;
				nGrabSuccessCount++;
			}
		}
		if (nGrabSuccessCount > 0)
			dAverageFocusGrabbed = dAverageFocusGrabbed / nGrabSuccessCount;

		dAverageFocusGrabbed = dAverageFocusGrabbed * 100;
		OutputDebugLogTo(8, TRUE, "Auto Focusing - Z: %d, Focus: %.5f - Optic Focus: %.5f", nNextZPos, dAverageFocusGrabbed, pFocusingToolDlg->m_dOpticsFocus[0]);
		mapZFocus.insert(std::pair<int, double>(nNextZPos, dAverageFocusGrabbed));

		if (theApp.m_pProgressDlg != NULL) {
			nLoadedDataPercentage = nLoadedDataPercentage + nLoadedDataPercentageStepSize;

			theApp.m_pProgressDlg->ShowWindow(TRUE);
			theApp.m_bProgressDlgOpen = TRUE;
			theApp.m_pProgressDlg->UpdateWindow();
			theApp.m_pProgressDlg->SetStatus("Auto Focusing Process...", nLoadedDataPercentage);
		}
	}

	nLoadedDataPercentage = 50;

	//Move down
	for (int nStep = 0; nStep < pFocusingToolDlg->m_nAutoFocusStepNo; nStep++) {
		int nNextZPos = nCurrentZPos - (nStep + 1)*pFocusingToolDlg->m_dAutoFocusStepSizeMM * dMotorResolution;

		//Validate Z Position before moving (without warning message box)
		if (theApp.motionParameters.IsSoftwareLimits(nMotorAxiz, nNextZPos)) {
			OutputDebugLogTo(8, TRUE, "Auto Focusing - Z Position %d is reach soft limit", nNextZPos);
			break;
		}

		nError = theApp.motionParameters.AbsoluteMove(nMotorAxiz, nNextZPos, theApp.motionParameters.getMovingSpeed(nMotorAxiz));
		if (nError) {
			OutputDebugLogTo(8, TRUE, "Auto Focusing - Failed to move motor axis %d Error %d", nMotorAxiz);
			break;
		}

		nError = theApp.motionParameters.WaitForMotionDone(nMotorAxiz, theApp.getMotionTimeOut());
		if (nError) {
			OutputDebugLogTo(8, TRUE, "Auto Focusing - CMotionControlDialog::Movement[Absolute] Axis%d Error %d", nMotorAxiz, nError);
			break;
		}
		theApp.motionParameters.WaitMotorSettlingTime(nMotorAxiz);

		//Get Focusing Value
		int nGrabSuccessCount = 0;
		double dAverageFocusGrabbed = 0;
		for (int nGrab = 0; nGrab < nGrabCount; nGrab++) {
			theApp.m_pTracks[nTrack].m_nOpticsRecalcMode = 1;

			nError = theApp.m_pTracks[nTrack].EnqueueAsyncGrab();
			if (!nError) {
				theApp.m_pTracks[nTrack].UpdateGrabImage(nDoc);

				double dMean, dNoise, dFocus;
				double dAvgFocus = 0;
				for (int nFocusRoi = 0; nFocusRoi < nFocusRoiCount; nFocusRoi++) {
					OpticsCharacteristics(&theApp.m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer, &theApp.m_pTracks[nTrack].m_rectOptics[nFocusRoi], dMean, dNoise, dFocus);
					dAvgFocus += dFocus;
				}
				dAvgFocus = dAvgFocus / nFocusRoiCount;

				dAverageFocusGrabbed += dAvgFocus;
				nGrabSuccessCount++;
			}
		}

		if (nGrabSuccessCount > 0)
			dAverageFocusGrabbed = dAverageFocusGrabbed / nGrabSuccessCount;

		dAverageFocusGrabbed = dAverageFocusGrabbed * 100;
		OutputDebugLogTo(8, TRUE, "Auto Focusing - Z: %d, Focus: %.5f - Optic Focus: %.5f", nNextZPos, dAverageFocusGrabbed, pFocusingToolDlg->m_dOpticsFocus[0]);
		mapZFocus.insert(std::pair<int, double>(nNextZPos, dAverageFocusGrabbed));

		if (theApp.m_pProgressDlg != NULL) {
			nLoadedDataPercentage = nLoadedDataPercentage + nLoadedDataPercentageStepSize;

			theApp.m_pProgressDlg->ShowWindow(TRUE);
			theApp.m_bProgressDlgOpen = TRUE;
			theApp.m_pProgressDlg->UpdateWindow();
			theApp.m_pProgressDlg->SetStatus("Auto Focusing Process...", nLoadedDataPercentage);
		}
	}

	//Find the best Z focus value
	double dBestFocus = 0;
	int nBestZFocus = 0;
	for (std::map<int, double>::iterator it = mapZFocus.begin(); it != mapZFocus.end(); it++) {
		if (it->second > dBestFocus) {
			dBestFocus = it->second;
			nBestZFocus = it->first;
		}
	}

	if (theApp.m_pProgressDlg != NULL) {
		theApp.m_pProgressDlg->SetStatus("Auto Focusing Process...", 100);
		theApp.m_pProgressDlg->ShowWindow(FALSE);
		theApp.m_bProgressDlgOpen = FALSE;
		theApp.m_pProgressDlg->m_nCurrentPos = 0;
	}

	bool bMoveToBestZFocusPosition = false;
	if (dBestFocus > 0) {
		CString strMsg;
		strMsg.Format("Best focus is at Z position %d (Focus value: %.2f).\nDo you want to move to best focus position?", nBestZFocus, dBestFocus);
		bMoveToBestZFocusPosition = AfxMessageBox(strMsg, MB_YESNO | MB_ICONQUESTION) == IDYES;
	}
	else {
		AfxMessageBox("Can not found the focusing area!");
	}

	if (bMoveToBestZFocusPosition) {
		nError = theApp.motionParameters.AbsoluteMove(nMotorAxiz, nBestZFocus, theApp.motionParameters.getMovingSpeed(nMotorAxiz));
		if (nError) {
			OutputDebugLogTo(8, TRUE, "Auto Focusing - Failed to move motor axis %d Error %d", nMotorAxiz);
		}

		if (!nError) {
			nError = theApp.motionParameters.WaitForMotionDone(nMotorAxiz, theApp.getMotionTimeOut());
			if (nError) {
				OutputDebugLogTo(8, TRUE, "Auto Focusing - CMotionControlDialog::Movement[Absolute] Axis%d Error %d", nMotorAxiz, nError);
			}
			else {
				theApp.motionParameters.WaitMotorSettlingTime(nMotorAxiz);
				nError = theApp.m_pTracks[nTrack].EnqueueAsyncGrab();
				if (!nError) {
					theApp.m_pTracks[nTrack].UpdateGrabImage(nDoc);
				}
			}
		}
	}
	else {
		//Move back previous Z Position
		nError = theApp.motionParameters.AbsoluteMove(nMotorAxiz, nCurrentZPos, theApp.motionParameters.getMovingSpeed(nMotorAxiz));
		if (nError) {
			OutputDebugLogTo(8, TRUE, "Auto Focusing - Failed to move motor axis %d Error %d", nMotorAxiz);
		}

		nError = theApp.motionParameters.WaitForMotionDone(nMotorAxiz, theApp.getMotionTimeOut());
		if (nError) {
			OutputDebugLogTo(8, TRUE, "Auto Focusing - CMotionControlDialog::Movement[Absolute] Axis%d Error %d", nMotorAxiz, nError);
		}
	}
	pFocusingToolDlg->m_bAutoFocusing = TRUE;

	return 0;
}