#ifndef _ADLINK_ERROR_CODE_DEF_H
#define _ADLINK_ERROR_CODE_DEF_H

#define ERR_NoError						(I16)0		//No Error	

// System Error ( -1 ~ -1000 )
#define ERR_OSVersion					(I16)-1		// Operation System type mismatched
#define ERR_OpenDriverFailed			(I16)-2		// Open device driver failed - Create driver interface failed
#define ERR_InsufficientMemory			(I16)-3		// System memory insufficiently
#define ERR_DeviceNotInitial			(I16)-4		// Cards not be initialized
#define ERR_NoDeviceFound				(I16)-5		// Cards not found(No card in your system)
#define ERR_CardIdDuplicate				(I16)-6		// Cards' ID Number duplicate 
#define ERR_DeviceAlreadyInitialed		(I16)-7		// Cards have been initialed 
#define ERR_InterruptNotEnable			(I16)-8		// Cards' interrupt events not enable or not be initialized
#define ERR_TimeOut						(I16)-9		// Function time out
#define ERR_ParametersInvalid			(I16)-10	// Function input parameters are invalid
#define ERR_SetEEPROM					(I16)-11	// Set data to EEPROM failed
#define ERR_GetEEPROM					(I16)-12	// Get data from EEPROM failed
#define ERR_FunctionNoAvailable			((I16)-13)	// Firmware not support.

#define ERR_Win32Error					(I16)-1000	// WIN32_API error, contact with ADLINK's FAE staff.




#endif  //_ADLINK_ERROR_CODE_DEF_H
