/// Application.cpp : Defines the class behaviors for the application.
/// Udupa; April'2012
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Application.h"
#include "MainFrame.h"
#include <direct.h>

#include "EvTabDoc.h"
#include "AppTabView.h"
#include "EvChildFrame.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include "AppView.h"
#include "CameraCode.h"
#include "IOCode.h"
#include "StatisticsDoc.h"
#include "StatisticsView.h"
#include "SplashScreen.h"
#include "StatisticsResetDlg.h"
//#include "LoadRecipeDlg.h"
//#include "CopyRecipeDlg.h"
//#include "DeleteRecipeDlg.h"
#include "ModifyAccounts.h"
#include "LoginDlg.h"
#include "ChangePasswordDlg.h"
#include "AdvSecurityDlg.h"
#include "LightControlDlg.h"
#include "IOTestDialog.h"
#include "IOTestDlg.h"
#include "BarcodeReaderDialog.h"
#include "QRBarcodeReaderDialog.h"

#include "PasswordDialog.h"
#include "MultipleRecipeDialog.h"
#include "ScanBarcode.h"
#include "ImageQueueHandler.h"
#include"ImageSelectionDlg.h"
#include "LotInfoDlg.h"
#include "AuditTrailDlg.h"
#include "DeviceConfig.h"
#include "DefectPrioritySelDlg.h"

#include "PVIInspectionParameters.h"
#include "CalibrationSettings.h"
#include <winsock2.h>
#include <IPHlpApi.h>
#include <stdlib.h>
#include "Encryption.h"
#include "TriangularSensroDlg.h"
#include "SequenceThread.h"
#include "FailedQRBarcodeDialog.h"
#include "DefectCodesDlg.h"
#include "namespace_utilities_function.h"
#include "DatumOffset.h"
#include "GlobalBackgroundThread.h"
#include "ConfigureImageSavingLimitDialog.h"

#include "SecsGemSetupDlg.h"
#import <msxml3.dll>


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

// CApplication

BEGIN_MESSAGE_MAP(CApplication, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_ONLINEMODE_VISIONONLINE, OnVisionOnline)
	ON_UPDATE_COMMAND_UI(ID_ONLINEMODE_VISIONONLINE, OnUpdateVisionOnline)
	ON_COMMAND(ID_GOOFFLINE, OnGooffline)
	ON_COMMAND(ID_ONLINE_RESETSTATSCOUNTER, OnOnlineResetstatscounter)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_RESETSTATSCOUNTER, OnUpdateOnlineResetstatscounter)
	ON_COMMAND(ID_ONLINE_STATSRESETMODE, OnOnlineStatsResetMode)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_STATSRESETMODE, OnUpdateOnlineStatsResetMode)
	ON_COMMAND(ID_ONLINE_SAVEPASSIMAGES, OnOnlineSavePassImages)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_SAVEPASSIMAGES, OnUpdateOnlineSavePassImages)
	ON_COMMAND(ID_ONLINE_SAVEFAILIMAGES, OnOnlineSaveFailImages)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_SAVEFAILIMAGES, OnUpdateOnlineSaveFailImages)
	ON_COMMAND(ID_CHECK_SAVE_AS_JPEG, OnOnlineSaveJpegImages)
	ON_UPDATE_COMMAND_UI(ID_CHECK_SAVE_AS_JPEG, OnUpdateOnlineSaveJpegImages) 
	ON_COMMAND(ID_CHECK_BMP_FORMAT, OnOnlineSaveBMPImages)
	ON_UPDATE_COMMAND_UI(ID_CHECK_BMP_FORMAT, OnUpdateOnlineSaveBMPImages)
	ON_COMMAND(ID_ONLINE_CONTINUEINSPECTION, OnOnlineContinueTopImageInsp)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_CONTINUEINSPECTION, OnUpdateOnlineContinueTopImageInsp)
	ON_COMMAND(ID_PARM_CONFIG, OnParmRecipeLoad)
	ON_UPDATE_COMMAND_UI(ID_PARM_CONFIG, OnUpdateParmRecipeLoad)
	ON_COMMAND(ID_PARM_SCAN_BARCODE, OnParmScanBarcode)
	ON_UPDATE_COMMAND_UI(ID_PARM_SCAN_BARCODE, OnUpdateParmScanBarcode)
	//ON_COMMAND(ID_PARM_CONFIG_SAVEAS, OnParmRecipeSaveAs)
	//ON_UPDATE_COMMAND_UI(ID_PARM_CONFIG_SAVEAS, OnUpdateParmRecipeSaveAs)
	//ON_COMMAND(ID_PARM_CONFIG_DELETE, OnParmRecipeDelete)
	//ON_UPDATE_COMMAND_UI(ID_PARM_CONFIG_DELETE, OnUpdateParmRecipeDelete)
	ON_UPDATE_COMMAND_UI(ID_PARM_CONFIG, OnUpdateParmConfig)
	ON_COMMAND(ID_RECIPE_BUTTON, OnRecipeButton)
	ON_UPDATE_COMMAND_UI(ID_RECIPE_BUTTON, OnUpdateRecipeButton)
	ON_UPDATE_COMMAND_UI(ID_PRODUCT_CODE_BUTTON, OnUpdateModeAndProductCodeButton)
	ON_UPDATE_COMMAND_UI(ID_MODE_BUTTON, OnUpdateModeAndProductCodeButton)
	ON_UPDATE_COMMAND_UI(ID_PARM_HARDWARE, OnUpdateHardwareButton)
	ON_COMMAND(ID_PARM_HARDWARE_ADSTWINCAT, OnConfigurationAdstwincatsettings)
	ON_UPDATE_COMMAND_UI(ID_PARM_HARDWARE_ADSTWINCAT, OnUpdateConfigurationAdstwincatsettings)
	ON_UPDATE_COMMAND_UI(ID_ONLINEMODE_OPTIONS, OnUpdateOnlinemodeOptions)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SETTINGS, OnUpdateToolsSettings)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_USER, OnUpdateToolsUser)
	ON_COMMAND(ID_SETTINGS_USERS, OnSettingsUsers)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_USERS, OnUpdateSettingsUsers)
	ON_COMMAND(ID_SETTINGS_ACCESSRIGHTS, OnSettingsAccessRights)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_ACCESSRIGHTS, OnUpdateSettingsAccessRights)
	ON_COMMAND(ID_USER_LOGIN, OnUserLogIn)
	ON_UPDATE_COMMAND_UI(ID_USER_LOGIN, OnUpdateUserLogIn)
	ON_COMMAND(ID_USER_LOGOUT, OnUserLogOut)
	ON_UPDATE_COMMAND_UI(ID_USER_LOGOUT, OnUpdateUserLogOut)
	ON_COMMAND(ID_USER_CHANGEPASSWORD, OnUserChangePassword)
	ON_UPDATE_COMMAND_UI(ID_USER_CHANGEPASSWORD, OnUpdateUserChangePassword)
	ON_UPDATE_COMMAND_UI(ID_USER_BUTTON, OnUpdateUserButton)
	ON_COMMAND(ID_USER_BUTTON, OnUserButton)
	ON_COMMAND(ID_VISIONSTATUS_BUTTON, OnVisionstatusButton)
	ON_UPDATE_COMMAND_UI(ID_VISIONSTATUS_BUTTON, OnUpdateVisionstatusButton)
	ON_COMMAND(ID_PARM_HARDEARE_IO, OnHardwareIO)
	ON_UPDATE_COMMAND_UI(ID_PARM_HARDEARE_IO, OnUpdateHardwareIO)
	ON_COMMAND(ID_ONLINE_BYPASSRESULT, OnOnlineBypassResult)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_BYPASSRESULT, OnUpdateOnlineBypassResult)
	ON_COMMAND(ID_HARDWARE_SYNCHRONOUS_GRAB, OnCameraSynchronousGrab)
	ON_UPDATE_COMMAND_UI(ID_HARDWARE_SYNCHRONOUS_GRAB, OnUpdateCameraSynchronousGrab)
	ON_COMMAND(ID_VIEWOPTIONS_AUDITTRAIL, OnViewOptionsAuditTrail)
	ON_UPDATE_COMMAND_UI(ID_VIEWOPTIONS_AUDITTRAIL, OnUpdateViewOptionsAuditTrail)
	ON_COMMAND(ID_VIEW_ZOOM_ALL, OnViewZoomAll)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_ALL, OnUpdateViewZoomAll)
	ON_COMMAND(ID_OPEN_LOT, OnOpenLot)
	ON_UPDATE_COMMAND_UI(ID_OPEN_LOT, OnUpdateOpenLot)
	ON_COMMAND(ID_CLOSE_LOT, OnCloseLot)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_LOT, OnUpdateCloseLot)

	ON_COMMAND(ID_INSPECTION_LOOP, OnInspectionLoop) //JP
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_LOOP, OnUpdateInspectionLoop)

	ON_COMMAND(ID_DEFAULT_SETTING, OnDefaultSetting) //JP
	ON_UPDATE_COMMAND_UI(ID_DEFAULT_SETTING, OnUpdateDefaultSetting)

	ON_COMMAND(ID_TOOLS_TEACH_INSP_LOC_BUTTON, OnParmOptionsTeachInspAndLoc)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_TEACH_INSP_LOC_BUTTON, OnUpdateParmOptionsTeachInspAndLoc)
	ON_COMMAND(ID_DEVICE_CONFIGURATION_BUTTON, OnParmOptionsDeviceConfiguration)
	ON_UPDATE_COMMAND_UI(ID_DEVICE_CONFIGURATION_BUTTON, OnUpdateParmOptionsDeviceConfiguration)
	ON_COMMAND(ID_DEFECT_PRIPRITY_SEL_BUTTON, OnParmOptionsDefectPrioritySelection)
	ON_UPDATE_COMMAND_UI(ID_DEFECT_PRIPRITY_SEL_BUTTON, OnUpdateParmOptionsDefectPrioritySelection)
	
	ON_COMMAND(ID_CONFIG_MOTOR, OnMotionControl)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_MOTOR, OnUpdateMotionControl)
	ON_UPDATE_COMMAND_UI(ID_INITIALIZING_BUTTON, OnUpdateMotorInitializing)

	

	ON_COMMAND(ID_PIXEL_RULER, OnPixelRuler)
	ON_UPDATE_COMMAND_UI(ID_PIXEL_RULER, OnUpdatePixelRuler)

	ON_COMMAND(ID_SEQ_LOOP, OnInspSequenceLoop)
	ON_UPDATE_COMMAND_UI(ID_SEQ_LOOP, OnUpdateInspSequenceLoop)
	
		
	ON_COMMAND(ID_TRIANGULAR_SENSOR_BUTTON, OnTriangularSensor)
	ON_UPDATE_COMMAND_UI(ID_TRIANGULAR_SENSOR_BUTTON, OnUpdateTriangularSensor)

	//SECS/GEM Settings
	ON_COMMAND(ID_SECSGEM_HSMS_SETTING, OnSecsGemHSMSSetting)
	ON_UPDATE_COMMAND_UI(ID_SECSGEM_HSMS_SETTING, OnUpdateSecsGemHSMSSetting)
	ON_COMMAND(ID_SECSGEM_COMMUNICATION_DIAGNOSTICS, OnSecsGemSetting)
	ON_UPDATE_COMMAND_UI(ID_SECSGEM_COMMUNICATION_DIAGNOSTICS, OnUpdateSecsGemSetting)
	ON_COMMAND(ID_SECSGEM_TERMINAL_SERVICE, OnSecsGemTerminalService)
	ON_UPDATE_COMMAND_UI(ID_SECSGEM_TERMINAL_SERVICE, OnUpdateSecsGemTerminalService)

	//Clamping Sequence
	ON_COMMAND(ID_CLAMPING_SEQUENCE_CLAMP, &CApplication::OnClampingSequenceClamp)
	ON_UPDATE_COMMAND_UI(ID_CLAMPING_SEQUENCE_CLAMP, &CApplication::OnUpdateClampingSequenceClamp)
	ON_COMMAND(ID_CLAMPING_SEQUENCE_UNCLAMP, &CApplication::OnClampingSequenceUnClamp)
	ON_UPDATE_COMMAND_UI(ID_CLAMPING_SEQUENCE_UNCLAMP, &CApplication::OnUpdateClampingSequenceUnClamp)

	ON_COMMAND(ID_BARCODE_READER_BUTTON, &CApplication::OnBarcodeReaderButton)
	ON_UPDATE_COMMAND_UI(ID_BARCODE_READER_BUTTON, &CApplication::OnUpdateBarcodeReaderButton)
	ON_COMMAND(ID_Z_SENSOR_SEQUENCE, &CApplication::OnButtonZSensorSequence)
	ON_UPDATE_COMMAND_UI(ID_Z_SENSOR_SEQUENCE, &CApplication::OnUpdateButtonZSensorSequence)
	
	ON_COMMAND(ID_MOTOR_TRIGGER, &CApplication::OnMotorTrigger)
	ON_UPDATE_COMMAND_UI(ID_MOTOR_TRIGGER, &CApplication::OnUpdateMotorTrigger)
	//ON_COMMAND(ID_BARCODE_SEQUENCE, &CApplication::OnBarcodeSequence)
	//ON_UPDATE_COMMAND_UI(ID_BARCODE_SEQUENCE, &CApplication::OnUpdateBarcodeSequence)
	ON_COMMAND(ID_READ_BARCODE_SEQUENCE, &CApplication::OnReadBarcodeSequence)
	ON_COMMAND(ID_CHECK_MULTIPLE_RUN, &CApplication::OnCheckMultipleRun)
	ON_UPDATE_COMMAND_UI(ID_CHECK_MULTIPLE_RUN, &CApplication::OnUpdateCheckMultipleRun)
	ON_COMMAND(ID_DRY_RUN, &CApplication::OnDryRun)
	ON_UPDATE_COMMAND_UI(ID_DRY_RUN, &CApplication::OnUpdateDryRun)
	
	ON_COMMAND(ID_BUTTON_BARCODE_SEQUENCE, &CApplication::OnButtonBarcodeSequence)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_BARCODE_SEQUENCE, &CApplication::OnUpdateButtonBarcodeSequence)
	ON_COMMAND(ID_BUTTON_SEQUENCE_OPTIONS, &CApplication::OnButtonSequenceOptions)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SEQUENCE_OPTIONS, &CApplication::OnUpdateButtonSequenceOptions)
	ON_COMMAND(ID_BUTTON_DEFECT_SELECTION, &CApplication::OnButtonDefectSelection)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DEFECT_SELECTION, &CApplication::OnUpdateButtonDefectSelection)
	ON_COMMAND(ID_BUTTON_VERIFICATION_MODE, &CApplication::OnButtonVerificationMode)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_VERIFICATION_MODE, &CApplication::OnUpdateButtonVerificationMode)
	ON_COMMAND(ID_BUTTON_SHOW_VERIFICATION_DLG, &CApplication::OnButtonShowVerificationDlg)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_VERIFICATION_DLG, &CApplication::OnUpdateButtonShowVerificationDlg)
	
	ON_COMMAND(ID_SAVE_ALL_BUTTON, &CApplication::OnSaveAllButton)
	ON_UPDATE_COMMAND_UI(ID_SAVE_ALL_BUTTON, &CApplication::OnUpdateSaveAllButton)
	ON_COMMAND(ID_IMAGE_SAVING_IMAGE_PARAM, &CApplication::OnImageSavingImageParam)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_SAVING_IMAGE_PARAM, &CApplication::OnUpdateImageSavingImageParam)
END_MESSAGE_MAP()


int OutPortSendString(int nController, CString* pstrData);


// CApplication construction

CApplication::CApplication()
{

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	//SetAppID(_T("Emage Equipment Pte Ltd.Ev_InCuvette.AppID.NoVersion"));
	SetAppID(_T("Emage Equipment Pte Ltd.Sherlock_Spyglass.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	for(int i=0; i<MAX_VIEWS; i++)
		m_pFrames[i] = NULL;

	m_pTracks = NULL;
	m_nCamLibraryTrackIndex = -1;

	m_bImagesViewInitialised = FALSE;
	m_nImagesViewCX = 0;
	m_nImagesViewCY = 0;
	bIsLeftButtonClicked = FALSE;

	for(int i=0;i<THREAD_COUNT;i++)
		m_bStepSelection[i]=TRUE;

	nTop =0;
	nTHR =0;

	m_bEmergencyStop = FALSE;
	m_bCurtainSensor = FALSE;
	m_bReceivedStopCmd = FALSE;

	for(int i=0;i<5;i++){
		CString strTemp;
		strTemp.Format("---Part Mid%d---",i+1);
		m_strPartFlex[i]=strTemp;
		m_nPartErrorCode[i] = 98765;
		m_nDieErrorCodeStats[i] = -98765;
		m_bPartInspect[i]  = TRUE;
	}

	m_bInspSeqLoop = FALSE;
	m_nSequenceCounter = 0;
	m_bBypassCurtainSensor = FALSE;
	m_bReceivedStartCmd = FALSE;
	m_bPixelRuler = FALSE;
	m_bReceivedResetCmd = FALSE;
	//m_bDryRunMachine = FALSE;
	m_nPPM = 0;
	m_nUPH = 0;
	m_nVerificationMode = 0;
	for (int nDie = 0; nDie < MAX_DIES; nDie++){
		for (int nFOV = 0; nFOV < MAX_FOV; nFOV++) {
			for (int nView = 0; nView < MAX_VIEWS; nView++) {
				m_nErrorCode[nDie][nFOV][nView] = 98765;
				
			}
			
			m_bDieInspection[nDie][nFOV] = FALSE;
			m_nFovPartErrorCode[nDie][nFOV] = 98765;


		}
	}
	for (int i = 0; i < 2; i++) {
		m_nSaveAllDieIndex[i] = 0;
	}
	
	m_bSetForAllTracks = FALSE;
	m_nInSpectionDelay = 0;
	m_bInspectionAll = FALSE;
	m_bSensorLibLoaded = FALSE;
	m_bTriangularSensorConnected = FALSE;
	m_TrigangualrsensorInstance = NULL;
	m_pImageSelectionDlg = NULL;

	m_bScanningEnabled = FALSE;
	

	m_bScanning = FALSE;
	m_bScanningCompleted = FALSE;
	m_bSingleRunScanning = TRUE;
	m_bStartScanThread = FALSE;
	m_bStartBiasingThread = FALSE;

	m_bClampingSequenceEnabled = FALSE;
	m_bUnClampingSequenceEnabled = FALSE;

	m_bBarcodeSequenceEnabled=FALSE;
	m_bDryRun = FALSE;
	m_bWetRun = FALSE;

	//-- Motor latching
	nZSensorPositionCount = 0;
	InterlockedExchange(&nZSensorPositionId, 0);

	eventRunZSensorSequence = CreateEventA(NULL, FALSE, FALSE, NULL);
	m_eventZSensorTrigCount = CreateEventA(NULL, TRUE, FALSE, NULL);
	m_eventZAxizFocus = CreateEventA(NULL, FALSE, FALSE, NULL);
	m_eventYAxisDatumOffset = CreateEventA(NULL, FALSE, FALSE, NULL);
	eventSaveImage = CreateEventA(NULL, FALSE, FALSE, NULL);
	eventDeepLearnSaveImage= CreateEventA(NULL, FALSE, FALSE, NULL);
	eventSaveImageKilled = CreateEventA(NULL, TRUE, TRUE, NULL);
	eventDeepLearnSaveImageKilled= CreateEventA(NULL, TRUE, TRUE, NULL);

	nLatchedPositionCount = 0;
	InterlockedExchange(&nLatchedPositionId, 0);

	eventRunLatching = CreateEventA(NULL, FALSE, FALSE, NULL);
//	eventClearMemory = CreateEventA(NULL, FALSE, FALSE, NULL);
	eventClearMemoryDone = CreateEventA(NULL, FALSE, FALSE, NULL);
	m_pVerificationMode = NULL;
	m_bVerificationMode = FALSE;
	m_bVerificationDialogOpened = FALSE;
	m_bEnableManagePackageId = FALSE;
	
	
	m_bProgressDlgOpen = FALSE;
	m_bEnableReScanOption = FALSE;

	m_bSFTEnableCommunication = FALSE;
	m_strSFTServerAddress = "";
	m_nSFTMoveTypeMoveIn = 2;
	m_nSFTMoveTypeMoveOut = 3;
	m_nSFTTimeOut = 25000;

	m_bEnableInspectionPriority = 0;
	m_nSequenceIntensityId = 0;
	m_pSamplingDlg = NULL;


	// Secsgem settings
	nMaxMultiOpenTransactions = 10;
	nEquipDeviceID = 100;
	strEquipLocalAddress = "127.0.0.1";
	strEquipRemoteAddress = "192.168.100.25";
	strConnectionMode = "Passive";
	nEquipLocalPortID = 6000;
	nEquipRemotePortID = 5000;
	nEquipConnectMode = 0;
	bEnableMultiOpenTransaction = FALSE;
	bIsGEMControllerRemoteMode = FALSE;
	nHSMST3Timeout = 45;
	nHSMST5Timeout = 10;
	nHSMST6Timeout = 5;
	nHSMST7Timeout = 10;
	nHSMST8Timeout = 5;
	nGEMControlState = 5;	// default Remote mode
	nGEMCommunicationState = 1;
	bStartRemoteCmdRcvd = FALSE;
	bEndLotMagzineBased = TRUE;
	bLeadFrameMappingError = FALSE;
	nLeadFrameMappingErrorPos = -1;
	bSoftTriggerStop = FALSE;

	m_bTerminalService = FALSE;
	m_bSecsGemDiagnostics = FALSE;
}

// The one and only CApplication object

CApplication theApp;


class CCustomCommandLineInfo : public CCommandLineInfo
{
public:
	int m_nArg[3];
	int m_nIndex;

	CCustomCommandLineInfo()
	{
		m_nArg[0] = -1;
		m_nArg[1] = 0;
		m_nArg[2] = 0;
		m_nIndex = 0;
	}

	virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast)
	{
		sscanf_s(pszParam, "%d", &m_nArg[m_nIndex++]);
	}
};

// CApplication initialization
int CApplication::LoadDefaults(int nMaster, int nTrackCount)
{
	CString strSec;
	if(nMaster == -1) {
		strSec = "Application (0-Master; 1-Server; 2-Ask)";
		nMaster = GetRegistryEntry(READ_REGPATH + "Preferences", strSec, -1);
		if(nMaster == -1) {
			PutRegistryEntry(WRITE_REGPATH + "Preferences", strSec, 0);
			nMaster = 0;
		}
	}

	if(nMaster==2) {
		int nResult = AfxMessageBox("Open as MASTER Application?", MB_YESNOCANCEL|MB_ICONINFORMATION);
		if(nResult == IDYES)
			m_bIsMasterApp = TRUE;
		else if(nResult == IDCANCEL)
			return -1;
	}
	else
		m_bIsMasterApp = (nMaster==0);

	if(m_bIsMasterApp && !IsSingleInstance("Sherlock_Spyglass"))
		return -1;

	//if(m_bIsMasterApp) {
	//	CString str, str1 = "";
	//	str1 = GetRegistryEntry(READ_REGPATH + "Preferences", "Application Path (Server)", str1);
	//	if(str1.IsEmpty()) {
	//		str1 = "D:\\";
	//		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Application Path (Server)", str1);
	//	}

	//	str.Format("%s\\Ev_DryLens.exe 1", str1);
	//	if(!LaunchProcess(str))
	//		OutputDebugLogTo(0, TRUE, "Failed to launch server application (%d)", GetLastError());
	//	else
	//		Sleep(1000);
	//}
	//else {
	//	if(!IsSingleInstance("Ev_DryLens Server"))
	//		return -1;
	//}
	
	int nValue = GetRegistryEntry(READ_REGPATH + "Preferences", "Camera (0-Controller; 1-Monitor; 2-Ask)", -1);
	if(nValue == -1) {
		nValue = 0;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Camera (0-Controller; 1-Monitor; 2-Ask)", nValue);
	}
	if(nValue==2) {
		int nResult = AfxMessageBox("Open as MASTER Camera Controller?", MB_YESNOCANCEL|MB_ICONINFORMATION);
		if(nResult == IDNO)
			m_bIsCameraMonitorMode = TRUE;
		else if(nResult == IDCANCEL)
			return -1;
	}
	else
		m_bIsCameraMonitorMode = (nValue==1);

	if(nTrackCount)
		m_nTotalTracks = nTrackCount;
	else
		m_nTotalTracks = GetRegistryEntry(READ_REGPATH + "Preferences", "Number of Tracks", -1);
	if((m_nTotalTracks < 1) || (m_nTotalTracks >  3/*MAX_TRACKS*/)) {
		m_nTotalTracks = 2;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Number of Tracks", m_nTotalTracks);
	}
	m_pTracks = new CTrackHandler[m_nTotalTracks];

	m_nTOP = GetRegistryEntry(READ_REGPATH + "Preferences", "Number of Top Station Intensities", -1);
	if((m_nTOP < 1) || (m_nTOP >  3)) {
		m_nTOP = 1;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Number of Top Station Intensities", m_nTOP);
	}
	m_nTotalViewDocs[STATION_TOP] = m_nTOP;

	m_nTHR = GetRegistryEntry(READ_REGPATH + "Preferences", "Number of THR Station Intensities", -1);
	if ((m_nTHR < 1) || (m_nTHR > 3)) {
		m_nTHR = 1;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Number of THR Station Intensities", m_nTHR);
	}
	m_nTotalViewDocs[STATION_THR] = m_nTHR;

	for(int i=0; i<m_nTotalTracks; i++)
		m_pTracks[i].m_nTotalDocs = m_nTotalViewDocs[i];
	
	m_nTotalViews = (m_nTHR + m_nTOP);

	m_nNoOfDies = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Number of Dies", -1);
	if((m_nNoOfDies < 1) || (m_nNoOfDies >  MAX_DIES)) {
		m_nNoOfDies = 5;//4;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Number of Dies", m_nNoOfDies);
	}
	
	m_nNoOfFovX = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Number of FOV X", -1);
	if ((m_nNoOfFovX < 1) || (m_nNoOfFovX >  12)) {
		m_nNoOfFovX = 12;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Number of FOV X", m_nNoOfFovX);
	}

	m_nNoOfFovY = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Number of FOV Y", -1);
	if ((m_nNoOfFovY < 1) || (m_nNoOfFovX >  5)) {
	m_nNoOfFovY = 3;
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Number of FOV Y", m_nNoOfFovY);
	}

	m_nNoOfFov = m_nNoOfFovX * m_nNoOfFovY;

	m_nNo_of_Defects = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Number of Defects", -1);
	if (m_nNo_of_Defects < 1) {
		m_nNo_of_Defects = 100;//4;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Number of Defects", m_nNo_of_Defects);
	}

	m_nFOVHeight = 8;
	m_nFOVHeight = GetRegistryEntry(READ_REGPATH + "Preferences", "FOV Height (mm)", m_nFOVHeight);
	PutRegistryEntry(WRITE_REGPATH + "Preferences", "FOV Height (mm)", m_nFOVHeight);

	m_nFOVWidth = 11;
	m_nFOVWidth = GetRegistryEntry(READ_REGPATH + "Preferences", "FOV Width (mm)", m_nFOVWidth);
	PutRegistryEntry(WRITE_REGPATH + "Preferences", "FOV Width (mm)", m_nFOVWidth);

	m_nMaxLineOutputLog = GetRegistryEntry(READ_REGPATH + "Preferences", "MaxLines OutPutLog", -1);
	if (m_nMaxLineOutputLog < 1) {
		m_nMaxLineOutputLog = 1000;//4;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "MaxLines OutPutLog", m_nMaxLineOutputLog);
	}

	/*m_nDeviceNoPerBlockX = 4;
	m_nDeviceNoPerBlockX = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Device No Per Block X", m_nDeviceNoPerBlockX);
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Device No Per Block X", m_nDeviceNoPerBlockX);

	m_nDeviceNoPerBlockY = 3;
	m_nDeviceNoPerBlockY = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Device No Per Block Y", m_nDeviceNoPerBlockY);
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Device No Per Block Y", m_nDeviceNoPerBlockY);*/

//	m_strSideCameraName.RemoveAll();
	m_strTopCameraName.RemoveAll();
//	m_strBottomCameraName.RemoveAll();
	m_strSideCalibrationName.RemoveAll();
	m_strTopCalibrationName.RemoveAll();
	m_strBottomCalibrationName.RemoveAll();
	m_strLightChannelName.RemoveAll();


	CString strTempTName = "";
	strTempTName = GetRegistryEntry(READ_REGPATH + "Preferences", "Top Calibration Name", strTempTName);
	if (strTempTName.IsEmpty()) {
		strTempTName = "Top Die;Top EnCap";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Top Calibration Name", strTempTName);
	}
	CString strTemp;
	int nPos = 0;
	do
	{
		strTemp = strTempTName.Tokenize(_T(";"), nPos);
		m_strTopCalibrationName.Add(strTemp);
	} while (strTemp != "");

	strTempTName = "";
	strTempTName = GetRegistryEntry(READ_REGPATH + "Preferences", "Bottom Calibration Name", strTempTName);
	if (strTempTName.IsEmpty()) {
		strTempTName = "Bottom Die;Bottom EnCap;Bottom Slot;Bottom Cover Layer;B5";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Bottom Calibration Name", strTempTName);
	}
	strTemp = "";
	nPos = 0;
	do
	{
		strTemp = strTempTName.Tokenize(_T(";"), nPos);
		m_strBottomCalibrationName.Add(strTemp);
	} while (strTemp != "");

	strTempTName = "";
	strTempTName = GetRegistryEntry(READ_REGPATH + "Preferences", "Side Calibration Name", strTempTName);
	if (strTempTName.IsEmpty()) {
		strTempTName = "Side Left Tilt;Side Right Tilt";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Side Calibration Name", strTempTName);
	}
	strTemp = "";
	nPos = 0;
	do
	{
		strTemp = strTempTName.Tokenize(_T(";"), nPos);
		m_strSideCalibrationName.Add(strTemp);
	} while (strTemp != "");

	strTempTName = "";
	strTempTName = GetRegistryEntry(READ_REGPATH + "Preferences", "Top Camera Name",strTempTName);
	if(strTempTName.IsEmpty()) {
		strTempTName = "TOP Die; Top Encap;";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Top Camera Name", strTempTName);
	}
	strTemp = "";
	nPos = 0;
	 do
	{
		strTemp = strTempTName.Tokenize(_T(";"), nPos);
		m_strTopCameraName.Add(strTemp);
	}while (strTemp != "");

	strTempTName = "";
	strTempTName = GetRegistryEntry(READ_REGPATH + "Preferences", "THR Camera Name",strTempTName);
	if(strTempTName.IsEmpty()) {
		strTempTName = "THR Die";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "THR Camera Name", strTempTName);
	}
	strTemp = "";
	nPos = 0;
	 do
	{
		strTemp = strTempTName.Tokenize(_T(";"), nPos);
		m_strTHRCameraName.Add(strTemp);
	}while (strTemp != "");

	/*

	strTempTName = "";
	strTempTName = GetRegistryEntry(READ_REGPATH + "Preferences", "Side Camera Name",strTempTName);
	if(strTempTName.IsEmpty()) {
		strTempTName = "Side Left Tilt;Side Right Tilt";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Side Camera Name", strTempTName);
	}
	strTemp = "";
	nPos = 0;
	 do
	{
		strTemp = strTempTName.Tokenize(_T(";"), nPos);
		m_strSideCameraName.Add(strTemp);
	}while (strTemp != "");
*/

	strTempTName = "";
	strTempTName = GetRegistryEntry(READ_REGPATH + "Light", "Light Channel Name",strTempTName);
	if(strTempTName.IsEmpty()) {
		strTempTName = "TOP1;TOP2;THR1";
		PutRegistryEntry(WRITE_REGPATH + "Light", "Light Channel Name", strTempTName);
	}
	strTemp = "";
	nPos = 0;
	 do
	{
		strTemp = strTempTName.Tokenize(_T(";"), nPos);
		m_strLightChannelName.Add(strTemp);
	}while (strTemp != "");


	nValue = GetRegistryEntry(READ_REGPATH + "Preferences", "Active Tracks", -1);
	if(nValue == -1) {
		m_cActiveTracks = 0x1f;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Active Tracks", m_cActiveTracks);
	}
	else
		m_cActiveTracks = nValue & 0xff;

	m_bLotOpened = GetRegistryEntry(READ_REGPATH + "Preferences", "Lot Opened", -1);
	if (m_bLotOpened==-1) {
		m_bLotOpened = FALSE;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Lot Opened", m_bLotOpened);
	}

	if (m_bLotOpened) {
		m_strLotInfo = "void";
		m_strLotInfo = GetRegistryEntry(READ_REGPATH + "Preferences", "Lot Info", m_strLotInfo);
		if (m_strLotInfo.CompareNoCase("void") == 0) {
			m_strLotInfo = " ";
			PutRegistryEntry(WRITE_REGPATH + "Preferences", "Lot Info", m_strLotInfo);
		}
		m_strLotConfigFile = "void";
		m_strLotConfigFile = GetRegistryEntry(READ_REGPATH + "Preferences", "Lot Info Receipe", m_strLotConfigFile);
		if (m_strLotConfigFile.CompareNoCase("void") == 0) {
			m_strLotConfigFile = " ";
			PutRegistryEntry(WRITE_REGPATH + "Preferences", "Lot Info Receipe", m_strLotConfigFile);
		}
	}
	else
	{
		m_strLotInfo = "";
	}


	m_strMainCfgDir = GetRegistryEntry(READ_REGPATH + "Preferences", "Folder: Recipe", "void");
	if (m_strMainCfgDir.CompareNoCase("void")==0) {
		m_strMainCfgDir = "C:\\Sherlock_Spyglass Configs";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Folder: Recipe", m_strMainCfgDir);
	}
	_mkdir(m_strMainCfgDir);
	CString strTempConfig = m_strMainCfgDir +"\\Engineering Config";
	_mkdir(strTempConfig);

	strTempConfig = m_strMainCfgDir +"\\Production Config";
	_mkdir(strTempConfig);

	m_strMainCfgModeDir = "void";
	m_strMainCfgModeDir = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Folder:Config Mode", "void");
	if (m_strMainCfgModeDir.CompareNoCase("void")==0) {
		m_strMainCfgModeDir = "Engineering Config";
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Folder:Config Mode", m_strMainCfgModeDir);
	}
	

	m_strConfigFile = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Recipe", "");
	if(m_strConfigFile.IsEmpty()){
		m_strConfigFile = "Default";
		PutRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Recipe", m_strConfigFile);
	}

	m_strCalibDir.Format("%s\\Calibration", m_strMainCfgDir);

	m_strSystemLoggingDir = GetRegistryEntry(READ_REGPATH + "Preferences", "Folder: SystemLog", "");
	if(m_strSystemLoggingDir.IsEmpty()) {
		m_strSystemLoggingDir="C:\\SystemLog";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Folder: SystemLog", m_strSystemLoggingDir);
	}
	_mkdir(m_strSystemLoggingDir); 

	m_strAuditLoggingDir = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Folder: AuditTrail", "");
	if(m_strAuditLoggingDir.IsEmpty()) {
		m_strAuditLoggingDir="C:\\AuditTrailLog";
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Folder: AuditTrail", m_strAuditLoggingDir);
	}
	_mkdir(m_strAuditLoggingDir); 

	m_LotSummaryRootDir = GetRegistryEntry(READ_REGPATH + "Preferences", "Folder: Statistics", "");
	if (m_LotSummaryRootDir.IsEmpty()) {
		m_LotSummaryRootDir = "C:\\Sherlock_Spyglass Stats";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Folder: Statistics", m_LotSummaryRootDir);
	}
	_mkdir(m_LotSummaryRootDir);

	m_strParmFolder.Format("%s\\ParmDatabase", m_LotSummaryRootDir);
	_mkdir(m_strParmFolder);

	m_ImageSaveFolder = "void";
	m_ImageSaveFolder = GetRegistryEntry(READ_REGPATH + "Preferences", "Folder: Image Save", m_ImageSaveFolder);
	if (m_ImageSaveFolder.CompareNoCase("void")==0) {
		m_ImageSaveFolder = "C:\\Sherlock_Spyglass Images";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Folder: Image Save", m_ImageSaveFolder);
	}
	_mkdir(m_ImageSaveFolder);

	m_nSequenceCounter = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "InspSequenceCounter", -1);
	if(m_nSequenceCounter==-1) {
		m_nSequenceCounter = 1;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "InspSequenceCounter", m_nSequenceCounter);
	}

	m_bSaveOnlineFailImages = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "SaveFailImages", -1);
	if(m_bSaveOnlineFailImages == -1) {
		m_bSaveOnlineFailImages = TRUE;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SaveFailImages", m_bSaveOnlineFailImages);
	}
	m_bSaveOnlineJpegImages = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "SaveJpegImages", -1);
	if(m_bSaveOnlineJpegImages == -1) {
		m_bSaveOnlineJpegImages = FALSE;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SaveJpegImages", m_bSaveOnlineJpegImages);
	}

	m_bSaveOnlineBMPImages = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "SaveBMPImages", -1);
	if (m_bSaveOnlineBMPImages == -1) {
		m_bSaveOnlineBMPImages = TRUE;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SaveBMPImages", m_bSaveOnlineBMPImages);
	}

	m_bSaveOnlinePassImages = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "SavePassImages", -1);
	if(m_bSaveOnlinePassImages==-1) {
		m_bSaveOnlinePassImages = TRUE;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SavePassImages", m_bSaveOnlinePassImages);
	}


	m_nSaveJpegQualityImages = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "SaveJpegQuality", -1);
	if (m_nSaveJpegQualityImages == -1) {
		m_nSaveJpegQualityImages = 50;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SaveJpegQuality", m_nSaveJpegQualityImages);
	}

	m_bOnlineContinueTopImageInsp = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "ContinueTopImageInsp", -1);
	if(m_bOnlineContinueTopImageInsp == -1) {
		m_bOnlineContinueTopImageInsp = FALSE;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "ContinueTopImageInsp", m_bOnlineContinueTopImageInsp);
	}

	if(m_bSaveOnlinePassImages || m_bSaveOnlineFailImages || m_bSaveOnlineJpegImages) {
		if(!CheckDiskFreeSpace()) {
			m_bSaveOnlinePassImages = FALSE;
			m_bSaveOnlineFailImages = FALSE;
			m_bSaveOnlineJpegImages = FALSE;
		}
	}

	m_bInspectionParallelism = GetRegistryEntry(READ_REGPATH + "Preferences", "Inspection Parallelism: Enable", -1);
	if(m_bInspectionParallelism == -1) {
		m_bInspectionParallelism = 1;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Inspection Parallelism: Enable", m_bInspectionParallelism);
	}

	CString str1, str2 = "void";
	str1 = GetRegistryEntry(READ_REGPATH + "Preferences", "Stats: Display Reset @(hhmm)", str1);
	if (str1.CompareNoCase("void")==0) {
		str1 = "0600";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Stats: Display Reset @(hhmm)", str1);
	}
	int nMinutes = __min(atoi(str1.Right(2).GetBuffer(0)), 59);
	int nHours = __min(atoi(str1.Mid(0, str1.GetLength()-2).GetBuffer(0)), 23);
	str2.Format("%02u%02u", nHours, nMinutes);
	if(str1 != str2)
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Stats: Display Reset @(hhmm)", str2);

	m_LotSummary.nDisplayStatsResetMin = nHours*60 + nMinutes;

	nValue = GetRegistryEntry(READ_REGPATH + "Preferences", "Stats: Recycle Lot After (days)", -1);
	if(nValue==-1) {
		nValue = 30;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Stats: Recycle Lot After (days)", nValue);
	}
	m_LotSummary.nLotRecycleDays = nValue;

	m_UserAccInfo.m_nLogoutTime = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "UserAutoLogoutMins", -1);
	if(m_UserAccInfo.m_nLogoutTime == -1) {
		m_UserAccInfo.m_nLogoutTime = 3;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "UserAutoLogoutMins", m_UserAccInfo.m_nLogoutTime);
	}
	
	//m_UserAccInfo.m_nLogTimeExt = 0;
	//m_UserAccInfo.m_nLogTimeExt = GetRegistryEntry(READ_REGPATH + "Preferences", "LogTimeExtInfo", m_UserAccInfo.m_nLogTimeExt);
	//PutRegistryEntry(WRITE_REGPATH + "Preferences","LogTimeExtInfo", m_UserAccInfo.m_nLogTimeExt);

	m_bEnableSimulationMode = GetRegistryEntry(READ_REGPATH + "Preferences", "SimulationMode: Enable", -1);
	if(m_bEnableSimulationMode == -1) {
		m_bEnableSimulationMode = 1;
		PutRegistryEntry(WRITE_REGPATH + "Preferences","SimulationMode: Enable", m_bEnableSimulationMode);
	}

	m_nMaxSimulationBuffers = 10;

	m_bEnableManagePackageId = GetRegistryEntry(READ_REGPATH + "Preferences", "Manage Package Id: Enable", -1);
	if (m_bEnableManagePackageId == -1) {
		m_bEnableManagePackageId = 1;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Manage Package Id: Enable", m_bEnableManagePackageId);
	}


	m_bEnableTopDatumPosToScanTHR = GetRegistryEntry(READ_REGPATH + "Preferences", "Use Top Station Datum Location to Scan THR", -1);
	if (m_bEnableTopDatumPosToScanTHR == -1) {
		m_bEnableTopDatumPosToScanTHR = 1;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Use Top Station Datum Location to Scan THR", m_bEnableTopDatumPosToScanTHR);
	}

	//m_nMaxSimulationBuffers = GetRegistryEntry(READ_REGPATH + "Preferences", "SimulationMode: MaxBuffers", m_nMaxSimulationBuffers);
	//PutRegistryEntry(WRITE_REGPATH + "Preferences", "SimulationMode: MaxBuffers", m_nMaxSimulationBuffers);

	//m_strSimulationImagesPath = "void";
	//m_strSimulationImagesPath = GetRegistryEntry(READ_REGPATH + "Preferences", "Folder: SimulationMode", m_strSimulationImagesPath);
	//if (m_strSimulationImagesPath.CompareNoCase("void")==0) {
	//	m_strSimulationImagesPath = "E:\\TestImagesOnline";
	//	PutRegistryEntry(WRITE_REGPATH + "Preferences", "Folder: SimulationMode", m_strSimulationImagesPath);
	//}

	//m_nUnitIndex = -1;
	//m_nUnitIndex = GetRegistryEntry(READ_REGPATH + "Preferences", "Unit (0-pixel; 1-micron; 2-mm)", m_nUnitIndex);
	//if(m_nUnitIndex==-1) {
	//	m_nUnitIndex = 1;
	//	PutRegistryEntry(WRITE_REGPATH + "Preferences", "Unit (0-pixel; 1-micron; 2-mm)", m_nUnitIndex);
	//}
	m_nUnitIndex = 0;
	m_nCurrentLightControllerIndex = 0;

	m_bClassPriorityRealtime = 1;
	//m_bClassPriorityRealtime = GetRegistryEntry(READ_REGPATH + "Preferences","Realtime Priority: Enable", m_bClassPriorityRealtime);
	//PutRegistryEntry(WRITE_REGPATH + "Preferences","Realtime Priority: Enable", m_bClassPriorityRealtime);

	m_nOperationMode = GetRegistryEntry(READ_REGPATH + "Preferences", "Operation Mode", -1);
	if(m_nOperationMode == -1) {
		m_nOperationMode = 1;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Operation Mode", m_nOperationMode);
	}

	m_bAllocFailedImageBuffer = GetRegistryEntry(READ_REGPATH + "Preferences", "Preserve FailedImages History", -1);
	if(m_bAllocFailedImageBuffer == -1) {
		m_bAllocFailedImageBuffer = 0;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Preserve FailedImages History", m_bAllocFailedImageBuffer);
	}

	m_bInspectionAffinity = GetRegistryEntry(READ_REGPATH + "Preferences", "Inspection Affinity: Enable", -1);
	if(m_bInspectionAffinity == -1) {
		m_bInspectionAffinity = 1;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Inspection Affinity: Enable", m_bInspectionAffinity);
	}

	m_nProductCodeSize = GetRegistryEntry(READ_REGPATH + "Preferences", "Product Code Size: ", -1);
	if(m_nProductCodeSize == -1) {
		m_nProductCodeSize = 5;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Product Code Size: ", m_nProductCodeSize);
	}

	m_nBarcodeSize = GetRegistryEntry(READ_REGPATH + "Preferences", "Barcode Size: ", -1);
	if(m_nBarcodeSize == -1) {
		m_nBarcodeSize = 17;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Barcode Size: ", m_nBarcodeSize);
	}

	m_bSynchronousGrabEnable = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "SynchronousGrab: Enable", -1);
	if(m_bSynchronousGrabEnable == -1) {
		m_bSynchronousGrabEnable = 0;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SynchronousGrab: Enable", m_bSynchronousGrabEnable);
	}

	m_nSystemRestartCounter = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "SystemRestartCounter", 0);
	m_nSystemRestartCounter++;
	if(m_nSystemRestartCounter > 20)
		m_nSystemRestartCounter = 0;
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SystemRestartCounter",m_nSystemRestartCounter);

	m_bSaveCrashImages = GetRegistryEntry(READ_REGPATH + "Preferences","Save Crash Images", -1);
	if(m_bSaveCrashImages == -1) {
		m_bSaveCrashImages = FALSE;
		PutRegistryEntry(WRITE_REGPATH + "Preferences","Save Crash Images",m_bSaveCrashImages);
	}

	/////////////////////////////////////////////
	nNoOfResetPerDay = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "LotNoOfReset", -1);
	if(nNoOfResetPerDay == -1) {
		nNoOfResetPerDay = 0;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "LotNoOfReset", nNoOfResetPerDay);
	}

	nValue = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "LotResetTime1", -1);
    if(nValue == -1) {
        nValue = 0;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "LotResetTime1", nValue);
	}
    tLotResetTime1.wHour = nValue;

	nValue = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "LotResetTime2", -1);
    if(nValue == -1) {
        nValue = 0;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "LotResetTime2", nValue);
	}
    tLotResetTime2.wHour = nValue;
	
	m_strProductCode = "----NoBarcode----";
	m_strMachineID = "void";
	m_strMachineID = GetRegistryEntry(READ_REGPATH + "Preferences", "Machine ID", m_strMachineID);
	if (m_strMachineID.CompareNoCase("void")==0) {
		m_strMachineID = "Sherlock_Spyglass #1";
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Machine ID", m_strMachineID);
	}
	m_LotSummary.strMachineID = m_strMachineID;


	m_bAutoResizeImage = GetRegistryEntry(READ_REGPATH + "Preferences", "Resize Images: Enable", TRUE);
	if(!m_bAutoResizeImage)
		m_bAutoResizeImageToMidRes = GetRegistryEntry(READ_REGPATH + "Preferences", "Resize Images ToMidRes: Enable", TRUE);
	m_bAutoSaveResizedImage = GetRegistryEntry(READ_REGPATH + "Preferences", "Resize Images: AutoSave", FALSE);

	m_nFirstGrabDelay = GetRegistryEntry(READ_REGPATH + "Preferences", "Sequence: First Grab Delay", -1);
	if(m_nFirstGrabDelay == -1) {
		m_nFirstGrabDelay = 200;
		PutRegistryEntry(READ_REGPATH + "Preferences", "Sequence: First Grab Delay", m_nFirstGrabDelay);
	}

	m_nSignalScanFrequency = GetRegistryEntry(READ_REGPATH + "Preferences", "Sequence: Signal Scan Frequency", -1);
	if(m_nSignalScanFrequency==-1) {
		m_nSignalScanFrequency = 10;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Sequence: Signal Scan Frequency", m_nSignalScanFrequency);
	}

	m_bStoredImagesLoop = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "StoredImagesLoop", -1);
	if(m_bStoredImagesLoop == -1) {
		m_bStoredImagesLoop = FALSE;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "StoredImagesLoop", m_bStoredImagesLoop);
	}
	
	m_bZoomAll = FALSE;
	m_bZoomAll = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Zoom All: Enable", 1);
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Zoom All: Enable", m_bZoomAll);

	m_nXAxisSettlingTime = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "X-Axis Settling Time", -1);
	if(m_nXAxisSettlingTime==-1) {
		m_nXAxisSettlingTime = 1000;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "X-Axis Settling Time", m_nXAxisSettlingTime);
	}

	m_nYAxisSettlingTime = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Y-Axis Settling Time", -1);
	if(m_nYAxisSettlingTime==-1) {
		m_nYAxisSettlingTime = 1000;
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Y-Axis Settling Time", m_nYAxisSettlingTime);
	}

	m_strLoadMultiImagesFolderPath = "void";
	strTemp.Format("Folder: StoredMultiImages");
	m_strLoadMultiImagesFolderPath = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", strTemp, m_strLoadMultiImagesFolderPath);
	if(m_strLoadMultiImagesFolderPath.CompareNoCase("void")==0) {
		m_strLoadMultiImagesFolderPath = "C:\\";
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", strTemp, m_strLoadMultiImagesFolderPath);
	}

	m_bBypassCurtainSensor = GetRegistryEntry(READ_REGPATH + "Preferences", "ByPass Curtain Sensor: Enable", -1);
	if(m_bBypassCurtainSensor == -1) {
		m_bBypassCurtainSensor = 0;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "ByPass Curtain Sensor: Enable", m_bBypassCurtainSensor);
	}

	m_bEnableReScanOption = GetRegistryEntry(READ_REGPATH + "Preferences", "Enable ReScan Option", -1);
	if (m_bEnableReScanOption == -1) {
		m_bEnableReScanOption = 0;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Enable ReScan Option", m_bEnableReScanOption);
	}

	m_bEnableRepeatScanOption = GetRegistryEntry(READ_REGPATH + "Preferences", "Do not Consider Statistic Data on Repeat Scan", -1);
	if (m_bEnableRepeatScanOption == -1) {
		m_bEnableRepeatScanOption = 0;
		PutRegistryEntry(WRITE_REGPATH + "Preferences", "Do not Consider Statistic Data on Repeat Scan", m_bEnableRepeatScanOption);
	}

	m_bEnableResourceDisplay = TRUE;
	m_bEnableResourceDisplay = GetRegistryEntry(READ_REGPATH + "Preferences", "Enable Resource Dialog", m_bEnableResourceDisplay);
	PutRegistryEntry(WRITE_REGPATH + "Preferences", "Enable Resource Dialog", m_bEnableResourceDisplay);

	m_bdisplayResourceWindow = true;
	m_bdisplayResourceWindow = GetRegistryEntry(READ_REGPATH + "Preferences", "Display Available Resource Dialog", m_bdisplayResourceWindow) == 1 ? true : false;
	PutRegistryEntry(WRITE_REGPATH + "Preferences", "Display Available Resource Dialog", m_bdisplayResourceWindow);
	
	m_bZoneMapping = false;
	m_bZoneMapping = GetRegistryEntry(READ_REGPATH + "Preferences", "Die Mapping Number order", m_bZoneMapping) == 1 ? true : false;
	PutRegistryEntry(WRITE_REGPATH + "Preferences", "Die Mapping Number order", m_bZoneMapping);


	m_bSubstrateMappingStyle = false;
	m_bSubstrateMappingStyle = GetRegistryEntry(READ_REGPATH + "Preferences", "Display Each Die Status", m_bSubstrateMappingStyle) == 1 ? true : false;
	PutRegistryEntry(WRITE_REGPATH + "Preferences", "Display Each Die Status", m_bSubstrateMappingStyle);


	// Secs/Gem Communication
	if (m_bIsMasterApp) {
		_secsgemEnableCommunication = false;
		_secsgemEnableCommunication = GetRegistryEntry(READ_REGPATH + "SECS/GEM", "Enable SECS/GEM Communication", _secsgemEnableCommunication) == 1 ? true : false;
		PutRegistryEntry(WRITE_REGPATH + "SECS/GEM", "Enable SECS/GEM Communication", _secsgemEnableCommunication);

		nGEMControlState = 4;
		nGEMControlState = GetRegistryEntry(READ_REGPATH + "SECS/GEM", "SECS/GEM Control State", nGEMControlState);
		PutRegistryEntry(WRITE_REGPATH + "SECS/GEM", "SECS/GEM Control State", nGEMControlState);

		/* If SECS/GEM Enable --> Need to Check Admin Rights*/
		if (_secsgemEnableCommunication) {
			BOOL fRet = FALSE;
			HANDLE hToken = NULL;
			if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
				TOKEN_ELEVATION Elevation;
				DWORD cbSize = sizeof(TOKEN_ELEVATION);
				if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
					fRet = Elevation.TokenIsElevated;
				}
			}
			if (hToken) {
				CloseHandle(hToken);
			}

			if (!fRet) {
				AfxMessageBox("Please Run Application as Administrator To Run SECS/GEM Mode!!!");
				return -1;
			}
		}
	}

	
	
	


	/*m_bDryRunMachine = FALSE;
	m_bDryRunMachine = GetRegistryEntry(READ_REGPATH + "Preferences","Machine Dry Run: Enable", m_bDryRunMachine);
	PutRegistryEntry(WRITE_REGPATH + "Preferences", "Machine Dry Run: Enable", m_bDryRunMachine);*/

	m_bLoadMultiImagesFromFile = FALSE;

	m_pLightCtrlDlg = NULL;
	m_pMotionControl = NULL;
	bOpenedMotionDialog = FALSE;

	m_pIoHardwareDlg = NULL;
	pIOControlDialog = NULL;

	m_pWndPVIDefectParameters = NULL;
	m_pScanBarcodeDlg = NULL;
	m_bLightCtrlDlgOpen = FALSE;
	m_bLoadComplete = FALSE;
	m_pCalibrationDlg = NULL;
	//m_pAuditTrailDlg = NULL;
	m_pDisplayMsgStatus = NULL;
	m_pInfoMsgDlg = NULL;
	

	m_bTeachPass = FALSE;
	m_bOnline = FALSE;
	m_bTeaching = FALSE;
	m_bInspecting = FALSE;
	m_bBarcodeScanning = FALSE;
	m_bRecipeFound = FALSE;
	m_bByPassResult = FALSE;

	for(int i=0;i<MAX_TRACKS;i++) {
		m_nFailImageCounter[i]=0;
		m_nCrashImageCounter[i]=0;
	}
	m_nDeviceCounter=0;
	m_nPPM = 0;
	m_nUPH = 0;
	m_RuntimeDisplayEnable=TRUE;
	m_bTrackInspSeq = FALSE; 
	m_bSuperUserMode = FALSE;
	m_nOutData = 0xff;

	m_UserAccInfo.UserAccount = NULL;
	m_UserAccInfo.strUserNames = NULL;

	m_strAppId = "Sherlock Holmes Spyglass Bot";

	m_UserAccInfo.m_strCurUserName = "Operator";	
	m_UserAccInfo.DefaultUser.strUsername = "engineer";
	CString strPswd = "SherlockHolmesBot";
	CString strEncr = m_strAppId;
	strEncr += strPswd;
	CString strOutput = EncryptDecrypt(strEncr);
	m_UserAccInfo.DefaultUser.strPassword = m_UserLog.EncodePassWord(&strOutput);
	m_UserAccInfo.DefaultUser.strAccLevel = "Administrator";
	m_UserAccInfo.m_bAccessLevel = false;
	m_UserAccInfo.m_nCurrentAccessLevel = 0;

	

	m_LotSummaryLotIDDir = "";
	m_bStream = FALSE;
	m_bSnap = FALSE;
	m_bCalib = FALSE;
	m_bCalibrationOpen = FALSE;
	m_bAllowStream = FALSE;
	m_nNavigateStaticImgTrack = 0;
	m_bInspSequenceOffline = FALSE;
	m_bModifierView = FALSE;

	/*m_nDevicePitchX = 0;
	m_nDevicePitchY = 0;
	m_nDeviceOffsetX = 0;
	m_nDeviceOffsetY = 0;*/
	//for(int i=0; i<MAX_DIRECTION; i++) {
	//	m_nNoOfBlock[i] = 0;
	//	m_dBlockPitch[i] = 0;
	//	m_nDeviceNoPerBlock[i] = 0;
	//	m_dDevicePitch[i] = 0;
	//	m_dDeviceScanArea[i] = 0;
	//	//m_dDeviceOffset[i] = 0;
	//	m_dDeviceOverlap[i] = 0;
	//}
	/*for(int nFOV=0; nFOV<MAX_FOV; nFOV++) {
		m_nXAxisPosition[nFOV] = 0;
		m_nYAxisPosition[nFOV] = 0;
	}*/

	return 0;
}

