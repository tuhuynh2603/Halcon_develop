#include "StdAfx.h"
#include "PylonUsb.h"
#include "CameraCode.h"
#include "PerformanceTimer.h"
#include "DebugLog.h"
#include "Imaging.h"
#include "ImageFunction.h"
#include "ImageBuffer.h"


CCamEventHandler::CCamEventHandler()
{
}

CCamEventHandler::~CCamEventHandler()
{
}

CCamera::CCamera()
{
	m_nGrabTimeout		= 1000;
	m_szResolution		= CSize(0,0);
	m_nTriggerMode		= TRIG_SOFT;
	m_pUserFunct		= NULL;

	m_bIsColor = FALSE;
	m_bMonitorMode = FALSE;
}

CCamera::~CCamera()
{
}

int _stdcall InitDLL()
{
	PylonInitialize();
	OutputDebugLog("C_PylonUsb DLL Loaded");

	return 0;
}

int _stdcall FreeDLL()
{
	PylonTerminate();

	return 0;
}

int _stdcall RegisterCamera(CString strCamFileName,
						 CString strMPFFileName,
						 void **pCamModule,
						 BOOL bCreateEvent,
						 BOOL bMonitorMode,
						BOOL bSetSoftTrigger,
						BOOL bSetOutputLine)
{
	if(strCamFileName.CompareNoCase("none")==0)
		return 0;

	Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
	Pylon::CDeviceInfo di;
	di.SetSerialNumber(strCamFileName.GetBuffer());
	strCamFileName.ReleaseBuffer();
	di.SetDeviceClass(Pylon::BaslerUsbDeviceClass);

	Pylon::IPylonDevice* pDevice = NULL;
	try { pDevice = tlFactory.CreateDevice(di); } catch(...) {}
	if(!pDevice) {
		OutputDebugLogTo(2, TRUE,"Camera %s not found", strCamFileName);
		return -1;
	}
	CCamera* pCamera = new CCamera();
	pCamera->m_Camera.Attach(pDevice);

	if(bMonitorMode) {
		pCamera->m_Camera.RegisterConfiguration((Pylon::CConfigurationEventHandler*) NULL, Pylon::RegistrationMode_ReplaceAll, Pylon::Ownership_ExternalOwnership);
		//pCamera->m_Camera.RegisterConfiguration((Pylon::CConfigurationEventHandler*) NULL, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_None);
		pCamera->m_Camera.MonitorModeActive = true;
		pCamera->m_bMonitorMode = TRUE;
	}

	////register here for exposure event
	//pCamera->m_pCamEventHandler = new CCamEventHandler();
    //pCamera->m_Camera.GrabCameraEvents = true;
	//pCamera->m_Camera.RegisterCameraEventHandler(pCamera->m_pCamEventHandler, "ExposureEndEventData", eMyExposureEndEvent, Pylon::RegistrationMode_ReplaceAll, Pylon::Ownership_ExternalOwnership);
	//
	//pCamera->m_pCamEventHandler->m_hCamEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
	pCamera->m_Camera.Open();

	CString strModelName;
	//strModelName.Format("%s", pCamera->m_Camera.GetDeviceInfo().GetModelName());
	//pCamera->m_bIsColor = FALSE;//(strModelName.Right(1).MakeLower()=="c");
	strModelName.Format("%s", pCamera->m_Camera.DeviceModelName.GetValue().c_str());
	pCamera->m_bIsColor = (strModelName.Right(1).MakeLower()=="c");

	pCamera->m_strCamName.Format("%s", pCamera->m_Camera.GetDeviceInfo().GetUserDefinedName().c_str());

	OutputDebugLogTo(2, TRUE,"[%s] Camera %s opened", pCamera->m_strCamName, strCamFileName);
	InitializeCriticalSection(&pCamera->m_csCamera);

	if(GenApi::IsReadable(pCamera->m_Camera.WidthMax)) {
		try {pCamera->m_szResolution.cx = (int) pCamera->m_Camera.WidthMax.GetValue(); }
		catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::Get WidthMax Exception"); }
		try { pCamera->m_szResolution.cy = (int) pCamera->m_Camera.HeightMax.GetValue(); }
		catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::Get HeightMax Exception"); }
	}
	else
		OutputDebugLogTo(2, TRUE,"********** [%s] Failed to get Resolution X/Y", pCamera->m_strCamName);

	if(!bMonitorMode) {
		//if(GenApi::IsAvailable(pCamera->m_Camera.EventSelector)) {
		//	try { pCamera->m_Camera.EventSelector.SetValue(Basler_UsbCameraParams::EventSelector_ExposureEnd); }
		//	catch(...) { OutputDebugLogTo(0, TRUE, "RegisterCamera::EventSelector_ExposureEnd Exception"); }
		//	/*try { pCamera->m_Camera.EventNotification.SetValue(Basler_UsbCameraParams::EventNotification_GenICamEvent); }
		//	catch(...) { OutputDebugLogTo(0, TRUE, "RegisterCamera::EventNotification_GenICamEvent Exception"); }*/
		//	try { pCamera->m_Camera.EventNotification.SetValue(Basler_UsbCameraParams::EventNotification_On); }
		//	catch(...) { OutputDebugLogTo(0, TRUE, "RegisterCamera::EventNotification_On Exception"); }
		//}

		if(GenApi::IsWritable(pCamera->m_Camera.PixelFormat)) {
			try { pCamera->m_Camera.PixelFormat.SetValue(pCamera->m_bIsColor ? Basler_UsbCameraParams::PixelFormat_BayerRG8 : Basler_UsbCameraParams::PixelFormat_Mono8); } 
			catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::PixelFormat Exception"); }
			OutputDebugLogTo(2, TRUE,"[%s] PixelFormat(%s) set", pCamera->m_strCamName, pCamera->m_bIsColor ? "BayerBG8" : "Mono8");
		} 
		else
			OutputDebugLogTo(2, TRUE,"********** [%s] Failed to set PixelFormat=(%s)", pCamera->m_strCamName, pCamera->m_bIsColor ? "BayerBG8" : "Mono8");

		if(GenApi::IsReadable(pCamera->m_Camera.OffsetX)) {
			try { pCamera->m_posAoi.x = (int) pCamera->m_Camera.OffsetX.GetValue(); } 
			catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::Get OffsetX Exception"); }
			try { pCamera->m_posAoi.y = (int) pCamera->m_Camera.OffsetY.GetValue(); }
			catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::Get OffsetY Exception"); }
		}
		else
			OutputDebugLogTo(2, TRUE,"********** [%s] Failed to get Offset X/Y", pCamera->m_strCamName);

		pCamera->m_posAoi = CPoint(0, 0);
		pCamera->m_szAoi = pCamera->m_szResolution;
		//SetCameraAoi(pCamera, CRect(pCamera->m_posAoi, pCamera->m_szAoi));

		if(GenApi::IsWritable(pCamera->m_Camera.TestImageSelector)) {
			try { pCamera->m_Camera.TestImageSelector.SetValue(Basler_UsbCameraParams::TestImageSelector_Off); }
			catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::TestImageSelector_Off Exception"); }
		}
		else
			OutputDebugLogTo(2, TRUE,"********** [%s] Failed to set TestImageSelector", pCamera->m_strCamName);

		if(GenApi::IsAvailable(pCamera->m_Camera.TriggerSelector.GetEntry(Basler_UsbCameraParams::TriggerSelector_FrameStart))) {
			if(IsWritable(pCamera->m_Camera.TriggerSelector)) {
				try { pCamera->m_Camera.TriggerSelector.SetValue(Basler_UsbCameraParams::TriggerSelector_FrameStart); }
				catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::TriggerSelector_FrameStart Exception"); }
			}
			if(IsWritable(pCamera->m_Camera.TriggerMode)) {
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_On); }
				catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::TriggerMode_Off Exception"); }
			}
			if(IsWritable(pCamera->m_Camera.ExposureMode)) {
				try { pCamera->m_Camera.ExposureMode.SetValue(Basler_UsbCameraParams::ExposureMode_Timed); }
				catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::ExposureMode_Timed Exception"); }
			}
			if(IsWritable(pCamera->m_Camera.ExposureAuto)) {
				try { pCamera->m_Camera.ExposureAuto.SetValue(Basler_UsbCameraParams::ExposureAuto_Off); }
				catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::ExposureAuto_Off Exception"); }
			}
		}

		if(GenApi::IsAvailable(pCamera->m_Camera.TriggerSelector.GetEntry(Basler_UsbCameraParams::TriggerSelector_FrameStart))) {
			if(IsWritable(pCamera->m_Camera.TriggerSelector)) {
				try { pCamera->m_Camera.TriggerSelector.SetValue(Basler_UsbCameraParams::TriggerSelector_FrameStart); }
				catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::TriggerSelector_AcquisitionStart Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerSelector:AcquisitionStart", pCamera->m_strCamName);

			/// Commented for Mold Forming... 
			//SetTriggerMode(pCamera, TRIG_SOFT);

			if(IsWritable(pCamera->m_Camera.AcquisitionFrameRateEnable)) {
				try { pCamera->m_Camera.AcquisitionFrameRateEnable.SetValue(false); }
				catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::AcquisitionFrameRateEnable Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set AcquisitioFrameRateEnable", pCamera->m_strCamName);


		}
		else {
			OutputDebugLogTo(2, TRUE, "RegisterCamera::TriggerSelector_FrameStart is not available");
		}

		if(bSetSoftTrigger) {
			pCamera->m_nTriggerMode = TRIG_SOFT;
			SetTriggerMode(pCamera, TRIG_SOFT, 0.0);
		}
		else {
			pCamera->m_nTriggerMode = TRIG_HARD;
			SetTriggerMode(pCamera, TRIG_HARD, 0.0);
		}

//		if(bSetOutputLine)
//			SetOutputLine(pCamera);
		/// Commented for Mold Forming... 
		//SetOutputLine(pCamera, true);
		pCamera->m_Camera.MaxNumBuffer = NUM_BUFFERS;
		
		
		OutputDebugLogTo(2, TRUE, "MaxNumBuffer: %d (Min %d - Max %d), MaxNumQueuedBuffer: %d, MaxNumGrabResults: %d", pCamera->m_Camera.MaxNumBuffer.GetValue(), 
							pCamera->m_Camera.MaxNumBuffer.GetMin(), pCamera->m_Camera.MaxNumBuffer.GetMax(),
							pCamera->m_Camera.MaxNumQueuedBuffer.GetValue(), pCamera->m_Camera.MaxNumGrabResults.GetValue());

		
		try { pCamera->m_Camera.StartGrabbing(); } 
		catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::StartGrabbing Exception"); }
	}
	else {
		/*try { pCamera->m_Camera.GetStreamGrabberParams().TransmissionType = Basler_GigEStreamParams::TransmissionType_UseCameraConfig; }
		catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::TransmissionType_UseCameraConfig Exception"); }*/
		///SetMulticastAddress(pCamera, TRUE);

		///try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera); }
		try { pCamera->m_Camera.StartGrabbing(); }
		catch(...) {
			OutputDebugLogTo(2, TRUE, "********** [%s] Monitor Mode: Failed to attach to camera", pCamera->m_strCamName);
			ReleaseCamera((void**) &pCamera);
			delete pCamera;
			return -1;
		}
	}

	*pCamModule = pCamera;

	SetTransferParametersThreadPriority(pCamera, 31);
	SetStreamMaximumTransferSize(pCamera, /*3000000*/ pCamera->m_Camera.GetStreamGrabberParams().MaxTransferSize.GetMax());
	SetStreamMaximumBufferSize(pCamera, /*pCamera->m_Camera.GetStreamGrabberParams().MaxBufferSize.GetMax()*/ 4194304);

	SetGrabEngineThreadPriority(pCamera, 24);
	SetGrabLoopThreadPriority(pCamera, 15);

	return 0;
}

