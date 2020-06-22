#include "stdafx.h"
#include "TriangularSensor.h"
#include "boost/array.hpp"
#include <vector>

#include "Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTriangularSensor::CTriangularSensor()
{
	hDLL = 0;

	lpInitSensorDLL = 0;
	lpFreeSensorDLL = 0;
	lpOpenInstance = 0;
	lpGetAllParameters = 0;
	lpGetTransmittedDataInfo = 0;
	lpProcessInstance = 0;
	lpProcessInstancePoll = 0;
	lpGetMasterValue = 0;
	lpSetMasterValue = 0;
	lpSetSampleRate = 0;
	lpGetSampleRate = 0;
	lpSetMeasureMode = 0;
	lpGetMeasureMode = 0;
	lpSetMeasurePeak = 0;
	lpGetMeasurePeak = 0;
	lpSetLaserPower = 0;
	lpGetLaserPower = 0;
	lpSetTriggerMode = 0;
	lpGetTriggerMode = 0;
	lpDataAvail = 0;
	
}

int CTriangularSensor::LoadSensorDll(CString strMotionCard)
{
	OutputDebugLogTo(0, TRUE, "Registering Sensor Card %s ...", strMotionCard);

	if (strMotionCard.CompareNoCase("none") != 0) {
		hDLL = LoadLibrary(strMotionCard);

		if (hDLL == NULL) {
			OutputDebugLogTo(0, TRUE, "Error Loading Sensor Card DLL: GetLastError %d", GetLastError());
			AfxMessageBox("Error Loading Sensor Card DLL");
			return -1;
		}

		lpInitSensorDLL = (INIT_SENSOR_DLL)GetProcAddress(hDLL, "InitSensorDLL");
		if (!lpInitSensorDLL)
			OutputDebugLogTo(0, TRUE, "Failed to register IO Card InitSensorDLL() Function");


		lpFreeSensorDLL = (FREE_SENSOR_DLL)GetProcAddress(hDLL, "FreeSensorDLL");
		if (!lpFreeSensorDLL)
			OutputDebugLogTo(0, TRUE, "Failed to register IO Card FreeSensorDLL() Function");


		lpOpenInstance = (OPEN_INSATNCE)GetProcAddress(hDLL, "OpenInstance");
		if (!lpOpenInstance)
			OutputDebugLogTo(0, TRUE, "Failed to register OpenInstance() Function");

		lpGetAllParameters = (GET_ALL_PARAMETERS)GetProcAddress(hDLL, "GetAllParameters");
		if (!lpGetAllParameters)
			OutputDebugLogTo(0, TRUE, "Failed to register GetAllParameters() Function");

		lpGetTransmittedDataInfo = (GET_TRANMITTED_DATA_INFO)GetProcAddress(hDLL, "GetTransmittedDataInfo");
		if (!lpGetTransmittedDataInfo)
			OutputDebugLogTo(0, TRUE, "Failed to register GetTransmittedDataInfo() Function");

		lpProcessInstance = (PROCESS_INSTANCE)GetProcAddress(hDLL, "ProcessInstance");
		if (!lpProcessInstance)
			OutputDebugLogTo(0, TRUE, "Failed to register ProcessInstance() Function");

		lpDataAvail = (DATA_AVAIL) GetProcAddress(hDLL, "ProcessDataAvail");
		if(!lpDataAvail)
			OutputDebugLogTo(0, TRUE, "Failed to register ProcessDataAvail() Function");

		lpProcessInstancePoll = (PROCESS_INSTANCE_POLL) GetProcAddress(hDLL, "ProcessInstancePoll");
		if(!lpProcessInstancePoll)
			OutputDebugLogTo(0, TRUE, "Failed to register ProcessInstance() Function");

		

		lpGetMasterValue = (GET_MASTERVALUE)GetProcAddress(hDLL, "GetMasterValue");
		if (!lpGetMasterValue)
			OutputDebugLogTo(0, TRUE, "Failed to register GetMasterValue() Function");

		lpSetMasterValue = (SET_MASTERVALUE)GetProcAddress(hDLL, "SetMasterValue");
		if (!lpSetMasterValue)
			OutputDebugLogTo(0, TRUE, "Failed to register SetMasterValue() Function");

		lpSetSampleRate = (SET_SAMPLERATE)GetProcAddress(hDLL, "Set_Samplerate");
		if (!lpSetSampleRate)
			OutputDebugLogTo(0, TRUE, "Failed to register Set_Samplerate() Function");

		lpGetSampleRate = (GET_SAMPLERATE)GetProcAddress(hDLL, "Get_Samplerate");
		if (!lpGetSampleRate)
			OutputDebugLogTo(0, TRUE, "Failed to register Get_Samplerate() Function");

		lpSetMeasureMode = (SET_MEASUREMODE)GetProcAddress(hDLL, "Set_MeasureMode");
		if (!lpSetMeasureMode)
			OutputDebugLogTo(0, TRUE, "Failed to register Set_MeasureMode() Function");

		lpGetMeasureMode = (GET_MEASUREMODE)GetProcAddress(hDLL, "Get_MeasureMode");
		if (!lpGetMeasureMode)
			OutputDebugLogTo(0, TRUE, "Failed to register Get_MeasureMode() Function");

		lpSetMeasurePeak = (SET_MEASUREPEAK)GetProcAddress(hDLL, "Set_MeasurePeak");
		if (!lpSetMeasurePeak)
			OutputDebugLogTo(0, TRUE, "Failed to register Set_MeasurePeak() Function");

		lpGetMeasurePeak = (GET_MEASUREPEAK)GetProcAddress(hDLL, "Get_MeasurePeak");
		if (!lpGetMeasurePeak)
			OutputDebugLogTo(0, TRUE, "Failed to register Get_MeasurePeak() Function");

		lpSetLaserPower = (SET_LASERPOWER)GetProcAddress(hDLL, "Set_LaserPower");
		if (!lpSetLaserPower)
			OutputDebugLogTo(0, TRUE, "Failed to register Set_LaserPower() Function");

		lpGetLaserPower = (GET_LASERPOWER)GetProcAddress(hDLL, "Get_LaserPower");
		if (!lpGetLaserPower)
			OutputDebugLogTo(0, TRUE, "Failed to register Get_LaserPower() Function");

		lpSetTriggerMode = (SET_TRIGGERMODE)GetProcAddress(hDLL, "Set_TriggerMode");
		if (!lpSetTriggerMode)
			OutputDebugLogTo(0, TRUE, "Failed to register Set_TriggerMode() Function");

		lpGetTriggerMode = (GET_TRIGGERMODE)GetProcAddress(hDLL, "Get_TriggerMode");
		if (!lpGetTriggerMode)
			OutputDebugLogTo(0, TRUE, "Failed to register Get_TriggerMode() Function");


		lpSetMovingAveraging = (SET_MOVING_AVERAGING) GetProcAddress(hDLL, "Set_Averaging");
		if(!lpSetMovingAveraging)
			OutputDebugLogTo(0, TRUE, "Failed to register Set_Averaging() Function");


		lpClearBuffer = (CLEAR_BUFFER) GetProcAddress(hDLL, "Clear_Buffer");
		if(!lpSetMovingAveraging)
			OutputDebugLogTo(0, TRUE, "Failed to register Clear_Buffer() Function");

		lpReadBlockData = (READ_BLOCK_DATA) GetProcAddress(hDLL, "ReadBlockData");
		if(!lpReadBlockData)
			OutputDebugLogTo(0, TRUE, "Failed to register ReadBlockData() Function");

		lpDataAvailEvent = (DATA_AVAIL_EVENT) GetProcAddress(hDLL, "DataAvailEvent");
		if(!lpDataAvailEvent)
			OutputDebugLogTo(0, TRUE, "Failed to register DataAvailEvent() Function");

	}
	else {
		OutputDebugLogTo(0, TRUE, "No Sensor Card");
	}

	return 0;
}

