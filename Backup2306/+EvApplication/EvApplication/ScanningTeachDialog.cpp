// ScanningTeachDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Application.h"
#include "ScanningTeachDialog.h"
#include "MotionControlDialog.h"
#include "afxdialogex.h"


// ScanningTeachDialog dialog

IMPLEMENT_DYNAMIC(ScanningTeachDialog, CDialog)

ScanningTeachDialog::ScanningTeachDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SCANNING_TEACH_DIALOG, pParent)
	, m_dDistanceZSensorToTop(0)
	, m_dDistanceTopToTHR(0)
	, m_dMotorXScanOffset(0)
	, m_dForwardTriggerOffset(0)
	, m_dInverseTriggerOffset(0)
{
}

ScanningTeachDialog::~ScanningTeachDialog()
{
}

void ScanningTeachDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Z_SENSOR_TO_TOP_DISTANCE, m_dDistanceZSensorToTop);
	DDX_Text(pDX, IDC_EDIT_TOP_TO_THR_DISTANCE, m_dDistanceTopToTHR);
	DDX_Text(pDX, IDC_EDIT_X_TRIGGER_OFFSET, m_dMotorXScanOffset);
	DDX_Text(pDX, IDC_EDIT_X_FORWARD_OFFSET, m_dForwardTriggerOffset);
	DDX_Text(pDX, IDC_EDIT_X_REVERSE_OFFSET, m_dInverseTriggerOffset);
	DDX_Control(pDX, IDC_COMBO_FOV_SETTING, m_comboZStartPosFOVSetting);
	DDX_Control(pDX, IDC_COMBO_FOV_SETTING_XY, m_comboZStartPosFOVSettingXY);
}


BEGIN_MESSAGE_MAP(ScanningTeachDialog, CDialog)
	ON_BN_CLICKED(IDC_SET_START_POS, &ScanningTeachDialog::OnBnClickedSetStartPos)
	ON_BN_CLICKED(IDC_MOVE_TO_START_POS, &ScanningTeachDialog::OnBnClickedMoveToStartPos)
	ON_BN_CLICKED(IDC_SET_ZREADY_POS, &ScanningTeachDialog::OnBnClickedSetZreadyPos)
	ON_BN_CLICKED(IDC_MOVE_TO_ZREADY_POS, &ScanningTeachDialog::OnBnClickedMoveToZreadyPos)
	ON_EN_CHANGE(IDC_EDIT_Z_SENSOR_TO_TOP_DISTANCE, &ScanningTeachDialog::OnEnChangeEditZSensorToTopDistance)
	ON_EN_CHANGE(IDC_EDIT_TOP_TO_THR_DISTANCE, &ScanningTeachDialog::OnEnChangeEditTopToThrDistance)
	ON_EN_CHANGE(IDC_EDIT_X_TRIGGER_OFFSET, &ScanningTeachDialog::OnEnChangeEditXTriggerOffset)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_TO_Z_SENSOR_POS, &ScanningTeachDialog::OnBnClickedButtonMoveToZSensorPos)
	ON_BN_CLICKED(IDC_BUTTON_READY_POS_SET, &ScanningTeachDialog::OnBnClickedButtonReadyPosSet)
	ON_BN_CLICKED(IDC_BUTTON_READY_POS_MOV, &ScanningTeachDialog::OnBnClickedButtonReadyPosMov)
	ON_BN_CLICKED(IDC_BUTTON_NEST_POS_MOV, &ScanningTeachDialog::OnBnClickedButtonNestPosMov)
	ON_BN_CLICKED(IDC_BUTTON_NEST_POS_SET, &ScanningTeachDialog::OnBnClickedButtonNestPosSet)
	ON_EN_CHANGE(IDC_EDIT_X_FORWARD_OFFSET, &ScanningTeachDialog::OnEnChangeEditXForwardOffset)
	ON_EN_CHANGE(IDC_EDIT_X_REVERSE_OFFSET, &ScanningTeachDialog::OnEnChangeEditXReverseOffset)
	
	ON_BN_CLICKED(IDC_SET_TOP_START_POS, &ScanningTeachDialog::OnBnClickedSetTopStartPos)
	ON_BN_CLICKED(IDC_MOVE_TO_TOP_START_POS, &ScanningTeachDialog::OnBnClickedMoveToTopStartPos)
	ON_BN_CLICKED(IDC_SET_TOP_ZREADY_POS, &ScanningTeachDialog::OnBnClickedSetTopZreadyPos)
	ON_BN_CLICKED(IDC_MOVE_TO_TOP_ZREADY_POS, &ScanningTeachDialog::OnBnClickedMoveToTopZreadyPos)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &ScanningTeachDialog::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// ScanningTeachDialog message handlers


