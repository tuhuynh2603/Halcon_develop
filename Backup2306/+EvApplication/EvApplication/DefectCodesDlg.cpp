// CDefectCodesDlg.cpp : implementation file
//
#include "stdafx.h"
#include "Application.h"
#include "DefectCodesDlg.h"
#include "ImageSelectionDlg.h"

enum
{
	EDIT_PARAM,
	CHECKBOX_PARAM,
	COMBO_PARAM
};

// CDefectCodesDlg dialog

IMPLEMENT_DYNAMIC(CDefectCodesDlg, CDialog)

CDefectCodesDlg::CDefectCodesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDefectCodesDlg::IDD, pParent)
{
}

CDefectCodesDlg::~CDefectCodesDlg()
{
}

void CDefectCodesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EEOR_CODE_CTRL, m_ctrlDefetCodes);
}

BEGIN_MESSAGE_MAP(CDefectCodesDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDefectCodesDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDOK, &CDefectCodesDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDefectCodesDlg message handlers
void CDefectCodesDlg::OnBnClickedButtonApply()
{
	RetrieveDefectCodesPage();
}

BOOL CDefectCodesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_ctrlDefetCodes.EnableDescriptionArea();
	m_ctrlDefetCodes.SetVSDotNetLook();
	m_ctrlDefetCodes.MarkModifiedProperties();
	m_ctrlDefetCodes.SetShowDragContext(FALSE);

	CRect rc;
	HDITEM hdItem;
	hdItem.mask = HDI_WIDTH;
	m_ctrlDefetCodes.GetWindowRect(&rc);

	hdItem.cxy = rc.Width()/2;
	m_ctrlDefetCodes.GetHeaderCtrl().SetItem(0, &hdItem);

	//Display Parameters
	DefectCodesPage();
	m_ctrlDefetCodes.ExpandAll(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int CDefectCodesDlg::getFovIndexFromDefectName(std::string strDefectName, int nTrack, int nNoFov, int nErrorCodeId)
{
	for (int nFov = 0; nFov < nNoFov; nFov++) {
		std::string strHashKey = strDefectName + "_Track" + std::to_string(nTrack) + "_FOV" + std::to_string(nFov);
		int nHasID = std::hash<std::string>()(strHashKey);
		if (-abs(nHasID) == nErrorCodeId)
			return nFov;
	}
	return -1;
}

void CDefectCodesDlg::DefectCodesPage()
{
	int nPVIGroupId = theApp.m_pImageSelectionDlg->nPVIGroupId;
	_defect = theApp.m_pImageSelectionDlg->getDefect();
	_group  = theApp.m_pImageSelectionDlg->getGroup();
	
	_defectGroupPosition.resize( _defect.size() );
	std::fill( _defectGroupPosition.begin(), _defectGroupPosition.end(), 0 );
	
	std::vector < CMFCPropertyGridColorProperty* > propertyTemp( _defect.size() );

	std::swap( _property, propertyTemp );

	_property.clear();
	
	_property.resize( _defect.size() );
	
	if( !_group.empty() ) {
		std::vector < CMFCPropertyGridProperty * > groupGui( _group.size(), NULL );
		std::vector < int > itemGroupId( _group.size(), 0 );

		for( size_t groupId = 0; groupId < _group.size(); ++groupId ) {
			groupGui[groupId] = AddPropertyGroup(_group[groupId].name.c_str());
		}
		
		std::vector<int> defectFovId;

		for (size_t nTrack = 0; nTrack < MAX_TRACKS; nTrack++)
			mapDefectName[nTrack].clear();

		for( size_t defectId = 0; defectId < _defect.size(); ++defectId ) {
			CString name;
	
			name.Format("Specifies the Defect Code for %s", _defect[defectId].description.c_str());

			COleVariant value = _defect[defectId].code.c_str();
			int groupId = _defect[defectId].groupId;

			std::vector < GroupInfo >::const_iterator g = std::find_if( _group.begin(), _group.end(),
				[groupId](GroupInfo & info) { return info.id == groupId; } );

			int nFov = -1;
			int nTrack = g->name.find("PVI TOP") != std::string::npos ? 0 : 1;
			if (groupId >= nPVIGroupId) {
				nFov = getFovIndexFromDefectName(_defect[defectId].description, nTrack, MAX_FOV, _defect[defectId].id);
			}
			defectFovId.push_back(nFov);

			if( g != _group.end() ) {
				//PVI TOP and PVI THR group
				if (groupId >= nPVIGroupId) {
					if (mapDefectName[nTrack].find(_defect[defectId].description.c_str()) != mapDefectName[nTrack].end()) {
						mapDefectName[nTrack][_defect[defectId].description.c_str()].push_back(defectId);
					}
					else {
						std::vector<int> defectItTemp;
						defectItTemp.push_back(defectId);
						mapDefectName[nTrack].insert(std::pair<CString, std::vector<int>>(_defect[defectId].description.c_str(), defectItTemp));
					}
				}
				else {
					AddPropertyItem(_defect[defectId].description.c_str(), value, 0, 0, name, EDIT_PARAM,
						groupGui[g - _group.begin()]);

					//Color Control
					_property[defectId] = new CMFCPropertyGridColorProperty(_T("     Defect Color"),
						RGB(210, 192, 254), NULL, _T("     Specifies the Defect Color"));

					groupGui[g - _group.begin()]->AddSubItem(_property[defectId]);

					_property[defectId]->EnableOtherButton(_T("Other..."));
					_property[defectId]->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
					_property[defectId]->SetColor(_defect[defectId].color);

					_defectGroupPosition[defectId] = itemGroupId[g - _group.begin()]++;
				}
			}
		}

		//PVI TOP and PVI THR group
		for (size_t groupId = nPVIGroupId; groupId < nPVIGroupId + theApp.m_nTotalTracks; ++groupId) {
			std::vector < GroupInfo >::const_iterator g = std::find_if(_group.begin(), _group.end(),
				[groupId](GroupInfo & info) { return info.id == groupId; });

			int nTrack = g->name.find("PVI TOP") != std::string::npos ? 0 : 1;

			CString strTemp;
			for (std::map<CString, std::vector<int>>::iterator it = mapDefectName[nTrack].begin(); it != mapDefectName[nTrack].end(); it++) {
				CMFCPropertyGridProperty* pDefectName = AddPropertyGroup(it->first);
				for (size_t i = 0; i < it->second.size(); i++) {
					size_t defectId = it->second[i];
					strTemp.Format("FOV %d Defect Code", defectFovId[defectId]);
					COleVariant value = _defect[defectId].code.c_str();
					AddPropertyItem(strTemp, value, 0, 0, "Dertemine Defect Error Code", EDIT_PARAM, groupGui[g - _group.begin()], TRUE, pDefectName);

					//Color Control
					_property[defectId] = new CMFCPropertyGridColorProperty(_T("     Defect Color"),
						RGB(210, 192, 254), NULL, _T("     Specifies the Defect Color"));

					
					pDefectName->AddSubItem(_property[defectId]);

					_property[defectId]->EnableOtherButton(_T("Other..."));
					_property[defectId]->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
					_property[defectId]->SetColor(_defect[defectId].color);
				}
				pDefectName->Expand(FALSE);
				groupGui[g - _group.begin()]->AddSubItem(pDefectName);
			}
		}
		
		for( size_t groupId = 0; groupId < _group.size(); ++groupId ) {
			m_ctrlDefetCodes.AddProperty(groupGui[groupId]);
		}
		
	}
}

void CDefectCodesDlg::RetrieveDefectCodesPage()
{
	string name;
	int nPVIGroupId = theApp.m_pImageSelectionDlg->nPVIGroupId;

	if( !_group.empty() ) {

		for( size_t defectId = 0; defectId < _defect.size(); ++defectId ) {
	
			int groupId = _defect[defectId].groupId;
	
			std::vector < GroupInfo >::const_iterator g = std::find_if( _group.begin(), _group.end(),
				[groupId](const GroupInfo & info) { return info.id == groupId; } );
	
			if( g != _group.end() ) {
				if (groupId < nPVIGroupId) {
					COleVariant value = RetrivePropertyItem(g - _group.begin(), _defectGroupPosition[defectId] * 2);
					_defect[defectId].code = CString(value).GetString();

					value = RetrivePropertyItem(g - _group.begin(), _defectGroupPosition[defectId] * 2 + 1);
					_defect[defectId].color = value.lVal;
					name = _defect[defectId].description;
					theApp.m_pImageSelectionDlg->setDefect(_defect[defectId]);
				}
			}
		}
	
		//PVI TOP and PVI THR group
		for (size_t groupId = nPVIGroupId; groupId < nPVIGroupId + theApp.m_nTotalTracks; ++groupId) {
			std::vector < GroupInfo >::const_iterator g = std::find_if(_group.begin(), _group.end(),
				[groupId](GroupInfo & info) { return info.id == groupId; });

			if (g != _group.end()) {
				int nTrack = g->name.find("PVI TOP") != std::string::npos ? 0 : 1;
				int nItem = g - _group.begin();
				int nCount = 0;
				for (std::map<CString, std::vector<int>>::iterator it = mapDefectName[nTrack].begin(); it != mapDefectName[nTrack].end(); it++) {
					int subItemCount = 0;
					for (size_t i = 0; i < it->second.size(); i++) {
						size_t defectId = it->second[i];
						
						COleVariant value = RetrivePropertyItem(nItem, nCount, TRUE, subItemCount++);
						_defect[defectId].code = CString(value).GetString();

						value = RetrivePropertyItem(nItem, nCount, TRUE, subItemCount++);
						_defect[defectId].color = value.lVal;

						theApp.m_pImageSelectionDlg->setDefect(_defect[defectId]);
					}
					nCount++;
				}
				
			}
			
		}

		
		CString name;
	
		name.Format("%s\\Defect code.dat", theApp.m_strMainCfgDir);
	
		theApp.m_pImageSelectionDlg->save( name.GetString() );
	}
}

COleVariant CDefectCodesDlg::RetrivePropertyItem(int nPropertyItem, int nSubItem, BOOL bSubPorperty, int nSubItemIndex)
{
	COleVariant cValue;
	CMFCPropertyGridProperty* pGroup;
	CMFCPropertyGridProperty* pSub;

	pGroup = m_ctrlDefetCodes.GetProperty(nPropertyItem);
	pSub   = pGroup->GetSubItem(nSubItem);

	cValue = pSub->GetValue();

	if(bSubPorperty){
		CMFCPropertyGridProperty* pSubItem;
		pSubItem   = pSub->GetSubItem(nSubItemIndex);
		cValue     = pSubItem->GetValue();
	}
	return cValue;
}

COleVariant CDefectCodesDlg::RetrivePropertyColor(int nPropertyItem, int nSubItem, BOOL bSubPorperty, int nSubItemIndex)
{
	COleVariant cColor;
	CMFCPropertyGridProperty* pGroup;
	CMFCPropertyGridProperty* pSub;

	pGroup = m_ctrlDefetCodes.GetProperty(nPropertyItem);
	pSub = pGroup->GetSubItem(nSubItem);
	
	cColor = pSub->GetValue();

	if(bSubPorperty){
		CMFCPropertyGridProperty* pSubItem;
		pSubItem   = pSub->GetSubItem(nSubItemIndex);
		cColor     = pSubItem->GetValue();
	}
	return cColor;
}

CMFCPropertyGridProperty* CDefectCodesDlg::AddPropertyGroup(CString strPropertyName)
{
	CMFCPropertyGridProperty* pSub;

	pSub = new CMFCPropertyGridProperty(_T(strPropertyName));

	return pSub;
}

void CDefectCodesDlg::AddPropertyItem(CString strPropertyName,
									 COleVariant cValue,
									 int nMin,
									 int nMax,
									 CString strTips,
									 int nType,
									 CMFCPropertyGridProperty* pGroup,
									 BOOL bSubProperty,
									 CMFCPropertyGridProperty* pSubGroup)
{
	CMFCPropertyGridProperty* pSubItem;

	if(nType == EDIT_PARAM){
		pSubItem = new CBoundedNumberSubProp(_T(strPropertyName), (COleVariant) (cValue), nMin, nMax, _T(strTips));
	}

	if(bSubProperty)
		pSubGroup->AddSubItem(pSubItem);
	else
		pGroup->AddSubItem(pSubItem);
}

void CDefectCodesDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
