#include "StdAfx.h"
#include <pylon/PylonIncludes.h>
#include "CameraCode.h"
#include "DebugLog.h"
#include "ImageFunction.h"
#include "Pylon.h"

//const int64_t bufferCount = 50;

CCamera::CCamera()
{
	m_nGrabTimeout		= 1000;
	m_szResolution		= CSize(0,0);
	m_nTriggerMode		= TRIG_SOFT;
	m_pUserFunct		= NULL;

	m_bIsColor = false;
	m_bMonitorMode = FALSE;
}

CCamera::~CCamera()
{
	OutputDebugLogTo(1, TRUE, "Pylon CameraLink Dynamic Library has been destroyed");
}

int _stdcall InitDLL()
{
	Pylon::PylonInitialize();
	OutputDebugLog("Pylon_CameraLink DLL Loaded");
	return 0;
}

int _stdcall FreeDLL()
{
	Pylon::PylonTerminate();
	OutputDebugLogTo(1, TRUE, "Pylon CameraLink Dynamic Library has been free");

	return 0;
}

int _stdcall RegisterCamera(CString strCamFileName, CString strMPFFileName, void **pCamModule, BOOL bMonitorMode, BOOL bSetSoftTrigger, BOOL bSetOutputLine)
{
	if(strCamFileName.CompareNoCase("none")==0)
		return 0;

	int nError = 0;

	CCamera * pCamera = new CCamera;

	OutputDebugLogTo(1, TRUE, "-------------------------------- RegisterCamera-------------------------------- ");
	//1. Use Bitflow to Open Board
	OutputDebugLogTo(1, TRUE, "RegisterCamera: pCamera->framegrabber.open()...");

	pCamera->framegrabber.open2();

	OutputDebugLogTo(1, TRUE, "RegisterCamera: pCamera->framegrabber.open()...Done");


	//2. Use Pylon to Attach Bitflow device
	Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
	Pylon::CDeviceInfo di;
	di.SetSerialNumber(strCamFileName.GetBuffer());
	strCamFileName.ReleaseBuffer();
	di.SetDeviceClass(Pylon::BaslerCameraLinkDeviceClass);

	Pylon::IPylonDevice* pDevice = NULL;
	try { pDevice = tlFactory.CreateDevice(di); } catch(...) {}
	if(!pDevice) {
		OutputDebugLogTo(1, TRUE, "Camera %s not found", strCamFileName);
		delete pCamera;
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "RegisterCamera: pCamera->m_Camera.Attach(pDevice)...");

	pCamera->m_Camera.Attach(pDevice);

	pCamera->m_bMonitorMode = bMonitorMode;

	if(bMonitorMode) {
		pCamera->m_Camera.RegisterConfiguration((Pylon::CConfigurationEventHandler*) NULL, Pylon::RegistrationMode_ReplaceAll, Pylon::Ownership_ExternalOwnership);
		pCamera->m_Camera.MonitorModeActive = true;
	}

	//3. Use Pylon to Open Camera
	OutputDebugLogTo(1, TRUE, "RegisterCamera: pCamera->m_Camera.Open()...");

	pCamera->m_Camera.Open();

	CString strModelName;
	strModelName.Format("%s", pCamera->m_Camera.GetDeviceInfo().GetModelName());
	pCamera->m_bIsColor = (strModelName.Right(1).MakeLower()=="c");

	pCamera->m_strCamName.Format("%s", pCamera->m_Camera.GetDeviceInfo().GetUserDefinedName().c_str());

	if( pCamera->m_strCamName.IsEmpty() )
		pCamera->m_strCamName.Format("no name");

	OutputDebugLogTo(1, TRUE, "[%s] Camera %s opened", pCamera->m_strCamName, strCamFileName);	

	InitializeCriticalSection(&pCamera->m_csCamera);

	//4. Query & Setting Camera Parameters
	if(GenApi::IsReadable(pCamera->m_Camera.WidthMax)) {
		try {pCamera->m_szResolution.cx = static_cast<int>(pCamera->m_Camera.WidthMax.GetValue()); }
		catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::Get WidthMax Exception"); }
		try { pCamera->m_szResolution.cy = static_cast<int>(pCamera->m_Camera.HeightMax.GetValue()); }
		catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::Get HeightMax Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to get Resolution X/Y", pCamera->m_strCamName);

	if(!bMonitorMode) {
		//+ Image Format Controls
		if(GenApi::IsWritable(pCamera->m_Camera.PixelFormat)) {
			try { pCamera->m_Camera.PixelFormat.SetValue(pCamera->m_bIsColor ? Basler_CLCameraParams::PixelFormat_BayerRG8 : Basler_CLCameraParams::PixelFormat_Mono8); } 
			catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::PixelFormat Exception"); }
			OutputDebugLogTo(1, TRUE, "[%s] PixelFormat(%s) set", pCamera->m_strCamName, pCamera->m_bIsColor ? "BayerBG8" : "Mono8");
		} 
		else
			OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set PixelFormat=(%s)", pCamera->m_strCamName, pCamera->m_bIsColor ? "BayerBG8" : "Mono8");

		//+ AOI Controls
		if(GenApi::IsReadable(pCamera->m_Camera.OffsetX)) {
			try { pCamera->m_posAoi.x = static_cast<int>(pCamera->m_Camera.OffsetX.GetValue()); } 
			catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::Get OffsetX Exception"); }
			try { pCamera->m_posAoi.y = static_cast<int>(pCamera->m_Camera.OffsetY.GetValue()); }
			catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::Get OffsetY Exception"); }
		}
		else
			OutputDebugLogTo(1, TRUE, "********** [%s] Failed to get Offset X/Y", pCamera->m_strCamName);

		pCamera->m_posAoi = CPoint(0, 0);
		pCamera->m_szAoi = pCamera->m_szResolution;

		/*if( pCamera->m_szAoi.cx != pCamera->framegrabber.getWidth() || pCamera->m_szAoi.cy != pCamera->framegrabber.getHeight() )
			OutputDebugLogTo(1, FALSE, "----- WARNING : Pylon AOI size [%d, %d], BitFlow size [%d, %d]. They are different!", pCamera->m_szAoi.cx, pCamera->m_szAoi.cy,
							 pCamera->framegrabber.getWidth(), pCamera->framegrabber.getHeight() );*/

		SetCameraAoi(pCamera, CRect(pCamera->m_posAoi, pCamera->m_szAoi));

		//+ Image Format Controls
		if(GenApi::IsWritable(pCamera->m_Camera.TestImageSelector)) {
			try { pCamera->m_Camera.TestImageSelector.SetValue(Basler_CLCameraParams::TestImageSelector_Off); }
			catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::TestImageSelector_Off Exception"); }
		}
		else
			OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TestImageSelector", pCamera->m_strCamName);

		//+ Transport Layer
		if(GenApi::IsWritable(pCamera->m_Camera.ClTapGeometry)) {
			try { pCamera->m_Camera.ClTapGeometry.SetValue(Basler_CLCameraParams::ClTapGeometry_Geometry1X10_1Y); }
			catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::ClTapGeometryEnums::ClTapGeometry_Geometry1X10_1Y Exception"); }
		}
		else
			OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set ClTapGeometry", pCamera->m_strCamName);

		//+ Acquisition Controls
		if(GenApi::IsAvailable(pCamera->m_Camera.TriggerSelector.GetEntry(Basler_CLCameraParams::TriggerSelector_FrameStart))) {
			
			//-Trigger Selector
			if(IsWritable(pCamera->m_Camera.TriggerSelector)) {
				try { pCamera->m_Camera.TriggerSelector.SetValue(Basler_CLCameraParams::TriggerSelector_FrameStart); }
				catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::TriggerSelector_FrameStart Exception"); }
			}

			//-Trigger Mode
			if(IsWritable(pCamera->m_Camera.TriggerMode)) {
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_CLCameraParams::TriggerMode_On); }
				catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::TriggerMode_On Exception"); }
			}
			/*if (IsWritable(pCamera->m_Camera.TriggerMode)) {
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_CLCameraParams::TriggerMode_Off); }
				catch (...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::TriggerMode_On Exception"); }
			}*/

			//-Exposure Mode
			if(IsWritable(pCamera->m_Camera.ExposureMode)) {
				try { pCamera->m_Camera.ExposureMode.SetValue(Basler_CLCameraParams::ExposureMode_Timed); }
				catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::ExposureMode_Timed Exception"); }
			}
			if(IsWritable(pCamera->m_Camera.ExposureAuto)) {
				try { pCamera->m_Camera.ExposureAuto.SetValue(Basler_CLCameraParams::ExposureAuto_Off); }
				catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::ExposureAuto_Off Exception"); }
			}
			//-Enable Acquisition Frame Rate
			if(IsWritable(pCamera->m_Camera.AcquisitionFrameRateEnable)) {
				try { pCamera->m_Camera.AcquisitionFrameRateEnable.SetValue(false); }
				catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::AcquisitionFrameRateEnable Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set AcquisitioFrameRateEnable", pCamera->m_strCamName);

			/*if(IsWritable(pCamera->m_Camera.AcquisitionMode)) {
				try { pCamera->m_Camera.AcquisitionMode.SetValue(Basler_CLCameraParams::AcquisitionMode_SingleFrame); }
				catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::AcquisitionMode_SingleFrame Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set AcquisitionMode_SingleFrame", pCamera->m_strCamName);*/

		}
		else {
			OutputDebugLogTo(1, TRUE, "RegisterCamera::TriggerSelector_FrameStart is not available");
		}

		if(bSetSoftTrigger) {
			pCamera->m_nTriggerMode = TRIG_HARD;
			SetTriggerMode(pCamera, TRIG_SOFT, 0);
		}
		else {
			pCamera->m_nTriggerMode = TRIG_SOFT;
			SetTriggerMode(pCamera, TRIG_HARD, 0);
		}

		if(bSetOutputLine)
			SetOutputLine(pCamera);

		pCamera->m_Camera.MaxNumBuffer = 50;

		OutputDebugLogTo(2, TRUE, "Pylon_CameraLink: MaxNumBuffer: %d (Min %d - Max %d), MaxNumQueuedBuffer: %d, MaxNumGrabResults: %d", pCamera->m_Camera.MaxNumBuffer.GetValue(),
			pCamera->m_Camera.MaxNumBuffer.GetMin(), pCamera->m_Camera.MaxNumBuffer.GetMax(),
			pCamera->m_Camera.MaxNumQueuedBuffer.GetValue(), pCamera->m_Camera.MaxNumGrabResults.GetValue());

		//try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
		//catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::StartGrabbing Exception"); }

		
	}
	else {
		//try { pCamera->m_Camera.GetStreamGrabberParams().TransmissionType = Basler_GigEStreamParams::TransmissionType_UseCameraConfig; }
		//catch(...) { OutputDebugLogTo(1, TRUE, "RegisterCamera::TransmissionType_UseCameraConfig Exception"); }

		//try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
		//catch(...) {
		//	OutputDebugLogTo(1, TRUE, "********** [%s] Monitor Mode: Failed to attach to camera", pCamera->m_strCamName);
		//	ReleaseCamera((void**) &pCamera);
		//	delete pCamera;
		//	return -1;
		//}
	}

	//5. Use Bitflow to Setup Acquisition Parameters
	OutputDebugLogTo(1, TRUE, "RegisterCamera: pCamera->framegrabber.set_up()...");
	pCamera->framegrabber.set_up2();
	
	if (pCamera->m_szAoi.cx != pCamera->framegrabber.getWidth() || pCamera->m_szAoi.cy != pCamera->framegrabber.getHeight())
		OutputDebugLogTo(1, FALSE, "----- WARNING : Pylon AOI size [%d, %d], BitFlow size [%d, %d]. They are different!", pCamera->m_szAoi.cx, pCamera->m_szAoi.cy,
			pCamera->framegrabber.getWidth(), pCamera->framegrabber.getHeight());


	*pCamModule = pCamera;

	return 0;
}

