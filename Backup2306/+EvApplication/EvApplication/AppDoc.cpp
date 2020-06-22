/// AppDoc.cpp : implementation of the CAppDoc class
/// Udupa; April'2012

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.

#include "Application.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include "AppView.h"
#include <direct.h>
#include "ImageQueueHandler.h"
#include "CameraCode.h"
#include "EvMainFrame.h"
#include "MainFrame.h"
#include "CameraSetupDialog.h"
#include "LightControlDlg.h"
#include "EdgeMatching.h"
#include "ColorConversion.h"
#include "SelectGoodBDlg.h"
#include "ImageSelectionDlg.h"
#include "StepSelection.h"
#include <propkey.h>
#include "CalibrationSettings.h"
#include "PVIInspectionParameters.h"
#include "MotionControlDialog.h"
#include "SelectFolder.h"
#include "namespace_utilities_function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAppDoc

IMPLEMENT_DYNCREATE(CAppDoc, CEvImageDoc)

BEGIN_MESSAGE_MAP(CAppDoc, CEvImageDoc)
	ON_COMMAND(ID_INSPECTION_INSPECT, OnInspectionInspect)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_INSPECT, OnUpdateInspectionInspect)
	ON_COMMAND(ID_STATIC_LOADFROMFILE, OnToolsStaticimageLoadFromFile)
	ON_COMMAND(ID_STATIC_LOADTEACHIMAGE, OnToolsStaticimageLoadTeachImage)
	ON_COMMAND(ID_STATIC_SAVE, OnToolsStaticimageSave)
	ON_UPDATE_COMMAND_UI(ID_STATIC_SAVE, OnUpdateToolsStaticimageSave)
	ON_UPDATE_COMMAND_UI(ID_STATIC_LOADFROMFILE, OnUpdateToolsStaticimageLoadFromFile)
	ON_UPDATE_COMMAND_UI(ID_STATIC_LOADTEACHIMAGE, OnUpdateToolsStaticimageLoadTeachImage)
	ON_COMMAND(ID_TOOLS_STATICIMAGE, OnToolsStaticimage)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_STATICIMAGE, OnUpdateToolsStaticimage)
	ON_COMMAND(ID_INSPECTION_TEACH, OnInspectionTeach)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_TEACH, OnUpdateInspectionTeach)
	ON_COMMAND(ID_INSPECTION_TEACH_SINGLE_IMAGE, OnInspectionTeachSingleImage)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_TEACH_SINGLE_IMAGE, OnUpdateInspectionTeachSingleImage)
	ON_COMMAND(ID_INSPECTION_DEBUG, OnInspectionDebug)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_DEBUG, OnUpdateInspectionDebug)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_NEXT, OnUpdateInspectionNext)
	ON_COMMAND(ID_INSPECTION_NEXT, OnInspectionNext)
	ON_COMMAND(ID_INSPECTION_ABORT, OnInspectionAbort)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_ABORT, OnUpdateInspectionAbort)
	ON_COMMAND(ID_CAMERA_SNAP, OnCameraSnap)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SNAP, OnUpdateCameraSnap)
	ON_COMMAND(ID_CAMERA_STREAM, OnCameraStream)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_STREAM, OnUpdateCameraStream)
	ON_COMMAND(ID_CAMERA_LIGHT, OnCameraLight)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_LIGHT, OnUpdateCameraLight)
	ON_COMMAND(ID_PARM_VISION, OnParmVision)
	ON_UPDATE_COMMAND_UI(ID_PARM_VISION, OnUpdateParmVision)
	ON_COMMAND(ID_PARM_TEACH, OnParmTeach)
	ON_UPDATE_COMMAND_UI(ID_PARM_TEACH, OnUpdateParmTeach)
	ON_COMMAND(ID_PARM_DEFECT, OnParmDefect)
	ON_UPDATE_COMMAND_UI(ID_PARM_DEFECT, OnUpdateParmDefect)
	ON_COMMAND(ID_INSPECTION_STOREDIMAGES, OnInspectionStoredimages)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_STOREDIMAGES, OnUpdateInspectionStoredimages)
	ON_COMMAND(ID_INSPECTION_CYCLE, OnInspectionCycle)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_CYCLE, OnUpdateInspectionCycle)
	ON_COMMAND(ID_INSPECTION_CAMERAACTIVE, OnInspectionCameraactive)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_CAMERAACTIVE, OnUpdateInspectionCameraactive)
	ON_COMMAND(ID_INSPECTION_CYCLEAUTOSTEPMODE, OnInspectCycleAutoStepMode)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_CYCLEAUTOSTEPMODE, OnUpdateInspectCycleAutoStepMode)
	ON_COMMAND(ID_INSPECTION_SETSTOREDIMAGEFOLDER, OnInspectStoredImagesPath)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_SETSTOREDIMAGEFOLDER, OnUpdateInspectStoredImagesPath)
	ON_COMMAND(ID_INSPECTION_SAVEPASSIMAGES, OnInspectSavePassImages)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_SAVEPASSIMAGES, OnUpdateInspectSavePassImages)
	ON_COMMAND(ID_INSPECTION_SAVEFAILIMAGES1, OnInspectSaveFailImages)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_SAVEFAILIMAGES1, OnUpdateInspectSaveFailImages)
	ON_COMMAND(ID_PARM_HARDWARE_CAMERA, OnCameraParmButton)
	ON_UPDATE_COMMAND_UI(ID_PARM_HARDWARE_CAMERA, OnUpdateCameraParmButton)
	ON_COMMAND(ID_VIEWOPTIONS_SETUPGRID, OnViewOptionsSetupGrid)
	ON_UPDATE_COMMAND_UI(ID_VIEWOPTIONS_SETUPGRID, OnUpdateViewOptionsSetupGrid)
	ON_COMMAND(ID_VIEWOPTIONS_SHOWOVERLAY, OnViewOptionsShowOverlay)
	ON_UPDATE_COMMAND_UI(ID_VIEWOPTIONS_SHOWOVERLAY, OnUpdateViewOptionsShowOverlay)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OPTIONS, OnUpdateViewOptions)
	ON_COMMAND(ID_FOCUSING_TOOL, OnViewOpticsPanel)
	ON_UPDATE_COMMAND_UI(ID_FOCUSING_TOOL, OnUpdateViewOpticsPanel)
	ON_COMMAND(ID_VIEW_OPTICS_ROI, OnViewOpticsRoi)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OPTICS_ROI, OnUpdateViewOpticsRoi)
	ON_COMMAND(ID_VIEWOPTIONS_SELECTGOODB, OnViewSelectGoodB)
	ON_UPDATE_COMMAND_UI(ID_VIEWOPTIONS_SELECTGOODB, OnUpdateViewSelectGoodB)
	ON_CBN_SELCHANGE(IDC_FRAMECOUNT_COMBO, OnSelchangeFramecountCombo)

	ON_COMMAND(ID_STEPDEBUG_SELECTION, OnInspectStepDebugSelection)
	ON_UPDATE_COMMAND_UI(ID_STEPDEBUG_SELECTION, OnUpdateInspectStepDebugSelection)

	ON_COMMAND(ID_PVI_DEFECT_SHOW, OnPviDefectShow)
	ON_UPDATE_COMMAND_UI(ID_PVI_DEFECT_SHOW, OnUpdatePviDefectShow)

	ON_COMMAND(ID_BUTTON_CALIBRATION_SETTINGS, OnCalibrationSettings)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CALIBRATION_SETTINGS, OnUpdateCalibrationSettings)
	//ON_MESSAGE(WM_OPTICS_FRAMECHANGED, OnSelchangeFramecountCombo)

	ON_COMMAND(ID_STATIC_LOADTEACHIMAGE_SINGLE, OnToolsStaticimageLoadTeachImageSingle)
	ON_UPDATE_COMMAND_UI(ID_STATIC_LOADTEACHIMAGE_SINGLE, OnUpdateToolsStaticimageLoadTeachImageSingle)
	ON_COMMAND(ID_LOAD_ALL_IMG_FILES_BUTTON, OnToolsStaticimageLoadAllFromFile)
	ON_UPDATE_COMMAND_UI(ID_LOAD_ALL_IMG_FILES_BUTTON, OnUpdateToolsStaticimageLoadAllFromFile)
	//	ON_COMMAND(ID_SAVE_ALL_BUTTON, OnToolsStaticimageSaveAll)
	//	ON_UPDATE_COMMAND_UI(ID_SAVE_ALL_BUTTON, OnUpdateToolsStaticimageSaveAll)

	ON_COMMAND(ID_INSP_ALL_BUTTON, OnInspectionInspectAll)
	ON_UPDATE_COMMAND_UI(ID_INSP_ALL_BUTTON, OnUpdateInspectionInspectAll)

	ON_COMMAND(ID_CAMERA_SNAP_ALL_BUTTON, OnCameraSnapAll)
	ON_UPDATE_COMMAND_UI(ID_CAMERA_SNAP_ALL_BUTTON, OnUpdateCameraSnapAll)

	ON_COMMAND(ID_BUTTON_SAMPLING, &CAppDoc::OnButtonSampling)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SAMPLING, &CAppDoc::OnUpdateButtonSampling)
END_MESSAGE_MAP()


// CAppDoc construction/destruction

CAppDoc::CAppDoc()
{
	m_ImageViewInfo.sizeImage.cx = 1024;
	m_ImageViewInfo.sizeImage.cy = 1024;

	m_pApp = NULL;
	m_pOpticsDialog = NULL;

	m_strConfigDir.Empty();

	m_bTeachSingleImage = FALSE;
	m_nInspCounter = 0;
	m_pTrack = NULL;
	m_bInspSavedImage = FALSE;
	m_bSavedImagesStep = FALSE;
	m_strLoadImage.Empty();

	///m_pContactLensInspWnd = NULL;
	m_bStoredImages = FALSE;
	m_bInspectCycle = FALSE;
	m_bCycleAutoStepMode = FALSE;
	m_bCameraActive = TRUE;
	m_bInspecting = FALSE;


	m_bSetupGrid = FALSE;
	m_pColorConvertor = NULL;
	m_nDefectCount = 0;
	//m_pRemoteInspDoc = NULL;
	m_nCalibLightGreyValue = 0;
	bIsFirstLightCalibration = FALSE;
	m_CalibTolerence = 1;
	m_bLiveImageSingle = FALSE;
	m_bInspectionDebug = FALSE;
	m_bExitLogEnry = FALSE;
	m_nNoofFov = 0;

	//InterlockedExchange(&nCameraGrabNo, -1);
//	nTotalGrabNo = 0;
}

CAppDoc::~CAppDoc()
{

	//if(m_hTrackImageUpdate) {
	//	//SetEvent(m_EventImageUpdate);
	//	if(WaitForSingleObject(m_hTrackImageUpdate, 500)!=WAIT_OBJECT_0)
	//		TerminateThread(m_hTrackImageUpdate, 0);
	//	m_hTrackImageUpdate = NULL;
	//	CloseHandle(m_EventImageUpdate);
	//}

	if (m_pOpticsDialog) {
		if (IsWindow(m_pOpticsDialog->m_hWnd))
			DestroyWindow(m_pOpticsDialog->m_hWnd);
		delete m_pOpticsDialog;
	}
	if (m_pColorConvertor)
		delete m_pColorConvertor;
}

BOOL CAppDoc::OnNewDocument()
{
	if (!CEvImageDoc::OnNewDocument())
		return FALSE;

	m_pApp = (CApplication*)AfxGetApp();
	if (m_pApp == NULL) {
		ASSERT(!"Failed to get Application reference");
		return FALSE;
	}

	return TRUE;
}




// CAppDoc serialization

void CAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAppDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAppDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAppDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAppDoc diagnostics

#ifdef _DEBUG
void CAppDoc::AssertValid() const
{
	CEvImageDoc::AssertValid();
}

void CAppDoc::Dump(CDumpContext& dc) const
{
	CEvImageDoc::Dump(dc);
}
#endif //_DEBUG


// CAppDoc commands


BOOL CAppDoc::SetParameters(CTrackHandler* pTrack, int nDoc)
{
	m_pTrack = pTrack;
	m_nDocIdx = nDoc;

	m_EventInspEnd = CreateEventA(NULL, FALSE, FALSE, NULL);

	return TRUE;
}

void CAppDoc::InitializeDocView()
{
	CSize szImgSize;
	int nTrigType;
	CRect RectROI;
	CString str;
	str.Format("DebugFlag C%d", m_pTrack->m_nTrack + 1);
	//m_lDebugFlag = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", str, (int)m_lDebugFlag);
	m_lDebugFlag = DEBUG_DRAW | DEBUG_PRINT | DEBUG_TIME;
	str.Format("Threshold %d_%d", m_pTrack->m_nTrack + 1, m_nDocIdx + 1);
	m_nThreshold = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", str, 128);

	//m_pApp->m_nUseCameraResolution = GetRegistryEntry(READ_REGPATH + "Preferences", "UseCameraResolution", m_pApp->m_nUseCameraResolution);

	//m_pTrack->LoadCameraParm();

	if (m_pTrack->m_FGResource.pCamModule) {
		GetResolution(&m_pTrack->m_FGResource, &szImgSize);

		GetCameraAoi(&m_pTrack->m_FGResource, &RectROI);
		OutputDebugLog("InitializeDocView(): GetCameraAoi Track %d (%d, %d)", m_pTrack->m_nTrack, RectROI.Width(), RectROI.Height());

		/*szImgSize = RectROI.Size();*/
		GetTrigMode(&m_pTrack->m_FGResource, (int*)&nTrigType);
		if (nTrigType == TRIG_NONE) {
			m_pTrack->m_bCamTrigAvail = FALSE;
			m_pTrack->m_bCamExtTrigEnable = FALSE;
		}
		else if (nTrigType == TRIG_SOFT) {
			OutputDebugLog("Camera Track %d Trigger SOFT", m_pTrack->m_nTrack);

			m_pTrack->m_bCamTrigAvail = TRUE;
			m_pTrack->m_bCamExtTrigEnable = FALSE;
		}
		else if (nTrigType == TRIG_HARD) {
			OutputDebugLog("Camera Track %d Trigger HARD", m_pTrack->m_nTrack);

			m_pTrack->m_bCamTrigAvail = TRUE;
			m_pTrack->m_bCamExtTrigEnable = FALSE;
		}
		else {
			AfxMessageBox("Error in setting TrigMode");
		}
		SetTrigMode(&m_pTrack->m_FGResource, TRIG_SOFT, 0);
	}
	else
		szImgSize = m_pTrack->m_rectAoi.Size();

	OutputDebugLog("Size x = %d  y = %d", szImgSize.cx, szImgSize.cy);

	m_bIsColor = 0;




	ResizeDoc(szImgSize);
	//SetDocSize(szImgSize);
	InitViewBuffer(&m_Buffer);///
	///LoadConfiguration();

	m_bufPVIBackup.Allocate(m_Buffer.GetImgSize());
	m_bufPVIBackup.ClrImg();

	if (m_bIsColor)
		m_pColorConvertor = new CConvertRawY8BGGR();

	m_bAutoResizeImage = m_pApp->m_bAutoResizeImage;
	m_bAutoResizeImageToMidRes = m_pApp->m_bAutoResizeImageToMidRes;

	CRect rect = CRect(CPoint(77, 85), CSize(35, 35));
	m_InspTickPos1[0] = rect.TopLeft();
	m_InspTickPos1[0].x += rect.Width() / 2;
	m_InspTickPos1[0].y -= 35;
	m_InspTickPos2[0] = m_InspTickPos1[0];
	m_InspTickPos2[0].y += rect.Height() + 70;

	m_InspTickPos1[1] = rect.TopLeft();
	m_InspTickPos1[1].Offset(-20, -20);
	m_InspTickPos2[1] = rect.BottomRight();
	m_InspTickPos2[1].Offset(20, 20);
	int temp = m_InspTickPos2[1].x;
	m_InspTickPos2[1].x = m_InspTickPos1[1].x;
	m_InspTickPos1[1].x = temp;

	m_InspTickPos1[2] = rect.TopLeft();
	m_InspTickPos1[2].x -= 35;
	m_InspTickPos1[2].y += rect.Height() / 2;
	m_InspTickPos2[2] = m_InspTickPos1[2];
	m_InspTickPos2[2].x += rect.Width() + 70;

	m_InspTickPos1[3] = rect.TopLeft();
	m_InspTickPos1[3].Offset(-20, -20);
	m_InspTickPos2[3] = rect.BottomRight();
	m_InspTickPos2[3].Offset(20, 20);

	m_bLightCalibration = FALSE;
	//m_EventImageUpdate = CreateEventA(NULL, FALSE, FALSE, NULL);
	m_EventLogInspectionData = CreateEventA(NULL, TRUE, FALSE, NULL);
	//m_hTrackImageUpdate = AfxBeginThread(TrackImageUpdate, this,THREAD_PRIORITY_LOWEST);
	AfxBeginThread(LogInspectionData, this, THREAD_PRIORITY_LOWEST);
	m_EventKillLiveSingle = CreateEventA(NULL, FALSE, FALSE, NULL);

	m_nNoofFov = MAX_FOV;

	if (m_pTrack->m_nTrack == STATION_TOP)
		m_nNoofFov = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	else if (m_pTrack->m_nTrack == STATION_THR)
		m_nNoofFov = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

	if (m_nNoofFov > MAX_FOV)
		m_nNoofFov = MAX_FOV;

	for (int nFOV = 0; nFOV < MAX_FOV; nFOV++) {
		//m_pTrack->m_nXAxisPosition[nFOV] = 0;
		//m_pTrack->m_nYAxisPosition[nFOV] = 0;
		m_nZAxisPosition[nFOV] = 0;
		//if (nFOV < m_pApp->m_nNoOfFov) {
		m_TeachBuffer[nFOV].Allocate(m_Buffer.GetImgSize());
		m_TeachBuffer[nFOV].ClrImg();

		//m_InspectionHandler[nFOV].Initialize(m_pTrack->m_nTrack);
		//CInspection* pInsp = &m_InspectionHandler[nFOV].m_Inspection;
		//pInsp->Initialize(m_pTrack->m_nTrack, m_pApp->m_bInspectionParallelism);
		//int nTrack = -1;
		//m_pApp->m_ThreadManager.m_nInspThreadIndex = nTrack;
		//for (int i = 0; i<THREAD_COUNT; i++)
		//	pInsp->m_ThreadParms[i].m_hThread = m_pApp->m_ThreadManager.CreateThread(InspectionThread, pInsp, INSPECTION_THREAD, nTrack);

	//}
	}

	m_InspectionHandler[m_pTrack->m_nFOVIndex].Initialize(m_pTrack->m_nTrack);
	int nTrack = -1;
	CInspection* pInsp = &m_InspectionHandler[m_pTrack->m_nFOVIndex].m_Inspection;
	pInsp->Initialize(m_pTrack->m_nTrack, m_pApp->m_bInspectionParallelism);
	m_pApp->m_ThreadManager.m_nInspThreadIndex = nTrack;

	if (m_pTrack->m_nTrack == STATION_TOP)
		grabbedImageId.resize(STATION_TOP_THREAD_COUNT);
	else if (m_pTrack->m_nTrack == STATION_THR)
		grabbedImageId.resize(STATION_THR_THREAD_COUNT);

	for (int i = 0; i < THREAD_COUNT; i++)
		pInsp->m_ThreadParms[i].m_hThread = m_pApp->m_ThreadManager.CreateThread(InspectionThread, pInsp, INSPECTION_THREAD, nTrack);
}

int CAppDoc::ResizeDoc(CSize szImage)
{
	if (!m_pbufImage || !m_pbufImage->IsAllocated() || (szImage.cx != m_pbufImage->GetImgSize().cx) || (szImage.cy != m_pbufImage->GetImgSize().cy)) {
		m_Buffer.Allocate(szImage, true, m_bIsColor);
		m_Buffer.ClrImg();
		InitViewBuffer(&m_Buffer);


		if (m_bIsColor) {
			m_BufSource.Allocate(szImage, false, TRUE);
			m_BufSource.ClrImg();
		}
		if (m_pApp->m_bAllocFailedImageBuffer) {
			for (int i = 0; i < 25; i++) {
				m_FailStack[i].bufFailImage.Allocate(m_Buffer.GetImgSize(), false, m_bIsColor);
				m_FailStack[i].bufFailImage.ClrImg();
			}
			OutputDebugLog("FailedImageBuffer Allocated");
		}

		SetDocSize(szImage);
		ZoomView(ZOOMFIT);
	}

	return 0;
}

void CAppDoc::CameraLive()
{
	if (m_pTrack->m_bLiveImage) {
		SetEvent(m_pTrack->m_EventKillLive);
		Sleep(100);
	}
	else {
		m_pTrack->m_bLiveImage = TRUE;
		AfxBeginThread(TrackLiveImage, this, THREAD_PRIORITY_LOWEST);
	}
}

void CAppDoc::CamLiveSingle()
{
	if (m_bLiveImageSingle) {
		SetEvent(m_EventKillLiveSingle);
		Sleep(100);
	}
	else {
		m_bLiveImageSingle = TRUE;
		AfxBeginThread(TrackLiveImageSingle, this, THREAD_PRIORITY_LOWEST);
	}
}

void CAppDoc::CameraLiveCalibration()
{
	if (m_pTrack->m_bLiveCalibration) {
		SetEvent(m_pTrack->m_EventKillLiveCalibration);
		Sleep(100);
	}
	else {
		m_pTrack->m_bLiveCalibration = TRUE;
		AfxBeginThread(TrackLightCalibrationLiveImage, this, THREAD_PRIORITY_LOWEST);
	}
}

int CAppDoc::CopyGrabbedBuffer(CImageBuffer* pGrabbedBuffer)
{
	//CPerformanceTimer timer;
	int nXSize = __min(m_pbufImage->GetImgSize().cx, pGrabbedBuffer->GetImgSize().cx);
	int nYSize = __min(m_pbufImage->GetImgSize().cy, pGrabbedBuffer->GetImgSize().cy);

	if (m_bIsColor) {
		CPerformanceTimer timer;
		timer.Reset();
		EnterCriticalSection(&m_csDisplayBuffer);
		CImageBuffer* pBuf = m_bBinaryMode ? &m_BufSource : &m_Buffer;
		m_pColorConvertor->ConvertRawY8BGGREx(
			nXSize, nYSize,
			pGrabbedBuffer->GetImgPtr(),
			pBuf->GetImgPtr());
		if (m_bBinaryMode) {
			CRect rcFOV = CRect(CPoint(0, 0), m_Buffer.GetImgSize());
			ImgCopy(&m_BufSource, &rcFOV, &m_Buffer, &rcFOV);
		}
		LeaveCriticalSection(&m_csDisplayBuffer);
		OutputDebugLogTo(1, TRUE, "Color Conversion Time: %5.1fms", timer.GetElapsedMSec());
	}
	else {
		CRect rect = CRect(0, 0, nXSize, nYSize);
		ImgCopy(pGrabbedBuffer, &rect, m_pbufImage, &rect);
	}

	//OutputDebugLogTo(2+m_pTrack->m_nTrack/5, TRUE, "CopyGrabbedBuffer time: %5.1fms", timer.GetElapsedMSec());
//	OutputDebugLogTo(3+m_pTrack->m_nTrack, TRUE, "[Image %d] CopyGrabbedBuffer", m_nDocIdx+1);

	return 0;
}

int CAppDoc::CameraGrab(BOOL bDisplay, BOOL bSilent)
{
	int nError;

	if (bDisplay)
		m_pTrack->m_nOpticsRecalcMode = 1;

	if (m_pApp->m_bLightCtrlDlgOpen || m_bDotCalibration)
	{
		nError = Grab(m_pTrack->m_pDoc[m_nDocIdx], bDisplay);
		if (nError) {
			m_pTrack->m_nOpticsRecalcMode = 0;
			if (!bSilent && !m_pApp->m_bOnline)
				OutputDebugLog("********* Image %d Snap failed", m_nDocIdx + 1);
			return IMAGE_SNAP_FAIL;
		}
		else if (!m_pApp->m_bOnline)
			SetImageTitle(NULL, FALSE);
		m_pApp->m_bCalib = FALSE;
	}
	else
	{
		CPerformanceTimer timer;
		for (int k = 0; k </*m_pApp->m_nTotalViews*/m_pTrack->m_nTotalDocs; k++) {
			timer.Reset();
			m_pTrack->RefreshLight(k);
			OutputDebugLog("Light Refresh[%d] = %5.1fms", k, timer.GetElapsedMSec());
			timer.Reset();
			nError = Grab(m_pTrack->m_pDoc[k], bDisplay);
			OutputDebugLog("Grab Refresh[%d] = %5.1fms", k, timer.GetElapsedMSec());
			if (nError) {
				m_pTrack->m_nOpticsRecalcMode = 0;
				if (!bSilent && !m_pApp->m_bOnline)
					OutputDebugLog("********* Image %d Snap failed", k);
				return IMAGE_SNAP_FAIL;
			}

			else if (!m_pApp->m_bOnline)
				SetImageTitle(NULL, FALSE);
		}
	}
	return 0;
}

int CAppDoc::CameraGrabUsingExposure(BOOL bDisplay, BOOL bSilent, BOOL bMoveSequence)
{
	int nError = 0;
	//CPerformanceTimer timer;

	int nDocCount = m_pApp->m_nTotalViewDocs[m_pTrack->m_nTrack];
	for (int nDoc = 0; nDoc < nDocCount; nDoc++) {
		if (bDisplay)
			m_pTrack->m_nOpticsRecalcMode = 1;

		//timer.Reset();
		if ((!m_bSetupGrid) || theApp.m_bOnline) {
			DrwDel(&m_pTrack->m_pDoc[nDoc]->m_ImageViewInfo);
			DrwUpdate(&m_pTrack->m_pDoc[nDoc]->m_ImageViewInfo);
		}

		OutputDebugLogTo(3 + m_pTrack->m_nTrack, TRUE, "Light Refresh [T%d]", m_pTrack->m_nTrack + 1);
		nError |= m_pTrack->EnqueueAsyncGrab();
		if (!nError)
			m_pTrack->UpdateGrabImage(nDoc);
		else
			m_pTrack->m_nOpticsRecalcMode = 0;
	}

	if (m_pApp->m_bSnap)
		m_pApp->m_bSnap = FALSE;

	if (nError) {
		if (!m_pApp->m_bOnline)
			OutputDebugLog("********* Image Snap failed");
		return IMAGE_SNAP_FAIL;
	}
	else if (!m_pApp->m_bOnline)
		SetImageTitle(NULL, FALSE);

	return 0;
}

int CAppDoc::CameraGrabSingle(BOOL bDisplay, BOOL bSilent)
{
	if (bDisplay)
		m_pTrack->m_nOpticsRecalcMode = 1;

	if ((!m_pTrack->m_bOpticsRoi && !m_bSetupGrid) || theApp.m_bOnline) {
		DrwDel(&m_ImageViewInfo);
		DrwUpdate(&m_ImageViewInfo);
	}

	int nError = m_pTrack->EnqueueAsyncGrab();
	if (!nError)
		m_pTrack->UpdateGrabImage(m_nDocIdx);
	else
		m_pTrack->m_nOpticsRecalcMode = 0;

	if (m_pApp->m_bSnap)
		m_pApp->m_bSnap = FALSE;

	if (nError) {
		if (!m_pApp->m_bOnline)
			OutputDebugLog("********* Image Snap failed");
		return IMAGE_SNAP_FAIL;
	}
	else if (!m_pApp->m_bOnline)
		SetImageTitle(NULL, FALSE);

	return 0;
}

int CAppDoc::GetSamplingStatus(int *pEnableInspection, int nTotalFov)
{
	if (pEnableInspection == NULL || nTotalFov < 1)
		return -1;

	int nTotalDie;

	if (m_nEnableInspection.size() != nTotalFov)
		return -1;

	for (int i = 0; i < m_nNoofFov; i++) {
		if (m_nEnableInspection[i] < 0 || m_nEnableInspection[i] > 2) {
			pEnableInspection[i] = 0;
		}
		else {
			pEnableInspection[i] = m_nEnableInspection[i];
		}
	}

}

int CAppDoc::SetSamplingStatus(const int *pEnableInspection, int nTotalFov)
{
	if (pEnableInspection == NULL || nTotalFov < 1)
		return -1;

	int nTotalDie;

	if (m_nEnableInspection.size() != m_nNoofFov)
		return -1;

	for (int i = 0; i < m_nNoofFov; i++) {

		if (pEnableInspection[i] < 0 || pEnableInspection[i] > 2) {
			m_nEnableInspection[i] = 0;
		}
		else {
			m_nEnableInspection[i] = pEnableInspection[i];
		}
	}

	return 0;
}

int CAppDoc::LoadSamplingParameters()
{

	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp;

	std::string str;

	int nTotalDie;

	nTotalDie = m_pApp->motionParameters.getTotalDieCount();
	strFile.Format("%s\\Sampling Parameters.info", m_strConfigDir);

	iniFile.SetPath(strFile);

	strKey.Format("Sampling Parameters");


	m_nEnableInspection.resize(m_nNoofFov);
	std::fill(m_nEnableInspection.begin(), m_nEnableInspection.end(), 2);

	if (iniFile.ReadFile()) {
		int nStatus = 0;
		for (int nFov = 0; nFov < m_nNoofFov; nFov++) {
			strTemp.Format("FOV %d", nFov);
			nStatus = iniFile.GetInteger(strKey, strTemp, m_nEnableInspection[nFov]);

			if (nStatus < 0 || nStatus > 2)
				m_nEnableInspection[nFov] = 2;
			else
				m_nEnableInspection[nFov] = nStatus;
		}
	}

	return 0;
}

void CAppDoc::SaveSamplingParameters()
{

	CIniFile iniFile;
	CFileException e;

	CString strFile, strKey, strTemp;

	std::string str;

	int nTotalDie;

	nTotalDie = m_pApp->motionParameters.getTotalDieCount();
	strFile.Format("%s\\Sampling Parameters.info", m_strConfigDir);

	iniFile.SetPath(strFile);

	strKey.Format("Sampling Parameters");

	if (m_nEnableInspection.size() != m_nNoofFov) {
		m_nEnableInspection.resize(m_nNoofFov);
		std::fill(m_nEnableInspection.begin(), m_nEnableInspection.end(), 2);
	}

	for (int nFov = 0; nFov < m_nNoofFov; nFov++) {
		strTemp.Format("FOV %d", nFov);
		iniFile.SetInteger(strKey, strTemp, m_nEnableInspection[nFov]);
	}

	iniFile.WriteFile();
}

int CAppDoc::LoadConfiguration()
{
	m_strConfigDir.Format("%s\\Doc%d", m_pTrack->m_strConfigDir, m_nDocIdx + 1);
	//m_strConfigDir = m_pTrack->m_strConfigDir;

	//if(m_pTrack->m_nTrack==0 && m_nDocIdx==0) {
	_mkdir(m_strConfigDir);
	//OutputDebugLogTo(8, TRUE, "Doc Config Folder: %s", m_strConfigDir);
	CCalibrationData* pCalibData;

	int nFOVIndexBk = m_pTrack->m_nFOVIndex;
	for (int nFOV = 0; nFOV < MAX_FOV; nFOV++) {
		m_InspectionHandler[nFOV].CleanUp();
		m_InspectionHandler[nFOV].CleanUpTeach();
	}

	if (m_pTrack->m_nTrack == STATION_TOP)
		m_nNoofFov = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	else if (m_pTrack->m_nTrack == STATION_THR)
		m_nNoofFov = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

	if (m_nNoofFov > MAX_FOV) {
		m_nNoofFov = MAX_FOV;
	}
	//Get total FOV for current station

	m_pApp->m_LightHandler.LoadLightSettings(m_nDocIdx, m_pTrack->m_nTrack);
	pCalibData = &m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;

	CString strCalibFolder;// = m_pApp->m_strCalibDir;
	strCalibFolder.Format("%s\\Calibration", m_pApp->m_strMainCfgDir);
	for (int nFOV = 0; nFOV < m_nNoofFov; nFOV++) {
		m_pTrack->m_nFOVIndex = nFOV;

		if (!m_nDocIdx) {
			m_InspectionHandler[nFOV].LoadCalibrationParameters(strCalibFolder, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFOV);
			m_InspectionHandler[nFOV].LoadDotCalibrationParameters(strCalibFolder, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFOV);
		}

		/*pCalibData = &m_pApp->m_pTracks[0].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;*/

		m_InspectionHandler[nFOV].CreateParameterList(m_pTrack->m_nTrack, m_nDocIdx, nFOV);
		m_InspectionHandler[nFOV].CreateTeachParmList(m_pTrack->m_nTrack/*m_pTrack->m_nTrack, m_nDocIdx, nFOV*/);
		//// clear teach param variables before reading values from recipe file ////
		m_InspectionHandler[nFOV].ClearTeachParam(NULL);
		m_InspectionHandler[nFOV].LoadRecipe(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
			&m_Buffer,
			m_strConfigDir,
			m_pApp->m_strConfigFile,
			m_pTrack->m_nTrack,
			m_pTrack->m_nTotalDocs,
			nFOV, m_nDocIdx);



		CAppDoc* pAppDoc = (CAppDoc*)this;
		m_pApp->m_pTracks[pAppDoc->m_pTrack->m_nTrack].m_pActiveDocTeach = pAppDoc;

		((CMainFrame*)m_pApp->m_pMainWnd)->InitializeProperties(this, nFOV);
		((CMainFrame*)m_pApp->m_pMainWnd)->InitializeTeachProperties(this, nFOV);

		int nError, nTeachStep = 2;//// 1-Device Loc & Dattam; 2-Region & Mask;
		CString strImgFullPath;
		strImgFullPath.Format("%s\\TeachTemplate_Pos%d.bmp", m_strConfigDir, nFOV + 1);

		if (!m_TeachBuffer[nFOV].IsAllocated()) {
			m_TeachBuffer[nFOV].Allocate(m_Buffer.GetImgSize());
			m_TeachBuffer[nFOV].ClrImg();
		}

		nError = LoadGrayScaleBMPFile(strImgFullPath, m_TeachBuffer[nFOV]);
		if (!nError) {
			for (int i = 0; i < nTeachStep; i++) {
				m_InspectionHandler[nFOV].AutoTeach(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
					&m_TeachBuffer[nFOV],
					m_strConfigDir, m_pApp->m_strConfigFile,
					!i ? FALSE : TRUE,
					m_pTrack->m_nTrack,
					m_nDocIdx,
					nFOV + 1,
					/*m_pApp->m_nNoOfFov*/m_nNoofFov,
					pCalibData);

			}
		}
		else {
			OutputDebugLogTo(1, TRUE, "AutoTeach Load Image Error [Teach Image Not Avaiable]");
			OutputDebugLogTo(9, TRUE, "CAppDoc::LoadConfiguration AutoTeach Image Error [Track%d FOV%d Doc%d]", m_pTrack->m_nTrack + 1, nFOV + 1, m_nDocIdx + 1);
		}

		if (m_pApp->m_pProgressDlg != NULL) {
			m_pApp->m_pProgressDlg->SetStatus("Loading Configuration....", m_pApp->m_pProgressDlg->GetCurrentLoad() + 9);
		}
	}

	LoadSamplingParameters();

	m_pTrack->m_nFOVIndex = nFOVIndexBk;

	m_lDebugFlag = m_lDebugFlag | DEBUG_DRAW | DEBUG_PRINT | DEBUG_TIME;

	DrawView(FALSE);
	DrawView();

	return 0;

}