int _stdcall ReleaseCamera(void **pCamModule)
{
	CString strMsg;
		
	if(pCamModule) {
		CCamera* pCamera =(CCamera*) *pCamModule;
		
		pCamera->m_Camera.StopGrabbing();
		//if(!PylonExecCommand(pCamera->m_hPylon, CAM_FEATURE, "AcquisitionStop"))
		//	OutputDebugLog("********** [%s] Failed to execute AcquisitionStop", pCamera->m_strCamName);

		//if(!PylonCancelGrab(pCamera->m_hPylon))
		//	OutputDebugLog("********** [%s] Failed to CancelGrab", pCamera->m_strCamName);

		DeleteCriticalSection(&pCamera->m_csCamera);
		//
		//if(!PylonFinishGrab(pCamera->m_hPylon))
		//	OutputDebugLog("********** [%s] Failed to FinishGrab", pCamera->m_strCamName);
		//SetTriggerMode((void**) pCamModule, TRIG_NONE);

		if(!pCamera->m_bMonitorMode && pCamera->m_nTriggerMode == TRIG_SOFT) {
			if(IsWritable(pCamera->m_Camera.TriggerMode)) {
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_Off); }
				catch(...) { OutputDebugLogTo(2, TRUE, "ReleaseCamera::TriggerMode_Off Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerMode", pCamera->m_strCamName);
		
			/*if(GenApi::IsAvailable(pCamera->m_Camera.EventSelector)) {
				try { pCamera->m_Camera.EventSelector.SetValue(Basler_UsbCameraParams::EventSelector_ExposureEnd); }
				catch(...) { OutputDebugLogTo(0, TRUE, "ReleaseCamera::EventSelector_ExposureEnd Exception"); }
				try { pCamera->m_Camera.EventNotification.SetValue(Basler_UsbCameraParams::EventNotification_Off); }
				catch(...) { OutputDebugLogTo(0, TRUE, "ReleaseCamera::EventNotification_Off Exception"); }
			}*/
		}
		//CloseHandle(pCamera->m_pCamEventHandler->m_hCamEvent);

		try { pCamera->m_Camera.Close(); }
		//if(!PylonCamClose(pCamera->m_hPylon))
		catch(...) { OutputDebugLog("********** [%s] Failed to CloseCamera", pCamera->m_strCamName); }
			
		if(pCamera->m_pUserFunct)
			pCamera->m_pUserFunct = NULL;
		
		//if(pCamera->m_pCamEventHandler)
		//	delete pCamera->m_pCamEventHandler;
		delete pCamera;
		pCamera = NULL;
		*pCamModule = pCamera;
	}
	return 0;
}

int _stdcall RegHookFunction(void *pCamModule, CALLBACKFUNCT UserFunct)
{
	if(pCamModule) {
		CCamera *pCamera =(CCamera*) pCamModule;

		pCamera->m_pUserFunct = UserFunct;
	}
	return 0;
}

int _stdcall SetGrabTimeOut(void *pCamModule,int nTimeout)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;
	pCamera->m_nGrabTimeout = nTimeout;

	return 0;
}

int _stdcall ImageGrab(void *pCamModule, CImageBuffer **pGrabbedImgBuf/*, BOOL bStreamMode*/)
{
	if(EnqueueAsyncGrab(pCamModule) == 0)
		return WaitForCompletion(pCamModule, pGrabbedImgBuf/*, bStreamMode*/);

	return -1;
}

int _stdcall EnqueueAsyncGrab(void *pCamModule)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera *pCamera =(CCamera*) pCamModule;

	if(!pCamera->m_bMonitorMode) {
		if(!pCamera->m_Camera.IsGrabbing()) {
			OutputDebugLogTo(2, TRUE, "********** [%s] EnqueueSnap: Camera is not ready", pCamera->m_strCamName);
			return -1;
		}

		if(pCamera->m_Camera.WaitForFrameTriggerReady(1000, Pylon::TimeoutHandling_Return)) {
			if(pCamera->m_nTriggerMode == TRIG_SOFT)
				pCamera->m_Camera.ExecuteSoftwareTrigger();
		}
		else {
			OutputDebugLogTo(2, TRUE, "********** [%s] EnqueueAsyncGrab: WaitForFrameTriggerReady failed", pCamera->m_strCamName);
			return -1;
		}
//		pCamera->m_Camera.ExecuteSoftwareTrigger();
	}
	else {
		if(!pCamera->m_Camera.WaitForFrameTriggerReady(1000, Pylon::TimeoutHandling_Return)) {
			OutputDebugLogTo(2, TRUE, "********** [%s] EnqueueSnap: WaitForFrameTriggerReady failed", pCamera->m_strCamName);
			return -1;
		}
	}

	return 0;
}

