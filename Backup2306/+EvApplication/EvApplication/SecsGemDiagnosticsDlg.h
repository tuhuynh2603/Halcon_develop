#pragma once

#include "Application.h"
#include "resource.h"
#include "afxwin.h"

// SecsGemDiagnosticsDlg dialog

enum GEM_VARIABLE_TYPE
{
	STATUS_VARIABLE,
	EQUIPMENT_CONSTANT,
	DATA_VARIABLE
};

enum CONTROL_STATE
{
	ONLINE_LOCAL,
	ONLINE_REMOTE,
	TOOL_OFFLINE,
	HOST_OFFLINE
};

class SecsGemDiagnosticsDlg : public CDialog
{
	DECLARE_DYNAMIC(SecsGemDiagnosticsDlg)

public:
	SecsGemDiagnosticsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SecsGemDiagnosticsDlg();
	CBrush m_brush;

	void initializeParameters();
	void controlGUIDisplayItems(BOOL enable);	//Enable/Disable GUI items while communication state changes to avoid unexpected calls to API

// Dialog Data
	enum { IDD = IDD_SECSGEM_COMMUNICATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedEnableCommunicationButton();
	afx_msg void OnBnClickedCommunicationOnlineButton();
	afx_msg void OnBnClickedLocalRadio();
	afx_msg void OnBnClickedRemoteRadio();
	afx_msg void OnBnClickedRefreshEventButton();
	afx_msg void OnBnClickedEnableEventButton();
	afx_msg void OnBnClickedDisableEventButton();
	afx_msg void OnBnClickedSendEventButton();
	afx_msg void OnBnClickedClearLogButton();
	afx_msg void OnBnClickedGetStatusVariableValueButton();
	afx_msg void OnBnClickedSetStatusVariableValueButton();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSetAlarmButton();
	afx_msg void OnBnClickedClearAlarmButton();
	afx_msg void OnBnClickedRefreshAlarmButton();
	afx_msg void OnCbnSelchangeGemVariablesTypeCombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

private:
	void InitializeGemVariables();
	void InitializeDataCollection();
	void InitializeAlarmManagement();
	void CommunicationStateUpdate();
	void ControlStateUpdate();
	void ProcessingStateUpdate();
	void SpoolingStateUpdate();
	void updateLogger();
	void SetControlState(int state);

	CButton   _enableCommunicationButton;
	CListBox  _loggerList;
	CString   _communicationState;
	CString   _controlState;
	CButton   _communicationOnlineButton;
	CComboBox _disabledEventComboBox;
	CComboBox _enabledEventComboBox;
	CComboBox _alarmComboBox;
	CComboBox _gemVariableComboBox;
	CComboBox _gemVariableTypeComboBox;
	CEdit     _statusVariableEditControl;
	CString   _gemVariableValue;
	CString   _spoolingState;
	CString   _spoolingSubState;
	int		  _selectedGemVariableType;
	int		  _selectedControlState;
	int		  _ToolControlState;
	int		  _selectedCommunicateState;
public:
	CString m_strHostDtateTime;
	afx_msg void OnBnClickedButton2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
