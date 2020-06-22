// IOInteraction.cpp : implementation file
//

#include "stdafx.h"
#include "IOInteraction.h"
#include "afxdialogex.h"
#include "Application.h"

static const UINT_PTR IO_CHECK_SENSOR_TIMER = 110;

// IOInteraction dialog

IMPLEMENT_DYNAMIC(IOInteraction, CDialogEx)

IOInteraction::IOInteraction(CWnd* pParent /*=NULL*/)
	: CDialogEx(IOInteraction::IDD, pParent)
{
	bitmapRect.SetRectEmpty();
	pIsDialogVisible = NULL;
	isCheckSensorTimerOn = FALSE;
	activeUnit = &units[0];
	motorActionButtonPressedIdentifier = 0;
	motorActionButtonPressingIsActivated = false;
	motorActionButtonIsPressed = false;
	isUpdatingSensorState = false;
	pointInSensor = false;
	istriggeringIO = false;

	loadBitmaps();
	fillUnits();

	backgroundBrush.CreateSolidBrush(RGB(220, 220, 220));
	standardFont.CreateFont(16, 0, 0, 0, 580, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Arial");


	m_brushChecked.CreateSolidBrush(RGB(50, 250, 50));
	m_brushUnchecked.CreateSolidBrush(RGB(220, 50, 50));
	m_brushBorder.CreateSolidBrush(RGB(0, 0, 0));
}

IOInteraction::~IOInteraction()
{
	m_brushChecked.DeleteObject();
	m_brushUnchecked.DeleteObject();
	m_brushBorder.DeleteObject();

	for (int i = 0; i < IO_STATUS_COUNT; i++) {
		m_prgnIOStatusSegment[i].DeleteObject();
	}

	DeleteObject(backgroundBrush);
}

void IOInteraction::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IOINTERACTION_TABCTRL, tabControl);
	DDX_Control(pDX, IDC_IOINTERACTION_PICTURECTRL, pictureControl);
	DDX_Control(pDX, IDC_IOINTERACTION_PICTURECTRL_UPCLOSE, pictureControlUpClose);
}

BEGIN_MESSAGE_MAP(IOInteraction, CDialogEx)
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_NOTIFY(TCN_SELCHANGE, IDC_IOINTERACTION_TABCTRL, &IOInteraction::OnTcnSelchangeIointeractionTabctrl)
	ON_BN_CLICKED(IDC_IOINTERACTION_TOWERLIGHTRED_ON, &IOInteraction::OnBnClickedIointeractionTowerlightredOn)
	ON_BN_CLICKED(IDC_IOINTERACTION_TOWERLIGHTORANGE_ON, &IOInteraction::OnBnClickedIointeractionTowerlightorangeOn)
	ON_BN_CLICKED(IDC_IOINTERACTION_TOWERLIGHTGREEN_ON, &IOInteraction::OnBnClickedIointeractionTowerlightgreenOn)
	ON_BN_CLICKED(IDC_IOINTERACTION_BUZZER_ON, &IOInteraction::OnBnClickedIointeractionBuzzerOn)
	ON_BN_CLICKED(IDC_IOINTERACTION_DOOROPEN, &IOInteraction::OnBnClickedIointeractionDooropen)
	ON_BN_CLICKED(IDC_IOINTERACTION_RESETBUTTONLAMP_ON, &IOInteraction::OnBnClickedIointeractionResetbuttonlampOn)
	ON_BN_CLICKED(IDC_IOINTERACTION_STOPBUTTONLAMP_ON, &IOInteraction::OnBnClickedIointeractionStopbuttonlampOn)
	ON_BN_CLICKED(IDC_IOINTERACTION_STARTBUTTONLAMP_ON, &IOInteraction::OnBnClickedIointeractionStartbuttonlampOn)
	ON_BN_CLICKED(IDC_IOINTERACTION_RESETBUTTONTRIGGER_STATUS, &IOInteraction::OnBnClickedIointeractionResetbuttontriggerStatus)
	ON_BN_CLICKED(IDC_IOINTERACTION_STOPBUTTONTRIGGER_STATUS, &IOInteraction::OnBnClickedIointeractionStopbuttontriggerStatus)
	ON_BN_CLICKED(IDC_IOINTERACTION_STARTPBUTTONTRIGGER_STATUS, &IOInteraction::OnBnClickedIointeractionStartpbuttontriggerStatus)
	ON_BN_CLICKED(IDC_IOINTERACTION_CYCLESTOPBUTTONLAMP_ON, &IOInteraction::OnBnClickedIointeractionCyclestopbuttonlampOn)
	ON_BN_CLICKED(IDC_IOINTERACTION_TOWERLIGHTBLUE_ON, &IOInteraction::OnBnClickedIointeractionTowerlightblueOn)
	ON_BN_CLICKED(IDC_IOINTERACTION_BUTTON_IONIZER_DIS_STOP, &IOInteraction::OnBnClickedIointeractionButtonIonizerDisStop)
	ON_BN_CLICKED(IDC_IOINTERACTION_BUTTON_IONIZER_RESET, &IOInteraction::OnBnClickedIointeractionButtonIonizerReset)
	ON_BN_CLICKED(IDC_IOINTERACTION_BUTTON_LOADER_READY_TO_RECEIVE, &IOInteraction::OnBnClickedIointeractionButtonLoaderReadyToReceive)
	ON_BN_CLICKED(IDC_IOINTERACTION_BUTTON_LOADER_TRANSFER_SUCCESS, &IOInteraction::OnBnClickedIointeractionButtonLoaderTransferSuccess)
	ON_BN_CLICKED(IDC_IOINTERACTION_BUTTON_UNLOADER_PANEL_AVAILABLE, &IOInteraction::OnBnClickedIointeractionButtonUnloaderPanelAvailable)
	ON_BN_CLICKED(IDC_IOINTERACTION_BUTTON_UNLOADER_PANEL_PUSH_OUT, &IOInteraction::OnBnClickedIointeractionButtonUnloaderPanelPushOut)
	ON_BN_CLICKED(IDC_IOINTERACTION_BUTTON_GUARD_DOOR_LOCK, &IOInteraction::OnBnClickedIointeractionButtonGuardDoorLock)
	ON_BN_CLICKED(IDC_IOINTERACTION_BUTTON_IONIZER_VALVE_ON, &IOInteraction::OnBnClickedIointeractionButtonIonizerValveOn)
END_MESSAGE_MAP()

// IOInteraction message handlers

