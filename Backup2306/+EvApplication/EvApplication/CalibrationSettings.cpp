// CalibrationSettings.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include "CalibrationSettings.h"
#include "afxdialogex.h"
#include "LightControlDlg.h"
#include "MotionControlDialog.h"
#include "SaveCalibRationPosition.h"
#include <direct.h>
// CCalibrationSettings dialog

IMPLEMENT_DYNAMIC(CCalibrationSettings, CDialogEx)

CCalibrationSettings::CCalibrationSettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalibrationSettings::IDD, pParent)
{
	
}

CCalibrationSettings::~CCalibrationSettings()
{
}

void CCalibrationSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for(int i=0; i<4; i++)
	{
		//DDX_Control(pDX, IDC_STATIC_DOWN_ARROW_1+i, m_ctrlDieImages[i]);
		//GetDlgItem(IDC_STATIC_IMG_SEL1+i)->ShowWindow(TRUE);
	}
}


BEGIN_MESSAGE_MAP(CCalibrationSettings, CDialogEx)
	ON_BN_CLICKED(IDC_CALIB_CLOSE, &CCalibrationSettings::OnBnClickedCalibClose)
	ON_BN_CLICKED(IDC_CALIB_INTENSITY_1, &CCalibrationSettings::OnBnClickedCalibIntensity1)
//	ON_BN_CLICKED(IDC_CALIB_INTENSITY_2, &CCalibrationSettings::OnBnClickedCalibIntensity2)
//	ON_BN_CLICKED(IDC_CALIB_INTENSITY_3, &CCalibrationSettings::OnBnClickedCalibIntensity3)
//	ON_BN_CLICKED(IDC_CALIB_INTENSITY_4, &CCalibrationSettings::OnBnClickedCalibIntensity4)
//	ON_BN_CLICKED(IDC_CALIB_INTENSITY_5, &CCalibrationSettings::OnBnClickedCalibIntensity5)
	ON_BN_CLICKED(IDC_CALIB_INTENSITY_6, &CCalibrationSettings::OnBnClickedCalibIntensity6)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalibrationSettings::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_DISTANCE_CALIBRATION, &CCalibrationSettings::OnBnClickedButtonDistanceCalibration)
	ON_BN_CLICKED(IDC_RADIO_TOP_STATION, &CCalibrationSettings::OnBnClickedRadioTopStation)
	ON_BN_CLICKED(IDC_RADIO_THR_CAMERA, &CCalibrationSettings::OnBnClickedRadioThrCamera)
	ON_BN_CLICKED(IDC_BUTTON_JIG_LEVELING, &CCalibrationSettings::OnBnClickedButtonJigLeveling)
	ON_BN_CLICKED(IDC_BUTTON_TEACH_Z_SENSOR_POS, &CCalibrationSettings::OnBnClickedButtonTeachZSensorPos)
END_MESSAGE_MAP()


// CCalibrationSettings message handlers


BOOL CCalibrationSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_pApp = (CApplication*) AfxGetApp();

	/*for(int i = 0;i<5;i++)
	{*/
	//	GetDlgItem(IDC_CALIB_INTENSITY_1+i)->ShowWindow(FALSE);
	//	if(i<4)
	//		GetDlgItem(IDC_CALIBDOWN_1+i)->ShowWindow(FALSE);
