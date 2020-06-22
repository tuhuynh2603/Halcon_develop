/// StatisticsView.cpp : implementation file
/// Udupa; April'2012

#include "stdafx.h"
#include "Application.h"
#include "StatisticsView.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include <direct.h>
#include "EncryptionCore.h"
#include <iostream>>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <map>
#include "DefectCodesDlg.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsView

IMPLEMENT_DYNAMIC(CStatisticsView, CDialogEx)

CStatisticsView::CStatisticsView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStatisticsView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInitialized = FALSE;
	InitializeCriticalSection(&m_csLogFile);
	m_bWriteFirstLot = FALSE;
	nPPMStripNo = 0;
	nPPMNetStripNo = 0;
	dAverageStripTimeMs = 0;
	dNetAverageStripTime = 0;

	nTotalBarNo=0;
	nGoodBarNo=0;
	nBadBarNo=0;
	nTotalDieNo = 0;
	nGoodDieNo = 0;
	nBadDieNo = 0;
	InitializeCriticalSection(&m_CriticalSectionUpdate);
	//m_brush.CreateSolidBrush(RGB(50, 50, 50));
	//m_font.CreateFont(35, 0, 0, 0, 800, TRUE, FALSE, FALSE, 
	//					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
	//					PROOF_QUALITY, FIXED_PITCH|FF_MODERN, "Segoe UI");

	CRect rctDesktop;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rctDesktop, NULL);
	//m_fontList.CreateFont(20, 0, 0, 0, 560, FALSE, FALSE, FALSE,
	m_fontList.CreateFont((int)(rctDesktop.Width() / 1280.0 * 15.52), 0, 0, 0, 350, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Segoe UI");

}

CStatisticsView::~CStatisticsView()
{
	DeleteCriticalSection(&m_csLogFile);
	m_fontList.DeleteObject();
	DeleteCriticalSection(&m_CriticalSectionUpdate);
	//m_brush.DeleteObject();
	//m_font.DeleteObject();
}

void CStatisticsView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsView)
	//}}AFX_DATA_MAP
	/*DDX_Control(pDX, IDC_DEFECTS_LIST, m_ctrlDefectsList);
	DDX_Control(pDX, IDC_SUMMARY_LIST, m_ctrlSummaryList);*/
	//DDX_Control(pDX, IDC_TAB_STATISTICS, m_TabControl);
	DDX_Control(pDX, IDC_DEFECTS_LIST, m_DefectList);
	DDX_Control(pDX, IDC_SUMMARY_LIST, m_SummaryList);
	DDX_Control(pDX, IDC_LIST_STATS_INFO, m_ListStatsInfo);
	DDX_Control(pDX, IDC_COMBO_DEVICE_LIST, m_comboDeviceList);
	DDX_Control(pDX, IDC_LIST_TOTAL_INFO, m_listTotalInfo);
	DDX_Control(pDX, IDC_COMBO_POSITION_LIST, m_comboPositionList);
}


BEGIN_MESSAGE_MAP(CStatisticsView, CDialogEx)
	//{{AFX_MSG_MAP(CStatisticsView)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP

    ON_NOTIFY(NM_CUSTOMDRAW, IDC_DEFECTS_LIST, OnCustomdrawList1)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SUMMARY_LIST, OnCustomdrawList2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_STATS_INFO, OnCustomdrawList3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_TOTAL_INFO, OnCustomdrawList4)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SUMMARY_LIST, &CStatisticsView::OnLvnItemchangedSummaryList)
	ON_BN_CLICKED(IDC_BUTTON_SUMMARY_LIST, &CStatisticsView::OnBnClickedButtonSummaryList)
//	ON_BN_CLICKED(IDC_BUTTON_DEFECT_LIST, &CStatisticsView::OnBnClickedButtonDefectList)
	ON_BN_CLICKED(IDC_BUTTON_STATS_INFO, &CStatisticsView::OnBnClickedButtonStatsInfo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_LIST, &CStatisticsView::OnCbnSelchangeComboDeviceList)
	ON_BN_CLICKED(IDC_BUTTON_TOTAL_INFO, &CStatisticsView::OnBnClickedButtonTotalInfo)
	ON_CBN_SELCHANGE(IDC_COMBO_POSITION_LIST, &CStatisticsView::OnCbnSelchangeComboPositionList)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CStatisticsView::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsView diagnostics

#ifdef _DEBUG
void CStatisticsView::AssertValid() const
{
	CDialogEx::AssertValid();
}

