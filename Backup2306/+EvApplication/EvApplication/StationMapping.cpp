// StationMapping.cpp : implementation file
//

#include "stdafx.h"
#include "StationMapping.h"
#include "afxdialogex.h"
#include "Application.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include "LightHandler.h"
#include "MotionControlDialog.h"
#include "LightHandler.h"
#include "LightControl.h"

// CStationMapping dialog

IMPLEMENT_DYNAMIC(CStationMapping, CDialogEx)

CStationMapping::CStationMapping(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStationMapping::IDD, pParent)
{
	pApp = (CApplication*) AfxGetApp();

	m_prgnPositionSelection = NULL;
	m_bPosSelection = NULL;
	m_prgnDieMapPositionSel = NULL;
	m_bDieMapPosSel = NULL;

	m_diePicCtrl = NULL;
	m_brushCheck.CreateSolidBrush(RGB(20,255,0));
	m_brushUncheck.CreateSolidBrush(RGB(125,125,120));
	m_brushBorder.CreateSolidBrush(RGB(0,0,0));
	
	//m_dZMotorRelativeMoveDisp = 0.0;
	m_strZMotorRelativeMoveDisp.Format("0.0");
	m_nZMotorPosDisp = 0;
	m_dZMotorScanMaxDisp = 0.0;

	for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++) {
		m_dZMotorRelativeMove[nTrack] = 0.0;
		m_nZMotorPos[nTrack] = 0;
		m_dZMotorScanMax[nTrack] = 0.0;
		m_bRelativeCheck[nTrack] = TRUE;//FALSE;
	}
	m_bPosSelection = new BOOL[MAX_DIE_FOVS];
}

CStationMapping::~CStationMapping()
{
	if(m_bDieMapPosSel != NULL)
		delete[] m_bDieMapPosSel;

	if(m_bPosSelection != NULL)
		delete[] m_bPosSelection;

	m_brushCheck.DeleteObject();
	m_brushUncheck.DeleteObject();
	m_brushBorder.DeleteObject();

	if(m_prgnPositionSelection != NULL) {
		for(int i=0; i<m_nPosSelectionCount; i++)
			m_prgnPositionSelection[i].DeleteObject();
		delete[] m_prgnPositionSelection;
	}
	m_prgnPositionSelection = NULL;

	if(m_prgnDieMapPositionSel != NULL) {
		for(int i=0; i<m_nDieMapSelCount; i++)
			m_prgnDieMapPositionSel[i].DeleteObject();
		delete[] m_prgnDieMapPositionSel;
	}
	m_prgnDieMapPositionSel = NULL;

	if (m_diePicCtrl != NULL)
		delete[] m_diePicCtrl;
	m_diePicCtrl = NULL;
}

void CStationMapping::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SM_CAMID_COMBO, m_ctrlCameraID);
	DDX_Control(pDX, IDC_SM_DIE_MAP_LIST, m_ctrlDieMap);
	DDX_Control(pDX, IDC_SM_DIE_MAP_POSID_LIST, m_ctrlDieMapPosID);
	//DDX_Text(pDX, IDC_SM_Z_MTR_SERVO_EDIT, m_dZMotorRelativeMoveDisp);
	DDX_Text(pDX, IDC_SM_Z_MTR_SERVO_EDIT, m_strZMotorRelativeMoveDisp);
	DDX_Text(pDX, IDC_SM_START_EDIT, m_nZMotorPosDisp);
	DDX_Text(pDX, IDC_SM_MAX_EDIT, m_dZMotorScanMaxDisp);	
}


BEGIN_MESSAGE_MAP(CStationMapping, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_VKEYTOITEM()
	ON_WM_TIMER()
	ON_WM_CLOSE()

	ON_CBN_SELCHANGE(IDC_SM_CAMID_COMBO, &CStationMapping::OnSelchangeSmCamidCombo)
	ON_BN_CLICKED(IDC_SM_SELECTALL_BUTTON, &CStationMapping::OnBnClickedSmSelectallButton)	

	ON_BN_CLICKED(IDC_SM_DIE_MAP_ADD_MFCBUTTON, &CStationMapping::OnBnClickedSmDieMapAddMfcbutton)
	ON_BN_CLICKED(IDC_SM_DIE_MAP_REMOVE_MFCBUTTON, &CStationMapping::OnBnClickedSmDieMapRemoveMfcbutton)
	ON_BN_CLICKED(IDC_SM_DIE_MAP_RETAKE_MFCBUTTON, &CStationMapping::OnBnClickedSmDieMapRetakeMfcbutton)
	//ON_LBN_DBLCLK(IDC_SM_DIE_MAP_LIST, &CStationMapping::OnDblclkSmDieMapList)
	ON_LBN_SELCHANGE(IDC_SM_DIE_MAP_LIST, &CStationMapping::OnSelchangeSmDieMapList)
	ON_BN_CLICKED(IDC_SM_DIE_MAP_SWAP_MFCBUTTON, &CStationMapping::OnBnClickedSmDieMapSwapMfcbutton)
	ON_BN_CLICKED(IDC_SM_DIE_MAP_POSMOVE_SETLIGHT_BUTTON, &CStationMapping::OnBnClickedSmDieMapPosmoveSetlightButton)
	ON_BN_CLICKED(IDC_SM_DIE_MAP_POSID_REMOVE_MFCBUTTON, &CStationMapping::OnBnClickedSmDieMapPosidRemoveMfcbutton)
	ON_BN_CLICKED(IDC_SM_DIE_MAP_LIVECAM_LIGHT_BUTTON, &CStationMapping::OnBnClickedSmDieMapLivecamLightButton)

	ON_BN_CLICKED(IDC_SM_SAVE_ALL_TEACH_BUTTON, &CStationMapping::OnBnClickedSmSaveAllTeachButton)
	ON_BN_CLICKED(IDC_SM_SAVE_ONE_TEACH_BUTTON, &CStationMapping::OnBnClickedSmSaveOneTeachButton)
	ON_BN_CLICKED(IDC_SM_LOAD_TEACH_BUTTON, &CStationMapping::OnBnClickedSmLoadTeachButton)
	ON_BN_CLICKED(IDC_SM_SET_IMG_ID_BUTTON, &CStationMapping::OnBnClickedSmSetImgIdButton)
	ON_EN_UPDATE(IDC_SM_START_EDIT, &CStationMapping::OnUpdateSmStartEdit)
	ON_EN_UPDATE(IDC_SM_MAX_EDIT, &CStationMapping::OnUpdateSmMaxEdit)

	ON_EN_UPDATE(IDC_SM_Z_MTR_SERVO_EDIT, &CStationMapping::OnUpdateSmZMtrServoEdit)
	ON_BN_CLICKED(IDC_SM_Z_MTR_SERVO_CHECK, &CStationMapping::OnClickedSmZMtrServoCheck)
	ON_BN_CLICKED(IDC_SM_Z_MTR_SERVO_UP_MFCBUTTON, &CStationMapping::OnBnClickedSmZMtrServoUpMfcbutton)
	ON_BN_CLICKED(IDC_SM_Z_MTR_SERVO_DOWN_MFCBUTTON, &CStationMapping::OnBnClickedSmZMtrServoDownMfcbutton)

	ON_BN_CLICKED(IDOK, &CStationMapping::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStationMapping::OnBnClickedCancel)

	ON_BN_CLICKED(IDC_SM_NEGLECT_MFCBUTTON, &CStationMapping::OnBnClickedSmNeglectMfcbutton)
	ON_BN_CLICKED(IDC_SM_SAVE_TEACH_IMAGE_ALL_BUTTON, &CStationMapping::OnBnClickedSmSaveTeachImageAllButton)
END_MESSAGE_MAP()


// CStationMapping message handlers

BOOL CStationMapping::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initialize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStationMapping::Initialize()
{
	int m_nNoOfBlock[2];
	int m_nDeviceNoPerBlock[2];

	pApp->motionParameters.GetMappingParameters(m_nNoOfBlock[0], m_nNoOfBlock[1], m_nDeviceNoPerBlock[0], m_nDeviceNoPerBlock[1]);

	m_nPosSelectionCount = m_nDeviceNoPerBlock[0]*m_nDeviceNoPerBlock[1];
	if(m_nPosSelectionCount > MAX_DIE_FOVS)
		m_nPosSelectionCount = MAX_DIE_FOVS;
	m_nDieMapSelCount = (pApp->m_nNoOfFov < MAX_DIE_MAP_FOVS) ? pApp->m_nNoOfFov : MAX_DIE_MAP_FOVS;

	/*for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++) {
		m_strDieMapContents[nTrack].RemoveAll();
		m_strPulseValue[nTrack].RemoveAll();
		for(int a=0; a<m_nDieMapSelCount; a++)
			m_strDiePosIDContents[nTrack][a].RemoveAll();
	}
	m_ctrlDieMap.ResetContent();
	m_ctrlDieMapPosID.ResetContent();*/

	if(m_prgnPositionSelection!=NULL)
		delete [] m_prgnPositionSelection;
	/*if(m_bPosSelection != NULL)
		delete[] m_bPosSelection;*/
	if(m_prgnDieMapPositionSel!=NULL)
		delete [] m_prgnDieMapPositionSel;
	if(m_bDieMapPosSel != NULL)
		delete[] m_bDieMapPosSel;

	if (m_diePicCtrl != NULL)
		delete[] m_diePicCtrl;

	m_diePicCtrl = new CPictureCtrl[m_nDieMapSelCount];
	for (int i = 0; i < m_nDieMapSelCount; i++) {
		m_diePicCtrl[i].Create("", 1, CRect(0, 0, 100, 100), this);
		m_diePicCtrl[i].ShowWindow(TRUE);
	}
	m_prgnPositionSelection = new CRgn[m_nPosSelectionCount];
	//m_bPosSelection = new BOOL[m_nPosSelectionCount];
	m_prgnDieMapPositionSel = new CRgn[m_nDieMapSelCount];
	m_bDieMapPosSel = new BOOL[m_nDieMapSelCount];
	
	CRect rectDie, rectDieMap;
	int nSel, nDeviceNoPerBlock, nTempMul=-1;
	/*int m_nNoOfBlock[2];
	int m_nDeviceNoPerBlock[2];

	pApp->motionParameters.GetMappingParameters(m_nNoOfBlock[0], m_nNoOfBlock[1], m_nDeviceNoPerBlock[0], m_nDeviceNoPerBlock[1]);
*/
	nDeviceNoPerBlock = (m_nDeviceNoPerBlock[1] > MAX_TRACKS) ? MAX_TRACKS : m_nDeviceNoPerBlock[1];
	for(int i=0; i<m_nPosSelectionCount; i++) {
		m_bPosSelection[i] = FALSE;
		if((i%nDeviceNoPerBlock)==0)
			nTempMul++;
		nSel = (nTempMul*(MAX_DIE_FOVS/MAX_TRACKS))+(i%nDeviceNoPerBlock);
		GetDlgItem(IDC_SM_SEL_STATIC1 + nSel)->GetWindowRect(&rectDie);		
		ScreenToClient(&rectDie);
		m_prgnPositionSelection[i].CreateRoundRectRgn(rectDie.left-2, rectDie.top-2, rectDie.right+2, rectDie.bottom+2, 2, 2);
	}
	CRect rectTemp;
	GetDlgItem(IDC_SM_DIE_MAP_SEL_GRP)->GetWindowRect(&rectTemp);
	ScreenToClient(&rectTemp);
	int nFovWidthRgn = (rectTemp.Width() / pApp->m_nNoOfFovX )-5;
	int nFovHeightRgn = (rectTemp.Height() / pApp->m_nNoOfFovY)-5;
	int nXPos = rectTemp.left + 15;
	int nYPos = rectTemp.top + 25;
	int nTemp = 0;
	for(int j=0; j<pApp->m_nNoOfFovY; j++) {
		for (int i = 0; i < pApp->m_nNoOfFovX; i++) {
			m_bDieMapPosSel[nTemp] = (nTemp>0) ? FALSE : TRUE;
			m_diePicCtrl[nTemp].SetWindowPos(NULL, nXPos + (i*nFovWidthRgn),(j==0?nYPos: nYPos-15) + (j*nFovHeightRgn), nFovWidthRgn -5, nFovWidthRgn -5, SWP_NOZORDER);
			m_diePicCtrl[nTemp].GetWindowRect(&rectDieMap);
			ScreenToClient(&rectDieMap);
			m_prgnDieMapPositionSel[nTemp].CreateRoundRectRgn(rectDieMap.left - 2, rectDieMap.top - 2, rectDieMap.right + 2, rectDieMap.bottom + 2, 2, 2);
			nTemp++;
		}
		
	}

	/*for(int k=0; k<MAX_DIE_MAP_FOVS; k++) {
		CString strText;
		strText.Format("FOV%d", k+1);
		GetDlgItem(IDC_SM_DIE_MAP_SEL_STATIC1 + k)->SetWindowTextA(strText);
		GetDlgItem(IDC_SM_DIE_MAP_SEL_STATIC1 + k)->ShowWindow((k<m_nDieMapSelCount)?TRUE:FALSE);
	}*/

	GetDlgItem(IDC_SM_SELECTION_GRP)->GetWindowRect(&m_rectPositionSelections);		
	ScreenToClient(&m_rectPositionSelections);
	GetDlgItem(IDC_SM_DIE_MAP_SEL_GRP)->GetWindowRect(&m_rectDieMapPosSel);		
	ScreenToClient(&m_rectDieMapPosSel);	
	
	SetTimer(1, 250, NULL);

	m_bLightDlgOpen = FALSE;
	m_nCurSelectedDieMapPosID = 0;
	m_ctrlCameraID.SetCurSel(0);
	StationMappingFile();
	ResetListBoxContents();
	pApp->ScannedFOVSequenceData(TRUE);
	for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++)
		pApp->m_pTracks[nTrack].m_nFOVIndex = m_nCurSelectedDieMapPosID;//0;
	//ScrollBarEnable(TRUE);
}

