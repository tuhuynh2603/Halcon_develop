/// 8124.cpp: 8124 Linear Scale control interface
//  November 2015
//  Advanced 4-CH Encoder Card with
//  High-speed Triggering Function
//  User’s Manual
#include "stdafx.h"
#include "8124.h"
#include "errorCodeDef.h"
//#include "PerformanceTimer.h"
#include "DebugLog.h"

const int encoderCardCount = 16;
int totalCardNumber = 0;

I16 g_i16CardID[encoderCardCount];

int _stdcall InitializeScaleLibrary()
{
	for (int i=0;i<encoderCardCount;i++)
		g_i16CardID[i] = -1;

	return 0;
}

int _stdcall FreeScaleLibrary()
{
	for (int i=0;i<encoderCardCount;i++)
		g_i16CardID[i] = -1;

	return 0;
}

int _stdcall RegisterScaleCards()
{
	I16 cardIdInBit = 0;
	I16 manualId = 0; // 0: Decided by system, 1:Decided by dip switch(SW1)
	I16 defaultSw = 0; // 0: Default output as PWM and Digital out, 1: Default output as SW2

	I16 returnCode = _8124_initialSw(&cardIdInBit, manualId, defaultSw);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_initialSw() function with parameters %d, %d with Error %d", manualId, defaultSw, returnCode);
		return -1;
	}

	I32 firwareVer, driverVer, dllVer;

	totalCardNumber = 0;

	for(I16 i=0; i<16; i++) {
		if((cardIdInBit >> i) & 0x1) {

			returnCode = _8124_get_version(i, &firwareVer, &driverVer, &dllVer);
			if(returnCode != ERR_NoError) {
				OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_get_version() function with parameters %d with Error %d", i, returnCode);
				return -1;
			}

			OutputDebugLogTo(1, TRUE, "PCI-8124: Found scale card %d, set as card ID %d for application. Firware ver %d, driver ver %d, DLL ver %d", i, totalCardNumber, firwareVer, driverVer, dllVer, totalCardNumber);

			g_i16CardID[totalCardNumber++] = i;
		}
	}

	if(totalCardNumber < 1) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 No Scale cards inside system");
		return -1;
	}
	else {
		OutputDebugLogTo(1, TRUE, "PCI-8124: %d cards have been registered", totalCardNumber);
	}

	return 0;
}

int _stdcall ReleaseScaleCard()
{
	for(int i=0;i<totalCardNumber;i++) {
		_8124_set_int_factor(g_i16CardID[i], 0, 4);
		_8124_set_int_factor(g_i16CardID[i], 1, 4);
		_8124_set_int_factor(g_i16CardID[i], 2, 4);
		_8124_set_int_factor(g_i16CardID[i], 3, 4);
	}

	Sleep(100);

	I16 returnCode = _8124_close();

	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_close() function with Error %d", returnCode);
		return -1;
	}

	return 0;
}

