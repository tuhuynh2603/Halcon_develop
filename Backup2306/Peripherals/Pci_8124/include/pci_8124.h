//  Copyright (C) 1995-2007 Adlink Technology INC.
//  All rights reserved.

#ifndef _PCI_8124_H
#define _PCI_8124_H

#define _MYWIN32

#ifdef __cplusplus
extern "C" {
#endif


#if defined (_MYWIN32)
	#define FNTYPE PASCAL
#elif defined (_RTX)
	#define FNTYPE _stdcall 
#endif

#include "type_def.h"
//#include "adlset.h"
// System: Section 5.2
I16 FNTYPE _8124_initialSw( I16 *CardIdInBit, I16 ManualId, I16 DefaultSw );  //default external sw
I16 FNTYPE _8124_close();
I16 FNTYPE _8124_get_version(I16 CardId, I32 *Firmware_ver, I32 *Driver_ver, I32 *DLL_ver);

// Encoder & Counter: Section 5.3
I16 FNTYPE _8124_set_encoder_input_mode( I16 CardId, I16 Channel, I16 IptMode ); //Old fun
I16 FNTYPE _8124_set_encoder_value( I16 CardId, I16 Channel, I32 EncValue );
I16 FNTYPE _8124_get_encoder_value( I16 CardId, I16 Channel, I32 *EncValue );
I16 FNTYPE _8124_set_encoder_up_down_count(I16 CardId, I16 Channel, I16 Inverse ); //OLD
I16 FNTYPE _8124_set_ez_clear( I16 CardId, I16 Channel, I16 Enable, I16 ClrLogic );
I16 FNTYPE _8124_set_counter_source( I16 CardId, I16 Channel, I16 CtnSrc ); //0-7 total 8 sources 

//PWM Configuration: Section 5.4
I16 FNTYPE _8124_set_pwm_source( I16 CardId, I16 Channel, I16 CmpEn, I16 LinearEn, I16 TimerEn ); //New function replace set_comparator_source
I16 FNTYPE _8124_set_pwm_mode( I16 CardId, I16 Channel, I16 PulseOrToggle );
I16 FNTYPE _8124_set_pwm_pulse_width( I16 CardId, I16 Channel, I16 WidthPara );
I16 FNTYPE _8124_set_pwm_toggle_dir(  I16 CardId, I16 Channel, I16 ToggleDir );

//TRG-OUT & TTL-OUT Configuration: Section 5.5
I16 FNTYPE _8124_set_trigger_source( I16 CardId, I16 Channel, I16 GroupSel, I16 PwmInBit, I16 TtlInInBit, I16 EzInBit );
I16 FNTYPE _8124_set_trgOut_logic( I16 CardId, I16 Channel, I16 Logic );
I16 FNTYPE _8124_set_ttlOut_source( I16 CardId, I16 Channel, I16 TtlOutSrc ); 
I16 FNTYPE _8124_set_ttlOut( I16 CardId, I16 Channel, I16 TtlOutValue ); //Digital out (if ttlout src == DO)

// Comparator: Section 5.6
I16 FNTYPE _8124_set_comparator_data( I16 CardId, I16 Channel, I32 CmpData ); // overwrite the next cmp data
I16 FNTYPE _8124_get_comparator_data( I16 CardId, I16 Channel, I32 *CmpData ); // get the next cmp data

//FIFO Comparing: Section 5.7
I16 FNTYPE _8124_reset_fifo( I16 CardId, I16 Channel );     // reset fifo to empty
I16 FNTYPE _8124_get_fifo_sts( I16 CardId, I16 Channel, I16 *FifoSts ); //New function
I16 FNTYPE _8124_set_fifo_data( I16 CardId, I16 Channel, I32 FifoData );  // put a data to fifo
I16 FNTYPE _8124_set_fifo_array( I16 CardId, I16 Channel, I32 *DataArr, I16 ArraySize ); // put a data array to fifo
I16 FNTYPE _8124_set_fifo_shift( I16 CardId, I16 Channel );
I16 FNTYPE _8124_set_fifo_level( I16 CardId, I16 Channel, I16 Level ); 
I16 FNTYPE _8124_get_fifo_level( I16 CardId, I16 Channel, I16 *Level );

// Linear Comparing: Section 5.8
I16 FNTYPE _8124_set_linear_source( I16 CardId, I16 Channel, I16 SetNumInBit );
I16 FNTYPE _8124_set_linear_compare( I16 CardId, I16 SetNum, I16 CntNum, I32 StartPoint, F64 RepeatTimes, I16 Interval );
I16 FNTYPE _8124_enable_linear_set( I16 CardId, I16 SetNum, I16 Enable );

// Manual Trigger: Section 5.9
I16 FNTYPE _8124_manual_trigger( I16 CardId, I16 Channel );

// Digital Input: Section 5.10
I16 FNTYPE _8124_get_di_sts( I16 CardId, I16 *TtlStsInBit, I16 *LtcStsInBit, I16 *EzStsInBit );

// Latch: Section 5.11
I16 FNTYPE _8124_set_latch( I16 CardId, I16 Channel, I16 Enable, I16 EdgeSel );
I16 FNTYPE _8124_get_latch_event_sts( I16 CardId, I16 *LatchEventInBit );
I16 FNTYPE _8124_get_latch_value(I16 CardId, I16 Channel, I32 *LatchValue );

// Timer: Section 5.12
I16 FNTYPE _8124_set_Timer( I16 CardId, I16 Channel, I16 WaitTtlIn , I16 TtlTrigLogic, I16 Interval );
I16 FNTYPE _8124_start_timer( I16 CardId, I16 Channel, I16 Start );

// Interrupt: Section 5.13
I16 FNTYPE _8124_set_int_factor( I16 CardId, I16 Channel, I16 IntFactorsInBit );
I16 FNTYPE _8124_get_int_factor( I16 CardId, I16 Channel, I16 *IntFactorsInBit );
I16 FNTYPE _8124_wait_single_int( I16 CardId, I16 Channel, I16 FactorBitNum, I32 TimeOutMs );
I16 FNTYPE _8124_reset_int( I16 CardId, I16 Channel, I16 FactorBitNum ); 


// Reservd function Note:Rtx not support
I16 FNTYPE _8124_initial_without_reset( I16 *CardIdInBit, I16 ManualId, I16 DefaultSw );
I16 FNTYPE _8124_close_without_reset();
I16 FNTYPE _8124_log_reg( I16 CardId );

// For Debug functions
I16 FNTYPE _8124_set_input_filter_enable( I16 CardId, I16 Channel, I16 Enable );
I16 FNTYPE _8124_get_noise_detect( I16 CardId, I16 Channel, I16 *DetectStatus );
I16 FNTYPE _8124_get_pwm_trigger_count( I16 CardId, I16 Channel, F64 *Count );
I16 FNTYPE _8124_reset_pwm_trigger_count( I16 CardId, I16 Channel );
I16 FNTYPE _8124_get_linear_next_compare( I16 CardId, I16 SetNum, I32 *CmpData );
I16 FNTYPE _8124_get_linear_remain_trigger_count( I16 CardId, I16 SetNum, F64 *Count );

//OLD Encorder function Note:RTX not support 
I16 FNTYPE _8124_initial( I16 *CardIdInBit ); 
I16 FNTYPE _8124_clear_encoder_value(I16 CardId, I16 Channel);
I16 FNTYPE _8124_get_ez_status(I16 CardId, I16 *EzStsInBit);
I16 FNTYPE _8124_enable_ez_clr( I16 CardId, I16 Channel ); 
I16 FNTYPE _8124_disable_ez_clr( I16 CardId, I16 Channel ); 
I16 FNTYPE _8124_set_trigger_pulse_width( I16 CardId, I16 Channel, I16 WidthSel ); 
I16 FNTYPE _8124_set_comparator_data_source( I16 CardId, I16 Channel, I16 CmpSrc ); //0:Disable, 1:FIFO, 2:Linear function, 3:CMP(Register, FIFO), 4:Timer
I16 FNTYPE _8124_get_fifo_status( I16 CardId, I16 Channel, I16 *FifoSts ); 
I16 FNTYPE _8124_set_linear_function(I16 CardId, I16 Channel, I16 Interval, U32 Repeat_times, I32 Start_pos, I16 Inverse);
I16 FNTYPE _8124_set_linear_function_enable(I16 CardId, I16 Channel, I16 Enable);
I16 FNTYPE _8124_int_enable(I16 CardId, HANDLE *phEvent); 
I16 FNTYPE _8124_int_disable(I16 CardId);

#ifdef __cplusplus
}
#endif

#endif