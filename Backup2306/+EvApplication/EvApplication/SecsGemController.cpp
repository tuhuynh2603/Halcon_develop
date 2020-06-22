/// Original: SecsGemController class for Insphere SecsToHost SDK.
/// Udupa; Apr'2017: Needs license to compile. Hence, added preprocessor directives to exclude SecsGem calls if license is not available

#include "StdAfx.h"
#include "Application.h"
#include "SecsGemController.h"
#include <algorithm>
#include "DebugLog.h"
#include "stdafx.h"
#import <msxml3.dll>
#include <iostream>
#include <fstream>
#include "ImageSelectionDlg.h"


using namespace MSXML2;
std::string extern ConvertWCSToMBS(const wchar_t* pstr, long wslen)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

	std::string dblstr(len, '\0');
	len = ::WideCharToMultiByte(CP_ACP, 0 /* no flags */,
		pstr, wslen /* not necessary NULL-terminated */,
		&dblstr[0], len,
		NULL, NULL /* no default char */);

	return dblstr;
}

std::string ConvertBSTRToMBS(BSTR bstr)
{
	int wslen = ::SysStringLen(bstr);
	return ConvertWCSToMBS((wchar_t*) bstr, wslen);
}

SecsGemController::SecsGemController() 
	: _enableTerminalDisplay(false)
{
}


SecsGemController::~SecsGemController(void)
{
}

int SecsGemController::initializeGem(bool reInitializtion)
{
	// Initialize SecsToHost.Net
	if(reInitializtion)
		return this->ReInitializeSecsToHost();
	else
		return this->InitializeSecsToHost();
}

void SecsGemController::setPath(CString equipmentModelPath, CString gemLogPath)
{
	_equipmentModelPath = equipmentModelPath;
	_gemLogPath			= gemLogPath;
}

ICOMGEMControllerPtr SecsGemController::getController() const
{
#ifndef _NO_SECSGEM
	return m_GEMController;
#else
	return nullptr;
#endif
}

ICOMMessageServiceManagerPtr SecsGemController::getServiceManager() const
{
#ifndef _NO_SECSGEM
	return getController()->GetServices();
#else
	return nullptr;
#endif

}

CommunicationState SecsGemController::getCommunicationState() const
{
#ifndef _NO_SECSGEM
	return getController()->GetCommunicationState();
#else
	return (CommunicationState) 0;
#endif
}

ControlState SecsGemController::getControlState() const
{
#ifndef _NO_SECSGEM
	return getController()->GetControlState();
#else
	return (ControlState) 0;
#endif
}

ProcessingState	SecsGemController::getProcessingState() const
{
#ifndef _NO_SECSGEM
	return getController()->GetProcessingState();
#else
	return (ProcessingState) 0;
#endif
}

ICOMSECsDataItemPtr SecsGemController::getSetAlarms() const
{
#ifndef _NO_SECSGEM
	return getController()->GetSetAlarms();
#else
	return nullptr;
#endif
}

ICOMSECsDataItemPtr SecsGemController::getEnabledAlarms() const
{
#ifndef _NO_SECSGEM
	return getController()->GetEnabledAlarms();
#else
	return nullptr;
#endif
}

ICOMSECsDataItemPtr SecsGemController::getDisabledEvents() const
{
#ifndef _NO_SECSGEM
	return getController()->GetDisabledEvents();
#else
	return nullptr;
#endif
}

ICOMSECsDataItemPtr SecsGemController::getEnabledEvents() const
{
#ifndef _NO_SECSGEM
	return getController()->GetEnabledEvents();
#else
	return nullptr;
#endif
}

ICOMSECsDataItemPtr SecsGemController::getAllStatusVariables() const
{
#ifndef _NO_SECSGEM
	return getController()->GetAllStatusVariables();
#else
	return nullptr;
#endif
}

ICOMSECsDataItemPtr SecsGemController::getAllEquipmentConstants() const
{
#ifndef _NO_SECSGEM
	return getController()->GetAllEquipmentConstants();
#else
	return nullptr;
#endif
}

ICOMSECsDataItemPtr SecsGemController::getAllDataVariables() const
{
#ifndef _NO_SECSGEM
	return getController()->GetAllDataVariables();
#else
	return nullptr;
#endif
}

SpoolingState SecsGemController::getSpoolingState() const
{
#ifndef _NO_SECSGEM
	return getController()->GetSpoolingState();
#else
	return (SpoolingState) 0;
#endif
}

SpoolingSubState SecsGemController::getSpoolingSubState() const
{
#ifndef _NO_SECSGEM
	return getController()->GetSpoolingSubState();
#else
	return (SpoolingSubState) 0;
#endif
}

void SecsGemController::activateEventReport(CString eventName, bool enable)
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

	getController()->ActivateEventReport_2(_bstr_t( eventName ), enable);
}

void SecsGemController::sendEventReport(CString eventName)
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	if(eventName == "")
		return;

	//This code is Sending an event via S6F11 to the Host application
	//The event "LotCompleted" is modeled in the tool model which has an associated CEID
	ICOMSECsMessagePtr p_Message = getServiceManager()->DataCollection->EventReportSend_2(_bstr_t( eventName ) );

	// Send only don't wait for reply
	ICOMSECsMessagePtr p_Reply = getController()->Send(p_Message);
	appendTerminalString(eventName);

	//if(p_Reply != nullptr) {
	//	if(p_Reply->DataItem->GetItem(0)->ToInt() == 0) {
	//		OutputDebugLogTo(9, TRUE, "SECS/GEM: EventReport:: %s is sent sucessfully!", eventName);
	//		appendTerminalString("Event: is sent successfully!");
	//	}
	//	else {
	//		OutputDebugLogTo(9, TRUE, "SECS/GEM: EventReport:: %s is not sent sucessfully!", eventName);
	//		appendTerminalString("Event: is not sent successfully!");
	//	}
	//}
	//else {
	//	OutputDebugLogTo(9, TRUE, "SECS/GEM: EventReport:: No reply received for %s!", eventName);
	//	appendTerminalString("No reply received!");
	//}
#endif
}

void SecsGemController::requestHostDateTime(CString &hostDateTime)
{
	ICOMSECsMessagePtr p_DateTimeMessage = getServiceManager()->GetControlDiagnostic()->DateTimeRequest();
	ICOMSECsMessagePtr p_Reply = getController()->Send(p_DateTimeMessage);
}

void SecsGemController::setAlarm(CString alarmName)
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	// Set Alarm occurrence. New alarm occurrence will be added to the
	// status variables "AlarmsSet". This will send S5F1 to host
	this->getController()->SetAlarm(_bstr_t( alarmName ));
#endif
}

void SecsGemController::clearAlarm(CString alarmName)
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	// Clear Alarm occurrence. The cleared alarm will be removed from
	// status variable "AlarmsSet". This will send S5F1 to host
	getController()->ClearAlarm(_bstr_t( alarmName ));
#endif
}

void SecsGemController::setLocal()
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	try {
		//Operation of the equipment is implemented by direct action of an operator
		getController()->SetLocal();
		theApp.bIsGEMControllerRemoteMode = FALSE;
		theApp.nGEMControlState = getController()->GetControlState();
	}
	catch(...) {
		//OutputDebugLogTo(9, TRUE, "SECS/GEM: setLocal() Exception!");
	}
#endif
}

void SecsGemController::setRemote()
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	try {
		//Host can exchange all the SECS II transactions with the equipment
		getController()->SetRemote();
		theApp.bIsGEMControllerRemoteMode = TRUE;
	}
	catch(...) {
		//OutputDebugLogTo(9, TRUE, "SECS/GEM: setRemote() Exception!");
	}
#endif
}

void SecsGemController::setOnline()
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	try {
		//S1F1 will automatically sent by GEM controller
		//S1F2 will reply and update the control state to either
		//OnlineLocal or OnlineRemote depending on user setting
		getController()->SetOnline();
		sendEventReport("EquipmentONLINE");
	}
	catch(...) {
		//OutputDebugLogTo(9, TRUE, "SECS/GEM: setOnline() Exception!");
	}
#endif
}

void SecsGemController::setOffline()
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	//If control state is in equipment offline state, do not set it to offline again
	if(getControlState() == ControlState_EquipmentOffline)
		return;

	try {
		//In this state, host can only send a S1F13 and S1F17, any other messages
		//will be aborted by GEM controller automatically
		getController()->SetOffline();
		sendEventReport("EquipmentOFFLINE");
	}
	catch(...) {
		//OutputDebugLogTo(9, TRUE, "SECS/GEM: setOffline() Exception!");
	}
#endif
}

void SecsGemController::setEnable()
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	//If communication is enabled, do not set it to enable again
	if(getCommunicationState() == CommunicationState_Enabled)
		return;

	try {
		//Enable the communication. When this method is called, the initial communication state will
		//be default to Enable/Not Communicating. Once S1F13 is established,  the communication state
		//will be changed to Enable/Communicating
		getController()->SetEnable();
	}
	catch(...) {
		//OutputDebugLogTo(9, TRUE, "SECS/GEM: SetEnable() Exception!");
	}
#endif
}

void SecsGemController::setDisable()
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	//If communication is disabled, do not set it to disable again
	if(getCommunicationState() == CommunicationState_Disabled)
		return;

	try {
		//Disable the communication. At this state, the communication protocol is disable and
		//no connect attempt is allowed
		getController()->SetDisable();
	}
	catch(...) {
		//OutputDebugLogTo(9, TRUE, "SECS/GEM: SetDisable() Exception!");
	}
#endif
}

void SecsGemController::setStatusVariablesValue(const CString name, const CString value)
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

	if (name == "ProcessState" && theApp.processState != atoi((char*)(LPCTSTR)value))
	{
		int state = atoi((char*)(LPCTSTR)value);
		CString prevState;
		theApp.previousProcessState = theApp.processState;
		theApp.processState = state;
		prevState.Format("%d", theApp.previousProcessState);

#ifndef _NO_SECSGEM
		try {
			this->getController()->SetAttribute(_bstr_t("PreviousProcessState"), AttributeType_SV, _bstr_t(prevState));
		}
		catch (...) {
			OutputDebugLogTo(9, TRUE, "SECS/GEM: Failed to set Previous Process State");
		}
#endif
	}
	else if (name == "ProcessState" && theApp.processState == atoi((char*)(LPCTSTR)value))
	{
		return;
	}

#ifndef _NO_SECSGEM
	try {
		this->getController()->SetAttribute(_bstr_t( name ), AttributeType_SV, _bstr_t( value ));
	}
	catch(...) {
		OutputDebugLogTo(9, TRUE, "SECS/GEM: setStatusVariablesValue() Exception!");
	}
#endif
}

CString SecsGemController::getStatusVariablesValue(const CString name)
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return "";
	}

#ifndef _NO_SECSGEM
	_bstr_t bstrStatusVariableValue(this->getController()->GetAttribute(_bstr_t( name ), AttributeType_SV));
	return (LPCSTR)bstrStatusVariableValue;
#else
	return "";
#endif
}

void SecsGemController::setDataVariablesValue(const CString name, const CString value)
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	try {
		this->getController()->SetAttribute(_bstr_t( name ), AttributeType_DV, _bstr_t( value ));
	}
	catch(...) {
		OutputDebugLogTo(9, TRUE, "SECS/GEM: setDataVariablesValue() Exception!");
	}
#endif
}

CString SecsGemController::getDataVariablesValue(const CString name)
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return "";
	}

#ifndef _NO_SECSGEM
	_bstr_t bstrDataVariableValue(this->getController()->GetAttribute(_bstr_t( name ), AttributeType_DV));
	return (LPCSTR)bstrDataVariableValue;
#endif
}

void SecsGemController::setEquipmentConstantValue(const CString name, const CString value)
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return;
	}

#ifndef _NO_SECSGEM
	try {
		this->getController()->SetAttribute(_bstr_t( name ), AttributeType_EC, _bstr_t( value ));
	}
	catch(...) {
		OutputDebugLogTo(9, TRUE, "SECS/GEM: setEquipmentConstantValue() Exception!");
	}
#endif
}

CString SecsGemController::getEquipmentConstantValue(CString name)
{
	if(getController() == nullptr) {
		appendTerminalString("ERROR! GEM controller is not initialized!");
		return "";
	}

#ifndef _NO_SECSGEM
	_bstr_t bstrEquipmentConstantValue(this->getController()->GetAttribute(_bstr_t( name ), AttributeType_EC));
	return (LPCSTR)bstrEquipmentConstantValue;
#endif
}

std::list< CString >& SecsGemController::getTerminalString()
{
	return _terminalString;
}

void SecsGemController::appendTerminalString(CString info)
{
	theApp.AddLogEvent(info, true, true);

	if(!_enableTerminalDisplay)
		return;

	_terminalString.push_back(info);
}

void SecsGemController::clearTerminalString()
{
	std::for_each(_terminalString.begin(), _terminalString.end(), 
				  [](CString value)
				  {
				  	 value.Empty();
				  	 ASSERT(value.GetLength() == 0);
				  } );

	_terminalString.clear();
}

void SecsGemController::activateTerminalInformationDisplay(bool enable)
{
	_enableTerminalDisplay = enable;
}

