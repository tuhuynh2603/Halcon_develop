// SecsGemDiagnosticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SecsGemDiagnosticsDlg.h"
#include "Application.h"
#include <fstream>
#include "afxdialogex.h"

static const UINT_PTR GEM_INFORMATION_UPDATE_TIMER = 800;
#define COLOR_BACKROUND RGB(94,136,158)


// SecsGemDiagnosticsDlg dialog

IMPLEMENT_DYNAMIC(SecsGemDiagnosticsDlg, CDialog)

SecsGemDiagnosticsDlg::SecsGemDiagnosticsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SecsGemDiagnosticsDlg::IDD, pParent)
	, _communicationState(_T("DISABLED"))
	, _controlState(_T("OFFLINE"))
	, _gemVariableValue(_T(""))
	, _spoolingState(_T(""))
	, _spoolingSubState(_T(""))
	, _selectedGemVariableType(0)
	, _selectedControlState(0)
	, _selectedCommunicateState(0)
	, m_strHostDtateTime(_T(""))
{
	m_brush.CreateSolidBrush(COLOR_BACKROUND);
}

SecsGemDiagnosticsDlg::~SecsGemDiagnosticsDlg()
{
	m_brush.DeleteObject();
}

void SecsGemDiagnosticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENABLE_COMMUNICATION_BUTTON, _enableCommunicationButton);
	DDX_Control(pDX, IDC_LOGGER_LIST, _loggerList);
	DDX_Text(pDX, IDC_COMM_STATE, _communicationState);
	DDX_Text(pDX, IDC_CONTROL_STATE, _controlState);
	DDX_Control(pDX, IDC_COMMUNICATION_ONLINE_BUTTON, _communicationOnlineButton);
	DDX_Control(pDX, IDC_DISABLED_EVENT_COMBO, _disabledEventComboBox);
	DDX_Control(pDX, IDC_ENABLED_EVENT_COMBO, _enabledEventComboBox);
	DDX_Control(pDX, IDC_GEM_VARIABLES_COMBO, _gemVariableComboBox);
	DDX_Control(pDX, IDC_STATUS_VARIABLES_VALUE_EDIT, _statusVariableEditControl);
	DDX_Text(pDX, IDC_STATUS_VARIABLES_VALUE_EDIT, _gemVariableValue);
	DDX_Control(pDX, IDC_ALARMS_COMBO, _alarmComboBox);
	DDX_Text(pDX, IDC_SPOOL_STATE, _spoolingState);
	DDX_Text(pDX, IDC_SPOOL_SUBSTATE, _spoolingSubState);
	DDX_Control(pDX, IDC_GEM_VARIABLES_TYPE_COMBO, _gemVariableTypeComboBox);
	DDX_CBIndex(pDX, IDC_GEM_VARIABLES_TYPE_COMBO, _selectedGemVariableType);
	DDX_Radio(pDX, IDC_LOCAL_RADIO, _selectedControlState);
	DDX_Text(pDX, IDC_EDIT_HOST_TIME, m_strHostDtateTime);
}


