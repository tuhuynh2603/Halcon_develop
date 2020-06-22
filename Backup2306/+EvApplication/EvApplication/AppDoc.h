/// AppDoc.h : interface of the CAppDoc class
/// Udupa; April'2012

#pragma once

#include "EvImageDoc.h"
#include "OpticsDialog.h"
#include "OpticsDlg.h"
#include "InspectionSettings.h"
#define NO_OF_ITERATION 100

class CConvertRawY8BGGR;



class CAppDoc : public CEvImageDoc
{
protected: // create from serialization only
	CAppDoc();
	DECLARE_DYNCREATE(CAppDoc)

// Attributes
public:
	CApplication* m_pApp;
	int m_nDocIdx;
	int m_nDefectCount;
	CImageBuffer m_Buffer;
	BOOL m_bIsColor;
	COpticsDialog* m_pOpticsDialog;
	CFailImageStack m_FailStack[25];
	//CAppDoc* m_pRemoteInspDoc;
	

	CString m_strLoadImage;
	//CInspectionHandler m_InspectionHandler;
	CTrackHandler* m_pTrack;
	
	CString m_strConfigDir;
	int m_nInspResult;
	int m_nInspCounter;
	BOOL m_bInspecting;

	std::vector< int > m_nFovCompletedTeach;
	BOOL m_bTeachSingleImage;
	BOOL m_bInspSavedImage;
	BOOL m_bSavedImagesStep;
	BOOL m_bInspectionDebug;
	BOOL m_bStoredImages;
	BOOL m_bInspectCycle;
	BOOL m_bCycleAutoStepMode;
	BOOL m_bCameraActive;
	BOOL m_bLightCalibration;
	BOOL m_bDotCalibration;
	BOOL m_bSetupGrid;
	BOOL bIsFirstLightCalibration;
	int m_nCalibLightGreyValue;
	int m_CalibTolerence;
	int nCalibRange;
	double dCalibDeviation;
	int m_nIterationCalib;
	HANDLE m_EventInspEnd;
	BOOL m_bLiveImageSingle;
	CInspectionSettings m_pInspectionSetting;

	std::vector < std::queue < long  >>grabbedImageId;
	std::vector < CImageBuffer > imageGrab;

	//--. For scanning
	volatile long nCameraGrabNo;
	//volatile long nCameraDummyGrabNo;
	int nTotalGrabNo;

	std::vector < int > m_nEnableInspection;

// Operations
public:
	BOOL SetParameters(CTrackHandler* pTrack, int nDoc);
	void InitializeDocView();

	CConvertRawY8BGGR* m_pColorConvertor;
	int CopyGrabbedBuffer(CImageBuffer* pGrabbedBuffer);
	int CameraGrab(BOOL bDisplay = TRUE, BOOL bSilent = FALSE);
	void CameraLive();
	void CameraLiveCalibration();
	int LoadConfiguration();
	int LoadConfiguration(int nFov);
	
	int LoadSamplingParameters();
	void SaveSamplingParameters();

	int SetSamplingStatus(const int *pEnableInspection, int nTotalFov);
	int GetSamplingStatus(int *pEnableInspection, int nTotalFov);

	int Teach();
	void DrawTrackStatus(BOOL bActive);
	int Inspect(int nError);
	int ContinuousInspect();
	int SaveOfflineImages(int nError,CString strImagePath,CString strImageName, CString strFolderName);

	
	void SetImageTitle(CString* str, BOOL bRefresh=TRUE);

	int ResizeDoc(CSize csSize);

	void SaveAllGrabbedImages(int nTrackIndex);
	void SaveDocImage(int nTrack,int nDoc,int nDie,int nFov, int nErrorCode);
	void SaveInspSeqImages(int nTrack,int nDoc,int nDie,int nFov);

	void SetOpticsCharacteristics(BOOL bForceCalculation = FALSE);
	void DrawOpticsROIs(BOOL bDrawRect);

	void ZoomView(int nMode);

	void SaveDebugFlags();
	void InspectStoredImages(BOOL bStep);
	int LoadStaticImage(CString str="", BOOL bDisplay=TRUE, BOOL bSameBitCount=FALSE);
	int ApplyParameters(int nFOV);
	BOOL Navigate(int& nIndex,int nDoc);

