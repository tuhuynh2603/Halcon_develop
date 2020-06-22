#pragma once
#include "resource.h"
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <vector>
#include "afxwin.h"
// CVerificationMode dialog

class CVerificationMode : public CDialogEx
{
	DECLARE_DYNAMIC(CVerificationMode)

public:
	CVerificationMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVerificationMode();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VERIFICATION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
//	afx_msg void OnCbnSelchangeComboDefectCategory();
	CComboBox m_comboDefectList;
	CComboBox m_comboDieNo;
	CComboBox m_comboFovNo;
	std::vector <int> _defectCode;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonApplyToFov();
	afx_msg void OnBnClickedButtonApplyToDie();
	afx_msg void OnBnClickedButtonRevert();
	void UpdateDieFovPositions(int nDieNo, int nFovNo);
	void Initialize();
	void BackUpPreviousMapping();

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
