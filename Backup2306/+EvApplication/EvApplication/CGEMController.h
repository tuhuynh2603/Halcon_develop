#include <iostream>
#include <vector>
using namespace std;

	template<typename _Arg1, typename _Arg2> struct ClosureBase {
		virtual void operator ()(_Arg1 &arg1, _Arg2 &arg2) = 0;
	};
	
	template<class _Ty, typename _Arg1, typename _Arg2> class Closure : public ClosureBase<_Arg1, _Arg2> {
		void (_Ty::* method_)(_Arg1 &, _Arg2 &);
		_Ty *objectPtr_;
	
	public:
		Closure(_Ty *objectPtr, void (_Ty::* method)(_Arg1 &, _Arg2 &)) : objectPtr_(objectPtr) { method_ = method; }
		virtual ~Closure() { }
		virtual void operator ()(_Arg1 &arg1, _Arg2 &arg2) { return (objectPtr_->*method_)(arg1, arg2); }
	};

	// Create a generator for Closure objects to get rid of template-parameters
	template<class _Ty, typename _Arg1, typename _Arg2> 
		inline Closure<_Ty, _Arg1, _Arg2> *CreateClosure(_Ty *object, void (_Ty::*method)(_Arg1 &, _Arg2 &) ) {
		return new Closure<_Ty, _Arg1, _Arg2>(object, method);
	}		

#pragma once
#import "..\\..\\Peripherals\\SecsGem\\Include\\Insphere.Connectivity.Common.tlb" no_namespace 
#import "..\\..\\Peripherals\\SecsGem\\Include\\Insphere.Connectivity.Common.ToolModel.tlb" no_namespace 
#import "..\\..\\Peripherals\\SecsGem\\Include\\Insphere.Connectivity.Application.MessageServices.tlb" no_namespace 
#import "..\\..\\Peripherals\\SecsGem\\Include\\Insphere.Connectivity.SecsToHost.Net.tlb" no_namespace named_guids

/*Need to Install proper Secs To Host SDK  
--> Go to SDK Installed Folder 
--> Run RegisterCOM_x64.exe to generate *.tlb file 
--> Copy *tlb genarated file to "Peripherals\SecsGem\Include\" Folder
*/

class CGEMController : public ICOMGEMControllerEvents
{
private:
	ICOMGEMControllerPtr m_GEMController;
	
public:
	__declspec(dllexport) CGEMController(void);
public:
	__declspec(dllexport) ~CGEMController(void);

private:
	// Declaring the event stub
	 HRESULT __stdcall QueryInterface(const IID &, void **);
     ULONG __stdcall AddRef(void) { return 1; }
     ULONG __stdcall Release(void) { return 1; }

	 HRESULT __stdcall raw_PrimaryMessageIn( VARIANT sender,  ICOMSECsPrimaryInEventArgs* e);
	 HRESULT __stdcall raw_SecondaryMessageIn( VARIANT sender,  ICOMSECsSecondaryInEventArgs* e);
	 HRESULT __stdcall raw_CommunicationStateChanged( VARIANT  sender,  ICOMSECsEventArgs* e);
	 HRESULT __stdcall raw_ControlStateChanged( VARIANT sender,  ICOMSECsEventArgs* e);
	 HRESULT __stdcall raw_ProcessingStateChanged( VARIANT sender,  ICOMSECsEventArgs* e);
	 HRESULT __stdcall raw_MessageActivityTrace( VARIANT sender,  ICOMSECsMessageActivityEventArgs* e);
	 HRESULT __stdcall raw_SpoolingStateChanged( VARIANT sender,  ICOMSECsEventArgs* e);
	 HRESULT __stdcall raw_ClockChanged( VARIANT sender,  ICOMSECsClockEventArgs* e);
	 HRESULT __stdcall raw_RemoteCommandIn( VARIANT sender,  ICOMSECsRemoteCommandEventArgs* e);

public:
	ICOMGEMControllerPtr __declspec(dllexport) GetGEMController();

public:
	typedef vector<ClosureBase<CGEMController, ICOMSECsPrimaryInEventArgs> *> CSECsPrimaryInEvent; // 
	CSECsPrimaryInEvent  CSECsPrimaryInEventCallback; // The delegates list. Define one per event

	typedef vector<ClosureBase<CGEMController, ICOMSECsSecondaryInEventArgs> *> CSECsSecondaryInEvent; // 
	CSECsSecondaryInEvent CSECsSecondaryInEventCallback; // The delegates list. Define one per event
	
	typedef vector<ClosureBase<CGEMController, ICOMSECsEventArgs> *> CCommunicationStateChangedEvent; // 
	CCommunicationStateChangedEvent CCommunicationStateChangedEventCallback; // The delegates list. Define one per event

	typedef vector<ClosureBase<CGEMController, ICOMSECsEventArgs> *> CControlStateChangedEvent; // 
	CControlStateChangedEvent CControlStateChangedEventCallback; // The delegates list. Define one per event

	typedef vector<ClosureBase<CGEMController, ICOMSECsEventArgs> *> CProcessingStateChangedEvent; // 
	CProcessingStateChangedEvent CProcessingStateChangedEventCallback; // The delegates list. Define one per event

	typedef vector<ClosureBase<CGEMController, ICOMSECsMessageActivityEventArgs> *> CMessageActivityTraceEvent; // 
	CMessageActivityTraceEvent CMessageActivityTraceEventCallback; // The delegates list. Define one per event

	typedef vector<ClosureBase<CGEMController, ICOMSECsEventArgs> *> CSpoolingStateChangedEvent; // 
	CSpoolingStateChangedEvent CSpoolingStateChangedEventCallback; // The delegates list. Define one per event

	typedef vector<ClosureBase<CGEMController, ICOMSECsClockEventArgs> *> CClockChangedEvent; // 
	CClockChangedEvent CClockChangedEventCallback; // The delegates list. Define one per event

	typedef vector<ClosureBase<CGEMController, ICOMSECsRemoteCommandEventArgs> *> CRemoteCommandInEvent; // 
	CRemoteCommandInEvent CRemoteCommandInEventCallback; // The delegates list. Define one per event

};