////////////////////////////////////////////
// Initialize the SecsToHost.Net 
int SecsGemController::InitializeSecsToHost()
{
	CString strMsg;

	if(getController() != nullptr)
	{
		return -2;
	}
#ifndef _NO_SECSGEM
	/////////////////////////////////////////////////////////////////////////////
	// Get the ICOMGEMControllerPtr reference
	CGEMController* p_GEMController = new CGEMController();

	m_GEMController = p_GEMController->GetGEMController();

	try {
		/////////////////////////////////////////////////////////////////////////////
		// Register all callbacks
		// Whenever Host application initiates a request message to equipment, the message will be notified in this event
		p_GEMController->CSECsPrimaryInEventCallback.push_back(CreateClosure(this, &SecsGemController::CSECsPrimaryInEventHandler));
		
		// This event carried the reply send asynchronously by the Host
		p_GEMController->CSECsSecondaryInEventCallback.push_back(CreateClosure(this, &SecsGemController::CSECsSecondaryInEventHandler));
		
		// This event notifies the equipment application about the activity performed by the GEM controller layer
		p_GEMController->CMessageActivityTraceEventCallback.push_back(CreateClosure(this, &SecsGemController::CMessageActivityTraceEventHandler));
		
		// Inform the application about communication state change trasition
		p_GEMController->CCommunicationStateChangedEventCallback.push_back(CreateClosure(this, &SecsGemController::CCommunicationStateChangedEventHandler));
		
		// Inform the application about the machine control state change
		p_GEMController->CControlStateChangedEventCallback.push_back(CreateClosure(this, &SecsGemController::CControlStateChangedEventHandler));
		
		// Inform the application about the machine processing state change
		p_GEMController->CProcessingStateChangedEventCallback.push_back(CreateClosure(this, &SecsGemController::CProcessingStateChangedEventHandler));
		
		// Inform the application about the spooling state change
		p_GEMController->CSpoolingStateChangedEventCallback.push_back(CreateClosure(this, &SecsGemController::CSpoolingStateChangedEventHandler));
		
		// Inform the application about the system clock change by Host via S2F31/F32
		p_GEMController->CClockChangedEventCallback.push_back(CreateClosure(this, &SecsGemController::CClockChangedEventHandler));
		
		// Inform the application for the RemoteCommand (S2F21, S2F41, S2F49) sent by Host
		p_GEMController->CRemoteCommandInEventCallback.push_back(CreateClosure(this, &SecsGemController::CRemoteCommandInEventHandler));

		/////////////////////////////////////////////////////////////////////////////
		// Initialize the GEMController object 
		getController()->Initialize(bstr_t(_equipmentModelPath), bstr_t(_gemLogPath));

		if (theApp.nGEMControlState == 4)
			setLocal();
		else if (theApp.nGEMControlState == 5)
			setRemote();
		else
			setOffline();

		return 0;
	} 
	catch (_com_error &e)
	{
		LPCTSTR lpErrorMessage = e.ErrorMessage();
		LPCTSTR lpErrorDescription = e.Description();

		strMsg.Format("SECSGEM Initialization Error1 = %s Error2 = %s", lpErrorMessage, lpErrorDescription);
		if (strMsg.Find("No License Found") < 0) {
			AfxMessageBox(strMsg);
			//		OutputDebugLogTo(9, TRUE, "SECS/GEM: Gem Initialization error! Please ensure GEM parameters and model builder path are corrected!\n Error Code: %s - Error Descriptions: %s", lpErrorMessage, lpErrorDescription);
			appendTerminalString("SECS/GEM: Gem Initialization error! Please ensure GEM parameters and model builder path are corrected!");
		}
		
		return -1;
	}
#endif
}

int SecsGemController::ReInitializeSecsToHost()
{
#ifndef _NO_SECSGEM
//	this.gemController.EquipmentModel.Persistence = true;
	getController()->EquipmentModel->Persistence = true;
	try
	{
		// Reinitialize the changes to the EquipmentModel object
		getController()->GetEquipmentModel()->GetGemConnection()->GetHSMS()->localIPAddress = bstr_t(theApp.strEquipLocalAddress);
		getController()->GetEquipmentModel()->GetGemConnection()->GetHSMS()->localPortNumber = theApp.nEquipLocalPortID;
		getController()->GetEquipmentModel()->GetGemConnection()->GetHSMS()->remoteIPAddress = bstr_t(theApp.strEquipRemoteAddress);
		getController()->GetEquipmentModel()->GetGemConnection()->GetHSMS()->remotePortNumber = theApp.nEquipRemotePortID;
		getController()->GetEquipmentModel()->GetGemConnection()->deviceId = theApp.nEquipDeviceID;
		getController()->GetEquipmentModel()->GetGemConnection()->maxOpenTransactions = theApp.nMaxMultiOpenTransactions;
		getController()->GetEquipmentModel()->GetGemConnection()->multipleOpen = theApp.bEnableMultiOpenTransaction;

//		gemController.EquipmentModel.GemConnection.HSMS.localPortNumber = 5000;

		getController()->PersistEquipmentModel();
		getController()->Disconnect();
		getController()->ReInitialize(bstr_t(_gemLogPath));

//		gemController.PersistEquipmentModel();
//		gemController.Disconnect();
//		gemController.ReInitialize(@"C:\LogFiles");
//		Logger("Successfully reinitialze the hostController");
	}
	catch (_com_error &e)
	{
		LPCTSTR lpErrorMessage = e.ErrorMessage();
		LPCTSTR lpErrorDescription = e.Description();

		OutputDebugLogTo(9, TRUE, "SECS/GEM: Gem ReInitialization error! Please ensure GEM parameters and model builder path are corrected!\n Error Code: %s - Error Descriptions: %s", lpErrorMessage, lpErrorDescription);
		return -1;
	}
#endif
	return 0;
}

////////////////////////////////////////////
// Terminal Display Handler
void SecsGemController::HandleTerminalDisplay(ICOMSECsPrimaryInEventArgs& args) {

#ifndef _NO_SECSGEM
	ICOMSECsMessagePtr p_Message = args.GetInputs();

	// S10F3
	//	<L
	//		<B TID=0>
	//		<A TEXT=Message from Host>
	//	>
	CString messageDetails = "";
	messageDetails.Format(_T("P: S%dF%d"),(long)p_Message->GetStream(), (long)p_Message->GetFunction());

	long lTransactionId = args.GettransactionId();
	CString iTID;
	iTID = (CString)p_Message->DataItem->GetItem(0)->GetItem(0)->GetValue();

	CString csText;
	csText = (CString)p_Message->DataItem->GetItem(0)->GetItem(1)->GetValue();
	
//	theApp.m_strHostMessageRecvd = csText;
	if (!theApp.m_bTerminalService)
	{
		theApp.OnSecsGemTerminalService();
	}

	theApp.m_pTerminalServiceDlg->m_strHostMessage = csText;
	theApp.m_pTerminalServiceDlg->UpdateData(FALSE);

	appendTerminalString(csText);

	// Send S10F4 to acknowledge Host
	ICOMSECsMessagePtr p_Reply = getServiceManager()->TerminalDisplay->TerminalDisplayAcknowledge(0);
	this->getController()->SendReply(p_Reply, lTransactionId);

#endif
}

void SecsGemController::HandleProcessProgramRequest(ICOMSECsPrimaryInEventArgs& args) {
#ifndef _NO_SECSGEM
                
	if (!theApp.RecipeFiles.size())
		theApp.ListAllProcessRecipes();

	bool recipeFound = false;
	CString strRecipe = "";
	ICOMSECsMessagePtr input = args.GetInputs();

	//Retrieving the requested recipe file	
	CString ppId = (CString)input->GetDataItem()->GetItem_2("PPID")->GetValue();
	CString recipeDir = ppId.Left(ppId.GetLength() /*- 4*/);
	CString recipeName = ppId.Right(ppId.GetLength() - ppId.FindOneOf("\\") - 1);
	CString folderName = recipeName.Left(recipeName.GetLength() /*- 4*/);

	// Check whether this recipe available in directory
	for (int i = 0; i < theApp.RecipeFiles.size(); i++)
	{
		strRecipe = theApp.RecipeFiles.at(i);
		if (recipeDir.CompareNoCase(strRecipe) == 0)
		{
			recipeFound = true;
			break;
		}
	}

	if (recipeFound){
		CString recipeFileName = theApp.m_strMainCfgDir + "\\" + theApp.m_strMainCfgModeDir + "\\" + folderName + "\\" + recipeName;


		SAFEARRAY * _result = getController()->ReadBinaryFromFile(bstr_t(recipeFileName));
		ICOMSECsMessagePtr recipeToReply = getServiceManager()->ProcessProgram->ProcessProgramData_2(bstr_t(ppId), _result);

		// Try to peak what is stored in the ppbody
		SAFEARRAY * _out = recipeToReply->DataItem->GetItem(0)->GetItem(1)->ToBytes();
 		getController()->SendReply(recipeToReply, args.transactionId);
	}
	else{
//		SAFEARRAY * _result = NULL;
		ICOMSECsMessagePtr recipeToReply = getServiceManager()->ProcessProgram->ProcessProgramData_2("", NULL);
		getController()->SendReply(recipeToReply, args.transactionId);
	}

#endif
}

void SecsGemController::OnHostRecipeDirList(ICOMSECsPrimaryInEventArgs& args) {
#ifndef _NO_SECSGEM

	ICOMSECsDataItemPtr p_recipeList = m_GEMController->GetAllStatusVariables();
	p_recipeList->Clear();
//	p_recipeList->Add("Test1.rcp", "Test1.rcp", SECsFormat_Ascii);
//	p_recipeList->Add("Test2.rcp", "Test2.rcp", SECsFormat_Ascii);
	theApp.ListAllProcessRecipes();
	// Check whether this recipe available in directory
	for (int i = 0; i < theApp.RecipeFiles.size(); i++)
	{
		CString strRecipe = theApp.RecipeFiles.at(i);
		p_recipeList->Add(bstr_t(strRecipe), bstr_t(strRecipe), SECsFormat_Ascii);
	}

	ICOMSECsMessagePtr recipeToReply = getServiceManager()->ProcessProgram->CurrentEPPDReply(p_recipeList);
	getController()->SendReply(recipeToReply, args.transactionId);

#endif
}

void SecsGemController::HandleNewEquipmentConstantRequest(ICOMSECsPrimaryInEventArgs& args)
{
#ifndef _NO_SECSGEM
	ICOMSECsMessagePtr p_Message = args.GetInputs();

	//When primary message comes in, the "SECsPrimaryInEventArgs.Input.NeedReply" carries
	//the flag of whether the requested message needs a reply or not.
	//The code here show how to send secondary reply back to host
	//The flows of this code is Host application trying to change certain equipment constants via
	//S2F15. In this case, the equipment application needs to update the equipment constants with the
	//value requested by Host and send the acknowledgement back to host
	for(int i = 0; i < p_Message->DataItem->GetItem_2("Ln")->GetCount(); ++i) {
		CString equipmentConstantName  = (CString) p_Message->DataItem->GetItem_2("Ln")->GetItem(i)->GetItem(0)->GetValue();
		long equipmentConstantId = (long)p_Message->DataItem->GetItem_2("Ln")->GetItem(i)->GetItem(0)->GetValue();
		CString equipmentConstantValue = (CString) p_Message->GetDataItem()->GetItem_2("Ln")->GetItem(i)->GetItem(1)->GetValue();

		//update the value of the request EC
		getController()->SetAttribute(bstr_t(equipmentConstantName), AttributeType_EC, bstr_t(equipmentConstantValue));

		UpdateEquipmentConstantValue(equipmentConstantId, equipmentConstantValue);
	}

	theApp.saveAllParameters();

	//When sending reply to Host, primary message's transaction ID need to pass in as arguments so that GEM controller
	//can correctly retrieve the transaction ID and send reply to the Host
	ICOMSECsMessagePtr reply = getServiceManager()->ControlDiagnostic->NewEquipmentConstantAcknowledge(0);
	getController()->SendReply(reply, args.transactionId);
#endif
}

void SecsGemController::UpdateEquipmentConstantValue(long Id, CString value)
{
	IEquipmentModelPtr oEquipmentModel = getController()->GetEquipmentModel();
	IEquipmentConstantsModelPtr oEquipmentConstants = oEquipmentModel->GetEquipmentConstants();

	for (long i = 0; i <oEquipmentConstants->Count; i++) 
	{
		IECIDModelPtr oECID = oEquipmentConstants->GetItem(i);

		long lEcId = oECID->Getid();
		if (Id == lEcId)
		{
			CString sEcLogicalName = oECID->GetlogicalName();
			SECSFormats oSvFormat = oECID->GetvalueType();

			theApp.ChangeEquipmentConstantValue(Id, sEcLogicalName, oSvFormat, value);

			break;
		}
	}
}

void SecsGemController::HandleProcessProgramLoadInquire(ICOMSECsPrimaryInEventArgs& args)
{
#ifndef _NO_SECSGEM
	ICOMSECsMessagePtr p_S7F2Reply = getServiceManager()->ProcessProgram->ProcessProgramLoadGrant(0);
	getController()->SendReply(p_S7F2Reply, args.transactionId);
#endif
}