BOOL IOInteraction::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	pictureControl.GetWindowRect(bitmapRect);
	ScreenToClient(bitmapRect);
	bitmapRect.DeflateRect(2, 2);
	bitmapRect.bottom = bitmapRect.top + 519;
	bitmapRect.right = bitmapRect.left + 519;

	posRectToClient();

	//Set title for each tab
	tabTitlesStr.push_back("");
	tabTitlesStr.push_back("Transport");
	tabTitlesStr.push_back("");
	TCITEM tabItem;
	tabItem.mask = TCIF_TEXT;
	for (unsigned int i = 0; i < tabTitlesStr.size(); ++i) {
		tabItem.pszText = ((LPSTR)(LPCTSTR)tabTitlesStr[i]);
		tabControl.InsertItem(i, &tabItem);
	}
	tabControl.SetCurSel(1);

	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA("");
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetFont(&standardFont);
	GetDlgItem(IDC_IOINTERACTION_TABCTRL)->SetFont(&standardFont);

	CRect rect;
	//Tower Light
	m_nResourceID[IO_STATUS_TOWERLIGHT_RED] = IDC_STATIC_IO_TOWERLIGHT_RED;
	m_nResourceID[IO_STATUS_TOWERLIGHT_OGRANGE] = IDC_STATIC_IO_TOWERLIGHT_ORANGE;
	m_nResourceID[IO_STATUS_TOWERLIGHT_GREEN] = IDC_STATIC_IO_TOWERLIGHT_GREEN;
	m_nResourceID[IO_STATUS_TOWERLIGHT_BLUE] = IDC_STATIC_IO_TOWERLIGHT_BLUE;
	m_nResourceID[IO_STATUS_BUZZER] = IDC_STATIC_IO_BUZZER;

	//Push Button LED
	m_nResourceID[IO_STATUS_BUTTONLAMP_RESET] = IDC_STATIC_IO_BUTTONLAMP_RESET;
	m_nResourceID[IO_STATUS_BUTTONLAMP_START] = IDC_STATIC_IO_BUTTONLAMP_START;
	m_nResourceID[IO_STATUS_BUTTONLAMP_CYCLE_STOP] = IDC_STATIC_IO_BUTTONLAMP_CYCLESTOP;
	m_nResourceID[IO_STATUS_BUTTONLAMP_STOP] = IDC_STATIC_IO_BUTTONLAMP_STOP;

	//Ionizer
	m_nResourceID[IO_STATUS_IONIZER_DIS_STOP] = IDC_STATIC_IO_IONIZER_DIS_STOP;
	m_nResourceID[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL] = IDC_STATIC_IO_IONIZER_RESET;
	m_nResourceID[IO_STATUS_IONIZER_ERROR] = IDC_STATIC_IO_IONIZER_ERROR;
	m_nResourceID[IO_STATUS_IONIZER_VALVE_ON] = IDC_STATIC_IO_IONIZER_VALVE_ON;


	//Loader
	m_nResourceID[IO_STATUS_LOADER_READY_TO_RECEIVE] = IDC_STATIC_IO_LOADER_READY_TO_RECEIVE;
	m_nResourceID[IO_STATUS_LOADER_TRANSFER_SUCCESS] = IDC_STATIC_IO_LOADER_TRANSFER_SUCCESS;
	m_nResourceID[IO_STATUS_LOADER_PANEL_AVAILABLE] = IDC_STATIC_IO_LOADER_PANEL_AVAILABLE;
	m_nResourceID[IO_STATUS_LOADER_PANEL_IN] = IDC_STATIC_IO_LOADER_PANEL_IN;

	//Unloader
	m_nResourceID[IO_STATUS_UNLOADER_PANEL_AVAILABLE] = IDC_STATIC_IO_UNLOADER_PANEL_AVAILABLE;
	m_nResourceID[IO_STATUS_UNLOADER_PANEL_PUSH_OUT] = IDC_STATIC_IO_UNLOADER_PANEL_PUSH_OUT;
	m_nResourceID[IO_STATUS_UNLOADER_READY_TO_RECEIVE] = IDC_STATIC_IO_UNLOADER_READY_TO_RECIEVE;
	m_nResourceID[IO_STATUS_UNLOADER_TRANSFER_SUCCESS] = IDC_STATIC_IO_UNLOADER_TRANSFER_SUCCESS;


	//Others
	m_nResourceID[IO_STATUS_DOOR_SWITCH] = IDC_STATIC_IO_DOOR_OPEN;
	m_nResourceID[IO_STATUS_GUARD_HEALTHY] = IDC_STATIC_IO_GUARD_HEALTHY;
	m_nResourceID[IO_STATUS_GUARD_BYPASS] = IDC_STATIC_IO_GUARD_BYPASS;
	m_nResourceID[IO_STATUS_INCOMING_AIR_PRESSURE] = IDC_STATIC_IO_INCOMING_AIR_PRESSURE;
	m_nResourceID[IO_STATUS_EMERGENCY] = IDC_STATIC_IO_EMERGENCY;
	m_nResourceID[IO_STATUS_GUARD_DOOR_LOCK] = IDC_STATIC_IO_GUARD_DOOR_LOCK;


	for (int i = 0; i < IO_STATUS_COUNT; i++) {
		m_bIOChecked[i] = FALSE;
		m_bStart[i] = TRUE;
		GetDlgItem(m_nResourceID[i])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		m_prgnIOStatusSegment[i].CreateEllipticRgn(rect.left + 8, rect.top + 8, rect.right - 8, rect.bottom - 8);
	}

	tabNumber = 1;
	changeTab(tabNumber);

	return TRUE;
}

void IOInteraction::OnOK()
{
	hideDialog();
}

void IOInteraction::OnCancel()
{
	hideDialog();
}

void IOInteraction::OnClose()
{
	hideDialog();
}

void IOInteraction::hideDialog()
{
	ShowWindow(SW_HIDE);

	if (pIsDialogVisible != NULL)
		*pIsDialogVisible = FALSE;
}

void IOInteraction::setDialogVisibility(BOOL * pIsDialogVisible)
{
	this->pIsDialogVisible = pIsDialogVisible;
}

void IOInteraction::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
		startCheckSensorTimer();
	else
		stopCheckSensorTimer();
}