//	}
	nSelDoc = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CCalibrationSettings::Initialize(int nDoc)
{

	/*for(int i = 0;i<5;i++)
	{
		GetDlgItem(IDC_CALIB_INTENSITY_1+i)->ShowWindow(FALSE);
		if(i<4)
			GetDlgItem(IDC_CALIBDOWN_1+i)->ShowWindow(FALSE);
	}*/
	CString strTemp;
	CString strCameraName;
	int nLightIDx = 1;
	GetDlgItem(IDC_CALIB_INTENSITY_1)->ShowWindow(TRUE);
//	strCameraName = m_pApp->m_strTopCameraName.GetAt(nTrack);
//	strTemp.Format("%s / %s", m_pApp->m_strLightChannelName.GetAt(nLightIDx),strCameraName);
	GetDlgItem(IDC_CALIB_INTENSITY_1)->SetWindowTextA("Light Correction");
	GetDlgItem(IDC_CALIB_INTENSITY_1)->EnableWindow(FALSE);

	
	CRect rect, rect1, rectGroup, rectGroup1;
	GetDlgItem(IDC_STATIC_GROUP_BOX1)->GetWindowRect(&rect);
	
	//GetDlgItem(IDC_STATIC_GROUP_BOX1)->ScreenToClient(&rect);
	strTemp.Format("Dot Calibration");
	GetDlgItem(IDC_CALIB_INTENSITY_6)->ShowWindow(TRUE);
	GetDlgItem(IDC_CALIB_INTENSITY_6)->SetWindowTextA(strTemp);
//	GetDlgItem(IDC_CALIB_INTENSITY_6)->GetWindowRect(&rect);
//	GetDlgItem(IDC_CALIB_INTENSITY_6)->SetWindowPos(NULL, rectGroup.left+30, rectGroup.bottom + 40, rect.Width(), rect.Height(), SWP_NOZORDER);
	nSelDoc = nDoc;

	m_nDotXAxisPosition= 0;
	m_nDotYAxisPosition = 0;
	m_nDotZAxisPositionTop = 0;
	m_nDotZAxisPositionBottom = 0;

	for (int nDoc = 0; nDoc < m_pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
		m_nXAxisPosition[nTrack][nDoc] = 0;
		m_nYAxisPosition[nTrack][nDoc] = 0;
		m_nZAxisPositionTop[nTrack][nDoc] = 0;
		m_nZAxisPositionBottom[nTrack][nDoc] = 0;
	}
	
	m_pApp->m_strCalibDir.Format("%s\\Calibration", m_pApp->m_strMainCfgDir);
	_mkdir(m_pApp->m_strCalibDir);
	CString strPath;
	strPath.Format("%s\\LightCorrection_MotionSettings_Track%d.mot", m_pApp->m_strCalibDir, nTrack + 1);
	LoadMotionParameters(strPath, FALSE);

	m_pApp->m_strCalibDir.Format("%s\\Calibration", m_pApp->m_strMainCfgDir);
	_mkdir(m_pApp->m_strCalibDir);
	strPath.Format("%s\\DotCalibration_MotionSettings_Track%d.mot", m_pApp->m_strCalibDir, nTrack + 1);
	LoadMotionParameters(strPath, TRUE);
}

void CCalibrationSettings::OnBnClickedCalibClose()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


void CCalibrationSettings::OnBnClickedCalibIntensity1()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	BOOL bTemp;
	GetDlgItem(IDC_CALIB_INTENSITY_1)->GetWindowTextA(strTemp);
	strTemp.CompareNoCase("Dot Calibration")!=0?bTemp=FALSE:bTemp=TRUE;
	//nDoc = 0;
	dlgCalibInfo.m_bDotCalib = bTemp;
	if(!bTemp){
		CString str;
		switch (nTrack) {
		case 0: str = m_pApp->m_strTopCalibrationName.GetAt(0);
			break;
		case 1: str = m_pApp->m_strBottomCalibrationName.GetAt(0);
			break;
		case 2: str = m_pApp->m_strSideCalibrationName.GetAt(0);
			break;
		}
		dlgCalibInfo.strCalibrationName = str;
		if(dlgCalibInfo.DoModal()==IDOK)
		{
			nCalibGreyValue = dlgCalibInfo.m_nCalibGreyValue;
			GoToCalibration(nCalibGreyValue,0,FALSE);
		}
		else {
			m_pApp->m_bCalibrationOpen = FALSE;
			return;
		}
			
	}
	else
		GoToCalibration(nCalibGreyValue,0,TRUE);
}