void CStationMapping::ClearAll()
{
	for (int nTrack = 0; nTrack<3; nTrack++) {

		int nCount = m_strDieMapContents[nTrack].GetCount();
		if (!m_strDieMapContents[nTrack].IsEmpty()) {
			m_strDieMapContents[nTrack].RemoveAll();
		}
		m_strPulseValue[nTrack].RemoveAll();
		for (int a = 0; a<8; a++)
			m_strDiePosIDContents[nTrack][a].RemoveAll();
	}
	m_ctrlDieMap.ResetContent();
	m_ctrlDieMapPosID.ResetContent();
}

void CStationMapping::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	CDC* pDC = &dc;
	
	for(int x=0; x<m_nPosSelectionCount; x++) {
		pDC->FillRgn(&m_prgnPositionSelection[x], m_bPosSelection[x] ? &m_brushCheck : &m_brushUncheck);
		pDC->FrameRgn(&m_prgnPositionSelection[x], &m_brushBorder, 1, 1);
	}
	for(int y=0; y<m_nDieMapSelCount; y++) {
		pDC->FillRgn(&m_prgnDieMapPositionSel[y], m_bDieMapPosSel[y] ? &m_brushCheck : &m_brushUncheck);
		pDC->FrameRgn(&m_prgnDieMapPositionSel[y], &m_brushBorder, 1, 1);
	}
}


void CStationMapping::ScrollBarEnable(BOOL bHorizontal)
{
	CString str;
    CSize sz;
    int dx = 0;
	CDC* pDC = m_ctrlDieMap.GetDC();
    for(int i=0; i<m_ctrlDieMap.GetCount(); i++){
        m_ctrlDieMap.GetText(i,str);
        sz = pDC->GetTextExtent(str);
        if(sz.cx > dx)
            dx = sz.cx;
    }
    m_ctrlDieMap.ReleaseDC(pDC);

    if(bHorizontal && m_ctrlDieMap.GetHorizontalExtent() < dx ){
        m_ctrlDieMap.SetHorizontalExtent(dx);
        ASSERT(m_ctrlDieMap.GetHorizontalExtent() == dx);
    }
}

//// *************************************************************************************** ////

void CStationMapping::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	BOOL bFoundPtInDieMap = FALSE;
	for(int i=0; i<m_nPosSelectionCount; i++) {
		if(m_prgnPositionSelection[i].PtInRegion(point)) {
			m_bPosSelection[i] = !m_bPosSelection[i];
			bFoundPtInDieMap = FALSE;
			break;
		}
	}

	for(int j=0; j<m_nDieMapSelCount; j++) {
		if(m_prgnDieMapPositionSel[j].PtInRegion(point)) {
			//m_bDieMapPosSel[j] = TRUE;//!m_bDieMapPosSel[j];
			bFoundPtInDieMap = TRUE;
			m_nCurSelectedDieMapPosID = j;
			int nTrack = m_ctrlCameraID.GetCurSel();
			pApp->m_pTracks[nTrack].m_nFOVIndex = j;
			break;
		}	
	}
	if(bFoundPtInDieMap) {
		for(int j=0; j<m_nDieMapSelCount; j++)
			m_bDieMapPosSel[j] = FALSE;
		m_bDieMapPosSel[m_nCurSelectedDieMapPosID] = TRUE;
		ResetListBoxContents();
	}

	InvalidateRect(!bFoundPtInDieMap ? &m_rectPositionSelections : m_rectDieMapPosSel);

	CDialogEx::OnLButtonDown(nFlags, point);
}


//void CStationMapping::OnRButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CDialogEx::OnRButtonDown(nFlags, point);
//}

void CStationMapping::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	int nTrack = m_ctrlCameraID.GetCurSel();
	for(int j=0; j<m_nDieMapSelCount; j++) {
		if(m_prgnDieMapPositionSel[j].PtInRegion(point)) {
			if(nTrack==(pApp->m_nTotalTracks-1) && j>0 && j<m_nDieMapSelCount-1) {
				return;
			}
			else {
				//if(AfxMessageBox("Confirm to move to positions and set light", MB_YESNO) == IDYES) {
					MoveToFOVPosAndSetLight();
				//	break;
				//}
			}
		}
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

int CStationMapping::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex)
{
	// TODO: Add your message handler code here and/or call default

	int nRow = pListBox->GetCurSel();
	if(nKey == VK_UP)
		NavigateListBoxContents(nRow, FALSE, TRUE);
	else if(nKey == VK_DOWN)
		NavigateListBoxContents(nRow, TRUE, TRUE); 

	return CDialogEx::OnVKeyToItem(nKey, pListBox, nIndex);
}

void CStationMapping::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
//	int nCurPos[2];
//	if(pApp->m_bStationMapDlgOpen) {
//		if(pApp->m_pMotionControl) {
//			for(int i=0; i<pApp->m_nTotalTracks-1; i++) {
//				nCurPos[i] = pApp->m_pMotionControl->GetCurrentPos(i+2);
//				if(nCurPos[i] != m_nZMotorPos[i])
//					m_nZMotorPos[i] = nCurPos[i];
//			}
//		}
//		int nTrack = m_ctrlCameraID.GetCurSel();
//		if(nTrack < pApp->m_nTotalTracks-1)
//			m_nZMotorPosDisp = (int)(m_nZMotorPos[nTrack]/*/pApp->m_pMotionControl->m_dMuToPulses*/);
////		if(nTrack == pApp->m_nTotalTracks - 1)
////			m_nZMotorPosDisp = (int)(m_nZMotorPos[/*nTrack*/0]/*/pApp->m_pMotionControl->m_dMuToPulses*/);
//		//if(::IsWindow(m_hWnd))
//			UpdateData(FALSE);
//	}
//	CDialogEx::OnTimer(nIDEvent);
}

//// *************************************************************************************** ////

