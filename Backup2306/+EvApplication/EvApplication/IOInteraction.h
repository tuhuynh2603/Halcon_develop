#pragma once

#include "afxwin.h"
#include "resource.h"
#include <vector>


// IOInteraction dialog

enum IOSensorActions {

	//Loader
	LEADFRAMEPUSHER_IN, LEADFRAMEPUSHER_OUT,
	LOADERMAGPUSHER_IN, LOADERMAGPUSHER_OUT,
	LOADERMAGCLAMP_OPEN, LOADERMAGCLAMP_CLOSE,

	LOADERMAGAZINEPRESENCE_STATUS,
	LOADERMAGAZINEFULL_STATUS,
	LOADERLEADFRAMEPRESENCE_STATUS,
	LOADERMAGAZINECRASH_STATUS,

	LOADERMAGAZIMECLAMPOPEN_STATUS,
	LOADERMAGAZINECLAMPCLOSE_STATUS,
	LOADERPUSHERIN_STATUS,
	LOADERPUSHEROUT_STATUS,
	LEADFRAMEPUSHERIN_STATUS,
	LEADFRAMEPUSHER_CRASH,
	LEADFRAMEPUSHEROUT_STATUS,
	LOADER_LF_MAPPING_SENSOR,


	//Transport
	GRIPPER_UP, GRIPPER_DOWN,
	GRIPPERCLAMP_OPEN, GRIPPERCLAMP_CLOSE,
	TRACKCLAMP_UP, TRACKCLAMP_DOWN,
	TRACKCLAMP2_UP, TRACKCLAMP2_DOWN,
	VACUUM_UP, VACUUM_DOWN,
	VACUUM_ON, VACUUM_OFF,
	VACUUM_PURGE_ON, VACUUM_PURGE_OFF,

	PULLPRESENTSENSOR_STATUS,
	PUSHJAMSENSOR_STATUS,
	SUBSTRATEPRESENCE_STATUS,
	WIDTHCRASH_STATUS,
	VACUUMUP_STATUS,
	VACUUMSWITCH_STATUS,
	GRIPPERUP_STATUS,


	//Unloader
	UNLOADERMAGPUSHER_IN, UNLOADERMAGPUSHER_OUT,
	UNLOADERMAGCLAMP_OPEN, UNLOADERMAGCLAMP_CLOSE,

	UNLOADERMAGAZINEPRESENCE_STATUS,
	UNLOADERMAGAZINEFULL_STATUS,
	UNLOADERLEADFRAMEPRESENCE_STATUS,
	UNLOADERMAGAZINECRASH_STATUS,

	UNLOADERMAGAZIMECLAMPOPEN_STATUS,
	UNLOADERMAGAZINECLAMPCLOSE_STATUS,
	UNLOADERPUSHERIN_STATUS,
	UNLOADERPUSHEROUT_STATUS,
	UNLOADER_LF_MAPPING_SENSOR,
};

enum IOStatusChecked {
	IO_STATUS_TOWERLIGHT_RED = 0,
	IO_STATUS_TOWERLIGHT_OGRANGE,
	IO_STATUS_TOWERLIGHT_GREEN,
	IO_STATUS_TOWERLIGHT_BLUE,
	IO_STATUS_BUTTONLAMP_RESET,
	IO_STATUS_BUTTONLAMP_START,
	IO_STATUS_BUTTONLAMP_CYCLE_STOP,
	IO_STATUS_BUTTONLAMP_STOP,
	IO_STATUS_BUZZER,
	IO_STATUS_DOOR_SWITCH,
	IO_STATUS_IONIZER_DIS_STOP,
	IO_STATUS_IONIZER_MAINTENANCE_SIGNAL,
	IO_STATUS_IONIZER_VALVE_ON,
	IO_STATUS_IONIZER_ERROR,
	IO_STATUS_LOADER_READY_TO_RECEIVE,
	IO_STATUS_LOADER_TRANSFER_SUCCESS,
	IO_STATUS_LOADER_PANEL_AVAILABLE,
	IO_STATUS_LOADER_PANEL_IN,
	IO_STATUS_UNLOADER_PANEL_AVAILABLE,
	IO_STATUS_UNLOADER_PANEL_PUSH_OUT,
	IO_STATUS_UNLOADER_READY_TO_RECEIVE,
	IO_STATUS_UNLOADER_TRANSFER_SUCCESS,
	IO_STATUS_GUARD_HEALTHY,
	IO_STATUS_GUARD_BYPASS,
	IO_STATUS_INCOMING_AIR_PRESSURE,
	IO_STATUS_EMERGENCY,
	IO_STATUS_GUARD_DOOR_LOCK,
	//Add New IO Status here
	IO_STATUS_COUNT
};

class IOInteraction : public CDialogEx
{
	DECLARE_DYNAMIC(IOInteraction)

public:
	IOInteraction(CWnd* pParent = NULL);   // standard constructor
	virtual ~IOInteraction();

	void setDialogVisibility(BOOL * pIsDialogVisible);