int CAppDoc::LoadConfiguration(int nFov)
{
	m_strConfigDir.Format("%s\\Doc%d", m_pTrack->m_strConfigDir, m_nDocIdx + 1);
	//m_strConfigDir = m_pTrack->m_strConfigDir;

	//if(m_pTrack->m_nTrack==0 && m_nDocIdx==0) {
	_mkdir(m_strConfigDir);
	//OutputDebugLogTo(8, TRUE, "Doc Config Folder: %s", m_strConfigDir);
	CCalibrationData* pCalibData;

	int nFOVIndexBk = m_pTrack->m_nFOVIndex;
	m_InspectionHandler[nFov].CleanUp();
	m_InspectionHandler[nFov].CleanUpTeach();

	m_pApp->m_LightHandler.LoadLightSettings(m_nDocIdx, m_pTrack->m_nTrack);

	if (!m_nDocIdx) {
		CString strCalibFolder;// = m_pApp->m_strCalibDir;
		strCalibFolder.Format("%s\\Calibration", m_pApp->m_strMainCfgDir);
		m_InspectionHandler[nFov].LoadCalibrationParameters(strCalibFolder, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFov);
		m_InspectionHandler[nFov].LoadDotCalibrationParameters(strCalibFolder, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFov);
	}

	/*pCalibData = &m_pApp->m_pTracks[0].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;*/
	pCalibData = &m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;

	m_InspectionHandler[nFov].CreateParameterList(m_pTrack->m_nTrack, m_nDocIdx, nFov);
	m_InspectionHandler[nFov].CreateTeachParmList(m_pTrack->m_nTrack/*m_pTrack->m_nTrack, m_nDocIdx, nFOV*/);
	//// clear teach param variables before reading values from recipe file ////
	m_InspectionHandler[nFov].ClearTeachParam(NULL);
	m_InspectionHandler[nFov].LoadRecipe(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
		&m_Buffer,
		m_strConfigDir,
		m_pApp->m_strConfigFile,
		m_pTrack->m_nTrack,
		m_pTrack->m_nTotalDocs,
		nFov, m_nDocIdx);

	CAppDoc* pAppDoc = (CAppDoc*)this;
	m_pApp->m_pTracks[pAppDoc->m_pTrack->m_nTrack].m_pActiveDocTeach = pAppDoc;

	((CMainFrame*)m_pApp->m_pMainWnd)->InitializeProperties(this, nFov);
	((CMainFrame*)m_pApp->m_pMainWnd)->InitializeTeachProperties(this, nFov);

	int nError, nTeachStep = 2;//// 1-Device Loc & Dattam; 2-Region & Mask;
	CString strImgFullPath;
	strImgFullPath.Format("%s\\TeachTemplate_Pos%d.bmp", m_strConfigDir, nFov + 1);

	if (!m_TeachBuffer[nFov].IsAllocated()) {
		m_TeachBuffer[nFov].Allocate(m_Buffer.GetImgSize());
		m_TeachBuffer[nFov].ClrImg();
	}

	//Get total FOV for current station
	int nCurrentFOV = m_pApp->m_nNoOfFov;
	if (m_pTrack->m_nTrack == STATION_TOP) {
		nCurrentFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	}
	else if (m_pTrack->m_nTrack == STATION_THR) {
		nCurrentFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();
	}

	nError = LoadGrayScaleBMPFile(strImgFullPath, m_TeachBuffer[nFov]);
	if (!nError) {
		for (int i = 0; i < nTeachStep; i++) {
			m_InspectionHandler[nFov].AutoTeach(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
				&m_TeachBuffer[nFov],
				m_strConfigDir, m_pApp->m_strConfigFile,
				!i ? FALSE : TRUE,
				m_pTrack->m_nTrack,
				m_nDocIdx,
				nFov + 1,
				/*m_pApp->m_nNoOfFov*/nCurrentFOV,
				pCalibData);
		}
	}
	else {
		OutputDebugLogTo(1, TRUE, "AutoTeach Load Image Error [Teach Image Not Avaiable]");
		OutputDebugLogTo(9, TRUE, "CAppDoc::LoadConfiguration AutoTeach Image Error [Track%d FOV%d Doc%d]", m_pTrack->m_nTrack + 1, nFov + 1, m_nDocIdx + 1);
	}

	LoadSamplingParameters();
	m_lDebugFlag = m_lDebugFlag | DEBUG_DRAW | DEBUG_PRINT | DEBUG_TIME;

	DrawView(FALSE);
	DrawView();

	return 0;

}

