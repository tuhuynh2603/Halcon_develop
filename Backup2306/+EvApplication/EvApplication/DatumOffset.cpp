// DatumOffset.cpp : implementation file
//

#include "stdafx.h"
#include "DatumOffset.h"
#include "afxdialogex.h"
#include "Application.h"

// CDatumOffset dialog

IMPLEMENT_DYNAMIC(CDatumOffset, CDialogEx)

CDatumOffset::CDatumOffset(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DATUM_OFFSET, pParent)
	, m_dTopRowX(0)
	, m_dTopRowY(0)
	, m_dTBottomRowY(0)
	, m_dTBottomRowX(0)
{
	pApp = (CApplication*)AfxGetApp();
	pMotionParms = &pApp->motionParameters;
}

CDatumOffset::~CDatumOffset()
{
}

void CDatumOffset::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TOP_ROW_X, m_dTopRowX);
	DDX_Text(pDX, IDC_BOTTOM_ROW_X, m_dTBottomRowX);
	DDX_Text(pDX, IDC_TOP_ROW_Y, m_dTopRowY);

	DDX_Text(pDX, IDC_BOTTOM_ROW_Y, m_dTBottomRowY);

}


BEGIN_MESSAGE_MAP(CDatumOffset, CDialogEx)
	ON_BN_CLICKED(IDC_SAVE_OFFSET, &CDatumOffset::OnBnClickedSaveOffset)
	ON_BN_CLICKED(IDC_DATUM_CANCEL, &CDatumOffset::OnBnClickedDatumCancel)
END_MESSAGE_MAP()


// CDatumOffset message handlers


BOOL CDatumOffset::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(TRUE);

	GetDlgItem(IDC_TOP_ROW_X)->EnableWindow(theApp.m_bEnableTopDatumPosToScanTHR);
	GetDlgItem(IDC_TOP_ROW_Y)->EnableWindow(theApp.m_bEnableTopDatumPosToScanTHR);
	GetDlgItem(IDC_BOTTOM_ROW_Y)->EnableWindow(theApp.m_bEnableTopDatumPosToScanTHR);
	GetDlgItem(IDC_BOTTOM_ROW_X)->EnableWindow(theApp.m_bEnableTopDatumPosToScanTHR);


	pMotionParms->GetDatumOffsetParameters(m_dTopRowX, m_dTBottomRowX, m_dTopRowY, m_dTBottomRowY);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDatumOffset::OnBnClickedSaveOffset()
{
	UpdateData(TRUE);

	pMotionParms->SetDatumOffsetParameters(m_dTopRowX, m_dTBottomRowX, m_dTopRowY, m_dTBottomRowY);

	UpdateData(FALSE);
	CDialogEx::OnOK();
}


void CDatumOffset::OnBnClickedDatumCancel()
{
	CDialogEx::OnCancel();
}