void IOInteraction::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == IO_CHECK_SENSOR_TIMER) {
		if (isCheckSensorTimerOn) {
			updateSensors();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void IOInteraction::startCheckSensorTimer()
{
	if (!isCheckSensorTimerOn) {
		isCheckSensorTimerOn = true;
		SetTimer(IO_CHECK_SENSOR_TIMER, 500, NULL);
	}
}

void IOInteraction::stopCheckSensorTimer()
{
	if (isCheckSensorTimerOn) {
		KillTimer(IO_CHECK_SENSOR_TIMER);
		isCheckSensorTimerOn = false;
	}
}

void IOInteraction::loadBitmaps()
{
	activeSensor.LoadBitmapA(IDB_GREENINDICATOR);
	inactiveSensor.LoadBitmapA(IDB_REDINDICATOR);
	errorSensor.LoadBitmapA(IDB_SENSOR_ERROR);

	onBtn.LoadBitmapA(IDB_ON_SET2);
	offBtn.LoadBitmapA(IDB_OFF_SET2);

	onBtnClicked.LoadBitmapA(IDB_ON_SET2_CLICKED);
	offBtnClicked.LoadBitmapA(IDB_OFF_SET2_CLICKED);

	arrowAngled45.LoadBitmapA(IDB_ARROW_SET2_ANGLED45);
	arrowAngled135.LoadBitmapA(IDB_ARROW_SET2_ANGLED135);
	arrowAngled225.LoadBitmapA(IDB_ARROW_SET2_ANGLED225);
	arrowAngled315.LoadBitmapA(IDB_ARROW_SET2_ANGLED315);
	arrowUp.LoadBitmapA(IDB_ARROW_SET2_UP);
	arrowDown.LoadBitmapA(IDB_ARROW_SET2_DOWN);
	arrowInto.LoadBitmapA(IDB_ARROW_SET2_INTO);
	arrowOutOf.LoadBitmapA(IDB_ARROW_SET2_OUTOF);

	arrowAngled45Clicked.LoadBitmapA(IDB_ARROW_SET2_ANGLED45_CLICKED);
	arrowAngled135Clicked.LoadBitmapA(IDB_ARROW_SET2_ANGLED135_CLICKED);
	arrowAngled225Clicked.LoadBitmapA(IDB_ARROW_SET2_ANGLED225_CLICKED);
	arrowAngled315Clicked.LoadBitmapA(IDB_ARROW_SET2_ANGLED315_CLICKED);
	arrowUpClicked.LoadBitmapA(IDB_ARROW_SET2_UP_CLICKED);
	arrowDownClicked.LoadBitmapA(IDB_ARROW_SET2_DOWN_CLICKED);
	arrowIntoClicked.LoadBitmapA(IDB_ARROW_SET2_INTO_CLICKED);
	arrowOutOfClicked.LoadBitmapA(IDB_ARROW_SET2_OUTOF_CLICKED);
}

BOOL IOInteraction::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void IOInteraction::OnTcnSelchangeIointeractionTabctrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	tabNumber = tabControl.GetCurSel();
	tabNumber = 1;
	changeTab(tabNumber);

	*pResult = 0;
}

void IOInteraction::changeTab(int tabNumber)
{
	activeUnit = &units[tabNumber];
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA("");
	setBackgroundImage("", pictureControlUpClose, 300, 300);
	for (size_t i = 0; i < activeUnit->previousSensorState.size(); ++i) {
		activeUnit->previousSensorState[i] = SENSOR_INIT_STATE;
	}
	InvalidateRect(bitmapRect, FALSE);
}

void IOInteraction::fillUnits()
{
	//Each unit represent a tab
	//Loader
	units[0].pictureNameStr.Format("IOModule_0");

	units[0].motorNameStr.push_back("Lead Frame Pusher In");           units[0].motorsPosRect.push_back(CRect(177, 227, 215, 265));           units[0].motorActionsEnum.push_back(LEADFRAMEPUSHER_IN);
	units[0].motorActionBmp.push_back(&arrowAngled45);                  units[0].motorActionClickedBmp.push_back(&arrowAngled45Clicked);
	units[0].motorNameStr.push_back("Lead Frame Pusher Out");          units[0].motorsPosRect.push_back(CRect(133, 227, 171, 265));           units[0].motorActionsEnum.push_back(LEADFRAMEPUSHER_OUT);
	units[0].motorActionBmp.push_back(&arrowAngled225);                 units[0].motorActionClickedBmp.push_back(&arrowAngled225Clicked);
	units[0].motorNameStr.push_back("Magazine Pusher In");	           units[0].motorsPosRect.push_back(CRect(420, 207, 458, 245));		   units[0].motorActionsEnum.push_back(LOADERMAGPUSHER_IN);
	units[0].motorActionBmp.push_back(&arrowAngled315);                 units[0].motorActionClickedBmp.push_back(&arrowAngled315Clicked);
	units[0].motorNameStr.push_back("Magazine Pusher Out");	           units[0].motorsPosRect.push_back(CRect(464, 207, 502, 245));		   units[0].motorActionsEnum.push_back(LOADERMAGPUSHER_OUT);
	units[0].motorActionBmp.push_back(&arrowAngled135);                 units[0].motorActionClickedBmp.push_back(&arrowAngled135Clicked);
	units[0].motorNameStr.push_back("Magazine Clamp Open");            units[0].motorsPosRect.push_back(CRect(331, 88, 369, 126));		       units[0].motorActionsEnum.push_back(LOADERMAGCLAMP_OPEN);
	units[0].motorActionBmp.push_back(&arrowUp);                        units[0].motorActionClickedBmp.push_back(&arrowUpClicked);
	units[0].motorNameStr.push_back("Magazine Clamp Close");           units[0].motorsPosRect.push_back(CRect(375, 88, 413, 126));		       units[0].motorActionsEnum.push_back(LOADERMAGCLAMP_CLOSE);
	units[0].motorActionBmp.push_back(&arrowDown);                      units[0].motorActionClickedBmp.push_back(&arrowDownClicked);

	units[0].sensorNameStr.push_back("Loader Magazine Presence");	   units[0].sensorsPosRect.push_back(CRect(303, 240, 312, 249));	       units[0].sensorActionsEnum.push_back(LOADERMAGAZINEPRESENCE_STATUS);
	units[0].sensorNameStr.push_back("Loader Magazine Full");		   units[0].sensorsPosRect.push_back(CRect(484, 401, 493, 410));	       units[0].sensorActionsEnum.push_back(LOADERMAGAZINEFULL_STATUS);
	units[0].sensorNameStr.push_back("Loader Lead Frame Presence");	   units[0].sensorsPosRect.push_back(CRect(333, 220, 342, 229));	       units[0].sensorActionsEnum.push_back(LOADERLEADFRAMEPRESENCE_STATUS);
	units[0].sensorNameStr.push_back("Loader Magazine Crash");	       units[0].sensorsPosRect.push_back(CRect(484, 271, 493, 280));	       units[0].sensorActionsEnum.push_back(LOADERMAGAZINECRASH_STATUS);

	units[0].sensorNameStr.push_back("Loader Magazine Clamp Open");	   units[0].sensorsPosRect.push_back(CRect(283, 122, 292, 131));	       units[0].sensorActionsEnum.push_back(LOADERMAGAZIMECLAMPOPEN_STATUS);
	units[0].sensorNameStr.push_back("Loader Magazine Clamp Close");   units[0].sensorsPosRect.push_back(CRect(305, 173, 314, 182));	       units[0].sensorActionsEnum.push_back(LOADERMAGAZINECLAMPCLOSE_STATUS);
	units[0].sensorNameStr.push_back("Loader Pusher In");	           units[0].sensorsPosRect.push_back(CRect(374, 210, 383, 219));	       units[0].sensorActionsEnum.push_back(LOADERPUSHERIN_STATUS);
	units[0].sensorNameStr.push_back("Loader Pusher Out");	           units[0].sensorsPosRect.push_back(CRect(471, 262, 480, 271));	       units[0].sensorActionsEnum.push_back(LOADERPUSHEROUT_STATUS);
	units[0].sensorNameStr.push_back("LeadFrame Pusher In");	       units[0].sensorsPosRect.push_back(CRect(245, 296, 254, 305));	       units[0].sensorActionsEnum.push_back(LEADFRAMEPUSHERIN_STATUS);
	
	/*if (theApp.machineVersion != "0") {
		units[0].sensorNameStr.push_back("LeadFrame Kicker Crash");	       units[0].sensorsPosRect.push_back(CRect(213, 302, 222, 311));	       units[0].sensorActionsEnum.push_back(LEADFRAMEPUSHER_CRASH);
	}*/

	units[0].sensorNameStr.push_back("LeadFrame Pusher Out");	       units[0].sensorsPosRect.push_back(CRect(232, 300, 241, 309));	       units[0].sensorActionsEnum.push_back(LEADFRAMEPUSHEROUT_STATUS);

	/*if (theApp.bEnableLeadFrameMapping) {
		units[0].sensorNameStr.push_back("LeadFrame Mapping Sensor");	       units[0].sensorsPosRect.push_back(CRect(243, 248, 252, 257));	       units[0].sensorActionsEnum.push_back(LOADER_LF_MAPPING_SENSOR);
	}*/

	for (int i = 0; i < units[0].sensorNameStr.size(); ++i)
		units[0].previousSensorState.push_back(SENSOR_OFF);

	//Transport
	units[1].pictureNameStr.Format("IOModule_1");

	units[1].motorNameStr.push_back("Gripper Up");				       units[1].motorsPosRect.push_back(CRect(399, 167, 437, 205));		   units[1].motorActionsEnum.push_back(GRIPPER_UP);
	units[1].motorActionBmp.push_back(&arrowUp);                        units[1].motorActionClickedBmp.push_back(&arrowUpClicked);
	units[1].motorNameStr.push_back("Gripper Down");				   units[1].motorsPosRect.push_back(CRect(441, 167, 479, 205));		   units[1].motorActionsEnum.push_back(GRIPPER_DOWN);
	units[1].motorActionBmp.push_back(&arrowDown);                      units[1].motorActionClickedBmp.push_back(&arrowDownClicked);
	units[1].motorNameStr.push_back("Gripper Clamp Open");		       units[1].motorsPosRect.push_back(CRect(269, 69, 305, 117));		       units[1].motorActionsEnum.push_back(GRIPPERCLAMP_OPEN);
	units[1].motorActionBmp.push_back(&arrowOutOf);                     units[1].motorActionClickedBmp.push_back(&arrowOutOfClicked);
	units[1].motorNameStr.push_back("Gripper Clamp Close");		       units[1].motorsPosRect.push_back(CRect(310, 69, 346, 117));		       units[1].motorActionsEnum.push_back(GRIPPERCLAMP_CLOSE);
	units[1].motorActionBmp.push_back(&arrowInto);                      units[1].motorActionClickedBmp.push_back(&arrowIntoClicked);
	units[1].motorNameStr.push_back("Track Clamps Up");			       units[1].motorsPosRect.push_back(CRect(5, 146, 41, 194));		       units[1].motorActionsEnum.push_back(TRACKCLAMP_UP);
	units[1].motorActionBmp.push_back(&arrowOutOf);                     units[1].motorActionClickedBmp.push_back(&arrowOutOfClicked);
	units[1].motorNameStr.push_back("Track Clamps Down");			   units[1].motorsPosRect.push_back(CRect(46, 146, 82, 194));		       units[1].motorActionsEnum.push_back(TRACKCLAMP_DOWN);
	units[1].motorActionBmp.push_back(&arrowInto);                      units[1].motorActionClickedBmp.push_back(&arrowIntoClicked);
	units[1].motorNameStr.push_back("Vacuum Up");					   units[1].motorsPosRect.push_back(CRect(122, 118, 160, 156));		   units[1].motorActionsEnum.push_back(VACUUM_UP);
	units[1].motorActionBmp.push_back(&arrowUp);                        units[1].motorActionClickedBmp.push_back(&arrowUpClicked);
	units[1].motorNameStr.push_back("Vacuum Down");				       units[1].motorsPosRect.push_back(CRect(165, 118, 203, 156));		   units[1].motorActionsEnum.push_back(VACUUM_DOWN);
	units[1].motorActionBmp.push_back(&arrowDown);                      units[1].motorActionClickedBmp.push_back(&arrowDownClicked);
	units[1].motorNameStr.push_back("Vacuum On");					   units[1].motorsPosRect.push_back(CRect(88, 357, 126, 395));		       units[1].motorActionsEnum.push_back(VACUUM_ON);
	units[1].motorActionBmp.push_back(&onBtn);                          units[1].motorActionClickedBmp.push_back(&onBtnClicked);
	units[1].motorNameStr.push_back("Vacuum Off");				       units[1].motorsPosRect.push_back(CRect(130, 357, 168, 395));		   units[1].motorActionsEnum.push_back(VACUUM_OFF);
	units[1].motorActionBmp.push_back(&offBtn);                         units[1].motorActionClickedBmp.push_back(&offBtnClicked);
	units[1].motorNameStr.push_back("Vacuum Purge On");			       units[1].motorsPosRect.push_back(CRect(191, 357, 229, 395));		   units[1].motorActionsEnum.push_back(VACUUM_PURGE_ON);
	units[1].motorActionBmp.push_back(&onBtn);                          units[1].motorActionClickedBmp.push_back(&onBtnClicked);
	units[1].motorNameStr.push_back("Vacuum Purge Off");			   units[1].motorsPosRect.push_back(CRect(234, 357, 272, 395));		   units[1].motorActionsEnum.push_back(VACUUM_PURGE_OFF);
	units[1].motorActionBmp.push_back(&offBtn);                         units[1].motorActionClickedBmp.push_back(&offBtnClicked);

	units[1].motorNameStr.push_back("Track Clamps 2 Up");			       units[1].motorsPosRect.push_back(CRect(6, 35, 41, 82));		       units[1].motorActionsEnum.push_back(TRACKCLAMP2_UP);
	units[1].motorActionBmp.push_back(&arrowOutOf);                     units[1].motorActionClickedBmp.push_back(&arrowOutOfClicked);
	units[1].motorNameStr.push_back("Track Clamps 2 Down");			   units[1].motorsPosRect.push_back(CRect(47, 35, 83, 82));		       units[1].motorActionsEnum.push_back(TRACKCLAMP2_DOWN);
	units[1].motorActionBmp.push_back(&arrowInto);                      units[1].motorActionClickedBmp.push_back(&arrowIntoClicked);



	units[1].sensorNameStr.push_back("Pull Presence");			       units[1].sensorsPosRect.push_back(CRect(270, 159, 279, 168));		   units[1].sensorActionsEnum.push_back(PULLPRESENTSENSOR_STATUS);
	units[1].sensorNameStr.push_back("Push Jam");					   units[1].sensorsPosRect.push_back(CRect(371, 115, 380, 124));	       units[1].sensorActionsEnum.push_back(PUSHJAMSENSOR_STATUS);
	units[1].sensorNameStr.push_back("Substrate Presence");		       units[1].sensorsPosRect.push_back(CRect(58, 296, 67, 305));	           units[1].sensorActionsEnum.push_back(SUBSTRATEPRESENCE_STATUS);
	units[1].sensorNameStr.push_back("Width Crash");			       units[1].sensorsPosRect.push_back(CRect(130, 180, 139, 189));	           units[1].sensorActionsEnum.push_back(WIDTHCRASH_STATUS);
	units[1].sensorNameStr.push_back("Vacuum Up");			           units[1].sensorsPosRect.push_back(CRect(40, 275, 49, 284));	           units[1].sensorActionsEnum.push_back(VACUUMUP_STATUS);
	units[1].sensorNameStr.push_back("Vacuum Switch");			       units[1].sensorsPosRect.push_back(CRect(127, 230, 136, 239));	       units[1].sensorActionsEnum.push_back(VACUUMSWITCH_STATUS);
	units[1].sensorNameStr.push_back("Gripper Up");			           units[1].sensorsPosRect.push_back(CRect(338, 161, 347, 170));	       units[1].sensorActionsEnum.push_back(GRIPPERUP_STATUS);

	for (int i = 0; i < units[1].sensorNameStr.size(); ++i)
		units[1].previousSensorState.push_back(SENSOR_OFF);

	//Unloader
	units[2].pictureNameStr.Format("IOModule_2");

	units[2].motorNameStr.push_back("Magazine Pusher In");			   units[2].motorsPosRect.push_back(CRect(62, 473, 100, 511));		       units[2].motorActionsEnum.push_back(UNLOADERMAGPUSHER_IN);
	units[2].motorActionBmp.push_back(&arrowAngled45);                  units[2].motorActionClickedBmp.push_back(&arrowAngled45Clicked);
	units[2].motorNameStr.push_back("Magazine Pusher Out");			   units[2].motorsPosRect.push_back(CRect(18, 473, 56, 511));		       units[2].motorActionsEnum.push_back(UNLOADERMAGPUSHER_OUT);
	units[2].motorActionBmp.push_back(&arrowAngled225);                 units[2].motorActionClickedBmp.push_back(&arrowAngled225Clicked);
	units[2].motorNameStr.push_back("Magazine Clamp Open");			   units[2].motorsPosRect.push_back(CRect(124, 90, 162, 128));		       units[2].motorActionsEnum.push_back(UNLOADERMAGCLAMP_OPEN);
	units[2].motorActionBmp.push_back(&arrowUp);                        units[2].motorActionClickedBmp.push_back(&arrowUpClicked);
	units[2].motorNameStr.push_back("Magazine Clamp Close");		   units[2].motorsPosRect.push_back(CRect(167, 90, 205, 167));		       units[2].motorActionsEnum.push_back(UNLOADERMAGCLAMP_CLOSE);
	units[2].motorActionBmp.push_back(&arrowDown);                      units[2].motorActionClickedBmp.push_back(&arrowDownClicked);

	units[2].sensorNameStr.push_back("Unloader Magazine Presence");	   units[2].sensorsPosRect.push_back(CRect(208, 242, 217, 251));	       units[2].sensorActionsEnum.push_back(UNLOADERMAGAZINEPRESENCE_STATUS);
	units[2].sensorNameStr.push_back("Unloader Magazine Full");		   units[2].sensorsPosRect.push_back(CRect(28, 271, 37, 280));	           units[2].sensorActionsEnum.push_back(UNLOADERMAGAZINEFULL_STATUS);
	units[2].sensorNameStr.push_back("Unloader Lead Frame Presence");  units[2].sensorsPosRect.push_back(CRect(164, 236, 173, 245));		   units[2].sensorActionsEnum.push_back(UNLOADERLEADFRAMEPRESENCE_STATUS);
	units[2].sensorNameStr.push_back("Unloader Magazine Crash");	   units[2].sensorsPosRect.push_back(CRect(28, 400, 37, 409));		       units[2].sensorActionsEnum.push_back(UNLOADERMAGAZINECRASH_STATUS);

	units[2].sensorNameStr.push_back("Unloader Magazine Clamp Open");  units[2].sensorsPosRect.push_back(CRect(226, 124, 235, 133));		   units[2].sensorActionsEnum.push_back(UNLOADERMAGAZIMECLAMPOPEN_STATUS);
	units[2].sensorNameStr.push_back("Unloader Magazine Clamp Close"); units[2].sensorsPosRect.push_back(CRect(213, 176, 222, 185));		   units[2].sensorActionsEnum.push_back(UNLOADERMAGAZINECLAMPCLOSE_STATUS);
	units[2].sensorNameStr.push_back("Unloader Pusher In");	           units[2].sensorsPosRect.push_back(CRect(133, 346, 142, 355));		   units[2].sensorActionsEnum.push_back(UNLOADERPUSHERIN_STATUS);
	units[2].sensorNameStr.push_back("Unloader Pusher Out");	       units[2].sensorsPosRect.push_back(CRect(40, 393, 49, 402));		       units[2].sensorActionsEnum.push_back(UNLOADERPUSHEROUT_STATUS);

	/*if (theApp.bEnableLeadFrameMapping) {
		units[2].sensorNameStr.push_back("LeadFrame Mapping Sensor");	       units[2].sensorsPosRect.push_back(CRect(245, 249, 251, 258));	       units[2].sensorActionsEnum.push_back(UNLOADER_LF_MAPPING_SENSOR);
	}*/
	for (int i = 0; i < units[2].sensorNameStr.size(); ++i)
		units[2].previousSensorState.push_back(SENSOR_OFF);
}

void IOInteraction::posRectToClient()
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < units[i].motorsPosRect.size(); ++j) {
			units[i].motorsPosRect[j].OffsetRect(bitmapRect.TopLeft());
			units[i].motorsPosRect[j].bottom = units[i].motorsPosRect[j].bottom + 1;
			units[i].motorsPosRect[j].right = units[i].motorsPosRect[j].right + 1;
		}

		for (int j = 0; j < units[i].sensorsPosRect.size(); ++j) {
			units[i].sensorsPosRect[j].OffsetRect(bitmapRect.TopLeft());
			units[i].sensorsPosRect[j].bottom = units[i].sensorsPosRect[j].bottom + 1;
			units[i].sensorsPosRect[j].right = units[i].sensorsPosRect[j].right + 1;
		}
	}
}