BOOL CApplication::InitInstance()
{
	///_CrtSetDbgFlag(_CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("MonsterVision"));
	///LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	CSplashScreen::ShowSplashScreen();

	// Parse command line for standard shell commands, DDE, file open
	CCustomCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if(LoadDefaults(cmdInfo.m_nArg[0], cmdInfo.m_nArg[1]))
		return FALSE;
	m_nFirstTrackNo = cmdInfo.m_nArg[2];

	/*if(!SetParameters())
		return FALSE;*/


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_EVAPPLICATIONTYPE,
		//RUNTIME_CLASS(CAppDoc),
		RUNTIME_CLASS(CEvTabDoc),
		RUNTIME_CLASS(CEvChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CAppTabView));
	if (!pDocTemplate)
		return FALSE;


	AddDocTemplate(pDocTemplate);
	
	CWinAppEx::m_bLoadWindowPlacement = FALSE;
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if(!pMainFrame)
		return FALSE;

	pMainFrame->LoadDefaults(READ_REGPATH);
	if(!pMainFrame->LoadFrame(IDR_MAINFRAME)) {
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	pMainFrame->m_strLotFolder = m_LotSummaryRootDir;
	/*((CMainFrame*) m_pMainWnd)->ShowStatsPane();	
	((CMainFrame*) m_pMainWnd)->ShowPreviousImages();
	((CMainFrame*) m_pMainWnd)->ShowDebugView();
*/
	if(!SetParameters())
		return FALSE;

	GetKeyboardManager()->ResetAll();
	
	if (!CheckForHalconnLicense()) {
		return FALSE;
	}

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	CRect rectClient;

	GetClientRect(((CMDIFrameWndEx *)m_pMainWnd)->m_hWndMDIClient,&rectClient);
	int cx = rectClient.Width()*0.02;
	int cy = rectClient.Height() /0.9;
//////////////
	int arrx[3] = { 0, cx,  cx<<1};
	int arry[3] = { 0,  0,  0 };
	int arrcx[3] = { cx, cx, cx,};
	int arrcy[3] = { cy, cy, cy,};

//////////////
	((CMDIFrameWndEx *)m_pMainWnd)->MoveWindow(arrx[0], arry[0], arrcx[0], arrcy[0],TRUE);
	m_pFrames[0] = (CMDIChildWndEx *) ((CMDIFrameWndEx *)m_pMainWnd)->GetActiveFrame();

	for(int i=1; i<m_nTotalViews; i++) {
		if (pDocTemplate->OpenDocumentFile(NULL)==0)
			return FALSE;
		m_pFrames[i] = (CMDIChildWndEx *) ((CMDIFrameWndEx *)m_pMainWnd)->GetActiveFrame();
	
	}

	motionParameters.LoadMotionParm();
	motionParameters.LoadScanningParm();
	motionParameters.LoadDeviceParameters();
	motionParameters.LoadZTeachPosition();
	motionParameters.loadScanningOptions();

	motionParameters.CreateScanPositions();
	InitializeDocView();

//////////////
	////Check app license
	if(!CheckApplicationLicense())
		return FALSE;


	
	/*if (!ReloadWindowPlacement(((CMainFrame*) m_pMainWnd)))
	{
		((CMainFrame*) m_pMainWnd)->ShowWindow(m_nCmdShow);
		((CMainFrame*) m_pMainWnd)->UpdateWindow();
	}*/
	return InitProcess();
}

int CApplication::ExitInstance()
{

//	timeEndPeriod(2);

	QRBarcodeReader.CloseConnection();
	BarcodeReader.CloseConnection();

	m_bExitApp = TRUE;

	if(m_bOnline)
		SetVisOnLineState(FALSE);

	SetEvent(m_EventRun);
	OutputDebugLogTo(7, TRUE, "in exit instance 1");
	//((CStatisticsView*)m_pFrames[2]->GetActiveView())->RecordLotStats(TRUE);

//	SetEvent(m_eventOnline);
//	SetEvent(m_EventMotionSensor); //Close Motion Thread
//	SetEvent(m_EventMotionSensorStart);//Close Motion Thread
	SetEvent(m_EventRefreshLight);//// close thread
	SetEvent(m_EventCommSendMsg);//Close Send Comm Thread;
//	SetEvent(m_EventIOtest);
	OutputDebugLogTo(7, TRUE, "in exit instance 2");

	SetEvent(m_eventStartScan);

	SetEvent(eventRunZSensorSequence);
	SetEvent(m_eventZSensorTrigCount);
	SetEvent(m_eventZAxizFocus);
	SetEvent(m_eventYAxisDatumOffset);
	SetEvent(eventSaveImage);
	SetEvent(eventDeepLearnSaveImage);
//	SetEvent(eventClearMemory);
	SetEvent(eventRunLatching);

	OutputDebugLogTo(7, TRUE, "in exit instance 3");

	for(int i = 0; i < m_nTotalTracks; i++) {
		SetEvent(m_pTracks[i].m_eventGrabbing);

		std::for_each(m_pTracks[i].eventDoInspection.begin(), m_pTracks[i].eventDoInspection.end(),
			[](HANDLE & handle) { SetEvent(handle); });

		std::for_each(m_pTracks[i].eventInspectionDone.begin(), m_pTracks[i].eventInspectionDone.end(),
			[](HANDLE & handle) { SetEvent(handle); });


		//Duplicate Inspection Event
		std::for_each(m_pTracks[i].eventDuplicateInspection.begin(), m_pTracks[i].eventDuplicateInspection.end(),
			[](HANDLE & handle) { SetEvent(handle); });

		std::for_each(m_pTracks[i].eventDuplicateInspectionDone.begin(), m_pTracks[i].eventDuplicateInspectionDone.end(),
			[](HANDLE & handle) { SetEvent(handle); });

//		SetEvent(m_pTracks[i].eventDoInspection);
//		SetEvent(m_pTracks[i].eventInspectionDone);
	/*	SetEvent(m_pTracks[i].m_eventScanning);*/
		SetEvent(m_pTracks[i].m_eventTrigCount);
	}
	
	OutputDebugLogTo(7, TRUE, "in exit instance 4");

	if (m_bStartScanThread) {
		if (WaitForSingleObject(eventScanThreadKilled, 3000) != WAIT_OBJECT_0)
			OutputDebugLogTo(0, TRUE, "----- ERROR: Timeout for waiting Scanning thread to be closed");
	}

	OutputDebugLogTo(7, TRUE, "in exit instance 5");

	if (WaitForSingleObject(eventSaveImageKilled, 3000) != WAIT_OBJECT_0)
		OutputDebugLogTo(0, TRUE, "----- ERROR: Timeout for waiting Image Saving thread to be closed");


	if (WaitForSingleObject(eventDeepLearnSaveImageKilled, 3000) != WAIT_OBJECT_0)
		OutputDebugLogTo(0, TRUE, "----- ERROR: Timeout for waiting Deep learning Image Saving thread to be closed");

	if (pGlobalBackgroundThread != NULL) {
		delete pGlobalBackgroundThread;
		pGlobalBackgroundThread = NULL;
	}
	
	AddLogEvent("I/O input signal can't be readable. Check I/O status");

	OutputDebugLogTo(7, TRUE, "in exit instance 4.9");

	if (pErrorMsgDlg != NULL) {
		delete pErrorMsgDlg;
		pErrorMsgDlg = NULL;
	}

	OutputDebugLogTo(7, TRUE, "in exit instance 6");
	
	for (int i = 0; i<m_nTotalTracks; i++) {

		if (WaitForSingleObject(m_pTracks[i].eventHardwareGrabThreadKilled, 5000) != WAIT_OBJECT_0)
			OutputDebugLogTo(0, TRUE, "----- ERROR: Timeout for waiting Hardware grabbing thread to be closed for %d camera", i);
	}
	
	OutputDebugLogTo(7, TRUE, "in exit instance 6.6");

	if(m_nCamLibraryTrackIndex >= 0) {
		for (int i = 0; i < m_nTotalTracks; i++)
			ReleaseCamera(&m_pTracks[i].m_FGResource);
//		ReleaseCameraCard(&m_pTracks[0].m_FGResource);
	}

	
	
	if(m_LightModule.hDLL) {
		m_LightModule.lpFreeIODLL(0);
		AfxFreeLibrary(m_LightModule.hDLL);
	}

	if(m_CommModule.hDLL) {
		m_CommModule.lpFreeIODLL(0);
		AfxFreeLibrary(m_CommModule.hDLL);
	}
	OutputDebugLogTo(7, TRUE, "in exit instance 7");

	if (m_pIoHardwareDlg != NULL) {
		if (IsWindow(m_pIoHardwareDlg->m_hWnd))
			m_pIoHardwareDlg->DestroyWindow();
		delete m_pIoHardwareDlg;
		m_pIoHardwareDlg = NULL;
	}
	OutputDebugLogTo(7, TRUE, "in exit instance 8");

	if (pIOControlDialog != NULL) {
		pIOControlDialog->DestroyWindow();
		delete pIOControlDialog;
		pIOControlDialog = NULL;
	}
	OutputDebugLogTo(7, TRUE, "in exit instance 8.1");



	 if (m_pMotionControl != NULL) {
		 if (IsWindow(m_pMotionControl->m_hWnd))
			 m_pMotionControl->DestroyWindow();
		 delete m_pMotionControl;
		 m_pMotionControl = NULL;
	 }

	OutputDebugLogTo(7, TRUE, "in exit instance 9");
	if (m_MotorResource.isDllOpened()) {

		OutputDebugLogTo(7, TRUE, "in exit instance 9.1");

		motionParameters.FreeMotionCard();
		OutputDebugLogTo(7, TRUE, "in exit instance 9.2");

		ioControl.ResetIO();
		OutputDebugLogTo(7, TRUE, "in exit instance 9.5");

		//	motionParameters.SetServoOffAllAxis();
		//	ioControl.SetK1ServoOff();
		//	ioControl.SetK2ServoOff();

		m_MotorResource.ReleaseCard(0);
		OutputDebugLogTo(7, TRUE, "in exit instance 9");

		m_MotorResource.ReleaseIOCard(0);
		m_MotorResource.FreeMotionDLL();
		m_MotorResource.closeDll();
	}

	if (m_ScaleResource.isDllOpened()) {
		m_ScaleResource.ReleaseScaleCard();
		m_ScaleResource.FreeScaleDll();
		m_ScaleResource.closeDll();
	}

	OutputDebugLogTo(7, TRUE, "in exit instance 10");


	if (m_pDispOnlinePanel != NULL) {
		if (IsWindow(m_pDispOnlinePanel->m_hWnd))
			m_pDispOnlinePanel->DestroyWindow();
		delete m_pDispOnlinePanel;
		m_pDispOnlinePanel = NULL;
	}

	if (m_pImageQueue != NULL) {
		delete m_pImageQueue;
		m_pImageQueue = NULL;
	}

	 OutputDebugLogTo(7, TRUE, "in exit instance 11");

	if (m_TrigangualrsensorInstance != NULL) {
		m_TriangularSensor.ReleaseSensor(m_TrigangualrsensorInstance);
		m_TrigangualrsensorInstance = NULL;
	}
	
	DeleteCriticalSection(&m_CriticalSection);
	DeleteCriticalSection(&m_csOnline);
	DeleteCriticalSection(&m_csFileAccess);
	DeleteCriticalSection(&m_csOutputData);
	DeleteCriticalSection(&m_CriticalSectionSaveImage);
	DeleteCriticalSection(&m_CriticalSectionDeepLearnSaveImage);
	DeleteCriticalSection(&m_CriticalSectionTOPDieShiftedInfo);

	//To Save Image Counter
	//CString strTemp;
	//for(nTrack = 0;nTrack<5;nTrack++){
		//strTemp.Format("Total Inspect Count Trk%d",nTrack);
		//PutRegistryEntry(WRITE_REGPATH + "Preferences",strTemp,m_pTracks[nTrack].m_nTotalInspCount);
	//}
	
//	KillDataBase();
	 OutputDebugLogTo(7, TRUE, "in exit instance 12");

	if (m_UserAccInfo.UserAccount != NULL) {
		delete[] m_UserAccInfo.UserAccount;
		m_UserAccInfo.UserAccount = NULL;
	}

	if (m_UserAccInfo.strUserNames != NULL) {
		delete[] m_UserAccInfo.strUserNames;
		m_UserAccInfo.strUserNames = NULL;
	}
	OutputDebugLogTo(7, TRUE, "in exit instance 13");


	if (m_pLightCtrlDlg != NULL) {
		if (IsWindow(m_pLightCtrlDlg->m_hWnd))
			m_pLightCtrlDlg->DestroyWindow();
		delete m_pLightCtrlDlg;
		m_pLightCtrlDlg = NULL;
	}
	 OutputDebugLogTo(7, TRUE, "in exit instance 14");

	if (m_pScanBarcodeDlg != NULL) {
		if (IsWindow(m_pScanBarcodeDlg->m_hWnd))
			m_pScanBarcodeDlg->DestroyWindow();
		delete m_pScanBarcodeDlg;
		m_pScanBarcodeDlg = NULL;
	}
	 //if(m_pAuditTrailDlg != NULL)
		//delete m_pAuditTrailDlg;
	
	OutputDebugLogTo(7, TRUE, "in exit instance 14.5");


	if (m_pTriSensorDlg != NULL) {
		 if (IsWindow(m_pTriSensorDlg->m_hWnd))
			 m_pTriSensorDlg->DestroyWindow();
		 delete m_pTriSensorDlg;
		 m_pTriSensorDlg = NULL;
	 }


	


	 //if (m_pIOHardwareStatusDlg != NULL) {
		// if (IsWindow(m_pIOHardwareStatusDlg->m_hWnd))
		//	 m_pIOHardwareStatusDlg->DestroyWindow();
		// delete m_pIOHardwareStatusDlg;
		// m_pIOHardwareStatusDlg = NULL;
	 //}
		 OutputDebugLogTo(7, TRUE, "in exit instance 15");


	 if(m_pWndPVIDefectParameters != NULL){
		if(IsWindow(m_pWndPVIDefectParameters->m_hWnd))
			m_pWndPVIDefectParameters->DestroyWindow();
		delete m_pWndPVIDefectParameters;
		m_pWndPVIDefectParameters = NULL;
	}
	 OutputDebugLogTo(7, TRUE, "in exit instance 16");


	if(m_pDisplayMsgStatus != NULL) {
		 if(IsWindow(m_pDisplayMsgStatus->m_hWnd))
			 m_pDisplayMsgStatus->DestroyWindow();
		delete m_pDisplayMsgStatus;
		m_pDisplayMsgStatus = NULL;
	}
	OutputDebugLogTo(7, TRUE, "in exit instance 17");

	if (m_pInfoMsgDlg != NULL) {
		if (IsWindow(m_pInfoMsgDlg->m_hWnd))
			m_pInfoMsgDlg->DestroyWindow();
		delete m_pInfoMsgDlg;
		m_pInfoMsgDlg = NULL;
	}
	OutputDebugLogTo(7, TRUE, "in exit instance 18");

	if(m_pCalibrationDlg != NULL){
		delete m_pCalibrationDlg;
		m_pCalibrationDlg = NULL;
	}

	if (m_pDistanceCalibration != NULL) {
		delete m_pDistanceCalibration;
		m_pDistanceCalibration = NULL;
	}

	if (m_pJigLeveling != NULL) {
		if (IsWindow(m_pJigLeveling->m_hWnd))
			m_pJigLeveling->DestroyWindow();
		delete m_pJigLeveling;
		m_pJigLeveling = NULL;
	}

	if (m_pTeachZSensorPosTool != NULL) {
		if (IsWindow(m_pTeachZSensorPosTool->m_hWnd))
			m_pTeachZSensorPosTool->DestroyWindow();
		delete m_pTeachZSensorPosTool;
		m_pTeachZSensorPosTool = NULL;
	}

	//for (int nDie = 0; nDie < m_nNoOfDies; nDie++) {
	//	for (int nFOV = 0; nFOV < MAX_FOV/*m_nNoOfFov*/; nFOV++) {
	//		m_colorFovDieMapping[nDie][nFOV].DeleteObject();
	//	}
	//}
	OutputDebugLogTo(7, TRUE, "in exit instance 20");

	if(m_pTracks)
		delete[] m_pTracks;

	AfxOleTerm(FALSE);
	OutputDebugLog("Sherlock_Spyglass Vision Application closed");

	m_UserAccInfo.m_nCurrentAccessLevel = 2;
	//m_strSideCameraName.RemoveAll();
	m_strTopCameraName.RemoveAll();
	//m_strBottomCameraName.RemoveAll();
	m_strLightChannelName.RemoveAll();

	//if (!m_ParametersDefectCode.IsEmpty()) {
	//	for (int i = 0; i<m_ParametersDefectCode.GetCount(); i++)
	//		delete m_ParametersDefectCode[i];
	//	m_ParametersDefectCode.RemoveAll();
	//}

	OutputDebugLogTo(7, TRUE, "in exit instance 21");

	if (m_pSequenceOptionDlg != NULL) {
		delete m_pSequenceOptionDlg;
		m_pSequenceOptionDlg = NULL;
	}

	if (m_pVerificationMode != NULL) {
		delete m_pVerificationMode;
		m_pVerificationMode = NULL;
	}

	if (m_pProgressDlg != NULL) {
		m_pProgressDlg->DestroyWindow();
		delete m_pProgressDlg;
		m_pProgressDlg = NULL;
	}

	OutputDebugLogTo(0, TRUE, "Good bye!");

	//LaunchWindowsExplorer();
	return CWinAppEx::ExitInstance();
}

// CApplication message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_LBUTTONUP()
//	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//if(!(GetKeyState(VK_SHIFT) & 0x8000))
		//GetDlgItem(IDC_VERSION_STATIC)->SetWindowText(GetCurrentVersion("Ev_InCuvette"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CApplication::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CApplication customization load/save methods

void CApplication::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CApplication::LoadCustomState()
{
}

void CApplication::SaveCustomState()
{
}

// CApplication message handlers



