#pragma once
#include "afxwin.h"
#include "resource.h"

#define INPUT_STATUS_COUNT 24
#define MAX_INPUT_STATUS_MODULE_1 16

// CIOTestDlg dialog

enum IOActions
{
	//Output IO
	START_LAMP, 
	CYCLE_STOP_LAMP, 
	IMMEDIATE_STOP_LAMP, 
	RESET_LAMP,
	K1_SERVO,
	TOWER_LIGHT_BLUE,
	TOWER_LIGHT_RED,
	TOWER_LIGHT_YELLOW,
	TOWER_LIGHT_GREEN,
	BUZZER,
	X_BIASING_CYLINDER_EXT,
	X_BIASING_CYLINDER_RET,
	Y_BIASING_CYLINDER_EXT,
	Y_BIASING_CYLINDER_RET,
	Z_BIASING_CYLINDER_EXT,
	Z_BIASING_CYLINDER_RET,

	//Module 2 IO'S

	SCANNER_CYLINDER_EXT,
	SCANNER_CYLINDER_RET,
	IONIZER_VALVE_ON,
	IONIZER_DIS_STOP,
	IONIZER_RESET,
	GAURD_DOOR_LOCK,
	CURTAIN_MUTE1,
	CURTAIN_MUTE2,
	K2_SERVO,
	SCANNER_CYLINDER_02_EXT,
	SCANNER_CYLINDER_02_RET,
	CRASH_PREVENTION_SENSOR,

};

class CIOTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIOTestDlg)

public:
	CIOTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIOTestDlg();
	int m_nNoOfOutSegments;
	int m_nNoOfInputSegments;
	BOOL m_bChecked[27];
	BOOL m_bInputChecked[INPUT_STATUS_COUNT];
	
	CRgn m_prgnInputStatusSegment[INPUT_STATUS_COUNT];
	CRgn m_prgnOutStatusSegment[27];


	CBrush m_brushChecked;
	CBrush m_brushUnchecked;
	CBrush m_brushBorder;
	CApplication* m_pApp;
	CString m_strOutputStatus[27];
	CString m_strInputStatus[INPUT_STATUS_COUNT];
	virtual void OnButtonClicked(UINT nID);
	void InputStatus();
	void OutputStatus(int action, BOOL bOn, BOOL &bError);
	void ReadOutputStatus();
	

	int nCardNo ;
	int nPortNo ;
	UINT pDataIn ;

	BOOL isCheckSensorTimerOn;


	BOOL	m_bStart[27];
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HARDWAREIOTEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CStatic* m_CtrlStatus1;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CString m_strDisplayStatus;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush* m_pBrush;
	CBrush m_pbrushStatus[2];
	CFont m_fontHead;
	
	
	afx_msg void OnBnClickedButtonXyzExt();
	afx_msg void OnBnClickedButtonXyzRet();
	afx_msg void OnBnClickedButtonInput19();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void startCheckSensorTimer();
	void stopCheckSensorTimer();
	void SafeDoorLock();
};