void IOInteraction::OnLButtonDown(UINT nFlags, CPoint point)
{
	pointInSensor = false;
	motorActionButtonPressingIsActivated = false;
	motorActionButtonIsPressed = false;

	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA("");

	for (size_t i = 0; i < activeUnit->motorsPosRect.size(); ++i) {
		if (activeUnit->motorsPosRect[i].PtInRect(point)) {
			motorActionButtonPressingIsActivated = true;
			motorActionButtonIsPressed = true;
			motorActionButtonPressedIdentifier = i;
			SetCapture();

			InvalidateRect(activeUnit->motorsPosRect[motorActionButtonPressedIdentifier], FALSE);
		}
	}

	for (size_t i = 0; i < activeUnit->sensorsPosRect.size(); ++i) {
		if (activeUnit->sensorsPosRect[i].PtInRect(point)) {
			BOOL bOn = FALSE;
			BOOL bError = FALSE;

			pointInSensor = true;
			//setBackgroundImage(activeUnit->sensorNameStr[i], pictureControlUpClose, 300, 300);

			CString msg = triggerIO(activeUnit->sensorActionsEnum[i], bOn, bError);
			GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
		}
	}

	if (!pointInSensor) {
		setBackgroundImage("", pictureControlUpClose, 300, 300);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void IOInteraction::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (motorActionButtonIsPressed) {
		if (activeUnit->motorsPosRect[motorActionButtonPressedIdentifier].PtInRect(point)) {
			BOOL bOn = FALSE;
			BOOL bError = FALSE;
			CString msg = triggerIO(activeUnit->motorActionsEnum[motorActionButtonPressedIdentifier], bOn, bError);
			GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
		}

		motorActionButtonIsPressed = false;
		InvalidateRect(activeUnit->motorsPosRect[motorActionButtonPressedIdentifier], FALSE);

		ReleaseCapture();
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void IOInteraction::OnPaint()
{
	CPaintDC dc(this);
	CDC dcMemory;
	CRect clientRect;
	CBitmap bitmap;
	CBitmap * pbitmap;

	CSize size;

	dcMemory.CreateCompatibleDC(&dc);
	GetClientRect(clientRect);
	bitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height());
	pbitmap = dcMemory.SelectObject(&bitmap);
	::FillRect(dcMemory.GetSafeHdc(), clientRect, (HBRUSH)(COLOR_BTNFACE + 1));

	filePathStr.Format("%s//Images//Sequence//%s.bmp", theApp.m_strMainCfgDir, activeUnit->pictureNameStr);
	initialBitmap = (HBITMAP)LoadImage(NULL, filePathStr, IMAGE_BITMAP, bitmapRect.Height(), bitmapRect.Width(), LR_LOADFROMFILE);
	dcMemory.DrawState(bitmapRect.TopLeft(), bitmapRect.Size(), initialBitmap, DST_BITMAP);
	pictureControl.SetWindowPos(NULL, bitmapRect.left, bitmapRect.top, bitmapRect.Width(), bitmapRect.Height(), SWP_NOZORDER);

	if (motorActionButtonPressingIsActivated) {
		if (motorActionButtonIsPressed) {
			dcMemory.DrawState(activeUnit->motorsPosRect[motorActionButtonPressedIdentifier].TopLeft(),
				activeUnit->motorActionClickedBmp[motorActionButtonPressedIdentifier]->GetBitmapDimension(),
				activeUnit->motorActionClickedBmp[motorActionButtonPressedIdentifier], DST_BITMAP);
		}
		else if (!motorActionButtonIsPressed) {
			dcMemory.DrawState(activeUnit->motorsPosRect[motorActionButtonPressedIdentifier].TopLeft(),
				activeUnit->motorActionBmp[motorActionButtonPressedIdentifier]->GetBitmapDimension(),
				activeUnit->motorActionBmp[motorActionButtonPressedIdentifier], DST_BITMAP);
			motorActionButtonPressingIsActivated = false;
		}
	}

	if (isUpdatingSensorState) {
		for (size_t i = 0; i < activeUnit->sensorActionsEnum.size(); ++i) {
			if (activeUnit->previousSensorState[i] == SENSOR_OFF) {
				dcMemory.DrawState(activeUnit->sensorsPosRect[i].TopLeft(), inactiveSensor.GetBitmapDimension(), inactiveSensor, DST_BITMAP);

				/*HICON hInactiveSensorIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_RED_INDICATOR),
					IMAGE_ICON, 10, 10, 0);
				CBrush* dsadTemp = (CBrush*)0;
				dcMemory.DrawState(activeUnit->sensorsPosRect[i].TopLeft(), activeUnit->sensorsPosRect[i].Size(), hInactiveSensorIcon, DST_ICON, dsadTemp);*/
			}
			else if (activeUnit->previousSensorState[i] == SENSOR_ON)
				dcMemory.DrawState(activeUnit->sensorsPosRect[i].TopLeft(), activeSensor.GetBitmapDimension(), activeSensor, DST_BITMAP);
			else if (activeUnit->previousSensorState[i] == SENSOR_ERROR)
				dcMemory.DrawState(activeUnit->sensorsPosRect[i].TopLeft(), errorSensor.GetBitmapDimension(), errorSensor, DST_BITMAP);
		}
		isUpdatingSensorState = false;
	}

	dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &dcMemory, 0, 0, SRCCOPY);

	dcMemory.SelectObject(pbitmap);
	dcMemory.DeleteDC();
	bitmap.DeleteObject();

	CDC* pDC = &dc;
	CRect rect, rectChannelMap;
	for (int i = 0; i < IO_STATUS_COUNT; i++) {
		pDC->FillRgn(&m_prgnIOStatusSegment[i], m_bIOChecked[i] ? &m_brushChecked : &m_brushUnchecked);
		pDC->FrameRgn(&m_prgnIOStatusSegment[i], &m_brushBorder, 1, 1);
		SetBkMode(dc, TRANSPARENT);
		GetDlgItem(m_nResourceID[i])->GetWindowRect(&rectChannelMap);
		rectChannelMap.top += 2;
		rectChannelMap.left += 2;
		rectChannelMap.right -= 2;
		rectChannelMap.bottom -= 2;
		ScreenToClient(&rectChannelMap);
	}
}

void IOInteraction::updateSensors()
{
	if (istriggeringIO)
		return;

	BOOL bOn;
	BOOL bError;

	//Transport Module
	isUpdatingSensorState = true;

	//IO Control in Picture Control
	for (size_t i = 0; i < activeUnit->sensorActionsEnum.size(); ++i) {
		triggerIO(activeUnit->sensorActionsEnum[i], bOn, bError);

		if (bError) {
			activeUnit->previousSensorState[i] = SENSOR_ERROR;
		}
		else if (!bError) {
			if (bOn) {
				activeUnit->previousSensorState[i] = SENSOR_ON;
			}
			else if (!bOn) {
				activeUnit->previousSensorState[i] = SENSOR_OFF;
			}
		}
		InvalidateRect(activeUnit->sensorsPosRect[i]);
	}

	//IO Control in Dialog
	for (int i = 0; i < IO_STATUS_COUNT; i++) {
		BOOL bIOStatus;
		int nError = GetIOStatus(i, bIOStatus);
		if (bIOStatus != m_bIOChecked[i]) {
			m_bIOChecked[i] = bIOStatus;
			InvalidateRgn(&m_prgnIOStatusSegment[i], FALSE);
		}
	}
}

int IOInteraction::GetIOStatus(int nIOStatusId, BOOL &bOn)
{
	int nError = 0;
	
	switch (nIOStatusId) {
		//Tower Light
	case IO_STATUS_TOWERLIGHT_RED:
		bOn = m_bIOChecked[IO_STATUS_TOWERLIGHT_RED];
		break;

	case IO_STATUS_TOWERLIGHT_OGRANGE:
		bOn = m_bIOChecked[IO_STATUS_TOWERLIGHT_OGRANGE];
		break;

	case IO_STATUS_TOWERLIGHT_GREEN:
		bOn = m_bIOChecked[IO_STATUS_TOWERLIGHT_GREEN];
		break;

	case IO_STATUS_TOWERLIGHT_BLUE:
		bOn = m_bIOChecked[IO_STATUS_TOWERLIGHT_BLUE];
		break;

		//Push Button LED
	case IO_STATUS_BUTTONLAMP_RESET:
		nError = theApp.ioControl.GetResetButtonStatus(bOn);
		break;

	case IO_STATUS_BUTTONLAMP_START:
		nError = theApp.ioControl.GetStartButtonStatus(bOn);

		break;

	case IO_STATUS_BUTTONLAMP_CYCLE_STOP:
		nError = theApp.ioControl.GetCycleStopButtonStatus(bOn);
		break;

	case IO_STATUS_BUTTONLAMP_STOP:
		nError = theApp.ioControl.GetStopButtonStatus(bOn);
		break;

	case IO_STATUS_BUZZER:
		bOn = m_bIOChecked[IO_STATUS_BUZZER];
		break;
		
		//Ionizer
	case IO_STATUS_IONIZER_DIS_STOP:
		bOn = m_bIOChecked[IO_STATUS_IONIZER_DIS_STOP];
		break;

	case IO_STATUS_IONIZER_MAINTENANCE_SIGNAL:
		bOn = m_bIOChecked[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL];
		break;

	case IO_STATUS_IONIZER_ERROR:
		nError = theApp.ioControl.GetIonizerErrorStatus(bOn);
		break;

	case IO_STATUS_IONIZER_VALVE_ON:
		bOn = m_bIOChecked[IO_STATUS_IONIZER_VALVE_ON];
		break;

		//Loader
	case IO_STATUS_LOADER_READY_TO_RECEIVE:
		bOn = m_bIOChecked[IO_STATUS_LOADER_READY_TO_RECEIVE];
		break;

	case IO_STATUS_LOADER_TRANSFER_SUCCESS:
		bOn = m_bIOChecked[IO_STATUS_LOADER_TRANSFER_SUCCESS];
		break;

	case IO_STATUS_LOADER_PANEL_AVAILABLE:
		nError = theApp.ioControl.GetLoaderPanelAvailableStatus(bOn);
		break;

	case IO_STATUS_LOADER_PANEL_IN:
		nError = theApp.ioControl.GetLoaderPanelInStatus(bOn);
		break;

		//Unloader
	case IO_STATUS_UNLOADER_PANEL_AVAILABLE:
		bOn = m_bIOChecked[IO_STATUS_UNLOADER_PANEL_AVAILABLE];
		break;

	case IO_STATUS_UNLOADER_PANEL_PUSH_OUT:
		bOn = m_bIOChecked[IO_STATUS_UNLOADER_PANEL_PUSH_OUT];
		break;

	case IO_STATUS_UNLOADER_READY_TO_RECEIVE:
		nError = theApp.ioControl.GetUnloaderReadyToReceiveStatus(bOn);
		break;

	case IO_STATUS_UNLOADER_TRANSFER_SUCCESS:
		nError = theApp.ioControl.GetUnloaderTransferSuccessStatus(bOn);
		break;


		//Other IO
	case IO_STATUS_DOOR_SWITCH:
		nError = theApp.ioControl.GetDoorSwitchStatus(bOn);
		break;

	case IO_STATUS_GUARD_HEALTHY:
		nError = theApp.ioControl.GetGuardHealthyStatus(bOn);
		break;

	case IO_STATUS_GUARD_BYPASS:
		nError = theApp.ioControl.GetGuardBypassStatus(bOn);
		break;

	case IO_STATUS_INCOMING_AIR_PRESSURE:
		nError = theApp.ioControl.GetAirPressureStatus(bOn);
		break;

	case IO_STATUS_EMERGENCY:
		nError = theApp.ioControl.GetEmergencyStatus(bOn);
		break;

	case IO_STATUS_GUARD_DOOR_LOCK:
		bOn = m_bIOChecked[IO_STATUS_GUARD_DOOR_LOCK];
		break;

	default:
		break;
	}

	return nError;
}

CString IOInteraction::triggerIO(IOSensorActions action, BOOL &bOn, BOOL &bError)
{
	istriggeringIO = true;

	CString msg = "Unknown I/O signal";
	bError = FALSE;
	int position = 0;
	int nError = -1;

	switch (action) {
		//Transport
	case GRIPPER_UP:
		if (theApp.ioControl.GripperUp()) {
			msg.Format("Gripper Up :: Failed");
			bError = TRUE;
		}
		else
			msg.Format("Gripper Up :: Completed");
		break;

	case GRIPPER_DOWN:
		if (int nError = theApp.ioControl.GripperDown()) {
			bError = TRUE;
			if (nError == 1)
				msg.Format("Cannot move Gripper Down. Vacuum Table is Up");			
			else
				msg.Format("Gripper Down :: Failed");
		}
		else
			msg.Format("Gripper Down :: Completed");
		break;

	case GRIPPERCLAMP_OPEN:
		if (theApp.ioControl.GripperClampOpen()) {
			bError = TRUE;
			msg.Format("GripperClamp Open :: Failed");
		}
		else
			msg.Format("GripperClamp Open :: Completed");
		break;

	case GRIPPERCLAMP_CLOSE:
		if (theApp.ioControl.GripperClampClose()) {
			bError = TRUE;
			msg.Format("GripperClamp Close :: Failed");
		}
		else
			msg.Format("GripperClamp Close :: Completed");
		break;

	case TRACKCLAMP_UP:
		nError = theApp.ioControl.TrackClampsUp();

		if (nError) {
			bError = TRUE;
			msg.Format("TrackClamps Up :: Failed");
		}
		else
			msg.Format("TrackClamps Up :: Completed");
		break;

	case TRACKCLAMP_DOWN:
		nError = theApp.ioControl.TrackClampsDown();

		if (nError) {
			bError = TRUE;
			msg.Format("TrackClamps Down :: Failed");
		}
		else
			msg.Format("TrackClamps Down :: Completed");
		break;
	case TRACKCLAMP2_UP:
		nError = theApp.ioControl.TrackClamps2Up();

		if (nError) {
			bError = TRUE;
			msg.Format("TrackClamps 2 Up :: Failed");
		}
		else
			msg.Format("TrackClamps 2 Up :: Completed");
		break;

	case TRACKCLAMP2_DOWN:
		nError = theApp.ioControl.TrackClamps2Down();

		if (nError) {
			bError = TRUE;
			msg.Format("TrackClamps 2 Down :: Failed");
		}
		else
			msg.Format("TrackClamps 2 Down :: Completed");
		break;


	case VACUUM_UP:
		if (int nError = theApp.ioControl.VacuumUp()) {

			if (nError == 1)
				msg.Format("Cannot move Vacuum Up, Gripper is Down");
			else
				msg.Format("Vacuum Up :: Failed");
			bError = TRUE;
		}
		else
			msg.Format("Vacuum Up :: Completed");
		break;

	case VACUUM_DOWN:
		if (theApp.ioControl.VacuumDown()) {
			bError = TRUE;
			msg.Format("Vacuum Down :: Failed");
		}
		else
			msg.Format("Vacuum Down :: Completed");
		break;

	case VACUUM_ON:
		if (theApp.ioControl.VacuumOn()) {
			msg.Format("Vacuum On :: Failed");
			bError = TRUE;
		}
		else
			msg.Format("Vacuum On :: Completed");
		break;

	case VACUUM_OFF:
		if (theApp.ioControl.VacuumOff()) {
			bError = TRUE;
			msg.Format("Vacuum Off :: Failed");
		}
		else
			msg.Format("Vacuum Off :: Completed");
		break;

	case VACUUM_PURGE_ON:
		if (theApp.ioControl.VacuumPurgeOn()) {
			msg.Format("Vacuum Purge On :: Failed");
			bError = TRUE;
		}
		else
			msg.Format("Vacuum Purge On :: Completed");
		break;

	case VACUUM_PURGE_OFF:
		if (theApp.ioControl.VacuumPurgeOff()) {
			bError = TRUE;
			msg.Format("Vacuum Purge Off :: Failed");
		}
		else
			msg.Format("Vacuum Purge Off :: Completed");
		break;

	case PULLPRESENTSENSOR_STATUS:
		if (theApp.ioControl.GetPullPresentSensorState(bOn)) {
			bError = TRUE;
			msg.Format("Failed to retrieve Pull Present Sensor Status");
		}
		else
			msg.Format("Pull Present Sensor Status :: %s", bOn ? "ON" : "OFF");
		break;

	case PUSHJAMSENSOR_STATUS:
		if (theApp.ioControl.GetPushJamSensorState(bOn)) {
			bError = TRUE;
			msg.Format("Failed to retrieve Push Jam Sensor Status");
		}
		else
			msg.Format("Push Jam Sensor Status :: %s", bOn ? "ON" : "OFF");
		break;

	case SUBSTRATEPRESENCE_STATUS:
		if (theApp.ioControl.GetSubstratePresence(bOn)) {
			bError = TRUE;
			msg.Format("Failed to retrieve Substrate Presence Status");
		}
		else
			msg.Format("Substrate Presence Status :: %s", bOn ? "ON" : "OFF");
		break;

	case WIDTHCRASH_STATUS:
		if (theApp.ioControl.GetWidthAdjustmentCrashStatus(bOn)) {
			bError = TRUE;
			msg.Format("Failed to retrieve Width Crash Status");
		}
		else
			msg.Format("Width Crash Status :: %s", bOn ? "ON" : "OFF");
		break;

	case VACUUMUP_STATUS:
		if (theApp.ioControl.GetVacuumUpStatus(bOn)) {
			bError = TRUE;
			msg.Format("Failed to retrieve Vacuum Up Status");
		}
		else
			msg.Format("Vacuum Up Status :: %s", bOn ? "ON" : "OFF");
		break;

	case VACUUMSWITCH_STATUS:
		if (theApp.ioControl.GetVacuumSwitchOnStatus(bOn)) {
			bError = TRUE;
			msg.Format("Failed to retrieve Vacuum Switch Status");
		}
		else
			msg.Format("Vacuum Switch Status :: %s", bOn ? "ON" : "OFF");
		break;

	case GRIPPERUP_STATUS:
		if (theApp.ioControl.GetGripperStatus(position)) {
			bError = TRUE;
			msg.Format("Failed to retrieve Gripper Up Status");
		}
		else {
			msg.Format("Gripper Up Status :: %s", position != 1 ? "OFF" : "ON");
			bOn = position != 1 ? false : true;
		}
		break;
	}

	istriggeringIO = false;

	return msg;
}

void IOInteraction::OnCaptureChanged(CWnd *pWnd)
{
	if (motorActionButtonIsPressed) {
		motorActionButtonIsPressed = false;
		InvalidateRect(activeUnit->motorsPosRect[motorActionButtonPressedIdentifier], FALSE);
	}

	CDialogEx::OnCaptureChanged(pWnd);
}

void IOInteraction::setBackgroundImage(const CString &imageName, CStatic &pictureControl, int height, int width)
{
	CString filePathStr;
	filePathStr.Format("%s//Images//Sequence//%s.bmp", theApp.m_strMainCfgDir, imageName);
	sensorUpCloseBitmap = (HBITMAP)LoadImage(NULL, filePathStr, IMAGE_BITMAP, height, width, LR_LOADFROMFILE);
	pictureControl.SetBitmap(sensorUpCloseBitmap);
}

void IOInteraction::OnBnClickedIointeractionTowerlightredOn()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_TOWERLIGHT_RED] == TRUE) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_RED] = TRUE;
	}
	else if (m_bStart[IO_STATUS_TOWERLIGHT_RED] == FALSE) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_RED] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_TOWERLIGHT_RED]) {
		nError = theApp.ioControl.SetTowerRedLightOn();
	}
	else {
		nError = theApp.ioControl.SetTowerRedLightOff();
	}
	
	if (nError) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_RED] = false;
		msg.Format("Tower Red Light :: Failed");
	}
	else {
		m_bStart[IO_STATUS_TOWERLIGHT_RED] = !m_bStart[IO_STATUS_TOWERLIGHT_RED];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_TOWERLIGHT_RED])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Tower Red Light :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionTowerlightorangeOn()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_TOWERLIGHT_OGRANGE] == TRUE) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_OGRANGE] = TRUE;
	}
	else if (m_bStart[IO_STATUS_TOWERLIGHT_OGRANGE] == FALSE) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_OGRANGE] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_TOWERLIGHT_OGRANGE]) {
		nError = theApp.ioControl.SetTowerOrangeLightOn();
	}
	else {
		nError = theApp.ioControl.SetTowerOrangeLightOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_OGRANGE] = false;
		msg.Format("Tower Orange Light :: Failed");
	}
	else {
		m_bStart[IO_STATUS_TOWERLIGHT_OGRANGE] = !m_bStart[IO_STATUS_TOWERLIGHT_OGRANGE];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_TOWERLIGHT_OGRANGE])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Tower Orange Light :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionTowerlightgreenOn()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_TOWERLIGHT_GREEN] == TRUE) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_GREEN] = TRUE;
	}
	else if (m_bStart[IO_STATUS_TOWERLIGHT_GREEN] == FALSE) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_GREEN] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_TOWERLIGHT_GREEN]) {
		nError = theApp.ioControl.SetTowerGreenLightOn();
	}
	else {
		nError = theApp.ioControl.SetTowerGreenLightOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_GREEN] = false;
		msg.Format("Tower Green Light :: Failed");
	}
	else {
		m_bStart[IO_STATUS_TOWERLIGHT_GREEN] = !m_bStart[IO_STATUS_TOWERLIGHT_GREEN];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_TOWERLIGHT_GREEN])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Tower Green Light :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionBuzzerOn()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_BUZZER] == TRUE) {
		m_bIOChecked[IO_STATUS_BUZZER] = TRUE;
	}
	else if (m_bStart[IO_STATUS_BUZZER] == FALSE) {
		m_bIOChecked[IO_STATUS_BUZZER] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_BUZZER]) {
		nError = theApp.ioControl.SetBuzzerOn();
	}
	else {
		nError = theApp.ioControl.SetBuzzerOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_BUZZER] = false;
		msg.Format("Buzzer On :: Failed");
	}
	else {
		m_bStart[IO_STATUS_BUZZER] = !m_bStart[IO_STATUS_BUZZER];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_BUZZER])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Buzzer On :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionDooropen()
{
	CString msg("Unknown I/O signal");
	BOOL bOn = FALSE;

	if (theApp.ioControl.GetDoorSwitchStatus(bOn))
		msg.Format("Failed to retrieve Door Open Status");
	else
		msg.Format("Door Open Status :: %s", bOn ? "ON" : "OFF");

	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionResetbuttonlampOn()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_BUTTONLAMP_RESET] == TRUE) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_RESET] = TRUE;
	}
	else if (m_bStart[IO_STATUS_BUTTONLAMP_RESET] == FALSE) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_RESET] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_BUTTONLAMP_RESET]) {
		nError = theApp.ioControl.ResetButtonLampOn();
	}
	else {
		nError = theApp.ioControl.ResetButtonLampOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_RESET] = FALSE;
		msg.Format("Reset Lamp :: Failed");
	}
	else {
		m_bStart[IO_STATUS_BUTTONLAMP_RESET] = !m_bStart[IO_STATUS_BUTTONLAMP_RESET];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_BUTTONLAMP_RESET])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Reset Lamp :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionStopbuttonlampOn()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_BUTTONLAMP_STOP] == TRUE) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_STOP] = TRUE;
	}
	else if (m_bStart[IO_STATUS_BUTTONLAMP_STOP] == FALSE) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_STOP] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_BUTTONLAMP_STOP]) {
		nError = theApp.ioControl.StopButtonLampOn();
	}
	else {
		nError = theApp.ioControl.StopButtonLampOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_STOP] = FALSE;
		msg.Format("Stop Lamp :: Failed");
	}
	else {
		m_bStart[IO_STATUS_BUTTONLAMP_STOP] = !m_bStart[IO_STATUS_BUTTONLAMP_STOP];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_BUTTONLAMP_STOP])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Stop Lamp :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionStartbuttonlampOn()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_BUTTONLAMP_START] == TRUE) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_START] = TRUE;
	}
	else if (m_bStart[IO_STATUS_BUTTONLAMP_START] == FALSE) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_START] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_BUTTONLAMP_START]) {
		nError = theApp.ioControl.StartButtonLampOn();
	}
	else {
		nError = theApp.ioControl.StartButtonLampOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_START] = false;
		msg.Format("Start Lamp :: Failed");
	}
	else {
		m_bStart[IO_STATUS_BUTTONLAMP_START] = !m_bStart[IO_STATUS_BUTTONLAMP_START];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_BUTTONLAMP_START])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Start Lamp :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionResetbuttontriggerStatus()
{
	CString msg("Unknown I/O signal");
	BOOL bOn = FALSE;

	if (theApp.ioControl.GetResetButtonStatus(bOn))
		msg.Format("Failed to retrieve Reset Button Lamp Status");
	else
		msg.Format("Reset Button Lamp Status :: %s", bOn ? "ON" : "OFF");

	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionStopbuttontriggerStatus()
{
	CString msg("Unknown I/O signal");
	BOOL bOn = FALSE;

	if (theApp.ioControl.GetStopButtonStatus(bOn))
		msg.Format("Failed to retrieve Stop Button Lamp Status");
	else
		msg.Format("Stop Button Lamp Status :: %s", bOn ? "ON" : "OFF");

	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionStartpbuttontriggerStatus()
{
	CString msg("Unknown I/O signal");
	BOOL bOn = FALSE;

	if (theApp.ioControl.GetStartButtonStatus(bOn))
		msg.Format("Failed to retrieve Start Button Lamp Status");
	else
		msg.Format("Start Button Lamp Status :: %s", bOn ? "ON" : "OFF");

	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionCyclestopbuttonlampOn()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_BUTTONLAMP_CYCLE_STOP] == TRUE) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_CYCLE_STOP] = TRUE;
	}
	else if (m_bStart[IO_STATUS_BUTTONLAMP_CYCLE_STOP] == FALSE) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_CYCLE_STOP] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_BUTTONLAMP_CYCLE_STOP]) {
		nError = theApp.ioControl.CycleStopButtonLampOn();
	}
	else {
		nError = theApp.ioControl.CycleStopButtonLampOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_BUTTONLAMP_CYCLE_STOP] = FALSE;
		msg.Format("Cycle Stop Lamp :: Failed");
	}
	else {
		m_bStart[IO_STATUS_BUTTONLAMP_CYCLE_STOP] = !m_bStart[IO_STATUS_BUTTONLAMP_CYCLE_STOP];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_BUTTONLAMP_CYCLE_STOP])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Cycle Stop Lamp :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}


