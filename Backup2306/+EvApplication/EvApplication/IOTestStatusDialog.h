#pragma once
#include "resource.h"



// CIOTestStatusDialog dialog

class CIOTestStatusDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CIOTestStatusDialog)

public:
	CIOTestStatusDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIOTestStatusDialog();
	CBrush* m_pBrush;
	CBrush m_pbrushStatus[5];
	CFont m_fontHead;
	int m_nErrorCode;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HARDWARE_STATUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strIOHStatus;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