void CStationMapping::OnSelchangeSmCamidCombo()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nTrack = m_ctrlCameraID.GetCurSel();
	//m_dZMotorRelativeMoveDisp = m_dZMotorRelativeMove[nTrack];
	m_strZMotorRelativeMoveDisp.Format("%.2f", m_dZMotorRelativeMove[nTrack]);
	m_dZMotorScanMaxDisp = m_dZMotorScanMax[nTrack];
	((CButton*) GetDlgItem(IDC_SM_Z_MTR_SERVO_CHECK))->SetCheck(m_bRelativeCheck[nTrack]);
	for(int z=0; z<m_nDieMapSelCount; z++)
		m_bDieMapPosSel[z] = (z>0)?FALSE:TRUE;

	m_nCurSelectedDieMapPosID = 0;
	ResetListBoxContents();
	UpAndDownZMotorServoButton((nTrack==pApp->m_nTotalTracks-1)?FALSE:TRUE);
	UpdateData(FALSE);
	InvalidateRect(m_rectDieMapPosSel);
}


void CStationMapping::OnBnClickedSmSelectallButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL bSelect = ((CButton*) GetDlgItem(IDC_SM_SELECTALL_BUTTON))->GetCheck();
	for(int i=0; i<m_nPosSelectionCount; i++)
		m_bPosSelection[i] = bSelect;

	UpdateData(FALSE);
	InvalidateRect(&m_rectPositionSelections);
}

//// *************************************************************************************** ////

void CStationMapping::OnBnClickedSmDieMapAddMfcbutton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString str;
	int nTrack, nCount;
	nTrack = m_ctrlCameraID.GetCurSel();
	nCount = m_ctrlDieMap.GetCount();
	str.Format("%2d: Z 0 Light: 0 0 0 0 0 0 0 0", nCount+1);
	m_ctrlDieMap.AddString(str);
	m_strDieMapContents[nTrack].Add(str);

	if(nCount>0)
		m_ctrlDieMap.SetCurSel(nCount);

	nCount = m_strPulseValue[nTrack].GetSize();
	str.Format("%2d: Pulse: 0 0 0 0 0 0 0 0", nCount+1);
	m_strPulseValue[nTrack].Add(str);

	UpdateData(FALSE);
}


void CStationMapping::OnBnClickedSmDieMapRemoveMfcbutton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nSel, nTrack;
	nSel = m_ctrlDieMap.GetCurSel();
	nTrack = m_ctrlCameraID.GetCurSel();
	if(nSel<0)
		return;

	RemoveFromDieMapPosId(nTrack, nSel);
	m_ctrlDieMap.DeleteString(nSel);
	//m_strDieMapContents[nTrack].RemoveAt(nSel);

	CString str, strTemp, strTemp1;
	int nCount, nPos;	
	nCount = m_ctrlDieMap.GetCount();
	for(int i=0; i<nCount; i++) {
		str.Empty();
		m_ctrlDieMap.GetText(i, str);
		strTemp.Format("%2d", i+1);
		str.Replace(str.Mid(0, str.Find(':')), strTemp);
		m_ctrlDieMap.DeleteString(i);
		m_ctrlDieMap.AddString(str);
		//m_strDieMapContents[nTrack].RemoveAt(i);
		//m_strDieMapContents[nTrack].SetAt(i, str);
	}
	
	m_strDieMapContents[nTrack].RemoveAll();
	for(int j=0; j<m_ctrlDieMap.GetCount(); j++) {
		CString strTempAdd;
		m_ctrlDieMap.GetText(j, strTempAdd);
		m_strDieMapContents[nTrack].Add(strTempAdd);
	}

	if(nCount>1)
		m_ctrlDieMap.SetCurSel(nCount-1);

	nCount = m_strPulseValue[nTrack].GetSize();
	CStringArray strPulseTempArr;
	int nTempPulseCount=1;
	strPulseTempArr.Copy(m_strPulseValue[nTrack]);
	m_strPulseValue[nTrack].RemoveAll();
	for(int j=0; j<nCount; j++) {
		if(j==nSel)
			continue;
		str.Empty();
		str = strPulseTempArr.GetAt(j);
		strTemp.Format("%2d", nTempPulseCount++);
		str.Replace(str.Mid(0, str.Find(':')), strTemp);
		m_strPulseValue[nTrack].Add(str);
	}
	UpdateData(FALSE);
}


void CStationMapping::OnBnClickedSmDieMapRetakeMfcbutton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nSel, nTrack, nCount, nChannel, nPulseVal;
	int nTempCount, nTempDoc;
	double dVal;
	CString str, strPulse;
	CString strTemp, strTempAppend="", strTempPulse="";
	nSel = m_ctrlDieMap.GetCurSel();
	if(nSel<0)
		return;
		
	nTrack = m_ctrlCameraID.GetCurSel();
	if(m_bLightDlgOpen) {
		int nStep = (MAX_TRACKS*nTrack)+0;
		pApp->m_LightHandler.UpdateLightStep(0, nStep, m_nCurSelectedDieMapPosID);//// Set
		for(int nView=0; nView<pApp->m_nTotalViewDocs[nTrack]; nView++) {
			nTempDoc = (MAX_TRACKS*nTrack)+ nView;
			nTempCount = pApp->m_LightHandler.m_nLightNoOfSegments[nTempDoc];
			for(int n=0; n<nTempCount; n++) {
				nChannel = pApp->m_pTracks[nTrack].m_nSegmentsOfImage[nView].GetAt(n);
				dVal = pApp->m_LightHandler.GetLight(0, nChannel, m_nCurSelectedDieMapPosID);				
				dVal = dVal*pApp->m_LightHandler.m_pMaxCurrents[nChannel] / 100;
				strTemp.Format(" %.2f", dVal);
				strTempAppend.Append(strTemp);
				nPulseVal = pApp->m_LightHandler.GetPulseWidth(0, nChannel, m_nCurSelectedDieMapPosID);
				strTempPulse.Format(" %d", nPulseVal);
				strPulse.Append(strTempPulse);
			}
		}
		pApp->m_LightHandler.UpdateLightStep(0, -2, m_nCurSelectedDieMapPosID); //// Reset
		nCount = m_ctrlDieMap.GetCount();
		m_ctrlDieMap.GetText(nSel, str);
		strTemp = str.Mid(0, str.ReverseFind(':')+1);
		strTemp += strTempAppend;
	}
	else {
		m_ctrlDieMap.GetText(nSel, strTemp);
	}

	CString strTempZPos, strTempLPos, strTempAdd;
	strTempZPos = strTemp.Mid(0, strTemp.Find('Z')+1);
	strTempLPos = strTemp.Mid(strTemp.Find('L'));
	if(pApp->m_pMotionControl) {
		if(nTrack < (pApp->m_nTotalTracks-1))
			strTempAdd.Format("%s %d %s", strTempZPos, (m_nZMotorPos[nTrack]/*/pApp->m_pMotionControl->m_dMuToPulses*/), strTempLPos);
		else {
			strTempAdd.Format("%s 0 %s", strTempZPos, strTempLPos);
			//strTempAdd.Format("%s %d %s", strTempZPos, (m_nZMotorPos[/*nTrack*/0]/*/pApp->m_pMotionControl->m_dMuToPulses*/), strTempLPos);
		}
	}
	m_ctrlDieMap.DeleteString(nSel);
	m_ctrlDieMap.AddString(strTempAdd);
	//m_strDieMapContents[nTrack].RemoveAt(nSel);
	m_strDieMapContents[nTrack].SetAt(nSel, strTempAdd);

	ScrollBarEnable(TRUE);
	//if(nCount>1)
		m_ctrlDieMap.SetCurSel(nSel);
	
	if(m_bLightDlgOpen) {
		nCount = m_strPulseValue[nTrack].GetSize();
		str = m_strPulseValue[nTrack].GetAt(nSel);
		strTemp = str.Mid(0, str.ReverseFind(':')+1);
		strTemp += strPulse;
		m_strPulseValue[nTrack].SetAt(nSel, strTemp);
		m_bLightDlgOpen = FALSE;
	}
	UpdateData(FALSE);
}

//void CStationMapping::OnDblclkSmDieMapList()
void CStationMapping::OnSelchangeSmDieMapList()
{
	// TODO: Add your control notification handler code here

	int nSel, nTrack;
	nSel = m_ctrlDieMap.GetCurSel();
	nTrack = m_ctrlCameraID.GetCurSel();
	if(nSel<0 /*|| nTrack==(pApp->m_nTotalTracks-1)*/)
		return;

	//if(AfxMessageBox("Confirm to move to Z position and set light", MB_YESNO) == IDYES) {
		if(!pApp->m_pTracks[nTrack].m_bCamAvail) {
			return;
		}

		UpdateStatusBar(&CString("Updating Z Position And Light..."));
		SetCursor(LoadCursor(NULL, IDC_WAIT));

		for(int i=0; i<pApp->m_nTotalTracks; i++) {
			for(int j=0; j<pApp->m_nTotalViewDocs[i]; j++) {
				if(pApp->m_pTracks[i].m_bLiveImage)
					pApp->m_pTracks[i].m_pDoc[j]->CameraLive();
			}
		}

		CString str, strPulse;
		m_ctrlDieMap.GetText(nSel, str);
		strPulse = m_strPulseValue[nTrack].GetAt(nSel);

		pApp->m_pFrames[!nTrack?0:1/*?pApp->m_nTop:pApp->m_nTop+pApp->m_nBottom)*/]->MDIActivate();
		int nStep;
		nStep = (MAX_TRACKS*nTrack)+0;
		pApp->m_LightHandler.UpdateLightStep(0, nStep, m_nCurSelectedDieMapPosID);	//// Set
		SetLightValues(nTrack, pApp->m_nTotalViewDocs[nTrack], m_nCurSelectedDieMapPosID, str, strPulse);
		pApp->m_LightHandler.UpdateLightStep(0, -2, m_nCurSelectedDieMapPosID); //// Reset
		SetZPositionValues(nTrack, m_nCurSelectedDieMapPosID, 0, str);
		
		if(nTrack < (pApp->m_nTotalTracks-1))//// NO Z Movement for Track 2 / Side Cam
			pApp->m_pTracks[nTrack].SetZPositionForSequence(0);
		int nErr = 0;//pApp->WaitForZPositionEnd(/*nTrack*/);
		if(!nErr)
			pApp->m_pTracks[nTrack].m_pDoc[0]->CameraGrabUsingExposure();

		SetCursor(LoadCursor(NULL, IDC_ARROW));
		UpdateStatusBar(&CString(""));
	//}
}