void ScanningTeachDialog::OnBnClickedSetStartPos()
{
	if (IDYES == AfxMessageBox("Do you want to set current position as THR START Position (X and Y Motor)?", MB_YESNO)) {

		if(pMotionParmsInitial != NULL) {
			pMotionParmsInitial->SetStartScanPos();
		}

		if(pMotionParms != NULL) {
			pMotionParms->SetStartScanPos();
		}
	}
}


void ScanningTeachDialog::OnBnClickedMoveToStartPos()
{
	if (theApp.m_pMotionControl != NULL) {
		pMotionParmsInitial->MoveToStartScanPos();
	}
}


void ScanningTeachDialog::OnBnClickedSetZreadyPos()
{
	if(IDYES == AfxMessageBox("Do you want to set current position as THR START Position (ST2 Z Motor)?", MB_YESNO)) {

		if(pMotionParmsInitial != NULL) {
			pMotionParmsInitial->SetZStartScanPos();
		}

		if(pMotionParms != NULL) {
			pMotionParms->SetZStartScanPos();
		}
	}
}

void ScanningTeachDialog::SetMotionParameters(MotionParameters * pMotionParmsIn, MotionParameters * pMotionParmsInitialIn)
{
	if(pMotionParmsIn != NULL && pMotionParmsInitialIn != NULL) {
		pMotionParms = pMotionParmsIn;
		pMotionParmsInitial = pMotionParmsInitialIn;

		m_dDistanceZSensorToTop = pMotionParms->GetZSensortoTopDistance();
		m_dDistanceTopToTHR = pMotionParms->GetTopToTHRDistance();
		m_dMotorXScanOffset = pMotionParms->GetMotorScanningOffset();
		m_dForwardTriggerOffset = pMotionParms->GetForwardTriggerOffset();
		m_dInverseTriggerOffset = pMotionParms->GetInverseTriggerOffset();

		m_comboZStartPosFOVSetting.SetCurSel(0);
		m_comboZStartPosFOVSettingXY.SetCurSel(0);

		UpdateData(FALSE);
	}
}

void ScanningTeachDialog::OnBnClickedMoveToZreadyPos()
{
	if(theApp.m_pMotionControl != NULL) {
		pMotionParmsInitial->moveToZStartScanPosPos();
	}
}


void ScanningTeachDialog::OnEnChangeEditZSensorToTopDistance()
{

	if(pMotionParms != NULL) {
		UpdateData(TRUE);
		if(m_dDistanceZSensorToTop < 0.0)
			m_dDistanceZSensorToTop = 0.0;
		UpdateData(FALSE);

		pMotionParms->SetZSensortoTopDistance(m_dDistanceZSensorToTop);
	}
}


void ScanningTeachDialog::OnEnChangeEditTopToThrDistance()
{
	if(pMotionParms != NULL) {
		UpdateData(TRUE);
		if(m_dDistanceTopToTHR < 0.0)
			m_dDistanceTopToTHR = 0.0;
		UpdateData(FALSE);

		pMotionParms->SetTopToTHRDistance(m_dDistanceTopToTHR);
	}
}


void ScanningTeachDialog::OnEnChangeEditXTriggerOffset()
{
	if(pMotionParms != NULL) {
		UpdateData(TRUE);
		if(m_dMotorXScanOffset <= 0.0)
			m_dMotorXScanOffset = 1;
		UpdateData(FALSE);

		pMotionParms->SetMotorScanningOffset(m_dMotorXScanOffset);
	}
}


void ScanningTeachDialog::OnBnClickedButtonMoveToZSensorPos()
{
	if (theApp.m_pMotionControl != NULL)
		pMotionParmsInitial->moveToZSensorTeachPos();
}


void ScanningTeachDialog::OnBnClickedButtonReadyPosSet()
{
	if (IDYES == AfxMessageBox("Do you want to set current position(X, Y, ST1-Z, ST2-Z) as Ready Position?", MB_YESNO)) {

		if (pMotionParmsInitial != NULL) {
			pMotionParmsInitial->SetReadyScanPos();
		}

		if (pMotionParms != NULL) {
			pMotionParms->SetReadyScanPos();
		}
	}
}