void CStatisticsView::Dump(CDumpContext& dc) const
{
	CDialogEx::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStatisticsView message handlers

void CStatisticsView::OnSize(UINT nType, int cx, int cy) 
{
	CDialogEx::OnSize(nType, cx, cy);
	CApplication* pApp = (CApplication*)AfxGetApp();
	CWnd* pWnd = GetDlgItem(IDC_DEFECTS_LIST);
	int nWidth;
	if(pWnd && IsWindow(pWnd->m_hWnd)) {
	//	//cx = cx >> 1;
		nWidth = (int) (cx/2);
		
			CRect RectButton;
			
			GetDlgItem(IDC_BUTTON_REFRESH)->GetClientRect(&RectButton);
			GetDlgItem(IDC_BUTTON_REFRESH)->SetWindowPos(NULL, 0, 0, RectButton.Width(), RectButton.Height(),
				SWP_NOZORDER);

			GetDlgItem(IDC_BUTTON_SUMMARY_LIST)->SetWindowPos(NULL,0, RectButton.Height(),RectButton.Width(),RectButton.Height(),
												SWP_NOZORDER);
			GetDlgItem(IDC_BUTTON_TOTAL_INFO)->SetWindowPos(NULL, 0, RectButton.Height() * 2, RectButton.Width(), RectButton.Height(),
				SWP_NOZORDER);
	/*	GetDlgItem(IDC_BUTTON_DEFECT_LIST)->SetWindowPos(NULL,0, RectButton.Height(),RectButton.Width(),RectButton.Height(),
											SWP_NOZORDER);
			GetDlgItem(IDC_BUTTON_STATS_INFO)->SetWindowPos(NULL,0, RectButton.Height()*2,RectButton.Width(),RectButton.Height(),
												SWP_NOZORDER);
			GetDlgItem(IDC_BUTTON_TOTAL_INFO)->SetWindowPos(NULL, 0, RectButton.Height() * 3, RectButton.Width(), RectButton.Height(),
												SWP_NOZORDER);
			GetDlgItem(IDC_COMBO_DEVICE_LIST)->SetWindowPos(NULL,0, RectButton.Height()*4,RectButton.Width(),RectButton.Height()*7,
												SWP_NOZORDER);
			GetDlgItem(IDC_COMBO_POSITION_LIST)->SetWindowPos(NULL, 0, RectButton.Height() * 5, RectButton.Width(), RectButton.Height() * 7,
												SWP_NOZORDER);*/
			
				if(IsWindow(m_SummaryList.m_hWnd))
					m_SummaryList.SetWindowPos(NULL, 
												RectButton.Width(),
												0,
												cx,
												cy,
												SWP_NOZORDER);

				if(IsWindow(m_DefectList.m_hWnd))
					m_DefectList.SetWindowPos(NULL, 
												RectButton.Width(),
												0,
												cx,
												cy,
												SWP_NOZORDER);

				if(IsWindow(m_ListStatsInfo.m_hWnd))
					m_ListStatsInfo.SetWindowPos(NULL, 
												RectButton.Width(),
												0,
												cx,
												cy,
												SWP_NOZORDER);

				if (IsWindow(m_listTotalInfo.m_hWnd))
					m_listTotalInfo.SetWindowPos(NULL,
						RectButton.Width(),
						0,
						cx,
						cy,
						SWP_NOZORDER);

			
		
		if(m_bInitialized) {
			CRect rect;
			/*m_ctrlSummaryList.GetClientRect(&rect);
			nWidth = rect.Width();
			m_ctrlSummaryList.SetColumnWidth(0, (int)(nWidth*0.30));
			m_ctrlSummaryList.SetColumnWidth(1, (int)(nWidth*0.40));
			m_ctrlSummaryList.SetColumnWidth(2, (int)(nWidth*0.40));

			m_ctrlDefectsList.GetClientRect(&rect);
			nWidth = rect.Width();
			m_ctrlDefectsList.SetColumnWidth(0, (int)(nWidth*0.5));
			m_ctrlDefectsList.SetColumnWidth(1, (int)(nWidth*0.49));*/
		}
	}
	
}

void CStatisticsView::OnInitialUpdate() 
{
	//Initialize();
	CApplication* pApp = (CApplication*)AfxGetApp();
	KillTimer(1);
	if(pApp->m_bIsMasterApp)
		SetTimer(1, 60000, NULL);
//	CDialogEx::OnInitialUpdate();
	CString str;
	m_comboDeviceList.ResetContent();
	for (int i = 0; i<pApp->m_nNoOfDies; i++) {
		str.Format("THA %d",i + 1);
		m_comboDeviceList.AddString(str);
	}
	m_comboPositionList.ResetContent();
	for (int i = 0; i<pApp->m_nNoOfFov; i++) {
		str.Format("Position %d", i + 1);
		m_comboPositionList.AddString(str);
	}
	m_comboDeviceList.SetCurSel(0);
	m_comboPositionList.SetCurSel(0);
	
}

void CStatisticsView::Initialize(BOOL bRefresh)
{
	//ListView_SetExtendedListViewStyle(m_ctrlSummaryList.m_hWnd, LVS_EX_FULLROWSELECT);// | LVS_EX_GRIDLINES);
	//ListView_SetExtendedListViewStyle(m_ctrlDefectsList.m_hWnd, LVS_EX_FULLROWSELECT);// | LVS_EX_GRIDLINES);
	CApplication* pApp = (CApplication*)AfxGetApp();

	m_SummaryList.DeleteAllItems();
	m_DefectList.DeleteAllItems();
	m_ListStatsInfo.DeleteAllItems();
	m_listTotalInfo.DeleteAllItems();
	//if (bRefresh) {
	//	for(int i=0;i<MAX_DIES+2;i++)
	//	m_listTotalInfo.DeleteColumn(0);
	//	m_listTotalInfo.DeleteColumn(1);
	//}
		
	COLORREF clrList = RGB(50,50,50);
	COLORREF clrText = RGB(255,255,255);

	

	m_SummaryList.SetExtendedStyle(m_SummaryList.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_TRANSPARENTBKGND | LVS_EX_FULLROWSELECT);
	m_DefectList.SetExtendedStyle(m_DefectList.GetExtendedStyle()| LVS_EX_DOUBLEBUFFER | LVS_EX_TRANSPARENTBKGND | LVS_EX_FULLROWSELECT);
	m_ListStatsInfo.SetExtendedStyle(m_ListStatsInfo.GetExtendedStyle()| LVS_EX_DOUBLEBUFFER | LVS_EX_TRANSPARENTBKGND | LVS_EX_FULLROWSELECT);
	m_listTotalInfo.SetExtendedStyle(m_ListStatsInfo.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_TRANSPARENTBKGND | LVS_EX_FULLROWSELECT);

	m_SummaryList.SetBkColor(clrList);
	m_SummaryList.SetTextBkColor(clrList);
	m_SummaryList.SetTextColor(clrText);
	m_DefectList.SetBkColor(clrList);
	m_DefectList.SetTextBkColor(clrList);
	m_DefectList.SetTextColor(clrText);
	m_ListStatsInfo.SetBkColor(clrList);
	m_ListStatsInfo.SetTextBkColor(clrList);
	m_ListStatsInfo.SetTextColor(clrText);
	
	m_listTotalInfo.SetBkColor(clrList);
	m_listTotalInfo.SetTextBkColor(clrList);
	m_listTotalInfo.SetTextColor(clrText);

	CImageList imageList; 
	imageList.Create(1, 23, ILC_COLOR, 1, 1);
	m_SummaryList.SetImageList(&imageList, LVSIL_SMALL);
	m_DefectList.SetImageList(&imageList, LVSIL_SMALL);
	m_ListStatsInfo.SetImageList(&imageList, LVSIL_SMALL);
	m_listTotalInfo.SetImageList(&imageList, LVSIL_SMALL);

	m_SummaryList.SetFont(&m_fontList);
	m_DefectList.SetFont(&m_fontList);
	m_ListStatsInfo.SetFont(&m_fontList);
	m_listTotalInfo.SetFont(&m_fontList);
	int nCol = 0;
	int nWidth;
	CRect rectArea;
	CString str;
	
	m_SummaryList.GetClientRect(&rectArea);
	nWidth = rectArea.Width();
	if (bRefresh)
		nWidth = nWidth*0.15;
	if (!bRefresh) {
		nCol = 0;

		// Create name of column in Statictis Sumamry List
		m_SummaryList.InsertColumn(nCol++, "Inspection", LVCFMT_CENTER, int(nWidth*0.8), -1);
		m_SummaryList.InsertColumn(nCol++, "Strip", LVCFMT_CENTER, int(nWidth*0.5), -1);
		m_SummaryList.InsertColumn(nCol++, "Die", LVCFMT_CENTER, int(nWidth*0.5), -1);
		//m_SummaryList.InsertColumn(nCol++, "TOP Unit", LVCFMT_CENTER, int(nWidth*0.5), -1);
		//m_SummaryList.InsertColumn(nCol++, "THR Unit", LVCFMT_CENTER, int(nWidth*0.5), -1);
		m_SummaryList.InsertColumn(nCol++, " ", LVCFMT_CENTER, int(nWidth*0.5), -1);
		m_SummaryList.InsertColumn(nCol++, " ", LVCFMT_CENTER, int(nWidth*0.5), -1);
		m_SummaryList.InsertColumn(nCol++, " ", LVCFMT_CENTER, int(nWidth*0.5), -1);


		//for (int i = 0; i < pApp->m_nTotalViewDocs[STATION_TOP]; i++)
		//{
		//	CString str;
		//	str = pApp->m_strTopCameraName[i];
		//	m_SummaryList.InsertColumn(nCol++, str, LVCFMT_CENTER, int(nWidth*0.5), -1);
		//}
		//for (int i = 0; i < pApp->m_nTotalViewDocs[STATION_THR]; i++)
		//{
		//	CString str;
		//	str = pApp->m_strTHRCameraName[i];
		//	m_SummaryList.InsertColumn(nCol++, str, LVCFMT_CENTER, int(nWidth*0.5), -1);
		//}

	/*	for (int i = 0; i<pApp->m_nTotalViewDocs[1]; i++)
		{
			CString str;
			str = pApp->m_strBottomCameraName[i];
			m_SummaryList.InsertColumn(nCol++, str, LVCFMT_CENTER, int(nWidth*0.5), -1);
		}
		for (int i = 0; i<pApp->m_nTotalViewDocs[2]; i++)
		{
			CString str;
			str = pApp->m_strSideCameraName[i];
			m_SummaryList.InsertColumn(nCol++, str, LVCFMT_CENTER, int(nWidth*0.5), -1);
		}*/
	}
	
	//for(int i=0;i<pApp->m_nNoOfFov;i++){

	// Name of row in Summary List
	AddItem(&m_SummaryList,(LPCTSTR)"Quantity", 0);
	AddItem(&m_SummaryList,(LPCTSTR)"Passed" , 0);
	AddItem(&m_SummaryList,(LPCTSTR)"Failed" , 0);
	AddItem(&m_SummaryList,(LPCTSTR)"Yield %", 0);
	AddItem(&m_SummaryList, (LPCTSTR)" ", 0);
	AddItem(&m_SummaryList, (LPCTSTR)"PPM", 0);
	AddItem(&m_SummaryList, (LPCTSTR)" ", 0);
	AddItem(&m_SummaryList, (LPCTSTR)" ", 0);
	AddItem(&m_SummaryList, (LPCTSTR)" ", 0);
	AddItem(&m_SummaryList, (LPCTSTR)" ", 0);
	AddItem(&m_SummaryList, (LPCTSTR)" ", 0);


	for (int i = 0; i < pApp->m_nTotalViewDocs[STATION_TOP]; i++)
	{
		str = pApp->m_strTopCameraName[i];
		m_DefectList.InsertColumn(nCol++, str, LVCFMT_CENTER, int(nWidth*0.5), -1);
	}
	for (int i = 0; i < pApp->m_nTotalViewDocs[STATION_THR]; i++)
	{
		str = pApp->m_strTHRCameraName[i];
		m_DefectList.InsertColumn(nCol++, str, LVCFMT_CENTER, int(nWidth*0.5), -1);
	}

	// Name of special posititon in Summary List
	m_SummaryList.SetItemText(5, 1, "Vision PPM");
	m_SummaryList.SetItemText(5, 2, "Net PPM");

	// Choose color of text in Summary List
	int i;
	for(i=0; i<m_SummaryList.GetItemCount(); i++){
		m_colorInfo2[i][0] = RGB(255, 192, 0);
	}

	int nMaxColumn = 5;
	for (int i = 1; i < nMaxColumn; i++) {
		m_colorInfo2[0][i] = RGB(150, 150, 255);
		m_colorInfo2[1][i] = RGB(50, 255, 50);
		m_colorInfo2[2][i] = RGB(255, 100, 100);
		m_colorInfo2[3][i] = RGB(50, 255, 50);
	}

	m_colorInfo2[5][1] = RGB(255, 192, 0);
	m_colorInfo2[5][2] = RGB(255, 192, 0);
		
	m_colorInfo2[6][1] = RGB(150, 150, 255);
	m_colorInfo2[6][2] = RGB(150, 150, 255);
	

	// Create table for Total Information
	m_listTotalInfo.GetClientRect(&rectArea);
	nWidth = rectArea.Width();
	if (bRefresh)
		nWidth = nWidth * 0.15;
	if (!bRefresh) {
		nCol = 0;
		m_listTotalInfo.InsertColumn(nCol++, " ", LVCFMT_CENTER, int(nWidth*0.5), -1);
		m_listTotalInfo.InsertColumn(nCol++, "Statistics", LVCFMT_LEFT, int(nWidth*2.3), -1);
		m_listTotalInfo.InsertColumn(nCol++, "TOP", LVCFMT_CENTER, int(nWidth*1.3), -1);
		m_listTotalInfo.InsertColumn(nCol++, "THR", LVCFMT_CENTER, int(nWidth*1.3), -1);
		m_listTotalInfo.InsertColumn(nCol++, "Total", LVCFMT_CENTER, int(nWidth*1.3), -1);
		m_listTotalInfo.InsertColumn(nCol++, " ", LVCFMT_CENTER, int(nWidth*1.3), -1);
	}
	
	// Create Rows
	int nRowTotalInf = 0;
	int nColTotalInf = 1;
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Device Location");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Encap Insufficient");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Encap Excess");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Encap Crack");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Encap Blow Hole");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Encap Contamination");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Encap Void");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Encap Wire And IC");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Encap Bulge");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Encap Glue");
	// Contact Error
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Contact Damage");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Contact Contamination");
	// Flex Error
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Flex Overcut");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Flex Damage");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Flex Contamination");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Flex Scratches");
	// Chicklet Swage hole Inspection 
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Hole Roundness");
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, "Hole Burr");

	// Add one row for PVI Defect Code
	AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
	m_listTotalInfo.SetItemText(nRowTotalInf++, nColTotalInf, " ");

	// Get Started Row of PVI Defect Code
	m_nStartedRowPVIDefectCode = nRowTotalInf;
	for (int nTrack = 0; nTrack <pApp->m_nTotalTracks; nTrack++) {
		std::map<int, CString> mapPVIDefect(pApp->m_pTracks[nTrack].m_mapPviDefectsStatsGeneral);

		for(std::map<int, CString>::iterator it = mapPVIDefect.begin(); it != mapPVIDefect.end(); it++) {
			AddItem(&m_listTotalInfo, (LPCTSTR) " ", 0);
			m_listTotalInfo.SetItemText(nRowTotalInf, nColTotalInf, it->second);

			// Get Id of PVI Defect Code
			m_nPVIDefectCodeId[nRowTotalInf++] = it->first;
		}
	}

	// Set text color for Total Information
	for (int i = 0; i < m_listTotalInfo.GetItemCount(); i++)
	{
		m_colorInfo4[i][1] = RGB(255, 91, 91);
		m_colorInfo4[i][2] = RGB(180, 198, 231);
		m_colorInfo4[i][3] = RGB(255, 192, 0);
		m_colorInfo4[i][4] = RGB(255, 91, 91);
	}
	

	m_bInitialized = TRUE;			   

	m_ListStatsInfo.ShowWindow(FALSE);
	m_DefectList.ShowWindow(FALSE);
	m_SummaryList.ShowWindow(TRUE);
	m_listTotalInfo.ShowWindow(FALSE);
	
}

void CStatisticsView::RefreshDefectsList()
{
	CApplication* pApp = (CApplication*)AfxGetApp();

	int i;	
	m_colorInfo1[0][0] = RGB(255, 255, 255);
	m_colorInfo1[1][0] = RGB(255, 255, 255);
	COLORREF clrEnabled = RGB(255,100,100);
	for (int j = 0; j < pApp->m_nTotalViews+2; j++){
		for (int i = 0; i<pApp->m_strDefectErrorCodesPriority.GetCount()+2; i++) {
			m_colorInfo1[i][j + 1] = clrEnabled;
		}
	}
}