void CStationMapping::OnBnClickedSmDieMapSwapMfcbutton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString str, strTemp, strTemp2;
	int nSel, nCount, nTrack;
	nTrack = m_ctrlCameraID.GetCurSel();
	nSel = m_ctrlDieMap.GetCurSel();
	if(nSel<0 /*||  (nTrack==2 && m_nCurSelectedDieMapPosID > 0 && m_nCurSelectedDieMapPosID < m_nDieMapSelCount-1)*/) 
	{
		if(nSel<0)
			AfxMessageBox("Please select to transfer!");		
		return;
	}
	
	nCount = m_ctrlDieMapPosID.GetCount();
	if(nCount < pApp->m_nTotalViewDocs[nTrack]) {
		m_ctrlDieMap.GetText(nSel, str);
		strTemp = str.Mid(0, str.Find(':'));
		if(!nCount) {
			m_ctrlDieMapPosID.AddString(strTemp);
			m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].Add(strTemp);
		}
		else {
			int nFound = m_ctrlDieMapPosID.FindString(0, strTemp);
			if(nFound<0) {
				m_ctrlDieMapPosID.AddString(strTemp);
				m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].Add(strTemp);
			}
		}
	}
	UpdateData(FALSE);
}


void CStationMapping::OnBnClickedSmDieMapPosmoveSetlightButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//if(AfxMessageBox("Confirm to move to positions and set light", MB_YESNO) == IDYES) {
		MoveToFOVPosAndSetLight();
	//}
	UpdateData(FALSE);
}


void CStationMapping::OnBnClickedSmDieMapPosidRemoveMfcbutton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nSel, nTrack;
	nSel = m_ctrlDieMapPosID.GetCurSel();
	nTrack = m_ctrlCameraID.GetCurSel();
	if(nSel>=0) {
		m_ctrlDieMapPosID.DeleteString(nSel);
		m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].RemoveAt(nSel);
	}
	UpdateData(FALSE);
}


void CStationMapping::OnBnClickedSmDieMapLivecamLightButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nTrack = m_ctrlCameraID.GetCurSel();
	if(!pApp->m_bLightCtrlDlgOpen) {
		int nSel, nCount;
		nCount = m_ctrlDieMap.GetCount();
		if(!nCount) {
			AfxMessageBox("Please add light values!");
			return;
		}

		nSel = m_ctrlDieMap.GetCurSel();
		if(nSel<0) {
			//nSel = 0;
			AfxMessageBox("Please select for light values!");
			return;
		}

		CString str, strPulse;
		m_ctrlDieMap.GetText(nSel, str);
		strPulse = m_strPulseValue[nTrack].GetAt(nSel);
		pApp->m_pFrames[!nTrack?0:1/*(nTrack==1?pApp->m_nTop:pApp->m_nBottom)*/]->MDIActivate();
		int nStep = (MAX_TRACKS*nTrack)+0;
		pApp->m_LightHandler.UpdateLightStep(0, nStep, m_nCurSelectedDieMapPosID);	//// Set
		SetLightValues(nTrack, pApp->m_nTotalViewDocs[nTrack], m_nCurSelectedDieMapPosID, str, strPulse);
		pApp->m_LightHandler.UpdateLightStep(0, -2, m_nCurSelectedDieMapPosID); //// Reset
		pApp->m_pTracks[nTrack].m_pDoc[0]->OnCameraLight();
	}

	UpdateData(FALSE);
}

void CStationMapping::OnBnClickedSmNeglectMfcbutton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString str, strTemp, strTemp2;
	int nCount, nTrack;
	nTrack = m_ctrlCameraID.GetCurSel();
	if((nTrack==2 && m_nCurSelectedDieMapPosID > 0 && m_nCurSelectedDieMapPosID < m_nDieMapSelCount-1))
		return;

	nCount = m_ctrlDieMapPosID.GetCount();
	if(nCount < pApp->m_nTotalViewDocs[nTrack]) {
		strTemp = " -";
		m_ctrlDieMapPosID.AddString(strTemp);
		m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].Add(strTemp);
	}
	UpdateData(FALSE);
}

//// *************************************************************************************** ////

void CStationMapping::OnBnClickedSmSaveAllTeachButton()		//// Teach FOV Sequeunce 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nTrack = m_ctrlCameraID.GetCurSel();
	//pApp->m_pFrames[!nTrack?0:(nTrack==1?pApp->m_nTop:pApp->m_nBottom)]->MDIActivate();
	//pApp->m_pTracks[nTrack].m_pDoc[0]->CameraLive();
	if(pApp->m_pMotionControl && !pApp->m_bMotionCtrlDlgOpen) {
		pApp->OnMotionControl();
	}

	UpdateData(FALSE);
}


void CStationMapping::OnBnClickedSmSaveOneTeachButton()	//// Pos IDs contents move down
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nTrack = m_ctrlCameraID.GetCurSel();
	int nRow = m_ctrlDieMapPosID.GetCurSel();
	NavigateListBoxContents(nRow, TRUE);

	UpdateData(FALSE);
}


void CStationMapping::OnBnClickedSmLoadTeachButton()	//// Pos IDs contents move up
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nTrack = m_ctrlCameraID.GetCurSel();
	int nRow = m_ctrlDieMapPosID.GetCurSel();
	NavigateListBoxContents(nRow, FALSE);

	UpdateData(FALSE);
}


void CStationMapping::OnBnClickedSmSetImgIdButton()		//// Save FOV Sequence 
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//int nTrack = m_ctrlCameraID.GetCurSel();	
	//if(pApp->m_pMotionControl /*&& nTrack < (pApp->m_nTotalTracks-1)*/) {
	//	pApp->m_nXAxisPosition[/*m_nCurSelectedDieMapPosID*/0] = pApp->m_pMotionControl->GetCurrentPos(0);
	//	pApp->m_nYAxisPosition[/*m_nCurSelectedDieMapPosID*/0] = pApp->m_pMotionControl->GetCurrentPos(1);
	//	for(int nDoc=0; nDoc<pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
	//		CString strTemp="";
	//		if(nDoc < m_strDiePosIDContents[nTrack][0].GetCount()) 
	//			strTemp = m_strDiePosIDContents[nTrack][0].GetAt(nDoc);
	//		
	//		if(!strTemp.IsEmpty())
	//			SetZPositionValues(nTrack, 0, nDoc, strTemp);
	//		else {
	//			if(nTrack < (pApp->m_nTotalTracks - 1))
	//				pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_nZAxisPosition[0] = pApp->m_pMotionControl->GetCurrentPos(nTrack + 2);
	//			//else
	//			//	pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_nZAxisPosition[0] = pApp->m_pMotionControl->GetCurrentPos(/*nTrack +*/ 2);
	//		}
	//	}
	//}
	//pApp->ScannedFOVSequenceData(FALSE);

	//UpdateData(FALSE);
}


void CStationMapping::OnUpdateSmStartEdit()			//// Current Pos
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);

	//int nTrack = m_ctrlCameraID.GetCurSel();
	//m_nZMotorPos[nTrack] = m_nZMotorPosDisp;

	///UpdateData(FALSE);
}


void CStationMapping::OnUpdateSmMaxEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);

	int nTrack = m_ctrlCameraID.GetCurSel();
	m_dZMotorScanMax[nTrack] = m_dZMotorScanMaxDisp;

	//UpdateData(FALSE);
}

//// *************************************************************************************** ////

void CStationMapping::OnUpdateSmZMtrServoEdit()		//// Realtive Move
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);

	int nTrack = m_ctrlCameraID.GetCurSel();
	//m_dZMotorRelativeMove[nTrack] = m_dZMotorRelativeMoveDisp;
	m_dZMotorRelativeMove[nTrack] = atof(m_strZMotorRelativeMoveDisp);//m_dZMotorRelativeMoveDisp;

	//UpdateData(FALSE);
}

void CStationMapping::OnClickedSmZMtrServoCheck()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nTrack = m_ctrlCameraID.GetCurSel();
	m_bRelativeCheck[nTrack] = ((CButton*) GetDlgItem(IDC_SM_Z_MTR_SERVO_CHECK))->GetCheck();

	//UpdateData(FALSE);
}


void CStationMapping::OnBnClickedSmZMtrServoUpMfcbutton()
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//int nTrack = m_ctrlCameraID.GetCurSel();
	//if (!pApp->m_pTracks[nTrack].m_bCamAvail || (pApp->m_strMotionCard.CompareNoCase("none") == 0))
	//	return;

	//if(pApp->m_pMotionControl) {
	//	pApp->m_pMotionControl->m_bSnapEnable = TRUE;
	//	pApp->m_pMotionControl->nTrack = nTrack;
	//	if (nTrack == pApp->m_nTotalTracks - 1)
	//		nTrack = 0;
	//	pApp->m_pMotionControl->m_dStepMM[nTrack+2] = atof(m_strZMotorRelativeMoveDisp);//m_dZMotorRelativeMoveDisp;
	//	double dValue = (pApp->m_pMotionControl->m_dMotorResolution/(double)pApp->m_pMotionControl->GetMMPerSec(nTrack+2));
	//	//dValue = (dValue*m_dZMotorRelativeMoveDisp*10)/10.0;	//// round off
	//	dValue = (dValue*atof(m_strZMotorRelativeMoveDisp)*10)/10.0;	//// round off
	//	pApp->m_pMotionControl->m_nStep[nTrack+2] = (int)dValue;
	//	pApp->m_pMotionControl->SetCommand(!nTrack ? MOVE_UP_TOP_Z : MOVE_UP_BOTTOM_Z);
	//}
	//
	//UpdateData(FALSE);
}


