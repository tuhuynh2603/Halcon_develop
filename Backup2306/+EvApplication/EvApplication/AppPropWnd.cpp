/// AppPropWnd.cpp : implementation of the AppPropWnd class
/// Udupa; April'2012
/// Udupa; Jan'2013; Added logging feature (parameter value changes) and Unit of measurement option

#include "stdafx.h"
#include "Application.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include "StatisticsView.h"

#include "AppPropWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CAppPropWnd::CAppPropWnd()
{
	Initialize();
}

BEGIN_MESSAGE_MAP(CAppPropWnd, CEvPropertiesWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CAppPropWnd::Initialize()
{
	CApplication* pApp = (CApplication*) AfxGetApp();
	m_nPos =  MAX_FOV;
	m_nSets = pApp->m_nTotalViews;
	m_nTopSets = pApp->m_nTOP;
	m_nTHRSets = pApp->m_nTHR;

	/*m_nBottomSets = pApp->m_nBottom;
	m_nSideSets = pApp->m_nSide;*/

	strTopName.RemoveAll();
	strTHRName.RemoveAll();
	//strSideName.RemoveAll();

	strTopName.Copy(pApp->m_strTopCameraName);
	strTHRName.Copy(pApp->m_strTHRCameraName);
	//strSideName.Copy(pApp->m_strSideCameraName);

	m_nUnitIndex = pApp->m_nUnitIndex;
}

void CAppPropWnd::RefreshPropList(CEvImageDoc* pDoc, int nFOV)
{
	CApplication* pApp = (CApplication*) AfxGetApp();
	CAppDoc* pAppDoc = (CAppDoc*) pDoc;

	int nTrack, nDocId, nView;
	nTrack = pAppDoc->m_pTrack->m_nTrack;
	nDocId = pAppDoc->m_nDocIdx;

	//Get total FOV for current station
	int nCurrentFOV = pApp->m_nNoOfFov;
	if(nTrack == STATION_TOP) {
		nCurrentFOV = pApp->motionParameters.nFOVPerDieinXTOP * pApp->motionParameters.nFOVPerDieinYTOP;
	}
	else if(nTrack == STATION_THR) {
		nCurrentFOV = pApp->motionParameters.nFOVPerDieinXTHR * pApp->motionParameters.nFOVPerDieinYTHR;
	}
	if(nCurrentFOV > MAX_FOV) {
		nCurrentFOV = MAX_FOV;
	}

	if (pAppDoc->m_pTrack->m_pActiveDocTeach != NULL)
		nDocId = pAppDoc->m_pTrack->m_pActiveDocTeach->m_nDocIdx;

	m_wndObjectComboPos.ResetContent();
	for (int i = 0; i < nCurrentFOV/*pApp->m_nNoOfFov*/; i++) {
		CString str;
		str.Format("Position %d", i + 1);
		m_wndObjectComboPos.AddString(str);
	}
	m_wndObjectComboPos.SetCurSel(nFOV);
	//nView = nTrack/* 0*/;// !nTrack ? nDocId : (nTrack == 1 ? m_nTopSets + nDocId : m_nTopSets + m_nBottomSets + nDocId);
	nView = !nTrack ? nDocId : m_nTopSets + nDocId;
	m_wndObjectCombo.SetCurSel(nView);

	CEvPropertiesWnd::RefreshPropList(pDoc, nFOV);
}

void CAppPropWnd::InitPropList(CEvImageDoc* pDoc, int nFOV)
{
	if(!pDoc) {
		/*for(int i=0; i<m_nSets; i++)
			m_wndPropLists[i].RemoveAll();*/
		for(int i=0; i<m_nPos; i++) {
			for(int j=0; j<m_nSets; j++) {
				m_wndPropLists[i][j].RemoveAll();
			}
		}
	}
	else {
		CAppDoc* pAppDoc = (CAppDoc*) pDoc;
		RefreshPropList(pDoc, nFOV);
		for(int i=0; i<pAppDoc->m_InspectionHandler[nFOV].m_Parameters.GetCount(); i++)
			InitGroup(&pDoc->m_ImageViewInfo, pAppDoc->m_InspectionHandler[nFOV].m_Parameters[i], i);
		//OnExpandAllProperties();
	}
}

void CAppPropWnd::SaveProperties()
{
	CAppDoc* pDoc = (CAppDoc*) m_pDoc;
	for(int i=0; i<pDoc->m_InspectionHandler[m_nCurPos].m_Parameters.GetCount(); i++)
		SaveGroup(pDoc->m_InspectionHandler[m_nCurPos].m_Parameters[i], i);

	
	CApplication* pApp = (CApplication*) AfxGetApp();
	pDoc->m_InspectionHandler[m_nCurPos].SaveInspectParameters(pDoc->m_strConfigDir, 
												 pApp->m_strConfigFile, m_nCurPos);
	
	pDoc->ApplyParameters(m_nCurPos);
	pDoc->m_pTrack->UpdateLotDefectsList();

	//m_wndPropLists[m_nCurSet].ResetOriginalValues();
	m_wndPropLists[m_nCurPos][m_nCurSet].ResetOriginalValues();
	SetModified(FALSE);
}

void CAppPropWnd::SaveAllProperties()
{
	int nOriginalPos = m_nCurPos;
	int nOriginalSet = m_nCurSet;

	CApplication* pApp = (CApplication*) AfxGetApp();
	for(int i=0; i<m_nPos; i++) {
		for(int j=0; j<m_nSets; j++) {
			if(m_wndPropLists[i][j].m_bIsModified) {
				int nTrack, nDoc;
				nTrack = j < m_nTopSets ? 0 : 1;
				nDoc = !nTrack ? j : j - m_nTopSets;
				m_nCurSet = j;
				m_nCurPos = i;
				m_pDoc = pApp->m_pTracks[nTrack].m_pDoc[nDoc];
				SaveProperties();
			}
		}
	}

	m_nCurSet = nOriginalSet;
	m_nCurPos = nOriginalPos;
	int nCurTrack = m_nCurSet < m_nTopSets ? 0 : 1;
	int nOrginalDoc = m_nCurSet < m_nTopSets ? m_nCurSet : m_nCurSet - m_nTopSets;
	m_pDoc = pApp->m_pTracks[nCurTrack].m_pDoc[nOrginalDoc];
}

void CAppPropWnd::SelChangedCamerasCombo()
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	int nTrack, nDoc;
	nTrack = m_nCurSet < m_nTopSets ? 0 : 1;
	nDoc = !nTrack ? m_nCurSet : m_nCurSet - m_nTopSets;
	m_pDoc = pApp->m_pTracks[nTrack].m_pDoc[nDoc];
}

