#pragma once
#include "afxcmn.h"

#define WS_EX_LAYERED	0x00080000
#define LWA_COLORKEY	0x00000001
#define LWA_ALPHA		0x00000002
#define ULW_COLORKEY    0x00000001
#define ULW_ALPHA		0x00000002
#define ULW_OPAQUE      0x00000004


typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

// CProgressDlg dialog

class CProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlg();
	void SetStatus(CString strStatus, int nPos);

	int GetCurrentLoad();

	CProgressCtrl ctrProgress;
	BYTE m_nGlass;
	CString m_strStatus;

	int m_nCurrentPos;

// Dialog Data
	enum { IDD = IDD_PROGRESS_DLG };

protected:
	CBrush m_brush;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};