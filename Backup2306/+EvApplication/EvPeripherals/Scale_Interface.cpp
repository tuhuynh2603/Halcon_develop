#include "stdafx.h"
#include "Scale_Interface.h"

#include "DebugLog.h"

ScaleInterface::ScaleInterface()
{
	_hDLL = NULL;

	_lpInitScaleDLL = NULL;
	_lpFreeScaleDLL = NULL;

	_lpRegisterScaleCards = NULL;
	_lpReleaseScaleCard = NULL;

	_lpInitializeScaleChannel = NULL;

	_lpResetScaleFifo = NULL;
	_lpSetScaleFifoArray = NULL;

	_lpSetScaleEncoderValue = NULL;
	_lpGetScaleEncoderValue = NULL;

	_lpGetScaleIntCount = NULL;

	_lpSetScaleLatch = NULL;
	_lpGetScaleLatchValue = NULL;

	_manualTrigger = NULL;
}

int ScaleInterface::LoadScaleDll(CString strScaleCard)
{
	if(strScaleCard.CompareNoCase("NONE") != 0 && strScaleCard.CompareNoCase("NONE.DLL") != 0) {

		OutputDebugLogTo(0, TRUE, "Registering Linear Scale Card %s ...", strScaleCard);

		_hDLL = AfxLoadLibrary(strScaleCard);

		if(_hDLL==NULL) {
			AfxMessageBox("Error Loading Linear Scale Library");
			return -1;
		}

		_lpInitScaleDLL = (INIT_SCALE_DLL) GetProcAddress (_hDLL, "InitializeScaleLibrary");
		if(!_lpInitScaleDLL)
			OutputDebugLogTo(0, TRUE, "Failed to register IO Card InitializeScaleLibrary() Function:%d", GetLastError());


		_lpFreeScaleDLL = (FREE_SCALE_DLL) GetProcAddress (_hDLL, "FreeScaleLibrary");
		if(!_lpFreeScaleDLL)
			OutputDebugLogTo(0, TRUE, "Failed to register IO Card FreeScaleLibrary() Function:%d", GetLastError());


		_lpRegisterScaleCards = (REGISTER_SCALE_CARDS) GetProcAddress (_hDLL, "RegisterScaleCards");
		if(!_lpRegisterScaleCards)
			OutputDebugLogTo(0, TRUE, "Failed to register RegisterScaleCards() Function:%d", GetLastError());

		_lpReleaseScaleCard = (RELEASE_SCALE_CARD) GetProcAddress (_hDLL, "ReleaseScaleCard");
		if(!_lpReleaseScaleCard)
			OutputDebugLogTo(0, TRUE, "Failed to register ReleaseScaleCard() Function:%d", GetLastError());

		_lpInitializeScaleChannel = (INITIALIZE_SCALE_CHANNEL) GetProcAddress (_hDLL, "InitializeScaleChannel");
		if(!_lpInitializeScaleChannel)
			OutputDebugLogTo(0, TRUE, "Failed to register InitializeScaleChannel() Function:%d", GetLastError());

		_lpResetScaleFifo = (RESET_SCALE_FIFO) GetProcAddress (_hDLL, "ResetScaleFifo");
		if(!_lpResetScaleFifo)
			OutputDebugLogTo(0, TRUE, "Failed to register ResetScaleFifo() Function:%d", GetLastError());

		_lpSetScaleFifoArray = (SET_SCALE_FIFO_ARRAY) GetProcAddress (_hDLL, "SetScaleFifoArray");
		if(!_lpSetScaleFifoArray)
			OutputDebugLogTo(0, TRUE, "Failed to register SetScaleFifoArray() Function:%d", GetLastError());

		_lpSetScaleEncoderValue = (SET_SCALE_ENCODER_VALUE) GetProcAddress (_hDLL, "SetScaleEncoderValue");
		if(!_lpSetScaleEncoderValue)
			OutputDebugLogTo(0, TRUE, "Failed to register SetScaleEncoderValue() Function:%d", GetLastError());

		_lpGetScaleEncoderValue = (GET_SCALE_ENCODER_VALUE) GetProcAddress (_hDLL, "GetScaleEncoderValue");
		if(!_lpGetScaleEncoderValue)
			OutputDebugLogTo(0, TRUE, "Failed to register GetScaleEncoderValue() Function:%d", GetLastError());

		_lpGetScaleIntCount = (GET_SCALE_INT_COUNT) GetProcAddress (_hDLL, "GetScaleIntCount");
		if(!_lpGetScaleIntCount)
			OutputDebugLogTo(0, TRUE, "Failed to register GetScaleIntCount() Function:%d", GetLastError());

		_lpSetScaleLatch = (SET_SCALE_LATCH) GetProcAddress (_hDLL, "SetScaleLatch");
		if(!_lpSetScaleLatch)
			OutputDebugLogTo(0, TRUE, "Failed to register SetScaleLatch() Function:%d", GetLastError());

		_lpGetScaleLatchValue = (GET_SCALE_LATCHVALUE) GetProcAddress (_hDLL, "GetScaleLatchValue");
		if(!_lpGetScaleLatchValue)
			OutputDebugLogTo(0, TRUE, "Failed to register GetScaleLatchValue() Function:%d", GetLastError());

		_manualTrigger = (MANUAL_TRIGGERRING) GetProcAddress (_hDLL, "ManualTrigger");
		if(!_manualTrigger)
			OutputDebugLogTo(0, TRUE, "Failed to register ManualTrigger() Function:%d", GetLastError());

		if(_lpInitScaleDLL)
			_lpInitScaleDLL();
	}
	else {
		OutputDebugLogTo(0, TRUE, "No Scale Card");
	}
	return 0;
}

