// CopyPVIInspectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include "PVIInspectionParameters.h"
#include "PropertyGridProperty.h"
#include "PropertyGridColorProperty.h"
#include "Parameters.h"
#include "afxdialogex.h"
#include <direct.h>
#include "CopyPVIInspectionDlg.h"
// CCopyPVIInspectionDlg dialog

enum
{
	EDIT_PARAM,
	CHECKBOX_PARAM,
	COMBO_PARAM
};

////////////////////////////////////////////////////////////////////////////////
// CCheckBoxProp class

CCheckBoxProp::CCheckBoxProp(const CString& strName, BOOL bCheck, LPCTSTR lpszDescr, DWORD dwData) :
	CMFCPropertyGridProperty(strName, COleVariant((long) bCheck), lpszDescr, dwData)
{
	m_rectCheck.SetRectEmpty();
}

void CCheckBoxProp::OnDrawName(CDC* pDC, CRect rect)
{
	m_rectCheck = rect;
	m_rectCheck.DeflateRect(1, 1);

	m_rectCheck.right = m_rectCheck.left + m_rectCheck.Height();

	rect.left = m_rectCheck.right + 1;

	CMFCPropertyGridProperty::OnDrawName(pDC, rect);

	OnDrawCheckBox(pDC, m_rectCheck, (m_varValue.boolVal));
}

void CCheckBoxProp::OnClickName(CPoint point)
{
	if(m_bEnabled && m_rectCheck.PtInRect(point)) {
		m_varValue.boolVal = !(m_varValue.boolVal);
		m_pWndList->InvalidateRect(m_rectCheck);

		m_pWndList->OnPropertyChanged(this);
	}
}

BOOL CCheckBoxProp::OnDblClk(CPoint point)
{
	if(m_bEnabled && m_rectCheck.PtInRect(point)) {
		return TRUE;
	}

	m_varValue.boolVal = !(m_varValue.boolVal);
	m_pWndList->InvalidateRect(m_rectCheck);
	return TRUE;
}

void CCheckBoxProp::OnDrawCheckBox(CDC * pDC, CRect rect, BOOL bChecked)
{
	COLORREF clrTextOld = pDC->GetTextColor();

	CMFCVisualManager::GetInstance()->OnDrawCheckBox(pDC, rect, FALSE, bChecked, m_bEnabled);

	pDC->SetTextColor(clrTextOld);
}

BOOL CCheckBoxProp::PushChar(UINT nChar)
{
	if(nChar == VK_SPACE) {
		OnDblClk(CPoint(-1, -1));
	}

	return TRUE;
}

CBoundedNumberSubProp::CBoundedNumberSubProp(const CString& strName, const COleVariant& varValue, int nMinValue, int nMaxValue, LPCTSTR lpszDescr) :
	CMFCPropertyGridProperty(strName, varValue, lpszDescr)
{
	m_nMinValue = nMinValue;
	m_nMaxValue = nMaxValue;
}

BOOL CBoundedNumberSubProp::OnUpdateValue()
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT_VALID(m_pWndList);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	BOOL bRet = TRUE;
	CString strText;
	m_pWndInPlace->GetWindowText(strText);

	BOOL bIsChanged = FormatProperty() != strText;
	if (bIsChanged) {
		int nItem = _ttoi(strText);

		if (m_nMinValue != m_nMaxValue) {

			if ((nItem < m_nMinValue) || (nItem > m_nMaxValue)) {

				static BOOL bRecursedHere = FALSE;
				if (bRecursedHere)
					return TRUE;
				bRecursedHere = TRUE;

				CString strMessage;
				strMessage.Format(_T("Value must be between %d and %d."), m_nMinValue, m_nMaxValue);
				AfxMessageBox(strMessage);

				bRecursedHere = FALSE;
				return FALSE;
			}

		}

		bRet = CMFCPropertyGridProperty::OnUpdateValue();

		//if (m_pParent != NULL) {
		//	m_pWndList->OnPropertyChanged(m_pParent);
		//}
	}

	return bRet;
}

////////////////////////////////////////////////////////////////////////////////
// CCopyPVIInspectionDlg class

IMPLEMENT_DYNAMIC(CCopyPVIInspectionDlg, CDialog)

CCopyPVIInspectionDlg::CCopyPVIInspectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_COPY_PVI_PARAM_DLG, pParent)
{
	m_pApp = (CApplication*) AfxGetApp();
	m_nTrackId = 0;
	m_nFOV = 0;
	m_nDocId = 0;
	m_nPviAreaId = 0;
	m_nDefectId = 0;
	m_nPVIParameterType = PVI_FOV_PARAM;

	m_bCommonCopyPVIInspectionSetUP = false;
	m_bCommonCopyPVIEdgeInspectionSetUP = false;
	m_bCommonCopyPVIEdgeInspectionRecheck = false;
	bResetCopyPVIInspection = false;

	bExpanded = false;
}

