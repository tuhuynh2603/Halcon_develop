// MotorTriggerTestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "MotorTriggerTestDialog.h"
#include "MotionParameters.h"
#include "DebugLog.h"


// CMotorTriggerTestDialog dialog

IMPLEMENT_DYNAMIC(CMotorTriggerTestDialog, CDialog)

CMotorTriggerTestDialog::CMotorTriggerTestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMotorTriggerTestDialog::IDD, pParent)
	, m_dInterval(10.)
	, m_nFIFOSize(3)
	, m_nTriggerCount(0)
	, m_nTrigCountRemain(0)
	, m_nTrigEnCount(0)
{
	
	bRunTimer = false;
}


CMotorTriggerTestDialog::~CMotorTriggerTestDialog()
{
	if(bRunTimer) {
		bRunTimer = false;
		//KillTimer(TRIGGER_COUNT_TIMER);
	}

}

void CMotorTriggerTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STEP_LENGTH, m_dInterval);
	DDX_Text(pDX, IDC_NO_STEPS, m_nFIFOSize);
	DDX_Text(pDX, IDC_TRIG_COUNT, m_nTriggerCount);
	DDX_Text(pDX, IDC_TRIG_COUNT_REMAIN, m_nTrigCountRemain);
	DDX_Text(pDX, IDC_TRIG_EN_COUNT, m_nTrigEnCount);
	DDX_Control(pDX, IDC_COMBO_FIFO_DIRECTION, m_comboFIFODirection);
	DDX_Control(pDX, IDC_COMBO_SCALE_ENGINE, m_comboScaleEngine);
}


BEGIN_MESSAGE_MAP(CMotorTriggerTestDialog, CDialog)
	ON_BN_CLICKED(IDC_SET_FIFO, &CMotorTriggerTestDialog::OnBnClickedSetFifo)
	ON_BN_CLICKED(IDC_MANUAL_TRIGGER, &CMotorTriggerTestDialog::OnBnClickedManualTrigger)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_RESET_TRIG, &CMotorTriggerTestDialog::OnBnClickedResetTrig)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CMotorTriggerTestDialog::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMotorTriggerTestDialog message handlers


void CMotorTriggerTestDialog::OnBnClickedSetFifo()
{
	UpdateData(TRUE);

	if(m_nFIFOSize <= 0)
		AfxMessageBox("Incorrect FIFO size %d",m_nFIFOSize);

	//0: Backward Direction; 1: Forward Direction
	int nDirection = m_comboFIFODirection.GetCurSel() == 0 ? -1 : 1;
	long * plFIFO = new long [m_nFIFOSize];
	int nFirstPos = 0;

	theApp.motionParameters.GetCurrentMotorPosition(0, nFirstPos);

	double dScaleResolution;
	theApp.motionParameters.GetMotorResolution(0, dScaleResolution);
	
	int nChanelId = m_comboScaleEngine.GetCurSel();
	for(int i=0;i<m_nFIFOSize;i++) {
		plFIFO[i] = nFirstPos + nDirection* Round((i+1)*m_dInterval * dScaleResolution);
		OutputDebugLogTo(1, FALSE,"Scale Channel %d FIFO[%d]: %d", nChanelId, i, plFIFO[i]);
	}

	//theApp.m_MotorResource.SetFifoArray(theApp.m_nMotorCardNo,0,0,plFIFO,m_nFIFOSize);
	theApp.m_ScaleResource.SetScaleFifoArray(theApp.m_nMotorCardNo, nChanelId, plFIFO, m_nFIFOSize);

	delete [] plFIFO;

	if(!bRunTimer) {
		SetTimer(TRIGGER_COUNT_TIMER, 50, NULL);
		bRunTimer = true;
	}
}


void CMotorTriggerTestDialog::OnBnClickedManualTrigger()
{
	//theApp.m_MotorResource.ManualTrigger(theApp.m_nMotorCardNo,0,0);
	UpdateData(TRUE);
	int nChanelId = m_comboScaleEngine.GetCurSel();
	theApp.m_ScaleResource.manualTrigger(theApp.m_nMotorCardNo, nChanelId);
}


LRESULT CMotorTriggerTestDialog::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	return HTCAPTION;
}


void CMotorTriggerTestDialog::OnBnClickedResetTrig()
{
	//theApp.m_MotorResource.ResetTriggerCount(theApp.m_nMotorCardNo, 0, 0); 
	UpdateData(TRUE);
	int nChanelId = m_comboScaleEngine.GetCurSel();
	theApp.m_ScaleResource.ResetScaleFifo(theApp.m_nMotorCardNo, nChanelId);
}


void CMotorTriggerTestDialog::OnTimer(UINT_PTR nIDEvent)
{
	if(bRunTimer) {
		//theApp.m_MotorResource.GetTriggerCount(theApp.m_nMotorCardNo,0,0,0, &m_nTriggerCount,&m_nTrigCountRemain,&m_nTrigEnCount);
		UpdateData(TRUE);
		int nChanelId = m_comboScaleEngine.GetCurSel();

		theApp.motionParameters.GetCurrentScalePosition(nChanelId, m_nTrigEnCount);
		int nError = theApp.m_ScaleResource.GetScaleIntCount(theApp.m_nMotorCardNo, nChanelId, &m_nTriggerCount);
		
		if (nError)
			OutputDebugLogTo(1, TRUE, "GetScaleIntCount ERROR!!!");

		UpdateData(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}


void CMotorTriggerTestDialog::OnBnClickedOk()
{
	if(bRunTimer) {
		bRunTimer = false;
		KillTimer(TRIGGER_COUNT_TIMER);
	}
	theApp.bEnableMotorTriggerTestDialog = FALSE;
	CDialog::OnOK();
}


void CMotorTriggerTestDialog::OnClose()
{
	if(bRunTimer) {
		bRunTimer = false;
		KillTimer(TRIGGER_COUNT_TIMER);
	}
	theApp.bEnableMotorTriggerTestDialog = FALSE;

	CDialog::OnClose();
}


void CMotorTriggerTestDialog::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	theApp.bEnableMotorTriggerTestDialog = FALSE;
	CDialog::OnCancel();
}

BOOL CMotorTriggerTestDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_comboFIFODirection.ResetContent();
	m_comboFIFODirection.AddString("Forward");
	m_comboFIFODirection.AddString("Backward");
	m_comboFIFODirection.SetCurSel(0);

	m_comboScaleEngine.ResetContent();
	m_comboScaleEngine.AddString("0");
	m_comboScaleEngine.AddString("1");
	m_comboScaleEngine.AddString("2");
	m_comboScaleEngine.SetCurSel(0);

	if (!bRunTimer) {
		SetTimer(TRIGGER_COUNT_TIMER, 50, NULL);
		bRunTimer = true;
	}
	return TRUE;
}