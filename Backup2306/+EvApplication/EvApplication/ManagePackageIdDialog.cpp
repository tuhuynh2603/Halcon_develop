// ManagePackageIdDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ManagePackageIdDialog.h"
#include "Application.h"
#include "afxdialogex.h"
#include "CopyPVIInspectionDlg.h"
//#include "inspectionParameter.h"
#include "operatorOverloading.h"
#include "namespace_utilities_function.h"

enum
{
	EDIT_PARAM,
	CHECKBOX_PARAM,
	COMBO_PARAM
};

namespace {
	
	bool checkDuplicatePackageId(const std::string & id) {
		
		// 1. Retrieve all configuration name
		std::list< std::string > configurationList = getConfigurationFileNameList( theApp.m_strMainCfgDir, theApp.m_strMainCfgModeDir);
	
		// 2. loop through all configuration and check for the package id
		for(std::list< std::string >::iterator configurationIterator = configurationList.begin(); configurationIterator != configurationList.end(); ++configurationIterator ) {
			std::string fileLocation = theApp.m_strMainCfgDir + "\\"+ theApp.m_strMainCfgModeDir + "\\" + configurationIterator->c_str() + "\\Package ID.txt";
			
			// parse file information
			std::list < std::string > packageIdContainer = readWordsFromFile( fileLocation );
	
			// check if the package id exist on current configuration
			std::list < std::string >::iterator packageIdIterator = std::find_if(packageIdContainer.begin(), packageIdContainer.end(), 
				[&](const std::string & str) {
					return str == id;
				} );
	
			if( packageIdIterator != packageIdContainer.end() && !packageIdContainer.empty() ) {

				// Only check the duplication of the id on other package than the current package
				if( strcmp( configurationIterator->c_str(), theApp.m_strConfigFile ) != 0 ) {
					std::ostringstream stringStream;
					stringStream << "Duplicate ID: \"" << *packageIdIterator << "\" at Configuration \"" << *configurationIterator << "\". Please retry with another package id"; 
					AfxMessageBox( stringStream.str().c_str(), MB_OK | MB_ICONINFORMATION );
					return true;
				}
			}
		}

		return false;
	}
};

// ManagePackageIdDialog dialog

IMPLEMENT_DYNAMIC(ManagePackageIdDialog, CDialog)

ManagePackageIdDialog::ManagePackageIdDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ManagePackageIdDialog::IDD, pParent)
	, _elementCount(0)
	, _elementCountTemp(0)
{

}

ManagePackageIdDialog::~ManagePackageIdDialog()
{
}

void ManagePackageIdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PACKAGE_ID_PROPERTY_GRID, packageIDPropertyGrid);
	DDX_Text(pDX, IDC_EDIT_PACKAGE_ID_COUNT, _elementCount);
}


BEGIN_MESSAGE_MAP(ManagePackageIdDialog, CDialog)
	ON_BN_CLICKED(IDOK, &ManagePackageIdDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ManagePackageIdDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPLY_PACKAGE_COUNT, &ManagePackageIdDialog::OnBnClickedApplyPackageCount)
END_MESSAGE_MAP()


// ManagePackageIdDialog message handlers

BOOL ManagePackageIdDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::string stringStream;
	stringStream = theApp.m_strMainCfgDir + "\\" + theApp.m_strMainCfgModeDir + "\\" +  theApp.m_strConfigFile +"\\Package ID.txt";
	packageIdArray = readWordsFromFile( stringStream.c_str() );

	//CString stringStream;
	//stringStream = theApp.m_strMainCfgDir + "\\" + theApp.m_strConfigFile + "\\Package ID.txt";
	//packageIdArray = readWordsFromFile(stringStream.GetString());

	_elementCount = packageIdArray.size();

	initializeWindow();

	UpdateData(FALSE);

	return TRUE;
}

void ManagePackageIdDialog::initializeWindow()
{
	packageIDPropertyGrid.RemoveAll();
	packageIDPropertyGrid.EnableDescriptionArea();
	packageIDPropertyGrid.SetVSDotNetLook();
	packageIDPropertyGrid.MarkModifiedProperties();
	packageIDPropertyGrid.SetShowDragContext(FALSE);

	CRect rc;
	HDITEM hdItem;
	hdItem.mask = HDI_WIDTH;
	packageIDPropertyGrid.GetWindowRect(&rc);

	hdItem.cxy = rc.Width()/2;
	packageIDPropertyGrid.GetHeaderCtrl().SetItem(0, &hdItem);

	// Display Package ID
	ManagePackageIdPage();
}