void CStatisticsView::UpdateContents(BOOL bRefresh)
{
	//CApplication* pApp = (CApplication*)AfxGetApp();
	//CString strVal;
	//int nIndexListTotal;
	//
	////int nDevice = m_comboDeviceList.GetCurSel();
	////int nPosition = m_comboPositionList.GetCurSel();
	//int nDevice = pApp->m_pTracks->m_nDieIndex;
	//int nPosition = pApp->m_pTracks->m_nFOVIndex; 

	////int nTotalStripsInsp=0, nTotalStripsPass=0, nTotalStripsFail=0,
	////	nTotalUnitsInsp=0, nTotalUnitsPass=0, nTotalUnitsFail=0,
	////	nTotalUnitsPassGoodA=0, nTotalUnitsPassGoodB=0;

	////strVal.Format("THA %d", nDevice+1);
	////if (m_SummaryList.GetItemText(0,0).Compare(strVal) != 0)
	////	m_SummaryList.SetItemText(0,0, strVal);
	////if (m_DefectList.GetItemText(0, 0).Compare(strVal) != 0)
	////	m_DefectList.SetItemText(0, 0, strVal);
	////strVal.Format("Position %d", nPosition + 1);
	////if (m_SummaryList.GetItemText(1, 0).Compare(strVal) != 0)
	////	m_SummaryList.SetItemText(1, 0, strVal);
	////if (m_DefectList.GetItemText(1, 0).Compare(strVal) != 0)
	////	m_DefectList.SetItemText(1, 0, strVal);

	//int nColNo;
	//int k = 1;
	//for (int j = 0; j<pApp->m_nTotalViews; j++) {
	//	k = 0;
	//	//for (int i = 0; i<pApp->m_nNoOfFov; i++) {
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lTotalUnitsInspected);
	//		if (m_SummaryList.GetItemText(k, j + 2).Compare(strVal) != 0)
	//			m_SummaryList.SetItemText(k, j + 2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lTotalUnitsPass);
	//		if (m_SummaryList.GetItemText(k, j + 2).Compare(strVal) != 0)
	//			m_SummaryList.SetItemText(k, j + 2, strVal);
	//		k++;
	//		if (pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lTotalUnitsInspected) {
	//			strVal.Format("%0.1f", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].dTotalYield);
	//		}
	//		else
	//			strVal = "0";

	//		if (m_SummaryList.GetItemText(k, j + 2).Compare(strVal) != 0)
	//			m_SummaryList.SetItemText(k, j + 2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lTotalUnitsFail);
	//		if (m_SummaryList.GetItemText(k, j + 2).Compare(strVal) != 0)
	//			m_SummaryList.SetItemText(k, j + 2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lUserModified);
	//		if (m_SummaryList.GetItemText(k, j + 2).Compare(strVal) != 0)
	//			m_SummaryList.SetItemText(k, j + 2, strVal);
	//		k++;
	//		k++;
	//	//}
	//}

	//if (bRefresh) {
	//	for (int j = 0; j < pApp->m_nTotalViews; j++) {
	//		for (int i = m_nTotalStaticDefectsCount; i < m_DefectList.GetItemCount(); i++) {
	//			m_DefectList.DeleteItem(i);
	//		}
	//	}
	//	for (int i = 0; i<pApp->m_strPviDefectsStats[nPosition].GetCount(); i++) {
	//		AddItem(&m_DefectList, (LPCTSTR)" ", 0);
	//		m_DefectList.SetItemText(i + m_nTotalStaticDefectsCount, 1, (LPCTSTR)pApp->m_strPviDefectsStats[nPosition].GetAt(i));
	//	}
	//}


	//int nCountTemp = 0;
	//for(int j=0;j<pApp->m_nTotalViews;j++){
	//k=0;
	////for(int i = 0;i<pApp->m_nNoOfFov;i++)
	//	{
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lDieLocation);
	//		if(m_DefectList.GetItemText(k, j+2).Compare(strVal) != 0)
	//			m_DefectList.SetItemText(k, j+2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lDatumLocation);
	//		if(m_DefectList.GetItemText(k, j+2).Compare(strVal) != 0)
	//			m_DefectList.SetItemText(k, j+2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lTiltInsp);
	//		if(m_DefectList.GetItemText(k, j+2).Compare(strVal) != 0)
	//			m_DefectList.SetItemText(k, j+2, strVal);
	//		k++; 
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lCoverLayerInspection);
	//		if(m_DefectList.GetItemText(k, j+2).Compare(strVal) != 0)
	//			m_DefectList.SetItemText(k, j+2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lCoverLayerAlignment);
	//		if (m_DefectList.GetItemText(k, j + 2).Compare(strVal) != 0)
	//			m_DefectList.SetItemText(k, j + 2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lEncapInsufficient);
	//		if(m_DefectList.GetItemText(k, j+2).Compare(strVal) != 0)
	//			m_DefectList.SetItemText(k, j+2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lEncapExcess);
	//		if(m_DefectList.GetItemText(k, j+2).Compare(strVal) != 0)
	//			m_DefectList.SetItemText(k, j+2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lEncapBlowHole);
	//		if(m_DefectList.GetItemText(k, j+2).Compare(strVal) != 0)
	//			m_DefectList.SetItemText(k, j+2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lEncapContamination);
	//		if (m_DefectList.GetItemText(k, j + 2).Compare(strVal) != 0)
	//			m_DefectList.SetItemText(k, j + 2, strVal);
	//		k++;
	//		strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lDieEdge);
	//		if(m_DefectList.GetItemText(k, j+2).Compare(strVal) != 0)
	//			m_DefectList.SetItemText(k, j+2, strVal);
	//		k += (nCountTemp+1);

	//		for (int x = 0; x < pApp->m_nPviNumberOfArea[nPosition][j]; x++) {
	//			for (int y = 0; y < pApp->m_nPviNoOfDefects[nPosition][j][x]; y++) {
	//				for (int z = 0; z < pApp->m_nTotalViews; z++) {
	//					strVal.Format("-");
	//					if (m_DefectList.GetItemText(k, z + 2).Compare(strVal) != 0)
	//						m_DefectList.SetItemText(k, z + 2, strVal);
	//				}
	//				strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][j].lPviDefectCounter[x][y]);
	//				if (m_DefectList.GetItemText(k, j + 2).Compare(strVal) != 0)
	//					m_DefectList.SetItemText(k, j + 2, strVal);
	//				
	//				k++;
	//				nCountTemp++;
	//			}
	//		}
	//	}
	//}
}

void CStatisticsView::UpdateTotalContents()
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	CString strVal;



	CString strResult[8];

	// Get Strip Column Value to Display
	strResult[0].Format("%d", nTotalBarNo);
	strResult[1].Format("%d", nGoodBarNo);
	strResult[2].Format("%d", nBadBarNo);

	if (nTotalBarNo > 0)
		strResult[3].Format("%.2f", nGoodBarNo * 100. / nTotalBarNo);
	else
		strResult[3].Format("0.00");

	// Get Die Column Value to Display
	strResult[4].Format("%d", nTotalDieNo);
	strResult[5].Format("%d", nGoodDieNo);
	strResult[6].Format("%d", nBadDieNo);

	if (nTotalDieNo > 0)
		strResult[7].Format("%.2f", nGoodDieNo * 100. / nTotalDieNo);
	else
		strResult[7].Format("0.00");

	// Display Strip Column value to Summary List
	int nRow = 0, nCol = 0;
	nRow = 0;
	nCol++;
	m_SummaryList.SetItemText(nRow++, nCol, strResult[0]);
	m_SummaryList.SetItemText(nRow++, nCol, strResult[1]);
	m_SummaryList.SetItemText(nRow++, nCol, strResult[2]);
	m_SummaryList.SetItemText(nRow++, nCol, strResult[3]);

	// Display Die Column value to Summary List
	nRow = 0;
	nCol++;
	m_SummaryList.SetItemText(nRow++, nCol, strResult[4]);
	m_SummaryList.SetItemText(nRow++, nCol, strResult[5]);
	m_SummaryList.SetItemText(nRow++, nCol, strResult[6]);
	m_SummaryList.SetItemText(nRow++, nCol, strResult[7]);
	
	// Display TOP Unit and THR Unit Column value to Summary List
	//for (int j = 0; j<pApp->m_nTotalTracks; j++) {

	//	nRow = 0;
	//	nCol++;

	//	strVal.Format("%d", pApp->m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsInspected);
	//	if (m_SummaryList.GetItemText(nRow, nCol).Compare(strVal) != 0)
	//		m_SummaryList.SetItemText(nRow, nCol, strVal);
	//	nRow++;

	//	strVal.Format("%d", pApp->m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsPass);
	//	if (m_SummaryList.GetItemText(nRow, nCol).Compare(strVal) != 0)
	//		m_SummaryList.SetItemText(nRow, nCol, strVal);
	//	nRow++;

	//	strVal.Format("%d", pApp->m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsFail);
	//	if (m_SummaryList.GetItemText(nRow, nCol).Compare(strVal) != 0)
	//		m_SummaryList.SetItemText(nRow, nCol, strVal);
	//	nRow++;

	//	if (pApp->m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsInspected) {
	//		strVal.Format("%0.2f", pApp->m_LotSummary.m_TotalDisplaySummary[j].dTotalYield);
	//	}
	//	else
	//		strVal = "0.00";
	//	if (m_SummaryList.GetItemText(nRow, nCol).Compare(strVal) != 0)
	//		m_SummaryList.SetItemText(nRow, nCol, strVal);
	//	nRow++;
	//}


	//strResult[0].Format("%2.02f", static_cast<double>(3600000 / dAverageStripTimeMs));		//Hours

	if (nPPMStripNo > 0)
		strResult[0].Format("%2.02f", static_cast<double>(60000 / dAverageStripTimeMs));		//Mins
	else
		strResult[0].Format("0");

	if (nPPMNetStripNo > 0)
		strResult[1].Format("%2.02f", static_cast<double>(60000 / dNetAverageStripTime));		//Mins
	else
		strResult[1].Format("0");
	
	m_SummaryList.SetItemText(6, 1, strResult[0]);
	m_SummaryList.SetItemText(6, 2, strResult[1]);

	int nIndexListTotal;
	int *nTotalCount;
	int *nDieTotalCount;

	nTotalCount = new int[pApp->m_nNo_of_Defects];
	nDieTotalCount = new int[pApp->m_nNo_of_Defects];

	int nIndex = 0;
	int nTempCountIndex = 0;
	int nRowIndex =0;
	for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++) {
		//	OutputDebugLogTo(9, TRUE, "UpdateTotalContents 2");
		for (int j = 0; j < pApp->m_nNo_of_Defects; j++)
		{
			nTotalCount[j] = 0;
			nDieTotalCount[j] = 0;
		}
		int nColNo;
		/*
			nTotalCount[0] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lTotalUnitsInspected;
			nTotalCount[1] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lTotalUnitsPass;
			if (nTotalCount[0] > 0)
				nTotalCount[2] = double(nTotalCount[1]) / double(nTotalCount[0]) * 100 + 0.04;
			else
				nTotalCount[2] = 0;
			nTotalCount[3] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lTotalUnitsFail;
			nTotalCount[4] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lUserModified;*/

			/*	for (nIndexListTotal = 1; nIndexListTotal < 2; nIndexListTotal++) {
					strVal.Format("%d", nTotalCount[nIndexListTotal]);
					if (m_listTotalInfo.GetItemText(nIndexListTotal, nTrack + 1).Compare(strVal) != 0)
						m_listTotalInfo.SetItemText(nIndexListTotal, nTrack + 1, strVal);
				}*/
		int nIndex = 0;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lDieLocation;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapInsufficient;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapExcess;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapCrack;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapBlowHole;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapContamination;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapVoid;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapWireAndIC;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapBulge;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapGlue;
		// Contact Error
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lContactDamage;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lContactContamination;
		// Flex Error	
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lFlexOvercut;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lFlexDamage;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lFlexContamination;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lFlexScratches;
		//Chicklet Error
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lChickletRoundness;
		nTotalCount[nIndex++] = pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lChickletBurr;
		



		/*for (int x = 0; x < pApp->m_nPviNumberOfArea[i][j]; x++) {
				for (int y = 0; y < pApp->m_nPviNoOfDefects[i][j][x]; y++) {
					strVal.Format("%d", pApp->m_LotSummary.m_DisplaySummary[nDevice][i][j].lPviDefectCounter[x][y]);
				}
		}*/

		for (nRowIndex = 0; nRowIndex < nIndex; nRowIndex++) {
			strVal.Format("%d", nTotalCount[nRowIndex]);
			if (m_listTotalInfo.GetItemText(nRowIndex, nTrack + 2).Compare(strVal) != 0)
				m_listTotalInfo.SetItemText(nRowIndex, nTrack + 2, strVal);
		}
		nRowIndex = nRowIndex + nTempCountIndex;//++
	//	for (int j = 0; j < pApp->m_nTotalTracks; j++) {
		/*int nCurrentFoV = MAX_FOV;
		if (nTrack == 0)
			nCurrentFoV = pApp->motionParameters.GetTotalFOVPerDieTop();
		else
			nCurrentFoV = pApp->motionParameters.GetTotalFOVPerDieTHR();
		for (int i = 0; i < nCurrentFoV; i++) {
				for (int x = 0; x < pApp->m_nPviNumberOfArea[i][nTrack]; x++) {
					for (int y = 0; y < pApp->m_nPviNoOfDefects[i][nTrack][x]; y++) {
					strVal.Format("%d", pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lTotalPviDefectCounter[i][nTrack][x][y]);
					if (m_listTotalInfo.GetItemText(nRowIndex, nTrack + 1).Compare(strVal) != 0)
						m_listTotalInfo.SetItemText(nRowIndex, nTrack + 1, strVal);
					nRowIndex++;
					nTempCountIndex++;
				}
			}
		}*/

		// Add one row for PVI Defect
		nRowIndex++;

		if (pApp->m_LotSummary.m_mapIndividualPviDefectCounter[nTrack].size() > 0) {
			for (std::map<int, long>::iterator it = pApp->m_LotSummary.m_mapIndividualPviDefectCounter[nTrack].begin();
				it != pApp->m_LotSummary.m_mapIndividualPviDefectCounter[nTrack].end(); it++) {
				strVal.Format("%d", it->second);
				if (m_listTotalInfo.GetItemText(nRowIndex, nTrack + 2).Compare(strVal) != 0)
					m_listTotalInfo.SetItemText(nRowIndex, nTrack + 2, strVal);
				nRowIndex++;
				nTempCountIndex++;
			}
		}
	}

	//	}
//		OutputDebugLogTo(9, TRUE, "UpdateTotalContents 3");
	