int _stdcall InitializeScaleChannel(int cardID, short channelID, short pwmChannelID, bool triggerOutRisingEdge, int nPulseWidthNs, short counterSource)
{
	if (cardID < 0 || cardID >= totalCardNumber) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 Scale Card ID %d is incorrect", cardID);
		return -1;
	}

	I16 nCardNo = g_i16CardID[cardID];

	I16 groupSel = 1; //0 - Output according to SW2 setting, 1 - PWM / TTL-IN / EZ signal
	I16 pmwInBit = 1 << pwmChannelID; //The source of PWM channel in bit format
	I16 ttlInInBit = 0; // The source of TTL-IN channel in bit format
	I16 ezInBit = 0; // The source of EZ channel in bit format

	I16 returnCode = _8124_set_trigger_source(nCardNo, channelID, groupSel, pmwInBit, ttlInInBit, ezInBit);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_trigger_source() function with parameters %d, %d, %d, %d, %d, %d with Error %d",
						 nCardNo, channelID, groupSel, pmwInBit, ttlInInBit, ezInBit, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set trigger source with parameters: Group selection %d, source of PWM channel 0x%x, source of TTL-IN channel 0x%x, source of EZ channel 0x%x",
					 cardID, channelID, groupSel, pmwInBit, ttlInInBit, ezInBit);

	I16 triggerOutLogic = triggerOutRisingEdge ? 1 : 0; // 0: DGND, 1: D5V.

	returnCode = _8124_set_trgOut_logic(nCardNo, channelID, triggerOutLogic);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_trgOut_logic() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, triggerOutLogic, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set trigger out logic as %d", cardID, channelID, triggerOutLogic);


	I16 ttlOutSrc = 1; //0: use as Digital output, 1: Follow TRG-OUT

	returnCode = _8124_set_ttlOut_source(nCardNo, channelID, ttlOutSrc);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_ttlOut_source() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, ttlOutSrc, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set TTL mode as %d", cardID, channelID, ttlOutSrc);

	I16 iptMode = 4; //0: OUT/DIR (default), 1: CW/CCW, 2: 1x AB-Phase, 3: 2x AB-Phase, 4: 4x AB-Phase


	returnCode = _8124_set_encoder_input_mode(nCardNo, channelID, iptMode);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_encoder_input_mode() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, iptMode, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set IPT mode as %d", cardID, channelID, iptMode);

	I16 fifoLevel = 3; //0: Low level = 0 (Empty), 1: Low level = 256 ( 1/4 FIFO size ), 2: Low level = 512 ( 1/2 FIFO size ) (Default), 3: Low level = 768 ( 3/4 FIFO size )
	returnCode = _8124_set_fifo_level(nCardNo, channelID, fifoLevel);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_fifo_level() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, fifoLevel, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set FIFO level as %d", cardID, channelID, fifoLevel);

	I16 inverseCounterMode = 1;// 0: Not inverse (default), 1: Inverse

	returnCode = _8124_set_encoder_up_down_count(nCardNo, channelID, inverseCounterMode);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_encoder_up_down_count() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, inverseCounterMode, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set Counter Mode as %d", cardID, channelID, inverseCounterMode);

	I16 enableEZFunction = 0; //0: Disable (default), 1: Enable
	I16 clrLogic = 0; //0: Photo couple: OFF -> ON (Falling edge) clear (default), 1: Photo couple: ON -> OFF (Rising edge) clear

	returnCode = _8124_set_ez_clear(nCardNo, channelID, enableEZFunction, clrLogic);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_ez_clear() function with parameters %d, %d, %d, %d with Error %d", nCardNo, channelID, enableEZFunction, clrLogic, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set EZ Function as %d and clear logic as %d", cardID, channelID, enableEZFunction, clrLogic);

	I16 pulseOrToggle = 0; //0: Pulse output, 1: Level switch output (toggle output)

	returnCode = _8124_set_pwm_mode(nCardNo, channelID, pulseOrToggle);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_pwm_mode() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, pulseOrToggle, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has selected pulse or toggle mode as %d", cardID, channelID, pulseOrToggle);

	I16 widthParameter = static_cast<I16>((nPulseWidthNs / 25 - 4) / 16); // Pulse width(ns) = ( WidthPara × 16 + 4 ) × 25

	returnCode = _8124_set_pwm_pulse_width(nCardNo, channelID, widthParameter);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_pwm_pulse_width() function with parameters %d, %d, %d; pulse width %d ns with Error %d", nCardNo, channelID, widthParameter, nPulseWidthNs, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set pulse width %d as parameter %d", cardID, channelID, nPulseWidthNs, widthParameter);

	I16 enableCMP = 1; //Enable CMP(FIFO comparing) source (0: Disable, 1: Enable)
	I16 enableLinearComparing = 0; // 0: Disable, 1: Enable
	I16 enableTimerSource = 0; // 0: Disable, 1:Enable

	returnCode = _8124_set_pwm_source(nCardNo, channelID, enableCMP, enableLinearComparing, enableTimerSource);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_pwm_source() function with parameters %d, %d, %d, %d, %d with Error %d",
						 nCardNo, channelID, enableCMP, enableLinearComparing, enableTimerSource, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set Enable CMP as %d, Enable Linear Comparing as %d, Enable Timer Source as %d",
					 cardID, channelID, enableCMP, enableLinearComparing, enableTimerSource);
	
	if(counterSource < 0 || counterSource > 7) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 Scale Card ID %d Channel %d has incorrect counter source %d", cardID, channelID, counterSource);
		return -1;
	}

	// Counter source
	//0 – 3: EA/EB 0 – 3
	//4 – 7: Timer 0 – 3
	returnCode = _8124_set_counter_source(nCardNo, channelID, counterSource);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_counter_source() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, counterSource, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set Counter Source as %d", cardID, channelID, counterSource);


	I16 enableFiter = 1;

	returnCode = _8124_set_input_filter_enable(nCardNo, channelID, enableFiter);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_input_filter_enable() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, enableFiter, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set Enable Filter as %d", cardID, channelID, enableFiter);

	return 0;
}