BOOL CApplication::SetParameters()
{
	HANDLE procHandle = GetCurrentProcess();
	if (m_bClassPriorityRealtime)
		SetPriorityClass(procHandle, HIGH_PRIORITY_CLASS);
	else
		OutputDebugLogString ("Process Class - NOT HIGH_PRIORITY_CLASS");

	m_bCamExtTrig = FALSE;
	//m_bCamExtTrig = (BOOL)GetRegistryEntry(READ_REGPATH + "Camera","Enable External Trigger",(int)m_bCamExtTrig);
	//PutRegistryEntry(WRITE_REGPATH + "Camera","Enable External Trigger",(int)m_bCamExtTrig);

	m_nGrabTimeout = -1;
	m_nGrabTimeout = GetRegistryEntry(READ_REGPATH + "Camera", "Grab Timeout", m_nGrabTimeout);
	if (m_nGrabTimeout==-1) {
		m_nGrabTimeout=1000;
		PutRegistryEntry(WRITE_REGPATH + "Camera", "Grab Timeout", m_nGrabTimeout);
	}

	// Camera delay
	PutRegistryEntry(WRITE_REGPATH + "Camera", "Trigger delay description", "Camera trigger delay is delay between signal to snap image and image exposure time.");

	m_nCameraTrigDelay.resize(m_nTotalTracks);		//Two Stations
	for(size_t i = 0; i<m_nCameraTrigDelay.size(); i++) {
		m_nCameraTrigDelay[i] = -1;
		m_nCameraTrigDelay[i] = GetRegistryEntry(READ_REGPATH + "Camera", "Trigger Delay Track [%d], us", m_nCameraTrigDelay[i]);
		if(m_nCameraTrigDelay[i] < 0) {
			m_nCameraTrigDelay[i] = 0;
			PutRegistryEntry(WRITE_REGPATH + "Camera", "Trigger Delay Track [%d], us", m_nCameraTrigDelay[i]);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	CString strSec;
	m_strLightLibrary = "void";
	strSec = "IO Library";
	m_strLightLibrary = GetRegistryEntry(READ_REGPATH + "Light", strSec, m_strLightLibrary);
	if(m_strLightLibrary.CompareNoCase("void")==0) {
		m_strLightLibrary = "I_GardaLight";
		PutRegistryEntry(WRITE_REGPATH + "Light", strSec, m_strLightLibrary);
	}
	if(m_strLightLibrary.CompareNoCase("None")!=0)
		m_strLightLibrary += ".dll";

	//m_lIOCardAddr = -1;
	//m_lIOCardAddr = GetRegistryEntry(READ_REGPATH + "Light","IO Card Address",(int)m_lIOCardAddr);
	//if (m_lIOCardAddr==-1) {
	//	m_lIOCardAddr = 0;
	//	PutRegistryEntry(WRITE_REGPATH + "Light","IO Card Address",(int)m_lIOCardAddr);
	//}

	if(InitializeLightLibrary()) {
		CString str;
		str.Format("Failed to initialize Light library %s", m_strLightLibrary);
		AfxMessageBox(str);
	}

	m_strIOInterfaceCard = "void";
	m_strIOInterfaceCard = GetRegistryEntry(READ_REGPATH + "Comm","Library",m_strIOInterfaceCard);
	if (m_strIOInterfaceCard.CompareNoCase("void")==0) {
		m_strIOInterfaceCard = "I_7230";
		PutRegistryEntry(WRITE_REGPATH + "Comm","Library",m_strIOInterfaceCard);
	}
	if(m_strIOInterfaceCard.CompareNoCase("None")!=0)
		m_strIOInterfaceCard+=".dll";

	m_lIOInterfaceCardAddr = -1;
	m_lIOInterfaceCardAddr = GetRegistryEntry(READ_REGPATH + "Comm","Card Address",(int)m_lIOInterfaceCardAddr);
	if (m_lIOInterfaceCardAddr==-1) {
		m_lIOInterfaceCardAddr = 0;
		PutRegistryEntry(WRITE_REGPATH + "Comm","Card Address",(int)m_lIOInterfaceCardAddr);
	}

	m_lIOReverseLogic = 1;
	m_lIOReverseLogic = GetRegistryEntry(READ_REGPATH + "Comm","Reverse Logic",(int)m_lIOReverseLogic);
	PutRegistryEntry(WRITE_REGPATH + "Comm","Reverse Logic",(int)m_lIOReverseLogic);


	m_lCOMMInCardNo = -1;
	m_lCOMMInCardNo = GetRegistryEntry(READ_REGPATH + "Comm","In-Card No",(int)m_lCOMMInCardNo);
	if (m_lCOMMInCardNo==-1) {
		m_lCOMMInCardNo = 0;
		PutRegistryEntry(WRITE_REGPATH + "Comm","In-Card No",(int)m_lCOMMInCardNo);
	}

	CString strIOPort = "void";
	strIOPort = GetRegistryEntry(READ_REGPATH + "Comm","In-Port Id",strIOPort);
	if (strIOPort.CompareNoCase("void")==0) {
		strIOPort = "PORT_1A";
		PutRegistryEntry(WRITE_REGPATH + "Comm","In-Port Id",strIOPort);
	}
	m_lCOMMInPortID = m_CommModule.GetIOPortID(strIOPort);

	m_lCOMMOutCardNo = -1;
	m_lCOMMOutCardNo = GetRegistryEntry(READ_REGPATH + "Comm","Out-Card No",(int)m_lCOMMOutCardNo);
	if (m_lCOMMOutCardNo==-1) {
		m_lCOMMOutCardNo = 0;
		PutRegistryEntry(WRITE_REGPATH + "Comm","Out-Card No",(int)m_lCOMMOutCardNo);
	}

	strIOPort = "void";
	strIOPort = GetRegistryEntry(READ_REGPATH + "Comm","Out-Port Id",strIOPort);
	if (strIOPort.CompareNoCase("void")==0) {
		strIOPort = "PORT_1A";
		PutRegistryEntry(WRITE_REGPATH + "Comm","Out-Port Id",strIOPort);
	}
	m_lCOMMOutPortID = m_CommModule.GetIOPortID(strIOPort);

	/*if(InitializeCommLibrary()) {
		CString str;
		str.Format("Failed to initialize I/O card (%s)", m_strIOInterfaceCard);
		AfxMessageBox(str);
	}*/


	/*m_strCommOPC = GetRegistryEntry(READ_REGPATH + "ADSTwinCAT", "Communication_Protocol", "none");
	if(m_strCommOPC == "none")
		PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", "Communication_Protocol", m_strCommOPC);
	 if(m_strCommOPC.CompareNoCase("TwinCAT")==0) {
		m_pADSInterface = new CADSInterface();
		m_nADSPortNr = 801;
		strSec.Format("ADS PortNr");
		m_nADSPortNr = GetRegistryEntry(READ_REGPATH + "ADSTwinCAT", strSec, m_nADSPortNr);
		PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_nADSPortNr);

		m_nMaxDelay = 2000000;
		strSec.Format("ADS MaxDelay");
		m_nMaxDelay = GetRegistryEntry(READ_REGPATH + "ADSTwinCAT", strSec, m_nMaxDelay);
		PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_nMaxDelay);

		m_nCycleTime = 1000000;
		strSec.Format("ADS CycleTime");
		m_nCycleTime = GetRegistryEntry(READ_REGPATH + "ADSTwinCAT", strSec, m_nCycleTime);
		PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_nCycleTime);

		m_nTimeout = 1000;
		strSec.Format("ADS Timeout");
		m_nTimeout = GetRegistryEntry(READ_REGPATH + "ADSTwinCAT", strSec, m_nTimeout);
		PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_nTimeout);

		m_nTxDelay = 5;
		strSec.Format("ADS TxDelay");
		m_nTxDelay = GetRegistryEntry(READ_REGPATH + "ADSTwinCAT", strSec, m_nTxDelay);
		PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_nTxDelay);

		for(int i=0; i<6; i++) {
			m_byteNetID[i] = 0;
			strSec.Format("NetID%d", i);
			m_byteNetID[i] = GetRegistryEntry(READ_REGPATH + "ADSTwinCAT", strSec, m_byteNetID[i]);
			PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_byteNetID[i]);
		}
	}*/	




	for(int nAxes=0; nAxes<AXES_COUNT; nAxes++) {
		strSec.Format("Motion Card ID:%d", nAxes+1);
		m_nAxes[nAxes] = GetRegistryEntry(READ_REGPATH + "Motion", strSec, m_nAxes[nAxes]);
		if(m_nAxes[nAxes]==0) {
			m_nAxes[nAxes] = 1500+nAxes;
			PutRegistryEntry(WRITE_REGPATH + "Motion", strSec, m_nAxes[nAxes]);
		}
		OutputDebugLogTo(0,TRUE,"App:Reg Read Axes=%d", m_nAxes[nAxes]);
	}

	m_strMotionCard = "Void";
	strSec = "Motion Library";
	m_strMotionCard = GetRegistryEntry(READ_REGPATH + "Motion", strSec, m_strMotionCard);
	if(m_strMotionCard.CompareNoCase("Void")==0) {
		m_strMotionCard = "M_7856";
		PutRegistryEntry(WRITE_REGPATH + "Motion", strSec, m_strMotionCard);
	}
	if(m_strMotionCard.CompareNoCase("None")!=0)
		m_strMotionCard += ".dll";
	m_bMachineOn = FALSE;

	m_strMotorParmFile = "void";
	m_strMotorParmFile = GetRegistryEntry(READ_REGPATH + "Motion", "Parameter File(.xml)", m_strMotorParmFile);
	if(m_strMotorParmFile.CompareNoCase("void")==0) {
		m_strMotorParmFile = "7856";
		PutRegistryEntry(WRITE_REGPATH + "Motion", "Parameter File(.xml)", m_strMotorParmFile);
	}

	if(m_strMotorParmFile.CompareNoCase("None") != 0)
		m_strMotorParmFile += ".xml";

	m_nMotorCardNo = -1;
	m_nMotorCardNo = GetRegistryEntry(READ_REGPATH + "Motion", "Card ID", m_nMotorCardNo);
	if(m_nMotorCardNo < 0) {
		m_nMotorCardNo = 0;
		PutRegistryEntry(WRITE_REGPATH + "Motion", "Card ID", m_nMotorCardNo);
	}

	m_nMotionTimeout = -1;
	m_nMotionTimeout = GetRegistryEntry(READ_REGPATH + "Motion", "Movement TimeOut (ms)", m_nMotionTimeout);
	if(m_nMotionTimeout < 1) {
		m_nMotionTimeout = 60000;
		PutRegistryEntry(WRITE_REGPATH + "Motion", "Movement TimeOut (ms)", m_nMotionTimeout);
	}


	if(InitializeMotionCard()) {
		CString str;
		str.Format("Failed to initialize Motion library %s", m_strMotionCard);
		AfxMessageBox(str);
	}

	//Scale Engine
	m_strScaleCard = "void";
	m_strScaleCard = GetRegistryEntry(READ_REGPATH + "Scale", "Library name (.dll)", m_strScaleCard);
	if (m_strScaleCard.CompareNoCase("void") == 0) {
		m_strScaleCard = "Pci_8124";
		PutRegistryEntry(WRITE_REGPATH + "Scale", "Library name (.dll)", m_strScaleCard);
	}
	if (m_strScaleCard.CompareNoCase("None") != 0)
		m_strScaleCard += ".dll";

	if (InitScaleCard()) {
		CString str;
		str.Format("Failed to initialize Scale library %s", m_strScaleCard);
		AfxMessageBox(str);
	}


	m_strSensorLibrary = "void";
	strSec = "Sensor Library";
	m_strSensorLibrary = GetRegistryEntry(READ_REGPATH + "Sensor", strSec, m_strSensorLibrary);
	if (m_strSensorLibrary.CompareNoCase("Void") == 0) {
		m_strSensorLibrary = "TS_2300";
		PutRegistryEntry(WRITE_REGPATH + "Sensor", strSec, m_strSensorLibrary);
	}
	if (m_strSensorLibrary.CompareNoCase("None") != 0)
		m_strSensorLibrary += ".dll";
	
	m_strSensorIpAddress = "void";
	m_strSensorIpAddress = GetRegistryEntry(READ_REGPATH + "Sensor", "IP Address", m_strSensorIpAddress);
	if (m_strSensorIpAddress.CompareNoCase("void") == 0) {
		m_strSensorIpAddress = "192.168.1.1";
		PutRegistryEntry(WRITE_REGPATH + "Sensor", "IP Address", m_strSensorIpAddress);
	}

	if (InitializeTriangularSensor()) {
		CString str;
		str.Format("Failed to initialize Sensor library %s", m_strSensorLibrary);
		AfxMessageBox(str);
	}


	nQRBarcodePortID = -1;
	nQRBarcodePortID = GetRegistryEntry(READ_REGPATH + "QR-Barcode Reader", "Port ID", nQRBarcodePortID);
	if (nQRBarcodePortID < 0) {
		nQRBarcodePortID = 9004;
		PutRegistryEntry(READ_REGPATH + "QR-Barcode Reader", "Port ID", nQRBarcodePortID);
	}

	strMultiMonitorPath = "";
	strMultiMonitorPath = GetRegistryEntry(READ_REGPATH + "QR-Barcode Reader", "Multi-Monitor Path", strMultiMonitorPath);
	if (strMultiMonitorPath == "") {
		strMultiMonitorPath.Format("C:\\Program Files (x86)\\KEYENCE Applications\\AutoID\\MultiMonitor_EN\\MultiMonitor.exe");
		PutRegistryEntry(READ_REGPATH + "QR-Barcode Reader", "Multi-Monitor Path", strMultiMonitorPath);
	}

	qrReaderIpAddress = "";
	qrReaderIpAddress = GetRegistryEntry(READ_REGPATH + "QR-Barcode Reader", "IP Address", qrReaderIpAddress);
	if (qrReaderIpAddress.CompareNoCase("") == 0) {
		qrReaderIpAddress = "None";
		PutRegistryEntry(WRITE_REGPATH + "QR-Barcode Reader", "IP Address", qrReaderIpAddress);
	}

	qrBarcodeReceivedTimeOutPeriod = 1000;
	qrBarcodeReceivedTimeOutPeriod = GetRegistryEntry(READ_REGPATH + "QR-Barcode Reader", "Received Time Out(ms)", qrBarcodeReceivedTimeOutPeriod);
	PutRegistryEntry(READ_REGPATH + "QR-Barcode Reader", "Received Time Out(ms)", qrBarcodeReceivedTimeOutPeriod);


	nBarcodePortID = -1;
	nBarcodePortID = GetRegistryEntry(READ_REGPATH + "Barcode Reader", "Barcode COMPort ID", nBarcodePortID);
	if (nBarcodePortID < 0) {
		nBarcodePortID = 3;
		PutRegistryEntry(READ_REGPATH + "Barcode Reader", "Barcode COMPort ID", nBarcodePortID);
	}

	if (QRBarcodeReader.Initialize(nQRBarcodePortID, qrReaderIpAddress)){
		if (qrReaderIpAddress.CompareNoCase("none") != 0) {
			CString str;
			str.Format("Error in initialization QR-Barcode reader port %d Ip address %s", nQRBarcodePortID, qrReaderIpAddress);
			AfxMessageBox(str);
		}
	}
	

	QRBarcodeReader.setQRReceivedTimeOutPeriod(qrBarcodeReceivedTimeOutPeriod);

	//SFT Communication

	m_bSFTEnableCommunication = false;
	m_bSFTEnableCommunication = GetRegistryEntry(READ_REGPATH + "SFT", "Enable SFT Communication", m_bSFTEnableCommunication) == 1 ? true : false;
	PutRegistryEntry(WRITE_REGPATH + "SFT", "Enable SFT Communication", m_bSFTEnableCommunication);

	m_strSFTServerAddress = "";
	m_strSFTServerAddress = GetRegistryEntry(READ_REGPATH + "SFT","SFT Server Address", m_strSFTServerAddress);
	if (m_strSFTServerAddress.CompareNoCase("") == 0) {
		m_strSFTServerAddress = "C:\\";
		PutRegistryEntry(WRITE_REGPATH + "SFT", "SFT Server Address", m_strSFTServerAddress);
	}

	m_nSFTMoveTypeMoveIn = GetRegistryEntry(READ_REGPATH + "SFT", "MoveType for MoveIn", m_nSFTMoveTypeMoveIn);
	PutRegistryEntry(WRITE_REGPATH + "SFT", "MoveType for MoveIn", m_nSFTMoveTypeMoveIn);

	m_nSFTMoveTypeMoveOut = GetRegistryEntry(READ_REGPATH + "SFT", "MoveType for MoveOut", m_nSFTMoveTypeMoveOut);
	PutRegistryEntry(WRITE_REGPATH + "SFT", "MoveType for MoveOut", m_nSFTMoveTypeMoveOut);

	m_nSFTTimeOut = -1;
	m_nSFTTimeOut = GetRegistryEntry(READ_REGPATH + "SFT", "MID Verification Time Out (ms)", m_nSFTTimeOut);
	if (m_nSFTTimeOut == -1) {
		m_nSFTTimeOut = 25000;
		PutRegistryEntry(WRITE_REGPATH + "SFT", "MID Verification Time Out (ms)", m_nSFTTimeOut);
	}

	//MoveType for MoveIn
	//MoveType for MoveOut
	//MID Verification Time Out (ms)

	//if (BarcodeReader.Initialize(nBarcodePortID)) {
	//	CString str;
	//	str.Format("Error in initialization Barcode Reader Port %d", nBarcodePortID);
	//		AfxMessageBox(str);
	//	}


	for(int i=0; i<5; i++) {
		m_nToolNo[i]	= 0;
		m_nMoldNo[i]	= 0;
		m_nGripperNo[i]	= 0;
		m_nFailNo[i]	= 0;
	}

	m_nGoodACounter = 0;
	m_nGoodBCounter = 0;
	m_nFailCounter = 0;
	m_ThreadManager.Initialize();

	pMotorTriggerTestDialog = NULL;
	bEnableMotorTriggerTestDialog = FALSE;

	m_EventRun = CreateEventA(NULL, FALSE, FALSE, NULL);
	m_nCommand = NO_COMMAND;
	m_nActiveTrackCommandIdx = -1;

	if(!(m_eventOnline = CreateEventA(NULL, TRUE, FALSE, NULL)))
		return FALSE;
	if(!(m_eventVisionReady = CreateEventA(NULL, TRUE, FALSE, NULL)))
		return FALSE;
	if(!(m_EventCommSendMsg = CreateEventA(NULL, TRUE, FALSE, NULL)))
		return FALSE;

	eventScanThreadKilled = CreateEventA(NULL, TRUE, TRUE, NULL);
	m_eventStartScan = CreateEventA(NULL, FALSE, FALSE, NULL);

	EventStartBarcodeTrigger= CreateEventA(NULL, FALSE, FALSE, NULL);
	m_eventBarcodeTriggerDone = CreateEventA(NULL, FALSE, FALSE, NULL);

	if (InitializeSecsgem()) {
		return FALSE;
	}
	theApp.bIsGEMControllerRemoteMode = FALSE;


	for(int i=0; i<m_nTotalTracks; i++)
		if(!m_pTracks[i].SetParameters(i))
			return FALSE;

	m_LightHandler.SetParameters(m_pTracks, m_nTotalTracks);

	m_bExitApp = FALSE;
	int nError;
	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
		if(m_pTracks[nTrack].m_strFGCard.CompareNoCase("none")==0)
			continue;

		nError = RegisterCameraCard(&m_pTracks[nTrack].m_FGResource, m_pTracks[nTrack].m_strFGCard);
		if (nError) {
			if (AfxMessageBox("Failed to Initialize Camera Library.\nContinue in offline mode?", MB_YESNO | MB_ICONSTOP) == IDNO)
				return FALSE;
			break;
		}

		m_nCamLibraryTrackIndex = nTrack;
		/*if(m_nCamLibraryTrackIndex < 0) {
			nError = RegisterCameraCard(&m_pTracks[nTrack].m_FGResource, m_pTracks[nTrack].m_strFGCard);
			if(nError) {
				if(AfxMessageBox("Failed to Initialize Camera Library.\nContinue in offline mode?", MB_YESNO|MB_ICONSTOP)==IDNO)
					return FALSE;
				break;
			}

			m_nCamLibraryTrackIndex = nTrack;
		}
		else {
			m_pTracks[nTrack].m_FGResource.CopyFrom(&m_pTracks[m_nCamLibraryTrackIndex].m_FGResource);
			m_pTracks[nTrack].m_FGResource.pCamModule = 0;
		}*/

		nError = RegisterCamera(&m_pTracks[nTrack].m_FGResource,
			m_pTracks[nTrack].m_strCameraFile,
			m_pTracks[nTrack].m_strFGCardFile,
			FALSE,
			m_bIsCameraMonitorMode,
			TRUE,
			TRUE);

		if(nError) {
			if(m_bIsCameraMonitorMode)
				AfxMessageBox("[Monitor Mode] Failed to attach to cameras\n\n\nPlease make sure that the MASTER application has\nregistered the cameras before opening in monitor mode", MB_OK|MB_ICONSTOP);
			///return -1;
			continue;
		}

		nError = SetGrabTimeout(&m_pTracks[nTrack].m_FGResource, m_nGrabTimeout);
		if (nError)
			return -1;
	}
	ConfigureLightControllers();	//// moved from main frame
	return TRUE;
}

#define SUPERUSER "neverfailtoadapt"

void CApplication::InitializeDocView()
{
	CEvTrackHandler** pTracks = new CEvTrackHandler*[m_nTotalTracks];
	for(int i=0; i<m_nTotalTracks; i++)
		pTracks[i] = (CEvTrackHandler*) &(m_pTracks[i]);
	((CMainFrame*) m_pMainWnd)->Initialize(pTracks, m_nTotalTracks);


 //	CAppDoc* pDoc[MAX_DOCS];
	//CEvSdiTemplate* pDocTemplate;
	//POSITION pos;
 //	for(int nViews=0; nViews<m_nTotalViews; nViews++) {
	//	CEvTabView* view = ((CEvTabView*) m_pFrames[nViews]->GetActiveView());
	//	for(int nDoc=0; nDoc<1; nDoc++) {
	//		pDocTemplate = view->m_SdiChildWnd[nDoc].m_pDocTemplate;
	//		pos = pDocTemplate->GetFirstDocPosition();
	//		pDoc[nViews] = (CAppDoc*) pDocTemplate->GetNextDoc(pos);
	//		//pDoc[nDoc] = view->GetDocument();
	//	}
	//	if(nViews<m_nTop)
	//		m_pTracks[0].InitializeDocView(pDoc);
	//	else if(nViews<m_nTop+m_nBottom)
	//		m_pTracks[1].InitializeDocView(pDoc);
	//	else 
	//		m_pTracks[2].InitializeDocView(pDoc);
	//}
	CAppDoc* pDoc[MAX_DOCS];
	CEvSdiTemplate* pDocTemplate;
	POSITION pos;
	int nView=0;
 	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
		
		for(int nDoc=0; nDoc<m_pTracks[nTrack].m_nTotalDocs; nDoc++) {
			CEvTabView* view = ((CEvTabView*) m_pFrames[nView]->GetActiveView());
			pDocTemplate = view->m_SdiChildWnd[0].m_pDocTemplate;
			pos = pDocTemplate->GetFirstDocPosition();
			pDoc[nDoc] = (CAppDoc*) pDocTemplate->GetNextDoc(pos);
			//pDoc[nDoc] = view->GetDocument();
			nView++;
		}
		m_pTracks[nTrack].InitializeDocView(pDoc);

		///view->SetTitle();
	}
	
	//if(m_nTotalViews > 1){
	//	//m_pTracks[0].m_pDoc[1]->m_pRemoteInspDoc = m_pTracks[0].m_pDoc[0];
	//	////m_pTracks[0].m_pDoc[2]->m_pRemoteInspDoc = m_pTracks[0].m_pDoc[0];
	//	for(int nView=1; nView<m_nTotalViews; nView++)//
	//		m_pTracks[0].m_pDoc[nView]->m_pRemoteInspDoc = m_pTracks[0].m_pDoc[0];//
	//}
	//for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
	////	for(int nDoc=0; nDoc<m_nTotalViewDocs[nTrack]; nDoc++) {
	//		m_pTracks[nTrack].m_pDoc[0]->m_pRemoteInspDoc = m_pTracks[nTrack].m_pDoc[0];
	//	//}
	//}
	//CString str;
	//str.Format(" FOV: %d, %d", m_pTracks[0].m_szImgSize.cx, m_pTracks[0].m_rectAOI.Size().cy);
	//UpdateStatusBar(&str, 1);
	OutputDebugLog("Initialize com port");
	m_CommInterface.Initialize();
}

void CApplication::BinarizeAllTracks(BOOL bBinaryMode)
{
	CEvImageDoc* pDoc;
 	for(int nViews=0; nViews<m_nTotalViews; nViews++) {
		pDoc = ((CEvTabView*) m_pFrames[nViews]->GetActiveView())->GetActiveDoc();
		SwitchImg(&pDoc->m_ImageViewInfo, bBinaryMode, pDoc->m_nThreshold);
	}
}

void CApplication::StreamAllTracksToggle()
{
	CAppDoc* pDoc;
 	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
		pDoc = (CAppDoc*) ((CEvTabView*) m_pFrames[nTrack]->GetActiveView())->GetActiveDoc();
		if(pDoc->m_pTrack->m_bCamAvail) {
			//pDoc->CameraLive();
			Sleep(20);
		}
	}
}

BOOL CApplication::InitProcess()
{
	CMainFrame *pMain;
	pMain = ((CMainFrame*)AfxGetMainWnd());
	//CWinThread* pThread;
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetBreakAlloc(681);
	//LaunchWindowsExplorer();

	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_csOnline);
	InitializeCriticalSection(&m_csFileAccess);
	InitializeCriticalSection(&m_csOutputData);
	InitializeCriticalSection(&m_CriticalSectionSaveImage);
	InitializeCriticalSection(&m_CriticalSectionDeepLearnSaveImage);
	InitializeCriticalSection(&m_CriticalSectionTOPDieShiftedInfo);


	if(m_bIsMasterApp)
	{
#ifdef _DEBUG
		CWinThread* pThread = AfxBeginThread(TrackCommandThread, this, THREAD_PRIORITY_NORMAL);
#else
		CWinThread* pThread = AfxBeginThread(TrackCommandThread, this, THREAD_PRIORITY_TIME_CRITICAL);
#endif
		


		if(AfxBeginThread(ZSensorSequence, this, THREAD_PRIORITY_HIGHEST) == NULL) {
			OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup Z Sensor Thread");
			return FALSE;
		}

		if(AfxBeginThread(FocusZAxis, this, THREAD_PRIORITY_HIGHEST) == NULL) {
			OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup Z Sensor Thread");
			return FALSE;

		}

		if (AfxBeginThread(YMotorToDatumOffset, this, THREAD_PRIORITY_HIGHEST) == NULL) {
			OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup Z Sensor Thread");
			return FALSE;
		}
		
	/*	if(AfxBeginThread(ZSensorReadData, this, THREAD_PRIORITY_HIGHEST) == NULL) {
			OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup Z Sensor Thread");
			return FALSE;

		}*/

		for (int nThread = 0; nThread < STATION_TOP_THREAD_COUNT; nThread++) {
			if (AfxBeginThread(InspectOnlineThread, &m_pTracks[STATION_TOP], THREAD_PRIORITY_BELOW_NORMAL) == NULL) {
				OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup 2-3D Inspection Thread");
				return FALSE;
			}

			//Duplicate Inspection
			if (AfxBeginThread(DuplicateInspectionThread, &m_pTracks[STATION_TOP], THREAD_PRIORITY_BELOW_NORMAL) == NULL) {
				OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup 2-3D DuplicateInspectionThread Thread");
				return FALSE;
			}
		}

		for (int nThread = 0; nThread < STATION_THR_THREAD_COUNT; nThread++) {
			if (AfxBeginThread(InspectOnlineThread, &m_pTracks[STATION_THR], THREAD_PRIORITY_BELOW_NORMAL) == NULL) {
				OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup 2-3D Inspection Thread");
				return FALSE;
			}

			//Duplicate Inspection
			if (AfxBeginThread(DuplicateInspectionThread, &m_pTracks[STATION_THR], THREAD_PRIORITY_BELOW_NORMAL) == NULL) {
				OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup 2-3D DuplicateInspectionThread Thread");
				return FALSE;
			}
		}
			
		/*if (AfxBeginThread(ClearInspectionData, this, THREAD_PRIORITY_BELOW_NORMAL) == NULL) {
			OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup Clear Inspection Thread");
			return FALSE;
		}*/

		if (AfxBeginThread(SaveImageThread, this, THREAD_PRIORITY_LOWEST) == NULL) {
			OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup Failed Image Saving Thread");
			return FALSE;
		}

    	if (AfxBeginThread(SaveDeepLeanrningImageThread, this, THREAD_PRIORITY_LOWEST) == NULL) {
				OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup Failed Image Saving Thread");
				return FALSE;
		}
		m_EventRefreshLight = CreateEventA(NULL, TRUE, FALSE, NULL);
		m_EventMotionSensor = CreateEventA(NULL, TRUE, FALSE, NULL);
		m_EventMotionSensorStart  = CreateEventA(NULL, TRUE, FALSE, NULL);
//		m_EventIOtest = CreateEventA(NULL, TRUE, FALSE, NULL);

//		AfxBeginThread(RefreshLightThread, this, THREAD_PRIORITY_LOWEST);	
	}

	m_pProgressDlg = new CProgressDlg();
	if (!m_pProgressDlg->Create(IDD_PROGRESS_DLG, dynamic_cast<CMainFrame*>(AfxGetMainWnd()))) {
		delete m_pProgressDlg;
		m_pProgressDlg = NULL;
	}
	else {
		m_pProgressDlg->CenterWindow();
		m_pProgressDlg->SetFocus();
	}

	m_pImageQueue = new CImageQueueHandler();
	m_pImageQueue->Initialize(m_pTracks[0].m_pDoc[0]->m_ImageViewInfo.sizeImage);

	/*m_pMotionControl = new CMotionControlDialog(m_pMainWnd);
	if(!m_pMotionControl->Create(IDD_MOTOR_DLG, m_pMainWnd)) {
		delete m_pMotionControl;
		m_pMotionControl = NULL;
	}
	*/
	m_pIoHardwareDlg = new CIOTestDlg(m_pMainWnd);
	if (!m_pIoHardwareDlg->Create(IDD_DIALOG_HARDWAREIOTEST, m_pMainWnd)) {
		delete m_pIoHardwareDlg;
		m_pIoHardwareDlg = NULL;
	}

	// I/O Hardware Control
	pIOControlDialog = new IOInteraction();
	if (!pIOControlDialog->Create(IDD_IOINTERACTION, AfxGetMainWnd())) {
		delete pIOControlDialog;
		pIOControlDialog = NULL;
	}

	m_pWndPVIDefectParameters = new CPVIInspectionParameters(m_pMainWnd);
	if(!m_pWndPVIDefectParameters->Create(IDD_PVI_INSPECTION_PARM, m_pMainWnd)) {
		delete m_pWndPVIDefectParameters;
		m_pWndPVIDefectParameters = NULL;
		//return FALSE;
	}

	m_pCalibrationDlg = new CCalibrationSettings(m_pMainWnd);
	if(!m_pCalibrationDlg->Create(IDD_CALIBRATIONSETTINGS,m_pMainWnd)) {
		delete m_pCalibrationDlg;
		m_pCalibrationDlg = NULL;
	}

	m_pDispOnlinePanel = new CDisplayOnlinePanel(m_pMainWnd);
	if (!m_pDispOnlinePanel->Create(IDC_ONLINE_DISPLAY_PANEL, m_pMainWnd)) {
		delete m_pDispOnlinePanel;
		m_pDispOnlinePanel = NULL;
	}
	
	m_pDisplayMsgStatus = new CMessageDlg(m_pMainWnd);
	if (!m_pDisplayMsgStatus->Create(IDD_DIALOGMSG_DLG, m_pMainWnd)) {
		delete m_pDisplayMsgStatus;
		m_pDisplayMsgStatus = NULL;
	}
	else {
		m_pDisplayMsgStatus->ShowWindow(FALSE);
	}

	m_pInfoMsgDlg = new CInfoMsgDlg(m_pMainWnd);
	if (!m_pInfoMsgDlg->Create(IDD_INFO_MESSAGE_DLG, m_pMainWnd)) {
		delete m_pInfoMsgDlg;
		m_pInfoMsgDlg = NULL;
	}
	else {
		m_pInfoMsgDlg->ShowWindow(FALSE);
	}

	pErrorMsgDlg = new CErrorDlg();
	if(!pErrorMsgDlg->Create(IDD_ERRORDLG, AfxGetMainWnd())) {
		delete pErrorMsgDlg;
		pErrorMsgDlg = NULL;
	}

	m_pVerificationMode = new CVerificationMode();
	if (!m_pVerificationMode->Create(IDD_VERIFICATION_DIALOG, AfxGetMainWnd())) {
		delete m_pVerificationMode;
		m_pVerificationMode = NULL;
	}

	

	m_pMainWnd->ShowWindow(SW_HIDE);
	//m_pMainWnd->ShowWindow(SW_HIDE);
	m_bLoadComplete = TRUE;

	//((CMainFrame*) m_pMainWnd)->RecalculateLayout();

	m_UserAccInfo.m_strCurUserName = "Operator";
	m_UserAccInfo.m_nCurrentAccessLevel = 0;
	m_UserAccInfo.m_strCurrentAccessLevel = "Operator";

	
	/*if(m_pFrames[m_nTotalViews]) {
		m_pStatsView = (CStatisticsView*)m_pFrames[m_nTotalViews]->GetActiveView();
		if(m_pStatsView && ::IsWindow(m_pStatsView->m_ctrlDefectsList.m_hWnd)) {
			m_pStatsView->Initialize();
			UpdateStatisticsWindow();
			AddLogEvent("Sherlock_Spyglass Vision Application started");
		}
	}
	else
		m_pStatsView = NULL*/;

	//m_pTracks[0].m_pDoc[0]->m_InspectionHandler.CreateParameterList();
	///LoadRecipes();

	
	m_pMainWnd->PostMessage(WM_LOAD_RECIPE);

	m_UserLog.LoadUserAccountData();
	m_AdvSecurityDlg.GetSecurityAccess();

	//if(m_AdvSecurityDlg.m_data.m_nLensInsp > 0)
	//	OnVisionOnline();

	for(int i=0; i<m_nTotalViews; i++)
		((CEvChildFrame*) m_pFrames[i])->EnableMaximizeBox();
	 
	CSplashScreen::HideSplashScreen();
	m_pMainWnd->SetFocus();

	if (m_pProgressDlg != NULL) {
		m_pProgressDlg->ShowWindow(SW_SHOWNA);
		m_bProgressDlgOpen = TRUE;
		m_pProgressDlg->SetStatus("Initialize The Doc View...", 0);
	}

	//if(!m_pStatsView/* && m_nTotalViews==1*/)
	{
		((CMainFrame*) m_pMainWnd)->MDIMaximize(m_pFrames[0]);
		((CEvTabView*) m_pFrames[0]->GetActiveView())->Invalidate();
	}
	//else
		//((CMainFrame*) m_pMainWnd)->MDIActivate(m_pFrames[0]);

	m_bDisplayAllStatsOffline = FALSE;//

	/*((CMainFrame*) m_pMainWnd)->ShowStatsPane();
	
	((CMainFrame*) m_pMainWnd)->ShowPreviousImages();

	((CMainFrame*) m_pMainWnd)->ShowDebugView();

	
	((CMainFrame*) m_pMainWnd)->RecalcLayout();*/
	int nLoadedDataPercentage = 0;
	double dStep = 3 * 1.0 / m_nTotalViews, dLoadedDataPercentage = nLoadedDataPercentage;

	for(int nView=0; nView<m_nTotalViews; nView++) {
		CEvTabView* view = ((CEvTabView*) m_pFrames[nView]->GetActiveView());
		view->SetTitle();

		if (m_pProgressDlg != NULL) {
			dLoadedDataPercentage += dStep;
			nLoadedDataPercentage = dLoadedDataPercentage;
			m_pProgressDlg->SetStatus("Initialize The Doc View...", nLoadedDataPercentage);
		}
	}
	OnDefaultSetting();

	SetUserName(m_UserAccInfo.m_strCurUserName);
	AddLogEvent("Logged in as Operator");

	pGlobalBackgroundThread = new CGlobalBackgroundThread();
	pGlobalBackgroundThread->RunBackgroundEvents();
	CString strStatus;
//	m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_CURTAIN_STATUS_STATIC)->SetWindowTextA("Guard Bypass is Off");
	if (m_bSaveOnlinePassImages || m_bSaveOnlinePassImages)
		strStatus = "Saving Images";
	else if (m_bDryRun)
		strStatus = "Dry Run";
	else if(m_bWetRun)
		strStatus = "Wet Run";
	else
		strStatus = "";

	m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_CURTAIN_STATUS_STATIC)->SetWindowTextA(strStatus.GetString());

	m_pMsgBox = new CMessageBox();
	if(!m_pMsgBox->Create(IDD_HAWKEYE_MSG_BOX, AfxGetMainWnd())) {
		delete m_pMsgBox;
		m_pMsgBox = NULL;
	}

	//m_pMainWnd->SendMessageA(WM_SET_FULLSCREEN, TRUE, FALSE);
	return TRUE;
}


int CApplication::InitializeLightLibrary()
{
	int nError = m_LightModule.LoadIODll(m_strLightLibrary);
	if(nError) {
		OutputDebugLogTo(0, TRUE, "Error loading Light I/O Library %s", m_strLightLibrary);
		return 0;
	}
	else if(m_strLightLibrary.CompareNoCase("none")!=0)
		OutputDebugLog("Light Library '%s' loaded", m_strLightLibrary);

	//int nError = m_LightModule.RegisterCard(m_lIOCardAddr, 0);
	//if(nError)
	//	return nError;

	//nError = m_LightModule.ConfigIOPort(m_lIOCardAddr, m_pTracks[nTrack].m_lLightPortID[nPort], IO_MODE_OUT, 0);
	//if(nError)
	//	return nError;
	if (m_bEnableResourceDisplay) {
		m_pResourceCheckDlg = new CResourceCheckDlg();
		if (!m_pResourceCheckDlg->Create(IDD_RESOURCE_MONITOR_DLG, AfxGetMainWnd())) {
			delete m_pResourceCheckDlg;
			m_pResourceCheckDlg = NULL;
			return TRUE;
		}
		m_pResourceCheckDlg->ShowWindow(m_bdisplayResourceWindow);
	}
	return 0;
}

int CApplication::ConfigureLightControllers()
{
	m_LightHandler.ConfigureControllers();

	return 0;
}

int CApplication::InitializeCommLibrary()
{
	int nError = m_CommModule.LoadIODll(m_strIOInterfaceCard);
	if(nError) {
		OutputDebugLog("Error loading Comm Library %s", m_strIOInterfaceCard);
		return nError;
	}
	else if(m_strIOInterfaceCard.CompareNoCase("none")!=0)
		OutputDebugLog("Comm Library '%s' loaded", m_strIOInterfaceCard);

	nError = m_CommModule.RegisterCard(m_lCOMMOutCardNo, m_lIOInterfaceCardAddr);
	if(nError)
		return nError;

	nError = m_CommModule.ConfigIOPort(m_lCOMMOutCardNo, m_lCOMMOutPortID, IO_MODE_OUT, 0);
	if(nError)
		return nError;


	return 0;
}


int CApplication::InitializeMotionCard()
{
	int nError = m_MotorResource.LoadMotorDll(m_strMotionCard);
	if(nError || !m_strMotionCard.CompareNoCase("none"))
		m_bMachineOn = FALSE;
	if(nError) {
		OutputDebugLog("Error loading Motion DLL %s", m_strMotionCard);	
		return nError;
	}
	else
		OutputDebugLog("Motion DLL %s loaded", m_strMotionCard);
	
	if (m_nMotorCardNo >= 0) {

		nError = m_MotorResource.RegisterCard(m_nMotorCardNo, m_nAxes[0]);
		if (nError)
			return nError;

		nError = m_MotorResource.RegisterIOCard(m_nMotorCardNo);
		if (nError)
			return nError;

		char * pcFileName = NULL;
		pcFileName = (char*)LPCTSTR(m_strMotorParmFile);
		m_MotorResource.InitializeCard(pcFileName);
		motionParameters.InitMotionCard();
	}

	return 0;
}