/////////////////////////////////////////////////
// Get all the status variables from the EquipmentModel
void SecsGemController::getAllStatusVariablesFromXml()
{
#ifndef _NO_SECSGEM
	IEquipmentModelPtr oEquipmentModel = getController()->GetEquipmentModel();
	IStatusVariablesModelPtr oStatusVariables = oEquipmentModel->GetStatusVariables();

	for (long i=0; i<oStatusVariables->Count; i++) {
		ISVIDModelPtr oSVID = oStatusVariables->GetItem(i);
		
		long lSvId = oSVID->Getid();
		_bstr_t sSvLogicalName = oSVID->GetlogicalName();

		_bstr_t sSvUnits = oSVID->Getunits();
		SECSFormats oSvFormat = oSVID->GetvalueType();
	}

#endif
}

/////////////////////////////////////////////////
// Get all the equipment constants from the EquipmentModel
void SecsGemController::getAllEquipmentConstantsFromXml()
{	
#ifndef _NO_SECSGEM
	IEquipmentModelPtr oEquipmentModel = getController()->GetEquipmentModel();
	IEquipmentConstantsModelPtr oEquipmentConstants = oEquipmentModel->GetEquipmentConstants();

	for (long i=0; i<oEquipmentConstants->Count; i++) {
		IECIDModelPtr oECID = oEquipmentConstants->GetItem(i);
		
		long lEcId = oECID->Getid();
		_bstr_t sEcLogicalName = oECID->GetlogicalName();

		_bstr_t sEcUnits = oECID->Getunits();
		SECSFormats oEcFormat = oECID->GetvalueType();

		_bstr_t sEcMin = oECID->Getmin();

		_bstr_t sEcMax = oECID->Getmax();
	}

#endif
}

/////////////////////////////////////////////////
// Get all the data collections from the EquipmentModel
void SecsGemController::getAllDataCollectionsFromXml()
{
#ifndef _NO_SECSGEM
	IEquipmentModelPtr oEquipmentModel = getController()->GetEquipmentModel();

	IDataCollectionsModelPtr oDataCollection = oEquipmentModel->GetDataCollections();

	ICEIDsModelPtr ceids = oDataCollection->GetCEIDs();

	for (int i = 0; i < ceids->Count; i++) {
		ICEIDModelPtr ceid = ceids->GetItem(i);
		
		_bstr_t sCeLogicalName = ceid->GetlogicalName();
	}
#endif
}

////////////////////////////////////////////
// PrimaryIn event handler
void SecsGemController::CSECsPrimaryInEventHandler(CGEMController &sender, ICOMSECsPrimaryInEventArgs &args) 
{
#ifndef _NO_SECSGEM
	ICOMSECsMessagePtr p_Message = args.GetInputs();

	switch(args.EventId) {
		case PrimaryEventType_NewEquipmentConstantRequest:{
			HandleNewEquipmentConstantRequest(args);
			break;
														  }
		case PrimaryEventType_RemoteCommand:	//Host send remote command
			//OnRemoteCommand(e);				from C# code
			break;
		
		case PrimaryEventType_ProcessProgramLoadInquire:
			HandleProcessProgramLoadInquire(args);
			break;
		
		case PrimaryEventType_ProcessProgramSend: //Host download recipe to equipment
			OnHostRecipeDownload(args);
			break;
		
		case PrimaryEventType_CurrentEPPDRequest: //List All recipes
			OnHostRecipeDirList(args);
			break;
		
		case PrimaryEventType_ProcessProgramRequest:
			HandleProcessProgramRequest(args);
			break;

		case PrimaryEventType_DeleteProcessProgramRequest :
			HandleProcessProgramDeleteRequest(args);
			break;

		case PrimaryEventType_TerminalDisplay: 
			HandleTerminalDisplay(args);
			break;

		case PrimaryEventType_CustomMessageRequest:
			OnCustomMessageRequest(args);
			break;
	}
#endif
}

////////////////////////////////////////////
// SecondaryIn event handler
void SecsGemController::CSECsSecondaryInEventHandler(CGEMController &sender, ICOMSECsSecondaryInEventArgs &args)
{
#ifndef _NO_SECSGEM
	CString message;
	switch (args.EventId) {
		case SecondaryEventType_ProcessProgramSendReply: // S7F4 Reply
			message.Format("Process Program Send Ack: %s", CString( args.GetInputs()->GetDataItem()->GetItem_2("ACKC7")->GetValue()) );
			//OutputDebugLogTo(9, TRUE, "SECS/GEM: %s", message);
			break;
		case SecondaryEventType_MapDataType2Download:
			//OnWaferMapDownload(args);
			break;

		case SecondaryEventType_CustomMessageReply:	//Custom message reply/acknowledgement
			OnCustomMessageReply(args);
			break;
	}
#endif
}

////////////////////////////////////////////
// CommunicationStateChanged event handler
void SecsGemController::CCommunicationStateChangedEventHandler(CGEMController &sender, ICOMSECsEventArgs &args)
{
	CString communicationState;

	switch (getCommunicationState()) {
		//In this state, SECS-II communication has two substates, COMMUNICATING
		//and NOT COMMUNICATING
		case CommunicationState_Enabled:
			communicationState = "ENABLED";
			break;
		//In this state, SECS-II communication with a host
		//computer is non-existent
		case CommunicationState_Disabled:
			communicationState = "DISABLED";
			break;

		//In this state, SECS-II communication, no messages other than S1F13, S1F14, and
		//S9Fx shall be sent
		case CommunicationState_EnabledNotCommunicating:
			communicationState = "ENABLED/NOT COMMUNICATING";
			break;
		//Communications have been established. Equipment may receive any message from Host,
		//including S1F13
		case CommunicationState_EnabledCommunicating:
			communicationState = "ENABLED/COMMUNICATING";
			break;
		//An establish communication has been sent
		case CommunicationState_WaitCRA:
			communicationState = "WAITCRA";
			break;
		//A connection transaction failure has occurred.
		case CommunicationState_WaitDelay:
			communicationState = "WAITDELAY";
			break;
		//Equipment waits for an S1F13 from the host
		case CommunicationState_WaitCRFromHost:
			communicationState = "WAITCRFROMHOST";
	}
}

////////////////////////////////////////////
// ControlStateChanged event handler
void SecsGemController::CControlStateChangedEventHandler(CGEMController &sender, ICOMSECsEventArgs &args)
{
	CString controlState;
	switch (getControlState()) {
		//Operation of the equipment is implemented by direct action of an operator.
		//All operation commands shall be available for input at the local operator
		//Console of the equipment
		case ControlState_OnlineLocal:
			controlState = "ONLINE/LOCAL";
			break;

		//While the REMOTE state is active, the host may operate the equipment to the
		//full extent available through the communications interface
		case ControlState_OnlineRemote:
			controlState = "ONLINE/REMOTE";
			break;

		//System maintains the offline state. It awaits operator instructions to attempt to
		//go online
		case ControlState_EquipmentOffline:
			controlState = "EQUIPMENT OFFLINE";
			break;

		//While HOST OFF-LINE state is active, the operator's intent is that the equipment be 
		//ON-LINE. However, the host has not agree
		case ControlState_HostOffline:
			controlState = "HOST OFFLINE";
			break;

		//The equipment has responded to an operator instruction to attempt to go to the
		//Online state
		case ControlState_AttemptOnline:
			controlState = "ATTEMPT ONLINE";
			break;
		
		//Unknown state
		default:
			controlState = "UNKNOWN";
			break;
	}
}

