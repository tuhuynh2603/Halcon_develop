// ThresholdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MsgCode.h"
#include "ThresholdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThresholdDialog dialog


CThresholdDialog::CThresholdDialog(CWnd* pParent /*=NULL*/)
	//: CWnd(CThresholdDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThresholdDialog)
	//}}AFX_DATA_INIT
	m_nThresholdValue = 128;

	//m_BrushActive.CreateSolidBrush(RGB(130, 160, 210));
	m_BrushActive.CreateSolidBrush(RGB(0, 0, 0));
}

CThresholdDialog::~CThresholdDialog()
{
	m_BrushActive.DeleteObject();
}

void CThresholdDialog::CreateNew(CView* pView)
{
	m_pView = pView;
	Create(NULL, "", WS_CHILD | WS_VISIBLE, CRect(0, 0, 210, 25), 
		pView->GetParent()->GetParent()->GetParent(), IDD_THRESHOLD_DLG);

	m_sliderBinarize.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 180, 25), this, IDC_THRES_BINARIZE_SLIDER);
	m_sliderBinarize.SetScrollRange(0,255);
	m_sliderBinarize.SetScrollPos(m_nThresholdValue);

	char str[4];
	sprintf_s(str, "%d", m_nThresholdValue);
	m_staticBinarize.Create(str, WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, CRect(180, 2, 210, 23), this, IDC_THRES_BINARIZE_STATIC);
	//m_staticBinarize.SetFont(&m_font);
}

void CThresholdDialog::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThresholdDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThresholdDialog, CWnd)
	//{{AFX_MSG_MAP(CThresholdDialog)
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThresholdDialog message handlers

BOOL CThresholdDialog::OnInitDialog() 
{
	//CWnd::OnInitDialog();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThresholdDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch (nSBCode)
	{
		case SB_LEFT:
			m_nThresholdValue=0;
			break;

		case SB_RIGHT:   
			m_nThresholdValue=255;
			break;

		case SB_PAGELEFT:
			m_nThresholdValue-=4;
			if (m_nThresholdValue<0)
				m_nThresholdValue=0;
			break;           

		case SB_PAGERIGHT:
			m_nThresholdValue+=4;
			if (m_nThresholdValue>255)
				m_nThresholdValue=255;
			break;

		case SB_THUMBPOSITION :
			break;

		case SB_THUMBTRACK :
			m_nThresholdValue = nPos;
			if (m_nThresholdValue < 0)
				m_nThresholdValue = 0;
			if (m_nThresholdValue>255)
				m_nThresholdValue=255;
			break;

		case SB_LINELEFT:
			m_nThresholdValue--;
			if (m_nThresholdValue < 0)
				m_nThresholdValue = 0;
			break;

		case SB_LINERIGHT:
			m_nThresholdValue++;
			if (m_nThresholdValue > 255)
				m_nThresholdValue = 255;
			break;
	}

	m_sliderBinarize.SetScrollPos(m_nThresholdValue);

	char str[4];
	sprintf_s(str, "%d", m_nThresholdValue);
	m_staticBinarize.SetWindowTextA(str);

	m_pView->SendMessage(WM_SET_THRESHOLD, (WPARAM) m_nThresholdValue);
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

	//m_pView->SetActiveWindow();
	//m_pView->SetFocus();	
}

void CThresholdDialog::IncThresholdValue(int nStep)
{
	int nPos = GetScrollPos(SB_HORZ);
	CScrollBar* pScrollBar = GetScrollBarCtrl(SB_HORZ);	
	if (nStep==0)
		OnHScroll(SB_LINERIGHT,nPos,pScrollBar);
	else
		OnHScroll(SB_PAGERIGHT,nPos,pScrollBar);
}

void CThresholdDialog::DecThresholdValue(int nStep)
{
	int nPos = GetScrollPos(SB_HORZ);
	CScrollBar* pScrollBar = GetScrollBarCtrl(SB_HORZ);	
	if (nStep==0)
		OnHScroll(SB_LINELEFT,nPos,pScrollBar);
	else
		OnHScroll(SB_PAGELEFT,nPos,pScrollBar);

}

HBRUSH CThresholdDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

    pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));
	//LOGBRUSH LogBrush;
	//m_BrushActive.GetLogBrush(&LogBrush);
	//pDC->SetBkColor(LogBrush.lbColor);

	return m_BrushActive;
}
