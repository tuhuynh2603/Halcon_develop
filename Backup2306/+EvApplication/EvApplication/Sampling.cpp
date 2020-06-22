// Sampling.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "afxdialogex.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include "Sampling.h"
#include "MathValidations.h"



// CSampling dialog

IMPLEMENT_DYNAMIC(CSampling, CDialogEx)

CSampling::CSampling(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SAMPLING, pParent)
{
	pMotion = NULL;

	nFovNoPerDieX = 0;
	nFovNoPerDieY = 0;
	nDieNoPerDeviceX = 0;
	nDieNoPerDeviceY = 0;
	nTotalDieNo = 0;
	nTotalFOVNo = 0;

	pnDieState = NULL;
	pnFOVState = NULL;

	brushCell[0].CreateSolidBrush(RGB(64, 64, 64)); // Available to use
	brushCell[1].CreateSolidBrush(RGB(200, 200, 200)); // Not available
	brushCell[2].CreateSolidBrush(RGB(0, 240, 0)); // Selected
	brushBackground.CreateSolidBrush(RGB(220, 220, 220));
	brushSelectedCell.CreateSolidBrush(RGB(0, 255, 0));

	prcDie = NULL;
	prcFOV = NULL;

	bRedrawing = FALSE;

	nAllowedDie = 0;
	nNotAllowedDie = 0;
	nSelectedDie = 0;

	nAllowedFOV = 0;
	nNotAllowedFOV = 0;
	nSelectedFOV = 0;

	InitializeCriticalSection(&CriticalSectionData);
	InitializeCriticalSection(&CriticalSectionDraw);

	nWindowOffset = 5;
	//nScrollBarOffset = 75;

	bMouseLButtonPressed = FALSE;

	strToolTipText.Format("station mapping");
	bActivatedToolTip = TRUE;

	m_FontText.CreateFont(15, 0, 0, 0, 60, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Arial");

	nMouseSelectedDie = -1;


	rcDieRelative = CRect();
	rcMappingRelative = CRect();

	rgDieMappingRgn.CreateRectRgnIndirect(rcDieRelative);
	dieMappingRgn.CreateRectRgnIndirect(rcDieRelative);

	rgFOVMappingRgn.CreateRectRgnIndirect(rcMappingRelative);
	fovMappingRgn.CreateRectRgnIndirect(rcMappingRelative);

	rg23DMappingAndScrollBarRegion.CreateRectRgnIndirect(rcMappingRelative);
	rg23DMappingScrollBarAndDieMappingRegion.CreateRectRgnIndirect(rcMappingRelative);

	//	nGrabWidth = 1;

	//	m_bContinuousMove = FALSE;

	isSelectingDie = FALSE;
	isSelectingDieMouseMoved = FALSE;
	selectingRect = CRect();
}

CSampling::~CSampling()
{
}

void CSampling::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_INTENSITY_SELCTION, m_ctlrIntensitySelection);
}

BEGIN_MESSAGE_MAP(CSampling, CDialogEx)
	ON_WM_CREATE()
	ON_WM_PAINT()
//	ON_WM_VSCROLL()
//	ON_WM_HSCROLL()
//	ON_WM_MOUSEWHEEL()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_BN_CLICKED(IDOK, &CSampling::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_SAMPLING, &CSampling::OnBnClickedButtonSaveSampling)
	ON_CBN_SELCHANGE(IDC_COMBO_INTENSITY_SELCTION, &CSampling::OnCbnSelchangeComboIntensitySelction)
END_MESSAGE_MAP()

BOOL CSampling::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetClientRect(rcClientWnd);

	//Create the ToolTip control
	if (!m_ToolTip.Create(this)) {
		TRACE0("Unable to create the ToolTip!");
	}
	else {

		int nScreenXSize = GetSystemMetrics(SM_CXSCREEN);
		int nScreenYSize = GetSystemMetrics(SM_CYSCREEN);

		CRect rcFOV = CRect(CPoint(0, 0), CPoint(nScreenXSize, nScreenYSize));

		m_ToolTip.AddTool(this, _T(strToolTipText), rcFOV, 1);

		m_ToolTip.Activate(bActivatedToolTip);

		m_ToolTip.SetDelayTime(TTDT_INITIAL, 10);
		m_ToolTip.SetDelayTime(TTDT_AUTOPOP, INFINITE);
	}

	m_ctlrIntensitySelection.ResetContent();

	CString strText;
	for (int nTrack = 0; nTrack < theApp.m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < theApp.m_pTracks[nTrack].m_nTotalDocs; nDoc++) {
			strText = "";
			if (nTrack == STATION_TOP) {
			//	if (nDoc == 0)
					strText = theApp.m_strTopCameraName.GetAt(nDoc);
				/*if (nDoc == 1)
					strText = theApp.m_strTopCameraName.GetAt(nDoc);*/
			}
			else if (nTrack == STATION_THR) {
			//	if (nDoc == 0)
					strText = theApp.m_strTHRCameraName.GetAt(nDoc);
			}
			m_ctlrIntensitySelection.AddString(strText.GetString());
		}
	}
	m_ctlrIntensitySelection.SetCurSel(0);

	int nBlockNoPerX = 0;
	int nBlockNoPerY = 0;
	int nDieNoPerX = 0;
	int nDieNoPerY = 0;

	theApp.motionParameters.GetMappingParameters(nBlockNoPerX, nBlockNoPerY, nDieNoPerX, nDieNoPerY);
	if (m_nTrackID == STATION_TOP) {
		theApp.motionParameters.GetMappingParametersTOP(nDieNoPerX, nDieNoPerY);
	}
	CreateTemplate(nBlockNoPerX, nBlockNoPerY, nDieNoPerX, nDieNoPerY);

	return TRUE;
}

