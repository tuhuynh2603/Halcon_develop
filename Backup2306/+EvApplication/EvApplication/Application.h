/// Application.h : main header file for the Ev_InCuvette application
/// Udupa; April'2012

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#define MAX_VIEWS			5
#define MAX_TRACKS			2
#define AXES_COUNT			3
#define MAX_DIRECTION		2
#define MAX_FOV				10
#define MAX_DIES			60
#define STATION_TOP_THREAD_COUNT 6
#define STATION_THR_THREAD_COUNT 6

#define PROCESS_STATE_IDLE	0
#define PROCESS_STATE_SETUP	1
#define PROCESS_STATE_READY	2
#define PROCESS_STATE_EXECUTING 3
#define PROCESS_STATE_PAUSE	4
#define PROCESS_STATE_STOP	5

#include "MessageBox.h"
#include "InspectionHandler.h"
#include "ApplicationDef.h"
#include "Hardware.h"
#include "ThreadManager.h"
#include "LotStruct.h"
#include "UserLoginAccount.h"
#include "SystemLogger.h"
#include "IO_Interface.h"
#include "Motor_Interface.h"
#include "AdvSecurityDlg.h"
#include "LightHandler.h"
#include "CommInterface.h"
#include "MessageDlg.h"
#include "PixelRuler.h"
#include "InfoMsgDlg.h"
#include "DisplayOnlinePanel.h"
#include "TriangularSensor.h"
#include "MotionControlDialog.h"
#include "MotionParameters.h"
#include "IOTestDlg.h"
#include "IOTestStatusDialog.h"
#include "IOParam.h"
#include "BarcodeReader.h"
#include "QRBarcodeReader.h"
#include "JigLeveling.h"
#include "GlobalBackgroundThread.h"
#include "operatorOverloading.h"
#include "MotorTriggerTestDialog.h"
#include "ErrorDlg.h"
#include "SequenceOptions.h"
#include "ProgressDlg.h"
#include "VerificationMode.h"
#include "DistanceCalibration.h"
#include <queue>
#include <map>
#include "ResourceCheckDlg.h"
#include "Sampling.h"
#include "TeachZSensorPosition.h"
#include "IOInteraction.h"

#include "TerminalServiceDlg.h"
#include "SecsGemDiagnosticsDlg.h"
#include "SecsGemController.h"
#include "Scale_Interface.h"

class CStatisticsView;
class CImageQueueHandler;
class CTrackHandler;
class CLightControlDlg;
class CScanBarcode;
class CImageSelectionDlg;
class CMotionControlDialog;
class CPVIInspectionParameters;
class CCalibrationSettings;
class CTriangularSensroDlg;
class CJigLeveling;
class CTeachZSensorPosition;
class TerminalServiceDlg;
class SecsGemDiagnosticsDlg;

enum
{
	STATION_TOP,
	STATION_THR,
	TOTAL_STATIONS,
};

enum IMAGE_FORMAT
{
	ORIGINAL_IMAGE,
	COMPRESSED_IMAGE
};

struct SaveImageInfo
{
	SaveImageInfo()
		: cameraId(-1)
		, nDocumentId (0)
		, grabId(-1)
		, dieColumnId(-1)
		, dieRowId(-1)
		, defectId(-1)
		, saveDefect(false)
		, saveAll(false)
		, compressedFormat(false)
	{
	};

	CImageBuffer* image;
	CString lotId;
	CString stripId;
	CString BarcodeScanTime;
	int cameraId;
	int grabId;
	int nDocumentId;
	int dieColumnId;
	int dieRowId;
	int nFov;
	int nDieNo;
	int defectId;

	bool saveDefect;
	bool saveAll;
	bool savePass;

	bool compressedFormat;
};



class CApplication : public CWinAppEx
{
public:
	CApplication();

	int m_nTHR, m_nTOP;
	int nTop, nTHR;
	int m_nTotalTracks;
	int m_nTotalViews;
	int m_nTotalViewDocs[MAX_TRACKS];
	int m_nMaxLineOutputLog;
	//CArray <int, int> m_nStartSegmentsOfImage;
	MotionParameters motionParameters;		//Girish
	CSampling *m_pSamplingDlg;
	IOControl ioControl;
	std::queue < SaveImageInfo > saveImageQueue;
	std::queue < SaveImageInfo > saveDeepLearnImageQueue;

	CMessageBox *m_pMsgBox;

	BOOL m_bScanningEnabled;
	BOOL m_bDryRun;
	BOOL m_bWetRun;

	/* ------------------------ SECS/GEM ------------------------ */