//	int nRowIndex = 0;
	//nTempCountIndex = 0;
	for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++) {
//		OutputDebugLogTo(9, TRUE, "UpdateTotalContents 4");
		nRowIndex = 0;
		int nIndex = 0;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lDieLocation;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		if (m_listTotalInfo.GetItemText(nRowIndex,pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex,pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapInsufficient;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex,pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex,pApp->m_nTotalTracks + 2, strVal);
		nIndex++;


		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapExcess;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapCrack;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapBlowHole;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex,pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex,pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapContamination;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapVoid;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapWireAndIC;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapBulge;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lEncapGlue;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;
		// Contact Error
		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lContactDamage;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lContactContamination;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;
		// Flex Error
		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lFlexOvercut;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lFlexDamage;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lFlexContamination;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lFlexScratches;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;
	
		//Chicklet Error
		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lChickletRoundness;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		nDieTotalCount[nIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lChickletBurr;
		strVal.Format("%d", nDieTotalCount[nIndex]);
		nRowIndex++;
		if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nTotalTracks + 2).Compare(strVal) != 0)
			m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nTotalTracks + 2, strVal);
		nIndex++;

		//nRowIndex++;
		////nRowIndex = nRowIndex + nTempCountIndex;
		//for (int j = 0; j < pApp->m_nTotalViews; j++) {
		//	for (int i = 0; i < pApp->m_nNoOfFov; i++){
		//		for (int x = 0; x < pApp->m_nPviNumberOfArea[i][j]; x++) {
		//			for (int y = 0; y < pApp->m_nPviNoOfDefects[i][j][x]; y++) {
		//				nDieTotalCount[nRowIndex] += pApp->m_LotSummary.m_TotalDisplaySummary[nTrack].lTotalPviDefectCounter[i][j][x][y];
		//				strVal.Format("%d", nDieTotalCount[nRowIndex]);
		//				if (m_listTotalInfo.GetItemText(nRowIndex, pApp->m_nNoOfDies + 1).Compare(strVal) != 0)
		//					m_listTotalInfo.SetItemText(nRowIndex, pApp->m_nNoOfDies + 1, strVal);
		//				nRowIndex++;
		//			}
		//		}
		//	}
		//}
//		OutputDebugLogTo(9, TRUE, "UpdateTotalContents 5");
	}
	
	// Get Defect Code Color for Column 0 of Total Information
	if (theApp.m_pImageSelectionDlg != NULL) {
		std::vector < ItemInfo > _defect;
		_defect = theApp.m_pImageSelectionDlg->getDefect();
		COLORREF clrList1 = RGB(70, 70, 70);
		COLORREF clrList2 = RGB(40, 40, 40);

		for (int nRowIndex = 0; nRowIndex < m_listTotalInfo.GetItemCount(); nRowIndex++) {
			if (nRowIndex < m_nStartedRowPVIDefectCode) {
				strVal.Format("%s", m_listTotalInfo.GetItemText(nRowIndex, 1));
				CString tmpDescription = " ";
				// If Empty, get standard background color
				if (tmpDescription.Compare(strVal) == 0) {
					m_DefectCodeColor[nRowIndex] = nRowIndex % 2 ? clrList1 : clrList2;
				}
				// Esle, get color follow defect code
				else {
					for (size_t defectId = 0; defectId < _defect.size(); defectId++) {
						tmpDescription = _defect[defectId].description.c_str();
						if (tmpDescription.Compare(strVal) == 0) {
							m_DefectCodeColor[nRowIndex] = _defect[defectId].color;
							break;
						}
					}
				}
			}
			else {
				int id = m_nPVIDefectCodeId[nRowIndex];
				m_DefectCodeColor[nRowIndex] = find_if(_defect.begin(), _defect.end(), [id](const ItemInfo & item) { return id == item.id; })->color;
			}
		}
	}


	delete[] nTotalCount;
	delete[] nDieTotalCount;
}

void CStatisticsView::AddItem(CListCtrl* pCtrl, CString strItem, int nIndex)
{
	LV_ITEM lvitem;

	lvitem.iSubItem = nIndex;
	lvitem.pszText = strItem.GetBuffer(0); 
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = pCtrl->GetItemCount();
	pCtrl->InsertItem(&lvitem);
}

void CStatisticsView::AddColumn(CListCtrl* pCtrl, CString strHeader, int nIndex, int nWidth)
{
	LVCOLUMN	lvColumn;
	memset(&lvColumn, 0, sizeof(lvColumn));

	lvColumn.mask = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER | LVCF_FMT;
	lvColumn.fmt = LVCFMT_RIGHT;
	lvColumn.cx = nWidth;
	lvColumn.pszText = strHeader.GetBuffer(0);
	lvColumn.cchTextMax = strHeader.GetLength();
	lvColumn.iSubItem = -1;
	lvColumn.iOrder = nIndex;
	pCtrl->InsertColumn(0, &lvColumn);
}

void CStatisticsView::OnCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = 0;		// Initialize with Default processing
	if(pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
		*pResult = CDRF_NOTIFYITEMDRAW;		// Request msg for each item
	else if(pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;	// Request msg for each sub-item
	else if(pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM)) {
		COLORREF clrList1 = RGB( 70, 70, 70);
		COLORREF clrList2 = RGB( 40, 40, 40);
		pLVCD->clrText = m_colorInfo1[pLVCD->nmcd.dwItemSpec][pLVCD->iSubItem];
		pLVCD->clrTextBk = pLVCD->nmcd.dwItemSpec % 2 ? clrList1 : clrList2;
		*pResult = CDRF_DODEFAULT;			// Req. Windows to paint itself
	}
}

void CStatisticsView::OnCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = 0;		// Initialize with Default processing
	if(pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
		*pResult = CDRF_NOTIFYITEMDRAW;		// Request msg for each ite
	else if(pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;	// Request msg for each sub-item
	else if(pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM)) {
		COLORREF clrList1 = RGB( 70, 70, 70);
		COLORREF clrList2 = RGB( 40, 40, 40);

		pLVCD->clrText = m_colorInfo2[pLVCD->nmcd.dwItemSpec][pLVCD->iSubItem];
		pLVCD->clrTextBk = pLVCD->nmcd.dwItemSpec % 2 ? clrList1 : clrList2;
		*pResult = CDRF_DODEFAULT;			// Req. Windows to paint itself
	}
}


void CStatisticsView::OnCustomdrawList3(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = 0;		// Initialize with Default processing
	if(pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
		*pResult = CDRF_NOTIFYITEMDRAW;		// Request msg for each ite
	else if(pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;	// Request msg for each sub-item
	else if(pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM)) {
		COLORREF clrList1 = RGB( 70, 70, 70);
		COLORREF clrList2 = RGB( 40, 40, 40);

		pLVCD->clrText = m_colorInfo3[pLVCD->nmcd.dwItemSpec][pLVCD->iSubItem];
		pLVCD->clrTextBk = pLVCD->nmcd.dwItemSpec % 2 ? clrList1 : clrList2;
		*pResult = CDRF_DODEFAULT;			// Req. Windows to paint itself
	}
}

void CStatisticsView::OnCustomdrawList4(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = 0;		// Initialize with Default processing
	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
		*pResult = CDRF_NOTIFYITEMDRAW;		// Request msg for each ite
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;	// Request msg for each sub-item
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM)) {
		COLORREF clrList1 = RGB(70, 70, 70);
		COLORREF clrList2 = RGB(40, 40, 40);

		pLVCD->clrText = m_colorInfo4[pLVCD->nmcd.dwItemSpec][pLVCD->iSubItem];

		// Column[0] of Total Information using to display Defect Code
		if (pLVCD->iSubItem != 0) {
			pLVCD->clrTextBk = pLVCD->nmcd.dwItemSpec % 2 ? clrList1 : clrList2;
		}
		else {
			pLVCD->clrTextBk = m_DefectCodeColor[pLVCD->nmcd.dwItemSpec];
		}
		*pResult = CDRF_DODEFAULT;			// Req. Windows to paint itself
	}
}

void CStatisticsView::InitializeLotStats()
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	if(!pApp->m_bIsMasterApp)
		return;

	LOTSUMMARY* pLotSummary = &pApp->m_LotSummary;

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString strCurDate;
	strCurDate.Format("%02u-%02u-%04u", sysTime.wMonth, sysTime.wDay, sysTime.wYear);
	if(strCurDate == pLotSummary->strDate)
		return;

	pLotSummary->strDate = strCurDate;

	time(&pLotSummary->tmStartTimeHr);
	GetLocalTime(&pLotSummary->timePrevRecord);

	pLotSummary->strStartTimeHr.Format("%02u:%02u:%02u", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	pLotSummary->strStartTimeDay = pLotSummary->strStartTimeHr;

	CString str = pApp->m_LotSummaryRootDir;

	//////////For UserDefined Statistics reset
	/*
	pApp->m_LotSummaryLotIDDir.Format("%s\\xx_%02u-%02u-%02u", pApp->m_LotSummaryRootDir.GetBuffer(0),
		sysTime.wYear%100, sysTime.wMonth, sysTime.wDay);
		*/
	/*pApp->m_LotSummaryLotIDDir.Format("%s\\%02u-%02u-%02u", pApp->m_LotSummaryRootDir.GetBuffer(0),
		sysTime.wYear%100, sysTime.wMonth, sysTime.wDay);*/

	pApp->m_LotSummaryLotIDDir.Format("%s", pApp->m_LotSummaryRootDir.GetBuffer(0));

	OutputDebugLogTo(9,TRUE,"InitializeLotStats: m_LotSummaryLotIDDir[%s]", pApp->m_LotSummaryLotIDDir);
	//_mkdir(pApp->m_LotSummaryLotIDDir);
	
	//_mkdir(pApp->m_LotSummaryLotIDDir + "\\Hourly Stats");

	//CreateHourlyFolder(sysTime);

	/////////////////////////////
}

//void CStatisticsView::RecordLotStats(BOOL bForced)
//{
//	CApplication* pApp = (CApplication*)AfxGetApp();
//	if(!pApp->m_bIsMasterApp)
//		return;
//
//	if(pApp->m_LotSummaryLotIDDir.IsEmpty())
//		return;
//
//	LOTSUMMARY* pLotSummary = &pApp->m_LotSummary;
//
//	SYSTEMTIME sysTime;
//	GetLocalTime(&sysTime);
//
//	time_t timeCurrent;
//	time(&timeCurrent);
//	//if(difftime(timeCurrent, pLotSummary->tmStartTimeHr) >= 3600 || sysTime.wDay != pLotSummary->timePrevRecord.wDay || bForced) {
//	//	pLotSummary->strEndTimeHr.Format("%02u:%02u:%02u", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
//	//	CreateHourlyFolder(sysTime);
// //       /////////////////// 
//	//	UpdateLotHourlyInfo();
//	//	if(!bForced) {
//	//		for (int nDie = 0; nDie<MAX_DIES; nDie++)
//	//			for (int j = 0; j<MAX_FOV; j++)
//	//				for (int i = 0; i<MAX_VIEWS; i++)
//	//					pLotSummary->m_HourlySummary[nDie][j][i].Init();
//	//	}
//	//	Sleep(50);
//
//	//	time(&pLotSummary->tmStartTimeHr);
//	//	pLotSummary->strStartTimeHr = pLotSummary->strEndTimeHr;
//	//}
//
////	if(bForced || sysTime.wDay != pLotSummary->timePrevRecord.wDay) {
////		pLotSummary->strEndTimeDay.Format("%02u:%02u:%02u", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
////		UpdateLotDailyInfo();
////		Sleep(50);
////		if(!bForced) {
////			for (int nDie = 0; nDie<MAX_DIES; nDie++)
////				for (int j = 0; j<MAX_FOV; j++)
////					for (int i = 0; i<MAX_VIEWS; i++)
////						pLotSummary->m_DailySummary[nDie][j][i].Init();
////
////			pLotSummary->strDate.Format("%02u-%02u-%04u", sysTime.wMonth, sysTime.wDay, sysTime.wYear);
////			pLotSummary->strStartTimeDay = pLotSummary->strEndTimeDay;
////
////			//////////////////////////////For UserDefined Statistics reset
////			/*
////			pApp->m_LotSummaryLotIDDir.Format("%s\\xx_%02u-%02u-%02u", pApp->m_LotSummaryRootDir.GetBuffer(0),
////				sysTime.wYear%100, sysTime.wMonth, sysTime.wDay);
////
////			*/
////			pApp->m_LotSummaryLotIDDir.Format("%s\\%02u-%02u-%02u", pApp->m_LotSummaryRootDir.GetBuffer(0),
////				sysTime.wYear%100, sysTime.wMonth, sysTime.wDay);
////
////
////			_mkdir(pApp->m_LotSummaryLotIDDir);
////			///_mkdir(pApp->m_LotSummaryLotIDDir + "\\Hourly Stats");
////
////////////////////////////////////////////////////////////
////
////
////			CFileFind FindDaily;
////			CString strLotDir;
////			strLotDir.Format("%s\\*.*", pApp->m_LotSummaryRootDir.GetBuffer(0));
////			BOOL bDailyFound = FindDaily.FindFile(strLotDir);
////
////			CTime curTime = CTime::GetCurrentTime();
////			CTime fileTime;
////			CTimeSpan timeSpan;
////			while(bDailyFound) {
////				bDailyFound = FindDaily.FindNextFile();
////				if(FindDaily.IsDots())
////					continue;
////
////				if(FindDaily.IsDirectory()) {
////					FindDaily.GetCreationTime(fileTime);
////					timeSpan = curTime - fileTime;
////					if(timeSpan.GetDays() >= pLotSummary->nLotRecycleDays) {
////						strLotDir = pApp->m_LotSummaryRootDir + "\\" + FindDaily.GetFileTitle();
////						DeleteDir(strLotDir);
////					}
////				}
////			}
////		}
////	}
//
//	int nCurMins = sysTime.wHour*60 + sysTime.wMinute;
//	int nPrevMins = pLotSummary->timePrevRecord.wHour*60 + pLotSummary->timePrevRecord.wMinute;
//	if(nCurMins >= pLotSummary->nDisplayStatsResetMin &&
//			nPrevMins < pLotSummary->nDisplayStatsResetMin) {
//		pLotSummary->InitDisplaySummary();
//	}
//
//	pLotSummary->timePrevRecord = sysTime;
//}