void CCalibrationSettings::OnBnClickedCalibIntensity2()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	BOOL bTemp;
	GetDlgItem(IDC_CALIB_INTENSITY_2)->GetWindowTextA(strTemp);
	strTemp.CompareNoCase("Dot Calibration")!=0?bTemp=FALSE:bTemp=TRUE;
	//nDoc = 1;
	dlgCalibInfo.m_bDotCalib = bTemp;
	if(!bTemp){
		CString str;
		switch (nTrack) {
		case 0: str = m_pApp->m_strTopCalibrationName.GetAt(1);
			break;
		case 1: str = m_pApp->m_strBottomCalibrationName.GetAt(1);
			break;
		case 2: str = m_pApp->m_strSideCalibrationName.GetAt(1);
			break;
		}
		dlgCalibInfo.strCalibrationName = str;
		if(dlgCalibInfo.DoModal()==IDOK)
		{
			nCalibGreyValue = dlgCalibInfo.m_nCalibGreyValue;
			GoToCalibration(nCalibGreyValue,1,FALSE);
		}
		else {
			m_pApp->m_bCalibrationOpen = FALSE;
			return;
		}
	}
	else
		GoToCalibration(nCalibGreyValue,0,TRUE);
}


void CCalibrationSettings::OnBnClickedCalibIntensity3()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	BOOL bTemp;
	GetDlgItem(IDC_CALIB_INTENSITY_3)->GetWindowTextA(strTemp);
	strTemp.CompareNoCase("Dot Calibration")!=0?bTemp=FALSE:bTemp=TRUE;
	//nDoc = 2;
	dlgCalibInfo.m_bDotCalib = bTemp;
	if(!bTemp){
		CString str;
		switch (nTrack) {
		case 0: str = m_pApp->m_strTopCalibrationName.GetAt(2);
			break;
		case 1: str = m_pApp->m_strBottomCalibrationName.GetAt(2);
			break;
		case 2: str = m_pApp->m_strSideCalibrationName.GetAt(2);
			break;
		}
		dlgCalibInfo.strCalibrationName = str;
		if(dlgCalibInfo.DoModal()==IDOK)
		{
			nCalibGreyValue = dlgCalibInfo.m_nCalibGreyValue;
			GoToCalibration(nCalibGreyValue,2,FALSE);
		}
		else {
			m_pApp->m_bCalibrationOpen = FALSE;
			return;
		}
	}
	else
		GoToCalibration(nCalibGreyValue,0,TRUE);
}


void CCalibrationSettings::OnBnClickedCalibIntensity4()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	BOOL bTemp;
	GetDlgItem(IDC_CALIB_INTENSITY_4)->GetWindowTextA(strTemp);
	strTemp.CompareNoCase("Dot Calibration")!=0?bTemp=FALSE:bTemp=TRUE;
	//nDoc = 3;
	dlgCalibInfo.m_bDotCalib = bTemp;
	if(!bTemp){
		CString str;
		switch (nTrack) {
		case 0: str = m_pApp->m_strTopCalibrationName.GetAt(3);
			break;
		case 1: str = m_pApp->m_strBottomCalibrationName.GetAt(3);
			break;
		case 2: str = m_pApp->m_strSideCalibrationName.GetAt(3);
			break;
		}
		dlgCalibInfo.strCalibrationName = str;
		if(dlgCalibInfo.DoModal()==IDOK)
		{
			nCalibGreyValue = dlgCalibInfo.m_nCalibGreyValue;
			GoToCalibration(nCalibGreyValue,3,FALSE);
		}
		else {
			m_pApp->m_bCalibrationOpen = FALSE;
			return;
		}
	}
	else
		GoToCalibration(nCalibGreyValue,0,TRUE);
}


