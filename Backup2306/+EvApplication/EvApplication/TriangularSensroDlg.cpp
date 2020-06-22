// TriangularSensroDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TriangularSensroDlg.h"
#include "Application.h"
#include "afxdialogex.h"


static const UINT_PTR TRIANGULAR_SENSOR_TIMER = 1;
// CTriangularSensroDlg dialog

IMPLEMENT_DYNAMIC(CTriangularSensroDlg, CDialogEx)

CTriangularSensroDlg::CTriangularSensroDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TRIANGULAR_SENSRO_DLG, pParent)
//	, m_strReadSensorData(_T(""))
	, m_strIpAddress(_T(""))
	, m_strMasterValueSet(_T(""))
{
	m_pApp = (CApplication*)AfxGetApp();
	m_pBrushTemp[0].CreateSolidBrush(RGB(0, 153, 0));
	m_pBrushTemp[1].CreateSolidBrush(RGB(153, 0, 0));
	m_bConnected = FALSE;
	m_bPause = FALSE;
	m_dZSensorTeachValue = 6.6188;
	m_dCurrentZValue = 0.0;
}

CTriangularSensroDlg::~CTriangularSensroDlg()
{
	m_fontList.DeleteObject();
	m_pBrushTemp[0].DeleteObject();
	m_pBrushTemp[1].DeleteObject();
}

void CTriangularSensroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_SENSOR_DIST, m_dReadSensorData);
//	DDX_Text(pDX, IDC_EDIT_SENSOR_DIST, m_strReadSensorData);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
	DDX_Text(pDX, IDC_EDIT_STR_IPADDRESS, m_strIpAddress);
	DDX_Text(pDX, IDC_EDIT_MASTER_SET_VALUE, m_strMasterValueSet);
	DDX_Control(pDX, IDC_MFCLINK1, m_btnMfcLinkControl);
	//	DDX_Control(pDX, IDC_PLAY_PAUSE_BUTTON, m_butPlayPause);

	DDX_Control(pDX, IDC_STATIC_SENSOR_DIST, m_ctrlSensorDistance);

	DDX_Control(pDX, IDC_COMBO_DIE_ID, m_comboDieIDTeach);
}


BEGIN_MESSAGE_MAP(CTriangularSensroDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTriangularSensroDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CTriangularSensroDlg::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTriangularSensroDlg::OnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, &CTriangularSensroDlg::OnSelchangingTab1)
//	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCLINK1, &CTriangularSensroDlg::OnBnClickedMfclink1)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_SENSOR, &CTriangularSensroDlg::OnBnClickedButtonConnectSensor)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_SENSOR, &CTriangularSensroDlg::OnBnClickedButtonDisconnectSensor)
	ON_BN_CLICKED(IDC_BUTTON_SET_MASTER_VALUE, &CTriangularSensroDlg::OnBnClickedButtonSetMasterValue)
	ON_BN_CLICKED(IDC_BUTTON_RESET_MASTER_VALUE, &CTriangularSensroDlg::OnBnClickedButtonResetMasterValue)
//	ON_BN_CLICKED(IDC_PLAY_PAUSE_BUTTON, &CTriangularSensroDlg::OnBnClickedPasuePlay)

ON_BN_CLICKED(IDC_STOP_PLAY, &CTriangularSensroDlg::OnBnClickedStopPlay)
ON_WM_SHOWWINDOW()
ON_BN_CLICKED(IDC_BUTTON_SET_Z_TEACH_POS, &CTriangularSensroDlg::OnBnClickedButtonSetZTeachPos)
END_MESSAGE_MAP()


// CTriangularSensroDlg message handlers


void CTriangularSensroDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	KillTimer(TRIANGULAR_SENSOR_TIMER);
	m_dCurrentZValue = 0.0;

	CDialogEx::OnOK();
}


