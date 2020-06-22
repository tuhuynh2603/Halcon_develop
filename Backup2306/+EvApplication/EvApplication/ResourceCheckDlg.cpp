//
#include "stdafx.h"
#include "ResourceCheckDlg.h"
#include "afxdialogex.h"
#include "Application.h"

// CResourceCheckDlg dialog


IMPLEMENT_DYNAMIC(CResourceCheckDlg, CDialog)

CResourceCheckDlg::CResourceCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResourceCheckDlg::IDD, pParent)
{
	m_strDiskSpace = "";
	m_strMemSpace = "";
	m_strSimulationStatus = "";
	m_bDiskOk = TRUE;
	m_bMemoryOk = TRUE;
	m_nInterval = 10;
	m_nStepUnit = 1;
	m_nGlass = 75;

	bFirstTimePositioning = TRUE;
}

CResourceCheckDlg::~CResourceCheckDlg()
{
	m_brush.DeleteObject();
}

void CResourceCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIMULATION_DISK_STATIC, m_strDiskSpace);
	DDX_Text(pDX, IDC_SIMULATION_RAM_STATIC, m_strMemSpace);
	DDX_Text(pDX, IDC_SIMULATION_STATUS_STATIC, m_strSimulationStatus);
}

BEGIN_MESSAGE_MAP(CResourceCheckDlg, CDialog)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SETSMSTATUS, OnSetSMStatus)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_SETFOCUS()
	ON_STN_CLICKED(IDC_SIMULATION_DISK_STATIC, &CResourceCheckDlg::OnStnClickedStatic)
	ON_STN_CLICKED(IDC_STATIC_AVAILABLE_SPACE, &CResourceCheckDlg::OnStnClickedStatic)
	ON_STN_CLICKED(IDC_SIMULATION_RAM_STATIC, &CResourceCheckDlg::OnStnClickedStatic)
	ON_STN_CLICKED(IDC_SIMULATION_STATUS_STATIC, &CResourceCheckDlg::OnStnClickedStatic)
	ON_WM_LBUTTONDOWN()
	ON_STN_DBLCLK(IDC_SIMULATION_DISK_STATIC, &CResourceCheckDlg::OnStnClickedStatic)
	ON_STN_DBLCLK(IDC_STATIC_AVAILABLE_SPACE, &CResourceCheckDlg::OnStnClickedStatic)
	ON_STN_DBLCLK(IDC_SIMULATION_RAM_STATIC, &CResourceCheckDlg::OnStnClickedStatic)
	ON_STN_DBLCLK(IDC_SIMULATION_STATUS_STATIC, &CResourceCheckDlg::OnStnClickedStatic)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CResourceCheckDlg message handlers

BOOL CResourceCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_brush.CreateSolidBrush(RGB(30, 30, 30));

	SLWA pSetLayeredWindowAttributes = NULL;
	HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL");
	pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32, "SetLayeredWindowAttributes");
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(m_hWnd, RGB(255, 0, 255), (255 * m_nGlass) / 100, LWA_COLORKEY | LWA_ALPHA);

	(m_nGlass >= 70) ? ReleaseTimer() : m_nGlass += m_nStepUnit;

	m_nCheckTimer = SetTimer(1, 1000, 0);
	//	m_strSimulationStatus = "*** Loading images to memory for SIMULATION... ***";
	//	UpdateData(FALSE);

	SetWindowPosition();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CResourceCheckDlg::SetWindowPosition()
{
	if (IsWindow(m_hWnd)) {

		if (bFirstTimePositioning) {
			CRect rect1, rect2;
			theApp.m_pMainWnd->GetWindowRect(&rect1);
			GetWindowRect(&rect2);

			SetWindowPos(NULL, rect1.Width() * 2 / 3, rect1.Height() - rect2.Height() * 2, 0, 0, SWP_NOSIZE);

			bFirstTimePositioning = FALSE;
		}
		else {
			CRect rcPosition;

			GetWindowRect(&rcPosition);

			SetWindowPos(NULL, rcPosition.left, rcPosition.top, 0, 0, SWP_NOSIZE);
		}
	}
	theApp.m_pMainWnd->SetFocus();
}

LRESULT CResourceCheckDlg::OnSetSMStatus(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);

	return 0;
}