int CAppDoc::Teach()
{
	CTrainingData* pData;
	CTrainingData* pDataFirst;
	CInspectionHandler* pInspHandler;
	CPVIInspectionSetup* pDataRegion;
	CCalibrationData* pCalibData;

	int nError, nTrack, nDocCount, nDocFrame, nTeachStep;
	CString strLoadImgMsg, strTeachImgName, strStatusBarMsg, strInfoMsg;

	LOGPEN TextColorRed, TextColorGreen, TextColor, TextColorYellow;
	TextColorRed.lopnColor = RGB(230, 50, 50);
	TextColorRed.lopnStyle = PS_SOLID;
	TextColorRed.lopnWidth.x = 1;

	TextColorYellow.lopnColor = RGB(230, 230, 0);
	TextColorYellow.lopnStyle = PS_SOLID;
	TextColorYellow.lopnWidth.x = 1;

	TextColorGreen.lopnColor = RGB(50, 230, 50);
	TextColorGreen.lopnStyle = PS_SOLID;
	TextColorGreen.lopnWidth.x = 1;

	nTeachStep = 2;		//// Device Loc; Dattam/Pattern Search; Region Search;
	nError = 0;
	nTrack = m_pTrack->m_nTrack;
	nDocCount = m_pApp->m_nTotalViewDocs[nTrack];

	m_bShowOverlay = TRUE;
	m_pTrack->m_bOpticsRoi = FALSE;

	if (m_pApp->m_bPixelRuler)
	{
		m_pApp->m_pPixelRulerDlg->ShowWindow(SW_HIDE);
		m_pApp->m_bPixelRuler = FALSE;
	}

	m_pApp->AddLogEvent("Teach Process Started");
	UpdateStatusBar(&CString(""));

	if (m_bBinaryMode)
		SwitchImg(&m_ImageViewInfo, FALSE);

	m_pApp->m_pWndPVIDefectParameters->m_nTrackId = nTrack;
	int nFOVIdxBk = m_pTrack->m_nFOVIndex;
	//m_pApp->m_pWndPVIDefectParameters->ShowWindow(TRUE);

	BOOL bLoadTeachImage = FALSE;
	strLoadImgMsg.Format("Do You Like To Load Teach Images?");
	if (AfxMessageBox(strLoadImgMsg, MB_YESNO) == IDYES)
		bLoadTeachImage = TRUE;

	//Get total FOV for current station
	int nCurrentFOV = m_pApp->m_nNoOfFov;
	if (m_pTrack->m_nTrack == STATION_TOP) {
		nCurrentFOV = m_pApp->motionParameters.nFOVPerDieinXTOP * m_pApp->motionParameters.nFOVPerDieinYTOP;
	}
	else if (m_pTrack->m_nTrack == STATION_THR) {
		nCurrentFOV = m_pApp->motionParameters.nFOVPerDieinXTHR * m_pApp->motionParameters.nFOVPerDieinYTHR;
	}
	int nNoOfFov = nCurrentFOV;

	if (nCurrentFOV > MAX_FOV) {
		nCurrentFOV = MAX_FOV;
	}

	int nStartFOV = 0;
	int nCurrentDoc = 0;
	if (m_bTeachSingleImage) {
		nStartFOV = m_pApp->m_pTracks[m_pTrack->m_nTrack].m_nFOVIndex;
		nCurrentFOV = nStartFOV + 1;
		nCurrentDoc = m_pTrack->m_pActiveDocTeach->m_nDocIdx;
	}
	m_nFovCompletedTeach.clear();

	for (int nFOV = nStartFOV; nFOV < nCurrentFOV/*m_pApp->m_nNoOfFov*/; nFOV++) {
		m_pTrack->m_nFOVIndex = nFOV;

		if (!m_bTeachSingleImage) {
			strLoadImgMsg.Format("Do You Like To Teach For Position%d ?", nFOV + 1);
			if (AfxMessageBox(strLoadImgMsg, MB_YESNO) == IDNO)
				continue;
		}

		if (bLoadTeachImage) {
			for (int nDoc1 = 0; nDoc1 < nDocCount; nDoc1++) {
				CString strTemp;
				strTemp.Format("%s\\TeachTemplate_Pos%d.bmp", m_pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_strConfigDir, nFOV + 1);
				nError = LoadGrayScaleBMPFile(strTemp, m_pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_Buffer);
				if (nError) {
					AfxMessageBox("Teach Image Not Available... Please Load Static Image!", IDOK);
					//m_pApp->m_pTracks[nTrack].m_pDoc[0]->OnToolsStaticimageLoadFromFile();
					CString strImagePath = LoadImage();
					if (strImagePath != "") {
						LoadImagesToDoc(strImagePath);
						nDoc1 = nDocCount;
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						DrawView();
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						return -1;
					}
				}
			}
		}
		else {
			//m_pApp->m_pTracks[nTrack].m_pDoc[0]->OnToolsStaticimageLoadFromFile();
			CString strImagePath;// = LoadImage();
			//CSelectFolder dlg;
			//dlg.SetPath(m_pApp->m_strLoadMultiImagesFolderPath);
			//dlg.m_bFindImage = TRUE;

			//if (dlg.DoModal() == IDOK)
			//{
			//	CString strTemp = dlg.GetPath();
			//	int nLength = strTemp.GetLength();
			//	strImagePath = strTemp.Left(nLength - 1);
			//}
			//else
			//	strImagePath.Empty();

			// New Dialog Select File - Mason
			CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap Files (*.bmp)|*.bmp|JPEG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||");
			dlg.m_ofn.lpstrInitialDir = m_pApp->m_strLoadMultiImagesFolderPath;

			if (dlg.DoModal() == IDOK) {
				strImagePath = dlg.GetPathName();
			}
			else
				strImagePath.Empty();

			if (strImagePath != "")
				LoadImagesToDoc(strImagePath);
			else {
				m_pApp->AddLogEvent("Teach Process Aborted");
				UpdateStatusBar(&CString("Teach Process Aborted"));
				DrawView();
				if (m_pApp->m_pInfoMsgDlg) {
					if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
						m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
				}
				return -1;
			}
		}

		DrawView(FALSE);
		nDocFrame = 0;
		CRect rectTemp;
		int nRectCountTemp;
		int nRectTempIndexLoc[5] = { 0 };

		pDataFirst = &m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[nFOV].m_Inspection.m_TrainingData;
		/*pCalibData = &m_pApp->m_pTracks[0].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;*/
		pCalibData = &m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;

		m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[nFOV].ClearTeachParam(pDataFirst);
		for (int nDoc = 0; nDoc < nDocCount; nDoc++) {
			strInfoMsg.Format("%s - Position%d [%s]", !nTrack ? "Top Camera" : "THR Camera",
				nFOV + 1,
				!nTrack ? m_pApp->m_strTopCameraName.GetAt(nDoc) : m_pApp->m_strTHRCameraName.GetAt(nDoc));
			if (m_pApp->m_pInfoMsgDlg) {
				if (!m_pApp->m_pInfoMsgDlg->IsWindowVisible())
					m_pApp->m_pInfoMsgDlg->ShowWindow(TRUE);
			}

			CString* strTemp = new CString;
			strTemp->Format("%d%d", nDoc, nFOV);
			m_pApp->m_pMainWnd->SendMessageA(WM_SET_CUR_POS_PARAM_DISP, (WPARAM)nTrack, (LPARAM)strTemp);
			delete strTemp;

			pInspHandler = &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFOV];
			pData = &pInspHandler->m_Inspection.m_TrainingData;
			//pDataRegion = pInspHandler->m_Inspection.m_pPviInspectionSetup;
			nDocFrame = !nTrack ? nDoc : m_pApp->m_nTOP + nDoc;

			BOOL bTempMsgLocYes = FALSE;
			BOOL bEnableLoc[2] = { FALSE };

			if (m_bTeachSingleImage && nDoc != nCurrentDoc) {
				HRegion hEmptyRegion;
				GenEmptyRegion(&hEmptyRegion);
				ConcatObj(pDataFirst->hObjectDeviceLocation, hEmptyRegion, &pDataFirst->hObjectDeviceLocation);
				TupleConcat(pDataFirst->hTupleProjectionParam, (HTuple(0).TupleConcat(0)).TupleConcat(0), &pDataFirst->hTupleProjectionParam);
				TupleConcat(pDataFirst->hTupleDatumCenterParam, (HTuple(0).TupleConcat(0)).TupleConcat(0), &pDataFirst->hTupleDatumCenterParam);
				TupleConcat(pDataFirst->hTupleTeachDoc, 1, &pDataFirst->hTupleTeachDoc);
				TupleConcat(pDataFirst->hTupleDatumTeachTuple, pData->bDattamSearchEnable, &pDataFirst->hTupleDatumTeachTuple);
				if (pData->bDattamSearchEnable)
					TupleConcat(pDataFirst->hTupleEdgeTeachTuple, pData->bDattamSearchEnable, &pDataFirst->hTupleEdgeTeachTuple);
				else
					TupleConcat(pDataFirst->hTupleEdgeTeachTuple, pData->bDeviceLocationEnable, &pDataFirst->hTupleEdgeTeachTuple);

				if (pData->bDattamSearchEnable)
					bEnableLoc[1] = TRUE;
				if (pData->bDeviceLocationEnable)
					bEnableLoc[0] = TRUE;


				nError = pInspHandler->Teach(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
					&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
					m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir,
					m_pApp->m_strConfigFile,
					0,
					bEnableLoc,
					FALSE,
					nTrack,
					nDoc,
					nFOV + 1,
					nNoOfFov,
					pDataFirst,
					pCalibData,
					FALSE);

				if (nError) {
					strInfoMsg = strInfoMsg + " - Error: " + m_pApp->GetFailedStr(nError, false);

					AfxMessageBox("Teach Failed At " + strInfoMsg);
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					ZoomSelectedDocView(nDocFrame);
					DrawView();
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					return -1;
				}

				continue;
			}


			if (pData->bDattamSearchEnable)
			{
				//nDocFrame = nTrack;
				ZoomSelectedDocView(nDocFrame, TRUE);

				TupleConcat(pDataFirst->hTupleDatumTeachTuple, 1, &pDataFirst->hTupleDatumTeachTuple);

				bEnableLoc[1] = TRUE;
				rectTemp = CRect(100, 100, 400, 400);
				nRectCountTemp = pData->nDattamSearchNoOfDattams;
				for (int nRect = 0; nRect < nRectCountTemp; nRect++)
				{
					if (pData->m_rectDattamSearch[nRect].Width() != 0 && pData->m_rectDattamSearch[nRect].Height() != 0)
						rectTemp = pData->m_rectDattamSearch[nRect];

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Draw ROI To Locate Datum/Pattern Search Area [Device Location]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexLoc[nDoc]++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						DrawView(FALSE);
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDattamSearch[nRect] = rectTemp;
				}
			}
			else
				TupleConcat(pDataFirst->hTupleDatumTeachTuple, 0, &pDataFirst->hTupleDatumTeachTuple);

			if (TRUE/*pData->bDeviceLocationEnable*/)
			{
				if (!pData->bDattamSearchEnable)
					ZoomSelectedDocView(nDocFrame, TRUE);

				TupleConcat(pDataFirst->hTupleEdgeTeachTuple, 1, &pDataFirst->hTupleEdgeTeachTuple);

				bTempMsgLocYes = TRUE;
				bEnableLoc[0] = TRUE;
				rectTemp = CRect(100, 100, 400, 400);

				if (pData->m_rectDeviceLoc.Width() != 0 && pData->m_rectDeviceLoc.Height() != 0)
					rectTemp = pData->m_rectDeviceLoc;

				DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp);
				DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

				strStatusBarMsg.Format("Draw ROI To Find The Location Of Device [Device Location]");
				m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

				if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
					rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexLoc[nDoc]++);
				else {
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					ZoomSelectedDocView(nDocFrame);
					return -1;
				}
				pData->m_rectDeviceLoc = rectTemp;


				//Get the edge line region search
				CRect horizontalROIRect(rectTemp.left, rectTemp.top, rectTemp.right, rectTemp.bottom - (rectTemp.Height() / 2));
				CRect verticalROIRect(rectTemp.left, rectTemp.top, rectTemp.right - (rectTemp.Width() / 2), rectTemp.bottom);


				if (pData->nDeviceLocationType == EDGE_DETECTION) {
					DrawView(FALSE);
					nRectTempIndexLoc[nDoc] = 0;

					//1.	Horizontal Line
					if (pData->nDeviceHorizontalDirection != NONE) {
						if (pData->m_rectDeviceHorizontalLineLoc.Width() != 0 && pData->m_rectDeviceHorizontalLineLoc.Height() != 0)
							horizontalROIRect = pData->m_rectDeviceHorizontalLineLoc;

						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &horizontalROIRect);
						DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

						strStatusBarMsg.Format("Draw ROI To Find The Horizontal Edge Line Of Device [Device Location]");
						m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

						if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
							horizontalROIRect = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexLoc[nDoc]++);
						else {
							m_pApp->AddLogEvent("Teach Process Aborted");
							UpdateStatusBar(&CString("Teach Process Aborted"));
							if (m_pApp->m_pInfoMsgDlg) {
								if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
									m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
							}
							ZoomSelectedDocView(nDocFrame);
							return -1;
						}
						pData->m_rectDeviceHorizontalLineLoc = horizontalROIRect;
					}


					//2.	Vertical Line
					if (pData->nDeviceVerticalDirection != NONE) {
						if (pData->m_rectDeviceVerticalLineLoc.Width() != 0 && pData->m_rectDeviceVerticalLineLoc.Height() != 0)
							verticalROIRect = pData->m_rectDeviceVerticalLineLoc;

						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &verticalROIRect);
						DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

						strStatusBarMsg.Format("Draw ROI To Find The Vertical Edge Line Of Device [Device Location]");
						m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

						if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
							verticalROIRect = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexLoc[nDoc]++);
						else {
							m_pApp->AddLogEvent("Teach Process Aborted");
							UpdateStatusBar(&CString("Teach Process Aborted"));
							if (m_pApp->m_pInfoMsgDlg) {
								if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
									m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
							}
							ZoomSelectedDocView(nDocFrame);
							return -1;
						}
						pData->m_rectDeviceVerticalLineLoc = verticalROIRect;
					}
				}

			}
			else {
				TupleConcat(pDataFirst->hTupleEdgeTeachTuple, 0, &pDataFirst->hTupleEdgeTeachTuple);

				bTempMsgLocYes = FALSE;
				bEnableLoc[0] = FALSE;// (pData->m_rectDeviceLoc.Width() != 0) ? TRUE : FALSE;
				for (int nTemp = 0; nTemp < pData->nDattamSearchNoOfDattams; nTemp++)
					bEnableLoc[1] |= FALSE;// (pData->m_rectDattamSearch[nTemp].Width() != 0) ? TRUE : FALSE;
			}

			nError = pInspHandler->Teach(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
				&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
				m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir,
				m_pApp->m_strConfigFile,
				0,
				bEnableLoc,
				FALSE,
				nTrack,
				nDoc,
				nFOV + 1,
				/*m_pApp->m_nNoOfFov*/nCurrentFOV,
				pDataFirst,
				pCalibData,
				FALSE);

			if (!nError) {
				strTeachImgName.Format("%s\\TeachTemplate_Pos%d.bmp", m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir, nFOV + 1);
				SaveGrayScaleBMPFile(strTeachImgName, *m_pTrack->m_SharedData.pProcBuffers[nDoc]);
				CRect rectTeach = CRect(CPoint(0, 0), m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer.GetImgSize());
				ImgCopy(m_pTrack->m_SharedData.pProcBuffers[nDoc], &rectTeach, &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_TeachBuffer[nFOV], &rectTeach);
				pInspHandler->SaveTeachParameters(m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir, m_pApp->m_strConfigFile, nFOV);
			}
			else if (nError) {
				AfxMessageBox("Teach Failed");
				m_pApp->AddLogEvent("Teach Process Aborted");
				UpdateStatusBar(&CString("Teach Process Aborted"));
				ZoomSelectedDocView(nDocFrame);
				DrawView();
				if (m_pApp->m_pInfoMsgDlg) {
					if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
						m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
				}
				return -1;
			}

			if ((bEnableLoc[0] || bEnableLoc[1]) && bTempMsgLocYes) {
				strStatusBarMsg.Format("Regions found");
				m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
				if (WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					ZoomSelectedDocView(nDocFrame);
					return -1;
				}
			}
		}
		DrawView();

		//// ---- Region Teach ---- ////
		DrawView(FALSE);
		int nRectTempIndexRgn[5] = { 0 };
		for (int nDoc = 0; nDoc < nDocCount; nDoc++) {
			strInfoMsg.Format("%s - Position%d", !nTrack ? "Top Camera" : "THR Camera",
				nFOV + 1,
				!nTrack ? m_pApp->m_strTopCameraName.GetAt(nDoc) : m_pApp->m_strTHRCameraName.GetAt(nDoc));
			if (m_pApp->m_pInfoMsgDlg) {
				if (!m_pApp->m_pInfoMsgDlg->IsWindowVisible())
					m_pApp->m_pInfoMsgDlg->ShowWindow(TRUE);
			}

			pInspHandler = &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFOV];
			pData = &pInspHandler->m_Inspection.m_TrainingData;
			pDataRegion = pInspHandler->m_Inspection.m_pPviInspectionSetup;
			nDocFrame = !nTrack ? nDoc : m_pApp->m_nTOP + nDoc;// !nTrack ? nDoc : (nTrack == 1 ? m_pApp->m_nTop + nDoc : m_pApp->m_nTop + m_pApp->m_nBottom + nDoc);
			pInspHandler->ClearTeachParam(NULL);

			//For Singler FOV Teaching
			if (m_bTeachSingleImage && nDoc != nCurrentDoc) {
				//pInspHandler->AutoTeach(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
				//	&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
				//	m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir, m_pApp->m_strConfigFile,
				//	TRUE,
				//	m_pTrack->m_nTrack,
				//	nDoc,
				//	nFOV + 1,
				//	m_pApp->m_nNoOfFov,
				//	pCalibData);
				continue;
			}

			BOOL bTempMsgRgnYes[3] = { FALSE };
			BOOL bEnableRgn[4] = { FALSE };
			//if (pDataRegion->bEnable) {
			ZoomSelectedDocView(nDocFrame, TRUE);
			bTempMsgRgnYes[0] = TRUE;
			nRectCountTemp = /*pDataRegion->bEnable ? */pData->nNoOfPviArea/* : 0*/;
			for (int nRect = 0; nRect < nRectCountTemp; nRect++) {
				DrawView();
				//// ---- Area Teach ---- ////
				DrawView(FALSE);
				nRectTempIndexRgn[nDoc] = 0;
				pDataRegion = &pInspHandler->m_Inspection.m_pPviInspectionSetup[nRect];
				if (pDataRegion->bEnable || pDataRegion->bEdgeEnable) {
					bEnableRgn[0] = TRUE;
					if (pDataRegion->bEdgeEnable && pDataRegion->nDeviceAreaType == POLYLINE) {
						int nOffset = 10;
						for (int nPointId = 0; nPointId < pDataRegion->nPolylinePointNumber; nPointId++) {
							rectTemp = CRect(pDataRegion->m_polylinePviArea[nPointId].x - nOffset, pDataRegion->m_polylinePviArea[nPointId].y - nOffset,
								pDataRegion->m_polylinePviArea[nPointId].x + nOffset, pDataRegion->m_polylinePviArea[nPointId].y + nOffset);
							DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, nPointId + POLYLINE);
						}
						DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
						strStatusBarMsg.Format("Draw Polyline ROI To Locate Pvi Area %d", nRect + 1);
						m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

						if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
							for (int nPointId = 0; nPointId < pDataRegion->nPolylinePointNumber; nPointId++) {
								rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexRgn[nDoc]++);
								pDataRegion->m_polylinePviArea[nPointId] = rectTemp.CenterPoint();
							}
						}
						else {
							m_pApp->AddLogEvent("Teach Process Aborted");
							UpdateStatusBar(&CString("Teach Process Aborted"));
							if (m_pApp->m_pInfoMsgDlg) {
								if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
									m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
							}
							ZoomSelectedDocView(nDocFrame);
							return -1;
						}
					}
					//For PVI Device Area Type: Rectangle, Polygon, Eclipse
					else if (pDataRegion->nDeviceAreaType == RECTANGLE) {
						rectTemp = CRect(100, 100, 400, 400);
						if (pDataRegion->m_rectPviArea.Width() != 0 && pDataRegion->m_rectPviArea.Height() != 0)
							rectTemp = pDataRegion->m_rectPviArea;

						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp);
						DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
						strStatusBarMsg.Format("Draw ROI To Locate Pvi Area %d", nRect + 1);
						//UpdateStatusBar(&strStatusBarMsg);
						m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

						if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexRgn[nDoc]++);
						else {
							m_pApp->AddLogEvent("Teach Process Aborted");
							UpdateStatusBar(&CString("Teach Process Aborted"));
							if (m_pApp->m_pInfoMsgDlg) {
								if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
									m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
							}
							ZoomSelectedDocView(nDocFrame);
							return -1;
						}
					}
					else if (pDataRegion->nDeviceAreaType == ECLIPSE) {
						rectTemp = CRect(100, 100, 400, 400);
						if (pDataRegion->m_rectPviArea.Width() != 0 && pDataRegion->m_rectPviArea.Height() != 0)
							rectTemp = pDataRegion->m_rectPviArea;

						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, 1);
						DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
						strStatusBarMsg.Format("Draw ROI To Locate Pvi Area %d", nRect + 1);
						//UpdateStatusBar(&strStatusBarMsg);
						m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

						if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexRgn[nDoc]++);
						else {
							m_pApp->AddLogEvent("Teach Process Aborted");
							UpdateStatusBar(&CString("Teach Process Aborted"));
							if (m_pApp->m_pInfoMsgDlg) {
								if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
									m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
							}
							ZoomSelectedDocView(nDocFrame);
							return -1;
						}
					}
					else if (pDataRegion->nDeviceAreaType == POLYGON) {
						int nOffset = 10;
						for (int nPointId = 0; nPointId < pDataRegion->nPolygonPointNumber; nPointId++) {
							rectTemp = CRect(pDataRegion->m_polygonPviArea[nPointId].x - nOffset, pDataRegion->m_polygonPviArea[nPointId].y - nOffset,
								pDataRegion->m_polygonPviArea[nPointId].x + nOffset, pDataRegion->m_polygonPviArea[nPointId].y + nOffset);
							DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
						}
						DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
						strStatusBarMsg.Format("Draw Polygon ROI To Locate Pvi Area %d", nRect + 1);
						m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

						if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
							for (int nPointId = 0; nPointId < pDataRegion->nPolygonPointNumber; nPointId++) {
								rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexRgn[nDoc]++);
								pDataRegion->m_polygonPviArea[nPointId] = rectTemp.CenterPoint();
							}
						}
						else {
							m_pApp->AddLogEvent("Teach Process Aborted");
							UpdateStatusBar(&CString("Teach Process Aborted"));
							if (m_pApp->m_pInfoMsgDlg) {
								if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
									m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
							}
							ZoomSelectedDocView(nDocFrame);
							return -1;
						}
					}

					if (pDataRegion->nDeviceAreaType != POLYGON)
						pDataRegion->m_rectPviArea = rectTemp;


					for (int nMaskRegions = 0; nMaskRegions < pInspHandler->m_Inspection.m_pPviInspectionSetup[nRect].nDeviceMaskNumber; nMaskRegions++) {
						DrawView();
						//// ---- Area Teach ---- ////
						DrawView(FALSE);
						nRectTempIndexRgn[nDoc] = 0;
						bEnableRgn[1] = TRUE;
						CRect rectTempMask = CRect(100, 100, 400, 400);
						if (pDataRegion->nDeviceMaskArea[nMaskRegions] == RECTANGLE || pDataRegion->nDeviceMaskArea[nMaskRegions] == ECLIPSE) {
							if (pDataRegion->m_rectPviMaskRegions[nMaskRegions].Width() != 0 && pDataRegion->m_rectPviMaskRegions[nMaskRegions].Height() != 0)
								rectTempMask = pDataRegion->m_rectPviMaskRegions[nMaskRegions];

							DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempMask);
							DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

							strStatusBarMsg.Format("Draw ROI To Locate Pvi Mask Region %d For Pvi Area %d", nMaskRegions + 1, nRect + 1);
							//UpdateStatusBar(&strStatusBarMsg);
							m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

							if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
								rectTempMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexRgn[nDoc]++);
							else {
								m_pApp->AddLogEvent("Teach Process Aborted");
								UpdateStatusBar(&CString("Teach Process Aborted"));
								if (m_pApp->m_pInfoMsgDlg) {
									if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
										m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
								}
								ZoomSelectedDocView(nDocFrame);
								return -1;
							}
							pDataRegion->m_rectPviMaskRegions[nMaskRegions] = rectTempMask;
						}
						else if (pDataRegion->nDeviceMaskArea[nMaskRegions] == POLYGON) {
							int nOffset = 10;
							for (int nPointId = 0; nPointId < pDataRegion->nPolygonPviMaskPointNumber[nMaskRegions]; nPointId++) {
								rectTempMask = CRect(pDataRegion->m_polygonPviMask[nMaskRegions][nPointId].x - nOffset, pDataRegion->m_polygonPviMask[nMaskRegions][nPointId].y - nOffset,
									pDataRegion->m_polygonPviMask[nMaskRegions][nPointId].x + nOffset, pDataRegion->m_polygonPviMask[nMaskRegions][nPointId].y + nOffset);
								DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempMask, POLYGON);
							}
							DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
							strStatusBarMsg.Format("Draw Polygon ROI To Locate Pvi Mask Region %d For Pvi Area %d", nMaskRegions + 1, nRect + 1);
							m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

							if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
								for (int nPointId = 0; nPointId < pDataRegion->nPolygonPviMaskPointNumber[nMaskRegions]; nPointId++) {
									rectTempMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexRgn[nDoc]++);
									pDataRegion->m_polygonPviMask[nMaskRegions][nPointId] = rectTempMask.CenterPoint();
								}
							}
							else {
								m_pApp->AddLogEvent("Teach Process Aborted");
								UpdateStatusBar(&CString("Teach Process Aborted"));
								if (m_pApp->m_pInfoMsgDlg) {
									if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
										m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
								}
								ZoomSelectedDocView(nDocFrame);
								return -1;
							}
						}

					}


				}
			}

			nError = pInspHandler->Teach(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
				&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
				m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir,
				m_pApp->m_strConfigFile,
				1,
				bEnableRgn,
				FALSE,
				nTrack,
				nDoc,
				nFOV + 1,
				/*m_pApp->m_nNoOfFov*/nCurrentFOV,
				pDataFirst,
				pCalibData,
				FALSE);

			if (!nError) {
				pInspHandler->SetPVIDefectParameters(m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir, nTrack, nFOV);
				pInspHandler->SaveTeachParameters(m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir, m_pApp->m_strConfigFile, nFOV);

				strStatusBarMsg.Format("Regions found");
				m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
				if (WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					ZoomSelectedDocView(nDocFrame);
					return -1;
				}

			}
			else if (nError) {
				AfxMessageBox("Teach Failed");
				m_pApp->AddLogEvent("Teach Process Aborted");
				UpdateStatusBar(&CString("Teach Process Aborted"));
				ZoomSelectedDocView(nDocFrame);
				DrawView();
				if (m_pApp->m_pInfoMsgDlg) {
					if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
						m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
				}
				return -1;
			}
			else {
				bTempMsgRgnYes[0] = FALSE;
				for (int nTemp1 = 0; nTemp1 < pData->nNoOfPviArea; nTemp1++) {
					bEnableRgn[0] |= FALSE;// (pDataRegion[nTemp1].m_rectPviArea.Width() != 0) ? TRUE : FALSE;
					for (int nTemp2 = 0; nTemp2 < pInspHandler->m_Inspection.m_pPviInspectionSetup[nTemp1].nDeviceMaskNumber; nTemp2++)
						bEnableRgn[1] |= FALSE;// (pDataRegion[nTemp1].m_rectPviMaskRegions[nTemp2].Width() != 0) ? TRUE : FALSE;
				}
			}


			// Encap Teach
			if (pInspHandler->m_Inspection.m_TrainingData.bEnableEncap) {
				//ZoomSelectedDocView(nDocFrame);
				bTempMsgRgnYes[1] = TRUE;
				bEnableRgn[2] = TRUE;
				int nGetRect = 1;
				int nRectTempIndexEncap = 0;
				int nOffset = 10;
				// Blue Encap Tracing Region
				if (pInspHandler->m_Inspection.m_TrainingData.bEnableEncapBlue) {
					// // // // Teach Region for Tracing Encap
					int nRectTempIndexEncapTracing;
					nRectTempIndexEncapTracing = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapTracingPolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap Location [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapTracingPolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncapTracing++);
							pInspHandler->m_Inspection.m_TrainingData.m_EncapTracingPolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					// // Region Encap Top 
					int nRectTempIndexEncapTop;
					nRectTempIndexEncapTop = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapTopPolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_EncapTopPolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapTopPolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_EncapTopPolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapTopPolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap Top Location [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapTopPolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncapTop++);
							pInspHandler->m_Inspection.m_TrainingData.m_EncapTopPolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					// // Region Encap Bottom 
					int nRectTempIndexEncapBottom;
					nRectTempIndexEncapBottom = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapBottomPolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap Bottom Location [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapBottomPolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncapBottom++);
							pInspHandler->m_Inspection.m_TrainingData.m_EncapBottomPolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}

					// // Region Find Left Line Flex for Generating Masking Encap
					nRectTempIndexEncap = 0;
					CRect rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location Find Left Line Flex for Generating Masking Encap [Tracing]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea = rectTempEncapMask;

					// Region Find Right Line Flex for Generating Masking Encap 
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingRightPolygonPointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingRightPolygonPointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingRightPolygonPointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location Find Right Line Flex for Generating Masking Encap [Tracing]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingRightPolygonPointArea = rectTempEncapMask;

					// Region Encap Masking Limit
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location Find Limit Line Flex for Generating Masking Encap [Tracing]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea = rectTempEncapMask;

					// // // // Encap Insufficient
					// // Left Encap Region For Checking Insufficient
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location For Checking Insufficient on Left Encap [Insufficient]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea = rectTempEncapMask;

					// // Right Encap Region For Checking Insufficient
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location For Checking Insufficient on Right Encap [Insufficient]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea = rectTempEncapMask;

					// // Region For Find border of the base
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location For Finding Border of the Base [Insufficient]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea = rectTempEncapMask;
					// // Middle Encap Region For Checking Insufficient
					int nRectTempIndexBlueEncapInsuffMidRegion;
					nRectTempIndexBlueEncapInsuffMidRegion = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nBlueEncapInsuffMidRegionPolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Location For Checking Insufficient on Middle Encap [Insufficient]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nBlueEncapInsuffMidRegionPolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexBlueEncapInsuffMidRegion++);
							pInspHandler->m_Inspection.m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}

					// // // // Encap Excess
					// // Left Encap Region For Checking Excess
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location For Checking Excess on Left Encap [Excess]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea = rectTempEncapMask;

					// // Right Encap Region For Checking Excess
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location For Checking Excess on Right Encap [Excess]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea = rectTempEncapMask;

					// // // // Glue - Mason
					int nRectIndex = 0;
					nOffset = 10;
					CRect rectTempEncap = CRect(100, 100, 400, 400);
					DrawView(FALSE);
					// Glue Mask Polygon
					for (int nPointId = 0; nPointId < ENCAP_GLUE_MASK_POINT; nPointId++) {
						CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonBEncapGlueMask[nPointId];
						rectTempEncap = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Draw Mask To Find Glue Defect Region Of Encap [Glue]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < ENCAP_GLUE_MASK_POINT; nPointId++) {
							rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
							pInspHandler->m_Inspection.m_TrainingData.m_polygonBEncapGlueMask[nPointId] = rectTempEncap.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}

					// // // // Contamination - Mason
					// Encap Flex Edge Mask ROI Rectangle
					DrawView(FALSE);
					nRectIndex = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectBEncapFlexEdgeMaskROI.Width() != 0 && pData->m_rectBEncapFlexEdgeMaskROI.Height() != 0)
						rectTempEncap = pData->m_rectBEncapFlexEdgeMaskROI;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw ROI To Find Edge Between Encap And Flex [Contamination]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectBEncapFlexEdgeMaskROI = rectTempEncap;
					DrawView(FALSE);


				// // // // Wire And Ic
				// Region for Inspect Wire
					int nRectTempIndexEncapWire;
					nRectTempIndexEncapWire = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapWirePolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_EncapWirePolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapWirePolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_EncapWirePolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapWirePolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap For Inspect Wire [Wire And IC]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapWirePolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncapWire++);
							pInspHandler->m_Inspection.m_TrainingData.m_EncapWirePolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}

				// Region for Inspect Wire (Bottom)
					int nRectTempIndexEncapWireBottom;
					nRectTempIndexEncapWireBottom = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapWireBottomPolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap Bottom For Inspect Wire [Wire And IC]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapWireBottomPolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncapWireBottom++);
							pInspHandler->m_Inspection.m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
				// Region for Inspect IC
					int nRectTempIndexEncapIC;
					nRectTempIndexEncapIC = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapICPolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_EncapICPolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapICPolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_EncapICPolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapICPolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap For Inspect IC [Wire And IC]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapICPolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncapIC++);
							pInspHandler->m_Inspection.m_TrainingData.m_EncapICPolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					// Bulge Blue Encap Inspection -Magnus
					nRectIndex = 0;
					rectTempEncap = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pData->m_rectBlueBulgeROI.Width() != 0 && pData->m_rectBlueBulgeROI.Height() != 0)
						rectTempEncap = pData->m_rectBlueBulgeROI;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw ROI for Bulge Inspection Region [Bulge]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectBlueBulgeROI = rectTempEncap;
					DrawView(FALSE); // End Bulge Blue Encap Inspection -Magnus
					
				}
				// Dark Encap Tracing Region
				else if (pInspHandler->m_Inspection.m_TrainingData.bEnableDarkEncap) {
					int nGetRect = 1;
					int nRectTempIndexEncap = 0;
					int nOffset = 10;
					// // Teach Region for Tracing Dark Encap
					// Region Dark Encap Outside 
					rectTemp = CRect(100, 100, 400, 400);

					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkEncapOutsidePolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].y - nOffset,
						pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Outside Encap Location [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkEncapOutsidePolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
							pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					// Region Dark Encap Inside  
					nRectTempIndexEncap = 0;
					DrawView(FALSE);
					rectTemp = CRect(100, 100, 400, 400);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkEncapInsidePolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Dark Encap Inside [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkEncapInsidePolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
							pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					// Region Bottom White Remove Dark Encap
					nRectTempIndexEncap = 0;
					CRect rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectBotWhiteDarkEncap.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectBotWhiteDarkEncap.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectBotWhiteDarkEncap;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location White region remove Dark Encap [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectBotWhiteDarkEncap = rectTempEncapMask;

					// Region Tracing Dark Encap 
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectAllRegionDarkEncap.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectAllRegionDarkEncap.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectAllRegionDarkEncap;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location Tracing Dark Encap [Tracing]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectAllRegionDarkEncap = rectTempEncapMask;

					// // Region Top flex Dark Encap
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectTopFlexDarkEncap.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectTopFlexDarkEncap.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectTopFlexDarkEncap;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location Find Top Dark Encap on Flex [Tracing]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectTopFlexDarkEncap = rectTempEncapMask;
					DrawView(FALSE);
					// Inspection Dark Encap ROI// 
						// Excess - magnus
					// Left Excess Mask Rectangle
					nRectTempIndexEncap = 0;
					CRect rectTempEncap = CRect(100, 100, 400, 400);
					if (pData->m_rectDarkExcessLeftMask.Width() != 0 && pData->m_rectDarkExcessLeftMask.Height() != 0)
						rectTempEncap = pData->m_rectDarkExcessLeftMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Left Mask Using To Check Excess In The Left Of Encap [Excess]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap ++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDarkExcessLeftMask = rectTempEncap;
					DrawView(FALSE);

					// Right Excess Mask Rectangle
					nRectTempIndexEncap = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDarkExcessRightMask.Width() != 0 && pData->m_rectDarkExcessRightMask.Height() != 0)
						rectTempEncap = pData->m_rectDarkExcessRightMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Right Mask Using To Check Excess In The Right Of Encap [Excess]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDarkExcessRightMask = rectTempEncap;
					DrawView(FALSE);

					// Insufficient - magnus
					// Cover Insufficient Mask Rectangle
					nRectTempIndexEncap = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDarkInsufficientCoverMask.Width() != 0 && pData->m_rectDarkInsufficientCoverMask.Height() != 0)
						rectTempEncap = pData->m_rectDarkInsufficientCoverMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Cover Mask Using To Find Edge Line And Check Insufficient In The Edge Of Encap [Insufficient]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDarkInsufficientCoverMask = rectTempEncap;
					DrawView(FALSE);

					// Inner Insufficient Mask Polygon
					nRectTempIndexEncap = 0;
					nOffset = 10;
					rectTempEncap = CRect(100, 100, 400, 400);
					for (int nPointId = 0; nPointId < DARK_ENCAP_INNER_INSUFFICIENT_MASK_POINT; nPointId++) {
						CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonDarkInsufficientMidMask[nPointId];
						rectTempEncap = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Draw Mask To Find Insufficient Region In The Inner Of Encap [Insufficient]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < DARK_ENCAP_INNER_INSUFFICIENT_MASK_POINT; nPointId++) {
							rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
							pInspHandler->m_Inspection.m_TrainingData.m_polygonDarkInsufficientMidMask[nPointId] = rectTempEncap.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					DrawView(FALSE);

					// Left Insufficient Mask Rectangle
					nRectTempIndexEncap = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDarkInsufficientLeftMask.Width() != 0 && pData->m_rectDarkInsufficientLeftMask.Height() != 0)
						rectTempEncap = pData->m_rectDarkInsufficientLeftMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Left Mask Using To Find Edge Region And Check Insufficient In The Edge Of Encap [Insufficient]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDarkInsufficientLeftMask = rectTempEncap;
					DrawView(FALSE);

					// Right Insufficient Mask Rectangle
					nRectTempIndexEncap = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDarkInsufficientRightMask.Width() != 0 && pData->m_rectDarkInsufficientRightMask.Height() != 0)
						rectTempEncap = pData->m_rectDarkInsufficientRightMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Right Mask Using To Find Edge Region And Check Insufficient In The Edge Of Encap [Insufficient]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDarkInsufficientRightMask = rectTempEncap;
					DrawView(FALSE);// end magnus

					// Glue - Mason
					// Glue Mask Rectangle
					nRectTempIndexEncap = 0;
					rectTempEncap = CRect(100, 100, 400, 400);
					if (pData->m_rectDarkGlueMask.Width() != 0 && pData->m_rectDarkGlueMask.Height() != 0)
						rectTempEncap = pData->m_rectDarkGlueMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw ROI Using To Find Glue Defect Region [Glue]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDarkGlueMask = rectTempEncap;
					DrawView(FALSE);

					// Contamination - Mason
					// Flex Edge Mask Rectangle
					nRectTempIndexEncap = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDarkFlexEdgeMask.Width() != 0 && pData->m_rectDarkFlexEdgeMask.Height() != 0)
						rectTempEncap = pData->m_rectDarkFlexEdgeMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw ROI Using To Re-Check Contamination Defect Region Between Flex And Encap [Contamination]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDarkFlexEdgeMask = rectTempEncap;
					DrawView(FALSE);

					// Bottom Edge Mask Rectangle
					nRectTempIndexEncap = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDarkBottomEdgeMask.Width() != 0 && pData->m_rectDarkBottomEdgeMask.Height() != 0)
						rectTempEncap = pData->m_rectDarkBottomEdgeMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw ROI Using To Re-Check Contamination Defect Region In The Bottom Of Encap [Contamination]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDarkBottomEdgeMask = rectTempEncap;
					DrawView(FALSE);

					// Right Edge Mask Rectangle
					nRectTempIndexEncap = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDarkRightEdgeMask.Width() != 0 && pData->m_rectDarkRightEdgeMask.Height() != 0)
						rectTempEncap = pData->m_rectDarkRightEdgeMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw ROI Using To Re-Check Contamination Defect Region In The Right Edge Of Encap [Contamination]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDarkRightEdgeMask = rectTempEncap;
					DrawView(FALSE);

					// Right Edge Mask Rectangle
					nRectTempIndexEncap = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDarkInnerMask.Width() != 0 && pData->m_rectDarkInnerMask.Height() != 0)
						rectTempEncap = pData->m_rectDarkInnerMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw ROI Using To Find Fuzzy Contamination Defect Region In The Inner Of Encap [Contamination]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDarkInnerMask = rectTempEncap;
					DrawView(FALSE);
					// // // // Void Dark Encap
					// // Region for Finding Masking for Left Egde of Die.
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea;


					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location For Finding Masking for Left Egde of Die [VOID]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea = rectTempEncapMask;
					// // Region for Finding Masking for Right Egde of Die.
					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location For Finding Masking for Right Egde of Die [VOID]");
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea = rectTempEncapMask;
					// // // // Wire And IC Dark Encap
					// // Region for Inspecting Wire Dark Encap.
					int nRectTempIndexDarkEncapWire;
					nRectTempIndexDarkEncapWire = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkEncapWirePolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap For Inspecting Wire [Wire And IC]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkEncapWirePolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexDarkEncapWire++);
							pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					// // Region for Inspecting Wire Dark Encap.
					int nRectTempIndexDarkEncapIC;
					nRectTempIndexDarkEncapIC = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkEncapICPolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap For Inspecting IC [Wire And IC]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkEncapICPolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexDarkEncapIC++);
							pInspHandler->m_Inspection.m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}				}
				// Dark Blue Encap Tracing Region
				else if (pInspHandler->m_Inspection.m_TrainingData.bEnableDarkBlueEncap) {
					int nRectIndex = 0;
					int nOffset = 10;
					CRect rectTempEncap = CRect(100, 100, 400, 400);
					DrawView(FALSE);
					// Encap Extract ROI Polygon
					for (int nPointId = 0; nPointId < DARK_BLUE_ENCAP_ROI_POINT; nPointId++) {
						CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonDBEncapExtractROI[nPointId];
						rectTempEncap = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Draw ROI To Find The Location Of Encap [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < DARK_BLUE_ENCAP_ROI_POINT; nPointId++) {
							rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
							pInspHandler->m_Inspection.m_TrainingData.m_polygonDBEncapExtractROI[nPointId] = rectTempEncap.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}

					// Encap Inner ROI Rectangle
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBEncapInnerROI.Width() != 0 && pData->m_rectDBEncapInnerROI.Height() != 0)
						rectTempEncap = pData->m_rectDBEncapInnerROI;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw ROI To Find The Inner Region Of Encap [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBEncapInnerROI = rectTempEncap;
					DrawView(FALSE);

					// Encap Top Edge Mask ROI Rectangle
					nRectIndex = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBEncapTopEdgeMaskROI.Width() != 0 && pData->m_rectDBEncapTopEdgeMaskROI.Height() != 0)
						rectTempEncap = pData->m_rectDBEncapTopEdgeMaskROI;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Top ROI To Find The Top Edge Of Encap [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBEncapTopEdgeMaskROI = rectTempEncap;

					// Encap Bottom Edge Mask ROI Rectangle
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBEncapBottomEdgeMaskROI.Width() != 0 && pData->m_rectDBEncapBottomEdgeMaskROI.Height() != 0)
						rectTempEncap = pData->m_rectDBEncapBottomEdgeMaskROI;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Bottom ROI To Find The Bottom Edge Of Encap [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBEncapBottomEdgeMaskROI = rectTempEncap;

					// Encap Left Edge Mask ROI Rectangle
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBEncapLeftEdgeMaskROI.Width() != 0 && pData->m_rectDBEncapLeftEdgeMaskROI.Height() != 0)
						rectTempEncap = pData->m_rectDBEncapLeftEdgeMaskROI;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Left ROI To Find The Left Edge Of Encap [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBEncapLeftEdgeMaskROI = rectTempEncap;

					// Encap Right Edge Mask ROI Rectangle
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBEncapRightEdgeMaskROI.Width() != 0 && pData->m_rectDBEncapRightEdgeMaskROI.Height() != 0)
						rectTempEncap = pData->m_rectDBEncapRightEdgeMaskROI;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Right ROI To Find The Right Edge Of Encap [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBEncapRightEdgeMaskROI = rectTempEncap;
					DrawView(FALSE);

					// Encap Flex Edge Mask ROI Rectangle
					nRectIndex = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBEncapFlexEdgeMaskROI.Width() != 0 && pData->m_rectDBEncapFlexEdgeMaskROI.Height() != 0)
						rectTempEncap = pData->m_rectDBEncapFlexEdgeMaskROI;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw ROI To Find Edge Between Encap And Flex [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBEncapFlexEdgeMaskROI = rectTempEncap;
					DrawView(FALSE);

					// Excess - Mason
					// Left Excess Mask Rectangle
					nRectIndex = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBLeftExcessMask.Width() != 0 && pData->m_rectDBLeftExcessMask.Height() != 0)
						rectTempEncap = pData->m_rectDBLeftExcessMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Left Mask Using To Check Excess In The Left Of Encap [Excess]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBLeftExcessMask = rectTempEncap;
					DrawView(FALSE);

					// Right Excess Mask Rectangle
					nRectIndex = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBRightExcessMask.Width() != 0 && pData->m_rectDBRightExcessMask.Height() != 0)
						rectTempEncap = pData->m_rectDBRightExcessMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Right Mask Using To Check Excess In The Right Of Encap [Excess]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBRightExcessMask = rectTempEncap;
					DrawView(FALSE);

					// Insufficient - Mason
					// Cover Insufficient Mask Rectangle
					nRectIndex = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBCoverInsufficientMask.Width() != 0 && pData->m_rectDBCoverInsufficientMask.Height() != 0)
						rectTempEncap = pData->m_rectDBCoverInsufficientMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Cover Mask Using To Find Edge Line And Check Insufficient In The Edge Of Encap [Insufficient]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBCoverInsufficientMask = rectTempEncap;
					DrawView(FALSE);
					
					// Inner Insufficient Mask Polygon
					nRectIndex = 0;
					nOffset = 10;
					rectTempEncap = CRect(100, 100, 400, 400);					
					for (int nPointId = 0; nPointId < DARK_BLUE_ENCAP_INNER_INSUFFICIENT_MASK_POINT; nPointId++) {
						CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonDBInnerInsufficientMask[nPointId];
						rectTempEncap = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Draw Mask To Find Insufficient Region In The Inner Of Encap [Insufficient]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < DARK_BLUE_ENCAP_INNER_INSUFFICIENT_MASK_POINT; nPointId++) {
							rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
							pInspHandler->m_Inspection.m_TrainingData.m_polygonDBInnerInsufficientMask[nPointId] = rectTempEncap.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					DrawView(FALSE);

					// Left Insufficient Mask Rectangle
					nRectIndex = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBLeftInsufficientMask.Width() != 0 && pData->m_rectDBLeftInsufficientMask.Height() != 0)
						rectTempEncap = pData->m_rectDBLeftInsufficientMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Left Mask Using To Find Edge Region And Check Insufficient In The Edge Of Encap [Insufficient]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBLeftInsufficientMask = rectTempEncap;
					DrawView(FALSE);

					// Right Insufficient Mask Rectangle
					nRectIndex = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBRightInsufficientMask.Width() != 0 && pData->m_rectDBRightInsufficientMask.Height() != 0)
						rectTempEncap = pData->m_rectDBRightInsufficientMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Right Mask Using To Find Edge Region And Check Insufficient In The Edge Of Encap [Insufficient]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBRightInsufficientMask = rectTempEncap;
					DrawView(FALSE);

					// Glue - Mason
					nRectIndex = 0;
					nOffset = 10;
					rectTempEncap = CRect(100, 100, 400, 400);
					DrawView(FALSE);
					// Glue Mask Polygon
					for (int nPointId = 0; nPointId < ENCAP_GLUE_MASK_POINT; nPointId++) {
						CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonDBEncapGlueMask[nPointId];
						rectTempEncap = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Draw Mask To Find Glue Defect Region Of Encap [Glue]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < ENCAP_GLUE_MASK_POINT; nPointId++) {
							rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
							pInspHandler->m_Inspection.m_TrainingData.m_polygonDBEncapGlueMask[nPointId] = rectTempEncap.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					DrawView(FALSE);

					// Void Dark Blue Encap Inspection -Magnus
					nRectIndex = 0;
					rectTempEncap = CRect(100, 100, 400, 400);

					if (pData->m_rectDBlueVoidROI.Width() != 0 && pData->m_rectDBlueVoidROI.Height() != 0)
						rectTempEncap = pData->m_rectDBlueVoidROI;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncap);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw ROI for Void Inspection Region [Void]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncap = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectDBlueVoidROI = rectTempEncap;
					DrawView(FALSE);					
					
					// // // // Inspect BlowHole DarkBlue Encap 
					// Region for tracing Left Mask to inspect BlowHole 

					nRectTempIndexEncap = 0;
					CRect rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location Find Left Mask to inspect BlowHole on Dark Blue Encap [Blow Hole]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea = rectTempEncapMask;

					// Region for tracing Right Mask to inspect BlowHole 

					nRectTempIndexEncap = 0;
					rectTempEncapMask = CRect(100, 100, 400, 400);
					DrawView();
					DrawView(FALSE);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea.Width() != 0 &&
						pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea.Height() != 0)
						rectTempEncapMask = pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempEncapMask);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Location Find Right Mask to inspect BlowHole on Dark Blue Encap [Blow Hole]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempEncapMask = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncap++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea = rectTempEncapMask;

					// // // // Inspect Wire And IC DarkBlue Encap

					// // Region for Inspect Wire And IC on Dark Blue Encap (Bottom)
					int nRectTempIndexDarkBlueEncapWireBottom;
					nRectTempIndexDarkBlueEncapWireBottom = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkBlueEncapWireBottomPolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap Bottom For Inspect Wire on Dark Blue Encap [Wire And IC]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkBlueEncapWireBottomPolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexDarkBlueEncapWireBottom++);
							pInspHandler->m_Inspection.m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					// Region for Inspect IC on Dark Blue Encap
					int nRectTempIndexDarkBlueEncapIC;
					nRectTempIndexDarkBlueEncapIC = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkBlueEncapICPolygonPointNumber; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap For Inspect IC on Dark Blue Encap [Wire And IC]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nDarkBlueEncapICPolygonPointNumber; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexDarkBlueEncapIC++);
							pInspHandler->m_Inspection.m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
				}

				//Encap Tracing Mask Teach
				int nRectTempIndexEncapSurfaceMask;
				for (int nMaskIdx = 0; nMaskIdx < pInspHandler->m_Inspection.m_TrainingData.nEncapTracingMaskNumber; nMaskIdx++) {
					nRectTempIndexEncapSurfaceMask = 0;
					DrawView();
					DrawView(FALSE);
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapTracingMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
						rectTemp = CRect(pInspHandler->m_Inspection.m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMaskIdx][nPointId].x - nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMaskIdx][nPointId].y - nOffset,
							pInspHandler->m_Inspection.m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMaskIdx][nPointId].x + nOffset, pInspHandler->m_Inspection.m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMaskIdx][nPointId].y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
					strStatusBarMsg.Format("Locate Encap Mask Surface Location %d [Tracing]", nMaskIdx + 1);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nEncapTracingMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
							rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectTempIndexEncapSurfaceMask++);
							pInspHandler->m_Inspection.m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMaskIdx][nPointId] = rectTemp.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
				}
				nError = pInspHandler->Teach(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
					&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
					m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir,
					m_pApp->m_strConfigFile,
					2,
					bEnableRgn,
					FALSE,
					nTrack,
					nDoc,
					nFOV + 1,
					nCurrentFOV,
					pDataFirst,
					pCalibData,
					FALSE);

				if (!nError) {
					pInspHandler->SaveTeachParameters(m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir, m_pApp->m_strConfigFile, nFOV);

					strStatusBarMsg.Format("Regions found");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
					if (WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
				}
				else if (nError) {
					strInfoMsg = strInfoMsg + " - Error: Encap Location Teaching";

					AfxMessageBox("Teach Failed At " + strInfoMsg);
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					ZoomSelectedDocView(nDocFrame);
					DrawView();
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					return -1;
				}


			}

			// Contact Teach
			if (pInspHandler->m_Inspection.m_TrainingData.bEnableContact) {

				DrawView(FALSE);
				int nRectIndex;
				int nOffset;
				CRect rectTempContact = CRect(100, 100, 400, 400);

				// Contact Mask
				nRectIndex = 0;
				for (int nMaskIdx = 0; nMaskIdx < pInspHandler->m_Inspection.m_TrainingData.nContactMaskNumber; nMaskIdx++) {
					
					rectTempContact = CRect(100, 100, 400, 400);
					if (pData->m_rectContactMask[nMaskIdx].Width() != 0 && pData->m_rectContactMask[nMaskIdx].Height() != 0)
						rectTempContact = pData->m_rectContactMask[nMaskIdx];

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempContact);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Contact Mask Using To Tracing Contact Region %d [Tracing]", nMaskIdx + 1);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempContact = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectContactMask[nMaskIdx] = rectTempContact;
				}
				DrawView(FALSE);

				// Contact Region Mask
				nRectIndex = 0;
				for (int nMaskIdx = 0; nMaskIdx < pInspHandler->m_Inspection.m_TrainingData.nContactRegionMaskNumber; nMaskIdx++) {
					
					rectTempContact = CRect(100, 100, 400, 400);
					if (pData->m_rectContactRegionMask[nMaskIdx].Width() != 0 && pData->m_rectContactRegionMask[nMaskIdx].Height() != 0)
						rectTempContact = pData->m_rectContactRegionMask[nMaskIdx];

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempContact);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Contact Region Mask Using To Check Area After Tracing Contact Region %d [Tracing]", nMaskIdx + 1);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempContact = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectContactRegionMask[nMaskIdx] = rectTempContact;
				}
				DrawView(FALSE);

				// Contact Tracing Mask			
				for (int nMaskIdx = 0; nMaskIdx < pInspHandler->m_Inspection.m_TrainingData.nContactTracingMaskNumber; nMaskIdx++) {
					nRectIndex = 0;
					nOffset = 10;
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nContactTracingMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
						CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonContactTracingMask[nMaskIdx][nPointId];
						rectTempContact = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempContact, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Locate Polygon Tracing Mask %d Of Contact [Tracing]", nMaskIdx + 1);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nContactTracingMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
							rectTempContact = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
							pInspHandler->m_Inspection.m_TrainingData.m_polygonContactTracingMask[nMaskIdx][nPointId] = rectTempContact.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}

					DrawView(FALSE);
				}

				nError = pInspHandler->Teach(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
					&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
					m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir,
					m_pApp->m_strConfigFile,
					6,
					bEnableRgn,
					FALSE,
					nTrack,
					nDoc,
					nFOV + 1,
					nCurrentFOV,
					pDataFirst,
					pCalibData,
					FALSE);

				if (!nError) {
					pInspHandler->SaveTeachParameters(m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir, m_pApp->m_strConfigFile, nFOV);

					strStatusBarMsg.Format("Contact Teaching Finished.");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
					if (WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
				}
				else if (nError) {
					strInfoMsg = strInfoMsg + " - Error: Contact Teaching";

					AfxMessageBox("Teach Failed At " + strInfoMsg);
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					ZoomSelectedDocView(nDocFrame);
					DrawView();
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					return -1;
				}
			}
			
			// Flex Teach
			if (pInspHandler->m_Inspection.m_TrainingData.bFlexEnable) {

				DrawView(FALSE);
				int nRectIndex;
				int nOffset;
				CRect rectTempFlex = CRect(100, 100, 400, 400);
				// Check Enable Substrate Mask
				if (pInspHandler->m_Inspection.m_TrainingData.nFlexSubstrateMaskNumber > 0) {
					pInspHandler->m_Inspection.m_TrainingData.bFlexEnableSubstrateMask = TRUE;
				}
				else {
					pInspHandler->m_Inspection.m_TrainingData.bFlexEnableSubstrateMask = FALSE;
				}

				if (pInspHandler->m_Inspection.m_TrainingData.bFlexEnableSubstrateMask) {
					nRectIndex = 0;

					// Flex Substrate Pattern ROI 1
					rectTempFlex = CRect(100, 100, 400, 400);
					if (pData->m_rectFlexSubstratePatternROI1.Width() != 0 && pData->m_rectFlexSubstratePatternROI1.Height() != 0)
						rectTempFlex = pData->m_rectFlexSubstratePatternROI1;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Substrate Pattern ROI 1 [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectFlexSubstratePatternROI1 = rectTempFlex;

					// Flex Substrate Pattern ROI 2
					rectTempFlex = CRect(100, 100, 400, 400);
					if (pData->m_rectFlexSubstratePatternROI2.Width() != 0 && pData->m_rectFlexSubstratePatternROI2.Height() != 0)
						rectTempFlex = pData->m_rectFlexSubstratePatternROI2;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Substrate Pattern ROI 2 [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectFlexSubstratePatternROI2 = rectTempFlex;

					// Flex Substrate Search Pattern ROI
					rectTempFlex = CRect(100, 100, 400, 400);
					if (pData->m_rectFlexSubstrateSearchPatternROI.Width() != 0 && pData->m_rectFlexSubstrateSearchPatternROI.Height() != 0)
						rectTempFlex = pData->m_rectFlexSubstrateSearchPatternROI;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Substrate Search Pattern ROI [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectFlexSubstrateSearchPatternROI = rectTempFlex;
					DrawView(FALSE);
				}
				
				// Flex Top Edge Line 
				nRectIndex = 0;
				nOffset = 10;
				for (int nPointId = 0; nPointId < 2; nPointId++) {
					CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexTopEdgeLine[nPointId];
					rectTempFlex = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex, POLYGON);
				}
				DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

				strStatusBarMsg.Format("Locate Polygon Top Edge Line For Flex Tracing [Tracing]");
				m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

				if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
					for (int nPointId = 0; nPointId < 2; nPointId++) {
						rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
						pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexTopEdgeLine[nPointId] = rectTempFlex.CenterPoint();
					}
				}
				else {
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					ZoomSelectedDocView(nDocFrame);
					return -1;
				}
				DrawView(FALSE);

				// Flex Bottom Edge Line 
				nRectIndex = 0;
				nOffset = 10;
				for (int nPointId = 0; nPointId < 2; nPointId++) {
					CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexBottomEdgeLine[nPointId];
					rectTempFlex = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex, POLYGON);
				}
				DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

				strStatusBarMsg.Format("Locate Polygon Bottom Edge Line For Flex Tracing [Tracing]");
				m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

				if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
					for (int nPointId = 0; nPointId < 2; nPointId++) {
						rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
						pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexBottomEdgeLine[nPointId] = rectTempFlex.CenterPoint();
					}
				}
				else {
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					ZoomSelectedDocView(nDocFrame);
					return -1;
				}
				DrawView(FALSE);

				// Flex Left Edge Line 
				nRectIndex = 0;
				nOffset = 10;
				for (int nPointId = 0; nPointId < 2; nPointId++) {
					CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexLeftEdgeLine[nPointId];
					rectTempFlex = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex, POLYGON);
				}
				DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

				strStatusBarMsg.Format("Locate Polygon Left Edge Line For Flex Tracing [Tracing]");
				m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

				if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
					for (int nPointId = 0; nPointId < 2; nPointId++) {
						rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
						pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexLeftEdgeLine[nPointId] = rectTempFlex.CenterPoint();
					}
				}
				else {
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					ZoomSelectedDocView(nDocFrame);
					return -1;
				}
				DrawView(FALSE);

				// Flex Right Edge Line 
				nRectIndex = 0;
				nOffset = 10;
				for (int nPointId = 0; nPointId < 2; nPointId++) {
					CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexRightEdgeLine[nPointId];
					rectTempFlex = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex, POLYGON);
				}
				DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

				strStatusBarMsg.Format("Locate Polygon Right Edge Line For Flex Tracing [Tracing]");
				m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

				if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
					for (int nPointId = 0; nPointId < 2; nPointId++) {
						rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
						pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexRightEdgeLine[nPointId] = rectTempFlex.CenterPoint();
					}
				}
				else {
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					ZoomSelectedDocView(nDocFrame);
					return -1;
				}
				DrawView(FALSE);

				if (pInspHandler->m_Inspection.m_TrainingData.bFlexEnableDynamicEdge) {
					// Flex Left Edge Line Mask
					nRectIndex = 0;
					rectTempFlex = CRect(100, 100, 400, 400);

					if (pData->m_rectFlexLeftEdgeLineMask.Width() != 0 && pData->m_rectFlexLeftEdgeLineMask.Height() != 0)
						rectTempFlex = pData->m_rectFlexLeftEdgeLineMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Left Edge Line Mask For Flex Tracing [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectFlexLeftEdgeLineMask = rectTempFlex;
					DrawView(FALSE);

					// Flex Right Edge Line Mask
					nRectIndex = 0;
					rectTempFlex = CRect(100, 100, 400, 400);

					if (pData->m_rectFlexRightEdgeLineMask.Width() != 0 && pData->m_rectFlexRightEdgeLineMask.Height() != 0)
						rectTempFlex = pData->m_rectFlexRightEdgeLineMask;

					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Draw Right Edge Line Mask For Flex Tracing [Tracing]");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pData->m_rectFlexRightEdgeLineMask = rectTempFlex;
					DrawView(FALSE);
				}

				// Flex Bottom ROI Rectangle
				nRectIndex = 0;
				rectTempFlex = CRect(100, 100, 400, 400);

				if (pData->m_rectFlexBottomROI.Width() != 0 && pData->m_rectFlexBottomROI.Height() != 0)
					rectTempFlex = pData->m_rectFlexBottomROI;

				DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex);
				DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

				strStatusBarMsg.Format("Draw ROI To Find The Edge Between Flex And Encap [Tracing]");
				m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

				if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
					rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
				else {
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					ZoomSelectedDocView(nDocFrame);
					return -1;
				}
				pData->m_rectFlexBottomROI = rectTempFlex;
				DrawView(FALSE);

				// Flex Straight Circuit Line Mask
				nRectIndex = 0;
				nOffset = 10;
				for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nFlexStraightCircuitLineMaskPolygonPointNumber; nPointId++) {
					CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexStraightCircuitLineMask[nPointId];
					rectTempFlex = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex, POLYGON);
				}
				DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

				strStatusBarMsg.Format("Locate Polygon Mask Contain Straight Circuit Line In Flex [Tracing]");
				m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

				if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nFlexStraightCircuitLineMaskPolygonPointNumber; nPointId++) {
						rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
						pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexStraightCircuitLineMask[nPointId] = rectTempFlex.CenterPoint();
					}
				}
				else {
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					ZoomSelectedDocView(nDocFrame);
					return -1;
				}
				DrawView(FALSE);

				// Flex Number Mask
				nRectIndex = 0;
				rectTempFlex = CRect(100, 100, 400, 400);

				if (pData->m_rectFlexNumberMask.Width() != 0 && pData->m_rectFlexNumberMask.Height() != 0)
					rectTempFlex = pData->m_rectFlexNumberMask;

				DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex);
				DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

				strStatusBarMsg.Format("Draw ROI Contain The Number Of Flex [Tracing]");
				m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

				if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
					rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
				else {
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					ZoomSelectedDocView(nDocFrame);
					return -1;
				}
				pData->m_rectFlexNumberMask = rectTempFlex;
				DrawView(FALSE);

				// Flex Tracing Mask			
				for (int nMaskIdx = 0; nMaskIdx < pInspHandler->m_Inspection.m_TrainingData.nFlexTracingMaskNumber; nMaskIdx++) {
					nRectIndex = 0;
					nOffset = 10;
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nFlexTracingMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
						CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexTracingMask[nMaskIdx][nPointId];
						rectTempFlex = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Locate Polygon Tracing Mask %d Of Flex [Tracing]", nMaskIdx + 1);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nFlexTracingMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
							rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
							pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexTracingMask[nMaskIdx][nPointId] = rectTempFlex.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					
					DrawView(FALSE);
				}

				// Flex Damage Mask			
				for (int nMaskIdx = 0; nMaskIdx < pInspHandler->m_Inspection.m_TrainingData.nFlexDamageMaskNumber; nMaskIdx++) {
					nRectIndex = 0;
					
					nOffset = 10;
					for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nFlexDamageMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
						CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexDamageMask[nMaskIdx][nPointId];
						rectTempFlex = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
						DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex, POLYGON);
					}
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Locate Polygon Damage Mask %d Of Flex [Damage]", nMaskIdx + 1);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nFlexDamageMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
							rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
							pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexDamageMask[nMaskIdx][nPointId] = rectTempFlex.CenterPoint();
						}
					}
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					
					DrawView(FALSE);
				}

				// Flex Substrate Mask
				if (pInspHandler->m_Inspection.m_TrainingData.bFlexEnableSubstrateMask) {
					for (int nMaskIdx = 0; nMaskIdx < pInspHandler->m_Inspection.m_TrainingData.nFlexSubstrateMaskNumber; nMaskIdx++) {
						nRectIndex = 0;
						nOffset = 10;
						for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nFlexSubstrateMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
							CPoint PointTemp = pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexSubstrateMask[nMaskIdx][nPointId];
							rectTempFlex = CRect(PointTemp.x - nOffset, PointTemp.y - nOffset, PointTemp.x + nOffset, PointTemp.y + nOffset);
							DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTempFlex, POLYGON);
						}
						DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

						strStatusBarMsg.Format("Locate Polygon Substrate Mask %d Of Flex [Scratches]", nMaskIdx + 1);
						m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

						if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
							for (int nPointId = 0; nPointId < pInspHandler->m_Inspection.m_TrainingData.nFlexSubstrateMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
								rectTempFlex = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, nRectIndex++);
								pInspHandler->m_Inspection.m_TrainingData.m_polygonFlexSubstrateMask[nMaskIdx][nPointId] = rectTempFlex.CenterPoint();
							}
						}
						else {
							m_pApp->AddLogEvent("Teach Process Aborted");
							UpdateStatusBar(&CString("Teach Process Aborted"));
							if (m_pApp->m_pInfoMsgDlg) {
								if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
									m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
							}
							ZoomSelectedDocView(nDocFrame);
							return -1;
						}
						DrawView(FALSE);
					}
				}

				//Check Flex Teach
				nError = pInspHandler->Teach(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
					&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
					m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir,
					m_pApp->m_strConfigFile,
					5,
					bEnableRgn,
					FALSE,
					nTrack,
					nDoc,
					nFOV + 1,
					nCurrentFOV,
					pDataFirst,
					pCalibData,
					FALSE);

				if (!nError) {
					pInspHandler->SaveTeachParameters(m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir, m_pApp->m_strConfigFile, nFOV);

					strStatusBarMsg.Format("Flex Teaching Finished.");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
					if (WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
				}
				else if (nError) {
					strInfoMsg = strInfoMsg + " - Error: Flex Teaching";

					AfxMessageBox("Teach Failed At " + strInfoMsg);
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					ZoomSelectedDocView(nDocFrame);
					DrawView();
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					return -1;
				}
			}


			/* Chicklet Swage Hole Teach */
			if (pInspHandler->m_Inspection.m_TrainingData.bEnableChickletSwageHole) {

				for (int i = 0; i < pInspHandler->m_Inspection.m_TrainingData.m_nChickletNumberOfHoles; i++)
				{

					CRect rectTemp = CRect(100, 100, 400, 400);
					if (pInspHandler->m_Inspection.m_TrainingData.m_rectChickletROI[i].Width() != 0)
						rectTemp = pInspHandler->m_Inspection.m_TrainingData.m_rectChickletROI[i];

					ZoomSelectedDocView(nDocFrame, TRUE);
					DrwTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rectTemp);
					DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);

					strStatusBarMsg.Format("Locate Chicket Swage Hole %d Location [Chicklet Swage Hole]", i + 1);
					//UpdateStatusBar(&strStatusBarMsg);
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);

					if (!WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
						rectTemp = GetTrackRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, i);// nRectTempIndexRgn[nDoc]++);
					else {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
					pInspHandler->m_Inspection.m_TrainingData.m_rectChickletROI[i] = rectTemp;
				}

				nError = pInspHandler->Teach(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
					&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
					m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir,
					m_pApp->m_strConfigFile,
					4,
					bEnableRgn,
					FALSE,
					nTrack,
					nDoc,
					nFOV + 1,
					nCurrentFOV,
					pDataFirst,
					pCalibData,
					FALSE);

				if (!nError) {
					pInspHandler->SaveTeachParameters(m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_strConfigDir, m_pApp->m_strConfigFile, nFOV);

					strStatusBarMsg.Format("Regions found");
					m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
					if (WaitResponse(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo)) {
						m_pApp->AddLogEvent("Teach Process Aborted");
						UpdateStatusBar(&CString("Teach Process Aborted"));
						if (m_pApp->m_pInfoMsgDlg) {
							if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
								m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
						}
						ZoomSelectedDocView(nDocFrame);
						return -1;
					}
				}
				else if (nError) {
					strInfoMsg = strInfoMsg + " - Error: Chicklet Swage Holes Location Teaching";

					AfxMessageBox("Teach Failed At " + strInfoMsg);
					m_pApp->AddLogEvent("Teach Process Aborted");
					UpdateStatusBar(&CString("Teach Process Aborted"));
					ZoomSelectedDocView(nDocFrame);
					DrawView();
					if (m_pApp->m_pInfoMsgDlg) {
						if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
							m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
					}
					return -1;
				}

			}

			//Completed Teach for this FOV --> Need to reload the recipe for this FOV
			m_nFovCompletedTeach.push_back(nFOV);
		}
	}
	m_bTeachSingleImage = FALSE;
	m_pTrack->m_nFOVIndex = nFOVIdxBk;
	m_pApp->AddLogEvent("Teach Process Completed");
	UpdateStatusBar(&CString("Teach Process Completed"));
	if (m_pApp->m_pInfoMsgDlg) {
		if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
			m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
	}

	return nError;
}

