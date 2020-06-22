#pragma once

#include "Application.h"
#include "resource.h"
#include "afxwin.h"

// TerminalServiceDlg dialog

class TerminalServiceDlg : public CDialog
{
	DECLARE_DYNAMIC(TerminalServiceDlg)

public:
	TerminalServiceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~TerminalServiceDlg();

	CBrush m_brush;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EQUIP_TERMINAL_SERVICE_DIALOG};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strClientMessage;
	CString m_strHostMessage;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSendMsg();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

};
