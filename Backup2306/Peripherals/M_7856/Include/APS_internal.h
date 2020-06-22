#ifndef _APS_INTERNAL_H
#define _APS_INTERNAL_H
#ifdef __cplusplus
extern "C" {
#endif


#if defined (_MYWIN32) || defined (_MYRTEVLIB)
	#include <windows.h>
	#define FNTYPE PASCAL
#elif defined (_MYLINUX)
	#ifndef FNTYPE
	#define FNTYPE
	#endif
#elif defined(_MYRTE)	
	#define FNTYPE	_stdcall
#else
	#define FNTYPE PASCAL
#endif

#include "type_def.h"
#include "type_def_pid.h"

/* This header file is used to put functions, that only internal-use-only functions,
   All function are export to .LIB
*/

// Return Device string.
I32 FNTYPE APS_get_board_name_str( I32 CardId, char* devName, I32 Size );

I32 FNTYPE APSI_set_hal_U32( I32 CardId, I32 ResourceId, I32 Offset, U32  Data );
I32 FNTYPE APSI_set_hal_U16( I32 CardId, I32 ResourceId, I32 Offset, U16  Data );
I32 FNTYPE APSI_set_hal_U8(  I32 CardId, I32 ResourceId, I32 Offset, U8   Data );
I32 FNTYPE APSI_get_hal_U32( I32 CardId, I32 ResourceId, I32 Offset, U32* Data );
I32 FNTYPE APSI_get_hal_U16( I32 CardId, I32 ResourceId, I32 Offset, U16* Data );
I32 FNTYPE APSI_get_hal_U8(  I32 CardId, I32 ResourceId, I32 Offset, U8*  Data );

// 1: Yes, 0: No, not pci-8392 return error.(For motion creator pro2)
I32 FNTYPE APSI_is_8392_has_mky( I32 Board_ID, I32 *YesOrNo ); 

//Sampling functions (For debug)
I32 FNTYPE APSI_get_single_sample_data( I32 CardId, I32 *DataArr );
I32 FNTYPE APSI_wait_trigger_sampling( I32 CardId, I32 Length, I32 PreTrgLen, I32 TimeOutMs, I32 *DataArr0, I32 *DataArr1, I32 *DataArr2, I32 *DataArr3, I32 *Tick, I32 *Info );

// Just for 825x
I32 FNTYPE APS_get_card_information( I32 Board_ID, I32 *MaxAxisNumber, I32 *CycleTimeUs );

I32 FNTYPE APS_get_Board_info( I32 Board_ID, I32 *devName, I32 *devType );

I32 FNTYPE APS_start_sampling( I32 CardId, I32 Cmd );
I32 FNTYPE APS_wait_trigger_sampling_adv( I32 CardId, I32 Length, I32 PreTrgLen, I32 TimeOutMs, I32 *DataArr );
I32 FNTYPE APS_dsp_version( I32 Board_ID );
I32 FNTYPE APS_set_axis_param_en( I32 Axis_ID, I32 Param_No, I32 Param_Data );
I32 FNTYPE APS_get_axis_param_en( I32 Axis_ID, I32 Param_No, I32 *Param_Data );

I32 FNTYPE APS_check_servo_param( I32 Axis_ID, I32 Para_No1, I32 *Para_Dat1, I32 Para_No2, I32 *Para_Dat2 );
I32 FNTYPE APS_change_servo_param( I32 Axis_ID, I32 Para_No1, I32 Para_Dat1, I32 Para_No2, I32 Para_Dat2 );
I32 FNTYPE APS_get_sscnet_alarm_history( I32 Axis_ID, I32 History_No, I32 *Alarm_No, I32 *Alarm_Detail );
I32 FNTYPE APS_reset_sscnet_alarm_history( I32 Axis_ID );
I32 FNTYPE APS_get_sscnet_monitor( I32 Axis_ID, I32 Arr_size, I32 *Data_arr );
I32 FNTYPE APS_set_sscnet_monitor_src( I32 Axis_ID, I32 Block_No, I32 Src1, I32 Src2, I32 Src3, I32 Src4 );
I32 FNTYPE APS_get_sscnet_monitor_src( I32 Axis_ID, I32 Block_No, I32 *Src1, I32 *Src2, I32 *Src3, I32 *Src4 );
I32 FNTYPE APS_get_next_point_index( I32 Axis_ID, I32 *Index );
I32 FNTYPE APS_fast_monitor( I32 Axis_ID, I32 Index, I32 *Data );
I32 FNTYPE APS_check_home_move_state( I32 Axis_ID );

//Fieldbus function
I32 FNTYPE APS_get_field_bus_slave_count( I32 Board_ID, I32 BUS_No, I32 *SlaveCount );

I32 FNTYPE APS_relative_move_profile( I32 Axis_ID, I32 Distance, I32 Max_Speed, I32 *pStrVel, I32 *pMaxVel, F64 *pTacc, F64 *pTdec, F64 *pTconst );
I32 FNTYPE APS_absolute_move_profile( I32 Axis_ID, I32 Position, I32 Max_Speed, I32 *pStrVel, I32 *pMaxVel, F64 *pTacc, F64 *pTdec, F64 *pTconst );
I32 FNTYPE APS_velocity_move_profile( I32 Axis_ID, I32 Max_Speed, I32 *pStrVel, I32 *pMaxVel, F64 *pTacc );

//Internal for 4XMO-C test
I32 FNTYPE APS_SetOffsetDataHandShake( I32 Board_ID, I32 BUS_No, I32 Module_ID, I32 Offset, U16 *Data, I32 DataNum );
I32 FNTYPE APS_GetOffsetDataHandShake( I32 Board_ID, I32 BUS_No, I32 Module_ID, I32 Offset, U16 *Data, I32 DataNum );
I32 FNTYPE APS_SetFpgaOffsetData( I32 Board_ID, I32 BUS_No, I32 Module_ID, I32 Offset, I32 Value );
I32 FNTYPE APS_GetFpgaOffsetData( I32 Board_ID, I32 BUS_No, I32 Module_ID, I32 Offset, I32 *Value );
I32 FNTYPE APS_SetSramOffsetData( I32 Board_ID, I32 BUS_No, I32 Module_ID, I32 SramNo, I32 Offset, I32 Value );
I32 FNTYPE APS_GetSramOffsetData( I32 Board_ID, I32 BUS_No, I32 Module_ID, I32 SramNo, I32 Offset, I32 *Value );

//for RTE use only 
I32 FNTYPE APS_get_compare_status_rte( I32 Axis_ID, I32 *CmpSts );//1XMO
I32 FNTYPE APS_motion_busy_rte( I32 Axis_ID, I32 *MotionBusy );//1XMO

I32 FNTYPE APS_load_param_from_file_by_axis( const char *pXMLFile, I32 Axis_ID );
I32 FNTYPE APS_get_param_from_file_ax( const char *pXMLFile, I32 Axis_ID, I32 Num, I32 *Value );


I32 FNTYPE APS_initial2( I32 *BoardID_InBits, I32 Mode );

I32 FNTYPE _sm2_host_handshake( I32 Board_ID, U16 CmdCode,
						U16 CArrSize, U32 *CData,
						U16 RArrSize, U32 *RData );

I32 FNTYPE _sm2_motion_handshake( I32 Board_ID, U16 CmdCode,
						U16 CArrSize, U32 *CData,
						U16 RArrSize, U32 *RData );

//for 825x v2 only
I32 FNTYPE APSI_825X_2_get_system_state( I32 Board_ID, I32 *RData      ); 
I32 FNTYPE APSI_825X_2_set_system_state( I32 Board_ID, I32  CData      );
I32 FNTYPE APSI_825X_2_get_cycle_timer ( I32 Board_ID, I32 *CycleTimer );
I32 FNTYPE APSI_825X_2_set_cycle_timer ( I32 Board_ID, I32  Value      );
I32 FNTYPE APSI_825X_2_rset_LOEC       ( I32 Board_ID, I32  CData      );
I32 FNTYPE APSI_825X_2_rset_MLD		   ( I32 Board_ID, I32  CData      ); 
I32 FNTYPE APSI_825X_2_get_CLD         ( I32 Board_ID, I32 *ServeLD,   I32 *MotionLD,   I32 *HostLD );
I32 FNTYPE APSI_825X_2_get_loop_ratio  ( I32 Board_ID, I32 *ServeLR,   I32 *MotionLR,   I32 *HostLR );
I32 FNTYPE APSI_825X_2_set_loop_ratio  ( I32 Board_ID, I32  ServeLR,   I32  MotionLR,   I32  HostLR );
I32 FNTYPE APSI_825X_2_get_MLD         ( I32 Board_ID, I32 *ServeMLD,  I32 *MotionMLD,  I32 *HostMLD  ); //max loop duration
I32 FNTYPE APSI_825X_2_get_LOEC	       ( I32 Board_ID, I32 *ServeLOEC, I32 *MotionLOEC, I32 *HostLOEC ); //Loop Over-run Error Counter
I32 FNTYPE APSI_825X_2_get_probe_MD    ( I32 Board_ID, I32 *ProbeSum,  I32 *Probe_zero, I32 *Probe_one );
I32 FNTYPE APSI_825X_2_get_probe_CD    ( I32 Board_ID, I32 *ProbeSum,  I32 *Probe_zero, I32 *Probe_one );
I32 FNTYPE APSI_825X_2_reset_probe_MD  ( I32 Board_ID, I32  CData    );
I32 FNTYPE APSI_825X_2_get_AI_ALL      ( I32 Board_ID, I32 *CH_Array );
I32 FNTYPE APSI_825X_2_get_ENC_ALL     ( I32 Board_ID, I32 *CH_Array );
I32 FNTYPE APSI_825X_2_set_AO_ALL      ( I32 Board_ID, I32 *CH_Array );
I32 FNTYPE APSI_825X_2_hhs_self_test   ( I32 Board_ID, I32  CArrSize, I32 *CData,    I32 RArrSize, I32 *RData );
I32 FNTYPE APSI_825X_2_AO_control_right( I32 Board_ID, I32  AO_Ch,    I32 *AO_Right, I32 *RData );
I32 FNTYPE APSI_825X_2_AO_host_control ( I32 Board_ID, I32  AO_Ch,    I32 *AO_Value, I32 *RData );
I32 FNTYPE APSI_825X_2_DO_control      ( I32 Board_ID, I32  Cmd,      I32  CData,    I32 *RData );
I32 FNTYPE APSI_825X_2_get_DI_status   ( I32 Board_ID, I32 *RData );

I32 FNTYPE APSI_825X_Start_Sampling    ( I32 Board_ID, I32 Cmd );
I32 FNTYPE APSI_825X_Get_SingleSampData( I32 Board_ID, STR_SAMP_DATA_4CH *pData );

I32 FNTYPE APSI_set_axis_parameter(      I32 Axis_ID, I32 ParamNum, I32 ParamClass, I32  ParamValue );
I32 FNTYPE APSI_get_axis_parameter(      I32 Axis_ID, I32 ParamNum, I32 ParamClass, I32 *ParamValue );

I32 FNTYPE APSI_get_axes_state( I32 Board_ID, I32 *RData );

I32 FNTYPE APSI_SM2_Set_Motion_CMD_A( I32 Board_ID, I32 *RData );
I32 FNTYPE APSI_SM2_Set_Motion_CMD  ( I32 Board_ID, I32  AxisNum, I32 MCMD );

//Internal functions of PCI-8258
//Non-Release
I32 FNTYPE APSI_8258_SelfTest( I32 Board_ID );
I32 FNTYPE APSI_8258_LineMove( I32 Board_ID );
I32 FNTYPE APSI_8258_GetCycleClock( I32 Board_ID, U32 *Clock );
I32 FNTYPE APSI_8258_GetTimeMeasure( I32 Board_ID, U16 ChSize, U32 *Time );
I32 FNTYPE APSI_8258_GetTimeMax( I32 Board_ID, U16 Ch, U32 *MaxTime );
I32 FNTYPE APSI_8258_ResetTimeMax( I32 Board_ID, U16 Ch );
I32 FNTYPE APSI_8258_GetOverloadCnt( I32 Board_ID, U16 Ch, U32 *Cnt );
I32 FNTYPE APSI_8258_ResetOverloadCnt( I32 Board_ID, U16 Ch );
I32 FNTYPE APSI_8258_ResetHostRatio( I32 Board_ID, U16 Ratio );
I32 FNTYPE APSI_8258_GetHostRatio( I32 Board_ID, U32 *Ratio );
I32 FNTYPE APSI_8258_HstSelfTest( I32 Board_ID );
I32 FNTYPE APSI_8258_DpramTest( I32 Board_ID, I32  PatternNo, I32 StartAddr, I32 EndAddr, I32 Enable );
I32 FNTYPE APSI_8258_FpgaTest( I32 Board_ID, I32  PatternNo, I32 StartAddr, I32 EndAddr, I32 Enable );
I32 FNTYPE APSI_8258_HpiTest( I32 Board_ID, I32  PatternNo, I32 StartAddr, I32 EndAddr, I32 Enable );
I32 FNTYPE APSI_8258_write_hpi( I32 Board_ID, U32 Offset, U32 Data );
I32 FNTYPE APSI_8258_read_hpi( I32 Board_ID, U32 Offset, U32 *Data );
I32 FNTYPE APSI_8258_EepTest( I32 Board_ID, I32  PatternNo, I32 StartAddr, I32 EndAddr, I32 Enable );
//Calibration
I32 FNTYPE APSI_8258_read_eeprom( I32 Board_ID, U16 Index, U16 *Data );
I32 FNTYPE APSI_8258_write_eeprom( I32 Board_ID, U16 Index, U16 Data );
//I32 FNTYPE APSI_8258_save_calib_to_eeprom( I32 Board_ID ); //Take off due to 
I32 FNTYPE APSI_8258_load_calib_from_eeprom( I32 Board_ID );
I32 FNTYPE APSI_8258_calib_aio( I32 Board_ID, U8 Channel, U8 Type, U16 Value );

//Fpga R/W
I32 FNTYPE APSI_8258_read_fpga( I32 Board_ID, U16 Mode, U32 Offset, U32 *Data );
I32 FNTYPE APSI_8258_write_fpga( I32 Board_ID, U16 Mode, U32 Offset, U32 Data );
//8258 PCI register R/W
I32 FNTYPE APSI_8258_write_pci_register( I32 Board_ID, U32 Offset, U32 Data );
I32 FNTYPE APSI_8258_read_pci_register( I32 Board_ID, U32 Offset, U32 *Data );

//Program download - APSI
I32 FNTYPE APSI_set_task_register( I32 Board_ID, I32 TaskNum, I32 RegNum, I32 RegVal );
I32 FNTYPE APSI_set_task_stack( I32 Board_ID, I32 TaskNum, I32 Size, I32 Offset, I32 *Data );
I32 FNTYPE APSI_get_task_stack( I32 Board_ID, I32 TaskNum, I32 Size, I32 Offset, I32 *Data );

//Compare
I32 FNTYPE APSI_get_lcmp_sts( I32 Board_ID, U16 ArrSets, U32 *RestPoints, I32 *CurrentPointValue, I32 *Interval );
I32 FNTYPE APSI_get_pwm_config( I32 Board_ID, U16 Ch, U16 *Mode, U16 *Logic, U16 *SrcInBit, U16 *ToggleEn );
I32 FNTYPE APSI_get_fcmp_sts( I32 Board_ID, U16 ArrSets, U16 *FreeSpaceArr, U16 *UsageArr, U16 *FifoFreeSpaceArr, U16 *FifoUsageArr, U32 *StsInBit );

//Parameter access
I32 FNTYPE APSI_pacc_get_status( I32 CardId, I32 *IsrErr, I32 *State, I32 *Percent );
I32 FNTYPE APSI_pacc_get_elapsed_time( I32 CardId, I32 *time_us );
I32 FNTYPE APSI_pacc_get_flash_para_cnt( I32 CardId, I32 *Cnt );
I32 FNTYPE APSI_pacc_get_dsp_para_cnt( I32 CardId, I32 *Cnt );
I32 FNTYPE APSI_pacc_get_flash_data_chk( I32 CardId, I32 *ChkStatus );
I32 FNTYPE APSI_pacc_get_flash_version( I32 CardId, I32 *Ver );
I32 FNTYPE APSI_pacc_get_err_para_info( I32 CardId, I32 *ParamType, I32 *ParamNo, I32 *AxisNo );

//Scan modules, Only for PCI-7856
I32 FNTYPE APSI_7856_reset_device( I32 Board_ID );
I32 FNTYPE APSI_7856_scan_all( I32 Board_ID );
I32 FNTYPE APSI_7856_scan( I32 Board_ID, I32 Module_ID );
I32 FNTYPE APSI_7856_start_io_cyclic( I32 Board_ID );
I32 FNTYPE APSI_7856_stop_io_cyclic( I32 Board_ID );
I32 FNTYPE APSI_7856_get_systme_status( I32 Board_ID, U16 *Status );
I32 FNTYPE APSI_7856_get_device_info( I32 Board_ID, I32 Module_ID, U8 *Status );

//For DB-8150 Not Release function
I32 FNTYPE APS_set_trigger_fifo_cmp_data( I32 Board_ID, I32 FCmpCh, I32 CmpVal );
I32 FNTYPE APS_enable_trigger_linear_cmp( I32 Board_ID, I32 LCmpCh, I32 Enable );
I32 FNTYPE APS_reset_trigger_fifo( I32 Board_ID, I32 FCmpCh );

//PID functions
I32 FNTYPE APSI_set_ctrl_gain( I32 Axis_ID, U16 Type, U16 Value );
I32 FNTYPE APSI_get_ctrl_gain( I32 Axis_ID, U16 Type, U16 *Value  );
I32 FNTYPE APSI_set_ctrl_gain_sync( I32 Axis_ID, __PCTRL_PID Pid );
I32 FNTYPE APSI_get_ctrl_gain_sync( I32 Axis_ID, __PCTRL_PID Pid );
I32 FNTYPE APSI_set_ctrl_shift( I32 Axis_ID, U16 Type, U16 Dir, U16 Value );
I32 FNTYPE APSI_get_ctrl_shift( I32 Axis_ID, U16 Type, U16 *Dir, U16 *Value );
I32 FNTYPE APSI_set_ctrl_biquad( I32 Axis_ID, U16 FilterNo, U16 Type, I16 Value );
I32 FNTYPE APSI_get_ctrl_biquad( I32 Axis_ID, U16 FilterNo, U16 Type, U16 *Value );
I32 FNTYPE APSI_set_ctrl_biquad_sync( I32 Axis_ID, __PCTRL_BIQUAD Bqd );
I32 FNTYPE APSI_get_ctrl_biquad_sync( I32 Axis_ID, __PCTRL_BIQUAD Bqd );
I32 FNTYPE APSI_set_ctrl_motor( I32 Axis_ID, U16 Limit, I32 Bias );
I32 FNTYPE APSI_get_ctrl_motor( I32 Axis_ID, U16 *Limit, I32 *Bias );
I32 FNTYPE APSI_set_ctrl_servo_rate( I32 Axis_ID, U16 Update );
I32 FNTYPE APSI_get_ctrl_servo_rate( I32 Axis_ID, U16 *Update );
I32 FNTYPE APSI_set_ctrl_loop_mode( I32 Axis_ID, U16 Mode );
I32 FNTYPE APSI_get_ctrl_loop_mode( I32 Axis_ID, U16 *Mode );
I32 FNTYPE APSI_set_ctrl_integration_limit( I32 Axis_ID, U32 Limit );
I32 FNTYPE APSI_get_ctrl_integration_limit( I32 Axis_ID, U32 *Limit );
I32 FNTYPE APSI_set_ctrl_d_samp_time( I32 Axis_ID, U16 Count );
I32 FNTYPE APSI_get_ctrl_d_samp_time( I32 Axis_ID, U16 *Count );
I32 FNTYPE APSI_set_ctrl_cmd_pos( I32 Axis_ID, I32 CmdPos );
I32 FNTYPE APSI_get_ctrl_cmd_pos( I32 Axis_ID, I32 *CmdPos );
I32 FNTYPE APSI_get_ctrl_cmd_info( I32 Axis_ID, __PCTRL_CMDINFO Info );
I32 FNTYPE APSI_get_ctrl_calc_info( I32 Axis_ID, __PCTRL_CALC Calc );
I32 FNTYPE APSI_get_ctrl_loop_sts( I32 Axis_ID, U32 *StsBitFormat );

//SID functions
I32 FNTYPE APSI_sid_close( I32 Axis_ID );
I32 FNTYPE APSI_sid_rcp_enable( I32 Axis_ID, U16 Enable );
I32 FNTYPE APSI_sid_set_rcp_amplitude( I32 Axis_ID, F64 Amplitude );
I32 FNTYPE APSI_sid_get_rcp_amplitude( I32 Axis_ID, F64 *Amplitude );
I32 FNTYPE APSI_sid_set_rcp_hysteresis( I32 Axis_ID, F64 Width );
I32 FNTYPE APSI_sid_get_rcp_hysteresis( I32 Axis_ID, F64 *Width );
I32 FNTYPE APSI_sid_frp_enable( I32 Axis_ID, U16 Enable );
I32 FNTYPE APSI_sid_get_rcp_pid( I32 Axis_ID, U16 *ReadFlag, __PSID_PID Pid );
I32 FNTYPE APSI_sid_frp_enable( I32 Axis_ID, U16 Enable );
I32 FNTYPE APSI_sid_set_frp_freq( I32 Axis_ID, F64 Freq );
I32 FNTYPE APSI_sid_get_frp_freq( I32 Axis_ID, F64 *Freq );
I32 FNTYPE APSI_sid_set_frp_amplitude( I32 Axis_ID, F64 Amp );
I32 FNTYPE APSI_sid_get_frp_amplitude( I32 Axis_ID, F64 *Amp );
I32 FNTYPE APSI_sid_set_frp_type( I32 Axis_ID, U16 Type );
I32 FNTYPE APSI_sid_get_frp_type( I32 Axis_ID, U16 *Type );
I32 FNTYPE APSI_sid_set_frp_update_time( I32 Axis_ID, F64 Time );
I32 FNTYPE APSI_sid_get_frp_update_time( I32 Axis_ID, F64 *Time );
I32 FNTYPE APSI_sid_set_frp_pid( I32 Axis_ID, __PSID_PID Pid );
I32 FNTYPE APSI_sid_get_frp_pid( I32 Axis_ID, __PSID_PID Pid );
I32 FNTYPE APSI_sid_get_freq_response( I32 Axis_ID, U16 *ReadFlag, __PSID_FREQ Freq );
I32 FNTYPE APSI_sid_set_deviation_limit( I32 Axis_ID, F64 DevLimit );
I32 FNTYPE APSI_sid_get_deviation_limit( I32 Axis_ID, F64 *DevLimit );
I32 FNTYPE APSI_sid_get_status( I32 Axis_ID, U32 *Status );

//Eep functions
I32 FNTYPE APSI_set_eep_aio_type( I32 Board_ID, U16 Type );
I32 FNTYPE APSI_get_eep_aio_type( I32 Board_ID, U16 *Type );
I32 FNTYPE APSI_set_eep_param_source( I32 Board_ID, U16 Source );
I32 FNTYPE APSI_get_eep_param_source( I32 Board_ID, U16 *Source );
I32 FNTYPE APSI_set_eep_servo_rate( I32 Board_ID, U16 Rate );
I32 FNTYPE APSI_get_eep_servo_rate( I32 Board_ID, U16 *Rate );
I32 FNTYPE APSI_get_eep_curr_servo_rate( I32 Board_ID, U16 *Rate );
I32 FNTYPE APSI_set_eep_motion_rate( I32 Board_ID, U16 Rate );
I32 FNTYPE APSI_get_eep_motion_rate( I32 Board_ID, U16 *Rate );
I32 FNTYPE APSI_set_eep_host_ratio( I32 Board_ID, U16 Ratio );
I32 FNTYPE APSI_get_eep_host_ratio( I32 Board_ID, U16 *Ratio );
I32 FNTYPE APSI_set_eep_drv_ctrl_mode( I32 Board_ID, U16 ModeInBit );
I32 FNTYPE APSI_get_eep_drv_ctrl_mode( I32 Board_ID, U16 *ModeInBit );
I32 FNTYPE APSI_get_eep_curr_drv_ctrl_mode( I32 Board_ID, U16 *ModeInBit );
I32 FNTYPE APSI_get_eep_stop_code( I32 Board_ID, U16 *Code );

//Loop switch
I32 FNTYPE APSI_loop_switch( I32 Axis_ID, U16 Mode, U32 SysIDCycTimeUs );
//Loop loading
I32 FNTYPE APSI_get_loop_loading( I32 Board_ID, PLPSTS Status );
//Raw data function //Only for MCP2 using //In release API, Please use APS_get_command_counter
I32 FNTYPE APS_get_raw_cmd_counter( I32 Axis_ID, I32 *Counter );

//Reboot DSP [Only for PCI-8254/8]
I32 FNTYPE APS_reboot_dsp( I32 Board_ID );

#ifdef __cplusplus
}
#endif
#endif