int CSampling::CreateTemplate(int nDieNoX, int nDieNoY, int nFOVNoX, int nFOVNoY)
{
	if (nFOVNoX < 1) return -1;
	if (nFOVNoY < 1) return -1;
	if (nDieNoX < 1)    return -1;
	if (nDieNoY < 1)    return -1;

	if (nFOVNoX == nFovNoPerDieX &&
		nFOVNoY == nFovNoPerDieY &&
		nDieNoX == nDieNoPerDeviceX &&
		nDieNoY == nDieNoPerDeviceY) {

		return ResetDieStates();
	}

	int i;
	CRect rcWindow;
	CRect rcItem;
	CRect rcDieMappingRect;
	//	CRect dieMappingRect;
	CRgn  rcDieMappingRgnTemp;
	CRgn  dieMappingRgnTemp;

	CRect rcFOVMappingRect;
	//	CRect dieMappingRect;
	CRgn  rcFOVMappingRgnTemp;
	CRgn  fovMappingRgnTemp;

	EnterCriticalSection(&CriticalSectionData);

	nFovNoPerDieX = nFOVNoX;
	nFovNoPerDieY = nFOVNoY;
	nDieNoPerDeviceX = nDieNoX;
	nDieNoPerDeviceY = nDieNoY;

	if (pnDieState != NULL)
		delete[] pnDieState;
	if (pnFOVState != NULL)
		delete[] pnFOVState;

	if (prcDie != NULL)
		delete[] prcDie;
	if (prcFOV != NULL)
		delete[] prcFOV;

	nTotalDieNo =  nDieNoPerDeviceX * nDieNoPerDeviceY;
	nTotalFOVNo = nFOVNoX * nFOVNoY;

	pnDieState = new int[nTotalDieNo];
	pnFOVState = new int[nTotalFOVNo];

	memset((void*)pnDieState, 0, nTotalDieNo * sizeof(int));
	memset((void*)pnFOVState, 0, nTotalFOVNo * sizeof(int));

	//Create drawnings areas
	prcDie = new CRect[nTotalDieNo];
	prcFOV = new CRect[nTotalFOVNo];

	CString strWindowTitle;

	strWindowTitle.Format("Station Mapping");

	SetWindowText(strWindowTitle);

	nAllowedDie = 0;
	nNotAllowedDie = nTotalDieNo;
	nSelectedDie = 0;

	nAllowedFOV = 0;
	nNotAllowedFOV = nTotalFOVNo;
	nSelectedFOV = 0;

	//Resize dialog
	GetDlgItem(IDC_STATIC_FOV_SELECTION)->GetWindowRect(&rcMappingRelative);
	ScreenToClient(&rcMappingRelative);

	rcFOVMappingRect = rcMappingRelative;
	rcFOVMappingRect.DeflateRect(1, 1);
	rcFOVMappingRgnTemp.CreateRectRgnIndirect(rcFOVMappingRect);
	rgFOVMappingRgn.CopyRgn(&rcFOVMappingRgnTemp);

	GetDlgItem(IDC_STATIC_DIE_SELECTION)->GetWindowRect(&rcDieRelative);
	ScreenToClient(&rcDieRelative);

	rcDieMappingRect = rcDieRelative;
	rcDieMappingRect.DeflateRect(1, 1);
	rcDieMappingRgnTemp.CreateRectRgnIndirect(rcDieMappingRect);
	rgDieMappingRgn.CopyRgn(&rcDieMappingRgnTemp);

	const int nScreenXSize = rcMappingRelative.Width();
	const int nScreenYSize = rcMappingRelative.Height();

	const int nDiePerX = nDieNoPerDeviceX;
	const int nDiePerY = nDieNoPerDeviceY;

//	int nDeviceBordersX = nDeviceNoPerSubstrateX * 3 - 1;
//	int nDeviceBordersY = nDeviceNoPerSubstrateY * 3 - 1;

	int nFreeSpaceDieX = nDiePerX ;
	int nFreeSpaceDieY = nDiePerY;

	int nMaxScreenDrawX = nScreenXSize /*- nDeviceBordersX*/ - nFreeSpaceDieX - nWindowOffset;
	int nMaxScreenDrawY = nScreenYSize /*- nDeviceBordersY */- nFreeSpaceDieY - nWindowOffset;

	nDieRectSize = nMaxScreenDrawX / nDiePerX < nMaxScreenDrawY / nDiePerY ? nMaxScreenDrawX / nDiePerX : nMaxScreenDrawY / nDiePerY;

	if (nDieRectSize < 2)
		nDieRectSize = 2;

	if (nDieRectSize > 20)
		nDieRectSize = 20;

	//Creat die rectangles
	int nDieOffsetX = 0;
	int nDieOffsetY = 0;
	int nDiePerLine = nDieNoPerDeviceX;

	int nX;
	int nY;
	int nDieDeviceIDX;
	int nDieDeviceIDY;

	for (i = 0; i<nTotalDieNo; i++) {

		nDieOffsetX = i % nDiePerLine;
		nDieDeviceIDX = nDieOffsetX / nDieNoPerDeviceX;
		nDieOffsetX = nDieOffsetX % nDieNoPerDeviceX;

		nDieOffsetY = i / nDiePerLine;
		nDieDeviceIDY = nDieOffsetY / nDieNoPerDeviceY;
		nDieOffsetY = nDieOffsetY % nDieNoPerDeviceY;

		nX = nDieDeviceIDX * (3 + 1 + (nDieRectSize + 1) * nDieNoPerDeviceX) + 2 + (nDieRectSize + 1) * nDieOffsetX + nWindowOffset / 2 + rcDieRelative.left;
		nY = nDieDeviceIDY * (3 + 1 + (nDieRectSize + 1) * nDieNoPerDeviceY) + 2 + (nDieRectSize + 1) * nDieOffsetY + nWindowOffset / 2 + rcDieRelative.top;

		prcDie[i] = CRect(nX, nY, nX + nDieRectSize, nY + nDieRectSize);
	}

	int nFOVPerLine = nFovNoPerDieX;
	int nFOVOffsetX = 0;
	int nFOVOffsetY = 0;
	int nFOVDeviceIDX;
	int nFOVDeviceIDY;

	int nSizeMulX = 1;
	int nSizeMulY = 1;

	if (m_nTrackID == STATION_THR) {
		nSizeMulX = 3;
		nSizeMulY = 3;
	}


	for (i = 0; i<nTotalFOVNo; i++) {

		nFOVOffsetX = i % nFOVPerLine;
		nFOVDeviceIDX = nFOVOffsetX / nFovNoPerDieX;
		nFOVOffsetX = nFOVOffsetX % nFovNoPerDieX;

		nFOVOffsetY = i / nFOVPerLine;
		nFOVDeviceIDY = nFOVOffsetY / nFovNoPerDieY;
		nFOVOffsetY = nFOVOffsetY % nFovNoPerDieY;

		nX = nFOVDeviceIDX * (3 + 1 + (nDieRectSize * nSizeMulX + 1) * nFovNoPerDieX) + 2 + (nDieRectSize * nSizeMulX + 1) * nFOVOffsetX + nWindowOffset / 2 + rcMappingRelative.left;
		nY = nFOVDeviceIDY * (3 + 1 + (nDieRectSize * nSizeMulY + 1) * nFovNoPerDieY) + 2 + (nDieRectSize * nSizeMulY + 1) * nFOVOffsetY + nWindowOffset / 2 + rcMappingRelative.top;

		prcFOV[i] = CRect(nX, nY, nX + nDieRectSize * nSizeMulX, nY + nDieRectSize * nSizeMulY);
	}

	GetWindowRect(&rcWindow);


	LeaveCriticalSection(&CriticalSectionData);

	Invalidate();

	return 0;
}