int _stdcall WaitForCompletion(void *pCamModule, CImageBuffer** pGrabbedImgBuf/*, BOOL bStreamMode*/)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera *pCamera =(CCamera*) pCamModule;

	Pylon::CGrabResultPtr pGrabResult;
	int nError = 0;
	
	if(pCamera->m_Camera.RetrieveResult(pCamera->m_nGrabTimeout, pGrabResult, Pylon::TimeoutHandling_Return)) {
		if(pGrabResult && pGrabResult->GrabSucceeded()) {
			pCamera->m_GrabBuffer.AllocImgIndirect(CSize(pGrabResult->GetWidth(), pGrabResult->GetHeight()), (BYTE*) pGrabResult->GetBuffer());
			*pGrabbedImgBuf = &pCamera->m_GrabBuffer;
			//if(bStreamMode){
			//	OutputDebugLogTo(0, TRUE, "Adding Frames to AVI");
			//	pCamera->aviWriter.Add(pGrabResult);
			//	//OutputDebugLog("Added without conversion [%d]", pCamera->aviWriter.CanAddWithoutConversion(pGrabResult));
			//	//pCamera->aviWriter.Add(pGrabResult);
			//}
		}
		else {
			OutputDebugLogTo(9, TRUE, "[%s] Grab Error; GrabStatus = %s (0x%X)", 
				pCamera->m_strCamName, pGrabResult->GetErrorDescription().c_str(), pGrabResult->GetErrorCode());
			nError = -2;
		}
	}
	else
		nError = -1;

	if(pCamera->m_pUserFunct)
		(*(pCamera->m_pUserFunct))(1000);

	return nError;
}

int _stdcall CancelPendingGrab(void* pCamModule)
{
	CCamera *pCamera =(CCamera*) pCamModule;

	try {
		pCamera->m_Camera.StopGrabbing();
	}
	catch(...) { OutputDebugLogTo(2, TRUE, "CancelPendingGrab::StopGrabbing Exception"); }

	try {

		pCamera->m_Camera.StartGrabbing();  //Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera);}
	} 
	catch(...) { OutputDebugLogTo(2, TRUE, "CancelPendingGrab::StartGrabbing Exception"); }

	OutputDebugLog("[%s] Cancelled grab", pCamera->m_strCamName);

	return 0;
}

//create OnCameraEvent function
void CCamEventHandler::OnCameraEvent(Camera_t& camera, intptr_t userProvidedId, GenApi::INode* /* pNode */)
{
	//if(m_bByPassEvent)
	//	return;

    if(userProvidedId == eMyExposureEndEvent) { //// Exposure End event
		//OutputDebugLog("Exposure End event. FrameID:%d ; Timestamp:%d ", (int)camera.ExposureEndEventFrameID.GetValue(), (int)camera.ExposureEndEventTimestamp.GetValue());
		//OutputDebugLogTo(2,TRUE,"Exposure End event Set");
		SetEvent(m_hCamEvent);
    }
}

int _stdcall WaitForExposure(void *pCamModule, HANDLE eventKill)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	////GenApi::INode* pNode;
	////pNode = NULL;
	////OutputDebugLogTo(2,TRUE,"Pylon::WaitForExposure Entered");	
	//DWORD dwObj;
//
	//dwObj = WaitForSingleObject(pCamera->m_pCamEventHandler->m_hCamEvent, /*INFINITE*/1000); ////which will be set in callback OnCameraEvent
	//if(dwObj == WAIT_TIMEOUT) {
	//	OutputDebugLogTo(2,TRUE,"WaitForExposure TimeOut");	
	//	//try { pCamera->m_pCamEventHandler->OnCameraEvent(pCamera->m_Camera, eMyExposureEndEvent, pNode); }
	//	//catch(...) { OutputDebugLog("OnCameraEvent Exception"); }
	//	return -1;
	//}
	////else
	////	OutputDebugLogTo(2,TRUE,"WaitForExposure Lasted");

	return 0;
}

int WaitForGrabEnd(void* pCam)
{
	return 0;
}

int _stdcall EnqueueAsyncGrabNoTrigger(void *pCamModule)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera *pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	BOOL bTrigHard = FALSE;
	if(pCamera->m_nTriggerMode == TRIG_HARD) {
		if(IsWritable(pCamera->m_Camera.TriggerSource)) {
			try { pCamera->m_Camera.TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Software); }
			catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerMode::TriggerSource_Software Exception"); }
		}
		else
			OutputDebugLogTo(2, TRUE, "********** [%s] Failed to set TriggerSource", pCamera->m_strCamName);
		pCamera->m_nTriggerMode = TRIG_SOFT;
		bTrigHard = TRUE;
	}

	int nResult = EnqueueAsyncGrab(pCamModule);

	if(bTrigHard) {
		Sleep(10);
		if(IsWritable(pCamera->m_Camera.TriggerSource)) {
			try { pCamera->m_Camera.TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Line1); }
			catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerMode::TriggerSource_Line1 Exception"); }
		}
		else
			OutputDebugLog("********** [%s] Failed to set TriggerSource", pCamera->m_strCamName);
		pCamera->m_nTriggerMode = TRIG_HARD;
	}

	return nResult;
}