int CApplication::InitScaleCard()
{
	int nError;
	CString str;

	nError = m_ScaleResource.LoadScaleDll(m_strScaleCard);
	if (nError) {
		str.Format("Error loading Linear Scale Library %s", m_strScaleCard);
		OutputDebugLogTo(0, TRUE, str.GetBuffer(0));
		str.ReleaseBuffer();
		return nError;
	}
	else {
		OutputDebugLogTo(0, TRUE, "Linear Scale Library %s loaded", m_strScaleCard);
	}

	nError = m_ScaleResource.RegisterScaleCards();
	if (nError) return nError;


	motionParameters.InitScaleCard();

	return 0;
}
int CApplication::InitializeTriangularSensor()
{
	int nError = m_TriangularSensor.LoadSensorDll(m_strSensorLibrary);
	if (nError || !m_strSensorLibrary.CompareNoCase("none"))
		m_bSensorLibLoaded = FALSE;
	if (nError) {
		OutputDebugLog("Error loading Triangular Sensor DLL %s", m_strSensorLibrary);
		m_bSensorLibLoaded = FALSE;
		return nError;
	}
	m_bSensorLibLoaded = TRUE;
	OutputDebugLog("Sensor DLL %s loaded", m_strSensorLibrary);

	BOOL bConnectTemp = TRUE;

	nError = m_TriangularSensor.InitSensor(m_TrigangualrsensorInstance);
	if (nError) {
		bConnectTemp = FALSE;
	}
	nError = m_TriangularSensor.OpenInstance(m_TrigangualrsensorInstance, m_strSensorIpAddress);
	if (nError) {
		bConnectTemp = FALSE;
	}
	nError = m_TriangularSensor.Set_Averaging(m_TrigangualrsensorInstance, 128);
	if(nError) {
		bConnectTemp = FALSE;
	}

	nError = m_TriangularSensor.GetAllParameters(m_TrigangualrsensorInstance);
	if (nError) {
		bConnectTemp = FALSE;
	}
	nError = m_TriangularSensor.GetTransmittedDataInfo(m_TrigangualrsensorInstance);
	if (nError) {
//		bConnectTemp = FALSE;
	}
	
	if (bConnectTemp)
		m_bTriangularSensorConnected = TRUE;
	else
		m_bTriangularSensorConnected = FALSE;

	return 0;
}


int OutPortWriteApp(BOOL bLightModule, long lCardNo, long lPortID, long lData, long lReserved)
{
	if(bLightModule)
		return theApp.m_LightModule.OutPortWrite(lCardNo, lPortID, lData, lReserved);
	else
		return theApp.m_CommModule.OutPortWrite(lCardNo, lPortID, lData, lReserved);
}

int OutPortSendString(int nController, CString* pstrData)
{
	int nError = theApp.m_LightHandler.SendLightCommandString(nController, pstrData);

	return nError;
}

int OutPortReadApp(BOOL bLightModule, long lCardNo, long lPortID, long *plData, long lReserved)
{
	if(bLightModule)
		return theApp.m_LightModule.OutPortRead(lCardNo, lPortID, plData, lReserved);
	else
		return theApp.m_CommModule.OutPortRead(lCardNo, lPortID, plData, lReserved);
}

int InPortReadApp(BOOL bLightModule, long lCardNo, long lPortID, long *plData, long lReserved)
{
	if(bLightModule)
		return theApp.m_LightModule.InPortRead(lCardNo, lPortID, plData, lReserved);
	else
		return theApp.m_CommModule.InPortRead(lCardNo, lPortID, plData, lReserved);
}

int CApplication::CheckPassword(int SecurityAccessNo)
{
	CPasswordDialog dlg(m_pMainWnd);
	if(m_bSuperUserMode)
		return 0;

	if (dlg.DoModal()==IDOK) {
		if(dlg.m_strPassword == SUPERUSER) {
			//m_bSuperUserMode = TRUE;
			//((CMainFrame*)m_pMainWnd)->m_wndLogo.SwitchImage();
			return 0;
		}
		else {
			AfxMessageBox("Wrong Password", MB_OK | MB_ICONERROR);
			return -1;
		}
	}

	return -1;
}

void CApplication::UpdateTrackStatus()
{
	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
		for(int nDoc=0; nDoc<m_pTracks[nTrack].m_nTotalDocs; nDoc++)
			m_pTracks[nTrack].m_pDoc[nDoc]->DrawTrackStatus(m_cActiveTracks & (1<<nTrack));
	}
}

CString CApplication::GetFailedStr (int err, bool abbrev)
{
	CString result;
	switch (abs(err)) {
		case PROCESS_ERROR :
		case IMAGE_SNAP_FAIL:
		case USER_ABORT :
			result = abbrev ? " ProcessError " : " Process Error";
			break;

		case VISION_TIMEOUT:
			result = abbrev ? " Timeout " : "Vision Timeout";
			break;
		case INSP_PASS:
			result = abbrev ? " Pass " : "Pass";
			break;
		case NO_IMAGE:
			result=abbrev ? " NoImage " : "No Image";
			break;
		case DEVICE_LOCATION:
			result = abbrev ? "DeviceLocation":"Device Location";
			break;
		case DATUM_LOCATION:
			result = abbrev ? "DatumLocation" : "Datum Location";
			break;
		case ENCAP_INSUFFICIENT:
			result = abbrev ? "EncapInsufficient" : "Encap Insufficient";
			break;
		case ENCAP_EXCESS:
			result = abbrev ? "ExcessEncap" : "Excess Encap";
			break;
		case ENCAP_CRACK:
			result = abbrev ? "CrackEncap" : "Crack Encap";
			break;
		case ENCAP_BLOWHOLE:
			result = abbrev ? "EncapBlowHole" : "Blow Hole";
			break;
		case ENCAP_CONTAMINATION:
			result = abbrev ? "Encap Contamination" : "Contamination";
			break;
		case ENCAP_VOID:
			result = abbrev ? "Encap Void" : "Void";
			break;
		case ENCAP_WIREANDIC:
			result = abbrev ? "Encap Wire And IC" : "Wire And IC";
			break;
		case ENCAP_BULGE:
			result = abbrev ? "Encap Bulge" : "Bulge";
			break;
		case ENCAP_GLUE:
			result = abbrev ? "Encap Glue" : "Encap Glue";
			break;
		// Contact Error
		case CONTACT_DAMAGE:
			result = abbrev ? "Contact Damage" : "Contact Damage";
			break;
		case CONTACT_CONTAMINATION:
			result = abbrev ? "Contact Contamination" : "Contact Contamination";
			break;
		// Flex Error
		case FLEX_OVERCUT:
			result = abbrev ? "Flex Overcut" : "Flex Overcut";
			break;
		case FLEX_DAMAGE:
			result = abbrev ? "Flex Damage" : "Flex Damage";
			break;
		case FLEX_CONTAMINATION:
			result = abbrev ? "Flex Contamination" : "Flex Contamination";
			break;
		case FLEX_SCRATCHES:
			result = abbrev ? "Flex Scratches" : "Flex Scratches";
			break;
		
		case CHICKLET_ROUNDNESS	:
			result = abbrev ? "Chicklet Swage Hole Roundness" : "Roundness";
			break;

		case CHICKLET_BURR:
			result = abbrev ? "Chicklet Swage Hole Burr" : "Burr";
			break;

		case RESULT_INVALID:
			result = abbrev ? " Unknown " :"Unknown";
			break;
	}
	return result;
}


CString CApplication::GetCuvetteFailedStr (int err, bool abbrev)
{
	CString result="";
	/*int nErrorCode = CUVETTE_DEFECT + err;
	switch (nErrorCode) {
		case 1 :
			result = abbrev ? " DarkScratches " : " Dark Scratches";
			break;

		case 2:
			result = abbrev ? " BigExcessParticles " : "Big Excess Particles";
			break;

		case 3 :
			result=abbrev ? " SandBeach " : "Sand Beach";
			break;

		case 4 :
			result = abbrev ? " DarkSpot " : " DarkSpot";
			break;

		case 5:
			result = abbrev ? " Contamination " : "Contamination";
			break;

		case 6 :
			result=abbrev ? " LightScratches " : "Light Scratches";
			break;

	}*/
	return result;
}


int CApplication::SavePresentationPassImages(int nTrack, int nError)
{
	CString strFailedStr, strFailedFileName, strPassFileName;
	CString strTime,strYear,strDay,strMonth,strHr,strMin,strSec;

	char strCurTime[] = {"            "};

	SYSTEMTIME sysCurTime;
	GetLocalTime(&sysCurTime);	
	/////////////

	strYear.Format("%d",sysCurTime.wYear);
	strYear = strYear.Right(2);

	if(sysCurTime.wMonth < 10)
		strMonth.Format("0%d",sysCurTime.wMonth);
	else
		strMonth.Format("%d",sysCurTime.wMonth);

	if(sysCurTime.wDay < 10)
		strDay.Format("0%d",sysCurTime.wDay);
	else
		strDay.Format("%d",sysCurTime.wDay);

	if(sysCurTime.wHour < 10)
		strHr.Format("0%d", sysCurTime.wHour);
	else
		strHr.Format("%d", sysCurTime.wHour);

	if(sysCurTime.wMinute < 10)
		strMin.Format("0%d", sysCurTime.wMinute);
	else
		strMin.Format("%d", sysCurTime.wMinute);

	if(sysCurTime.wSecond < 10)
		strSec.Format("0%d", sysCurTime.wSecond);
	else
		strSec.Format("%d", sysCurTime.wSecond);
	
	sprintf_s(strCurTime,"%s%s%s%s%s%s",strYear,strMonth,strDay,strHr,strMin,strSec);

	///Udupa; TODO
	//if(m_bLogDefImages && m_nSaveImgSelection){
	//		
	//	strPassFileName.Format("%s\\%s_%s_1_0.bmp", m_strPresentPassDirTrk[nTrack],m_strMachineID,strCurTime);

	//	SaveGrayScaleBMPFile(strPassFileName, m_pTracks[nTrack].m_pDoc[0]->m_Buffer);
	//}

	return 0;
}

int CApplication::SetVisOnLineState(BOOL bStatus)
{
	if(!m_bExitApp){
		m_pMainWnd->SendMessageA(WM_SET_ONLINE_STATUS);
	}

	CString strMsg;
	int nError;

	if (bStatus) {
		UpdateStatusBar(&CString(""), 2);
		UpdateStatusBar(&CString(""), 3);
	

		m_bLoadMultiImagesFromFile = FALSE;
	
		m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_MACHINE_ID_STATIC)->SetWindowTextA(m_strMachineID);
		m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_LOT_ID_STATIC)->SetWindowTextA(m_strLotInfo);
		m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_RECEIPE_STATUS_STATIC)->SetWindowTextA(m_strConfigFile);
	//	m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_CURTAIN_STATUS_STATIC)->SetWindowTextA("Guard Bypass is Off");
		//m_pDispOnlinePanel->m_strMachineId = m_strMachineID;
		//m_pDispOnlinePanel->m_strRecipe = m_strConfigFile;
		CRect rect1, rect2;
		SystemParametersInfo(SPI_GETWORKAREA, NULL, &rect1, 0);
		m_pDispOnlinePanel->GetWindowRect(&rect2);

		m_pDispOnlinePanel->SetWindowPos(NULL, 90, 0, rect2.Width(), rect2.Height(), SWP_NOSIZE);

		m_pDispOnlinePanel->ShowWindow(TRUE);

		if(m_bTrackInspSeq)
			m_bTrackInspSeq = FALSE;
		else {
			m_bStream = FALSE;
			InitializeLotStats();
			if (m_bLotOpened)
			{
				//if (m_strLotConfigFile.Compare(m_strConfigFile) != 0) {
				//	m_strConfigFile = m_strLotConfigFile;
				//	LoadRecipes();
				//}
				AddLogEvent("Loading Lot Info");
				((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.LoadLotInfo();
				((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.LoadTotalLotInfo();
				UpdateStatisticsWindow();
				AddLogEvent("Info Loaded");
			}
	
			m_pMainWnd->SendMessageA(WM_SET_FULLSCREEN, m_bOnline, TRUE);
		}
		ApplyDebugFlag(DEBUG_DRAW|DEBUG_PRINT|DEBUG_TIME|DEBUG_SAVE_BAD_IMAGE);

		AddLogEvent("Vision ONLINE");
	}
	else {
		m_pDispOnlinePanel->ShowWindow(FALSE);
		
		ApplyDebugFlag(DEBUG_DRAW|DEBUG_PRINT|DEBUG_TIME);
		
	}

	if(!m_bExitApp) {
		m_pMainWnd->SendMessageA(WM_SET_FULLSCREEN,m_bOnline, TRUE);
	}
	if(m_bAllocFailedImageBuffer) {
		for(int i=0;i<MAX_TRACKS;i++) {
			if(m_nFailImageCounter[i] >= 25)
				m_nFailImageCounter[i] = 0;
		}
	}

	((CMainFrame*)m_pMainWnd)->m_dlgPane->EnableGripper(!bStatus);
	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->EnableGripper(!bStatus);
	((CMainFrame*)m_pMainWnd)->m_wndOutputPane->EnableGripper(!bStatus);

	return 0;
}

BOOL SetPrivilege(HANDLE hToken,LPCTSTR lpszPrivilege,BOOL bChange)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	BOOL bReturnValue = FALSE;

	if (lpszPrivilege != NULL && !bChange)
	{
		if (LookupPrivilegeValue( 
			NULL,            // lookup privilege on local system
			lpszPrivilege,   // privilege to lookup 
			&luid )) 
		{      // receives LUID of privilege
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		}
	}

	AdjustTokenPrivileges(hToken,bChange,&tp,
		sizeof(TOKEN_PRIVILEGES), 
		(PTOKEN_PRIVILEGES) NULL,
		(PDWORD) NULL);  
	// Call GetLastError to determine whether the function succeeded.
	
	if (GetLastError() == ERROR_SUCCESS) 
	{ 
		bReturnValue = TRUE;
	} 
	 
	return bReturnValue;
} 

BOOL TakeOwnership(LPCTSTR lpszFile)
{
	int file[256];
	char error[256];
	DWORD description;
	SECURITY_DESCRIPTOR sd;
	SECURITY_INFORMATION info_owner=OWNER_SECURITY_INFORMATION;
		
	TOKEN_USER *owner = (TOKEN_USER*)file;
	HANDLE token;
	
	
	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_READ | TOKEN_ADJUST_PRIVILEGES,&token))
	{
		//To Get the Current Process Token & opening it to adjust the previleges
		if(SetPrivilege(token,SE_TAKE_OWNERSHIP_NAME,FALSE))
		{
			//Enabling the privilege
			if(GetTokenInformation(token,TokenUser,owner,sizeof(file),&description))
			{
				//Get the information on the opened token
				if(SetSecurityDescriptorOwner(&sd,owner->User.Sid,FALSE))
				{
					//replace any owner information present on the security descriptor
					if(SetFileSecurity(lpszFile,info_owner,&sd))
						return(TRUE);
					else
					{
						// Call GetLastError to determine whether the function succeeded.
						sprintf_s(error,"Error in SetFileSecurity Error No : %d",GetLastError());
						MessageBox(NULL,error,NULL,MB_OK);
					}//SetFileSecurity
				}
				else
				{
					sprintf_s(error,"Error in SetSecurityDescriptorOwner Error No : %d",GetLastError());
					MessageBox(NULL,error,NULL,MB_OK);
				}//SetSecurityDescriptorOwner
			}
			else
			{
				sprintf_s(error,"Error in GetTokenInformation Error No : %d",GetLastError());
				MessageBox(NULL,error,NULL,MB_OK);
			}//GetTokenInformation
		}
		else
		{
			sprintf_s(error,"Error in SetPrivilege No : %d",GetLastError());
			MessageBox(NULL,error,NULL,MB_OK);
		}//SetPrivilege
	}
	else
	{
		sprintf_s(error,"Error in OpenProcessToken No : %d",GetLastError());
		MessageBox(NULL,error,NULL,MB_OK);
	}//OpenProcessToken

	SetPrivilege(token,SE_TAKE_OWNERSHIP_NAME,TRUE);//Disabling the set previlege

	return(FALSE);
}




BOOL SetPermission(LPCTSTR lpszFile, LPCTSTR lpszAccess, DWORD dwAccessMask)
{
	int buff[512];
	char domain[512];
	char error[256];
	
	DWORD domain_size=sizeof(domain);
	DWORD acl_size;

	SECURITY_DESCRIPTOR sd1;
	SECURITY_INFORMATION info_dacl=DACL_SECURITY_INFORMATION;
	PSID sid = (PSID*)buff;
	ACL *acl;
	SID_NAME_USE sidname;
	DWORD sid_size=sizeof(buff);
	
	InitializeSecurityDescriptor(&sd1,SECURITY_DESCRIPTOR_REVISION);
	//to get the SID 
	if(LookupAccountName(NULL,lpszAccess,sid,&sid_size,domain,&domain_size,&sidname))
	{
		acl_size=sizeof(ACL)+sizeof(ACCESS_ALLOWED_ACE)-sizeof(DWORD)+GetLengthSid(buff);
		acl = (ACL *)malloc(acl_size);
		//To calculate the size of an ACL, 
		InitializeAcl(acl,acl_size,ACL_REVISION);

		if(AddAccessAllowedAce(acl,ACL_REVISION,dwAccessMask,sid))
		{
			if(SetSecurityDescriptorDacl(&sd1,TRUE,acl,FALSE))
			{
				if(SetFileSecurity(lpszFile,info_dacl,&sd1))
					return(TRUE);
				else
				{
					sprintf_s(error,"Error in SetFileSecurity Error No : %d",GetLastError());
					MessageBox(NULL,error,NULL,MB_OK);
				}//SetFileSecurity
			}
			else
			{
				sprintf_s(error,"Error in SetSecurityDescriptorDacl Error No : %d",GetLastError());
				MessageBox(NULL,error,NULL,MB_OK);
			}//SetSecurityDescriptorDacl
		}
		else
		{
			sprintf_s(error,"Error in AddAccessAllowedAce Error No : %d",GetLastError());
			MessageBox(NULL,error,NULL,MB_OK);
		}//AddAccessAllowedAce
	}
	else
	{
		sprintf_s(error,"Error in LookupAccountName No : %d",GetLastError());
		MessageBox(NULL,error,NULL,MB_OK);
	}//LookupAccountName

	free(acl);
	return(FALSE);
}

int DeleteDir(LPCTSTR lpszName)
{
	char lpszNameTemp[MAX_PATH];
	memset(lpszNameTemp, NULL, MAX_PATH);
	sprintf(lpszNameTemp, "%s", lpszName);

	SHFILEOPSTRUCT fileOpStruct;
	fileOpStruct.hwnd = theApp.m_pMainWnd->m_hWnd;
	fileOpStruct.wFunc = FO_DELETE;
	fileOpStruct.pFrom = lpszNameTemp;
	fileOpStruct.pTo = NULL;
	fileOpStruct.fFlags = FOF_NOCONFIRMATION  | FOF_SILENT;

	int nError = SHFileOperation(&fileOpStruct);

	return nError;
}

//	int ret = -1;
//	char name1[256];
//	WIN32_FIND_DATA info;
//    HANDLE hp;
////    int n;
//    char *cp;
//	
//	sprintf_s(name1, "%s\\*.*",lpszName);
//    hp = FindFirstFile(name1,&info);
//    if(!hp || hp==INVALID_HANDLE_VALUE)
//        return ret;
//    do
//    {
//		cp = info.cFileName;
//        if(cp[1]==0 && *cp=='.')
//            continue;
//        else if(cp[2]==0 && *cp=='.' && cp[1]=='.')
//            continue;
//        sprintf_s(name1,"%s\\%s",lpszName,info.cFileName);
//		if(info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)
//		{
//			SetFileAttributes(name1,info.dwFileAttributes&~FILE_ATTRIBUTE_READONLY);
//		}
//		if(info.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
//		{
//			TakeOwnership(name1);
//			SetPermission(name1,"everyone",GENERIC_ALL);
//			DeleteDir(name1);
//		}
//		else
//		{
//			TakeOwnership(name1);
//			SetPermission(name1,"everyone",GENERIC_ALL);
//			DeleteFile(name1);
//		}
//    }
//    while(FindNextFile(hp,&info));
//	FindClose(hp);
//	if(info.dwFileAttributes&FILE_ATTRIBUTE_READONLY)
//	{
//		SetFileAttributes(lpszName,info.dwFileAttributes&~FILE_ATTRIBUTE_READONLY);
//	}
//	if(RemoveDirectory(lpszName))
//	{
//		OutputDebugLog("Delete Directory success");
//		ret = 0;
//	}	
//	else
//	{
//		OutputDebugLog("error %d\n",GetLastError());
//	}
//	return ret;
//}

void CApplication::UpdateStatisticsWindow(BOOL bRefresh)
{
	///Udupa; TODO
	//if(!m_bOnlineViewing)
	//{
	//	//if(m_pStatsView && ::IsWindow(m_pStatsView->m_ctrlDefectsList.m_hWnd))
	//		//m_pStatsView->UpdateContents();
	//}

//	((CMainFrame*) m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdateContents(FALSE);
	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdateTotalContents();
//	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdateStatsInfo();
}

void CApplication::UpdateStatistics(size_t  GoodDieCount, size_t  BadDieCount)
{
	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdateStatistics(GoodDieCount, BadDieCount);
	UpdateStatisticsWindow();
}

void CApplication::UpdateStatisticsOnReScan(size_t  GoodDieCount, size_t  BadDieCount)
{
	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdateStatisticsOnReScan(GoodDieCount, BadDieCount);
//	UpdateStatisticsWindow();
}
void CApplication::UpdateStatisticsOnRepeatScan(size_t  GoodDieCount, size_t  BadDieCount)
{
	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdateStatisticsOnRepeatScan(GoodDieCount, BadDieCount);
	//	UpdateStatisticsWindow();
}

void CApplication::RecordLotStats()
{
	/*if(m_pStatsView)
		m_pStatsView->RecordLotStats();*/
	/*((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.RecordLotStats();
	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.RecordLotStats(TRUE);*/
}

int CApplication::AddLogEvent(CString strEvent,int nEventLog,BOOL m_bAuditTrail)
{
	/*if(m_pStatsView)
		return m_pStatsView->UpdateAuditTrail(strEvent, TRUE);*/

	if (m_pMainWnd == NULL)
		return -1;

	CString strEventLog;
	strEventLog.Format("     %s     %s      %s",m_UserAccInfo.m_strCurrentAccessLevel,m_UserAccInfo.m_strCurUserName, strEvent);
	m_pMainWnd->SendMessageA(WM_ADD_EVENT_LOG, (WPARAM)&strEventLog,nEventLog);
	
	if (nEventLog != COMM_LOG && m_bAuditTrail && ((CMainFrame*)m_pMainWnd)->m_dlgPaneStats != NULL)
		return ((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdateAuditTrail(strEvent, TRUE);
	else
		return 0;
	//return -1;
}



void CApplication::UpdateLotSummary(int nDevice,int nFov,int nView, int nError, BOOL allStats)
{

	TRACKSUMMARY* pDisplayStats = &m_LotSummary.m_DisplaySummary[nDevice][nFov][nView];
	
	m_bDisplayAllStatsOffline = TRUE;
	if(m_bDisplayAllStatsOffline || m_bOnline)
		pDisplayStats->lTotalUnitsInspected++; 

	if(nError==GOOD_UNIT){
		if(m_bDisplayAllStatsOffline || m_bOnline)
			pDisplayStats->lTotalUnitsPass++;
	}
	else{
		if(m_bDisplayAllStatsOffline || m_bOnline)
			pDisplayStats->lTotalUnitsFail++;
	}
	if(m_bDisplayAllStatsOffline || m_bOnline){
		pDisplayStats->dTotalYield = double(pDisplayStats->lTotalUnitsPass) /
										 double(pDisplayStats->lTotalUnitsInspected) * 100 + 0.04;
	}
	if(nError<GOOD_UNIT){
		switch (abs(nError)) {
			case IMAGE_SNAP_FAIL:
				break;
			case PROCESS_ERROR :
			case USER_ABORT :
				pDisplayStats->lTotalUnitsInvalid++;
				break;

			case INSP_FAIL_DOUBLE_LENS:
			case INSP_FAIL_NO_DEVICE:
			case DEVICE_LOCATION:
				pDisplayStats->lDieLocation++;
				break;
			case ENCAP_INSUFFICIENT:
				pDisplayStats->lEncapInsufficient++;
				break;
		
			case ENCAP_EXCESS:
				pDisplayStats->lEncapExcess++;
				break;
			case ENCAP_CRACK:
				pDisplayStats->lEncapCrack++;
				break;
			case ENCAP_BLOWHOLE:
				pDisplayStats->lEncapBlowHole++;
				break;
			case ENCAP_CONTAMINATION:
				pDisplayStats->lEncapContamination++;
				break;
			case ENCAP_VOID:
				pDisplayStats->lEncapVoid++;
				break;
			case ENCAP_WIREANDIC:
				pDisplayStats->lEncapWireAndIC++;
				break;
			case ENCAP_BULGE:
				pDisplayStats->lEncapBulge++;
				break;
			case ENCAP_GLUE:
				pDisplayStats->lEncapGlue++;
				break;
			// Contact Error
			case CONTACT_DAMAGE:
				pDisplayStats->lContactDamage++;
				break;
			case CONTACT_CONTAMINATION:
				pDisplayStats->lContactContamination++;
				break;
			// Flex Error
			case FLEX_OVERCUT:
				pDisplayStats->lFlexOvercut++;
				break;
			case FLEX_DAMAGE:
				pDisplayStats->lFlexDamage++;
				break;
			case FLEX_CONTAMINATION:
				pDisplayStats->lFlexContamination++;
				break;
			case FLEX_SCRATCHES:
				pDisplayStats->lFlexScratches++;
				break;
			case CHICKLET_ROUNDNESS:
				pDisplayStats->lChickletRoundness++;
				break;

			case CHICKLET_BURR:
				pDisplayStats->lChickletBurr++;
				break;

			case USER_MODIFIED:
				pDisplayStats->lUserModified++;
				break;
			default: {
				if(m_LotSummary.m_mapIndividualPviDefectCounter[nView].find(nError) != m_LotSummary.m_mapIndividualPviDefectCounter[nView].end()) {
					m_LotSummary.m_mapIndividualPviDefectCounter[nView][nError]++;
				}
			}
				break;
		}
	}
}


void CApplication::UpdateTotalLotSummary(int nTrack, int nError, BOOL allStats)
{
	
	TRACKSUMMARY* pDisplayTotalStats = &m_LotSummary.m_TotalDisplaySummary[nTrack];
	//if (m_bLotOpened && m_bOnline)
	
		pDisplayTotalStats->lTotalUnitsInspected++;

	if (nError == GOOD_UNIT) {
	
			pDisplayTotalStats->lTotalUnitsPass++;
	}
	else {
	
			pDisplayTotalStats->lTotalUnitsFail++;
	}
	
		pDisplayTotalStats->dTotalYield = double(pDisplayTotalStats->lTotalUnitsPass) /
			double(pDisplayTotalStats->lTotalUnitsInspected) * 100 + 0.04;
	
	if (nError<GOOD_UNIT) {
		switch (abs(nError)) {
		case IMAGE_SNAP_FAIL:
			break;
		case PROCESS_ERROR:
		case USER_ABORT:
			pDisplayTotalStats->lTotalUnitsInvalid++;
			break;
		case DEVICE_LOCATION:
			pDisplayTotalStats->lDieLocation++;
			break;
		case ENCAP_INSUFFICIENT:
			pDisplayTotalStats->lEncapInsufficient++;
			break;

		case ENCAP_EXCESS:
			pDisplayTotalStats->lEncapExcess++;
			break;

		case ENCAP_CRACK:
			pDisplayTotalStats->lEncapCrack++;
			break;

		case ENCAP_BLOWHOLE:
			pDisplayTotalStats->lEncapBlowHole++;
			break;

		case ENCAP_CONTAMINATION:
			pDisplayTotalStats->lEncapContamination++;
			break;

		case ENCAP_VOID:
			pDisplayTotalStats->lEncapVoid++;
			break;

		case ENCAP_WIREANDIC:
			pDisplayTotalStats->lEncapWireAndIC++;
			break;

		case ENCAP_BULGE:
			pDisplayTotalStats->lEncapBulge++;
			break;

		case ENCAP_GLUE:
			pDisplayTotalStats->lEncapGlue++;
			break;
		// Contact Error
		case CONTACT_DAMAGE:
			pDisplayTotalStats->lContactDamage++;
			break;

		case CONTACT_CONTAMINATION:
			pDisplayTotalStats->lContactContamination++;
			break;
		// Flex Error
		case FLEX_OVERCUT:
			pDisplayTotalStats->lFlexOvercut++;
			break;

		case FLEX_DAMAGE:
			pDisplayTotalStats->lFlexDamage++;
			break;

		case FLEX_CONTAMINATION:
			pDisplayTotalStats->lFlexContamination++;
			break;

		case FLEX_SCRATCHES:
			pDisplayTotalStats->lFlexScratches++;
			break;

		case CHICKLET_ROUNDNESS:
			pDisplayTotalStats->lChickletRoundness++;
			break;

		case CHICKLET_BURR:
			pDisplayTotalStats->lChickletBurr++;
			break;

		case USER_MODIFIED:
			pDisplayTotalStats->lUserModified++;
			break;
		default: 
			if(m_LotSummary.m_mapIndividualPviDefectCounter[nTrack].find(nError) != m_LotSummary.m_mapIndividualPviDefectCounter[nTrack].end()) {
				m_LotSummary.m_mapIndividualPviDefectCounter[nTrack][nError]++;
			}
			break;
		}
	}
}

void CApplication::LoadRecipes()
{
	if (m_pProgressDlg != NULL) {
		int nLoadedDataPercentage = m_pProgressDlg->m_nCurrentPos;

		m_pProgressDlg->ShowWindow(TRUE);
		m_bProgressDlgOpen = TRUE;
		m_pProgressDlg->UpdateWindow();
		m_pProgressDlg->SetStatus("Loading Configuration....", nLoadedDataPercentage);
	}

	motionParameters.LoadScanningParm();
	motionParameters.LoadDeviceParameters();
	motionParameters.LoadZTeachPosition();
	motionParameters.loadScanningOptions();

	CString strTemp = m_strMainCfgDir +"\\"+m_strMainCfgModeDir;
	_mkdir(strTemp);
	m_strConfigDir.Format("%s\\%s", strTemp, m_strConfigFile);
	_mkdir(m_strConfigDir);

	

	for (int nTrack = 0; nTrack<m_nTotalTracks; nTrack++) {
		int m_nNoofFov;
		if (nTrack == STATION_TOP)
			m_nNoofFov = motionParameters.GetTotalFOVPerDieTop();
		else if (nTrack == STATION_THR)
			m_nNoofFov = motionParameters.GetTotalFOVPerDieTHR();

		if (m_nNoofFov > MAX_FOV) {
			m_nNoofFov = MAX_FOV;
		}
		for (int nDoc = 0; nDoc < m_pTracks[nTrack].m_nTotalDocs; nDoc++) {
			for (int nFov = 0; nFov < m_nNoofFov; nFov++) {
				m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.ClearAllNccModels();
			}
		}
	}

	if (m_pProgressDlg != NULL) {
		m_pProgressDlg->SetStatus("Loading Configuration....", 10);
	}

	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
		m_pTracks[nTrack].LoadConfiguration();
	}

	SetEvent(m_EventRefreshLight);//// Set Event for Refresh Light

	SetRecipeName(m_strConfigFile);

	InitializeDefectCode();
	m_pVerificationMode->Initialize();
//	m_pImageSelectionDlg->VerificationDefects();

//	LoadDefectCodeParameters(m_strConfigDir, m_strConfigFile);
	motionParameters.CreateScanPositions();
	motionParameters.CreateNewPositions();
//	((CMainFrame*)m_pMainWnd)->InitializeDefectProperties();
	
	ResizeDieSelectionDlg();

	if (m_pProgressDlg != NULL) {
		m_pProgressDlg->SetStatus("Configuration Loaded....", 100);
		m_pProgressDlg->ShowWindow(FALSE);
		m_bProgressDlgOpen = FALSE;
		m_pProgressDlg->m_nCurrentPos = 0;
	}

	m_pMainWnd->ShowWindow(SW_SHOW);
}

void CApplication::LoadRecipes(int nTrack, int nFov)
{
	motionParameters.LoadScanningParm();
	motionParameters.LoadDeviceParameters();
	motionParameters.LoadZTeachPosition();
	motionParameters.loadScanningOptions();

	CString strTemp = m_strMainCfgDir + "\\" + m_strMainCfgModeDir;
	_mkdir(strTemp);
	m_strConfigDir.Format("%s\\%s", strTemp, m_strConfigFile);
	_mkdir(m_strConfigDir);

	for(int nDoc = 0; nDoc < m_pTracks[nTrack].m_nTotalDocs; nDoc++) {
		m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.ClearAllNccModels();
	}

	m_pTracks[nTrack].LoadConfiguration(nFov);

	SetEvent(m_EventRefreshLight);//// Set Event for Refresh Light

	SetRecipeName(m_strConfigFile);

	InitializeDefectCode();
	//	LoadDefectCodeParameters(m_strConfigDir, m_strConfigFile);
	motionParameters.CreateScanPositions();
//	((CMainFrame*) m_pMainWnd)->InitializeDefectProperties();

	ResizeDieSelectionDlg();

}

void CApplication::AutoLoadRecipes()
{
	m_strConfigFile = GetRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Recipe", "");
	if(m_strConfigFile.IsEmpty()) {
		m_strConfigFile = "Default";
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Recipe", m_strConfigFile);
	}

	m_pMainWnd->SendMessage(WM_LOAD_RECIPE);
}

void CApplication::InitializePositionProperties()
{

}

void CApplication::SaveRecipeInfo()
{
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Folder:Config Mode", m_strMainCfgModeDir);
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Recipe", m_strConfigFile);
}

int CApplication::CopyRecipe(CString strRecipeSrcMode,CString strRecipeDesMode,CString strRecipeSrc, CString strRecipeDst)
{
	CString strSrc, strDest, strSrc1, strDest1;

	strSrc.Format("%s\\%s", m_strMainCfgDir, strRecipeSrcMode);
	strDest.Format("%s\\%s", m_strMainCfgDir, strRecipeDesMode);
	_mkdir(strDest);

	strSrc1.Format("%s\\%s", strSrc, strRecipeSrc);
	strDest1.Format("%s\\%s", strDest, strRecipeDst);
	_mkdir(strDest1);

	HANDLE hFile;
	int nCount = 0;
	WIN32_FIND_DATA FindFileData;
	int nFlag = 0;
	if ((hFile = FindFirstFile(strSrc1 + "\\*.", &FindFileData)) != INVALID_HANDLE_VALUE) {
		do {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				CString str = FindFileData.cFileName;
				if (str.Find(".") == -1/* && str.CompareNoCase("_Common")!=0*/) {
					nFlag = 1;
					break;
				}
			}
		} while (FindNextFile(hFile, &FindFileData));
	}
	FindClose(hFile);
	if (nFlag == 1) {
		strSrc.Format("%s\\%s.mot", strSrc1, strRecipeSrc);
		strDest.Format("%s\\%s.mot", strDest1, strRecipeDst);
		CopyFileA(strSrc, strDest, FALSE);

		strSrc.Format("%s\\DefectPrioritySelection.dat", strSrc1);
		strDest.Format("%s\\DefectPrioritySelection.dat", strDest1);
		CopyFileA(strSrc, strDest, FALSE);

		strSrc.Format("%s\\DeviceConfiguration.dat", strSrc1);
		strDest.Format("%s\\DeviceConfiguration.dat", strDest1);
		CopyFileA(strSrc, strDest, FALSE);

		strSrc.Format("%s\\Scanning Parameters.info", strSrc1);
		strDest.Format("%s\\Scanning Parameters.info", strDest1);
		CopyFileA(strSrc, strDest, FALSE);

		strSrc.Format("%s\\Scanning Options.info", strSrc1);
		strDest.Format("%s\\Scanning Options.info", strDest1);
		CopyFileA(strSrc, strDest, FALSE);
		
		strSrc.Format("%s\\%s.dec", strSrc1, strRecipeSrc);
		strDest.Format("%s\\%s.dec", strDest1, strRecipeDst);
		CopyFileA(strSrc, strDest, FALSE);


		for (int nTrack = 0; nTrack<m_nTotalTracks; nTrack++)
			m_pTracks[nTrack].CopyRecipe(strRecipeSrcMode, strRecipeDesMode, strRecipeSrc, strRecipeDst);
		return 0;
	}
	else {
		AfxMessageBox("Source Receipe is Empty");
		return -1;
	}

}

void CApplication::ApplyDebugFlag(long lDebugFlag)
{
	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++)
		for(int nDoc=0; nDoc<m_pTracks[nTrack].m_nTotalDocs; nDoc++)
			if(m_pTracks[nTrack].m_pDoc[nDoc])
				m_pTracks[nTrack].m_pDoc[nDoc]->m_lDebugFlag=lDebugFlag;
}


void CApplication::SaveCrashImages(int nTrack)
{
	if(m_bSaveCrashImages) {
		ULARGE_INTEGER ulAvailable, ulTotal, ulFree;

		BOOL bDiskDrive = GetDiskFreeSpaceEx((LPCTSTR) "E:\\", &ulAvailable, &ulTotal, &ulFree);
		int nFreeSpace = (int)(ulFree.QuadPart/1000000);

		if(nFreeSpace >= 400 && bDiskDrive) {
			CString strFolder, strCrashFileName, strTemp;

			strFolder.Format("%s\\Crash Images Folder%d",m_ImageSaveFolder, m_nSystemRestartCounter);
			_mkdir(strFolder);

			strCrashFileName.Format("%s\\Track%d_No%d.bmp", strFolder, nTrack+1, m_nCrashImageCounter[nTrack]++);

			SaveGrayScaleBMPFile(strCrashFileName, m_pTracks[nTrack].m_pDoc[0]->m_Buffer);

			if(m_nCrashImageCounter[nTrack] >= 25)
				m_nCrashImageCounter[nTrack] = 0;
		}
	}
}