void CCalibrationSettings::OnBnClickedCalibIntensity5()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	BOOL bTemp;
	GetDlgItem(IDC_CALIB_INTENSITY_5)->GetWindowTextA(strTemp);
	strTemp.CompareNoCase("Dot Calibration")!=0?bTemp=FALSE:bTemp=TRUE;
	//nDoc = 4;
	dlgCalibInfo.m_bDotCalib = bTemp;
	if(!bTemp){
		CString str;
		switch (nTrack) {
		case 0: str = m_pApp->m_strTopCalibrationName.GetAt(4);
			break;
		case 1: str = m_pApp->m_strBottomCalibrationName.GetAt(4);
			break;
		case 2: str = m_pApp->m_strSideCalibrationName.GetAt(4);
			break;
		}
		dlgCalibInfo.strCalibrationName = str;
		if(dlgCalibInfo.DoModal()==IDOK)
		{
			nCalibGreyValue = dlgCalibInfo.m_nCalibGreyValue;
			GoToCalibration(nCalibGreyValue,4,FALSE);
		}
		else {
			m_pApp->m_bCalibrationOpen = FALSE;
			return;
		}
	}
	else
		GoToCalibration(nCalibGreyValue,0,TRUE);
}


void CCalibrationSettings::OnBnClickedCalibIntensity6()
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	BOOL bTemp;
	GetDlgItem(IDC_CALIB_INTENSITY_6)->GetWindowTextA(strTemp);
	strTemp.CompareNoCase("Dot Calibration")!=0?bTemp=FALSE:bTemp=TRUE;
	dlgCalibInfo.m_bDotCalib = bTemp;
	if(!bTemp){
		CString str;
		switch (nTrack) {
		case 0: str = m_pApp->m_strTopCalibrationName.GetAt(0);
			break;
		case 1: str = m_pApp->m_strBottomCalibrationName.GetAt(0);
			break;
		case 2: str = m_pApp->m_strSideCalibrationName.GetAt(0);
			break;
		}
		dlgCalibInfo.strCalibrationName = str;
		if(dlgCalibInfo.DoModal()==IDOK)
		{
			nCalibGreyValue = dlgCalibInfo.m_nCalibGreyValue;
			GoToCalibration(nCalibGreyValue,0,FALSE);
		}
		else {
			m_pApp->m_bCalibrationOpen = FALSE;
			return;
		}
	}
	else
		GoToCalibration(nCalibGreyValue,0,TRUE);
}