CCopyPVIInspectionDlg::~CCopyPVIInspectionDlg()
{
}

void CCopyPVIInspectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID_COPY_PVI_PARAM_SELECTION, m_ctrlCopyPVIInsp);
}


BEGIN_MESSAGE_MAP(CCopyPVIInspectionDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_COPY_PVI_INSP_PARAM, &CCopyPVIInspectionDlg::OnBnClickedButtonCopyPviInspParam)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_COPY_PVI_INSP_PARAM, &CCopyPVIInspectionDlg::OnBnClickedButtonApplyCopyPviInspParam)
END_MESSAGE_MAP()


// CCopyPVIInspectionDlg message handlers

BOOL CCopyPVIInspectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Substrate Parameters
	m_ctrlCopyPVIInsp.RemoveAll();

	m_ctrlCopyPVIInsp.EnableDescriptionArea();
	m_ctrlCopyPVIInsp.SetVSDotNetLook();
	m_ctrlCopyPVIInsp.MarkModifiedProperties();
	m_ctrlCopyPVIInsp.SetShowDragContext(FALSE);

	CRect rc;
	HDITEM hdItem;
	hdItem.mask = HDI_WIDTH;
	m_ctrlCopyPVIInsp.GetWindowRect(&rc);

	hdItem.cxy = rc.Width() * 2 / 3;
	m_ctrlCopyPVIInsp.GetHeaderCtrl().SetItem(0, &hdItem);

	if(m_nPVIParameterType == PVI_SETUP_PARAM)
		PVIInspSelectionPage();
	else if(m_nPVIParameterType == PVI_FOV_PARAM)
		PVIFOVInspSelectionPage();
	else if(m_nPVIParameterType == PVI_DEFECT_PARAM)
		PVIDefectCharacteristicSelectionPage();

	return TRUE;
}

void CCopyPVIInspectionDlg::PVIInspSelectionPage()
{
	///------ADDING PARAMETERES FOR--- SUBSTRATE TEACH 
	COleVariant cValue;
	int nCurrentNoFOV = 0;
	if(m_nTrackId == STATION_TOP)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	else if(m_nTrackId == STATION_THR)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

	CMFCPropertyGridProperty* pPVIInspParamCommon = AddPropertyGroup("Common Position");
	cValue.intVal = m_bCommonCopyPVIInspectionSetUP;
	AddPropertyItem("Common PVI Inspection SetUP", cValue, 0, 0, "Check To Copy PVI Inspection SetUp Parameter", CHECKBOX_PARAM, pPVIInspParamCommon);

	cValue.intVal = m_bCommonCopyPVIEdgeInspectionSetUP;
	AddPropertyItem("Common Edge PVI Inspection SetUP", cValue, 0, 0, "Check To Copy PVI Inspection SetUp Parameter", CHECKBOX_PARAM, pPVIInspParamCommon);

	cValue.intVal = m_bCommonCopyPVIEdgeInspectionRecheck;
	AddPropertyItem("Common PVI Inspection Recheck", cValue, 0, 0, "Check To Copy PVI Inspection Recheck Parameter", CHECKBOX_PARAM, pPVIInspParamCommon);

	cValue.intVal = bResetCopyPVIInspection;
	AddPropertyItem("Reset The Check Box For All Position", cValue, 0, 0, "Check To Reset All The Check Box For All Position", CHECKBOX_PARAM, pPVIInspParamCommon);

	m_ctrlCopyPVIInsp.AddProperty(pPVIInspParamCommon);

	m_bCopyPVIInspectionSetUP.resize(nCurrentNoFOV);
	m_bCopyEdgePVIInspectionSetUP.resize(nCurrentNoFOV);
	m_bCopyEdgePVIInspectionRecheck.resize(nCurrentNoFOV);
	for(size_t nFOV = 0; nFOV < nCurrentNoFOV; nFOV++) {
		CString strTemp;
		strTemp.Format("Position_%d", nFOV + 1);
		CMFCPropertyGridProperty* pPVIInspParamPos = AddPropertyGroup(strTemp);

		pInspHandler = &m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[nFOV];
		int nPviArea = pInspHandler->m_Inspection.m_TrainingData.nNoOfPviArea;
		m_bCopyPVIInspectionSetUP[nFOV].resize(nPviArea, false);
		m_bCopyEdgePVIInspectionSetUP[nFOV].resize(nPviArea, false);
		m_bCopyEdgePVIInspectionRecheck[nFOV].resize(nPviArea, false);

		for(size_t nAreaId = 0; nAreaId < nPviArea; nAreaId++) {
			strTemp.Format("PVI Area %d", nAreaId + 1);
			CMFCPropertyGridProperty* pPVIInspParamArea = AddPropertyGroup(strTemp);

			cValue.intVal = m_bCopyPVIInspectionSetUP[nFOV][nAreaId];
			AddPropertyItem("PVI Inspection SetUP", cValue, 0, 0, "Check To Copy PVI Inspection SetUp Parameter", CHECKBOX_PARAM, pPVIInspParamPos, TRUE, pPVIInspParamArea);

			cValue.intVal = m_bCopyEdgePVIInspectionSetUP[nFOV][nAreaId];
			AddPropertyItem("Edge PVI Inspection SetUP", cValue, 0, 0, "Check To Copy Edge PVI Inspection SetUp Parameter", CHECKBOX_PARAM, pPVIInspParamPos, TRUE, pPVIInspParamArea);

			cValue.intVal = m_bCopyEdgePVIInspectionRecheck[nFOV][nAreaId];
			AddPropertyItem("PVI Inspection Recheck", cValue, 0, 0, "Check To Copy PVI Inspection SetUp Parameter", CHECKBOX_PARAM, pPVIInspParamPos, TRUE, pPVIInspParamArea);

			pPVIInspParamArea->Expand(FALSE);
			pPVIInspParamPos->AddSubItem(pPVIInspParamArea);
		}

		m_ctrlCopyPVIInsp.AddProperty(pPVIInspParamPos);
	}
}

