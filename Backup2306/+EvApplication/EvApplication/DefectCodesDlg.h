#pragma once
#include "afxwin.h"
#include "afxpropertygridctrl.h"
#include "CopyPVIInspectionDlg.h"
#include "ImageSelectionDlg.h"

// CDefectCodesDlg dialog
class CDefectCodesDlg : public CDialog
{
	DECLARE_DYNAMIC(CDefectCodesDlg)

public:
	CDefectCodesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDefectCodesDlg();

// Dialog Data
	enum { IDD = IDD_DEFECTCODESDLG };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonApply();
	virtual BOOL OnInitDialog();
	CMFCPropertyGridCtrl m_ctrlDefetCodes;

	void DefectCodesPage();
	void RetrieveDefectCodesPage();

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

	COLORREF SetDefectColor(int nDefectID);
	COleVariant RetrivePropertyColor(int nPropertyItem, int nSubItem, BOOL bSubPorperty = FALSE, int nSubItemIndex = 0);
	
	int getFovIndexFromDefectName(std::string strDefectName, int nTrack, int nNoFov, int nErrorCodeId);
private:
	std::vector < ItemInfo > _defect;
	std::vector < GroupInfo > _group;
	std::vector < int > _defectGroupPosition;
	std::vector < CMFCPropertyGridColorProperty* > _property;
	std::map<CString, std::vector<int>> mapDefectName[MAX_TRACKS];
public:
	afx_msg void OnBnClickedOk();
};