void ScanningTeachDialog::OnBnClickedButtonReadyPosMov()
{
	if (theApp.m_pMotionControl != NULL)
		pMotionParmsInitial->moveToReadyScanPos();
}


void ScanningTeachDialog::OnBnClickedButtonNestPosMov()
{
	if (theApp.m_pMotionControl != NULL)
		pMotionParmsInitial->moveToNestReplacementPos();
}


void ScanningTeachDialog::OnBnClickedButtonNestPosSet()
{
	if (IDYES == AfxMessageBox("Do you want to set current position as Nest Replacement Position?", MB_YESNO)) {

		if (pMotionParmsInitial != NULL) {
			pMotionParmsInitial->SetNestReplacementPos();
		}

		if (pMotionParms != NULL) {
			pMotionParms->SetNestReplacementPos();
		}
	}
}


void ScanningTeachDialog::OnEnChangeEditXForwardOffset()
{
	if (pMotionParms != NULL) {
		UpdateData(TRUE);
		/*if (m_dForwardTriggerOffset < 0.0)
			m_dForwardTriggerOffset = 0.0;*/
		//UpdateData(FALSE);

		pMotionParms->SetForwardTriggerOffset(m_dForwardTriggerOffset);
	}
}


void ScanningTeachDialog::OnEnChangeEditXReverseOffset()
{
	if (pMotionParms != NULL) {
		UpdateData(TRUE);
		/*if (m_dForwardTriggerOffset < 0.0)
		m_dForwardTriggerOffset = 0.0;*/
		//UpdateData(FALSE);

		pMotionParms->SetInverseTriggerOffset(m_dInverseTriggerOffset);
	}
}

void ScanningTeachDialog::OnBnClickedSetTopStartPos()
{
	CString strMsg;
	UpdateData(TRUE);
	int nFovSetting = m_comboZStartPosFOVSettingXY.GetCurSel();
	strMsg.Format("Set current position as TOP START Position (X and Y Motor) For FOV %d?", nFovSetting + 1);

	if (IDYES == AfxMessageBox(strMsg, MB_YESNO)) {

		if (pMotionParmsInitial != NULL) {
			pMotionParmsInitial->SetTOPStartScanPos(nFovSetting);
		}

		if (pMotionParms != NULL) {
			pMotionParms->SetTOPStartScanPos(nFovSetting);
		}
	}
}


void ScanningTeachDialog::OnBnClickedMoveToTopStartPos()
{
	if (theApp.m_pMotionControl != NULL) {
		UpdateData(TRUE);
		int nFovSetting = m_comboZStartPosFOVSettingXY.GetCurSel();
		pMotionParmsInitial->MoveToTOPStartScanPos(nFovSetting);
	}
}


void ScanningTeachDialog::OnBnClickedSetTopZreadyPos()
{
	// TODO: Add your control notification handler code here
	CString strMsg;
	UpdateData(TRUE);
	int nFovSetting = m_comboZStartPosFOVSetting.GetCurSel();
	strMsg.Format("Do you want to set current position as TOP START Position (ST1 Z Motor) For FOV %d?", nFovSetting + 1);
	if (IDYES == AfxMessageBox(strMsg, MB_YESNO)) {

		if (pMotionParmsInitial != NULL) {
			pMotionParmsInitial->SetTOPZStartScanPos(nFovSetting);
		}

		if (pMotionParms != NULL) {
			pMotionParms->SetTOPZStartScanPos(nFovSetting);
		}
	}
}


void ScanningTeachDialog::OnBnClickedMoveToTopZreadyPos()
{
	// TODO: Add your control notification handler code here
	if (theApp.m_pMotionControl != NULL) {
		UpdateData(TRUE);
		int nFovSetting = m_comboZStartPosFOVSetting.GetCurSel();
		pMotionParmsInitial->moveToTOPZStartScanPosPos(nFovSetting);
	}
}


void ScanningTeachDialog::OnBnClickedButtonApply()
{
	pMotionParms->SaveMotionParm();

	pMotionParms->SaveScanningParm(false);

	pMotionParmsInitial->LoadMotionParm();
	pMotionParmsInitial->LoadScanningParm();

	pMotionParmsInitial->SetMotionMovementParameters(*pMotionParms);
}