void IOInteraction::OnBnClickedIointeractionTowerlightblueOn()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_TOWERLIGHT_BLUE] == TRUE) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_BLUE] = TRUE;
	}
	else if (m_bStart[IO_STATUS_TOWERLIGHT_BLUE] == FALSE) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_BLUE] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_TOWERLIGHT_BLUE]) {
		nError = theApp.ioControl.SetTowerBlueLightOn();
	}
	else {
		nError = theApp.ioControl.SetTowerBlueLightOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_TOWERLIGHT_BLUE] = false;
		msg.Format("Tower Blue Light :: Failed");
	}
	else {
		m_bStart[IO_STATUS_TOWERLIGHT_BLUE] = !m_bStart[IO_STATUS_TOWERLIGHT_BLUE];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_TOWERLIGHT_BLUE])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Tower Blue Light :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}

void IOInteraction::OnBnClickedIointeractionButtonIonizerDisStop()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_IONIZER_DIS_STOP] == TRUE) {
		m_bIOChecked[IO_STATUS_IONIZER_DIS_STOP] = TRUE;
	}
	else if (m_bStart[IO_STATUS_IONIZER_DIS_STOP] == FALSE) {
		m_bIOChecked[IO_STATUS_IONIZER_DIS_STOP] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_IONIZER_DIS_STOP]) {
		nError = theApp.ioControl.SetIonizerDischargeStopOn();
	}
	else {
		nError = theApp.ioControl.SetIonizerDischargeStopOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_IONIZER_DIS_STOP] = false;
		msg.Format("Ionizer Discharge Stop :: Failed");
	}
	else {
		m_bStart[IO_STATUS_IONIZER_DIS_STOP] = !m_bStart[IO_STATUS_IONIZER_DIS_STOP];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_IONIZER_DIS_STOP])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Ionizer Discharge Stop :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}