void CAppDoc::ZoomSelectedDocView(int nDoc, BOOL bZoom)
{
	if (bZoom) {
		m_pApp->m_pFrames[nDoc]->MDIActivate();
		if (!m_pApp->m_pFrames[nDoc]->IsZoomed()) {
			m_pApp->OnMaximize(FALSE);
			m_pApp->m_pFrames[nDoc]->MDIMaximize();
		}
	}
	else {
		if (m_pApp->m_pFrames[nDoc]->IsZoomed()) {
			m_pApp->m_pFrames[nDoc]->MDIRestore();
			m_pApp->OnMaximize(TRUE);
		}
	}
}

void CAppDoc::DrawTrackStatus(BOOL bActive)
{
	DrwDel(&m_ImageViewInfo);

	if (!bActive) {
		LOGPEN TextColorRed;
		TextColorRed.lopnColor = RGB(255, 0, 0);
		TextColorRed.lopnStyle = PS_SOLID;
		TextColorRed.lopnWidth.x = 1;

		CString strMsg = "Inspection Disabled";

		DrwStr(&m_ImageViewInfo, &strMsg, &CPoint(20, 20), &TextColorRed, 120);
	}
	DrwUpdate(&m_ImageViewInfo);
}

int CAppDoc::Inspect(int nError)
{
	CPerformanceTimer timerInspect;
	timerInspect.Reset();

	if (m_bBinaryMode)
		SwitchImg(&m_ImageViewInfo, FALSE);

	m_pTrack->m_bOpticsRoi = FALSE;

	int nTrack, nDocCount;
	nTrack = m_pTrack->m_nTrack;
	nDocCount = m_pApp->m_nTotalViewDocs[nTrack];
	if (m_pApp->m_RuntimeDisplayEnable) {
		DrawView(FALSE);
		for (int z = 0; z < nDocCount; z++)
			m_pApp->m_pTracks[nTrack].m_pDoc[z]->m_bShowOverlay = TRUE;
	}

	if (!nError) {

		CTrainingData* pTrainingDataFirst = &m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[m_pTrack->m_nFOVIndex].m_Inspection.m_TrainingData;
		CInspectinonData* pInspData = &m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[/*m_pTrack->m_nFOVIndex*/0].m_Inspection.m_InspectionData;

		CPerformanceTimer timer;
		nError = -DEVICE_LOCATION;/*m_InspectionHandler[m_pTrack->m_nFOVIndex].Inspect(&m_pTrack->m_SharedData,
													&m_pApp->m_pTracks[nTrack].m_pDoc[m_nDocIdx]->m_Buffer,
													m_lDebugFlag,
													FALSE,
													nTrack,
													m_nDocIdx,
													m_pTrack->m_nFOVIndex,
													m_pApp->m_nNoOfFov,
													pTrainingDataFirst,
													pInspData);*/

		double dTime = timer.GetElapsedMSec();
		/*OutputDebugLogTo(nTrack+1, TRUE,"[Image%d] RESULT: '%s'  (%3.2fms)",
							 m_nDocIdx+1, m_InspectionHandler[m_pTrack->m_nFOVIndex].m_strErrorMsg, dTime);*/
		SetInspectionTime(&m_ImageViewInfo, dTime);
		DrawView();
	}
	else {
		UpdateStatusBar(&CString("Image Snap Failure"), 2);
		UpdateStatusBar(&CString("Fail Image Snap"), 3);
	}

	if (m_lDebugFlag & DEBUG_STEP_MODE) {
		DrawView(FALSE);

		int nDocFrame = !nTrack ? m_nDocIdx : m_pApp->m_nTotalViewDocs[nTrack - 1] + 0;
		if (!m_pApp->m_pFrames[nDocFrame]->IsZoomed()) {
			m_pApp->OnMaximize(FALSE);
			m_pApp->m_pFrames[nDocFrame]->MDIMaximize();
		}

		CImageBuffer bufBk[5];
		CRect rect = CRect(CPoint(0, 0), m_Buffer.GetImgSize());
		for (int x = 0; x < nDocCount; x++) {
			bufBk[x].Allocate(rect.Size());
			ImgCopy(&m_pApp->m_pTracks[nTrack].m_pDoc[x]->m_Buffer, &rect, &bufBk[x], &rect);
		}
		for (int x = 0; x < nDocCount; x++) {
			COverlayArray* pArray = &m_InspectionHandler[m_pTrack->m_nFOVIndex].m_Inspection.m_arrayOverlay[0];
			int nCount = (int)pArray->GetCount();
			if (nCount) {
				for (int nIndex = 0; nIndex < nCount; nIndex++) {
					m_InspectionHandler[m_pTrack->m_nFOVIndex].m_Inspection.DebugStep(m_pApp->m_pTracks[nTrack].m_SharedData.pImageViews[x],
						m_pApp->m_pTracks[nTrack].m_SharedData.pProcBuffers[x],//&m_Buffer, 
						nIndex, 0);
					nDocFrame = !nTrack ? m_nDocIdx : m_pApp->m_nTotalViewDocs[nTrack - 1] + x;
					m_pApp->m_pFrames[nDocFrame]->MDIActivate();
					if (!m_pApp->m_pFrames[nDocFrame]->IsZoomed()) {
						m_pApp->OnMaximize(FALSE);
						m_pApp->m_pFrames[nDocFrame]->MDIMaximize();
					}

					if (!Navigate(nIndex, x)) {
						x = nDocCount;
						break;
					}
				}
			}
		}

		for (int x = 0; x < nDocCount; x++) {
			ImgCopy(&bufBk[x], &rect, &m_pApp->m_pTracks[nTrack].m_pDoc[x]->m_Buffer, &rect);
			COverlayArray* pArray = &m_InspectionHandler[m_pTrack->m_nFOVIndex].m_Inspection.m_arrayOverlay[0];
			int nCount = (int)pArray->GetCount();
			/*for(int nIndex=0; nIndex<nCount; nIndex++) {
				if(!pArray->GetAt(nIndex).hImage.IsInitialized())
					DrawRegion(m_pApp->m_pTracks[nTrack].m_SharedData.pImageViews[x], pArray->GetAt(nIndex));
			}*/
			nDocFrame = !nTrack ? m_nDocIdx : m_pApp->m_nTotalViewDocs[nTrack - 1] + x;
			if (m_pApp->m_pFrames[nDocFrame]->IsZoomed()) {
				m_pApp->m_pFrames[nDocFrame]->MDIRestore();
				m_pApp->OnMaximize(TRUE);
			}
		}

		DrawView();
	}

	LOGPEN TextColorRed, TextColorGreen, TextColor, TextColorYellow;

	TextColorRed.lopnColor = RGB(230, 50, 50);
	TextColorRed.lopnStyle = PS_SOLID;
	TextColorRed.lopnWidth.x = 1;

	TextColorYellow.lopnColor = RGB(230, 230, 0);
	TextColorYellow.lopnStyle = PS_SOLID;
	TextColorYellow.lopnWidth.x = 1;

	TextColorGreen.lopnColor = RGB(50, 230, 50);
	TextColorGreen.lopnStyle = PS_SOLID;
	TextColorGreen.lopnWidth.x = 1;

	CString strDefectMsg;
	if (m_pApp->m_RuntimeDisplayEnable) {
		if (nError < GOOD_UNIT) {
			LOGPEN TextDefectColor;
			//int nCount=1;//// Initialized count to 1; for Process error
			//for(int nTrack=0; nTrack<m_pApp->m_nTotalTracks; nTrack++) {
				//CArray<int>* pArray = &pInsp->m_Inspection.nErrorCodeArray[nTrack];
				//nCount = pArray->GetCount();
				//for(int i=0; i<nCount; i++) {
			CString strDefectMsg;
			strDefectMsg.Format("%s FAIL", m_pApp->GetFailedStr(nError, FALSE));//m_pApp->GetFailedStr(nError1, false);
			//TextDefectColor = TextColorRed;//GetDefectcolor(pArray->GetAt(i));

			TextDefectColor = TextColorRed;//GetDefectcolor(pArray->GetAt(i));
			DrwStr(&m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_ImageViewInfo, &strDefectMsg, &CPoint(180, 20), &TextDefectColor, 100);
			//}
		//}
		}
		else {
			strDefectMsg = "PASS";//.Format(" PASS");
			TextColor = TextColorGreen;
			//for(int i=0; i<m_pApp->m_nTotalTracks; i++)
			DrwStr(&m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_ImageViewInfo, &strDefectMsg, &CPoint(180, 20), &TextColor, 120);
		}

		CRect rect = CRect(CPoint(80, 80), CSize(60, 60));
		BOOL bFill = m_nInspCounter % 2;
		if (bFill)
			rect.DeflateRect(6, 6);
		else
			TextColorYellow.lopnWidth.x = 10;
		//for(int i=0; i<m_pApp->m_nTotalTracks; i++) {
		DrwRect(&m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_ImageViewInfo, &rect, &TextColorYellow/*, bFill*/);
		DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_ImageViewInfo);
		//}
	}

	return nError;
}

int CAppDoc::InspectAllDocOnline(int nDieFOV, int nDocumentId, int nScannedImageID)
{
	int inspectionReturnValue = GOOD_UNIT;

	int nThreadId = nScannedImageID % STATION_THR_THREAD_COUNT;

	CApplication* pApp = (CApplication*)AfxGetApp();
	CTrainingData* pTrainingDataFirst;
	CInspectinonData* pInspData;
	CCalibrationData* pCalibData;

	CStringArray strArrayInspValues;
	strArrayInspValues.RemoveAll();

	int nTrack = m_pTrack->m_nTrack;

	pTrainingDataFirst = &m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandlerOnline[nThreadId][nDieFOV].m_Inspection.m_TrainingData;
	pApp->m_pTracks[nTrack].m_pDoc[0]->inspectionOnline(nThreadId, nDieFOV).ClearTeachParam(pTrainingDataFirst);
	pInspData = &pApp->m_pTracks[0].m_pDoc[0]->inspectionOnline(nThreadId, nDieFOV).m_Inspection.m_InspectionData;
	pCalibData = &pApp->m_pTracks[nTrack].m_pDoc[0]->inspectionOnline(nThreadId, nDieFOV).m_Inspection.m_CalibrationData;

	if (pApp->m_bPixelRuler)
	{
		pApp->m_pPixelRulerDlg->ShowWindow(SW_HIDE);
		pApp->m_bPixelRuler = FALSE;
	}

	pApp->m_pTracks[0].m_pDoc[0]->DrawView(FALSE);
	if (pApp->m_pTracks[0].m_pDoc[0]->m_bBinaryMode) {
		for (int nDoc = 0; nDoc < pApp->m_nTotalViewDocs[nTrack]; nDoc++)
			SwitchImg(&pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, FALSE);
	}

	pApp->m_pTracks[nTrack].m_bOpticsRoi = FALSE;
	if (pApp->m_RuntimeDisplayEnable) {
		for (int nDoc = 0; nDoc < pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
			pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bShowOverlay = TRUE;
		}
	}

	int nCurrentFOV = pApp->m_nNoOfFov;
	if (nTrack == STATION_TOP) {
		nCurrentFOV = pApp->motionParameters.GetTotalFOVPerDieTop();
	}
	else {
		nCurrentFOV = pApp->motionParameters.GetTotalFOVPerDieTHR();
	}

	for (int nDoc = 0; nDoc < nDocumentId; nDoc++) {
		HRegion hEmptyRegion;
		GenEmptyRegion(&hEmptyRegion);
		ConcatObj(pTrainingDataFirst->hObjectDeviceLocation, hEmptyRegion, &pTrainingDataFirst->hObjectDeviceLocation);
		TupleConcat(pTrainingDataFirst->hTupleProjectionParam, (HTuple(0).TupleConcat(0)).TupleConcat(0), &pTrainingDataFirst->hTupleProjectionParam);
		TupleConcat(pTrainingDataFirst->hTupleDatumCenterParam, (HTuple(0).TupleConcat(0)).TupleConcat(0), &pTrainingDataFirst->hTupleDatumCenterParam);
		TupleConcat(pTrainingDataFirst->hTupleTeachDoc, 0, &pTrainingDataFirst->hTupleTeachDoc);
		TupleConcat(pTrainingDataFirst->hTupleDatumTeachTuple, 0, &pTrainingDataFirst->hTupleDatumTeachTuple);
		TupleConcat(pTrainingDataFirst->hTupleEdgeTeachTuple, 0, &pTrainingDataFirst->hTupleEdgeTeachTuple);
	}

	for (int nInsp = 0; nInsp < 2; nInsp++) {	//// 0-Device Loc Insp; 1-Rest All Insp [Region, Encap, Die, ...]; ////

		CInspectionHandler* pInspHandler = &pApp->m_pTracks[nTrack].m_pDoc[nDocumentId]->inspectionOnline(nThreadId, nDieFOV);
		inspectionReturnValue = pInspHandler->Inspect(&m_pTrack->m_SharedData,
			&pApp->m_pTracks[nTrack].m_pDoc[nDocumentId]->imageGrab[nScannedImageID],
			&pApp->m_pTracks[nTrack].m_pDoc[nDocumentId]->m_TeachBuffer[nDieFOV],
			0,
			!nInsp ? FALSE : TRUE,
			nTrack,
			nDocumentId,
			pApp->m_nTotalViewDocs[nTrack],
			nDieFOV + 1,
			nCurrentFOV,
			pTrainingDataFirst,
			pInspData,
			pCalibData,
			strArrayInspValues);
	}

	return inspectionReturnValue;
}

int CAppDoc::InspectAllDoc(int nError)
{
	CTrainingData* pTrainingDataFirst;
	CInspectinonData* pInspData;
	CCalibrationData* pCalibData;

	LOGPEN TextColorRed, TextColorGreen, TextColor, TextColorYellow;
	int nTrack, nErrInsp[5];
	CStringArray strArrayInspValues;
	strArrayInspValues.RemoveAll();
	BOOL bStepDebug;


	TextColorRed.lopnColor = RGB(230, 50, 50);
	TextColorRed.lopnStyle = PS_SOLID;
	TextColorRed.lopnWidth.x = 1;
	TextColorYellow.lopnColor = RGB(230, 230, 0);
	TextColorYellow.lopnStyle = PS_SOLID;
	TextColorYellow.lopnWidth.x = 1;
	TextColorGreen.lopnColor = RGB(50, 230, 50);
	TextColorGreen.lopnStyle = PS_SOLID;
	TextColorGreen.lopnWidth.x = 1;

	nTrack = m_pTrack->m_nTrack;
	pTrainingDataFirst = &m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[m_pTrack->m_nFOVIndex].m_Inspection.m_TrainingData;
	m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[m_pTrack->m_nFOVIndex].ClearTeachParam(pTrainingDataFirst);
	pInspData = &m_pApp->m_pTracks[0].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_InspectionData;
	/*pCalibData = &m_pApp->m_pTracks[0].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;*/
	pCalibData = &m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;

	if (m_pApp->m_bPixelRuler)
	{
		m_pApp->m_pPixelRulerDlg->ShowWindow(SW_HIDE);
		m_pApp->m_bPixelRuler = FALSE;
	}
	if (m_lDebugFlag & DEBUG_STEP_MODE)
		bStepDebug = 1;

	int nCurrentDoc = 0;
	if (m_pTrack->m_pActiveDocTeach != NULL)
		nCurrentDoc = m_pTrack->m_pActiveDocTeach->m_nDocIdx;
	DrawView(FALSE, nCurrentDoc);


	if (m_bBinaryMode) {
		for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++)
			SwitchImg(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, FALSE);
	}

	m_pTrack->m_bOpticsRoi = FALSE;
	if (m_pApp->m_RuntimeDisplayEnable) {
		for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
			//DrwDel(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);				
			m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bShowOverlay = TRUE;
		}
	}

	//Get total FOV for current station
	int nCurrentFOV = m_pApp->m_nNoOfFov;
	if (m_pTrack->m_nTrack == STATION_TOP) {
		nCurrentFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	}
	else if (m_pTrack->m_nTrack == STATION_THR) {
		nCurrentFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();
	}

	if (!nError) {
		for (int nInsp = 0; nInsp < 2; nInsp++) {	//// 0-Device Loc Insp; 1-Rest All Insp [Region, Encap, Die, ...]; ////
			for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
				if (nDoc != nCurrentDoc) {
					HRegion hEmptyRegion;
					GenEmptyRegion(&hEmptyRegion);
					ConcatObj(pTrainingDataFirst->hObjectDeviceLocation, hEmptyRegion, &pTrainingDataFirst->hObjectDeviceLocation);
					TupleConcat(pTrainingDataFirst->hTupleProjectionParam, (HTuple(0).TupleConcat(0)).TupleConcat(0), &pTrainingDataFirst->hTupleProjectionParam);
					TupleConcat(pTrainingDataFirst->hTupleDatumCenterParam, (HTuple(0).TupleConcat(0)).TupleConcat(0), &pTrainingDataFirst->hTupleDatumCenterParam);
					TupleConcat(pTrainingDataFirst->hTupleTeachDoc, 0, &pTrainingDataFirst->hTupleTeachDoc);
					TupleConcat(pTrainingDataFirst->hTupleDatumTeachTuple, 0, &pTrainingDataFirst->hTupleDatumTeachTuple);
					TupleConcat(pTrainingDataFirst->hTupleEdgeTeachTuple, 0, &pTrainingDataFirst->hTupleEdgeTeachTuple);
					continue;
				}

				long lDebugFlagCurrent = 0;
				if (nDoc == nCurrentDoc)
					lDebugFlagCurrent = m_lDebugFlag;


				CPerformanceTimer timer;
				CInspectionHandler* pInspHandler = &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[m_pTrack->m_nFOVIndex];
				nErrInsp[nDoc] = pInspHandler->Inspect(&m_pTrack->m_SharedData,
					&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
					&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_TeachBuffer[m_pTrack->m_nFOVIndex],
					lDebugFlagCurrent,
					!nInsp ? FALSE : TRUE,
					nTrack,
					nDoc,
					m_pApp->m_nTotalViewDocs[nTrack],
					m_pTrack->m_nFOVIndex + 1,
					/*m_pApp->m_nNoOfFov*/nCurrentFOV,
					pTrainingDataFirst,
					pInspData,
					pCalibData,
					strArrayInspValues);

				double dTime = timer.GetElapsedMSec();
				if (nInsp == 0) {
					OutputDebugLogTo(nTrack + 1, TRUE, "[Track%d Image FOV%d] Device Location Time: (%3.2fms)", nTrack + 1, m_pTrack->m_nFOVIndex + 1, dTime);
				}
				else {
					OutputDebugLogTo(nTrack + 1, TRUE, "[Track%d Image FOV%d] Region Inspection Time: (%3.2fms)", nTrack + 1, m_pTrack->m_nFOVIndex + 1, dTime);
				}

				SetInspectionTime(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, dTime);
			}
		}
		for (int nCount = 0; nCount < strArrayInspValues.GetCount(); nCount++)
			m_strLogInspectionData.Add(strArrayInspValues.GetAt(nCount));
		SetEvent(m_EventLogInspectionData);
	}
	else {
		UpdateStatusBar(&CString("Inspection Error"), 3);
	}
	if (m_lDebugFlag & DEBUG_STEP_MODE) {
		for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
			if (nDoc != nCurrentDoc)
				continue;
			DrwDel(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
			DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
		}
		if (m_pApp->m_pInfoMsgDlg) {
			if (!m_pApp->m_pInfoMsgDlg->IsWindowVisible())
				m_pApp->m_pInfoMsgDlg->ShowWindow(TRUE);
		}
		BOOL bAborted = FALSE;
		for (int nInsp = 0; nInsp < 2; nInsp++) {
			for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++)
			{
				if (nDoc != nCurrentDoc)
					continue;
				CString strInfoMsg;
				int nDocFrame = !nTrack ? nDoc : m_pApp->m_nTOP + nDoc;// !nTrack ? nDoc : (nTrack == 1 ? m_pApp->m_nTop + nDoc : m_pApp->m_nTop + m_pApp->m_nBottom + nDoc);
				ZoomSelectedDocView(nDocFrame, TRUE);
				CInspectionHandler* pInspHandler = &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[m_pTrack->m_nFOVIndex];
				COverlayArray* pArray = &pInspHandler->m_Inspection.m_arrayOverlay[nInsp];
				int nCount = (int)pArray->GetCount();
				CRect rect = CRect(CPoint(0, 0), m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer.GetImgSize());
				CImageBuffer bufBk;
				bufBk.Allocate(rect.Size());
				ImgCopy(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer, &rect, &bufBk, &rect);
				m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer.ClrImg();
				strInfoMsg.Format("%s - Position%d", !nTrack ? "Top Camera" : "THR Camera",
					m_pTrack->m_nFOVIndex + 1,
					!nTrack ? m_pApp->m_strTopCameraName.GetAt(nDoc) : m_pApp->m_strTHRCameraName.GetAt(nDoc));

				if (nCount) {
					for (int nIndex = 0; nIndex < nCount; nIndex++) {
						COverlayData* pData = &pInspHandler->m_Inspection.m_arrayOverlay[nInsp].GetAt(nIndex);
						CString strStatusBarMsg;
						strStatusBarMsg = pData->strDebugMessage;
						m_pApp->UpdateInfoMsgDlg(strInfoMsg, strStatusBarMsg);
						m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer.ClrImg();
						pInspHandler->m_Inspection.DebugStep(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer, nIndex, nInsp);
						if (!Navigate(nIndex, nDoc)) {
							bAborted = TRUE;
							break;
						}
					}
				}
				ImgCopy(&bufBk, &rect, &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer, &rect);
				if (bAborted) {
					nDoc = m_pApp->m_nTotalViewDocs[nTrack];
					ZoomSelectedDocView(nDocFrame, FALSE);
					nInsp = 2;
					bAborted = FALSE;
				}
				ZoomSelectedDocView(nDocFrame, FALSE);
			}
		}

		if (m_pApp->m_pInfoMsgDlg) {
			if (m_pApp->m_pInfoMsgDlg->IsWindowVisible())
				m_pApp->m_pInfoMsgDlg->ShowWindow(FALSE);
		}
		for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
			DrwDel(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
			DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
		}
	}
	CString strDefectMsg;
	for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
		if (nDoc != nCurrentDoc)
			continue;
		if (m_pApp->m_RuntimeDisplayEnable) {
			if (m_pApp->m_pWndPVIDefectParameters) {
				int nDocFrame = !nTrack ? nDoc : m_pApp->m_nTOP + nDoc;// !nTrack ? nDoc : (nTrack == 1 ? m_pApp->m_nTop + nDoc : m_pApp->m_nTop + m_pApp->m_nBottom + nDoc);
				if (m_pApp->m_pWndPVIDefectParameters->IsWindowVisible() && m_pApp->m_pWndPVIDefectParameters->m_bPVIDefectImgEnable[m_pTrack->m_nFOVIndex][nDocFrame] /*&& nErrInsp[nDoc]==-PVI_INSP*/) {
					m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[m_pTrack->m_nFOVIndex].m_Inspection.DisplayPVIDefectImage(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo,
						&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
						m_pApp->m_pWndPVIDefectParameters->m_ComboPviArea.GetCurSel());
				}
			}
			if (nErrInsp[nDoc] < GOOD_UNIT) {
				if (nErrInsp[nDoc] > -ERROR_CODES || nErrInsp[nDoc] == -PROCESS_ERROR) {
					strDefectMsg.Format("%s ", m_pApp->GetFailedStr(nErrInsp[nDoc], FALSE));
					DrwStr(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &strDefectMsg, &CPoint(240, 20), &TextColorRed, 80);
				}
				else {
					CString strTemp("PVI Defect");
					std::map<int, CString> mapPVIDefect(m_pApp->m_pTracks[nTrack].m_mapPviDefectsStatsGeneral);
					if (mapPVIDefect.find(nErrInsp[nDoc]) != mapPVIDefect.end()) {
						strTemp.Format("%s", mapPVIDefect[nErrInsp[nDoc]]);
					}
					DrwStr(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &strTemp, &CPoint(240, 20), &TextColorRed, 80);
				}

			}
			else {
				strDefectMsg = "PASS";
				DrwStr(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &strDefectMsg, &CPoint(240, 20), &TextColorGreen, 80);
			}

			CRect rect = CRect(CPoint(80, 80), CSize(60, 60));
			BOOL bFill = m_nInspCounter % 2;
			if (bFill)
				rect.DeflateRect(6, 6);
			else
				TextColorYellow.lopnWidth.x = 10;
			DrwRect(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &rect, &TextColorYellow/*, bFill*/);
			//DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
		}
	}

	nError = GOOD_UNIT;
	for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
		if (nDoc != nCurrentDoc)
			continue;

		CInspectionHandler* pInspHandler = &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[m_pTrack->m_nFOVIndex];
		for (int j = 0; j < pInspHandler->m_Inspection.m_arrayOverlayInspection.GetCount(); j++) {
			OutputDebugLogTo(3, TRUE, "Draw Overlay Doc %d: %d", nDoc, j);
			if (!pInspHandler->m_Inspection.m_arrayOverlayInspection[j].hImage.IsInitialized())
				DrawRegion(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, pInspHandler->m_Inspection.m_arrayOverlayInspection[j]);
		}
		if (nError == GOOD_UNIT) {
			if (nErrInsp[nDoc] < GOOD_UNIT)
				nError = nErrInsp[nDoc];
		}

		int nView = nTrack;// !nTrack ? nDoc : (nTrack == 1 ? m_pApp->m_nTop + nDoc : m_pApp->m_nTop + m_pApp->m_nBottom + nDoc);
		if (nErrInsp[nDoc] == -NO_IMAGE)
			m_pApp->m_nErrorCode[m_pTrack->m_nDieIndex][m_pTrack->m_nFOVIndex][nView] = 0;
		else
			m_pApp->m_nErrorCode[m_pTrack->m_nDieIndex][m_pTrack->m_nFOVIndex][nView] = nErrInsp[nDoc];

		if (!m_pApp->m_bOnline || m_pApp->m_bDisplayAllStatsOffline) {
			//	m_pApp->UpdateLotSummary(m_pTrack->m_nDieIndex, m_pTrack->m_nFOVIndex, nView, nErrInsp[nDoc], !m_pApp->m_bOnline);
		}
	}

	if (!m_pApp->m_bOnline || m_pApp->m_bDisplayAllStatsOffline)
		m_pApp->UpdateStatisticsWindow();
	DrawView(TRUE, nCurrentDoc);

	m_nInspCounter++;
	if (m_nInspCounter == 100)
		m_nInspCounter = 0;

	return nError;
}