void CCalibrationSettings::GoToCalibration(int nCalibGreyValue,int nDoc,BOOL bDotCalibration)
{
	if(!bDotCalibration){
		int nDocFrame = 0;// !nTrack ? nDoc : (nTrack == 1 ? m_pApp->m_nTop + nDoc : m_pApp->m_nTop + m_pApp->m_nBottom + nDoc);
		m_pApp->m_pFrames[nDocFrame]->MDIActivate();

		m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bLightCalibration = TRUE;
		if(m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bLightCalibration){
			//if(m_pApp->m_pLightCtrlDlg)
			//	delete m_pApp->m_pLightCtrlDlg;
			////m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_nDocIdx = nDoc;
			//m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_nCalibLightGreyValue = nCalibGreyValue;
			//BOOL bTemp;
			//nTrack == 2?bTemp=TRUE:bTemp=FALSE;
			//m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->bIsFirstLightCalibration = bTemp;
			//m_pApp->m_pLightCtrlDlg = new CLightControlDlg();
			//m_pApp->m_pLightCtrlDlg->m_nTrackId = nTrack;
			//m_pApp->m_pLightCtrlDlg->m_nDocId = nDoc;
			//m_pApp->m_pLightCtrlDlg->nCalibGreyValue = nCalibGreyValue;
			//m_pApp->m_bLightCtrlDlgOpen = TRUE;
			//UpdateStatusBar(&CString(""));

			//if(!m_pApp->m_pLightCtrlDlg->Create(IDD_LIGHT_CONTROL_DLG, AfxGetMainWnd())) {
			//	delete m_pApp->m_pLightCtrlDlg;
			//	m_pApp->m_pLightCtrlDlg = NULL;
			//	return;
			//}
			m_pApp->m_nCalibXAxisPosition = m_nXAxisPosition[nTrack][nDoc];
			m_pApp->m_nCalibYAxisPosition = m_nYAxisPosition[nTrack][nDoc];
			m_pApp->m_nCalibZAxisPositionTop = m_nZAxisPositionTop[nTrack][nDoc];
			m_pApp->m_nCalibZAxisPositionBottom = m_nZAxisPositionBottom[nTrack][nDoc];
			
			OutputDebugLogTo(9, TRUE, "m_pApp->m_nCalibXAxisPosition = %d,m_pApp->m_nCalibYAxisPosition=%d,m_pApp->m_nCalibZAxisPositionTop = %d,m_pApp->m_nCalibZAxisPositionBottom = %d",
				m_pApp->m_nCalibXAxisPosition, m_pApp->m_nCalibYAxisPosition, m_pApp->m_nCalibZAxisPositionTop, m_pApp->m_nCalibZAxisPositionBottom);

			m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_nCalibLightGreyValue = nCalibGreyValue;
			
			dlgCalibInfo.m_nCalibGreyVal[bDotCalibration] = nCalibGreyValue;

			OutputDebugLogTo(7,TRUE,"Light Calibration Initated For Doc%d",nDoc);
			this->ShowWindow(FALSE);
			m_pApp->m_nCommand = CALIBRATE_LIGHT;
			m_pApp->m_nActiveTrackCommandIdx = m_pApp->m_pTracks[nTrack].m_nTrack;
			m_pApp->m_pTracks[nTrack].m_pActiveDoc = m_pApp->m_pTracks[nTrack].m_pDoc[nDoc];
			SetEvent(m_pApp->m_EventRun);
		}
	}
	else {
		int nDocFrame = 0;// !nTrack ? nSelDoc : (nTrack == 1 ? m_pApp->m_nTop + nSelDoc : m_pApp->m_nTop + m_pApp->m_nBottom + nSelDoc);
		m_pApp->m_pFrames[nDocFrame]->MDIActivate();
		m_pApp->m_pTracks[nTrack].m_pDoc[nSelDoc]->m_InspectionHandler[0].LoadDotCalibrationParameters(m_pApp->m_strCalibDir, m_pApp->m_strConfigFile, nTrack, 0);
		dlgCalibInfo.m_nCalibGreyValue = m_pApp->m_pTracks[nTrack].m_pDoc[nSelDoc]->m_InspectionHandler[0].m_Inspection.m_CalibrationData.nInputAlongX;
		dlgCalibInfo.m_nCalibValueY = m_pApp->m_pTracks[nTrack].m_pDoc[nSelDoc]->m_InspectionHandler[0].m_Inspection.m_CalibrationData.nInputAlongY;
		dlgCalibInfo.strCalibrationName = "Dot Calibration";
		if (dlgCalibInfo.DoModal() == IDOK) {
			m_pApp->m_pTracks[nTrack].m_pDoc[nSelDoc]->m_InspectionHandler[0].m_Inspection.m_CalibrationData.nInputAlongX = dlgCalibInfo.m_nCalibGreyValue;
			m_pApp->m_pTracks[nTrack].m_pDoc[nSelDoc]->m_InspectionHandler[0].m_Inspection.m_CalibrationData.nInputAlongY = dlgCalibInfo.m_nCalibValueY;

			dlgCalibInfo.m_nCalibGreyVal[bDotCalibration] = dlgCalibInfo.m_nCalibGreyValue;
		}
		else {
			m_pApp->m_bCalibrationOpen = FALSE;
			return;
		}

		m_pApp->m_nCalibXAxisPosition = m_nDotXAxisPosition;
		m_pApp->m_nCalibYAxisPosition = m_nDotYAxisPosition;
		m_pApp->m_nCalibZAxisPositionTop = m_nDotZAxisPositionTop;
		m_pApp->m_nCalibZAxisPositionBottom = m_nDotZAxisPositionBottom;
		OutputDebugLogTo(9, TRUE, "m_pApp->m_nCalibXAxisPosition = %d,m_pApp->m_nCalibYAxisPosition=%d,m_pApp->m_nCalibZAxisPositionTop = %d,m_pApp->m_nCalibZAxisPositionBottom = %d",
			m_pApp->m_nCalibXAxisPosition, m_pApp->m_nCalibYAxisPosition, m_pApp->m_nCalibZAxisPositionTop, m_pApp->m_nCalibZAxisPositionBottom);
		m_pApp->m_pTracks[nTrack].m_pDoc[nSelDoc]->m_bDotCalibration = TRUE;
		this->ShowWindow(FALSE);
		m_pApp->m_nCommand = CALIBRATE;
		m_pApp->m_nActiveTrackCommandIdx = m_pApp->m_pTracks[nTrack].m_nTrack;
		m_pApp->m_pTracks[nTrack].m_pActiveDoc = m_pApp->m_pTracks[nTrack].m_pDoc[nSelDoc];
		SetEvent(m_pApp->m_EventRun);
	}

}

