#pragma once

#include "resource.h"
#include <vector>
#include "afxpropertygridctrl.h"
#include <list>

// ManagePackageIdDialog dialog

class ManagePackageIdDialog : public CDialog
{
	DECLARE_DYNAMIC(ManagePackageIdDialog)

public:
	ManagePackageIdDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ManagePackageIdDialog();

// Dialog Data
	enum { IDD = IDD_MANAGE_PACKAGE_ID_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedApplyPackageCount();
	DECLARE_MESSAGE_MAP()

private:
	std::list< std::string > packageIdArray;
	CMFCPropertyGridCtrl packageIDPropertyGrid;
	size_t _elementCount;
	size_t _elementCountTemp;

	void ManagePackageIdPage();
	void RetrieveManagePackageIdPage(size_t count);


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

	void initializeWindow();
};