////////////////////////////////////////////
// ProcessingStateChanged event handler
void SecsGemController::CProcessingStateChangedEventHandler(CGEMController &sender, ICOMSECsEventArgs &args)
{
	CString processState;
	switch (getProcessingState()) {
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

	CString message;
	message.Format("Processing state changed to %s", processState);
	appendTerminalString(message);
}

////////////////////////////////////////////
// MessageActivityTrace event handler
void SecsGemController::CMessageActivityTraceEventHandler(CGEMController &sender, ICOMSECsMessageActivityEventArgs &args)
{
	CString csSeverity;
	Severity severity = args.GetSeverity();
	
	switch(severity) {
		case Severity_Detail:
			csSeverity="Detail";
			break;

		case Severity_Error:
			csSeverity="Error";
			break;
		
		case Severity_Info:
			csSeverity="Info";
			break;

		case Severity_Warning:
			csSeverity="Warning";
			break;

	}
	
	CString csMessage;
	csMessage.Format(_T("%s"), (LPCTSTR)args.Getmessage());
	
	// Format the message
	CString csFullMessage;
	csFullMessage.Append(csSeverity + ": ");
	csFullMessage.Append(csMessage);
	
	appendTerminalString(csFullMessage);
}

////////////////////////////////////////////
// SpoolingStateChanged event handler
void SecsGemController::CSpoolingStateChangedEventHandler(CGEMController &sender, ICOMSECsEventArgs &args)
{
	CString spoolingState;
	CString spoolingSubState;

	switch (getSpoolingState()) {
		case SpoolingState_Active:
			spoolingState = "ACTIVE";
			break;

		case SpoolingState_Inactive:
			spoolingState = "INACTIVE";
			break;
	}

	switch (getSpoolingSubState()) {
		case SpoolingSubState_SpoolLoadNotFull:
			spoolingSubState = "SPOOL LOAD/NOT FULL";
			break;

		case SpoolingSubState_SpoolLoadFull:
			spoolingSubState = "SPOOL LOAD/FULL";
			break;

		case SpoolingSubState_SpoolUnloadNoOutput:
			spoolingSubState = "SPOOL UNLOAD/NO OUTPUT";
			break;

		case SpoolingSubState_SpoolUnloadOutputTransmit:
			spoolingSubState = "SPOOL UNLOAD/OUTPUT TRANSMIT";
			break;

		case SpoolingSubState_SpoolUnloadOutputPurge:
			spoolingSubState = "SPOOL UNLOAD/OUTPUT PURGE";
			break;

	}
}

////////////////////////////////////////////
// ClockChanged event handler
void SecsGemController::CClockChangedEventHandler(CGEMController &sender, ICOMSECsClockEventArgs &args)
{
	// TO DO
	if (args.IsSuccessful)
	{
		appendTerminalString("Clock changed successfully!");
	}
	else
	{
		appendTerminalString("Clock changed failed!. Error:");
		OutputDebugLogTo(9, TRUE, "Clock changed failed!. Error: %s", args.ExceptionMessage);
	}
}

////////////////////////////////////////////
// RemoteCommandIn event handler
void SecsGemController::CRemoteCommandInEventHandler(CGEMController &sender, ICOMSECsRemoteCommandEventArgs &args)
{
#ifndef _NO_SECSGEM
	_bstr_t lpCommandLogicalName = args.GetCommandLogicalName();
	_bstr_t lpCommandName = args.GetCommandName();
	char HACK = -1;

	RemoteCommandType iCommandType = args.GetCommandType();
	CString LotID, ProcessRecipe;
	ICOMSECsMessagePtr p_S2F42Reply;
	
	int subStrateCount = 0;
	int operation = -1;
	if (iCommandType == RemoteCommandType_S2F21RemoteCommand)
	{
		OutputDebugLogTo(9, TRUE, "Received Remote Command - S2F21");
		ICOMSECsMessagePtr p_S2F22Reply = getServiceManager()->ControlDiagnostic->HostCommandAcknowledge(lpCommandLogicalName, 0, NULL);
		getController()->SendReply(p_S2F22Reply, args.transactionId);
	}
	else if (iCommandType == RemoteCommandType_S2F41HostCommand || iCommandType == RemoteCommandType_S2F49EnhancedRemoteCommand) {

		if(lpCommandLogicalName == _bstr_t("PP-SELECT"))
		{
			operation = 0;
			ICommandParameterCollectionModelPtr m_CommandParameters = args.GetCommandParameters();
			OutputDebugLogTo(9, TRUE, "Received PP_SELECT Remote Command - S2F41");
		long lParametersCount = m_CommandParameters->GetCount();
	
		for(long i = 0; i<lParametersCount; i++) {
			ICommandParameterModelPtr m_CommandParameter = m_CommandParameters->GetItem(i);
		
	//			_bstr_t commandLogicalName = m_CommandParameter->GetlogicalName();
			_bstr_t commandName = m_CommandParameter->Getname();
				_bstr_t value = m_CommandParameters->GetItem(i)->Getvalue();
			SECSFormats commandFormat = m_CommandParameter->GetparameterType();
				if (commandName == _bstr_t("LotId"))
					LotID = (LPCTSTR)value;
				else if (commandName == _bstr_t("PPID"))
					ProcessRecipe = (LPCTSTR)value;
				else if (commandName == _bstr_t("SubstrateCount"))
					subStrateCount = _wtoi(value);
			}
		}
		else if (lpCommandLogicalName == _bstr_t("START"))
		{
			operation = 1;
			ICommandParameterCollectionModelPtr m_CommandParameters = args.GetCommandParameters();
			OutputDebugLogTo(9, TRUE, "Received START Remote Command - S2F41");
			long lParametersCount = m_CommandParameters->GetCount();
		}
		else if (lpCommandLogicalName == _bstr_t("STOP"))
		{
			operation = 2;
			ICommandParameterCollectionModelPtr m_CommandParameters = args.GetCommandParameters();
			OutputDebugLogTo(9, TRUE, "Received STOP Remote Command - S2F41");
			long lParametersCount = m_CommandParameters->GetCount();
		}
		else if (lpCommandLogicalName == _bstr_t("ABORT"))
		{
			operation = 3;
			ICommandParameterCollectionModelPtr m_CommandParameters = args.GetCommandParameters();
			OutputDebugLogTo(9, TRUE, "Received ABORT Remote Command - S2F41");
			long lParametersCount = m_CommandParameters->GetCount();
		}
		else if (lpCommandLogicalName == _bstr_t("GOLOCAL"))
		{
			operation = 4;
			ICommandParameterCollectionModelPtr m_CommandParameters = args.GetCommandParameters();
			OutputDebugLogTo(9, TRUE, "Received GOLOCAL Remote Command - S2F41");
			long lParametersCount = m_CommandParameters->GetCount();
		}
		else if (lpCommandLogicalName == _bstr_t("GOREMOTE"))
		{
			operation = 5;
			ICommandParameterCollectionModelPtr m_CommandParameters = args.GetCommandParameters();
			OutputDebugLogTo(9, TRUE, "Received GOREMOTE Local Command - S2F41");
			long lParametersCount = m_CommandParameters->GetCount();
		}
		else if (lpCommandLogicalName == _bstr_t("PAUSE"))
		{
			operation = 6;
			ICommandParameterCollectionModelPtr m_CommandParameters = args.GetCommandParameters();
			OutputDebugLogTo(9, TRUE, "Received PAUSE Remote Command - S2F41");
			long lParametersCount = m_CommandParameters->GetCount();
		}
		else
		{
			operation = -1;
		}

		switch (operation)
		{
		case 0:		// PP-SELECT
			if (theApp.processState == PROCESS_STATE_READY || theApp.processState == PROCESS_STATE_STOP)
			{
				if (StartMachineProcessSelectCommand(LotID, ProcessRecipe, subStrateCount))
				{
					HACK = 3;		// Invalid parameter

					p_S2F42Reply = getServiceManager()->ControlDiagnostic->HostCommandAcknowledge(lpCommandLogicalName, HACK, NULL);
					getController()->SendReply(p_S2F42Reply, args.transactionId);
				}
				else
				{
					HACK = 4;		// Acknowledge will perform later

					p_S2F42Reply = getServiceManager()->ControlDiagnostic->HostCommandAcknowledge(lpCommandLogicalName, HACK, NULL);
					getController()->SendReply(p_S2F42Reply, args.transactionId);

					CString strCategories = ProcessRecipe.Left(ProcessRecipe.FindOneOf("\\"));
					CString recipeName = ProcessRecipe.Right(ProcessRecipe.GetLength() - ProcessRecipe.FindOneOf("\\") - 1);
					CString folderName = recipeName.Left(recipeName.GetLength() - 4);

					theApp.StartProcessSelectCommandSECSGEM(LotID, strCategories, folderName, subStrateCount);
				}
			}
			else if (theApp.processState == PROCESS_STATE_EXECUTING)
			{
				HACK = 5;		// Rejected already in desired condition
				p_S2F42Reply = getServiceManager()->ControlDiagnostic->HostCommandAcknowledge(lpCommandLogicalName, HACK, NULL);
				getController()->SendReply(p_S2F42Reply, args.transactionId);
			}
			else
			{
				HACK = 2;		// Cannor perform now
				p_S2F42Reply = getServiceManager()->ControlDiagnostic->HostCommandAcknowledge(lpCommandLogicalName, HACK, NULL);
				getController()->SendReply(p_S2F42Reply, args.transactionId);
			}

			break;

		case 1:		// START
			if (theApp.processState == PROCESS_STATE_READY || theApp.processState == PROCESS_STATE_PAUSE || theApp.processState == PROCESS_STATE_STOP)
			{
				HACK = 0;
				StartMachineCommand();
			}
			else if (theApp.processState == PROCESS_STATE_EXECUTING)
				HACK = 5;
			else
				HACK = 2;

			p_S2F42Reply = getServiceManager()->ControlDiagnostic->HostCommandAcknowledge(lpCommandLogicalName, HACK, NULL);
			getController()->SendReply(p_S2F42Reply, args.transactionId);
			break;

		case 2:		// STOP
			if (theApp.processState == PROCESS_STATE_EXECUTING || theApp.processState == PROCESS_STATE_READY)
			{
				HACK = 0;
				StopMachineCommand();
			}
			else if (theApp.processState == PROCESS_STATE_STOP)
				HACK = 5;
			else
				HACK = 2;

			p_S2F42Reply = getServiceManager()->ControlDiagnostic->HostCommandAcknowledge(lpCommandLogicalName, HACK, NULL);
			getController()->SendReply(p_S2F42Reply, args.transactionId);
			break;

		case 4:		// GOLOCAL
			if(theApp.processState == PROCESS_STATE_EXECUTING)
				HACK = 5;
			else
			{
				HACK = 0;
				setLocal();
			}
			p_S2F42Reply = getServiceManager()->ControlDiagnostic->HostCommandAcknowledge(lpCommandLogicalName, HACK, NULL);
			getController()->SendReply(p_S2F42Reply, args.transactionId);
			break;

		case 5:		// GOREMOTE
			if (theApp.processState == PROCESS_STATE_EXECUTING)
				HACK = 5;
			else
			{
				HACK = 0;
				setRemote();
			}
			p_S2F42Reply = getServiceManager()->ControlDiagnostic->HostCommandAcknowledge(lpCommandLogicalName, HACK, NULL);
			getController()->SendReply(p_S2F42Reply, args.transactionId);
			break;

		case 6:		// PAUSE
			if (theApp.processState == PROCESS_STATE_EXECUTING)
			{
				HACK = 0;
				PauseMachineCommand();
			}
			else
				HACK = 2;

			p_S2F42Reply = getServiceManager()->ControlDiagnostic->HostCommandAcknowledge(lpCommandLogicalName, HACK, NULL);
			getController()->SendReply(p_S2F42Reply, args.transactionId);
			break;

		default:
			break;
		}

		if (iCommandType == RemoteCommandType_S2F49EnhancedRemoteCommand)
		{
			OutputDebugLogTo(9, TRUE, "Received Remote Command - S2F50");
			ICOMSECsMessagePtr p_S2F50Reply = getServiceManager()->ControlDiagnostic->EnhancedRemoteCommandAcknowledge(lpCommandLogicalName, 0, NULL);
			getController()->SendReply(p_S2F50Reply, args.transactionId);
		}
	}
#endif
}

void SecsGemController::CustomMessageGeneration()
{
#ifndef _NO_SECSGEM
	//Create a customized message with S64F101 with a 'wait' flag enabled in the following SML
	//[S64F101 (W)] 
	//	<L [5] <A 'B1002'> OperatorBatchId 
	//	<A 'Dave Hunter'> OperatorName 
	//	<U4 3001> OperationId 
	//	<L [3] 
	//		<F4 1.02> Temperature 
	//		<U4 242> ActionId 
	//		<A 'Replenish Material'> Action Description 
	//	> 
	//	<L [2] 
	//		<B 0x01> Machine status flag 
	//		<A 'Machine is operational'> Machine status description
	//	>
	//>

	//If needReply is set to true, the SecsToHost compiler monitor the transaction T3 timeout
	ICOMSECsMessagePtr p_CustomMessage = getServiceManager()->CustomMessage->CreateMessage(64, 101, true);

	//Add a LIST data type to the collection
	p_CustomMessage->DataItem->AddList();

	//Add OperatorBatchId item to the root LIST collection
	p_CustomMessage->GetDataItem()->GetItem(0)->Add("OperatorBatchId", "B1002", SECsFormat_Ascii);

	//Add OperatorName item to the root LIST collection
	p_CustomMessage->GetDataItem()->GetItem(0)->Add("OperatorName", "Dave Hunter", SECsFormat_Ascii);

	//Add OperatorId to the root LIST coolection
	p_CustomMessage->GetDataItem()->GetItem(0)->Add("OperationId", 3001, SECsFormat_U4);

	//Add LIST item to the root LIST collection
	p_CustomMessage->GetDataItem()->GetItem(0)->AddList();

	//Add Temperature
	p_CustomMessage->GetDataItem()->GetItem(0)->GetItem(3)->Add("Temperature", 1.02, SECsFormat_F4);

	//Add ActionID
	p_CustomMessage->GetDataItem()->GetItem(0)->GetItem(3)->Add("ActionId", "242", SECsFormat_U4);

	//Add Action Description
	p_CustomMessage->GetDataItem()->GetItem(0)->GetItem(3)->Add("ActionDescription", "Replenish Material", SECsFormat_Ascii);

	//Add another LIST item to the root LIST collection
	p_CustomMessage->GetDataItem()->GetItem(0)->AddList();

	//Add Machine status flag
	p_CustomMessage->GetDataItem()->GetItem(0)->GetItem(4)->Add("MachineStatus", "1", SECsFormat_Binary);

	//Add Machine status description
	p_CustomMessage->GetDataItem()->GetItem(0)->GetItem(4)->Add("MachineStatusDescription", "Machine is operational", SECsFormat_Ascii);

	//Send message S64F101 to equipment
	getController()->Send(p_CustomMessage);
#endif
}

void SecsGemController::OnCustomMessageReply(ICOMSECsSecondaryInEventArgs &args)
{
#ifndef _NO_SECSGEM
	ICOMSECsMessagePtr output = args.GetOutputs();
	CString SF;
	SF.Format("S%dF%d", output->GetStream(), output->GetFunction());

	long lStreamId = args.GetOutputs()->GetStream();
	long lFunctionId = args.GetOutputs()->GetFunction();

	if (lStreamId == 14 && lFunctionId == 2) {
		// OBJID
		_bstr_t sObjId = (_bstr_t)args.GetOutputs()->DataItem->GetItem(0)->GetItem(0)->GetItem(0)->GetItem(0)->GetValue();

		// MapData
		_bstr_t sMapData = (_bstr_t)args.GetOutputs()->DataItem->GetItem(0)->GetItem(0)->GetItem(0)->GetItem(1)->GetItem(0)->GetItem(1)->GetValue();

		OutputDebugLogTo(9, TRUE, "2 Mapdata for StripId - %s received from Host", sObjId);
		appendTerminalString("Mapdata received from Host");

		std::string Mapdata = ConvertBSTRToMBS(sMapData);

		ofstream myfile;
		myfile.open("d:\\mapdata\\mapdata.xml");
		myfile.write(Mapdata.c_str(), Mapdata.length());
		myfile.close();
		OutputDebugLogTo(9, TRUE, "Mapdata saved to local PC");
		int ret = readMapDataFromHost("D:\\MapData\\MapData.xml");
		if (ret == 0)
		{
			theApp.mapFileReceived = true;
			OutputDebugLogTo(9, TRUE, "Mapdata read finished from file");
		}
		else
		{
			OutputDebugLogTo(9, TRUE, "Mapdata failed to read.");
		}
	}
	else if (lStreamId == 10 && lFunctionId == 2) {
		int acknowledge = (int)args.GetOutputs()->DataItem->GetItem_2("ACKC10")->GetValue();
		OutputDebugLogTo(9, TRUE, "SECS/GEM: Terminal Request acknowledge code: %d", acknowledge);
		appendTerminalString("Terminal Request acknowledge code: " + acknowledge);
	}
	else if (SF == "S64F102") {
		HandleS6F102(args.GetOutputs());
	}
	else
	{

	}

#endif
}

//Handle custom generated message S64F101
void SecsGemController::HandleS6F102(ICOMSECsMessagePtr output)
{
	if(output != nullptr) {
		//Get the acknowledgement code
		int acknowledge = output->GetDataItem()->GetItem(0)->GetItem(0)->ToInt();

		//if acknowledge is non-zero indicate there is an error
		if(acknowledge != 0) {
			//Get the error code
			CString errorCode = (CString)output->GetDataItem()->GetItem(0)->GetItem(1)->GetItem(0)->GetValue();
			CString errorDescription = (CString)output->GetDataItem()->GetItem(0)->GetItem(1)->GetItem(1)->GetValue();

			CString message;
			message.Format("S64F102, Host replies: %d. Error code: %s Error Descriptions: %s", acknowledge, errorCode, errorDescription);
			appendTerminalString(message);
			//OutputDebugLogTo(9, TRUE, "SECS/GEM: %s", message);
		}
	}
}

//Handle custom message send by the Host side and send the reply accordingly
void SecsGemController::OnCustomMessageRequest(ICOMSECsPrimaryInEventArgs& args)
{
#ifndef _NO_SECSGEM
	ICOMSECsMessagePtr input = args.GetInputs();
	CString SF;
	long lStreamId		= input->GetStream();
	long lFunctionId	= input->GetFunction();
	SF.Format("S%dF%d", lStreamId, lFunctionId);

	if ((lStreamId > 14) && (lStreamId < 64))
	{
		getController()->UnrecognizedStream(input->Header);
	}
	else if ((lStreamId > 64) && (lStreamId < 101))
	{
		getController()->UnrecognizedStream(input->Header);
	}
	else if(SF == "S101F1") {
		HandleS101F1(input, args.transactionId);
	}
	else
	{
		getController()->UnrecognizedFunction(input->Header);
	}

#endif
}

void SecsGemController::HandleS101F1(ICOMSECsMessagePtr input, int transactionId)
{
#ifndef _NO_SECSGEM
	//Get custom event id
	CString eventId = (CString)input->GetDataItem()->GetItem(0)->GetItem(0)->GetValue();

	//Get event descriptions
	CString eventDescriptions = (CString)input->GetDataItem()->GetItem(0)->GetItem(1)->GetValue();

	//Get Machine Flag
	CString machineFlag = (CString)input->GetDataItem()->GetItem(0)->GetItem(2)->GetItem(0)->GetValue();

	//Get some other value
	CString otherValue = (CString)input->GetDataItem()->GetItem(0)->GetItem(2)->GetItem(1)->GetItem(0)->GetValue();

	if(input->GetNeedReply()) {
		//Prepare the reply message
		ICOMSECsMessagePtr p_Reply = getServiceManager()->CustomMessage->CreateMessage(101, 2, false);

		p_Reply->GetDataItem()->AddList();

		p_Reply->GetDataItem()->GetItem(0)->Add("ACK", bstr_t(machineFlag), SECsFormat_Binary);

		p_Reply->GetDataItem()->GetItem(0)->AddList();
		p_Reply->GetDataItem()->GetItem(0)->GetItem(1)->Add("Description", "Host Acknowledge OK", SECsFormat_Ascii);

		//Reply S101F2 to equipment with the reference of transactionId from the primary message
		getController()->SendReply(p_Reply, transactionId);
		//OutputDebugLogTo(9, TRUE, "SECS/GEM: Received S101F1 from Host and Equipment replied with S101F2");
		appendTerminalString("Received S101F1 from Host and Equipment replied with S101F2");
	}
#endif
}

//Send/Received Array Data item
void SecsGemController::sendArrayItem()
{
}

void SecsGemController::HandleIncomingArrayMessage(ICOMSECsPrimaryInEventArgs& args)
{
	
}

// Host initiates process program upload
void SecsGemController::OnHostRecipeUploadRequest(ICOMSECsPrimaryInEventArgs &args)
{
#ifndef _NO_SECSGEM
	ICOMSECsMessagePtr input = args.GetInputs();

	//Retrieving the requested recipe file
	CString recipeFileName = (CString)input->GetDataItem()->GetItem_2("PPID")->GetValue();
	recipeFileName = this->getController()->GetProcessProgramPath() + "\\" + recipeFileName;

	//if(theApp.DoesFileExist(recipeFileName)) {
	//	//Trying to open the request recipe file
	//	std::fstream fileStream(recipeFileName, std::ios::in | std::ios::binary | std::ios::ate);
	//
	//	char* ppBody = 0;
	//	int size = 0;
	//
	//	if(fileStream.is_open()) {
	//		size = static_cast<int>(fileStream.tellg());
	//		ppBody = new char[static_cast<int>(size + 1)];
	//		memset(ppBody, 0, static_cast<int>(size + 1));
	//
	//		fileStream.seekg(0, std::ios::beg);
	//		
	//		//Read the binary content
	//		fileStream.read(ppBody, size);
	//
	//		//Send the recipe body in the reply
	//		
	//		ICOMSECsMessagePtr p_RecipeToReply = getServiceManager()->GetProcessProgram()->ProcessProgramData(bstr_t(recipeFileName), ppBody);
	//
	//		getController()->SendReply(p_RecipeToReply, args.transactionId);
	//
	//		appendTerminalString("Recipe: " + recipeFileName + " downloaded!");
	//		OutputDebugLogTo(9, TRUE, "Recipe: %s downloaded!", recipeFileName);
	//	}
	//
	//	fileStream.close();
	//}
	//else {
	//	ICOMSECsMessagePtr p_RecipeToReply = getServiceManager()->GetProcessProgram()->ProcessProgramData("", NULL);
	//
	//	getController()->SendReply(p_RecipeToReply, args.GettransactionId());
	//
	//	//Acknowledge the equipment via S7F4
	//	ICOMSECsMessagePtr p_Acknowledge = getServiceManager()->GetProcessProgram()->ProcessProgramAcknowledge(0);
	//
	//	getController()->SendReply(p_Acknowledge, args.GettransactionId());
	//
	//	//Retrieve the PPID
	//	CString ppid = (CString)input->GetDataItem()->GetItem_2("Ln")->GetItem_2("PPID")->GetValue();
	//
	//	//Retrieve the PPBODY
	//	CString ppBody = (CString)input->GetDataItem()->GetItem_2("Ln")->GetItem_2("PPBODY")->GetValue();
	//
	//	getController()->SaveProcessProgramToDisk_2(bstr_t(ppid), bstr_t(ppBody));
	//}
#endif
}

void SecsGemController::OnHostRecipeDownload(ICOMSECsPrimaryInEventArgs &args)
{
#ifndef _NO_SECSGEM
	//Host must send S7F1/F2 and equipment application must either grant or not grant the
	//host requests. If PP Inquire is not granted, the process program download will be rejected
	ICOMSECsMessagePtr input = args.GetInputs();
	
	//Acknowledge the equipment via S7F4
	ICOMSECsMessagePtr p_Acknowledge = getServiceManager()->GetProcessProgram()->ProcessProgramAcknowledge(0);

	getController()->SendReply(p_Acknowledge, args.GettransactionId());

	//Retrieve the the PPID
	CString ppid = (CString)input->GetDataItem()->GetItem_2("Ln")->GetItem_2("PPID")->GetValue();
	//int len = ppid.GetLength();
	//ppid.Delete(len - 4, 4);
	//theApp.LoadConfigFile(theApp.configurationCategories, ppid);
	CString ppid1 = "D:\\Test21.rcp";
	//Retrieve the PPBODY
	CString ppbody = (CString)input->GetDataItem()->GetItem_2("Ln")->GetItem_2("PPBODY")->GetValue();

//	CString recipeFileName = this->getController()->GetProcessProgramPath() + "\\" + ppId;
	
	//Persist the downloaded process program to the path specified
	getController()->SaveProcessProgramToDisk_2(bstr_t(ppid1), bstr_t(ppbody));
#endif
}

// S7F17
void SecsGemController::HandleProcessProgramDeleteRequest(ICOMSECsPrimaryInEventArgs& args) {
#ifndef _NO_SECSGEM

	bool *bDelete;
	ICOMSECsMessagePtr input = args.GetInputs();
	long size = input->GetDataItem()->GetItem_2("Ln")->Count;
	bDelete = new bool[size];
	char ACK7 = -1;

	CString strRecipe = "";
	CString strRecipeDir = "";

	if(!theApp.RecipeFiles.size())
		theApp.ListAllProcessRecipes();

	//Retrieving the requested recipe file	
	for (int i = 0; i < size; i++)
	{
		bDelete[i] = false;
		strRecipe = (CString)input->DataItem->GetItem(0)->GetItem(i)->GetValue();

		// Check whether this recipe available in directory
		for (int j = 0; j < theApp.RecipeFiles.size(); j++)
		{
			strRecipeDir = theApp.RecipeFiles.at(j);

			if (strRecipe.CompareNoCase(theApp.currentRecipeLoaded) == 0)
			{
				ACK7 = 1;			// Permission not granted can't delete current recipe in use
				break;
			}
			else if (strRecipeDir.CompareNoCase(strRecipe) == 0)
			{
				// Delete Recipe
				if(!theApp.DeleteRecipeFile(strRecipe));
					bDelete[i] = true;
				break;
			}

			if(j == theApp.RecipeFiles.size()-1)
			{
				ACK7 = 4;			// Recipe not found
			}
		}
	}

	if (ACK7 < 0)
	{
		for (int i = 0; i < size; i++)
		{
			if (bDelete[i])			// All recipes deleted successfully
				ACK7 = 0;
			else
				ACK7 = 1;			// Failed to delete recipe(s) successfully
		}
	}

	//Acknowledge the equipment via S7F4
	ICOMSECsMessagePtr p_Acknowledge = getServiceManager()->GetProcessProgram()->DeleteProcessProgramAcknowledge(ACK7);
	getController()->SendReply(p_Acknowledge, args.GettransactionId());

#endif
}

//Send terminal message request to Host via S10F1/F2
void SecsGemController::SendTerminalMessageToHost(CString message)
{
#ifndef _NO_SECSGEM
	ICOMSECsMessagePtr S10F1 = getServiceManager()->GetTerminalDisplay()->TerminalRequest(bstr_t(message));
	ICOMSECsMessagePtr S10F2 = getController()->Send(S10F1);
#endif
}

void SecsGemController::RequestMapDataFromHost(CString substrateBarcodeID)
{
	appendTerminalString("Send Request for Mapdata from Host");

	ICOMMessageServiceManagerPtr p_ServiceManager = getController()->GetServices();
	ICOMSECsMessagePtr p_S14F1 = p_ServiceManager->CustomMessage->CreateMessage(14, 1, true);

	p_S14F1->DataItem->AddList();
	p_S14F1->DataItem->GetItem(0)->Add("OBJSPEC", "", SECsFormat_Ascii);
	p_S14F1->DataItem->GetItem(0)->Add("OBJTYPE", "Substrate", SECsFormat_Ascii);
	p_S14F1->DataItem->GetItem(0)->AddList();
	p_S14F1->DataItem->GetItem(0)->GetItem(2)->Add("OBJID", substrateBarcodeID.GetBuffer(), SECsFormat_Ascii);
	p_S14F1->DataItem->GetItem(0)->AddList();
	p_S14F1->DataItem->GetItem(0)->GetItem(3)->AddList();
	p_S14F1->DataItem->GetItem(0)->GetItem(3)->GetItem(0)->Add("ATTRID", "SubstrateType", SECsFormat_Ascii);
	p_S14F1->DataItem->GetItem(0)->GetItem(3)->GetItem(0)->Add("ATTRDATA", "Strip", SECsFormat_Ascii);
	p_S14F1->DataItem->GetItem(0)->GetItem(3)->GetItem(0)->Add("ATTRRELN", "0", SECsFormat_Ascii);
	p_S14F1->DataItem->GetItem(0)->AddList();
	p_S14F1->DataItem->GetItem(0)->GetItem(4)->Add("ATTRID", "MapData", SECsFormat_Ascii);

	ICOMSECsMessagePtr p_S14F2 = getController()->Send(p_S14F1);
	theApp.mapFileReceived = false;

/*	if(p_S14F2 != nullptr) {

		// OBJID
		_bstr_t sObjId = (_bstr_t) p_S14F2->DataItem->GetItem(0)->GetItem(0)->GetItem(0)->GetItem(0)->GetValue();

		// MapData
		_bstr_t sMapData = (_bstr_t) p_S14F2->DataItem->GetItem(0)->GetItem(0)->GetItem(0)->GetItem(1)->GetItem(0)->GetItem(1)->GetValue();

		OutputDebugLogTo(9, TRUE, "Mapdata from Host : %s", sMapData);

		std::string Mapdata = ConvertBSTRToMBS(sMapData);

		ofstream myfile;
		myfile.open("D:\\MapData\\MapData.xml");
		myfile.write(Mapdata.c_str(), Mapdata.length());
		myfile.close();

		readMapDataFromHost("D:\\MapData\\MapData.xml");
	}
*/
}

int SecsGemController::readMapDataFromHost(std::string filePath)
{
//	xml.load(filePath);
	int binDefinitionCount = 0;
	int intPredefineFailBinCount = 0;

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
		std::string::size_type sz;

				  //Initialize COM Library:
		CoInitialize(NULL);

		//Create an instance of the DOMDocument object:
		docPtr.CreateInstance(__uuidof(DOMDocument30));

		// Load a document:
		_variant_t varXml(filePath.c_str());//XML file to load
		_variant_t varResult((bool) TRUE);//result 

		varResult = docPtr->load(varXml);

		if((bool) varResult == FALSE) {
			//printf("*** Error:failed to load XML file. ***\n");
			MessageBox(0, "Error: failed to load XML file. Check the file name.", \
				"Load XML file", MB_OK | MB_ICONWARNING);
			return -1;
		}

		//Collect all or selected nodes by tag name:
		NodeListPtr = docPtr->getElementsByTagName(strFindText);
		mapData.BinDefinition.resize(BIN_QUALITY_COUNT);

		//Output the number of nodes:
		//printf("Number of nodes: %d\n", (NodeListPtr->length));

		//Output root node:
		docPtr->documentElement->get_nodeName(&bstrItemText);

		for(i = 0; i < (NodeListPtr->length); i++) {

			if(pIDOMNode) pIDOMNode->Release();
			NodeListPtr->get_item(i, &pIDOMNode);

			if(pIDOMNode) {

				pIDOMNode->get_nodeTypeString(&bstrNodeType);

				//We process only elements (nodes of "element" type): 
				BSTR temp = L"element";
				std::string BinCode = "BinCode";

				if(lstrcmp((LPCTSTR) bstrNodeType, (LPCTSTR) temp) == 0) {
					n++;//element node's number

					pIDOMNode->get_nodeName(&bstrItemNode);

					if(lstrcmp((LPCTSTR) bstrItemNode, (LPCTSTR) L"MapData") == 0)
						continue;

					pIDOMNode->get_text(&bstrItemText);

					pIDOMNode->get_parentNode(&pIParentNode);
					pIParentNode->get_nodeName(&bstrItemParent);

					pIDOMNode->get_childNodes(&childList);

					/*assert(bstrItemNode != nullptr);
					std::wstring ws(bstrItemNode, SysStringLen(bstrItemNode));*/
					std::string str = ConvertBSTRToMBS(bstrItemNode);

					if(strcmp(str.c_str(), BinCode.c_str()) == 0) {
						//strMapData[nCount++] = bstrItemText;
						mapData.binCode.push_back(ConvertBSTRToMBS(bstrItemText).c_str());
					}

					//Get the attributes:
					int j = 0;//loop-index variable
					long length;// number of attributes in the collection

					DOMNamedNodeMapPtr = pIDOMNode->attributes;

					hResult = DOMNamedNodeMapPtr->get_length(&length);

					if(SUCCEEDED(hResult)) {
						//Loop through the number of attributes:
						for(j = 0; j < length; j++) {
							//get attribute node:							
							DOMNamedNodeMapPtr->get_item(j, &pIAttrNode);

							pIAttrNode->get_nodeTypeString(&bstrAttrType);//type as string
							//pIAttrNode->get_nodeType(&bstrAttrType);//enumerated type
							//printf("Attribute type: %d\n", bstrAttrType);
							pIAttrNode->get_nodeName(&bstrAttrName);
							
							pIAttrNode->get_text(&bstrAttrText);
							
							std::string str = ConvertBSTRToMBS(bstrAttrName);

							if(strcmp(str.c_str(), "LayoutId") == 0)
								strLayoutId = bstrAttrText;

							if(strcmp(ConvertBSTRToMBS(bstrItemNode).c_str(), "Dimension") == 0) {
								if(strcmp(ConvertBSTRToMBS(strLayoutId).c_str(), "SubstrateLayout") == 0) {
									if(strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "X") == 0)
										mapData.nSubstrateColumnCount = _wtoi(bstrAttrText);
									else if(strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "Y") == 0)
										mapData.nSubstrateRowCount = _wtoi(bstrAttrText);
								}
								else if(strcmp(ConvertBSTRToMBS(strLayoutId).c_str(), "UnitLayout") == 0) {
									if(strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "X") == 0)
										mapData.nUnitColumnCount = _wtoi(bstrAttrText);
									else if(strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "Y") == 0)
										mapData.nUnitRowCount = _wtoi(bstrAttrText);
								}
							}
							else if(strcmp(ConvertBSTRToMBS(bstrItemNode).c_str(), "Substrate") == 0) {
								if(strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "SubstrateType") == 0)
									mapData.strSubstrateType = ConvertBSTRToMBS(bstrAttrText);
								else if(strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "SubstrateId") == 0)
									mapData.strSubstrateId = ConvertBSTRToMBS(bstrAttrText);
							}
							else if (strcmp(ConvertBSTRToMBS(bstrItemNode).c_str(), "SubstrateMap") == 0) {
								if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "Orientation") == 0)
									mapData.strOrientation = ConvertBSTRToMBS(bstrAttrText);
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "OriginLocation") == 0)
									mapData.strOriginLocation = ConvertBSTRToMBS(bstrAttrText);
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "SubstrateSide") == 0)
									mapData.strSubstrateSide = ConvertBSTRToMBS(bstrAttrText);
								else if (strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "AxisDirection") == 0)
									mapData.strAxisDirection = ConvertBSTRToMBS(bstrAttrText);
							}
							else if(strcmp(ConvertBSTRToMBS(bstrItemNode).c_str(), "BinDefinition") == 0 && strcmp(ConvertBSTRToMBS(bstrItemParent).c_str(), "BinDefinitions") == 0) {
								if(strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "BinCode") == 0) {
									sscanf(ConvertBSTRToMBS(bstrAttrText).c_str(), "%x", &mapData.BinDefinition.at(binDefinitionCount).nBinCode);
								}
								else if(strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "BinCount") == 0) {
									sscanf(ConvertBSTRToMBS(bstrAttrText).c_str(), "%d", &mapData.BinDefinition.at(binDefinitionCount).nBinCount);
								}
								else if(strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "BinQuality") == 0) {
									mapData.BinDefinition.at(binDefinitionCount).strBinQuality = ConvertBSTRToMBS(bstrAttrText).c_str();
								}
								else if(strcmp(ConvertBSTRToMBS(bstrAttrName).c_str(), "BinDescription") == 0) {
									mapData.BinDefinition.at(binDefinitionCount).strBinDescription = ConvertBSTRToMBS(bstrAttrText).c_str();
								}
								if (j == (length - 1))
									binDefinitionCount++;
							}
						}
					}
					//	::MessageBox(NULL, (LPCSTR) bstrItemText,"parsing", MB_OK);		
				}
			}
		}
		//Do not forget to release interfaces:
		pIDOMNode->Release();
		pIDOMNode = NULL;
		pIParentNode->Release();
		pIParentNode = NULL;
	}
	catch(...) {
		MessageBox(NULL, ("*** Exception occurred ***"), ("Error message"), MB_OK);
	}

	mapData.scanning2D3DImageStatus.resize(mapData.nUnitRowCount *  mapData.nUnitColumnCount);
	mapData.scanning2D3DImageEnableInspection.resize(mapData.nUnitRowCount *  mapData.nUnitColumnCount);
	mapData.inspectionResultMap.resize(mapData.nUnitRowCount *  mapData.nUnitColumnCount);
	std::fill(mapData.inspectionResultMap.begin(), mapData.inspectionResultMap.end(), NOT_INSPECTED);

	int nGrabID = 0;
	char strBinCode[5] = "";
	int intBuff;
	int intPredefineFail = 0;

	for (int k = 0; k < binDefinitionCount; k++)
	{
		if (mapData.BinDefinition.at(k).strBinQuality == "Bad")
			intPredefineFailBinCount = mapData.BinDefinition.at(k).nBinCount;
	}

	for(int i = 0; i < mapData.nUnitRowCount; i++) {
		for(int j = 0; j <  mapData.nUnitColumnCount; j++, nGrabID++) {
			strncpy(strBinCode, mapData.binCode[i].c_str() + j*(mapData.binCode[i].size() / mapData.nUnitColumnCount), mapData.binCode[i].size() / mapData.nUnitColumnCount);
			sscanf(strBinCode, "%x", &intBuff);
			mapData.scanning2D3DImageStatus[nGrabID] = intBuff;

			if(intBuff == mapData.BinDefinition[BIN_PASS].nBinCode)
				mapData.scanning2D3DImageEnableInspection[nGrabID] = TRUE;
			else
			{
				intPredefineFail++;
				mapData.scanning2D3DImageEnableInspection[nGrabID] = FALSE;
			}
		}
	}

	if (intPredefineFailBinCount != intPredefineFail)
	{
		setAlarm("BIN_COUNT_MISMATCH_ERROR");
		AfxMessageBox("Bad Unit Bincount doesn't tally. Please check again.");
		return -1;
	}

	if (mapData.strSubstrateId.c_str() != theApp.strInspectDeviceBarcodeID)
	{
		setAlarm("STRIPID_MISMATCH_ERROR");
		AfxMessageBox("Map Strip ID and Bardcode ID doesn't tally. Please check again.");
		return -1;
	}

	return 0;
}