BOOL CTriangularSensroDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rctDesktop;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rctDesktop, NULL);
	m_fontList.CreateFont((int)(rctDesktop.Width() / 1280.0 * 13.52), 0, 0, 0, 350, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Segoe UI");

//	GetDlgItem(IDC_EDIT_SENSOR_DIST)->SetFont(&m_fontList);
//	GetDlgItem(IDC_STATIC_MEASURED)->SetFont(&m_fontList);
	GetDlgItem(IDC_STATIC_STATUS)->SetFont(&m_fontList);

//	//m_tabFailImg.Create(IDD_IMAGE_DLG, this);
	//m_TabCtrlSettings.AddSSLPage("Fail Img", 1, &m_tabFailImg);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTriangularSensroDlg::Initailize()
{
	if (m_bConnected) {
		GetDlgItem(IDC_BUTTON_CONNECT_SENSOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextA("Connected");
		GetDlgItem(IDC_BUTTON_DISCONNECT_SENSOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STR_IPADDRESS)->EnableWindow(TRUE);
		SetTimer(TRIANGULAR_SENSOR_TIMER, 100, NULL);
	}
	else {
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextA("Not Connected");
		GetDlgItem(IDC_BUTTON_CONNECT_SENSOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DISCONNECT_SENSOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STR_IPADDRESS)->EnableWindow(TRUE);
	}
	//UpdateData(TRUE);
	m_strIpAddress = m_pApp->m_strSensorIpAddress;
	
	int nDeviceNoPerSubstrateX, nDeviceNoPerSubstrateY, nDieNoPerDeviceX, nDieNoPerDeviceY;
	m_pApp->motionParameters.GetMappingParameters(nDeviceNoPerSubstrateX, nDeviceNoPerSubstrateY, nDieNoPerDeviceX, nDieNoPerDeviceY);

	int nTotalDie = nDeviceNoPerSubstrateX * nDeviceNoPerSubstrateY;
	m_comboDieIDTeach.ResetContent();
	for (int nDie = 0; nDie < nTotalDie; nDie++) {
		CString strDieID;
		strDieID.Format("Die %d", nDie);
		m_comboDieIDTeach.AddString(strDieID);
	}
	m_comboDieIDTeach.AddString("Common Die");
	m_comboDieIDTeach.SetCurSel(0);

	CString text;
	text.Format("%3.06f", 0.00000);
	m_ctrlSensorDistance.SetWindowText(text);

	CString strlink;
	strlink.Format("http://%s", m_strIpAddress);
	m_btnMfcLinkControl.SetURL(_T(strlink));

	m_btnMfcLinkControl.SetTooltip(_T("Opens IDL home for Settings"));
	// resize the button to contain the button text or bitmap
	m_btnMfcLinkControl.SizeToContent();

	OutputDebugLogTo(6, TRUE, "Loaded 1");

	m_tabMeasurementModeDlg.Create(IDD_DIALOG_MEASUREMENT_MODE, this);
	m_tabCtrl.AddSSLPage("Measurement Mode", 0, &m_tabMeasurementModeDlg);

	m_tabMeasurementRateDlg.Create(IDD_DIALOG_MEASUREMENT_RATE, this);
	m_tabCtrl.AddSSLPage("Measurement Rate", 1, &m_tabMeasurementRateDlg);

	m_tabTriggerModeDlg.Create(IDD_DIALOG_TRIGGER_MODE, this);
	m_tabCtrl.AddSSLPage("Trigger Mode", 2, &m_tabTriggerModeDlg);

	if (!m_pApp->m_TrigangualrsensorInstance && !m_bConnected) {
		int nError = m_pApp->m_TriangularSensor.InitSensor(m_pApp->m_TrigangualrsensorInstance);
		if (nError < 0) {
			AfxMessageBox("Init Sensorcard failed");
		}
	}
	else {
//		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextA("No Dll Loaded");
	}
//	OutputDebugLogTo(6, TRUE, "Loaded 2");
	UpdateData(FALSE);
}


void CTriangularSensroDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CString text;
	int nDataAvail = 1;
	double dData = 0.0;
	vector <double> zValues;
	if(nIDEvent == TRIANGULAR_SENSOR_TIMER ) {
		m_pApp->m_TriangularSensor.ProcessInstancePoll(m_pApp->m_TrigangualrsensorInstance, dData);

		m_dCurrentZValue = dData;
		if (dData < 0) {
			text = "Out of range";
		}
		else if (dData > 0) {
			text.Format("%3.06f", dData);
		}
		else {
			text.Format("Failed To Get Z Data");
			int nDataVail = 0;
			m_pApp->m_TriangularSensor.ProcessDataAvail(m_pApp->m_TrigangualrsensorInstance, nDataVail);
			OutputDebugLogTo(1, TRUE, "Data Avail: %d", nDataAvail);
		}
		m_ctrlSensorDistance.SetWindowText(text);

		if(m_bConnected)
			UpdateData(FALSE);
	}
}

void CTriangularSensroDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	KillTimer(0);
	m_dCurrentZValue = 0.0;
	CDialogEx::OnCancel();
}


void CTriangularSensroDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int getTabPageId, getTabPageIndex;
	CString strTitle;

	m_tabCtrl.GetOnSelChange(pNMHDR, pResult);
	getTabPageId = m_tabCtrl.GetSSLActivePage();

	getTabPageIndex = m_tabCtrl.GetSSLPageIndex(getTabPageId);
	m_tabCtrl.GetSSLPageTitle(getTabPageIndex, strTitle);

	m_nActiveTab = getTabPageIndex;
	if (m_bConnected) {
		if (m_nActiveTab == 1) {
			m_tabMeasurementRateDlg.InitializeMeasuringRate();
			m_tabMeasurementRateDlg.GetValues();
		}
		else if (m_nActiveTab == 2) {
			m_tabTriggerModeDlg.InitializeCombo();
			m_tabTriggerModeDlg.GetTriggerMode();
		}
		else {
			m_tabMeasurementModeDlg.InitializeComoboBox();
			m_tabMeasurementModeDlg.GetValues();
		}
	}
	
	*pResult = 0;
	*pResult = 0;
}


void CTriangularSensroDlg::OnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int getTabPageId, getTabPageIndex;
	CString strTitle;

	getTabPageId = m_tabCtrl.GetSSLActivePage();
	getTabPageIndex = m_tabCtrl.GetSSLPageIndex(getTabPageId);
	m_tabCtrl.GetSSLPageTitle(getTabPageIndex, strTitle);

	*pResult = 0;
}

//HBRUSH CTriangularSensroDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	m_pBrush = &m_pBrushTemp[3];
//	if (pWnd->GetDlgCtrlID() == IDC_STATIC_STATUS) {
//		if(m_bConnected)
//			m_pBrush = &m_pBrushTemp[0];
//		else
//			m_pBrush = &m_pBrushTemp[1];
//
//		pDC->SetBkMode(TRANSPARENT);
//		pDC->SetTextColor(RGB(255, 255, 255));
//	}
//	if(pWnd->GetDlgCtrlID() == IDC_PASUE_PLAY) {
//		if(m_bPause)
//			m_pBrush = &m_pBrushTemp[0];
//		else
//			m_pBrush = &m_pBrushTemp[1];
//	}
//	return *m_pBrush;
//}


void CTriangularSensroDlg::OnBnClickedMfclink1()
{
	// TODO: Add your control notification handler code here
	ShellExecute(
		NULL, // or your can use GetSafeHwnd()
		"open",
		"http://stackoverflow.com/a/29181060/4057688",
		NULL,
		NULL,
		SW_SHOWNORMAL
	);
}


