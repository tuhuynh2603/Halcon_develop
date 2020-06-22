#include "StdAfx.h"
#include "IDL2300.h"
#include "IOCode.h"
//#include "Controller.h"
#include "PerformanceTimer.h"
#include "DebugLog.h"
#include <math.h>
#include <vector>

//CController* pController = NULL;

//////////////////////////////////////////////////////////////////////////////
#ifndef _countof
#define _countof(x) (sizeof (x)/sizeof (x[0]))
#endif // _countof

#define CHECK_ERROR(errCode) \
	if ((errCode)!=ERR_NOERROR) \
		return Error (#errCode, m_sensorInstance);

volatile int nValuesForFrame;
HANDLE event = NULL;

int PrintError(CString strMsg)
{
	//_tprintf(_T("Error!\n%s"), err);
	OutputDebugLogTo(0, TRUE, "[IDL2300] %s", strMsg);
	return -1;
}

int Error(LPCTSTR err, int sensor)
{
	char out[1024], buf[1024];
	GetError(sensor, buf, sizeof(buf));
	OutputDebugLogTo(6, TRUE, "[IDL2300] Error in %s\n%s", err, buf);
	//_stprintf(out, _T("Error in %s\n%s"), err, buf);
	return -1;// PrintError(out);
}


int _stdcall InitSensorDLL(DWORD &m_sensorInstance)
{
	
	m_sensorInstance = CreateSensorInstance(SENSOR_ILD2300);
	if (!m_sensorInstance) {
		OutputDebugLogTo(0, TRUE, "[IDL2300] create driver instance failed**********");
		return -1;
	}
	
	return 0;

}

int _stdcall FreeSensorDLL(DWORD &m_sensorInstance)
{
	if (m_sensorInstance)
	{
		::CloseSensor(m_sensorInstance);
		::ReleaseSensorInstance(m_sensorInstance);
		m_sensorInstance = NULL;
	}
	return 0;
}



int _stdcall OpenInstance(DWORD &m_sensorInstance,CString strIPAddress)
{
	// If logging should be enabled
	// EnableLogging (sensor, TRUE, 31, 127, _T(".\\Log.txt"), TRUE, FALSE, 0);
	if (m_sensorInstance == NULL)
	{
		m_sensorInstance = CreateSensorInstance(SENSOR_ILD2300);
		if (!m_sensorInstance) {
			OutputDebugLogTo(0, TRUE, "[IDL2300] create driver instance failed**********");
			return -1;
		}

	}
	CHECK_ERROR(OpenSensorTCPIP(m_sensorInstance, strIPAddress/*_T("169.254.168.150")*/));

	return ERR_NOERROR;
}

int _stdcall GetAllParameters(DWORD &m_sensorInstance)
{
	CString str;
	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Get_AllParameters"), _T("SP_Additional"), 1));

	int iErr = 0;
	ERR_CODE err = GetParameterInt(m_sensorInstance, _T("SA_ErrorNumber"), &iErr);
	if (err != ERR_NOERROR && err != ERR_NOT_FOUND)
		return Error(_T("GetParameterInt (SA_ErrorNumber)"), m_sensorInstance);
	if (iErr != 0)
	{
		char cErr[1024], buf[1024];
		DWORD len = sizeof(cErr);
		CHECK_ERROR(GetParameterString(m_sensorInstance, _T("SA_ErrorText"), cErr, &len));
		_stprintf(buf, _T("Sensor returned error code after command Get_Settings\n%d: %s"), iErr, cErr);
		return PrintError(buf);
	}

	double range;
	CHECK_ERROR(GetParameterDouble(m_sensorInstance, _T("SA_Range"), &range));
	str.Format(("Sensor range: %.0f mm\n"), range);
	PrintError(str);
	//_tprintf(_T("Sensor range: %.0f mm\n"), range);

	double measRate;
	CHECK_ERROR(GetParameterDouble(m_sensorInstance, _T("SA_Measrate"), &measRate));
	str.Format(("Sensor speed: %.1f kHz\n"), measRate);
	PrintError(str);
	//_tprintf(_T("Sensor speed: %.1f kHz\n"), measRate);

	return ERR_NOERROR;
}

int _stdcall GetTransmittedDataInfo(DWORD &m_sensorInstance)
{
	CString str;
	int iCnt = 0;
	CHECK_ERROR(ExecSCmdGetInt(m_sensorInstance, _T("Get_TransmittedDataInfo"), _T("IA_ValuesPerFrame"), &iCnt));

	TCHAR name[256], sVal[1024];
	int iVal;
	DWORD iLen;
	double dVal;
	//_tprintf(_T("GetTransmittedDataInfo:\r\n"));
	PrintError("GetTransmittedDataInfo:");
	for (int i = 0; i<iCnt; i++)
	{
		_stprintf(name, _T("IA_Index%d"), i + 1);
		CHECK_ERROR(GetParameterInt(m_sensorInstance, name, &iVal));
		//_tprintf(_T("%2d: "), iVal);
		str.Format(("%s = %2d"), name,iVal);
		PrintError(str);


		_stprintf(name, _T("IA_Raw_Name%d"), i + 1);
		iLen = sizeof(sVal);
		CHECK_ERROR(GetParameterString(m_sensorInstance, name, sVal, &iLen));
		//_tprintf(_T("%s ["), sVal);
		str.Format(("%s = %s"), name, sVal);
		PrintError(str);

		_stprintf(name, _T("IA_Raw_RangeMin%d"), i + 1);
		CHECK_ERROR(GetParameterDouble(m_sensorInstance, name, &dVal));
		//_tprintf(_T("%.1f - "), dVal);
		str.Format(("%s = %.1f"), name, dVal);
		PrintError(str);

		_stprintf(name, _T("IA_Raw_RangeMax%d"), i + 1);
		CHECK_ERROR(GetParameterDouble(m_sensorInstance, name, &dVal));
		//_tprintf(_T("%.1f "), dVal);
		str.Format(("%s = %.1f"), name, dVal);
		PrintError(str);

		_stprintf(name, _T("IA_Raw_Unit%d"), i + 1);
		iLen = sizeof(sVal);
		CHECK_ERROR(GetParameterString(m_sensorInstance, name, sVal, &iLen));
		//_tprintf(_T("%s] @ "), sVal);
		str.Format(("%s = %s"), name, sVal);
		PrintError(str);

		_stprintf(name, _T("IA_Raw_Datarate%d"), i + 1);
		CHECK_ERROR(GetParameterDouble(m_sensorInstance, name, &dVal));
		//_tprintf(_T("%.1f Hz\r\n    "), dVal);
		str.Format(("%s = %.1fHz"), name, dVal);
		PrintError(str);

		_stprintf(name, _T("IA_Scaled_Name%d"), i + 1);
		iLen = sizeof(sVal);
		CHECK_ERROR(GetParameterString(m_sensorInstance, name, sVal, &iLen));
		//_tprintf(_T("%s ["), sVal);
		str.Format(("%s = %s"), name, sVal);
		PrintError(str);

		_stprintf(name, _T("IA_Scaled_RangeMin%d"), i + 1);
		CHECK_ERROR(GetParameterDouble(m_sensorInstance, name, &dVal));
		//_tprintf(_T("%.1f - "), dVal);
		str.Format(("%s = %.1f"), name, dVal);
		PrintError(str);

		_stprintf(name, _T("IA_Scaled_RangeMax%d"), i + 1);
		CHECK_ERROR(GetParameterDouble(m_sensorInstance, name, &dVal));
		//_tprintf(_T("%.1f "), dVal);
		str.Format(("%s = %.1f"), name, dVal);
		PrintError(str);

		_stprintf(name, _T("IA_Scaled_Unit%d"), i + 1);
		iLen = sizeof(sVal);
		CHECK_ERROR(GetParameterString(m_sensorInstance, name, sVal, &iLen));
		//_tprintf(_T("%s] @ "), sVal);
		str.Format(("%s = %s"), name, sVal);
		PrintError(str);

		_stprintf(name, _T("IA_Scaled_Datarate%d"), i + 1);
		CHECK_ERROR(GetParameterDouble(m_sensorInstance, name, &dVal));
		//_tprintf(_T("%.1f Hz\r\n"), dVal);
		str.Format(("%s = %.1fHz"), name, dVal);
		PrintError(str);
	}

	if(iCnt > 0)
		nValuesForFrame = iCnt;


//	OutputDebugLogTo(7, TRUE, "nValuesForFrame  cnt= %d", nValuesForFrame);

	return iCnt;
}

int _stdcall ProcessInstance(DWORD &m_sensorInstance, double &dDataReceived, std::vector <double> &zValues, int nValuesToRead)
{
	if(nValuesToRead > 10000)
		return -1;

	int avail, read, raw[10000];
	double scaled[10000];
	if(m_sensorInstance) {
		DataAvail(m_sensorInstance, &avail);
		if(avail < nValuesToRead || avail < 0)
			nValuesToRead = avail;

		CHECK_ERROR(TransferData(m_sensorInstance, raw, scaled, nValuesToRead, &read));
		if(read>0) {
			for(int j = 0; j < nValuesToRead; j++)
				zValues.push_back(scaled[j]);
			//zValues.assign(scaled, scaled + nValuesToRead);
			dDataReceived = scaled[read - nValuesToRead];
		}
	}

	return 0;
}

int _stdcall ProcessInstance_Old(DWORD &m_sensorInstance, double &dDataReceived, std::vector <double> &zValues, int nValuesToRead)
{

	int avail, read, raw[10000];
	//int avail, read, *raw; 
	double scaled[10000];
//	double *scaled;
//	OutputDebugLogTo(0, TRUE, "[IDL2300]1");
	if (m_sensorInstance) {
		DataAvail(m_sensorInstance, &avail);

		if(avail < nValuesToRead || avail < 0)
			nValuesToRead = avail;


		read = min(sizeof(scaled)/sizeof(double), avail);
		read = read - (read%nValuesToRead);
		OutputDebugLogTo(0, TRUE, "Avail %d, Values Read %d", avail, nValuesToRead);
		//TransferData(m_sensorInstance, raw, scaled, read, &read);
		CHECK_ERROR(TransferData(m_sensorInstance, raw, scaled, read, &read));

		if (read>0)
		{
			for(int j = 0; j < nValuesToRead; j++)
				zValues.push_back(scaled[read - nValuesToRead + j]);

			dDataReceived = scaled[read - nValuesToRead];
		}
	}

	return 0;
}

int _stdcall ProcessDataAvail(DWORD &m_sensorInstance, int &nDataAvail)
{
	
	if(m_sensorInstance) 
		CHECK_ERROR(DataAvail(m_sensorInstance, &nDataAvail));

	return 0;
}


int _stdcall ProcessInstancePoll(DWORD &m_sensorInstance, double &dDataReceived)
{
	if(nValuesForFrame < 0)
		nValuesForFrame = 1;

	int avail, read, raw;
	double scaled;
	if(m_sensorInstance) {

		CHECK_ERROR(Poll(m_sensorInstance, &raw, &scaled, 1/*only one value*/));

		dDataReceived = scaled;
	}

	return 0;
}

int _stdcall SetMasterValue(DWORD &m_sensorInstance, double dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;

	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, "Set_MasterValue", "SP_Master", 0/* = Distance */));
