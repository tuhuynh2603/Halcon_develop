// SecsGemSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SecsGemSetupDlg.h"
#include "afxdialogex.h"


// SecsGemSetupDlg dialog
#define COLOR_BACKROUND RGB(94,136,158)


IMPLEMENT_DYNAMIC(SecsGemSetupDlg, CDialog)

SecsGemSetupDlg::SecsGemSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SECSGEM_SETUP_DIALOG, pParent)
	, m_nMaxMultiOpenTransactions(0)
	, m_nEquipDeviceID(100)
	, m_strEquipLocalAddress(_T("127.0.0.1"))
	, m_strEquipRemoteAddress(_T("192.168.100.25"))
	, m_nEquipLocalPortID(6000)
	, m_nEquipRemotePortID(5000)
	, m_nEquipConnectMode(0)
	, m_nHSMST3Timeout(0)
	, m_nHSMST5Timeout(0)
	, m_nHSMST6Timeout(0)
	, m_nHSMST7Timeout(0)
	, m_nHSMST8Timeout(0)
{
	m_brush.CreateSolidBrush(COLOR_BACKROUND);
}

SecsGemSetupDlg::~SecsGemSetupDlg()
{
	m_brush.DeleteObject();
}

void SecsGemSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_EQUIP_MULTI_OPEN, m_bEnableMultiOpenTransaction);
	//  DDX_CBIndex(pDX, IDC_COMBO_EQUIP_MAX_OPEN_TRANSACTION, m_nMaxMultiOpenTransactions);
	DDX_Text(pDX, IDC_EDIT_EQUIP_DEVICE_ID, m_nEquipDeviceID);
	DDX_Text(pDX, IDC_EDIT_EQUIP_LOCAL_ADDRESS, m_strEquipLocalAddress);
	//DDX_Text(pDX, IDC_EDIT_EQUIP_REMOTE_ADDRESS, m_strEquipRemoteAddress);
	DDX_Text(pDX, IDC_EDIT_EQUIP_LOCAL_PORTID, m_nEquipLocalPortID);
	DDX_Text(pDX, IDC_EDIT_EQUIP_REMOTE_PORTID, m_nEquipRemotePortID);
	//  DDX_CBIndex(pDX, IDC_COMBO_EQUIP_CONNECT_MODE, m_nEquipConnectMode);
	//DDX_Control(pDX, IDC_COMBO_EQUIP_MAX_OPEN_TRANSACTION, m_cmbMaxOpenTransactions);
	DDX_Control(pDX, IDC_COMBO_EQUIP_CONNECT_MODE, m_cmbConnectionMode);
	DDX_Text(pDX, IDC_EDIT_HSMST3, m_nHSMST3Timeout);
	DDX_Text(pDX, IDC_EDIT_HSMST5, m_nHSMST5Timeout);
	DDX_Text(pDX, IDC_EDIT_HSMST6, m_nHSMST6Timeout);
	DDX_Text(pDX, IDC_EDIT_HSMST7, m_nHSMST7Timeout);
	DDX_Text(pDX, IDC_EDIT_HSMST8, m_nHSMST8Timeout);
}