void ManagePackageIdDialog::OnBnClickedOk()
{
	UpdateData( FALSE );
	RetrieveManagePackageIdPage( _elementCount );
}


void ManagePackageIdDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void ManagePackageIdDialog::ManagePackageIdPage()
{
	COleVariant value;
	
	CMFCPropertyGridProperty* packageIdGroup = AddPropertyGroup("Package ID");

	std::string subject;
	std::string description;
	std::ostringstream stringStream;

	for( size_t id = 0; id < _elementCount; ++id ) {
		
		if( !packageIdArray.empty() )
			value = packageIdArray.front().c_str();
		else
			value = "";

		stringStream.clear();
		stringStream.str(std::string());
		stringStream << "Package ID " << id + 1;
		subject = stringStream.str();
		description = "Specifies The Package ID To Link With Configuration.";

		AddPropertyItem(subject.c_str(), value, 0, INT_MAX, description.c_str(), EDIT_PARAM, packageIdGroup);

		if( !packageIdArray.empty() )
			packageIdArray.pop_front();
	}

	packageIDPropertyGrid.AddProperty(packageIdGroup);
}

void ManagePackageIdDialog::RetrieveManagePackageIdPage(size_t count)
{
	packageIdArray.clear();

	for( size_t id = 0; id < count; ++id ) {
		COleVariant value = RetrivePropertyItem( 0, static_cast<int>( id ) );
		packageIdArray.push_back( CString(value).GetString() );
	}

	if( !packageIdArray.empty() ) {
		packageIdArray.sort();
		packageIdArray.erase( std::unique( packageIdArray.begin(), packageIdArray.end() ), packageIdArray.end() );
		std::remove_if( packageIdArray.begin(), packageIdArray.end(), [](const std::string & s) { return s.empty(); } );

		// Do not save the package id there is duplication of id
		if( std::any_of( packageIdArray.begin(), packageIdArray.end(), [&](const std::string & id) { return checkDuplicatePackageId( id ); } ) ) {
			return;
		}
	}

//	std::ostringstream stringStream;
	CString strPackageIdPath;
//	stringStream << theApp.m_strMainCfgDir << "\\" << theApp.configurationCategories  "\\" << theApp.m_strConfigFile << "\\Package ID.txt\0";
	strPackageIdPath = theApp.m_strMainCfgDir  + "\\" + theApp.m_strMainCfgModeDir + "\\"+ theApp.m_strConfigFile + "\\Package ID.txt";
//	writeWordsToFile(stringStream.str(), packageIdArray);
	writeWordsToFile(strPackageIdPath.GetString(), packageIdArray );
}

CMFCPropertyGridProperty* ManagePackageIdDialog::AddPropertyGroup(CString strPropertyName)
{
	CMFCPropertyGridProperty* pSub;

	pSub = new CMFCPropertyGridProperty(_T(strPropertyName));

	return pSub;
}

void ManagePackageIdDialog::AddPropertyItem(CString strPropertyName,
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

COleVariant ManagePackageIdDialog::RetrivePropertyItem(int nPropertyItem,
													   int nSubItem,
													   BOOL bSubPorperty,
													   int nSubItemIndex)
{
	COleVariant cColor;
	CMFCPropertyGridProperty* pGroup;
	CMFCPropertyGridProperty* pSub;

	pGroup = packageIDPropertyGrid.GetProperty(nPropertyItem);
	pSub = pGroup->GetSubItem(nSubItem);
	
	cColor = pSub->GetValue();

	if(bSubPorperty){
		CMFCPropertyGridProperty* pSubItem;
		pSubItem   = pSub->GetSubItem(nSubItemIndex);
		cColor     = pSubItem->GetValue();
	}
	return cColor;
}

void ManagePackageIdDialog::OnBnClickedApplyPackageCount()
{
	_elementCountTemp = _elementCount;

	UpdateData(TRUE);

	if( _elementCount < 0 ) {
		if( AfxMessageBox("Package ID count must be more than 0. Please try again.", MB_OK | MB_ICONERROR ) == IDOK )
			return;
	}
	// Do not allow user to resize property grid if the data count is less than the entered data count
	else if( _elementCount < _elementCountTemp ) {

		std::ostringstream stringStream;
		stringStream << "Error! Changes cannot be made. Please try again with package ID count more than " << _elementCountTemp << ".";
		if( AfxMessageBox( stringStream.str().c_str(), MB_OK | MB_ICONERROR ) == IDOK ) {
			_elementCount = _elementCountTemp;
			UpdateData(FALSE);
			return;
		}
	}

	RetrieveManagePackageIdPage( _elementCountTemp );

	initializeWindow();
}