//void CStatisticsView::UpdateLotHourlyInfo()
//{
//	CApplication* pApp = (CApplication*)AfxGetApp();
//	LOTSUMMARY* pLotSummary = &((CApplication*)AfxGetApp())->m_LotSummary;
//
//	int nTotalUnitsInsp = 0, nTotalUnitsPass = 0, nTotalUnitsFail = 0, nGrabRetry = 0;
//	int nBottom = 0;
//	int nSide = 0;
//	int nTotalDefectCount[5][8][12][25];
//	int nDefects = 25;
//
//	
//	CString strStartTime, strEndTime, strHourlyLotFile;
//	if (!pLotSummary->strStartTimeHr.IsEmpty())
//		strStartTime.Format("%c%c%c%c", pLotSummary->strStartTimeHr[0], pLotSummary->strStartTimeHr[1],
//			pLotSummary->strStartTimeHr[3], pLotSummary->strStartTimeHr[4]);
//	if (!pLotSummary->strEndTimeHr.IsEmpty())
//		strEndTime.Format("%c%c%c%c", pLotSummary->strEndTimeHr[0], pLotSummary->strEndTimeHr[1],
//			pLotSummary->strEndTimeHr[3], pLotSummary->strEndTimeHr[4]);
//
//	/////////////////////For UserDefined Statistics reset
//	//strHourlyLotFile.Format(pApp->m_LotSummaryLotIDDir + "\\Hourly Stats\\%s-%s.hr", 
//	//strStartTime, strEndTime);
//
//	strHourlyLotFile.Format(pApp->m_LotSummaryLotIDDir + "\\" + m_strHourlyFolder + "\\%s-%s.csv",
//		strStartTime, strEndTime);
//	//////////////////////////////////
//
//	OutputDebugLog("UpdateLotHourlyInfo:strHourlyLotFile = %s", strHourlyLotFile);
//	FILE* file;
//	errno_t err = fopen_s(&file, strHourlyLotFile, "a+");
//	if (err || file == NULL)
//		return;
//
//	for (int k = 0; k < pApp->m_nNoOfDies; k++)
//	{
//		nBottom = 0;
//		nSide = 0;
//		fprintf(file, "\n\nDevice %d\n", k + 1);
//		for (int i = 0; i < pApp->m_nNoOfFov; i++)
//		{
//			fprintf(file, "\n,Position %9d\n", i + 1);
//			fprintf(file, ",,,Total units Inspected,Total Units Pass,Total Yield,Total Unit Fail,Die Location,Datum Location,Tilt Inspection,Cover Layer Inspection,Encap Insufficient,Encap Excess,Die Inspection,Pvi Inspection");
//			nBottom = 0;
//			nSide = 0;
//			for (int j = 0; j<pApp->m_nTotalViews; j++)
//			{
//
//				if (j < pApp->m_nTotalViewDocs[0]) {
//					fprintf(file, "\n,,%s", pApp->m_strTopCameraName[j]);
//				}
//				else if (j >= pApp->m_nTotalViewDocs[0] && j < (pApp->m_nTotalViewDocs[1] + pApp->m_nTotalViewDocs[0])) {
//					fprintf(file, "\n,,%s", pApp->m_strBottomCameraName[nBottom]);
//					nBottom++;
//				}
//				else
//				{
//					fprintf(file, "\n,,%s", pApp->m_strSideCameraName[nSide]);
//					nSide++;
//				}
//				nTotalDefectCount[k][i][j][0] = pApp->m_LotSummary.m_DailySummary[k][i][j].lTotalUnitsInspected;
//				nTotalDefectCount[k][i][j][1] = pApp->m_LotSummary.m_DailySummary[k][i][j].lTotalUnitsPass;
//				nTotalDefectCount[k][i][j][2] = pApp->m_LotSummary.m_DailySummary[k][i][j].dTotalYield;
//				nTotalDefectCount[k][i][j][3] = pApp->m_LotSummary.m_DailySummary[k][i][j].lTotalUnitsFail;
//				nTotalDefectCount[k][i][j][4] = pApp->m_LotSummary.m_DailySummary[k][i][j].lDieLocation;
//				nTotalDefectCount[k][i][j][5] = pApp->m_LotSummary.m_DailySummary[k][i][j].lDatumLocation;
//				nTotalDefectCount[k][i][j][6] = pApp->m_LotSummary.m_DailySummary[k][i][j].lTiltInsp;
//				nTotalDefectCount[k][i][j][7] = pApp->m_LotSummary.m_DailySummary[k][i][j].lCoverLayerInspection;
//				nTotalDefectCount[k][i][j][8] = pApp->m_LotSummary.m_DailySummary[k][i][j].lEncapInsufficient;
//				nTotalDefectCount[k][i][j][9] = pApp->m_LotSummary.m_DailySummary[k][i][j].lEncapExcess;
//				nTotalDefectCount[k][i][j][10] = pApp->m_LotSummary.m_DailySummary[k][i][j].lDieEdge;
//
//
//
//				fprintf(file, ",%9d,%9d,%9d,%9d,%9d,%9d,%9d,%9d,%9d,%9d,%9d,%9d", nTotalDefectCount[k][i][j][0],
//					nTotalDefectCount[k][i][j][1], nTotalDefectCount[k][i][j][2], nTotalDefectCount[k][i][j][3], nTotalDefectCount[k][i][j][4],
//					nTotalDefectCount[k][i][j][5], nTotalDefectCount[k][i][j][6], nTotalDefectCount[k][i][j][7], nTotalDefectCount[k][i][j][8],
//					nTotalDefectCount[k][i][j][9], nTotalDefectCount[k][i][j][10], nTotalDefectCount[k][i][j][11]);
//			}
//		}
//	}
//	fclose(file);
//}

void CStatisticsView::UpdateLotInfo()
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	LOTSUMMARY* pLotSummary = &((CApplication*)AfxGetApp())->m_LotSummary;

	
	int nBottom = 0;
	int nSide = 0;
	int *nTotalDefectCount[5][8][12];

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 12; k++) {
				nTotalDefectCount[i][j][k] = new int[pApp->m_nNo_of_Defects];
			}
		}
	}

	int nDefects = 25;
	
	CString strLotFile;

	CString strTemp;
	if (!pApp->m_strLotInfo.IsEmpty()) {
		strTemp.Format("%s\\%s", pApp->m_LotSummaryRootDir, pApp->m_strLotConfigFile);
		_mkdir(strTemp);
		strLotFile = pApp->m_strLotInfo;
	}
	else
	{
		strTemp.Format("%s\\DUMMY LOT CONFIG", pApp->m_LotSummaryRootDir);
		_mkdir(strTemp);
		strLotFile = "DUMMY LOT";
	}
	CString strTempDir;
	strTempDir.Format("%s\\%s", strTemp, strLotFile);
	_mkdir(strTempDir);

	strLotFile = strTempDir + "\\" + strLotFile + ".csv";
	FILE* file;
	errno_t err = fopen_s(&file, strLotFile, "w");
	if (err || file == NULL)
		return;

	fprintf(file, "Lot ID,,[%s]\n", pApp->m_strLotInfo);
	fprintf(file, "Operator ID,,%s\n", pApp->m_UserAccInfo.m_strCurUserName);
	fprintf(file, "Recipe Name,,%s\n", pApp->m_strLotConfigFile);
	fprintf(file, "System ID,,%s\n", pLotSummary->strMachineID);
	fprintf(file, "Start Time,,[%s]\n", pLotSummary->strStartLotTime);
	fprintf(file, "End Time,,[%s]\n", pLotSummary->strEndLotTime);
	//fprintf(file, "Comments,,%s\n", pLotSummary->strComments);

	//for (int k = 0; k < pApp->m_nNoOfDies; k++)
	//{
	//	nBottom = 0;
	//	nSide = 0;
	//	fprintf(file, "\n\nTHA %d\n", k + 1);
	//	for (int i = 0; i < pApp->m_nNoOfFov; i++)
	//	{
	//		fprintf(file, "\n,Position %9d\n", i + 1);
	//		strTemp.Format(",,,Total Inspected,Total Pass,Total Yield(%%),Total Fail,User Modified,Die Location,Datum Location,Tilt Inspection,Cover Layer Inspection,Cover Layer Alignment,Encap Insufficient,Encap Excess,Encap BlowHole,Encap Contamination,Die Edge");
	//		int nCount = pApp->m_strPviDefectsStats[i].GetCount();
	//		for (int nTempIndex = 0; nTempIndex < nCount; nTempIndex++) {
	//			strTemp.Format("%s,%s", strTemp, pApp->m_strPviDefectsStats[i].GetAt(nTempIndex));
	//		}
	//		fprintf(file, strTemp);
	//		nBottom = 0;
	//		nSide = 0;
	//		int nTempCountPviIndex=0;
	//		for (int j = 0; j<pApp->m_nTotalViews; j++)
	//		{

	//			if (j < pApp->m_nTotalTracks) {
	//				fprintf(file, "\n,,%s", pApp->m_strTopCameraName[j]);
	//			}
	//			/*else if (j >= pApp->m_nTotalViewDocs[0] && j < (pApp->m_nTotalViewDocs[1] + pApp->m_nTotalViewDocs[0])) {
	//				fprintf(file, "\n,,%s", pApp->m_strBottomCameraName[nBottom]);
	//				nBottom++;
	//			}
	//			else
	//			{
	//				fprintf(file, "\n,,%s", pApp->m_strSideCameraName[nSide]);
	//				nSide++;
	//			}*/
	//			for (int nTemp = 0; nTemp < pApp->m_nNo_of_Defects; nTemp++) {
	//				nTotalDefectCount[k][i][j][nTemp] = -1;
	//			}

	//			int nDefectIndexTemp = 0;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lTotalUnitsInspected;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lTotalUnitsPass;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].dTotalYield;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lTotalUnitsFail;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lUserModified;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lDieLocation;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lDatumLocation;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lTiltInsp;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lCoverLayerInspection;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lCoverLayerAlignment;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lEncapInsufficient;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lEncapExcess;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lEncapBlowHole;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lEncapContamination;
	//			nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lDieEdge;
	//			int nTempCount = nDefectIndexTemp;
	//			nDefectIndexTemp = nDefectIndexTemp + nTempCountPviIndex;
	//			for (int x = 0; x < pApp->m_nPviNumberOfArea[i][j]; x++) {
	//				for (int y = 0; y < pApp->m_nPviNoOfDefects[i][j][x]; y++) {
	//					nTotalDefectCount[k][i][j][nDefectIndexTemp++] = pApp->m_LotSummary.m_DisplaySummary[k][i][j].lPviDefectCounter[x][y];
	//					nTempCountPviIndex++;
	//				}
	//			}

	//			for (int nTempIndex = 0; nTempIndex < nTempCount+nCount; nTempIndex++) {
	//				fprintf(file, ",%9d", nTotalDefectCount[k][i][j][nTempIndex]);
	//			}
	//		}
	//	}
	//}
	fclose(file);
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 12; k++) {
				delete[] nTotalDefectCount[i][j][k];
			}
		}
	}
}