int SecsGemController::getMappingDetails(std::string &stripId, std::string &stripType, int &nRowCount, int &nColumnCount)
{
	if(mapData.nUnitColumnCount <= 0 || mapData.nUnitRowCount <= 0)
		return - 1;

	stripId = mapData.strSubstrateId;
	stripType = mapData.strSubstrateType;
	nRowCount = mapData.nUnitRowCount;
	nColumnCount = mapData.nUnitColumnCount;

	return 0;
}

int SecsGemController::getMappingInspectionEnableStatus(std::vector<bool> &enableInspection)
{
	if(enableInspection.size() != 2*mapData.scanning2D3DImageEnableInspection.size()) {
		OutputDebugLogTo(9, TRUE, "Error: Miss match in Image count.");
		return -1;
	}

	enableInspection[0] = mapData.scanning2D3DImageEnableInspection[0];
	enableInspection[1] = mapData.scanning2D3DImageEnableInspection[0];

	for(int i=2; i< enableInspection.size(); i++)
		enableInspection[i] = mapData.scanning2D3DImageEnableInspection[int(i/2)];
	return 0;
}

void SecsGemController::UpdateSubstrateResults(CString substrateBarcodeID)
{
	CString csPassBinCount, csFailBinCount, csNotProcessBinCount;
	CString csPassBinCode, csNotProcessBinCode, csMapPredefineFailCode;
	CString csChipDimensionX, csChipDimensionY;
	CString csUnitDimensionX, csUnitDimensionY;
	CString *csRowData;
	CString csResultCode;
	CString csEntireRowCode;
	CString csTemp;
	CString strOrientation = mapData.strOrientation.c_str();
	CString strOriginLocation = mapData.strOriginLocation.c_str();
	CString strSubstrateSide = "TopSide";	// mapData.strSubstrateSide.c_str();
	CString strAxisDirection = mapData.strAxisDirection.c_str();

	int nPassBinCount, nFailBinCount, nNotProcessBinCount, nMapPredefineFailCount;
	int nChipDimensionX, nChipDimensionY;
	int nUnitDimensionX, nUnitDimensionY;
	
	CString csFailSubstrateLocationCount;
	CString csFailInkDieCount = 0;
	CString csFailDieLocationCount = 0;
	CString csFailDieOffsetCount = 0;
	CString csFailDieRotationCount = 0;
	CString csFailEpoxyCount = 0;
	CString csFailPVIUnitSurfaceCount = 0;
	CString csFailPVIDeviceCount = 0;
	CString csFailPVIUnitEdgeCount = 0;
	CString csFailMissingWireCount = 0;
	CString csFailWireCount = 0;
	CString csFailSaggingWireCount = 0;
	CString csFailWireLoopHeightCount = 0;
	CString csFailWireLengthCount = 0;
	CString csFailWireShortToDieCount = 0;
	CString csFailOffPadCount = 0;
	CString csFailMissingBallBondPadCount = 0;
	CString csFailBallLocationBondPadCount = 0;
	CString csFailBallDiameterBondPadCount = 0;
	CString csFailStitchDimensionCount = 0;
	CString csFailStitchQualityCount = 0;
	CString csFailStudBallCount = 0;
	CString csFailSmashBondCount = 0;
	CString csFailHRBOffPadCount = 0;
	CString csFailUnitClubBallCount = 0;
	CString csFailDeviceClubBallCount = 0;

	int nUnknownDefectCount = 0;
	int nFailSubstrateLocationCount = 0;
	int nFailInkDieCount = 0;
	int nFailDieLocationCount = 0;
	int nFailDieOffsetCount = 0;
	int nFailDieRotationCount = 0;
	int nFailEpoxyCount = 0;
	int nFailPVIUnitSurfaceCount = 0;
	int nFailPVIDeviceCount = 0;
	int nFailPVIUnitEdgeCount = 0;
	int nFailMissingWireCount = 0;
	int nFailWireCount = 0;
	int nFailSaggingWireCount = 0;
	int nFailWireLoopHeightCount = 0;
	int nFailWireLengthCount = 0;
	int nFailWireShortToDieCount = 0;
	int nFailOffPadCount = 0;
	int nFailMissingBallBondPadCount = 0;
	int nFailBallLocationBondPadCount = 0;
	int nFailBallDiameterBondPadCount = 0;
	int nFailStitchDimensionCount = 0;
	int nFailStitchQualityCount = 0;
	int nFailStudBallCount = 0;
	int nFailSmashBondCount = 0;
	int nFailLiftedBallCount = 0;
	int nFailHRBOffPadCount = 0;
	int nFailUnitClubBallCount = 0;
	int nFailDeviceClubBallCount = 0;

	CString csSubstrateLocationCode;
	CString csInkDieCode;
	CString csDieLocationCode;
	CString csDieOffsetCode;
	CString csDieRotationCode;
	CString csEpoxyCode;
	CString csPVIUnitSurfaceCode;
	CString csPVIDeviceCode;
	CString csPVIUnitEdgeCode;
	CString csMissingWireCode;
	CString csWireCode;
	CString csSaggingWireCode;
	CString csWireLoopHeightCode;
	CString csWireLengthCode;
	CString csWireShortToDieCode;
	CString csOffPadCode;
	CString csMissingBallBondPadCode;
	CString csBallLocationBondPadCode;
	CString csBallDiameterBondPadCode;
	CString csStitchDimensionCode;
	CString csStitchQualityCode;
	CString csStudBallCode;
	CString csSmashBondCode;
	CString csLiftedBallCode;
	CString csHRBOffPadCode;
	CString csUnitClubBallCode;
	CString csDeviceClubBallCode;

	nPassBinCount = 0;
	nFailBinCount = 0;
	nNotProcessBinCount = 0;
	nMapPredefineFailCount = 0;

	nChipDimensionX = mapData.nSubstrateColumnCount;	// 100;
	nChipDimensionY = mapData.nSubstrateRowCount;		// 120;

	csChipDimensionX.Format("%d", nChipDimensionX);
	csChipDimensionY.Format("%d", nChipDimensionY);

	nUnitDimensionX = mapData.nUnitColumnCount;		// 1000;
	nUnitDimensionY = mapData.nUnitRowCount;		// 1200;

	csUnitDimensionX.Format("%d", nUnitDimensionX);
	csUnitDimensionY.Format("%d", nUnitDimensionY);

	csPassBinCode = "0000";
	csNotProcessBinCode = "AAAA";
	csMapPredefineFailCode = "5000";

	csRowData = new CString[mapData.nUnitRowCount];

	for (int y = 0; y < mapData.nUnitRowCount; y++)
	{
		for (int x = 0; x < mapData.nUnitColumnCount; x++)
		{
			if (mapData.scanning2D3DImageEnableInspection[y * mapData.nUnitColumnCount + x]) {

//				OutputDebugLogTo(9, FALSE, "Result code: %d", mapData.inspectionResultMap[y * mapData.nUnitColumnCount + x]);

				if ((mapData.inspectionResultMap[y * mapData.nUnitColumnCount + x]) == GOOD_UNIT)
				{
					csResultCode = csPassBinCode;
					nPassBinCount++;
				}
				else if ((mapData.inspectionResultMap[y * mapData.nUnitColumnCount + x]) == NOT_INSPECTED)
				{
					csResultCode = csNotProcessBinCode;
					nNotProcessBinCount++;
				}
				else 
				{
					nFailBinCount++;
					switch(mapData.inspectionResultMap[y * mapData.nUnitColumnCount + x])
					{
					case 001:
						csSubstrateLocationCode = theApp.defctCodeArr[3].c_str();
						csResultCode = csSubstrateLocationCode;
						nFailSubstrateLocationCount++;
						break;
					case 102:
						csInkDieCode = theApp.defctCodeArr[4].c_str();
						csResultCode = csInkDieCode;
						nFailInkDieCount++;
						break;
					case 103:
						csDieLocationCode = theApp.defctCodeArr[5].c_str();
						csResultCode = csDieLocationCode;
						nFailDieLocationCount++;
						break;
					case 104:
						csDieOffsetCode = theApp.defctCodeArr[6].c_str();
						csResultCode = csDieOffsetCode;
						nFailDieOffsetCount++;
						break;
					case 105:
						csDieRotationCode = theApp.defctCodeArr[7].c_str();
						csResultCode = csDieRotationCode;
						nFailDieRotationCount++;
						break;
					case 201:
						csEpoxyCode = theApp.defctCodeArr[8].c_str();
						csResultCode = csEpoxyCode;
						nFailEpoxyCount++;
						break;
					case 301:
						csPVIUnitSurfaceCode = theApp.defctCodeArr[9].c_str();
						csResultCode = csPVIUnitSurfaceCode;
						nFailPVIUnitSurfaceCount++;
						break;
					case 302:
						csPVIDeviceCode = theApp.defctCodeArr[10].c_str();
						csResultCode = csPVIDeviceCode;
						nFailPVIDeviceCount++;
						break;
					case 303:
						csPVIUnitEdgeCode = theApp.defctCodeArr[11].c_str();
						csResultCode = csPVIUnitEdgeCode;
						nFailPVIUnitEdgeCount++;
						break;
					case 401:
						csMissingWireCode = theApp.defctCodeArr[12].c_str();
						csResultCode = csMissingWireCode;
						nFailMissingWireCount++;
						break;
					case 402:
						csWireCode = theApp.defctCodeArr[13].c_str();
						csResultCode = csWireCode;
						nFailWireCount++;
						break;
					case 403:
						csSaggingWireCode = theApp.defctCodeArr[14].c_str();
						csResultCode = csSaggingWireCode;
						nFailSaggingWireCount++;
						break;
					case 405:
						csWireLoopHeightCode = theApp.defctCodeArr[15].c_str();
						csResultCode = csWireLoopHeightCode;
						nFailWireLoopHeightCount++;
						break;
					case 406:
						csWireLengthCode = theApp.defctCodeArr[16].c_str();
						csResultCode = csWireLengthCode;
						nFailWireLengthCount++;
						break;
					case 407:
						csWireShortToDieCode = theApp.defctCodeArr[17].c_str();
						csResultCode = csWireShortToDieCode;
						nFailWireShortToDieCount++;
						break;
					case 408:
						csOffPadCode = theApp.defctCodeArr[18].c_str();
						csResultCode = csOffPadCode;
						nFailOffPadCount++;
						break;
					case 501:
						csMissingBallBondPadCode = theApp.defctCodeArr[19].c_str();
						csResultCode = csMissingBallBondPadCode;
						nFailMissingBallBondPadCount++;
						break;
					case 502:
						csBallLocationBondPadCode = theApp.defctCodeArr[20].c_str();
						csResultCode = csBallLocationBondPadCode;
						nFailBallLocationBondPadCount++;
						break;
					case 503:
						csBallDiameterBondPadCode = theApp.defctCodeArr[21].c_str();
						csResultCode = csBallDiameterBondPadCode;
						nFailBallDiameterBondPadCount++;
						break;
					case 504:
						csStitchDimensionCode = theApp.defctCodeArr[22].c_str();
						csResultCode = csStitchDimensionCode;
						nFailStitchDimensionCount++;
						break;
					case 505:
						csStitchQualityCode = theApp.defctCodeArr[23].c_str();
						csResultCode = csStitchQualityCode;
						nFailStitchQualityCount++;
						break;
					case 506:
						csStudBallCode = theApp.defctCodeArr[24].c_str();
						csResultCode = csStudBallCode;
						nFailStudBallCount++;
						break;
					case 508:
						csSmashBondCode = theApp.defctCodeArr[25].c_str();
						csResultCode = csSmashBondCode;
						nFailSmashBondCount++;
						break;
					case 509:
						csLiftedBallCode = theApp.defctCodeArr[26].c_str();
						csResultCode = csLiftedBallCode;
						nFailLiftedBallCount++;
						break;
					case 510:
						csHRBOffPadCode = theApp.defctCodeArr[27].c_str();
						csResultCode = csHRBOffPadCode;
						nFailHRBOffPadCount++;
						break;
					case 511:
						csUnitClubBallCode = theApp.defctCodeArr[28].c_str();
						csResultCode = csUnitClubBallCode;
						nFailUnitClubBallCount++;
						break;
					case 512:
						csDeviceClubBallCode = theApp.defctCodeArr[29].c_str();
						csResultCode = csDeviceClubBallCode;
						nFailDeviceClubBallCount++;
						break;
					}
				}
			}
			else {
				csResultCode = csMapPredefineFailCode;
				nMapPredefineFailCount++;
			}

			csRowData[y] += csResultCode;
		}
		csTemp = "					<BinCode>" + csRowData[y] + "</BinCode>\n";
		csEntireRowCode += csTemp;
	}

	csPassBinCount.Format("%d", nPassBinCount);
	csFailBinCount.Format("%d", nMapPredefineFailCount);
	csNotProcessBinCount.Format("%d", nNotProcessBinCount);
	csFailSubstrateLocationCount.Format("%d", nFailSubstrateLocationCount);
	csFailInkDieCount.Format("%d", nFailInkDieCount);
	csFailDieLocationCount.Format("%d", nFailDieLocationCount);
	csFailDieOffsetCount.Format("%d", nFailDieOffsetCount);
	csFailDieRotationCount.Format("%d", nFailDieRotationCount);
	csFailEpoxyCount.Format("%d", nFailEpoxyCount);
	csFailPVIUnitSurfaceCount.Format("%d", nFailPVIUnitSurfaceCount);
	csFailPVIDeviceCount.Format("%d", nFailPVIDeviceCount);
	csFailPVIUnitEdgeCount.Format("%d", nFailPVIUnitEdgeCount);
	csFailMissingWireCount.Format("%d", nFailMissingWireCount);
	csFailWireCount.Format("%d", nFailWireCount);
	csFailSaggingWireCount.Format("%d", nFailSaggingWireCount);
	csFailWireLoopHeightCount.Format("%d", nFailWireLoopHeightCount);
	csFailWireLengthCount.Format("%d", nFailWireLengthCount);
	csFailWireShortToDieCount.Format("%d", nFailWireShortToDieCount);
	csFailOffPadCount.Format("%d", nFailOffPadCount);
	csFailMissingBallBondPadCount.Format("%d", nFailMissingBallBondPadCount);
	csFailBallLocationBondPadCount.Format("%d", nFailBallLocationBondPadCount);
	csFailBallDiameterBondPadCount.Format("%d", nFailBallDiameterBondPadCount);
	csFailStitchDimensionCount.Format("%d", nFailStitchDimensionCount);
	csFailStitchQualityCount.Format("%d", nFailStitchQualityCount);
	csFailStudBallCount.Format("%d", nFailStudBallCount);
	csFailSmashBondCount.Format("%d", nFailSmashBondCount);
	csFailHRBOffPadCount.Format("%d", nFailHRBOffPadCount);
	csFailUnitClubBallCount.Format("%d", nFailUnitClubBallCount);
	csFailDeviceClubBallCount.Format("%d", nFailDeviceClubBallCount);

	CString csSV_Val =
		"<?xml version='1.0' encoding='utf-8' ?>\n" 
		"<MapData xmlns=\"urn:semi-org:xsd.E142-1.V0805.SubstrateMap\">\n" 
		"	<Layouts>\n" 
		"		<Layout LayoutId=\"SubstrateLayout\" TopLevel=\"true\" DefaultUnits=\"mm\">\n" 
		"			<Dimension X=\"" + csChipDimensionX + "\" Y=\"" + csChipDimensionY + "\"/>\n"
		"			<ChildLayouts>\n" 
		"				<ChildLayout LayoutId=\"UnitLayout\"/>\n" 
		"			</ChildLayouts>\n" 
		"		</Layout>\n" 
		"		<Layout LayoutId=\"UnitLayout\" DefaultUnits=\"mm\">\n" 
		"			<Dimension X=\"" + csUnitDimensionX + "\" Y=\"" + csUnitDimensionY + "\"/>\n"
		"		</Layout>\n" 
		"	</Layouts>\n" 
		"	<Substrates>\n" 
		"		<Substrate SubstrateType=\"Strip\" SubstrateId=\"" + substrateBarcodeID + "\"/>\n" 
		"	</Substrates>\n" 
		"	<SubstrateMaps>\n" 
		"		<SubstrateMap SubstrateType=\"Strip\" SubstrateId=\"" + substrateBarcodeID + "\" LayoutSpecifier=\"SubstrateLayout/UnitLayout\" Orientation=\"" + strOrientation + "\"" 
		" OriginLocation=\"" + strOriginLocation + "\" SubstrateSide=\"" + strSubstrateSide + "\" AxisDirection=\"" + strAxisDirection + "\">\n"
		"			<Overlay MapName=\"PanelMap\" MapVersion=\"1\">\n" 
		"				<BinCodeMap BinType=\"Integer2\" NullBin=\"FFFF\" MapType=\"2DArray\">\n" 
		"					<BinDefinitions>\n" 
		"						<BinDefinition BinCode=\"" + csPassBinCode + "\" BinCount=\"" + csPassBinCount + "\" BinQuality=\"Pass\" BinDescription=\"Tested Ok\"/>\n"
		"						<BinDefinition BinCode=\"" + csMapPredefineFailCode + "\" BinCount=\"" + csFailBinCount + "\" BinQuality=\"Fail\" BinDescription=\"Tested Failed\"/>\n"
		"						<BinDefinition BinCode=\"" + csNotProcessBinCode + "\" BinCount=\"" + csNotProcessBinCount + "\" BinQuality=\"Unknown\" BinDescription=\"NotProcessed\"/>\n"
		"						<BinDefinition BinCode=\"" + csSubstrateLocationCode + "\" BinCount=\"" + csFailSubstrateLocationCount + "\" BinQuality=\"Fail\" BinDescription=\"SubstrateLocation\"/>\n"
		"						<BinDefinition BinCode=\"" + csInkDieCode + "\" BinCount=\"" + csFailInkDieCount + "\" BinQuality=\"Fail\" BinDescription=\"InkDie\"/>\n"
		"						<BinDefinition BinCode=\"" + csDieLocationCode + "\" BinCount=\"" + csFailDieLocationCount + "\" BinQuality=\"Fail\" BinDescription=\"DieLocation\"/>\n"
		"						<BinDefinition BinCode=\"" + csDieOffsetCode + "\" BinCount=\"" + csFailDieOffsetCount + "\" BinQuality=\"Fail\" BinDescription=\"DieOffset\"/>\n"
		"						<BinDefinition BinCode=\"" + csDieRotationCode + "\" BinCount=\"" + csFailDieRotationCount + "\" BinQuality=\"Fail\" BinDescription=\"DieRotation\"/>\n"
		"						<BinDefinition BinCode=\"" + csEpoxyCode + "\" BinCount=\"" + csFailEpoxyCount + "\" BinQuality=\"Fail\" BinDescription=\"Epoxy\"/>\n"
		"						<BinDefinition BinCode=\"" + csPVIUnitSurfaceCode + "\" BinCount=\"" + csFailPVIUnitSurfaceCount + "\" BinQuality=\"Fail\" BinDescription=\"PVIUnitSurface\"/>\n"
		"						<BinDefinition BinCode=\"" + csPVIDeviceCode + "\" BinCount=\"" + csFailPVIDeviceCount + "\" BinQuality=\"Fail\" BinDescription=\"PVIDevice\"/>\n"
		"						<BinDefinition BinCode=\"" + csPVIUnitEdgeCode + "\" BinCount=\"" + csFailPVIUnitEdgeCount + "\" BinQuality=\"Fail\" BinDescription=\"PVIUnitEdge\"/>\n"
		"						<BinDefinition BinCode=\"" + csMissingWireCode + "\" BinCount=\"" + csFailMissingWireCount + "\" BinQuality=\"Fail\" BinDescription=\"MissingWire\"/>\n"
		"						<BinDefinition BinCode=\"" + csWireCode + "\" BinCount=\"" + csFailWireCount + "\" BinQuality=\"Fail\" BinDescription=\"Wire\"/>\n"
		"						<BinDefinition BinCode=\"" + csSaggingWireCode + "\" BinCount=\"" + csFailSaggingWireCount + "\" BinQuality=\"Fail\" BinDescription=\"SaggingWire\"/>\n"
		"						<BinDefinition BinCode=\"" + csWireLoopHeightCode + "\" BinCount=\"" + csFailWireLoopHeightCount + "\" BinQuality=\"Fail\" BinDescription=\"LoopHeight\"/>\n"
		"						<BinDefinition BinCode=\"" + csWireLengthCode + "\" BinCount=\"" + csFailWireLengthCount + "\" BinQuality=\"Fail\" BinDescription=\"WireLength\"/>\n"
		"						<BinDefinition BinCode=\"" + csWireShortToDieCode + "\" BinCount=\"" + csFailWireShortToDieCount + "\" BinQuality=\"Fail\" BinDescription=\"ShortToDie\"/>\n"
		"						<BinDefinition BinCode=\"" + csOffPadCode + "\" BinCount=\"" + csFailOffPadCount + "\" BinQuality=\"Fail\" BinDescription=\"OffPad\"/>\n"
		"						<BinDefinition BinCode=\"" + csMissingBallBondPadCode + "\" BinCount=\"" + csFailMissingBallBondPadCount + "\" BinQuality=\"Fail\" BinDescription=\"MissingBallBondPad\"/>\n"
		"						<BinDefinition BinCode=\"" + csBallLocationBondPadCode + "\" BinCount=\"" + csFailBallLocationBondPadCount + "\" BinQuality=\"Fail\" BinDescription=\"BallLocationBondPad\"/>\n"
		"						<BinDefinition BinCode=\"" + csBallDiameterBondPadCode + "\" BinCount=\"" + csFailBallDiameterBondPadCount + "\" BinQuality=\"Fail\" BinDescription=\"BallDiameterBondPad\"/>\n"
		"						<BinDefinition BinCode=\"" + csStitchDimensionCode + "\" BinCount=\"" + csFailStitchDimensionCount + "\" BinQuality=\"Fail\" BinDescription=\"StitchDimension\"/>\n"
		"						<BinDefinition BinCode=\"" + csStitchQualityCode + "\" BinCount=\"" + csFailStitchQualityCount + "\" BinQuality=\"Fail\" BinDescription=\"StitchQuality\"/>\n"
		"						<BinDefinition BinCode=\"" + csStudBallCode + "\" BinCount=\"" + csFailStudBallCount + "\" BinQuality=\"Fail\" BinDescription=\"StudBall\"/>\n"
		"						<BinDefinition BinCode=\"" + csSmashBondCode + "\" BinCount=\"" + csFailSmashBondCount + "\" BinQuality=\"Fail\" BinDescription=\"SmashBond\"/>\n"
		"						<BinDefinition BinCode=\"" + csHRBOffPadCode + "\" BinCount=\"" + csFailHRBOffPadCount + "\" BinQuality=\"Fail\" BinDescription=\"HRBOffPad\"/>\n"
		"						<BinDefinition BinCode=\"" + csUnitClubBallCode + "\" BinCount=\"" + csFailUnitClubBallCount + "\" BinQuality=\"Fail\" BinDescription=\"UnitClubBall\"/>\n"
		"						<BinDefinition BinCode=\"" + csDeviceClubBallCode + "\" BinCount=\"" + csFailDeviceClubBallCount + "\" BinQuality=\"Fail\" BinDescription=\"DeviceClubBall\"/>\n"
		"					</BinDefinitions>\n" +
		csEntireRowCode +
		"				</BinCodeMap>\n" 
		"			</Overlay>\n" 
		"		</SubstrateMap>\n" 
		"	</SubstrateMaps>\n" 
		"</MapData>";

	CString csSV_Name = "ResultMap";
	HRESULT result = this->getController()->SetAttribute(_bstr_t(csSV_Name), AttributeType_DV, _bstr_t(csSV_Val));

	//// Save result file to disk
	//CStdioFile file;
	//CString strFileName = "d:\\mapdata\\result_" + substrateBarcodeID + ".txt";
	//if (file.Open(strFileName, CFile::modeCreate | CFile::modeWrite)) {
	//	file.WriteString(csSV_Name);
	//	file.WriteString("\n");
	//}
}