void CCopyPVIInspectionDlg::RetrievePVIInspSelectionPage()
{
	int nCount = 0;
	int nItem = 0;
	int subItemCount = 0;

	COleVariant cValue;
	int nCurrentNoFOV = 0;
	if(m_nTrackId == STATION_TOP)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	else if(m_nTrackId == STATION_THR)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

	cValue = RetrivePropertyItem(nItem, nCount++);
	m_bCommonCopyPVIInspectionSetUP = (cValue.boolVal != 0);

	cValue = RetrivePropertyItem(nItem, nCount++);
	m_bCommonCopyPVIEdgeInspectionSetUP = (cValue.boolVal != 0);

	cValue = RetrivePropertyItem(nItem, nCount++);
	m_bCommonCopyPVIEdgeInspectionRecheck = (cValue.boolVal != 0);

	cValue = RetrivePropertyItem(nItem, nCount++);
	bResetCopyPVIInspection = (cValue.boolVal != 0);
	nItem++;


	for(size_t nFOV = 0; nFOV < nCurrentNoFOV; nFOV++) {
		pInspHandler = &m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[nFOV];
		int nPviArea = pInspHandler->m_Inspection.m_TrainingData.nNoOfPviArea;
		nCount = 0;
		for(size_t nAreaId = 0; nAreaId < nPviArea; nAreaId++) {
			subItemCount = 0;
			cValue = RetrivePropertyItem(nItem, nCount, TRUE, subItemCount++);
			m_bCopyPVIInspectionSetUP[nFOV][nAreaId] = (cValue.boolVal != 0);

			cValue = RetrivePropertyItem(nItem, nCount, TRUE, subItemCount++);
			m_bCopyEdgePVIInspectionSetUP[nFOV][nAreaId] = (cValue.boolVal != 0);

			cValue = RetrivePropertyItem(nItem, nCount, TRUE, subItemCount++);
			m_bCopyEdgePVIInspectionRecheck[nFOV][nAreaId] = (cValue.boolVal != 0);
			nCount++;
		}

		nItem++;
	}
}

void CCopyPVIInspectionDlg::PVIFOVInspSelectionPage()
{
	///------ADDING PARAMETERES FOR--- SUBSTRATE TEACH 
	COleVariant cValue;
	int nCurrentNoFOV = 0;
	if(m_nTrackId == STATION_TOP)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	else if(m_nTrackId == STATION_THR)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

	CMFCPropertyGridProperty* pPVIInspParamCommon = AddPropertyGroup("Common Position");
	cValue.intVal = m_bCommonCopyPVIParam;
	AddPropertyItem("Common PVI Inspection Parameter", cValue, 0, 0, "Check To Copy PVI Inspection Parameter", CHECKBOX_PARAM, pPVIInspParamCommon);

	cValue.intVal = bResetCopyPVIInspection;
	AddPropertyItem("Reset The Check Box For All Position", cValue, 0, 0, "Check To Reset All The Check Box For All Position", CHECKBOX_PARAM, pPVIInspParamCommon);

	m_ctrlCopyPVIInsp.AddProperty(pPVIInspParamCommon);

	CMFCPropertyGridProperty* pPVIInspParamPos = AddPropertyGroup("PVI Parameter FOV");
	m_bCopyPVIInspectionFOV.resize(nCurrentNoFOV, false);
	for(size_t nFOV = 0; nFOV < nCurrentNoFOV; nFOV++) {
		CString strTemp;
		strTemp.Format("Position_%d", nFOV + 1);
		cValue.intVal = m_bCopyPVIInspectionFOV[nFOV];
		AddPropertyItem(strTemp, cValue, 0, 0, "Check To Copy PVI Inspection Parameter", CHECKBOX_PARAM, pPVIInspParamPos);
	}
	m_ctrlCopyPVIInsp.AddProperty(pPVIInspParamPos);
}

