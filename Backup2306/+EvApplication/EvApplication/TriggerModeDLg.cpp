// TriggerModeDLg.cpp : implementation file
//

#include "stdafx.h"
#include "TriggerModeDLg.h"
#include "Application.h"

// CTriggerModeDLg dialog

//IMPLEMENT_DYNAMIC(CTriggerModeDLg, CDialogEx)

CTriggerModeDLg::CTriggerModeDLg(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(IDD_DIALOG_TRIGGER_MODE, pParent)
{

}

CTriggerModeDLg::~CTriggerModeDLg()
{
}

void CTriggerModeDLg::DoDataExchange(CDataExchange* pDX)
{
	CTabPageSSL::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboTriggerMode);
}


BEGIN_MESSAGE_MAP(CTriggerModeDLg, CTabPageSSL)
	
	ON_BN_CLICKED(IDC_BUTTON_SUBMIT, &CTriggerModeDLg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTriggerModeDLg message handlers


void CTriggerModeDLg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CApplication *pApp = (CApplication*)AfxGetApp();
	int nSetData = m_comboTriggerMode.GetCurSel();
	pApp->m_TriangularSensor.Set_TriggerMode(pApp->m_TrigangualrsensorInstance,nSetData);
}

void CTriggerModeDLg::InitializeCombo()
{
	// TODO: Add your control notification handler code here
	m_comboTriggerMode.ResetContent();
	m_comboTriggerMode.AddString("None");
	m_comboTriggerMode.AddString("Edge");
	m_comboTriggerMode.AddString("Level");
	m_comboTriggerMode.AddString("Software");

}

void CTriggerModeDLg::GetTriggerMode()
{
	CApplication *pApp = (CApplication*)AfxGetApp();
	int nSetData;
	pApp->m_TriangularSensor.Get_TriggerMode(pApp->m_TrigangualrsensorInstance,nSetData);

	m_comboTriggerMode.SetCurSel(nSetData);
}