BEGIN_MESSAGE_MAP(SecsGemDiagnosticsDlg, CDialog)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_ENABLE_COMMUNICATION_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedEnableCommunicationButton)
	ON_BN_CLICKED(IDC_COMMUNICATION_ONLINE_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedCommunicationOnlineButton)
	ON_BN_CLICKED(IDC_LOCAL_RADIO, &SecsGemDiagnosticsDlg::OnBnClickedLocalRadio)
	ON_BN_CLICKED(IDC_REMOTE_RADIO, &SecsGemDiagnosticsDlg::OnBnClickedRemoteRadio)
	ON_BN_CLICKED(IDC_REFRESH_EVENT_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedRefreshEventButton)
	ON_BN_CLICKED(IDC_ENABLE_EVENT_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedEnableEventButton)
	ON_BN_CLICKED(IDC_DISABLE_EVENT_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedDisableEventButton)
	ON_BN_CLICKED(IDC_SEND_EVENT_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedSendEventButton)
	ON_BN_CLICKED(IDC_CLEAR_LOG_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedClearLogButton)
	ON_BN_CLICKED(IDC_GET_GEM_VARIABLE_VALUE_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedGetStatusVariableValueButton)
	ON_BN_CLICKED(IDC_SET_GEM_VARIABLE_VALUE_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedSetStatusVariableValueButton)
	ON_BN_CLICKED(IDOK, &SecsGemDiagnosticsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SET_ALARM_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedSetAlarmButton)
	ON_BN_CLICKED(IDC_CLEAR_ALARM_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedClearAlarmButton)
	ON_BN_CLICKED(IDC_REFRESH_ALARM_BUTTON, &SecsGemDiagnosticsDlg::OnBnClickedRefreshAlarmButton)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_GEM_VARIABLES_TYPE_COMBO, &SecsGemDiagnosticsDlg::OnCbnSelchangeGemVariablesTypeCombo)
	ON_BN_CLICKED(IDC_BUTTON2, &SecsGemDiagnosticsDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// SecsGemDiagnosticsDlg message handlers

////////////////////////////////////////////
// Initialize Alarm Management Tab
void SecsGemDiagnosticsDlg::InitializeAlarmManagement()
{
	UpdateData(TRUE);

	// Get All the enabled alarm to the cboAlarms
	_alarmComboBox.ResetContent();

	for (int i = 0; i < theApp.getGEMController().getEnabledAlarms()->Count; i++) {
		CString csEnabledAlarm = (CString)theApp.getGEMController().getEnabledAlarms()->GetItem(i)->GetValue();
		_alarmComboBox.AddString(csEnabledAlarm);
	}
	_alarmComboBox.SetCurSel(0);

	UpdateData(FALSE);
}

////////////////////////////////////////////
// Initialize Gem Variables Tab
void SecsGemDiagnosticsDlg::InitializeGemVariables()
{
	UpdateData(TRUE);

	//Initialize GEM variables according its variables
	_gemVariableComboBox.ResetContent();

	switch(_selectedGemVariableType) {
		case STATUS_VARIABLE:
			for (int k = 0; k < theApp.getGEMController().getAllStatusVariables()->Count; k++) {
				_bstr_t statusVariable(theApp.getGEMController().getAllStatusVariables()->GetItem(k)->GetName());
				_gemVariableComboBox.AddString(statusVariable);
			}

			break;

		case EQUIPMENT_CONSTANT:
			for (int k = 0; k < theApp.getGEMController().getAllEquipmentConstants()->Count; k++) {
				_bstr_t equipmentConstant(theApp.getGEMController().getAllEquipmentConstants()->GetItem(k)->GetName());
				_gemVariableComboBox.AddString(equipmentConstant);
			}

			break;

		case DATA_VARIABLE:
			for (int k = 0; k < theApp.getGEMController().getAllDataVariables()->Count; k++) {
				_bstr_t dataVariable(theApp.getGEMController().getAllDataVariables()->GetItem(k)->GetName());
				_gemVariableComboBox.AddString(dataVariable);
			}

			break;
	}

	_gemVariableValue = "";
	_gemVariableComboBox.SetCurSel(0);

	UpdateData(FALSE);
}

////////////////////////////////////////////
// Initialize Data Collection Tab
void SecsGemDiagnosticsDlg::InitializeDataCollection()
{
	UpdateData(TRUE);
	// Initialize the Disabled Events ComboBox
	int count = 0;
	count = _disabledEventComboBox.GetCount();
	for (int k = 0; k < count; k++) {
		_disabledEventComboBox.DeleteString(0);
	}

	for (int i = 0; i < theApp.getGEMController().getDisabledEvents()->Count; i++) {
		CString cs_DisabledEvent = (CString)theApp.getGEMController().getDisabledEvents()->GetItem(i)->GetValue();
		_disabledEventComboBox.AddString(cs_DisabledEvent);
	}

	_disabledEventComboBox.SetCurSel(0);

	// Initialize the Enabled Events ComboBox
	count = _enabledEventComboBox.GetCount();
	for (int ii = 0; ii < count; ii++) {
		_enabledEventComboBox.DeleteString(0);
	}

	for (int j = 0; j < theApp.getGEMController().getEnabledEvents()->Count; j++) {
		CString cs_EnabledEvent = (CString)theApp.getGEMController().getEnabledEvents()->GetItem(j)->GetValue();
		_enabledEventComboBox.AddString(cs_EnabledEvent);
	}

	_enabledEventComboBox.SetCurSel(0);

	theApp.getGEMController().getAllDataCollectionsFromXml();
	UpdateData(FALSE);
}

////////////////////////////////////////////
// CommunicationStateChanged event handler
void SecsGemDiagnosticsDlg::CommunicationStateUpdate()
{
	if(theApp.getGEMController().getController() == nullptr)
		return;

	UpdateData(TRUE);
	
	switch (theApp.getGEMController().getCommunicationState()) {
		//In this state, SECS-II communication has two substates, COMMUNICATING
		//and NOT COMMUNICATING
		case CommunicationState_Enabled:
			_communicationState = "ENABLED";
			_enableCommunicationButton.SetWindowText("DISABLE");
			controlGUIDisplayItems(TRUE);
			break;
		//In this state, SECS-II communication with a host
		//computer is non-existent
		case CommunicationState_Disabled:
			_communicationState = "DISABLED";
			_enableCommunicationButton.SetWindowText("ENABLE");
			controlGUIDisplayItems(FALSE);
			break;
		//In this state, SECS-II communication, no messages other than S1F13, S1F14, and
		//S9Fx shall be sent
		case CommunicationState_EnabledNotCommunicating:
			_communicationState = "ENABLED/NOT COMMUNICATING";
			_enableCommunicationButton.SetWindowText("DISABLE");
			controlGUIDisplayItems(FALSE);
			break;
		//Communications have been established. Equipment may receive any message from Host,
		//including S1F13
		case CommunicationState_EnabledCommunicating:
			_communicationState = "ENABLED/COMMUNICATING";
			_enableCommunicationButton.SetWindowText("DISABLE");
			controlGUIDisplayItems(TRUE);
			break;
		//An establish communication has been sent
		case CommunicationState_WaitCRA:
			_communicationState = "WAITCRA";
			_enableCommunicationButton.SetWindowText("DISABLE");
			controlGUIDisplayItems(FALSE);
			break;
		//A connection transaction failure has occurred.
		case CommunicationState_WaitDelay:
			_communicationState = "WAITDELAY";
			_enableCommunicationButton.SetWindowText("DISABLE");
			controlGUIDisplayItems(FALSE);
			break;
		//Equipment waits for an S1F13 from the host
		case CommunicationState_WaitCRFromHost:
			_communicationState = "WAITCRFROMHOST";
			_enableCommunicationButton.SetWindowText("DISABLE");
			controlGUIDisplayItems(FALSE);
	}
	
	UpdateData(FALSE);
}

////////////////////////////////////////////
// ControlStateChanged event handler
void SecsGemDiagnosticsDlg::ControlStateUpdate()
{
	if(theApp.getGEMController().getController() == nullptr)
		return;

	UpdateData(TRUE);

	BOOL bRadioButtonLocalRemoteEnable = TRUE;
	/*To Enable Control for this Radio button
	--> Must have:
	1. Communication is ON
	2. Control State is ONLINE
	*/
	CommunicationState CSStatus = theApp.getGEMController().getCommunicationState();
	if (CSStatus != CommunicationState_Enabled && CSStatus != CommunicationState_EnabledCommunicating) {
		bRadioButtonLocalRemoteEnable = FALSE;
	}
	

	switch (theApp.getGEMController().getControlState()) {
		//Operation of the equipment is implemented by direct action of an operator.
		//All operation commands shall be available for input at the local operator
		//Console of the equipment
		case ControlState_OnlineLocal:
			_controlState = "ONLINE/LOCAL";
			_communicationOnlineButton.SetWindowText("GO OFFLINE");
			_selectedControlState = ONLINE_LOCAL;
			_ToolControlState = ONLINE_LOCAL;
			//Operation of the equipment is implemented by direct action of an operator
			theApp.getGEMController().setLocal();
			break;

		//While the REMOTE state is active, the host may operate the equipment to the
		//full extent available through the communications interface
		case ControlState_OnlineRemote:
			_controlState = "ONLINE/REMOTE";
			_communicationOnlineButton.SetWindowText("GO OFFLINE");
			_selectedControlState = ONLINE_REMOTE;
			_ToolControlState = ONLINE_REMOTE;
			break;

		//System maintains the offline state. It awaits operator instructions to attempt to
		//go online
		case ControlState_EquipmentOffline:
			_controlState = "EQUIPMENT OFFLINE";
			_communicationOnlineButton.SetWindowText("GO ONLINE");
			bRadioButtonLocalRemoteEnable = FALSE;
			_ToolControlState = TOOL_OFFLINE;
			break;

		//While HOST OFF-LINE state is active, the operator's intent is that the equipment be 
		//ON-LINE. However, the host has not agree
		case ControlState_HostOffline:
			_controlState = "HOST OFFLINE";
			_communicationOnlineButton.SetWindowText("GO ONLINE");
			bRadioButtonLocalRemoteEnable = FALSE;

			break;
			_ToolControlState = HOST_OFFLINE;
		//The equipment has responded to an operator instruction to attempt to go to the
		//Online state
		case ControlState_AttemptOnline:
			_controlState = "ATTEMPT ONLINE";
			_communicationOnlineButton.SetWindowText("GO ONLINE");
			break;
		
		//Unknown state
		default:
			_controlState = "UNKNOWN";
			_communicationOnlineButton.SetWindowText("GO ONLINE");
			break;
	}

	
	GetDlgItem(IDC_LOCAL_RADIO)->EnableWindow(bRadioButtonLocalRemoteEnable);
	GetDlgItem(IDC_REMOTE_RADIO)->EnableWindow(bRadioButtonLocalRemoteEnable);

	UpdateData(FALSE);
}

////////////////////////////////////////////
// ProcessingStateChanged event handler
void SecsGemDiagnosticsDlg::ProcessingStateUpdate()
{
	if(theApp.getGEMController().getController() == nullptr)
		return;

	UpdateData(TRUE);
	CString processState;
	
	switch (theApp.getGEMController().getProcessingState()) {
		//Equipment is awaiting instructions
		case ProcessingState_Idle:
			processState = "IDLE";
			break;

		//All external conditions neessary for process execution are satisfied, such as
		//ensuring material is present at the equipment, input/output ports are in
		//the proper state, parameters are within limits, etc
		case ProcessingState_Setup:
			processState = "SETUP";
			break;

		//Equipment is ready for process execution and is awaiting a START command
		//from the operator or the host
		case ProcessingState_Ready:
			processState = "READY";
			break;

		//It is the state in which equipment is executing a process program automatically and
		//can continue to do so without external intervention
		case ProcessingState_Executing:
			processState = "EXECUTING";
			break;

		//In this state, processing is suspended and the equipment is awaiting a command
		case ProcessingState_Pause:
			processState = "PAUSE";
			break;
		
		//Unknown processing state
		default:
			processState = "UNKNOWN";
			break;	
	}

	UpdateData(FALSE);
}

////////////////////////////////////////////
// SpoolingStateChanged event handler
void SecsGemDiagnosticsDlg::SpoolingStateUpdate()
{
	if(theApp.getGEMController().getController() == nullptr)
		return;

	UpdateData(TRUE);
	switch (theApp.getGEMController().getSpoolingState()) {
		case SpoolingState_Active:
			_spoolingState = "ACTIVE";
			break;

		case SpoolingState_Inactive:
			_spoolingState = "INACTIVE";
			break;
	}

	switch (theApp.getGEMController().getSpoolingSubState()) {
		case SpoolingSubState_SpoolLoadNotFull:
			_spoolingSubState = "SPOOL LOAD/NOT FULL";
			break;

		case SpoolingSubState_SpoolLoadFull:
			_spoolingSubState = "SPOOL LOAD/FULL";
			break;

		case SpoolingSubState_SpoolUnloadNoOutput:
			_spoolingSubState = "SPOOL UNLOAD/NO OUTPUT";
			break;

		case SpoolingSubState_SpoolUnloadOutputTransmit:
			_spoolingSubState = "SPOOL UNLOAD/OUTPUT TRANSMIT";
			break;

		case SpoolingSubState_SpoolUnloadOutputPurge:
			_spoolingSubState = "SPOOL UNLOAD/OUTPUT PURGE";
			break;

	}
	UpdateData(FALSE);
}

void SecsGemDiagnosticsDlg::updateLogger()
{
	for(size_t i = 0; i < theApp.getGEMController().getTerminalString().size(); ++i) {
		_loggerList.AddString( theApp.getGEMController().getTerminalString().front() );
		theApp.getGEMController().getTerminalString().pop_front();
	}

	UpdateData(FALSE);

	theApp.getGEMController().clearTerminalString();
}

// Enable the Communication (Open listening port)
void SecsGemDiagnosticsDlg::OnBnClickedEnableCommunicationButton()
{
	CString communicationState = "";

	_enableCommunicationButton.GetWindowText(communicationState);

	if(communicationState == "ENABLE")
		//Enable the communication. When this method is called, the initial communication state will
		//be default to Enable/Not Communicating. Once S1F13 is established,  the communication state
		//will be changed to Enable/Communicating
		theApp.getGEMController().setEnable();
	else
		//Disable the communication. At this state, the communication protocol is disable and
		//no connect attempt is allowed
		theApp.getGEMController().setDisable();
}

// Set control state to Online/Offline 
void SecsGemDiagnosticsDlg::OnBnClickedCommunicationOnlineButton()
{
	CString onlineState = "";

	_communicationOnlineButton.GetWindowText(onlineState);

	if(onlineState == "GO ONLINE")
		//S1F1 will automatically sent by GEM controller
		//S1F2 will reply and update the control state to either
		//OnlineLocal or OnlineRemote depending on user setting
		theApp.getGEMController().setOnline();
	else
		//In this state, host can only send a S1F13 and S1F17, any other messages
		//will be aborted by GEM controller automatically
		theApp.getGEMController().setOffline();
}

// Set Control State to Local
void SecsGemDiagnosticsDlg::OnBnClickedLocalRadio()
{
	_selectedControlState = ONLINE_LOCAL;

	//Operation of the equipment is implemented by direct action of an operator
	theApp.getGEMController().setLocal();
}

// Set Control State to Remote
void SecsGemDiagnosticsDlg::OnBnClickedRemoteRadio()
{
	_selectedControlState = ONLINE_REMOTE;

	//Host can exchange all the SECS II transactions with the equipment
	theApp.getGEMController().setRemote();
}


void SecsGemDiagnosticsDlg::OnBnClickedRefreshEventButton()
{
	InitializeDataCollection();
}

/// Enable EventReport
void SecsGemDiagnosticsDlg::OnBnClickedEnableEventButton()
{
	//Get the disabled event to be set to enabled
	CString disabledEvent = "";
	_disabledEventComboBox.GetWindowText(disabledEvent);

	if(disabledEvent != "") {
		theApp.getGEMController().activateEventReport(disabledEvent, true);

		InitializeDataCollection();
	}
	else {
		AfxMessageBox("Error! Please select an Event to enable!", MB_OK | MB_ICONERROR);
	}
}

/// Disable EventReport
void SecsGemDiagnosticsDlg::OnBnClickedDisableEventButton()
{
	//Get the enabled event to be set to enabled
	CString enabledEvent = "";
	_enabledEventComboBox.GetWindowText(enabledEvent);

	if(enabledEvent != "") {
		theApp.getGEMController().activateEventReport(enabledEvent, false);

		InitializeDataCollection();
	}
	else {
		AfxMessageBox("Error! Please select an Event to disable!", MB_OK | MB_ICONERROR);
	}
}

/// Send the selected Enabled EventReport to Host
void SecsGemDiagnosticsDlg::OnBnClickedSendEventButton()
{
	CString str_EnabledEvent = "";
	_enabledEventComboBox.GetWindowText(str_EnabledEvent);
	
	if (str_EnabledEvent != "") {
		theApp.getGEMController().sendEventReport(str_EnabledEvent);
	}
	else {
		AfxMessageBox("Error! Please select an Event to send!", MB_OK | MB_ICONERROR);
	}
}

/// Clear the Logger List
void SecsGemDiagnosticsDlg::OnBnClickedClearLogButton()
{
	_loggerList.ResetContent();
}

/// Get the Status Variable's value from the Tool Model
void SecsGemDiagnosticsDlg::OnBnClickedGetStatusVariableValueButton()
{
	int index = _gemVariableComboBox.GetCurSel();

	CString selectedGemVariable;
	_gemVariableComboBox.GetLBText(index, selectedGemVariable);

	UpdateData(TRUE);

	switch(_selectedGemVariableType) {
		case STATUS_VARIABLE:
			_gemVariableValue = theApp.getGEMController().getStatusVariablesValue(selectedGemVariable);
			break;

		case EQUIPMENT_CONSTANT:
			_gemVariableValue = theApp.getGEMController().getEquipmentConstantValue(selectedGemVariable);
			break;

		case DATA_VARIABLE:
			_gemVariableValue = theApp.getGEMController().getDataVariablesValue(selectedGemVariable);
			break;
	}

	UpdateData(FALSE);
}

/// Set Status Variable value
void SecsGemDiagnosticsDlg::OnBnClickedSetStatusVariableValueButton()
{
	UpdateData(TRUE);
	
	CString selectedStatusVariableName;
	int index = _gemVariableComboBox.GetCurSel();
	_gemVariableComboBox.GetLBText(index, selectedStatusVariableName);

	switch(_selectedGemVariableType) {
		case STATUS_VARIABLE:
			theApp.getGEMController().setStatusVariablesValue(selectedStatusVariableName, _gemVariableValue);
			break;
		
		case EQUIPMENT_CONSTANT:
			theApp.getGEMController().setEquipmentConstantValue(selectedStatusVariableName, _gemVariableValue);
			break;

		case DATA_VARIABLE:
			theApp.getGEMController().setDataVariablesValue(selectedStatusVariableName, _gemVariableValue);
			break;
	}

	UpdateData(FALSE);
}

// Close this dialog
void SecsGemDiagnosticsDlg::OnBnClickedOk()
{
	try {
		//if(theApp.getGEMController().getController() != nullptr)
		//	theApp.getGEMController().getController()->SetDisable();

		KillTimer(GEM_INFORMATION_UPDATE_TIMER);

		theApp.nGEMControlState			= theApp.getGEMController().getControlState();
		theApp.nGEMCommunicationState	= theApp.getGEMController().getCommunicationState();
	}
	catch(_com_error& e) {
		CString errorMessage;
		errorMessage.Format("Error! GEM COM link error.\n Error code: %s - Error Descriptions: %s", e.ErrorMessage(), e.Description());
		AfxMessageBox(errorMessage, MB_OK | MB_ICONERROR);
	}
	catch(...) {
		AfxMessageBox("ERROR! GEM controller is not initialized or get destroyed!", MB_OK | MB_ICONERROR);
	}

	CDialog::OnOK();
}

// Set Alarm S5F1
void SecsGemDiagnosticsDlg::OnBnClickedSetAlarmButton()
{
	CString selectedAlarmName;
	int index = _alarmComboBox.GetCurSel();
	_alarmComboBox.GetLBText(index, selectedAlarmName);

	// Set Alarm occurrence. New alarm occurrence will be added to the
	// status variables "AlarmsSet". This will send S5F1 to host
	theApp.getGEMController().setAlarm( selectedAlarmName );
}

// Clear Alarm S5F1
void SecsGemDiagnosticsDlg::OnBnClickedClearAlarmButton()
{
	CString selectedAlarmName;
	int index = _alarmComboBox.GetCurSel();
	_alarmComboBox.GetLBText(index, selectedAlarmName);

	// Clear Alarm occurrence. The cleared alarm will be removed from
	// status variable "AlarmsSet". This will send S5F1 to host
	theApp.getGEMController().clearAlarm( selectedAlarmName );
}


void SecsGemDiagnosticsDlg::OnBnClickedRefreshAlarmButton()
{
	InitializeAlarmManagement();
}

BOOL SecsGemDiagnosticsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	////if current configuration is not in production, disable communication
	//if(strcmp(theApp.configurationCategories, "Production") != 0)
	//	theApp.getGEMController().setDisable();
	//else 
	//	theApp.getGEMController().setEnable();
	_selectedCommunicateState	= theApp.nGEMCommunicationState;
	_ToolControlState			= theApp.nGEMControlState;

	if(_selectedCommunicateState)
		theApp.getGEMController().setEnable();
	else
		theApp.getGEMController().setDisable();

	SetControlState(_ToolControlState);

	_gemVariableTypeComboBox.AddString("Status Variables");
	_gemVariableTypeComboBox.AddString("Equipment Constants");
	_gemVariableTypeComboBox.AddString("Data Variables");
	_gemVariableTypeComboBox.SetCurSel(0);

	initializeParameters();

	return TRUE;
}


void SecsGemDiagnosticsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	initializeParameters();

	controlGUIDisplayItems(FALSE);
}

void SecsGemDiagnosticsDlg::initializeParameters()
{
	// Initialize Data Collection
	InitializeDataCollection();

	// Initialize GEM Variables
	InitializeGemVariables();

	// Initialize Alarm Management UI
	InitializeAlarmManagement();

	//Initialize timer
	SetTimer(GEM_INFORMATION_UPDATE_TIMER, 50, NULL);
}

//Enable/Disable GUI items while communication state changes to avoid unexpected calls to API
void SecsGemDiagnosticsDlg::controlGUIDisplayItems(BOOL enable)
{
	_disabledEventComboBox.EnableWindow(enable);
	_enabledEventComboBox.EnableWindow(enable);
	_gemVariableTypeComboBox.EnableWindow(enable);
	_gemVariableComboBox.EnableWindow(enable);
	_statusVariableEditControl.EnableWindow(enable);
	_alarmComboBox.EnableWindow(enable);

	GetDlgItem(IDC_ENABLE_EVENT_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_DISABLE_EVENT_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_REFRESH_EVENT_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_SEND_EVENT_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_GET_GEM_VARIABLE_VALUE_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_SET_GEM_VARIABLE_VALUE_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_SET_ALARM_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_CLEAR_ALARM_BUTTON)->EnableWindow(enable);
	GetDlgItem(IDC_REFRESH_ALARM_BUTTON)->EnableWindow(enable);
	/*GetDlgItem(IDC_LOCAL_RADIO)->EnableWindow(enable);
	GetDlgItem(IDC_REMOTE_RADIO)->EnableWindow(enable);*/
}


void SecsGemDiagnosticsDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == GEM_INFORMATION_UPDATE_TIMER) {

		CommunicationStateUpdate();
		ControlStateUpdate();
		ProcessingStateUpdate();
//		SpoolingStateUpdate();

		updateLogger();

		if (theApp.bStartRemoteCmdRcvd && theApp.m_bOnline)
		{
			theApp.bStartRemoteCmdRcvd = FALSE;
			OnBnClickedOk();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void SecsGemDiagnosticsDlg::OnCbnSelchangeGemVariablesTypeCombo()
{
	UpdateData(TRUE);

	InitializeGemVariables();

	UpdateData(FALSE);
}

void SecsGemDiagnosticsDlg::SetControlState(int state)
{
	if (theApp.getGEMController().getController() == nullptr)
		return;

	UpdateData(TRUE);

	GetDlgItem(IDC_LOCAL_RADIO)->EnableWindow(TRUE);
	GetDlgItem(IDC_REMOTE_RADIO)->EnableWindow(TRUE);

	switch (state) {
		//Operation of the equipment is implemented by direct action of an operator.
		//All operation commands shall be available for input at the local operator
		//Console of the equipment

	case ControlState_OnlineLocal:
		_controlState = "ONLINE/LOCAL";
		_communicationOnlineButton.SetWindowText("GO OFFLINE");
		_selectedControlState = ONLINE_LOCAL;
		//Operation of the equipment is implemented by direct action of an operator
		theApp.getGEMController().setLocal();
		break;

		//While the REMOTE state is active, the host may operate the equipment to the
		//full extent available through the communications interface
	case ControlState_OnlineRemote:
		_controlState = "ONLINE/REMOTE";
		_communicationOnlineButton.SetWindowText("GO OFFLINE");
		_selectedControlState = ONLINE_REMOTE;
		theApp.getGEMController().setRemote();
		break;

		//System maintains the offline state. It awaits operator instructions to attempt to
		//go online
	case ControlState_EquipmentOffline:
		_controlState = "EQUIPMENT OFFLINE";
		_communicationOnlineButton.SetWindowText("GO ONLINE");
		{
			GetDlgItem(IDC_LOCAL_RADIO)->EnableWindow(FALSE);
			GetDlgItem(IDC_REMOTE_RADIO)->EnableWindow(FALSE);
		}
		theApp.getGEMController().setOffline();
		break;

		//While HOST OFF-LINE state is active, the operator's intent is that the equipment be 
		//ON-LINE. However, the host has not agree
	case ControlState_HostOffline:
		_controlState = "HOST OFFLINE";
		_communicationOnlineButton.SetWindowText("GO ONLINE");
		{
			GetDlgItem(IDC_LOCAL_RADIO)->EnableWindow(FALSE);
			GetDlgItem(IDC_REMOTE_RADIO)->EnableWindow(FALSE);
		}
		break;

		//The equipment has responded to an operator instruction to attempt to go to the
		//Online state
	case ControlState_AttemptOnline:
		_controlState = "ATTEMPT ONLINE";
		_communicationOnlineButton.SetWindowText("GO ONLINE");
		break;

		//Unknown state
	default:
		_controlState = "UNKNOWN";
		_communicationOnlineButton.SetWindowText("GO ONLINE");
		break;
	}
	UpdateData(FALSE);
}

void SecsGemDiagnosticsDlg::OnBnClickedButton2()
{
	theApp.getGEMController().requestHostDateTime(m_strHostDtateTime);
	SetDlgItemText(IDC_EDIT_HOST_TIME, m_strHostDtateTime);
}

HBRUSH SecsGemDiagnosticsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pWnd == this)
		return m_brush;
	else {
		if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN) {
			pDC->SetBkColor(COLOR_BACKROUND);
			return m_brush;
		}
		return NULL;
	}
}