void CCopyPVIInspectionDlg::RetrievePVIFOVInspSelectionPage()
{
	int nCount = 0;
	int nItem = 0;
	int subItemCount = 0;

	COleVariant cValue;
	int nCurrentNoFOV = 0;
	if(m_nTrackId == STATION_TOP)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	else if(m_nTrackId == STATION_THR)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

	cValue = RetrivePropertyItem(nItem, nCount++);
	m_bCommonCopyPVIInspectionSetUP = (cValue.boolVal != 0);

	cValue = RetrivePropertyItem(nItem, nCount++);
	bResetCopyPVIInspection = (cValue.boolVal != 0);
	nItem++;

	nCount = 0;
	for(size_t nFOV = 0; nFOV < nCurrentNoFOV; nFOV++) {
		cValue = RetrivePropertyItem(nItem, nCount++);
		m_bCopyPVIInspectionFOV[nFOV] = (cValue.boolVal != 0);
	}
}

void CCopyPVIInspectionDlg::PVIDefectCharacteristicSelectionPage()
{
	///------ADDING PARAMETERES FOR--- SUBSTRATE TEACH 
	COleVariant cValue;
	int nCurrentNoFOV = 0;
	if(m_nTrackId == STATION_TOP)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	else if(m_nTrackId == STATION_THR)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

	CMFCPropertyGridProperty* pPVIInspParamCommon = AddPropertyGroup("Common Position");

	m_bCommonCopyPVIInspectionDefect.resize(10, false);
	for(size_t i = 0; i < m_bCommonCopyPVIInspectionDefect.size(); i++) {
		cValue.intVal = m_bCommonCopyPVIInspectionDefect[i];
		CString strTemp;
		strTemp.Format("Common PVI Area %d", i + 1);
		AddPropertyItem(strTemp, cValue, 0, 0, "Check To Copy Defect Characteristic Parameter", CHECKBOX_PARAM, pPVIInspParamCommon);
	}

	cValue.intVal = bResetCopyPVIInspection;
	AddPropertyItem("Reset The Check Box For All Position", cValue, 0, 0, "Check To Reset All The Check Box For All Position", CHECKBOX_PARAM, pPVIInspParamCommon);

	m_ctrlCopyPVIInsp.AddProperty(pPVIInspParamCommon);

	m_bCopyPVIDefectCharacteristic.resize(nCurrentNoFOV);
	m_bCreateNewPVIDefectCharacteristic.resize(nCurrentNoFOV);
	for(size_t nFOV = 0; nFOV < nCurrentNoFOV; nFOV++) {
		CString strTemp;
		strTemp.Format("Position_%d", nFOV + 1);
		CMFCPropertyGridProperty* pPVIInspParamPos = AddPropertyGroup(strTemp);

		pInspHandler = &m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[nFOV];
		int nPviArea = pInspHandler->m_Inspection.m_TrainingData.nNoOfPviArea;
		m_bCopyPVIDefectCharacteristic[nFOV].resize(nPviArea);
		m_bCreateNewPVIDefectCharacteristic[nFOV].resize(nPviArea);
		for(size_t nAreaId = 0; nAreaId < nPviArea; nAreaId++) {
			strTemp.Format("PVI Area %d", nAreaId + 1);
			CMFCPropertyGridProperty* pPVIInspParamArea = AddPropertyGroup(strTemp);

			int nPviDefectCount = pInspHandler->m_Inspection.m_pPviInspectionSetup[nAreaId].nDefectCount;
			m_bCopyPVIDefectCharacteristic[nFOV][nAreaId].resize(nPviDefectCount, false);
			for(size_t nDefectId = 0; nDefectId < nPviDefectCount; nDefectId++) {
				strTemp = pInspHandler->m_Inspection.m_pPviInspectionSetup[nAreaId].m_pPviDefect[nDefectId].strDefectName;

				cValue.intVal = m_bCopyPVIDefectCharacteristic[nFOV][nAreaId][nDefectId];
				AddPropertyItem(strTemp, cValue, 0, 0, "Check To Copy PVI Defect Characteristic Parameter", CHECKBOX_PARAM, pPVIInspParamPos, TRUE, pPVIInspParamArea);
			}
			//Instead of copy to the current defect, we can create the new one too
			cValue.intVal = m_bCreateNewPVIDefectCharacteristic[nFOV][nAreaId];
			AddPropertyItem("Create New Defect Characteristic", cValue, 0, 0, "Check To Create New PVI Defect Characteristic Parameter", CHECKBOX_PARAM, pPVIInspParamPos, TRUE, pPVIInspParamArea);
			
			pPVIInspParamArea->Expand(FALSE);
			pPVIInspParamPos->AddSubItem(pPVIInspParamArea);
		}

		m_ctrlCopyPVIInsp.AddProperty(pPVIInspParamPos);
	}
}

