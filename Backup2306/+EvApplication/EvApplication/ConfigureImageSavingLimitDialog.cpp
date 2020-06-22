// ConfigureImageSavingLimitDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigureImageSavingLimitDialog.h"
#include "afxdialogex.h"
#include "Application.h"
#include "GlobalBackgroundThread.h"

// ConfigureImageSavingLimitDialog dialog

IMPLEMENT_DYNAMIC(ConfigureImageSavingLimitDialog, CDialog)

ConfigureImageSavingLimitDialog::ConfigureImageSavingLimitDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ConfigureImageSavingLimitDialog::IDD, pParent)
	, _imageSaveLimitPercentage(10)
	, _enableImageSaveLimitCheck(FALSE)
{

}

ConfigureImageSavingLimitDialog::~ConfigureImageSavingLimitDialog()
{
}

void ConfigureImageSavingLimitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IMAGE_SAVE_LIMIT_PERCENTAGE, _imageSaveLimitPercentage);
	DDX_Check(pDX, IDC_ENABLE_IMAGE_SAVING_LIMIT_CHECK, _enableImageSaveLimitCheck);
}


BEGIN_MESSAGE_MAP(ConfigureImageSavingLimitDialog, CDialog)
	ON_BN_CLICKED(IDOK, &ConfigureImageSavingLimitDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ConfigureImageSavingLimitDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// ConfigureImageSavingLimitDialog message handlers


void ConfigureImageSavingLimitDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (_imageSaveLimitPercentage < 0 || _imageSaveLimitPercentage > 100) {
		if (AfxMessageBox("Image saving percentage must be within range from 0% to 100%. Please retry!", MB_OK | MB_ICONERROR) == IDOK)
			return;
	}

	saveParameters();

	CDialog::OnOK();
}


void ConfigureImageSavingLimitDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

int  ConfigureImageSavingLimitDialog::getImageSaveLimitPercentage() const
{
	return _imageSaveLimitPercentage;
}

BOOL ConfigureImageSavingLimitDialog::getEnableImageSaveLimitCheckStatus() const
{
	return _enableImageSaveLimitCheck;
}

BOOL ConfigureImageSavingLimitDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	loadParameters();

	UpdateData(FALSE);

	return TRUE;
}

void ConfigureImageSavingLimitDialog::saveParameters()
{
	theApp.pGlobalBackgroundThread->setSaveImageLimitParameters(_enableImageSaveLimitCheck, _imageSaveLimitPercentage);

	theApp.pGlobalBackgroundThread->saveSequenceOptionParameters();
}

void ConfigureImageSavingLimitDialog::loadParameters()
{
	theApp.pGlobalBackgroundThread->loadSequenceOptionParameters();
	
	_enableImageSaveLimitCheck = theApp.pGlobalBackgroundThread->getEnableSaveImageLimitStatus();
	_imageSaveLimitPercentage = theApp.pGlobalBackgroundThread->getSaveImageLimitPercentage();
}