void CAppDoc::UpdateImages(CString *strZoneDefect, CString strResult)
{
	//bufImageUpdate = m_pApp->m_SharedData[m_pApp->m_pTracks->m_nTrack].pProcBuffers;
	bufImageUpdate = m_pTrack->m_SharedData.pProcBuffers;
	m_strZoneDefect = strZoneDefect;
	m_strResult = strResult;
	//SetEvent(m_EventImageUpdate);
}

int CAppDoc::UpdateImageToOriginal()
{
	CRect rect = CRect(CPoint(0, 0), m_Buffer.GetImgSize());
	int nTrack = m_pTrack->m_nTrack;
	for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
		DrwDel(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
		ImgCopy(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bufPVIBackup, &rect, &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer, &rect);

		////CString str;
		////str.Format("C:\\Rakshith\\TempDelete\\OrgFOV%dTrack%dDoc%d.bmp", m_pTrack->m_nFOVIndex+1, nTrack+1, nDoc+1);
		////SaveGrayScaleBMPFile(str, m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bufPVIBackup);
		////m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bufPVIBackup.ClrImg();

		CInspectionHandler* pInspHandler = &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[m_pTrack->m_nFOVIndex];
		for (int j = 0; j < pInspHandler->m_Inspection.m_arrayOverlayInspection.GetCount(); j++) {
			if (!pInspHandler->m_Inspection.m_arrayOverlayInspection[j].hImage.IsInitialized())
				DrawRegion(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, pInspHandler->m_Inspection.m_arrayOverlayInspection[j]);
		}
		DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
	}

	return 0;
}

int CAppDoc::SaveOfflineImages(int nError, CString strImagePath, CString strImageName, CString strFolderName)
{
	CString strFolder, str1, str2, str3, str4, strRecipeFolder, strTemp, strImageDesc, strFile;
	str4 = "";
	str1 = m_pApp->m_ImageSaveFolder + "\\Sorted StoredImages";
	_mkdir(str1);
	strRecipeFolder.Format("%s\\%s", str1, m_pApp->m_strConfigFile);
	_mkdir(strRecipeFolder);
	strTemp.Format("%s"/*\\%s*/, strRecipeFolder/*, m_pApp->m_strLotInfo*/);
	_mkdir(strTemp);
	if (nError) {
		str3.Format("%s\\FAIL", strTemp);
		_mkdir(str3);
	}
	else {
		str3.Format("%s\\PASS", strTemp);
		_mkdir(str3);
	}
	str4.Format("%s%s", str3, strFolderName);
	_mkdir(str4);
	if (!str4.IsEmpty()) {
		strFolder.Format("%s\\%s", str4, strImageName);
		CopyFileA(strImagePath, strFolder, FALSE);
	}

	return 0;
}

void CAppDoc::OnCloseDocument()
{
	m_Buffer.Free();
	m_bufPVIBackup.Free();
	for (int nFOV = 0; nFOV < MAX_FOV; nFOV++) {
		if (m_TeachBuffer[nFOV].IsAllocated())
			m_TeachBuffer[nFOV].Free();
	}

	if (m_bLiveImageSingle) {
		m_bLiveImageSingle = FALSE;
		SetEvent(m_EventKillLiveSingle);
	}

	if (m_pApp->m_bAllocFailedImageBuffer) {
		for (int i = 0; i < 25; i++)
			m_FailStack[i].bufFailImage.Free();
	}

	if (m_pTrack != NULL) {
		m_pTrack->m_pDoc[m_nDocIdx] = NULL;

		if (m_pTrack->m_bLiveImage) {
			m_pTrack->m_bLiveImage = FALSE;
			SetEvent(m_pTrack->m_EventKillLive);
		}
		if (m_pTrack->m_bLiveCalibration) {
			m_pTrack->m_bLiveCalibration = FALSE;
			SetEvent(m_pTrack->m_EventKillLiveCalibration);
		}
		if (m_pTrack->m_bContInsp) {
			m_pTrack->m_bContInsp = FALSE;
			SetEvent(m_pTrack->m_EventKillInspCycle);
		}
		if (m_pTrack->m_bTrackContInsp) {
			m_pTrack->m_bTrackContInsp = FALSE;
			SetEvent(m_pTrack->m_EventKillInspCycle);
		}
		if (m_pTrack->m_bAutoRun) {
			m_pTrack->m_bAutoRun = FALSE;
			SetEvent(m_pTrack->m_EventKillAutoRun);
		}
		if (m_bInspSavedImage)
			InspectStoredImages(FALSE);
		CString str;
		str.Format("Threshold %d_%d", m_pTrack->m_nTrack + 1, m_nDocIdx + 1);
		PutRegistryEntry(READ_REGPATH + "Preferences\\Persistence", str, m_nThreshold);
	}
	m_bExitLogEnry = TRUE;
	SetEvent(m_EventLogInspectionData);
	//m_InspectionHandler.CloseDocument();
	m_InspectionHandler[m_pTrack->m_nFOVIndex].m_Inspection.Close();

	CloseHandle(m_EventInspEnd);

	CEvImageDoc::OnCloseDocument();
}

int CAppDoc::ContinuousInspect()
{
	if (m_pTrack->m_bContInsp) {
		m_pTrack->m_bContInsp = FALSE;
		SetEvent(m_pTrack->m_EventKillInspCycle);
	}
	else {
		m_pTrack->m_bContInsp = TRUE;
		AfxBeginThread(DocInspectCycle, this, THREAD_PRIORITY_LOWEST);
	}
	return 0;
}

void CAppDoc::SetOpticsCharacteristics(BOOL bForceCalculation)
{

	if (m_pTrack->m_bViewOpticsPanel || bForceCalculation) {
		BOOL bChanged = FALSE;
		if (m_pTrack->m_bOpticsRoi) {
			if (m_bTrackRectangle) {
				for (int i = 0; i < m_pTrack->m_nOpticsFrames; i++) {
					CRect rect = GetTrackRect(&m_ImageViewInfo, i);
					if (m_pTrack->m_rectOptics[i] != rect) {
						m_pTrack->m_rectOptics[i] = rect;
						bChanged = TRUE;
					}
				}
			}
			else {
				m_pTrack->m_bOpticsRoi = FALSE;
				//m_pTrack->m_rectOptics[0] = GetTrackRect(&m_ImageViewInfo, 0);
			}
			if (bChanged)
				m_pTrack->m_nOpticsRecalcMode = 2;
			//OutputDebugLog("SET OPTICS PARAMETERS1");
		}

		///Udupa; TODO
		if (m_pTrack->m_nOpticsRecalcMode != 0) {
			COpticsDlg* pOpticsDlg = &((CMainFrame*)AfxGetMainWnd())->m_wndOpticsPanel;
			double dMean, dNoise, dFocus;
			OPTICS_PARM OpticsParm;

			//if(pOpticsDlg->m_nCircular){
			for (int i = 0; i < m_pTrack->m_nOpticsFrames; i++) {
				if (OpticsCharacteristics(&m_Buffer, &m_pTrack->m_rectOptics[i], dMean, dNoise, dFocus) != -1) {
					OpticsParm.nIndex = i;
					OpticsParm.dMean = dMean;
					OpticsParm.dSNR = dNoise;
					OpticsParm.dFocus = dFocus;
					OpticsParm.nCalcMode = m_pTrack->m_nOpticsRecalcMode;
					//					if(::IsWindow(pOpticsDlg->m_hWnd))
					pOpticsDlg->SendMessage(WM_OPTICS_SETPARM, WPARAM(&OpticsParm));
					//else
				}
				dNoise = pOpticsDlg->m_dOpticsSNR[0];
			}
			pOpticsDlg->SendMessage(WM_OPTICS_REFRESHGRAPH, WPARAM(/*1*/m_pTrack->m_nOpticsFrames));
			m_pTrack->m_nOpticsRecalcMode = 0;
		}
		if (bChanged) {
			COpticsDlg* pOpticsDlg = &((CMainFrame*)AfxGetMainWnd())->m_wndOpticsPanel;
			DelStr(&m_ImageViewInfo);
			DelRect(&m_ImageViewInfo);
			DrawOpticsROIs(FALSE);
			DrwUpdate(&m_ImageViewInfo);
		}
	}

}

void CAppDoc::DrawOpticsROIs(BOOL bDrawRect)
{
	///Udupa; TODO
	LOGPEN TextColorGreen;
	CString strMsg;
	CPoint strPosOuter;
	CPoint strPosInner;
	//	int m_dUpperGapWidth,m_dLowerGapWidth;

	////m_dUpperGapWidth=20;
	////m_dLowerGapWidth=20;
	TextColorGreen.lopnStyle = PS_DOT;
	TextColorGreen.lopnWidth.x = 1;

	COpticsDlg* pOpticsDlg = &((CMainFrame*)AfxGetMainWnd())->m_wndOpticsPanel;
	pOpticsDlg->m_bCheckROI = (m_pTrack->m_bOpticsRoi == TRUE);
	//if(pOpticsDlg->m_nCircular){
	for (int i = 0; i < m_pTrack->m_nOpticsFrames; i++) {
		if (bDrawRect)
			DrwTrackRect(&m_ImageViewInfo, &m_pTrack->m_rectOptics[i]);
		TextColorGreen.lopnColor = RGB(0, 255, 0);
		strMsg.Format("%d", i + 1);
		strPosOuter.x = m_pTrack->m_rectOptics[i].right - 16;
		strPosOuter.y = m_pTrack->m_rectOptics[i].top - 38;
		DrwStr(&m_ImageViewInfo, &strMsg, &strPosOuter, &TextColorGreen, 30);
		TextColorGreen.lopnColor = pOpticsDlg->m_clrROI[i];
		DrwRect(&m_ImageViewInfo, &CRect(CPoint(strPosOuter + CPoint(-10, 0)), CSize(35, 38)), &TextColorGreen);
	}
	if (bDrawRect) {
		SetTrackRectProperties(&m_ImageViewInfo, TRUE, FALSE, TRUE);
	}
}

void CAppDoc::SaveAllGrabbedImages(int nTrackIndex)
{
	CString strImageDir, strTemp;


	strImageDir.Format("%s\\IMAGES", m_pApp->m_LotSummaryRootDir);
	_mkdir(strImageDir);
	strTemp = strImageDir;
	strImageDir.Format("%s\\Track%d", strTemp, nTrackIndex + 1);
	_mkdir(strImageDir);

	strTemp = strImageDir;

	//////////////////////////////////////////
	CString strYear, strMonth, strDay, strHr, strMin, strSec, strMilSec;
	CString strFile;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	strYear.Format("%d", sysTime.wYear);
	if (sysTime.wMonth < 10)
		strMonth.Format("0%d", sysTime.wMonth);
	else
		strMonth.Format("%d", sysTime.wMonth);

	if (sysTime.wDay < 10)
		strDay.Format("0%d", sysTime.wDay);
	else
		strDay.Format("%d", sysTime.wDay);

	if (sysTime.wHour < 10)
		strHr.Format("0%d", sysTime.wHour);
	else
		strHr.Format("%d", sysTime.wHour);

	if (sysTime.wMinute < 10)
		strMin.Format("0%d", sysTime.wMinute);
	else
		strMin.Format("%d", sysTime.wMinute);

	if (sysTime.wSecond < 10)
		strSec.Format("0%d", sysTime.wSecond);
	else
		strSec.Format("%d", sysTime.wSecond);

	if (sysTime.wMilliseconds < 10)
		strMilSec.Format("00%d", sysTime.wMilliseconds);
	else
		if (sysTime.wMilliseconds < 100 && sysTime.wMilliseconds >= 10)
			strMilSec.Format("0%d", sysTime.wMilliseconds);
		else
			strMilSec.Format("%d", sysTime.wMilliseconds);

	strFile = strYear + strMonth + strDay + strHr + strMin + strSec + strMilSec + ".bmp";

	strImageDir.Format("%s\\%s", strTemp, strFile);
	SaveGrayScaleBMPFile(strImageDir, m_pTrack->m_pDoc[0]->m_Buffer);


	//////////////////////////////////////////

}

void CAppDoc::SaveDocImage(int nTrack, int nDoc, int nDie, int nFov, int nErrorCode)
{

	m_pApp->m_pImageQueue->PushImageBuffer(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
		nTrack,
		nDoc,
		nDie,
		nFov,
		nErrorCode,
		m_InspectionHandler[m_pTrack->m_nFOVIndex].m_Inspection.m_arrDefectRects,
		m_InspectionHandler[m_pTrack->m_nFOVIndex].m_Inspection.m_arrDefectRegions);
}


void CAppDoc::SaveInspSeqImages(int nTrack, int nDoc, int nDie, int nFov)
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString strTemp;
	CString strFile, strImageDesc;
	CString m_strImageFolders;
	m_strImageFolders.Format("%s\\Sorted StoredImages", m_pApp->m_ImageSaveFolder);
	strImageDesc.Format("HP_FCI_THA%d_Pos%d_", nDie + 1, nFov + 1);
	//strImageDesc = m_pApp->GetFailedStr(pBuf->m_nErrorCode,FALSE);
	switch (nTrack) {
	case 0:	strTemp.Format("T%d", nDoc + 1);
		strImageDesc.Append(strTemp);
		break;
	case 1: strTemp.Format("B%d", nDoc + 1);
		strImageDesc.Append(strTemp);
		break;
	case 2:	strTemp.Format("S%d", nDoc + 1);
		strImageDesc.Append(strTemp);
		break;
	}

	CString strFolder, strRecipeFolder, strImage;
	strRecipeFolder.Format("%s\\%s", m_strImageFolders, m_pApp->m_strConfigFile);
	_mkdir(strRecipeFolder);
	if (!m_pApp->m_strLotInfo.IsEmpty())
		strFolder.Format("%s\\%s", strRecipeFolder, m_pApp->m_strLotInfo);
	else
		strFolder.Format("%s\\DUMMY_LOT", strRecipeFolder);
	_mkdir(strFolder);
	strTemp.Format("%s\\Seq%d", strFolder, m_pApp->m_nSequenceCounter);
	_mkdir(strTemp);
	strFolder.Format("%s\\%s", strTemp, m_pApp->m_strPartFlex[nDie]);
	_mkdir(strFolder);
	strFile.Format("%s\\%s", strFolder, strImageDesc);
	SaveGrayScaleBMPFile(strFile + ".bmp", m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer);
	OutputDebugLog("%s saved!", strFile);

}



void CAppDoc::ZoomView(int nMode)
{
	POSITION pos = GetFirstViewPosition();
	CAppView* pView = (CAppView*)GetNextView(pos);
	if (nMode == ZOOMOUT)
		pView->ZoomOut();
	else if (nMode == ZOOMIN)
		pView->ZoomIn();
	else if (nMode == ZOOMFIT) {
		pView->ZoomFit();
		pView->ZoomFit();
	}
}


void CAppDoc::OnInspectionTeach()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnInspectionTeach();
		return;
	}*/
	m_pApp->m_nCommand = TEACH;
	m_pApp->m_nActiveTrackCommandIdx = m_pTrack->m_nTrack;
	m_pTrack->m_pActiveDoc = m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0];//this;
	SetEvent(m_pApp->m_EventRun);
	//m_pApp->m_pTracks[0].m_nCommand = TEACH;
	//m_pApp->m_pTracks[0].m_pActiveDoc = m_pApp->m_pTracks[0].m_pDoc[0];//this;
	//SetEvent(m_pApp->m_pTracks[0].m_EventRun);
}


void CAppDoc::OnUpdateInspectionTeach(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateInspectionTeach(pCmdUI);
		return;
	}*/

	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_bStoredImages &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pTrack->m_bContInsp &&
		!m_pTrack->m_bCamExtTrigEnable &&
		!m_bInspSavedImage &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nTeach));
}

void CAppDoc::OnInspectionTeachSingleImage()
{
	/*if(m_pRemoteInspDoc) {
	m_pRemoteInspDoc->OnInspectionTeach();
	return;
	}*/
	/*m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0]->*/m_bTeachSingleImage = TRUE;
	m_pApp->m_nCommand = TEACH;
	m_pApp->m_nActiveTrackCommandIdx = m_pTrack->m_nTrack;
	m_pTrack->m_pActiveDoc = this;// m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0];//this;
	SetEvent(m_pApp->m_EventRun);
	//m_pApp->m_pTracks[0].m_nCommand = TEACH;
	//m_pApp->m_pTracks[0].m_pActiveDoc = m_pApp->m_pTracks[0].m_pDoc[0];//this;
	//SetEvent(m_pApp->m_pTracks[0].m_EventRun);
}


void CAppDoc::OnUpdateInspectionTeachSingleImage(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
	m_pRemoteInspDoc->OnUpdateInspectionTeach(pCmdUI);
	return;
	}*/

	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_bStoredImages &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pTrack->m_bContInsp &&
		!m_pTrack->m_bCamExtTrigEnable &&
		!m_bInspSavedImage &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nTeach));
}


void CAppDoc::OnInspectionDebug()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnInspectionDebug();
		return;
	}*/

	if (m_bStoredImages) {
		for (int nTrack = 0; nTrack < m_pApp->m_nTotalTracks; nTrack++) {
			if (!m_pApp->m_bSetForAllTracks && m_pTrack->m_nTrack)
				continue;

			m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_lDebugFlag &= ~DEBUG_STEP_MODE;
		}

		InspectStoredImages(TRUE);
	}
	else {
		m_bInspectionDebug = !m_bInspectionDebug;

		if (m_bInspectionDebug) {
			m_lDebugFlag |= DEBUG_STEP_MODE;
		}
		else
		{
			m_lDebugFlag &= ~DEBUG_STEP_MODE;
		}
	}
}


void CAppDoc::OnUpdateInspectionDebug(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateInspectionDebug(pCmdUI);
		return;
	}
*/
	BOOL bChecked = (m_bInspSavedImage && m_bSavedImagesStep) || m_bInspectionDebug;
	pCmdUI->Enable((!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pTrack->m_bContInsp &&
		!m_bInspSavedImage &&
		!m_pTrack->m_bCamExtTrigEnable &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nStepDebug))
		|| bChecked);// && m_pApp->m_bSuperUserMode);

	pCmdUI->SetCheck(bChecked);
}

void CAppDoc::OnInspectionInspect()
{
	/*if(m_pRemoteInspDoc){
		m_pRemoteInspDoc->OnInspectionInspect();
		return;
	}*/
	//m_pApp->bIsLeftButtonClicked = FALSE;

	//m_lDebugFlag &= ~DEBUG_STEP_MODE;
	CTrackHandler* pTrack;
	UpdateStatusBar(&CString(""));

	if (GetKeyState(VK_SHIFT) & 0x8000) {
		for (int i = 0; i < m_pApp->m_nTotalTracks; i++) {
			pTrack = &m_pApp->m_pTracks[i];
			if (pTrack->m_pDoc[m_nDocIdx]->m_bStoredImages)
				pTrack->m_pDoc[m_nDocIdx]->InspectStoredImages(FALSE);
			else if (pTrack->m_pDoc[m_nDocIdx]->m_bInspectCycle)
				pTrack->m_pDoc[m_nDocIdx]->ContinuousInspect();
			else {
				m_pApp->m_nCommand = INSPECT;
				m_pApp->m_nActiveTrackCommandIdx = m_pTrack->m_nTrack;
				pTrack->m_pActiveDoc = pTrack->m_pDoc[m_nDocIdx];
				SetEvent(m_pApp->m_EventRun);
			}
		}
	}
	else {
		if (m_bStoredImages)
			InspectStoredImages(FALSE);
		else if (m_bInspectCycle)
			ContinuousInspect();
		else {
			m_pApp->m_nCommand = INSPECT;
			m_pApp->m_nActiveTrackCommandIdx = m_pTrack->m_nTrack;
			m_pTrack->m_pActiveDoc = m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[m_nDocIdx];//this;
			SetEvent(m_pApp->m_EventRun);
		}
	}
}


void CAppDoc::OnUpdateInspectionInspect(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateInspectionInspect(pCmdUI);
		return;
	}
*/
	BOOL bChecked = m_pTrack->m_bContInsp || (m_bInspSavedImage && !m_bSavedImagesStep && !m_pApp->m_bInspectionAll);
	pCmdUI->Enable((!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pTrack->m_bContInsp &&
		!m_bInspSavedImage &&
		!m_pTrack->m_bCamExtTrigEnable &&
		!m_pApp->m_bSetForAllTracks &&
		// !m_pApp->m_bLotOpened&&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nInspect))
		|| bChecked);// && m_pApp->m_bSuperUserMode);

	pCmdUI->SetCheck(bChecked);
}


void CAppDoc::OnInspectionInspectAll()
{
	UpdateStatusBar(&CString(""));
	/*if (m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnInspectionInspectAll();
		return;
	}*/


	for (int i = 0; i < m_pApp->m_nTotalTracks; i++) {
		for (int j = 0; j < m_pApp->m_nTotalViewDocs[i]; j++) {

			if (m_bInspectionDebug) {
				m_pApp->m_pTracks[i].m_pDoc[j]->m_bInspectionDebug = TRUE;
				m_pApp->m_pTracks[i].m_pDoc[j]->m_lDebugFlag |= DEBUG_STEP_MODE;
			}
			else {
				m_pApp->m_pTracks[i].m_pDoc[j]->m_bInspectionDebug = FALSE;
				m_pApp->m_pTracks[i].m_pDoc[j]->m_lDebugFlag &= ~DEBUG_STEP_MODE;
			}
		}
	}
	m_pApp->m_bInspectionAll = !m_pApp->m_bInspectionAll;


	if (m_pApp->m_bSetForAllTracks)
		InspectStoredImages(FALSE);
	else {
		m_pApp->m_nCommand = INSPECT_ALL;
		m_pApp->m_nActiveTrackCommandIdx = m_pTrack->m_nTrack;
		m_pTrack->m_pActiveDoc = m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0];//this;
		SetEvent(m_pApp->m_EventRun);
	}
}


void CAppDoc::OnUpdateInspectionInspectAll(CCmdUI *pCmdUI)
{
	/*if (m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateInspectionInspectAll(pCmdUI);
		return;
	}*/
	BOOL bChecked = (m_bInspSavedImage && !m_bSavedImagesStep && !m_pApp->m_bInspectionAll);
	pCmdUI->Enable((!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pTrack->m_bLiveImage &&
		(!m_bStoredImages || m_pApp->m_bSetForAllTracks) &&
		(!m_pApp->m_pTracks[0].m_pDoc[0]->m_bInspectCycle && !m_pApp->m_pTracks[1].m_pDoc[0]->m_bInspectCycle/* && !m_pApp->m_pTracks[2].m_pDoc[0]->m_bInspectCycle*/) &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nInspect)) && !bChecked);

	pCmdUI->SetCheck(m_pApp->m_bInspectionAll);
}

void CAppDoc::OnInspectionNext()
{
	/*if(m_pTrack->m_pDoc[0]->m_pRemoteInspDoc) {
		m_pTrack->m_pDoc[0]->m_pRemoteInspDoc->Continue();
		return;
	}*/

	if (m_bSavedImagesStep && m_bStoredImages) {
		m_pApp->m_pTracks[m_pApp->m_nStepTrack].m_pDoc[m_pApp->m_nStepDoc]->Continue();
	}
	else
		Continue();
}

void CAppDoc::OnUpdateInspectionNext(CCmdUI *pCmdUI)
{
	/*if(m_pTrack->m_pDoc[0]->m_pRemoteInspDoc) {
		m_pTrack->m_pDoc[0]->m_pRemoteInspDoc->OnUpdateInspectionNext(pCmdUI);
		return;
	}
*/
	pCmdUI->Enable(CEvImageDoc::m_bWaiting && !m_pTrack->m_bAutoRun);
}



void CAppDoc::OnInspectionAbort()
{
	/*if(m_pTrack->m_pDoc[0]->m_pRemoteInspDoc) {
		m_pTrack->m_pDoc[0]->m_pRemoteInspDoc->OnInspectionAbort();
		return;
	}*/
	if (m_pApp->m_bSetForAllTracks && m_bSavedImagesStep) {
		for (int nTrack = 0; nTrack < m_pApp->m_nTotalTracks; nTrack++) {
			m_pApp->m_pTracks[nTrack].m_pDoc[0]->Abort();
		}
	}
	else
		Abort();
}


void CAppDoc::OnUpdateInspectionAbort(CCmdUI *pCmdUI)
{
	/*if(m_pTrack->m_pDoc[0]->m_pRemoteInspDoc) {
		m_pTrack->m_pDoc[0]->m_pRemoteInspDoc->OnUpdateInspectionAbort(pCmdUI);
		return;
	}*/

	pCmdUI->Enable(CEvImageDoc::m_bWaiting && !m_pTrack->m_bAutoRun);
}


void CAppDoc::InspectStoredImages(BOOL bStep)
{
	if (m_pApp->m_bSetForAllTracks) {
		for (int nTrack = 0; nTrack < m_pApp->m_nTotalTracks; nTrack++) {
			m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_bSavedImagesStep = bStep;
		}
	}
	else
		m_bSavedImagesStep = bStep;

	if (m_bInspSavedImage) {
		m_pApp->m_bInspecting = FALSE;
		Continue();
		SetEvent(m_pTrack->m_EventKillInspectImages);
	}
	else {
		if (m_pApp->m_bSetForAllTracks) {
			for (int nTrack = 0; nTrack < m_pApp->m_nTotalTracks; nTrack++) {
				m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_bInspSavedImage = TRUE;
			}
		}
		else
			m_bInspSavedImage = TRUE;
		m_pApp->m_ThreadManager.CreateThread(TrackInspectImages, this, INSPECTION_THREAD, -1);
	}
}

void CAppDoc::OnCameraSnap()
{
	//m_pApp->m_bSynchronousGrabEnable ? m_pApp->SynchronousCameraSnap() : CameraGrab();
//	CameraGrabUsingExposure();

	m_pTrack->RefreshLight(m_nDocIdx);
	CameraGrabSingle();
}


void CAppDoc::OnUpdateCameraSnap(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;
	for (int i = 0; i < m_pApp->m_nTotalTracks; i++)
	{
		for (int j = 0; j < m_pApp->m_nTotalViewDocs[i]; j++) {
			if (m_pApp->m_pTracks[i].m_pDoc[j]->m_bLiveImageSingle)
				bEnable = TRUE;
		}
	}
	pCmdUI->Enable(m_pTrack->m_bCamEnable &&
		m_pTrack->m_bCamAvail &&
		!bEnable &&
		!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_pTrack->m_bContInsp &&
		!m_pApp->m_bTrackInspSeq &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nGrab));
}


void CAppDoc::OnCameraStream()
{
	if (!m_pTrack->m_bCamAvail)
		return;

	m_pTrack->RefreshLight(m_nDocIdx);
	CamLiveSingle();
}


void CAppDoc::OnUpdateCameraStream(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;
	for (int i = 0; i < m_pApp->m_nTotalTracks; i++)
	{
		for (int j = 0; j < m_pApp->m_nTotalViewDocs[i]; j++) {
			if (m_pApp->m_pTracks[i].m_pDoc[j]->m_bLiveImageSingle && j != m_nDocIdx)
				bEnable = TRUE;
		}
	}
	pCmdUI->Enable(m_pTrack->m_bCamEnable &&
		m_pTrack->m_bCamAvail &&
		!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!bEnable &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_pTrack->m_bContInsp &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pApp->m_bAllowStream &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nLive));

	//pCmdUI->SetCheck(m_pTrack->m_bLiveImage);
	pCmdUI->SetCheck(m_bLiveImageSingle);
}


void CAppDoc::OnCameraLight()
{
	if (m_pApp->m_pLightCtrlDlg)
		delete m_pApp->m_pLightCtrlDlg;

	m_pApp->m_pLightCtrlDlg = new CLightControlDlg();
	m_pApp->m_pLightCtrlDlg->m_nTrackId = m_pTrack->m_nTrack;
	m_pApp->m_pLightCtrlDlg->m_nDocId = m_nDocIdx;

	m_pApp->m_bLightCtrlDlgOpen = TRUE;
	if (!m_pApp->m_pLightCtrlDlg->Create(IDD_LIGHT_CONTROL_DLG, AfxGetMainWnd())) {
		delete m_pApp->m_pLightCtrlDlg;
		m_pApp->m_pLightCtrlDlg = NULL;
		m_pApp->m_bLightCtrlDlgOpen = FALSE;
		return;
	}

	CRect rect1, rect2;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rect1, 0);
	m_pApp->m_pLightCtrlDlg->GetWindowRect(&rect2);

	m_pApp->m_pLightCtrlDlg->SetWindowPos(NULL, rect1.Width() - rect2.Width(), rect1.Height() - rect2.Height() - 20, 0, 0, SWP_NOSIZE);

	m_pApp->m_pLightCtrlDlg->ShowWindow(TRUE);
}


void CAppDoc::OnUpdateCameraLight(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;
	for (int i = 0; i < m_pApp->m_nTotalTracks; i++)
	{
		for (int j = 0; j < m_pApp->m_nTotalViewDocs[i]; j++) {
			if (m_pApp->m_pTracks[i].m_pDoc[j]->m_bLiveImageSingle)
				bEnable = TRUE;
		}
	}
	pCmdUI->Enable(!m_pApp->m_bOnline && 
		!m_pApp->m_bLightCtrlDlgOpen &&
		!bEnable &&
		!m_pApp->m_bTeaching &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nLightSet));// && m_pApp->m_bSuperUserMode);
}


void CAppDoc::OnParmVision()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnParmVision();
		return;
	}*/

	((CMainFrame*)m_pApp->m_pMainWnd)->ShowProperties(this, m_pTrack->m_nFOVIndex);
}


void CAppDoc::OnUpdateParmVision(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateParmVision(pCmdUI);
		return;
	}*/
	BOOL bChecked = ((CMainFrame*)m_pApp->m_pMainWnd)->bIsPropertiesVisiblePane;
	/*pCmdUI->SetCheck(bChecked);*/
	pCmdUI->Enable(m_pApp->m_bSuperUserMode ||
		!m_pTrack->m_bContInsp &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_bSavedImagesStep &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		// !bChecked &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nVisionParm));
}