// CSampling message handlers

void CSampling::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

int CSampling::SetDieStates(std::vector <int> pnFOVStatus)
{
	if (pnDieState == NULL) return -1;

	int i;

	int nResultNo = nTotalFOVNo;

	if (nResultNo < 1) return -1;

	BOOL bRedraw = FALSE;

	EnterCriticalSection(&CriticalSectionData);

	for (i = 0; i<nResultNo; i++) {

		pnFOVState[i] = pnFOVStatus[i];

		if (pnFOVStatus[i] == 0) {
			nAllowedFOV++;
		}
		else if (pnFOVStatus[i] == 1) {
			nNotAllowedFOV++;
		}
		else {
			nSelectedFOV++;
			nAllowedFOV++;
		}
	}

	bRedraw = TRUE;

	LeaveCriticalSection(&CriticalSectionData);

	if (bRedraw)
		Invalidate();

	return 0;
}

int CSampling::SetIntensityID(int nTrackID, int nDocID)
{
	m_nTrackID = nTrackID;
	m_nDocID = nDocID;
	int nComboIndex = 0;

	for (int i = 0; i <= m_nTrackID; i++) {
		for (int nDoc = 0; nDoc < theApp.m_pTracks[i].m_nTotalDocs; nDoc++) {
			if (nDocID == nDoc && m_nTrackID == i)
				break;
			nComboIndex++;
		}
	}

	m_ctlrIntensitySelection.SetCurSel(nComboIndex);
	
	return 0;
}