//	CHECK_ERROR(SetDoubleExecSCmd(m_sensorInstance, "Set_MasterValue", "SP_MasterValue", dSetData));
	

//	CHECK_ERROR(ExecSCmdGetInt(m_sensorInstance, _T("Set_MasterValue"), _T("SP_Master"), &iCnt));
	
//	if (iCnt > 0) {
		CHECK_ERROR(SetParameterDouble(m_sensorInstance, "SP_MasterValue", dSetData));
//		int err = SensorCommand(m_sensorInstance);

		//_tprintf(_T("%.1f - "), dVal);
		str.Format(("%s = %.1f"), "SP_Master", dSetData);
		PrintError(str);
//	}
		OutputDebugLogTo(7, TRUE, "Set_MasterValue %d", dSetData);
//		CHECK_ERROR(ExecSCmd(m_sensorInstance, "SP_MasterValue"));

	return 0;
}

int _stdcall GetMasterValue(DWORD &m_sensorInstance, double &dSetData)
{
	int iCnt;
	TCHAR name[256], sVal[1024];
	CString str;


	SetParameterString(m_sensorInstance, "S_Command", "Get_MasterValue");
	int err = SensorCommand(m_sensorInstance);
	GetParameterDouble(m_sensorInstance, "SA_MasterValue", &dSetData);


	//CHECK_ERROR(ExecSCmdGetInt(m_sensorInstance, "Get_MasterValue", "SA_Master", &iCnt));
//	CHECK_ERROR(SetDoubleExecSCmd(m_sensorInstance, _T("Get_MasterValue"), _T("SA_Master"), iCnt));

//	CHECK_ERROR(ExecSCmdGetInt(m_sensorInstance, _T("Get_MasterValue"), _T("SA_Master"), &iCnt));

//	if (iCnt > 0) {
//		CHECK_ERROR(GetParameterDouble(m_sensorInstance, "SA_MasterValue", &dSetData));
		//_tprintf(_T("%.1f - "), dVal);
		str.Format(("%s = %.1f"), "SA_MasterValue", dSetData);
		PrintError(str);
//	}
	return err;
}

