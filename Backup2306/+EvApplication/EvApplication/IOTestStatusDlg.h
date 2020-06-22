#pragma once


// CIOTestStatusDlg dialog

class CIOTestStatusDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIOTestStatusDlg)

public:
	CIOTestStatusDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIOTestStatusDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