	//HANDLE m_EventImageUpdate;
	//HANDLE m_hTrackImageUpdate;
	HANDLE m_EventLogInspectionData;
	BOOL m_bExitLogEnry;
	CStringArray m_strLogInspectionData;

	void UpdateImages(CString *strZoneDefect,CString strResult);
	CImageBuffer** bufImageUpdate;
	CString* m_strZoneDefect;
	CString m_strResult;

	//CImageBuffer m_DisplayBuffer;
	
	int LoadImagesToDoc(CString strPath);
	int LoadImagesToDocForInspection(CString strPath, int nFov);
	int LoadMultipleImagesToDoc(CStringArray& strFileNames, int nPosition=0, int nDevice=0,BOOL bDisplay=TRUE);
	void DrawView(BOOL bUpdate=TRUE);
	void DrawView(BOOL bUpdate, int nDocIdx);
	int CameraGrabUsingExposure(BOOL bDisplay = TRUE, BOOL bSilent = FALSE,BOOL bMoveSequence = FALSE);
	void SaveStaticImages(BOOL bTeachImg = FALSE);
	//int ReverseFind(LPCTSTR text, LPCTSTR toFind);
	void SaveStaticImagePath(CString strPath);
	void ZoomSelectedDocView(int nDoc, BOOL bZoom=FALSE);
	int SetLightCalibration();
	//void SetLightCalibrationEvent();
	void SaveAutoLightImages();
	int  SetDotCalibration();
	//void SetLightDialog();

	CInspectionHandler m_InspectionHandler[MAX_FOV];
	CInspectionHandler m_InspectionHandlerOnline[STATION_THR_THREAD_COUNT][MAX_FOV];
	int m_nZAxisPosition[/*MAX_FOV*/8];
	int InspectAllDoc(int nError);
	int InspectAllDocOnline(int nDieFOV, int nDocumentId, int nScannedImageID);
	int m_nNoofFov;
	int InspectQueueBuffers(int nTrack, int nDoc, int nFovIndex, int ndieIndex);

	CImageBuffer m_TeachBuffer[MAX_FOV];
	int UpdateImageToOriginal();
	CImageBuffer m_bufPVIBackup;
	int CameraGrabSingle(BOOL bDisplay = TRUE, BOOL bSilent = FALSE);
	void CamLiveSingle();
	HANDLE m_EventKillLiveSingle;
	int GetImageFilePaths(CString strStartFolder, CStringArray& arrayFilePaths);
	int SaveTeachParmAutoTeach(int nFov);
	const CInspectionHandler & inspection(unsigned char id = 0) const;
	CInspectionHandler & inspection(unsigned char id = 0);
	CInspectionHandler & inspectionOnline(unsigned char threadId = 0, unsigned char id = 0);


