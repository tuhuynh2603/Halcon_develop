
#include "StdAfx.h"
#include "CGEMController.h"

CGEMController::CGEMController(void)
{
	HRESULT hr = CoInitialize(0);
	// Initialize COM Interface
	//m_GEMController = ICOMGEMControllerPtr(__uuidof(COMGEMController));
	HRESULT hRes = m_GEMController.CreateInstance(CLSID_COMGEMController);

	try {

	// Subscribe to GEMController callbacks
	m_GEMController->SubscribeCallback(this);
	} catch (_com_error &e)
	{
		LPCTSTR lpErrorDescription = e.Description();
		LPCTSTR lpErrorMessage = e.ErrorMessage();
		CString strMsg;
		strMsg.Format("SECSGEM Initialization Error1 = %s Error2 = %s", lpErrorMessage, lpErrorDescription);
		AfxMessageBox(strMsg);
		
	}

}

CGEMController::~CGEMController(void)
{
	try {
		m_GEMController->Dispose();
	} catch (_com_error &e)
	{
		LPCTSTR lpErrorDescription = e.Description();
		LPCTSTR lpErrorMessage = e.ErrorMessage();
	}
}

HRESULT CGEMController::QueryInterface(const IID & iid,void ** pp)
{
    if (iid == __uuidof(ICOMGEMControllerEvents) || iid == __uuidof(IUnknown))
    {
        *pp = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

HRESULT CGEMController::raw_MessageActivityTrace(VARIANT sender, ICOMSECsMessageActivityEventArgs* e)
{
	for(CMessageActivityTraceEvent::iterator it = CMessageActivityTraceEventCallback.begin(); it != CMessageActivityTraceEventCallback.end(); it++) {
		(*(*it))(*this, *e);
	}
	return 0;
}

HRESULT CGEMController::raw_PrimaryMessageIn(VARIANT sender, ICOMSECsPrimaryInEventArgs* e)
{
	for(CSECsPrimaryInEvent::iterator it = CSECsPrimaryInEventCallback.begin(); it != CSECsPrimaryInEventCallback.end(); it++) {
		(*(*it))(*this, *e);
	}
	return 0;
}

HRESULT CGEMController::raw_SecondaryMessageIn(VARIANT sender, ICOMSECsSecondaryInEventArgs* e)
{
	for(CSECsSecondaryInEvent::iterator it = CSECsSecondaryInEventCallback.begin(); it != CSECsSecondaryInEventCallback.end(); it++) {
		(*(*it))(*this, *e);
	}
	return 0;
}

HRESULT CGEMController::raw_CommunicationStateChanged(VARIANT sender, ICOMSECsEventArgs* e)
{
	for(CCommunicationStateChangedEvent::iterator it = CCommunicationStateChangedEventCallback.begin(); it != CCommunicationStateChangedEventCallback.end(); it++) {
		(*(*it))(*this, *e);
	}
	/*CommunicationState commState = m_GEMController->GetCommunicationState();
	
	switch (commState) {
	case CommunicationState_Enabled:
		printf("ENABLED");
		break;
	case CommunicationState_Disabled:
		printf("DISABLED");
		break;

	case CommunicationState_EnabledNotCommunicating:
		printf("ENABLED/NOT COMMUNICATING");
		break;

	case CommunicationState_EnabledCommunicating:
		printf("ENABLED/COMMUNICATING");
		break;
	case CommunicationState_WaitCRA:
		printf("WAITCRA");
		break;
	
	case CommunicationState_WaitDelay:
		printf("WAITDELAY");
		break;
	}*/

	return 0;
}
HRESULT CGEMController::raw_ControlStateChanged(VARIANT sender, ICOMSECsEventArgs* e)
{
	for(CControlStateChangedEvent::iterator it = CControlStateChangedEventCallback.begin(); it != CControlStateChangedEventCallback.end(); it++) {
		(*(*it))(*this, *e);
	}
	return 0;
}
HRESULT CGEMController::raw_ProcessingStateChanged(VARIANT sender, ICOMSECsEventArgs* e)
{
	for(CProcessingStateChangedEvent::iterator it = CProcessingStateChangedEventCallback.begin(); it != CProcessingStateChangedEventCallback.end(); it++) {
		(*(*it))(*this, *e);
	}
	return 0;
}

HRESULT CGEMController::raw_SpoolingStateChanged(VARIANT sender, ICOMSECsEventArgs* e)
{
	for(CSpoolingStateChangedEvent::iterator it = CSpoolingStateChangedEventCallback.begin(); it != CSpoolingStateChangedEventCallback.end(); it++) {
		(*(*it))(*this, *e);
	}
	return 0;
}

HRESULT CGEMController::raw_ClockChanged(VARIANT sender, ICOMSECsClockEventArgs* e)
{
	for(CClockChangedEvent::iterator it = CClockChangedEventCallback.begin(); it != CClockChangedEventCallback.end(); it++) {
		(*(*it))(*this, *e);
	}
	return 0;
}

HRESULT CGEMController::raw_RemoteCommandIn(VARIANT sender, ICOMSECsRemoteCommandEventArgs* e)
{
	for(CRemoteCommandInEvent::iterator it = CRemoteCommandInEventCallback.begin(); it != CRemoteCommandInEventCallback.end(); it++) {
		(*(*it))(*this, *e);
	}
	
	/*_bstr_t lpCommandLogicalName = e->GetCommandLogicalName();
	_bstr_t lpCommandName = e->GetCommandName();

	RemoteCommandType iCommandType = e->GetCommandType();

	
	
	if (iCommandType == RemoteCommandType_S2F41HostCommand) {
		ICommandParameterCollectionModelPtr m_CommandParameters =  e->GetCommandParameters();

		long lParametersCount = m_CommandParameters->GetCount();
	
		for(long i = 0; i<lParametersCount; i++) {
			ICommandParameterModelPtr m_CommandParameter = m_CommandParameters->GetItem(i);
		
			_bstr_t commandLogicalName = m_CommandParameter->GetlogicalName();
			_bstr_t commandName = m_CommandParameter->Getname();

			SECSFormats commandFormat = m_CommandParameter->GetparameterType();
		}
	}*/
	return 0;
}

ICOMGEMControllerPtr CGEMController::GetGEMController() {
	return m_GEMController;
}