void CStationMapping::OnBnClickedSmZMtrServoDownMfcbutton()
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//int nTrack = m_ctrlCameraID.GetCurSel();
	//if (!pApp->m_pTracks[nTrack].m_bCamAvail || (pApp->m_strMotionCard.CompareNoCase("none") == 0))
	//	return;

	//if(pApp->m_pMotionControl) {
	//	pApp->m_pMotionControl->m_bSnapEnable = TRUE;
	//	pApp->m_pMotionControl->nTrack = nTrack;
	//	if (nTrack == pApp->m_nTotalTracks - 1)
	//		nTrack = 0;
	//	pApp->m_pMotionControl->m_dStepMM[nTrack+2] = atof(m_strZMotorRelativeMoveDisp);//m_dZMotorRelativeMoveDisp;
	//	double dValue = (pApp->m_pMotionControl->m_dMotorResolution/(double)pApp->m_pMotionControl->GetMMPerSec(nTrack+2));
	//	//dValue = (dValue*m_dZMotorRelativeMoveDisp*10)/10.0;	//// round off
	//	dValue = (dValue*atof(m_strZMotorRelativeMoveDisp)*10)/10.0;	//// round off
	//	pApp->m_pMotionControl->m_nStep[nTrack+2] = (int)dValue;
	//	pApp->m_pMotionControl->SetCommand(!nTrack ? MOVE_DOWN_TOP_Z : MOVE_DOWN_BOTTOM_Z);
	//}
	//UpdateData(FALSE);
}

//// *************************************************************************************** ////

void CStationMapping::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateStatusBar(&CString("Updating All Values..."));
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	pApp->m_bStationMapDlgOpen = FALSE;
	pApp->m_bAllowStream = FALSE;
	KillTimer(1);
	StationMappingFile(FALSE);

//// Saving All Tracks - Docs - FOVs Set Light Values to Light Files(.ltv) on Click OK ////
	CString strTemp, str, strPulse;
	int nSel, nDiePosCount, nDocCount, nTempCount;
	int nPrevZValue;
	for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++) {
		nDocCount = pApp->m_nTotalViewDocs[nTrack];
		for(int nFOV=0; nFOV<m_nDieMapSelCount; nFOV++) {
			nDiePosCount = m_strDiePosIDContents[nTrack][nFOV].GetCount();
			nTempCount = (nDocCount <= nDiePosCount) ? nDocCount : nDiePosCount;
			for(int nDoc=0; nDoc<nDocCount; nDoc++) {
				nSel = 0;
				str = "";
				strPulse = "";
				if (nDoc < nTempCount) {
					strTemp = m_strDiePosIDContents[nTrack][nFOV].GetAt(nDoc);
					if (atoi(strTemp)) {
						nSel = atoi(strTemp);
						//m_ctrlDieMap.GetText(nSel-1, str);
						str = m_strDieMapContents[nTrack].GetAt(nSel - 1);
						strPulse = m_strPulseValue[nTrack].GetAt(nSel - 1);
					}
					else {
						nSel = 0;
						str.Format("%2d: Z %d Light: 0 0 0 0 0 0 0 0", nDoc + 1, nPrevZValue);
						strPulse.Format("%2d: Pulse: 0 0 0 0 0 0 0 0", nDoc + 1);
						//bGrab = FALSE;
					}
				}
				else {
					if (nTrack < pApp->m_nTotalTracks - 1) {
						nSel = 0;
						str.Format("%2d: Z %d Light: 0 0 0 0 0 0 0 0", nDoc + 1, nPrevZValue);
						strPulse.Format("%2d: Pulse: 0 0 0 0 0 0 0 0", nDoc + 1);
					}
				}
				int nStep = (MAX_TRACKS*nTrack)+nDoc;
				pApp->m_LightHandler.UpdateLightStep(0, nStep, nFOV);	//// Set
				SetLightValues(nTrack, nDocCount, nFOV, str, strPulse);
				pApp->m_LightHandler.SaveLightSettings(nDoc, /*m_nCurSelectedDieMapPosID*/nFOV, nTrack);
				pApp->m_LightHandler.UpdateLightStep(0, -2, nFOV); //// Reset
				nPrevZValue = SetZPositionValues(nTrack, nFOV, nDoc, str);
			}
		}
	}
	pApp->ScannedFOVSequenceData(FALSE);

	/*for (int nFOV = 0; nFOV < pApp->m_nNoOfFov; nFOV++) {
		strTemp.Format("Position = %d,XPos = %d", nFOV + 1, pApp->m_nXAxisPosition[nFOV]);
		pApp->AddLogEvent(strTemp, MOTION_LOG);
		strTemp.Format("Position = %d,YPos = %d", nFOV + 1, pApp->m_nYAxisPosition[nFOV]);
		pApp->AddLogEvent(strTemp, MOTION_LOG);
		for (int nTrack = 0; nTrack < pApp->m_nTotalTracks - 1; nTrack++) {
			for (int nDoc = 0; nDoc < pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
				strTemp.Format("T%d FOV%d Doc%d %s = %d", nTrack + 1, nFOV + 1, nDoc + 1, !nTrack ? "TopZPos" : "BotZPos", pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_nZAxisPosition[nFOV]);
				pApp->AddLogEvent(strTemp, MOTION_LOG);
			}
		}
	}*/
	//pApp->m_LightHandler.RefreshLight();
	//pApp->m_LightHandler.ResetLightSteps();
	SetEvent(pApp->m_EventRefreshLight);//// Set Event for Refresh Light

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	UpdateStatusBar(&CString(""));
	UpdateData(FALSE);

	CDialogEx::OnOK();
}


void CStationMapping::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateStatusBar(&CString("Updating All Values..."));
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	pApp->m_bStationMapDlgOpen = FALSE;
	pApp->m_bAllowStream = FALSE;
	KillTimer(1);

	for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++) {
		for(int nFOV=0; nFOV<pApp->m_nNoOfFov; nFOV++) {
			for(int nDoc=0; nDoc<pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
				pApp->m_LightHandler.LoadLightSettings(nDoc, nFOV, nTrack);
			}
		}
	}
	//pApp->m_LightHandler.RefreshLight();
	//pApp->m_LightHandler.ResetLightSteps();
	SetEvent(pApp->m_EventRefreshLight);//// Set Event for Refresh Light

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	UpdateStatusBar(&CString(""));
	UpdateData(FALSE);

	CDialogEx::OnCancel();
}


void CStationMapping::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);
	UpdateStatusBar(&CString("Updating All Values..."));
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	pApp->m_bStationMapDlgOpen = FALSE;	
	pApp->m_bAllowStream = FALSE;
	KillTimer(1);

	for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++) {
		for(int nFOV=0; nFOV<pApp->m_nNoOfFov; nFOV++) {
			for(int nDoc=0; nDoc<pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
				pApp->m_LightHandler.LoadLightSettings(nDoc, nFOV, nTrack);
			}
		}
	}
	//pApp->m_LightHandler.RefreshLight();
	//pApp->m_LightHandler.ResetLightSteps();
	SetEvent(pApp->m_EventRefreshLight);//// Set Event for Refresh Light

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	UpdateStatusBar(&CString(""));
	UpdateData(FALSE);

	CDialogEx::OnClose();
}

//// *************************************************************************************** ////

void CStationMapping::UpAndDownZMotorServoButton(BOOL bEnable)
{
	GetDlgItem(IDC_SM_Z_MTR_SERVO_UP_MFCBUTTON)->EnableWindow(bEnable);
	GetDlgItem(IDC_SM_Z_MTR_SERVO_DOWN_MFCBUTTON)->EnableWindow(bEnable);
	GetDlgItem(IDC_SM_SAVE_ALL_TEACH_BUTTON)->EnableWindow(bEnable);
}

void CStationMapping::ResetListBoxContents()
{
	int nTrack, nSel;
	nTrack = m_ctrlCameraID.GetCurSel();

	m_ctrlDieMap.ResetContent();
	m_ctrlDieMapPosID.ResetContent();

	/*if(nTrack==2 && m_nCurSelectedDieMapPosID > 0 && m_nCurSelectedDieMapPosID < m_nDieMapSelCount-1)
		return;*/

	for(int a=0; a<m_strDieMapContents[nTrack].GetCount(); a++)
		m_ctrlDieMap.AddString(m_strDieMapContents[nTrack].GetAt(a));

	for(int b=0; b<m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].GetCount(); b++)
		m_ctrlDieMapPosID.AddString(m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].GetAt(b));
}

void CStationMapping::NavigateListBoxContents(int nRow, BOOL bDown, BOOL bKeyDown)
{
	CString strListBox;
	int nRowChange, nTrack, nCount;
	BOOL bNavigate = FALSE;
	
	nRowChange = bDown ? nRow+1 : nRow-1;
	nTrack = m_ctrlCameraID.GetCurSel();
	nCount = m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].GetCount();

	if(!bDown && nRow > 0 && nRow <= nCount-1)
		bNavigate = TRUE;
	else if(bDown && nRow >= 0 && nRow < nCount-1)
		bNavigate = TRUE;

	if(bNavigate) {
		strListBox = m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].GetAt(nRow);
		m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].SetAt(nRow, m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].GetAt(nRowChange));
		m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].SetAt(nRowChange, strListBox);
		
		m_ctrlDieMapPosID.ResetContent();
		for(int i=0; i<nCount; i++) {
			m_ctrlDieMapPosID.AddString(m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].GetAt(i));
		}
		m_ctrlDieMapPosID.SetCurSel(bKeyDown ? nRow : nRowChange);
	}
}

void CStationMapping::RemoveFromDieMapPosId(int nTrack, int nSel)
{
	CString str, strTemp, strVal;

	m_ctrlDieMap.GetText(nSel, str);
	strTemp = str.Mid(0, str.Find(':'));

	//for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++) {
		for(int a=0; a<m_nDieMapSelCount; a++) {
			for(int b=0; b<m_strDiePosIDContents[nTrack][a].GetCount(); b++) {
				strVal = m_strDiePosIDContents[nTrack][a].GetAt(b);
				if(strTemp.CompareNoCase(strVal)==0) {
					m_strDiePosIDContents[nTrack][a].RemoveAt(b);
					OutputDebugLogTo(1,TRUE,"[Camera%d][FOV%d] Die Map Position Id %s REMOVED", nTrack+1, a+1, strTemp);
				}
			}
		}
	//}
	
	ResetListBoxContents();
}

