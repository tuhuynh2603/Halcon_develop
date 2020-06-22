#pragma once
#include "afxwin.h"

#define WS_EX_LAYERED	0x00080000
#define LWA_COLORKEY	0x00000001
#define LWA_ALPHA		0x00000002
#define ULW_COLORKEY    0x00000001
#define ULW_ALPHA		0x00000002
#define ULW_OPAQUE      0x00000004

#define IDT_TIMER		0x00000005

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

// CResourceCheckDlg dialog

class CResourceCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CResourceCheckDlg)

public:
	CResourceCheckDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResourceCheckDlg();

	void ReleaseTimer();
	int m_nInterval;
	int m_nStepUnit;
	int m_nGlass;
	UINT_PTR m_nCheckTimer;

	BOOL m_bDiskOk;
	BOOL m_bMemoryOk;

	BOOL bFirstTimePositioning;

	void SetWindowPosition();

	// Dialog Data
	enum { IDD = 505};

protected:
	CBrush m_brush;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	LRESULT OnSetSMStatus(WPARAM wParam = 0, LPARAM lParam = 0);

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDiskSpace;
	CString m_strMemSpace;
	CString m_strSimulationStatus;
	void CheckResourceAvailability(bool bForced = FALSE);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnStnClickedStatic();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