int _stdcall Set_Samplerate(DWORD &m_sensorInstance, double dSetData)
{
	double iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;

	SetParameterString(m_sensorInstance, "S_Command", "Set_Samplerate");
	SetParameterDouble(m_sensorInstance, "SP_Measrate", dSetData);
	int err = SensorCommand(m_sensorInstance);

//	SetIntExecSCmd(m_sensorInstance, "Set_Samplerate", "SP_Measrate", iCnt/* = Distance */);

//	CHECK_ERROR(SetDoubleExecSCmd(m_sensorInstance, _T("Set_Samplerate"), _T("SP_Measrate"), iCnt));
//	if (iCnt > 0) {
//		CHECK_ERROR(SetParameterDouble(m_sensorInstance, "SP_Measrate", dSetData));
		//_tprintf(_T("%.1f - "), dVal);
		str.Format(("%s = %.1f"), "SP_Measrate", dSetData);
		PrintError(str);
//	}
		return err;
}

int _stdcall Get_Samplerate(DWORD &m_sensorInstance, double &dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;
	CHECK_ERROR(ExecSCmdGetInt(m_sensorInstance, "Get_Samplerate", "SA_Measrate", &iCnt));
//	CHECK_ERROR(SetDoubleExecSCmd(m_sensorInstance, _T("Get_Samplerate"), _T("SA_Measrate"), iCnt));

//	if (iCnt > 0) {
		CHECK_ERROR(GetParameterDouble(m_sensorInstance, "SA_Measrate", &dSetData));
		//_tprintf(_T("%.1f - "), dVal);
		str.Format(("%s = %.1f"), "SA_Measrate", dSetData);
		PrintError(str);
//	}
		return 0;
}

