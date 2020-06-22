#pragma once
#include "resource.h"
#include "afxwin.h"

// SequenceOptions dialog
class CGlobalBackgroundThread;

class SequenceOptions : public CDialogEx
{
	DECLARE_DYNAMIC(SequenceOptions)

public:
	SequenceOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~SequenceOptions();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SEQUENCE_OPTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL m_bEnableBarcodeReader;

	
	BOOL m_bEnableIonizer;
	BOOL m_bDryRun;
	BOOL m_bWetRun;
	BOOL m_bEnableTopScanning;
	BOOL m_bEnableTHRScanning;


	
	void LoadData(void);
	void SaveData() const;

private:
	MotionParameters * pMotionParms;
	CGlobalBackgroundThread * pSequence;

public:
	afx_msg void OnBnClickedCheckDryRun();
	afx_msg void OnBnClickedCheckWetRun();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
	BOOL m_bEnableDatumOffset;
	BOOL m_bEnableBarcodeReaderWithCylinder;


};
