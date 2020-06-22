// InspectionSettings.cpp : implementation file
//

#include "stdafx.h"
#include "InspectionSettings.h"
#include "afxdialogex.h"


// CInspectionSettings dialog

IMPLEMENT_DYNAMIC(CInspectionSettings, CDialogEx)

CInspectionSettings::CInspectionSettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_bCheckForAllTracks(FALSE)
	, m_nInspectionDelay(0)
{

}

CInspectionSettings::~CInspectionSettings()
{
}

void CInspectionSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Check(pDX, IDC_CHECK_FOR_ALL_TRACKS, bCheckedForAllTracks);
	//  DDX_Text(pDX, IDC_EDIT_ADD_DELAY_TO_INSPECTION, m_strInspectionDelay);
	DDX_Check(pDX, IDC_CHECK_FOR_ALL_TRACKS, m_bCheckForAllTracks);
	DDX_Text(pDX, IDC_EDIT_ADD_DELAY_TO_INSPECTION, m_nInspectionDelay);
}


BEGIN_MESSAGE_MAP(CInspectionSettings, CDialogEx)
END_MESSAGE_MAP()


// CInspectionSettings message handlers