void CAppDoc::OnParmTeach()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnParmTeach();
		return;
	}*/

	((CMainFrame*)m_pApp->m_pMainWnd)->ShowTeachProperties(this, m_pTrack->m_nFOVIndex);
}


void CAppDoc::OnUpdateParmTeach(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateParmTeach(pCmdUI);
		return;
	}*/
	BOOL bChecked = ((CMainFrame*)m_pApp->m_pMainWnd)->bIsPropertiesVisiblePaneTeach;
	/*pCmdUI->SetCheck(bChecked);*/
	pCmdUI->Enable(m_pApp->m_bSuperUserMode ||
		!m_pTrack->m_bContInsp &&
		!m_pApp->m_bOnline &&
		//!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_bSavedImagesStep &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		// !bChecked &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nTeachParm));
}


void CAppDoc::OnParmDefect()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnParmDefect();
		return;
	}*/
	//((CMainFrame*)m_pApp->m_pMainWnd)->InitializeDefectProperties(this);
	((CMainFrame*)m_pApp->m_pMainWnd)->ShowDefectProperties();
}


void CAppDoc::OnUpdateParmDefect(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateParmDefect(pCmdUI);
		return;
	}*/
	BOOL bChecked = ((CMainFrame*)m_pApp->m_pMainWnd)->bIsPropertiesVisiblePaneDefect;
	/*pCmdUI->SetCheck(bChecked);*/
	pCmdUI->Enable(m_pApp->m_bSuperUserMode ||
		!m_pTrack->m_bContInsp &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_bSavedImagesStep &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		// !bChecked &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nDefectParm));
}


void CAppDoc::SetImageTitle(CString* str, BOOL bRefresh)
{
	m_strLoadImage = str ? *str : "";
	if (bRefresh)
		SetDocStatusText(&m_ImageViewInfo, NULL);
}

void CAppDoc::OnToolsStaticimageLoadFromFile()
{
	CString strImagePath;
	if (GetKeyState(VK_SHIFT) & 0x8000) {
		//m_pApp->m_bLoadMultiImagesFromFile = !m_pApp->m_bLoadMultiImagesFromFile;
		//if(m_pApp->m_bLoadMultiImagesFromFile && (AfxMessageBox("Confirm Load Multiple Images Path", MB_YESNO)==IDYES)) {
		//	CFolderDialog dlg("Stored Multiple Images", m_pApp->m_strLoadMultiImagesFolderPath);
		//	if(dlg.DoModal() == IDOK) {
		//		CString strTemp;
		//		strTemp = dlg.GetPathName();
		//		if(!strTemp.IsEmpty()) {
		//			m_pApp->m_strLoadMultiImagesFolderPath = strTemp;
		//			PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Folder: StoredMultiImages", m_pApp->m_strLoadMultiImagesFolderPath);
		//		}
		//	}
		//}

		//if(!m_pApp->m_bLoadMultiImagesFromFile){
		//	int nCurPos, nDevice, nPos, nSel;
		//	nCurPos = ((CMainFrame*) m_pApp->m_pMainWnd)->m_dlgPane->m_wndDlg.nPosNew;
		//	if(nCurPos < 0)
		//		return;
		//	nDevice = nCurPos/MAX_FOV;
		//	nSel = ((nDevice*MAX_FOV) + (nCurPos%m_pApp->m_nNoOfFov));
		//	((CMainFrame*) m_pApp->m_pMainWnd)->m_dlgPane->m_wndDlg.m_ctrlDieImages[nSel].m_bSelected = FALSE;
		//	((CMainFrame*) m_pApp->m_pMainWnd)->m_dlgPane->m_wndDlg.m_ctrlDieImages[nSel].Invalidate();

		//	((CMainFrame*) m_pApp->m_pMainWnd)->m_dlgPane->m_wndDlg.nPosNew = -1;
		//	((CMainFrame*) m_pApp->m_pMainWnd)->m_dlgPane->m_wndDlg.nPosOld = -1;
		//}
	}
	else {
		//CSelectFolder dlg;
		//dlg.SetPath(m_pApp->m_strLoadMultiImagesFolderPath);
		//dlg.m_bFindImage = TRUE;

		//if (dlg.DoModal() == IDOK)
		//{
		//	CString strTemp = dlg.GetPath();
		//	int nLength = strTemp.GetLength();
		//	strImagePath = strTemp.Left(nLength - 1);
		//	m_pApp->m_strLoadMultiImagesFolderPath = strImagePath.Left(strImagePath.ReverseFind('\\') + 1);
		//}
		//else
		//	strImagePath.Empty();

		// New Dialog Select File - Mason
		CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap Files (*.bmp)|*.bmp|JPEG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||");
		dlg.m_ofn.lpstrInitialDir = m_pApp->m_strLoadMultiImagesFolderPath;

		if (dlg.DoModal() == IDOK) {
			strImagePath = dlg.GetPathName();
			m_pApp->m_strLoadMultiImagesFolderPath = strImagePath.Left(strImagePath.ReverseFind('\\') + 1);
		}
		else
			strImagePath.Empty();

		//strImagePath = LoadImage();
		if (!strImagePath.IsEmpty()) {
			LoadStaticImage(strImagePath);
			////m_pApp->LoadDocImages(strImagePath);
			//SaveStaticImagePath(strImagePath);
			//LoadImagesToDoc(strImagePath);
			//CString* strTemp = new CString;
			//strTemp->Format("%d%d", m_nDocIdx, m_pTrack->m_nFOVIndex);
			//m_pApp->m_pMainWnd->SendMessageA(WM_SET_CUR_POS_PARAM_DISP, (WPARAM)m_pTrack->m_nTrack, (LPARAM)strTemp);
			//delete strTemp;

			if (m_bStoredImages)
				m_bStoredImages = !m_bStoredImages;
		}

	}
}

void CAppDoc::OnUpdateToolsStaticimageLoadFromFile(CCmdUI *pCmdUI)
{
	/*if (m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateToolsStaticimageLoadFromFile(pCmdUI);
		return;
	}*/
	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_bSavedImagesStep &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		!m_bStoredImages &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nLoadImg));
}

void CAppDoc::OnToolsStaticimageLoadAllFromFile()
{
	m_pApp->m_bLoadMultiImagesFromFile = TRUE;
	m_pApp->m_strLoadMultiImagesArray.RemoveAll();
	if (m_pApp->m_bLoadMultiImagesFromFile) {
		OutputDebugLogTo(9, TRUE, "LoadImageAll 1");
		//CSelectFolder dlg;
		//dlg.SetPath(m_pApp->m_strLoadMultiImagesFolderPath);
		//	CFolderDialog dlg(" ", m_pApp->m_strLoadMultiImagesFolderPath);

		// New Dialog Select Folder - Mason
		CFolderPickerDialog dlg;
		dlg.m_ofn.lpstrInitialDir = m_pApp->m_strLoadMultiImagesFolderPath;

		OutputDebugLogTo(9, TRUE, "LoadImageAll 2");
		if (dlg.DoModal() == IDOK) {
			CString strTemp;
			//strTemp = dlg.GetPath();
			strTemp = dlg.GetPathName();
			if (!strTemp.IsEmpty()) {
				m_pApp->m_strLoadMultiImagesFolderPath = strTemp;
				PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Folder: StoredMultiImages", m_pApp->m_strLoadMultiImagesFolderPath);
			}
		}
		else
			return;
	}

	CString strImageName, strPos;
	GetImageFilePaths(m_pApp->m_strLoadMultiImagesFolderPath + "\\", m_pApp->m_strLoadMultiImagesArray);
	strImageName = m_pApp->m_strLoadMultiImagesArray.GetAt(0);
	int nTempLen, nTempSel;
	int nPos = 0;


	CString strDevice = strImageName.Mid(strImageName.ReverseFind('D')); //JY
	nPos = strDevice.Find(' ');
	strImageName = strDevice.Mid(0, nPos);
	CString StrTempDie = strImageName.Mid(5, nPos);
	//m_pTrack->m_charTempPosNameForInsp = strDevice[nPos - 1];
	nTempSel = atoi(StrTempDie/*&m_pTrack->m_charTempPosNameForInsp*/);
	int nDeviceTemp = nTempSel - 1;

	int nFovNoPerY, nFovNoPerX, nDieNoPerX, nDIeNoPerY;

	m_pApp->motionParameters.GetMappingParameters(nDieNoPerX, nDIeNoPerY, nFovNoPerX, nFovNoPerY);


	int nNoofDie = nDieNoPerX * nDIeNoPerY, nNoOfFov[] = { nFovNoPerY ,nFovNoPerX *nFovNoPerY };

	int nTotalImageNoTHR = m_pApp->motionParameters.GetTotalImageGrabsTHR();
	int nTotalImageNoTOP = m_pApp->motionParameters.GetTotalImageGrabsTOP();

	for (int nDoc = 0; nDoc < theApp.m_pTracks[STATION_TOP].m_nTotalDocs; nDoc++) {
		if (theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->nTotalGrabNo != nTotalImageNoTOP) {
			theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->nTotalGrabNo = nTotalImageNoTOP;
			theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->imageGrab.clear();
			m_pApp->m_pTracks[STATION_TOP].m_pDoc[nDoc]->imageGrab.resize(nTotalImageNoTOP);
		}
	}

	for (int nDoc = 0; nDoc < theApp.m_pTracks[STATION_THR].m_nTotalDocs; nDoc++) {
		if (theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->nTotalGrabNo != nTotalImageNoTHR) {
			theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->nTotalGrabNo = nTotalImageNoTHR;
			theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->imageGrab.clear();
			m_pApp->m_pTracks[STATION_THR].m_pDoc[nDoc]->imageGrab.resize(nTotalImageNoTHR);
		}
	}

	CWaitCursor wait;
	for (int nDie = 0; nDie < nNoofDie; nDie++) {
		for (int nTrack = 0; nTrack < m_pApp->m_nTotalTracks; nTrack++) {
			for (int nDoc = 0; nDoc < theApp.m_pTracks[nTrack].m_nTotalDocs; nDoc++) {
				for (int nFov = 0; nFov < nNoOfFov[nTrack]; nFov++)
					m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->LoadMultipleImagesToDoc(m_pApp->m_strLoadMultiImagesArray, nFov + (nDie* nNoOfFov[nTrack]), nDie, FALSE);
			}
		}

	}
	wait.Restore();
	for (int nTrack = 0; nTrack < m_pApp->m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < theApp.m_pTracks[nTrack].m_nTotalDocs; nDoc++) {
			m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->LoadMultipleImagesToDoc(m_pApp->m_strLoadMultiImagesArray, 0, nDeviceTemp);
		}
	}
	//((CMainFrame*)m_pApp->m_pMainWnd)->m_dlgPane->m_wndDlg.EnableDieRect(0, nDeviceTemp);
}

void CAppDoc::OnUpdateToolsStaticimageLoadAllFromFile(CCmdUI *pCmdUI)
{
	//if (m_pRemoteInspDoc) {
	//	m_pRemoteInspDoc->OnUpdateToolsStaticimageLoadAllFromFile(pCmdUI);
	//	return;
	//}

	pCmdUI->Enable(!m_pApp->m_pTracks[0].m_bLiveImage && !m_pApp->m_pTracks[1].m_bLiveImage /*&& !m_pApp->m_pTracks[2].m_bLiveImage*/ &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bInspSequenceOffline &&
		!m_pApp->m_bTeaching &&
		!m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_bSavedImagesStep &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		!m_bStoredImages &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nLoadImgAll));
}

void CAppDoc::OnToolsStaticimageLoadTeachImage()
{
	int nError = -1;
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnToolsStaticimageLoadTeachImage();
		return;
	}*/

	//CString strTeachImage;
	//strTeachImage.Format("%s\\%s\\Cam1\\TeachTemplate.bmp",m_pApp->m_strMainCfgDir,m_pApp->m_strConfigFile);



	int nId = 0;
	int nFovNoPerY, nFovNoPerX, nDieNoPerX, nDIeNoPerY, nMaxDie;
	m_pApp->motionParameters.GetMappingParameters(nDieNoPerX, nDIeNoPerY, nFovNoPerX, nFovNoPerY);
	nMaxDie = nDieNoPerX * nDIeNoPerY;
	int nFov[2] = { m_pApp->motionParameters.GetTotalImageGrabsTOP(),m_pApp->motionParameters.GetTotalImageGrabsTHR() };

	for (int nDoc = 0; nDoc < theApp.m_pTracks[STATION_TOP].m_nTotalDocs; nDoc++) {
		if (theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->nTotalGrabNo != nFov[STATION_TOP]) {
			theApp.m_pTracks[STATION_TOP].m_pDoc[nDoc]->nTotalGrabNo = nFov[STATION_TOP];
			m_pApp->m_pTracks[STATION_TOP].m_pDoc[nDoc]->imageGrab.clear();
			m_pApp->m_pTracks[STATION_TOP].m_pDoc[nDoc]->imageGrab.resize(nFov[STATION_TOP]);
		}
	}

	for (int nDoc = 0; nDoc < theApp.m_pTracks[STATION_THR].m_nTotalDocs; nDoc++) {
		if (theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->nTotalGrabNo != nFov[STATION_THR]) {
			theApp.m_pTracks[STATION_THR].m_pDoc[nDoc]->nTotalGrabNo = nFov[STATION_THR];
			m_pApp->m_pTracks[STATION_THR].m_pDoc[nDoc]->imageGrab.clear();
			m_pApp->m_pTracks[STATION_THR].m_pDoc[nDoc]->imageGrab.resize(nFov[STATION_THR]);
		}
	}
	/*for (int j = 0; j <nMaxDie; j++) {*/
	for (int nTrack = 0; nTrack < m_pApp->m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < m_pApp->m_pTracks[nTrack].m_nTotalDocs; nDoc++) {
			for (int i = 0; i < nFov[nTrack] / nMaxDie; i++) {
				CString strImagePath;
				strImagePath.Format("%s\\Cam%d\\Doc%d\\TeachTemplate_Pos%d.bmp", m_pApp->m_strConfigDir, nTrack + 1, nDoc + 1, i + 1);

				if (nTrack == STATION_TOP)
				{
					int nRow = i / nFovNoPerX, nColumn = i % nFovNoPerX; long cameraGrabId;
					m_pApp->motionParameters.GetCameraGrabIDTOP(0, nRow, nColumn, 0, cameraGrabId);
					nId = cameraGrabId;
				}
				if (nTrack == STATION_THR)
				{
					int nRow = i / nFovNoPerX, nColumn = i % nFovNoPerX; long cameraGrabId;
					m_pApp->motionParameters.GetCameraGrabIDTHR(0, nRow, nColumn, 0, cameraGrabId);
					nId = cameraGrabId;
				}
				LoadGrayScaleBMPFile(strImagePath, m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab[nId]);
				nError = m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->LoadStaticImage();
				if (nError) {
					if (AfxMessageBox("Teach Image Not Available!!!\nDo You Like To Load Static Image?", MB_YESNO) == IDYES)
						m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->OnToolsStaticimageLoadFromFile();
					break;
				}
			}
		}
	}
	//}
}

int CAppDoc::LoadStaticImage(CString strImagePath, BOOL bDisplay, BOOL bSameBitCount)
{
	m_pTrack->m_nOpticsRecalcMode = 2;
	if (strImagePath.IsEmpty())
		//strImagePath.Format("%s\\TeachTemplate.bmp", m_strConfigDir);
		strImagePath.Format("%s\\TeachTemplate_Pos%d.bmp", m_strConfigDir, m_pTrack->m_nFOVIndex + 1);



	CPerformanceTimer timerReset;
	timerReset.Reset();
	try {
		if (m_bIsColor) {
			EnterCriticalSection(&m_csDisplayBuffer);
			CImageBuffer* pBuf = m_bBinaryMode ? &m_BufSource : &m_Buffer;
			int nError = LoadGrayScaleBMPFile(strImagePath, *pBuf, bSameBitCount);
			if (nError) {
				LeaveCriticalSection(&m_csDisplayBuffer);
				return nError;
			}

			if (m_bBinaryMode) {
				CRect rcFOV = CRect(CPoint(0, 0), m_Buffer.GetImgSize());
				ImgCopy(&m_BufSource, &rcFOV, &m_Buffer, &rcFOV);
			}
			LeaveCriticalSection(&m_csDisplayBuffer);
		}
		else {
			int nError = LoadGrayScaleBMPFile(strImagePath, m_Buffer, bSameBitCount);
			if (nError) {
				if (bDisplay /*&& m_pTrack->m_nTrack == m_pApp->m_nTotalTracks-1*/) {
					m_Buffer.ClrImg();
					DrwDel(&m_ImageViewInfo);
					DrwUpdate(&m_ImageViewInfo);
				}
				return nError;
			}
		}
		SetImageTitle(&strImagePath.Mid(strImagePath.ReverseFind('\\') + 1), bDisplay);

		if (bDisplay) {
			DrwDel(&m_ImageViewInfo);
			DrwUpdate(&m_ImageViewInfo);
		}
	}
	catch (...) {
		return -1;
	}


	if (!m_pTrack->m_bCamEnable)
		///		m_pApp->m_pImageSelectionDlg->CopyImagesToStats(m_nDocIdx);

		OutputDebugLogTo(2, TRUE, "[Doc%d] Total Load Time: %5.1fms", m_nDocIdx + 1, timerReset.GetElapsedMSec());

	return 0;
}

void CAppDoc::OnUpdateToolsStaticimageLoadTeachImage(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateToolsStaticimageLoadTeachImage(pCmdUI);
		return;
	}*/
	//pCmdUI->Enable(FALSE);
	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_bSavedImagesStep &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		!m_bStoredImages &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nLoadTeachImgAll));
}

void CAppDoc::OnToolsStaticimageLoadTeachImageSingle()
{
	int nError = -1;
	nError = m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[m_nDocIdx]->LoadStaticImage();
	if (nError) {
		AfxMessageBox("Teach Image Not Available!!!");
		return;
	}
}

void CAppDoc::OnUpdateToolsStaticimageLoadTeachImageSingle(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateToolsStaticimageLoadTeachImageSingle(pCmdUI);
		return;
	}*/

	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_bSavedImagesStep &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		!m_bStoredImages &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nLoadTeachImg));
}

void CAppDoc::OnToolsStaticimageSave()
{
	CString Folder, strFileName;

	CFileDialog   dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap Files (*.bmp)|*.bmp|JPEG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||");

	if (dlg.DoModal() == IDOK)
	{
		strFileName = dlg.GetPathName();

		int nFilterIndex = dlg.GetOFN().nFilterIndex;

		CString strExt = dlg.GetFileExt();
		if (strExt.IsEmpty() || (nFilterIndex < 0 && nFilterIndex > 2)) {
			strExt = nFilterIndex == 1 ? "bmp" : "jpg";
			strFileName += "." + strExt;
		}

		if (strExt == "bmp")
			SaveGrayScaleBMPFile(strFileName, *m_pbufImage);
		else if (strExt == "jpg")
			SaveJPGFile(strFileName, *m_pbufImage, theApp.m_nSaveJpegQualityImages);
	}
}


void CAppDoc::OnUpdateToolsStaticimageSave(CCmdUI *pCmdUI)
{
	/*if (m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateToolsStaticimageSave(pCmdUI);
		return;
	}*/
	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		!m_bStoredImages &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nSaveImg));
}

//void CAppDoc::OnToolsStaticimageSaveAll()
//{
//
//
//	CString path = "C:\\";
//	if (GetFolder(&path, path)) {
//		if (!path.IsEmpty()) {
//			CString strStatusMessage;
//			strStatusMessage.Format("Saved Images Folder = %s", path);
//			UpdateStatusBar(&strStatusMessage, 1);
//
//			CreateDirectoryTree(path);
//
//			CString fileName;
//		}
//	}
//
//
//	BOOL bSaveOne = FALSE;
//	CString strFile, strTemp, strFolder, strFileName;
//
//	CString strPath;
//	CSelectFolder dlg;
//	dlg.m_bSave = TRUE;
//	CString strFolderType[] = { "TOP_STATION","THR_STATION" };
//	dlg.SetPath(m_pApp->m_strLoadMultiImagesFolderPath);
//	if (dlg.DoModal() == IDOK)
//	{
//		CString strTemp = dlg.GetPath();
//		strPath.Format("%s%s", strTemp, dlg.m_strFileName);
//		
//	
//	}
//	else
//		strPath.Empty();
//
//	
//	if (!strPath.IsEmpty()) {
//		int nFovNoPerY, nFovNoPerX, nDieNoPerX, nDIeNoPerY;
//
//		m_pApp->motionParameters.GetMappingParameters(nDieNoPerX, nDIeNoPerY, nFovNoPerX, nFovNoPerY);
//
//		int nNoofDie = nDieNoPerX*nDIeNoPerY, nNoOfFov[] = { nFovNoPerY ,nFovNoPerX *nFovNoPerY };
//		CWaitCursor wait;
//		for (int nDie = 0; nDie < nNoofDie; nDie++) {
//			for (int nTrack = 0; nTrack<m_pApp->m_nTotalTracks; nTrack++) {
//				for (int nFov = 0; nFov < nNoOfFov[nTrack]; nFov++){
//					
//					int nRow = (nFov ) / nFovNoPerX, nColumn = (nFov ) % nFovNoPerX;
//
//					CString strName;
//					int nPos = strPath.ReverseFind('\\');
//					strFolder = strPath.Left(nPos);
//					strFileName = strPath.Mid(nPos);
//					strTemp.Format("%s\\%s", strFolder, strFolderType[nTrack]);
//					mkdir(strTemp);
//
//					strTemp.Format("%s\\%s", strFolder, strFolderType[nTrack]);
//					strName.Format("%s%s DieNo%03d Row%03d Col%03d FOV%03d", strFileName, strFolderType[nTrack], nDie , nRow , nColumn , nFov );
//					strFile.Format("%s%s.bmp", strTemp, strName);
//					long cameraGrabId;
//					if (nTrack == STATION_TOP)
//					{
//						m_pApp->motionParameters.GetCameraGrabIDTOP(0, nRow, nColumn , nDie , cameraGrabId);
//					}
//					else if (nTrack == STATION_THR)
//						m_pApp->motionParameters.GetCameraGrabIDTHR(0, nRow , nColumn , nDie , cameraGrabId);
//					if (!m_pApp->m_pTracks[nTrack].imageGrab.empty())
//					if (m_pApp->m_pTracks[nTrack].imageGrab[cameraGrabId].IsAllocated())
//					SaveGrayScaleBMPFile(strFile, m_pApp->m_pTracks[nTrack].imageGrab[cameraGrabId]);
//				}
//			}
//		
//		}
//		wait.Restore();
//	}
//}


//void CAppDoc::OnUpdateToolsStaticimageSaveAll(CCmdUI *pCmdUI)
//{
//	/*if (m_pRemoteInspDoc) {
//		m_pRemoteInspDoc->OnUpdateToolsStaticimageSaveAll(pCmdUI);
//		return;
//	}*/
//	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
//		!m_pApp->m_bOnline &&
//		!m_pApp->m_bTeaching &&
//		!m_bInspecting &&
//		!m_pApp->m_bTrackInspSeq &&
//		!m_bInspSavedImage &&
//		!m_bInspectionDebug &&
//		!m_bStoredImages &&
//		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nSaveImgAll));
//}

void CAppDoc::SaveStaticImages(BOOL bTeachImg)
{
	CString strFile, strImageFinalPath;
	CString strType[] = { "T", "B", "S" };
	UpdateStatusBar(&CString(""));

	if (bTeachImg) {
		strFile.Format("%s\\TeachTemplate.bmp", m_strConfigDir);
		SaveGrayScaleBMPFile(strFile, m_Buffer);
	}
	else {
		strFile = SaveImage(FALSE);
		int nPos = strFile.GetLength();
		if (nPos > 0) {
			strImageFinalPath.Format("%s_%s%d.bmp", strFile, strType[m_pTrack->m_nTrack], m_nDocIdx + 1);
			SaveGrayScaleBMPFile(strImageFinalPath, m_Buffer);
		}
	}
}

void CAppDoc::OnToolsStaticimage()
{
	OnToolsStaticimageLoadFromFile();
}


void CAppDoc::OnUpdateToolsStaticimage(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nLoadImg));
}


void CAppDoc::OnInspectionStoredimages()
{
	//if(m_pRemoteInspDoc) {
	//	m_pRemoteInspDoc->OnInspectionStoredimages();
	//	return;
	//}
	CString strFolderPath;
	CTrackHandler* pTrack;
	if (!m_bStoredImages) {
		if (GetFolder(&strFolderPath, m_pTrack->m_strStoredImagesPath)) {
			if (!strFolderPath.IsEmpty()) {
				CString strTemp;
				if (m_pInspectionSetting.DoModal() == IDOK) {
					m_pApp->m_bSetForAllTracks = m_pInspectionSetting.m_bCheckForAllTracks;
					m_pApp->m_nInSpectionDelay = m_pInspectionSetting.m_nInspectionDelay;

					if (m_pApp->m_bSetForAllTracks) {
						for (int i = 0; i < m_pApp->m_nTotalTracks; i++) {
							pTrack = &m_pApp->m_pTracks[i];
							pTrack->m_strStoredImagesPath = strFolderPath;
							strTemp.Format("Folder: StoredImagesT%02d", pTrack->m_nTrack + 1);
							PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", strTemp, strFolderPath);
						}
					}
					else
						m_pTrack->m_strStoredImagesPath = strFolderPath;
					strTemp.Format("Folder: StoredImagesT%02d", m_pTrack->m_nTrack + 1);
					PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", strTemp, strFolderPath);

					if (m_pApp->m_bSetForAllTracks) {
						BOOL bStoredImages = !m_bStoredImages;
						for (int i = 0; i < m_pApp->m_nTotalTracks; i++) {
							pTrack = &m_pApp->m_pTracks[i];
							pTrack->m_pDoc[m_nDocIdx]->m_bStoredImages = bStoredImages;
							if (bStoredImages)
								pTrack->m_pDoc[m_nDocIdx]->m_bInspectCycle = FALSE;
						}
					}
					else {
						m_bStoredImages = !m_bStoredImages;
						if (m_bStoredImages) {
							m_bInspectCycle = FALSE;
						}
					}
				}
			}
		}
	}
	else {
		m_bStoredImages = !m_bStoredImages;
		if (m_pApp->m_bSetForAllTracks) {
			BOOL bStoredImages = m_bStoredImages;
			for (int i = 0; i < m_pApp->m_nTotalTracks; i++) {
				pTrack = &m_pApp->m_pTracks[i];
				pTrack->m_pDoc[m_nDocIdx]->m_bStoredImages = bStoredImages;
			}
		}
		m_pApp->m_bSetForAllTracks = m_bStoredImages;
	}


	CString str;
	if (m_bStoredImages)
		str.Format("Stored Images Folder: %s", m_pTrack->m_strStoredImagesPath);
	else
		str.Format("");
	UpdateStatusBar(&str, 3);
}


void CAppDoc::OnUpdateInspectionStoredimages(CCmdUI *pCmdUI)
{/*
	if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateInspectionStoredimages(pCmdUI);
		return;
	}*/
	BOOL bTempContInsp = FALSE;
	for (int nTrack = 0; nTrack < m_pApp->m_nTotalTracks; nTrack++) {
		bTempContInsp |= m_pApp->m_pTracks[nTrack].m_bContInsp;
	}
	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pTrack->m_bCamExtTrigEnable &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		// !m_pTrack->m_bContInsp &&
		!bTempContInsp &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nInspectSavedImage));// && m_pApp->m_bSuperUserMode);

	pCmdUI->SetCheck(m_bStoredImages);
}


void CAppDoc::OnInspectionCycle()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnInspectionCycle();
		return;
	}*/

	if (GetKeyState(VK_SHIFT) & 0x8000) {
		BOOL bInspectCycle = !m_bInspectCycle;
		CTrackHandler* pTrack;
		for (int i = 0; i < m_pApp->m_nTotalTracks; i++) {
			pTrack = &m_pApp->m_pTracks[i];
			pTrack->m_pDoc[m_nDocIdx]->m_bInspectCycle = bInspectCycle;
			if (bInspectCycle)
				pTrack->m_pDoc[m_nDocIdx]->m_bStoredImages = FALSE;
		}
	}
	else {
		m_bInspectCycle = !m_bInspectCycle;
		if (m_bInspectCycle) {
			m_bStoredImages = FALSE;
			OnInspectionInspect();
		}
	}
}


void CAppDoc::OnUpdateInspectionCycle(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateInspectionCycle(pCmdUI);
		return;
	}*/

	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pTrack->m_bCamExtTrigEnable &&
		!m_bInspSavedImage &&
		!m_pTrack->m_bContInsp &&
		!m_bStoredImages &&
		!m_pApp->m_bInspectionAll &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nInspectCycle));// && m_pApp->m_bSuperUserMode);

	pCmdUI->SetCheck(m_bInspectCycle);
}


void CAppDoc::OnInspectionCameraactive()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnInspectionCameraactive();
		return;
	}
*/
	BOOL bCameraActive = !m_bCameraActive;
	CTrackHandler* pTrack;
	for (int i = 0; i < m_pApp->m_nTotalTracks; i++) {
		pTrack = &m_pApp->m_pTracks[i];
		for (int j = 0; j < m_pApp->m_nTotalViewDocs[i]; j++)
			pTrack->m_pDoc[j]->m_bCameraActive = bCameraActive;
	}

	//if(GetKeyState(VK_SHIFT) & 0x8000) {
	//	BOOL bCameraActive = !m_bCameraActive;
	//	CTrackHandler* pTrack;
	//	for(int i=0; i<m_pApp->m_nTotalTracks; i++) {
	//		pTrack = &m_pApp->m_pTracks[i];
	//		pTrack->m_pDoc[m_nDocIdx]->m_bCameraActive = bCameraActive;
	//	}
	//}
	//else {
	//	m_bCameraActive = !m_bCameraActive;
	//}
}


void CAppDoc::OnUpdateInspectionCameraactive(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateInspectionCameraactive(pCmdUI);
		return;
	}*/

	pCmdUI->Enable(!m_pApp->m_bOnline &&
		m_pTrack->m_bCamAvail &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nCamEnable));
	pCmdUI->SetCheck(m_bCameraActive);
}

void CAppDoc::OnInspectCycleAutoStepMode()
{
	m_bCycleAutoStepMode = !m_bCycleAutoStepMode;
}

void CAppDoc::OnUpdateInspectCycleAutoStepMode(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pTrack->m_bCamExtTrigEnable &&
		!m_pTrack->m_bContInsp &&
		!m_bInspSavedImage);// && m_pApp->m_bSuperUserMode);

	pCmdUI->SetCheck(m_bCycleAutoStepMode);
}


void CAppDoc::OnInspectStoredImagesPath()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnInspectStoredImagesPath();
		return;
	}*/

	CString strFolderPath;
	BOOL bAll = GetKeyState(VK_SHIFT) & 0x8000;
	if (GetFolder(&strFolderPath, m_pTrack->m_strStoredImagesPath)) {
		if (!strFolderPath.IsEmpty()) {
			CString strTemp;
			if (bAll || GetKeyState(VK_SHIFT) & 0x8000) {
				CTrackHandler* pTrack;
				for (int i = 0; i < m_pApp->m_nTotalTracks; i++) {
					pTrack = &m_pApp->m_pTracks[i];
					pTrack->m_strStoredImagesPath = strFolderPath;
					strTemp.Format("Folder: StoredImagesT%02d", pTrack->m_nTrack + 1);
					PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", strTemp, strFolderPath);
				}
			}
			else {
				m_pTrack->m_strStoredImagesPath = strFolderPath;
				strTemp.Format("Folder: StoredImagesT%02d", m_pTrack->m_nTrack + 1);
				PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", strTemp, strFolderPath);
			}

			//UpdateStatusBar(&CString("Stored Images Folder = " + strFolderPath), 3);
			m_bStoredImages = FALSE;
			OnInspectionStoredimages();
		}
	}
}

void CAppDoc::OnUpdateInspectStoredImagesPath(CCmdUI *pCmdUI)
{
	//if(m_pRemoteInspDoc) {
	//	m_pRemoteInspDoc->OnUpdateInspectStoredImagesPath(pCmdUI);
	//	return;
	//}

	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pTrack->m_bCamExtTrigEnable &&
		!m_pTrack->m_bContInsp &&
		!m_bInspSavedImage && m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nSetFolder);// && m_pApp->m_bSuperUserMode);
}

void CAppDoc::OnInspectSavePassImages()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnInspectSavePassImages();
		return;
	}*/

	if (m_lDebugFlag & DEBUG_SAVE_GOOD_IMAGE)
		m_lDebugFlag &= ~DEBUG_SAVE_GOOD_IMAGE;
	else
		m_lDebugFlag |= DEBUG_SAVE_GOOD_IMAGE;

	SaveDebugFlags();
}

void CAppDoc::OnUpdateInspectSavePassImages(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateInspectSavePassImages(pCmdUI);
		return;
	}*/
	BOOL bEnable = FALSE; //Girish commented. No functionality at this moment 
	pCmdUI->Enable(bEnable && !m_pApp->m_bOnline && m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nSaveOfflinePassImages);
	pCmdUI->SetCheck((m_lDebugFlag & DEBUG_SAVE_GOOD_IMAGE) ? 1 : 0);
}