// Original with only 3 bincode types (Pass/Fail/Not Inspect)
void SecsGemController::UpdateSubstrateResults2(CString substrateBarcodeID)
{
	CString csPassBinCount, csFailBinCount, csNotProcessBinCount;
	CString csPassBinCode, csFailBinCode, csNotProcessBinCode;
	CString csChipDimensionX, csChipDimensionY;
	CString csUnitDimensionX, csUnitDimensionY;
	CString *csRowData;
	CString csResultCode;
	CString csEntireRowCode;
	CString csTemp;
	CString strOrientation = mapData.strOrientation.c_str();
	CString strOriginLocation = mapData.strOriginLocation.c_str();
	CString strSubstrateSide = "TopSide";	// mapData.strSubstrateSide.c_str();
	CString strAxisDirection = mapData.strAxisDirection.c_str();

	int nPassBinCount, nFailBinCount, nNotProcessBinCount;
	int nChipDimensionX, nChipDimensionY;
	int nUnitDimensionX, nUnitDimensionY;

	nPassBinCount = 0;
	nFailBinCount = 0;
	nNotProcessBinCount = 0;

	nChipDimensionX = mapData.nSubstrateColumnCount;	// 100;
	nChipDimensionY = mapData.nSubstrateRowCount;		// 120;

	csChipDimensionX.Format("%d", nChipDimensionX);
	csChipDimensionY.Format("%d", nChipDimensionY);

	nUnitDimensionX = mapData.nUnitColumnCount;		// 1000;
	nUnitDimensionY = mapData.nUnitRowCount;		// 1200;

	csUnitDimensionX.Format("%d", nUnitDimensionX);
	csUnitDimensionY.Format("%d", nUnitDimensionY);

	csPassBinCode = "0000";
	csFailBinCode = "5000";
	csNotProcessBinCode = "AAAA";


	int nTotalChip = nChipDimensionX * nChipDimensionY;
	int nTotalUnitChip = nUnitDimensionX * nUnitDimensionY;

	for (int nChip = 0; nChip < nTotalChip; nChip++) {
		csRowData = new CString[mapData.nUnitRowCount];
		for (int y = 0; y < mapData.nUnitRowCount; y++)
		{
			for (int x = 0; x < mapData.nUnitColumnCount; x++)
			{
				if (/*mapData.scanning2D3DImageEnableInspection[y * mapData.nUnitColumnCount + x]*/TRUE) {
					if ((mapData.inspectionResultMap[y * mapData.nUnitColumnCount + x + nTotalUnitChip* nChip]) == GOOD_UNIT)
					{
						csResultCode = csPassBinCode;
						nPassBinCount++;
					}
					else if ((mapData.inspectionResultMap[y * mapData.nUnitColumnCount + x + nTotalUnitChip * nChip]) == NOT_INSPECTED)
					{
						csResultCode = csNotProcessBinCode;
						nNotProcessBinCount++;
					}
					else
					{
						csResultCode = csFailBinCode;
						nFailBinCount++;
					}
				}
				else {
					csResultCode = csNotProcessBinCode;
					nNotProcessBinCount++;
				}

				csRowData[y] += csResultCode;
			}
			csTemp = "					<BinCode>" + csRowData[y] + "</BinCode>\n";
			csEntireRowCode += csTemp;
		}
	}
	

	csPassBinCount.Format("%d", nPassBinCount);
	csFailBinCount.Format("%d", nFailBinCount);
	csNotProcessBinCount.Format("%d", nNotProcessBinCount);

	CString csSV_Val =
		"<?xml version='1.0' encoding='utf-8' ?>\n"
		"<MapData xmlns=\"urn:semi-org:xsd.E142-1.V0805.SubstrateMap\">\n"
		"	<Layouts>\n"
		"		<Layout LayoutId=\"SubstrateLayout\" TopLevel=\"true\" DefaultUnits=\"mm\">\n"
		"			<Dimension X=\"" +  csChipDimensionX + "\" Y=\"" + csChipDimensionY + "\"/>\n"
		"			<ChildLayouts>\n"
		"				<ChildLayout LayoutId=\"UnitLayout\"/>\n"
		"			</ChildLayouts>\n"
		"		</Layout>\n"
		"		<Layout LayoutId=\"UnitLayout\" DefaultUnits=\"mm\">\n"
		"			<Dimension X=\"" + csUnitDimensionX + "\" Y=\"" + csUnitDimensionY + "\"/>\n"
		"		</Layout>\n"
		"	</Layouts>\n"
		"	<Substrates>\n"
		"		<Substrate SubstrateType=\"Strip\" SubstrateId=\"" + substrateBarcodeID + "\"/>\n"
		"	</Substrates>\n"
		"	<SubstrateMaps>\n"
		"		<SubstrateMap SubstrateType=\"Strip\" SubstrateId=\"" + substrateBarcodeID + "\" LayoutSpecifier=\"SubstrateLayout/UnitLayout\" Orientation=\"" + strOrientation + "\""
		" OriginLocation=\"" + strOriginLocation + "\" SubstrateSide=\"" + strSubstrateSide + "\" AxisDirection=\"" + strAxisDirection + "\">\n"
		"			<Overlay MapName=\"PanelMap\" MapVersion=\"1\">\n"
		"				<BinCodeMap BinType=\"Integer2\" NullBin=\"FFFF\" MapType=\"2DArray\">\n"
		"					<BinDefinitions>\n"
		"						<BinDefinition BinCode=\"" + csPassBinCode + "\" BinCount=\"" + csPassBinCount + "\" BinQuality=\"Pass\" BinDescription=\"Tested Ok\"/>\n"
		"						<BinDefinition BinCode=\"" + csFailBinCode + "\" BinCount=\"" + csFailBinCount + "\" BinQuality=\"Fail\" BinDescription=\"Tested Failed\"/>\n"
		"						<BinDefinition BinCode=\"" + csNotProcessBinCode + "\" BinCount=\"" + csNotProcessBinCount + "\" BinQuality=\"Unknown\" BinDescription=\"NotProcessed\"/>\n"
		"					</BinDefinitions>\n" +
		csEntireRowCode +
		"				</BinCodeMap>\n"
		"			</Overlay>\n"
		"		</SubstrateMap>\n"
		"	</SubstrateMaps>\n"
		"</MapData>";

	CString csSV_Name = "ResultMap";
	HRESULT result = this->getController()->SetAttribute(_bstr_t(csSV_Name), AttributeType_DV, _bstr_t(csSV_Val));

	// Save result file to disk
	CStdioFile file;
	CString strFileName = "d:\\mapdata\\result_" + substrateBarcodeID + ".txt";
	if (file.Open(strFileName, CFile::modeCreate | CFile::modeWrite)) {
		file.WriteString(csSV_Val);
		file.WriteString("\n");
	}
}