	int m_nNoOfIOSegments;
	BOOL	m_bStart[IO_STATUS_COUNT];
	BOOL m_bIOChecked[IO_STATUS_COUNT];
	CRgn m_prgnIOStatusSegment[IO_STATUS_COUNT];
	int m_nResourceID[IO_STATUS_COUNT];

	CBrush m_brushChecked;
	CBrush m_brushUnchecked;
	CBrush m_brushBorder;


	int GetIOStatus(int nIOStatusId, BOOL &bOn);
	// Dialog Data
	enum { IDD = IDD_IOINTERACTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnTcnSelchangeIointeractionTabctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedIointeractionTowerlightredOn();
	afx_msg void OnBnClickedIointeractionTowerlightorangeOn();
	afx_msg void OnBnClickedIointeractionTowerlightgreenOn();
	afx_msg void OnBnClickedIointeractionBuzzerOn();
	afx_msg void OnBnClickedIointeractionDooropen();
	afx_msg void OnBnClickedIointeractionResetbuttonlampOn();
	afx_msg void OnBnClickedIointeractionStopbuttonlampOn();
	afx_msg void OnBnClickedIointeractionStartbuttonlampOn();
	afx_msg void OnBnClickedIointeractionResetbuttontriggerStatus();
	afx_msg void OnBnClickedIointeractionStopbuttontriggerStatus();
	afx_msg void OnBnClickedIointeractionStartpbuttontriggerStatus();

	DECLARE_MESSAGE_MAP()

private:
	void hideDialog();
	void startCheckSensorTimer();
	void stopCheckSensorTimer();
	void updateSensors();
	void loadBitmaps();
	void changeTab(int tabNumber);
	void fillUnits();
	void posRectToClient();
	CString triggerIO(IOSensorActions action, BOOL &bOn, BOOL &bError);
	void setBackgroundImage(const CString &imageName, CStatic &pictureControl, int height, int width);

	CTabCtrl tabControl;
	CStatic pictureControl;
	CStatic pictureControlUpClose;

	CButton resetButtonLamp;
	CButton stopButtonLamp;
	CButton startButtonLamp;
	CButton doorOpen;

	CRect bitmapRect;

	CBrush backgroundBrush;
	CFont standardFont;

	std::vector<CString> tabTitlesStr;

	BOOL * pIsDialogVisible;
	BOOL isCheckSensorTimerOn;

	int  motorActionButtonPressedIdentifier;
	bool motorActionButtonPressingIsActivated;
	bool motorActionButtonIsPressed;
	bool isUpdatingSensorState;
	bool pointInSensor;
	bool istriggeringIO;

	int tabNumber;
	CString filePathStr;
	HBITMAP initialBitmap;
	HBITMAP sensorUpCloseBitmap;

	enum IOSensorState { SENSOR_ON, SENSOR_OFF, SENSOR_ERROR, SENSOR_INIT_STATE };
	struct Unit {
		std::vector<CRect>				motorsPosRect;
		std::vector<IOSensorActions>	motorActionsEnum;
		std::vector<CString>			motorNameStr;
		std::vector<CBitmap *>          motorActionBmp;
		std::vector<CBitmap *>          motorActionClickedBmp;
		std::vector<CRect>				sensorsPosRect;
		std::vector<IOSensorActions>	sensorActionsEnum;
		std::vector<IOSensorState>		previousSensorState;
		std::vector<CString>			sensorNameStr;
		CString							pictureNameStr;
	}units[4];

	Unit * activeUnit;

	CBitmap activeSensor;
	CBitmap inactiveSensor;
	CBitmap errorSensor;

	CBitmap onBtn;
	CBitmap offBtn;

	CBitmap onBtnClicked;
	CBitmap offBtnClicked;

	CBitmap arrowAngled45;
	CBitmap arrowAngled135;
	CBitmap arrowAngled225;
	CBitmap arrowAngled315;
	CBitmap arrowUp;
	CBitmap arrowDown;
	CBitmap arrowInto;
	CBitmap arrowOutOf;

	CBitmap arrowAngled45Clicked;
	CBitmap arrowAngled135Clicked;
	CBitmap arrowAngled225Clicked;
	CBitmap arrowAngled315Clicked;
	CBitmap arrowUpClicked;
	CBitmap arrowDownClicked;
	CBitmap arrowIntoClicked;
	CBitmap arrowOutOfClicked;
public:
	afx_msg void OnBnClickedIointeractionCyclestopbuttonlampOn();
	afx_msg void OnBnClickedIointeractionTowerlightblueOn();
	afx_msg void OnBnClickedIointeractionButtonIonizerDisStop();
	afx_msg void OnBnClickedIointeractionButtonIonizerReset();
	afx_msg void OnBnClickedIointeractionButtonLoaderReadyToReceive();
	afx_msg void OnBnClickedIointeractionButtonLoaderTransferSuccess();
	afx_msg void OnBnClickedIointeractionButtonUnloaderPanelAvailable();
	afx_msg void OnBnClickedIointeractionButtonUnloaderPanelPushOut();
	afx_msg void OnBnClickedIointeractionButtonGuardDoorLock();
	afx_msg void OnBnClickedIointeractionButtonIonizerValveOn();
};