	// Secsgem settings
	bool bEnableMultiOpenTransaction;
	bool bIsGEMControllerRemoteMode;
	int nMaxMultiOpenTransactions;
	int nEquipDeviceID;
	CString strEquipLocalAddress;
	CString strEquipRemoteAddress;
	CString strConnectionMode;
	int nEquipLocalPortID;
	int nEquipRemotePortID;
	int nEquipConnectMode;
	int nHSMST3Timeout;
	int nHSMST5Timeout;
	int nHSMST6Timeout;
	int nHSMST7Timeout;
	int nHSMST8Timeout;
	int nGEMControlState;
	int nGEMCommunicationState;
	bool bStartRemoteCmdRcvd;
	bool bReloadConfig;
	BOOL bEndLotMagzineBased;
	BOOL bLeadFrameMappingError;
	int nLeadFrameMappingErrorPos;
	bool bSoftTriggerStop;

	//-- Secs/gem information
	int secsinitRetCode;
	int processState;
	int previousProcessState;
	bool isSecsGemEnabled() const;
	bool isSecsGemInitialized();
	bool mapFileReceived;
	std::string defctCodeArr[50];
	SecsGemController& getGEMController();
	void sendGemEvent(CString eventName);
	void readEquipmentMapData(std::string filePath);
	void saveEquipmentMapData(std::string filePath);
	int DeleteRecipeFile(CString strDelRecipeFile);
	void StartCommandSECSGEM();
	void StopCommandSECSGEM();
	void PauseCommandSECSGEM();
	void AbortCommandSECSGEM();
	int StartProcessSelectCommandSECSGEM(CString strLotId, CString strCategories, CString strRecipe, int nSubstrateCount);
	void ListAllProcessRecipes();
	void ChangeEquipmentConstantValue(long lConstId, CString strConstLogicalName, SECSFormats oSvFormat, CString value);
	void saveAllParameters();
	void updateDefectCodes(CString defName, CString defCode);

	// Equipment Terminal Service
	TerminalServiceDlg* m_pTerminalServiceDlg;
	BOOL m_bTerminalService;
	CString m_strHostMessageRecvd;

	SecsGemDiagnosticsDlg* m_pSecsGemDiagnosticsDlg;
	BOOL m_bSecsGemDiagnostics;



	// For Secs/Gem communication
	bool _secsgemEnableCommunication;
	SecsGemController _gemController;

	std::vector<CString> RecipeFiles;
	CString currentRecipeLoaded;


	int InitializeSecsgem(bool reInitializtion = false);


	/* ------------------------ SECS/GEM End ------------------------ */


	//Clamping Sequence
	BOOL m_bClampingSequenceEnabled;
	BOOL m_bUnClampingSequenceEnabled;

	BOOL m_bBarcodeSequenceEnabled;
	BOOL m_bScanning; //for disable mapping
	BOOL m_bScanningCompleted;
	BOOL m_bSingleRunScanning;



	CTrackHandler* m_pTracks;
	//CSharedData m_SharedData[MAX_TRACKS];
	int m_nFirstTrackNo;
	int m_nOperationMode;
	CMDIChildWndEx* m_pFrames[MAX_VIEWS];
	CThreadManager m_ThreadManager;
	
	


	CStatisticsView* m_pStatsView;
	//CImageSelectionDlg* m_pImageSelectionDlg; //JP
	BOOL m_bInspectionParallelism;
	BOOL m_bStepSelection[THREAD_COUNT];//JP for Selecting defect for the step debug
	BOOL m_bStepSelectionEnable[THREAD_COUNT];
	BOOL m_bPixelRuler;
	BOOL m_TriSensorOpened;
	BOOL bIsLeftButtonClicked;
	BOOL m_bIsMasterApp;
	BOOL m_bIsCameraMonitorMode;

	BOOL m_bImagesViewInitialised;
	BOOL m_bSetForAllTracks;
	int m_nInSpectionDelay;
	int m_nNoOfDies;
	int m_nNoOfFov;
	int m_nNoOfFovX;
	int m_nNoOfFovY;
	int m_nTrackCountGroup;
	int m_nFOVWidthCalculated;
	int m_nFOVHeightCalculated;
	CStringArray m_strTopCameraName;
	CStringArray m_strTHRCameraName;
//	CStringArray m_strSideCameraName;
	
	CStringArray m_strTopCalibrationName;
	CStringArray m_strBottomCalibrationName;
	CStringArray m_strSideCalibrationName;
	
	int m_nImagesViewCX;
	int m_nImagesViewCY;
	//int m_nTop;
//	int m_nBottom;
//	int m_nSide;

    CUserLoginAccount m_UserLog;
	UserAccountInfo m_UserAccInfo;
	BOOL m_bSuperUserMode;
	LOTSUMMARY m_LotSummary;
	
