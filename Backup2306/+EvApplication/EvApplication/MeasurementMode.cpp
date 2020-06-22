// MeasurementMode.cpp : implementation file
//

#include "stdafx.h"
#include "MeasurementMode.h"
#include "Application.h"
#include "afxdialogex.h"


// CMeasurementMode dialog

//IMPLEMENT_DYNAMIC(CMeasurementMode, CDialogEx)

CMeasurementMode::CMeasurementMode(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(IDD_DIALOG_MEASUREMENT_MODE, pParent)
{

}

CMeasurementMode::~CMeasurementMode()
{
}

void CMeasurementMode::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboMeasureArrangements);
	DDX_Control(pDX, IDC_COMBO2, m_comboPeakToBeMeasured);
	DDX_Control(pDX, IDC_COMBO3, m_comboLaserPower);
}


BEGIN_MESSAGE_MAP(CMeasurementMode, CTabPageSSL)
	ON_BN_CLICKED(IDOK, &CMeasurementMode::OnBnClickedOk)
END_MESSAGE_MAP()


// CMeasurementMode message handlers


void CMeasurementMode::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	SetValues();
}

void CMeasurementMode::InitializeComoboBox()
{
	m_comboMeasureArrangements.ResetContent();
	m_comboMeasureArrangements.AddString("Distance diffuse (DIST_DIFFUSE)");
	m_comboMeasureArrangements.AddString("Distance direct (DIST_DIRECT)");
	m_comboMeasureArrangements.AddString("Thickness");

	m_comboPeakToBeMeasured.ResetContent();
	m_comboPeakToBeMeasured.AddString("Greatest Amplitude (DISTA)");
	m_comboPeakToBeMeasured.AddString("Greatest Area (DISTW)");
	m_comboPeakToBeMeasured.AddString("First Peak (DIST1)");

	m_comboLaserPower.ResetContent();
	m_comboLaserPower.AddString("Full");
	m_comboLaserPower.AddString("Reduced");
	m_comboLaserPower.AddString("Off");
}

void CMeasurementMode::GetValues()
{
	CApplication *pApp = (CApplication*)AfxGetApp();
	int nData = 0;
	pApp->m_TriangularSensor.Get_MeasureMode(pApp->m_TrigangualrsensorInstance,nData);
	m_comboMeasureArrangements.SetCurSel(nData);
	pApp->m_TriangularSensor.Get_MeasurePeak(pApp->m_TrigangualrsensorInstance, nData);
	m_comboPeakToBeMeasured.SetCurSel(nData);
	pApp->m_TriangularSensor.Get_LaserPower(pApp->m_TrigangualrsensorInstance, nData);
	m_comboLaserPower.SetCurSel(nData);
}

void CMeasurementMode::SetValues()
{
	CApplication *pApp = (CApplication*)AfxGetApp();
	int nData = 0;
	nData = m_comboMeasureArrangements.GetCurSel();
	pApp->m_TriangularSensor.Set_MeasureMode(pApp->m_TrigangualrsensorInstance, nData);
	
	nData = m_comboPeakToBeMeasured.GetCurSel();
	pApp->m_TriangularSensor.Set_MeasurePeak(pApp->m_TrigangualrsensorInstance, nData);
	
	nData = m_comboLaserPower.GetCurSel();
	pApp->m_TriangularSensor.Set_LaserPower(pApp->m_TrigangualrsensorInstance, nData);
}