	void DuplicateInspectionOnline();
	void DuplicateInspectionOnline(int nThreadId);

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CAppDoc();
	virtual void OnCloseDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPoint m_InspTickPos1[4], m_InspTickPos2[4];

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnInspectionInspect();
	afx_msg void OnUpdateInspectionInspect(CCmdUI *pCmdUI);
	afx_msg void OnToolsStaticimageLoadFromFile();
	afx_msg void OnUpdateToolsStaticimageLoadFromFile(CCmdUI *pCmdUI);
	afx_msg void OnToolsStaticimageLoadTeachImage();
	afx_msg void OnUpdateToolsStaticimageLoadTeachImage(CCmdUI *pCmdUI);
	afx_msg void OnToolsStaticimageSave();
	afx_msg void OnUpdateToolsStaticimageSave(CCmdUI *pCmdUI);
	afx_msg void OnToolsStaticimage();
	afx_msg void OnUpdateToolsStaticimage(CCmdUI *pCmdUI);
	afx_msg void OnInspectionTeach();
	afx_msg void OnUpdateInspectionTeach(CCmdUI *pCmdUI);
	afx_msg void OnInspectionTeachSingleImage();
	afx_msg void OnUpdateInspectionTeachSingleImage(CCmdUI *pCmdUI);
	afx_msg void OnInspectionDebug();
	afx_msg void OnUpdateInspectionDebug(CCmdUI *pCmdUI);
	afx_msg void OnUpdateInspectionNext(CCmdUI *pCmdUI);
	afx_msg void OnInspectionNext();
	afx_msg void OnInspectionAbort();
	afx_msg void OnUpdateInspectionAbort(CCmdUI *pCmdUI);
	afx_msg void OnCameraSnap();
	afx_msg void OnUpdateCameraSnap(CCmdUI *pCmdUI);
	afx_msg void OnCameraStream();
	afx_msg void OnUpdateCameraStream(CCmdUI *pCmdUI);
	afx_msg void OnCameraLight();
	afx_msg void OnUpdateCameraLight(CCmdUI *pCmdUI);
	afx_msg void OnParmVision();
	afx_msg void OnUpdateParmVision(CCmdUI *pCmdUI);
	afx_msg void OnInspectionStoredimages();
	afx_msg void OnUpdateInspectionStoredimages(CCmdUI *pCmdUI);
	afx_msg void OnInspectionCycle();
	afx_msg void OnUpdateInspectionCycle(CCmdUI *pCmdUI);
	afx_msg void OnInspectionCameraactive();
	afx_msg void OnUpdateInspectionCameraactive(CCmdUI *pCmdUI);
	afx_msg void OnInspectCycleAutoStepMode();
	afx_msg void OnUpdateInspectCycleAutoStepMode(CCmdUI *pCmdUI);
	afx_msg void OnInspectStoredImagesPath();
	afx_msg void OnUpdateInspectStoredImagesPath(CCmdUI *pCmdUI);
	afx_msg void OnInspectSavePassImages();
	afx_msg void OnUpdateInspectSavePassImages(CCmdUI *pCmdUI);
	afx_msg void OnInspectSaveFailImages();
	afx_msg void OnUpdateInspectSaveFailImages(CCmdUI *pCmdUI);
	afx_msg void OnCameraParmButton();
	afx_msg void OnUpdateCameraParmButton(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewOptions(CCmdUI *pCmdUI);
	afx_msg void OnViewOptionsSetupGrid();
	afx_msg void OnUpdateViewOptionsSetupGrid(CCmdUI *pCmdUI);
	afx_msg void OnViewOptionsShowOverlay();
	afx_msg void OnUpdateViewOptionsShowOverlay(CCmdUI *pCmdUI);
	afx_msg void OnViewOpticsRoi();
	afx_msg void OnUpdateViewOpticsRoi(CCmdUI* pCmdUI);
	afx_msg void OnViewOpticsPanel();
	afx_msg void OnUpdateViewOpticsPanel(CCmdUI* pCmdUI);
	afx_msg void OnViewSelectGoodB();
	afx_msg void OnUpdateViewSelectGoodB(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeFramecountCombo();

	afx_msg void OnInspectStepDebugSelection();
	afx_msg void OnUpdateInspectStepDebugSelection(CCmdUI* pCmdUI);

	afx_msg void OnParmTeach();
	afx_msg void OnUpdateParmTeach(CCmdUI *pCmdUI);

	afx_msg void OnParmDefect();
	afx_msg void OnUpdateParmDefect(CCmdUI *pCmdUI);

	afx_msg void OnPviDefectShow();
	afx_msg void OnUpdatePviDefectShow(CCmdUI *pCmdUI);

	afx_msg void OnCalibrationSettings();
	afx_msg void OnUpdateCalibrationSettings(CCmdUI *pCmdUI);

	afx_msg void OnToolsStaticimageLoadTeachImageSingle();
	afx_msg void OnUpdateToolsStaticimageLoadTeachImageSingle(CCmdUI *pCmdUI);

	afx_msg void OnToolsStaticimageLoadAllFromFile();
	afx_msg void OnUpdateToolsStaticimageLoadAllFromFile(CCmdUI *pCmdUI);
	//afx_msg void OnToolsStaticimageSaveAll();
	//afx_msg void OnUpdateToolsStaticimageSaveAll(CCmdUI *pCmdUI);
	afx_msg void OnInspectionInspectAll();
	afx_msg void OnUpdateInspectionInspectAll(CCmdUI *pCmdUI);

	afx_msg void OnCameraSnapAll();
	afx_msg void OnUpdateCameraSnapAll(CCmdUI *pCmdUI);


	afx_msg void OnButtonSampling();
	afx_msg void OnUpdateButtonSampling(CCmdUI *pCmdUI);;
	int EnableInspection(int nFovID, int nDieID, bool &bStatus);
};