int CSampling::ResetDieStates()
{

	EnterCriticalSection(&CriticalSectionData);

	if (nTotalFOVNo < 1) {
		LeaveCriticalSection(&CriticalSectionData);
		return -1;
	}

	memset((void*)pnFOVState, 0, nTotalFOVNo * sizeof(int));

	nAllowedFOV = 0;

	nNotAllowedFOV = 0;

	nSelectedFOV = 0;

	LeaveCriticalSection(&CriticalSectionData);

	Invalidate();

	return 0;
}

BOOL CSampling::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE)
		m_ToolTip.RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSampling::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	BOOL bDie = FALSE;
	BOOL bFOV = FALSE;
	int nDIeID = 0;
	int nFOVID = 0;

	CPoint pointRel = point;

	if (PtInRect(&pointRel, &rcDieRelative)) {

		CRect  * prcDiePtr = prcDie;

		for (int i = 0; i<nTotalDieNo; i++, prcDiePtr++) {
			CRect rcDie = CRect(prcDiePtr->left,
				prcDiePtr->top,
				prcDiePtr->right,
				prcDiePtr->bottom);

			CheckRect(&rcDieRelative, &rcDie);

			if (RectInRect(rcDieRelative, rcDie)) {

				if (PtInRect(&pointRel, &rcDie) && PtInRect(&pointRel, &rcDieRelative)) {
					bDie = TRUE;
					nDIeID = i;
					break;
				}
			}
		}
	}

	if (PtInRect(&pointRel, &rcMappingRelative)) {

		CRect  * prcFOVPtr = prcFOV;

		for (int i = 0; i<nTotalFOVNo; i++, prcFOVPtr++) {
			CRect rcFOV = CRect(prcFOVPtr->left,
				prcFOVPtr->top,
				prcFOVPtr->right,
				prcFOVPtr->bottom);

			CheckRect(&rcMappingRelative, &rcFOV);

			if (RectInRect(rcMappingRelative, rcFOV)) {

				if (PtInRect(&pointRel, &rcFOV) && PtInRect(&pointRel, &rcMappingRelative)) {
					bFOV = TRUE;
					nFOVID = i;
					break;
				}
			}
		}
	}

	if (bDie) {

		if (pnDieState[nDIeID] == 0) {
			pnDieState[nDIeID] = 2;
			nSelectedDie++;
		}
		else if (pnDieState[nDIeID] == 2) {
			pnDieState[nDIeID] = 0;
			nSelectedDie--;
		}

		isSelectingDie = TRUE;
		isSelectingDieMouseMoved = FALSE;
		selectingStartPos = point;
		selectingRect = CRect();
		dieStateAtSelectingStartPos = pnDieState[nDIeID];
		SetCapture();

		InvalidateRgn(&rgDieMappingRgn, FALSE);
	}

	if (bFOV) {

		if (pnFOVState[nFOVID] == 0) {
			pnFOVState[nFOVID] = 2;
			nSelectedFOV++;
		}
		else if (pnFOVState[nFOVID] == 2) {
			pnFOVState[nFOVID] = 0;
			nSelectedFOV--;
		}

		isSelectingFOV = TRUE;
		isSelectingFOVMouseMoved = FALSE;
		selectingStartPosFOV = point;
		selectingRectFOV = CRect();
		dieStateAtSelectingStartPosFOV = pnFOVState[nFOVID];
//		SetCapture();

		InvalidateRgn(&rgFOVMappingRgn, FALSE);
	}
	//else if(nGrabColumnNo > 0 && nGrabRowNo > 0) {

	//	int nDieMappingWidth = (rcDieRelative.Width() - 10 - (nGrabColumnNo - 1)) / nGrabColumnNo;
	//	int nDieMappingHeight = (rcDieRelative.Height() - 10 - (nGrabRowNo - 1)) / nGrabRowNo;

	//	if(nDieMappingWidth > nDieMappingHeight)
	//		nDieMappingWidth = nDieMappingHeight;

	//	int nOffsetDieMappingX = (rcDieRelative.Width() - (nDieMappingWidth * nGrabColumnNo + nGrabColumnNo - 1)) / 2;
	//	int nOffsetDieMappingY = (rcDieRelative.Height() - (nDieMappingWidth * nGrabRowNo + nGrabRowNo - 1)) / 2;

	//	for(int i = 0; i<nGrabRowNo; i++) {
	//		for(int j = 0; j<nGrabColumnNo; j++) {
	//			CRect rcRect = CRect(nOffsetDieMappingX + j*(nDieMappingWidth + 1), nOffsetDieMappingY + i*(nDieMappingWidth + 1),
	//				nOffsetDieMappingX + j*(nDieMappingWidth + 1) + nDieMappingWidth, nOffsetDieMappingY + i*(nDieMappingWidth + 1) + nDieMappingWidth) + rcDieRelative.TopLeft();

	//			CheckRect(&rcClientWnd, &rcRect);

	//			if(RectInRect(rcClientWnd, rcRect)) {

	//				if(PtInRect(&pointRel, &rcRect)) {
	//					if(nSelectedGrabID != i*nGrabColumnNo + j) {

	//						nSelectedGrabID = i*nGrabColumnNo + j;

	//					//	m_ListBoxDie.ResetContent();

	//						int nDieZPositionNo = 0;

	//						int nError = motionParameters.GetBondDieZLevelNoPerDie(nSelectedGrabID % nGrabColumnNo, nSelectedGrabID / nGrabColumnNo, nDieZPositionNo);
	//						if(!nError) {

	//							int nZLevelID = 0;
	//							CString strZPosition;

	//							int nPositionID = 0;
	//							int nTemp = 0;

	//							nError = motionParameters.GetBondDieZLevelPosition(nSelectedGrabID % nGrabColumnNo, nSelectedGrabID / nGrabColumnNo, nPositionID);			//Girish
	//							if(nError) return;

	//							for(int k = 0; k<nDieZPositionNo; k++) {
	//								nError = motionParameters.GetBondDieZLevelIDPerDie(nSelectedGrabID % nGrabColumnNo, nSelectedGrabID / nGrabColumnNo, k, nZLevelID);
	//								if(nError) continue;
	//								int nIndex = 0;

	//								if(m_vecZLevelIndexVal[nPositionID] <= k)
	//									m_vecZLevelIndexVal[nPositionID] = k + 1;

	//								if(nPositionID == 0)
	//									nIndex = k + 1;
	//								else {
	//									for(int j = 0; j < nPositionID; j++) {
	//										nIndex += m_vecZLevelIndexVal[j];
	//									}
	//									nIndex += k + 1;
	//								}

	//								//								strZPosition.Format("%02d", nZLevelID + 1);
	//								strZPosition.Format("%02d -> %02d", nIndex, nZLevelID + 1);				//Girish


	//								m_ListBoxDie.AddString(strZPosition);
	//							}

	//							if(nDieZPositionNo > 0)
	//								m_ListBoxDie.SetCurSel(0);

	//							OnLbnSelchangeListDieZPositions();

	//							//UpdateData(FALSE);
	//						}

	//						InvalidateRgn(&dieMappingRgn, FALSE);
	//					}

	//					i = nGrabRowNo;
	//					j = nGrabColumnNo;

	//					break;
	//				}
	//			}

	//		}
	//	}

	//}

	CDialogEx::OnLButtonDown(nFlags, point);
}