	int nNoOfResetPerDay;
    CString strLotResetTime1;
	CString strLotResetTime2;
	SYSTEMTIME tLotResetTime1;
	SYSTEMTIME tLotResetTime2;

	CString m_LotSummaryRootDir;
	CString m_ImageSaveFolder;
	CString m_LotSummaryLotIDDir;

	CString m_strConfigFile;
	CString m_strConfigDir;
	CString m_strMainCfgDir;
	CString m_strMainCfgModeDir;
	CString m_strAppId;

	CString m_strConfigModifiedDate;
	CString m_strSystemLoggingDir;

	CString m_strDefImageStore;

	int m_nGrabTimeout;
	std::vector<int> m_nCameraTrigDelay;

	CString m_strMachineID;
	CString m_strProductCode;

	BOOL m_bOnline;

	CRITICAL_SECTION m_CriticalSection;
	CRITICAL_SECTION m_csOnline;
	CRITICAL_SECTION m_csFileAccess;
	CRITICAL_SECTION m_csOutputData;

	CRITICAL_SECTION m_CriticalSectionSaveImage;
	CRITICAL_SECTION m_CriticalSectionDeepLearnSaveImage;




	CRITICAL_SECTION m_CriticalSectionTOPDieShiftedInfo;

	HANDLE m_eventOnline;
	HANDLE m_eventVisionReady;
	HANDLE m_EventCommSendMsg;

	HANDLE eventScanThreadKilled;
	HANDLE m_eventStartScan;
	HANDLE m_eventStartYBiasing;
	HANDLE m_eventStartZBiasing;
	HANDLE EventStartBarcodeTrigger;
	HANDLE m_eventBarcodeTriggerDone;

	HANDLE eventSaveImage;
	HANDLE eventDeepLearnSaveImage;

	HANDLE eventSaveImageKilled;
	HANDLE eventDeepLearnSaveImageKilled;


//	HANDLE eventClearMemory;
	HANDLE eventClearMemoryDone; 

	HANDLE m_hTrackCommandThread;
	HANDLE m_EventRun;
	TrackCommand m_nCommand;
	int m_nActiveTrackCommandIdx;

	BOOL m_bStartScanThread;	
	BOOL m_bStartBiasingThread;

	DWORD m_TrigangualrsensorInstance;

	CImageQueueHandler* m_pImageQueue;
	BOOL m_bAutoResizeImage;
	BOOL m_bAutoResizeImageToMidRes;
	BOOL m_bAutoSaveResizedImage;
	BOOL m_bCalibrationOpen;
	int m_nSystemRestartCounter;
	BOOL m_bSaveCrashImages;
	int m_nCurrentLightControllerIndex;

	CString	m_strLightLibrary;
	CString	m_strIOInterfaceCard;
	CString m_strMotorParmFile;
	CString m_strMotionCard;
	int m_nMotorCardNo;
	int m_nMotionTimeout;
	MotorInterface m_MotorResource;
	CTriangularSensor m_TriangularSensor;

	//Scale Engine
	int InitScaleCard();
	ScaleInterface m_ScaleResource;
	CString m_strScaleCard;

	BOOL m_bTriangularSensorConnected;
	BOOL m_bEmergencyStop;
	BOOL m_bCurtainSensor;
	BOOL m_bBypassCurtainSensor;
	BOOL m_bMachineOn;
	BOOL m_bSensorLibLoaded;
	int m_nAxes[AXES_COUNT];
	CLightControlDlg* m_pLightCtrlDlg;
	CCalibrationSettings *m_pCalibrationDlg;
	CScanBarcode* m_pScanBarcodeDlg;
	BOOL m_bLightCtrlDlgOpen;
	BOOL m_bEnableReScanOption;
	BOOL m_bEnableRepeatScanOption;
	int m_lIOCardAddr;
	int m_lIOInterfaceCardAddr;

	CString m_strSensorLibrary;
	CString m_strSensorIpAddress;

	//CAuditTrailDlg* m_pAuditTrailDlg;
	CWnd* m_pAuditTrailDialog;
	CString m_strAuditLoggingDir;
	CString m_strParmFolder;

	int m_lIOReverseLogic;

	CLightHandler m_LightHandler;

	CIOInterface m_LightModule;
	CIOInterface m_CommModule;

	long m_lCOMMInCardNo;
	long m_lCOMMInPortID;
	long m_lCOMMOutCardNo;
	long m_lCOMMOutPortID;

	CGlobalBackgroundThread * pGlobalBackgroundThread;

	CErrorDlg * pErrorMsgDlg;