int _stdcall SetScaleLatch(int cardID, short channelID, bool risingEdge)
{
	if (cardID < 0 || cardID >= totalCardNumber) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 Scale Card ID %d is incorrect", cardID);
		return -1;
	}


	I16 nCardNo = g_i16CardID[cardID];

	I16 returnCode = _8124_set_int_factor(nCardNo, channelID, 0x10);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_int_factor() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, 0x10, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set Interrupt Factor as %d", cardID, channelID, 0x10);

	I16 enableLatch = 1; // 0: Disable, 1: Enable
	I16 latchEdge = risingEdge ? 1 : 0;

	returnCode = _8124_set_latch(nCardNo, channelID, enableLatch, latchEdge);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_latch() function with parameters %d, %d, %d, %d with Error %d", nCardNo, channelID, enableLatch, latchEdge, returnCode);
		return -1;
	}

	OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set Latch enable as %d and latch edge as %d", cardID, channelID, enableLatch, latchEdge);

	return 0;
}

int _stdcall ResetScaleFifo(int cardID, short channelID)
{
	if (cardID < 0 || cardID >= totalCardNumber) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 Scale Card ID %d is incorrect", cardID);
		return -1;
	}

	I16 nCardNo = g_i16CardID[cardID];

	I16 returnCode = _8124_reset_fifo(nCardNo, channelID);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_reset_fifo() function with parameters %d, %d with Error %d", nCardNo, channelID, returnCode);
		return -1;
	}

	//OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d _8124_reset_fifo() completed", cardID, channelID);

	returnCode = _8124_reset_pwm_trigger_count(nCardNo, channelID);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_reset_pwm_trigger_count() function with parameters %d, %d with Error %d", nCardNo, channelID, returnCode);
		return -1;
	}

	//OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d _8124_reset_pwm_trigger_count() completed", cardID, channelID);

	return 0;
}

int _stdcall SetScaleFifoArray(int cardID, short channelID, long *plDataArray, short fifoSize)
{
	if (cardID < 0 || cardID >= totalCardNumber) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 Scale Card ID %d is incorrect", cardID);
		return -1;
	}

	I16 nCardNo = g_i16CardID[cardID];

	I16 returnCode = _8124_set_fifo_array(nCardNo, channelID, plDataArray, fifoSize);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_fifo_array() function with parameters %d, %d, %x, %d with Error %d", nCardNo, plDataArray, fifoSize, returnCode);
		return -1;
	}

	//OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d _8124_set_fifo_array() completed with array size %d", cardID, channelID, fifoSize);

	returnCode = _8124_set_fifo_shift(nCardNo, channelID);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_fifo_shift() function with parameters %d, %d with Error %d", nCardNo, channelID, returnCode);
		return -1;
	}

	//OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d _8124_set_fifo_shift() completed", cardID, channelID);

	return 0;
}