void CStatisticsView::UpdateLotInfoMID(int nDie)
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	LOTSUMMARY* pLotSummary = &((CApplication*)AfxGetApp())->m_LotSummary;

  
	int nBottom = 0;
	int nSide = 0;
	int *nTotalDefectCount[5][8][12];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 12; k++) {
				nTotalDefectCount[i][j][k] = new int[pApp->m_nNo_of_Defects];
			}
		}
	}
	
	CString strLotFile;

	CString strTemp1;
	if (!pApp->m_strLotInfo.IsEmpty()) {
		strTemp1.Format("%s\\%s", pApp->m_LotSummaryRootDir, pApp->m_strLotConfigFile);
		_mkdir(strTemp1);
		strLotFile = pApp->m_strLotInfo;
	}
	else
	{
		strTemp1.Format("%s\\DUMMY LOT CONFIG", pApp->m_LotSummaryRootDir);
		_mkdir(strTemp1);
		strLotFile = "DUMMY LOT";
	}
	CString strTempDir;
	strTempDir.Format("%s\\%s", strTemp1, strLotFile);
	_mkdir(strTempDir);

	strLotFile = strTempDir + "\\" + strLotFile + "_Info.csv";
	
	FILE* file;
	errno_t err = fopen_s(&file, strLotFile, "a+");
	if (err || file == NULL)
		return;

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString strCurDate;
	strCurDate.Format("%02u-%02u-%04u", sysTime.wMonth, sysTime.wDay, sysTime.wYear);

	if (!m_bWriteFirstLot) {
		fprintf(file, "%s\n", "\n\nLOT SUMMARY");
		fprintf(file, "%s\n", "----------------------");
		fprintf(file, " Machine ID  %s\n", pLotSummary->strMachineID);
		fprintf(file, " Start Date  %s\n", pLotSummary->strDate);
		fprintf(file, "-----------------------------------\n");
	}
	
	nBottom = 0;
	nSide = 0;
	CString strTemp;
	fprintf(file, "\n\nTHA %d : %s\n", nDie + 1,pApp->m_strPartFlex[nDie]);
	
	fclose(file);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 12; k++) {
				delete[] nTotalDefectCount[i][j][k];
			}
		}
	}
}

void CStatisticsView::CreateHourlyFolder(SYSTEMTIME sysTime)
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	int nNoOfResetPerDay,nLotResetTime1,nLotResetTime2;
	
    nLotResetTime1   = pApp->tLotResetTime1.wHour;
	nLotResetTime2	 = pApp->tLotResetTime2.wHour;
	nNoOfResetPerDay = pApp->nNoOfResetPerDay;
	
	if(0 == nNoOfResetPerDay)
	{
		m_strHourlyFolder.Format("HourlyStats_0.00-23.59");
	}
	
	if(1 == nNoOfResetPerDay)
	{
		if(sysTime.wHour < nLotResetTime1)
			m_strHourlyFolder.Format("HourlyStats_0.00-%d.00",nLotResetTime1);
		
		if(sysTime.wHour >= nLotResetTime1)
			m_strHourlyFolder.Format("HourlyStats_%d.00-23.59",nLotResetTime1);
		
	}
	
	if(2 == nNoOfResetPerDay)
	{
		if(nLotResetTime1 < nLotResetTime2)
		{
			if(sysTime.wHour < nLotResetTime1)
				m_strHourlyFolder.Format("HourlyStats_0.00-%d.00",nLotResetTime1);
			
			if((sysTime.wHour >= nLotResetTime1)&&(sysTime.wHour < nLotResetTime2))
				m_strHourlyFolder.Format("HourlyStats_%d.00-%d.00",nLotResetTime1,nLotResetTime2);
			
			if(sysTime.wHour >= nLotResetTime2)
				m_strHourlyFolder.Format("HourlyStats_%d.00-23.59",nLotResetTime2);
		}
		
		if(nLotResetTime1 > nLotResetTime2)
		{
			if(sysTime.wHour < nLotResetTime2)
				m_strHourlyFolder.Format("HourlyStats_0.00-%d.00",nLotResetTime2);
			
			if((sysTime.wHour >= nLotResetTime2)&&(sysTime.wHour < nLotResetTime1))
				m_strHourlyFolder.Format("HourlyStats_%d.00-%d.00",nLotResetTime2,nLotResetTime1);
			
			if(sysTime.wHour >= nLotResetTime1)
				m_strHourlyFolder.Format("HourlyStats_%d.00-23.59",nLotResetTime1);
		}
		
		if(nLotResetTime1 == nLotResetTime2)
		{
			if(sysTime.wHour < nLotResetTime1)
				m_strHourlyFolder.Format("HourlyStats_0.00-%d.00",nLotResetTime1);
			
			if(sysTime.wHour >= nLotResetTime1)
				m_strHourlyFolder.Format("HourlyStats_%d.00-23.59",nLotResetTime1);
		}
	}
	_mkdir(pApp->m_LotSummaryLotIDDir + "\\" + m_strHourlyFolder);
	
    
}

void CStatisticsView::CheckAutoLogOff()
{
	time_t timeCurrent;
	time(&timeCurrent);
	CApplication* pApp = (CApplication*)AfxGetApp();
	
	// no auto log off for operator to prevent no operator recorded in report
	if(pApp->m_UserAccInfo.m_nCurrentAccessLevel == 0)
		return;

	if(difftime(timeCurrent, pApp->m_UserAccInfo.tmStartLogoutTimeHr) >= (60 * pApp->m_UserAccInfo.m_nLogoutTime)) {
		if(pApp->m_UserAccInfo.m_nCurrentAccessLevel>0){ // operator will not auto log out
			if(!pApp->m_bOnline){ // only auto log off in offline to prevent inspection run withour userID
				pApp->OnUserLogOut();
				time(&pApp->m_UserAccInfo.tmStartLogoutTimeHr);
			}
		}	
	}
}

void CStatisticsView::OnTimer(UINT_PTR nIDEvent)
{
	CheckAutoLogOff();
	//RecordLotStats();
	CDialogEx::OnTimer(nIDEvent);
}

HBRUSH CStatisticsView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//if(pWnd->GetDlgCtrlID() == IDC_STATISTICS_CAPTION) {
 //       pDC->SetTextColor(RGB(80,180,255));
 //       pDC->SetBkMode(TRANSPARENT);
	//}
 // 
	//return m_brush;
	return 0;
}

void CStatisticsView::OnLvnItemchangedSummaryList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//// TODO: Add your control notification handler code here
	//*pResult = 0;
}

BOOL CStatisticsView::OnEraseBkgnd(CDC* pDC) 
{
	//CRect rect;
 //   GetClientRect(&rect);
 //   pDC->FillRect(&rect, &m_brush);
	CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
	
	//return CFormView::OnEraseBkgnd(pDC);
}