int _stdcall ReleaseCamera(void **pCamModule)
{
	if(pCamModule) {
		CCamera* pCamera =(CCamera*) *pCamModule;

		DeleteCriticalSection(&pCamera->m_csCamera);

		OutputDebugLogTo(1, TRUE, "ReleaseCamera: TriggerMode.SetValue...");
		if(!pCamera->m_bMonitorMode && pCamera->m_nTriggerMode == TRIG_SOFT) {
			if(IsWritable(pCamera->m_Camera.TriggerMode)) {
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_CLCameraParams::TriggerMode_Off); }
				catch(...) { OutputDebugLogTo(1, TRUE, "ReleaseCamera::TriggerMode_Off Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerMode", pCamera->m_strCamName);
		}

		OutputDebugLogTo(1, TRUE, "ReleaseCamera: framegrabber.close()...!!!");
		pCamera->framegrabber.close();

		/*OutputDebugLogTo(1, TRUE, "ReleaseCamera: m_Camera.Close()...");
		try { pCamera->m_Camera.Close(); }
		catch(...) { OutputDebugLogTo(1, TRUE, "********** [%s] Failed to CloseCamera", pCamera->m_strCamName); }*/

		if(pCamera->m_pUserFunct)
			pCamera->m_pUserFunct = NULL;
		
		/*delete pCamera;
		pCamera = NULL;
		*pCamModule = pCamera;*/
	
		OutputDebugLogTo(1, TRUE, "ReleaseCamera: End Function");
	}
	return 0;
}