void CCopyPVIInspectionDlg::RetrievePVIDefectCharacteristicSelectionPage()
{
	int nCount = 0;
	int nItem = 0;
	int subItemCount = 0;

	COleVariant cValue;
	int nCurrentNoFOV = 0;
	if(m_nTrackId == STATION_TOP)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
	else if(m_nTrackId == STATION_THR)
		nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

	for(size_t i = 0; i < m_bCommonCopyPVIInspectionDefect.size(); i++) {
		cValue = RetrivePropertyItem(nItem, nCount++);
		m_bCommonCopyPVIInspectionDefect[i] = (cValue.boolVal != 0);
	}
	
	cValue = RetrivePropertyItem(nItem, nCount++);
	bResetCopyPVIInspection = (cValue.boolVal != 0);
	nItem++;


	for(size_t nFOV = 0; nFOV < nCurrentNoFOV; nFOV++) {
		pInspHandler = &m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[nFOV];
		int nPviArea = pInspHandler->m_Inspection.m_TrainingData.nNoOfPviArea;
		nCount = 0;
		for(size_t nAreaId = 0; nAreaId < nPviArea; nAreaId++) {
			subItemCount = 0;
			int nPviDefectCount = pInspHandler->m_Inspection.m_pPviInspectionSetup[nAreaId].nDefectCount;

			for(size_t nDefectId = 0; nDefectId < nPviDefectCount; nDefectId++) {
				cValue = RetrivePropertyItem(nItem, nCount, TRUE, subItemCount++);
				m_bCopyPVIDefectCharacteristic[nFOV][nAreaId][nDefectId] = (cValue.boolVal != 0);
			}

			cValue = RetrivePropertyItem(nItem, nCount, TRUE, subItemCount++);
			m_bCreateNewPVIDefectCharacteristic[nFOV][nAreaId] = (cValue.boolVal != 0);

			nCount++;
		}
		nItem++;
	}
}

void CCopyPVIInspectionDlg::OnBnClickedButtonCopyPviInspParam()
{
	// TODO: Add your control notification handler code here
	if(m_nPVIParameterType == PVI_SETUP_PARAM)
		RetrievePVIInspSelectionPage();
	else if(m_nPVIParameterType == PVI_FOV_PARAM)
		RetrievePVIFOVInspSelectionPage();
	else if(m_nPVIParameterType == PVI_DEFECT_PARAM)
		RetrievePVIDefectCharacteristicSelectionPage();

	CString strConfirmQuestion = "Copy PVI Inspection Parameters From ";
	if(m_nTrackId == STATION_TOP)
		strConfirmQuestion += "TOP Station, ";
	else if(m_nTrackId == STATION_THR)
		strConfirmQuestion += "THR Station, ";
	CString strTemp;
	strTemp.Format("Position_%d ", m_nFOV + 1);
	strConfirmQuestion += strTemp + "To All Checked Positions Parameter!!!\nAre you sure?";

	if(AfxMessageBox(strConfirmQuestion, MB_YESNO) == IDYES) {
		//Copy all parameter
		if(m_nPVIParameterType == PVI_SETUP_PARAM)
			CopyPVISetupParameter();
		else if(m_nPVIParameterType == PVI_FOV_PARAM)
			CopyPVIFOVParameter();
		else if(m_nPVIParameterType == PVI_DEFECT_PARAM)
			CopyPVIDefectCharacteristicParameter();

		CDialog::OnOK();
	}
}