void CApplication::CheckConfiguration()
{
	//HANDLE hFile;
	//WIN32_FIND_DATA FindFileData;
	//hFile = FindFirstFile(m_strMainCfgDir + "\\" + m_strConfigFile,&FindFileData);
	//
	//if(hFile == INVALID_HANDLE_VALUE)
	//{ 
	//	CString strMsg;
	//	int nCreateDefaultConfig = 0;
	//	nCreateDefaultConfig = GetRegistryEntry(READ_REGPATH + "Preferences","Create Default Config", nCreateDefaultConfig);
	//	PutRegistryEntry(WRITE_REGPATH + "Preferences","Create Default Config", 0);//Reset to Zero Each Time
	//	strMsg.Format("The Configuration File \"%s\" is Not Found in Configuration Directory.",m_strConfigFile);
	//	if(nCreateDefaultConfig)
	//	{  
	//		if(AfxMessageBox(strMsg + "\n\n\tDo You Want To Create Default Configuration ? ",MB_YESNO|MB_ICONQUESTION) == IDYES)
	//		{
	//			m_strConfigFile = "Default";
	//			return;
	//		}
	//		else{
	//			
	//			AfxMessageBox("Application Need To Close. Please Manually Copy the Configuration File !",MB_OK|MB_ICONSTOP);
	//			exit(0);
	//		}
	//	}
	//	
	//	else
	//	{
	//		AfxMessageBox(strMsg + " Application Need To Close \n\n\t\tPlease Manually Copy the Configuration File !",MB_OK|MB_ICONSTOP);
	//		exit(0);
	//		
	//	}
	//}
	//else
	//	return;
	//
	//
}

BOOL CApplication::CheckDiskFreeSpace()
{
	ULARGE_INTEGER ulAvailable, ulTotal, ulFree;
	CString strImageDir, strMsg, strTemp;
//	int nCount;
	strImageDir.Format("%s", m_ImageSaveFolder);
	strTemp = strImageDir;
	
	BOOL bDiskDrive = GetDiskFreeSpaceEx((LPCTSTR)strImageDir, &ulAvailable, &ulTotal, &ulFree);
	int nFreeSpace = (int)(ulFree.QuadPart/1000000);
	
	if(!bDiskDrive){
		AfxMessageBox("Please Specify a valid Drive for Image saving");
		return FALSE;
	}
	
	if(nFreeSpace <= 200) {
		strMsg.Format("Not Enough Space in Image Saving Directory %s\nDo You Want to Delete Old Images ?",strImageDir);
		if(AfxMessageBox(strMsg,MB_ICONSTOP | MB_YESNO) == IDYES){
			strImageDir.Format("%s\\IMAGES",strTemp);
			DeleteDir(strImageDir);
			AfxMessageBox("   Deleted Old Images   ");
			return TRUE;
			
		}
		else
			return FALSE;
	}
	else
		return TRUE;
	
}

void CApplication::OnVisionOnline()
{

	for(int i=0; i<m_nTotalTracks; i++) {
		for(int j=0; j<m_nTotalViewDocs[i]; j++)
			m_pTracks[i].m_pDoc[j]->SetImageTitle(NULL, !m_bOnline);
	}

	InitializeLotStats();
		
	m_bOnline = !m_bOnline;
	
	SetVisOnLineState(m_bOnline);
}

void CApplication::OnUpdateVisionOnline(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bOnline);
    bool bLive = FALSE;
    bool bStoredImageInsp = FALSE;
	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
		bLive |= m_pTracks[nTrack].m_bLiveImage;
		bStoredImageInsp |= m_pTracks[nTrack].m_pDoc[0] != NULL && m_pTracks[nTrack].m_pDoc[0]->m_bInspSavedImage;
	}

	pCmdUI->Enable(!bLive &&
				   !m_bTeaching &&
				   !m_bInspecting &&
				   !m_bTrackInspSeq &&
				   !bStoredImageInsp && 
				   (m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nProdMode));
}

void CApplication::OnGooffline()
{
	if(m_bOnline)
		OnVisionOnline();
}

void CApplication::Goonline()
{
	if (!m_bOnline)
		OnVisionOnline();
}

void CApplication::OnOnlineResetstatscounter()
{
	
	//CSystemLoggingInfo* pSystemLoggingInfo;
 //   pSystemLoggingInfo = new CSystemLoggingInfo();
	//SYSTEMTIME Stime;
	//GetLocalTime(&Stime);
	//pSystemLoggingInfo->SaveStatisticsCounterReset(&Stime);
	m_LotSummary.InitDisplaySummary();
	UpdateStatisticsWindow(TRUE);
	m_strParmFolder.Format("%s\\ParmDatabase", m_LotSummaryRootDir);
	//m_pStatsView->RecordLotStats();
	//((CMainFrame*) m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.RecordLotStats();
	AddLogEvent("Statistical Counters are Reset");
}

void CApplication::OnUpdateOnlineResetstatscounter(CCmdUI *pCmdUI)
{
	BOOL bTrackLiveImg, bDocInsp;
	for (int nTrack = 0; nTrack<m_nTotalTracks; nTrack++) {
		bTrackLiveImg &= m_pTracks[nTrack].m_bLiveImage;
		bDocInsp &= m_pTracks[nTrack].m_pDoc[0]->m_bInspecting;


	}
	BOOL bStepInspectionEnabled = FALSE;
	BOOL bStoredInspectionEnabled = FALSE;
	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			bStepInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspSavedImage;
			bStoredInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bStoredImages;
		}
	}

	pCmdUI->Enable(!m_bOnline &&
		!bStepInspectionEnabled &&
		!m_bTeaching &&
		!m_bTrackInspSeq &&
		!m_bInspecting &&
		!bTrackLiveImg &&
		!bDocInsp &&
		!bStoredInspectionEnabled &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nResetStatCounters));
}

void CApplication::OnOnlineStatsResetMode()
{
    CStatisticsResetDlg StaisticsReset;	
	StaisticsReset.DoModal();
}

void CApplication::OnUpdateOnlineStatsResetMode(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nResetStatCounters);
}

void CApplication::OnOnlineSavePassImages()
{
	m_bSaveOnlinePassImages = !m_bSaveOnlinePassImages;

	if(!CheckDiskFreeSpace())
		m_bSaveOnlinePassImages = FALSE;
	else {
		for(int i=0; i<m_nTotalTracks+1; i++)
			OutputDebugLogTo(i+1, TRUE, m_bSaveOnlinePassImages ? "*** PASS-Image-Save enabled ***" : "*** PASS-Image-Save disabled ***");

		if(m_bSaveOnlinePassImages == TRUE)
			AddLogEvent("Online Save Pass images enabled");
		else 
			AddLogEvent("Online Save Pass images Disabled");
	}

	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SavePassImages", m_bSaveOnlinePassImages);
}

void CApplication::OnUpdateOnlineSavePassImages(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nSaveOnlinePassImages);
	pCmdUI->SetCheck(m_bSaveOnlinePassImages);
}

void CApplication::OnOnlineSaveFailImages()
{
	m_bSaveOnlineFailImages = !m_bSaveOnlineFailImages;

	if(!CheckDiskFreeSpace())
		m_bSaveOnlineFailImages = FALSE;
	else {
		for(int i=0; i<m_nTotalTracks+1; i++)
			OutputDebugLogTo(i+1, TRUE, m_bSaveOnlineFailImages ? "*** FAIL-Image-Save enabled ***" : "*** FAIL-Image-Save disabled ***");

		if(m_bSaveOnlineFailImages == TRUE)
			AddLogEvent("Online Save fail images enabled");
		else 
			AddLogEvent("Online Save fail images Disabled");
	}

	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SaveFailImages", m_bSaveOnlineFailImages);
}

void CApplication::OnUpdateOnlineSaveFailImages(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nSaveOnlineFailImages);
	pCmdUI->SetCheck(m_bSaveOnlineFailImages);
}

void CApplication::OnOnlineSaveJpegImages()
{
	m_bSaveOnlineJpegImages = !m_bSaveOnlineJpegImages;
	m_bSaveOnlineBMPImages = !m_bSaveOnlineJpegImages;

	if(!CheckDiskFreeSpace())
		m_bSaveOnlineJpegImages = FALSE;
	else {
		for(int i=0; i<m_nTotalTracks+1; i++)
			OutputDebugLogTo(i+1, TRUE, m_bSaveOnlineJpegImages ? "*** Online Save Jpeg Format images enabled ***" : "*** Online Save Jpeg Format images Disabled ***");

		if(m_bSaveOnlineJpegImages == TRUE)
			AddLogEvent("Online Save Jpeg Format images enabled");
		else 
			AddLogEvent("Online Save Jpeg Format images Disabled");
	}

	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SaveJpegImages", m_bSaveOnlineJpegImages);
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SaveBMPImages", m_bSaveOnlineBMPImages);

}

void CApplication::OnUpdateOnlineSaveJpegImages(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nSaveOnlineJpegImages);
	pCmdUI->SetCheck(m_bSaveOnlineJpegImages);
}

void CApplication::OnOnlineSaveBMPImages()
{
	m_bSaveOnlineBMPImages = !m_bSaveOnlineBMPImages;
	m_bSaveOnlineJpegImages = !m_bSaveOnlineBMPImages;

	if (!CheckDiskFreeSpace())
		m_bSaveOnlineBMPImages = FALSE;
	else {
		for (int i = 0; i<m_nTotalTracks + 1; i++)
			OutputDebugLogTo(i + 1, TRUE, m_bSaveOnlineBMPImages ? "*** Online Save BMP Format images enabled ***" : "*** Online Save BMP Format images Disabled ***");

		if (m_bSaveOnlineBMPImages == TRUE)
			AddLogEvent("Online Save BMP Format images enabled");
		else
			AddLogEvent("Online Save BMP Format images Disabled");
	}
	
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SaveJpegImages", m_bSaveOnlineJpegImages);
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "SaveBMPImages", m_bSaveOnlineBMPImages);
}

void CApplication::OnUpdateOnlineSaveBMPImages(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nSaveOnlineBMPImages);
	pCmdUI->SetCheck(m_bSaveOnlineBMPImages);
}

void CApplication::OnOnlineContinueTopImageInsp()
{
	m_bOnlineContinueTopImageInsp = !m_bOnlineContinueTopImageInsp;

	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "ContinueTopImageInsp", m_bOnlineContinueTopImageInsp);
}

void CApplication::OnUpdateOnlineContinueTopImageInsp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nInspect);
	pCmdUI->SetCheck(m_bOnlineContinueTopImageInsp);
}

void CApplication::OnParmRecipeLoad()
{
	CMultipleRecipeDialog dlg;
	dlg.DoModal();
}

void CApplication::OnUpdateParmRecipeLoad(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nManageCfg) &&
		!m_pTracks[0].m_bLiveImage && (m_nTotalTracks<2 || !m_pTracks[1].m_bLiveImage));
}

void CApplication::OnParmScanBarcode()
{
	if(m_pScanBarcodeDlg)
		delete m_pScanBarcodeDlg;

	m_pScanBarcodeDlg = new CScanBarcode();
	if(!m_pScanBarcodeDlg->Create(IDD_SCAN_BARCODE_DIALOG, AfxGetMainWnd()))
	{
		delete m_pScanBarcodeDlg;
		m_pScanBarcodeDlg = NULL;
		return;
	}

	CRect rect1, rect2;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rect1, 0);
	m_pScanBarcodeDlg->GetWindowRect(&rect2);

	m_pScanBarcodeDlg->SetWindowPos(NULL, rect1.Width() - rect2.Width(), rect1.Height()-rect2.Height()-20, 0, 0, SWP_NOSIZE);
	m_pScanBarcodeDlg->ShowWindow(TRUE);
	m_pScanBarcodeDlg->DisableQuitButton(m_bOnline);
}

void CApplication::OnUpdateParmScanBarcode(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline && 
		(m_UserAccInfo.m_nCurrentAccessLevel >=1 /*m_AdvSecurityDlg.m_data.m_nScanBarcode*/));
}

//void CApplication::OnParmRecipeSaveAs()
//{
//	CCopyRecipeDlg dlg;
//	dlg.DoModal();
//}
//
//void CApplication::OnUpdateParmRecipeSaveAs(CCmdUI *pCmdUI)
//{
//	pCmdUI->Enable(!m_bOnline && 
//		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nCreateConfig));
//}
//
//void CApplication::OnParmRecipeDelete()
//{
//	CDeleteRecipeDlg dlg;
//	dlg.DoModal();
//}
//
//void CApplication::OnUpdateParmRecipeDelete(CCmdUI *pCmdUI)
//{
//	pCmdUI->Enable(!m_bOnline && 
//		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nCreateConfig));
//}

void CApplication::OnUpdateParmConfig(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(!m_bOnline && (TRUE || m_bSuperUserMode));
}

void CApplication::SetRecipeName(CString strRecipe)
{
	m_pMainWnd->SendMessage(WM_SET_RECIPE_NAME,(WPARAM)&strRecipe, (LPARAM)&m_strMainCfgModeDir);
}

void CApplication::SetUserName(CString strUser)
{
	CMFCRibbonBar* pRibbon = &((CMainFrame*) m_pMainWnd)->m_wndRibbonBar;
	CMFCRibbonBaseElement* pElement = pRibbon->FindByID(ID_USER_BUTTON, TRUE, TRUE);
	pElement->SetText(strUser);
	pRibbon->ForceRecalcLayout();
}

void CApplication::SetOnlineStatus()
{
	CMFCRibbonBar* pRibbon = &((CMainFrame*) m_pMainWnd)->m_wndRibbonBar;
	CMFCRibbonBaseElement* pElement = pRibbon->FindByID(ID_VISIONSTATUS_BUTTON, TRUE, TRUE);
	pElement->SetText(m_bOnline ? "ONLINE" : "OFFLINE");
}

void CApplication::OnRecipeButton()
{
	OnParmRecipeLoad();
}

void CApplication::OnUpdateRecipeButton(CCmdUI *pCmdUI)
{
	BOOL bStepInspectionEnabled = FALSE;
	
	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			bStepInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspSavedImage;
		}
	}
	pCmdUI->Enable(!m_bOnline &&
		!bStepInspectionEnabled &&
		!m_bTeaching &&
		!m_bInspecting &&
		!m_bInspectionAll &&
		!m_bInspSequenceOffline &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nManageCfg) &&
		!m_pTracks[0].m_bLiveImage && (m_nTotalTracks<2 || !m_pTracks[1].m_bLiveImage));
}

void CApplication::OnUpdateModeAndProductCodeButton(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CApplication::OnUpdateHardwareButton(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline);
}

void CApplication::SaveADSTwinCATSetting()
{
	CString strSec;
	if (m_strCommOPC.CompareNoCase("TwinCAT")==0) {
		strSec.Format("ADS PortNr");
		PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_nADSPortNr);
		strSec.Format("ADS CycleTime");
		PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_nCycleTime);
		strSec.Format("ADS MaxDelay");
		PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_nMaxDelay);
		strSec.Format("ADS Timeout");
		PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_nTimeout);

		for(int i=0; i<6; i++) {
			//m_byteNetID[i] = 0;
			strSec.Format("NetID%d", i);
			PutRegistryEntry(WRITE_REGPATH + "ADSTwinCAT", strSec, m_byteNetID[i]);
		}
	}
}

void CApplication::OnConfigurationAdstwincatsettings() 
{
	//CADSTwinCATdlg dlg;

	//dlg.m_nNetID0 = m_byteNetID[0];
	//dlg.m_nNetID1 = m_byteNetID[1];
	//dlg.m_nNetID2 = m_byteNetID[2];
	//dlg.m_nNetID3 = m_byteNetID[3];
	//dlg.m_nNetID4 = m_byteNetID[4];
	//dlg.m_nNetID5 = m_byteNetID[5];

	//dlg.m_nADSPortNr = m_nADSPortNr;
	//dlg.m_nMaxDelay  = m_nMaxDelay;
	//dlg.m_nCycleTime = m_nCycleTime;
	//dlg.m_nTimeout   = m_nTimeout;


	//if (dlg.DoModal()==IDOK) {
	//	m_byteNetID[0] = dlg.m_nNetID0;
	//	m_byteNetID[1] = dlg.m_nNetID1;
	//	m_byteNetID[2] = dlg.m_nNetID2;
	//	m_byteNetID[3] = dlg.m_nNetID3;
	//	m_byteNetID[4] = dlg.m_nNetID4;
	//	m_byteNetID[5] = dlg.m_nNetID5;


	//	m_nMaxDelay	 = dlg.m_nMaxDelay;
	//	m_nCycleTime = dlg.m_nCycleTime;
	//	m_nTimeout   = dlg.m_nTimeout;
	//	m_nADSPortNr = dlg.m_nADSPortNr;

	//	SaveADSTwinCATSetting();
	//	AfxMessageBox("Please restart the application!",MB_OK);
	//}	
}

void CApplication::OnUpdateConfigurationAdstwincatsettings(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(!m_bOnline && 
	//			   (m_strCommOPC.CompareNoCase("TwinCAT")==0) &&
	//			   m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nCameraCfg);
}

void CApplication::UpdateWatchDog(int nData)
{
	m_pTracks[0].UpdateWatchDog(nData);
}

void CApplication::UpdateLotChangedAck()
{
	m_pTracks[0].UpdateLotChangedAck();
}

void CApplication::OnUpdateOnlinemodeOptions(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CApplication::OnUpdateToolsSettings(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline);
}


void CApplication::OnUpdateToolsUser(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CApplication::OnSettingsUsers() 
{
	CModifyAccounts dlg;
	dlg.m_nLogoutTime = m_UserAccInfo.m_nLogoutTime;
	if(dlg.DoModal() == IDOK) {
		m_UserAccInfo.m_nLogoutTime = dlg.m_nLogoutTime;
		CString strAutoLogout;
		strAutoLogout.Format("Auto Logout time changed to %d minutes", m_UserAccInfo.m_nLogoutTime);
		AddLogEvent(strAutoLogout);
		PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "UserAutoLogoutMins", m_UserAccInfo.m_nLogoutTime);
	}	
}

void CApplication::OnUpdateSettingsUsers(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel == 2);
}

void CApplication::OnSettingsAccessRights() 
{
	CAdvSecurityDlg dlg;
	if(dlg.DoModal() == IDOK)
		m_AdvSecurityDlg.GetSecurityAccess();
}

void CApplication::OnUpdateSettingsAccessRights(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel == 2);
}

void CApplication::OnUserLogIn()
{
	int p;
	CLoginDlg dlg;
	SYSTEMTIME sysTime;
	CString strUsername, strUserpwd, strLevel;
	CString m_strAccessLevel, strStoredLevel;
	CString strStoredPswd, strPassword;
	bool nClickedOK = FALSE;
	CString strMsg, strHr, strMin, strSec;
		
	GetLocalTime(&sysTime);
	time(&m_UserAccInfo.tmStartLogoutTimeHr);

	m_UserAccInfo.m_strCurrentAccessLevel = "Operator";
	m_UserAccInfo.m_strCurUserName = "Operator";
	m_UserAccInfo.m_bAccessLevel = false;
	
	if(dlg.DoModal() == IDOK) {
		strUsername = dlg.m_strUsername;
		strUserpwd = dlg.m_strPassword;
		m_UserAccInfo.m_strCurrentAccessLevel = dlg.m_strUserLevel;
		nClickedOK = TRUE;
	}
	
	if(nClickedOK) {
		if(strUsername.CompareNoCase(m_UserAccInfo.DefaultUser.strUsername) == 0) {
			BOOL bGivenAccess = FALSE;
			//strUserpwd += "ADMN";
			CString strEncr = m_strAppId;
			CString strOutput, strMd5;
			strStoredPswd = m_UserLog.DecodePassWord(&m_UserAccInfo.DefaultUser.strPassword);
			strOutput = EncryptDecrypt(strStoredPswd);
			int nAppidLength = m_strAppId.GetLength();
			CString strTempAppID = strOutput.Left(nAppidLength);
			if (strTempAppID.Compare(m_strAppId) == 0) {
				strMd5 = strOutput.Right(strOutput.GetLength() - nAppidLength);
				if (strUserpwd.Compare(strMd5) == 0) {
					if (m_UserAccInfo.m_strCurrentAccessLevel.Compare(m_UserAccInfo.DefaultUser.strAccLevel) == 0) {
						m_strAccessLevel = "ADMN";
						m_UserAccInfo.m_bAccessLevel = true;
						m_UserAccInfo.m_strCurUserName = strUsername;
						m_UserAccInfo.m_nCurrentAccessLevel = 2;
						((CMainFrame*)m_pMainWnd)->ViewFullScreen(FALSE);
						AddLogEvent("Logged In");
					}
					else {
						m_UserAccInfo.m_nCurrentAccessLevel = 0;
						m_UserAccInfo.m_strCurrentAccessLevel = "Operator";
						AfxMessageBox("Please check your access level");
						AddLogEvent("Login incorrect");
					}
				}
				else {
					m_UserAccInfo.m_nCurrentAccessLevel = 0;
					m_UserAccInfo.m_strCurrentAccessLevel = "Operator";
					AfxMessageBox("Wrong Password");
					AddLogEvent("Login incorrect");
				}
			}
			else {
				m_UserAccInfo.m_nCurrentAccessLevel = 0;
				m_UserAccInfo.m_strCurrentAccessLevel = "Operator";
				AfxMessageBox("Wrong Password");
				AddLogEvent("Login incorrect");
			}
		}
		else {
			if(m_UserAccInfo.m_strCurrentAccessLevel.CompareNoCase("Administrator") == 0) {
				m_strAccessLevel = "ADMN";
				m_UserAccInfo.m_nCurrentAccessLevel = 2;
			}
			else if(m_UserAccInfo.m_strCurrentAccessLevel.CompareNoCase("Technician") == 0){ 
				m_strAccessLevel = "TECH";
				m_UserAccInfo.m_nCurrentAccessLevel = 1;
			}
			else{
				m_strAccessLevel = "OPRT";
				m_UserAccInfo.m_nCurrentAccessLevel = 0;
			}
			strPassword = strUserpwd + m_strAccessLevel;
			if(m_UserAccInfo.nNumofUsers > 0) {
				for(p = 0; p < m_UserAccInfo.nNumofUsers; p++) {
					if(strUsername.CompareNoCase(m_UserAccInfo.UserAccount[p].strUsername) == 0) {
						BOOL bGivenAccess = FALSE;
						strStoredPswd = m_UserAccInfo.UserAccount[p].strPassword;
						strStoredPswd = m_UserLog.DecodePassWord(&strStoredPswd);

						CString strOutput, strMd5;

						strOutput = EncryptDecrypt(strStoredPswd);
						int nAppidLength = m_strAppId.GetLength();
						CString strTempAppID = strOutput.Left(nAppidLength);
						if (strTempAppID.Compare(m_strAppId) == 0) {
							strMd5 = strOutput.Right(strOutput.GetLength()-nAppidLength);
							if (strPassword.Compare(strMd5) == 0) {
								m_UserAccInfo.m_strCurUserName = strUsername;
								if ((m_strAccessLevel == "ADMN") || (m_strAccessLevel == "TECH")) {
									if (strMd5 == strPassword)
										AddLogEvent("Logged in");
									m_UserAccInfo.m_bAccessLevel = true;
									((CMainFrame*)m_pMainWnd)->ViewFullScreen(FALSE);
								}
								bGivenAccess = TRUE;
							}
							else {
								m_strAccessLevel = "";
								m_UserAccInfo.m_nCurrentAccessLevel = 0;
								m_UserAccInfo.m_strCurrentAccessLevel = "Operator";
								AfxMessageBox("Wrong Password");
								AddLogEvent("Login incorrect");
								bGivenAccess = TRUE;
							}
						}
						if (bGivenAccess)
							break;
					  
					}
					else
						continue;
				}
				if(p == m_UserAccInfo.nNumofUsers){
					m_strAccessLevel = "";
					m_UserAccInfo.m_strCurUserName = "Operator";
					m_UserAccInfo.m_nCurrentAccessLevel = 0;
					m_UserAccInfo.m_strCurrentAccessLevel = "Operator";
					AfxMessageBox("User Name doesn't exist");
					AddLogEvent("Login incorrect");
				}
			}
			else {
				m_strAccessLevel = "";
				m_UserAccInfo.m_strCurUserName = "Operator";
				m_UserAccInfo.m_nCurrentAccessLevel = 0;
				m_UserAccInfo.m_strCurrentAccessLevel = "Operator";
				AfxMessageBox("User Name doesn't exist");
				AddLogEvent("Login incorrect");
			}
		}
		if(m_UserAccInfo.m_strCurUserName.CompareNoCase("Operator")!=0){
			
			CSystemLoggingInfo* pSystemLoggingInfo;
			pSystemLoggingInfo = new CSystemLoggingInfo();
			
			pSystemLoggingInfo->SaveLoginDetails(&sysTime,
												 m_UserAccInfo.m_strCurUserName,
												 m_strAccessLevel);

			if(pSystemLoggingInfo)
				delete pSystemLoggingInfo;
			pSystemLoggingInfo = NULL;
			

        }

		//LaunchWindowsExplorer();
		SetUserName(m_UserAccInfo.m_strCurUserName);
		//AddLogEvent("Logged In");
	}	
}

void CApplication::OnUpdateUserLogIn(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_UserAccInfo.m_strCurUserName.CompareNoCase("Operator"));
}

void CApplication::OnUserLogOut()
{
	SYSTEMTIME sysTime;
	CString strMsg, strHr, strMin, strSec;
	
	m_UserAccInfo.m_bAccessLevel = false;
	GetLocalTime(&sysTime);

	CSystemLoggingInfo* pSystemLoggingInfo;
    pSystemLoggingInfo = new CSystemLoggingInfo();

	pSystemLoggingInfo->SaveLogoutDetails(&sysTime);

	AddLogEvent("Logged Out");

	//((CMainFrame*)m_pMainWnd)->ViewFullScreen(TRUE);
	m_UserAccInfo.m_strCurUserName = "Operator";
	m_UserAccInfo.m_nCurrentAccessLevel = 0;
	SetUserName(m_UserAccInfo.m_strCurUserName);
	m_UserAccInfo.m_strCurrentAccessLevel = "Operator";

	//LaunchWindowsExplorer();
	if(pSystemLoggingInfo) {
		delete pSystemLoggingInfo;
		pSystemLoggingInfo = NULL;
	}
}

void CApplication::OnUpdateUserLogOut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_UserAccInfo.m_strCurUserName.CompareNoCase("Operator"));
}


void CApplication::OnUserChangePassword() 
{
	CIniFile iniFile;
	CString strSec, strDataFileName;
	
	strDataFileName =  m_strMainCfgDir + "\\" + "LogAccount" + ".lgn";
	iniFile.SetPath(strDataFileName);
	iniFile.ReadFile();
	
	LoginInfo TempChangeNewPswd;
	CString strEncodePswd, strAccLevel, strPswd, strLevel;


	int p;
	LoginInfo* pInfo = NULL;
	if(m_UserAccInfo.m_strCurUserName.CompareNoCase("Engineer") == 0) {
		pInfo = &m_UserAccInfo.DefaultUser;
		strAccLevel = "Administrator";
		strLevel = "ADMN";
		p = -1;
	}
	else {
		for(p = 0; p < m_UserAccInfo.nNumofUsers; p++) {
			if(m_UserAccInfo.m_strCurUserName.CompareNoCase(m_UserAccInfo.UserAccount[p].strUsername) == 0) {
				strAccLevel = m_UserAccInfo.UserAccount[p].strAccLevel;
				if(strAccLevel.CompareNoCase("Administrator") == 0) 
					strLevel = "ADMN";
				else if(strAccLevel.CompareNoCase("Technician") == 0) 
					strLevel = "TECH";
				else if(strAccLevel.CompareNoCase("Operator") == 0) 
					strLevel = "OPRT";
				pInfo = &m_UserAccInfo.UserAccount[p];
				break;
			}
			else
				continue;
		}
	}
	if(!pInfo) {
		AfxMessageBox("User Name doesn't exist");
		return;
	}

	CChangePasswordDlg dlg;
	if(dlg.DoModal() == IDOK) {
		strPswd = m_UserAccInfo.m_strNewPassword + strLevel;
		strEncodePswd = m_UserLog.EncodePassWord(&strPswd);
		TempChangeNewPswd.strUsername = m_UserAccInfo.m_strCurUserName;
		TempChangeNewPswd.strAccLevel = strAccLevel;
		TempChangeNewPswd.strPassword = strEncodePswd;


		pInfo->strUsername = m_UserAccInfo.m_strCurUserName;
		pInfo->strAccLevel = strAccLevel;
		pInfo->strPassword = strEncodePswd;

		strSec.Format("User%d", p+1);
		iniFile.SetCString(strSec, "Name", m_UserAccInfo.m_strCurUserName);
		iniFile.SetCString(strSec, "Level", strAccLevel);
		iniFile.SetCString(strSec, "Pswd", strEncodePswd);
		iniFile.WriteFile();
	}
}

void CApplication::OnUpdateUserChangePassword(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_UserAccInfo.m_strCurUserName.CompareNoCase("Operator"));
}

void CApplication::OnUserButton()
{
	if(m_UserAccInfo.m_strCurUserName.CompareNoCase("Operator")) {
		CString str;
		str.Format("Log-out user '%s'?", m_UserAccInfo.m_strCurUserName);
		if(AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION)==IDYES)
			OnUserLogOut();
	}
	else
		OnUserLogIn();
}

void CApplication::OnUpdateUserButton(CCmdUI *pCmdUI)
{
	///pCmdUI->Enable(TRUE);
	pCmdUI->Enable((!m_bOnline || m_bSuperUserMode));// && 
		           //!m_UserAccInfo.m_strCurUserName.CompareNoCase("Operator"));
}


void CApplication::OnVisionstatusButton()
{
	OnVisionOnline();
}

void CApplication::OnUpdateVisionstatusButton(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bOnline);
    
	bool bLive = FALSE;
    bool bStoredImageInsp = FALSE;
	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
		bLive |= m_pTracks[nTrack].m_bLiveImage;
		bStoredImageInsp |= ((m_pTracks[nTrack].m_pDoc[0] != NULL) && m_pTracks[nTrack].m_pDoc[0]->m_bInspSavedImage);
	}

	pCmdUI->Enable(!bLive &&
				   !m_bTeaching &&
				   !m_bInspecting &&
				   !m_bTrackInspSeq &&
				   !bStoredImageInsp && 
				   (m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nProdMode));
}

void CApplication::OnHardwareIO()
{
	/*if(m_pIoHardwareDlg)
		m_pIoHardwareDlg->ShowWindow(TRUE);
	
	CRect rect1, rect2;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rect1, 0);
	m_pIoHardwareDlg->GetWindowRect(&rect2);

	m_pIoHardwareDlg->SetWindowPos(NULL, rect1.Width() - rect2.Width(), rect1.Height() - rect2.Height() - 0, 0, 0, SWP_NOSIZE);*/

	if (pIOControlDialog) {
		pIOControlDialog->ShowWindow(SW_SHOW);
	}
}

void CApplication::OnUpdateHardwareIO(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline && 
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nIOTest));
}

void CApplication::OnOnlineBypassResult()
{
	m_bByPassResult = !m_bByPassResult;
	if(m_bByPassResult == TRUE)
		AddLogEvent("Online Bypass Result is Enabled");
	else
		AddLogEvent("Online Bypass Result is Disabled");
}

void CApplication::OnUpdateOnlineBypassResult(CCmdUI *pCmdUI)
{
	BOOL bEnable = FALSE;
	pCmdUI->Enable(!m_bOnline && 
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nByPassResult) && bEnable);
	pCmdUI->SetCheck(m_bByPassResult);
}

void CApplication::OnCameraSynchronousGrab()
{
	m_bSynchronousGrabEnable = !m_bSynchronousGrabEnable;
	PutRegistryEntry(WRITE_REGPATH + "Preferences", "Synchronous Grab: Enable", m_bSynchronousGrabEnable);
}

void CApplication::OnUpdateCameraSynchronousGrab(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bSynchronousGrabEnable);
	pCmdUI->Enable(!m_bOnline);
}

int CApplication::LoadDocImages(CString strFileName)
{
	int nError;
	int nIndex = -1;
	int nStrLength = strFileName.GetLength();
	CString strImageFinalPath;
	for(int nDoc=0; nDoc<m_nTotalViews; nDoc++){
		nIndex = strFileName.Find("SSB.bmp");
		if(nIndex > 0)
			strImageFinalPath = strFileName.Left(nStrLength-7);
		else
			strImageFinalPath = strFileName.Left(nStrLength-6);
		switch(nDoc){
			case 0:
				strImageFinalPath = strImageFinalPath + "T1.bmp";
				break;
			case 1:
				strImageFinalPath = strImageFinalPath + "T1.bmp";
				break;
			case 2:
				strImageFinalPath = strImageFinalPath + "T3.bmp";
				break;
			case 3:
				strImageFinalPath = strImageFinalPath + "T4.bmp";
				break;
			case 4:
				strImageFinalPath = strImageFinalPath + "T5.bmp";
				break;
			case 5:
				strImageFinalPath = strImageFinalPath + "B1.bmp";
				break;
			case 6:
				strImageFinalPath = strImageFinalPath + "B2.bmp";
				break;
			case 7:
				strImageFinalPath = strImageFinalPath + "B3.bmp";
				break;
			case 8:
				strImageFinalPath = strImageFinalPath + "B4.bmp";
				break;
			case 9:
				strImageFinalPath = strImageFinalPath + "B5.bmp";
				break;
			case 10:
				strImageFinalPath = strImageFinalPath + "S1.bmp";
				break;
			case 11:
				strImageFinalPath = strImageFinalPath + "S2.bmp";
				break;

		}
		nError = m_pTracks[0].m_pDoc[nDoc]->LoadStaticImage(strImageFinalPath);
	}
	return nError;
}

int CApplication::SynchronousCameraSnap(BOOL bDraw)
{
	//CPerformanceTimer TimerTrack[3];
	//int nError = 0;
	//
	//OutputDebugLogTo(9,TRUE,"CApplication::SynchronousCameraSnap Start");
	//for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
	//	m_pTracks[nTrack].m_pDoc[0]->CameraGrabUsingExposure();
	//}
	//OutputDebugLogTo(9,TRUE,"CApplication::SynchronousCameraSnap End");

	//for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++)
	//	SetEvent(m_pTracks[nTrack].m_EventGrabEnd);

	return 0;
}

void CApplication::SynchronousCameraStream()
{
	if(!m_pTracks[0].m_bCamAvail)
		return;

	BOOL bLive = m_pTracks[0].m_bLiveImage;
	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++)
		m_pTracks[nTrack].m_bLiveImage = !bLive;

	if(bLive) {
		SetEvent(m_pTracks[0].m_EventKillLive);
		ResetEvent(m_EventMotionSensor);
		Sleep(100);
	}
	else {
		SetEvent(m_EventMotionSensor);
		AfxBeginThread(TrackLiveAll, this, THREAD_PRIORITY_LOWEST);
	}
}

void CApplication::OnViewOptionsAuditTrail()
{
	// TODO: Add your command handler code here
	//if(m_pAuditTrailDlg)
	//	delete m_pAuditTrailDlg;

	//m_pAuditTrailDlg = new CAuditTrailDlg();
	//if(!m_pAuditTrailDlg->Create(IDD_AUDITTRAIL_DLG, AfxGetMainWnd())) {
	//	delete m_pAuditTrailDlg;
	//	m_pAuditTrailDlg = NULL;
	//	return;
	//}

	//CRect rect1, rect2;
	//SystemParametersInfo(SPI_GETWORKAREA, NULL, &rect1, 0);
	//m_pAuditTrailDlg->GetWindowRect(&rect2);

	//int r = rect1.Width();
	//int t = rect2.Width();
	//m_pAuditTrailDlg->CenterWindow();
	////m_pAuditTrailDlg->SetWindowPos(NULL, rect1.Width() - rect2.Width()-700, rect1.Height()-rect2.Height()-200, 0, 0, SWP_NOSIZE);
	//m_pAuditTrailDlg->ShowWindow(TRUE);

	CAuditTrailDlg dlg;
	dlg.DoModal();

}


void CApplication::OnUpdateViewOptionsAuditTrail(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(TRUE);
	pCmdUI->Enable(!m_bOnline && m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nAuditTrail);
}

//void CApplication::LaunchWindowsExplorer()
//{
//	#ifdef _DEBUG
// 
//	#else
//		if(m_UserAccInfo.m_nCurrentAccessLevel > 1){
//			STARTUPINFO si;
//			PROCESS_INFORMATION pi;
//
//			ZeroMemory( &si, sizeof( si ) );
//			si.cb = sizeof( si );
//			CreateProcess( NULL, "C:\\WINDOWS\\explorer.exe", NULL, NULL, false, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi );	
//		}else{
//			int retval = ::_tsystem( _T("taskkill /F /IM explorer.exe") );
//
//			//PROCESSENTRY32 entry;
//			//entry.dwFlags = sizeof( PROCESSENTRY32 );
//
//			//HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
//
//			//if ( Process32First( snapshot, &entry ) == TRUE ) {
//			//  		while ( Process32Next( snapshot, &entry ) == TRUE ) {
//			//  			if ( stricmp( entry.szExeFile, "explorer.exe" ) == 0 ) {
//			//  				HANDLE hProcess = OpenProcess( /*PROCESS_QUERY_INFORMATION | PROCESS_VM_READ |*/ PROCESS_TERMINATE, FALSE, entry.th32ProcessID );
//			//			UINT exitCode = 0;
//			//  				TerminateProcess( hProcess, exitCode );
//
//			//  				CloseHandle( hProcess );
//
//			//  				break;
//			//  			}
//			//  		}
//			//}
//		}
//	#endif
//	
//}

