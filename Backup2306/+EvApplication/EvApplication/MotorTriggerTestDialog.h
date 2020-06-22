#pragma once

#define TRIGGER_COUNT_TIMER 10

// CMotorTriggerTestDialog dialog

class CMotorTriggerTestDialog : public CDialog
{
private:
	double m_dInterval;
	int m_nFIFOSize;
	bool bRunTimer;

	CComboBox m_comboFIFODirection;
	CComboBox m_comboScaleEngine;

	DECLARE_DYNAMIC(CMotorTriggerTestDialog)

public:
	CMotorTriggerTestDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMotorTriggerTestDialog();

// Dialog Data
	enum { IDD = IDD_MOTOR_TRIGGER_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedSetFifo();
	afx_msg void OnBnClickedManualTrigger();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedResetTrig();
private:
	int m_nTriggerCount;
public:
	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
private:
	int m_nTrigCountRemain;
	int m_nTrigEnCount;
	virtual void OnCancel();
};
