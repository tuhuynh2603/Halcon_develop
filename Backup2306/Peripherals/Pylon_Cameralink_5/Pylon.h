#pragma once

#include <pylon/cameralink/BaslerCameraLinkInstantCamera.h>
#include "Basler_cameralink.h"
#include "ImageBuffer.h"

/* BitFlow SDK serial number 3071-6417-6709-3564 */

typedef Pylon::CBaslerCameraLinkInstantCamera Camera_t;

typedef void (CALLBACK* CALLBACKFUNCT)(int);

extern "C" int _stdcall InitDLL();

extern "C" int _stdcall FreeDLL();

extern "C" int _stdcall RegisterCamera(CString strCamFileName, CString strMPFFileName, void **pCamModule, BOOL bMonitorMode, BOOL bSetSoftTrigger, BOOL bSetOutputLine);

extern "C" int _stdcall ReleaseCamera(void **pCamModule);
extern "C" int _stdcall WaitForCompletion(void *pCamModule, CImageBuffer** pGrabbedImgBuf);
extern "C" int _stdcall CancelPendingGrab(void* pCamModule);
extern "C" int _stdcall EnqueueAsyncGrab(void *pCamModule);

extern "C" int _stdcall SetTriggerMode(void *pCamModule, int nMode, double dTrigDelay);
extern "C" int _stdcall SetTriggerParms(void *pCamModule, int nTriggerPolarity, int nPreTriggerDelay, int nDebouncingTime);
extern "C" int _stdcall GetTriggerMode(void *pCamModule, int *pnMode);
extern "C" int _stdcall SetGrabTimeOut(void *pCamModule, int nTimeout);
extern "C" int _stdcall GetResolution(void *pCamModule, CSize *pSize);

extern "C" int _stdcall ImageGrab(void *pCamModule, CImageBuffer** pGrabbedImgBuf);
extern "C" int _stdcall EnqueueAsyncGrabNoTrigger(void *pCamModule);

extern "C" int _stdcall RegHookFunction(void *pCamModule, CALLBACKFUNCT UserFunct);

extern "C" int _stdcall WaitForExposure(void *pCamModule, HANDLE eventKill);

int WaitForGrabEnd(void* pCamera);

extern "C" int _stdcall GetCameraAoi(void *pCamModule, CRect *pRect);
extern "C" int _stdcall SetCameraAoi(void *pCamModule, CRect rect);
extern "C" int _stdcall GetCameraGain(void *pCamModule, int *pnGain, int nGainMode);
extern "C" int _stdcall SetCameraGain(void *pCamModule, int nGain, int nGainMode);
extern "C" int _stdcall GetCameraGainRange(void *pCamModule, int * pnGainMin, int * pnGainMax, int nGainMode);
extern "C" int _stdcall GetExposure(void *pCamModule, int *pnExposure);
extern "C" int _stdcall SetExposure(void *pCamModule, int nExposure);
extern "C" int _stdcall GetExposureRange(void *pCamModule, double * pdMinExposure, double * pdMaxExposure);
extern "C" int _stdcall GetBlackLevel(void *pCamModule, int *pnBlackLevel);
extern "C" int _stdcall SetBlackLevel(void *pCamModule, int nBlackLevel);
extern "C" int _stdcall GetBytesPerPkt(void *pCamModule, long *pnBytesPerPkt, long* pnBytesPerPktMin, long* pnBytesPerPktMax, long* pnBytesPerPktInc);
extern "C" int _stdcall SetBytesPerPkt(void *pCamModule, long nBytesPerPkt , int nType);

extern "C" int _stdcall SetOutputLine(void *pCamModule);

extern "C" int _stdcall SetWhiteBalance(void *pCamModule, int nValue, int nColorSelect);

extern "C" int _stdcall EnableOutputLine(void *pCamModule, bool enable);

class CCamera
{
public:
	Basler_Camera_Interface::Basler_beA4000_62km framegrabber;

	long m_lCamNo;
	bool m_bIsColor;

	Camera_t m_Camera;
	CImageBuffer m_GrabBuffer;

	CString m_strCamName;
	int m_nGrabTimeout;
	
	CSize m_szResolution;

	CPoint m_posAoi;
	CSize m_szAoi;

	CRITICAL_SECTION m_csCamera;

	int m_nTriggerMode;
	BOOL m_bMonitorMode;

	CCamera();
	~CCamera();
	void (_stdcall* m_pUserFunct)(int);
};