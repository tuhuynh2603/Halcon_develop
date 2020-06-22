#pragma once
#include "resource.h"


// CMessageBox dialog

class CMessageBox : public CDialog
{
	DECLARE_DYNAMIC(CMessageBox)
	CFont font;
	bool bTimerOn;
	int nDuration;

public:
	CMessageBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMessageBox();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HAWKEYE_MSG_BOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_strMsg;
public:
	void Msg(CString _strMsg, int _nDuration = 0);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
