#ifndef _LOT_STRUCT
#define _LOT_STRUCT
#include<map>

class TRACKSUMMARY {

public :
	TRACKSUMMARY();
	void Init();

	long lTotalUnitsInspected;
	long lTotalUnitsPass;
	long lTotalUnitsFail;
	double dTotalYield;
	long lTotalUnitsInvalid;

	long lDieLocation;
	long lDatumLocation;

	long lEncapInsufficient;
	long lEncapExcess;
	long lEncapCrack;
	long lEncapBlowHole;
	long lEncapContamination;
	long lEncapVoid;
	long lEncapWireAndIC;
	long lEncapBulge;
	long lEncapGlue;
	// Contact Error
	long lContactDamage;
	long lContactContamination;
	// Flex Error
	long lFlexOvercut;
	long lFlexDamage;
	long lFlexContamination;
	long lFlexScratches;

	long lChickletRoundness;
	long lChickletBurr;
	long lDieEdge;
	long lUserModified;
	long lPviDefectCounter[10][10];
	long lTotalPviDefectCounter[36][12][10][10];

};


class LOTSUMMARY {

public :
	LOTSUMMARY();
	void Init();
	void InitDisplaySummary(int nDoc=-1,int nTrack=-1);

	CString	strMachineID;

	CString	strDate;

	CString	strStartLotTime;
	CString	strEndLotTime;

	CString	strStartTimeDay;
	CString	strEndTimeDay;
	CString	strStartTimeHr;
	CString	strEndTimeHr;

	CString strOperatorID;
	CString strLotID;
	CString strPkgName;
	CString strComments;

	time_t tmStartTimeHr;
	unsigned short nDisplayStatsResetMin;
	unsigned short nLotRecycleDays;
	SYSTEMTIME timePrevRecord;

	/*TRACKSUMMARY m_DailySummary[MAX_DIES][MAX_FOV][MAX_VIEWS];
	TRACKSUMMARY m_HourlySummary[MAX_DIES][MAX_FOV][MAX_VIEWS];*/
	TRACKSUMMARY m_DisplaySummary[MAX_DIES][MAX_FOV][MAX_VIEWS];
	TRACKSUMMARY m_TotalDisplaySummary[MAX_VIEWS];
	std::map<int, long> m_mapIndividualPviDefectCounter[MAX_VIEWS];
};

#endif