void CCopyPVIInspectionDlg::CopyPVIDefectCharacteristicParameter()
{
	pInspHandler = &m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[m_nFOV];
	CPVIInspectionSetup* currentPviInspectionSetup = pInspHandler->m_Inspection.m_pPviInspectionSetup;
	
	for(size_t nFOV = 0; nFOV < m_bCopyPVIDefectCharacteristic.size(); nFOV++) {
		bool bIsCopied = false;
		pInspHandlerCopy = &m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[nFOV];
		pInspHandlerCopy->GetPVIDefectParameters(m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_strConfigDir, m_nTrackId, nFOV);
		for(size_t nAreaId = 0; nAreaId < m_bCopyPVIDefectCharacteristic[nFOV].size(); nAreaId++) {
			for(size_t nDefectId = 0; nDefectId < m_bCopyPVIDefectCharacteristic[nFOV][nAreaId].size(); nDefectId++) {
				if(m_bCopyPVIDefectCharacteristic[nFOV][nAreaId][nDefectId]) {
					pInspHandlerCopy->m_Inspection.m_pPviInspectionSetup[nAreaId].m_pPviDefect[nDefectId] = currentPviInspectionSetup[m_nPviAreaId].m_pPviDefect[m_nDefectId];
					bIsCopied = true;
				}
			}
		
			if(m_bCreateNewPVIDefectCharacteristic[nFOV][nAreaId]) {
				int nDefectCount = pInspHandlerCopy->m_Inspection.m_pPviInspectionSetup[nAreaId].nDefectCount;
				pInspHandlerCopy->m_Inspection.m_pPviInspectionSetup[nAreaId].m_pPviDefect[nDefectCount] = currentPviInspectionSetup[m_nPviAreaId].m_pPviDefect[m_nDefectId];
				pInspHandlerCopy->m_Inspection.m_pPviInspectionSetup[nAreaId].nDefectCount++;
				bIsCopied = true;
			}
		}
		if(bIsCopied)
			pInspHandlerCopy->SetPVIDefectParameters(m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_strConfigDir, m_nTrackId, nFOV);
	}
}

void CCopyPVIInspectionDlg::CopyPVISetupParameter()
{
	pInspHandler = &m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[m_nFOV];
	CPVIInspectionSetup* currentPviInspectionSetup = &pInspHandler->m_Inspection.m_pPviInspectionSetup[m_nPviAreaId];
	bool bReteach = false;

	for(size_t nFOV = 0; nFOV < m_bCopyPVIInspectionSetUP.size(); nFOV++) {
		if(!isAnyCopyEnable(m_bCopyPVIInspectionSetUP[nFOV]) &&
			!isAnyCopyEnable(m_bCopyEdgePVIInspectionSetUP[nFOV]) &&
			!isAnyCopyEnable(m_bCopyEdgePVIInspectionRecheck[nFOV]))
			continue;
		bool bIsCopied = false;
		pInspHandlerCopy = &m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[nFOV];
		pInspHandlerCopy->GetPVIDefectParameters(m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_strConfigDir, m_nTrackId, nFOV);

		if(isAnyCopyEnable(m_bCopyPVIInspectionSetUP[nFOV]))
			bReteach = true;

		for(size_t nAreaId = 0; nAreaId < m_bCopyPVIInspectionSetUP[nFOV].size(); nAreaId++) {
			CPVIInspectionSetup* copyPviInspectionSetup = &pInspHandlerCopy->m_Inspection.m_pPviInspectionSetup[nAreaId];
			if(m_bCopyPVIInspectionSetUP[nFOV][nAreaId]) {
				copyPviInspectionSetup->bEnable = currentPviInspectionSetup->bEnable;
				copyPviInspectionSetup->nDeviceMaskNumber = currentPviInspectionSetup->nDeviceMaskNumber;
				for(int nMask = 0; nMask < copyPviInspectionSetup->nDeviceMaskNumber; nMask++) {
					copyPviInspectionSetup->nDeviceMaskArea[nMask] = currentPviInspectionSetup->nDeviceMaskArea[nMask];
					copyPviInspectionSetup->nPolygonPviMaskPointNumber[nMask] = currentPviInspectionSetup->nPolygonPviMaskPointNumber[nMask];
				}
				copyPviInspectionSetup->nPolygonPointNumber = currentPviInspectionSetup->nPolygonPointNumber;
				copyPviInspectionSetup->nPolylinePointNumber = currentPviInspectionSetup->nPolylinePointNumber;
			}

			if(m_bCopyEdgePVIInspectionSetUP[nFOV][nAreaId]) {
				copyPviInspectionSetup->bEdgeEnable = currentPviInspectionSetup->bEdgeEnable;
				copyPviInspectionSetup->nMinEdgeContrast = currentPviInspectionSetup->nMinEdgeContrast;
				copyPviInspectionSetup->nMaxEdgeContrast = currentPviInspectionSetup->nMaxEdgeContrast;
				copyPviInspectionSetup->dEdgeRegionOpen = currentPviInspectionSetup->dEdgeRegionOpen;
				copyPviInspectionSetup->dEdgeOffset = currentPviInspectionSetup->dEdgeOffset;
				copyPviInspectionSetup->dEdgeWidth = currentPviInspectionSetup->dEdgeWidth;
			}

			if(m_bCopyEdgePVIInspectionRecheck[nFOV][nAreaId]) {
				copyPviInspectionSetup->bPVIRecheckEnable = currentPviInspectionSetup->bPVIRecheckEnable;
				copyPviInspectionSetup->nMinGVDiff = currentPviInspectionSetup->nMinGVDiff;
				copyPviInspectionSetup->nIntensity = currentPviInspectionSetup->nIntensity;
			}
			bIsCopied = true;
		}

		if(bIsCopied) {
			m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[nFOV].SetPVIDefectParameters(m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_strConfigDir, m_nTrackId, nFOV);
		}
	}

	if(bReteach) {
		CString str;
		str.Format("ReTeach");
		m_pApp->AddLogEvent(str);
		AfxMessageBox("Please Re-Teach");
	}
}

