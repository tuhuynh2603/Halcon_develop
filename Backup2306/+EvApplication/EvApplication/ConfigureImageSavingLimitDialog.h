#pragma once

#include "resource.h"

// ConfigureImageSavingLimitDialog dialog

class ConfigureImageSavingLimitDialog : public CDialog
{
	DECLARE_DYNAMIC(ConfigureImageSavingLimitDialog)

public:
	ConfigureImageSavingLimitDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~ConfigureImageSavingLimitDialog();

	int  getImageSaveLimitPercentage() const;
	BOOL getEnableImageSaveLimitCheckStatus() const;

	// Dialog Data
	enum { IDD = IDD_CONFIGURE_IMAGE_SAVING_LIMIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	int _imageSaveLimitPercentage;
	BOOL _enableImageSaveLimitCheck;

	void saveParameters();
	void loadParameters();
};
