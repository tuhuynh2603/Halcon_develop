#pragma once
#include <vector>

typedef int (CALLBACK* INIT_SENSOR_DLL)(DWORD &m_SensorInstance);

typedef int (CALLBACK* FREE_SENSOR_DLL)(DWORD &m_SensorInstance);

typedef int (CALLBACK* OPEN_INSATNCE)(DWORD &m_SensorInstance,CString strIPAddress);

typedef int (CALLBACK* GET_ALL_PARAMETERS)(DWORD &m_SensorInstance);

typedef int (CALLBACK* GET_TRANMITTED_DATA_INFO)(DWORD &m_SensorInstance);

typedef int (CALLBACK* PROCESS_INSTANCE)(DWORD &m_SensorInstance, double &dDataReceived, std::vector <double> & dZvalues, int nReadData);

typedef int (CALLBACK* PROCESS_INSTANCE_POLL)(DWORD &m_SensorInstance,double &dDataReceived);

typedef int (CALLBACK* GET_MASTERVALUE)(DWORD &m_SensorInstance, double &dDataReceived);

typedef int (CALLBACK* SET_MASTERVALUE)(DWORD &m_SensorInstance, double dDataReceived);

typedef int (CALLBACK* SET_SAMPLERATE)(DWORD &m_SensorInstance, double dDataReceived);

typedef int (CALLBACK* GET_SAMPLERATE)(DWORD &m_SensorInstance, double &dDataReceived);

typedef int (CALLBACK* SET_MEASUREMODE)(DWORD &m_SensorInstance, int nDataReceived);

typedef int (CALLBACK* GET_MEASUREMODE)(DWORD &m_SensorInstance, int &nDataReceived);

typedef int (CALLBACK* SET_MEASUREPEAK)(DWORD &m_SensorInstance, int nDataReceived);

typedef int (CALLBACK* GET_MEASUREPEAK)(DWORD &m_SensorInstance, int &nDataReceived);

typedef int (CALLBACK* SET_LASERPOWER)(DWORD &m_SensorInstance, int nDataReceived);

typedef int (CALLBACK* GET_LASERPOWER)(DWORD &m_SensorInstance, int &nDataReceived);

typedef int (CALLBACK* SET_TRIGGERMODE)(DWORD &m_SensorInstance, int nDataReceived);

typedef int (CALLBACK* GET_TRIGGERMODE)(DWORD &m_SensorInstance, int &nDataReceived);

typedef int (CALLBACK* SET_MOVING_AVERAGING)(DWORD &m_SensorInstance, int nMovingCount);

typedef int (CALLBACK* CLEAR_BUFFER)(DWORD &m_SensorInstance);

typedef int (CALLBACK* DATA_AVAIL)(DWORD &m_SensorInstance, int & nDataAvail);

typedef int (CALLBACK* DATA_AVAIL_EVENT)(DWORD &m_SensorInstance, int nReadData);

typedef int (CALLBACK* READ_BLOCK_DATA)(DWORD &m_SensorInstance, int nReadData, std::vector <double> &zValues);


class CTriangularSensor
{
public:
	HINSTANCE					hDLL;
	INIT_SENSOR_DLL				lpInitSensorDLL;
	FREE_SENSOR_DLL				lpFreeSensorDLL;
	OPEN_INSATNCE				lpOpenInstance;
	GET_ALL_PARAMETERS			lpGetAllParameters;
	GET_TRANMITTED_DATA_INFO	lpGetTransmittedDataInfo;
	PROCESS_INSTANCE			lpProcessInstance;
	PROCESS_INSTANCE_POLL		lpProcessInstancePoll;
	GET_MASTERVALUE				lpGetMasterValue;
	SET_MASTERVALUE				lpSetMasterValue;
	SET_SAMPLERATE				lpSetSampleRate;
	GET_SAMPLERATE				lpGetSampleRate;
	SET_MEASUREMODE				lpSetMeasureMode;
	GET_MEASUREMODE				lpGetMeasureMode;
	SET_MEASUREPEAK				lpSetMeasurePeak;
	GET_MEASUREPEAK				lpGetMeasurePeak;
	SET_LASERPOWER				lpSetLaserPower;
	GET_LASERPOWER				lpGetLaserPower;
	SET_TRIGGERMODE				lpSetTriggerMode;
	GET_TRIGGERMODE				lpGetTriggerMode;
	SET_MOVING_AVERAGING		lpSetMovingAveraging;
	CLEAR_BUFFER				lpClearBuffer;
	DATA_AVAIL					lpDataAvail;
	DATA_AVAIL_EVENT			lpDataAvailEvent;
	READ_BLOCK_DATA				lpReadBlockData;

	CTriangularSensor();

	int LoadSensorDll(CString strMotorCard);
	int InitSensor(DWORD &m_SensorInstance);
	int ReleaseSensor(DWORD &m_SensorInstance);
	int OpenInstance(DWORD &m_SensorInstance, CString strIPAddress);
	int ProcessInstance(DWORD &m_SensorInstance, double &dDataReceived, std::vector <double> & dZValues, int nReadData);
	int ProcessDataAvail(DWORD &m_SensorInstance, int &nDataAvail);
	int ProcessInstancePoll(DWORD &m_SensorInstance, double &dDataReceived);
	int GetAllParameters(DWORD &m_SensorInstance);
	int GetTransmittedDataInfo(DWORD &m_SensorInstance);
	int GetMasterValue(DWORD &m_SensorInstance, double &dData);
	int SetMasterValue(DWORD &m_SensorInstance, double dData);
	int Set_SampleRate(DWORD &m_SensorInstance, double dData);
	int Get_SampleRate(DWORD &m_SensorInstance, double &dData);
	int Set_MeasureMode(DWORD &m_SensorInstance, int nDataReceived);
	int Get_MeasureMode(DWORD &m_SensorInstance, int &nData);
	int Set_MeasurePeak(DWORD &m_SensorInstance, int dSetData);
	int Get_MeasurePeak(DWORD &m_SensorInstance, int &dSetData);
	int Set_LaserPower(DWORD &m_SensorInstance, int dSetData);
	int Get_LaserPower(DWORD &m_SensorInstance, int &dSetData);
	int Set_TriggerMode(DWORD &m_SensorInstance, int dSetData);
	int Set_Averaging(DWORD &m_SensorInstance, int dMovingCount);
	int Get_TriggerMode(DWORD &m_SensorInstance, int &dSetData);
	int Clear_Buffer(DWORD &m_SensorInstance);
	int Data_Avail_Event(DWORD &m_SensorInstance, int nReadData);
	int ReadBlockData(DWORD &m_SensorInstance, int nReadData, std::vector <double> &zValues);
};