void CCalibrationSettings::SaveStartPositionCalib(BOOL bSave)		//// Save Start position 
{
	// TODO: Add your control notification handler code here
	if (m_pApp->m_bCalibrationOpen)
		m_pApp->m_pCalibrationDlg->EnableWindow(TRUE);

	if (bSave) {
		CSaveCalibRationPosition dlgSavePosition;
		dlgSavePosition.nTrack = nTrack;
		if (dlgSavePosition.DoModal() == IDOK) {
			if (!dlgSavePosition.m_bSelectedDotCalibration) {
				if (m_pApp->m_pMotionControl) {			

					/*m_nDotXAxisPosition = m_pApp->m_pMotionControl->GetCurrentPos(0);
					m_nDotYAxisPosition = m_pApp->m_pMotionControl->GetCurrentPos(1);
					m_nDotZAxisPositionTop = m_pApp->m_pMotionControl->GetCurrentPos(2);
					m_nDotZAxisPositionBottom = m_pApp->m_pMotionControl->GetCurrentPos(3);*/

					m_pApp->motionParameters.GetCurrentMotorPosition(0, m_nDotXAxisPosition);
					m_pApp->motionParameters.GetCurrentMotorPosition(1, m_nDotYAxisPosition);
					m_pApp->motionParameters.GetCurrentMotorPosition(2, m_nDotZAxisPositionTop);
				
					m_pApp->m_strCalibDir.Format("%s\\Calibration", m_pApp->m_strMainCfgDir);
					_mkdir(m_pApp->m_strCalibDir);
					CString strPath;
					strPath.Format("%s\\DotCalibration_MotionSettings_Track%d.mot", m_pApp->m_strCalibDir, nTrack + 1);
					SaveMotionParameters(strPath, TRUE);
				}
			}
			else {
				for (int i = 0; i < m_pApp->m_nTotalViewDocs[nTrack]; i++) {
					if (dlgSavePosition.m_bCheckedIntensity[i]) {
						if (m_pApp->m_pMotionControl) {
							/*m_nXAxisPosition[nTrack][i] = m_pApp->m_pMotionControl->GetCurrentPos(0);
							m_nYAxisPosition[nTrack][i] = m_pApp->m_pMotionControl->GetCurrentPos(1);
							m_nZAxisPositionTop[nTrack][i] = m_pApp->m_pMotionControl->GetCurrentPos(2);
							m_nZAxisPositionBottom[nTrack][i] = m_pApp->m_pMotionControl->GetCurrentPos(3);*/


							m_pApp->motionParameters.GetCurrentMotorPosition(0, m_nXAxisPosition[nTrack][i]);
							m_pApp->motionParameters.GetCurrentMotorPosition(1, m_nYAxisPosition[nTrack][i]);
							m_pApp->motionParameters.GetCurrentMotorPosition(2, m_nZAxisPositionTop[nTrack][i]);
						}
					}
				}
				m_pApp->m_strCalibDir.Format("%s\\Calibration", m_pApp->m_strMainCfgDir);
				_mkdir(m_pApp->m_strCalibDir);
				CString strPath;
				strPath.Format("%s\\LightCorrection_MotionSettings_Track%d.mot", m_pApp->m_strCalibDir, nTrack + 1);
				SaveMotionParameters(strPath, FALSE);
			}
		}
	}
	CString strPath;
	strPath.Format("%s\\DotCalibration_MotionSettings_Track%d.mot", m_pApp->m_strCalibDir, nTrack + 1);
	LoadMotionParameters(strPath, TRUE);
	strPath.Format("%s\\LightCorrection_MotionSettings_Track%d.mot", m_pApp->m_strCalibDir, nTrack + 1);
	LoadMotionParameters(strPath, FALSE);
}