void IOInteraction::OnBnClickedIointeractionButtonIonizerReset()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL] == TRUE) {
		m_bIOChecked[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL] = TRUE;
	}
	else if (m_bStart[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL] == FALSE) {
		m_bIOChecked[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL]) {
		nError = theApp.ioControl.SetIonizerMaintenanceSignalOn();
	}
	else {
		nError = theApp.ioControl.SetIonizerMaintenanceSignalOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL] = false;
		msg.Format("Ionizer Reset :: Failed");
	}
	else {
		m_bStart[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL] = !m_bStart[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_IONIZER_MAINTENANCE_SIGNAL])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Ionizer Reset :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}


void IOInteraction::OnBnClickedIointeractionButtonLoaderReadyToReceive()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_LOADER_READY_TO_RECEIVE] == TRUE) {
		m_bIOChecked[IO_STATUS_LOADER_READY_TO_RECEIVE] = TRUE;
	}
	else if (m_bStart[IO_STATUS_LOADER_READY_TO_RECEIVE] == FALSE) {
		m_bIOChecked[IO_STATUS_LOADER_READY_TO_RECEIVE] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_LOADER_READY_TO_RECEIVE]) {
		nError = theApp.ioControl.SetLoaderReadyToReceiveOn();
	}
	else {
		nError = theApp.ioControl.SetLoaderReadyToReceiveOn();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_LOADER_READY_TO_RECEIVE] = false;
		msg.Format("Loader Ready To Receive :: Failed");
	}
	else {
		m_bStart[IO_STATUS_LOADER_READY_TO_RECEIVE] = !m_bStart[IO_STATUS_LOADER_READY_TO_RECEIVE];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_LOADER_READY_TO_RECEIVE])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Loader Ready To Receive :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}