///Udupa; Jan'2013
int CStatisticsView::UpdateAuditTrail(CString strEvent, BOOL bSave)
{
	CApplication* pApp = (CApplication*) AfxGetApp();

	CStdioFile txtFile;
	CFileException e;

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString strDate, strTime, strOutput, strEncr, strLogFile, strMd5;
	strDate.Format("%04u-%02u-%02u", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	strTime.Format("%02u:%02u:%02u", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	strOutput.Format("%s\t  %s     \t%s\t  %s\n", strTime, pApp->m_UserAccInfo.m_strCurrentAccessLevel, pApp->m_UserAccInfo.m_strCurUserName, strEvent);
	CString strLog = strOutput;
	int nIndex = strLog.Find('%');
	if(nIndex >= 0)
		strLog.Replace("%", "%%");
	OutputDebugLogTo(0, TRUE, "%s", strLog);

	EnterCriticalSection(&m_csLogFile);

	try {
		if(bSave) {
			strLogFile.Format("%s\\LogFilesDir\\LogFile_%s.txt", pApp->m_strParmFolder, strDate);
			_mkdir(pApp->m_strParmFolder + "\\" + "LogFilesDir");

			SetFileAttributes(strLogFile, FILE_ATTRIBUTE_NORMAL);
			BOOL bOpen = txtFile.Open(strLogFile, CFile::modeReadWrite | CFile::shareDenyWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::typeText, &e);

			if(!bOpen) {
				//if(!m_bAuditTrailValidated && !pApp->m_bOnline) {
					AfxMessageBox("Audit Trail Log History is disabled. Please provide valid path for Sherlock_Spyglass Stats");
					//m_bAuditTrailValidated = TRUE;
				//}
			}
			else {
				strEncr = "";
				int nLength = (int) txtFile.GetLength();
				if(nLength > HASHLEN) {
					char strTemp[HASHLEN+1];
					txtFile.SeekToBegin();
					txtFile.Read(strTemp, HASHLEN);
					strTemp[HASHLEN] = 0;
					CString strSavedMd5 = strTemp;

					txtFile.ReadString(strEncr);
					strMd5 = md5(strEncr);
					if(strMd5.Compare(strSavedMd5)) {
						CString strTemp;
						strTemp.Format("%s  %s>  %s:  Audit Trail Log file is corrupt; Contents flushed\r\n", strDate, strTime, pApp->m_UserAccInfo.m_strCurUserName);
						OutputDebugLogTo(0, TRUE, "%s", strTemp);
						//m_EditInfo.SetSel(0,-1, TRUE);
						//m_EditInfo.ReplaceSel(strTemp);
						strEncr = EncryptDecrypt(strTemp);
						txtFile.SetLength(HASHLEN);
						txtFile.SeekToEnd();
						txtFile.WriteString(strEncr);
					}
				}

				strOutput = EncryptDecrypt(strOutput);

				strEncr += strOutput;
				strMd5 = md5(strEncr);

				txtFile.SeekToBegin();
				txtFile.WriteString(strMd5);
				txtFile.SeekToEnd();
				txtFile.WriteString(strOutput);
				txtFile.Flush();
				txtFile.Close();	

				txtFile.Open(strLogFile, CFile::modeRead | CFile::shareDenyWrite , &e);
				txtFile.Close();

				SetFileAttributes(strLogFile, FILE_ATTRIBUTE_READONLY);
			}
		}

		///Udupa; June'2015
	/*	if(pApp->m_pAuditTrailDialog)
			pApp->m_pAuditTrailDialog->SendMessage(WM_AUDITTRAIL_UPDATE, (WPARAM) &sysTime, (LPARAM) &strEvent);*/
	}
	catch(...) {
		OutputDebugLogTo(9, TRUE, "Exception in UpdateAuditTrail");
	}
	LeaveCriticalSection(&m_csLogFile);

	return 0;
}




IMPLEMENT_DYNAMIC(CDialogPaneStatsview,CDockablePane)
BEGIN_MESSAGE_MAP(CDialogPaneStatsview,CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

CDialogPaneStatsview::CDialogPaneStatsview(){}
CDialogPaneStatsview::~CDialogPaneStatsview(){}

int CDialogPaneStatsview::OnCreate(LPCREATESTRUCT lp)
{
	
	if(CDockablePane::OnCreate(lp)==-1)
		return -1;
	if(!m_wndStatsDlg.Create(CStatisticsView::IDD,this))
		return -1;
	//m_wndDlg.Initialize();
	bIsClosed = FALSE;
	bRefreshPane = FALSE;
	m_wndStatsDlg.OnInitialUpdate();
	m_wndStatsDlg.Initialize();
	m_wndStatsDlg.InitializeLotStats();
	m_wndStatsDlg.UpdateTotalContents();
	m_wndStatsDlg.UpdateAuditTrail("Sherlock_Spyglass Vision Application started",TRUE);
	m_wndStatsDlg.ShowWindow(SW_SHOWDEFAULT);
	return  0;
}
void CDialogPaneStatsview::OnSize(UINT nType,int cx,int cy)
{
	CDockablePane::OnSize(nType,cx,cy);
	//m_wndStatsDlg.SetSize(cx,cy);
	m_wndStatsDlg.SetWindowPos(NULL,0,0,cx,cy,SWP_NOACTIVATE|SWP_NOZORDER);
	m_wndStatsDlg.Invalidate();
}

void CDialogPaneStatsview::OnPressCloseButton()
{
	CDockablePane::OnPressCloseButton();
	bIsClosed = TRUE;
}


void CDialogPaneStatsview::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	return;
}


void CStatisticsView::OnBnClickedButtonSummaryList()
{
	// TODO: Add your control notification handler code here
	UpdateTotalContents();
	m_DefectList.ShowWindow(FALSE);
	m_SummaryList.ShowWindow(TRUE);
	m_ListStatsInfo.ShowWindow(FALSE);
	m_listTotalInfo.ShowWindow(FALSE);
}


void CStatisticsView::OnBnClickedButtonDefectList()
{
	// TODO: Add your control notification handler code here
	UpdateContents();
	m_DefectList.ShowWindow(TRUE);
	m_SummaryList.ShowWindow(FALSE);
	m_ListStatsInfo.ShowWindow(FALSE);
	m_listTotalInfo.ShowWindow(FALSE);

}


void CStatisticsView::OnBnClickedButtonStatsInfo()
{
	// TODO: Add your control notification handler code here
	m_DefectList.ShowWindow(FALSE);
	m_SummaryList.ShowWindow(FALSE);
	m_ListStatsInfo.ShowWindow(TRUE);
	m_listTotalInfo.ShowWindow(FALSE);
}



void CStatisticsView::LoadLotInfo()
{
	 
	CApplication* pApp = (CApplication*)AfxGetApp();
	LOTSUMMARY* pLotSummary = &((CApplication*)AfxGetApp())->m_LotSummary;
	CString strLotFile;
	CString strTemp;
	if (pApp->m_bLotOpened) {
		
		if (!pApp->m_strLotInfo.IsEmpty()) {
			strTemp.Format("%s\\%s\\%s", pApp->m_LotSummaryRootDir, pApp->m_strLotConfigFile, pApp->m_strLotInfo);
			strLotFile = pApp->m_strLotInfo;
		}
		else
		{
			strTemp.Format("%s\\DUMMY LOT", pApp->m_LotSummaryRootDir);
			strLotFile = "DUMMY LOT";
		}
		strLotFile = strTemp + "\\" + strLotFile + ".csv";
		OutputDebugLog("LoadLotInfo %s", strLotFile);
	}	
	else
		return;

	CAtlString resToken;
	CStringArray respone;
	int curPos = 0;
	int nLineCount = 0;
	CString strLine;
	char c[5000];
	int nDevice = -1;
	int nPosition = -1;
	int nView = -1;
	std::ifstream file(strLotFile);
	if (!file.good())
		return;
	while (!file.eof()) {
		
		file.getline(c,5000);
		strLine = c;
		nLineCount = 0;
		curPos = 0;
		respone.RemoveAll();
		do
		{
			resToken = strLine.Tokenize(_T(","), curPos);
			respone.Add((CString)resToken);
			nLineCount++;
		} while (resToken != "");

		//for (int i = 0; i < nLineCount; i++)
		{
			if (!respone[0].Find("THA", 0))
			{
				int nLength = respone[0].GetLength();
				char cDevice = respone[0].GetAt(nLength - 1);
				CString strDevice;
				strDevice.Format("%c", cDevice);
				nDevice = atoi(strDevice);
				/*if (nDevice == 24)
				{
					OutputDebugLogTo(9, TRUE, "BEFORE nDevice = %d ", nDevice);
				}
				OutputDebugLogTo(9, TRUE, "BEFORE nDevice = %d ", nDevice);*/
				nDevice = nDevice - 1;
				nPosition = -1;
			//	OutputDebugLogTo(9, TRUE, "nDevice = %d cDevice = %c", nDevice, cDevice);
			}
			if (!respone[0].Find("Position", 0))
			{
				int nLength = respone[0].GetLength();
				char cDevice = respone[0].GetAt(nLength - 1);
				CString strDevice;
				strDevice.Format("%c", cDevice);
				nPosition = atoi(strDevice);
				nPosition = nPosition - 1;
				nView = -1;;
			}
			/*for (int nTop = 0; nTop < pApp->m_nTotalTracks; nTop++) {
				if (!respone[0].Compare(pApp->m_strTopCameraName[nTop])!=0)
				{
					nView = nTop ;
				}
			}*/
			for (int nTop = 0; nTop < pApp->m_nTotalViewDocs[STATION_TOP]; nTop++) {
				if (!respone[0].Compare(pApp->m_strTopCameraName[nTop]) != 0)
				{
					nView = nTop;
				}
			}

			for (int nTop = 0; nTop < pApp->m_nTotalViewDocs[STATION_THR]; nTop++) {
				if (!respone[0].Compare(pApp->m_strTHRCameraName[nTop]) != 0)
				{
					nView = nTop + pApp->m_nTotalViewDocs[STATION_TOP];
				}
			}
			
			/*for (int nTop = 0; nTop < pApp->m_nTotalViewDocs[1]; nTop++) {
				if (!respone[0].Compare(pApp->m_strBottomCameraName[nTop]) != 0)
				{
					nView = nTop + pApp->m_nTotalViewDocs[0];
				}
			}
			for (int nTop = 0; nTop < pApp->m_nTotalViewDocs[2]; nTop++) {
				if (!respone[0].Compare(pApp->m_strSideCameraName[nTop]) != 0)
				{
					nView = nTop + pApp->m_nTotalViewDocs[0] + pApp->m_nTotalViewDocs[1];
				}
			}*/
			int nData = 1;
			if (nDevice != -1 && nPosition != -1 && nView != -1) {
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lTotalUnitsInspected = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lTotalUnitsPass = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].dTotalYield = atof(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lTotalUnitsFail = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lUserModified = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lDieLocation = atoi(respone[nData++]);
				
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lEncapInsufficient = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lEncapExcess = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lEncapCrack = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lEncapBlowHole = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lEncapContamination = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lEncapVoid = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lEncapWireAndIC = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lEncapBulge = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lEncapGlue = atoi(respone[nData++]);
				// Contact Error
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lContactDamage = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lContactContamination = atoi(respone[nData++]);
				// Flex Error
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lFlexOvercut = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lFlexDamage = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lFlexContamination = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lFlexScratches = atoi(respone[nData++]);

				// Chicklet Swage Hole Error
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lChickletRoundness = atoi(respone[nData++]);
				pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lChickletBurr = atoi(respone[nData++]);


				while (1) {
					if (atoi(respone[nData])!= -1)
						break;
					else
						nData++;
				}
				for (int x = 0; x < pApp->m_nPviNumberOfArea[nPosition][nView]; x++) {
					for (int y = 0; y < pApp->m_nPviNoOfDefects[nPosition][nView][x]; y++) {
						pApp->m_LotSummary.m_DisplaySummary[nDevice][nPosition][nView].lPviDefectCounter[x][y] = atoi(respone[nData++]);
					}
				}
				nView = -1;
			}
		}
	}
	respone.RemoveAll();
}


void CStatisticsView::UpdateStatsInfo()
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	int j = 0;
	CString strTempVal;
	for (int i = 0; i < pApp->m_nNoOfDies; i++,j++) {
		m_ListStatsInfo.SetItemText(j, 1, (LPCTSTR)pApp->m_strPartFlex[i]);
	}	
	for (int i = 0; i < pApp->m_nNoOfDies; i++, j++) {
		if (pApp->m_bPartInspect[i])
			strTempVal = "1-Inspect";
		else
			strTempVal = "0-No Inspect";
		m_ListStatsInfo.SetItemText(j, 1, strTempVal);
	}

	m_ListStatsInfo.SetItemText(j++, 1, (LPCTSTR)pApp->m_strLotInfo);
	m_ListStatsInfo.SetItemText(j++, 1, (LPCTSTR)pApp->m_strConfigFile);
	strTempVal.Format("%d", pApp->m_nPPM);
	m_ListStatsInfo.SetItemText(j++, 1, (LPCTSTR)strTempVal);
	strTempVal.Format("%d", pApp->m_nUPH);
	m_ListStatsInfo.SetItemText(j++, 1, strTempVal);
}


void CStatisticsView::OnCbnSelchangeComboDeviceList()
{
	// TODO: Add your control notification handler code here
	UpdateContents(TRUE);
}


void CStatisticsView::OnBnClickedButtonTotalInfo()
{
	// TODO: Add your control notification handler code here
	UpdateTotalContents();
	m_SummaryList.ShowWindow(FALSE);
	m_DefectList.ShowWindow(FALSE);
	m_ListStatsInfo.ShowWindow(FALSE);
	m_listTotalInfo.ShowWindow(TRUE);
}


void CStatisticsView::OnCbnSelchangeComboPositionList()
{
	// TODO: Add your control notification handler code her
	UpdateContents(TRUE);
}


void CStatisticsView::UpdateTotalLotInfo()
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	LOTSUMMARY* pLotSummary = &((CApplication*)AfxGetApp())->m_LotSummary;


	int nBottom = 0;
	int nSide = 0;
	CArray<int> nTotalDefectCount[5];
	CArray<int> nTotalDefectCountTotal;
	int nDefects = 25;

	/*for (int i = 0; i < 5; i++) {
		nTotalDefectCount[i] = new int[pApp->m_nNo_of_Defects];
	}*/
	//nTotalDefectCountTotal = new int[pApp->m_nNo_of_Defects];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < pApp->m_nNo_of_Defects; j++) {
			nTotalDefectCount[i].Add(0);
			nTotalDefectCountTotal.Add(0);
		}
	}

	CString strLotFile;

	CString strTemp;
	if (!pApp->m_strLotInfo.IsEmpty()) {
		strTemp.Format("%s\\%s", pApp->m_LotSummaryRootDir, pApp->m_strLotConfigFile);
		_mkdir(strTemp);
		strLotFile = pApp->m_strLotInfo;
	}
	else
	{
		strTemp.Format("%s\\DUMMY LOT CONFIG", pApp->m_LotSummaryRootDir);
		_mkdir(strTemp);
		strLotFile = "DUMMY LOT";
	}
	CString strTempDir;
	strTempDir.Format("%s\\%s", strTemp, strLotFile);
	_mkdir(strTempDir);

	strLotFile = strTempDir + "\\" + strLotFile + "_Total.csv";
	FILE* file;
	errno_t err = fopen_s(&file, strLotFile, "w");
	if (err || file == NULL)
		return;

	fprintf(file, "Lot ID,,[%s]\n", pApp->m_strLotInfo);
	fprintf(file, "Operator ID,,%s\n", pApp->m_UserAccInfo.m_strCurUserName);
	fprintf(file, "Recipe Name,,%s\n", pApp->m_strLotConfigFile);
	fprintf(file, "System ID,,%s\n", pLotSummary->strMachineID);
	fprintf(file, "Start Time,,[%s]\n", pLotSummary->strStartLotTime);
	fprintf(file, "End Time,,[%s]\n\n\n", pLotSummary->strEndLotTime);
	//fprintf(file, "Comments,,%s\n", pLotSummary->strComments);
	/*for (int k = 0; k < pApp->m_nNoOfDies; k++)
	{
		strTemp.Format(",,,Total Inspected,Total Pass,Total Yield,Total Fail,User Modified,Die Location,Datum Location,Tilt Inspection,Cover Layer Inspection,Cover Layer Alignment,Encap Insufficient,Encap Excess,Encap BlowHole,Encap Contamination,Die Edge");
		for (int i = 0; i < pApp->m_nNoOfFov; i++)
		{
			int nCount = pApp->m_strPviDefectsStats[i].GetCount();
			for (int nTempIndex = 0; nTempIndex < nCount; nTempIndex++) {
				strTemp.Format("%s,%s", strTemp, pApp->m_strPviDefectsStats[i].GetAt(nTempIndex));
			}
		}
	}*/
	fprintf(file, strTemp);
	for (int k = 0; k < pApp->m_nNoOfDies; k++)
	{
		fprintf(file, "\n,,THA %d", k + 1);
		int nDefectIndexTemp = 0;
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++,pApp->m_LotSummary.m_TotalDisplaySummary[k].lTotalUnitsInspected);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++,pApp->m_LotSummary.m_TotalDisplaySummary[k].lTotalUnitsPass);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++,pApp->m_LotSummary.m_TotalDisplaySummary[k].dTotalYield);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++,pApp->m_LotSummary.m_TotalDisplaySummary[k].lTotalUnitsFail);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++,pApp->m_LotSummary.m_TotalDisplaySummary[k].lUserModified);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++,pApp->m_LotSummary.m_TotalDisplaySummary[k].lDieLocation);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++,pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapInsufficient);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++,pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapExcess);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapCrack);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++,pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapBlowHole);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++,pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapContamination);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapVoid);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapWireAndIC);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapBulge);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapGlue);
		// Contact Error
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lContactDamage);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lContactContamination);
		// Flex Error
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lFlexOvercut);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lFlexDamage);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lFlexContamination);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lFlexScratches);

		// Chicklet Swage Hole Error
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lChickletRoundness);
		nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lChickletBurr);

		/*for (int j = 0; j < pApp->m_nTotalViews; j++) {
			for (int i = 0; i < pApp->m_nNoOfFov; i++)
			{
				for (int x = 0; x < pApp->m_nPviNumberOfArea[i][j]; x++) {
					for (int y = 0; y < pApp->m_nPviNoOfDefects[i][j][x]; y++) {
						nTotalDefectCount[k].SetAt(nDefectIndexTemp++, pApp->m_LotSummary.m_TotalDisplaySummary[k].lTotalPviDefectCounter[i][j][x][y]);
					}
				}
			}
		}*/

		if(pApp->m_LotSummary.m_mapIndividualPviDefectCounter[k].size() > 0) {
			for(std::map<int, long>::iterator it = pApp->m_LotSummary.m_mapIndividualPviDefectCounter[k].begin();
			it != pApp->m_LotSummary.m_mapIndividualPviDefectCounter[k].end(); it++) {
				nTotalDefectCount[k].SetAt(nDefectIndexTemp++, it->second);
			}
		}
		

		for (int nTempIndex = 0; nTempIndex < nDefectIndexTemp; nTempIndex++) {
			fprintf(file, ",%9d", nTotalDefectCount[k].GetAt(nTempIndex));
		}
	}
	fprintf(file, "\n,,Total");
	int nDefectIndexTemp = 0;
	for (int k = 0; k < pApp->m_nNoOfDies; k++)
	{
		nDefectIndexTemp = 0;
		int nTemp = 0;
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lTotalUnitsInspected);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lTotalUnitsPass);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].dTotalYield);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lTotalUnitsFail);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lUserModified);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lDieLocation);
		
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapInsufficient);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapExcess);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapCrack);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapBlowHole);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapContamination);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapVoid);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapWireAndIC);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapBulge);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lEncapGlue);
		// Contact Error
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lContactDamage);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lContactContamination);
		// Flex Error
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lFlexOvercut);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lFlexDamage);		
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lFlexContamination);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lFlexScratches);

		// Chicklet Swage Hole Errror
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lChickletRoundness);
		nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
		nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lChickletBurr);
		/*for (int j = 0; j < pApp->m_nTotalViews; j++) {
			for (int i = 0; i < pApp->m_nNoOfFov; i++)
			{
				for (int x = 0; x < pApp->m_nPviNumberOfArea[i][j]; x++) {
					for (int y = 0; y < pApp->m_nPviNoOfDefects[i][j][x]; y++) {
						nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
						nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + pApp->m_LotSummary.m_TotalDisplaySummary[k].lTotalPviDefectCounter[i][j][x][y]);
					}
				}
			}
		}*/

		if(pApp->m_LotSummary.m_mapIndividualPviDefectCounter[k].size() > 0) {
			for(std::map<int, long>::iterator it = pApp->m_LotSummary.m_mapIndividualPviDefectCounter[k].begin();
			it != pApp->m_LotSummary.m_mapIndividualPviDefectCounter[k].end(); it++) {
				nTemp = nTotalDefectCountTotal.GetAt(nDefectIndexTemp);
				nTotalDefectCountTotal.SetAt(nDefectIndexTemp++, nTemp + it->second);
			}
		}
		
	}
	for (int nTempIndex = 0; nTempIndex < nDefectIndexTemp; nTempIndex++) {
		fprintf(file, ",%9d", nTotalDefectCountTotal[nTempIndex]);
	}
	fclose(file);

	for (int i = 0; i < 5; i++) {
		nTotalDefectCount[i].RemoveAll();
	}
	nTotalDefectCountTotal.RemoveAll();
}



