// Ihar, November 2015
// Scale interface for PCI 8124

#pragma once

typedef int (CALLBACK* INIT_SCALE_DLL)();

typedef int (CALLBACK* FREE_SCALE_DLL)();

typedef int (CALLBACK* REGISTER_SCALE_CARDS)();

typedef int (CALLBACK* RELEASE_SCALE_CARD)();

typedef int (CALLBACK* INITIALIZE_SCALE_CHANNEL)(int cardID, short channelID, short pwmChannelID, bool triggerOutRisingEdge, int nPulseWidthNs, short counterSource);

typedef int (CALLBACK* SET_SCALE_LATCH) (int cardID, short channelID, bool risingEdge);

typedef int (CALLBACK* RESET_SCALE_FIFO)(int cardID, short channelID);

typedef int (CALLBACK* SET_SCALE_FIFO_ARRAY)(int cardID, short channelID, long *plDataArray, short fifoSize);

typedef int (CALLBACK* SET_SCALE_ENCODER_VALUE)(int cardID, short channelID, long encoderValue);

typedef int (CALLBACK* GET_SCALE_ENCODER_VALUE)(int cardID, short channelID, long * encoderValue);

typedef int (CALLBACK* GET_SCALE_INT_COUNT)(int cardID, short channelID, int * count);

typedef int (CALLBACK* GET_SCALE_LATCHVALUE) (int cardID, short channelID, long* plValue, bool waitInterrupt);

typedef int (CALLBACK* MANUAL_TRIGGERRING) (int cardID, short channelID);

class ScaleInterface
{
public:
	ScaleInterface();

	int LoadScaleDll(CString strScaleCard);
	int FreeScaleDll();

	int RegisterScaleCards();
	int ReleaseScaleCard();
	int InitializeScaleChannel(int cardID, short channelID, short pwmChannelID, bool triggerOutRisingEdge, int pulseWidthNs, short counterSource);

	int ResetScaleFifo(int cardID, short channelID);
	int SetScaleFifoArray(int cardID, short channelID, long * data, short size);

	int SetScaleEncoderValue(int cardID, short channelID, long encoderValue);
	int GetScaleEncoderValue(int cardID, short channelID, long * encoderValue);

	int GetScaleIntCount(int cardID, short channelID, int * count);

	int SetScaleLatch(int cardID, short channelID, bool risingEdge);
	int GetScaleLatchValue(int cardID, short channelID, long * value, bool waitInterrupt = true);

	bool isDllOpened() const;

	void closeDll();

	int manualTrigger(int cardId, short channelId);

private:
	HINSTANCE _hDLL;

	INIT_SCALE_DLL           _lpInitScaleDLL;
	FREE_SCALE_DLL           _lpFreeScaleDLL;

	REGISTER_SCALE_CARDS     _lpRegisterScaleCards;
	RELEASE_SCALE_CARD       _lpReleaseScaleCard;

	INITIALIZE_SCALE_CHANNEL _lpInitializeScaleChannel;

	RESET_SCALE_FIFO         _lpResetScaleFifo;
	SET_SCALE_FIFO_ARRAY     _lpSetScaleFifoArray;

	SET_SCALE_ENCODER_VALUE  _lpSetScaleEncoderValue;
	GET_SCALE_ENCODER_VALUE  _lpGetScaleEncoderValue;

	GET_SCALE_INT_COUNT      _lpGetScaleIntCount;

	SET_SCALE_LATCH          _lpSetScaleLatch;
	GET_SCALE_LATCHVALUE     _lpGetScaleLatchValue;

	MANUAL_TRIGGERRING _manualTrigger;
};