void IOInteraction::OnBnClickedIointeractionButtonLoaderTransferSuccess()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_LOADER_TRANSFER_SUCCESS] == TRUE) {
		m_bIOChecked[IO_STATUS_LOADER_TRANSFER_SUCCESS] = TRUE;
	}
	else if (m_bStart[IO_STATUS_LOADER_TRANSFER_SUCCESS] == FALSE) {
		m_bIOChecked[IO_STATUS_LOADER_TRANSFER_SUCCESS] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_LOADER_TRANSFER_SUCCESS]) {
		nError = theApp.ioControl.SetLoaderTransferSuccessOn();
	}
	else {
		nError = theApp.ioControl.SetLoaderTransferSuccessOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_LOADER_TRANSFER_SUCCESS] = false;
		msg.Format("Loader Transfer Success :: Failed");
	}
	else {
		m_bStart[IO_STATUS_LOADER_TRANSFER_SUCCESS] = !m_bStart[IO_STATUS_LOADER_TRANSFER_SUCCESS];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_LOADER_TRANSFER_SUCCESS])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Loader Transfer Success :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}


void IOInteraction::OnBnClickedIointeractionButtonUnloaderPanelAvailable()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_UNLOADER_PANEL_AVAILABLE] == TRUE) {
		m_bIOChecked[IO_STATUS_UNLOADER_PANEL_AVAILABLE] = TRUE;
	}
	else if (m_bStart[IO_STATUS_UNLOADER_PANEL_AVAILABLE] == FALSE) {
		m_bIOChecked[IO_STATUS_UNLOADER_PANEL_AVAILABLE] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_UNLOADER_PANEL_AVAILABLE]) {
		nError = theApp.ioControl.SetUnloaderPanelAvailableOn();
	}
	else {
		nError = theApp.ioControl.SetUnloaderPanelAvailableOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_UNLOADER_PANEL_AVAILABLE] = false;
		msg.Format("Unloader Panel Available :: Failed");
	}
	else {
		m_bStart[IO_STATUS_UNLOADER_PANEL_AVAILABLE] = !m_bStart[IO_STATUS_UNLOADER_PANEL_AVAILABLE];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_UNLOADER_PANEL_AVAILABLE])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Unloader Panel Available :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}


