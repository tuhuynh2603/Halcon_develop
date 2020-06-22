#include "stdafx.h"
#include "Application.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LOTSUMMARY::LOTSUMMARY()
{
	Init();
}

void LOTSUMMARY::Init()
{
	strOperatorID.Empty();
	strLotID.Empty();
	strPkgName.Empty();
	strComments.Empty();

	strStartLotTime.Empty();
	strEndLotTime.Empty();
	strStartTimeDay.Empty();
	strEndTimeDay.Empty();
	strStartTimeHr.Empty();
	strEndTimeHr.Empty();
	strDate.Empty();
	//for(int nDie=0;nDie<MAX_DIES;nDie++){
	//	for(int j=0;j<MAX_FOV;j++){
	//		for(int i=0; i<MAX_VIEWS; i++) {
	//			m_DisplaySummary[nDie][j][i].Init();
	//		}
	//	}
	//}
	memset(&timePrevRecord, 0, sizeof(SYSTEMTIME));
}


void LOTSUMMARY::InitDisplaySummary(int nDoc,int nFov)
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	if(nFov==-1 ||nDoc==-1)
		for (int nView = 0; nView <pApp->m_nTotalTracks; nView++) {
			///*for (int j = 0; j < MAX_FOV; j++) {*/
			//	for (int i = 0; i < MAX_VIEWS; i++)
			//		m_DisplaySummary[nDie][0][i].Init();
			//		;
			////}
				m_TotalDisplaySummary[nView].Init();
				for(std::map<int, long>::iterator it = m_mapIndividualPviDefectCounter[nView].begin(); it != m_mapIndividualPviDefectCounter[nView].end(); it++) {
					it->second = 0;
				}
		}
	/*else
		;*/
	//	m_DisplaySummary[0][nFov][nDoc].Init(); 
}


TRACKSUMMARY::TRACKSUMMARY()
{
	Init();
}

void TRACKSUMMARY::Init()
{
	lTotalUnitsInspected = 0;
	lTotalUnitsPass	= 0;

	lTotalUnitsFail	= 0;
	lTotalUnitsInvalid = 0;
	dTotalYield = 0;


	lDieLocation			= 0;
	lDatumLocation			= 0;
	lEncapInsufficient		= 0;
	lEncapExcess			= 0;
	lEncapCrack				= 0;
	lEncapBlowHole			= 0;
	lEncapContamination		= 0;
	lEncapVoid				= 0;
	lEncapWireAndIC			= 0;
	lEncapBulge				= 0;
	lEncapGlue				= 0;
	//Contact Error
	lContactDamage			= 0;
	lContactContamination	= 0;
	//Flex Error
	lFlexOvercut			= 0;
	lFlexDamage				= 0;
	lFlexContamination		= 0;
	lFlexScratches			= 0;

	lChickletRoundness		= 0;
	lChickletBurr			= 0;
	lDieEdge				= 0;
	lUserModified			= 0;
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			lPviDefectCounter[i][j] = 0;

	for (int k= 0; k<MAX_FOV;k++)
		for (int l = 0; l<MAX_VIEWS; l++)
			for (int i = 0; i < 10; i++)
				for (int j = 0; j < 10; j++)
					lTotalPviDefectCounter[k][l][i][j] = 0;
}