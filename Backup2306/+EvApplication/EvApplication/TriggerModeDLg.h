#pragma once

#include "resource.h"
#include "TabPageSSL.h"
#include "afxwin.h"
// CTriggerModeDLg dialog

class CTriggerModeDLg : public CTabPageSSL
{
	//DECLARE_DYNAMIC(CTriggerModeDLg)

public:
	CTriggerModeDLg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTriggerModeDLg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRIGGER_MODE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CComboBox m_comboTriggerMode;
	void InitializeCombo();
	void GetTriggerMode();
};
