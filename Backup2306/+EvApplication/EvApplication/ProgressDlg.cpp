// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"
#include "Application.h"


// CProgressDlg dialog

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
	, m_strStatus(_T(""))
{
	m_strStatus = "";
	m_nGlass    = 95;
	m_nCurrentPos = 0;
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUS1, m_strStatus);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_BAR, ctrProgress);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



BOOL CProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_brush.CreateSolidBrush(RGB(10, 10, 10));

	SLWA pSetLayeredWindowAttributes = NULL;  
	HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL"); 
	pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	pSetLayeredWindowAttributes(m_hWnd, RGB(255, 0, 255), (255 * m_nGlass) / 100, LWA_COLORKEY | LWA_ALPHA);

	ctrProgress.SetBkColor(RGB(255, 255, 255));
	ctrProgress.SetBarColor(RGB(94, 136, 158));

	ctrProgress.SetRange32(0,100);
	return TRUE;
}

void CProgressDlg::SetStatus(CString strStatus, int nPos)
{
	ctrProgress.SetPos(nPos);
	m_strStatus.Format("%s %d%%", strStatus, nPos);
	
	if (nPos >= 0 && nPos <= 100)
		m_nCurrentPos = nPos;
	else
		m_nCurrentPos = 0;

	UpdateData(FALSE);
}

int CProgressDlg::GetCurrentLoad()
{
	return ctrProgress.GetPos();
}

// CProgressDlg message handlers

HBRUSH CProgressDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT )//nCtlColor)
{
	/*HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;*/

	int id = pWnd->GetDlgCtrlID();
	if( id == IDC_STATIC_STATUS1 ||
		id == IDC_STATIC)
    {
        pDC->SetTextColor( RGB(255, 255, 255));
        pDC->SetBkMode( TRANSPARENT );	
		
    }
	if( id == IDC_STATIC_PROGRESS_BAR )
    {
        pDC->SetTextColor( RGB(255, 255, 255));
        pDC->SetBkColor(RGB(255, 255, 255));
    }
	
	return m_brush;
}

void CProgressDlg::OnOK()
{
}

void CProgressDlg::OnCancel()
{
}