void CTriangularSensroDlg::OnBnClickedButtonConnectSensor()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_pApp->m_TrigangualrsensorInstance)
		return;

	GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextA("Not Connected");

	m_bConnected = FALSE;
	if (m_pApp->m_bSensorLibLoaded && !m_pApp->m_TrigangualrsensorInstance) {
		int nError = m_pApp->m_TriangularSensor.InitSensor(m_pApp->m_TrigangualrsensorInstance);
		if (nError < 0) {
			AfxMessageBox("Init Sensorcard failed");
			return;
		}
	}

	int nError = m_pApp->m_TriangularSensor.OpenInstance(m_pApp->m_TrigangualrsensorInstance,m_strIpAddress);
	if (nError < 0) {
		CString strMsg;
		strMsg.Format("Connection to %s Failed", m_strIpAddress);
		AfxMessageBox(strMsg);
	}
	else 
		m_bConnected = TRUE;

	nError = m_pApp->m_TriangularSensor.Set_Averaging(m_pApp->m_TrigangualrsensorInstance, 128);
	if(nError) {
		m_bConnected = FALSE;
	}

	if (m_pApp->m_TriangularSensor.GetAllParameters(m_pApp->m_TrigangualrsensorInstance) < 0) {
		CString strMsg;
		strMsg.Format("GetAllParameters() Failed");
		AfxMessageBox(strMsg);
	}
	else if (m_pApp->m_TriangularSensor.GetTransmittedDataInfo(m_pApp->m_TrigangualrsensorInstance) < 0) {
		CString strMsg;
		strMsg.Format("GetTransmittedDataInfo() Failed");
		AfxMessageBox(strMsg);
	}
	
	if(m_bConnected){
		
		m_bConnected = TRUE;
		GetDlgItem(IDC_EDIT_STR_IPADDRESS)->EnableWindow(FALSE);
		CString strlink;
		strlink.Format("http://%s", m_strIpAddress);
		m_btnMfcLinkControl.SetURL(_T(strlink));
		
	//	SetTimer(TRIANGULAR_SENSOR_TIMER, 50, NULL);
		
		GetDlgItem(IDC_BUTTON_CONNECT_SENSOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextA("Connected");
		GetDlgItem(IDC_BUTTON_DISCONNECT_SENSOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STR_IPADDRESS)->EnableWindow(TRUE);
		SetTimer(TRIANGULAR_SENSOR_TIMER, 100, NULL);
	}
	else 
		m_pApp->m_TriangularSensor.ReleaseSensor(m_pApp->m_TrigangualrsensorInstance);
}


void CTriangularSensroDlg::OnBnClickedButtonDisconnectSensor()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	m_pApp->m_TriangularSensor.ReleaseSensor(m_pApp->m_TrigangualrsensorInstance);
	m_bConnected = FALSE;
	GetDlgItem(IDC_EDIT_STR_IPADDRESS)->EnableWindow(TRUE);
	UpdateData();
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextA("Dis-Connected");
	GetDlgItem(IDC_BUTTON_CONNECT_SENSOR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DISCONNECT_SENSOR)->EnableWindow(FALSE);
	m_dCurrentZValue = 0.0;
	KillTimer(0);
}


void CTriangularSensroDlg::OnBnClickedButtonSetMasterValue()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	double dData = atof(m_strMasterValueSet.GetString());
	OutputDebugLogTo(7, TRUE, "set Master Val -  %f", dData);
	m_pApp->m_TriangularSensor.SetMasterValue(m_pApp->m_TrigangualrsensorInstance, dData);

	double dDataRead;
	m_pApp->m_TriangularSensor.GetMasterValue(m_pApp->m_TrigangualrsensorInstance,dDataRead);
	m_strMasterValueSet.Format("%3.04f", dDataRead);
	OutputDebugLogTo(7, TRUE, "set Master Val -  %s", m_strMasterValueSet);

	UpdateData(FALSE);
}


