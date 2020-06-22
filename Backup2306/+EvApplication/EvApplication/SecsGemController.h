#pragma once

#include "CGEMController.h"
#include <list>
#include <atlstr.h>

#define _NO_SECSGEM
#undef _NO_SECSGEM

enum binQuality
{
	BIN_PASS,
	BIN_FAIL,
	BIN_UNKNOWN,
	BIN_QUALITY_COUNT
};

struct BinDefinitions
{
	std::string strBinDescription;
	std::string strBinQuality;
	unsigned int nBinCode;
	int nBinCount;
};

struct substrateMapData
{
	int nUnitRowCount;
	int nUnitColumnCount;
	int nSubstrateRowCount;
	int nSubstrateColumnCount;
	std::string strOrientation;
	std::string strOriginLocation;
	std::string strSubstrateSide;
	std::string strAxisDirection;
	std::string strSubstrateId;
	std::string strSubstrateType;
	std::vector< std::string > binCode;
	std::vector< unsigned int > scanning2D3DImageStatus;
	std::vector <bool > scanning2D3DImageEnableInspection;
	std::vector <BinDefinitions > BinDefinition;
	std::vector< int > inspectionResultMap;
};


class SecsGemController
{
public:
	SecsGemController();
	~SecsGemController();

	int initializeGem(bool reInitializtion = false);
	substrateMapData mapData;

	void setPath(CString equipmentModelPath, CString gemLogPath);

	ICOMGEMControllerPtr		 getController() const;
	ICOMMessageServiceManagerPtr getServiceManager() const;
	CommunicationState			 getCommunicationState() const;
	ControlState				 getControlState() const;
	ProcessingState				 getProcessingState() const;
	ICOMSECsDataItemPtr			 getSetAlarms() const;
	ICOMSECsDataItemPtr			 getEnabledAlarms() const;
	ICOMSECsDataItemPtr			 getDisabledEvents() const;
	ICOMSECsDataItemPtr			 getEnabledEvents() const;
	ICOMSECsDataItemPtr			 getAllStatusVariables() const;
	ICOMSECsDataItemPtr			 getAllEquipmentConstants() const;
	ICOMSECsDataItemPtr			 getAllDataVariables() const;
	SpoolingState				 getSpoolingState() const;
	SpoolingSubState			 getSpoolingSubState() const;
	
	void getAllStatusVariablesFromXml(); // Get all the status variables from the EquipmentModel
	void getAllEquipmentConstantsFromXml(); // Get all the equipment constants from the EquipmentModel
	void getAllDataCollectionsFromXml(); //Get all the data collections from the EquipmentModel

	void						 activateEventReport(CString eventName, bool enable);
	void						 sendEventReport(CString eventName);
	void						 setAlarm(CString alarmName);
	void						 clearAlarm(CString alarmName);
	void						 setLocal();
	void						 setRemote();
	void						 setOnline();
	void						 setOffline();
	void						 setEnable();
	void						 setDisable();
	void						 RequestMapDataFromHost(CString substrateBarcodeID);
	void						 UpdateSubstrateResults(CString substrateBarcodeID);
	void						 UpdateSubstrateResults2(CString substrateBarcodeID);
	int							 readMapDataFromHost(std::string filePath);			//Girish
	int							 getMappingDetails(std::string &stripId, std::string &stripType, int &nRowCount, int &nColumnCount );
	int							 getMappingInspectionEnableStatus(std::vector <bool> &scanning2D3DImageEnableInspection);

	void						 setStatusVariablesValue(const CString name, const CString value);
	CString						 getStatusVariablesValue(const CString name);
	void						 setDataVariablesValue(const CString name, const CString value);
	CString						 getDataVariablesValue(const CString name);
	void						 setEquipmentConstantValue(const CString name, const CString value);
	CString						 getEquipmentConstantValue(const CString name);