void CCalibrationSettings::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//if (m_pApp->m_bCalibrationOpen)
	//	m_pApp->m_pCalibrationDlg->EnableWindow(FALSE);

	if (!m_pApp->m_bMotionCtrlDlgOpen) {
		m_pApp->OnMotionControl();
	}
}


void CCalibrationSettings::SaveMotionParameters(CString strPath,BOOL bIsDotCalibration)
{
	// TODO: Add your control notification handler code here
	CIniFile iniFile;
	CFileException e;

	CString strKey, strFile, strTemp;
	iniFile.SetPath(strPath);

	if (bIsDotCalibration){
		strKey.Format("DotCalibration");
		strTemp.Format("XPos");
		iniFile.SetInteger(strKey, strTemp, m_nDotXAxisPosition);
		strTemp.Format("YPos");
		iniFile.SetInteger(strKey, strTemp, m_nDotYAxisPosition);
		strTemp.Format("Top ZPos");
		iniFile.SetInteger(strKey, strTemp, m_nDotZAxisPositionTop);
		strTemp.Format("Bottom ZPos");
		iniFile.SetInteger(strKey, strTemp, m_nDotZAxisPositionBottom);
	}
	else {
		strKey.Format("LightCorrection");
		for (int i = 0; i < m_pApp->m_nTotalViewDocs[nTrack]; i++) {
			strTemp.Format("Doc%d XPos",i+1);
			iniFile.SetInteger(strKey, strTemp, m_nXAxisPosition[nTrack][i]);
			strTemp.Format("Doc%d YPos", i + 1);
			iniFile.SetInteger(strKey, strTemp, m_nYAxisPosition[nTrack][i]);
			strTemp.Format("Doc%d Top ZPos", i + 1);
			iniFile.SetInteger(strKey, strTemp, m_nZAxisPositionTop[nTrack][i]);
			strTemp.Format("Doc%d Bottom ZPos", i + 1);
			iniFile.SetInteger(strKey, strTemp, m_nZAxisPositionBottom[nTrack][i]);
		}
		
	}
	iniFile.WriteFile();
}