int _stdcall RegHookFunction(void *pCamModule, CALLBACKFUNCT UserFunct)
{
	if(pCamModule) {
		CCamera *pCamera = (CCamera*) pCamModule;

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

int _stdcall ImageGrab(void *pCamModule, CImageBuffer **pGrabbedImgBuf)
{
	
	if( !EnqueueAsyncGrab( pCamModule ) )
		return WaitForCompletion( pCamModule, pGrabbedImgBuf );

	return 0;
}

int _stdcall EnqueueAsyncGrab(void *pCamModule)
{
	CCamera *pCamera = (CCamera*) pCamModule;

	//SetTriggerMode( pCamModule, TRIG_SOFT, 0 );

	pCamera->framegrabber.start_acquisition2();

	OutputDebugLogTo(1, TRUE, "TriggerSoftware.Execute()...");

	pCamera->m_Camera.TriggerSoftware.Execute();

	OutputDebugLogTo(1, TRUE, "TriggerSoftware.Execute()... Done");

	//if(!pCamModule) {
	//	ASSERT(!"CamModule is not Initialised");
	//	return -1;
	//}
	//
	//CCamera *pCamera = (CCamera*) pCamModule;
	//
	//
	//if(!pCamera->m_bMonitorMode) {
	//	if(!pCamera->m_Camera.IsGrabbing()) {
	//		OutputDebugLogTo(1, TRUE, "********** [%s] EnqueueSnap: Camera is not ready", pCamera->m_strCamName);
	//
	//		return -1;
	//	}
	//
	//	if(pCamera->m_Camera.WaitForFrameTriggerReady(1000, Pylon::TimeoutHandling_Return)) {
	//		if(pCamera->m_nTriggerMode == TRIG_SOFT)
	//			pCamera->m_Camera.ExecuteSoftwareTrigger();
	//	}
	//	else {
	//		OutputDebugLogTo(1, TRUE, "********** [%s] EnqueueAsyncGrab: WaitForFrameTriggerReady failed", pCamera->m_strCamName);
	//		return -1;
	//	}
	//
	//}
	//else {
	//	if(!pCamera->m_Camera.WaitForFrameTriggerReady(1000, Pylon::TimeoutHandling_Return)) {
	//		OutputDebugLogTo(1, TRUE, "********** [%s] EnqueueSnap: WaitForFrameTriggerReady failed", pCamera->m_strCamName);
	//		return -1;
	//	}
	//}

	return 0;
}

int _stdcall WaitForCompletion(void *pCamModule, CImageBuffer** pGrabbedImgBuf)
{
	CCamera *pCamera = (CCamera*) pCamModule;

	pCamera->framegrabber.getImage2(pCamera->m_GrabBuffer);
	*pGrabbedImgBuf = &pCamera->m_GrabBuffer;

	return 0;

	//if(!pCamModule) {
	//	ASSERT(!"CamModule is not Initialised");
	//	return -1;
	//}
	//
	//CCamera *pCamera =(CCamera*) pCamModule;
	//
	//Pylon::CGrabResultPtr pGrabResult;
	//int nError = 0;
	//
	//if(pCamera->m_Camera.RetrieveResult(pCamera->m_nGrabTimeout, pGrabResult, Pylon::TimeoutHandling_Return)) {
	//	if(pGrabResult && pGrabResult->GrabSucceeded()) {
	//		pCamera->m_GrabBuffer.AllocateIndirect(CSize(pGrabResult->GetWidth(), pGrabResult->GetHeight()), (BYTE*) pGrabResult->GetBuffer());
	//
	//		*pGrabbedImgBuf = pCamera->m_GrabBuffer;
	//	}
	//	else {
	//		OutputDebugLogTo(1, TRUE, "[%s] Grab Error; GrabStatus = %s (0x%X)", 
	//			pCamera->m_strCamName, pGrabResult->GetErrorDescription().c_str(), pGrabResult->GetErrorCode());
	//		nError = -2;
	//	}
	//}
	//else {
	//	OutputDebugLogTo(1, TRUE, "[%s] RetrieveResult Error; Timeout is %d", pCamera->m_strCamName, pCamera->m_nGrabTimeout); 
	//
	//	nError = -1;
	//}
	//
	//
	//if(pCamera->m_pUserFunct)
	//	(*(pCamera->m_pUserFunct))(1000);
	//
	//return nError;
}

int _stdcall CancelPendingGrab(void* pCamModule)
{
	//CCamera *pCamera = (CCamera*) pCamModule;
	//
	//try { pCamera->m_Camera.StopGrabbing(); }
	//	catch(...) { OutputDebugLogTo(1, TRUE, "CancelPendingGrab::StopGrabbing Exception"); }
	//try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	//	catch(...) { OutputDebugLogTo(1, TRUE, "CancelPendingGrab::StartGrabbing Exception"); }

	//OutputDebugLogTo(1, TRUE, "[%s] Cancelled grab", pCamera->m_strCamName);

	return 0;
}

int _stdcall WaitForExposure(void *pCamModule, HANDLE eventKill)
{

	return -1;
}

int WaitForGrabEnd(void* pCam)
{
	return 0;
}

//<---------------
// NOT IN USE
int _stdcall EnqueueAsyncGrabNoTrigger(void *pCamModule)
{
	return 0;

	//if(!pCamModule) {
	//	ASSERT(!"CamModule is not Initialised");
	//	return -1;
	//}
	//
	//CCamera *pCamera = (CCamera*) pCamModule;
	//if(pCamera->m_bMonitorMode)
	//	return 0;
	//
	//bool bTrigHard = false;
	//if(pCamera->m_nTriggerMode == TRIG_HARD) {
	//	if(IsWritable(pCamera->m_Camera.TriggerSource)) {
	//		try { pCamera->m_Camera.TriggerSource.SetValue(Basler_CLCameraParams::TriggerSource_Software); }
	//		catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerMode::TriggerSource_Software Exception"); }
	//	}
	//	else
	//		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerSource", pCamera->m_strCamName);
	//	pCamera->m_nTriggerMode = TRIG_SOFT;
	//	bTrigHard = true;
	//}
	//
	//int nResult = EnqueueAsyncGrab(pCamModule);
	//
	//if(bTrigHard) {
	//	Sleep(10);
	//	if(IsWritable(pCamera->m_Camera.TriggerSource)) {
	//		try { pCamera->m_Camera.TriggerSource.SetValue(Basler_CLCameraParams::TriggerSource_Line1); }
	//		catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerMode::TriggerSource_Line1 Exception"); }
	//	}
	//	else
	//		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerSource", pCamera->m_strCamName);
	//	pCamera->m_nTriggerMode = TRIG_HARD;
	//}
	//
	//return nResult;
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

	//if(pCamera->m_nTriggerMode == nMode) return 0;

	OutputDebugLogTo(1, TRUE, "SetTriggerMode::%d", nMode);

	switch(nMode) {
		case TRIG_NONE:
			if(IsWritable(pCamera->m_Camera.TriggerMode)) { 
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_CLCameraParams::TriggerMode_Off); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerMode::TriggerMode_Off Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerMode", pCamera->m_strCamName);

			pCamera->m_nGrabTimeout = 1000;

			//OutputDebugLogTo(1, TRUE, "[%s] Set trigger mode to OFF is completed", pCamera->m_strCamName);

			break;

		case TRIG_SOFT:
			if(IsWritable(pCamera->m_Camera.TriggerMode)) { 
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_CLCameraParams::TriggerMode_On); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerMode::TriggerMode_On Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerMode", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerSource)) {
				try { pCamera->m_Camera.TriggerSource.SetValue(Basler_CLCameraParams::TriggerSource_Software); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerMode::TriggerSource_Software Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerSource", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerDelayAbs)) {
				try { pCamera->m_Camera.TriggerDelayAbs.SetValue(0.0); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerDelayAbs::dTrigDelay Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerDelayAbs:dTrigDelay", pCamera->m_strCamName, dTrigDelay);

			pCamera->m_nGrabTimeout = 1000;

			//OutputDebugLogTo(1, TRUE, "[%s] Set trigger mode to SOFT is completed", pCamera->m_strCamName);

			break;

		case TRIG_HARD:
			if(IsWritable(pCamera->m_Camera.TriggerMode)) { 
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_CLCameraParams::TriggerMode_On); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerMode::TriggerMode_On Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerMode", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerSource)) {
				try { pCamera->m_Camera.TriggerSource.SetValue(Basler_CLCameraParams::TriggerSource_CC1); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerMode::TriggerSource_CC1 Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerSource:CC1", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerActivation)) {
				try { pCamera->m_Camera.TriggerActivation.SetValue(Basler_CLCameraParams::TriggerActivation_RisingEdge); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerActivation::TriggerActivation_RisingEdge Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerActivation:RisingEdge", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerDelayAbs)) {
				try { pCamera->m_Camera.TriggerDelayAbs.SetValue(dTrigDelay); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerDelayAbs::dTrigDelay Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerDelayAbs:dTrigDelay", pCamera->m_strCamName, dTrigDelay);

			pCamera->m_nGrabTimeout = INFINITE;

			//OutputDebugLogTo(1, TRUE, "[%s] Set trigger mode to HARD is completed, delay %.1f us", pCamera->m_strCamName, dTrigDelay);

			break;

		case TRIG_HARD_NO_DELAY:
			if(IsWritable(pCamera->m_Camera.TriggerMode)) { 
				try { pCamera->m_Camera.TriggerMode.SetValue(Basler_CLCameraParams::TriggerMode_On); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerMode::TriggerMode_On Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerMode", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerSource)) {
				try { pCamera->m_Camera.TriggerSource.SetValue(Basler_CLCameraParams::TriggerSource_CC1); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerMode::TriggerSource_CC1 Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerSource:CC1", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerActivation)) {
				try { pCamera->m_Camera.TriggerActivation.SetValue(Basler_CLCameraParams::TriggerActivation_RisingEdge); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerActivation::TriggerActivation_RisingEdge Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerActivation:RisingEdge", pCamera->m_strCamName);

			if(IsWritable(pCamera->m_Camera.TriggerDelayAbs)) {
				try { pCamera->m_Camera.TriggerDelayAbs.SetValue(0.0); }
				catch(...) { OutputDebugLogTo(1, TRUE, "SetTriggerDelayAbs::dTrigDelay Exception"); }
			}
			else
				OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set TriggerDelayAbs:dTrigDelay", pCamera->m_strCamName, 0.0);

			//OutputDebugLogTo(1, TRUE, "[%s] Set trigger mode to HARD is completed, delay 0.0 us", pCamera->m_strCamName);
			

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

	return -1;
}

int _stdcall GetTriggerMode(void *pCamModule, int *pnMode)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;

	*pnMode = pCamera->m_nTriggerMode;

	return 0;

}

int _stdcall GetResolution(void *pCamModule, CSize *pSize)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;

	*pSize = pCamera->m_szResolution;

	return 0;
}

int _stdcall GetCameraAoi(void *pCamModule, CRect *pRect)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;

	if(GenApi::IsReadable(pCamera->m_Camera.OffsetX) && GenApi::IsReadable(pCamera->m_Camera.OffsetY)) {
		try { pCamera->m_posAoi.x = static_cast<int>(pCamera->m_Camera.OffsetX.GetValue()); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetCameraAoi::Get OffsetX Exception"); }
		try { pCamera->m_posAoi.y = static_cast<int>(pCamera->m_Camera.OffsetY.GetValue()); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetCameraAoi::Get OffsetY Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to get Offset X/Y", pCamera->m_strCamName);
	
	if(GenApi::IsReadable(pCamera->m_Camera.Width) && GenApi::IsReadable(pCamera->m_Camera.Height)) {
		try { pCamera->m_szAoi.cx = static_cast<int>(pCamera->m_Camera.Width.GetValue()); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetCameraAoi::Get Width Exception"); }
		try { pCamera->m_szAoi.cy = static_cast<int>(pCamera->m_Camera.Height.GetValue()); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetCameraAoi::Get Height Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to get AOI X/Y", pCamera->m_strCamName);

	pRect->SetRect(pCamera->m_posAoi.x, pCamera->m_posAoi.y, pCamera->m_posAoi.x+pCamera->m_szAoi.cx, pCamera->m_posAoi.y+pCamera->m_szAoi.cy);

	return 0;
}

int _stdcall SetCameraAoi(void *pCamModule, CRect rect)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	// Check current values
	CRect currectAoi;

	int error = GetCameraAoi(pCamModule, &currectAoi);
	if(error)
		return error;

	if(currectAoi == rect)
		return 0;


	EnterCriticalSection(&pCamera->m_csCamera);

	bool bGrabbing = false;

	//if(pCamera->m_Camera.IsGrabbing()) {
	//	bGrabbing = true;
	//	try { pCamera->m_Camera.StopGrabbing(); }
	//		catch(...) { OutputDebugLogTo(1, TRUE, "SetCameraAoi::StopGrabbing Exception"); }
	//}


	pCamera->m_szAoi = rect.Size();
	pCamera->m_szAoi.cx &= ~3;
	pCamera->m_posAoi = rect.TopLeft();

	OutputDebugLogTo(1, TRUE, "[%s] SetCameraAOI: Offset(%d,%d) Size(%d,%d) Resolution(%d,%d)", pCamera->m_strCamName,
		pCamera->m_posAoi.x, pCamera->m_posAoi.y,
		pCamera->m_szAoi.cx, pCamera->m_szAoi.cy,
		pCamera->m_szResolution.cx, pCamera->m_szResolution.cy);

	if(GenApi::IsWritable(pCamera->m_Camera.OffsetX)) {
		try { pCamera->m_Camera.OffsetX.SetValue(0); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetCameraAoi::Set OffsetX Exception"); }
		try { pCamera->m_Camera.OffsetY.SetValue(0); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetCameraAoi::Set OffsetY Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set Offset X/Y", pCamera->m_strCamName);

	if(GenApi::IsWritable(pCamera->m_Camera.Width)) {
		try { pCamera->m_Camera.Width.SetValue(pCamera->m_szAoi.cx); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetCameraAoi::Set Width Exception"); }
		try { pCamera->m_Camera.Height.SetValue(pCamera->m_szAoi.cy); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetCameraAoi::Set Height Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set AOI X/Y", pCamera->m_strCamName);

	if(GenApi::IsWritable(pCamera->m_Camera.OffsetX)) {
		try { pCamera->m_Camera.OffsetX.SetValue(pCamera->m_posAoi.x); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetCameraAoi::Set OffsetX Exception"); }
		try { pCamera->m_Camera.OffsetY.SetValue(pCamera->m_posAoi.y); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetCameraAoi::Set OffsetY Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set Offset X/Y", pCamera->m_strCamName);
	

	//if(bGrabbing) {
	//	try { pCamera->m_Camera.StartGrabbing(Pylon::GrabStrategy_OneByOne); }
	//		catch(...) { OutputDebugLogTo(1, TRUE, "SetCameraAoi::StartGrabbing Exception"); }
	//}

	LeaveCriticalSection(&pCamera->m_csCamera);


	return 0;
}

int _stdcall GetExposure(void *pCamModule, int * pnExposure)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;

	double dExposureValue;
	if(GenApi::IsReadable(pCamera->m_Camera.ExposureTimeAbs)) {
		try { 
			dExposureValue = pCamera->m_Camera.ExposureTimeAbs.GetValue(); 
			*pnExposure = dExposureValue;
		}
		catch(...) { OutputDebugLogTo(1, TRUE, "GetExposure::ExposureTimeAbs Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to get ExposureTimeAbs", pCamera->m_strCamName);

	return 0;
}

int _stdcall SetExposure(void *pCamModule, int nExposure)
{

	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	double dExposureValue = nExposure;
	if(GenApi::IsWritable(pCamera->m_Camera.ExposureTimeAbs)) {
		try { pCamera->m_Camera.ExposureTimeAbs.SetValue(dExposureValue); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetExposure::ExposureTimeAbs Exception (Exposure Value: %.2f) GetLastError: %d", dExposureValue, GetLastError()); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set ExposureTimeAbs", pCamera->m_strCamName);


	return 0;
}

int _stdcall GetExposureRange(void *pCamModule, double * pdMinExposure, double * pdMaxExposure)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;

	if(GenApi::IsReadable(pCamera->m_Camera.ExposureTimeAbs)) {
		try { *pdMinExposure = pCamera->m_Camera.ExposureTimeAbs.GetMin(); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetExposureRange::ExposureTimeAbs.GetMin() Exception"); }

		try { *pdMaxExposure = pCamera->m_Camera.ExposureTimeAbs.GetMax(); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetExposureRange::ExposureTimeAbs.GetMax() Exception"); }
	}
	else {
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to get ExposureTimeAbs", pCamera->m_strCamName);
	}

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
		try { pCamera->m_Camera.GainSelector.SetValue(nGainMode==0 ? Basler_CLCameraParams::GainSelector_All : 
			(nGainMode==1 ? Basler_CLCameraParams::GainSelector_Tap1 : Basler_CLCameraParams::GainSelector_Tap2)); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetCameraGain::GainSelector Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set GainSelector", pCamera->m_strCamName);

	if(GenApi::IsReadable(pCamera->m_Camera.GainRaw)) {
		try { *pnGain = (int)pCamera->m_Camera.GainRaw.GetValue(); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetCameraGain::GainRaw Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to get GainRaw", pCamera->m_strCamName);

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
		try { pCamera->m_Camera.GainSelector.SetValue(nGainMode==0 ? Basler_CLCameraParams::GainSelector_All : 
			(nGainMode==1 ? Basler_CLCameraParams::GainSelector_Tap1 : Basler_CLCameraParams::GainSelector_Tap2)); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetCameraGain::GainSelector Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set GainSelector", pCamera->m_strCamName);

	if(GenApi::IsWritable(pCamera->m_Camera.GainRaw)) {
		try { pCamera->m_Camera.GainRaw.SetValue(nGain); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetCameraGain::GainRaw Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set GainRaw", pCamera->m_strCamName);

	return 0;
}

int _stdcall GetCameraGainRange(void *pCamModule, int * pnGainMin, int * pnGainMax, int nGainMode)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;

	if(GenApi::IsWritable(pCamera->m_Camera.GainSelector)) {
		try { pCamera->m_Camera.GainSelector.SetValue(nGainMode==0 ? Basler_CLCameraParams::GainSelector_All : 
			(nGainMode==1 ? Basler_CLCameraParams::GainSelector_All : Basler_CLCameraParams::GainSelector_All)); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetCameraGain::GainSelector Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set GainSelector", pCamera->m_strCamName);

	if(GenApi::IsReadable(pCamera->m_Camera.GainRaw)) {
		try { *pnGainMin = static_cast<int>(pCamera->m_Camera.GainRaw.GetMin()); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetCameraGain::Gain.GetMin() Exception"); }

		try { *pnGainMax = static_cast<int>(pCamera->m_Camera.GainRaw.GetMax()); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetCameraGain::Gain.GetMax() Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to get GainRaw", pCamera->m_strCamName);

	return 0;
}

int _stdcall SetWhiteBalance(void *pCamModule, int nValue, int nColorSelect)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(GenApi::IsWritable(pCamera->m_Camera.BalanceRatioSelector)) {
		try { pCamera->m_Camera.BalanceRatioSelector.SetValue(nColorSelect==0 ? Basler_CLCameraParams::BalanceRatioSelector_Red : 
			(nColorSelect==1 ? Basler_CLCameraParams::BalanceRatioSelector_Green : Basler_CLCameraParams::BalanceRatioSelector_Blue)); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetWhiteBalance::BalanceRatioSelector Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set BalanceRatioSelector", pCamera->m_strCamName);

	if(GenApi::IsWritable(pCamera->m_Camera.BalanceRatioRaw)) {
		try { pCamera->m_Camera.BalanceRatioRaw.SetValue(nValue); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetWhiteBalance::BalanceRatioRaw Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set BalanceRatioRaw", pCamera->m_strCamName);

	return 0;
}

int _stdcall GetBlackLevel(void *pCamModule, int *pnBlackLevel)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;

	if(GenApi::IsReadable(pCamera->m_Camera.BlackLevelRaw)) {
		try { *pnBlackLevel = static_cast<int>(pCamera->m_Camera.BlackLevelRaw.GetValue()); }
		catch(...) { OutputDebugLogTo(1, TRUE, "GetBlackLevel::BlackLevelRaw Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to get BLackLevelRaw", pCamera->m_strCamName);

	return 0;
}

int _stdcall SetBlackLevel(void *pCamModule, int nBlackLevel)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(GenApi::IsWritable(pCamera->m_Camera.BlackLevelRaw)) {
		try { pCamera->m_Camera.BlackLevelRaw.SetValue(nBlackLevel); }
		catch(...) { OutputDebugLogTo(1, TRUE, "SetBlackLevel::nBlackLevel Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set BlackLevelRaw", pCamera->m_strCamName);

	return 0;
}

int _stdcall GetBytesPerPkt(void *pCamModule, long *pnBytesPerPkt, long* pnBppMin, long* pnBppMax, long* pnBppInc)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;

	return -1;
}

int _stdcall SetBytesPerPkt(void *pCamModule, long nBytesPerPkt,int TransLayerPos)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}
	CCamera* pCamera = (CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	return -1;
}

int _stdcall SetOutputLine(void *pCamModule)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera *pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(IsWritable(pCamera->m_Camera.LineSelector)) {
		try { pCamera->m_Camera.LineSelector.SetValue(Basler_CLCameraParams::LineSelector_ClSpare); }
		catch(...) { OutputDebugLogTo(1, TRUE, "LineSelector::LineSelector_ClSpare Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set LineSelector", pCamera->m_strCamName);

	if(IsWritable(pCamera->m_Camera.LineMode)) {
		try { pCamera->m_Camera.LineMode.SetValue(Basler_CLCameraParams::LineMode_Output); }
		catch(...) { OutputDebugLogTo(1, TRUE, "LineMode::LineMode_Output Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set LineMode", pCamera->m_strCamName);

	if(IsWritable(pCamera->m_Camera.LineSource)) {
		try { pCamera->m_Camera.LineSource.SetValue(Basler_CLCameraParams::LineSource_ExposureActive); }
		catch(...) { OutputDebugLogTo(1, TRUE, "LineSource::LineSource_ExposureActive Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set LineSource", pCamera->m_strCamName);

	if(IsWritable(pCamera->m_Camera.LineInverter)) {
		try { pCamera->m_Camera.LineInverter.SetValue(false); }
		catch(...) { OutputDebugLogTo(1, TRUE, "LineInverter (false) Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set LineInverter", pCamera->m_strCamName);

	//if(IsWritable(pCamera->m_Camera.UserOutputSelector)) {
	//	try { pCamera->m_Camera.UserOutputSelector.SetValue(Basler_CLCameraParams::UserOutputSelector_UserOutputClSpare); }
	//	catch(...) { OutputDebugLogTo(1, TRUE, "UserOutputSelector::UserOutputSelector_UserOutputClSpare Exception"); }
	//}
	//else
	//	OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set UserOutputSelector", pCamera->m_strCamName);

	return 0;
}


int _stdcall EnableOutputLine(void *pCamModule, bool enable)
{
	if(!pCamModule) {
		ASSERT(!"CamModule is not Initialised");
		return -1;
	}

	CCamera *pCamera =(CCamera*) pCamModule;
	if(pCamera->m_bMonitorMode)
		return 0;

	if(enable) {
		if(IsWritable(pCamera->m_Camera.LineSource)) {
			try { pCamera->m_Camera.LineSource.SetValue(Basler_CLCameraParams::LineSource_ExposureActive); }
			catch(...) { OutputDebugLogTo(1, TRUE, "LineSource::LineSource_ExposureActive Exception"); }
		}
		else
			OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set LineSource", pCamera->m_strCamName);
	}
	else {
		if(IsWritable(pCamera->m_Camera.LineSource)) {
		try { pCamera->m_Camera.LineSource.SetValue(Basler_CLCameraParams::LineSource_UserOutput1); }
		catch(...) { OutputDebugLogTo(1, TRUE, "LineSource::LineSource_UserOutput1 Exception"); }
	}
	else
		OutputDebugLogTo(1, TRUE, "********** [%s] Failed to set LineSource", pCamera->m_strCamName);
	}

	return 0;
}