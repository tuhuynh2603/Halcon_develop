/// 8124.h: 8124 Linear Scale control interface
/// November 2015

#pragma once

#include "pci_8124.h"

extern "C" int _stdcall InitializeScaleLibrary();

extern "C" int _stdcall FreeScaleLibrary();

extern "C" int _stdcall RegisterScaleCards();

extern "C" int _stdcall ReleaseScaleCard();

extern "C" int _stdcall InitializeScaleChannel(int cardID, short channelID, short pwmChannelID, bool triggerOutRisingEdge, int nPulseWidthNs, short counterSource);

extern "C" int _stdcall SetScaleLatch(int cardID, short channelID, bool risingEdge);

extern "C" int _stdcall ResetScaleFifo(int cardID, short channelID);

extern "C" int _stdcall SetScaleFifoArray(int cardID, short channelID, long *plDataArray, short fifoSize);

extern "C" int _stdcall SetScaleEncoderValue(int cardID, short channelID, long encoderValue);

extern "C" int _stdcall GetScaleEncoderValue(int cardID, short channelID, long * encoderValue);

extern "C" int _stdcall GetScaleIntCount(int cardID, short channelID, int * count);

extern "C" int _stdcall GetScaleLatchValue(int cardID, short channelID, long* plValue, bool waitInterrupt);

extern "C" int _stdcall ManualTrigger(int cardId, short channelId);