void CCalibrationSettings::LoadMotionParameters(CString strPath, BOOL bIsDotCalibration)
{
	// TODO: Add your control notification handler code here
	CIniFile iniFile;
	CFileException e;

	CString strKey, strFile, strTemp;
	iniFile.SetPath(strPath);

	if (iniFile.ReadFile()) {
		if (bIsDotCalibration) {
			strKey.Format("DotCalibration");
			strTemp.Format("XPos");
			m_nDotXAxisPosition = iniFile.GetInteger(strKey, strTemp, m_nDotXAxisPosition);
			strTemp.Format("YPos");
			m_nDotYAxisPosition = iniFile.GetInteger(strKey, strTemp, m_nDotYAxisPosition);
			strTemp.Format("Top ZPos");
			m_nDotZAxisPositionTop = iniFile.GetInteger(strKey, strTemp, m_nDotZAxisPositionTop);
			strTemp.Format("Bottom ZPos");
			m_nDotZAxisPositionBottom = iniFile.GetInteger(strKey, strTemp, m_nDotZAxisPositionBottom);
		}
		else {
			strKey.Format("LightCorrection");
			for (int i = 0; i < m_pApp->m_nTotalViewDocs[nTrack]; i++) {
				strTemp.Format("Doc%d XPos", i + 1);
				m_nXAxisPosition[nTrack][i] = iniFile.GetInteger(strKey, strTemp, m_nXAxisPosition[nTrack][i]);
				strTemp.Format("Doc%d YPos", i + 1);
				m_nYAxisPosition[nTrack][i] = iniFile.GetInteger(strKey, strTemp, m_nYAxisPosition[nTrack][i]);
				strTemp.Format("Doc%d Top ZPos", i + 1);
				m_nZAxisPositionTop[nTrack][i] = iniFile.GetInteger(strKey, strTemp, m_nZAxisPositionTop[nTrack][i]);
				strTemp.Format("Doc%d Bottom ZPos", i + 1);
				m_nZAxisPositionBottom[nTrack][i] = iniFile.GetInteger(strKey, strTemp, m_nZAxisPositionBottom[nTrack][i]);
			}

		}
	}
}

void CCalibrationSettings::OnBnClickedButtonDistanceCalibration()
{
	if (theApp.m_pDistanceCalibration == NULL) {
		
		theApp.m_pDistanceCalibration = new CDistanceCalibration();
		if (!theApp.m_pDistanceCalibration->Create(IDD_CALIB_DISTANCE, AfxGetMainWnd())) {
			delete theApp.m_pDistanceCalibration;
			theApp.m_pDistanceCalibration = NULL;
			return;
		}
	}

	theApp.m_pDistanceCalibration->CenterWindow();
	theApp.m_pDistanceCalibration->ShowWindow(TRUE);

	ShowWindow(SW_HIDE);
}

void CCalibrationSettings::OnBnClickedRadioTopStation()
{
	nTrack = STATION_TOP;
}

void CCalibrationSettings::OnBnClickedRadioThrCamera()
{
	nTrack = STATION_THR;
}

void CCalibrationSettings::OnBnClickedButtonJigLeveling()
{
	if (theApp.m_pJigLeveling == NULL) {
		theApp.m_pJigLeveling = new CJigLeveling();
		if (!theApp.m_pJigLeveling->Create(IDD_DIALOG_JIG_LEVELING, AfxGetMainWnd())) {
			delete theApp.m_pJigLeveling;
			theApp.m_pJigLeveling = NULL;
			return;
		}
	}

	theApp.m_pJigLeveling->CenterWindow();
	theApp.m_pJigLeveling->ShowWindow(TRUE);

	ShowWindow(SW_HIDE);
}


void CCalibrationSettings::OnBnClickedButtonTeachZSensorPos()
{
	// TODO: Add your control notification handler code here
	if (theApp.m_pTeachZSensorPosTool == NULL) {
		theApp.m_pTeachZSensorPosTool = new CTeachZSensorPosition();
		if (!theApp.m_pTeachZSensorPosTool->Create(IDD_DIALOG_TEACH_Z_SENSOR_POS, AfxGetMainWnd())) {
			delete theApp.m_pTeachZSensorPosTool;
			theApp.m_pTeachZSensorPosTool = NULL;
			return;
		}
	}

	theApp.m_pTeachZSensorPosTool->CenterWindow();
	theApp.m_pTeachZSensorPosTool->ShowWindow(TRUE);

	ShowWindow(SW_HIDE);
}