	CCommInterface m_CommInterface;
	CPixelRuler *m_pPixelRulerDlg;
	CTriangularSensroDlg *m_pTriSensorDlg;
	CDialogPane *m_pImageSelectionDlg;
	CDistanceCalibration *m_pDistanceCalibration;
	int m_nCrashImageCounter[MAX_TRACKS];

	//HANDLE m_hVisionResetThread;
	//HANDLE m_hPollingThread;

	int m_nFirstGrabDelay;
	int m_nSignalScanFrequency;
	int m_bEnableSimulationMode;
	BOOL m_bEnableManagePackageId;
	BOOL m_bEnableTopDatumPosToScanTHR;
	CString m_strSimulationImagesPath;
	int m_nMaxSimulationBuffers;

	int m_nUnitIndex;

	int m_bContinuousLoop;

	BOOL m_bTeachPass;
	BOOL m_bTeaching;
	BOOL m_bInspecting;

	BOOL m_bBarcodeScanning;
	BOOL m_bRecipeFound;

	BOOL m_bTrackInspSeq;
	BOOL m_bCamExtTrig;

	int m_nCamLibraryTrackIndex;

	BOOL m_bExitApp;

	BYTE m_cActiveTracks;
	BOOL m_bByPassResult;
	BOOL m_RuntimeDisplayEnable;

	BOOL m_bClassPriorityRealtime;
	BOOL m_bAllocFailedImageBuffer;
	int m_nFailImageCounter[MAX_TRACKS];
	BOOL m_nSequenceCounter;
	CPerformanceTimer InspectSeqTime;

	BOOL m_bInspSeqLoop;

    int m_nDeviceCounter;
	int m_nOutData;

	int m_nBarcodeSize;
	int m_nProductCodeSize;

	BOOL m_bSaveOnlinePassImages;
	BOOL m_bSaveOnlineFailImages;
	BOOL m_bSaveOnlineJpegImages;
	BOOL m_bSaveOnlineBMPImages;
	BOOL m_bOnlineContinueTopImageInsp;
	int m_nSaveJpegQualityImages;
	BOOL m_bStoredImagesLoop;
	
	CString m_strCommOPC;
	//CADSInterface *m_pADSInterface;
	BYTE m_byteNetID[6];
	unsigned int m_nADSPortNr;
	int m_nCycleTime;
	int m_nTxDelay;
	int m_nMaxDelay;
	int m_nTimeout;
	void SaveADSTwinCATSetting();

	void UpdateWatchDog(int nData);
	void UpdateLotChangedAck();

	int m_nToolNo[5];
	int m_nMoldNo[5];
	int m_nGripperNo[5];
	int m_nFailNo[5];
	int m_nResultType;
	int m_nGoodACounter;
	int m_nGoodBCounter;
	int m_nFailCounter;

	BOOL m_bInspectionAffinity;
	BOOL m_bLoadComplete;
	BOOL m_bSynchronousGrabEnable;
	CAdvSecurityDlg m_AdvSecurityDlg;
	BOOL m_bZoomAll;
	BOOL m_bLotOpened;// Is Lot Information Entered
	CString m_strLotInfo;
	CString m_strLotConfigFile;
	BOOL m_bDisplayAllStatsOffline;
	//LOTSUMMARY m_LotSummary;
	BOOL m_bStream;
	BOOL m_bSnap;
	BOOL m_bCalib;
	BOOL m_bAllowStream;
	int m_nFOVHeight;
	int m_nFOVWidth;
	/*int m_nDeviceNoPerBlockX;
	int m_nDeviceNoPerBlockY;
	int m_nDevicePitchX;
	int m_nDevicePitchY;
	int m_nDeviceOffsetX;
	int m_nDeviceOffsetY;*/

	CMotionControlDialog* m_pMotionControl;
	CJigLeveling* m_pJigLeveling;
	CTeachZSensorPosition* m_pTeachZSensorPosTool;
	BOOL bOpenedMotionDialog;
	CVerificationMode* m_pVerificationMode;
	BOOL m_bVerificationMode;
	BOOL m_bVerificationDialogOpened;

	CIOTestDlg* m_pIoHardwareDlg;

	//I/O Hardware Control
	IOInteraction * pIOControlDialog;

	CMotorTriggerTestDialog * pMotorTriggerTestDialog;
	CProgressDlg* m_pProgressDlg;
	BOOL m_bProgressDlgOpen;

	BOOL bEnableMotorTriggerTestDialog;