CString CApplication::DecodeLicenseKey(CString strContent, CString strDecodeKey)
{
	CString strDecodedContent;
	int nPwdLength, nKeyLength;
	unsigned char* strCharContent;
	//unsigned char nameChar[1];

	nPwdLength = strContent.GetLength();
	strCharContent = new unsigned char[nPwdLength];
	nKeyLength = strDecodeKey.GetLength();

	for(int i=0, j=0; i<nPwdLength; i++, j++)
	{
		unsigned char charContent = strContent.GetAt(i);
		strCharContent[i] = charContent;
		if(j==nKeyLength)
			j=0;
		unsigned char charKey = strDecodeKey.GetAt(j);
		strCharContent[i] -= charKey;
		unsigned char charDecodedContent = strCharContent[i];
		if(charDecodedContent < 32 || charDecodedContent > 127)
		{
			charDecodedContent = charDecodedContent - 32 + 127;
			strCharContent[i] = charDecodedContent;
		}
		strDecodedContent += charDecodedContent;
	}
	delete[] strCharContent;

	int nTotal = strDecodedContent.GetLength();
	return strDecodedContent;
}

CString CApplication::EncodeKey(CString strContent, CString strKey)
{
	CString strEncodedContent;
	int nPwdLength, nKeyLength;
	unsigned char* strCharContent;
	//unsigned char nameChar[1];

	nPwdLength = strContent.GetLength();
	strCharContent = new unsigned char[nPwdLength];
	nKeyLength = strKey.GetLength();

	for(int i=0, j=0; i<nPwdLength; i++, j++)
	{
		unsigned char charContent = strContent.GetAt(i);
		strCharContent[i] = charContent;
		if(j==nKeyLength)
			j=0;
		unsigned char charKey = strKey.GetAt(j);
		strCharContent[i] += charKey;
		unsigned char charEncodedContent = strCharContent[i];
		if(charEncodedContent > 127)
		{
			charEncodedContent = charEncodedContent - 127 + 32;
			strCharContent[i] = charEncodedContent;
		}
		strEncodedContent += charEncodedContent;
	}
	delete[] strCharContent;

	int nTotal = strEncodedContent.GetLength();
	return strEncodedContent;
}

int CApplication::CheckApplicationLicense()
{
	
	BOOL bLicenseFound = FALSE;
	CStdioFile file;
	CFileException e;

	CString strLine, strLicenseDate, strDecodedKey;
	CString strFile = m_strMainCfgDir+"\\EmageEquipmentLicense.lic";
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
//	char str[256];

	//strDate.Format("%04d%02d%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	CString strCurrentDate, strPreviousDate, strTempCurrentDate;
	strPreviousDate = GetRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Domain Information", "");
	if(strPreviousDate.IsEmpty()){
		strPreviousDate = "";
		PutRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Domain Information", strPreviousDate);
	}

	if(strPreviousDate != "")
		strPreviousDate = DecodeLicenseKey(strPreviousDate, "NeverKnow");

	CTime CurrentDate, TempCurrentDate;
	CTimeSpan TimeSpan(31, 0, 0, 0);
	CurrentDate = CTime(sysTime.wYear, sysTime.wMonth, sysTime.wDay,0,0,0);
	TempCurrentDate = CurrentDate + TimeSpan;
	strCurrentDate.Format("%d%02d%02d",CurrentDate.GetYear(), CurrentDate.GetMonth(), CurrentDate.GetDay());
	int nCurrentDate = sysTime.wYear*10000 + sysTime.wMonth*100 + sysTime.wDay;
	
	if(!file.Open(strFile, CFile::modeRead, &e))
	{
		AfxMessageBox("No License Found");
		return FALSE;
	}
	else
	{
		int nLine = 0;
		while(file.ReadString(strLine))
		{
			if(nLine != 6)
				nLine++;
			else
			{
				CString strDecode1 = DecodeLicenseKey(strLine, "NeverKnow");
				strLicenseDate = strDecode1.Right(8);
				strTempCurrentDate.Format("%d%02d%02d",TempCurrentDate.GetYear(), TempCurrentDate.GetMonth(), TempCurrentDate.GetDay());
				if(strPreviousDate == "" && atoi(strLicenseDate) > atoi(strTempCurrentDate)) {
					AfxMessageBox("Invalid License");
					return FALSE;
				}
				CString strDecode2 = DecodeLicenseKey(strDecode1, strLicenseDate);
				int nIndex = strDecode2.GetLength();
				strDecodedKey = strDecode2.Left(nIndex-8);

				// Check License for which apps
				int nAppNameIndexEnd = strDecodedKey.Find("Application");
				CString strAppName = strDecodedKey.Left(nAppNameIndexEnd);
				strAppName = strAppName.Right(nAppNameIndexEnd-13);
				if(strAppName.Compare(m_strAppId))
				{
					AfxMessageBox("Incorrect License!");
					return FALSE;
				}

				int nPhyAddrStart = strDecodedKey.Find(":");
				strDecodedKey = strDecodedKey.Right(nIndex-8-nPhyAddrStart-1);
				strDecodedKey = strDecodedKey.Left(12);
				if(!strDecodedKey.Compare("xxxxxxxxxxxx"))
				{
					if(strPreviousDate != "" && atoi(strPreviousDate) > atoi(strCurrentDate)) {
						AfxMessageBox("Invalid License");
						return FALSE;
					}
					else {
						int nLicenseDate = atoi(strLicenseDate);
						int nTrialLeft = nLicenseDate - nCurrentDate;
						if(nTrialLeft <= 0)
						{
							AfxMessageBox("License Expired!");
							return FALSE;
						}
						bLicenseFound = TRUE;
					}
				}
				else
				{
					// Compare decoded Physical Address with Physical Address of PC

					DWORD dwSize = 0;
					DWORD dwRetVal = 0;
					ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

					ULONG family = AF_UNSPEC;

					LPVOID lpMsgBuf = NULL;
					PIP_ADAPTER_ADDRESSES pAddresses = NULL;
					ULONG outBufLen = 0;
					ULONG Iterations = 0;

					PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
					PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
					PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
					PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
					IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
					IP_ADAPTER_PREFIX *pPrefix = NULL;
					outBufLen = 15000;

					do 
					{
						pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
						if (pAddresses == NULL) 
						{
							AfxMessageBox("Memory allocation failed for IP_ADAPTER_ADDRESSES struct");
							return FALSE;
						}

						dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

						if (dwRetVal == ERROR_BUFFER_OVERFLOW) 
						{
							FREE(pAddresses);
							pAddresses = NULL;
						} 
						else 
							break;
						Iterations++;

					} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < 3));

					if (dwRetVal == NO_ERROR)
					{ 
						// If successful, output some information from the data we received
						pCurrAddresses = pAddresses;
						while (pCurrAddresses) 
						{
							CString strMacAddr, strTemp;
							if (pCurrAddresses->PhysicalAddressLength != 0) 
							{
								for (int i = 0; i < (int) pCurrAddresses->PhysicalAddressLength; i++) 
								{
									strTemp.Format("%.2X", (int) pCurrAddresses->PhysicalAddress[i]);
									strMacAddr = strMacAddr + strTemp;
								}
								if(!strMacAddr.Compare(strDecodedKey))
								{
									bLicenseFound = TRUE;
									break;
								}
							}
							pCurrAddresses = pCurrAddresses->Next;
						}
					}
					else
						AfxMessageBox("GetAdaptersAddresses failed");
				}
			}
		}
	}
	if(!bLicenseFound)
		AfxMessageBox("Incorrect License!");

	strCurrentDate = EncodeKey(strCurrentDate, "NeverKnow");
	PutRegistryEntry(READ_REGPATH + "Preferences\\Persistence", "Domain Information", strCurrentDate);

	return bLicenseFound;
}

void CApplication::OnViewZoomAll()
{
	// TODO: Add your command handler code here
	m_bZoomAll = !m_bZoomAll;
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "Zoom All: Enable", m_bZoomAll);
}


void CApplication::OnUpdateViewZoomAll(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bOnline  && m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nZoomAll);
	pCmdUI->SetCheck(!m_bOnline && m_bZoomAll);
}

void CApplication::OnOpenLot()
{
	 // TODO: Add your command handler code here
	int nError = OpenLotDlg();
	if(nError!=-1){
		OnVisionOnline();
	}
}


int CApplication::OpenLotDlg()
{
	CLotInfoDlg dlg;
	 CString strMsg;
	 SYSTEMTIME sysTime;
	 GetLocalTime(&sysTime);
	 dlg.m_strLotID.Format("%02u-%02u-%04u_%02u-%02u-%02u", 
		sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	 dlg.m_strOperator = m_UserAccInfo.m_strCurUserName;
	 dlg.m_strConfigFile = m_LotSummary.strPkgName;
	 dlg.m_strComments = m_LotSummary.strComments;
	 
	 if (dlg.DoModal()==IDCANCEL){
		return -1;
	 }

	// ((CMainFrame*)AfxGetMainWnd())->WriteToPipe("*" + m_strConfigFile + "\n");
	 m_LotSummary.strStartLotTime.Format("%02u:%02u:%02u", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	 m_bLotOpened=TRUE;
	
	 UpdateStatisticsWindow(TRUE);
	/* m_strConfigFile = dlg.m_strConfigFile;
	 m_strLotInfo = dlg.m_strLotID;*/
	 LoadRecipes();
	/// m_LotSummary.strMachineID = dlg.m_strMachineID;
	 m_strLotInfo = dlg.m_strLotID;
	 m_LotSummary.strOperatorID = dlg.m_strOperator;
	 m_strLotConfigFile = dlg.m_strConfigFile;
	 m_LotSummary.strComments = dlg.m_strComments;
 
	 
	 PutRegistryEntry(WRITE_REGPATH + "Preferences", "Lot Opened", m_bLotOpened);
	 PutRegistryEntry(WRITE_REGPATH + "Preferences", "Lot Info", m_strLotInfo);
	 strMsg.Format("Lot Opened - Lot ID :: [%s] - Operator :: [%s] - Recipe :: [%s]",m_LotSummary.strLotID, m_LotSummary.strOperatorID, m_LotSummary.strPkgName);
	 AddLogEvent(strMsg);

	 return 0;

}

void CApplication::OnUpdateOpenLot(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(0);
	 //// TODO: Add your command update UI handler code here
	 //BOOL bBusy = FALSE;
	 //for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++)
	 // bBusy |= m_pTracks[nTrack].m_bContInsp ||
		// m_pTracks[nTrack].m_bTrackContInsp ||
		// m_pTracks[nTrack].m_bLiveImage;
	 //pCmdUI->Enable(!bBusy &&
		//		   !m_bLotOpened &&
		//	//       !m_bOnline &&
		//		   !m_bTeaching &&
		//		   !m_bInspecting &&
		//		   (m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nOpenLot));

}


void CApplication::OnCloseLot()
{
 // TODO: Add your command handler code here

	 CFile   file;
	 CString strDataFileName;
	 CString strCrntFile,str, strMsg;

	 str.Format("Do you wish to close the lot - %s?", m_strLotInfo);
	 if(AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION)==IDNO)
	  return;
 
	 strMsg.Format("Lot Closed - Lot ID :: [%s] - Operator :: [%s] - Recipe :: [%s]",m_LotSummary.strLotID, m_LotSummary.strOperatorID, m_LotSummary.strPkgName);
	 AddLogEvent(strMsg);

	 //m_bOnline=TRUE;
	 if(m_bOnline)
		OnVisionOnline();
 
	 SYSTEMTIME sysTime;
	 GetLocalTime(&sysTime);
	 m_LotSummary.strEndTimeDay.Format("%02u:%02u:%02u", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
//	 ((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdateLotInfo();				Girish Commented, 2018/09/23
//	 ((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdateTotalLotInfo();
	 m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	 m_bLotOpened = FALSE;
	 m_strLotInfo = "";
	 m_strLotConfigFile = "";
	 m_LotSummary.Init();

	 PutRegistryEntry(WRITE_REGPATH + "Preferences", "Lot Opened",m_bLotOpened);
	 PutRegistryEntry(WRITE_REGPATH + "Preferences", "Lot Info",m_strLotInfo);
	 PutRegistryEntry(WRITE_REGPATH + "Preferences", "Lot Info Receipe", m_strLotConfigFile);
	 UpdateStatisticsWindow(TRUE);

}


void CApplication::OnUpdateCloseLot(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(0);
 // TODO: Add your command update UI handler code here
 /*pCmdUI->Enable(m_bLotOpened &&
				(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nCloseLot));*/
}

//void CApplication::OnCameraStream()
//{
//	if(!m_pTracks->m_bCamAvail)
//		return;
//
//	if(GetKeyState(VK_SHIFT) & 0x8000)
//		StreamAllTracksToggle();
//	else
//		m_bSynchronousGrabEnable ? SynchronousCameraStream() : m_pTracks->m_pDoc[0]->CameraLive();
//}


//void CApplication::OnUpdateCameraStream(CCmdUI *pCmdUI)
//{
//	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(m_pTracks->m_bCamEnable && 
//					m_pTracks->m_bCamAvail &&
//				   !m_bOnline &&
//				   !m_bTeaching &&
//				   !m_bInspecting &&
//				   !m_pTracks->m_bContInsp &&
//				   !m_bTrackInspSeq &&
//				   (m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nLive));
//	
//	pCmdUI->SetCheck(m_pTracks->m_bLiveImage);
//}


void CApplication::OnInspectionLoop()
{
	m_bStoredImagesLoop = !m_bStoredImagesLoop;
	PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "StoredImagesLoop", m_bStoredImagesLoop);
}

void CApplication::OnUpdateInspectionLoop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline && (m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nInspectLoop));
	pCmdUI->SetCheck(m_bStoredImagesLoop);
}


void CApplication::RefreshDefectsList()
{

	((CMainFrame*) m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.RefreshDefectsList();
}

void CApplication::InitializeLotStats()
{
	((CMainFrame*) m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.InitializeLotStats();
}

void CApplication::UpdateUPH(double dStripTime, double dNetSequenceTime, int nDieNo)
{

	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdateUPH( dStripTime, dNetSequenceTime, nDieNo);
}

void CApplication::UpdatePrintBar(size_t BadDevicecount)
{

	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdatePrintBar(BadDevicecount);
}

void CApplication::UpdatePrintBarOnReScan(size_t BadDevicecount)
{

	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdatePrintbarOnReScan(BadDevicecount);
}

void CApplication::UpdatePrintBarOnRepeatScan(size_t BadDevicecount)
{

	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.UpdatePrintbarOnRepeatScan(BadDevicecount);
}


void CApplication::OnDefaultSetting()
{

	CPerformanceTimer timer;
	timer.Reset();
	CRect rectClient;

	
	((CMDIFrameWndEx *)m_pMainWnd)->BeginWaitCursor();
	((CMDIFrameWndEx *)m_pMainWnd)->LockWindowUpdate();
	m_nTrackCountGroup = 0;
	
	int nLoadedDataPercentage = m_pProgressDlg->m_nCurrentPos;
	double dStep = 10 * 1.0 / m_nTotalViews, dLoadedDataPercentage = nLoadedDataPercentage;

	for (int i = m_nTotalViews -1 ; i >= 0; i--) {
		m_pFrames[i]->ActivateFrame();
		((CMDIFrameWndEx *)m_pMainWnd)->MDITabNewGroup(TRUE);
	}
	for (int i = 0; i < m_nTotalViews; i++) {
		m_pFrames[i]->ActivateFrame();
		((CMDIFrameWndEx *)m_pMainWnd)->MDITabMoveToNextGroup(TRUE);

		if (m_pProgressDlg != NULL) {
			dLoadedDataPercentage += dStep;
			nLoadedDataPercentage = dLoadedDataPercentage;
			m_pProgressDlg->SetStatus("Initialize The Doc View...", nLoadedDataPercentage);
		}
	}

	((CMainFrame*) m_pMainWnd)->ShowStatsPane();
	
	((CMainFrame*) m_pMainWnd)->ShowPreviousImages();

	((CMainFrame*)m_pMainWnd)->OnShowOutputLog();

	((CMainFrame*) m_pMainWnd)->HideParmPanes();

	((CMainFrame*) m_pMainWnd)->RecalcLayout();
	((CMDIFrameWndEx *)m_pMainWnd)->UnlockWindowUpdate();
	((CMDIFrameWndEx *)m_pMainWnd)->EndWaitCursor();
	OutputDebugLogTo(0,TRUE,"Default view timing %3.2f", timer.GetElapsedMSec());
}

void CApplication::OnUpdateDefaultSetting(CCmdUI *pCmdUI)
{
	BOOL nCheck=FALSE;
		const CObList& TabGrps = ((CMDIFrameWndEx *)m_pMainWnd)->GetMDITabGroups();
		int nTabGroupCount = TabGrps.GetCount();
		POSITION pos = TabGrps.GetHeadPosition();
		for (int i= 0;i<nTabGroupCount;i++)
		{
			CMFCTabCtrl* pNextWnd = DYNAMIC_DOWNCAST(CMFCTabCtrl, TabGrps.GetNext(pos));
			int nTabCount = pNextWnd->GetTabsNum();
			/*switch(i)
			{
				case 0:if(nTabCount>m_nTop || nTabCount<m_nTop)
							nCheck =  TRUE;
						break;
				case 1:if(nTabCount>m_nBottom || nTabCount<m_nBottom)
							nCheck =  TRUE;
						break;
				case 2:if(nTabCount>m_nSide || nTabCount<m_nSide)
							nCheck =  TRUE;
						break;
			}*/
			
		}
	pCmdUI->Enable(nCheck);
}

void CMDIClientAreaWnd::MDITabMoveToNextGroup(BOOL bNext)
{
	CApplication *m_pApp = (CApplication*) AfxGetApp();
	CMFCTabCtrl* pActiveWnd = FindActiveTabWndByActiveChild();
	
	if (pActiveWnd == NULL)
	{
		return;
	}
	ASSERT_VALID(pActiveWnd);
	POSITION pos;
	if(m_pApp->m_nTrackCountGroup == 0)
		pos = m_lstTabbedGroups.GetHeadPosition();
	else{
		pos = m_lstTabbedGroups.Find(pActiveWnd);
		bNext ? m_lstTabbedGroups.GetNext(pos) : m_lstTabbedGroups.GetPrev(pos);
	}

	if (pos == NULL)
	{
		return;
	}
	CMFCTabCtrl* pNextTabWnd = DYNAMIC_DOWNCAST(CMFCTabCtrl, m_lstTabbedGroups.GetAt(pos));
	ASSERT_VALID(pNextTabWnd);

	MoveWindowToTabGroup(pActiveWnd, pNextTabWnd);
}

void CMDIClientAreaWnd::MDITabNewGroup(BOOL bVert)
{
	// if m_groupAlignment has already been set (e.g. it's not GROUP_NO_ALIGN) the bVert parameter should be ignored and
	// the new group should be created according to the existing alignment (because we do not support nested tabbed groups)
	CApplication *m_pApp = (CApplication*) AfxGetApp();

	if (m_groupAlignment == GROUP_VERT_ALIGN)
	{
		bVert = TRUE;
	}
	else if (m_groupAlignment == GROUP_HORZ_ALIGN)
	{
		bVert = FALSE;
	}

	CMFCTabCtrl* pActiveWnd = FindActiveTabWndByActiveChild();
	if (pActiveWnd == NULL)
	{
		return;
	}
	ASSERT_VALID(pActiveWnd);

	CRect rect;
	GetClientRect(&rect);
	ScreenToClient(&rect);
	int cx = (int)rect.Width()/3;
	int cy = (int) rect.Height();
//////////////
	int arrx[3] = { 0, cx,  cx<<1};
	int arry[3] = { 0,  0,  0 };
	int arrcx[3] = { cx, cx, cx,};
	int arrcy[3] = { cy, cy, cy,};
	/*if (bVert)
	{
		rect.left = rect.Width()/3.65;

	}
	else
	{
		rect.top += rect.Height() / 2;
	}
*/
	//CMFCTabCtrl* pNewTabWnd = CreateNewTabGroup(pActiveWnd, CRect(arrx[m_pApp->m_nTrackCountGroup], arry[m_pApp->m_nTrackCountGroup], arrcx[m_pApp->m_nTrackCountGroup], arrcy[m_pApp->m_nTrackCountGroup]), bVert);
	CMFCTabCtrl* pNewTabWnd = CreateNewTabGroup(pActiveWnd, CRect(arrx[0], arry[0], arrcx[0], arrcy[0]), bVert);
	
	MoveWindowToTabGroup(pActiveWnd, pNewTabWnd);
	
}

void CApplication::OnParmOptionsTeachInspAndLoc()
{
}

void CApplication::OnUpdateParmOptionsTeachInspAndLoc(CCmdUI *pCmdUI)
{
	BOOL bTrackLiveImg = FALSE;
	BOOL bDocInsp = FALSE;
	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
		bTrackLiveImg &= m_pTracks[nTrack].m_bLiveImage;
		bDocInsp &= m_pTracks[nTrack].m_pDoc[0]->m_bInspecting;
	}
	BOOL bStepInspectionEnabled = FALSE;
	BOOL bStoredInspectionEnabled = FALSE;
	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			bStepInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspSavedImage;
			bStoredInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bStoredImages;
		}
	}

	pCmdUI->Enable(!m_bOnline &&
		!bStepInspectionEnabled &&
		!m_bTeaching &&
		!m_bTrackInspSeq &&
		!m_bInspecting &&
		!bTrackLiveImg &&
		!bDocInsp &&
		!bStoredInspectionEnabled &&
		!m_bLightCtrlDlgOpen && !m_bInspSequenceOffline &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nTeachInspLoc));
}

void CApplication::ShowMotionInitializing()
{
	CMFCRibbonBar* pRibbon = &((CMainFrame*) m_pMainWnd)->m_wndRibbonBar;
	CMFCRibbonBaseElement* pElement = pRibbon->FindByID(ID_INITIALIZING_BUTTON, TRUE, TRUE);
	pElement->SetText((m_MotorResource.isDllOpened()/* && *//*!m_pMotionControl || *//*m_pMotionControl->m_bIsStarting*/) ? "---   Initializing Motor. Please Wait   ---" : "");
	pRibbon->ForceRecalcLayout();
}

void CApplication::OnUpdateMotorInitializing(CCmdUI *pCmdUI)
{
	BOOL bEnable = m_MotorResource.isDllOpened(); /*&& (/*!m_pMotionControl || *//*m_pMotionControl->m_bIsStarting);*/
	pCmdUI->SetCheck(bEnable);
	pCmdUI->Enable(bEnable);
}


void CApplication::OnMotionControl()
{
	
	if(m_pMotionControl == NULL) {

		m_pMotionControl = new CMotionControlDialog(&motionParameters);

		if(!m_pMotionControl->Create(IDD_MOTOR_DLG, dynamic_cast<CMainFrame*>(AfxGetMainWnd()))) {
			delete m_pMotionControl;
			m_pMotionControl = NULL;

			bOpenedMotionDialog = FALSE;

			return;
		}
	}

	bOpenedMotionDialog = TRUE;

	m_pMotionControl->bChecked = &bOpenedMotionDialog;

	m_pMotionControl->ResetValues();

	m_pMotionControl->DisplayPos();

	m_pMotionControl->ShowWindow(TRUE);
}

void CApplication::OnUpdateMotionControl(CCmdUI *pCmdUI)
{
	BOOL bTrackLiveImg, bDocInsp;
	for (int nTrack = 0; nTrack<m_nTotalTracks; nTrack++) {
		bTrackLiveImg &= m_pTracks[nTrack].m_bLiveImage;
		bDocInsp &= m_pTracks[nTrack].m_pDoc[0]->m_bInspecting;


	}
	BOOL bStepInspectionEnabled = FALSE;
	BOOL bStoredInspectionEnabled = FALSE;
	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			bStepInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspSavedImage;
			bStoredInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bStoredImages;
		}
	}

	pCmdUI->Enable(!m_bOnline &&
		!bStepInspectionEnabled &&
		!m_bTeaching &&
		!m_bTrackInspSeq &&
		!m_bInspecting &&
		!bTrackLiveImg &&
		!bDocInsp &&
		!bStoredInspectionEnabled &&
		!m_bMotionCtrlDlgOpen && !m_bLightCtrlDlgOpen &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nMotionControl));
}

void CApplication::OnParmOptionsDeviceConfiguration()
{
	//InitializeDefectCode();
//	CreateDefectCodeParmList();
	CDeviceConfig Dlg;
	Dlg.DoModal();
}

void CApplication::OnUpdateParmOptionsDeviceConfiguration(CCmdUI *pCmdUI)
{
	BOOL bTrackLiveImg, bDocInsp;
	for (int nTrack = 0; nTrack<m_nTotalTracks; nTrack++) {
		bTrackLiveImg &= m_pTracks[nTrack].m_bLiveImage;
		bDocInsp &= m_pTracks[nTrack].m_pDoc[0]->m_bInspecting;


	}
	BOOL bStepInspectionEnabled = FALSE;
	BOOL bStoredInspectionEnabled = FALSE;
	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			bStepInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspSavedImage;
			bStoredInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bStoredImages;
		}
	}

	pCmdUI->Enable(!m_bOnline &&
		!bStepInspectionEnabled &&
		!m_bTeaching &&
		!m_bTrackInspSeq &&
		!m_bInspecting &&
		!bTrackLiveImg &&
		!bDocInsp &&
		!bStoredInspectionEnabled &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nDeviceCfg));
}


void CApplication::OnParmOptionsDefectPrioritySelection()
{
	CDefectPrioritySelDlg Dlg;
	Dlg.DoModal();
}

void CApplication::OnUpdateParmOptionsDefectPrioritySelection(CCmdUI *pCmdUI)
{
	BOOL bTrackLiveImg, bDocInsp;
	for (int nTrack = 0; nTrack<m_nTotalTracks; nTrack++) {
		bTrackLiveImg &= m_pTracks[nTrack].m_bLiveImage;
		bDocInsp &= m_pTracks[nTrack].m_pDoc[0]->m_bInspecting;


	}
	BOOL bStepInspectionEnabled = FALSE;
	BOOL bStoredInspectionEnabled = FALSE;
	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			bStepInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspSavedImage;
			bStoredInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bStoredImages;
		}
	}

	pCmdUI->Enable(!m_bOnline &&
		!bStepInspectionEnabled &&
		!m_bTeaching &&
		!m_bTrackInspSeq &&
		!m_bInspecting &&
		!bTrackLiveImg &&
		!bDocInsp &&
		!bStoredInspectionEnabled &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nDefectPriority));
}

void CApplication::ResizeDieSelectionDlg()
{
//	((CMainFrame*) m_pMainWnd)->m_dlgPane->m_wndDlg.InitiDieControls();
	((CMainFrame*)m_pMainWnd)->m_dlgPane->InitiDieControls();
//	((CMainFrame*) m_pMainWnd)->m_dlgPane->m_wndDlg.SetSize(200, 200);
	((CMainFrame*) m_pMainWnd)->m_dlgPane->Invalidate();
	//((CMainFrame*) m_pMainWnd)->m_dlgPane->m_wndDlg.SetWindowPos(NULL,0,0,cx,cy,SWP_NOACTIVATE|SWP_NOZORDER);
//	((CMainFrame*) m_pMainWnd)->m_dlgPane->m_wndDlg.Invalidate();
	((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->bRefreshPane = TRUE;
	((CMainFrame*)m_pMainWnd)->ShowStatsPane();
	//((CMainFrame*)m_pMainWnd)->m_dlgPaneStats->m_wndStatsDlg.Invalidate();
}


void CApplication::OnMaximize(BOOL bEnable)
{
	m_pMainWnd->SendMessage(WM_HIDE_PANES,bEnable);
}

//void CApplication::OnToolsStaticimageLoadMultiImgFromFile()
//{
//	
//}
//
//
//void CApplication::OnUpdateToolsStaticimageLoadMultiImgFromFile(CCmdUI *pCmdUI)
//{
//	BOOL bTrackLiveImg, bDocInsp;
//	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
//		bTrackLiveImg &= m_pTracks[nTrack].m_bLiveImage;
//		bDocInsp &= m_pTracks[nTrack].m_pDoc[0]->m_bInspecting;
//	}
//
//	pCmdUI->Enable(!m_bOnline &&
//				   !m_bTeaching &&
//				   !m_bTrackInspSeq &&
//				   !m_bInspecting &&
//				   !bTrackLiveImg &&
//				   !bDocInsp &&
//				   (m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nLoadImg));
//	pCmdUI->SetCheck(m_bLoadMultiImagesFromFile);
//}

void CApplication::StepDebugNavigate(int nStep)
{
	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++)
		m_pTracks[nTrack].StepDebugNavigate(nStep);
}

int CApplication::NavigateDieImages(int nKey)
{
	//// commented on customer request ////
	int nErr = -1;
	//if(m_bLoadMultiImagesFromFile) {
	//	nErr = ((CMainFrame*) m_pMainWnd)->m_dlgPane->m_wndDlg.NavigateDieImages(nKey);
	//}
	//else {
	//	nErr = m_pTracks[m_nNavigateStaticImgTrack].NavigateStaticImage(nKey);
	//}

	return nErr;
}

void CApplication::SetCurPosParamDisplay(int nTrack, CString* str)
{
	//CMainFrame* pFrame = ((CMainFrame*) m_pMainWnd);
	if(m_pMainWnd && ::IsWindow(m_pMainWnd->m_hWnd)) {
		char charDoc, charFOV;
		int nDoc, nFOV;
		charDoc = str->GetAt(0);
		charFOV = str->GetAt(1);
		nDoc = atoi(&charDoc);
		nFOV = atoi(&charFOV);

		if(((CMainFrame*) m_pMainWnd)->IsWindowVisible() && 
		(((CMainFrame*) m_pMainWnd)->m_pwndTeachProperties->IsDocked() || 
		((CMainFrame*) m_pMainWnd)->m_pwndTeachProperties->IsFloating())) {
			((CMainFrame*) m_pMainWnd)->m_pwndTeachProperties->InitPropList(m_pTracks[nTrack].m_pDoc[nDoc], nFOV);
		}
	}
}


//void CApplication::OnInspInspectionSequence()
//{
//	m_bInspSequenceOffline = !m_bInspSequenceOffline;
//	SynchronousCameraStream();
//	if(!m_bInspSequenceOffline)
//		 PutRegistryEntry(WRITE_REGPATH + "Preferences\\Persistence", "InspSequenceCounter", m_nSequenceCounter);
//
//	m_bLoadMultiImagesFromFile = FALSE;
//}
//
//void CApplication::OnUpdateInspInspectionSequence(CCmdUI *pCmdUI)
//{
//	BOOL bTrackLiveImg, bDocInsp;
//	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
//		bTrackLiveImg &= m_pTracks[nTrack].m_bLiveImage;
//		bDocInsp &= m_pTracks[nTrack].m_pDoc[0]->m_bInspecting;
//		
//
//	}
//	BOOL bStepInspectionEnabled = FALSE;
//	BOOL bStoredInspectionEnabled = FALSE;
//	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
//		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
//			bStepInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspSavedImage;
//			bStoredInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bStoredImages;
//		}
//	}
//
//	pCmdUI->Enable(!m_bOnline &&
//				   !bStepInspectionEnabled &&
//				   !m_bTeaching &&
//				   !m_bTrackInspSeq &&
//				   !m_bInspecting &&
//				   !bTrackLiveImg &&
//				   !bDocInsp &&
//				   !bStoredInspectionEnabled &&
//				   (m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nInspectionSeq));
//	pCmdUI->SetCheck(m_bInspSequenceOffline);
//}

//void CApplication::OnCameraSnapAll()
//{
//	SynchronousCameraSnap();
//}
//
//void CApplication::OnUpdateCameraSnapAll(CCmdUI *pCmdUI)
//{
//	BOOL bTrackLiveImg, bTrackContInsp, bDocInsp,bLiveSingle;
//	for(int nTrack=0; nTrack<m_nTotalTracks; nTrack++) {
//		bTrackLiveImg &= m_pTracks[nTrack].m_bLiveImage;
//		bTrackContInsp &= m_pTracks[nTrack].m_bContInsp;
//		bDocInsp &= m_pTracks[nTrack].m_pDoc[0]->m_bInspecting;
//		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
//			bLiveSingle &= m_pTracks[nTrack].m_pDoc[nDoc]->m_bLiveImageSingle;
//		}
//	}
//	
//	pCmdUI->Enable(m_pTracks[0].m_bCamAvail && m_pTracks[1].m_bCamAvail && m_pTracks[2].m_bCamAvail &&
//				   m_pTracks[0].m_bCamEnable && m_pTracks[1].m_bCamEnable && m_pTracks[2].m_bCamEnable &&
//				   !bTrackLiveImg &&
//				   !bTrackContInsp &&
//				   !bDocInsp &&
//				   !bLiveSingle &&
//				   !m_bOnline &&
//				   !m_bTeaching &&
//				   !m_bInspecting &&
//				   !m_bTrackInspSeq &&
//				   (m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nGrabAll));
//}

void CApplication::ShowMessageDlgBox(BOOL bTrue)
{
	CRect rect1, rect2;
	//SystemParametersInfo(SPI_GETWORKAREA, NULL, &rect1, 0);
	m_pDisplayMsgStatus->GetWindowRect(&rect2);
	//m_pDisplayMsgStatus->SetWindowPos(NULL, (int)(rect1.Width()/2 - rect2.Width()/2), rect2.Height(), rect2.Width(), rect2.Height(), SWP_NOSIZE);
	CMFCRibbonBar* pRibbon = &((CMainFrame*)m_pMainWnd)->m_wndRibbonBar;
	CMFCRibbonBaseElement* pElement = pRibbon->FindByID(ID_INSP_SAVE_SEQ_IMAGES, TRUE, TRUE);
	rect1 = pElement->GetRect();
	m_pDisplayMsgStatus->SetWindowPos(NULL, (int)(rect1.right+20), (int)(rect1.top+10), rect2.Width(), rect2.Height(), SWP_NOZORDER);

	m_pDisplayMsgStatus->ShowWindow(bTrue && m_MotorResource.isDllOpened());
	m_pDisplayMsgStatus->GetDlgItem(IDC_STATIC_MSG_INFO)->SetWindowTextA("Please Wait... All Motors Moving To Home");

}

void CApplication::OnInspSequenceLoop()
{
	m_bInspSeqLoop = !m_bInspSeqLoop;
}

void CApplication::OnUpdateInspSequenceLoop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nInspSeqLoop);
	pCmdUI->SetCheck(m_bInspSeqLoop);
}

void CApplication::OnPixelRuler()
{
	m_bPixelRuler = !m_bPixelRuler;
	if(m_bPixelRuler) {
		m_pPixelRulerDlg = new CPixelRuler();
		if(!m_pPixelRulerDlg->Create(IDD_DIALOG_PIXEL_RULER, m_pMainWnd)) {
			delete m_pPixelRulerDlg;
			m_pPixelRulerDlg = NULL;
			return ;
		}
		CRect rect1, rect2;
		m_pMainWnd->GetWindowRect(&rect1);
		m_pPixelRulerDlg->GetWindowRect(&rect2);

		m_pPixelRulerDlg->SetWindowPos(NULL, rect1.right - rect2.Width() - 130, rect1.top + 60, 0, 0, SWP_NOSIZE);
		m_pPixelRulerDlg->ShowWindow(TRUE);
		m_pMainWnd->SetFocus();
	}
	else {
		if(m_pPixelRulerDlg)
			delete m_pPixelRulerDlg;
		for(int i=0;i<m_nTotalTracks;i++){
			for(int j=0; j<m_nTotalViewDocs[i]; j++) {
				DrwDel(&m_pTracks[i].m_pDoc[j]->m_ImageViewInfo);
				DrwUpdate(&m_pTracks[i].m_pDoc[j]->m_ImageViewInfo);
				//m_StationHandler[i].m_pDoc[j]->UpdateAllViews(FALSE);	
			}
		}
	}
}

void CApplication::OnUpdatePixelRuler(CCmdUI *pCmdUI)
{
	BOOL bTrackLiveImg, bDocInsp;
	for (int nTrack = 0; nTrack<m_nTotalTracks; nTrack++) {
		bTrackLiveImg &= m_pTracks[nTrack].m_bLiveImage;
		bDocInsp &= m_pTracks[nTrack].m_pDoc[0]->m_bInspecting;
	}
	BOOL bStepInspectionEnabled = FALSE;
	BOOL bStoredInspectionEnabled = FALSE;
	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			bStepInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspSavedImage;
			//bStoredInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bStoredImages;
		}
	}

	pCmdUI->Enable(!m_bOnline &&
		!bStepInspectionEnabled &&
		!m_bTeaching &&
		!m_bTrackInspSeq &&
		!m_bInspecting &&
		!bTrackLiveImg &&
		!bDocInsp &&
		!bStoredInspectionEnabled &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nPixelRuler));
	pCmdUI->SetCheck(m_bPixelRuler);
}

int CApplication::OnReceiveResetCmd()
{
	if(m_bReceivedResetCmd){
		if(m_bEmergencyStop)
		{
			return -1;
		}
		else if(m_bCurtainSensor)
		{
			return -2;
		}
		//else{
		//	//m_pDisplayMsgStatus->GetDlgItem(IDC_STATIC_MSG_INFO)->SetWindowTextA("Please Wait... All Axis Moving To Home");
		//	ShowMessageDlgBox(TRUE);
		//	int nError;
		//	for(int i=0;i<NO_OF_AXIS;i++)
		//		nError = m_MotorResource.InitializeAxis(m_nAxes[i],0);
		//	Sleep(100);
		//	nError = m_pMotionControl->Homing(-1);
		//	if(nError)
		//		OutputDebugLogTo(9,TRUE,"CApplication::OnReceiveResetCmd Homing Error");//// for testing
		//	for(int i=0;i<NO_OF_AXIS;i++){
		//		nError = m_MotorResource.WaitForMotionDone(m_nAxes[i], INFINITE);
		//		if(nError) {
		//			OutputDebugLogTo(9, TRUE, "Homing [Axis%d] FAILED (Wait Error=%d) **********", i+1, nError);//// for testing
		//		}
		//	}
		//	ShowMessageDlgBox(FALSE);
		//	m_bEmergencyStop = FALSE;
		//	m_bCurtainSensor = FALSE;
		//	m_bReceivedStopCmd = FALSE;
		//	m_bStream = FALSE;
		//	SetEvent(m_EventMotionSensor);l
		//	SetEvent(m_eventOnline);
		//	ResetEvent(m_EventMotionSensorStart);


		//	m_bReceivedResetCmd = FALSE;
		//	return 0;
		//}
	}
	return 0;
}

int CApplication::UpdateInfoMsgDlg(CString strCamDocName, CString strInfoMsg)
{
	if (m_pInfoMsgDlg != NULL) {
		if (m_pInfoMsgDlg->IsWindowVisible()) {
			m_pInfoMsgDlg->UpdateDlgItems(strCamDocName, strInfoMsg);
		}
	}

	return 0;
}