void CAppPropWnd::SelChangedCamerasComboPos()
{
	//CAppDoc* pDoc = (CAppDoc*) m_pDoc;	
	//for(int i=0; i<pDoc->m_InspectionHandler[m_nCurPos].m_ParametersTeach.GetCount(); i++)
	//	InitGroup(&pDoc->m_ImageViewInfo, pDoc->m_InspectionHandler[m_nCurPos].m_ParametersTeach[i], i);
}

void CAppPropWnd::LogParmChange(CParameters* pParm, int nInfoIndex, double dOldValue, double dNewValue)
{
	CApplication* pApp = (CApplication*) AfxGetApp();
	CAppDoc* pDoc = (CAppDoc*) m_pDoc;
	int nFOV = pDoc->m_pTrack->m_nFOVIndex;
	CString strMessage;
	
	if (pDoc->m_pTrack->m_nTrack == STATION_TOP) {
		strMessage.Format("Parm-Change [Position%d,Intensity:%s, %s.%s.%s] %3.1f->%3.1f",
			nFOV + 1, 
			pApp->m_strTopCameraName[pDoc->m_nDocIdx],
			pParm->strInspection,
			pParm->arrParmInfo[nInfoIndex]->strSection,
			pParm->arrParmInfo[nInfoIndex]->strName,
			dOldValue,
			dNewValue);
	}
	else if (pDoc->m_pTrack->m_nTrack == STATION_THR) {
		strMessage.Format("Parm-Change [Position%d,Intensity:%s, %s.%s.%s] %3.1f->%3.1f",
			nFOV + 1,
			pApp->m_strTHRCameraName[pDoc->m_nDocIdx],
			pParm->strInspection,
			pParm->arrParmInfo[nInfoIndex]->strSection,
			pParm->arrParmInfo[nInfoIndex]->strName,
			dOldValue,
			dNewValue);
	}

	pApp->AddLogEvent(strMessage);
}

void CAppPropWnd::SetCurView(CEvImageDoc* pDoc)
{
	CAppDoc* pAppDoc = (CAppDoc*) pDoc;
	//RefreshPropList(pAppDoc);
}