void CCopyPVIInspectionDlg::CopyPVIFOVParameter()
{
	if(isAnyCopyEnable(m_bCopyPVIInspectionFOV)) {
		BeginWaitCursor();
		pInspHandler = &m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[m_nFOV];
		for(size_t nFOV = 0; nFOV < m_bCopyPVIInspectionFOV.size(); nFOV++) {
			if(m_bCopyPVIInspectionFOV[nFOV]) {
				pInspHandler->SetPVIDefectParameters(m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_strConfigDir, m_nTrackId, nFOV);
				pInspHandlerCopy = &m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_InspectionHandler[nFOV];
				pInspHandler->CopyPVIParameters(m_pApp->m_pTracks[m_nTrackId].m_pDoc[m_nDocId]->m_strConfigDir, m_pApp->m_strConfigFile,
												pInspHandlerCopy, m_nTrackId, nFOV);

				m_pApp->LoadRecipes(m_nTrackId, nFOV);
			}
		}
		EndWaitCursor();

		CString str;
		str.Format("ReTeach");
		m_pApp->AddLogEvent(str);
		AfxMessageBox("Please Re-Teach");
	}
}

void CCopyPVIInspectionDlg::ClearSelection()
{
	if(m_nPVIParameterType == PVI_SETUP_PARAM) {
		m_bCopyPVIInspectionSetUP.clear();
		m_bCopyEdgePVIInspectionSetUP.clear();
		m_bCopyEdgePVIInspectionRecheck.clear();
		m_bCommonCopyPVIInspectionSetUP = m_bCommonCopyPVIEdgeInspectionSetUP = m_bCommonCopyPVIEdgeInspectionRecheck = false;
	}
	else if(m_nPVIParameterType == PVI_FOV_PARAM) {
		m_bCopyPVIInspectionFOV.clear();
		m_bCommonCopyPVIParam = false;
	}
	else if(m_nPVIParameterType == PVI_DEFECT_PARAM) {
		m_bCommonCopyPVIInspectionDefect.clear();
		m_bCopyPVIDefectCharacteristic.clear();
	}
}

void CCopyPVIInspectionDlg::OnBnClickedButtonApplyCopyPviInspParam()
{
	// TODO: Add your control notification handler code here
	if(m_nPVIParameterType == PVI_SETUP_PARAM)
		RetrievePVIInspSelectionPage();
	else if(m_nPVIParameterType == PVI_FOV_PARAM)
		RetrievePVIFOVInspSelectionPage();
	else if(m_nPVIParameterType == PVI_DEFECT_PARAM)
		RetrievePVIDefectCharacteristicSelectionPage();

	//Reset
	if(bResetCopyPVIInspection) {
		int nCurrentNoFOV = 0;
		if(m_nTrackId == STATION_TOP)
			nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTop();
		else if(m_nTrackId == STATION_THR)
			nCurrentNoFOV = m_pApp->motionParameters.GetTotalFOVPerDieTHR();

		if(m_nPVIParameterType == PVI_SETUP_PARAM) {
			for(size_t nFOV = 0; nFOV < nCurrentNoFOV; nFOV++) {
				std::fill(m_bCopyPVIInspectionSetUP[nFOV].begin(), m_bCopyPVIInspectionSetUP[nFOV].end(), m_bCommonCopyPVIInspectionSetUP);
				std::fill(m_bCopyEdgePVIInspectionSetUP[nFOV].begin(), m_bCopyEdgePVIInspectionSetUP[nFOV].end(), m_bCommonCopyPVIEdgeInspectionSetUP);
				std::fill(m_bCopyEdgePVIInspectionRecheck[nFOV].begin(), m_bCopyEdgePVIInspectionRecheck[nFOV].end(), m_bCommonCopyPVIEdgeInspectionRecheck);
			}
		}
		else if(m_nPVIParameterType == PVI_FOV_PARAM) {
			std::fill(m_bCopyPVIInspectionFOV.begin(), m_bCopyPVIInspectionFOV.end(), m_bCommonCopyPVIInspectionSetUP);
		}
		else if(m_nPVIParameterType == PVI_DEFECT_PARAM) {
			for(size_t nFOV = 0; nFOV < m_bCopyPVIDefectCharacteristic.size(); nFOV++) {
				for(size_t nAreaId = 0; nAreaId < m_bCopyPVIDefectCharacteristic[nFOV].size(); nAreaId++) {
					std::fill(m_bCopyPVIDefectCharacteristic[nFOV][nAreaId].begin(), m_bCopyPVIDefectCharacteristic[nFOV][nAreaId].end(), m_bCommonCopyPVIInspectionDefect[nAreaId]);
				}
			}
		}

		bResetCopyPVIInspection = false;
	}

	OnInitDialog();
	bExpanded = !bExpanded;
	m_ctrlCopyPVIInsp.ExpandAll(bExpanded);
}