void SecsGemController::StartMachineCommand()
{
	theApp.StartCommandSECSGEM();
}

void SecsGemController::StopMachineCommand()
{
	theApp.StopCommandSECSGEM();
}

void SecsGemController::PauseMachineCommand()
{
	theApp.PauseCommandSECSGEM();
}

void SecsGemController::AbortMachineCommand()
{
	theApp.AbortCommandSECSGEM();
}

int SecsGemController::StartMachineProcessSelectCommand(CString strLotId, CString strRecipe, int nSubstrateCount)
{
	bool bValidCategory = false;
	bool recipeFound = false;

	// Check recipe availiable in the Directory
	CString recipeDir = strRecipe.Left(strRecipe.GetLength() - 4);

	CString strCategories = strRecipe.Left(strRecipe.FindOneOf("\\"));

	if (strCategories.CompareNoCase("Engineering") == 0)
		bValidCategory = true;

	if(!bValidCategory && (strCategories.CompareNoCase("Production") == 0))
		bValidCategory = true;

	if (!bValidCategory && (strCategories.CompareNoCase("Qualification") == 0))
		bValidCategory = true;

	if (!bValidCategory)
		return -1;

	if (strLotId == "*null*")
		return -1;

	if (nSubstrateCount < 1)
		return -1;

	if (!theApp.RecipeFiles.size())
		theApp.ListAllProcessRecipes();

	// Check whether this recipe available in directory
	for (int i = 0; i < theApp.RecipeFiles.size(); i++)
	{
		strRecipe = theApp.RecipeFiles.at(i);
		if (recipeDir.CompareNoCase(strRecipe) == 0)
		{
			recipeFound = true;
			break;
		}
	}

	if (!recipeFound)
		return -1;

	return 0;
}