int CTriangularSensor::InitSensor(DWORD &m_SensorInstance)
{
	int nError;

	if (lpInitSensorDLL) {
		nError = lpInitSensorDLL(m_SensorInstance);
		if (nError) {
			AfxMessageBox("Failed to Register Sensor Card");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Registered Sensor Card");
	}
	else
		OutputDebugLogTo(0, TRUE, "No RegisterCard() Function");

	return 0;
}

int CTriangularSensor::ReleaseSensor(DWORD &m_SensorInstance)
{
	int nError;

	if (lpFreeSensorDLL) {
		nError = lpFreeSensorDLL(m_SensorInstance);
		if (nError) {
			AfxMessageBox("Failed to Release Sensor Card");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Released Sensor Card");
	}
	else
		OutputDebugLogTo(0, TRUE, "No ReleaseCard() Function");



	///if(lpSetServoOn != NULL)
	///	lpSetServoOn(nAxis, pAxisParm->nServoOn);

	///HomeMove(pMM, nAxis, -1000, -1000, -10000, 100, 100, true);
	///OutputDebugLogTo(0, TRUE, "HomeMove done");

	return 0;
}

int CTriangularSensor::OpenInstance(DWORD &m_SensorInstance, CString strIPAddress)
{
	int nError;

	if (lpOpenInstance) {
		nError = lpOpenInstance(m_SensorInstance,strIPAddress);
		if (nError) {
			OutputDebugLogTo(0, TRUE, "Failed to Open Traingualr Sensor");
//			AfxMessageBox("Failed to Open Traingualr Sensor");
			return nError;
		}
	//	else
	//		OutputDebugLogTo(0, TRUE, "Initialized OpenInstance");
	}
	else
		OutputDebugLogTo(0, TRUE, "No OpenInstance() Function");

	return 0;
}

int CTriangularSensor::ProcessInstance(DWORD &m_SensorInstance, double &dDataReceived, vector <double> &zValues, int nReadDatas)
{
	int nError;

	if (lpProcessInstance) {
		nError = lpProcessInstance(m_SensorInstance,dDataReceived, zValues, nReadDatas);
		if (nError) {
			OutputDebugLogTo(0, TRUE, "Failed to ProcessInstance");
			//AfxMessageBox("Failed to ProcessInstance");
			return nError;
		}
	//	else
	//		OutputDebugLogTo(0, TRUE, "Initialized ProcessInstance");
	}
	else
		OutputDebugLogTo(0, TRUE, "No ProcessInstance() Function");

	return 0;
}

int CTriangularSensor::ProcessDataAvail(DWORD &m_SensorInstance, int &DataAvail)
{
	int nError;

	if(lpDataAvail) {
		nError = lpDataAvail(m_SensorInstance, DataAvail);
		if(nError) {
			OutputDebugLogTo(0, TRUE, "Failed to DataAvail");
			//AfxMessageBox("Failed to ProcessInstance");
			return nError;
		}
		//	else
		//		OutputDebugLogTo(0, TRUE, "Initialized ProcessInstance");
	}
	else
		OutputDebugLogTo(0, TRUE, "No DataAvail() Function");

	return 0;
}

int CTriangularSensor::ProcessInstancePoll(DWORD &m_SensorInstance, double &dDataReceived)
{
	int nError;

	if(lpProcessInstancePoll) {
		nError = lpProcessInstancePoll(m_SensorInstance, dDataReceived);
		if(nError) {
			OutputDebugLogTo(0, TRUE, "Failed to ProcessInstance Poll");
			//AfxMessageBox("Failed to ProcessInstance");
			return nError;
		}
		//	else
		//		OutputDebugLogTo(0, TRUE, "Initialized ProcessInstance");
	}
	else
		OutputDebugLogTo(0, TRUE, "No ProcessInstancePoll() Function");

	return 0;
}

int CTriangularSensor::GetAllParameters(DWORD &m_SensorInstance)
{
	int nError;

	if (lpGetAllParameters) {
		nError = lpGetAllParameters(m_SensorInstance);
		if (nError) {
			OutputDebugLogTo(0, TRUE, "Failed GetAllParameters");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Initialized GetAllParameters");
	}
	else
		OutputDebugLogTo(0, TRUE, "No GetAllParameters() Function");

	return 0;
}

int CTriangularSensor::GetTransmittedDataInfo(DWORD &m_SensorInstance)
{
	int nError;

	if (lpGetTransmittedDataInfo) {
		nError = lpGetTransmittedDataInfo(m_SensorInstance);
		if (nError < 0) {
			OutputDebugLogTo(0, TRUE, "Failed GetTransmittedDataInfo");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Initialized GetTransmittedDataInfo");
	}
	else
		OutputDebugLogTo(0, TRUE, "No GetTransmittedDataInfo() Function");


	return 0;
}


int CTriangularSensor::SetMasterValue(DWORD &m_SensorInstance, double dData)
{
	int nError;

	if (lpSetMasterValue) {
		nError = lpSetMasterValue(m_SensorInstance,dData);
		if (nError) {
			AfxMessageBox("Failed SetMasterValue");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "SetMasterValue Success value = %f", dData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No SetMasterValue() Function");


	return 0;
}

int CTriangularSensor::GetMasterValue(DWORD &m_SensorInstance, double &dData)
{
	int nError;

	if (lpGetMasterValue) {
		nError = lpGetMasterValue(m_SensorInstance, dData);
		if (nError) {
			AfxMessageBox("Failed GetMasterValue");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "GetMasterValue Success value = %3.04f", dData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No GetMasterValue() Function");


	return 0;
}

int CTriangularSensor::Set_SampleRate(DWORD &m_SensorInstance, double dData)
{
	int nError;

	if (lpSetSampleRate) {
		nError = lpSetSampleRate(m_SensorInstance, dData);
		if (nError) {
			AfxMessageBox("Failed Set_SampleRate");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Set_SampleRate Success value = %f",dData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Set_SampleRate() Function");


	return 0;
}

int CTriangularSensor::Get_SampleRate(DWORD &m_SensorInstance, double &dData)
{
	int nError;

	if (lpGetSampleRate) {
		nError = lpGetSampleRate(m_SensorInstance, dData);
		if (nError) {
			AfxMessageBox("Failed Get_SampleRate");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Get_SampleRate Success value = %3.02f",dData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Get_SampleRate() Function");


	return 0;
}

int CTriangularSensor::Set_MeasureMode(DWORD &m_SensorInstance, int nDataReceived)
{
	int nError;

	if (lpSetMeasureMode) {
		nError = lpSetMeasureMode(m_SensorInstance, nDataReceived);
		if (nError) {
			AfxMessageBox("Failed Set_MeasureMode");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Set_MeasureMode Success value = %d", nDataReceived);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Set_MeasureMode() Function");


	return 0;
}

int CTriangularSensor::Get_MeasureMode(DWORD &m_SensorInstance, int &dData)
{
	int nError;

	if (lpGetMeasureMode) {
		nError = lpGetMeasureMode(m_SensorInstance, dData);
		if (nError) {
			AfxMessageBox("Failed Get_MeasureMode");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Get_MeasureMode Success value = %d", dData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Get_MeasureMode() Function");


	return 0;
}

int CTriangularSensor::Set_MeasurePeak(DWORD &m_SensorInstance, int nSetData)
{
	int nError;

	if (lpSetMeasurePeak) {
		nError = lpSetMeasurePeak(m_SensorInstance, nSetData);
		if (nError) {
			AfxMessageBox("Failed Set_MeasurePeak");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Set_MeasurePeak Success value = %d", nSetData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Set_MeasurePeak() Function");


	return 0;
}

int CTriangularSensor::Get_MeasurePeak(DWORD &m_SensorInstance, int &nSetData)
{
	int nError;

	if (lpGetMeasurePeak) {
		nError = lpGetMeasurePeak(m_SensorInstance, nSetData);
		if (nError) {
			AfxMessageBox("Failed Get_MeasurePeak");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Get_MeasurePeak Success value = %d", nSetData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Get_MeasurePeak() Function");


	return 0;
}

int CTriangularSensor::Set_LaserPower(DWORD &m_SensorInstance, int nSetData)
{
	int nError;

	if (lpSetLaserPower) {
		nError = lpSetLaserPower(m_SensorInstance, nSetData);
		if (nError) {
			AfxMessageBox("Failed Set_LaserPower");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Set_LaserPower Success value = %d", nSetData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Set_LaserPower() Function");


	return 0;
}

int CTriangularSensor::Get_LaserPower(DWORD &m_SensorInstance, int &nSetData)
{
	int nError;

	if (lpGetLaserPower) {
		nError = lpGetLaserPower(m_SensorInstance, nSetData);
		if (nError) {
			AfxMessageBox("Failed Get_LaserPower");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Get_LaserPower Success value = %d", nSetData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Get_LaserPower() Function");


	return 0;
}

int CTriangularSensor::Set_TriggerMode(DWORD &m_SensorInstance, int nSetData)
{
	int nError;

	if (lpSetTriggerMode) {
		nError = lpSetTriggerMode(m_SensorInstance, nSetData);
		if (nError) {
			AfxMessageBox("Failed Set_TriggerMode");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Set_TriggerMode Success value = %d", nSetData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Set_TriggerMode() Function");

	return 0;
}

int CTriangularSensor::Get_TriggerMode(DWORD &m_SensorInstance, int &nSetData)
{
	int nError;

	if (lpGetTriggerMode) {
		nError = lpGetTriggerMode(m_SensorInstance, nSetData);
		if (nError) {
			AfxMessageBox("Failed Get_TriggerMode");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Get_TriggerMode Success value = %d", nSetData);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Get_TriggerMode() Function");

	return 0;
}

int CTriangularSensor::Set_Averaging(DWORD &m_SensorInstance, int nMovingCount)
{
	int nError;

	if(lpSetMovingAveraging) {
		nError = lpSetMovingAveraging(m_SensorInstance, nMovingCount);
		if(nError) {
			OutputDebugLogTo(0, TRUE, "Failed Set_Averaging");
			return nError;
		}
		else
			OutputDebugLogTo(0, TRUE, "Set_Averaging Success value = %d", nMovingCount);
	}
	else
		OutputDebugLogTo(0, TRUE, "No Set_Averaging() Function");

	return 0;
}

int CTriangularSensor::Clear_Buffer(DWORD &m_SensorInstance)
{
	int nError;

	if(lpClearBuffer) {
		nError = lpClearBuffer(m_SensorInstance);
		if(nError) {
			OutputDebugLogTo(0, TRUE,"Failed Clear Buffer Error No %d", nError);
			return nError;
		}
		/*else
			OutputDebugLogTo(0, TRUE, "Clear Buffer Success");*/
	}
	else
		OutputDebugLogTo(0, TRUE, "No Clear Buffer Function");

}

int CTriangularSensor::ReadBlockData(DWORD &m_SensorInstance, int nReadData, std::vector <double> &zValues)
{
	int nError;

	if(lpReadBlockData) {
		nError = lpReadBlockData(m_SensorInstance, nReadData, zValues);
		if(nError) {
			AfxMessageBox("Failed to Read Block");
			return nError;
		}
		/*else
		OutputDebugLogTo(0, TRUE, "Clear Buffer Success");*/
	}
	else
		OutputDebugLogTo(0, TRUE, "Failed to Read Block");

}

int CTriangularSensor::Data_Avail_Event(DWORD &m_SensorInstance, int nReadData)
{
	int nError;

	if(lpDataAvailEvent) {
		nError = lpDataAvailEvent(m_SensorInstance, nReadData);
		if(nError) {
			AfxMessageBox("Failed to Create Data Avail Event");
			return nError;
		}
		/*else
		OutputDebugLogTo(0, TRUE, "Clear Buffer Success");*/
	}
	else
		OutputDebugLogTo(0, TRUE, "Failed to Create Data Avail Event");

}