void CTriangularSensroDlg::OnBnClickedButtonResetMasterValue()
{
	// TODO: Add your control notification handler code here
	double dData = 0.0000;
	m_pApp->m_TriangularSensor.SetMasterValue(m_pApp->m_TrigangualrsensorInstance,dData);

	double dDataRead;
	m_pApp->m_TriangularSensor.GetMasterValue(m_pApp->m_TrigangualrsensorInstance,dDataRead);
	m_strMasterValueSet.Format("%3.04f", dDataRead);
	UpdateData(FALSE);
}

//void CTriangularSensroDlg::OnBnClickedPasuePlay()
//{
//	UpdateData(TRUE);
//	m_bPause != m_bPause;
//	CBitmap bmp;
//
//	CButton* pBtn = (CButton*) GetDlgItem(IDC_PLAY_PAUSE_BUTTON);	
////	pBtn->ModifyStyle(0, BS_BITMAP);	
//	if(m_bPause) {
////		bmp.LoadBitmap(IDB_MOTION_GO_BMP);
//		pBtn->SetBitmap((HBITMAP) LoadImage(AfxGetApp()->m_hInstance,
//			MAKEINTRESOURCE(IDB_MOTION_GO_BMP),
//			IMAGE_BITMAP, 16, 16, LR_COLOR));
//	}
//	else {
//		pBtn->SetBitmap((HBITMAP) LoadImage(AfxGetApp()->m_hInstance,
//			MAKEINTRESOURCE(IDB_MOTION_STOP_BMP),
//			IMAGE_BITMAP, 16, 16, LR_COLOR));
////		bmp.LoadBitmap(IDB_MOTION_STOP_BMP);
//	}
//
//
////	pBtn->SetBitmap(bmp);
//
//	UpdateData(FALSE);
//	// TODO: Add your control notification handler code here
//}



void CTriangularSensroDlg::OnBnClickedStopPlay()
{
	UpdateData(TRUE);
		m_bPause != m_bPause;
	// TODO: Add your control notification handler code here
}


void CTriangularSensroDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(m_bConnected) {
		GetDlgItem(IDC_BUTTON_CONNECT_SENSOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextA("Connected");
		GetDlgItem(IDC_BUTTON_DISCONNECT_SENSOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STR_IPADDRESS)->EnableWindow(TRUE);
		SetTimer(TRIANGULAR_SENSOR_TIMER, 100, NULL);
	}
	else {
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowTextA("Not Connected");
		GetDlgItem(IDC_BUTTON_CONNECT_SENSOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DISCONNECT_SENSOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STR_IPADDRESS)->EnableWindow(TRUE);
	}

//	SetTimer(TRIANGULAR_SENSOR_TIMER, 50, NULL);

	// TODO: Add your message handler code here
}


void CTriangularSensroDlg::OnBnClickedButtonSetZTeachPos()
{

	CString str;
	CString strDieID;
	int nDeviceNoPerSubstrateX, nDeviceNoPerSubstrateY, nDieNoPerDeviceX, nDieNoPerDeviceY;
	m_pApp->motionParameters.GetMappingParameters(nDeviceNoPerSubstrateX, nDeviceNoPerSubstrateY, nDieNoPerDeviceX, nDieNoPerDeviceY);

	int nTotalDie = nDeviceNoPerSubstrateX * nDeviceNoPerSubstrateY;
	int nCurrentSel = m_comboDieIDTeach.GetCurSel();
	if (nCurrentSel == 0) {
		strDieID = "Common Die";
	}
	else {
		strDieID.Format("Die %d", nCurrentSel - 1);
	}

	str.Format("Do you want save %3.06f as Teach Position for %s", m_dCurrentZValue, strDieID);
	if(AfxMessageBox(str, MB_YESNO) == IDYES) {
		if(nCurrentSel == 0)
			theApp.motionParameters.SetZSensorTeachValue(m_dCurrentZValue);
		else
			theApp.motionParameters.SetZSensorTeachValue(nCurrentSel - 1, m_dCurrentZValue);

		theApp.motionParameters.SaveZTeachPosition();
	}
	// TODO: Add your control notification handler code here
}