void CApplication::InitializeDefectCode()
{
	int nView = -1;
	int nPviDefectCount = 0;
	
	for(int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		//Get total FOV for current station
		int nCurrentFOV = m_nNoOfFov;
		if(nTrack == STATION_TOP) {
			nCurrentFOV = motionParameters.GetTotalFOVPerDieTop();
		}
		else if(nTrack == STATION_THR) {
			nCurrentFOV = motionParameters.GetTotalFOVPerDieTHR();
		}
		if(nCurrentFOV > MAX_FOV) {
			nCurrentFOV = MAX_FOV;
		}

		for(int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			for(int nFov = 0; nFov < nCurrentFOV; nFov++) {
				nView = !nTrack ? nDoc : m_nTOP + nDoc;
				int nAreaCount = m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.m_TrainingData.nNoOfPviArea;
				m_nPviNumberOfArea[nFov][nView] = nAreaCount;
				for(int nArea = 0; nArea < nAreaCount; nArea++) {
					int nAreaDefectCount = m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.m_pPviInspectionSetup[nArea].nDefectCount;
					m_nPviNoOfDefects[nFov][nView][nArea] = nAreaDefectCount;
					for(int nDefect = 0; nDefect < nAreaDefectCount; nDefect++) {
						nPviDefectCount++;
					}
				}
			}
			
		}
	}
	m_nTotalDefectsCount = 0;
	m_nNo_of_Defects = nPviDefectCount + 20;

	int nGroupId = 1;
	m_pImageSelectionDlg->addGroup("Device Location", nGroupId++);
	m_pImageSelectionDlg->addGroup("Encap", nGroupId++);
	m_pImageSelectionDlg->addGroup("Contact", nGroupId++);
	m_pImageSelectionDlg->addGroup("Flex", nGroupId++);
	m_pImageSelectionDlg->addGroup("Chicklet", nGroupId++);

	m_pImageSelectionDlg->addGroup("Missing Die", -MISSING_DIE);

	int nDefectGroupId = 1;
	m_pImageSelectionDlg->addDefect("Device Location", "Device Location", -DEVICE_LOCATION, nDefectGroupId++, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Encap Insufficient", "Encap Insufficient", -ENCAP_INSUFFICIENT, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Encap Excess", "Encap Excess", -ENCAP_EXCESS, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Encap Crack", "Encap Crack", -ENCAP_CRACK, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Encap Blow Hole", "Encap Blow Hole", -ENCAP_BLOWHOLE, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Encap Contamination", "Encap Contamination", -ENCAP_CONTAMINATION, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Encap Void", "Encap Void", -ENCAP_VOID, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Encap Wire And IC", "Wire And IC", -ENCAP_WIREANDIC, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Encap Bulge", "Bulge", -ENCAP_BULGE, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Encap Glue", "Encap Glue", -ENCAP_GLUE, nDefectGroupId++, RGB(19, 59, 69), false);
	// Contact Error
	m_pImageSelectionDlg->addDefect("Contact Damage", "Contact Damage", -CONTACT_DAMAGE, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Contact Contamination", "Contact Contamination", -CONTACT_CONTAMINATION, nDefectGroupId++, RGB(19, 59, 69), false);
	// Flex Error
	m_pImageSelectionDlg->addDefect("Flex Overcut", "Flex Overcut", -FLEX_OVERCUT, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Flex Damage", "Flex Damage", -FLEX_DAMAGE, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Flex Contamination", "Flex Contamination", -FLEX_CONTAMINATION, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Flex Scratches", "Flex Scratches", -FLEX_SCRATCHES, nDefectGroupId++, RGB(19, 59, 69), false);

	m_pImageSelectionDlg->addDefect("Hole Roundness", "Hole Roundness", -CHICKLET_ROUNDNESS, nDefectGroupId, RGB(19, 59, 69), false);
	m_pImageSelectionDlg->addDefect("Hole Burr", "Hole Burr", -CHICKLET_BURR, nDefectGroupId++, RGB(19, 59, 69), false);

	m_pImageSelectionDlg->addDefect("Missing Die", "Missing Die", MISSING_DIE, -MISSING_DIE, RGB(240, 59, 69), true);
	
	m_pImageSelectionDlg->nPVIGroupId = nGroupId;
	nGroupId = nDefectGroupId;
	m_pImageSelectionDlg->addGroup("PVI TOP", nDefectGroupId++);
	m_pImageSelectionDlg->addGroup("PVI THR", nDefectGroupId++);


	int nCurrentFoV = motionParameters.GetTotalFOVPerDieTHR();
	nView = -1;
	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		m_pTracks[nTrack].m_mapPviDefectsStatsGeneral.clear();

		int nCurrentFoV = MAX_FOV;
		if(nTrack == 0)
			nCurrentFoV = motionParameters.GetTotalFOVPerDieTop();
		else
			nCurrentFoV = motionParameters.GetTotalFOVPerDieTHR();

		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			
			for(int nFov = 0; nFov < nCurrentFoV; nFov++) {
				nView = !nTrack ? nDoc : m_nTOP + nDoc;
				int nAreaCount = m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.m_TrainingData.nNoOfPviArea;
				m_nPviNumberOfArea[nFov][nView] = nAreaCount;
				for(int nArea = 0; nArea < nAreaCount; nArea++) {
					int nAreaDefectCount = m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.m_pPviInspectionSetup[nArea].nDefectCount;
					m_nPviNoOfDefects[nFov][nView][nArea] = nAreaDefectCount;
					for(int nDefect = 0; nDefect < nAreaDefectCount; nDefect++) {
						CString strDefectDisplay, strDefectCodeTemp;

						int nErrorCodeId = m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[nDefect].m_nErrorCodeID;
						strDefectDisplay = m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[nDefect].strDefectName;
						strDefectCodeTemp.Format("%s Fov %d", strDefectDisplay,  nFov);
						m_pImageSelectionDlg->addDefect(strDefectDisplay.GetString(), strDefectDisplay.GetString(), -nErrorCodeId, nGroupId + nTrack, RGB(19, 59, 69), false);
					
						//Update the Unique Defect Name
						m_pTracks[nTrack].m_mapPviDefectsStatsGeneral.insert(std::pair<int, CString>(-nErrorCodeId, strDefectDisplay));
						m_LotSummary.m_mapIndividualPviDefectCounter[nTrack].insert(std::pair<int, long>(-nErrorCodeId, 0));
					}
				}
			}
		}
	}

	CString file;

	file.Format("%s\\Defect code.dat", m_strMainCfgDir);

	m_pImageSelectionDlg->load(file.GetString());

	std::map<CString, int> mapInspPriority;
	m_nDefectErrorCodesPriority.RemoveAll();
	m_nDefectErrorCodesPriorityTemp.RemoveAll();
	m_strDefectErrorCodesPriorityTemp.RemoveAll();
	
	
	for(int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		//Get total FOV for current station
		int nCurrentFOV = m_nNoOfFov;
		if(nTrack == STATION_TOP) {
			nCurrentFOV = motionParameters.GetTotalFOVPerDieTop();
		}
		else if(nTrack == STATION_THR) {
			nCurrentFOV = motionParameters.GetTotalFOVPerDieTHR();
		}
		if(nCurrentFOV > MAX_FOV) {
			nCurrentFOV = MAX_FOV;
		}
		for(int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			for(int nFov = 0; nFov < nCurrentFOV; nFov++) {
				int nAreaCount = m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.m_TrainingData.nNoOfPviArea;
				for(int nArea = 0; nArea < nAreaCount; nArea++) {
					int nAreaDefectCount = m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.m_pPviInspectionSetup[nArea].nDefectCount;
					for(int nDefect = 0; nDefect < nAreaDefectCount; nDefect++) {
						CString strDefectDisplay;
						CString strCam;
						strCam = !nTrack ? "Top" : "THR"/*!nTrack ? "Top" : nTrack == 2 ? "Side" : "Bottom"*/;
						strDefectDisplay.Format("%s_%s", strCam, m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[nDefect].strDefectName);
						if (mapInspPriority.find(strDefectDisplay) == mapInspPriority.end()) {
							m_strDefectErrorCodesPriorityTemp.Add(strDefectDisplay);
							m_nDefectErrorCodesPriorityTemp.Add(m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[nFov].m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[nDefect].m_nInspPriority);
							mapInspPriority.insert(std::pair<CString, int>(strDefectDisplay, 1));
						}
						else {
							mapInspPriority[strDefectDisplay] += 1;
						}
					}
				}
			}

			//Encap Defect Priority
			CString strDefectDisplay;
			strDefectDisplay = "Encap Glue";
			if (mapInspPriority.find(strDefectDisplay) == mapInspPriority.end()) {
				m_strDefectErrorCodesPriorityTemp.Add(strDefectDisplay);
				m_nDefectErrorCodesPriorityTemp.Add(m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[0].m_Inspection.m_EncapParm.nGluePriority);
				mapInspPriority.insert(std::pair<CString, int>(strDefectDisplay, 1));
			}
			else {
				mapInspPriority[strDefectDisplay] += 1;
			}

			strDefectDisplay = "Encap Contamination";
			if (mapInspPriority.find(strDefectDisplay) == mapInspPriority.end()) {
				m_strDefectErrorCodesPriorityTemp.Add(strDefectDisplay);
				m_nDefectErrorCodesPriorityTemp.Add(m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[0].m_Inspection.m_EncapParm.nContaminationPriority);
				mapInspPriority.insert(std::pair<CString, int>(strDefectDisplay, 1));
			}
			else {
				mapInspPriority[strDefectDisplay] += 1;
			}
			
			strDefectDisplay = "Encap Blowhole";
			if (mapInspPriority.find(strDefectDisplay) == mapInspPriority.end()) {
				m_strDefectErrorCodesPriorityTemp.Add(strDefectDisplay);
				m_nDefectErrorCodesPriorityTemp.Add(m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[0].m_Inspection.m_EncapParm.nBlowHolePriority);
				mapInspPriority.insert(std::pair<CString, int>(strDefectDisplay, 1));
			}
			else {
				mapInspPriority[strDefectDisplay] += 1;
			}

			strDefectDisplay = "Encap Void";
			if (mapInspPriority.find(strDefectDisplay) == mapInspPriority.end()) {
				m_strDefectErrorCodesPriorityTemp.Add(strDefectDisplay);
				m_nDefectErrorCodesPriorityTemp.Add(m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[0].m_Inspection.m_EncapParm.nVoidPriority);
				mapInspPriority.insert(std::pair<CString, int>(strDefectDisplay, 1));
			}
			else {
				mapInspPriority[strDefectDisplay] += 1;
			}

			strDefectDisplay = "Encap Wire And IC";
			if (mapInspPriority.find(strDefectDisplay) == mapInspPriority.end()) {
				m_strDefectErrorCodesPriorityTemp.Add(strDefectDisplay);
				m_nDefectErrorCodesPriorityTemp.Add(m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[0].m_Inspection.m_EncapParm.nWireAndICPriority);
				mapInspPriority.insert(std::pair<CString, int>(strDefectDisplay, 1));
			}
			else {
				mapInspPriority[strDefectDisplay] += 1;
			}

			strDefectDisplay = "Encap Bulge";
			if (mapInspPriority.find(strDefectDisplay) == mapInspPriority.end()) {
				m_strDefectErrorCodesPriorityTemp.Add(strDefectDisplay);
				m_nDefectErrorCodesPriorityTemp.Add(m_pTracks[nTrack].m_pDoc[nDoc]->m_InspectionHandler[0].m_Inspection.m_EncapParm.nBulgePriority);
				mapInspPriority.insert(std::pair<CString, int>(strDefectDisplay, 1));
			}
			else {
				mapInspPriority[strDefectDisplay] += 1;
			}
		}
	}
	
	m_strDefectErrorCodesPriority.RemoveAll();
	CDefectPrioritySelDlg dlgPriorityList;
	dlgPriorityList.InitializePriorityList();
}

int CApplication::CheckForHalconnLicense()
{
	try {
		HTuple hSystem;
		GetSystem("temporary_mem_cache", &hSystem);
		if (hSystem == "true") {
			//SetSystem("temporary_mem_cache", "false");
		}
		else {
			SetSystem("temporary_mem_cache", "true");
		}

		GetSystem("reentrant", &hSystem);
		if (hSystem == "false") {
			SetSystem("reentrant", "true");
		}
		else {
			//SetSystem("reentrant", "false");
		}

		GetSystem("parallelize_operators", &hSystem);
		if (hSystem == "true") {
			SetSystem("parallelize_operators", "false");
		}
		else {
			//SetSystem("parallelize_operators", "true");
		}

		GetSystem("thread_pool", &hSystem);
		if (hSystem == "false") {
			SetSystem("thread_pool", "true ");
		}
		else {
			//SetSystem("thread_pool", "false ");
		}

		GetSystem("image_cache_capacity", &hSystem);
		if (hSystem != 4194304) {
			SetSystem("image_cache_capacity", 4194304);
		}
		else {
			//SetSystem("image_cache_capacity", 0);
		}

		GetSystem("global_mem_cache", &hSystem);
		if (hSystem != "exclusive") {
			SetSystem("global_mem_cache", "exclusive");
		}
		else {
			//SetSystem("global_mem_cache", "idle");
		}

		GetSystem("alloctmp_max_blocksize", &hSystem);
		if (hSystem != -1) {
			SetSystem("alloctmp_max_blocksize", -1);
		}
		else {
			//SetSystem("alloctmp_max_blocksize", 2);
		}
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[Inspection::Initialize] %s", ex.ErrorMessage().Text());
		LogMessage(9, str);
		AfxMessageBox("Unregistered Sherlock_Spyglass Inspection module");
		return FALSE;
	}
	catch (...) {}

	return TRUE;
}

void CApplication::OnSecsGemHSMSSetting()
{
	SecsGemSetupDlg SecsgemSetupDialog;

	SecsgemSetupDialog.DoModal();
}

void CApplication::OnUpdateSecsGemHSMSSetting(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline && isSecsGemEnabled() && (getGEMController().getController() != nullptr) &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nIOTest));
}

void CApplication::OnSecsGemSetting()
{
	/*SecsGemDiagnosticsDlg dialog;
	dialog.DoModal();*/

	if (m_pSecsGemDiagnosticsDlg != NULL) {
		m_pSecsGemDiagnosticsDlg->ShowWindow(TRUE);
	}
	else {
		m_pSecsGemDiagnosticsDlg = new SecsGemDiagnosticsDlg(m_pMainWnd);

		if (!m_pSecsGemDiagnosticsDlg->Create(IDD_SECSGEM_COMMUNICATION_DIALOG, m_pMainWnd)) {
			delete m_pSecsGemDiagnosticsDlg;
			m_pSecsGemDiagnosticsDlg = NULL;
			return;
		}

		CRect rect1, rect2;
		m_pMainWnd->GetWindowRect(&rect1);
		m_pSecsGemDiagnosticsDlg->GetWindowRect(&rect2);

		m_pSecsGemDiagnosticsDlg->SetWindowPos(NULL, rect1.right - rect2.Width() - 130, rect1.top + 60, 0, 0, SWP_NOSIZE);
		m_pSecsGemDiagnosticsDlg->ShowWindow(TRUE);
		m_pMainWnd->SetFocus();
	}

	PutRegistryEntry(WRITE_REGPATH + "SECS/GEM", "SECS/GEM Control State", nGEMControlState);
}

void CApplication::OnUpdateSecsGemSetting(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline && isSecsGemEnabled() && (getGEMController().getController() != nullptr) &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nIOTest));
}

void CApplication::OnSecsGemTerminalService()
{
	if (m_pTerminalServiceDlg != NULL) {
		m_pTerminalServiceDlg->ShowWindow(TRUE);
	}
	else {
		m_pTerminalServiceDlg = new TerminalServiceDlg(m_pMainWnd);
		if (!m_pTerminalServiceDlg->Create(IDD_EQUIP_TERMINAL_SERVICE_DIALOG, m_pMainWnd)) {
			delete m_pTerminalServiceDlg;
			m_pTerminalServiceDlg = NULL;
			return;
		}

		CRect rect1, rect2;
		m_pMainWnd->GetWindowRect(&rect1);
		m_pTerminalServiceDlg->GetWindowRect(&rect2);

		m_pTerminalServiceDlg->SetWindowPos(NULL, rect1.right - rect2.Width() - 130, rect1.top + 60, 0, 0, SWP_NOSIZE);
		m_pTerminalServiceDlg->ShowWindow(TRUE);
		m_pMainWnd->SetFocus();
	}
}

void CApplication::OnUpdateSecsGemTerminalService(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline && isSecsGemEnabled() && (getGEMController().getController() != nullptr) &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nIOTest));

	pCmdUI->SetCheck(m_bTerminalService);
}

void CApplication::OnTriangularSensor()
{
	/*if (m_pTriSensorDlg)
	{
		delete m_pTriSensorDlg;
		m_pTriSensorDlg = NULL;
	}*/

	if(m_pTriSensorDlg != NULL){
		m_pTriSensorDlg->ShowWindow(TRUE);
	}
	else {
		m_pTriSensorDlg = new CTriangularSensroDlg(m_pMainWnd);
		if(!m_pTriSensorDlg->Create(IDD_TRIANGULAR_SENSRO_DLG, m_pMainWnd)) {
			delete m_pTriSensorDlg;
			m_pTriSensorDlg = NULL;
			return;
		}
		CRect rect1, rect2;
		m_pMainWnd->GetWindowRect(&rect1);
		m_pTriSensorDlg->GetWindowRect(&rect2);

		m_pTriSensorDlg->SetWindowPos(NULL, rect1.right - rect2.Width(), rect1.top + 60, 0, 0, SWP_NOSIZE);
		m_pTriSensorDlg->ShowWindow(TRUE);
		m_pTriSensorDlg->m_bConnected = m_bTriangularSensorConnected;
		m_pTriSensorDlg->Initailize();
	}
}

void CApplication::OnUpdateTriangularSensor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline &&
		!m_bTeaching &&
		!m_bTrackInspSeq &&
		!m_bInspecting &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nTriangularSensor));
}

void CApplication::OnBarcodeReaderButton()
{
	// TODO: Add your command handler code here
	/*CBarcodeReaderDialog barcodeReaderDlg;
	barcodeReaderDlg.DoModal();*/


	CQRBarcodeReaderDialog QRBarcodeReaderDialog;

	QRBarcodeReaderDialog.DoModal();
}


void CApplication::OnUpdateBarcodeReaderButton(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bTeaching &&
		!m_bInspecting &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nBarcodeReader) );

}

int CApplication::getMotionTimeOut() const
{
	return m_nMotionTimeout;
}

BOOL CApplication::IsMotionDialogOpened() const
{
	return bOpenedMotionDialog;
}

void CApplication::OnButtonZSensorSequence()
{
	m_bScanningEnabled = !m_bScanningEnabled;

	if(m_bScanningEnabled) {
		pGlobalBackgroundThread->Run();
		/*CreateScanThread();
		SetEvent(m_eventStartScan);*/
	}
}

void CApplication::OnUpdateButtonZSensorSequence(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bTeaching &&
		!m_bInspecting &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nSequenceRun));

	pCmdUI->SetCheck(m_bScanningEnabled);
}

void CApplication::CreateScanThread()
{
	if(!m_bStartScanThread) {
		
		if(AfxBeginThread(ScanThread, this, THREAD_PRIORITY_HIGHEST) == NULL) {
			OutputDebugLogTo(0, FALSE, "----- ERROR: Can't startup Scanning Thread");
			return;
		}
		m_bStartScanThread = TRUE;
	
	}
}

void CApplication::OnClampingSequenceClamp()
{
	m_nCommand = CLAMP_SEQUENCE;
	m_pTracks[0].m_pActiveDoc = m_pTracks[0].m_pDoc[0];
	m_nActiveTrackCommandIdx = 0;
	SetEvent(m_EventRun);
}


void CApplication::OnUpdateClampingSequenceClamp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline &&
		!m_bTeaching &&
		!m_bInspecting &&
		!m_bBarcodeSequenceEnabled &&
		!m_bUnClampingSequenceEnabled &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nClamp));

	pCmdUI->SetCheck(m_bClampingSequenceEnabled);
}

void CApplication::OnClampingSequenceUnClamp()
{
	m_nCommand = UNCLAMP_SEQUENCE;
	m_pTracks[0].m_pActiveDoc = m_pTracks[0].m_pDoc[0];
	m_nActiveTrackCommandIdx = 0;
	SetEvent(m_EventRun);
}

void CApplication::OnUpdateClampingSequenceUnClamp(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline &&
		!m_bTeaching &&
		!m_bInspecting &&
		!m_bBarcodeSequenceEnabled &&
		!m_bClampingSequenceEnabled &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nUnClamp));

	pCmdUI->SetCheck(m_bUnClampingSequenceEnabled);
}

void CApplication::OnMotorTrigger()
{
	bEnableMotorTriggerTestDialog = !bEnableMotorTriggerTestDialog;

	if(bEnableMotorTriggerTestDialog) {

		if(pMotorTriggerTestDialog == NULL) {

			pMotorTriggerTestDialog = new CMotorTriggerTestDialog();
			if(!pMotorTriggerTestDialog->Create(IDD_MOTOR_TRIGGER_TEST, AfxGetMainWnd())) {
				delete pMotorTriggerTestDialog;
				pMotorTriggerTestDialog = NULL;
				return;
			}
		}

		pMotorTriggerTestDialog->ShowWindow(TRUE);
	}
	else {
		if(pMotorTriggerTestDialog != NULL) {
			delete pMotorTriggerTestDialog;
			pMotorTriggerTestDialog = NULL;
		}
	}

	SetFocus(m_pMainWnd->m_hWnd);
	// TODO: Add your command handler code here
}


void CApplication::OnUpdateMotorTrigger(CCmdUI *pCmdUI)
{

//	BOOL bAccess = (m_nCurrentAccessLevel >= m_SecurityDialog.m_data.m_nMotorTrigger) ? TRUE : FALSE;
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(bEnableMotorTriggerTestDialog&& (m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nMotorTrigger)/* && !bSequenceRun*/);
}


void CApplication::BarcodeSequence()
{
	int nError;
	CString strDeviceID;
	BOOL bAllowScan = TRUE;

	nError = QRBarcodeReader.GetDeviceScanningStatus(strDeviceID, bAllowScan);

	if(!nError) {
		QRBarcodeReader.GetDeviceInfo(strDeviceID);
		strInspectDeviceBarcodeID = strDeviceID;
		OutputDebugLogTo(7, TRUE, "Barcode ID: %s", strDeviceID);

	}
	else {
		theApp.runQRBarcodeFailSequence();
		OutputDebugLogTo(6, TRUE, "Final ID: %s", theApp.strInspectDeviceBarcodeID);
		//	nError = theApp.ioControl.SetscannerCylinder01Retension();
	}

	//Lot display
	m_strLotInfo = theApp.strInspectDeviceBarcodeID.Mid(0, 4);
	theApp.m_pDispOnlinePanel->GetDlgItem(IDC_ONLINE_LOT_ID_STATIC)->SetWindowTextA(theApp.m_strLotInfo);
}

void CApplication::OnUpdateBarcodeSequence(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CApplication::OnReadBarcodeSequence()
{
	// TODO: Add your command handler code here
}

void CApplication::runQRBarcodeFailSequence()
{
	bool repeat = true;
	while (repeat) {
		CFailedQRBarcodeDialog dialog;

		if (dialog.DoModal() == IDOK) {

			//if (dialog.getActionSelection() == AFTER_UNLOAD_STRIP_SEQUENCE) {
			//	if (AfxMessageBox("Do you want to continue sequence with next strip?", MB_YESNO | MB_ICONINFORMATION) == IDYES) {
			//		repeat = false;
			//		OutputDebugLogTo(4, TRUE, "Failed QR-Barcode, Selected Next Strip");
			//		/*return NEXT_STRIP_SEQUENCE;*/
			//	}
			//	else {
			//		repeat = false;
			//		OutputDebugLogTo(4, TRUE, "Failed QR-Barcode, Selected Machine Stop");
			//		/*return STOP_MACHINE_SEQUENCE;*/
			//	}
			//}
			//else if (dialog.getActionSelection() == ABORT_LOGIN_STRIP_SEQUENCE || dialog.getActionSelection() == ABORT_INPUT_BARCODE_SEQUENCE) {
			//	repeat = true;
			//}
			//else {
			//	repeat = false;
			//	OutputDebugLogTo(4, TRUE, "Failed QR-Barcode, Selected Continue Sequence");
			//	/*return CONTINUE_SEQUENCE;*/
			//}
			repeat = false;
		}
	};
}




void CApplication::OnCheckMultipleRun()
{
	// TODO: Add your command handler code here
	m_bScanningEnabled = !m_bScanningEnabled;
}


void CApplication::OnUpdateCheckMultipleRun(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(m_bScanningEnabled);
}


void CApplication::OnDryRun()
{
	m_bDryRun = !m_bDryRun;
}


void CApplication::OnUpdateDryRun(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(m_bDryRun);
}

void CApplication::ModuleReplacementSequence()
{
	int nError;

	/*if(!motionParameters.IsMotorHomed(X_SERVO_MOTOR)) {
		nError = motionParameters.HomeMove(X_SERVO_MOTOR, false);
		if(nError) 
			OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for X motor failed");
		
	}
	if(!motionParameters.IsMotorHomed(Y_SERVO_MOTOR)) {
		nError = motionParameters.HomeMove(Y_SERVO_MOTOR, false);
		if(nError) 
			OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Y motor failed");
		
	}

	if(!motionParameters.IsMotorHomed(ST1_Z_SERVO_MOTOR)) {
		nError = motionParameters.HomeMove(ST1_Z_SERVO_MOTOR, false);
		if(nError) 
			OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Z motor failed");
		
	}

	nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, INFINITE);
	if(nError) 
		OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for X motor failed");

	nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, INFINITE);
	if(nError)
		OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Y motor failed");

	nError = motionParameters.WaitForMotionDone(ST1_Z_SERVO_MOTOR, INFINITE);
	if(nError)
		OutputDebugLogTo(0, TRUE, "----- ERROR : Home move for Z motor failed");
	
	nError = motionParameters.AbsoluteMove(X_SERVO_MOTOR, motionParameters.getMotorModuleUnloadPosition(X_SERVO_MOTOR), motionParameters.getMovingSpeed(X_SERVO_MOTOR), false);

	if(nError) {
		motionParameters.StopAxis(X_SERVO_MOTOR);
		OutputDebugLogTo(0, TRUE, "----- ERROR : Module unload position move for X motor failed");
	}

	nError = motionParameters.AbsoluteMove(Y_SERVO_MOTOR, motionParameters.getMotorModuleUnloadPosition(Y_SERVO_MOTOR), motionParameters.getMovingSpeed(Y_SERVO_MOTOR), false);

	if(nError) {
		motionParameters.StopAxis(Y_SERVO_MOTOR);
		OutputDebugLogTo(0, TRUE, "----- ERROR : Module unload position move for Y motor failed");

	}

	nError = motionParameters.AbsoluteMove(ST1_Z_SERVO_MOTOR, motionParameters.getMotorModuleUnloadPosition(ST1_Z_SERVO_MOTOR), motionParameters.getMovingSpeed(ST1_Z_SERVO_MOTOR), false);

	if(nError) {
		motionParameters.StopAxis(ST1_Z_SERVO_MOTOR);
		OutputDebugLogTo(0, TRUE, "----- ERROR : Module unload position move for Z motor failed");


	nError = motionParameters.WaitForMotionDone(X_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {
		motionParameters.StopAxis(X_SERVO_MOTOR);
		return;
	}

	nError = motionParameters.WaitForMotionDone(Y_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {
		motionParameters.StopAxis(Y_SERVO_MOTOR);
		return;
	}

	nError = motionParameters.WaitForMotionDone(ST1_Z_SERVO_MOTOR, theApp.getMotionTimeOut());
	if(nError) {
		motionParameters.StopAxis(ST1_Z_SERVO_MOTOR);
		return;
	}*/

	AfxMessageBox("Nest Replacement Sequence Completed.");
}


void CApplication::OnButtonBarcodeSequence()
{
	m_nCommand = BARCODE_SEQUENCE;
	m_pTracks[0].m_pActiveDoc = m_pTracks[0].m_pDoc[0];
	m_nActiveTrackCommandIdx = 0;
	SetEvent(m_EventRun);
}


void CApplication::OnUpdateButtonBarcodeSequence(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline &&
		!m_bTeaching &&
		!m_bInspecting &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nBarCode));

	pCmdUI->SetCheck(m_bBarcodeSequenceEnabled);
}

void CApplication::OnButtonSequenceOptions()
{
	if (m_pSequenceOptionDlg != NULL) {
		m_pSequenceOptionDlg->ShowWindow(TRUE);
	}
	else {
		m_pSequenceOptionDlg = new SequenceOptions();
		if (!m_pSequenceOptionDlg->Create(IDD_DIALOG_SEQUENCE_OPTIONS, AfxGetMainWnd())) {
			delete m_pSequenceOptionDlg;
			m_pSequenceOptionDlg = NULL;
			return;
		}
		m_pSequenceOptionDlg->ShowWindow(TRUE);
	}
}

void CApplication::OnUpdateButtonSequenceOptions(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline && (m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nScanningOptions));
}

void CApplication::UpdateDefectCodes(CString defName, CString defCode)
{
	std::vector < ItemInfo > _defect;
	std::vector < GroupInfo > _group;
	CString name;

	_defect = m_pImageSelectionDlg->getDefect();
	//	_group = pSubstrateInspectionResultsWnd->getGroup();

	for (size_t defectId = 0; defectId < _defect.size(); ++defectId) {

		name = _defect[defectId].description.c_str();

		if (name == defName)
		{
			_defect[defectId].code = defCode;
			theApp.m_pImageSelectionDlg->setDefect(_defect[defectId]);
			break;
		}
	}

	name.Format("%s\\Defect code.dat", m_strMainCfgDir);
	m_pImageSelectionDlg->save(name.GetString());
}

void CApplication::OnButtonDefectSelection()
{
	CDefectCodesDlg dlg;
	dlg.DoModal();
	// TODO: Add your command handler code here
}


void CApplication::OnUpdateButtonDefectSelection(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bSuperUserMode ||
		!m_bOnline &&
		!m_bTeaching &&
		!m_bInspecting &&
		(m_UserAccInfo.m_nCurrentAccessLevel >=m_AdvSecurityDlg.m_data.m_nDefectParm));



}
void CApplication::OnButtonVerificationMode()
{
	if (m_pVerificationMode != NULL)
		m_bVerificationMode = !m_bVerificationMode;
}


void CApplication::OnUpdateButtonVerificationMode(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nVerificationMode);

	pCmdUI->SetCheck(m_bVerificationMode);
	// TODO: Add your command update UI handler code here
}


void CApplication::OnButtonShowVerificationDlg()
{
	if (m_pVerificationMode != NULL) {
		m_pVerificationMode->ShowWindow(TRUE);
		theApp.m_bVerificationDialogOpened = TRUE;
	}
}


void CApplication::OnUpdateButtonShowVerificationDlg(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nVerificationWindow);
	pCmdUI->SetCheck(m_bVerificationDialogOpened);
	
}


//int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
//{
//	TCHAR szDir[MAX_PATH];
//	switch (uMsg) {
//	case BFFM_INITIALIZED:
//		if (lpData) {
//			strcpy_s(szDir, (char*)lpData);
//			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
//		}
//		break;
//	case BFFM_SELCHANGED: {
//		if (SHGetPathFromIDList((LPITEMIDLIST)lParam, szDir)) {
//			SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir);
//		}
//		break;
//	}
//	default:
//		break;
//	}
//
//	return 0;
//}

CString strTmpPath;

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	switch (uMsg) {
	case BFFM_INITIALIZED:
		if (lpData) {
			strcpy(szDir, strTmpPath.GetBuffer(strTmpPath.GetLength()));
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);
		}
		break;
	case BFFM_SELCHANGED: {
		if (SHGetPathFromIDList((LPITEMIDLIST)lParam, szDir)) {
			SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir);
		}
		break;
	}
	default:
		break;
	}

	return 0;
}

BOOL GetFolder(CString* strSelectedFolder, const char* strStartFolder)
{
	//char pszDisplayName[MAX_PATH];
	//LPITEMIDLIST lpID;
	//BROWSEINFOA bi;
	//CWnd hwnd;
	//CString msg;

	//bi.hwndOwner = hwnd.GetSafeHwnd();
	//bi.pidlRoot = NULL;
	//bi.pszDisplayName = pszDisplayName;
	//msg.LoadString(ID_SELECT_FOLDER);

	//bi.lpszTitle = _T(msg);
	////bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	//bi.ulFlags = BIF_USENEWUI;
	//bi.lpfn = BrowseCallbackProc;
	//if (strStartFolder == NULL) {
	//	bi.lParam = FALSE;
	//}
	//else {
	//	strTmpPath.Format("%s", strStartFolder);
	//	bi.lParam = TRUE;
	//}
	//bi.iImage = NULL;
	//lpID = SHBrowseForFolderA(&bi);
	//if (lpID != NULL) {
	//	BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
	//	if (b == TRUE) {
	//		if (strlen(pszDisplayName)>3)
	//			strcat(pszDisplayName, "\\");
	//		strSelectedFolder->Format("%s", pszDisplayName);
	//		return TRUE;
	//	}
	//}
	//else {
	//	strSelectedFolder->Empty();
	//}
	//return FALSE;

	/*CSelectFolder dlg;
	dlg.SetPath(strStartFolder);
	if (dlg.DoModal() == IDOK)
	{
	strSelectedFolder->Format("%s", dlg.GetPath());
	return TRUE;
	}
	return FALSE;*/

	// New Dialog Select Folder - Mason
	CFolderPickerDialog	dlg;
	dlg.m_ofn.lpstrInitialDir = strStartFolder;

	if (dlg.DoModal() == IDOK) {
		strSelectedFolder->Format("%s", dlg.GetPathName());
		return TRUE;
	}
	else {
		strSelectedFolder->Empty();
	}

	return FALSE;
}

void CApplication::OnSaveAllButton()
{
	m_nCommand = SAVE_ALL_IMAGES;
	m_pTracks[0].m_pActiveDoc = m_pTracks[0].m_pDoc[0];
	m_nActiveTrackCommandIdx = 0;
	SetEvent(m_EventRun);
}

void CApplication::OnUpdateSaveAllButton(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_bOnline &&
		!m_bTeaching &&
		!m_bInspecting &&
		!m_bTrackInspSeq &&
		/*!m_bInspSavedImage &&
		!m_bInspectionDebug &&
		!m_bStoredImages &&*/
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nSaveImgAll));
}

void CApplication::SaveAllImages()
{
	CString folderPath = "C:\\";
	CString stringTemp = "";
	CString strFolderType[] = { "TOP_STATION","THR_STATION", "ENCAP_STATION" };
	CString strFile, strTemp, strFolder, strFileName;

	if (GetFolder(&folderPath, folderPath)) {
		if (!folderPath.IsEmpty()) {
			stringTemp.Format("Stored Images Folder = " + folderPath);
			UpdateStatusBar(&stringTemp, 1);

		//	folderPath += m_strConfigFile;
			_mkdir(folderPath);

			for (int i = 0; i < m_nTotalTracks; ++i) {
				//				if (m_StationHandler[i].getRealId() != STATION_BOND) {
				stringTemp.Format("\\%s", strFolderType[i]);
				_mkdir(folderPath + stringTemp);
				//for (int j = 0; j < m_pTracks[i].m_nTotalDocs; ++j) {
				//	stringTemp.Format("\\%s\\Doc %d", strFolderType[i], j + 1);
				//	_mkdir(folderPath + stringTemp);
				//}
//				}
			}
		}
		int nFovNoPerY, nFovNoPerX, nDieNoPerX, nDIeNoPerY;

		motionParameters.GetMappingParameters(nDieNoPerX, nDIeNoPerY, nFovNoPerX, nFovNoPerY);

		int nNoofDie = nDieNoPerX*nDIeNoPerY, nNoOfFov[] = { nFovNoPerY ,nFovNoPerX *nFovNoPerY };
		CWaitCursor wait;
		for (int nDie = 0; nDie < nNoofDie; nDie++) {
			for (int nTrack = 0; nTrack< m_nTotalTracks; nTrack++) {
				for (int nDoc = 0; nDoc < m_pTracks[nTrack].m_nTotalDocs; nDoc++) {
					for (int nFov = 0; nFov < nNoOfFov[nTrack]; nFov++) {

						int nRow = (nFov) / nFovNoPerX, nColumn = (nFov) % nFovNoPerX;

						CString strName;
						int nPos = folderPath.ReverseFind('\\');
						strFolder = folderPath.Left(nPos);
						strFileName = folderPath.Mid(nPos);
						strTemp.Format("%s\\%s\\Doc %02d", strFolder, strFolderType[nTrack], nDoc);
					//	mkdir(strTemp);
						CreateDirectoryTree(strTemp);
						strTemp.Format("%s\\%s\\Doc %02d", strFolder, strFolderType[nTrack], nDoc);
						strName.Format("%s%s Doc%02d DieNo%03d Row%03d Col%03d FOV%03d", strFileName, strFolderType[nTrack], nDoc, nDie, nRow, nColumn, nFov);
						strFile.Format("%s%s.bmp", strTemp, strName);
						long cameraGrabId;
						if (nTrack == STATION_TOP)
							motionParameters.GetCameraGrabIDTOP(0, nRow, nColumn, nDie, cameraGrabId);
						else if (nTrack == STATION_THR)
							motionParameters.GetCameraGrabIDTHR(0, nRow, nColumn, nDie, cameraGrabId);

						if (!m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab.empty())
							if (m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab[cameraGrabId].IsAllocated())
								SaveGrayScaleBMPFile(strFile, m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab[cameraGrabId]);
					}
				}
			}
		}
	}
}

bool CApplication::IsSFTEnabled() const
{
	return m_bSFTEnableCommunication;
}

int CApplication::getSFTMoveTypeMoveIn() const
{
	return m_nSFTMoveTypeMoveIn;
}

int CApplication::getSFTMoveTypeMoveOut() const
{
	return m_nSFTMoveTypeMoveOut;
}