void IOInteraction::OnBnClickedIointeractionButtonUnloaderPanelPushOut()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_UNLOADER_PANEL_PUSH_OUT] == TRUE) {
		m_bIOChecked[IO_STATUS_UNLOADER_PANEL_PUSH_OUT] = TRUE;
	}
	else if (m_bStart[IO_STATUS_UNLOADER_PANEL_PUSH_OUT] == FALSE) {
		m_bIOChecked[IO_STATUS_UNLOADER_PANEL_PUSH_OUT] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_UNLOADER_PANEL_PUSH_OUT]) {
		nError = theApp.ioControl.SetUnloaderPanelPushOutOn();
	}
	else {
		nError = theApp.ioControl.SetUnloaderPanelPushOutOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_UNLOADER_PANEL_PUSH_OUT] = false;
		msg.Format("Unloader Push Out :: Failed");
	}
	else {
		m_bStart[IO_STATUS_UNLOADER_PANEL_PUSH_OUT] = !m_bStart[IO_STATUS_UNLOADER_PANEL_PUSH_OUT];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_UNLOADER_PANEL_PUSH_OUT])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Unloader Push Out :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}


void IOInteraction::OnBnClickedIointeractionButtonGuardDoorLock()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_GUARD_DOOR_LOCK] == TRUE) {
		m_bIOChecked[IO_STATUS_GUARD_DOOR_LOCK] = TRUE;
	}
	else if (m_bStart[IO_STATUS_GUARD_DOOR_LOCK] == FALSE) {
		m_bIOChecked[IO_STATUS_GUARD_DOOR_LOCK] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_GUARD_DOOR_LOCK]) {
		nError = theApp.ioControl.SetGuardDoorLockOn();
	}
	else {
		nError = theApp.ioControl.SetGuardDoorLockOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_GUARD_DOOR_LOCK] = false;
		msg.Format("Guard Door Lock :: Failed");
	}
	else {
		m_bStart[IO_STATUS_GUARD_DOOR_LOCK] = !m_bStart[IO_STATUS_GUARD_DOOR_LOCK];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_GUARD_DOOR_LOCK])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Guard Door Lock :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}


void IOInteraction::OnBnClickedIointeractionButtonIonizerValveOn()
{
	CString msg;
	int nError = 0;

	if (m_bStart[IO_STATUS_IONIZER_VALVE_ON] == TRUE) {
		m_bIOChecked[IO_STATUS_IONIZER_VALVE_ON] = TRUE;
	}
	else if (m_bStart[IO_STATUS_IONIZER_VALVE_ON] == FALSE) {
		m_bIOChecked[IO_STATUS_IONIZER_VALVE_ON] = FALSE;
	}

	if (m_bIOChecked[IO_STATUS_IONIZER_VALVE_ON]) {
		nError = theApp.ioControl.SetIonizerValveOn();
	}
	else {
		nError = theApp.ioControl.SetIonizerValveOff();
	}

	if (nError) {
		m_bIOChecked[IO_STATUS_IONIZER_VALVE_ON] = false;
		msg.Format("Ionizer Valve On :: Failed");
	}
	else {
		m_bStart[IO_STATUS_IONIZER_VALVE_ON] = !m_bStart[IO_STATUS_IONIZER_VALVE_ON];
		CRect rect;
		GetDlgItem(m_nResourceID[IO_STATUS_IONIZER_VALVE_ON])->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
		msg.Format("Ionizer Valve On :: Completed");
	}
	GetDlgItem(IDC_IOINTERACTION_FEEDBACK)->SetWindowTextA(msg);
}