int ScaleInterface::FreeScaleDll()
{
	if(_lpFreeScaleDLL)
		return _lpFreeScaleDLL();

	return 0;
}

int ScaleInterface::RegisterScaleCards()
{
	if(_lpRegisterScaleCards)
		return _lpRegisterScaleCards();
	else
		OutputDebugLogTo(0, TRUE, "No RegisterScaleCard() Function");

	return 0;
}

int ScaleInterface::ReleaseScaleCard()
{
	if(_lpReleaseScaleCard)
		return _lpReleaseScaleCard();
	else
		OutputDebugLogTo(0, TRUE, "No ReleaseScaleCard() Function");

	return 0;
}

int ScaleInterface::InitializeScaleChannel(int cardID, short channelID, short pwmChannelID, bool triggerOutRisingEdge, int pulseWidthNs, short counterSource)
{
	if(_lpInitializeScaleChannel)
		return _lpInitializeScaleChannel(cardID, channelID, pwmChannelID, triggerOutRisingEdge, pulseWidthNs, counterSource);
	else
		OutputDebugLogTo(0, TRUE, "No InitializeScaleChannel() Function");

	return 0;
}

int ScaleInterface::ResetScaleFifo(int cardID, short channelID)
{
	if(_lpResetScaleFifo)
		return _lpResetScaleFifo(cardID, channelID);
	else
		OutputDebugLogTo(1, TRUE, "No ResetScaleFifo() Function");

	return 0;
}

int ScaleInterface::SetScaleFifoArray(int cardID, short channelID, long * data, short size)
{
	if(_lpSetScaleFifoArray)
		return _lpSetScaleFifoArray(cardID, channelID, data, size);
	else
		OutputDebugLogTo(1, TRUE, "No SetScaleFifoArray() Function");

	return 0;
}

int ScaleInterface::SetScaleEncoderValue(int cardID, short channelID, long encoderValue)
{
	if(_lpSetScaleEncoderValue)
		return _lpSetScaleEncoderValue(cardID, channelID, encoderValue);
	else
		OutputDebugLogTo(1, TRUE, "No SetScaleEncoderValue() Function");

	return 0;
}

int ScaleInterface::GetScaleEncoderValue(int cardID, short channelID, long * encoderValue)
{
	if(_lpGetScaleEncoderValue)
		return _lpGetScaleEncoderValue(cardID, channelID, encoderValue);
	else
		OutputDebugLogTo(1, TRUE, "No GetScaleEncoderValue() Function");

	return 0;
}

int ScaleInterface::GetScaleIntCount(int cardID, short channelID, int * count)
{
	if(_lpGetScaleIntCount)
		return _lpGetScaleIntCount(cardID, channelID, count);
	else
		OutputDebugLogTo(1, TRUE, "No GetScaleIntCount() Function");

	return 0;
}

int ScaleInterface::SetScaleLatch(int cardID, short channelID, bool risingEdge)
{
	if(_lpSetScaleLatch)
		return _lpSetScaleLatch(cardID, channelID, risingEdge);
	else
		OutputDebugLogTo(1, TRUE, "No SetScaleLatch() Function");

	return 0;
}

int ScaleInterface::GetScaleLatchValue(int cardID, short channelID, long * value, bool waitInterrupt)
{
	if(_lpGetScaleLatchValue)
		return _lpGetScaleLatchValue(cardID, channelID, value, waitInterrupt);
	else
		OutputDebugLogTo(1, TRUE, "No GetScaleLatchValue() Function");

	return 0;
}

bool ScaleInterface::isDllOpened() const
{
	return _hDLL != NULL;
}

void ScaleInterface::closeDll()
{
	if(isDllOpened())
		AfxFreeLibrary(_hDLL);
}

int ScaleInterface::manualTrigger(int cardId, short channelId)
{
	if(_manualTrigger)
		return _manualTrigger(cardId, channelId);
	else
		OutputDebugLogTo(1, TRUE, "No manualTrigger() Function");

	return 0;
}