	CBarcodeReader BarcodeReader;
	CQRBarcodeReader QRBarcodeReader;
	int nBarcodePortID;
	int nQRBarcodePortID;
	CString qrReaderIpAddress;
	CString strMultiMonitorPath;
	BOOL bEnableBarcode;
	CString strInspectDeviceBarcodeID;
	CString strInspectDeviceScanTimeStamp;
	CString strProdcuctionLotID;
	int qrBarcodeReceivedTimeOutPeriod;
	int magazineCounter;



	BOOL m_bMotionCtrlDlgOpen;

	BOOL m_bLoadMultiImagesFromFile;
	CString m_strLoadMultiImagesFolderPath;
	CStringArray  m_strLoadMultiImagesArray;
	CStringArray m_strLightChannelName;
	BOOL m_bInspSequenceOffline;
//	int m_nXAxisPosition[MAX_FOV];//8
//	int m_nYAxisPosition[MAX_FOV];//8
	BOOL m_bLightCalibration;
	BOOL m_bDotCalibration;
	int m_nCalibXAxisPosition;
	int m_nCalibYAxisPosition;
	int m_nCalibZAxisPositionTop;
	int m_nCalibZAxisPositionBottom;
	int m_nStepTrack;
	int m_nStepDoc;
	int m_nXAxisSettlingTime;
	int m_nYAxisSettlingTime;

	HANDLE m_EventRefreshLight;
	HANDLE m_EventMotionSensor;
	HANDLE m_EventMotionSensorStart;
	HANDLE m_EventLogInspectionData;
//	HANDLE m_EventIOtest;


	CPVIInspectionParameters* m_pWndPVIDefectParameters;
	BOOL m_bAutoLightImageSave;//for Calibration;
	CMessageDlg *m_pDisplayMsgStatus;
	//for ethernet comm
	CString m_strPartFlex[MAX_DIES];
	int m_nPartErrorCode[MAX_DIES];
	int m_nDieErrorCodeStats[MAX_DIES];
	BOOL m_bPartInspect[MAX_DIES];
	int m_nFovPartErrorCode[MAX_DIES][MAX_FOV];
	BOOL m_bReceivedStartCmd;
	BOOL m_bModifierView;
	BOOL m_bReceivedResetCmd;	
	BOOL m_bReceivedStopCmd;
	int m_nVerificationMode;
	BOOL m_bShowDieVerification;
	BOOL m_bInspectionAll;
	int m_nPPM;
	int m_nUPH;
	CDisplayOnlinePanel *m_pDispOnlinePanel;
	CInfoMsgDlg* m_pInfoMsgDlg;
	SequenceOptions* m_pSequenceOptionDlg;

	int UpdateInfoMsgDlg(CString strCamDocName, CString strInfoMsg);
//	CArray<CParameters*> m_ParametersDefectCode;
	CArray<int> m_nDefectErrorCodesPriority;
	CArray<int> m_nDefectErrorCodesPriorityTemp;
	CStringArray m_strDefectErrorCodesPriority;
	CStringArray m_strDefectErrorCodesPriorityTemp;
	BOOL m_bEnableInspectionPriority;

	int m_nNo_of_Defects;
	int m_nErrorCode[MAX_DIES][MAX_FOV][MAX_VIEWS];
	CString m_strCalibDir;
	int m_nPviNumberOfArea[MAX_FOV][MAX_VIEWS];
	int m_nPviNoOfDefects[MAX_FOV][MAX_VIEWS][10];
	CStringArray m_strPviDefectsStats[MAX_FOV][MAX_VIEWS];
	int m_nSaveAllDieIndex[2];

	int nZSensorPositionCount;
	volatile long nZSensorPositionId;
	HANDLE eventRunZSensorSequence;
	HANDLE m_eventZSensorTrigCount;
	HANDLE m_eventZAxizFocus;
	HANDLE m_eventYAxisDatumOffset;

	//-- Motor latching
	std::vector<int> latchedPositionX;
	std::vector<int> latchedPositionY;
	std::vector<int> latchedPositionZ;
	int nLatchedPositionCount;
	volatile long nLatchedPositionId;

	HANDLE eventRunLatching;	
	std::vector< int > inspectionResultStripMap;


	//SFT Communication
	bool m_bSFTEnableCommunication;
	CString m_strSFTServerAddress;
	int m_nSFTMoveTypeMoveIn;
	int m_nSFTMoveTypeMoveOut;
	int m_nSFTTimeOut;

	int m_nSequenceIntensityId;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void InitializeDocView();
	BOOL SetParameters();
	BOOL InitProcess();
	int LoadDefaults(int nMaster, int nTrackCount);

	int CheckPassword(int SecurityAccessNo=0);
	int CheckForHalconnLicense();
	int CheckApplicationLicense();
	CString DecodeLicenseKey(CString strContent, CString strDecodeKey);
	CString EncodeKey(CString strContent, CString strKey);