void CAppDoc::OnInspectSaveFailImages()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnInspectSaveFailImages();
		return;
	}*/

	if (m_lDebugFlag & DEBUG_SAVE_BAD_IMAGE)
		m_lDebugFlag &= ~DEBUG_SAVE_BAD_IMAGE;
	else
		m_lDebugFlag |= DEBUG_SAVE_BAD_IMAGE;

	SaveDebugFlags();
}

void CAppDoc::OnUpdateInspectSaveFailImages(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateInspectSaveFailImages(pCmdUI);
		return;
	}*/
	BOOL bEnable = FALSE; //Girish commented. No functionality at this moment 

	pCmdUI->Enable(bEnable && !m_pApp->m_bOnline && m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nSaveOfflineFailImages);
	pCmdUI->SetCheck((m_lDebugFlag & DEBUG_SAVE_BAD_IMAGE) ? 1 : 0);
}

void CAppDoc::SaveDebugFlags()
{
	CString str;
	str.Format("DebugFlag C%d", m_pTrack->m_nTrack + 1);
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", str, (int)m_lDebugFlag);
}

void CAppDoc::OnCameraParmButton()
{
	m_pTrack->m_pCamSetupDlg = new CCameraSetupDialog(m_pTrack);
	m_pTrack->m_pCamSetupDlg->m_nExposure = m_pTrack->m_nExposure;
	m_pTrack->m_pCamSetupDlg->m_nGain = m_pTrack->m_nGain;

	m_pTrack->m_pCamSetupDlg->m_nBlackLevel = m_pTrack->m_nBlackLevel;

	CString str;
	str.Format("Camera Setup [Camera %d]", m_pTrack->m_nTrack + 1);
	m_pTrack->m_pCamSetupDlg->m_strTitle = str;
	if (!m_pTrack->m_pCamSetupDlg->Create(IDD_CAMERA_SETUP_DIALOG, (CMainFrame*)AfxGetMainWnd())) {
		m_pTrack->m_pCamSetupDlg = NULL;
		return;
	}
	m_pTrack->m_bCamSetupDlgOpen = TRUE;
	CRect rectParent;
	CRect sizeCanvas;
	((m_pTrack->m_pCamSetupDlg)->GetParent())->GetClientRect(&rectParent);
	m_pTrack->m_pCamSetupDlg->GetClientRect(&sizeCanvas);
	m_pTrack->m_pCamSetupDlg->SetWindowPos(NULL, (rectParent.right - 20) - sizeCanvas.Width(), rectParent.top + 100, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_pTrack->m_pCamSetupDlg->ShowWindow(TRUE);
}

void CAppDoc::OnUpdateCameraParmButton(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pApp->m_bOnline &&
		!m_pTrack->m_bCamSetupDlgOpen &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nCameraCfg));
}

void CAppDoc::OnViewOptionsSetupGrid()
{
	m_bSetupGrid = !m_bSetupGrid;
	DeleteAllDisplay(TRUE);
	if (!m_bSetupGrid)
		return;

	LOGPEN PenInfo, PenInfo1;
	CPoint point1, point2;
	CSize  szImgSize, szFraction;

	PenInfo.lopnColor = RGB(0, 255, 0);
	PenInfo.lopnStyle = PS_SOLID;
	PenInfo.lopnWidth.x = 1;

	PenInfo1.lopnColor = RGB(255, 0, 0);
	PenInfo1.lopnStyle = PS_SOLID;
	PenInfo1.lopnWidth.x = 5;

	int nGridNo = 8;
	szImgSize = m_Buffer.GetImgSize();
	///GetResolution(&m_pTrack->m_FGResource, &szImgSize);
	szFraction.cx = szImgSize.cx / nGridNo;
	szFraction.cy = szImgSize.cy / nGridNo;

	for (int i = 1; i < nGridNo; i++) {
		point1.x = szFraction.cx * i;
		point1.y = 0;
		point2.x = szFraction.cx * i;
		point2.y = szImgSize.cy;
		DrwLine(&m_ImageViewInfo, &point1, &point2, &PenInfo);

		point1.x = 0;
		point1.y = szFraction.cy * i;
		point2.x = szImgSize.cx;
		point2.y = szFraction.cy * i;
		DrwLine(&m_ImageViewInfo, &point1, &point2, &PenInfo);
	}

	point1.x = szImgSize.cx / 2;
	point1.y = 0;
	point2.x = szImgSize.cx / 2;
	point2.y = szImgSize.cy;
	DrwLine(&m_ImageViewInfo, &point1, &point2, &PenInfo1);

	point1.x = 0;
	point1.y = szImgSize.cy / 2;
	point2.x = szImgSize.cx;
	point2.y = szImgSize.cy / 2;
	DrwLine(&m_ImageViewInfo, &point1, &point2, &PenInfo1);

	CRect rectImg(CPoint(0, 0), szImgSize);
	DrwCircle(&m_ImageViewInfo, &rectImg, &PenInfo1);

	DrwUpdate(&m_ImageViewInfo);
}

void CAppDoc::OnUpdateViewOptionsSetupGrid(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bSetupGrid);
	pCmdUI->Enable(!m_pApp->m_bOnline && m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nSetupGrid);
}

void CAppDoc::OnViewOpticsRoi()
{
	m_pTrack->m_bOpticsRoi = !m_pTrack->m_bOpticsRoi;
	if (m_pTrack->m_bOpticsRoi) {
		DrawOpticsROIs(TRUE);
		DrwUpdate(&m_ImageViewInfo);
		UpdateStatusBar(&CString("Optics Characteristics"), 2);
		UpdateStatusBar(&CString("Reposition the ROI"), 3);
	}
	else {
		for (int i = 0; i < m_pTrack->m_nOpticsFrames; i++)
			m_pTrack->m_rectOptics[i] = GetTrackRect(&m_ImageViewInfo, i);
		DrwDel(&m_ImageViewInfo);
		DrwUpdate(&m_ImageViewInfo);
		UpdateStatusBar(&CString(""), 2);
		UpdateStatusBar(&CString(""), 3);
	}
}

void CAppDoc::OnUpdateViewOpticsRoi(CCmdUI* pCmdUI)
{

	pCmdUI->SetCheck(m_pTrack->m_bOpticsRoi);
	pCmdUI->Enable(m_pTrack->m_bViewOpticsPanel &&
		!m_pApp->m_bTeaching &&
		!m_pTrack->m_bContInsp &&
		!m_pTrack->m_bTrackContInsp &&
		!m_pApp->m_bInspecting
		/*!m_pApp->m_bCalibrating &&*/
		/* !m_pApp->m_bCalibratingDevice &&*/
		&& !m_pApp->m_bOnline && (m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= 0 /*m_pApp->m_AdvSecurityDlg.m_data.m_nOpticsROI*/));
	/* pCmdUI->Enable(!m_pApp->m_bOnline && (m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nOpticalPanel));*/
}


void CAppDoc::OnSelchangeFramecountCombo()
{
	CString strFileName, strDiectoryName, strTrack;
	int nFrames = m_pTrack->m_nOpticsFrames;

	int nOffsetTop, nOffsetBottom;
	nOffsetTop = nOffsetBottom = 20;

	strFileName.Format("Camera%d", m_pTrack->m_nTrack + 1);
	strTrack.Format("Track%d", m_pTrack->m_nTrack + 1);
	strDiectoryName = m_pTrack->m_pApp->m_strMainCfgDir + "\\" + m_pApp->m_strConfigFile[0] + "\\" + strTrack;
	//((CMainFrame*)AfxGetMainWnd())->m_wndOpticsPanel.OnResizeFrames(m_pTrack->m_nOpticsFrames, m_pTrack->m_nOpticsAvgSize);
	((CMainFrame*)AfxGetMainWnd())->m_wndOpticsPanel.OnResizeFrames(m_pTrack->m_nOpticsFrames, m_pTrack->m_nOpticsAvgSize, nOffsetTop, nOffsetBottom);

	////	m_pTrack->SaveOpticsSettings(/*m_pTrack->*/m_strConfigDir[g_nConPosIdx], m_pContactLensBVApp->m_strConfigFile[m_pContactLensBVApp->m_nConFilePosIdx]);
	m_pTrack->SaveOpticsSettings(/*strDiectoryName, m_pApp->m_strConfigFile[0]*/);


	if (m_pTrack->m_bOpticsRoi && nFrames != m_pTrack->m_nOpticsFrames) {
		for (int i = 0; i < nFrames; i++)
			m_pTrack->m_rectOptics[i] = GetTrackRect(&m_ImageViewInfo, i);
		DrwDel(&m_ImageViewInfo);
		DrawOpticsROIs(TRUE);

		m_pTrack->m_nOpticsRecalcMode = 2;
		DrwUpdate(&m_ImageViewInfo);
		SetTrackRectProperties(&m_ImageViewInfo, TRUE, FALSE, TRUE);
	}
}

void CAppDoc::OnViewOpticsPanel()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	m_pTrack->m_bViewOpticsPanel = !(pMain->m_wndOpticsPanel.IsVisible());

	int nOffsetTop, nOffsetBottom;
	nOffsetTop = nOffsetBottom = 20;

	if (m_pTrack->m_bViewOpticsPanel) {
		pMain->m_wndOpticsPanel.m_nCurrentTrack = m_pTrack->m_nTrack;
		pMain->m_wndOpticsPanel.m_nCurrentDoc = m_nDocIdx;

		CString strFocusingToolName = "Focusing Tool - ";
		if (m_pTrack->m_nTrack == STATION_TOP) {
			strFocusingToolName = strFocusingToolName + theApp.m_strTopCameraName.GetAt(m_nDocIdx);
		}
		else if (m_pTrack->m_nTrack == STATION_THR) {
			strFocusingToolName = strFocusingToolName + theApp.m_strTHRCameraName.GetAt(m_nDocIdx);
		}
		pMain->m_wndOpticsPanel.SetWindowText(strFocusingToolName);

		//pMain->m_wndOpticsPanel.OnResizeFrames(m_pTrack->m_nOpticsFrames, m_pTrack->m_nOpticsAvgSize);
		pMain->DisplayOpticsPanel(TRUE);
		m_pTrack->m_nOpticsRecalcMode = 2;
		UpdateAllViews(FALSE);
		//pMain->m_wndOpticsPanel.SetStats(m_pTrack->m_nOpticsFrames, m_pTrack->m_nOpticsAvgSize);
		pMain->m_wndOpticsPanel.SetStats(m_pTrack->m_nOpticsFrames, m_pTrack->m_nOpticsAvgSize, nOffsetTop, nOffsetBottom);

		//Show Focusing ROI
		m_pTrack->m_bOpticsRoi = TRUE;
		DrawOpticsROIs(TRUE);
		DrwUpdate(&m_ImageViewInfo);
		UpdateStatusBar(&CString("Optics Characteristics"), 2);
		UpdateStatusBar(&CString("Reposition the ROI"), 3);
	}
	else {
		pMain->DisplayOpticsPanel(FALSE);
		m_pTrack->m_bOpticsRoi = FALSE;
		DrwDel(&m_ImageViewInfo);
		DrwUpdate(&m_ImageViewInfo);
		//pMain->m_wndOpticsPanel.GetStats(m_pTrack->m_nOpticsFrames, m_pTrack->m_nOpticsAvgSize);
		pMain->m_wndOpticsPanel.GetStats(m_pTrack->m_nOpticsFrames, m_pTrack->m_nOpticsAvgSize, nOffsetTop, nOffsetBottom);
		UpdateStatusBar(&CString(""), 2);
		UpdateStatusBar(&CString(""), 3);
	}

}

void CAppDoc::OnUpdateViewOpticsPanel(CCmdUI* pCmdUI)
{
	/*if (m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateViewOpticsPanel(pCmdUI);
		return;
	}*/

	pCmdUI->Enable(!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_pApp->m_bTrackInspSeq &&
		!m_pTrack->m_bContInsp &&
		!m_pTrack->m_bCamExtTrigEnable &&
		!m_bInspSavedImage &&
		!m_bStoredImages &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nOpticalPanel));
}

void CAppDoc::OnViewSelectGoodB()
{
	CSelectGoodBDlg dlg;
	dlg.DoModal();
}

void CAppDoc::OnUpdateViewSelectGoodB(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_pApp->m_bOnline &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= 1/*m_pApp->m_AdvSecurityDlg.m_data.m_nSelectGoodB*/));
}

void CAppDoc::OnViewOptionsShowOverlay()
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnViewOptionsShowOverlay();
		return;
	}*/

	for (int i = 0; i < m_pTrack->m_nTotalDocs; i++)
	{
		m_pTrack->m_pDoc[i]->m_bShowOverlay = !m_pTrack->m_pDoc[i]->m_bShowOverlay;
		DrwUpdate(&m_pTrack->m_pDoc[i]->m_ImageViewInfo);
	}
}

void CAppDoc::OnUpdateViewOptionsShowOverlay(CCmdUI *pCmdUI)
{
	/*if(m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateViewOptionsShowOverlay(pCmdUI);
		return;
	}*/

	pCmdUI->SetCheck(m_bShowOverlay);
	pCmdUI->Enable(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nShowOverLay);
}

void CAppDoc::OnUpdateViewOptions(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pApp->m_bOnline);
}

int CAppDoc::ApplyParameters(int nFOV)
{
	//if(m_pTrack->m_nTrack==0 && m_nDocIdx==0) {
		//OutputDebugLogTo(8, TRUE, "Doc Config Folder: %s", m_pTrack->m_strConfigDir);
		//m_InspectionHandler[m_pTrack->m_nFOVIndex].LoadRecipe(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
		//								&m_Buffer,
		//								/*m_pTrack->*/m_strConfigDir,
		//								m_pApp->m_strConfigFile,
		//								m_pTrack->m_nTrack,
		//								m_pTrack->m_nFOVIndex,m_nDocIdx);
	m_InspectionHandler[/*m_pTrack->m_nFOVIndex*/nFOV].LoadInspectParameters(m_strConfigDir, m_pApp->m_strConfigFile, nFOV);
	m_InspectionHandler[/*m_pTrack->m_nFOVIndex*/nFOV].LoadTeachParameters(m_strConfigDir, m_pApp->m_strConfigFile, nFOV);
	//m_InspectionHandler[/*m_pTrack->m_nFOVIndex*/nFOV].LoadCalibrationParameters(m_strConfigDir, m_pApp->m_strConfigFile, nFOV);
//	}
	return 0;
}

BOOL CAppDoc::Navigate(int& nIndex, int nDoc)
{
	while (TRUE) {
		if (WaitResponse(&m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[nDoc]->m_ImageViewInfo))
			return FALSE;
		/*if(m_bPrevStep) {
			if(nIndex == 0)
				continue;
			nIndex-=2;
		}*/
		break;
	}

	return TRUE;
}




void CAppDoc::OnInspectStepDebugSelection()
{
	CStepSelection dlg;
	dlg.DoModal();
}


void CAppDoc::OnUpdateInspectStepDebugSelection(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
	//pCmdUI->Enable((!m_pTrack->m_bLiveImage &&
	//			   !m_pApp->m_bOnline &&
	//			   !m_pApp->m_bTeaching &&
	//			   !m_pApp->m_bInspecting &&
	//			   !m_pApp->m_bTrackInspSeq &&
	//			   !m_pApp->m_bLightCtrlDlgOpen &&
	//			   !m_pTrack->m_bContInsp &&
	//			   !m_bInspSavedImage &&
	//			   !m_pTrack->m_bCamExtTrigEnable &&
	//			   (m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nStepSelection)));// && m_pApp->m_bSuperUserMode);

}

int CAppDoc::LoadImagesToDoc(CString strPath)
{
	int nError = 0, nTrack, nPos = -1, nCount = 0;
	bool bCheckFileName = false;
	if (bCheckFileName) {
		CString strImageName, strImageExt, strImageFinalPath, strPos;
		CString strType[] = { "TOP", "THR" };

		nTrack = m_pTrack->m_nTrack;
		nCount = m_pApp->m_nTotalViewDocs[nTrack];
		nPos = strPath.ReverseFind('_');

		UpdateStatusBar(&CString(""));
		if (nPos > 0) {
			strImageExt = strPath.Mid(strPath.ReverseFind('.'));
			strImageName = strPath.Mid(0, nPos);
			for (int nDoc = 0; nDoc < nCount; nDoc++) {
				strImageFinalPath.Format("%s_%s%s", strImageName, strType[nTrack], strImageExt);
				nError = m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->LoadStaticImage(strImageFinalPath);
				if (nError)
					UpdateStatusBar(&CString("Load Image Error"));

				CRect rectTemp = CRect(CPoint(0, 0), m_pTrack->m_SharedData.pProcBuffers[m_nDocIdx]->GetImgSize());
				ImgCopy(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer, &rectTemp, &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bufPVIBackup, &rectTemp);
			}
		}
		else
			UpdateStatusBar(&CString("Load Image Error"));

		if (nTrack == m_pApp->m_nTotalTracks - 1) {
			m_pTrack->m_charTempPosNameForInsp = '0';
			m_pTrack->m_nFOVIndex = m_pApp->m_pTracks[nTrack - 1].m_nFOVIndex;//0;
		}
		else {

			// Find Fov 		
			int nTempLen, nTempSel;
			CString strRPos, strRPosTemp, strCPos;
			int nRTemp, nCTemp;

			strRPosTemp = strImageName.Mid(strImageName.ReverseFind('R') + 1, 3);
			strRPos = strRPosTemp.Mid(0, strRPosTemp.ReverseFind('_'));
			nRTemp = atoi(strRPos);
			strCPos = strImageName.Mid(strImageName.ReverseFind('C') + 1, 3);
			nCTemp = atoi(strCPos);
			m_pTrack->m_nFOVIndex = (m_pApp->m_nNoOfFovX * (nRTemp - 1) + nCTemp) - 1;

			CString strDevice = strImageName.Mid(strImageName.ReverseFind('A'));
			nPos = strDevice.Find('_');
			strImageName = strDevice.Mid(0, nPos);
			CString StrTempDie = strImageName.Mid(4, nPos);
			nTempSel = atoi(StrTempDie);
			m_pTrack->m_nDieIndex = nTempSel - 1;
		}
	}
	else {
		nError = m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0]->LoadStaticImage(strPath);
		if (nError)
			UpdateStatusBar(&CString("Load Image Error"));

		CRect rectTemp = CRect(CPoint(0, 0), m_pTrack->m_SharedData.pProcBuffers[m_nDocIdx]->GetImgSize());
		ImgCopy(&m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_Buffer, &rectTemp, &m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_bufPVIBackup, &rectTemp);
	}

	return nError;
}


int CAppDoc::LoadImagesToDocForInspection(CString strPath, int nFov)
{
	int nError, nTrack, nPos = -1, nCount = 0;
	CString strImageName, strImageExt, strImageFinalPath, strPos;
	CString strType[] = { "TOP", "THR" };

	nTrack = m_pTrack->m_nTrack;
	nCount = m_pApp->m_nTotalViewDocs[nTrack];
	nPos = strPath.ReverseFind('_');

	UpdateStatusBar(&CString(""));
	if (nPos > 0) {
		strImageExt = strPath.Mid(strPath.ReverseFind('.'));
		strImageName = strPath.Mid(0, nPos);
		for (int nDoc = 0; nDoc < nCount; nDoc++) {
			strImageFinalPath.Format("%s_%s%s", strImageName, strType[nTrack], strImageExt);
			nError = m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->LoadStaticImage(strImageFinalPath);
			if (nError)
				UpdateStatusBar(&CString("Load Image Error"));
			else {
				// Find Fov 		
				int nTempLen, nTempSel;
				CString strRPos, strRPosTemp, strCPos;
				int nRTemp, nCTemp;

				strRPosTemp = strImageName.Mid(strImageName.ReverseFind('R') + 1, 3);
				strRPos = strRPosTemp.Mid(0, strRPosTemp.ReverseFind('_'));
				nRTemp = atoi(strRPos);
				strCPos = strImageName.Mid(strImageName.ReverseFind('C') + 1, 3);
				nCTemp = atoi(strCPos);
				m_pTrack->m_nFOVIndex = (m_pApp->m_nNoOfFovX * (nRTemp - 1) + nCTemp) - 1;

				CString strDevice = strImageName.Mid(strImageName.ReverseFind('A'));
				nPos = strDevice.Find('_');
				strImageName = strDevice.Mid(0, nPos);
				CString StrTempDie = strImageName.Mid(4, nPos);
				nTempSel = atoi(StrTempDie);
				m_pTrack->m_nDieIndex = nTempSel - 1;
			}
			CRect rectTemp = CRect(CPoint(0, 0), m_pTrack->m_SharedData.pProcBuffers[m_nDocIdx]->GetImgSize());
			ImgCopy(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer, &rectTemp, &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bufPVIBackup, &rectTemp);
		}
	}
	else
		UpdateStatusBar(&CString("Load Image Error"));


	return nError;
}

int CAppDoc::LoadMultipleImagesToDoc(CStringArray& strFileNames, int nPosition, int nDevice, BOOL bDisplay)
{
	DrawView(FALSE);
	UpdateStatusBar(&CString(""));

	int nTrack, nDocCount;
	CString strPath, strDevice, strFov, strRPos, strCPos, strSubPath;
	int nfovx = m_pApp->m_nNoOfFovX, nFovY = m_pApp->m_nNoOfFovY;

	CString strPosType, strFolderType[] = { "TOP_STATION","THR_STATION" };
	nTrack = m_pTrack->m_nTrack;
	nDocCount = m_pApp->m_nTotalViewDocs[nTrack];
	//	m_pApp->m_pTracks[nTrack].m_nDieIndex = nDevice;


	int nDieRow, nDieColumn, nDieFOV, nDieNo;
	if (nTrack == STATION_TOP)
	{
		m_pApp->motionParameters.GetDeviceDetailsTOP(nPosition, nDieRow, nDieColumn, nDieNo, nDieFOV);
		m_pApp->m_pTracks[STATION_TOP].m_nFOVIndex = nDieFOV;
	}
	else if (nTrack == STATION_THR) {
		m_pApp->motionParameters.GetDeviceDetailsTHR(nPosition, nDieRow, nDieColumn, nDieNo, nDieFOV);
		m_pApp->m_pTracks[STATION_THR].m_nFOVIndex = nDieFOV;
	}

	if (m_pApp->m_bLoadMultiImagesFromFile) {

		strPath = m_pApp->m_strLoadMultiImagesFolderPath;
		strDevice.Format("%s Doc%02d DieNo%03d", strFolderType[nTrack], m_nDocIdx, nDieNo + 1);
		strPosType.Format("Row%03d Col%03d", nDieRow + 1, nDieColumn + 1);
		strFov.Format("FOV%03d", nDieFOV + 1);
		strSubPath.Format("%s\\%s\\Doc %02d\\", strPath, strFolderType[nTrack], m_nDocIdx);

		for (int nDoc = 0; nDoc < nDocCount; nDoc++)
			m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer.ClrImg();

		int nError = -1, nDevicePos = -1, nPos = -1;
		CString strImgPath, strImgName, strTempName;

		for (int nCount = 0; nCount < strFileNames.GetCount(); nCount++) {
			nDevicePos = strFileNames[nCount].Find(LPCSTR(strDevice));
			nPos = strFileNames[nCount].Find(LPCSTR(strPosType));
			if (nDevicePos > 0 && nPos > 0) {
				strImgPath = strFileNames[nCount];
				strTempName = strSubPath.Mid(0, nDevicePos);
				for (int nDoc = 0; nDoc < nDocCount; nDoc++) {
					strImgName.Format("%s%s %s %s.bmp", strTempName, strDevice, strPosType, strFov);
					if (bDisplay)
					{
						nError = m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->LoadStaticImage(strImgName, bDisplay);
						if (nError) {
							UpdateStatusBar(&CString("Load Image Error"));
						}
						else

							UpdateStatusBar(&CString(""));
					}
					else if (!bDisplay) {
						long cameraGrabId;
						if (nTrack == STATION_TOP)
							m_pApp->motionParameters.GetCameraGrabIDTOP(0, nDieRow, nDieColumn, nDieNo, cameraGrabId);
						else if (nTrack == STATION_THR)
							m_pApp->motionParameters.GetCameraGrabIDTHR(0, nDieRow, nDieColumn, nDieNo, cameraGrabId);

						LoadGrayScaleBMPFile(strImgName, m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab[cameraGrabId]);

					}

				}
				if (!nError) {
					if (nTrack == m_pApp->m_nTotalTracks - 1)
						m_pTrack->m_charTempPosNameForInsp = '0';
					else {
						int nTempLen;
						strRPos = strImgName.Mid(strImgName.ReverseFind('P'), 4);
						nTempLen = strRPos.GetLength();
						m_pTrack->m_charTempPosNameForInsp = strRPos[nTempLen - 1];
					}
					break;
				}
			}
			else
				UpdateStatusBar(&CString("Load Image Error"));
		}
	}
	DrawView(bDisplay);

	return 0;
}


int CAppDoc::GetImageFilePaths(CString strStartFolder, CStringArray& arrayFilePaths)
{
	CString strFilePath;
	strFilePath.Format("%s%s", strStartFolder, "*.*");
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
			GetImageFilePaths(strFolder, arrayFilePaths);
		}
		else if (!strFile.Right(3).CompareNoCase("bmp"))
			arrayFilePaths.Add(strStartFolder + strFile);
	}
	return 0;
}

void CAppDoc::DrawView(BOOL bUpdate)
{
	int nTrack, nCount;

	nTrack = m_pTrack->m_nTrack;
	nCount = m_pApp->m_nTotalViewDocs[nTrack];

	for (int nView = 0; nView < nCount; nView++) {
		if (bUpdate)
			DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nView]->m_ImageViewInfo);
		else
			DrwDel(&m_pApp->m_pTracks[nTrack].m_pDoc[nView]->m_ImageViewInfo);
	}
}

void CAppDoc::DrawView(BOOL bUpdate, int nDocIdx)
{
	int nTrack, nCount;

	nTrack = m_pTrack->m_nTrack;
	nCount = m_pApp->m_nTotalViewDocs[nTrack];
	if (nDocIdx < nCount) {
		if (bUpdate)
			DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDocIdx]->m_ImageViewInfo);
		else
			DrwDel(&m_pApp->m_pTracks[nTrack].m_pDoc[nDocIdx]->m_ImageViewInfo);
	}
}

void CAppDoc::SaveStaticImagePath(CString strPath)
{
	CString strTemp;
	m_pTrack->m_strLoadStaticImgFolderPath = strPath;
	strTemp.Format("Folder: StaticImagesT%02d", m_pTrack->m_nTrack + 1);
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", strTemp, m_pTrack->m_strLoadStaticImgFolderPath);
}

void CAppDoc::OnPviDefectShow()
{
	CRect rect1, rect2;

	m_pApp->m_pWndPVIDefectParameters->m_nTrackId = m_pTrack->m_nTrack;
	m_pApp->m_pWndPVIDefectParameters->m_nDocId = m_nDocIdx;

	m_pApp->m_pWndPVIDefectParameters->Initialize(m_pApp->m_pTracks[m_pTrack->m_nTrack].m_nFOVIndex);

	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rect1, 0);
	m_pApp->m_pWndPVIDefectParameters->GetWindowRect(&rect2);

	m_pApp->m_pWndPVIDefectParameters->SetWindowPos(NULL, rect1.Width() - rect2.Width(), rect1.Height() - rect2.Height() - 20, 0, 0, SWP_NOSIZE);

	m_pApp->m_pWndPVIDefectParameters->ShowWindow(TRUE);
}

void CAppDoc::OnUpdatePviDefectShow(CCmdUI *pCmdUI)
{
	//if (m_pRemoteInspDoc) {
	//	m_pRemoteInspDoc->OnUpdateParmVision(pCmdUI);
	//	return;
	//}
	pCmdUI->Enable(!m_pTrack->m_bContInsp &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_bSavedImagesStep &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		// !bChecked &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nPviDefectParm));
}



void CAppDoc::OnCalibrationSettings()
{
	m_pApp->m_bCalibrationOpen = TRUE;
	m_pApp->m_pCalibrationDlg->nTrack = m_pTrack->m_nTrack;
	//m_pApp->m_pCalibrationDlg->nDoc = this->m_nDocIdx;

	m_pApp->m_pCalibrationDlg->Initialize(m_nDocIdx);

	CRect rect1, rect2;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rect1, 0);
	m_pApp->m_pCalibrationDlg->GetWindowRect(&rect2);

	m_pApp->m_pCalibrationDlg->SetWindowPos(NULL, (rect1.Width() / 2) - (rect2.Width() / 2), (rect1.Height() / 2) - (rect2.Height() / 2), rect2.Width(), rect2.Height(), SWP_NOSIZE);

	m_pApp->m_pCalibrationDlg->ShowWindow(TRUE);


}

void CAppDoc::OnUpdateCalibrationSettings(CCmdUI *pCmdUI)
{
	/*if (m_pRemoteInspDoc) {
		m_pRemoteInspDoc->OnUpdateViewOpticsPanel(pCmdUI);
		return;
	}*/
	pCmdUI->Enable(!m_pTrack->m_bContInsp &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_bSavedImagesStep &&
		!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		!m_pApp->m_bLightCalibration &&
		!m_pApp->m_bDotCalibration &&
		!m_bStoredImages &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nCalibSettings));
}