void CResourceCheckDlg::CheckResourceAvailability(bool bForced)
{
	MEMORYSTATUSEX statEx;
	statEx.dwLength = sizeof(statEx);

	__int64 availableBytes, totalBytes, freeBytes;
	CString strDir = theApp.m_LotSummaryRootDir.Left(theApp.m_LotSummaryRootDir.Find(':') + 1) + "\\";
	char* strLogDir = strDir.GetBuffer(0);

	BOOL success = GlobalMemoryStatusEx(&statEx);

	if (success) {

		__int64 availApprox = ((__int64)statEx.ullAvailPhys) >> 20;
		double availableGb = availApprox / 1024.0;
		m_strMemSpace.Format("Memory: %5.1f GB", availableGb);

		if (availApprox < 512 || (bForced && availApprox <= 768)) {
			if (m_bMemoryOk || bForced) {
				m_bMemoryOk = FALSE;
				//if(theApp.m_bSaveAllImages) {
				//	m_strSimulationStatus = "*** Low Memory; Fast-Save BYPASSED ***";
				//	OutputDebugLogTo(9, TRUE, "************* Low Memory (%4.1fGB); Not saving image to disk   *************", availableGb);
				//}
				//else
				m_strSimulationStatus = "Low Memory";
			}
		}
		else if (availApprox > 768) {
			if (!m_bMemoryOk || bForced) {
				m_bMemoryOk = TRUE;
				//if(theApp.m_bSaveAllImages) {
				//	m_strSimulationStatus = "Fast-Save Enabled; Saving all images to disk";
				//	OutputDebugLogTo(9, TRUE, "************* Memory Ok (%4.1fGB); Resumed saving", availableGb);
				//}
				//else
				m_strSimulationStatus = "";
			}
		}
	}
	else {
		m_bMemoryOk = FALSE;
		//if(theApp.m_bSaveAllImages) {
		//	m_strSimulationStatus = "Error in checking memory; Fast-Save disabled";
		//	OutputDebugLogTo(9, TRUE, "************* Error in checking for available memory");
		//}
	}

	success = GetDiskFreeSpaceEx(strLogDir, (PULARGE_INTEGER)&availableBytes, (PULARGE_INTEGER)&totalBytes, (PULARGE_INTEGER)&freeBytes);
	if (success) {

		double availableGb = (availableBytes >> 20) / 1024.0;
		m_strDiskSpace.Format("Drive %s %5.1f GB", strDir, availableGb);


		if (availableBytes * 100 / totalBytes < 5) {
			if (m_bDiskOk || bForced) {
				m_bDiskOk = FALSE;
				//if(theApp.m_bSaveAllImages) {
				//	m_strSimulationStatus = "*** Low Disk Space; Fast-Save BYPASSED ***";
				//	OutputDebugLogTo(9, TRUE, "************* [%s] Low Disk Space (%4.1fGB)", strLogDir, availableGb);
				//}
				//else
				m_strSimulationStatus = "Low Disk Space";
			}
		}
		else {
			if (!m_bDiskOk || bForced) {
				m_bDiskOk = TRUE;
				///if(theApp.m_bSaveAllImages) {
				///	m_strSimulationStatus = "Fast-Save Enabled; Saving all images to disk";
				///	OutputDebugLogTo(9, TRUE, "************* [%s] Disk Space Ok (%4.1fGB)", strLogDir, availableGb);
				///}
				///else
				m_strSimulationStatus = "";
			}
		}
	}
	else {
		m_bDiskOk = FALSE;
		m_strSimulationStatus = "Error in checking disk; Fast-Save disabled";
		OutputDebugLogTo(9, TRUE, "************* [%s] Error in checking for available Disk Space", strLogDir);
	}

}

void CResourceCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nCheckTimer) {
		CheckResourceAvailability();
		UpdateData(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

HBRUSH CResourceCheckDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//For changing the Edit Font Color------

	if (pWnd->GetDlgCtrlID() == IDC_SIMULATION_DISK_STATIC)
	{
		// Set color as red.
		//pDC->SetBkColor( TRANSPARENT );
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);

	}
	if (pWnd->GetDlgCtrlID() == IDC_SIMULATION_RAM_STATIC)
	{
		// Set color as red.
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_AVAILABLE_SPACE)
	{
		// Set color as red.
		//pDC->SetBkColor( TRANSPARENT );
		pDC->SetTextColor(RGB(255, 147, 25));
		pDC->SetBkMode(TRANSPARENT);

	}
	if (pWnd->GetDlgCtrlID() == IDC_SIMULATION_STATUS_STATIC)
	{
		// Set color as red.
		pDC->SetTextColor(RGB(200, 210, 250));
		pDC->SetBkMode(TRANSPARENT);
	}

	//----------------------------------------
	//pDC->SetBkMode( TRANSPARENT );

	return m_brush;
}

void CResourceCheckDlg::OnOK()
{
	theApp.m_pMainWnd->SetFocus();
	//return;
	//
	//CDialog::OnOK();
}

void CResourceCheckDlg::OnCancel()
{
	theApp.m_pMainWnd->SetFocus();
	//return;
	//
	//CDialog::OnCancel();
}

void CResourceCheckDlg::ReleaseTimer()
{
	KillTimer(IDT_TIMER);
}

void CResourceCheckDlg::OnClose()
{
	KillTimer(m_nCheckTimer);

	CDialog::OnClose();
}

void CResourceCheckDlg::OnSetFocus(CWnd* pOldWnd)
{
	//CDialog::OnSetFocus(pOldWnd);

	//theApp.m_pMainWnd->SetFocus();
}

void CResourceCheckDlg::OnStnClickedStatic()
{
	theApp.m_pMainWnd->SetFocus();
}

void CResourceCheckDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//theApp.m_pMainWnd->SetFocus();

	CDialog::OnLButtonDown(nFlags, point);
}

void CResourceCheckDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//theApp.m_pMainWnd->SetFocus();

	CDialog::OnLButtonDblClk(nFlags, point);
}


LRESULT CResourceCheckDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return HTCAPTION;
	//return CDialog::OnNcHitTest(point);
}


void CResourceCheckDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	theApp.m_pMainWnd->SetFocus();

	CDialog::OnLButtonUp(nFlags, point);
}