BEGIN_MESSAGE_MAP(SecsGemSetupDlg, CDialog)
	ON_WM_CTLCOLOR()

	//ON_CBN_SELCHANGE(IDC_COMBO_EQUIP_MAX_OPEN_TRANSACTION, &SecsGemSetupDlg::OnCbnSelchangeComboEquipMaxOpenTransaction)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIP_CONNECT_MODE, &SecsGemSetupDlg::OnSelchangeComboEquipConnectMode)
	ON_BN_CLICKED(IDAPPLY, &SecsGemSetupDlg::OnBnClickedApply)
	ON_BN_CLICKED(IDOK, &SecsGemSetupDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void SecsGemSetupDlg::initializeSetup()
{
	m_bEnableMultiOpenTransaction.SetCheck(theApp.bEnableMultiOpenTransaction);
	m_nMaxMultiOpenTransactions		= theApp.nMaxMultiOpenTransactions;
	m_nEquipDeviceID				= theApp.nEquipDeviceID;
	m_strEquipLocalAddress			= theApp.strEquipLocalAddress;
	m_strEquipRemoteAddress			= theApp.strEquipRemoteAddress;
	m_nEquipLocalPortID				= theApp.nEquipLocalPortID;
	m_nEquipRemotePortID			= theApp.nEquipRemotePortID;
	m_nEquipConnectMode				= theApp.nEquipConnectMode;
	m_nHSMST3Timeout				= theApp.nHSMST3Timeout;
	m_nHSMST5Timeout				= theApp.nHSMST5Timeout;
	m_nHSMST6Timeout				= theApp.nHSMST6Timeout;
	m_nHSMST7Timeout				= theApp.nHSMST7Timeout;
	m_nHSMST8Timeout				= theApp.nHSMST8Timeout;
}

// SecsGemSetupDlg message handlers


BOOL SecsGemSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	initializeSetup();

	//m_cmbMaxOpenTransactions.SetCurSel(m_nMaxMultiOpenTransactions-1);

	if(theApp.strConnectionMode == "Passive")
		m_cmbConnectionMode.SetCurSel(1);
	else
		m_cmbConnectionMode.SetCurSel(0);

	//OnCbnSelchangeComboEquipMaxOpenTransaction();
	OnSelchangeComboEquipConnectMode();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

//void SecsGemSetupDlg::OnCbnSelchangeComboEquipMaxOpenTransaction()
//{
//	theApp.nMaxMultiOpenTransactions = m_cmbMaxOpenTransactions.GetCurSel() + 1;
//}

void SecsGemSetupDlg::OnSelchangeComboEquipConnectMode()
{
	theApp.nEquipConnectMode = m_cmbConnectionMode.GetCurSel();

	if (!theApp.nEquipConnectMode)		// Active = 0
	{
		m_strEquipLocalAddress		= theApp.strEquipRemoteAddress;
		m_nEquipLocalPortID			= theApp.nEquipRemotePortID;
		theApp.strConnectionMode	= "Active";
	}
	else
	{
		m_strEquipLocalAddress		= theApp.strEquipLocalAddress;
		m_nEquipLocalPortID			= theApp.nEquipLocalPortID;
		theApp.strConnectionMode	= "Passive";
	}

	UpdateData(FALSE);
}


void SecsGemSetupDlg::OnBnClickedApply()
{
	UpdateData(TRUE);

	theApp.nEquipDeviceID = m_nEquipDeviceID;

	theApp.nEquipConnectMode = m_cmbConnectionMode.GetCurSel();

	if (!theApp.nEquipConnectMode)		// Active = 0
	{
		theApp.strEquipRemoteAddress = m_strEquipLocalAddress;
		theApp.nEquipRemotePortID = m_nEquipLocalPortID;
	}
	else
	{
		theApp.strEquipLocalAddress = m_strEquipLocalAddress;
		theApp.nEquipLocalPortID = m_nEquipLocalPortID;
	}

	theApp.nEquipConnectMode = m_nEquipConnectMode;
	theApp.bEnableMultiOpenTransaction = m_bEnableMultiOpenTransaction.GetCheck();
	theApp.nHSMST3Timeout = m_nHSMST3Timeout;
	theApp.nHSMST5Timeout = m_nHSMST5Timeout;
	theApp.nHSMST6Timeout = m_nHSMST6Timeout;
	theApp.nHSMST7Timeout = m_nHSMST7Timeout;
	theApp.nHSMST8Timeout = m_nHSMST8Timeout;
}


void SecsGemSetupDlg::OnBnClickedOk()
{
	CDialog::OnOK();

	UpdateData(TRUE);

	theApp.nEquipDeviceID = m_nEquipDeviceID;

	theApp.nEquipConnectMode = m_cmbConnectionMode.GetCurSel();

	if (!theApp.nEquipConnectMode)		// Active = 0
	{
		theApp.strEquipRemoteAddress = m_strEquipLocalAddress;
		theApp.nEquipRemotePortID = m_nEquipLocalPortID;
		theApp.strConnectionMode = "Active";
	}
	else
	{
		theApp.strEquipLocalAddress = m_strEquipLocalAddress;
		theApp.nEquipLocalPortID = m_nEquipLocalPortID;
		theApp.strConnectionMode = "Passive";
	}

	theApp.nEquipConnectMode = m_nEquipConnectMode;
	theApp.bEnableMultiOpenTransaction = m_bEnableMultiOpenTransaction.GetCheck();
	theApp.nHSMST3Timeout = m_nHSMST3Timeout;
	theApp.nHSMST5Timeout = m_nHSMST5Timeout;
	theApp.nHSMST6Timeout = m_nHSMST6Timeout;
	theApp.nHSMST7Timeout = m_nHSMST7Timeout;
	theApp.nHSMST8Timeout = m_nHSMST8Timeout;
	theApp.nEquipRemotePortID = m_nEquipRemotePortID;

	theApp.saveEquipmentMapData(".\\SherlockGemsModel.xml");
}

HBRUSH SecsGemSetupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pWnd == this)
		return m_brush;
	else {
		if (nCtlColor == CTLCOLOR_STATIC) {
			pDC->SetBkColor(COLOR_BACKROUND);
			return m_brush;
		}
		return NULL;
	}
		
}