int CAppDoc::SetLightCalibration()
{
	if (m_bLightCalibration /*&& m_pApp->m_pLightCtrlDlg && m_pApp->m_bLightCtrlDlgOpen*/) {
		m_pApp->m_bLightCalibration = TRUE;
		DrwDel(&m_ImageViewInfo);
		UpdateStatusBar(&CString("Light Calibration Processing..."));

		CImageBuffer bufCalibPosTemp, bufCalibNegTemp;
		bufCalibPosTemp.Allocate(m_Buffer.GetImgSize());
		bufCalibNegTemp.Allocate(m_Buffer.GetImgSize());

		CPerformanceTimer timer;
		int nFOV, nReferenceImgCount, nAvgImgCount, nError, XAxis, YAxis;
		BOOL bFirst, bRefImg, bAvgImg, bLightCorrection;

		nFOV = 0;//// cabliration for first FOV
		nReferenceImgCount = 4;//// for motor movement (small)
		nAvgImgCount = 30;//// for avg image capture loop
		XAxis = 0;
		YAxis = 1;
		bFirst = bRefImg = bAvgImg = bLightCorrection = FALSE;
		BOOL bCapture = FALSE;
		CCalibrationData* pCalibData = &m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0]->m_InspectionHandler[nFOV].m_Inspection.m_CalibrationData;
		m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0]->m_InspectionHandler[nFOV].ClearCalibParam(pCalibData, TRUE);

		for (int nOuterLoop = 0; nOuterLoop < nReferenceImgCount; nOuterLoop++) {
			m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0]->m_InspectionHandler[nFOV].ClearCalibParam(pCalibData, FALSE);

			//// motor movement  - small movement////
			//if (m_pApp->m_pMotionControl) {					//Girish Commented
			//	int nXValue, nYValue;
			//	nXValue = (abs(m_pApp->m_nCalibXAxisPosition) + (2000 * nOuterLoop));
			//	m_pApp->m_pMotionControl->m_nMoveTo[XAxis] = -nXValue;
			//	if (!nOuterLoop) {
			//		nYValue = (abs(m_pApp->m_nCalibYAxisPosition));
			//		m_pApp->m_pMotionControl->m_nMoveTo[YAxis] = -nYValue;
			//		
			//		if(m_pTrack->m_bCamAvail)
			//			nError = m_pApp->m_pMotionControl->LinearMovement(TRUE);
			//	}
			//	else {
			//		if (m_pTrack->m_bCamAvail)
			//			nError = m_pApp->m_pMotionControl->Movement(XAxis, TRUE);
			//	}
			//}

			if (!bCapture) {
				CString strMsg = "Do you Want To Load Image";
				if (AfxMessageBox(strMsg, MB_YESNO) == IDYES) {
					CString strImagePath = LoadImage();
					int nError = LoadGrayScaleBMPFile(strImagePath, m_Buffer);
					bCapture = FALSE;
				}
				else {
					bCapture = TRUE;
				}
			}
			for (int nInnerLoop = 0; nInnerLoop < nAvgImgCount; nInnerLoop++) {
				timer.Reset();
				//if (!m_pTrack->m_bCamAvail || !m_bCameraActive) {
					//int jy = 0;
					//break;

					//CString strTempLoadPath;
					//strTempLoadPath.Format("C:\\Rakshith\\TempDelete\\HP_FCI_%d_%d_Device1_Pos1___T2.bmp", nOuterLoop + 1, nInnerLoop + 1);
					//int nError = LoadGrayScaleBMPFile(strTempLoadPath, m_Buffer);
				//}

//// commented on requirement; Issue List - DT2407 - Issue index 4 ////
				//else {
				//	OnCameraSnap();  //CameraGrab();
				//}
				if (bCapture) {
					OnCameraSnap();
				}
				bRefImg = TRUE;
				bAvgImg = FALSE;
				bFirst = (!nInnerLoop) ? TRUE : FALSE;
				if (nInnerLoop == nAvgImgCount - 1)
					bAvgImg = TRUE;
				if ((nOuterLoop == nReferenceImgCount - 1) && (nInnerLoop == nAvgImgCount - 1)) {
					//bRefImg = bAvgImg = FALSE;
					bLightCorrection = TRUE;
				}

				//OutputDebugLogTo(9, TRUE, "[Light Calibration] GreyValue%d; RefImg%d; AvgImg%d; LightCorrection%d", m_nCalibLightGreyValue, bRefImg, bAvgImg, bLightCorrection);

				m_InspectionHandler[nFOV].m_Inspection.LightCalibrationMode(&m_Buffer, m_nCalibLightGreyValue,
					&bufCalibPosTemp, &bufCalibNegTemp,
					bFirst, bRefImg, bAvgImg, bLightCorrection,
					pCalibData);

				OutputDebugLogTo(6, TRUE, "[Light Calibration] Time = %3.1fms", timer.GetElapsedMSec());
				//				CString strTemp;
				//				strTemp.Format("C:\\Rakshith\\TempDelete\\HP_FCI_%d_%d_Device1_Pos1___T2.bmp", nOuterLoop+1, nInnerLoop+1);
				//				SaveGrayScaleBMPFile(strTemp, m_Buffer);

				bIsFirstLightCalibration = FALSE;
				//if (nCalibRange == 0) {
				//	m_bLightCalibration = FALSE;
				//	if (m_pApp->m_pLightCtrlDlg && m_pApp->m_bLightCtrlDlgOpen)
				//		m_pApp->m_pLightCtrlDlg->OnOK();
				//	OutputDebugLogTo(6, TRUE, "[Light Calibration]sucess", nCalibRange);
				//	UpdateStatusBar(&CString("[Light Calibration]sucess"));
				//	m_pApp->m_bCalib = TRUE;
				//	m_pApp->m_pCalibrationDlg->ShowWindow(TRUE);
				//	break;
				//}
				//else {
				//	//if(m_pApp->m_bAutoLightImageSave)
				//	SaveAutoLightImages();

				//	OutputDebugLogTo(6, TRUE, "[Light Calibration]After LightCalibrationMode Light Range %d Iteration =%d deviation = %3.2f", nCalibRange, m_nIterationCalib, dCalibDeviation);
				//	m_pApp->m_pLightCtrlDlg->SetAutoLightIntensity(nCalibRange, m_nCalibLightGreyValue);
				//}
			}
			//			CString strTemp1;
			//			strTemp1.Format("C:\\Rakshith\\TempDelete\\AvgImage_%d.bmp", nOuterLoop + 1);
			//			SaveGrayScaleBMPFile(strTemp1, bufCalibTemp);
		}
		/*if (m_nIterationCalib == NO_OF_ITERATION) {
			m_bLightCalibration = FALSE;
			if (m_pApp->m_pLightCtrlDlg && m_pApp->m_bLightCtrlDlgOpen)
				m_pApp->m_pLightCtrlDlg->OnCancel();
			UpdateStatusBar(&CString("[Light Calibration]Error"));
			m_pApp->m_bCalib = FALSE;
			m_pApp->m_pCalibrationDlg->ShowWindow(TRUE);
		}*/

		m_bLightCalibration = FALSE;
		m_pApp->m_bCalib = FALSE;

		CString str[] = { "T", "B", "S" };
		CString strCalibPath;

		CRect rectTemp = CRect(CPoint(0, 0), bufCalibPosTemp.GetImgSize());
		ImgCopy(&bufCalibPosTemp, &rectTemp, &m_Buffer, &rectTemp);
		DrwUpdate(&m_ImageViewInfo);

		if (AfxMessageBox("Save Calibrated Data?", MB_YESNO) == IDYES) {
			strCalibPath.Format("%s\\CalibImagePos_%s%d.bmp", m_pApp->m_strCalibDir, str[m_pTrack->m_nTrack], nFOV + 1);
			SaveGrayScaleBMPFile(strCalibPath, bufCalibPosTemp);
			strCalibPath.Format("%s\\CalibImageNeg_%s%d.bmp", m_pApp->m_strCalibDir, str[m_pTrack->m_nTrack], nFOV + 1);
			SaveGrayScaleBMPFile(strCalibPath, bufCalibNegTemp);
			m_InspectionHandler[nFOV].SaveCalibrationParameters(m_pApp->m_strCalibDir, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFOV);
			AfxMessageBox("Calibrated Image Saved.", IDOK);
		}
		else
			AfxMessageBox("Calibration Abort", IDOK);
		//		strCalibPath.Format("C:\\Rakshith\\TempDelete\\ResultCalib.bmp");
		//		SaveGrayScaleBMPFile(strCalibPath, m_Buffer);

		//AfxMessageBox("Calibration Done and Calib Image Saved.", IDOK);
		UpdateStatusBar(&CString("Calibration Done"));

		if (!m_pApp->m_pCalibrationDlg->IsWindowVisible())
			m_pApp->m_pCalibrationDlg->ShowWindow(TRUE);

		m_pApp->m_bLightCalibration = FALSE;
	}

	return 0;
}

void CAppDoc::SaveAutoLightImages()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	CString strImgName, strPath, strFolder;
	double dC1, dC2, dC3, dC4, dC5, dC6, dC7, dC8;

	strPath = m_pApp->m_ImageSaveFolder + "\\Sorted StoredImages";
	_mkdir(strPath);
	strFolder.Format("%s\\AutoLight", strPath);
	_mkdir(strFolder);

	dC1 = m_pApp->m_pLightCtrlDlg->m_nLightPulseChannels[0];
	dC2 = m_pApp->m_pLightCtrlDlg->m_nLightPulseChannels[1];
	dC3 = m_pApp->m_pLightCtrlDlg->m_nLightPulseChannels[2];
	dC4 = m_pApp->m_pLightCtrlDlg->m_nLightPulseChannels[3];
	dC5 = m_pApp->m_pLightCtrlDlg->m_nLightPulseChannels[4];
	dC6 = m_pApp->m_pLightCtrlDlg->m_nLightPulseChannels[5];
	dC7 = m_pApp->m_pLightCtrlDlg->m_nLightPulseChannels[6];
	dC8 = m_pApp->m_pLightCtrlDlg->m_nLightPulseChannels[7];
	strImgName.Format("%02d-%02d-%02d.%03d C1_%.2f C2_%.2f C3_%.2f C4_%.2f C5_%.2f C6_%.2f C7_%.2f C8_%.2f",
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds,
		dC1, dC2, dC3, dC4, dC5, dC6, dC7, dC8);

	SaveGrayScaleBMPFile(strFolder + "\\" + strImgName + ".bmp", m_Buffer);
}


int CAppDoc::SetDotCalibration()
{
	int nError;
	if (m_bDotCalibration) {
		m_pApp->m_bDotCalibration = TRUE;
		BOOL bIsFound;
		int nErr = 0;
		int nFOV = 0;
		double dGreyValueOut;

		DrwDel(&m_ImageViewInfo);
		/*if (m_pApp->m_pMotionControl) {		//Girish commented
			int nXValue, nYValue;
			nXValue = abs(m_pApp->m_nCalibXAxisPosition);
			m_pApp->m_pMotionControl->m_nMoveTo[0] = -nXValue;

			nYValue = (abs(m_pApp->m_nCalibYAxisPosition));
			m_pApp->m_pMotionControl->m_nMoveTo[1] = -nYValue;

			if (m_pTrack->m_bCamAvail)
				nError = m_pApp->m_pMotionControl->LinearMovement(TRUE);
		}*/
		CString strMsg = "Do you Want To Load Image";
		if (AfxMessageBox(strMsg, MB_YESNO) == IDYES) {
			CString strImagePath = LoadImage();
			int nError = LoadGrayScaleBMPFile(strImagePath, m_Buffer);
		}
		else {
			OnCameraSnap();
		}

		UpdateStatusBar(&CString("Dot Calibration Processing..."));

		int nDocFrame = 0;// !m_pTrack->m_nTrack ? m_nDocIdx : (m_pTrack->m_nTrack == 1 ? m_pApp->m_nTop + m_nDocIdx : m_pApp->m_nTop + m_pApp->m_nBottom + m_nDocIdx);
		ZoomSelectedDocView(nDocFrame, TRUE);

		DrwTrackRect(&m_ImageViewInfo, &m_InspectionHandler[nFOV].m_Inspection.m_CalibrationData.rectDotCalib);
		DrwUpdate(&m_ImageViewInfo);
		if (!WaitResponse(&m_ImageViewInfo))
			m_InspectionHandler[nFOV].m_Inspection.m_CalibrationData.rectDotCalib = GetTrackRect(&m_ImageViewInfo, 0);
		else {
			if (!m_pApp->m_pCalibrationDlg->IsWindowVisible())
				m_pApp->m_pCalibrationDlg->ShowWindow(TRUE);

			ZoomSelectedDocView(nDocFrame, FALSE);

			m_pApp->m_bDotCalibration = FALSE;
			UpdateStatusBar(&CString("Dot Calibration Abort"));
			return -1;
		}

		m_InspectionHandler[nFOV].m_Inspection.DotCalibrationMode(&m_pTrack->m_SharedData, &m_Buffer, m_pTrack->m_nTrack, m_nDocIdx);
		DrwUpdate(&m_ImageViewInfo);

		if (AfxMessageBox("Save Dot Calibration Data?", MB_YESNO) == IDYES) {
			/*CString str[] = { "T", "B", "S" };*/
			CString str[] = { "T", "THR" };
			CString strPath;
			m_pApp->m_strCalibDir.Format("%s\\Calibration", m_pApp->m_strMainCfgDir);
			_mkdir(m_pApp->m_strCalibDir);
			strPath.Format("%s\\DotCalibration_Pos%d_%s%d.bmp", m_pApp->m_strCalibDir, /*m_pTrack->m_nFOVIndex+*/1, str[m_pTrack->m_nTrack], m_nDocIdx + 1);
			SaveGrayScaleBMPFile(strPath, m_Buffer);
			m_InspectionHandler[nFOV].SaveDotCalibrationParameters(m_pApp->m_strCalibDir, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFOV);
			m_pTrack->LoadPixelData(m_pApp->m_strCalibDir, m_pApp->m_strConfigFile, m_pTrack->m_nTrack);
			AfxMessageBox("Calibration Data Saved!", IDOK);
		}
		else
			AfxMessageBox("Calibration Abort", IDOK);

		DrwDel(&m_ImageViewInfo);
		UpdateStatusBar(&CString("Dot Calibration Done"));

		bIsFirstLightCalibration = FALSE;
		/*if(nCalibRange==0){
		m_bLightCalibration = FALSE;
		if(m_pApp->m_pLightCtrlDlg && m_pApp->m_bLightCtrlDlgOpen)
		m_pApp->m_pLightCtrlDlg->OnOK();
		OutputDebugLogTo(6,TRUE,"[Light Calibration]sucess",nCalibRange);
		UpdateStatusBar(&CString("[Light Calibration]sucess"));
		m_pApp->m_bCalib = FALSE;
		m_pApp->m_pCalibrationDlg->ShowWindow(TRUE);
		}*/

		ZoomSelectedDocView(nDocFrame, FALSE);

		m_pApp->m_bDotCalibration = FALSE;
	}
	/*if(m_nIterationCalib==100)
	{
	m_nIterationCalib = 0;
	m_bLightCalibration = FALSE;
	if(m_pApp->m_pLightCtrlDlg && m_pApp->m_bLightCtrlDlgOpen)
	m_pApp->m_pLightCtrlDlg->OnCancel();
	UpdateStatusBar(&CString("[Light Calibration]Error"));
	m_pApp->m_bCalib = FALSE;
	m_pApp->m_pCalibrationDlg->ShowWindow(TRUE);
	}*/

	m_bLightCalibration = FALSE;
	m_pApp->m_bCalib = FALSE;

	if (!m_pApp->m_pCalibrationDlg->IsWindowVisible())
		m_pApp->m_pCalibrationDlg->ShowWindow(TRUE);

	return 0;
}



void CAppDoc::OnCameraSnapAll()
{
	m_pApp->SynchronousCameraSnap();
}

void CAppDoc::OnUpdateCameraSnapAll(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;
	for (int i = 0; i < m_pApp->m_nTotalTracks; i++)
	{
		for (int j = 0; j < m_pApp->m_nTotalViewDocs[i]; j++) {
			if (m_pApp->m_pTracks[i].m_pDoc[j]->m_bLiveImageSingle)
				bEnable = TRUE;
		}
	}
	/*pCmdUI->Enable(m_pTrack->m_bCamEnable &&
		m_pTrack->m_bCamAvail &&
		!bEnable &&
		!m_pTrack->m_bLiveImage &&
		!m_pApp->m_bOnline &&
		!m_pApp->m_bTeaching &&
		!m_pApp->m_bInspecting &&
		!m_pTrack->m_bContInsp &&
		!m_pApp->m_bTrackInspSeq &&
		(m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nGrabAll));*/

	pCmdUI->Enable(bEnable);	//Girish, Not in use for now
}



int CAppDoc::InspectQueueBuffers(int nTrack, int nDocTotal, int nFovIndex, int nDieIndex)
{
	CTrainingData* pTrainingDataFirst;
	CInspectinonData* pInspData;
	CCalibrationData* pCalibData;
	int nError = 0;
	CPerformanceTimer timerInspect;
	LOGPEN TextColorRed, TextColorGreen, TextColor, TextColorYellow;
	int nErrInsp[5];
	CStringArray strArrayInspValues;
	strArrayInspValues.RemoveAll();
	BOOL bStepDebug;

	OutputDebugLogTo(8, TRUE, "m_nTrack = %d,nDocTotal = %d,nFovIndex = %d,nDieIndex = %d", nTrack, nDocTotal, nFovIndex, nDieIndex);
	TextColorRed.lopnColor = RGB(230, 50, 50);
	TextColorRed.lopnStyle = PS_SOLID;
	TextColorRed.lopnWidth.x = 1;
	TextColorYellow.lopnColor = RGB(230, 230, 0);
	TextColorYellow.lopnStyle = PS_SOLID;
	TextColorYellow.lopnWidth.x = 1;
	TextColorGreen.lopnColor = RGB(50, 230, 50);
	TextColorGreen.lopnStyle = PS_SOLID;
	TextColorGreen.lopnWidth.x = 1;

	pTrainingDataFirst = &m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[nFovIndex].m_Inspection.m_TrainingData;
	m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[nFovIndex].ClearTeachParam(pTrainingDataFirst);
	pInspData = &m_pApp->m_pTracks[0].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_InspectionData;
	/*pCalibData = &m_pApp->m_pTracks[0].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;*/
	pCalibData = &m_pApp->m_pTracks[nTrack].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;

	if (m_pApp->m_bPixelRuler)
	{
		m_pApp->m_pPixelRulerDlg->ShowWindow(SW_HIDE);
		m_pApp->m_bPixelRuler = FALSE;
	}
	if (m_lDebugFlag & DEBUG_STEP_MODE)
		bStepDebug = 1;

	DrawView(FALSE);
	timerInspect.Reset();
	if (m_bBinaryMode) {
		for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++)
			SwitchImg(m_pApp->m_pTracks[nTrack].m_SharedData.pImageViews[nDoc], FALSE);
	}

	m_pTrack->m_bOpticsRoi = FALSE;
	if (m_pApp->m_RuntimeDisplayEnable) {
		for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
			//DrwDel(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);				
			m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bShowOverlay = TRUE;
		}
	}

	//Get total FOV for current station
	int nCurrentFOV = m_pApp->m_nNoOfFov;
	if (m_pTrack->m_nTrack == STATION_TOP) {
		nCurrentFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	}
	else if (m_pTrack->m_nTrack == STATION_THR) {
		nCurrentFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();
	}

	if (!nError) {
		for (int nInsp = 0; nInsp < 2; nInsp++) {	//// 0-Device Loc Insp; 1-Rest All Insp [Region, Encap, Die, ...]; ////
			for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {

				if ((nFovIndex == m_pApp->m_nNoOfFov - 1) && !nTrack && !nDoc && !nInsp) {
					m_pApp->m_pTracks[/*nTrack*/0].m_pDoc[/*nDoc*/0]->m_InspectionHandler[/*m_pTrack->m_nFOVIndex*/0].ClearInspParam(pInspData, FALSE);
					m_pApp->m_pTracks[/*nTrack*/0].m_pDoc[/*nDoc*/0]->m_InspectionHandler[/*m_pTrack->m_nFOVIndex*/0].ClearCalibParam(pCalibData, FALSE, TRUE);
				}

				if (!nDoc && !nInsp) {
					m_pApp->m_pTracks[/*nTrack*/0].m_pDoc[/*nDoc*/0]->m_InspectionHandler[/*m_pTrack->m_nFOVIndex*/0].ClearInspParam(pInspData, TRUE);
				}

				CPerformanceTimer timer;
				CInspectionHandler* pInspHandler = &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFovIndex];
				nErrInsp[nDoc] = pInspHandler->Inspect(&m_pTrack->m_SharedData,
					m_pTrack->m_SharedData.pProcBuffers[nDoc],
					&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_TeachBuffer[nFovIndex],
					m_lDebugFlag,
					!nInsp ? FALSE : TRUE,
					nTrack,
					nDoc,
					m_pApp->m_nTotalViewDocs[nTrack],
					nFovIndex + 1,
					/*m_pApp->m_nNoOfFov*/nCurrentFOV,
					pTrainingDataFirst,
					pInspData,
					pCalibData,
					strArrayInspValues);

				double dTime = timer.GetElapsedMSec();
				/*OutputDebugLogTo(nTrack + 1, TRUE, "[Image%d] RESULT: '%s'  (%3.2fms)", nDoc + 1, pInspHandler->m_strErrorMsg, dTime);*/

				SetInspectionTime(m_pApp->m_pTracks[nTrack].m_SharedData.pImageViews[nDoc], dTime);
				//DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
			/*	m_pApp->m_strLogInspectionData.RemoveAll();
				m_pApp->m_strLogInspectionData.Copy(strArrayInspValues);
				SetEvent(m_pApp->m_EventLogInspectionData);*/
			}
		}
	}
	else {
		//UpdateStatusBar(&CString("Image Snap Failure"),2);
		//UpdateStatusBar(&CString("Fail Image Snap"),3);
		UpdateStatusBar(&CString("Inspection Error"), 3);
	}
	CString strDefectMsg;
	for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
		if (m_pApp->m_RuntimeDisplayEnable) {
			if (m_pApp->m_pWndPVIDefectParameters) {
				int nDocFrame = 0;// !nTrack ? nDoc : (nTrack == 1 ? m_pApp->m_nTop + nDoc : m_pApp->m_nTop + m_pApp->m_nBottom + nDoc);
				if (m_pApp->m_pWndPVIDefectParameters->IsWindowVisible() && m_pApp->m_pWndPVIDefectParameters->m_bPVIDefectImgEnable[nFovIndex][nDocFrame] /*&& nErrInsp[nDoc]==-PVI_INSP*/) {
					m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFovIndex].m_Inspection.DisplayPVIDefectImage(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo,
						&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer,
						m_pApp->m_pWndPVIDefectParameters->m_ComboPviArea.GetCurSel());
				}
			}
			if (nErrInsp[nDoc] < GOOD_UNIT) {
				if (nErrInsp[nDoc] > -ERROR_CODES || nErrInsp[nDoc] == -PROCESS_ERROR) {
					strDefectMsg.Format("%s ", m_pApp->GetFailedStr(nErrInsp[nDoc], FALSE));
					DrwStr(m_pApp->m_pTracks[nTrack].m_SharedData.pImageViews[nDoc], &strDefectMsg, &CPoint(240, 20), &TextColorRed, 80);
				}
				else {
					CString strTemp("PVI Defect");
					int nErrorFind = nErrInsp[nDoc];
					if (m_pApp->m_pTracks[nTrack].m_mapPviDefectsStatsGeneral.find(nErrorFind) != m_pApp->m_pTracks[nTrack].m_mapPviDefectsStatsGeneral.end()) {
						strTemp.Format("PVI_%s", m_pApp->m_pTracks[nTrack].m_mapPviDefectsStatsGeneral[nErrInsp[nDoc]]);
					}
					DrwStr(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo, &strTemp, &CPoint(240, 20), &TextColorRed, 80);
				}

			}
			else {
				strDefectMsg = "PASS";
				DrwStr(m_pApp->m_pTracks[nTrack].m_SharedData.pImageViews[nDoc], &strDefectMsg, &CPoint(240, 20), &TextColorGreen, 80);
			}

			CRect rect = CRect(CPoint(80, 80), CSize(60, 60));
			BOOL bFill = m_nInspCounter % 2;
			if (bFill)
				rect.DeflateRect(6, 6);
			else
				TextColorYellow.lopnWidth.x = 10;
			DrwRect(m_pApp->m_pTracks[nTrack].m_SharedData.pImageViews[nDoc], &rect, &TextColorYellow/*, bFill*/);
			//DrwUpdate(&m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
		}
		Sleep(m_pApp->m_nInSpectionDelay);
	}

	nError = GOOD_UNIT;
	for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
		if (m_pApp->m_pWndPVIDefectParameters) {
			if (m_pApp->m_pWndPVIDefectParameters->IsWindowVisible() &&
				m_pApp->m_pWndPVIDefectParameters->m_bPVIDefectImgEnable[nFovIndex][nDoc] //&&
				/*nErrInsp[nDoc] == -PVI_INSP*/)
				continue;
		}
		CInspectionHandler* pInspHandler = &m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFovIndex];
		for (int j = 0; j < pInspHandler->m_Inspection.m_arrayOverlayInspection.GetCount(); j++) {
			if (!pInspHandler->m_Inspection.m_arrayOverlayInspection[j].hImage.IsInitialized())
				DrawRegion(m_pApp->m_pTracks[nTrack].m_SharedData.pImageViews[nDoc], pInspHandler->m_Inspection.m_arrayOverlayInspection[j]);
		}
		if (nError == GOOD_UNIT) {
			if (nErrInsp[nDoc] < GOOD_UNIT)
				nError = nErrInsp[nDoc];
		}
		//nErrInsp[nDoc] = -11111;
		int nView = 0;// !nTrack ? nDoc : (nTrack == 1 ? m_pApp->m_nTop + nDoc : m_pApp->m_nTop + m_pApp->m_nBottom + nDoc);
		if (nErrInsp[nDoc] == -NO_IMAGE)
			m_pApp->m_nErrorCode[nDieIndex][nFovIndex][nView] = 0;
		else
			m_pApp->m_nErrorCode[nDieIndex][nFovIndex][nView] = nErrInsp[nDoc];

		if (!m_pApp->m_bOnline || m_pApp->m_bDisplayAllStatsOffline) {
			m_pApp->UpdateLotSummary(nDieIndex, nFovIndex, nView, nErrInsp[nDoc], !m_pApp->m_bOnline);
		}
	}

	if (!m_pApp->m_bOnline || m_pApp->m_bDisplayAllStatsOffline)
		m_pApp->UpdateStatisticsWindow();
	DrawView(TRUE);

	return nError;
}



int CAppDoc::SaveTeachParmAutoTeach(int nFOV)
{
	m_strConfigDir.Format("%s\\Doc%d", m_pTrack->m_strConfigDir, m_nDocIdx + 1);

	_mkdir(m_strConfigDir);

	CCalibrationData* pCalibData;

	m_pTrack->m_nFOVIndex = nFOV;

	m_pApp->m_LightHandler.LoadLightSettings(m_nDocIdx, m_pTrack->m_nTrack);

	if (!m_nDocIdx) {
		CString strCalibFolder;// = m_pApp->m_strCalibDir;
		strCalibFolder.Format("%s\\Calibration", m_pApp->m_strMainCfgDir);
		m_InspectionHandler[nFOV].LoadCalibrationParameters(strCalibFolder, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFOV);
		m_InspectionHandler[nFOV].LoadDotCalibrationParameters(strCalibFolder, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFOV);
	}

	pCalibData = &m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;

	m_InspectionHandler[nFOV].ClearTeachParam(NULL);
	m_InspectionHandler[nFOV].LoadTeachParameters(m_strConfigDir, m_pApp->m_strConfigFile, nFOV);

	//Get total FOV for current station
	int nCurrentFOV = m_pApp->m_nNoOfFov;
	if (m_pTrack->m_nTrack == STATION_TOP) {
		nCurrentFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	}
	else if (m_pTrack->m_nTrack == STATION_THR) {
		nCurrentFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();
	}

	int nError, nTeachStep = 2;//// 1-Device Loc & Dattam; 2-Region & Mask;
	CString strImgFullPath;
	strImgFullPath.Format("%s\\TeachTemplate_Pos%d.bmp", m_strConfigDir, nFOV + 1);
	nError = LoadGrayScaleBMPFile(strImgFullPath, m_TeachBuffer[nFOV]);
	if (!nError) {
		for (int i = 0; i < nTeachStep; i++) {
			m_InspectionHandler[nFOV].AutoTeach(&m_pTrack->m_SharedData,
				&m_TeachBuffer[nFOV],
				m_strConfigDir, m_pApp->m_strConfigFile,
				!i ? FALSE : TRUE,
				m_pTrack->m_nTrack,
				m_nDocIdx,
				nFOV + 1,
				nCurrentFOV,
				pCalibData);
		}
	}
	else {
		OutputDebugLogTo(1, TRUE, "AutoTeach Load Image Error [Teach Image Not Avaiable]");
		OutputDebugLogTo(9, TRUE, "CAppDoc::LoadConfiguration AutoTeach Image Error [Track%d FOV%d Doc%d]",
			m_pTrack->m_nTrack + 1, nFOV + 1, m_nDocIdx + 1);
		CString str;
		str.Format("Teach Image Not Avaiable for Track%d Doc%d Position %d", m_pTrack->m_nTrack + 1, m_nDocIdx + 1, nFOV + 1);
		m_pApp->AddLogEvent(str);

	}

	m_lDebugFlag = m_lDebugFlag | DEBUG_DRAW | DEBUG_PRINT | DEBUG_TIME;

	DrawView(FALSE);
	DrawView();

	CString str;
	str.Format("AutoTeach Process Completed");
	m_pApp->AddLogEvent(str);

	return 0;

}

const CInspectionHandler & CAppDoc::inspection(unsigned char id) const
{
	return m_InspectionHandler[id];
}

CInspectionHandler & CAppDoc::inspection(unsigned char id)
{
	return m_InspectionHandler[id];
}

CInspectionHandler & CAppDoc::inspectionOnline(unsigned char threadId, unsigned char id)
{
	return m_InspectionHandlerOnline[threadId][id];
}

void CAppDoc::DuplicateInspectionOnline(int nThreadId)
{
	m_strConfigDir.Format("%s\\Doc%d", m_pTrack->m_strConfigDir, m_nDocIdx + 1);
	CCalibrationData* pCalibData;

	if (m_pTrack->m_nTrack == STATION_TOP)
		m_nNoofFov = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	else if (m_pTrack->m_nTrack == STATION_THR)
		m_nNoofFov = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

	if (m_nNoofFov > MAX_FOV) {
		m_nNoofFov = MAX_FOV;
	}

	for (int nFOV = 0; nFOV < m_nNoofFov; nFOV++) {
		m_InspectionHandlerOnline[nThreadId][nFOV].CleanUpTeach();
		m_InspectionHandlerOnline[nThreadId][nFOV].CleanUp();
	}

	//Get total FOV for current station
	pCalibData = &m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;

	CString strCalibFolder;// = m_pApp->m_strCalibDir;
	strCalibFolder.Format("%s\\Calibration", m_pApp->m_strMainCfgDir);
	for (int nFOV = 0; nFOV < m_nNoofFov; nFOV++) {
		if (!m_nDocIdx) {
			m_InspectionHandlerOnline[nThreadId][nFOV].LoadCalibrationParameters(strCalibFolder, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFOV);
			m_InspectionHandlerOnline[nThreadId][nFOV].LoadDotCalibrationParameters(strCalibFolder, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFOV);
		}

		m_InspectionHandlerOnline[nThreadId][nFOV].CreateParameterList(m_pTrack->m_nTrack, m_nDocIdx, nFOV);
		m_InspectionHandlerOnline[nThreadId][nFOV].CreateTeachParmList(m_pTrack->m_nTrack);
		//// Clear teach param variables before reading values from recipe file ////
		m_InspectionHandlerOnline[nThreadId][nFOV].ClearTeachParam(NULL);
		m_InspectionHandlerOnline[nThreadId][nFOV].LoadRecipe(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
			&m_Buffer,
			m_strConfigDir,
			m_pApp->m_strConfigFile,
			m_pTrack->m_nTrack,
			m_pTrack->m_nTotalDocs,
			nFOV, m_nDocIdx);

		int nError, nTeachStep = 2;//// 1-Device Loc & Dattam; 2-Region & Mask;
		for (int i = 0; i < nTeachStep; i++) {
			m_InspectionHandlerOnline[nThreadId][nFOV].AutoTeach(&m_pTrack->m_SharedData,
				&m_TeachBuffer[nFOV],
				m_strConfigDir, m_pApp->m_strConfigFile,
				!i ? FALSE : TRUE,
				m_pTrack->m_nTrack,
				m_nDocIdx,
				nFOV + 1,
				m_nNoofFov,
				pCalibData);
		}
	}
}

void CAppDoc::DuplicateInspectionOnline()
{
	m_strConfigDir.Format("%s\\Doc%d", m_pTrack->m_strConfigDir, m_nDocIdx + 1);
	CCalibrationData* pCalibData;

	int nFOVIndexBk = m_pTrack->m_nFOVIndex;

	if (m_pTrack->m_nTrack == STATION_TOP)
		m_nNoofFov = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	else if (m_pTrack->m_nTrack == STATION_THR)
		m_nNoofFov = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

	if (m_nNoofFov > MAX_FOV) {
		m_nNoofFov = MAX_FOV;
	}
	//Get total FOV for current station
	pCalibData = &m_pApp->m_pTracks[m_pTrack->m_nTrack].m_pDoc[0]->m_InspectionHandler[0].m_Inspection.m_CalibrationData;

	CString strCalibFolder;// = m_pApp->m_strCalibDir;
	strCalibFolder.Format("%s\\Calibration", m_pApp->m_strMainCfgDir);
	for (int nFOV = 0; nFOV < m_nNoofFov; nFOV++) {
		m_pTrack->m_nFOVIndex = nFOV;

		if (!m_nDocIdx) {
			for (int nThread = 0; nThread < STATION_THR_THREAD_COUNT; nThread++) {
				m_InspectionHandlerOnline[nThread][nFOV].LoadCalibrationParameters(strCalibFolder, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFOV);
				m_InspectionHandlerOnline[nThread][nFOV].LoadDotCalibrationParameters(strCalibFolder, m_pApp->m_strConfigFile, m_pTrack->m_nTrack, nFOV);
			}
		}

		for (int nThread = 0; nThread < STATION_THR_THREAD_COUNT; nThread++) {
			//// Clear teach param variables before reading values from recipe file ////
			m_InspectionHandlerOnline[nThread][nFOV].ClearTeachParam(NULL);
			m_InspectionHandlerOnline[nThread][nFOV].LoadRecipe(&m_pTrack->m_SharedData,//&m_ImageViewInfo,
				&m_Buffer,
				m_strConfigDir,
				m_pApp->m_strConfigFile,
				m_pTrack->m_nTrack,
				m_pTrack->m_nTotalDocs,
				nFOV, m_nDocIdx);
		}

		int nError, nTeachStep = 2;//// 1-Device Loc & Dattam; 2-Region & Mask;
		for (int i = 0; i < nTeachStep; i++) {
			for (int nThread = 0; nThread < STATION_THR_THREAD_COUNT; nThread++) {
				m_InspectionHandlerOnline[nThread][nFOV].AutoTeach(&m_pTrack->m_SharedData,
					&m_TeachBuffer[nFOV],
					m_strConfigDir, m_pApp->m_strConfigFile,
					!i ? FALSE : TRUE,
					m_pTrack->m_nTrack,
					m_nDocIdx,
					nFOV + 1,
					m_nNoofFov,
					pCalibData);
			}
		}
	}
	m_pTrack->m_nFOVIndex = nFOVIndexBk;
}

int CAppDoc::EnableInspection(int nFovID, int nDieID, bool & bStatus)
{
	if (nFovID >= m_nNoofFov || nFovID < 0)
		return -1;

	bStatus = m_nEnableInspection[nFovID];

	return 0;
}

void CAppDoc::OnButtonSampling()
{

	int nBlockNoPerX = 0;
	int nBlockNoPerY = 0;
	int nDieNoPerX = 0;
	int nDieNoPerY = 0;

	if (theApp.m_pSamplingDlg == NULL) {
		theApp.m_pSamplingDlg = new CSampling();
		if (!theApp.m_pSamplingDlg->Create(IDD_DIALOG_SAMPLING, AfxGetMainWnd())) {
			theApp.m_pSamplingDlg = NULL;
			return;
		}
	}

	theApp.m_pSamplingDlg->SetIntensityID(m_pTrack->m_nTrack, m_nDocIdx);


	theApp.motionParameters.GetMappingParameters(nBlockNoPerX, nBlockNoPerY, nDieNoPerX, nDieNoPerY);

	if (m_pTrack->m_nTrack == STATION_TOP) {
		nDieNoPerX = nDieNoPerX / 4;
		nDieNoPerY = nDieNoPerY / 3;
	}

	theApp.m_pSamplingDlg->CreateTemplate(nBlockNoPerX, nBlockNoPerY, nDieNoPerX, nDieNoPerY);
	theApp.m_pSamplingDlg->SetDieStates(m_nEnableInspection);

	theApp.m_pSamplingDlg->ShowWindow(TRUE);
}

void CAppDoc::OnUpdateButtonSampling(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pApp->m_bOnline && (m_pApp->m_UserAccInfo.m_nCurrentAccessLevel >= m_pApp->m_AdvSecurityDlg.m_data.m_nScanningOptions));
}