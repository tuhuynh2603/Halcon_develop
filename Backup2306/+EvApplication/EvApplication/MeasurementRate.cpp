// MeasurementRate.cpp : implementation file
//

#include "stdafx.h"
#include "MeasurementRate.h"
#include "Application.h"
#include "afxdialogex.h"


// CMeasurementRate dialog

//IMPLEMENT_DYNAMIC(CMeasurementRate, CDialogEx)

CMeasurementRate::CMeasurementRate(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(IDD_DIALOG_MEASUREMENT_RATE, pParent)
{

}

CMeasurementRate::~CMeasurementRate()
{
}

void CMeasurementRate::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MEASURING_RATE, m_comboMeasuringRate);
}


BEGIN_MESSAGE_MAP(CMeasurementRate, CTabPageSSL)
	//ON_CBN_SELCHANGE(IDC_COMBO_MEASURING_RATE, &CMeasurementRate::OnCbnSelchangeComboMeasuringRate)
	ON_BN_CLICKED(IDOK2, &CMeasurementRate::OnBnClickedOk2)
END_MESSAGE_MAP()


// CMeasurementRate message handlers
void CMeasurementRate::InitializeMeasuringRate()
{ 
	m_comboMeasuringRate.ResetContent();
	CString str;
	str.Format("1.5");
	m_comboMeasuringRate.AddString(str);
	str.Format("2.5");
	m_comboMeasuringRate.AddString(str);
	str.Format("5.0");
	m_comboMeasuringRate.AddString(str);
	str.Format("10.0");
	m_comboMeasuringRate.AddString(str);
	str.Format("20.0");
	m_comboMeasuringRate.AddString(str);
	str.Format("30.0");
	m_comboMeasuringRate.AddString(str);
	str.Format("49.0");
	m_comboMeasuringRate.AddString(str);
}

void CMeasurementRate::GetValues()
{
	CApplication *pApp = (CApplication*)AfxGetApp();
	double dData;
	pApp->m_TriangularSensor.Get_SampleRate(pApp->m_TrigangualrsensorInstance, dData);

	CString str;
	str.Format("%2.01f", dData);
	int nSel = m_comboMeasuringRate.FindString(0,str);

	m_comboMeasuringRate.SetCurSel(nSel);
}




void CMeasurementRate::OnBnClickedOk2()
{
	// TODO: Add your control notification handler code here
	int nSel = m_comboMeasuringRate.GetCurSel();
	CString str;
	m_comboMeasuringRate.GetLBText(nSel, str);

	double dData = atof(str);

	CApplication *pApp = (CApplication*)AfxGetApp();
	pApp->m_TriangularSensor.Set_SampleRate(pApp->m_TrigangualrsensorInstance, dData);

}