void CStatisticsView::LoadTotalLotInfo()
{
	CApplication* pApp = (CApplication*)AfxGetApp();
	LOTSUMMARY* pLotSummary = &((CApplication*)AfxGetApp())->m_LotSummary;
	CString strLotFile;
	CString strTemp;
	if (pApp->m_bLotOpened) {
		if (!pApp->m_strLotInfo.IsEmpty()) {
			strTemp.Format("%s\\%s\\%s", pApp->m_LotSummaryRootDir, pApp->m_strLotConfigFile, pApp->m_strLotInfo);
			strLotFile = pApp->m_strLotInfo;
		}
		else
		{
			strTemp.Format("%s\\DUMMY LOT", pApp->m_LotSummaryRootDir);
			strLotFile = "DUMMY LOT";
		}
		strLotFile = strTemp + "\\" + strLotFile + "_Total.csv";
		OutputDebugLog("LoadLotInfo %s", strLotFile);
	}
	else
		return;
	CAtlString resToken;
	CStringArray respone;
	int curPos = 0;
	int nLineCount = 0;
	CString strLine;
	char c[5000];
	int nDevice = -1;
	std::ifstream file(strLotFile);
	if (!file.good())
		return;

	while (!file.eof()) {
		file.getline(c, 5000);
		strLine = c;
		nLineCount = 0;
		curPos = 0;
		respone.RemoveAll();
		do
		{
			resToken = strLine.Tokenize(_T(","), curPos);
			respone.Add((CString)resToken);
			nLineCount++;
		} while (resToken != "");
		{
			if (!respone[0].Find("THA", 0))
			{
				int nLength = respone[0].GetLength();
				char cDevice = respone[0].GetAt(nLength - 1);
				CString strDevice;
				strDevice.Format("%c", cDevice);
				nDevice = atoi(strDevice);
				nDevice = nDevice - 1;
			}
			int nData = 1;
			if (nDevice != -1) {
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lTotalUnitsInspected = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lTotalUnitsPass = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].dTotalYield = atof(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lTotalUnitsFail = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lUserModified = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lDieLocation = atoi(respone[nData++]);
				
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lEncapInsufficient = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lEncapExcess = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lEncapCrack = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lEncapBlowHole = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lEncapContamination = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lEncapVoid = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lEncapWireAndIC = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lEncapBulge = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lEncapGlue = atoi(respone[nData++]);
				// Contact Error
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lContactDamage = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lContactContamination = atoi(respone[nData++]);
				// Flex Error
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lFlexOvercut = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lFlexDamage = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lFlexContamination = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lFlexScratches = atoi(respone[nData++]);

				
				// Chicklet Swage Hole Error 
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lChickletRoundness = atoi(respone[nData++]);
				pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lChickletBurr = atoi(respone[nData++]);
				/*for (int j = 0; j < pApp->m_nTotalViews; j++) {
					for (int i = 0; i < pApp->m_nNoOfFov; i++)
					{
						for (int x = 0; x < pApp->m_nPviNumberOfArea[i][j]; x++) {
							for (int y = 0; y < pApp->m_nPviNoOfDefects[i][j][x]; y++) {
								pApp->m_LotSummary.m_TotalDisplaySummary[nDevice].lTotalPviDefectCounter[i][j][x][y] = atoi(respone[nData++]);
							}
						}
					}
				}*/

				if(pApp->m_LotSummary.m_mapIndividualPviDefectCounter[nDevice].size() > 0) {
					for(std::map<int, long>::iterator it = pApp->m_LotSummary.m_mapIndividualPviDefectCounter[nDevice].begin();
					it != pApp->m_LotSummary.m_mapIndividualPviDefectCounter[nDevice].end(); it++) {
						it->second = atoi(respone[nData++]);
					}
				}
				
				nDevice = -1;
			}
		}
	}
	respone.RemoveAll();
}

void CStatisticsView::OnBnClickedButtonRefresh()
{
	if (AfxMessageBox("Confirm to Reset Statistical Counters", MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;

	CApplication* pApp = (CApplication*)AfxGetApp();
	nPPMStripNo = 0;
	nPPMNetStripNo = 0;
	dAverageStripTimeMs = 0;
	dNetAverageStripTime = 0;

	nTotalBarNo=0;
	nGoodBarNo=0;
	nBadBarNo=0;
	nTotalDieNo=0;
	nGoodDieNo=0;
	nBadDieNo=0;

	pApp->OnOnlineResetstatscounter();
}

void CStatisticsView::UpdateUPH(double dStripTime, double dNetStripTime, int nDieNo)
{
	if (nDieNo < 1) return;

	EnterCriticalSection(&m_CriticalSectionUpdate);

	dAverageStripTimeMs = (dAverageStripTimeMs * nPPMStripNo + dStripTime) / (nPPMStripNo + 1);
	nPPMStripNo++;

	dNetAverageStripTime = (dNetAverageStripTime * nPPMNetStripNo + dNetStripTime) / (nPPMNetStripNo + 1);
	nPPMNetStripNo++;

	UpdateTotalContents();

	LeaveCriticalSection(&m_CriticalSectionUpdate);
}

void CStatisticsView::UpdatePrintBar(size_t BadDevicecount)
{
	nTotalBarNo++;

	if(BadDevicecount > 0)
		nBadBarNo++;
	else
		nGoodBarNo++;
}

void CStatisticsView::UpdateStatistics(size_t GoodDieCount, size_t  BadDieCount)
{

	nGoodDieNo += GoodDieCount;
	nBadDieNo +=	BadDieCount;
	nTotalDieNo = nGoodDieNo + nBadDieNo;
}

void CStatisticsView::UpdatePrintbarOnReScan(size_t BadDevicecount)
{
	if (BadDevicecount <= 0)
		return;

	nTotalBarNo--;
	nBadBarNo--;
}

void CStatisticsView::UpdateStatisticsOnReScan(size_t GoodDieCount, size_t  BadDieCount)
{
	if (BadDieCount <= 0)
		return;

	nBadDieNo -= BadDieCount;
	nGoodDieNo -= GoodDieCount;

	nTotalDieNo = nGoodDieNo + nBadDieNo;
}

void CStatisticsView::UpdatePrintbarOnRepeatScan(size_t BadDevicecount)
{
	nTotalBarNo--;

	if (BadDevicecount > 0) {
		nBadBarNo--;
		if (nBadBarNo < 0)
			nBadBarNo = 0;
	}
	else {
		nGoodBarNo--;
		if (nGoodBarNo < 0)
			nGoodBarNo = 0;
	}

	if (nTotalBarNo != (nBadBarNo + nGoodBarNo))
		nTotalBarNo = nBadBarNo + nGoodBarNo;
}

void CStatisticsView::UpdateStatisticsOnRepeatScan(size_t GoodDieCount, size_t  BadDieCount)
{
	nBadDieNo -= BadDieCount;
	if (nBadDieNo < 0)
		nBadDieNo = 0;

	nGoodDieNo -= GoodDieCount;
	if (nGoodDieNo < 0)
		nGoodDieNo = 0;

	nTotalDieNo = nGoodDieNo + nBadDieNo;

}