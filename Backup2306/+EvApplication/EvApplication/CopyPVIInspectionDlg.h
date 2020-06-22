#pragma once
#include "resource.h"

// CCheckBoxProp class
class CPropertyGridProperty;
class CParameters;
class CPropertyGridColorProperty;

enum
{
	PVI_FOV_PARAM,
	PVI_SETUP_PARAM,
	PVI_DEFECT_PARAM
};

class CCheckBoxProp : public CMFCPropertyGridProperty
{
public:
	CCheckBoxProp(const CString& strName, BOOL bCheck, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);

protected:
	virtual BOOL OnEdit(LPPOINT /*lptClick*/) { return FALSE; }
	virtual void OnDrawButton(CDC* /*pDC*/, CRect /*rectButton*/) {}
	virtual void OnDrawValue(CDC* /*pDC*/, CRect /*rect*/) {}
	virtual BOOL HasButton() const { return FALSE; }

	virtual BOOL PushChar(UINT nChar);
	virtual void OnDrawCheckBox(CDC * pDC, CRect rectCheck, BOOL bChecked);
	virtual void OnDrawName(CDC* pDC, CRect rect);
	virtual void OnClickName(CPoint point);
	virtual BOOL OnDblClk(CPoint point);

protected:
	CRect m_rectCheck;
};

class CBoundedNumberSubProp : public CMFCPropertyGridProperty
{
public:
	CBoundedNumberSubProp(const CString& strName, const COleVariant& varValue, int nMinValue, int nMaxValue, LPCTSTR lpszDescr = NULL);

	virtual BOOL OnUpdateValue();
private:
	int m_nMinValue;
	int m_nMaxValue;
};


// CCopyPVIInspectionDlg dialog

class CCopyPVIInspectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CCopyPVIInspectionDlg)

public:
	CCopyPVIInspectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCopyPVIInspectionDlg();

	CMFCPropertyGridCtrl m_ctrlCopyPVIInsp;

	CApplication* m_pApp;
	CInspectionHandler* pInspHandler;
	CInspectionHandler* pInspHandlerCopy;
	int m_nTrackId;
	int m_nFOV;
	int m_nDocId;
	int m_nPviAreaId;
	int m_nDefectId;
	int m_nPVIParameterType;

	std::vector< std::vector < bool > > m_bCopyPVIInspectionSetUP;
	std::vector< std::vector < bool > > m_bCopyEdgePVIInspectionSetUP;
	std::vector< std::vector < bool > > m_bCopyEdgePVIInspectionRecheck;
	bool m_bCommonCopyPVIInspectionSetUP;
	bool m_bCommonCopyPVIEdgeInspectionSetUP;
	bool m_bCommonCopyPVIEdgeInspectionRecheck;
	bool m_bCommonCopyPVIParam;
	bool bResetCopyPVIInspection;

	std::vector< bool > m_bCopyPVIInspectionFOV;
	std::vector< bool > m_bCommonCopyPVIInspectionDefect;
	std::vector< std::vector < std::vector < bool > > > m_bCopyPVIDefectCharacteristic;
	std::vector< std::vector < bool > > m_bCreateNewPVIDefectCharacteristic;

	virtual BOOL OnInitDialog();
	bool bExpanded;

	void PVIInspSelectionPage();
	void PVIFOVInspSelectionPage();
	void PVIDefectCharacteristicSelectionPage();

	void RetrievePVIInspSelectionPage();
	void RetrievePVIFOVInspSelectionPage();
	void RetrievePVIDefectCharacteristicSelectionPage();

	void ClearSelection();

	void CopyPVIDefectCharacteristicParameter();
	void CopyPVISetupParameter();
	void CopyPVIFOVParameter();

	bool isAnyCopyEnable(std::vector < bool > bCopyPVI);

	CMFCPropertyGridProperty* AddPropertyGroup(CString strPropertyName);

	void AddPropertyItem(CString strPropertyName,
		COleVariant cValue,
		int nMin,
		int nMax,
		CString strTips,
		int nType,
		CMFCPropertyGridProperty* pGroup,
		BOOL bSubProperty = false,
		CMFCPropertyGridProperty* pSubGroup = NULL);

	COleVariant RetrivePropertyItem(int nPropertyItem,
		int nSubItem,
		BOOL bSubPorperty = FALSE,
		int nSubItemIndex = 0);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COPY_PVI_PARAM_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCopyPviInspParam();
	afx_msg void OnBnClickedButtonApplyCopyPviInspParam();
};
