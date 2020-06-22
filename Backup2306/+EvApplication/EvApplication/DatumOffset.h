#pragma once
#include "Application.h"
#include "resource.h"
// CDatumOffset dialog

class CDatumOffset : public CDialogEx
{
	DECLARE_DYNAMIC(CDatumOffset)

public:
	CDatumOffset(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDatumOffset();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATUM_OFFSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double m_dTopRowX;
	double m_dTopRowY;
	double m_dTBottomRowY;
	double m_dTBottomRowX;
	virtual BOOL OnInitDialog();
	CApplication* pApp;
	MotionParameters * pMotionParms;
	afx_msg void OnBnClickedSaveOffset();
	afx_msg void OnBnClickedDatumCancel();
};