void CStationMapping::MoveToFOVPosAndSetLight()
{
	CString strTemp, str, strPulse;
	int nTrack, nSel, nStep, nErr;
	int DiePosCount, nDocCount, nTempCount;

	nTrack = m_ctrlCameraID.GetCurSel();
	if (!pApp->m_pTracks[nTrack].m_bCamAvail) {
		return;
	}

	pApp->m_pTracks[nTrack].m_pDoc[0]->DrawView(FALSE);
	UpdateStatusBar(&CString("Updating Positions And Light..."));
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++)
		pApp->m_pTracks[nTrack].m_nFOVIndex = m_nCurSelectedDieMapPosID;//0;

	nDocCount = pApp->m_nTotalViewDocs[nTrack];
	DiePosCount = m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].GetCount();
	nTempCount = (nDocCount <= DiePosCount) ? nDocCount : DiePosCount;
	pApp->m_pFrames[!nTrack ? 0 :1 /*(nTrack == 1 ? pApp->m_nTop : pApp->m_nBottom)*/]->MDIActivate();

	/*for (int i = 0; i<pApp->m_nTotalTracks; i++) {
		for (int j = 0; j<pApp->m_nTotalViewDocs[i]; j++) {
			if (pApp->m_pTracks[i].m_bLiveImage)
				pApp->m_pTracks[i].m_pDoc[j]->CameraLive();
		}
	}*/

	SetFOVPositionToMotionCtrl(nTrack, m_nCurSelectedDieMapPosID, FALSE);
	nErr = 0;
	pApp->m_pTracks[nTrack].m_pDoc[0]->CameraGrabUsingExposure();


	//for(int nDoc=0; nDoc<nDocCount; nDoc++) {
	//			nSel = 0;
	//			str = "";
	//			strPulse = "";
	//	if(nDoc < nTempCount) {
	//		strTemp = m_strDiePosIDContents[nTrack][m_nCurSelectedDieMapPosID].GetAt(nDoc);
	//		if(atoi(strTemp)) {
	//			nSel = atoi(strTemp);
	//			//m_ctrlDieMap.GetText(nSel-1, str);
	//			str = m_strDieMapContents[nTrack].GetAt(nSel-1);
	//			strPulse = m_strPulseValue[nTrack].GetAt(nSel-1);
	//		}
	//		else {
	//			nSel = 0;
	//			str = "";
	//			strPulse = "";
	//		}
	//	}
	//		nStep = (MAX_TRACKS*nTrack)+nDoc;
	//		pApp->m_LightHandler.UpdateLightStep(0, nStep, m_nCurSelectedDieMapPosID);	//// Set
	//		SetLightValues(nTrack, pApp->m_nTotalViewDocs[nTrack], m_nCurSelectedDieMapPosID, str, strPulse);
	//		pApp->m_LightHandler.UpdateLightStep(0, -2, m_nCurSelectedDieMapPosID); //// Reset
	//		SetZPositionValues(nTrack, m_nCurSelectedDieMapPosID, nDoc, str);

	//		pApp->m_pTracks[nTrack].SetZPositionForSequence(nDoc);
	//		nErr = 0;//pApp->WaitForZPositionEnd(/*nTrack*/);
	//		if(!nErr)
	//			OutputDebugLogTo(9,TRUE,"CStationMapping::MoveToFOVPosAndSetLight WaitForPositionEnd Error");
	//}

	SetCursor(LoadCursor(NULL, IDC_ARROW));
	UpdateStatusBar(&CString(""));
}

void CStationMapping::StationMappingFile(BOOL bRead)
{
	CIniFile iniFile;
	CFileException e;

	CString strKey, strFile, strTemp, strGetVal="";
	int nCamId, nTempCount=-1;
	BOOL bCheck=FALSE;

	UpdateData(TRUE);
	//strFile.Format("%s\\ScanningSequenceMapping.dat", pApp->m_strConfigDir);
	strFile.Format("%s\\%s\\%s\\ScanningSequenceMapping.dat", pApp->m_strMainCfgDir, pApp->m_strMainCfgModeDir, pApp->m_strConfigFile);
	iniFile.SetPath(strFile);
	
	strKey.Format("StationMap");
	if(bRead && iniFile.ReadFile()) {
		ClearAll();

		nCamId = iniFile.GetInteger(strKey, "Camera ID", 0);
		for(int nCam=0; nCam<pApp->m_nTotalTracks; nCam++) {
			strTemp.Format("ServoMotorValue[%d]", nCam);
			m_dZMotorRelativeMove[nCam] = iniFile.GetDouble(strKey, strTemp, m_dZMotorRelativeMove[nCam]);
			strTemp.Format("MotorScanMax[%d]", nCam);			
			m_dZMotorScanMax[nCam] = iniFile.GetDouble(strKey, strTemp, m_dZMotorScanMax[nCam]);
			strTemp.Format("Relative Move Check[%d]", nCam);			
			m_bRelativeCheck[nCam] = iniFile.GetBoolean(strKey, strTemp, m_bRelativeCheck[nCam]);		
		}

		for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++) {
			for(int a=0; a<m_nDieMapSelCount; a++) {
				nTempCount = -1;// = m_strDiePosIDContents[a].GetCount();
				strTemp.Format("Die Pos IDs[%d][%d] Count:", nTrack+1, a+1);
				nTempCount = iniFile.GetInteger(strKey, strTemp, nTempCount);
				for(int b=0; b<nTempCount; b++) {
					strTemp.Format("Die Pos IDs[%d][%d][%d]:", nTrack+1, a+1, b+1);
					strGetVal = iniFile.GetCString(strKey, strTemp, strGetVal);
					m_strDiePosIDContents[nTrack][a].Add(strGetVal);
				}
			}

			nTempCount = -1;
			strTemp.Format("Die Pos Count[T%d]:", nTrack+1);
			nTempCount = iniFile.GetInteger(strKey, strTemp, nTempCount);
			for(int c=0; c<nTempCount; c++) {
				strTemp.Format("Die Pos Contents[T%d][%d]:", nTrack+1, c+1);
				strGetVal = iniFile.GetCString(strKey, strTemp, strGetVal);
				//m_ctrlDieMap.AddString(strGetVal);
				m_strDieMapContents[nTrack].Add(strGetVal);
			}

			nTempCount = -1;
			strTemp.Format("Die Pos Pulse Count[T%d]:", nTrack+1);
			nTempCount = iniFile.GetInteger(strKey, strTemp, nTempCount);
			for(int d=0; d<nTempCount; d++) {
				strTemp.Format("Die Pos Pulse Contents[T%d][%d]:", nTrack+1, d+1);
				strGetVal = iniFile.GetCString(strKey, strTemp, strGetVal);
				m_strPulseValue[nTrack].Add(strGetVal);
			}
		}

		//strTemp.Format("Die Pos Count:");
		//nTempCount = iniFile.GetInteger(strKey, strTemp, nTempCount);
		//for(int c=0; c<nTempCount; c++) {
		//	strTemp.Format("Die Pos Contents[%d]:", c+1);
		//	strGetVal = iniFile.GetCString(strKey, strTemp, strGetVal);
		//	m_ctrlDieMap.AddString(strGetVal);
		//}
		
		/*strTemp.Format("Die Pos Pulse Count:");
		nTempCount = iniFile.GetInteger(strKey, strTemp, nTempCount);
		for(int d=0; d<nTempCount; d++) {
			strTemp.Format("Die Pos Pulse Contents[%d]:", d+1);
			strGetVal = iniFile.GetCString(strKey, strTemp, strGetVal);
			m_strPulseValue.Add(strGetVal);
		}*/

		int m_nNoOfBlock[2];
		int m_nDeviceNoPerBlock[2];

		pApp->motionParameters.GetMappingParameters(m_nNoOfBlock[0], m_nNoOfBlock[1], m_nDeviceNoPerBlock[0], m_nDeviceNoPerBlock[1]);


		nTempCount = m_nDeviceNoPerBlock[0]*m_nDeviceNoPerBlock[1];
		if(nTempCount > MAX_DIE_FOVS)
			nTempCount = MAX_DIE_FOVS;
		strTemp.Format("Pos Selection Count:");
		nTempCount = iniFile.GetInteger(strKey, strTemp, nTempCount);
		for(int e=0; e<nTempCount; e++) {
			strTemp.Format("Pos Selection[%d]", e+1);
			m_bPosSelection[e] = iniFile.GetBoolean(strKey, strTemp, m_bPosSelection[e]);
		}

		m_ctrlCameraID.SetCurSel(nCamId);
		//m_dZMotorRelativeMoveDisp = m_dZMotorRelativeMove[nCamId];
		m_strZMotorRelativeMoveDisp.Format("%.2f", m_dZMotorRelativeMove[nCamId]);
		m_dZMotorScanMaxDisp = m_dZMotorScanMax[nCamId];
		((CButton*) GetDlgItem(IDC_SM_Z_MTR_SERVO_CHECK))->SetCheck(m_bRelativeCheck[nCamId]);
	}
	else {
		nCamId = m_ctrlCameraID.GetCurSel();
		iniFile.SetInteger(strKey, "Camera ID", nCamId);
		for(int nCam=0; nCam<pApp->m_nTotalTracks; nCam++) {
			strTemp.Format("ServoMotorValue[%d]", nCam);
			iniFile.SetDouble(strKey, strTemp, m_dZMotorRelativeMove[nCam]);			
			strTemp.Format("MotorScanMax[%d]", nCam);			
			iniFile.SetDouble(strKey, strTemp, m_dZMotorScanMax[nCam]);
			strTemp.Format("Relative Move Check[%d]", nCam);			
			iniFile.SetBoolean(strKey, strTemp, m_bRelativeCheck[nCam]);
		}

		for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++) {
			for(int a=0; a<m_nDieMapSelCount; a++) {
				int nTempCount = m_strDiePosIDContents[nTrack][a].GetCount();
				strTemp.Format("Die Pos IDs[%d][%d] Count:", nTrack+1, a+1);
				iniFile.SetInteger(strKey, strTemp, nTempCount);
				for(int b=0; b<nTempCount; b++) {
					strTemp.Format("Die Pos IDs[%d][%d][%d]:", nTrack+1, a+1, b+1);
					iniFile.SetCString(strKey, strTemp, m_strDiePosIDContents[nTrack][a].GetAt(b));
				}
			}

			nTempCount = m_strDieMapContents[nTrack].GetCount();//m_ctrlDieMap.GetCount();
			strTemp.Format("Die Pos Count[T%d]:", nTrack+1);
			iniFile.SetInteger(strKey, strTemp, nTempCount);
			for(int c=0; c<nTempCount; c++) {
				strTemp.Format("Die Pos Contents[T%d][%d]:", nTrack+1, c+1);
				//m_ctrlDieMap.GetText(c, strGetVal);
				strGetVal = m_strDieMapContents[nTrack].GetAt(c);
				iniFile.SetCString(strKey, strTemp, strGetVal);
			}

			nTempCount = m_strPulseValue[nTrack].GetSize();
			strTemp.Format("Die Pos Pulse Count[T%d]:", nTrack+1);
			iniFile.SetInteger(strKey, strTemp, nTempCount);
			for(int d=0; d<nTempCount; d++) {
				strTemp.Format("Die Pos Pulse Contents[T%d][%d]:", nTrack+1, d+1);
				strGetVal = m_strPulseValue[nTrack].GetAt(d);
				iniFile.SetCString(strKey, strTemp, strGetVal);
			}
		}

		//nTempCount = m_ctrlDieMap.GetCount();
		//strTemp.Format("Die Pos Count:");
		//iniFile.SetInteger(strKey, strTemp, nTempCount);
		//for(int c=0; c<nTempCount; c++) {
		//	strTemp.Format("Die Pos Contents[%d]:", c+1);
		//	m_ctrlDieMap.GetText(c, strGetVal);
		//	iniFile.SetCString(strKey, strTemp, strGetVal);
		//}

		/*nTempCount = m_strPulseValue.GetSize();
		strTemp.Format("Die Pos Pulse Count:");
		iniFile.SetInteger(strKey, strTemp, nTempCount);
		for(int d=0; d<nTempCount; d++) {
			strTemp.Format("Die Pos Pulse Contents[%d]:", d+1);
			strGetVal = m_strPulseValue.GetAt(d);
			iniFile.SetCString(strKey, strTemp, strGetVal);
		}*/

		strTemp.Format("Pos Selection Count:");
		iniFile.SetInteger(strKey, strTemp, m_nPosSelectionCount);
		for(int e=0; e<m_nPosSelectionCount; e++) {
			strTemp.Format("Pos Selection[%d]", e+1);
			iniFile.SetBoolean(strKey, strTemp, m_bPosSelection[e]);
		}
		iniFile.WriteFile();
	}
	UpAndDownZMotorServoButton((nCamId==pApp->m_nTotalTracks-1)?FALSE:TRUE);
	
	UpdateData(FALSE);
}

