// VerificationMode.cpp : implementation file
//

#include "stdafx.h"
#include "VerificationMode.h"
#include "afxdialogex.h"
#include "Application.h"
#include "MotionParameters.h"
#include "ImageSelectionDlg.h"
#include "GlobalBackgroundThread.h"


// CVerificationMode dialog

IMPLEMENT_DYNAMIC(CVerificationMode, CDialogEx)

CVerificationMode::CVerificationMode(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VERIFICATION_DIALOG, pParent)
{
}

CVerificationMode::~CVerificationMode()
{
}

void CVerificationMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEFECT_CATEGORY, m_comboDefectList);
	DDX_Control(pDX, IDC_COMBO_DIE_SELECT, m_comboDieNo);
	DDX_Control(pDX, IDC_COMBO_FOV, m_comboFovNo);
}


BEGIN_MESSAGE_MAP(CVerificationMode, CDialogEx)
	ON_WM_CREATE()
//	ON_WM_CHANGECBCHAIN()
ON_WM_PAINT()
ON_WM_VSCROLL()
ON_WM_HSCROLL()
ON_WM_MOUSEWHEEL()
ON_WM_NCHITTEST()
//ON_BN_CLICKED(IDOK, &CVerificationMode::OnBnClickedOk)
ON_WM_LBUTTONDOWN()
//ON_CBN_SELCHANGE(IDC_COMBO_DEFECT_CATEGORY, &CVerificationMode::OnCbnSelchangeComboDefectCategory)
ON_BN_CLICKED(IDC_BUTTON_APPLY_TO_FOV, &CVerificationMode::OnBnClickedButtonApplyToFov)
ON_BN_CLICKED(IDC_BUTTON_APPLY_TO_DIE, &CVerificationMode::OnBnClickedButtonApplyToDie)
ON_BN_CLICKED(IDC_BUTTON_REVERT, &CVerificationMode::OnBnClickedButtonRevert)
ON_WM_SHOWWINDOW()
ON_BN_CLICKED(IDCANCEL, &CVerificationMode::OnBnClickedCancel)
ON_BN_CLICKED(IDOK, &CVerificationMode::OnBnClickedOk)
END_MESSAGE_MAP()


// CVerificationMode message handlers

BOOL CVerificationMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	Initialize();
	return TRUE;  
}


void CVerificationMode::Initialize()
{
	m_comboDefectList.ResetContent();
	int nBlockX, nBlockY, nFovX, nFovY;
	std::vector < ItemInfo > _defects;
	_defects = theApp.m_pImageSelectionDlg->getDefect();
	m_comboDefectList.ResetContent();
	for (int i = 0; i < _defects.size(); i++) {
		m_comboDefectList.AddString(_defects[i].code.c_str());
	}
	m_comboDefectList.SetCurSel(0);

	theApp.motionParameters.GetMappingParameters(nBlockX, nBlockY, nFovX, nFovY);
	CString strTemp;
	m_comboDieNo.ResetContent();
	for (int i = 0; i < nBlockX* nBlockY; i++) {
		strTemp.Format("%d", i);
		m_comboDieNo.AddString(strTemp);
	}
	m_comboDieNo.SetCurSel(0);

	m_comboFovNo.ResetContent();
	for (int i = 0; i < nFovX *nFovY; i++) {
		strTemp.Format("%d", i);
		m_comboFovNo.AddString(strTemp);
	}
	m_comboFovNo.SetCurSel(0);

	_defectCode = theApp.m_pImageSelectionDlg->getDefectCode();
}

void CVerificationMode::BackUpPreviousMapping() {
	_defectCode = theApp.m_pImageSelectionDlg->getDefectCode();
}

void CVerificationMode::OnBnClickedButtonApplyToFov()
{
	CString strTemp;
	m_comboDieNo.GetLBText(m_comboDieNo.GetCurSel(), strTemp);
	int nDieNo = atoi(strTemp);
	m_comboFovNo.GetLBText(m_comboFovNo.GetCurSel(), strTemp);
	int nDieFov = atoi(strTemp);

	std::string strDefectCode;

	m_comboDefectList.GetLBText(m_comboDefectList.GetCurSel(), strTemp);

	theApp.m_pImageSelectionDlg->ApplyVerificationResultsToFov(nDieNo, nDieFov, strTemp.GetString());

}


void CVerificationMode::OnBnClickedButtonApplyToDie()
{
	CString strTemp;
	m_comboDieNo.GetLBText(m_comboDieNo.GetCurSel(), strTemp);
	int nDieNo = atoi(strTemp);
//	m_comboFovNo.GetLBText(m_comboFovNo.GetCurSel(), strTemp);
//	int nDieFov = atoi(strTemp);

//	std::string strDefectCode;

	m_comboDefectList.GetLBText(m_comboDefectList.GetCurSel(), strTemp);

	theApp.m_pImageSelectionDlg->ApplyVerificationResultsToDie(nDieNo, strTemp.GetString());
	
}


void CVerificationMode::OnBnClickedButtonRevert()
{
	theApp.m_pImageSelectionDlg->setDefectCodes(_defectCode);
	theApp.m_pImageSelectionDlg->Invalidate();

}

void CVerificationMode::UpdateDieFovPositions(int nDie, int nFov)
{
	if (nDie >= 0)
		m_comboDieNo.SetCurSel(nDie);

	if (nFov >= 0)
		m_comboFovNo.SetCurSel(nFov);

	UpdateData(FALSE);
}

void CVerificationMode::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);


	theApp.m_bVerificationDialogOpened = TRUE;

	// TODO: Add your message handler code here
}


void CVerificationMode::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	theApp.m_bVerificationDialogOpened = FALSE;
	CDialogEx::OnCancel();
}


void CVerificationMode::OnBnClickedOk()
{
	theApp.m_bVerificationDialogOpened = FALSE;

	if (theApp.pGlobalBackgroundThread != NULL) {
		if (theApp.pGlobalBackgroundThread->IsRunning())
			theApp.pGlobalBackgroundThread->Run();
	}

	CDialogEx::OnOK();
}