int _stdcall SetScaleEncoderValue(int cardID, short channelID, long encoderValue)
{
	if (cardID < 0 || cardID >= totalCardNumber) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 Scale Card ID %d is incorrect", cardID);
		return -1;
	}

	I16 nCardNo = g_i16CardID[cardID];

	I16 returnCode = _8124_set_encoder_value(nCardNo, channelID, encoderValue);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_set_encoder_value() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, encoderValue, returnCode);
		return -1;
	}

	//OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d has set encoder value as %d", cardID, channelID, encoderValue);

	return 0;
}

int _stdcall GetScaleEncoderValue(int cardID, short channelID, long * encoderValue)
{
	if (cardID < 0 || cardID >= totalCardNumber) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 Scale Card ID %d is incorrect", cardID);
		return -1;
	}

	I16 nCardNo = g_i16CardID[cardID];

	I16 returnCode = _8124_get_encoder_value(nCardNo, channelID, encoderValue);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_get_encoder_value() function with parameters %d, %d with Error %d", nCardNo, channelID, returnCode);
		return -1;
	}

	return 0;
}

int _stdcall GetScaleIntCount(int cardID, short channelID, int * count)
{
	if (cardID < 0 || cardID >= totalCardNumber) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 Scale Card ID %d is incorrect", cardID);
		return -1;
	}


	I16 nCardNo = g_i16CardID[cardID];

	F64 dCount;
	I16 returnCode = _8124_get_pwm_trigger_count(nCardNo, channelID, &dCount);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_get_pwm_trigger_count() function with parameters %d, %d with Error %d", nCardNo, channelID, returnCode);
		return -1;
	}

	*count = static_cast<int>(dCount);

	return 0;
}

int _stdcall GetScaleLatchValue(int cardID, short channelID, long* plValue, bool waitInterrupt)
{
	if (cardID < 0 || cardID >= totalCardNumber) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 Scale Card ID %d is incorrect", cardID);
		return -1;
	}

	I16 nCardNo = g_i16CardID[cardID];

	if(waitInterrupt) {

		I16 bitID = 4;

		I16 returnCode = _8124_reset_int(nCardNo, channelID, bitID);
		if(returnCode != ERR_NoError) {
			OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_reset_int() function with parameters %d, %d, %d with Error %d", nCardNo, channelID, bitID, returnCode);
			return -1;
		}

		//OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d _8124_reset_int() completed parameter value as %d", cardID, channelID, bitID);

		returnCode = _8124_wait_single_int(nCardNo, channelID, bitID, -1);
		if(returnCode != ERR_NoError) {
			OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_wait_single_int() function with parameters %d, %d, %d, -1 with Error %d", nCardNo, channelID, bitID, returnCode);
			return -1;
		}

		//OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d _8124_wait_single_int() completed parameter value as %d", cardID, channelID, bitID);

	}

	I16 returnCode = _8124_get_latch_value(nCardNo, channelID, plValue);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_get_latch_value() function with parameters %d, %d with Error %d", nCardNo, channelID, returnCode);
		return -1;
	}

	//OutputDebugLogTo(1, TRUE, "PCI-8124: Card %d Channel %d _8124_get_latch_value() completed parameter value as %d", cardID, channelID, *plValue);

	return 0;
}

int _stdcall ManualTrigger(int cardId, short channelId)
{
	if (cardId < 0 || cardId >= totalCardNumber) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 Scale Card ID %d is incorrect", cardId);
		return -1;
	}

	I16 nCardNo = g_i16CardID[cardId];

	I16 returnCode = _8124_manual_trigger(nCardNo, channelId);
	if(returnCode != ERR_NoError) {
		OutputDebugLogTo(0, TRUE, "----- ERROR : PCI-8124 _8124_get_encoder_value() function with parameters %d, %d with Error %d", nCardNo, channelId, returnCode);
		return -1;
	}

	return 0;
}