void CStationMapping::SetFOVPositionToMotionCtrl(int nTrack, int nFOV, BOOL bHome)
{
//	int nStartPos[2];
//	int nNewPos[2];
//	int nAxisX, nAxisY;
//	int nColFOV, nRowFOV;
//	BOOL nTempDieMap = TRUE;
//
//
//	//if (nFOV >= pApp->m_nNoOfFovX) {
//		nTempDieMap = FALSE;
//		nColFOV = nFOV%pApp->m_nNoOfFovX;
//		nRowFOV = nFOV/pApp->m_nNoOfFovX;
//	//}
//
//	nAxisX = 0;//// X-Axis
//	nAxisY = 1;//// Y-Axis
//	//if(pApp->m_nDeviceNoPerBlock[0] > pApp->m_nDeviceNoPerBlock[1]) {
//	//	nAxisX = 1;//// Y-Axis
//	//	nAxisY = 0;//// X-Axis
//	//}
//
//	for(int nAxis=0; nAxis<NO_OF_AXIS; nAxis++) {
////		if(pApp->m_pMotionControl->m_bServoDisable[nAxis])
//			pApp->m_pMotionControl->SetServo(pApp->m_nAxes[nAxis]);
//	}
//	
//	nStartPos[0] = abs(pApp->m_nXAxisPosition[0]);//// teach start Pos X
//	nStartPos[1] = abs(pApp->m_nYAxisPosition[0]);//// teach start Pos Y
//	
//	if (!nColFOV)
//		nNewPos[0] = pApp->m_nXAxisPosition[0];
//	else
//	{
//		
//		double dValue = (pApp->m_pMotionControl->m_dMotorResolution / (double)pApp->m_pMotionControl->GetMMPerSec(nAxisX));
//		double dVal = pApp->m_nFOVWidthCalculated - pApp->motionParameters.m_dDeviceOverlap[nAxisX];
//		double dMMToPPS = ((dValue*dVal) * 10) / 10.0;//// round off
//		nNewPos[nAxisX] = nStartPos[nAxisX] + (dMMToPPS*(double)nColFOV);
//		OutputDebugLogTo(3, TRUE, "nAxisX = %d,nNewPos[%d] = %d,StartPos[%d] = %d,DMMToPPS = %3.02f,nRowFov = %d ,dVal=%3.02f",
//			nAxisX, nAxisX, nNewPos[nAxisX], nAxisX, nStartPos[nAxisX], dMMToPPS, nColFOV);
//	}
//
//
//	if (!nRowFOV)
//		nNewPos[1] = pApp->m_nYAxisPosition[0];
//	else
//	{
//		double dValue = (pApp->m_pMotionControl->m_dMotorResolution / (double)pApp->m_pMotionControl->GetMMPerSec(nAxisY));
//		double dVal = pApp->m_nFOVHeightCalculated - pApp->motionParameters.m_dDeviceOverlap[nAxisY];
//		double dMMToPPS = ((dValue*dVal) * 10) / 10.0;	//// round ofYf
//		nNewPos[nAxisY] = nStartPos[nAxisY] + (dMMToPPS*(double)nRowFOV);
//		OutputDebugLogTo(3, TRUE, "nAxisY = %d,nNewPos[%d] = %d,StartPos[%d] = %d,DMMToPPS = %3.02f,nRowFov = %d",
//			nAxisY, nAxisY, nNewPos[nAxisY], nAxisY, nStartPos[nAxisY], dMMToPPS, nRowFOV);
//
//	}
//
//	//pApp->m_pMotionControl->SetCommand(bFwdMovement ? MOVE_RIGHT : MOVE_LEFT);
//	pApp->m_pMotionControl->m_nMoveTo[nAxisX] = !nColFOV ? nNewPos[nAxisX] : -nNewPos[nAxisX];
//	pApp->m_pMotionControl->m_bAxisGO[nAxisX] = TRUE;
//	
//	pApp->m_pMotionControl->AbsoluteMove(nAxisX, FALSE);
//	int nError = pApp->m_MotorResource.WaitForMotionDone(pApp->m_nAxes[nAxisX], INFINITE);
//	if(nError) {
//		OutputDebugLogTo(9, TRUE, "APS_absolute_move[Axis1] FAILED (Wait Error=%d) **********", nError);//// for testing
//	}
//	pApp->m_pMotionControl->m_bAxisGO[nAxisX] = FALSE;
//	Sleep(pApp->m_nXAxisSettlingTime);//// supposed to check and add settling time
//
//	pApp->m_pMotionControl->m_nMoveTo[nAxisY] = !nRowFOV ? nNewPos[nAxisY] : -nNewPos[nAxisY];
//	pApp->m_pMotionControl->m_bAxisGO[nAxisY] = TRUE;
//	pApp->m_pMotionControl->AbsoluteMove(nAxisY, FALSE);
//	nError = pApp->m_MotorResource.WaitForMotionDone(pApp->m_nAxes[nAxisY], INFINITE);
//	if(nError) {
//		OutputDebugLogTo(9, TRUE, "APS_absolute_move[Axis2] FAILED (Wait Error=%d) **********", nError);//// for testing
//	}
//	pApp->m_pMotionControl->m_bAxisGO[nAxisY] = FALSE;
//	Sleep(pApp->m_nYAxisSettlingTime);//// supposed to check and add settling time
}

int CStationMapping::SetZPositionValues(int nTrack, int nFOV, int nDoc, CString strSel)
{
	int nZValue = 0;
	int nTempLPos, nTempZPos;
	CString strTemp;

	if(!strSel.IsEmpty()) {
		nTempZPos = strSel.Find('Z')+1;
		nTempLPos = strSel.Find('L');
		strTemp = strSel.Mid(nTempZPos, (nTempLPos-nTempZPos));
		nZValue = atoi(strTemp);
		
	}
	pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_nZAxisPosition[nFOV] = nZValue;
	//OutputDebugLogTo(9,TRUE,"SetZPositionValues::[T%d][FOV%d][Doc%d]ZValue=%d", nTrack, nFOV, nDoc, pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_nZAxisPosition[nFOV]);
	return nZValue;
}

