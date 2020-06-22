// IOTestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "IOTestDialog.h"
#include "afxdialogex.h"
#include "ApplicationDef.h"

// CIOTestDialog dialog

IMPLEMENT_DYNAMIC(CIOTestDialog, CDialogEx)

CIOTestDialog::CIOTestDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIOTestDialog::IDD, pParent)
{
	m_nNoOfSegments = 1;

	m_pApp = (CApplication*) AfxGetApp();

	m_brushChecked.CreateSolidBrush(RGB(50,250,50));
	m_brushUnchecked.CreateSolidBrush(RGB(220,50,50));
	m_brushBorder.CreateSolidBrush(RGB(0,0,0));
}

CIOTestDialog::~CIOTestDialog()
{
	m_brushChecked.DeleteObject();
	m_brushUnchecked.DeleteObject();
	m_brushBorder.DeleteObject();
	for(int i=0; i<m_nNoOfSegments; i++)
		m_prgnLightSegment[i].DeleteObject();
}

void CIOTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIOTestDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CIOTestDialog::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CIOTestDialog message handlers

BOOL CIOTestDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int nCardNo = 0;
	int nPortNo = 1;
	UINT pDataIn = 0;
	CRect rect;
	for(int i=0; i<m_nNoOfSegments; i++) {
		m_bChecked[i] = FALSE;
		GetDlgItem(IDC_SEGMENT0+i)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		m_prgnLightSegment[i].CreateEllipticRgn(rect.left-2, rect.top-2, rect.right+2, rect.bottom+2);
	}

	CString str;
	if(m_pApp->m_CommModule.hDLL)
		str.Format("I/O Module: %s", m_pApp->m_strIOInterfaceCard);
	else
		str = "No I/O module loaded";

	m_pApp->m_MotorResource.InPortReadBit(nCardNo, nPortNo, IN_MOTOR_CURTAIN_SENSOR, &pDataIn);
	if (pDataIn)
		m_bChecked[0] = TRUE;

	//m_pApp->m_MotorResource.InPortReadBit(nCardNo, nPortNo, OUT_MOTOR_CONTACTOR_CONTROL, &pDataIn);
	//if (pDataIn && OUT_MOTOR_CONTACTOR_CONTROL)
	//	m_bChecked[1] = TRUE;

	GetDlgItem(IDC_IOSTATUS_STATIC)->SetWindowText(str);

	SetTimer(0, 1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIOTestDialog::OnPaint() 
{
	CPaintDC dc(this);

	CDC* pDC = &dc;
	CRect rect;

	for(int i=0; i<m_nNoOfSegments; i++) {
		pDC->FillRgn(&m_prgnLightSegment[i], m_bChecked[i] ? &m_brushChecked : &m_brushUnchecked);
		pDC->FrameRgn(&m_prgnLightSegment[i], &m_brushBorder, 1, 1);
	}
}

void CIOTestDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	/*int nIndex = -1;
	CRect rect;
	for(int i=1; i<8; i++) {
		if(m_prgnLightSegment[i].PtInRegion(point)) {
			nIndex = i;
			break;
		}
	}

	if (nIndex != -1) {
		m_bChecked[nIndex] = !m_bChecked[nIndex];
		int nError = m_pApp->m_MotorResource.OutPortWriteBit(0,1, OUT_MOTOR_CONTACTOR_CONTROL,m_bChecked[nIndex]);
		if (!nError) {
			OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_MOTOR_CONTACTOR_CONTROL);
		}
		CRect rect;
		GetDlgItem(IDC_SEGMENT0 + nIndex)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
	}*/
}


void CIOTestDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int nCardNo = 0;
	int nPortNo = 1;
	UINT pDataIn = 0;
	m_pApp->m_MotorResource.InPortReadBit(nCardNo, nPortNo, IN_MOTOR_CURTAIN_SENSOR, &pDataIn);
	if (pDataIn)
		m_bChecked[0] = TRUE;
	Invalidate();
	CDialogEx::OnTimer(nIDEvent);
}


void CIOTestDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	KillTimer(0);
	CDialogEx::OnOK();
}


void CIOTestDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(0);
	CDialogEx::OnClose();
}