	int SetVisOnLineState(BOOL bStatus);

	BOOL m_bDieInspection[MAX_DIES][MAX_FOV];
	int m_nTotalDefectsCount;
	void LoadRecipes();
	void LoadRecipes(int nTrack, int nFov);
	void AutoLoadRecipes();
	void SaveRecipeInfo();
	void ShowMotionInitializing();
	int CopyRecipe(CString strRecipeSrcMode,CString strRecipeDesMode,CString strRecipeSrc, CString strRecipeDst);
	CString GetFailedStr(int err,bool abbrev);
	CString GetCuvetteFailedStr(int err,bool abbrev);

	void UpdateLotSummary(int nDevice, int nFov, int nView, int nError, BOOL allStats = FALSE);
	void UpdateTotalLotSummary(int nTrack,int nError, BOOL allStats = FALSE);
	void UpdateStatisticsWindow(BOOL bRefresh = FALSE);
	void UpdateStatistics(size_t  GoodDieCount, size_t  BadDieCount);
	void UpdateStatisticsOnReScan(size_t  GoodDieCount,size_t  BadDieCount);
	void UpdateStatisticsOnRepeatScan(size_t  GoodDieCount, size_t  BadDieCount);
	void RecordLotStats();
	int AddLogEvent(CString strEvent,int nEventLog=0,BOOL m_bAuditTrail=TRUE);

	void SaveCrashImages(int nTrack);

	int InitializeLightLibrary();
	int InitializeCommLibrary();
	int InitializeMotionCard();
	int InitializeTriangularSensor();
	int ConfigureLightControllers();

	int m_bEnableResourceDisplay;
	bool m_bdisplayResourceWindow;
	bool m_bZoneMapping;
	CResourceCheckDlg* m_pResourceCheckDlg;
	bool m_bSubstrateMappingStyle;

// Implementation
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	BOOL CheckDiskFreeSpace();
	void ApplyDebugFlag(long lDebugFlag);

	void UpdateTrackStatus();
	int SavePresentationPassImages(int nTrack, int nError);

	void CheckConfiguration();
	void SetRecipeName(CString strRecipe);
	void SetUserName(CString strUser);
	void SetOnlineStatus();

	void BinarizeAllTracks(BOOL bBinaryMode);
	void StreamAllTracksToggle();
	int SynchronousCameraSnap(BOOL bDraw=TRUE);
	void SynchronousCameraStream();

	void InitializeLotStats();
	void RefreshDefectsList();
	void UpdateUPH(double dStripTime, double NetSequenceTime, int nDieNo);
	void UpdatePrintBar(size_t BadDevicecount);
	void UpdatePrintBarOnReScan(size_t BadDeviceCount);
	void UpdatePrintBarOnRepeatScan(size_t BadDeviceCount);
	int LoadDocImages(CString strFileName);
	void InitializePositionProperties();
	void ResizeDieSelectionDlg();
	void OnMaximize(BOOL bEnable);
	
	void StepDebugNavigate(int nStep);
	int NavigateDieImages(int nKey);
	int m_nNavigateStaticImgTrack;
	void SetCurPosParamDisplay(int nTrack, CString* str);
	int OpenLotDlg();
	void ShowMessageDlgBox(BOOL bTrue);
	int OnReceiveResetCmd();
	
	int SaveDefectCodeParameters(CString strRecipeFolder, CString strRecipeFile);
	void InitializeDefectCode();
	BOOL IsMotionDialogOpened() const;
	void CreateScanThread();
	void runQRBarcodeFailSequence();	
	void ModuleReplacementSequence();
	void UpdateDefectCodes(CString defName, CString defCode);


	void SaveAllImages();

	bool IsSFTEnabled() const;
	int getSFTMoveTypeMoveIn() const;
	int getSFTMoveTypeMoveOut() const;
	int getSFTTimeOut() const;
	std::string getSFTServerAddressPath() const;

	int getSequenceIntensityId();
	void SetSequenceIntensityId(int nIntnsity);
	

	//BOOL m_bDryRunMachine;