BOOL CSampling::OnEraseBkgnd(CDC* /*pDC*/)
{
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CSampling::OnPaint()
{
	/*EnterCriticalSection(&CriticalSectionDraw);
	if (bRedrawing)
		return;
	else
		bRedrawing = TRUE;

	LeaveCriticalSection(&CriticalSectionDraw);*/

	int i, j;

	CPaintDC dc(this);
	CDC dcMemory;
	CRect ClientRect;
	GetClientRect(ClientRect);

	dcMemory.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	CBitmap * pbitmap;

	bitmap.CreateCompatibleBitmap(&dc, ClientRect.Width(), ClientRect.Height());

	pbitmap = dcMemory.SelectObject(&bitmap);

	::FillRect(dcMemory.GetSafeHdc(), ClientRect, (HBRUSH)(COLOR_BTNFACE + 1));

	CRect  * prcDiePtr = prcDie;
	int * pnDieStatePtr = pnDieState;
	CRect rcRect;

//	if (nZoom == 1) {
		for (i = 0; i<nTotalDieNo; i++, pnDieStatePtr++, prcDiePtr++) {
			rcRect = CRect(prcDiePtr->left,
				prcDiePtr->top,
				prcDiePtr->right,
				prcDiePtr->bottom);

			if (CheckRect(&rcDieRelative, &rcRect)) continue;

			dcMemory.FillRect(rcRect, &brushCell[*pnDieStatePtr]);
		}

		CRect  * prcFOVPtr = prcFOV;
		int * pnFOVStatePtr = pnFOVState;

		//	if (nZoom == 1) {
		for (i = 0; i<nTotalFOVNo; i++, pnFOVStatePtr++, prcFOVPtr++) {
			rcRect = CRect(prcFOVPtr->left,
				prcFOVPtr->top,
				prcFOVPtr->right,
				prcFOVPtr->bottom);

			if (CheckRect(&rcMappingRelative, &rcRect)) continue;

			dcMemory.FillRect(rcRect, &brushCell[*pnFOVStatePtr]);
		}

	//}
	//else {
	//	for (i = 0; i<nTotalNo; i++, pnDieStatePtr++, prcDiePtr++) {
	//		rcRect = CRect(prcDiePtr->left   * nZoom - nHorizontalScrollBar,
	//			prcDiePtr->top    * nZoom - nVerticalScrollBar,
	//			prcDiePtr->right  * nZoom - nHorizontalScrollBar,
	//			prcDiePtr->bottom * nZoom - nVerticalScrollBar);

	//		if (CheckRect(&rcMappingRelative, &rcRect)) continue;

	//		dcMemory.FillRect(rcRect, &brushCell[*pnDieStatePtr]);
	//	}
	//}

	if (nMouseSelectedDie > -1 && nMouseSelectedDie < nTotalDieNo) {

		rcRect = CRect(prcDie[nMouseSelectedDie].left ,
			prcDie[nMouseSelectedDie].top,
			prcDie[nMouseSelectedDie].right,
			prcDie[nMouseSelectedDie].bottom);

		if (!CheckRect(&rcDieRelative, &rcRect))
			dcMemory.FrameRect(rcRect, &brushSelectedCell);
	}

	int referenceBondColumn = 0;
	int referenceBondRow = 0;

	//--. Draw die mapping
	//if (nGrabColumnNo > 0 && nGrabRowNo > 0) {

	//	int nDieMappingWidth = (rcDieRelative.Width() - 10 - (nGrabColumnNo - 1)) / nGrabColumnNo;
	//	int nDieMappingHeight = (rcDieRelative.Height() - 10 - (nGrabRowNo - 1)) / nGrabRowNo;

	//	if (nDieMappingWidth > nDieMappingHeight)
	//		nDieMappingWidth = nDieMappingHeight;

	//	int nOffsetDieMappingX = (rcDieRelative.Width() - (nDieMappingWidth * nGrabColumnNo + nGrabColumnNo - 1)) / 2;
	//	int nOffsetDieMappingY = (rcDieRelative.Height() - (nDieMappingWidth * nGrabRowNo + nGrabRowNo - 1)) / 2;

	//	/*if (pMotion != NULL)
	//		pMotion->getReferenceBondImageId(referenceBondColumn, referenceBondRow);*/

	//	for (i = 0; i<nGrabRowNo; i++) {
	//		for (j = 0; j<nGrabColumnNo; j++) {
	//			rcRect = CRect(nOffsetDieMappingX + j*(nDieMappingWidth + 1), nOffsetDieMappingY + i*(nDieMappingWidth + 1),
	//				nOffsetDieMappingX + j*(nDieMappingWidth + 1) + nDieMappingWidth, nOffsetDieMappingY + i*(nDieMappingWidth + 1) + nDieMappingWidth) + rcDieRelative.TopLeft();

	//			if (CheckRect(&rcDieRelative, &rcRect)) continue;

	//			if (nSelectedGrabID == i*nGrabColumnNo + j)
	//				dcMemory.FillRect(rcRect, &brushCell[2]);
	//			else
	//				dcMemory.FillRect(rcRect, &brushCell[0]);

	//			/*if (j == referenceBondColumn && i == referenceBondRow) {
	//				CBrush brushTemp;
	//				brushTemp.CreateSolidBrush(RGB(255, 0, 0));
	//				dcMemory.FrameRect(rcRect, &brushTemp);
	//				brushTemp.DeleteObject();
	//			}*/
	//		}
	//	}

	//}

	//--. Draw statistics
	dcMemory.SetBkColor(RGB(220, 220, 220));

	CFont * pOldFont;
	CString strText;

	pOldFont = dcMemory.SelectObject(&m_FontText);

	strText.Format("Available: %d, Selected: %d, Not Available: %d", nAllowedFOV, nSelectedFOV, nTotalFOVNo - nAllowedFOV);

	rcRect = CRect(prcFOV[0].left ,
		prcFOV[nTotalFOVNo - 1].bottom,
		prcFOV[0].left  + 400,
		prcFOV[nTotalFOVNo - 1].bottom + 25);

	if (!CheckRect(&rcMappingRelative, &rcRect))
		dcMemory.DrawTextA(strText, -1, rcRect, DT_LEFT);

	dcMemory.SelectObject(pOldFont);

	if (isSelectingDie) {
		if (selectingRect.TopLeft() != selectingRect.BottomRight()) {
			CBrush* pOldBrush = (CBrush*)dcMemory.SelectStockObject(NULL_BRUSH);

			CPen blackPen;
			blackPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			CPen* pOldPen = dcMemory.SelectObject(&blackPen);

			dcMemory.Rectangle(selectingRect);

			dcMemory.SelectObject(pOldBrush);
			dcMemory.SelectObject(pOldPen);
		}
	}

	//dc.BitBlt(rcMappingRelative.left,rcMappingRelative.top,rcMappingRelative.Width(), rcMappingRelative.Height(), &dcMemory, 0, 0, SRCCOPY);

	dc.BitBlt(0, 0, ClientRect.Width(), ClientRect.Height(), &dcMemory, 0, 0, SRCCOPY);

	dcMemory.SelectObject(pbitmap);

	dcMemory.DeleteDC();
	bitmap.DeleteObject();

	bRedrawing = FALSE;
}

LRESULT CSampling::OnNcHitTest(CPoint point)
{
	BOOL bWire = FALSE;
	int wireId = 0;

	CPoint pointRel = point;

	ScreenToClient(&pointRel);

	if (PtInRect(&pointRel, &rcClientWnd)) {

		CRect  * prcDiePtr = prcDie;
		CRect rcDie;

		for (int i = 0; i<nTotalDieNo; i++, prcDiePtr++) {
			rcDie = CRect(prcDiePtr->left ,
				prcDiePtr->top ,
				prcDiePtr->right ,
				prcDiePtr->bottom );

			CheckRect(&rcClientWnd, &rcDie);

			if (RectInRect(rcClientWnd, rcDie)) {

				if (PtInRect(&pointRel, &rcDie) && PtInRect(&pointRel, &rcMappingRelative)) {
					bWire = TRUE;
					wireId = i;
					break;
				}
			}
		}

	}

	if (bWire) {

		const int nDiePerLine = nDieNoPerDeviceX;

		int nDieDeviceIDX;
		int nDieDeviceIDY;
		int nDieIDX;
		int nDieIDY;
		int nDieIDNo = wireId;

		nDieDeviceIDY = nDieIDNo / ( nDieNoPerDeviceX * nDieNoPerDeviceY);
		nDieIDNo = nDieIDNo - nDieDeviceIDY * (nDieNoPerDeviceX * nDieNoPerDeviceY);
		nDieIDY = nDieIDNo / ( nDieNoPerDeviceX);
		nDieIDNo = nDieIDNo - nDieIDY * ( nDieNoPerDeviceX);
		nDieDeviceIDX = nDieIDNo / nDieNoPerDeviceX;
		nDieIDX = nDieIDNo - nDieDeviceIDX * nDieNoPerDeviceX;

		CString strToolTipTextNew;

		if (pnDieState[wireId] == 0)
			strToolTipTextNew.Format("Row %d, Column %d: Available", wireId / nDiePerLine + 1, wireId % nDiePerLine + 1);
		else if (pnDieState[wireId] == 1)
			strToolTipTextNew.Format("Row %d, Column %d: Not Available", wireId / nDiePerLine + 1, wireId % nDiePerLine + 1);
		else if (pnDieState[wireId] == 2)
			strToolTipTextNew.Format("Row %d, Column %d: Selected", wireId / nDiePerLine + 1, wireId % nDiePerLine + 1);

		if (strToolTipText.CompareNoCase(strToolTipTextNew) != 0) {

			strToolTipText = strToolTipTextNew;
			m_ToolTip.UpdateTipText((LPCTSTR)strToolTipText, this, 1);
		}


		if (!bActivatedToolTip) {
			bActivatedToolTip = TRUE;
			m_ToolTip.Activate(bActivatedToolTip);
		}

		if (nMouseSelectedDie != wireId) {
			nMouseSelectedDie = wireId;
			InvalidateRgn(&rgDieMappingRgn, FALSE);
		}
	}
	else {
		if (bActivatedToolTip) {
			bActivatedToolTip = FALSE;
			m_ToolTip.Activate(bActivatedToolTip);
		}

		if (nMouseSelectedDie != -1) {
			nMouseSelectedDie = -1;
			InvalidateRgn(&rgDieMappingRgn, FALSE);
		}

	}

	return CDialogEx::OnNcHitTest(point);
}

void CSampling::OnMouseMove(UINT nFlags, CPoint point)
{
	if (isSelectingDie) {
		CRect tempRect = rcMappingRelative;
		tempRect.DeflateRect(3, 3);

		if (point.x >= tempRect.left && point.x <= tempRect.right)
			selectingEndPos.x = point.x;
		else if (point.x < tempRect.left)
			selectingEndPos.x = tempRect.left;
		else if (point.x > tempRect.right)
			selectingEndPos.x = tempRect.right;

		if (point.y >= tempRect.top && point.y <= tempRect.bottom)
			selectingEndPos.y = point.y;
		else if (point.y < tempRect.top)
			selectingEndPos.y = tempRect.top;
		else if (point.y > tempRect.bottom)
			selectingEndPos.y = tempRect.bottom;

		// To ensure area of selecting rectangle more than 0
		if (selectingEndPos.x == selectingStartPos.x)
			++selectingEndPos.x;
		if (selectingEndPos.y == selectingStartPos.y)
			++selectingEndPos.y;

		selectingRect = CRect(selectingStartPos, selectingEndPos);

		isSelectingDieMouseMoved = TRUE;

		InvalidateRgn(&rgDieMappingRgn, FALSE);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CSampling::OnLButtonUp(UINT nFlags, CPoint point)
{
	int wireId = 0;

	if (isSelectingDie) {
		if (isSelectingDieMouseMoved && (selectingRect.TopLeft() != selectingRect.BottomRight())) {
			CRect  * prcDiePtr = prcDie;
			CRect intersectRect;

			for (int i = 0; i<nTotalDieNo; i++, prcDiePtr++) {
				CRect rcDie = CRect(prcDiePtr->left ,
					prcDiePtr->top ,
					prcDiePtr->right,
					prcDiePtr->bottom);

				CheckRect(&rcClientWnd, &rcDie);

				if (RectInRect(rcClientWnd, rcDie)) {
					rcDie.NormalizeRect();
					selectingRect.NormalizeRect();

					if (IntersectRect(intersectRect, &rcDie, &selectingRect)) {
						wireId = i;
						if (dieStateAtSelectingStartPos == 2) {
							if (pnDieState[wireId] != dieStateAtSelectingStartPos) {
								pnDieState[wireId] = 2;
								++nSelectedDie;
							}
						}
						else if (dieStateAtSelectingStartPos == 0) {
							if (pnDieState[wireId] != dieStateAtSelectingStartPos) {
								pnDieState[wireId] = 0;
								--nSelectedDie;
							}
						}
					}
				}
			}

			isSelectingDieMouseMoved = FALSE;
		}

		ReleaseCapture();
		isSelectingDie = FALSE;
	}

	InvalidateRgn(&rgDieMappingRgn, FALSE);

	CDialogEx::OnLButtonUp(nFlags, point);
}

// CSetup23DLocationTeachDlg message handlers
void CSampling::SetMotionParameters(MotionParameters * pMotionIn)
{
	if (pMotionIn != NULL) {
		pMotion = pMotionIn;

		motionParameters = *pMotion;
	}
}

void CSampling::OnBnClickedButtonSaveSampling()
{
	theApp.m_pTracks[m_nTrackID].m_pDoc[m_nDocID]->SetSamplingStatus(pnFOVState, nTotalFOVNo);
	theApp.m_pTracks[m_nTrackID].m_pDoc[m_nDocID]->SaveSamplingParameters();
	theApp.m_pTracks[m_nTrackID].m_pDoc[m_nDocID]->LoadSamplingParameters();
}


void CSampling::OnCbnSelchangeComboIntensitySelction()
{
	UpdateData(TRUE);

	int nBlockNoPerX = 0;
	int nBlockNoPerY = 0;
	int nDieNoPerX = 0;
	int nDieNoPerY = 0;

	int nIndex = 0;
	nIndex = m_ctlrIntensitySelection.GetCurSel();

	CString strText;
	m_ctlrIntensitySelection.GetLBText(m_ctlrIntensitySelection.GetCurSel(), strText);

	for (int nTrack = 0; nTrack < theApp.m_nTotalTracks; nTrack++) {
		for (int nDoc = 0; nDoc < theApp.m_pTracks[nTrack].m_nTotalDocs; nDoc++) {
			if (nTrack == STATION_TOP) {
				if (!strText.Compare(theApp.m_strTopCameraName.GetAt(nDoc))) {
					m_nDocID = nDoc; 
					m_nTrackID = nTrack;
					break;
				}
			}
			else if (nTrack == STATION_THR) {
				if (!strText.Compare(theApp.m_strTHRCameraName.GetAt(nDoc))){
					m_nDocID = nDoc;
					m_nTrackID = nTrack;
					break;
				}
			}
		}
	}

	theApp.motionParameters.GetMappingParameters(nBlockNoPerX, nBlockNoPerY, nDieNoPerX, nDieNoPerY);

	if (m_nTrackID == STATION_TOP) {
		theApp.motionParameters.GetMappingParametersTOP(nDieNoPerX, nDieNoPerY);
	}

	theApp.m_pSamplingDlg->CreateTemplate(nBlockNoPerX, nBlockNoPerY, nDieNoPerX, nDieNoPerY);
	theApp.m_pTracks[m_nTrackID].m_pDoc[m_nDocID]->GetSamplingStatus(pnFOVState, nTotalFOVNo);
}