int _stdcall Set_MeasureMode(DWORD &m_sensorInstance, int dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;


	int err = SetIntExecSCmd(m_sensorInstance, "Set_MeasureMode", "SP_MeasureMode", dSetData/* = Distance */);


	return 0;
////	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Set_MeasureMode"), _T("SP_MeasureMode"), iCnt));
////	if (iCnt > 0) {
//		CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_MeasureMode", dSetData));
//		//_tprintf(_T("%.1f - "), dVal);
//		str.Format(("%s = %.1f"), "SP_MeasureMode", dSetData);
//		PrintError(str);
////	}
}

int _stdcall Get_MeasureMode(DWORD &m_sensorInstance, int &dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;

	CHECK_ERROR(ExecSCmdGetInt(m_sensorInstance, "Get_MeasureMode", "SA_MeasureMode", &dSetData));


////	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Get_MeasureMode"), _T("SA_MeasureMode"), iCnt));
//
////	if (iCnt > 0) {
//		CHECK_ERROR(GetParameterInt(m_sensorInstance, "SP_MeasureMode", &dSetData));
//		//_tprintf(_T("%.1f - "), dVal);
//		str.Format(("%s = %.1f"), "SP_MeasureMode", dSetData);
//		PrintError(str);
////	}
	return 0;

}