	std::list< CString >&		 getTerminalString();
	void						 appendTerminalString(CString info);
	void						 clearTerminalString();
	void						 activateTerminalInformationDisplay(bool enable);
	//Send terminal message request to Host via S10F1/F2
	void						SendTerminalMessageToHost(CString message);
	void						requestHostDateTime(CString &hostDateTime);
/////////////////////////////////////////////////////////////////////////////
private:
	ICOMGEMControllerPtr m_GEMController; // SecsToHost.Net local variables
	CString _equipmentModelPath;
	CString _gemLogPath;
	std::list < CString > _terminalString;
	bool	_enableTerminalDisplay;

/////////////////////////////////////////////////////////////////////////////
// Some helper methods
	int InitializeSecsToHost();
	int ReInitializeSecsToHost();
	void HandleTerminalDisplay(ICOMSECsPrimaryInEventArgs& args);
	void HandleRemoteCommand(ICOMSECsPrimaryInEventArgs& args);
	void HandleHostCommand(ICOMSECsPrimaryInEventArgs& args);
	void HandleProcessProgramRequest(ICOMSECsPrimaryInEventArgs& args);
	void OnHostRecipeDirList(ICOMSECsPrimaryInEventArgs& args);
	void HandleNewEquipmentConstantRequest(ICOMSECsPrimaryInEventArgs& args);
	void HandleProcessProgramLoadInquire(ICOMSECsPrimaryInEventArgs& args);
	void UpdateEquipmentConstantValue(long Id, CString value);
	void StartMachineCommand();
	void StopMachineCommand();
	void PauseMachineCommand();
	void AbortMachineCommand();
	int StartMachineProcessSelectCommand(CString strLotId, CString strRecipe, int nSubstrateCount);

	//From equipment perspective to send the custom message and handle its secondary reply/acknowledgement
	void CustomMessageGeneration();	//sample code to generate custom message
	void OnCustomMessageReply(ICOMSECsSecondaryInEventArgs &args);
	void HandleS6F102(ICOMSECsMessagePtr output);				//Handle custom generated message

	//Handle custom message send by the Host side and send the reply accordingly
	void OnCustomMessageRequest(ICOMSECsPrimaryInEventArgs& args);
	void HandleS101F1(ICOMSECsMessagePtr input, int transactionId);

	//Send/Received Array Data item
	void sendArrayItem();
	void HandleIncomingArrayMessage(ICOMSECsPrimaryInEventArgs& args);

	// Host initiates process program upload
	void OnHostRecipeUploadRequest(ICOMSECsPrimaryInEventArgs &args);
	void OnHostRecipeDownload(ICOMSECsPrimaryInEventArgs &args);
	void HandleProcessProgramDeleteRequest(ICOMSECsPrimaryInEventArgs &args);

/////////////////////////////////////////////////////////////////////////////
// Callback handlers
	// Callback handler for CSECsPrimaryInEvent
	void CSECsPrimaryInEventHandler(CGEMController &sender, ICOMSECsPrimaryInEventArgs &args);

	// Callback handler for CSECsSecondaryInEvent
	void CSECsSecondaryInEventHandler(CGEMController &sender, ICOMSECsSecondaryInEventArgs &args);

	// Callback handler for CCommunicationStateChangedEvent
	void CCommunicationStateChangedEventHandler(CGEMController &sender, ICOMSECsEventArgs &args);

	// Callback handler for CControlStateChangedEvent
	void CControlStateChangedEventHandler(CGEMController &sender, ICOMSECsEventArgs &args);

	// Callback handler for CProcessingStateChangedEvent
	void CProcessingStateChangedEventHandler(CGEMController &sender, ICOMSECsEventArgs &args);

	// Callback handler for CMessageActivityTraceEvent
	void CMessageActivityTraceEventHandler(CGEMController &sender, ICOMSECsMessageActivityEventArgs &args);

	// Callback handler for CSpoolingStateChangedEvent
	void CSpoolingStateChangedEventHandler(CGEMController &sender, ICOMSECsEventArgs &args);

	// Callback handler for CClockChangedEvent
	void CClockChangedEventHandler(CGEMController &sender, ICOMSECsClockEventArgs &args);

	// Callback handler for CRemoteCommandInEvent
	void CRemoteCommandInEventHandler(CGEMController &sender, ICOMSECsRemoteCommandEventArgs &args);
};


std::string  ConvertBSTRToMBS(BSTR bstr);