	//void LaunchWindowsExplorer();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnVisionOnline();
	afx_msg void OnUpdateVisionOnline(CCmdUI *pCmdUI);
	afx_msg void OnGooffline();
	afx_msg void Goonline();
	afx_msg void OnOnlineResetstatscounter();
	afx_msg void OnUpdateOnlineResetstatscounter(CCmdUI *pCmdUI);
	afx_msg void OnOnlineStatsResetMode();
	afx_msg void OnUpdateOnlineStatsResetMode(CCmdUI *pCmdUI);
	afx_msg void OnOnlineSavePassImages();
	afx_msg void OnUpdateOnlineSavePassImages(CCmdUI *pCmdUI);
	afx_msg void OnOnlineSaveFailImages();
	afx_msg void OnUpdateOnlineSaveFailImages(CCmdUI *pCmdUI);
	afx_msg void OnOnlineSaveJpegImages();
	afx_msg void OnUpdateOnlineSaveJpegImages(CCmdUI *pCmdUI);
	afx_msg void OnOnlineSaveBMPImages();
	afx_msg void OnUpdateOnlineSaveBMPImages(CCmdUI *pCmdUI);
	afx_msg void OnOnlineContinueTopImageInsp();
	afx_msg void OnUpdateOnlineContinueTopImageInsp(CCmdUI *pCmdUI);
	afx_msg void OnParmRecipeLoad();
	afx_msg void OnUpdateParmRecipeLoad(CCmdUI *pCmdUI);
	afx_msg void OnParmScanBarcode();
	afx_msg void OnUpdateParmScanBarcode(CCmdUI *pCmdUI);
	//afx_msg void OnParmRecipeSaveAs();
	//afx_msg void OnUpdateParmRecipeSaveAs(CCmdUI *pCmdUI);
	//afx_msg void OnParmRecipeDelete();
	//afx_msg void OnUpdateParmRecipeDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateParmConfig(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRecipeButton(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeAndProductCodeButton(CCmdUI *pCmdUI);
	afx_msg void OnUpdateHardwareButton(CCmdUI *pCmdUI);
	afx_msg void OnConfigurationAdstwincatsettings();
	afx_msg void OnUpdateConfigurationAdstwincatsettings(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOnlinemodeOptions(CCmdUI *pCmdUI);

	afx_msg void OnUpdateToolsSettings(CCmdUI *pCmdUI);
	afx_msg void OnUpdateToolsUser(CCmdUI *pCmdUI);
	afx_msg void OnSettingsUsers();
	afx_msg void OnUpdateSettingsUsers(CCmdUI* pCmdUI);
	afx_msg void OnSettingsAccessRights();
	afx_msg void OnUpdateSettingsAccessRights(CCmdUI* pCmdUI);
	afx_msg void OnUserLogIn();
	afx_msg void OnUpdateUserLogIn(CCmdUI* pCmdUI);
	afx_msg void OnUserLogOut();
	afx_msg void OnUpdateUserLogOut(CCmdUI* pCmdUI);
	afx_msg void OnUserChangePassword();
	afx_msg void OnUpdateUserChangePassword(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUserButton(CCmdUI *pCmdUI);
	afx_msg void OnUserButton();
	afx_msg void OnVisionstatusButton();
	afx_msg void OnUpdateVisionstatusButton(CCmdUI *pCmdUI);
	afx_msg void OnRecipeButton();
	afx_msg void OnHardwareIO();
	afx_msg void OnUpdateHardwareIO(CCmdUI *pCmdUI);
	afx_msg void OnOnlineBypassResult();
	afx_msg void OnUpdateOnlineBypassResult(CCmdUI *pCmdUI);
	afx_msg void OnCameraSynchronousGrab();
	afx_msg void OnUpdateCameraSynchronousGrab(CCmdUI *pCmdUI);
	afx_msg void OnViewOptionsAuditTrail();
	afx_msg void OnUpdateViewOptionsAuditTrail(CCmdUI *pCmdUI);
	afx_msg void OnViewZoomAll();
	afx_msg void OnUpdateViewZoomAll(CCmdUI *pCmdUI);
	afx_msg void OnOpenLot();
	afx_msg void OnUpdateOpenLot(CCmdUI *pCmdUI);
	afx_msg void OnCloseLot();
	afx_msg void OnUpdateCloseLot(CCmdUI *pCmdUI);
//	afx_msg void OnCameraStream();
//	afx_msg void OnUpdateCameraStream(CCmdUI *pCmdUI);

	afx_msg void OnInspectionLoop();
	afx_msg void OnUpdateInspectionLoop(CCmdUI *pCmdUI);
	
	afx_msg void OnDefaultSetting();
	afx_msg void OnUpdateDefaultSetting(CCmdUI *pCmdUI);

	afx_msg void OnParmOptionsTeachInspAndLoc();
	afx_msg void OnUpdateParmOptionsTeachInspAndLoc(CCmdUI *pCmdUI);
	afx_msg void OnParmOptionsDeviceConfiguration();
	afx_msg void OnUpdateParmOptionsDeviceConfiguration(CCmdUI *pCmdUI);
	afx_msg void OnParmOptionsDefectPrioritySelection();
	afx_msg void OnUpdateParmOptionsDefectPrioritySelection(CCmdUI *pCmdUI);
	afx_msg void OnMotionControl();
	afx_msg void OnUpdateMotionControl(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMotorInitializing(CCmdUI *pCmdUI);

	//afx_msg void OnToolsStaticimageLoadMultiImgFromFile();
	//afx_msg void OnUpdateToolsStaticimageLoadMultiImgFromFile(CCmdUI *pCmdUI);

	//afx_msg void OnInspInspectionSequence();
	//afx_msg void OnUpdateInspInspectionSequence(CCmdUI *pCmdUI);
	
	/*afx_msg void OnCameraSnapAll();
	afx_msg void OnUpdateCameraSnapAll(CCmdUI *pCmdUI);*/

	
	//afx_msg void OnInspSequenceLoop();
	//afx_msg void OnUpdateInspSequenceLoop(CCmdUI *pCmdUI);

	//afx_msg void OnInspSaveSequenceImages();
	//afx_msg void OnUpdateInspSaveSequenceImages(CCmdUI *pCmdUI);

	afx_msg void OnPixelRuler();
	afx_msg void OnUpdatePixelRuler(CCmdUI *pCmdUI);

	afx_msg void OnInspSequenceLoop();
	afx_msg void OnUpdateInspSequenceLoop(CCmdUI *pCmdUI);

	afx_msg void OnTriangularSensor();
	afx_msg void OnUpdateTriangularSensor(CCmdUI *pCmdUI);


	afx_msg void OnSecsGemHSMSSetting();
	afx_msg void OnUpdateSecsGemHSMSSetting(CCmdUI *pCmdUI);

	afx_msg void OnSecsGemSetting();
	afx_msg void OnUpdateSecsGemSetting(CCmdUI *pCmdUI);

	afx_msg void OnSecsGemTerminalService();
	afx_msg void OnUpdateSecsGemTerminalService(CCmdUI *pCmdUI);



	//afx_msg void OnToolsDryRun();
	//afx_msg void OnUpdateToolsDryRun(CCmdUI *pCmdUI);
	afx_msg void OnBarcodeReaderButton();
	afx_msg void OnUpdateBarcodeReaderButton(CCmdUI *pCmdUI);
	int getMotionTimeOut() const;
	
	//Clamping Sequence
	afx_msg void OnClampingSequenceClamp();
	afx_msg void OnUpdateClampingSequenceClamp(CCmdUI *pCmdUI);
	afx_msg void OnClampingSequenceUnClamp();
	afx_msg void OnUpdateClampingSequenceUnClamp(CCmdUI *pCmdUI);

	afx_msg void OnButtonZSensorSequence();
	afx_msg void OnUpdateButtonZSensorSequence(CCmdUI *pCmdUI);
	
	
	afx_msg void OnMotorTrigger();
	afx_msg void OnUpdateMotorTrigger(CCmdUI *pCmdUI);
	afx_msg void BarcodeSequence();
	afx_msg void OnUpdateBarcodeSequence(CCmdUI *pCmdUI);
	afx_msg void OnReadBarcodeSequence();
	afx_msg void OnCheckMultipleRun();
	afx_msg void OnUpdateCheckMultipleRun(CCmdUI *pCmdUI);
	afx_msg void OnDryRun();
	afx_msg void OnUpdateDryRun(CCmdUI *pCmdUI);
	
	afx_msg void OnButtonBarcodeSequence();
	afx_msg void OnUpdateButtonBarcodeSequence(CCmdUI *pCmdUI);
	afx_msg void OnButtonSequenceOptions();
	afx_msg void OnUpdateButtonSequenceOptions(CCmdUI *pCmdUI);
	afx_msg void OnButtonDefectSelection();
	afx_msg void OnUpdateButtonDefectSelection(CCmdUI *pCmdUI);
	afx_msg void OnButtonVerificationMode();
	afx_msg void OnUpdateButtonVerificationMode(CCmdUI *pCmdUI);
	afx_msg void OnButtonShowVerificationDlg();
	afx_msg void OnUpdateButtonShowVerificationDlg(CCmdUI *pCmdUI);

	afx_msg void OnSaveAllButton();
	afx_msg void OnUpdateSaveAllButton(CCmdUI *pCmdUI);
	int  getImageSaveLimitPercentage() const;
	afx_msg void OnImageSavingImageParam();
	afx_msg void OnUpdateImageSavingImageParam(CCmdUI *pCmdUI);
};

int DeleteDir(LPCTSTR lpszName);
BOOL GetFolder(CString* strSelectedFolder, const char* strStartFolder);
extern CApplication theApp;