int _stdcall Set_MeasurePeak(DWORD &m_sensorInstance, int dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;

	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, "Set_MeasurePeak", "SP_MeasurePeak", dSetData/* = Distance */));

//	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Set_MeasurePeak"), _T("SP_MeasurePeak"), iCnt));
//	if (iCnt > 0) {
//		CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_MeasurePeak", dSetData));
//		//_tprintf(_T("%.1f - "), dVal);
//		str.Format(("%s = %.1f"), "SP_MeasurePeak", dSetData);
//		PrintError(str);
////	}
	return 0;
}

int _stdcall Get_MeasurePeak(DWORD &m_sensorInstance, int &dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;

	CHECK_ERROR(ExecSCmdGetInt(m_sensorInstance, "Get_MeasurePeak", "SA_MeasurePeak", &dSetData));
//	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Get_MeasurePeak"), _T("SA_MeasurePeak"), iCnt));

//	if (iCnt > 0) {
		//CHECK_ERROR(GetParameterInt(m_sensorInstance, "SP_MeasurePeak", &dSetData));
		////_tprintf(_T("%.1f - "), dVal);
		//str.Format(("%s = %.1f"), "SP_MeasurePeak", dSetData);
		//PrintError(str);
//	}
	return 0;
}

int _stdcall Set_LaserPower(DWORD &m_sensorInstance, int dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;

	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, "Set_LaserPower", "SP_LaserPower", dSetData));
//	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Set_LaserPower"), _T("SP_LaserPower"), iCnt));
//	if (iCnt > 0) {
		//CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_LaserPower", dSetData));
		////_tprintf(_T("%.1f - "), dVal);
		//str.Format(("%s = %.1f"), "SP_LaserPower", dSetData);
		//PrintError(str);
//	}
	return 0;
}

int _stdcall Get_LaserPower(DWORD &m_sensorInstance, int &dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;

	CHECK_ERROR(ExecSCmdGetInt(m_sensorInstance, "Get_LaserPower", "SA_LaserPower", &dSetData));
	//	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Get_LaserPower"), _T("SA_LaserPower"), iCnt));

//	if (iCnt > 0) {
	/*	CHECK_ERROR(GetParameterInt(m_sensorInstance, "SP_LaserPower", &dSetData));*/
		//_tprintf(_T("%.1f - "), dVal);
		str.Format(("%s = %.1f"), "SP_LaserPower", dSetData);
		PrintError(str);
//	}
		return 0;
}

int _stdcall Set_TriggerMode(DWORD &m_sensorInstance, int dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;
	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, "Set_TriggerMode", "SP_TriggerMode", dSetData));


//	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Set_TriggerMode"), _T("SP_TriggerMode"), iCnt));
//	if (iCnt > 0) {
//		CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_TriggerMode", dSetData));
		//_tprintf(_T("%.1f - "), dVal);
		str.Format(("%s = %.1f"), "SP_TriggerMode", dSetData);
		PrintError(str);
//	}
		return 0;
}

int _stdcall Get_TriggerMode(DWORD &m_sensorInstance, int &dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;
//	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Get_TriggerMode"), _T("SA_TriggerMode"), iCnt));

	CHECK_ERROR(ExecSCmdGetInt(m_sensorInstance, "Get_TriggerMode", "SA_TriggerMode", &dSetData));

//	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, "Set_TriggerMode", "SP_TriggerMode", dSetData));
//	if (iCnt > 0) {
//		CHECK_ERROR(GetParameterInt(m_sensorInstance, "SP_TriggerMode", &dSetData));
		//_tprintf(_T("%.1f - "), dVal);
		str.Format(("%s = %.1f"), "SP_TriggerMode", dSetData);
		PrintError(str);
//	}
		return 0;
}