int CApplication::getSFTTimeOut() const
{
	return m_nSFTTimeOut;
}

std::string CApplication::getSFTServerAddressPath() const
{
	return m_strSFTServerAddress.GetString();
}


int CApplication::getImageSaveLimitPercentage() const
{
	return pGlobalBackgroundThread->getSaveImageLimitPercentage();
}

void CApplication::OnImageSavingImageParam()
{
	ConfigureImageSavingLimitDialog configureImageSaveLimitDialog;

	configureImageSaveLimitDialog.DoModal();
}


int CApplication::getSequenceIntensityId()
{
	return m_nSequenceIntensityId;
}

void CApplication::SetSequenceIntensityId(int nIntnsity)
{
	m_nSequenceIntensityId = nIntnsity;
}

void CApplication::OnUpdateImageSavingImageParam(CCmdUI *pCmdUI)
{
	BOOL bTrackLiveImg, bDocInsp;
	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		bTrackLiveImg &= m_pTracks[nTrack].m_bLiveImage;
		bDocInsp &= m_pTracks[nTrack].m_pDoc[0]->m_bInspecting;


	}
	BOOL bStepInspectionEnabled = FALSE;
	BOOL bStoredInspectionEnabled = FALSE;
	for (int nTrack = 0; nTrack < m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < m_nTotalViewDocs[nTrack]; nDoc++) {
			bStepInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspSavedImage;
			bStoredInspectionEnabled |= m_pTracks[nTrack].m_pDoc[nDoc]->m_bStoredImages;
		}
	}

	pCmdUI->Enable(!m_bOnline &&
		!bStepInspectionEnabled &&
		!m_bTeaching &&
		!m_bTrackInspSeq &&
		!m_bInspecting &&
		!bTrackLiveImg &&
		!bDocInsp &&
		!bStoredInspectionEnabled &&
		(m_UserAccInfo.m_nCurrentAccessLevel >= m_AdvSecurityDlg.m_data.m_nDeviceCfg));
}

//-- Secs/gem information
bool CApplication::isSecsGemEnabled() const
{
#ifdef _NO_SECSGEM
	return false;
#else
	return _secsgemEnableCommunication;
#endif
}

//-- Secs/gem initialization
bool CApplication::isSecsGemInitialized()
{
	if (getGEMController().getController() != nullptr)
		return true;
	else
		return false;
}

SecsGemController& CApplication::getGEMController()
{
	return _gemController;
}

void CApplication::sendGemEvent(CString eventName)
{
	if (getGEMController().getController() == nullptr && !isSecsGemEnabled() && eventName.IsEmpty())
		return;

	getGEMController().sendEventReport(eventName);
}

void CApplication::readEquipmentMapData(std::string filePath)

{
	//ICOMSECsDataItemPtr p_EnabledEvents = getGEMController().getEnabledEvents();
	//
	//for (int i = 0; i<p_EnabledEvents->Count; i++) {
	//	CString cs_EnabledEvent = (CString)p_EnabledEvents->GetItem(i)->GetValue();
	//	_bstr_t cs_Name = p_EnabledEvents->GetItem(i)->GetName();
	//	ICOMSECsDataItemPtr test = p_EnabledEvents->GetItem_2(cs_Name);
	//}

	// Obtain Secsgem settings

	string EnableMultiOpenTransaction;

	try {
		//Qualify namespase explicitly to avoid Compiler Error C2872 "ambiguous symbol" during linking.
		//Now Msxml2.dll use the "MSXML2" namespace
		//(see http://support.microsoft.com/default.aspx?scid=kb;en-us;316317):
		MSXML2::IXMLDOMDocumentPtr docPtr;//pointer to DOMDocument object
		MSXML2::IXMLDOMNodeListPtr NodeListPtr;//indexed access. and iteration through the collection of nodes
		MSXML2::IXMLDOMNodePtr DOMNodePtr;//pointer to the node

		MSXML2::IXMLDOMNode *pIDOMNode = NULL;//pointer to element's node
		MSXML2::IXMLDOMNode *pIParentNode = NULL;//pointer to parent node
		MSXML2::IXMLDOMNode *pIAttrNode = NULL;//pointer to attribute node
		MSXML2::IXMLDOMNamedNodeMapPtr DOMNamedNodeMapPtr;//iteration through the collection of attribute nodes
		MSXML2::IXMLDOMNodeList *childList = NULL;//node list containing the child nodes

												  //Variable with the name of node to find: 
		BSTR strFindText = L" ";//" " means to output every node
								//Variables to store item's name, parent, text and node type:
		BSTR bstrItemText, bstrItemNode, bstrItemParent, bstrNodeType;

		//Variables to store attribute's name,type and text:	 
		BSTR bstrAttrName, bstrAttrType, bstrAttrText;
		BSTR strLayoutId;
		HRESULT hResult;

		int i = 0;//loop-index variable
		int n = 0;//lines counter


				  //Initialize COM Library:
		CoInitialize(NULL);

		//Create an instance of the DOMDocument object:
		docPtr.CreateInstance(__uuidof(DOMDocument));

		// Load a document:
		_variant_t varXml(filePath.c_str());//XML file to load
		_variant_t varResult((bool)TRUE);//result 

		varResult = docPtr->load(varXml);

		if ((bool)varResult == FALSE) {
			//printf("*** Error:failed to load XML file. ***\n");
			MessageBox(0, "Error: failed to load XML file. Check the file name.", \
				"Load XML file", MB_OK | MB_ICONWARNING);
			return;
		}

		//Collect all or selected nodes by tag name:
		NodeListPtr = docPtr->getElementsByTagName(strFindText);
		int binDefinitionCount = 0;
		//Output the number of nodes:
		//printf("Number of nodes: %d\n", (NodeListPtr->length));

		//Output root node:
		docPtr->documentElement->get_nodeName(&bstrItemText);

		for (i = 0; i < (NodeListPtr->length); i++) {

			if (pIDOMNode) pIDOMNode->Release();
			NodeListPtr->get_item(i, &pIDOMNode);


			if (pIDOMNode) {

				pIDOMNode->get_nodeTypeString(&bstrNodeType);

				//We process only elements (nodes of "element" type): 
				BSTR temp = L"element";

				if (lstrcmp((LPCTSTR)bstrNodeType, (LPCTSTR)temp) == 0) {
					n++;//element node's number

					pIDOMNode->get_nodeName(&bstrItemNode);

					pIDOMNode->get_text(&bstrItemText);

					pIDOMNode->get_parentNode(&pIParentNode);
					pIParentNode->get_nodeName(&bstrItemParent);

					pIDOMNode->get_childNodes(&childList);

					/*assert(bstrItemNode != nullptr);
					std::wstring ws(bstrItemNode, SysStringLen(bstrItemNode));*/
					std::string str = ConvertBSTRToMBS(bstrItemNode);


					//Get the attributes:
					int j = 0;//loop-index variable
					long length;// number of attributes in the collection

					DOMNamedNodeMapPtr = pIDOMNode->attributes;

					hResult = DOMNamedNodeMapPtr->get_length(&length);

					if (SUCCEEDED(hResult)) {
						//Loop through the number of attributes:
						for (j = 0; j < length; j++) {
							//get attribute node:							
							DOMNamedNodeMapPtr->get_item(j, &pIAttrNode);

							pIAttrNode->get_nodeTypeString(&bstrAttrType);//type as string
																		  //pIAttrNode->get_nodeType(&bstrAttrType);//enumerated type
																		  //printf("Attribute type: %d\n", bstrAttrType);
							pIAttrNode->get_nodeName(&bstrAttrName);

							pIAttrNode->get_text(&bstrAttrText);

							std::string str = ConvertBSTRToMBS(bstrAttrName);

							if (strcmp(ConvertBSTRToMBS(bstrItemNode).c_str(), "GemConnection") == 0)
							{
								if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "deviceId") == 0)
									nEquipDeviceID = atoi(ConvertBSTRToMBS(bstrAttrText).c_str());
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "multipleOpen") == 0)
								{
									EnableMultiOpenTransaction = ConvertBSTRToMBS(bstrAttrText);
									bEnableMultiOpenTransaction = EnableMultiOpenTransaction == "true" ? 1 : 0;
								}
							}
							else if (strcmp(ConvertBSTRToMBS(bstrItemNode).c_str(), "HSMS") == 0)
							{
								if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "localPortNumber") == 0)
									nEquipLocalPortID = atoi(ConvertBSTRToMBS(bstrAttrText).c_str());
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "remotePortNumber") == 0)
									nEquipRemotePortID = atoi(ConvertBSTRToMBS(bstrAttrText).c_str());
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "localIPAddress") == 0)
									strEquipLocalAddress = ConvertBSTRToMBS(bstrAttrText).c_str();
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "remoteIPAddress") == 0)
									strEquipRemoteAddress = ConvertBSTRToMBS(bstrAttrText).c_str();
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "T8Timeout") == 0)
									nHSMST8Timeout = atoi(ConvertBSTRToMBS(bstrAttrText).c_str());
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "T7Timeout") == 0)
									nHSMST7Timeout = atoi(ConvertBSTRToMBS(bstrAttrText).c_str());
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "T6Timeout") == 0)
									nHSMST6Timeout = atoi(ConvertBSTRToMBS(bstrAttrText).c_str());
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "T5Timeout") == 0)
									nHSMST5Timeout = atoi(ConvertBSTRToMBS(bstrAttrText).c_str());
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "T3Timeout") == 0)
									nHSMST3Timeout = atoi(ConvertBSTRToMBS(bstrAttrText).c_str());
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "connectionMode") == 0)
									strConnectionMode = ConvertBSTRToMBS(bstrAttrText).c_str();
							}
						}
					}
				}
			}
		}
		//Do not forget to release interfaces:
		pIDOMNode->Release();
		pIDOMNode = NULL;
		pIParentNode->Release();
		pIParentNode = NULL;
	}

	catch (...) {
		MessageBox(NULL, ("*** Exception occurred ***"), ("Error message"), MB_OK);
	}
}

void CApplication::saveEquipmentMapData(std::string filePath)
{
	try {
		//Qualify namespase explicitly to avoid Compiler Error C2872 "ambiguous symbol" during linking.
		//Now Msxml2.dll use the "MSXML2" namespace
		//(see http://support.microsoft.com/default.aspx?scid=kb;en-us;316317):
		MSXML2::IXMLDOMDocumentPtr docPtr;//pointer to DOMDocument object
		MSXML2::IXMLDOMNodeListPtr NodeListPtr;//indexed access. and iteration through the collection of nodes
		MSXML2::IXMLDOMNodePtr DOMNodePtr;//pointer to the node

		MSXML2::IXMLDOMNode *pIDOMNode = NULL;//pointer to element's node
		MSXML2::IXMLDOMNode *pIParentNode = NULL;//pointer to parent node
		MSXML2::IXMLDOMNode *pIAttrNode = NULL;//pointer to attribute node
		MSXML2::IXMLDOMNamedNodeMapPtr DOMNamedNodeMapPtr;//iteration through the collection of attribute nodes
		MSXML2::IXMLDOMNodeList *childList = NULL;//node list containing the child nodes

												  //Variable with the name of node to find: 
		BSTR strFindText = L" ";//" " means to output every node
								//Variables to store item's name, parent, text and node type:
		BSTR bstrItemText, bstrItemNode, bstrItemParent, bstrNodeType;

		//Variables to store attribute's name,type and text:	 
		BSTR bstrAttrName, bstrAttrType, bstrAttrText;
		BSTR strLayoutId;
		HRESULT hResult;

		int i = 0;//loop-index variable
		int n = 0;//lines counter

				  //Initialize COM Library:
		CoInitialize(NULL);

		//Create an instance of the DOMDocument object:
		docPtr.CreateInstance(__uuidof(DOMDocument));

		// Load a document:
		_variant_t varXml(filePath.c_str());//XML file to load
		_variant_t varResult((bool)TRUE);//result 

		varResult = docPtr->load(varXml);

		if ((bool)varResult == FALSE) {
			//printf("*** Error:failed to load XML file. ***\n");
			MessageBox(0, "Error: failed to load XML file. Check the file name.", \
				"Load XML file", MB_OK | MB_ICONWARNING);
			return;
		}

		//Collect all or selected nodes by tag name:
		NodeListPtr = docPtr->getElementsByTagName(strFindText);
		int binDefinitionCount = 0;
		//Output the number of nodes:
		//printf("Number of nodes: %d\n", (NodeListPtr->length));

		//Output root node:
		docPtr->documentElement->get_nodeName(&bstrItemText);

		for (i = 0; i < (NodeListPtr->length); i++) {

			if (pIDOMNode) pIDOMNode->Release();
			NodeListPtr->get_item(i, &pIDOMNode);


			if (pIDOMNode) {

				pIDOMNode->get_nodeTypeString(&bstrNodeType);

				//We process only elements (nodes of "element" type): 
				BSTR temp = L"element";

				if (lstrcmp((LPCTSTR)bstrNodeType, (LPCTSTR)temp) == 0) {
					n++;//element node's number

					pIDOMNode->get_nodeName(&bstrItemNode);

					pIDOMNode->get_text(&bstrItemText);

					pIDOMNode->get_parentNode(&pIParentNode);
					pIParentNode->get_nodeName(&bstrItemParent);

					pIDOMNode->get_childNodes(&childList);

					/*assert(bstrItemNode != nullptr);
					std::wstring ws(bstrItemNode, SysStringLen(bstrItemNode));*/
					std::string str = ConvertBSTRToMBS(bstrItemNode);


					//Get the attributes:
					int j = 0;//loop-index variable
					long length;// number of attributes in the collection

					DOMNamedNodeMapPtr = pIDOMNode->attributes;

					hResult = DOMNamedNodeMapPtr->get_length(&length);

					if (SUCCEEDED(hResult)) {
						//Loop through the number of attributes:
						for (j = 0; j < length; j++) {
							//get attribute node:							
							DOMNamedNodeMapPtr->get_item(j, &pIAttrNode);

							pIAttrNode->get_nodeTypeString(&bstrAttrType);//type as string
																		  //pIAttrNode->get_nodeType(&bstrAttrType);//enumerated type
																		  //printf("Attribute type: %d\n", bstrAttrType);
							pIAttrNode->get_nodeName(&bstrAttrName);

							pIAttrNode->get_text(&bstrAttrText);

							std::string str = ConvertBSTRToMBS(bstrAttrName);

							if (strcmp(ConvertBSTRToMBS(bstrItemNode).c_str(), "GemConnection") == 0)
							{
								if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "deviceId") == 0)
								{
									std::wstring s = std::to_wstring(nEquipDeviceID);
									pIAttrNode->put_text(SysAllocString(s.c_str()));
								}
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "multipleOpen") == 0)
								{
									if (bEnableMultiOpenTransaction)
										pIAttrNode->put_text(L"true");
									else
										pIAttrNode->put_text(L"false");
								}
							}
							else if (strcmp(ConvertBSTRToMBS(bstrItemNode).c_str(), "HSMS") == 0)
							{
								//T8Timeout
								if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "T8Timeout") == 0)
								{
									std::wstring s = std::to_wstring(nHSMST8Timeout);
									pIAttrNode->put_text(SysAllocString(s.c_str()));
								}
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "T7Timeout") == 0)
								{
									std::wstring s = std::to_wstring(nHSMST7Timeout);
									pIAttrNode->put_text(SysAllocString(s.c_str()));
								}
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "T6Timeout") == 0)
								{
									std::wstring s = std::to_wstring(nHSMST6Timeout);
									pIAttrNode->put_text(SysAllocString(s.c_str()));
								}
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "T5Timeout") == 0)
								{
									std::wstring s = std::to_wstring(nHSMST5Timeout);
									pIAttrNode->put_text(SysAllocString(s.c_str()));
								}
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "T3Timeout") == 0)
								{
									std::wstring s = std::to_wstring(nHSMST3Timeout);
									pIAttrNode->put_text(SysAllocString(s.c_str()));
								}
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "localPortNumber") == 0)
								{
									std::wstring s = std::to_wstring(nEquipLocalPortID);
									pIAttrNode->put_text(SysAllocString(s.c_str()));
								}
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "remotePortNumber") == 0)
								{
									std::wstring s = std::to_wstring(nEquipRemotePortID);
									pIAttrNode->put_text(SysAllocString(s.c_str()));
								}
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "localIPAddress") == 0)
									pIAttrNode->put_text(strEquipLocalAddress.AllocSysString());
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "remoteIPAddress") == 0)
									pIAttrNode->put_text(strEquipRemoteAddress.AllocSysString());
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "connectionMode") == 0)
									pIAttrNode->put_text(strConnectionMode.AllocSysString());
							}
						}
					}
				}
			}
		}
		//Do not forget to release interfaces:
		pIDOMNode->Release();
		pIDOMNode = NULL;
		pIParentNode->Release();
		pIParentNode = NULL;
		docPtr->save(varXml);
	}

	catch (...) {
		MessageBox(NULL, ("*** Exception occurred ***"), ("Error message"), MB_OK);
	}

	InitializeSecsgem(true);
}

int CApplication::DeleteRecipeFile(CString strDelRecipeFile)
{
	CString m_strMsg, strHr, strMin, strSec, strCategories, strDelFile;
	CString strSel[2], str;

	int cat = strDelRecipeFile.FindOneOf("\\");
	strCategories = strDelRecipeFile.Left(cat);

	strDelFile = strDelRecipeFile.Right(strDelRecipeFile.GetLength() - cat - 1);

	strSel[0] = strCategories;

	HANDLE hFile;
	WIN32_FIND_DATA FindFileData;
	hFile = FindFirstFile(theApp.m_strMainCfgDir + "\\" + strCategories + "\\" + strDelFile, &FindFileData);
	if (hFile == INVALID_HANDLE_VALUE) {
		FindClose(hFile);
		return -1;
		//	AfxMessageBox("ERROR: File not found", MB_ICONSTOP);
	}
	else {
		strSel[1] = strDelFile;
		char strTemp[MAX_PATH];
		memset(strTemp, NULL, MAX_PATH);
		sprintf(strTemp, "%s\\%s\\%s", theApp.m_strMainCfgDir.GetBuffer(0), strSel[0].GetBuffer(0), strSel[1].GetBuffer(0));

		SHFILEOPSTRUCT fileOpStruct;
		//		fileOpStruct.hwnd = m_hWnd;
		fileOpStruct.wFunc = FO_DELETE;
		fileOpStruct.pFrom = strTemp;
		fileOpStruct.pTo = NULL;
		fileOpStruct.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
		SHFileOperation(&fileOpStruct);

		ListAllProcessRecipes();

		theApp.sendGemEvent("RecipeDeleted");
		theApp.getGEMController().setDataVariablesValue("PPChangeStatus", getValueInString(3));
		m_strMsg = "config. file has been deleted\r\n";
		m_strMsg = CTime::GetCurrentTime().Format("%H:%M:%S") + "   " + m_UserAccInfo.m_strCurUserName + "   " + "'" + strSel[0] + " - " + strDelFile + "' " + m_strMsg;

		return 0;
	}
}

void CApplication::StartCommandSECSGEM()
{
	// Start Machine
	bStartRemoteCmdRcvd = TRUE;
	bSoftTriggerStop = FALSE;

	if (processState == PROCESS_STATE_PAUSE)
	{
		/*if (theApp.pMachineSequence == NULL)
			theApp.pMachineSequence = new MachineSequence();

		theApp.pMachineSequence->activateModuleRunMode();
		theApp.pMachineSequence->Run(TRUE);*/
	}
	else
	{
		// automatically clear error dialog without user interaction
		/*theApp.pMachineSequence->Retry(0);
		theApp.pMachineSequence->Retry(2);*/
	}

	getGEMController().setStatusVariablesValue("ProcessState", getValueInString(PROCESS_STATE_EXECUTING));		//Girish Krishna
	OutputDebugLogTo(9, TRUE, "Set Machine ProcessState to EXEC");
	sendGemEvent("ProcessingStateChanged");
}

void CApplication::StopCommandSECSGEM()
{
	/*bStartRemoteCmdRcvd = FALSE;
	if (theApp.pMachineSequence != NULL) {

		bSoftTriggerStop = TRUE;
		if (theApp.pMachineSequence->IsRunning()) {
			int nLotSubstrateCount = pMachineSequence->getSubstrateCountPerLot();
			int nProcessSubstrateCount = pMachineSequence->getSubstrateCount();

			if (nLotSubstrateCount > 0 && nProcessSubstrateCount < nLotSubstrateCount) {
				CString strErrorMessage;
				strErrorMessage.Format("ERROR : %d Substrates completed out of %d", nProcessSubstrateCount, nLotSubstrateCount);
				AfxMessageBox(strErrorMessage);
				getGEMController().setAlarm("LOT_END_SUBSTRATE_COUNT_ERROR");

				theApp.pSubstrateInspectionResultsWnd->updateInspectionResults();

				if (theApp.isSecsGemEnabled() && theApp.isSecsGemInitialized()) {
					if (theApp.getGEMController().getControlState() != ControlState_HostOffline)
						theApp.getGEMController().UpdateSubstrateResults(strInspectDeviceBarcodeID);
					theApp.sendGemEvent("StripProcessingCompleted");
				}
			}
		}
	}*/
}

void CApplication::PauseCommandSECSGEM()
{
	getGEMController().setStatusVariablesValue("ProcessState", getValueInString(PROCESS_STATE_PAUSE));		//Girish Krishna
	OutputDebugLogTo(9, TRUE, "Set Machine ProcessState to PAUSE");
	sendGemEvent("ProcessingStateChanged");

	/*if (theApp.pMachineSequence != NULL) {
		theApp.pMachineSequence->Stop();
		theApp.sendGemEvent("ProcessingPaused");
	}*/
}

void CApplication::AbortCommandSECSGEM()
{
	//if (theApp.pMachineSequence != NULL) {

	//	if (theApp.pMachineSequence->IsRunning()) {

	//		//theApp.pMachineSequence->Stop();
	//		theApp.pMachineSequence->RemoveSequence();
	//		//Msg
	//		//			theApp.pALibabaMsgBox->Msg("Machine Sequence Stopped", 3500);
	//	}
	//}
}

int CApplication::StartProcessSelectCommandSECSGEM(CString strLotId, CString strCategories, CString strRecipe, int nSubstrateCount)
{
	//LoadConfigFile(strCategories, strRecipe);

	//update_currentLotInformation(strLotId, strLotId, strLotId, "");
	//pMachineSequence->setMagazineCountPerLot(0);
	//pMachineSequence->setSubstrateCountPerLot(nSubstrateCount);

	//bEndLotMagzineBased = FALSE;
	//bStartRemoteCmdRcvd = FALSE;

	//pSubstrateInspectionResultsWnd->setup(machineID.GetString(), strLotId.GetString(), strLotId.GetString(),
	//	strLotId.GetString(), "");

	//getGEMController().setStatusVariablesValue("ProcessState", getValueInString(READY));		//Girish Krishna
	//OutputDebugLogTo(9, TRUE, "Set Machine ProcessState to READY");
	//sendGemEvent("ProcessingStateChanged");

	//if (theApp.pMachineSequence == NULL)
	//	theApp.pMachineSequence = new MachineSequence();

	//theApp.pMachineSequence->activateModuleRunMode();
	//theApp.pMachineSequence->Run(TRUE);

	return 0;
}

void CApplication::ListAllProcessRecipes()
{
	HANDLE hFile;
	WIN32_FIND_DATA FindFileData;
	CString strCategories;

	RecipeFiles.clear();

	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
			strCategories = "Engineering";
		else if (i == 1)
			strCategories = "Production";
		else
			strCategories = "Qualification";

		if ((hFile = FindFirstFile(m_strMainCfgDir + "\\" + strCategories + "\\*.*", &FindFileData)) != INVALID_HANDLE_VALUE) {
			do {
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					CString str = FindFileData.cFileName;
					if (IsPackageFolder(&str))
					{
						RecipeFiles.push_back(strCategories + "\\" + str);
					}
				}
			} while (FindNextFile(hFile, &FindFileData));
		}
	}
	FindClose(hFile);
}

void CApplication::ChangeEquipmentConstantValue(long lConstId, CString strConstLogicalName, SECSFormats oSvFormat, CString value)

{
	int nValue = 0;
	double dValue = 0;
	bool bValue = false;

	if (oSvFormat == 2)
	{
		if (value == "-1")		// "TRUE")
			bValue = true;
		else
			bValue = false;
	}
	else if (oSvFormat == 3 || oSvFormat == 4)
	{
		dValue = atof((LPCSTR)value);
	}
	else if (oSvFormat == 5 || oSvFormat == 6 || oSvFormat == 7 || oSvFormat == 8 ||
		oSvFormat == 10 || oSvFormat == 11 || oSvFormat == 12 || oSvFormat == 13)
	{
		nValue = _wtoi((_bstr_t)value);
	}
	else
	{

	}

	/*InspectionParameter parameter;

	if (strConstLogicalName == "MagazineHeight")
	{
		pMachineSequence->getSequenceInfo().magazine.heightMm = dValue;
	}
	else if (strConstLogicalName == "MagazineLength")
	{
		pMachineSequence->getSequenceInfo().magazine.lengthMm = dValue;
	}
	else if (strConstLogicalName == "MagazineWidth")
	{
		pMachineSequence->getSequenceInfo().magazine.widthMm = dValue;
	}
	else if (strConstLogicalName == "MagazinePitch")
	{
		pMachineSequence->getSequenceInfo().magazine.pitchMm = dValue;
	}
	else if (strConstLogicalName == "MagazineFirstOffset")
	{
		pMachineSequence->getSequenceInfo().magazine.firstPositionOffsetMm = dValue;
	}
	else if (strConstLogicalName == "MagazineBottomSupportDepth")
	{
		pMachineSequence->getSequenceInfo().magazine.bottomOffsetMm = dValue;
	}
	else if (strConstLogicalName == "SubstrateWidth")
	{
		pMachineSequence->getSequenceInfo().magazine.substrateWidthMm = dValue;
	}
	else if (strConstLogicalName == "SubstrateCount")
	{
		pMachineSequence->getSequenceInfo().magazine.substrateCount = nValue;
	}
	else if (strConstLogicalName == "NumberOfBlockX")
	{
		bReloadConfig = true;
		motionParameters.setNumberOfBlockX(nValue);
	}
	else if (strConstLogicalName == "NumberOfBlockY")
	{
		bReloadConfig = true;
		motionParameters.setNumberOfBlockY(nValue);
	}
	else if (strConstLogicalName == "BlockPitchX")
	{
		bReloadConfig = true;
		motionParameters.setBlockPitchX(dValue);
	}
	else if (strConstLogicalName == "BlockPitchY")
	{
		bReloadConfig = true;
		motionParameters.setBlockPitchY(dValue);
	}
	else if (strConstLogicalName == "DiePerBlockX")
	{
		bReloadConfig = true;
		motionParameters.setNumberOfDiePerBlockX(nValue);
	}
	else if (strConstLogicalName == "DiePerBlockY")
	{
		bReloadConfig = true;
		motionParameters.setNumberOfDiePerBlockY(nValue);
	}
	else if (strConstLogicalName == "DiePitchX")
	{
		bReloadConfig = true;
		motionParameters.setDiePitchX(dValue);
	}
	else if (strConstLogicalName == "DiePitchY")
	{
		bReloadConfig = true;
		motionParameters.setDiePitchY(dValue);
	}
	else if (strConstLogicalName == "DiePerImageX")
	{
		bReloadConfig = true;
		motionParameters.setNumberOfImagePerDieX(nValue);
	}
	else if (strConstLogicalName == "DiePerImageY")
	{
		bReloadConfig = true;
		motionParameters.setNumberOfImagePerDieY(nValue);
	}
	else if (strConstLogicalName == "DeviceSizeX")
	{
		bReloadConfig = true;
		motionParameters.setDeviceSizeX(dValue);
	}
	else if (strConstLogicalName == "DeviceSizeY")
	{
		bReloadConfig = true;
		motionParameters.setDeviceSizeY(dValue);
	}
	else if (strConstLogicalName == "BondSearchAreaX")
	{
		bReloadConfig = true;
		motionParameters.setBondSearchX(dValue);
	}
	else if (strConstLogicalName == "BondSearchAreaY")
	{
		bReloadConfig = true;
		motionParameters.setBondSearchY(dValue);
	}
	else if (strConstLogicalName == "Enable23DScanning")
	{
		pMachineSequence->bEnable23DScanning = bValue;
	}
	else if (strConstLogicalName == "EnableBondScanning")
	{
		pMachineSequence->bEnableBondScan = bValue;
	}
	else if (strConstLogicalName == "EnableQRBarcodeScanning")
	{
		pMachineSequence->enableQRBarcodeScanning = bValue;
	}
	else if (strConstLogicalName == "EnableVacuumSequence")
	{
		pMachineSequence->bEnableVacuum = bValue;
	}
	else if (strConstLogicalName == "EnableVacuumSwitch")
	{
		pMachineSequence->bCheckVcuumSwitch = bValue;
	}
	else if (strConstLogicalName == "EnableLotStripBarcodeComparison")
	{
		pMachineSequence->enableLotStripBarcodeComparison = bValue;
	}
	else if (strConstLogicalName == "EnableImageSavingLimitCheck")
	{
		pMachineSequence->setEnableSaveImageLimitStatus(bValue);
	}
	else if (strConstLogicalName == "EnableContinuosInspectFailCheck")
	{
		pMachineSequence->setEnableContinuousInspectFailStatus(bValue);
	}
	else if (strConstLogicalName == "ImageSavingLimit")
	{
		pMachineSequence->setSaveImageLimitPercentage(nValue);
	}
	else if (strConstLogicalName == "ContinuousInspectFailCount")
	{
		pMachineSequence->setContinuousInspectFailCount(nValue);
	}
	else if (strConstLogicalName == "SubstrateWidthLoaderOffset")
	{
		motionParameters.setWidthVariationOffset(dValue);
	}
	else if (strConstLogicalName == "MagazineLengthLoaderOffset")
	{
		motionParameters.setLoaderXVariationOffset(dValue);
	}
	else if (strConstLogicalName == "MagazineLengthUnloaderOffset")
	{
		motionParameters.setUnloaderXVariationOffset(dValue);
	}
	else if (strConstLogicalName == "MagazineWidthLoaderOffset")
	{
		motionParameters.setLoaderMagazineWidthOffset(dValue);
	}
	else if (strConstLogicalName == "MagazineWidthUnloaderOffset")
	{
		motionParameters.setUnloaderMagazineWidthOffset(dValue);
	}
	else if (strConstLogicalName == "Not inspected")
	{
		updateDefectCodes("Not inspected", value);
	}
	else if (strConstLogicalName == "Unknown defect")
	{
		updateDefectCodes("Unknown defect", value);
	}
	else if (strConstLogicalName == "Good unit")
	{
		updateDefectCodes("Good unit", value);
	}
	else if (strConstLogicalName == "Substrate location")
	{
		updateDefectCodes("Substrate location", value);
	}
	else if (strConstLogicalName == "Inked die")
	{
		updateDefectCodes("Inked die", value);
	}
	else if (strConstLogicalName == "Die location")
	{
		updateDefectCodes("Die location", value);
	}
	else if (strConstLogicalName == "Die offset")
	{
		updateDefectCodes("Die offset", value);
	}
	else if (strConstLogicalName == "Die rotation")
	{
		updateDefectCodes("Die rotation", value);
	}
	else if (strConstLogicalName == "Epoxy")
	{
		updateDefectCodes("Epoxy", value);
	}
	else if (strConstLogicalName == "PVI Unit Surface")
	{
		updateDefectCodes("PVI Unit Surface", value);
	}
	else if (strConstLogicalName == "PVI Device")
	{
		updateDefectCodes("PVI Device", value);
	}
	else if (strConstLogicalName == "PVI Unit Edge")
	{
		updateDefectCodes("PVI Unit Edge", value);
	}
	else if (strConstLogicalName == "Missing Wire")
	{
		updateDefectCodes("Missing Wire", value);
	}
	else if (strConstLogicalName == "Wire")
	{
		updateDefectCodes("Wire", value);
	}
	else if (strConstLogicalName == "Sway Wire")
	{
		updateDefectCodes("Sway Wire", value);
	}
	else if (strConstLogicalName == "Wire Loop Height")
	{
		updateDefectCodes("Wire Loop Height", value);
	}
	else if (strConstLogicalName == "Wire Length")
	{
		updateDefectCodes("Wire Length", value);
	}
	else if (strConstLogicalName == "Wire Short to Die")
	{
		updateDefectCodes("Wire Short to Die", value);
	}
	else if (strConstLogicalName == "Off Pad")
	{
		updateDefectCodes("Off Pad", value);
	}
	else if (strConstLogicalName == "Missing ball on bond pad")
	{
		updateDefectCodes("Missing ball on bond pad", value);
	}
	else if (strConstLogicalName == "Ball location on bond pad")
	{
		updateDefectCodes("Ball location on bond pad", value);
	}
	else if (strConstLogicalName == "Bincode Ball diameter on bond pad")
	{
		updateDefectCodes("Ball diameter on bond pad", value);
	}
	else if (strConstLogicalName == "Stitch dimension")
	{
		updateDefectCodes("Stitch dimension", value);
	}
	else if (strConstLogicalName == "Stitch dimension")
	{
		updateDefectCodes("Stitch dimension", value);
	}
	else if (strConstLogicalName == "Stitch quality")
	{
		updateDefectCodes("Stitch quality", value);
	}
	else if (strConstLogicalName == "Stud ball")
	{
		updateDefectCodes("Stud ball", value);
	}
	else if (strConstLogicalName == "Smash bond")
	{
		updateDefectCodes("Smash bond", value);
	}
	else if (strConstLogicalName == "Off pad")
	{
		updateDefectCodes("Off pad", value);
	}
	else if (strConstLogicalName == "Unit Club Ball")
	{
		updateDefectCodes("Unit Club Ball", value);
	}
	else if (strConstLogicalName == "Device Club Ball")
	{
		updateDefectCodes("Device Club Ball", value);
	}
	else
	{

	}*/
}

void CApplication::saveAllParameters()
{
	//pMachineSequence->saveSequenceOptionParameters();
	//pMachineSequence->SaveData();

	//motionParameters.GetWidthMotorPosition(pMachineSequence->getSequenceInfo().magazine.substrateWidthMm, pMachineSequence->getSequenceInfo().transport.width.ready.position);

	//motionParameters.GetLoaderZMotorFirstSubstratePosition(pMachineSequence->getSequenceInfo().magazine.firstPositionOffsetMm, pMachineSequence->getSequenceInfo().magazine.bottomOffsetMm, pMachineSequence->getSequenceInfo().loader.servo.magazineReady.position);

	//motionParameters.GetUnloaderZMotorFirstSubstratePosition(pMachineSequence->getSequenceInfo().magazine.firstPositionOffsetMm, pMachineSequence->getSequenceInfo().magazine.bottomOffsetMm, pMachineSequence->getSequenceInfo().unloader.servo.magazineReady.position);

	//// Get loader and unloader X ready position
	//motionParameters.getLoaderXMotorPosition(pMachineSequence->getSequenceInfo().magazine.lengthMm, pMachineSequence->getSequenceInfo().loader.stepperX.ready.position);
	//motionParameters.getUnloaderXMotorPosition(pMachineSequence->getSequenceInfo().magazine.lengthMm, pMachineSequence->getSequenceInfo().unloader.stepperX.ready.position);

	//// Get loader and unloader Y magazine ready position
	//motionParameters.getLoaderYMagazineReadyPosition(pMachineSequence->getSequenceInfo().magazine.widthMm, motionParameters.getLoaderMagazineWidthOffset(), pMachineSequence->getSequenceInfo().magazine.substrateWidthMm, pMachineSequence->getSequenceInfo().loader.stepperY.ready.position);
	//motionParameters.getUnloaderYMagazineReadyPosition(pMachineSequence->getSequenceInfo().magazine.widthMm, motionParameters.getUnloaderMagazineWidthOffset(), pMachineSequence->getSequenceInfo().magazine.substrateWidthMm, pMachineSequence->getSequenceInfo().unloader.stepperY.ready.position);
	////

	//motionParameters.SaveMotionParm();
	//motionParameters.updateSubstrateMappingParameterForGEM();

	//motionParameters.SaveScanningParm();
	//motionParameters.saveSetupParameters();

	//if (bReloadConfig)
	//{
	//	bReloadConfig = false;
	//	LoadConfigFile();
	//}
}

void CApplication::updateDefectCodes(CString defName, CString defCode)
{
	//std::vector < ItemInfo > _defect;
	//std::vector < GroupInfo > _group;
	//CString name;

	//_defect = pSubstrateInspectionResultsWnd->getDefect();
	////	_group = pSubstrateInspectionResultsWnd->getGroup();

	//for (size_t defectId = 0; defectId < _defect.size(); ++defectId) {

	//	name = _defect[defectId].description.c_str();

	//	if (name == defName)
	//	{
	//		_defect[defectId].code = defCode;
	//		theApp.pSubstrateInspectionResultsWnd->setDefect(_defect[defectId]);
	//		break;
	//	}
	//}

	//name.Format("%s\\Defect code.dat", m_strMainCfgDir);
	//pSubstrateInspectionResultsWnd->save(name.GetString());
}

//Initialize SECS/GEM
int CApplication::InitializeSecsgem(bool reInitializtion)
{
	if (isSecsGemEnabled()) {
		_secsgemEnableCommunication = false;
		if (!reInitializtion)
		{
			getGEMController().setPath(".\\SherlockGemsModel.xml", "C:/Temp");
			getGEMController().activateTerminalInformationDisplay(true);
		}
		secsinitRetCode = getGEMController().initializeGem(reInitializtion);
		CString secspost = secsinitRetCode == 0 ? "SECS-GEM Initialized Successfully" : "SECS - GEM Failed to Initialize";
		_secsgemEnableCommunication = secsinitRetCode == 0 ? true : false;
		AddLogEvent(secspost, true, true);

		if (!reInitializtion)
		{
			readEquipmentMapData(".\\SherlockGemsModel.xml");
			theApp.getGEMController().setStatusVariablesValue("PPExecName", m_strConfigFile);
		}

		if (secsinitRetCode)
		{
			_secsgemEnableCommunication = false;
			AfxMessageBox("Failed to Initialize SECSGEM. Please ensure that Dongle is connected and relaunch the application.");
			return -1;
		}
	}
	return 0;
}