void CStationMapping::SetLightValues(int nTrack, int nDocCount, int nFOV, CString strSel, CString strPulse)
{
	int nController = 0;
//// Assign zero value ////

	int nTempChannelCount;
	//if(!pApp->m_bStationMapDlgOpen) {
		nTempChannelCount = pApp->m_LightHandler.m_bShareAllChannel ? pApp->m_LightHandler.m_nSegmentsPerTrack : pApp->m_nTotalTracks * pApp->m_LightHandler.m_nSegmentsPerTrack;
		for(int nTempChannel=0; nTempChannel<nTempChannelCount; nTempChannel++) {
			pApp->m_LightHandler.SetLight(nController, nTempChannel, 0, nFOV, FALSE);
			pApp->m_LightHandler.SetPulseWidth(nController, nTempChannel, 0, nFOV, FALSE);
		}
	//}

	int nChannel, nTokenPos; 
	int nTempDoc, nTempCount;
	CString strTemp, strTempVal;

//// --- Light Intensity --- ////

	if(!strSel.IsEmpty()) {
		double dTempVal;
		nTokenPos = 0;
		strTemp = strSel.Mid(strSel.ReverseFind(':')+1, strSel.GetLength());
		for(int nView=0; nView<nDocCount; nView++) {
			nTempDoc = (MAX_TRACKS * nTrack) + nView;
			nTempCount = pApp->m_LightHandler.m_nLightNoOfSegments[nTempDoc];
			for(int n=0; n<nTempCount; n++) {
				nChannel = pApp->m_pTracks[nTrack].m_nSegmentsOfImage[nView].GetAt(n);
				strTempVal = (nTokenPos >= 0) ? strTemp.Tokenize(" ", nTokenPos) : "";
				dTempVal = strTempVal.IsEmpty() ? 0 : atof(strTempVal);
				double dMinCur = pApp->m_LightHandler.m_pMaxCurrents[nChannel]+0.001;
				pApp->m_LightHandler.SetLight(nController, nChannel, (int)(((dTempVal*100)/dMinCur)), nFOV, FALSE);
			}
		}
		//pApp->m_LightHandler.SetAllLights(nController, nFOV);
	}

//// --- Pulse Width --- ////

	if(!strPulse.IsEmpty()) {
		int nTempPulseVal;
		nTokenPos = 0;
		strTemp = strPulse.Mid(strPulse.ReverseFind(':')+1, strPulse.GetLength());
		for(int nView=0; nView<nDocCount; nView++) {
			nTempDoc = (MAX_TRACKS * nTrack) + nView;
			nTempCount = pApp->m_LightHandler.m_nLightNoOfSegments[nTempDoc];
			for(int n=0; n<nTempCount; n++) {
				nChannel = pApp->m_pTracks[nTrack].m_nSegmentsOfImage[nView].GetAt(n);
				strTempVal = (nTokenPos >= 0) ? strTemp.Tokenize(" ", nTokenPos) : "";
				nTempPulseVal = strTempVal.IsEmpty() ? 0 : atoi(strTempVal);
				pApp->m_LightHandler.SetPulseWidth(nController, nChannel, nTempPulseVal, nFOV, FALSE);
			}
		}
		//pApp->m_LightHandler.SetAllPulseWidth(nController, nFOV);
	}
}


//void CStationMapping::OnBnClickedSmSaveTeachImageAllButton()
//{
//	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
//	int nDocCount, DiePosCount, nTempCount, nSel, nStep;
//	CString strTeachImgPath, strTemp, str, strPulse;
//
//	//// only if Motion Ctrl & Cam Avaialble ////
//	if (pApp->m_pTracks[0].m_bCamEnable && pApp->m_pTracks[0].m_pDoc[0]->m_bCameraActive && (pApp->m_strMotionCard.CompareNoCase("none") != 0)) {
//		for (int nFOV = 0; nFOV < pApp->m_nNoOfFov; nFOV++) {
//			SetFOVPositionToMotionCtrl(/*nTrack*/0, nFOV, FALSE);
//
//			for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++) {	
//				nDocCount = pApp->m_nTotalViewDocs[nTrack];
//				DiePosCount = m_strDiePosIDContents[nTrack][nFOV].GetCount();
//				nTempCount = (nDocCount <= DiePosCount) ? nDocCount : DiePosCount;
//
//				for(int nDoc=0; nDoc<nDocCount; nDoc++) {
//					nSel = 0;
//					str = "";
//					strPulse = "";
//					if (nDoc < nTempCount) {
//						strTemp = m_strDiePosIDContents[nTrack][nFOV].GetAt(nDoc);
//						if (atoi(strTemp)) {
//							nSel = atoi(strTemp);
//							str = m_strDieMapContents[nTrack].GetAt(nSel - 1);
//							strPulse = m_strPulseValue[nTrack].GetAt(nSel - 1);
//						}
//						else {
//							nSel = 0;
//							str.Format("%2d: Z 0 Light: 0 0 0 0", nCount + 1);;
//							strPulse = "";
//						}
//					}
//					nStep = (MAX_TRACKS*nTrack) + nDoc;
//					pApp->m_LightHandler.UpdateLightStep(0, nStep, nFOV);	//// Set
//					SetLightValues(nTrack, pApp->m_nTotalViewDocs[nTrack], nFOV, str, strPulse);
//					pApp->m_LightHandler.UpdateLightStep(0, -2, nFOV); //// Reset
//					SetZPositionValues(nTrack, nFOV, nDoc, str);
//
//					if(nTrack < pApp->m_nTotalTracks-1)
//						pApp->m_pTracks[nTrack].SetZPositionForSequence(nDoc);
//				}
//				
//				pApp->m_pTracks[nTrack].m_pDoc[0]->CameraGrabUsingExposure();
//
//				CRect rectTeach = CRect(CPoint(0, 0), pApp->m_pTracks[nTrack].m_pDoc[0]->m_Buffer.GetImgSize());
//				for (int nDoc1 = 0; nDoc1 <nDocCount; nDoc1++) {
//					ImgCopy(&pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_Buffer, &rectTeach, &pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_TeachBuffer[nFOV], &rectTeach);
//
//					if (pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_Buffer.IsAllocated()) {
//						strTeachImgPath.Format("%s\\TeachTemplate_Pos%d.bmp", pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_strConfigDir, nFOV + 1);
//						SaveGrayScaleBMPFile(strTeachImgPath, pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_Buffer);
//						////CString strTemp;
//						////strTemp.Format("C:\\Rakshith\\TempDelete\\ImgT%dFOV%dDoc%d.bmp", nTrack + 1, nFOV + 1, nDoc1 + 1);
//						////SaveGrayScaleBMPFile(strTemp, pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_TeachBuffer[nFOV]);
//					}
//				}
//			}
//		}
//	}
//	UpdateData(FALSE);
//}


void CStationMapping::OnBnClickedSmSaveTeachImageAllButton()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nDocCount, DiePosCount, nTempCount, nSel, nStep;
	CString strTeachImgPath, strTemp, str, strPulse;
	int nPrevZValue = 0;
	//// only if Motion Ctrl & Cam Avaialble ////
	if (pApp->m_pTracks[0].m_bCamEnable && pApp->m_pTracks[0].m_pDoc[0]->m_bCameraActive && (pApp->m_strMotionCard.CompareNoCase("none") != 0)) {
		for (int nFOV = 0; nFOV < pApp->m_nNoOfFov; nFOV++) {
			SetFOVPositionToMotionCtrl(/*nTrack*/0, nFOV, FALSE);

			for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++) {
				pApp->m_pTracks[nTrack].m_nFOVIndex = nFOV;
				nDocCount = pApp->m_nTotalViewDocs[nTrack];
				DiePosCount = m_strDiePosIDContents[nTrack][nFOV].GetCount();
				nTempCount = (nDocCount <= DiePosCount) ? nDocCount : DiePosCount;

				LogMessage(8, "nTrack=%d, nDocCount=%d, DiePosCount=%d, nTempCount=%d", nTrack, nDocCount, DiePosCount, nTempCount);

				BOOL bGrab = TRUE;
				for (int nDoc = 0; nDoc<nDocCount; nDoc++) {
					nSel = 0;
					str = "";
					strPulse = "";
					if (nDoc < nTempCount) {
						strTemp = m_strDiePosIDContents[nTrack][nFOV].GetAt(nDoc);
						if (atoi(strTemp)) {
							nSel = atoi(strTemp);
							str = m_strDieMapContents[nTrack].GetAt(nSel - 1);
							strPulse = m_strPulseValue[nTrack].GetAt(nSel - 1);
						}
						else {
							nSel = 0;
							str.Format("%2d: Z %d Light: 0 0 0 0 0 0 0 0", nDoc + 1, nPrevZValue);
							strPulse.Format("%2d: Pulse: 0 0 0 0 0 0 0 0", nDoc + 1);
							//bGrab = FALSE;
						}
					}
					else {
						if (nTrack < pApp->m_nTotalTracks - 1) {
							nSel = 0;
							str.Format("%2d: Z %d Light: 0 0 0 0 0 0 0 0", nDoc + 1, nPrevZValue);
							strPulse.Format("%2d: Pulse: 0 0 0 0 0 0 0 0", nDoc + 1);
						}
						else
							bGrab = FALSE;
					}
					/*if (!bGrab)
						break;*/
					nStep = (MAX_TRACKS*nTrack) + nDoc;
					pApp->m_LightHandler.UpdateLightStep(0, nStep, nFOV); //// Set
					LogMessage(8, "(UpdateLightStep) nStep=%d, nFOV=%d", nStep, nFOV);
					SetLightValues(nTrack, pApp->m_nTotalViewDocs[nTrack], nFOV, str, strPulse);
					LogMessage(8, "(SetLightValues) str=%s, nFOV=%d", str, nFOV);
					pApp->m_LightHandler.UpdateLightStep(0, -2, nFOV); //// Reset
					nPrevZValue = SetZPositionValues(nTrack, nFOV, nDoc, str);
					LogMessage(8, "(SetZPositionValues) nDoc=%d nPrevZValue = %d", nDoc, nPrevZValue);

					//if (nTrack < pApp->m_nTotalTracks - 1)
					//	pApp->m_pTracks[nTrack].SetZPositionForSequence(nDoc);
				}

				//LogMessage(8, "bGrab=%d", bGrab);
				if (bGrab)
					pApp->m_pTracks[nTrack].m_pDoc[0]->CameraGrabUsingExposure(1,0,TRUE);

				CRect rectTeach = CRect(CPoint(0, 0), pApp->m_pTracks[nTrack].m_pDoc[0]->m_Buffer.GetImgSize());
				for (int nDoc1 = 0; nDoc1 <nDocCount; nDoc1++) {
					ImgCopy(&pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_Buffer, &rectTeach, &pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_TeachBuffer[nFOV], &rectTeach);

					if (pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_Buffer.IsAllocated()) {
						strTeachImgPath.Format("%s\\TeachTemplate_Pos%d.bmp", pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_strConfigDir, nFOV + 1);
						SaveGrayScaleBMPFile(strTeachImgPath, pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_Buffer);
						////CString strTemp;
						////strTemp.Format("C:\\Rakshith\\TempDelete\\ImgT%dFOV%dDoc%d.bmp", nTrack + 1, nFOV + 1, nDoc1 + 1);
						////SaveGrayScaleBMPFile(strTemp, pApp->m_pTracks[nTrack].m_pDoc[nDoc1]->m_TeachBuffer[nFOV]);
					}
				}
			}
		}
	}
	UpdateData(FALSE);
}