int _stdcall Set_DataOutInterface(DWORD &m_sensorInstance)
{
	int iCnt = 1;
	TCHAR name[256], sVal[1024];
	CString str;

	CHECK_ERROR(SetParameterString(m_sensorInstance, "S_Command", "Set_DataOutInterface"));
	CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_OutputRS422", 0));// disable RS422
	CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_OutputAnalog", 0));// disable analog
	CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_OutputEthernet", 1));// enable Ethernet
	str.Format(("%s = %d"), "SP_OutputEthernet", 1);
	PrintError(str);

	CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_OutputErrorOut", 0));// disable error

	int err = SensorCommand(m_sensorInstance);

	return 0;
}

int _stdcall Set_Averaging(DWORD &m_sensorInstance, int nMovingCount)
{
	int iCnt = 1;
	TCHAR name[256], sVal[1024];
	CString str;
//	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Set_Averaging"), _T("SP_AveragingType"), iCnt));
//	if (iCnt > 0) {

	CHECK_ERROR(SetParameterString(m_sensorInstance, "S_Command", "Set_Averaging"));
	CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_AveragingType", 1));
	str.Format(("%s = %d"), "SP_AveragingType", 1);
	PrintError(str);

	CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_MovingCount", nMovingCount));
	str.Format(("%s = %d"), "SP_MovingCount", nMovingCount);
	PrintError(str);

	int err = SensorCommand(m_sensorInstance);


//	CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_AveragingType", dSetData));
//	//_tprintf(_T("%.1f - "), dVal);
//	str.Format(("%s = %.1f"), "SP_MovingCount", dSetData);
//	PrintError(str);
//
//
//		CHECK_ERROR(SetParameterInt(m_sensorInstance, "SP_MovingCount", dSetData));
//		//_tprintf(_T("%.1f - "), dVal);
//		str.Format(("%s = %.1f"), "SP_MovingCount", dSetData);
//		PrintError(str);
////	}
		return 0;
}



int _stdcall Get_Averaging(DWORD &m_sensorInstance, int &dSetData)
{
	int iCnt = 0;
	TCHAR name[256], sVal[1024];
	CString str;

	CHECK_ERROR(ExecSCmdGetInt(m_sensorInstance, "Get_Averaging", "SA_AveragingType", &dSetData));

//	CHECK_ERROR(SetIntExecSCmd(m_sensorInstance, _T("Get_TriggerMode"), _T("SA_TriggerMode"), iCnt));

//	if (iCnt > 0) {
//		CHECK_ERROR(GetParameterInt(m_sensorInstance, "SA_TriggerMode", &dSetData));
		//_tprintf(_T("%.1f - "), dVal);
		str.Format(("%s = %.1f"), "SP_TriggerMode", dSetData);
		PrintError(str);
//	}
		return 0;

}

int _stdcall Clear_Buffer(DWORD &m_sensorInstance)
{
	int nError = 0;
	nError	= ExecSCmd(m_sensorInstance, "Clear_Buffers");

	if(event != NULL)
		ResetEvent(event);

	return nError;
}

int _stdcall DataAvailEvent(DWORD &m_sensorInstance, int nReadData)
{
	int err = SetIntExecSCmd(m_sensorInstance, "DataAvail_Event", "IP_EventOnAvailableValues", nReadData);
	/* error handling, if err!=ERR_NOERROR */
	
	GetParameterDWORD_PTR(m_sensorInstance, "IA_DataAvailEvent", (DWORD_PTR *)&event);

	return 0;
}

int _stdcall ReadBlockData(DWORD &m_sensorInstance, int nReadData, std::vector <double> &zValues)
{
	int avail, read = 0;

	int* raw = NULL;
	double*scaled = NULL;
	if (nReadData > 0) {
		raw = new int[nReadData];
		scaled = new double[nReadData];
	}
	else {
		raw = new int[10000];
		scaled = new double[10000];
	}
	
	if(m_sensorInstance) {

		if(WaitForSingleObject(event, 1000) == WAIT_OBJECT_0)
			CHECK_ERROR(TransferData(m_sensorInstance, raw, scaled, nReadData, &read));

		//		OutputDebugLogTo(0, TRUE, "ReadData %d, Read %d", nReadData, read);

		if(read > 0) {
			for(int j = 0; j < read; j++)
				zValues.push_back(scaled[j]);
			//zValues.assign(scaled, scaled + nValuesToRead);	
		}
	}

	return 0;
}