int _stdcall SetTriggerMode(void *pCamModule, int nMode, double dTrigDelay)
{
	CString strMsg;
	
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera* pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	switch(nMode) {
		case TRIG_NONE:
			if(IsWritable(pCamera->m_Camera.TriggerMode)) { 
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_Off); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerMode::TriggerMode_Off Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerMode", pCamera->m_strCamName);

			pCamera->m_nGrabTimeout = 1000;

			OutputDebugLog("[%s] Set trigger mode to OFF is completed", pCamera->m_strCamName);

			break;

		case TRIG_SOFT:
			if(IsWritable(pCamera->m_Camera.TriggerMode)) { 
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_On); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerMode::TriggerMode_On Exception"); }
			}
			else
				OutputDebugLogTo(2,TRUE,"********** [%s] Failed to set TriggerMode", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerSource)) {
				try { pCamera->m_Camera.TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Software); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerMode::TriggerSource_Software Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerSource", pCamera->m_strCamName);
			break;

			if(IsWritable(pCamera->m_Camera.TriggerDelay)) {
				try { pCamera->m_Camera.TriggerDelay.SetValue(0.0); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerDelayAbs::dTrigDelay Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerDelayAbs:dTrigDelay", pCamera->m_strCamName, dTrigDelay);

			pCamera->m_nGrabTimeout = 1000;

			OutputDebugLog("[%s] Set trigger mode to SOFT is completed", pCamera->m_strCamName);

		case TRIG_HARD:
			if(IsWritable(pCamera->m_Camera.TriggerMode)) { 
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_On); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerMode::TriggerMode_On Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerMode", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerSource)) {
				try { pCamera->m_Camera.TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Line1); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerMode::TriggerSource_Line1 Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerSource:Line1", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerActivation)) {
				try { pCamera->m_Camera.TriggerActivation.SetValue(Basler_UsbCameraParams::TriggerActivation_RisingEdge); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerActivation::TriggerActivation_RisingEdge Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerActivation:RisingEdge", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerDelay)) {
				try { pCamera->m_Camera.TriggerDelay.SetValue(dTrigDelay); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerDelayAbs::dTrigDelay Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerDelayAbs:dTrigDelay", pCamera->m_strCamName, dTrigDelay);

			pCamera->m_nGrabTimeout = INFINITE;

			OutputDebugLog("[%s] Set trigger mode to HARD is completed, delay %.1f us", pCamera->m_strCamName, dTrigDelay);

			break;

		case TRIG_HARD_NO_DELAY:
			if(IsWritable(pCamera->m_Camera.TriggerMode)) {
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_UsbCameraParams::TriggerMode_On); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerMode::TriggerMode_On Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerMode", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerSource)) {
				try { pCamera->m_Camera.TriggerSource.SetValue(Basler_UsbCameraParams::TriggerSource_Line1); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerMode::TriggerSource_Line1 Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerSource:Line1", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerActivation)) {
				try { pCamera->m_Camera.TriggerActivation.SetValue(Basler_UsbCameraParams::TriggerActivation_RisingEdge); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerActivation::TriggerActivation_RisingEdge Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerActivation:RisingEdge", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerDelay)) {
				try { pCamera->m_Camera.TriggerDelay.SetValue(0.0); }
				catch(...) { OutputDebugLogTo(2, TRUE, "SetTriggerDelayAbs::dTrigDelay Exception"); }
			}
			else
				OutputDebugLog("********** [%s] Failed to set TriggerDelayAbs:dTrigDelay", pCamera->m_strCamName, 0.0);

			OutputDebugLog("[%s] Set trigger mode to HARD is completed, delay 0.0 us", pCamera->m_strCamName);


			pCamera->m_nGrabTimeout = INFINITE;
			break;
		default:
			break;
	}
	pCamera->m_nTriggerMode = nMode;

	return 0;
}

int _stdcall SetTriggerParms(void *pCamModule, int nTriggerPolarity, int nPreTriggerDelay, int nDebouncingTime)
{
	CString strMsg;
	
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	//CCamera* pCamera =(CCamera*) pCamModule;
	//if(pCamera->m_bMonitorMode)
	//	return 0;

	//if(!PylonSetEnumFeature(pCamera->m_hPylon, CAM_FEATURE, "LineSelector", "Line1"))
	//	OutputDebugLog("********** [%s] Failed to set LineSelector", pCamera->m_strCamName);

	//if(!PylonSetEnumFeature(pCamera->m_hPylon, CAM_FEATURE, "TriggerActivation", nTriggerPolarity == RISING_EDGE? "RisingEdge" : "FallingEdge"))
	//	OutputDebugLog("********** [%s] Failed to set TriggerActivation", pCamera->m_strCamName);
	//if(!PylonSetFloatFeature(pCamera->m_hPylon, CAM_FEATURE, "TriggerDelayAbs", nPreTriggerDelay*1000))
	//	OutputDebugLog("********** [%s] Failed to set TriggerDelayAbs", pCamera->m_strCamName);

	//double dVal = nDebouncingTime * 1000;
	//if(!PylonSetFloatFeature(pCamera->m_hPylon, CAM_FEATURE, "LineDebouncerTimeAbs", dVal))
	//	OutputDebugLog("********** [%s] Failed to set LineDebouncerTimeAbs", pCamera->m_strCamName);
	//
	//dVal = 0;
	//double dMin = 0, dMax = 0;
	//if(!PylonGetFloatFeature(pCamera->m_hPylon, CAM_FEATURE, "LineDebouncerTimeAbs", &dVal, &dMin, &dMax))
	//	OutputDebugLog("********** [%s] Failed to get LineDebouncerTimeAbs", pCamera->m_strCamName);
	//OutputDebugLog("LineDebouncerTimeAbs Read: Value=%5.1f,  Min=%5.1f,  Max=%5.1f", dVal, dMin, dMax);

	//return 0;
	
	return -1;
}

int _stdcall GetTriggerMode(void *pCamModule,int *pnMode)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;

	*pnMode = pCamera->m_nTriggerMode;

	return 0;

}

int _stdcall GetResolution(void *pCamModule,CSize *pSize)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;

	*pSize = pCamera->m_szResolution;
	return 0;
}

int _stdcall GetCameraAoi(void *pCamModule, CRect *pRect)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;

	if(GenApi::IsReadable(pCamera->m_Camera.OffsetX)) {
		try { pCamera->m_posAoi.x = (int) pCamera->m_Camera.OffsetX.GetValue(); }
		catch(...) { OutputDebugLogTo(2, TRUE, "GetCameraAoi::Get OffsetX Exception"); }
		try { pCamera->m_posAoi.y = (int) pCamera->m_Camera.OffsetY.GetValue(); }
		catch(...) { OutputDebugLogTo(2, TRUE, "GetCameraAoi::Get OffsetY Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to get Offset X/Y", pCamera->m_strCamName);
	
	if(GenApi::IsReadable(pCamera->m_Camera.Width)) {
		try { pCamera->m_szAoi.cx = (int) pCamera->m_Camera.Width.GetValue(); }
		catch(...) { OutputDebugLogTo(2, TRUE, "GetCameraAoi::Get Width Exception"); }
		try { pCamera->m_szAoi.cy = (int) pCamera->m_Camera.Height.GetValue(); }
		catch(...) { OutputDebugLogTo(2, TRUE, "GetCameraAoi::Get Height Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to get AOI X/Y", pCamera->m_strCamName);

	pRect->SetRect(pCamera->m_posAoi.x, pCamera->m_posAoi.y, pCamera->m_posAoi.x+pCamera->m_szAoi.cx, pCamera->m_posAoi.y+pCamera->m_szAoi.cy);

	return 0;
}

int _stdcall SetCameraAoi(void *pCamModule, CRect rect)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	EnterCriticalSection(&pCamera->m_csCamera);

	try { pCamera->m_Camera.StopGrabbing(); } 
	catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::StopGrabbing Exception"); }

	pCamera->m_szAoi = rect.Size();
	pCamera->m_szAoi.cx &= ~3;
	pCamera->m_posAoi = rect.TopLeft();

	OutputDebugLog("[%s] SetCameraAOI: Offset(%d,%d) Size(%d,%d) Resolution(%d,%d)", pCamera->m_strCamName,
																					pCamera->m_posAoi.x, pCamera->m_posAoi.y,
																					pCamera->m_szAoi.cx, pCamera->m_szAoi.cy,
																					pCamera->m_szResolution.cx, pCamera->m_szResolution.cy);

	if(pCamera->m_posAoi.x == 0 && pCamera->m_posAoi.y == 0){
		if(GenApi::IsWritable(pCamera->m_Camera.OffsetX)) {
			try { pCamera->m_Camera.OffsetX.SetValue(pCamera->m_posAoi.x); }
			catch(...) { OutputDebugLogTo(2, TRUE, "SetCameraAoi::Set OffsetX Exception"); }
			try { pCamera->m_Camera.OffsetY.SetValue(pCamera->m_posAoi.y); }
			catch(...) { OutputDebugLogTo(2, TRUE, "SetCameraAoi::Set OffsetY Exception"); }
		}
		else
			OutputDebugLog("********** [%s] Failed to set Offset X/Y", pCamera->m_strCamName);
	}

	if(GenApi::IsWritable(pCamera->m_Camera.Width)) {
		try { pCamera->m_Camera.Width.SetValue(pCamera->m_szAoi.cx); }
		catch(...) { OutputDebugLogTo(2, TRUE, "SetCameraAoi::Set Width Exception"); }
		try { pCamera->m_Camera.Height.SetValue(pCamera->m_szAoi.cy); }
		catch(...) { OutputDebugLogTo(2, TRUE, "SetCameraAoi::Set Height Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set AOI X/Y", pCamera->m_strCamName);

	if(pCamera->m_posAoi.x > 0 || pCamera->m_posAoi.y > 0){
		if(GenApi::IsWritable(pCamera->m_Camera.OffsetX)) {
			try { pCamera->m_Camera.OffsetX.SetValue(pCamera->m_posAoi.x); }
			catch(...) { OutputDebugLogTo(2, TRUE, "SetCameraAoi::Set OffsetX Exception"); }
			try { pCamera->m_Camera.OffsetY.SetValue(pCamera->m_posAoi.y); }
			catch(...) { OutputDebugLogTo(2, TRUE, "SetCameraAoi::Set OffsetY Exception"); }
		}
		else
			OutputDebugLog("********** [%s] Failed to set Offset X/Y", pCamera->m_strCamName);
	}

	try { pCamera->m_Camera.StartGrabbing(); } 
	catch(...) { OutputDebugLogTo(2, TRUE, "RegisterCamera::Start Exception"); }
	LeaveCriticalSection(&pCamera->m_csCamera);

	return 0;
}

int _stdcall GetExposure(void *pCamModule, int *pnExposure)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;

	if(GenApi::IsReadable(pCamera->m_Camera.ExposureTime)) {	//ExposureTime was previosuly ExposureTimeRaw
		try { *pnExposure = (int) pCamera->m_Camera.ExposureTime.GetValue(); }
		catch(...) { OutputDebugLogTo(2, TRUE, "GetExposure::ExposureTimeAbs Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to get ExposureTimeAbs", pCamera->m_strCamName);

	return 0;
}

int _stdcall SetExposure(void *pCamModule, int nExposure)
{

	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(nExposure < 40)
		nExposure = 40;
	if(GenApi::IsWritable(pCamera->m_Camera.ExposureTime)) {		//ExposureTime was previously ExposureTimeAbs
		try { pCamera->m_Camera.ExposureTime.SetValue(nExposure); }
		catch(...) { OutputDebugLogTo(2, TRUE, "SetExposure::ExposureTimeAbs Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set ExposureTimeAbs", pCamera->m_strCamName);


	return 0;
}

int _stdcall GetCameraGain(void *pCamModule, int *pnGain, int nGainMode)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;

	if(GenApi::IsWritable(pCamera->m_Camera.GainSelector)) {
		/*try { pCamera->m_Camera.GainSelector.SetValue(nGainMode==0 ? Basler_UsbCameraParams::GainSelector_All : 
			(nGainMode==1 ? Basler_UsbCameraParams::GainSelector_Tap1 : Basler_UsbCameraParams::GainSelector_Tap2)); }
		catch(...) { OutputDebugLogTo(0, TRUE, "GetCameraGain::GainSelector Exception"); }*/

		try { pCamera->m_Camera.GainSelector.SetValue(Basler_UsbCameraParams::GainSelector_All); }
		catch(...) { OutputDebugLogTo(2, TRUE, "GetCameraGain::GainSelector Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set GainSelector", pCamera->m_strCamName);

	if(GenApi::IsReadable(pCamera->m_Camera.Gain)) {	//Gain was previously GainRaw
		try { *pnGain = (int) pCamera->m_Camera.Gain.GetValue(); }
		catch(...) { OutputDebugLogTo(2, TRUE, "GetCameraGain::GainRaw Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to get GainRaw", pCamera->m_strCamName);

	return 0;
}

int _stdcall SetCameraGain(void *pCamModule, int nGain, int nGainMode)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(GenApi::IsWritable(pCamera->m_Camera.GainSelector)) {
		/*try { pCamera->m_Camera.GainSelector.SetValue(nGainMode==0 ? Basler_UsbCameraParams::GainSelector_All : 
			(nGainMode==1 ? Basler_UsbCameraParams::GainSelector_Tap1 : Basler_UsbCameraParams::GainSelector_Tap2)); }
		catch(...) { OutputDebugLogTo(0, TRUE, "SetCameraGain::GainSelector Exception"); }*/

		try { pCamera->m_Camera.GainSelector.SetValue(Basler_UsbCameraParams::GainSelector_All); }
		catch(...) { OutputDebugLogTo(2, TRUE, "SetCameraGain::GainSelector Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set GainSelector", pCamera->m_strCamName);

	double dGainDB = log10(nGain / 36.0) * 20;

	if(GenApi::IsWritable(pCamera->m_Camera.Gain)) {	//Gain was previously GainRaw
		try { pCamera->m_Camera.Gain.SetValue(dGainDB); }
		catch(...) { OutputDebugLogTo(2, TRUE, "SetCameraGain::GainRaw Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set GainRaw", pCamera->m_strCamName);

	return 0;
}

int _stdcall SetWhiteBalance(void *pCamModule, int nValue, int nColorSelect)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(GenApi::IsWritable(pCamera->m_Camera.BalanceRatioSelector)) {
		try { pCamera->m_Camera.BalanceRatioSelector.SetValue(nColorSelect==0 ? Basler_UsbCameraParams::BalanceRatioSelector_Red : 
			(nColorSelect==1 ? Basler_UsbCameraParams::BalanceRatioSelector_Green : Basler_UsbCameraParams::BalanceRatioSelector_Blue)); }
		catch(...) { OutputDebugLogTo(2, TRUE, "SetWhiteBalance::BalanceRatioSelector Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set BalanceRatioSelector", pCamera->m_strCamName);

	if(GenApi::IsWritable(pCamera->m_Camera.BalanceRatio)) {	//BalanceRatio was previously BalanceRatioRaw
		try { pCamera->m_Camera.BalanceRatio.SetValue(nValue); }
		catch(...) { OutputDebugLogTo(2, TRUE, "SetWhiteBalance::BalanceRatioRaw Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set BalanceRatioRaw", pCamera->m_strCamName);

	return 0;
}

int _stdcall GetBlackLevel(void *pCamModule, int *pnBlackLevel)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;

	if(GenApi::IsReadable(pCamera->m_Camera.BlackLevel)) {	//BlackLevel was previously BlackLevelRaw
		try { *pnBlackLevel = (int) pCamera->m_Camera.BlackLevel.GetValue(); }
		catch(...) { OutputDebugLogTo(2, TRUE, "GetBlackLevel::BlackLevelRaw Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to get BLackLevelRaw", pCamera->m_strCamName);

	return 0;
}

int _stdcall SetBlackLevel(void *pCamModule, int nBlackLevel)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(GenApi::IsWritable(pCamera->m_Camera.BlackLevel)) {	//BlackLevel was previously BlackLevelRaw
		try { pCamera->m_Camera.BlackLevel.SetValue(nBlackLevel); }
		catch(...) { OutputDebugLogTo(2, TRUE, "SetBlackLevel::nBlackLevel Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set BlackLevelRaw", pCamera->m_strCamName);

	return 0;
}


int _stdcall GetBytesPerPkt(void *pCamModule, long *pnBytesPerPkt, long* pnBppMin, long* pnBppMax, long* pnBppInc)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;

	//PylonGetIntFeature(pCamera->m_hPylon, CAM_FEATURE, "GevSCPSPacketSize", pnBytesPerPkt, pnBppMin, pnBppMax, pnBppInc);
	//return 0;

	return -1;
}

int _stdcall SetBytesPerPkt(void *pCamModule, long nBytesPerPkt,int TransLayerPos)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;
	//CString StrType;
	//int i;

	//switch(TransLayerPos) {
	//	case 0:
	//		StrType.Format("GevSCPSPacketSize");
	//		
	//		PylonCancelGrab(pCamera->m_hPylon);
	//		PylonExecCommand(pCamera->m_hPylon, CAM_FEATURE, "AcquisitionStop");
	//		PylonFinishGrab(pCamera->m_hPylon);

	//	break;
	//case 1:
	//		StrType.Format("GevSCPD");
	//	break;
	//case 2:
	//		StrType.Format("GevSCFTD");
	//	break;
	//case 3:
	//		StrType.Format("GevSCBWR");
	//	break;
	//case 4:
	//		StrType.Format("GevSCBWRA");
	//	break;

	//}
	//PylonSetIntFeature(pCamera->m_hPylon, CAM_FEATURE, StrType, nBytesPerPkt/*350*/);

	//return 0;

	return -1;
}

int _stdcall SetOutputLine(void *pCamModule)
{
	/*if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera *pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(IsWritable(pCamera->m_Camera.LineSelector)) { 
		try { pCamera->m_Camera.LineSelector.SetValue(BaslerUsbCamera::L); }
		catch(...) { OutputDebugLogTo(0, TRUE, "LineSelector::LineSelector_Out1 Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set LineSelector", pCamera->m_strCamName);

	if(IsWritable(pCamera->m_Camera.LineMode)) { 
		try { pCamera->m_Camera.LineMode.SetValue(BaslerUsbCamera::LineMode_Output); }
		catch(...) { OutputDebugLogTo(0, TRUE, "LineMode::LineMode_Output Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set LineMode", pCamera->m_strCamName);

	if(IsWritable(pCamera->m_Camera.LineSource)) { 
		try { pCamera->m_Camera.LineSource.SetValue(BaslerUsbCamera::LineSource_ExposureActive); }
		catch(...) { OutputDebugLogTo(0, TRUE, "LineSource::LineSource_ExposureActive Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set LineSource", pCamera->m_strCamName);

	if(IsWritable(pCamera->m_Camera.UserOutputSelector)) { 
		try { pCamera->m_Camera.UserOutputSelector.SetValue(BaslerUsbCamera::UserOutputSelector_UserOutput1); }
		catch(...) { OutputDebugLogTo(0, TRUE, "UserOutputSelector::UserOutputSelector_UserOutput1 Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set UserOutputSelector", pCamera->m_strCamName);*/


	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera *pCamera = (CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(IsWritable(pCamera->m_Camera.LineSelector)) {
		try { pCamera->m_Camera.LineSelector.SetValue(Basler_UsbCameraParams::LineSelector_Line2); }
		catch(...) { OutputDebugLogTo(2, TRUE, "LineSelector::LineSelector_Out1 Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set LineSelector", pCamera->m_strCamName);

	//if(IsWritable(pCamera->m_Camera.LineMode)) {
	//	try { pCamera->m_Camera.LineMode.SetValue(Basler_UsbCameraParams::LineMode_Output); }
	//	catch(...) { OutputDebugLogTo(0, TRUE, "LineMode::LineMode_Output Exception"); }
	//}
	//else
	//	OutputDebugLog("********** [%s] Failed to set LineMode", pCamera->m_strCamName);

	if(IsWritable(pCamera->m_Camera.LineSource)) {
		try { pCamera->m_Camera.LineSource.SetValue(Basler_UsbCameraParams::LineSource_ExposureActive); }
		catch(...) { OutputDebugLogTo(2, TRUE, "LineSource::LineSource_ExposureActive Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set LineSource", pCamera->m_strCamName);

	if(IsWritable(pCamera->m_Camera.LineInverter)) {
		try { pCamera->m_Camera.LineInverter.SetValue(false); }
		catch(...) { OutputDebugLogTo(2, TRUE, "LineInverter (false) Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set LineInverter", pCamera->m_strCamName);

	if(IsWritable(pCamera->m_Camera.UserOutputSelector)) {
		try { pCamera->m_Camera.UserOutputSelector.SetValue(Basler_UsbCameraParams::UserOutputSelector_UserOutput1); }
		catch(...) { OutputDebugLogTo(2, TRUE, "UserOutputSelector::UserOutputSelector_UserOutput1 Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to set UserOutputSelector", pCamera->m_strCamName);

	return 0;
}

void SetMulticastAddress(CCamera* pCamera, char* strIpAddress)
{
	return;
	/*try { pCamera->m_Camera.GetStreamGrabberParams().TransmissionType = 
		(strIpAddress ? Basler_GigEStreamParams::TransmissionType_Multicast :
		Basler_GigEStreamParams::TransmissionType_Unicast); }
	catch(...) { OutputDebugLogTo(0, TRUE, "EnableMulticast::TransmissionType Exception"); }

	if(strIpAddress) {
		try { pCamera->m_Camera.GetStreamGrabberParams().DestinationAddr = strIpAddress; }
		catch(...) { OutputDebugLogTo(0, TRUE, "EnableMulticast::DestinationAddr Exception"); }

		try { pCamera->m_Camera.GetStreamGrabberParams().DestinationPort = 49152; }
		catch(...) { OutputDebugLogTo(0, TRUE, "EnableMulticast::DestinationAddr Exception"); }
	}

	OutputDebugLogTo(0, TRUE, "[%s] ------ Multicasting %s ------------", pCamera->m_strCamName, strIpAddress ? "ENABLED" : "DISABLED");*/
}

int _stdcall EnableMulticast(void *pCamModule, char* strIpAddress)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	pCamera->m_Camera.StopGrabbing();

	SetMulticastAddress(pCamera, strIpAddress);

	pCamera->m_Camera.StartGrabbing(); //Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera);

	return 0;
}


int _stdcall CameraStream(void *pCamModule, CString* strFileName, int nFrameRate)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	uint32_t nWidth;
	uint32_t nHeight;

	CCamera* pCamera =(CCamera*) pCamModule;
	Pylon::SAviCompressionOptions compressionOptions("MSVC", false);
	if(EnqueueAsyncGrab(pCamModule) == 0){
		Pylon::CGrabResultPtr pGrabResult;

		if(pCamera->m_Camera.RetrieveResult(pCamera->m_nGrabTimeout, pGrabResult, Pylon::TimeoutHandling_Return)) {
			if(pGrabResult && pGrabResult->GrabSucceeded()){
				nWidth	= pGrabResult->GetWidth();
				nHeight = pGrabResult->GetHeight();
			}
			else 
				goto FailedGrab;
		}
		else
			goto FailedGrab;
	}
	else
			goto FailedGrab;

	Pylon::SAviCompressionOptions* pCompressionOptions = NULL;
	
    //pCompressionOptions = &compressionOptions;

	OutputDebugLog("AVI Frame rate %d", nFrameRate);
	pCamera->aviWriter.Open(strFileName->GetBuffer(), 
					   nFrameRate,
					   Pylon::PixelType_Mono8, 
					   nWidth, 
					   nHeight, 
					   Pylon::ImageOrientation_BottomUp, // Some compression codecs will not work with top down oriented images.
					   pCompressionOptions);

	OutputDebugLog("AVI Frame rate Open Completed");
	strFileName->ReleaseBuffer();
	return 0;

FailedGrab:
	return -1;
}


int _stdcall StopStream(void *pCamModule)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera =(CCamera*) pCamModule;
	pCamera->aviWriter.Close();
	OutputDebugLog("Done saving AVI file");
	return 0;
}

int _stdcall EnableOutputLine(void *pCamModule, bool enable)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera *pCamera = (CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(enable) {
		if(IsWritable(pCamera->m_Camera.LineSource)) {
			try { pCamera->m_Camera.LineSource.SetValue(Basler_UsbCameraParams::LineSource_ExposureActive); }
			catch(...) { OutputDebugLogTo(2, TRUE, "LineSource::LineSource_ExposureActive Exception"); }
		}
		else
			OutputDebugLog("********** [%s] Failed to set LineSource", pCamera->m_strCamName);
	}
	else {
		if(IsWritable(pCamera->m_Camera.LineSource)) {
			try { pCamera->m_Camera.LineSource.SetValue(Basler_UsbCameraParams::LineSource_UserOutput1); }
			catch(...) { OutputDebugLogTo(2, TRUE, "LineSource::LineSource_UserOutput1 Exception"); }
		}
		else
			OutputDebugLog("********** [%s] Failed to set LineSource", pCamera->m_strCamName);
	}

	return 0;
}

int _stdcall SetTransferParametersThreadPriority(void *pCamModule, int nPriorityValue)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	int threadPririty = 0;

	CCamera* pCamera = (CCamera*)pCamModule;
	if (pCamera->m_bMonitorMode)
		return 0;

	try { pCamera->m_Camera.StopGrabbing(); }
	catch (...) {
		OutputDebugLogTo(0, TRUE, "SetTransferParametersThreadPriority::StopGrabbing Exception");
	}


	if (IsWritable(pCamera->m_Camera.GetStreamGrabberParams().TransferLoopThreadPriority)) {
		try {
			pCamera->m_Camera.GetStreamGrabberParams().TransferLoopThreadPriority.SetValue(nPriorityValue);
		}
		catch (...) {
			OutputDebugLogTo(0, TRUE, "SetTransferParametersThreadPriority::TransferLoopThreadPriority Exception");
		}
	}
	else {
		OutputDebugLogTo(0, TRUE, "********** [%s] Failed to set TransferLoopThreadPriority", pCamera->m_strCamName);
	}

	if (IsReadable(pCamera->m_Camera.GetStreamGrabberParams().TransferLoopThreadPriority)) {
		try {
			threadPririty = pCamera->m_Camera.GetStreamGrabberParams().TransferLoopThreadPriority.GetValue();
			OutputDebugLogTo(0, TRUE, "TransferLoopThreadPriority::TransferLoopThreadPriority value %d", threadPririty);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "GetTransferParametersThreadPriority::TransferLoopThreadPriority Exception"); }
	}
	else {
		OutputDebugLogTo(0, TRUE, "********** [%s] Failed to set Read TransferLoopThreadPriority", pCamera->m_strCamName);
	}

	try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetTransferParametersThreadPriority::StartGrabbing Exception"); }

	/*if (threadPririty != nPriorityValue)
		return -1;*/

	return 0;
}

int _stdcall GetTransferParametersThreadPriority(void *pCamModule, int * nPriorityValue)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera* pCamera = (CCamera*)pCamModule;
	if (pCamera->m_bMonitorMode)
		return 0;

	try { pCamera->m_Camera.StopGrabbing(); }
	catch (...) { OutputDebugLogTo(0, TRUE, "GetTransferParametersThreadPriority::StopGrabbing Exception"); }

	if (IsReadable(pCamera->m_Camera.GetStreamGrabberParams().TransferLoopThreadPriority)) {
		try {
			*nPriorityValue = pCamera->m_Camera.GetStreamGrabberParams().TransferLoopThreadPriority.GetValue();
			//			OutputDebugLogTo(0, TRUE, "TransferLoopThreadPriority::TransferLoopThreadPriority value %d", *nPriorityValue);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "GetTransferParametersThreadPriority::TransferLoopThreadPriority Exception"); }
	}
	else {
		OutputDebugLogTo(0, TRUE, "********** [%s] Failed to set Read TransferLoopThreadPriority", pCamera->m_strCamName);
	}

	try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	catch (...) { OutputDebugLogTo(0, TRUE, "GetTransferParametersThreadPriority::StartGrabbing Exception"); }
}

int _stdcall SetGrabEngineThreadPriority(void *pCamModule, int nPriorityValue)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera* pCamera = (CCamera*)pCamModule;
	if (pCamera->m_bMonitorMode)
		return 0;

	try { pCamera->m_Camera.StopGrabbing(); }
	catch (...) {
		OutputDebugLogTo(0, TRUE, "InternalGrabEngineThreadPriority::StopGrabbing Exception");
	}

	if (IsWritable(pCamera->m_Camera.InternalGrabEngineThreadPriorityOverride)) {
		//	OutputDebugLogTo(0, TRUE, "InternalGrabEngineThreadPriority trying to Set to %d", 1);
		try {
			pCamera->m_Camera.InternalGrabEngineThreadPriorityOverride.SetValue(true);
			//	OutputDebugLogTo(0, TRUE, "InternalGrabEngineThreadPriorityOverride Set to %d", true);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "InternalGrabEngineThreadPriorityOverride Exception"); }
	}
	else {
		OutputDebugLogTo(0, TRUE, "********** [%s] Failed to set InternalGrabEngineThreadPriorityOverride", pCamera->m_strCamName);
	}

	if (IsWritable(pCamera->m_Camera.InternalGrabEngineThreadPriority)) {
		//	OutputDebugLogTo(0, TRUE, "InternalGrabEngineThreadPriority trying to Set to %d", nPriorityValue);
		try {
			pCamera->m_Camera.InternalGrabEngineThreadPriority.SetValue(nPriorityValue);
			OutputDebugLogTo(0, TRUE, "InternalGrabEngineThreadPriority Set to %d", nPriorityValue);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "TransferLoopThreadPriority Exception"); }
	}
	else {
		OutputDebugLogTo(0, TRUE, "********** [%s] Failed to set InternalGrabEngineThreadPriority", pCamera->m_strCamName);
	}

	try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	catch (...) { OutputDebugLogTo(0, TRUE, "InternalGrabEngineThreadPriority::StartGrabbing Exception"); }

	return 0;
}

int _stdcall SetGrabLoopThreadPriority(void *pCamModule, int nPriorityValue)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera* pCamera = (CCamera*)pCamModule;
	if (pCamera->m_bMonitorMode)
		return 0;

	try { pCamera->m_Camera.StopGrabbing(); }
	catch (...) {
		OutputDebugLogTo(0, TRUE, "GrabLoopThreadPriority::StopGrabbing Exception");
	}

	//	pCamera->m_Camera.InternalGrabEngineThreadPriority

	if (IsWritable(pCamera->m_Camera.GrabLoopThreadPriorityOverride)) {
		//		OutputDebugLogTo(0, TRUE, "GrabLoopThreadPriorityOverride trying to Set to %d", nPriorityValue);
		try {
			pCamera->m_Camera.GrabLoopThreadPriorityOverride.SetValue(true);
			//			OutputDebugLogTo(0, TRUE, "GrabLoopThreadPriorityOverride Set to %d", nPriorityValue);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "GrabLoopThreadPriorityOverride Exception"); }
	}
	else {
		OutputDebugLogTo(0, TRUE, "********** [%s] Failed to set GrabLoopThreadPriorityOverride", pCamera->m_strCamName);
	}

	int nValue;
	if (IsWritable(pCamera->m_Camera.GrabLoopThreadPriority)) {
		//		OutputDebugLogTo(0, TRUE, "GrabLoopThreadPriority trying to Set to %d", nPriorityValue);
		try {
			pCamera->m_Camera.GrabLoopThreadPriority.SetValue(nPriorityValue);
			OutputDebugLogTo(0, TRUE, "GrabLoopThreadPriority Set to %d", nPriorityValue);
			/*nValue = pCamera->m_Camera.GrabLoopThreadPriority.GetMax();
			OutputDebugLogTo(0, TRUE, "GrabLoopThreadPriority Max value %d", nValue);
			nValue = pCamera->m_Camera.GrabLoopThreadPriority.GetMin();
			OutputDebugLogTo(0, TRUE, "GrabLoopThreadPriority Min value %d", nValue);
*/
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "GrabLoopThreadPriority Exception"); }
	}
	else {
		OutputDebugLogTo(0, TRUE, "********** [%s] Failed to set GrabLoopThreadPriority", pCamera->m_strCamName);
	}

	try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	catch (...) { OutputDebugLogTo(0, TRUE, "GrabLoopThreadPriority::StartGrabbing Exception"); }

	return 0;
}

int _stdcall GetStreamMaximumTransferSize(void *pCamModule, int * pdPayloadTransferSize)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*)pCamModule;

	if (GenApi::IsReadable(pCamera->m_Camera.GetStreamGrabberParams().MaxTransferSize)) {
		try {
			*pdPayloadTransferSize = static_cast<int>(pCamera->m_Camera.GetStreamGrabberParams().MaxTransferSize.GetValue());
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "GetStreamMaximumTransferSize() Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to GetStreamMaximumTransferSize()", pCamera->m_strCamName);

	return 0;

}

int _stdcall SetStreamMaximumTransferSize(void *pCamModule, int nPayloadTransferSize)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*)pCamModule;
	if (pCamera->m_bMonitorMode)
		return 0;

	try { pCamera->m_Camera.StopGrabbing(); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetStreamMaximumTransferSize::StopGrabbing Exception"); }

	if (IsWritable(pCamera->m_Camera.GetStreamGrabberParams().MaxTransferSize)) {
		//	int64_t value = (int64_t)nPayloadTransferSize;
		try {
			pCamera->m_Camera.GetStreamGrabberParams().MaxTransferSize.SetValue(nPayloadTransferSize);
			OutputDebugLogTo(0, TRUE, "MaxTransferSize set to %d", nPayloadTransferSize);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "SetStreamMaximumTransferSize() Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to SetStreamMaximumTransferSize() error  %d ", pCamera->m_strCamName, GetLastError());

	try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetStreamMaximumTransferSize::StartGrabbing Exception"); }

	return 0;
}

int _stdcall GetStreamMaximumTransferRange(void *pCamModule, int * pdMinPayload, int * pdMaxPayload)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*)pCamModule;

	if (GenApi::IsReadable(pCamera->m_Camera.GetStreamGrabberParams().MaxTransferSize)) {
		try {
			*pdMinPayload = pCamera->m_Camera.GetStreamGrabberParams().MaxTransferSize.GetMin();
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "GetStreamMaximumTransferRange() Exception"); }

		try {
			*pdMaxPayload = pCamera->m_Camera.GetStreamGrabberParams().MaxTransferSize.GetMax();
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "GetStreamMaximumTransferRange() Exception"); }
	}
	else {
		OutputDebugLog("********** [%s] Failed to Get GetStreamMaximumTransferRange", pCamera->m_strCamName);
	}

	return 0;
}

int _stdcall SetStreamMaximumBufferSize(void *pCamModule, int bufferSize)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*)pCamModule;
	if (pCamera->m_bMonitorMode)
		return 0;

	try { pCamera->m_Camera.StopGrabbing(); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetStreamMaximumBufferSize::StopGrabbing Exception"); }

	if (IsWritable(pCamera->m_Camera.GetStreamGrabberParams().MaxBufferSize)) {
		//	int64_t value = (int64_t)nPayloadTransferSize;
		try {

			pCamera->m_Camera.GetStreamGrabberParams().MaxBufferSize.SetValue(bufferSize);
			OutputDebugLogTo(0, TRUE, "SetStreamMaximumBufferSize() Set to %d", bufferSize);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "SetStreamMaximumBufferSize() Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to SetStreamMaximumBufferSize() error  %d ", pCamera->m_strCamName, GetLastError());

	try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetStreamMaximumBufferSize::StartGrabbing Exception"); }

	return 0;
}

int _stdcall GetStreamMaximumBufferSize(void *pCamModule, int *bufferSize)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*)pCamModule;

	if (GenApi::IsReadable(pCamera->m_Camera.GetStreamGrabberParams().MaxNumBuffer)) {
		try {
			*bufferSize = static_cast<int>(pCamera->m_Camera.GetStreamGrabberParams().MaxNumBuffer.GetValue());
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "GetStreamMaximumBufferSize() Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to GetStreamMaximumBufferSize()", pCamera->m_strCamName);

	return 0;
}

int _stdcall SetStreamMaximumBufferNumber(void *pCamModule, int bufferNumber)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*)pCamModule;
	if (pCamera->m_bMonitorMode)
		return 0;

	try { pCamera->m_Camera.StopGrabbing(); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetStreamMaximumBufferNumber::StopGrabbing Exception"); }

	if (IsWritable(pCamera->m_Camera.GetStreamGrabberParams().MaxNumBuffer)) {
		//	int64_t value = (int64_t)nPayloadTransferSize;
		try {
			pCamera->m_Camera.GetStreamGrabberParams().MaxNumBuffer.SetValue(bufferNumber);
			OutputDebugLogTo(0, TRUE, "SetStreamMaximumBufferNumber() Set to %d", bufferNumber);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "SetStreamMaximumBufferNumber() Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to SetStreamMaximumBufferNumber() error  %d ", pCamera->m_strCamName, GetLastError());

	try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetStreamMaximumBufferNumber::StartGrabbing Exception"); }

	return 0;
}

int _stdcall GetStreamMaximumBufferRange(void *pCamModule, int * pdMinBufSize, int * pdMaxBufferSize)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*)pCamModule;

	if (GenApi::IsReadable(pCamera->m_Camera.GetStreamGrabberParams().MaxBufferSize)) {
		try {
			*pdMinBufSize = pCamera->m_Camera.GetStreamGrabberParams().MaxBufferSize.GetMin();
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "GetStreamMaximumTransferRange() Exception"); }

		try {
			*pdMaxBufferSize = pCamera->m_Camera.GetStreamGrabberParams().MaxBufferSize.GetMax();
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "GetStreamMaximumTransferRange() Exception"); }
	}
	else {
		OutputDebugLog("********** [%s] Failed to Get GetStreamMaximumTransferRange", pCamera->m_strCamName);
	}

	return 0;
}

int _stdcall SetDefaultTransperSizeParameters(void *pCamModule)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*)pCamModule;
	if (pCamera->m_bMonitorMode)
		return 0;

	try { pCamera->m_Camera.StopGrabbing(); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetDefaultTransperSizeParameters::StopGrabbing Exception"); }

	if (IsWritable(pCamera->m_Camera.GetStreamGrabberParams().MaxTransferSize)) {
		try {
			pCamera->m_Camera.GetStreamGrabberParams().MaxTransferSize.SetValue(3000000);
			OutputDebugLogTo(0, TRUE, "MaxTransferSize set to %d", 3000000);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "MaxTransferSize Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to MaxTransferSize error  %d ", pCamera->m_strCamName, GetLastError());

	/*if (IsWritable(pCamera->m_Camera.GetStreamGrabberParams().MaxNumBuffer)) {
		try {
			pCamera->m_Camera.GetStreamGrabberParams().MaxNumBuffer.SetValue(16);
			OutputDebugLogTo(0, TRUE, "MaxNumBuffer set to %d", 16);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "MaxNumBuffer Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to MaxNumBuffer() error  %d ", pCamera->m_strCamName, GetLastError());*/

	if (IsWritable(pCamera->m_Camera.GetStreamGrabberParams().MaxBufferSize)) {
		try {
			pCamera->m_Camera.GetStreamGrabberParams().MaxBufferSize.SetValue(4194304);
			OutputDebugLogTo(0, TRUE, "MaxBufferSize set to %d", 4194304);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "MaxBufferSize Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to MaxBufferSize error  %d ", pCamera->m_strCamName, GetLastError());

	/*if (IsWritable(pCamera->m_Camera.GetStreamGrabberParams().MaxBufferSize)) {
		try {
			pCamera->m_Camera.GetStreamGrabberParams().MaxBufferSize.SetValue(4194304);
			OutputDebugLogTo(0, TRUE, "MaxBufferSize set to %d", 4194304);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "MaxBufferSize Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to MaxBufferSize error  %d ", pCamera->m_strCamName, GetLastError());*/


	if (IsWritable(pCamera->m_Camera.GetStreamGrabberParams().NumMaxQueuedUrbs)) {
		try {
			pCamera->m_Camera.GetStreamGrabberParams().NumMaxQueuedUrbs.SetValue(64);
			OutputDebugLogTo(0, TRUE, "NumMaxQueuedUrbs set to %d", 64);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "NumMaxQueuedUrbs Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to NumMaxQueuedUrbs error  %d ", pCamera->m_strCamName, GetLastError());


	try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetDefaultTransperSizeParameters::StartGrabbing Exception"); }

	return 0;


}

int _stdcall SetDeviceLinkThroughputLimit(void *pCamModule)
{
	if (!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*)pCamModule;
	if (pCamera->m_bMonitorMode)
		return 0;

	try { pCamera->m_Camera.StopGrabbing(); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetDeviceLinkThroughputLimit::StopGrabbing Exception"); }

	/*if (IsWritable(pCamera->m_Camera.DeviceLinkThroughputLimitMode)) {
		try {
			pCamera->m_Camera.DeviceLinkThroughputLimitMode.SetValue(BaslerCameraLinkDeviceClass::DeviceLinkThroughputLimitMode_On);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "DeviceLinkThroughputLimit Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to DeviceLinkThroughputLimit error  %d ", pCamera->m_strCamName, GetLastError());*/

	if (IsWritable(pCamera->m_Camera.DeviceLinkThroughputLimit)) {
		try {
			pCamera->m_Camera.DeviceLinkThroughputLimit.SetValue(36000000);
			OutputDebugLogTo(0, TRUE, "DeviceLinkThroughputLimit set to %d", 36000000);
		}
		catch (...) { OutputDebugLogTo(0, TRUE, "DeviceLinkThroughputLimit Exception"); }
	}
	else
		OutputDebugLog("********** [%s] Failed to DeviceLinkThroughputLimit error  %d ", pCamera->m_strCamName, GetLastError());


	try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	catch (...) { OutputDebugLogTo(0, TRUE, "SetDeviceLinkThroughputLimit::StartGrabbing Exception"); }

	return 0;
}