bool CCopyPVIInspectionDlg::isAnyCopyEnable(std::vector < bool > bCopyPVI)
{
	return std::any_of(bCopyPVI.begin(), bCopyPVI.end(), [](bool bIsCopy) {return bIsCopy; });
}

CMFCPropertyGridProperty* CCopyPVIInspectionDlg::AddPropertyGroup(CString strPropertyName)
{
	CMFCPropertyGridProperty* pSub;

	pSub = new CMFCPropertyGridProperty(_T(strPropertyName));

	return pSub;
}

void CCopyPVIInspectionDlg::AddPropertyItem(CString strPropertyName,
	COleVariant cValue,
	int nMin,
	int nMax,
	CString strTips,
	int nType,
	CMFCPropertyGridProperty* pGroup,
	BOOL bSubProperty,
	CMFCPropertyGridProperty* pSubGroup)
{
	CMFCPropertyGridProperty* pSubItem = NULL;

	if(nType == CHECKBOX_PARAM)
		pSubItem = new CCheckBoxProp(_T(strPropertyName), cValue.intVal, _T(strTips));

	/*if(nType == EDIT_PARAM) {
	pSubItem = new CBoundedNumberSubProp(_T(strPropertyName), (COleVariant) (cValue), nMin, nMax, _T(strTips));

	switch(cValue.vt) {
	case VT_INT:
	case VT_UINT:
	case VT_I2:
	case VT_I4:
	case VT_UI2:
	case VT_UI4:
	pSubItem->EnableSpinControl(TRUE, nMin, nMax);
	break;
	case VT_R8:
	pSubItem = new CDoubleProp(_T(strPropertyName), cValue.dblVal, _T(strTips));
	pSubItem->FormatProperty();
	break;
	}
	}
	if(nType == COMBO_PARAM) {
	CString strSelected;
	CString strName;
	int nPos = 0;
	int nSel = 0;
	CString str;

	strName.Format("%s", strPropertyName);
	nSel = (int) cValue.iVal;
	while(TRUE) {
	if(nPos == 0) {
	nPos = strPropertyName.Find(':');
	str.Format("%s", (nPos < 1) ? strName : strName.Left(nPos));
	strSelected = GetSelectedComboItem(strName.Right(strName.GetLength() - nPos - 1), nSel);
	pSubItem = new CMFCPropertyGridProperty(_T(str), _T(strSelected), _T(strTips));
	if(nPos > 0)
	strName = strName.Mid(nPos + 1);
	}
	if(nPos >= 1 && strName.GetLength() > 0) {
	nPos = strName.Find(';');
	str.Format("%s", (nPos < 1) ? strName : strName.Left(nPos));
	pSubItem->AddOption(_T(str));
	if(nPos < 0)
	break;
	}
	if(nPos > 0)
	strName = strName.Mid(nPos + 1);
	}

	pSubItem->AllowEdit(FALSE);
	}*/

	if(bSubProperty)
		pSubGroup->AddSubItem(pSubItem);
	else
		pGroup->AddSubItem(pSubItem);
}

COleVariant CCopyPVIInspectionDlg::RetrivePropertyItem(int nPropertyItem, int nSubItem, BOOL bSubPorperty, int nSubItemIndex)
{
	COleVariant cValue;
	CMFCPropertyGridProperty* pGroup;
	CMFCPropertyGridProperty* pSub;

	pGroup = m_ctrlCopyPVIInsp.GetProperty(nPropertyItem);
	pSub = pGroup->GetSubItem(nSubItem);

	cValue = pSub->GetValue();

	if(bSubPorperty) {
		CMFCPropertyGridProperty* pSubItem;
		pSubItem = pSub->GetSubItem(nSubItemIndex);
		cValue = pSubItem->GetValue();
	}
	return cValue;
}
