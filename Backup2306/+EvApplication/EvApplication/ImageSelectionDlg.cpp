// ImageSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include "afxdialogex.h"
#include "AppView.h"
#include <direct.h>
#include <iomanip>
#include "MathValidations.h"
#include "namespace_utilities_function.h"
#include "operatorOverloading.h"
#include "ImageSelectionDlg.h"
#include "VerificationMode.h"


void DefectContainer::addGroup(const std::string & name, int id)
{
	if (std::none_of(_group.begin(), _group.end(), [id](const GroupInfo & info) { return info.id == id; })) {
		_group.push_back(GroupInfo(name, id));
	}
}

void DefectContainer::addDefect(const std::string & description, const std::string & code, int id, int groupId, bool good)
{
	addDefect(description, code, id, groupId, RGB(128, 128, 128), good);
}

void DefectContainer::addDefect(const std::string & description, const std::string & code, int id, int groupId,
	COLORREF color, bool good)
{
	if (std::none_of(_defect.begin(), _defect.end(), [id](const ItemInfo & info) { return info.id == id; })) {
		_defect.push_back(ItemInfo(description, code, id, groupId, color, good));
	}
}

void DefectContainer::setDefect(const ItemInfo & info)
{
	std::vector < ItemInfo >::iterator v = std::find_if(_defect.begin(), _defect.end(),
		[info](const ItemInfo & item) { return info.id == item.id; });

	if (v != _defect.end())
		v->set(info);
}

const std::vector < ItemInfo > & DefectContainer::getDefect() const
{
	return _defect;
}

std::string DefectContainer::getCode(int id) const
{
	std::vector < ItemInfo >::const_iterator v = std::find_if(_defect.begin(), _defect.end(),
		[id](const ItemInfo & item) { return id == item.id; });

	if (v != _defect.end())
		return v->code;
	else
		return "unknown code";
}

int DefectContainer::getId(int id) const
{
	std::vector < ItemInfo >::const_iterator v = std::find_if(_defect.begin(), _defect.end(),
		[id](const ItemInfo & item) { return id == item.id; });

	if (v != _defect.end())
		return v->id;
	else
		return UNKNOWN_DEFECT;
}

int DefectContainer::getCode(std::string code) const					//Girish
{
	std::vector < ItemInfo >::const_iterator v = std::find_if(_defect.begin(), _defect.end(),
		[code](const ItemInfo & item) { return strcmp(code.c_str(), item.code.c_str()) == 0; });

	if (v != _defect.end())
		return v->id;
	else
		return UNKNOWN_DEFECT;
}

std::vector < ItemInfo >::iterator DefectContainer::getInfo(int id)
{
	return std::find_if(_defect.begin(), _defect.end(), [id](const ItemInfo & item) { return id == item.id; });
}

std::vector < ItemInfo >::const_iterator DefectContainer::getInfo(int id) const
{
	return std::find_if(_defect.begin(), _defect.end(), [id](const ItemInfo & item) { return id == item.id; });
}

bool DefectContainer::isGood(int id) const
{
	std::vector < ItemInfo >::const_iterator v = std::find_if(_defect.begin(), _defect.end(),
		[id](const ItemInfo & item) { return id == item.id; });

	if (v != _defect.end())
		return v->good;
	else
		return false;
}

GroupInfo DefectContainer::getGroup(int id) const
{
	std::vector < GroupInfo >::const_iterator v = std::find_if(_group.begin(), _group.end(),
		[id](const GroupInfo & item) { return id == item.id; });

	if (v != _group.end())
		return *v;
	else
		return GroupInfo();
}

const std::vector < GroupInfo > & DefectContainer::getGroup() const
{
	return _group;
}

void DefectContainer::load(const std::string & name)
{
	int i = 0;
	CIniFile parameter;

	parameter.SetPath(name.c_str());
	if (parameter.ReadFile()) {

		for (std::vector < ItemInfo >::iterator item = _defect.begin(); item != _defect.end(); ++item) {
			CString sectionName;

			sectionName.Format("Defect %d", item->id);
			item->color =  parameter.GetLong(sectionName.GetString(), "Color", item->color);
			item->code = parameter.GetCString(sectionName.GetString(), "Code", item->code.c_str());
			setDefect(_defect[item-_defect.begin()]);
			//theApp.UpdateDefectCodes(item->description.c_str(), item->code.c_str());
		}
	}
	save(name);
}

void DefectContainer::save(const std::string & name) const
{
	int i = 0;
	CIniFile parameter;

	parameter.SetPath(name.c_str());

	for (std::vector < ItemInfo >::const_iterator item = _defect.begin(); item != _defect.end(); ++item) {
		CString sectionName;

		sectionName.Format("Defect %d", item->id);
		parameter.SetLong(sectionName.GetString(), "Color", item->color);
		parameter.SetCString(sectionName.GetString(), "Code", item->code.c_str());
	//	theApp.defctCodeArr[i++] = item->code;
	}

	parameter.WriteFile();
}

DeviceContainer::DeviceContainer()
	: _blockXCount(0)
	, _blockYCount(0)
	, _deviceXCount(0)
	, _deviceYCount(0)
	, _goodDeviceCount(0)
	, _badDeviceCount(0)
{
}

DeviceContainer::~DeviceContainer()
{ }

size_t DeviceContainer::goodDieCount() const
{
	return _dieCode.size() - _badDieNo.size();
}

size_t DeviceContainer::badDieCount() const
{
	return _badDieNo.size() - rejectDieCount();
}

size_t DeviceContainer::goodDeviceCount() const
{
	return _goodDeviceCount;
}

size_t DeviceContainer::badDeviceCount() const
{
	return _badDeviceCount;
}

size_t DeviceContainer::remainDeviceCount() const
{
	return _deviceCode.size() - (_goodDeviceCount + _badDeviceCount);
}

size_t DeviceContainer::totalDeviceCount()  const
{
	return _deviceCode.size();
}

size_t DeviceContainer::rejectDeviceCount() const
{
	size_t count = 0;

	for (std::vector <int>::const_iterator device = _deviceCode.begin(); device != _deviceCode.end(); ++device) {
		if (isGood(*device) && *device != GOOD_UNIT && *device > 0)
			++count;
	}

	return count;
}

size_t DeviceContainer::rejectDieCount() const
{
	size_t count = 0;

	for (std::vector <int>::const_iterator die = _dieErrorCode.begin(); die != _dieErrorCode.end(); ++die) {
		if (isGood(*die) && *die != GOOD_UNIT && *die != NOT_INSPECTED && *die > 0)
			++count;
	}

	return count;
}

std::vector < std::pair < int, size_t > > DeviceContainer::getDefectReport() const
{
	std::vector < std::pair < int, size_t > > report(getDefect().size());

	for (size_t defectId = 0; defectId < getDefect().size(); ++defectId) {
		report[defectId].first = getDefect()[defectId].id;
		report[defectId].second = 0;
	}

	int i = 0;
	int nTempRowId, nTempColumnId, nTempDieId, nTempFov;
	CString strTempKey;
	for (std::vector < int >::const_iterator device = _deviceCode.begin(); device != _deviceCode.end(); ++device) {
		int nErrorCode = *device;
		report[getInfo(getId(nErrorCode)) - getDefect().begin()].second++;

		i++;
	}

	return report;
}

std::map < std::string, size_t > DeviceContainer::getCodeReport() const
{
	std::map < std::string, size_t > report;

	for (size_t defectId = 0; defectId < getDefect().size(); ++defectId) {
		if (!getDefect()[defectId].good)
			report.insert(std::pair<std::string, size_t>(getDefect()[defectId].code, 0u));
	}

	int i = 0;
	int nTempRowId, nTempColumnId, nTempDieId, nTempFov;
	CString strTempKey;
	for (std::vector < int >::const_iterator device = _deviceCode.begin(); device != _deviceCode.end(); ++device) {
		int nErrorCode = *device;

		if (!getInfo(getId(nErrorCode))->good)
			++report[getInfo(getId(nErrorCode))->code];

		i++;
	}

	return report;
}

std::map < std::string, size_t > DeviceContainer::getRejectReport() const
{
	std::map < std::string, size_t > report;

	for (size_t defectId = 0; defectId < getDefect().size(); ++defectId) {
		if (getDefect()[defectId].good)
			report.insert(std::pair<std::string, size_t>(getDefect()[defectId].code, 0u));
	}

	int i = 0;
	int nTempRowId, nTempColumnId, nTempDieId, nTempFov;
	CString strTempKey;
	for (std::vector < int >::const_iterator device = _deviceCode.begin(); device != _deviceCode.end(); ++device) {
		int nErrorCode = *device;

		if (getInfo(getId(nErrorCode))->good && getId(nErrorCode) != GOOD_UNIT)
			++report[getInfo(getId(nErrorCode))->code];

		i++;
	}

	return report;
}

bool DeviceContainer::areContinuousBadDevices(size_t minimumCount) const
{
	if (minimumCount == 0 || minimumCount > _deviceCode.size())
		return false;

	size_t count = 0;

	for (std::vector < int >::const_iterator code = _deviceCode.begin(); code != _deviceCode.end(); ++code) {
		if (!isGood(*code))
			++count;
		else
			count = 0;

		if (count >= minimumCount)
			return true;
	}

	return false;
}

void DeviceContainer::_setReScan()
{
	OutputDebugLogTo(8, TRUE, "In set rescan");

	theApp.UpdatePrintBarOnReScan(theApp.m_pImageSelectionDlg->badDeviceCount());
	theApp.UpdateStatisticsOnReScan(theApp.m_pImageSelectionDlg->goodDieCount(), theApp.m_pImageSelectionDlg->badDieCount());

	long nCameraGrabId;
	int nMappingId;
	for (int die = 0; die < _badDieNo.size(); die++)
	{
		int badDie = _badDieNo[die];

		theApp.motionParameters.SetReScanStatus(badDie, true);

		std::fill(_dieCode[badDie].begin(), _dieCode[badDie].end(), NOT_INSPECTED);
		std::fill(_dieThrCode[badDie].begin(), _dieThrCode[badDie].end(), NOT_INSPECTED);
		std::fill(_encapTopCode[badDie].begin(), _encapTopCode[badDie].end(), NOT_INSPECTED);
		std::fill(_dieTopCode[badDie].begin(), _dieTopCode[badDie].end(), NOT_INSPECTED);
	//	std::fill(_dieErrorCode.begin(), _dieErrorCode.end(), NOT_INSPECTED);
		_dieErrorCode[badDie] = NOT_INSPECTED;
		for (int nFov = 0; nFov < _dieCode[badDie].size(); nFov++) {
			theApp.motionParameters.GetCameraGrabIDTHR(0, badDie, nFov, nCameraGrabId);
			theApp.motionParameters.getCameraGrabIdTHR(nCameraGrabId, nMappingId);
			_deviceCode[nMappingId] = NOT_INSPECTED;
		}
	}

	_badDeviceCount = 0;
	_badDeviceCountTop = 0;
	strDefectName.clear();
	_badDieNo.clear();
	std::fill(_deviceCountDone.begin(), _deviceCountDone.end(), false);

}

void DeviceContainer::_resetResults()
{
	_goodDeviceCount = 0;
	_badDeviceCount = 0;
	_badDeviceCountTop = 0;
	strDefectName.clear();
	_badDieNo.clear();

	std::fill(_deviceCountDone.begin(), _deviceCountDone.end(), false);
	std::fill(_dieErrorCode.begin(), _dieErrorCode.end(), NOT_INSPECTED);

}

void DeviceContainer::_reset()
{
	_goodDeviceCount = 0;
	_badDeviceCount = 0;
	_badDeviceCountTop = 0;
	strDefectName.clear();
	_badDieNo.clear();


	std::fill(_deviceCode.begin(), _deviceCode.end(), NOT_INSPECTED);
	std::fill(_deviceCountDone.begin(), _deviceCountDone.end(), false);
	std::fill(_dieErrorCode.begin(), _dieErrorCode.end(), NOT_INSPECTED);
	
	for(int i=0; i< _dieCode.size(); i++)
		std::fill(_dieCode[i].begin(), _dieCode[i].end(), NOT_INSPECTED);

	for (int i = 0; i< _dieThrCode.size(); i++)
		std::fill(_dieThrCode[i].begin(), _dieThrCode[i].end(), NOT_INSPECTED);

	for (int i = 0; i< _encapTopCode.size(); i++)
		std::fill(_encapTopCode[i].begin(), _encapTopCode[i].end(), NOT_INSPECTED);

	for (int i = 0; i< _dieTopCode.size(); i++)
		std::fill(_dieTopCode[i].begin(), _dieTopCode[i].end(), NOT_INSPECTED);
}


bool DeviceContainer::_setDieStatus(int dieId, int nFov, int code)
{
	if (dieId >= _dieCode.size() || nFov >= _dieCode[dieId].size())
		return false;

	if (/*(_dieCode[dieId][nFov] == NOT_INSPECTED || _dieCode[dieId][nFov] == GOOD_UNIT) && */_dieCode[dieId][nFov] != code) {
		_dieCode[dieId][nFov] = code;

		if ((code != GOOD_UNIT && _dieErrorCode[dieId] == GOOD_UNIT) || _dieErrorCode[dieId] == NOT_INSPECTED)
			_dieErrorCode[dieId] = code;

		if (code != GOOD_UNIT) {
			if (std::none_of(_badDieNo.begin(), _badDieNo.end(), [dieId](int & val) { return dieId == val; })) {
				_badDieNo.push_back(dieId);
			}	
		}
	}

	return true;
}

bool DeviceContainer::_setDeviceStatus(size_t id, int code, bool bCount, bool bOverWrite)
{
	if (id >= _deviceCode.size() || _deviceCode[id] == MISSING_DIE)
		return false;

	bool bReturnValue = false;
	int nDieNo;
//	theApp.motionParameters.GetDieMappingNo(id, nDieNo);

	if ((_deviceCode[id] == NOT_INSPECTED || _deviceCode[id] == GOOD_UNIT)  && _deviceCode[id] != code && !bOverWrite) {
		_deviceCode[id] = code;
		//	_deviceCode[id] = getId(code);
		bReturnValue = true;

	//	_deviceErrorCode[nDieNo] = code;
	}

	if (bCount && !_deviceCountDone[id]) {
		if (code == GOOD_UNIT)
			++_goodDeviceCount;
		else {
			if(strDefectName.empty() )
				strDefectName = getCode(code);
			++_badDeviceCount;
		}
		_deviceCountDone[id] = true;	
		bReturnValue = true;
	}
	else if (!bCount) {
		if (code != GOOD_UNIT) {
			++_badDeviceCountTop;
			if (strDefectName.empty())
				strDefectName = getCode(code);
		}
	}
	if (bOverWrite) {
		if ((_deviceCode[id] == NOT_INSPECTED || _deviceCode[id] == GOOD_UNIT) && _deviceCode[id] != code) {
			int nErrorCode = code;
			if (_deviceCode[id] == GOOD_UNIT) {
				_goodDeviceCount--;
				_badDeviceCount++;

				theApp.m_LotSummary.m_TotalDisplaySummary[STATION_THR].lTotalUnitsPass--;
				theApp.m_LotSummary.m_TotalDisplaySummary[STATION_THR].lTotalUnitsFail++;
				theApp.m_LotSummary.m_TotalDisplaySummary[STATION_THR].dTotalYield = double(theApp.m_LotSummary.m_TotalDisplaySummary[STATION_THR].lTotalUnitsPass) /
				double(theApp.m_LotSummary.m_TotalDisplaySummary[STATION_THR].lTotalUnitsInspected) * 100 + 0.04;

				int nErrorCode = code;
				strDefectName = getCode(nErrorCode);
			}
			_deviceCode[id] = nErrorCode;

			bReturnValue = true;
		}
	}


	return bReturnValue;
}

void DeviceContainer::_setTopThrDeviceStatus(long CameraId, int code, int nStationId, int nDocId )
{
	int nRPos, nCpos;
	int nDeviceNo;
	int nFOV;
	if (nStationId == STATION_TOP) {
		theApp.motionParameters.GetDeviceDetailsTOP(CameraId, nRPos, nCpos, nDeviceNo, nFOV);
		if (nDocId == 0){

			_dieTopCode[nDeviceNo][nFOV] = code;
		}

		else if (nDocId) {

			_encapTopCode[nDeviceNo][nFOV] = code;
		}
	}

	if (nStationId == STATION_THR) {
		theApp.motionParameters.GetDeviceDetailsTHR(CameraId, nRPos, nCpos, nDeviceNo, nFOV);
		_dieThrCode[nDeviceNo][nFOV] = code;
	}

}

bool DeviceContainer::_updateDieResults(int nDie, int nErrorCode)
{
	/*int nTotalFOV = theApp.motionParameters.GetTotalFOVPerDieTHR();
	for(int i = 0; i < nTotalFOV; i++) {
		theApp.motionParameters.GetCameraGrabIDTHR(0, )
	}*/
	return false;
}

void DeviceContainer::_resize(size_t blockXCount, size_t blockYCount, size_t deviceXCount, size_t deviceYCount)
{
	if (blockXCount > 0 && blockYCount > 0 && deviceXCount > 0 && deviceYCount > 0) {
		_blockXCount = blockXCount;
		_blockYCount = blockYCount;
		_deviceXCount = deviceXCount;
		_deviceYCount = deviceYCount;

		size_t totalCount = _blockXCount * _blockYCount * _deviceXCount * _deviceYCount;

		_deviceCode.resize(totalCount);
		_deviceRoi.resize(totalCount);
		_deviceCountDone.resize(totalCount);
		_dieTopCode.resize(_blockXCount * _blockYCount);
		_encapTopCode.resize(_blockXCount * _blockYCount);
		_dieThrCode.resize(_blockXCount * _blockYCount);
		_dieCode.resize(_blockXCount * _blockYCount);
		_dieErrorCode.resize(_blockXCount * _blockYCount);
		_dieRoi.resize(_blockXCount * _blockYCount);
		_deviceErrorCode.resize(_blockXCount * _blockYCount);

		for (int i = 0; i < _dieCode.size(); i++) {
			_dieCode[i].resize(_deviceXCount * _deviceYCount);
			std::fill(_dieCode[i].begin(), _dieCode[i].end(), NOT_INSPECTED);
		}

		for (int i = 0; i < _dieThrCode.size(); i++) {
			_dieThrCode[i].resize(_deviceXCount * _deviceYCount);
			std::fill(_dieThrCode[i].begin(), _dieThrCode[i].end(), NOT_INSPECTED);
		}

		for (int i = 0; i < _dieTopCode.size(); i++) {
			_dieTopCode[i].resize(_deviceXCount * _deviceYCount);           
			std::fill(_dieTopCode[i].begin(), _dieTopCode[i].end(), NOT_INSPECTED);
		}

		for (int i = 0; i < _encapTopCode.size(); i++) {
			_encapTopCode[i].resize(1);            
			std::fill(_encapTopCode[i].begin(), _encapTopCode[i].end(), NOT_INSPECTED);
		}
		std::fill(_deviceCode.begin(), _deviceCode.end(), NOT_INSPECTED);
		std::fill(_deviceRoi.begin(), _deviceRoi.end(), CRect());
		std::fill(_deviceCountDone.begin(), _deviceCountDone.end(), false);
		std::fill(_dieErrorCode.begin(), _dieErrorCode.end(), NOT_INSPECTED);
		std::fill(_dieRoi.begin(), _dieRoi.end(), CRect());

		_goodDeviceCount = 0;
		_badDeviceCount = 0;
		_badDeviceCountTop = 0;
	}
}

void CDialogPane::Initialize()
{
	Invalidate(TRUE);

	scrollBarH.Create(SBS_HORZ | SBS_TOPALIGN | WS_CHILD, CRect(5, 5, 100, 30), this, IDC_MAPPING_SCROLLBAR_H);
	scrollBarV.Create(SBS_VERT | SBS_LEFTALIGN | WS_CHILD, CRect(5, 30, 30, 130), this, IDC_MAPPING_SCROLLBAR_V);

	GetWindowRect(rcSubstrateRelative);
	GetClientRect(rcClientWnd);

	//Create the ToolTip control
	if(!m_ToolTip.Create(this)) {
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

	InitiDieControls();
}

int CDialogPane::ResetDieStates()
{
	EnterCriticalSection(&CriticalSectionData);

	if (nTotalNo < 1) {
		LeaveCriticalSection(&CriticalSectionData);
		return -1;
	}

	if(theApp.pGlobalBackgroundThread->IsReScanEnabled())
		_setReScan();
	else
		_reset();

	LeaveCriticalSection(&CriticalSectionData);

	Invalidate();

	return 0;
}

void CDialogPane::InitiDieControls()
{
	int i;
	CRect rcItem;

	EnterCriticalSection(&CriticalSectionData);
	nDeviceNoPerSubstrateX = 30;
	nDeviceNoPerSubstrateY = 2;
	nDieNoPerDeviceX = 1;
	nDieNoPerDeviceY = 3;

	nDieNoPerDeviceTOPX = 1;
	nDieNoPerDeviceTOPY = 5;

	m_pApp->motionParameters.GetMappingParameters(nDeviceNoPerSubstrateX, nDeviceNoPerSubstrateY, nDieNoPerDeviceX, nDieNoPerDeviceY);
	m_pApp->motionParameters.GetMappingParametersTOP(nDieNoPerDeviceTOPX, nDieNoPerDeviceTOPY);


	nTotalNo = nDeviceNoPerSubstrateX * nDeviceNoPerSubstrateY * nDieNoPerDeviceTOPX * nDieNoPerDeviceTOPY;
	_resize(nDeviceNoPerSubstrateX, nDeviceNoPerSubstrateY, nDieNoPerDeviceTOPX, nDieNoPerDeviceTOPY);
	//Create drawnings areas
	CString strWindowTitle;
	strWindowTitle.Format("Substrate Mapping:  %d units. %dx%d blocks, %dx%d units per block || Zoom %d%%",
		nTotalNo, nDeviceNoPerSubstrateX, nDeviceNoPerSubstrateY, nDieNoPerDeviceTOPX, nDieNoPerDeviceTOPY, (1 << (nZoom - 1)) * 100);

	SetWindowText(strWindowTitle);

//Resize dialog
	const int nScreenXSize = GetSystemMetrics(SM_CXSCREEN) / 2;
	const int nScreenYSize = GetSystemMetrics(SM_CYSCREEN) / 2;

	const int nDiePerX = nDeviceNoPerSubstrateX * nDieNoPerDeviceTOPX;
	const int nDiePerY = nDeviceNoPerSubstrateY * nDieNoPerDeviceTOPY;

	int nDeviceBordersX = nDeviceNoPerSubstrateX * 3 - 1;
	int nDeviceBordersY = nDeviceNoPerSubstrateY * 3 - 1;

	int nFreeSpaceDieX = nDiePerX + nDeviceNoPerSubstrateX;
	int nFreeSpaceDieY = nDiePerY + nDeviceNoPerSubstrateY;

	int nMaxScreenDrawX = nScreenXSize - nDeviceBordersX - nFreeSpaceDieX - nWindowOffset;
	int nMaxScreenDrawY = nScreenYSize - nDeviceBordersY - nFreeSpaceDieY - nWindowOffset;

	nDieRectSize = nMaxScreenDrawX / nDiePerX < nMaxScreenDrawY / nDiePerY ? nMaxScreenDrawX / nDiePerX : nMaxScreenDrawY / nDiePerY;

	if(nDieRectSize < 2)
		nDieRectSize = 2;

	if(nDieRectSize > 20)
		nDieRectSize = 20;

	//Creat die rectangles
	int nDieOffsetX = 0;
	int nDieOffsetY = 0;
	int nDiePerLine = nDeviceNoPerSubstrateX * nDieNoPerDeviceTOPX;

	int nX;
	int nY;
	int nDieDeviceIDX;
	int nDieDeviceIDY;
	int nDie = 0;

	CRect firstUnitBlock, endUnitBLock;
	for(i = 0; i<nTotalNo; i++) {

		nDieOffsetX = i % nDiePerLine;
		nDieDeviceIDX = nDieOffsetX / nDieNoPerDeviceTOPX;
		nDieOffsetX = nDieOffsetX % nDieNoPerDeviceTOPX;

		nDieOffsetY = i / nDiePerLine;
		nDieDeviceIDY = nDieOffsetY / nDieNoPerDeviceTOPY;
		nDieOffsetY = nDieOffsetY % nDieNoPerDeviceTOPY;

		nX = nDieDeviceIDX * (3 + 1 + (nDieRectSize + 1) * nDieNoPerDeviceTOPX) + 2 + (nDieRectSize + 1) * nDieOffsetX + nWindowOffset / 2 + rcMappingRelative.left;
		nY = nDieDeviceIDY * (10 + 1 + (nDieRectSize + 1) * nDieNoPerDeviceTOPY) + 2 + (nDieRectSize + 1) * nDieOffsetY  + nWindowOffset / 2 + rcMappingRelative.top;

		if (theApp.m_bSubstrateMappingStyle)
			nY = nY+(nDieDeviceIDY * nDieRectSize * 2 );

		_deviceRoi[i] = CRect(nX, nY, nX + nDieRectSize, nY + nDieRectSize);

		if (nDieOffsetY == nDieNoPerDeviceTOPY - 1 && nDieOffsetX == 0&& theApp.m_bSubstrateMappingStyle)
			_dieRoi[nDie++] = CRect(nX, nY + (nDieRectSize + 1) , nX + nDieRectSize * (nDieNoPerDeviceTOPX + 1) -3, nY + (nDieRectSize + 3) + nDieRectSize*2 -6 );

	}

	nZoom = 1;

	rcSubstrateRelative = CRect(0,
		0,
		nX + nDieRectSize + nWindowOffset/2,
		nY + nDieRectSize + nWindowOffset/2);

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	nScrollBarHMax = rcSubstrateRelative.Width() - nWindowOffset;
	nScrollBarVMax = rcSubstrateRelative.Height() - nWindowOffset;
	nHorizontalScrollBar = 0;
	nVerticalScrollBar = 0;

	CRect ClientRect;
	GetClientRect(ClientRect);
	int nVScrollMax;
	int nHScrollMax;
	if(ClientRect.Height() < rcSubstrateRelative.Height())
		nVScrollMax = (rcSubstrateRelative.Height() - ClientRect.Height()) + 20;// + nWindowOffset + 20/*scrolbar width*/;
	else
		nVScrollMax = 0;

	if(ClientRect.Width() < rcSubstrateRelative.Width())
		nHScrollMax = rcSubstrateRelative.Width() - ClientRect.Width() + nWindowOffset + 20;
	else
		nHScrollMax = 0;


	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	info.nMin = 0;
	info.nMax = nScrollBarHMax*nZoom;
	info.nPage = (nScrollBarHMax*nZoom) / 10;//rcClientWnd.Width();
	info.nPos = nHorizontalScrollBar;
	info.nTrackPos = nHorizontalScrollBar;

	scrollBarH.SetScrollInfo(&info);

	info.nMax = nScrollBarVMax*nZoom;
	info.nPage = (nScrollBarVMax*nZoom) / 10;//rcClientWnd.Height();
	info.nPos = nVerticalScrollBar;
	info.nTrackPos = nVerticalScrollBar;

	scrollBarV.SetScrollInfo(&info);
	
	LeaveCriticalSection(&CriticalSectionData);

	Invalidate();
}

//void CDialogPane::_reset()
//{
//	_goodDeviceCount = 0;
//	_badDeviceCount = 0;
//
//	std::fill(_deviceCode.begin(), _deviceCode.end(), NOT_INSPECTED);
//	std::fill(_deviceCountDone.begin(), _deviceCountDone.end(), false);
//	Invalidate();
//}

//void CDialogPane::_resize(size_t blockXCount, size_t blockYCount, size_t deviceXCount, size_t deviceYCount)
//{
//	if(blockXCount > 0 && blockYCount > 0 && deviceXCount > 0 && deviceYCount > 0) {
//		_blockXCount = blockXCount;
//		_blockYCount = blockYCount;
//		_deviceXCount = deviceXCount;
//		_deviceYCount = deviceYCount;
//
//		size_t totalCount = _blockXCount * _blockYCount * _deviceXCount * _deviceYCount;
//
//		_deviceCode.resize(totalCount);
//		_deviceRoi.resize(totalCount);
//		_deviceCountDone.resize(totalCount);
//		
//		std::fill(_deviceCode.begin(), _deviceCode.end(), NOT_INSPECTED);
//		std::fill(_deviceRoi.begin(), _deviceRoi.end(), CRect());
//		std::fill(_deviceCountDone.begin(), _deviceCountDone.end(), false);
//
//		_goodDeviceCount = 0;
//		_badDeviceCount = 0;
//	}
//}


void CDialogPane::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch(nSBCode) {
		case SB_LINEUP:
			if(nVerticalScrollBar > 0)
				nVerticalScrollBar--;
			break;
		case SB_LINEDOWN: //+1
			if(nVerticalScrollBar < nScrollBarVMax*nZoom - 1)
				nVerticalScrollBar++;
			break;
		case SB_PAGEUP://-10
			if(nVerticalScrollBar > 10 * nZoom)
				nVerticalScrollBar -= 10 * nZoom;
			break;
		case SB_PAGEDOWN://+10
			if(nVerticalScrollBar < nScrollBarVMax*nZoom - 10 * nZoom)
				nVerticalScrollBar += 10 * nZoom;
			break;
		case SB_THUMBPOSITION:
			nVerticalScrollBar = (int) nPos;
			break;
		case SB_THUMBTRACK:
			nVerticalScrollBar = nPos;
			break;

		default:
			return;
	}

	scrollBarV.SetScrollPos(nVerticalScrollBar, TRUE);

	Invalidate();

	CDockablePane::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDialogPane::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch(nSBCode) {
		case SB_LINEUP:
			if(nHorizontalScrollBar > 0)
				nHorizontalScrollBar--;
			break;
		case SB_LINEDOWN: //+1
			if(nHorizontalScrollBar < nScrollBarHMax*nZoom - 1)
				nHorizontalScrollBar++;
			break;
		case SB_PAGEUP://-10
			if(nHorizontalScrollBar > 10 * nZoom)
				nHorizontalScrollBar -= 10 * nZoom;
			break;
		case SB_PAGEDOWN://+10
			if(nHorizontalScrollBar < nScrollBarHMax*nZoom - 10 * nZoom)
				nHorizontalScrollBar += 10 * nZoom;
			break;
		case SB_THUMBPOSITION:
			nHorizontalScrollBar = nPos;
			break;
		case SB_THUMBTRACK:
			nHorizontalScrollBar = nPos;
			break;

		default:
			return;
	}

	scrollBarH.SetScrollPos(nHorizontalScrollBar, TRUE);

	Invalidate();

	CDockablePane::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDialogPane::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta > 0)
		Zoom(true);
	else
		Zoom(false);

	return CDockablePane::OnMouseWheel(nFlags, zDelta, pt);
}

void CDialogPane::Zoom(bool bZoomIn)
{
	bool bZooming = false;
	nPrevZoom = nZoom;

	if(bZoomIn) {
		if(nZoom < 3) {
			nZoom++;
			bZooming = true;
		}
	}
	else {
		if(nZoom > 1) {
			nZoom--;
			bZooming = true;
		}
	}

	if(bZooming) {
		if(bMouseLButtonPressed) {
			GetCursorPos(&ptMouseInitPos);
			ScreenToClient(&ptMouseInitPos);
		}

		GetClientRect(rcClientWnd);

		CString strWindowTitle;

		strWindowTitle.Format("Substrate results:  %d units. %dx%d blocks, %dx%d units per block || Zoom %d%%",
			nTotalNo, nDeviceNoPerSubstrateX, nDeviceNoPerSubstrateY, nDieNoPerDeviceTOPX, nDieNoPerDeviceTOPY, (1 << (nZoom - 1)) * 100);

		SetWindowText(strWindowTitle);

		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;
		info.nMin = 0;

		info.nMax = nScrollBarHMax*nZoom;
		info.nPage = (nScrollBarHMax*nZoom) / 10;
		nHorizontalScrollBar = nHorizontalScrollBar * nPrevZoom / nZoom;
		if(nHorizontalScrollBar > nScrollBarHMax*(nZoom - 1))
			nHorizontalScrollBar = nScrollBarHMax*(nZoom - 1);
		info.nPos = nHorizontalScrollBar;
		info.nTrackPos = nHorizontalScrollBar;

		scrollBarH.SetScrollInfo(&info);

		info.nMax = nScrollBarVMax*nZoom;
		info.nPage = (nScrollBarVMax*nZoom) / 10;
		nVerticalScrollBar = nVerticalScrollBar * nPrevZoom / nZoom;
		if(nVerticalScrollBar > nScrollBarVMax*(nZoom - 1))
			nVerticalScrollBar = nScrollBarVMax*(nZoom - 1);
		info.nPos = nVerticalScrollBar;
		info.nTrackPos = nVerticalScrollBar;

		scrollBarV.SetScrollInfo(&info);

		if(nScrollBarHMax*nZoom > rcClientWnd.Width())
			scrollBarH.ShowWindow(SW_SHOW);
		else
			scrollBarH.ShowWindow(SW_HIDE);

		if(nScrollBarVMax*nZoom > rcClientWnd.Height())
			scrollBarV.ShowWindow(SW_SHOW);
		else
			scrollBarV.ShowWindow(SW_HIDE);

		Invalidate();
	}
}

void CDialogPane::SetSize(int nType, int cx,int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	GetClientRect(rcClientWnd);

	//Position Scroll Bars
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	info.nMin = 0;

	scrollBarH.SetWindowPos(NULL, 0, cy - 20, cx - 20, 20, SWP_SHOWWINDOW);
	scrollBarV.SetWindowPos(NULL, cx - 20, 0, 20, cy, SWP_SHOWWINDOW);

	if(nScrollBarHMax*nZoom > rcClientWnd.Width()) {
		info.nMax = nScrollBarHMax*nZoom;
		info.nPage = (nScrollBarHMax*nZoom) / 10;
		info.nPos = nHorizontalScrollBar;
		info.nTrackPos = nHorizontalScrollBar;

		scrollBarH.SetScrollInfo(&info);

		scrollBarH.ShowWindow(SW_SHOW);
	}
	else {
		scrollBarH.ShowWindow(SW_HIDE);
	}

	if(nScrollBarVMax*nZoom > rcClientWnd.Height()) {
		info.nMax = nScrollBarVMax*nZoom;
		info.nPage = (nScrollBarVMax*nZoom) / 10;
		info.nPos = nVerticalScrollBar;
		info.nTrackPos = nVerticalScrollBar;

		scrollBarV.SetScrollInfo(&info);

		scrollBarV.ShowWindow(SW_SHOW);
	}
	else {
		scrollBarV.ShowWindow(SW_HIDE);
	}
}



void CDialogPane::OnLButtonDown(UINT nFlags, CPoint point)
{

	if (theApp.m_bScanning)
		return;
	CApplication* pApp = (CApplication*)AfxGetApp();
	
	CTrackHandler* pTrack = &pApp->m_pTracks[0];
	CAppDoc* pDoc = pTrack->m_pDoc[0];
	int nError = 0;
	bool bFoundDie = false;
	int i = 0;

	CPoint pointRel = point;

	if(PtInRect(&pointRel, &rcClientWnd)) {
		std::vector < CRect >::const_iterator roi = _deviceRoi.begin();
		CRect rcDie;
		
		// Varible using for changing Image by using arrow key
		isSelectingMap = false;
		nCurrentDieNo = 0;

		for(i = 0; i<nTotalNo; i++, ++roi) {
			rcDie = CRect(roi->left * nZoom - nHorizontalScrollBar,
				roi->top * nZoom - nVerticalScrollBar,
				roi->right * nZoom - nHorizontalScrollBar,
				roi->bottom * nZoom - nVerticalScrollBar);

			CheckRect(&rcClientWnd, &rcDie);

			if(RectInRect(rcClientWnd, rcDie)) {

				if(PtInRect(&pointRel, &rcDie)) {
					bFoundDie = true;

					// Can use arrow key to change Image when selecting in Substrate firstly.
					isSelectingMap = true;
					nCurrentDieNo = i;

					break;
				}
			}
		}
	}

	if(bFoundDie) {
		int nBlockNoX;
		int nBlockNoY;
		int nDieX;
		int nDieY;
		int nImageIndex[TOTAL_STATIONS] = {0,0};

		m_pApp->motionParameters.GetMappingParameters(nBlockNoX, nBlockNoY, nDieX, nDieY);

		int nRPos, nCpos, nPos;
		int nCameraGrabIdTHR;
		long nCameraGrabIdTop;
		long nCameraGrabIdEncap;
		nDieViewNo = i;

		m_pApp->motionParameters.getCameraGrabIdTHR(nDieViewNo, nCameraGrabIdTHR);
		m_pApp->motionParameters.GetDeviceDetailsTHR(nCameraGrabIdTHR, nRPos, nCpos, nDeviceNo, nFOV);

		m_pApp->m_pVerificationMode->UpdateDieFovPositions(nDeviceNo, nFOV);

		pApp->m_pTracks[1].m_nFOVIndex = nFOV;
		nImageIndex[STATION_THR] = nCameraGrabIdTHR;
		nPosNew = nCameraGrabIdTHR;

		//For Top Station
		int nRPosTopStation, nCposTopStation, nDeviceNoTopStation, nFOVTopStation;
		m_pApp->motionParameters.getCameraGrabIdTop(nDieViewNo, nCameraGrabIdTop);
		m_pApp->motionParameters.GetDeviceDetailsTOP(nCameraGrabIdTop, nRPosTopStation, nCposTopStation, nDeviceNoTopStation, nFOVTopStation);
		
		pApp->m_pTracks[STATION_TOP].m_nFOVIndex = nFOVTopStation;
		nImageIndex[STATION_TOP] = nCameraGrabIdTop;

		//For Encap Station
		/*int nRPosEncapStation, nCposEncapStation, nDeviceNoEncapStation, nFOVEncapStation;
		m_pApp->motionParameters.getCameraGrabIdTop(nDieViewNo, nCameraGrabIdEncap);
		m_pApp->motionParameters.GetDeviceDetailsTOP(nCameraGrabIdEncap, nRPosEncapStation, nCposEncapStation, nDeviceNoEncapStation, nFOVEncapStation);
		
		pApp->m_pTracks[STATION_TOP].m_nFOVIndex = nFOV;*/

		if (pApp->m_bLoadMultiImagesFromFile && !((pApp->m_bOnline && pApp->m_bShowDieVerification) || pApp->m_bInspSequenceOffline)) {
			for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++) {
				pApp->m_nSaveAllDieIndex[nTrack] = nImageIndex[nTrack];
				pApp->m_pTracks[nTrack].m_pDoc[0]->LoadMultipleImagesToDoc(pApp->m_strLoadMultiImagesArray, nImageIndex[nTrack], nDeviceNo);
			}
		}
		else if (!pApp->m_bInspSequenceOffline || pApp->m_bOnline) {

			for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++) {
				for (int nDoc = 0; nDoc < pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
					pApp->m_pTracks[nTrack].m_pDoc[nDoc]->DrawView(0);
					pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer.ClrImg();
				
					if (!pApp->m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab.empty()) {
						if (m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab[nImageIndex[nTrack]].IsAllocated()) {
							pApp->m_pTracks[nTrack].m_pDoc[nDoc]->CopyGrabbedBuffer(&pApp->m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab[nImageIndex[nTrack]]);
							DrwUpdate(&pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
						}
					}
				}
			}
		}

		BOOL m_bInspectionDebug = FALSE;
		for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++)
			for (int nDoc = 0; nDoc < pApp->m_nTotalViewDocs[nTrack]; nDoc++)
				m_bInspectionDebug |= pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspectionDebug;
			
		if (!m_bInspectionDebug) {
			for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++)
			{
				for (int nDoc = 0; nDoc < pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
					pApp->m_pTracks->m_pActiveDocTeach = pApp->m_pTracks[nTrack].m_pDoc[nDoc];
					pApp->m_pTracks[nTrack].m_pDoc[nDoc]->InspectAllDoc(nError);
				}
			}
		}
		if(leftMouseDownSelectedDie != i) {
			leftMouseDownSelectedDie = i;
			Invalidate();
		}
	}
	else {

		if(leftMouseDownSelectedDie != i) {
			leftMouseDownSelectedDie = -1;
			Invalidate();
		}
	}

	CDockablePane::OnLButtonDown(nFlags, point);
}

int CDialogPane::NavigateDieImages(int nKey)
{
	CApplication* pApp = (CApplication*) AfxGetApp();

	/*if(!pApp->m_bLoadMultiImagesFromFile)
	return -1;
	*/
	CRect rect;
	int nPos, nDevice, nSel;
	BOOL bSelChange = FALSE;
	CString strToolTipTextNew;
	nDevice = nPosNew / MAX_FOV;
	nPos = nPosNew % pApp->m_nNoOfFov;
	if(nKey == (int) VK_UP || nKey == (int) VK_DOWN) {
		BOOL bKeyUp = (nKey == (int) VK_UP);
		if(bKeyUp) {
			if(nDevice>0) {
				nDevice -= 1;
				bSelChange = TRUE;
			}
		}
		else {
			if(nDevice<pApp->m_nNoOfDies - 1) {
				nDevice += 1;
				bSelChange = TRUE;
			}
		}
	}
	else if(nKey == (int) VK_LEFT || nKey == (int) VK_RIGHT) {
		BOOL bKeyLeft = (nKey == (int) VK_LEFT);
		if(bKeyLeft) {
			if(nPos>0) {
				nPos -= 1;
				bSelChange = TRUE;
			}
		}
		else {
			if(nPos<pApp->m_nNoOfFov - 1) {
				nPos += 1;
				bSelChange = TRUE;
			}
		}
		if(bSelChange)
			nPosNew = ((nDevice*MAX_FOV) + (nPos%pApp->m_nNoOfFov));
	}

	if(bSelChange && (nPosOld > -1)) {
		int nOldDevice = nPosOld / MAX_FOV;
		nSel = ((nOldDevice*MAX_FOV) + (nPosOld%pApp->m_nNoOfFov));
		m_ctrlDieImages[nSel].m_bSelected = FALSE;
		m_ctrlDieImages[nSel].Invalidate();

		strToolTipTextNew.Format("Row %d, Column %d: %d", nDevice + 1, nPos + 1, pApp->m_nPartErrorCode[nDevice]);
	//	GetDlgItem(IDC_STATIC_DISPLAY_MSG)->SetWindowTextA(strToolTipTextNew);
		if (strToolTipText.CompareNoCase(strToolTipTextNew) != 0) {

			strToolTipText = strToolTipTextNew;
			m_ToolTip.UpdateTipText((LPCTSTR)strToolTipText, this, 1);
		}
		CStringArray strArray;
		pApp->m_pTracks[0].m_pDoc[0]->GetImageFilePaths(m_pApp->m_strLoadMultiImagesFolderPath + "\\", strArray);
		for(int nTrack = 0; nTrack<pApp->m_nTotalTracks; nTrack++)
			pApp->m_pTracks[nTrack].m_pDoc[0]->LoadMultipleImagesToDoc(strArray, nPos, nDevice);
		//for(int nTrack=0; nTrack<pApp->m_nTotalTracks; nTrack++)
		//pApp->m_pTracks[nTrack].m_pDoc[0]->Inspect(0);

		nSel = ((nDevice*MAX_FOV) + (nPosNew%pApp->m_nNoOfFov));
		m_ctrlDieImages[nSel].m_bSelected = !m_ctrlDieImages[nSel].m_bSelected;
		m_ctrlDieImages[nSel].Invalidate();
		nPosOld = nPosNew = nSel;
	}

	return 0;
}



void CDialogPane::OnPaint()
{

	EnterCriticalSection(&CriticalSectionDraw);

	if(bRedrawing)
		return;

	bRedrawing = true;

	LeaveCriticalSection(&CriticalSectionDraw);

	int i;
	int dieId;

	CPaintDC dc(this);
	CDC dcMemory;
	CRect ClientRect;
	GetClientRect(ClientRect);

	dcMemory.CreateCompatibleDC(&dc);

	CBitmap bitmap;
	CBitmap * pbitmap;

	bitmap.CreateCompatibleBitmap(&dc, ClientRect.Width(), ClientRect.Height());

	pbitmap = dcMemory.SelectObject(&bitmap);

	::FillRect(dcMemory.GetSafeHdc(), ClientRect, brushBackground);

	std::vector < CRect >::const_iterator roi = _deviceRoi.begin();
	std::vector < CRect >::const_iterator dieRoi = _dieRoi.begin();
	std::vector < int >::const_iterator code = _deviceCode.begin();
	CRect rcRect;
	CRect rcDieRect;

	int nTempRowId, nTempColumnId, nTempDieId, nTempFov;
	CString strTempKey;

	if(nZoom == 1) {

		int nMostTop = -1, nMostLeft = -1, nMostRight = -1, nMostBottom = -1;
		for(i = 0; i<nTotalNo; i++, ++code, ++roi) {
			rcRect = CRect(roi->left - nHorizontalScrollBar,
				roi->top - nVerticalScrollBar,
				roi->right - nHorizontalScrollBar,
				roi->bottom - nVerticalScrollBar);

			if(CheckRect(&rcClientWnd, &rcRect)) continue;
			
			int nErrorCode = *code;

			dcMemory.FillRect(rcRect, &getInfo(getId(nErrorCode))->brush);

			if (m_pApp->motionParameters.nDisplayDieNo(i, dieId)) {
				CString str;
				CFont pFont;

				pFont.CreateFont(14 * nZoom, 0, 0, 0, 60, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Arial");
				CPen pDiePen;
				pDiePen.CreatePen(PS_SOLID, 2, getInfo(getId(nErrorCode))->color);
				dcMemory.SelectObject(&pDiePen);
				dcMemory.SelectObject(&pFont);
				str.Format("%d", dieId + 1);
				dcMemory.SetBkColor(getInfo(getId(nErrorCode))->color);
				dcMemory.DrawTextA(str, -1, rcRect, DT_CENTER);
		//		dcMemory.SelectObject(pFont);

			}	

			if (rcRect.top < nMostTop || nMostTop == -1) {
				nMostTop = rcRect.top;
			}
			if (rcRect.left < nMostLeft || nMostLeft == -1) {
				nMostLeft = rcRect.left;
			}
			if (rcRect.right > nMostRight) {
				nMostRight = rcRect.right;
			}
			if (rcRect.bottom > nMostBottom) {
				nMostBottom = rcRect.bottom;
			}
		}

		//Bounding Block 1
		CRect rcRect = CRect(nMostLeft, nMostTop, nMostRight, nMostBottom / 2 - 5);
		if (!CheckRect(&rcClientWnd, &rcRect)) {
			CRgn prgnDieBounday;
			prgnDieBounday.CreateRoundRectRgn(rcRect.left - 1, rcRect.top - 1, rcRect.right + 1, rcRect.bottom + 2, 2, 2);
			dcMemory.FrameRgn(&prgnDieBounday, &m_brushBorder, 2, 2);
		}

		//Bounding Block 2
		rcRect = CRect(nMostLeft, nMostBottom / 2 + 6, nMostRight, nMostBottom);
		if (!CheckRect(&rcClientWnd, &rcRect)) {
			CRgn prgnDieBounday;
			prgnDieBounday.CreateRoundRectRgn(rcRect.left - 1, rcRect.top - 1, rcRect.right + 1, rcRect.bottom + 3, 2, 2);
			dcMemory.FrameRgn(&prgnDieBounday, &m_brushBorder, 2, 2);
		}
		

		if (theApp.m_bSubstrateMappingStyle) {
			for (int i = 0; i < _dieRoi.size(); i++, ++dieRoi) {
				rcDieRect = CRect(dieRoi->left - nHorizontalScrollBar,
					dieRoi->top - nVerticalScrollBar,
					dieRoi->right - nHorizontalScrollBar,
					dieRoi->bottom - nVerticalScrollBar);

				if (CheckRect(&rcClientWnd, &rcDieRect)) continue;
				int nDieID;
				theApp.motionParameters.GetDieMappingNo(i, nDieID);

				int nErrorCode = _dieErrorCode[nDieID];
				
				CFont * pOldFont;
				CString strText;
					pOldFont = dcMemory.SelectObject(&m_FontDieStatus);

				CBrush brush;
				if (nErrorCode == GOOD_UNIT) {
					dcMemory.SetBkColor(RGB(147, 255, 128));
					brush.CreateSolidBrush(RGB(147, 255, 128));
						strText.Format("Die %d Status: PASS (%s)", nDieID, getInfo(nErrorCode)->code.c_str());
					dcMemory.FillRect(rcDieRect, &brush);
				}
				else if (nErrorCode != NOT_INSPECTED && nErrorCode != GOOD_UNIT) {
					dcMemory.SetBkColor(RGB(255, 153, 153));
					brush.CreateSolidBrush(RGB(255, 153, 153));
						strText.Format("Die %d Status: FAIL (%s)", nDieID, getInfo(nErrorCode)->code.c_str());
					dcMemory.FillRect(rcDieRect, &brush);
				}
				else {
					dcMemory.FillRect(rcDieRect, &getInfo(nErrorCode)->brush);
					dcMemory.SetBkColor(getInfo(getId(nErrorCode))->color);
						strText.Format("Die %d Status: %s", nDieID, getInfo(nErrorCode)->code.c_str());
				}
					dcMemory.DrawTextA(strText, -1, rcDieRect, DT_LEFT);
				dcMemory.SelectObject(pOldFont);
			}
		}

	}
	else {

		for (i = 0; i < nTotalNo; i++, ++code, ++roi) {
			rcRect = CRect(roi->left   * nZoom - nHorizontalScrollBar,
				roi->top    * nZoom - nVerticalScrollBar,
				roi->right  * nZoom - nHorizontalScrollBar,
				roi->bottom * nZoom - nVerticalScrollBar);

			if (CheckRect(&rcClientWnd, &rcRect)) continue;

			
			int nErrorCode = *code;

			dcMemory.FillRect(rcRect, &getInfo(getId(nErrorCode))->brush);
			if (m_pApp->motionParameters.nDisplayDieNo(i, dieId)) {
				CString str;
				CFont pDieFont;
				CPen pDiePen;

				pDiePen.CreatePen(PS_SOLID, 2, getInfo(getId(nErrorCode))->color);
				pDieFont.CreateFont(16 * nZoom, 0, 0, 0, 60, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Arial");
				dcMemory.SelectObject(pDieFont);
				dcMemory.SelectObject(&pDiePen);

				//	pen.CreatePen(PS_SOLID, 2, getInfo(NOT_INSPECTED)->color);
				str.Format("%d", dieId + 1);
				dcMemory.SetBkColor(getInfo(getId(nErrorCode))->color);
				dcMemory.DrawTextA(str, -1, rcRect, DT_CENTER);
				//	dcMemory.SelectObject(pFont);
			}
		}

	

		if (theApp.m_bSubstrateMappingStyle) {
			for (int i = 0; i < _dieRoi.size(); i++, ++dieRoi) {
				rcDieRect = CRect(dieRoi->left* nZoom - nHorizontalScrollBar,
					dieRoi->top * nZoom - nVerticalScrollBar,
					dieRoi->right * nZoom - nHorizontalScrollBar,
					dieRoi->bottom* nZoom - nVerticalScrollBar);

				if (CheckRect(&rcClientWnd, &rcDieRect)) continue;

				int nDieID;
				theApp.motionParameters.GetDieMappingNo(i, nDieID);

				int nErrorCode = _dieErrorCode[nDieID];
				
				CFont * pOldFont;
				CString strText;
					pOldFont = dcMemory.SelectObject(&m_FontStatusText);
					//	strText.Format("%s", getInfo(nErrorCode)->code.c_str());

				CBrush brush;
				if (nErrorCode == GOOD_UNIT) {
					dcMemory.SetBkColor(RGB(147, 255, 128));
					brush.CreateSolidBrush(RGB(147, 255, 128));
						strText.Format("Die %d Status: PASS (%s)", nDieID, getInfo(nErrorCode)->code.c_str());
					dcMemory.FillRect(rcDieRect, &brush);
				}
				else if (nErrorCode != NOT_INSPECTED && nErrorCode != GOOD_UNIT) {
					dcMemory.SetBkColor(RGB(255, 153, 153));
					brush.CreateSolidBrush(RGB(255, 153, 153));
						strText.Format("Die %d Status: FAIL (%s)", nDieID, getInfo(nErrorCode)->code.c_str());
					dcMemory.FillRect(rcDieRect, &brush);
				}
				else {
					dcMemory.FillRect(rcDieRect, &getInfo(nErrorCode)->brush);
					dcMemory.SetBkColor(getInfo(getId(nErrorCode))->color);
						strText.Format("Die %d Status: %s", nDieID, getInfo(nErrorCode)->code.c_str());
				}
					dcMemory.DrawTextA(strText, -1, rcDieRect, DT_LEFT);
				dcMemory.SelectObject(pOldFont);
			}
		}
	}

	/*if (nImageSelectedDie > -1 && nImageSelectedDie < nTotalNo) {

		rcRect = CRect(_deviceRoi[nImageSelectedDie].left   * nZoom - nHorizontalScrollBar - 1,
			_deviceRoi[nImageSelectedDie].top    * nZoom - nVerticalScrollBar - 1,
			_deviceRoi[nImageSelectedDie].right  * nZoom - nHorizontalScrollBar + 1,
			_deviceRoi[nImageSelectedDie].bottom * nZoom - nVerticalScrollBar + 1);

		dcMemory.FrameRect(rcRect, &brushSelectedImage);
	}*/

	if(leftMouseDownSelectedDie > -1 && leftMouseDownSelectedDie < nTotalNo) {
		rcRect = CRect(_deviceRoi[leftMouseDownSelectedDie].left   * nZoom - nHorizontalScrollBar - 1,
			_deviceRoi[leftMouseDownSelectedDie].top    * nZoom - nVerticalScrollBar - 1,
			_deviceRoi[leftMouseDownSelectedDie].right  * nZoom - nHorizontalScrollBar + 1,
			_deviceRoi[leftMouseDownSelectedDie].bottom * nZoom - nVerticalScrollBar + 1);

		CPen pen;

		if(_deviceCode[leftMouseDownSelectedDie] == GOOD_UNIT)
			pen.CreatePen(PS_SOLID, 2, getInfo(NOT_INSPECTED)->color);
		else
			pen.CreatePen(PS_SOLID, 2, getInfo(GOOD_UNIT)->color);

		CPen * oldPen = dcMemory.SelectObject(&pen);

		dcMemory.MoveTo(rcRect.left, rcRect.top);
		dcMemory.LineTo(rcRect.right, rcRect.bottom);

		dcMemory.MoveTo(rcRect.right, rcRect.top);
		dcMemory.LineTo(rcRect.left, rcRect.bottom);

		pen.DeleteObject();
		dcMemory.SelectObject(oldPen);
	}

	//--. Draw statistics
	dcMemory.SetBkColor(RGB(220, 220, 220));

	CFont * pOldFont;
	CString strText;

	pOldFont = dcMemory.SelectObject(&m_FontDieStatus);

	if (!theApp.strInspectDeviceBarcodeID.IsEmpty())
		strText.Format("Bad units: %d, Good units: %d, Remained units: %d, Barcode ID: %s", _badDeviceCount, _goodDeviceCount, _deviceRoi.size() - _goodDeviceCount - _badDeviceCount, theApp.strInspectDeviceBarcodeID);
	else
		strText.Format("Bad units: %d, Good units: %d, Remained units: %d", _badDeviceCount, _goodDeviceCount, _deviceRoi.size() - _goodDeviceCount - _badDeviceCount);
			
	rcRect = CRect(_deviceRoi[0].left   * nZoom - nHorizontalScrollBar,
		(_deviceRoi[nTotalNo - 1].bottom)* nZoom - nVerticalScrollBar + 10,
		_deviceRoi[0].left   * nZoom - nHorizontalScrollBar + 800,
		(_deviceRoi[nTotalNo - 1].bottom) * nZoom - nVerticalScrollBar + 25);

	if (theApp.m_bSubstrateMappingStyle) 
		rcRect = CRect(_deviceRoi[0].left   * nZoom - nHorizontalScrollBar,
			(_deviceRoi[nTotalNo - 1].bottom + 32)* nZoom - nVerticalScrollBar,
			_deviceRoi[0].left   * nZoom - nHorizontalScrollBar + 800,
			(_deviceRoi[nTotalNo - 1].bottom + 36) * nZoom - nVerticalScrollBar + 25);
	
	dcMemory.DrawTextA(strText, -1, rcRect, DT_LEFT);

	dcMemory.SelectObject(pOldFont);

	CFont *pStatusFont;
	pStatusFont = dcMemory.SelectObject(&m_FontStatusText);

	
	if (!theApp.m_bSubstrateMappingStyle) {
		if (_badDeviceCount == 0 && _goodDeviceCount == _deviceRoi.size()) {
			strText.Format("PANEL Status: PASS");
			dcMemory.SetBkColor(RGB(147, 255, 128));

		}
		else if (_badDeviceCount > 0 || _badDeviceCountTop > 0) {
			strText.Format("PANEL Status: FAIL (%s)", strDefectName.c_str());
			dcMemory.SetBkColor(RGB(255, 153, 153));
		}
		else {
			strText.Format("PANEL Status:");
			//dcMemory.SetBkColor(RGB(204, 221, 255));
			dcMemory.SetBkColor(RGB(220, 220, 220));
		}

		rcRect = CRect(_deviceRoi[0].left   * nZoom - nHorizontalScrollBar,
			_deviceRoi[nTotalNo - 1].bottom * nZoom - nVerticalScrollBar + 45,
			_deviceRoi[0].left   * nZoom - nHorizontalScrollBar + 840,
			_deviceRoi[nTotalNo - 1].bottom * nZoom - nVerticalScrollBar + 105);

		dcMemory.DrawTextA(strText, -1, rcRect, DT_LEFT);
	}

	dcMemory.SelectObject(pStatusFont);

	dc.BitBlt(0, 0, ClientRect.Width(), ClientRect.Height(), &dcMemory, 0, 0, SRCCOPY);

	dcMemory.SelectObject(pbitmap);

	dcMemory.DeleteDC();
	bitmap.DeleteObject();

	bRedrawing = false;
	}

LRESULT CDialogPane::OnNcHitTest(CPoint point)
{
	bool bFoundDie = false;
	int i = 0;

	CPoint pointRel = point;

	ScreenToClient(&pointRel);

	if (PtInRect(&pointRel, &rcClientWnd)) {
		std::vector < CRect >::const_iterator roi = _deviceRoi.begin();
		CRect rcDie;

		for (i = 0; i < nTotalNo; i++, ++roi) {
			rcDie = CRect(roi->left * nZoom - nHorizontalScrollBar,
				roi->top * nZoom - nVerticalScrollBar,
				roi->right * nZoom - nHorizontalScrollBar,
				roi->bottom * nZoom - nVerticalScrollBar);

			CheckRect(&rcClientWnd, &rcDie);

			if (RectInRect(rcClientWnd, rcDie)) {

				if (PtInRect(&pointRel, &rcDie)) {
					bFoundDie = true;
					break;
				}
			}
		}
	}

	if (bFoundDie) {
		const int nDiePerLine = nDeviceNoPerSubstrateX * nDieNoPerDeviceTOPX;

		CString strToolTipTextNew;
		/*int _inverseY = 0;
		int _inverseX = 0;
		int rowId = _inverseY ? nTotalNo / nDiePerLine - i / nDiePerLine : i / nDiePerLine + 1;
		int columnId = _inverseX ? nDiePerLine - i % nDiePerLine : i % nDiePerLine + 1;*/
		int dieId, rowId, columnId, nFov;

		m_pApp->motionParameters.getImageMappingDetails(i, rowId, columnId, dieId, nFov);

		int nErrorCode = _deviceCode[i];

		strToolTipTextNew.Format("Die %d, Row %d, Column %d, FOV %d: %s", dieId + 1, rowId + 1, columnId + 1, nFov + 1, getInfo(getId(nErrorCode))->code.c_str());

		if (strToolTipText.CompareNoCase(strToolTipTextNew) != 0) {

			strToolTipText = strToolTipTextNew;
			m_ToolTip.UpdateTipText((LPCTSTR)strToolTipText, this, 1);
		}

		if (!bActivatedToolTip) {
			bActivatedToolTip = TRUE;
			m_ToolTip.Activate(bActivatedToolTip);
		}

		if (nMouseSelectedDie != i) {
			nMouseSelectedDie = i;
			Invalidate();
		}
	}
	else {
		if (bActivatedToolTip) {
			bActivatedToolTip = FALSE;
			m_ToolTip.Activate(bActivatedToolTip);
		}

		if (nMouseSelectedDie != -1) {
			nMouseSelectedDie = -1;
			Invalidate();
		}
	}

	return CDockablePane::OnNcHitTest(point);
}


BOOL CDialogPane::PreTranslateMessage(MSG* pMsg)
{
	// Whether inspect when press arrow key or not
	BOOL bFoundDie = false;

	if(pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_DOWN) {
			scrollBarV.SetFocus();

			if (isSelectingMap) {
				int nBlockNoX;
				int nBlockNoY;
				int nDieX;
				int nDieY;
				int nImageIndex[TOTAL_STATIONS] = { 0,0 };

				m_pApp->motionParameters.GetMappingParameters(nBlockNoX, nBlockNoY, nDieX, nDieY);

				if (nCurrentDieNo + nBlockNoX < nTotalNo) {
					nCurrentDieNo = nCurrentDieNo + nBlockNoX;
					bFoundDie = true;
				}
			}
		}
		else if (pMsg->wParam == VK_RIGHT) {
			scrollBarH.SetFocus();

			if (isSelectingMap) {
				if (nCurrentDieNo + 1 < nTotalNo) {
					nCurrentDieNo = nCurrentDieNo + 1;
					bFoundDie = true;
				}
			}
		}
		else if (pMsg->wParam == VK_LEFT) {
			scrollBarH.SetFocus();

			if (isSelectingMap) {
				if (nCurrentDieNo - 1 >= 0) {
					nCurrentDieNo = nCurrentDieNo - 1;
					bFoundDie = true;
				}
			}
		}
		else if (pMsg->wParam == VK_UP) {
			scrollBarV.SetFocus();

			if (isSelectingMap) {
				int nBlockNoX;
				int nBlockNoY;
				int nDieX;
				int nDieY;
				int nImageIndex[TOTAL_STATIONS] = { 0,0 };

				m_pApp->motionParameters.GetMappingParameters(nBlockNoX, nBlockNoY, nDieX, nDieY);

				if (nCurrentDieNo - nBlockNoX >= 0) {
					nCurrentDieNo = nCurrentDieNo - nBlockNoX;
					bFoundDie = true;
				}
			}
		}
		else if(pMsg->wParam == VK_ADD || pMsg->wParam == VK_OEM_PLUS) //+
			Zoom(true);
		else if(pMsg->wParam == VK_SUBTRACT || pMsg->wParam == VK_OEM_MINUS) //-
			Zoom(false);
	}

	// Inspecting when pressing arrow key
	if (bFoundDie) {
		if (theApp.m_bScanning)
			return 0;
		CApplication* pApp = (CApplication*)AfxGetApp();

		CTrackHandler* pTrack = &pApp->m_pTracks[0];
		CAppDoc* pDoc = pTrack->m_pDoc[0];
		int nError = 0;

		int nBlockNoX;
		int nBlockNoY;
		int nDieX;
		int nDieY;
		int nImageIndex[TOTAL_STATIONS] = { 0,0 };

		m_pApp->motionParameters.GetMappingParameters(nBlockNoX, nBlockNoY, nDieX, nDieY);

		int nRPos, nCpos, nPos;
		int nCameraGrabIdTHR;
		long nCameraGrabIdTop;
		long nCameraGrabIdEncap;

		nDieViewNo = nCurrentDieNo;

		m_pApp->motionParameters.getCameraGrabIdTHR(nDieViewNo, nCameraGrabIdTHR);
		m_pApp->motionParameters.GetDeviceDetailsTHR(nCameraGrabIdTHR, nRPos, nCpos, nDeviceNo, nFOV);

		m_pApp->m_pVerificationMode->UpdateDieFovPositions(nDeviceNo, nFOV);

		pApp->m_pTracks[1].m_nFOVIndex = nFOV;
		nImageIndex[STATION_THR] = nCameraGrabIdTHR;
		nPosNew = nCameraGrabIdTHR;

		//For Top Station
		int nRPosTopStation, nCposTopStation, nDeviceNoTopStation, nFOVTopStation;
		m_pApp->motionParameters.getCameraGrabIdTop(nDieViewNo, nCameraGrabIdTop);
		m_pApp->motionParameters.GetDeviceDetailsTOP(nCameraGrabIdTop, nRPosTopStation, nCposTopStation, nDeviceNoTopStation, nFOVTopStation);

		pApp->m_pTracks[STATION_TOP].m_nFOVIndex = nFOVTopStation;
		nImageIndex[STATION_TOP] = nCameraGrabIdTop;

		//For Encap Station
		/*int nRPosEncapStation, nCposEncapStation, nDeviceNoEncapStation, nFOVEncapStation;
		m_pApp->motionParameters.getCameraGrabIdTop(nDieViewNo, nCameraGrabIdEncap);
		m_pApp->motionParameters.GetDeviceDetailsTOP(nCameraGrabIdEncap, nRPosEncapStation, nCposEncapStation, nDeviceNoEncapStation, nFOVEncapStation);

		pApp->m_pTracks[STATION_TOP].m_nFOVIndex = nFOV;*/

		if (pApp->m_bLoadMultiImagesFromFile && !((pApp->m_bOnline && pApp->m_bShowDieVerification) || pApp->m_bInspSequenceOffline)) {
			for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++) {
				pApp->m_nSaveAllDieIndex[nTrack] = nImageIndex[nTrack];
				pApp->m_pTracks[nTrack].m_pDoc[0]->LoadMultipleImagesToDoc(pApp->m_strLoadMultiImagesArray, nImageIndex[nTrack], nDeviceNo);
			}
		}
		else if (!pApp->m_bInspSequenceOffline || pApp->m_bOnline) {

			for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++) {
				for (int nDoc = 0; nDoc < pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
					pApp->m_pTracks[nTrack].m_pDoc[nDoc]->DrawView(0);
					pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_Buffer.ClrImg();

					if (!pApp->m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab.empty()) {
						if (m_pApp->m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab[nImageIndex[nTrack]].IsAllocated()) {
							pApp->m_pTracks[nTrack].m_pDoc[nDoc]->CopyGrabbedBuffer(&pApp->m_pTracks[nTrack].m_pDoc[nDoc]->imageGrab[nImageIndex[nTrack]]);
							DrwUpdate(&pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_ImageViewInfo);
						}
					}
				}
			}
		}

		BOOL m_bInspectionDebug = FALSE;
		for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++)
			for (int nDoc = 0; nDoc < pApp->m_nTotalViewDocs[nTrack]; nDoc++)
				m_bInspectionDebug |= pApp->m_pTracks[nTrack].m_pDoc[nDoc]->m_bInspectionDebug;

		if (!m_bInspectionDebug) {
			for (int nTrack = 0; nTrack < pApp->m_nTotalTracks; nTrack++)
			{
				for (int nDoc = 0; nDoc < pApp->m_nTotalViewDocs[nTrack]; nDoc++) {
					pApp->m_pTracks->m_pActiveDocTeach = pApp->m_pTracks[nTrack].m_pDoc[nDoc];
					pApp->m_pTracks[nTrack].m_pDoc[nDoc]->InspectAllDoc(nError);
				}
			}
		}

		if (leftMouseDownSelectedDie != nCurrentDieNo) {
			leftMouseDownSelectedDie = nCurrentDieNo;
			Invalidate();
		}
	}

	if(pMsg->message == WM_MOUSEMOVE)
		m_ToolTip.RelayEvent(pMsg);

	return CDockablePane::PreTranslateMessage(pMsg);
}

INT_PTR CDialogPane::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
{
	pTI->hwnd = m_hWnd;
	pTI->uFlags = 0; // we need to differ tools by ID not window handle
	pTI->lpszText = LPSTR_TEXTCALLBACK; // tell tooltips to send TTN_NEEDTEXT	
	int iIndx;
	CRect rect;
	for (iIndx = 0; iIndx < nTotalNo; iIndx++)
	{
		m_ctrlDieImages[iIndx].GetWindowRect(&rect);
		ScreenToClient(&rect);
		if (rect.PtInRect(point))
		{
			// point is in this rectangle. 
			pTI->rect = rect;
			// TTN_NEEDTEXT Message will use this ID to fill NMHDR structure
			pTI->uId = iIndx;
			break;
		}
	}

	return iIndx; // return index of rectangle
}

void CDialogPane::OnToolTipNotify(NMHDR *pNMHDR, LRESULT *pResult)
{
	WCHAR m_szTipBufW[MAX_PATH];
	char m_szTipBufA[MAX_PATH];
	*pResult = 1;
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;

	CString strToolTipTextNew;

	UINT uiID = pNMHDR->idFrom;

	int iID = GetDlgCtrlID();

	CString csIn;
	GetWindowText(csIn);
	
	int nPos, nDevice;
	nPos = uiID % m_pApp->m_nNoOfFov;
	nDevice = uiID / m_pApp->m_nNoOfFov;
	//if (m_pApp->m_bShowDieVerification)
	strToolTipTextNew.Format("Row %d, Column %d:%s:ErrorCode=%d", nDevice + 1, nPos + 1,m_pApp->m_bDieInspection[nDevice][nPos]?"Inspected":"Not Inspected", m_pApp->m_nFovPartErrorCode[nDevice][nPos]);
	//else
	//	strToolTipTextNew.Format("Row %d, Column %d", nDevice + 1, nPos + 1);


#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		lstrcpyn(m_szTipBufA, strToolTipTextNew, _countof(m_szTipBufA));
		pTTTA->lpszText = m_szTipBufA;
	}
	else
	{
		_mbstowcsz(m_szTipBufW, strToolTipTextNew, _countof(m_szTipBufW));
		pTTTW->lpszText = m_szTipBufW;
	}
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		_wcstombsz(m_szTipBufA, csToolText, _countof(m_szTipBufA));
		pTTTA->lpszText = m_szTipBufA;
	}
	else
	{
		lstrcpyn(m_szTipBufW, csToolText, _countof(m_szTipBufW));
		pTTTW->lpszText = m_szTipBufW;
	}
#endif
	//	return TRUE;
}


int CDialogPane::setDieStatus(std::vector <int>  & result, std::vector <int> & dieColumn, std::vector <int> & dieRow, std::vector <int> & dieNo, std::vector <int>& dieFov, int nStationId, int nDocId, bool bOverWrite)
{
	if(result.size() != dieColumn.size() || result.size() != dieRow.size() || result.size() !=  dieNo.size())
		return -1;

	for(size_t i = 0; i < result.size(); i++) {
		bool removeItem = false;
		if(dieColumn[i] < 0 || dieColumn[i] >= m_pApp->motionParameters.getScanColumnNoTHR()) {
			OutputDebugLogTo(0, TRUE, "----- ERROR: Incorrect die column. Die %dx%d, DieNo %d,Result %d",
				dieRow[i], dieColumn[i], dieNo[i], result[i]);
			removeItem = true;
		}
		else if(dieRow[i] < 0 || dieRow[i] >= m_pApp->motionParameters.getScanRowNoTHR()) {
			OutputDebugLogTo(0, TRUE, "----- ERROR: Incorrect die row. Die %dx%d, DieNo %d, Result %d",
				 dieRow[i], dieColumn[i], dieNo[i], result[i]);
			removeItem = true;
		}

		if(removeItem) {
			result.erase(result.begin() + i);
			dieColumn.erase(dieColumn.begin() + i);
			dieRow.erase(dieRow.begin() + i);
			dieNo.erase(dieNo.begin() + i);
			i--;
		}

		bool redraw = false;
		for(size_t i = 0; i < result.size(); i++) {
			int nDieId = -1;
			long nCameraId = -1;
			int nRealId;
			
			if (nStationId == STATION_THR) {

				m_pApp->motionParameters.GetCameraGrabIDTHR(0, dieRow[i], dieColumn[i], dieNo[i], nCameraId);
				m_pApp->motionParameters.getMappingDieIdTHR(nCameraId, nDieId);

				_setTopThrDeviceStatus(nCameraId, result[i], nStationId, nDocId);

				if (nDieId > -1) {
					redraw = _setDeviceStatus(nDieId, result[i], true, bOverWrite);
				}
			}
			else if (nStationId == STATION_TOP) {
				std::vector <int> nDieIds;
				m_pApp->motionParameters.GetCameraGrabIDTOP(0, dieRow[i], dieColumn[i], dieNo[i], nCameraId);
		
				m_pApp->motionParameters.getMappingDieIdTOP(nCameraId, nDieId);

				_setTopThrDeviceStatus(nCameraId, result[i], nStationId, nDocId);

				if (nDieId > -1) {
					redraw = _setDeviceStatus(nDieId, result[i], false);
					m_pApp->motionParameters.getCameraGrabIdTHR(nDieId, nRealId);
					//m_pApp->motionParameters.setInspectionEnableTHR(nRealId, result[i] == GOOD_UNIT ? true : false);
				}
			}
			if (redraw)
				Invalidate();
		}
	}
}

bool CDialogPane::SetDeviceStatus(int id, int code, bool bCount)
{
	if(id >= _deviceCode.size())
		return false;

	bool bReturnValue = false;

	if(_deviceCode[id] != code) {
		if (_deviceCode[id] == GOOD_UNIT && code == MISSING_DIE)
			_goodDeviceCount--;
	//	else if (_deviceCode[id] != GOOD_UNIT && code == MISSING_DIE)
	//		_badDeviceCount--;


		_deviceCode[id] = code;
	//	_deviceCode[id] = getId(code);
		bReturnValue = true;
	}

	/*if (bCount) {
		if (code == GOOD_UNIT)
			++_goodDeviceCount;
		else
			++_badDeviceCount;
		_deviceCountDone[id] = true;
	}*/

	return bReturnValue;
}

IMPLEMENT_DYNAMIC(CDialogPane,CDockablePane)
BEGIN_MESSAGE_MAP(CDialogPane,CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCHITTEST()
	ON_WM_KEYDOWN() // Enable press key down event
END_MESSAGE_MAP()

CDialogPane::CDialogPane()
{

	m_pApp = (CApplication*)AfxGetApp();
	//	motionParameters = m_pApp->motionParameters;
	bCloseLButtonThread = FALSE;
	/*nMaxNoDisplay = (m_pApp->m_nNoOfFov*m_pApp->m_nNoOfDies);
	nImageSets = m_pApp->m_nNoOfDies;*/
	nPosNew = -1;
	nPosOld = -1;
	m_ctrlDieImages = NULL;
	m_prgnDieMapPos = NULL;
	bFovSelected = NULL;
	m_pBrushTemp[0].CreateSolidBrush(RGB(255, 0, 0));
	m_pBrushTemp[1].CreateSolidBrush(RGB(0, 255, 0));
	m_pBrushTemp[2].CreateSolidBrush(RGB(128, 128, 128));
	for (int i = 0; i < 5; i++) {
		m_bCheckPass[i] = FALSE;
		m_nKeyInErrorCode[i] = 98765;
	}
	m_brushBorder.CreateSolidBrush(RGB(0, 0, 0));
	m_brushBorderSelected.CreateSolidBrush(RGB(255, 255, 0));


	//Girish
	pMotion = NULL;
	nDeviceNoPerSubstrateX = 0;
	nDeviceNoPerSubstrateY = 0;
	nDieNoPerDeviceX = 0;
	nDieNoPerDeviceY = 0;
	nDieNoPerDeviceTOPX = 0;
	nDieNoPerDeviceTOPY = 0;
	nTotalNo = 0;

	//	pnDieState = NULL;
	
	brushCell[0].CreateSolidBrush(RGB(64, 64, 64)); // Available to use
	brushCell[1].CreateSolidBrush(RGB(200, 200, 200)); // Not available
	brushCell[2].CreateSolidBrush(RGB(0, 240, 0)); // Selected
	brushBackground.CreateSolidBrush(RGB(238, 238, 255));
	brushSelectedCell.CreateSolidBrush(RGB(0, 255, 0));
	brushSelectedImage.CreateSolidBrush(RGB(255, 255, 0));

	//	prcDie = NULL;
	nZoom = 1;
	nPrevZoom = 0;
	bRedrawing = FALSE;
	nAllowedDie = 0;
	nNotAllowedDie = 0;
	nSelectedDie = 0;

	InitializeCriticalSection(&CriticalSectionData);
	InitializeCriticalSection(&CriticalSectionDraw);

	nWindowOffset = 1;
	//nScrollBarOffset = 75;
	nScrollBarHMax = 0;
	nScrollBarVMax = 0;
	bMouseLButtonPressed = FALSE;
	strToolTipText.Format("23D station mapping");
	bActivatedToolTip = TRUE;

	m_FontText.CreateFont(15, 0, 0, 0, 60, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Arial");

	m_FontDieStatus.CreateFont(17, 0, 0, 0, 60, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Arial");

	m_FontStatusText.CreateFont(30, 0, 0, 0, 60, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "Arial");

	nMouseSelectedDie = -1;
	leftMouseDownSelectedDie = -1;

	//	rcDieRelative = CRect();
	//	rcMappingRelative = CRect();

	scrollBarHRect = CRect();
	scrollBarVRect = CRect();

	/*rg23DMappingRgn.CreateRectRgnIndirect(rcMappingRelative);
	dieMappingRgn.CreateRectRgnIndirect(rcMappingRelative);
	rg23DMappingAndScrollBarRegion.CreateRectRgnIndirect(rcMappingRelative);
	rg23DMappingScrollBarAndDieMappingRegion.CreateRectRgnIndirect(rcMappingRelative);*/

	//	nGrabWidth = 1;

	//	m_bContinuousMove = FALSE;

	isSelectingDie = FALSE;
	isSelectingDieMouseMoved = FALSE;
	selectingRect = CRect();

	addGroup("Not inspected", NOT_INSPECTED);
	addGroup("Unknown defect", UNKNOWN_DEFECT);
	addGroup("Good unit", GOOD_UNIT);
	addGroup("Process Error", ERROR_PROCESS);

	addDefect("Not inspected", "Not inspected", NOT_INSPECTED, NOT_INSPECTED, RGB(64, 64, 64), true);
	addDefect("Unknown defect", "Unknown defect", UNKNOWN_DEFECT, UNKNOWN_DEFECT, RGB(255, 0, 0), false);
	addDefect("Good unit", "Good unit", GOOD_UNIT, GOOD_UNIT, RGB(20, 230, 20), true);
	addDefect("Process Error", "Process Error", ERROR_PROCESS, ERROR_PROCESS, RGB(0, 0, 255), true);

	nPVIGroupId = 8;

	// Varible for change Image using arrow key
	isSelectingMap = FALSE;
	nCurrentDieNo = 0;
}

CDialogPane::~CDialogPane()
{

		m_fontList.DeleteObject();
	bCloseLButtonThread = TRUE;
	//SetEvent(m_hPicCtrlImgHistory);
	//	CloseHandle(m_hPicCtrlImgHistory);///
	if (m_ctrlDieImages != NULL) {
		delete[] m_ctrlDieImages;
	}

	if (bFovSelected != NULL)
		delete[] bFovSelected;

	if (m_prgnDieMapPos != NULL) {
		for (int i = 0; i<nMaxNoDisplay; i++)
			m_prgnDieMapPos[i].DeleteObject();
		delete[] m_prgnDieMapPos;
	}
	m_prgnDieMapPos = NULL;

	m_brushUncheck.DeleteObject();
	m_brushBorder.DeleteObject();

	m_pBrushTemp[0].DeleteObject();
	m_pBrushTemp[1].DeleteObject();
	m_pBrushTemp[2].DeleteObject();
}

int CDialogPane::OnCreate(LPCREATESTRUCT lp)
{
	
	if(CDockablePane::OnCreate(lp)==-1)
		return -1;
	/*if(!m_wndDlg.Create(CImageSelectionDlg::IDD,this))
		return -1;*/
	bIsClosed = FALSE;
	bHideEnable = FALSE;
	//m_wndDlg.Initialize();
	Initialize();
//	m_wndDlg.ShowWindow(SW_SHOWDEFAULT);
	return  0;
}
void CDialogPane::OnSize(UINT nType,int cx,int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	GetClientRect(rcClientWnd);

	//Position Scroll Bars
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_ALL;
	info.nMin = 0;

	scrollBarH.SetWindowPos(NULL, 0, cy - 20, cx - 20, 20, SWP_SHOWWINDOW);
	scrollBarV.SetWindowPos(NULL, cx - 20, 0, 20, cy, SWP_SHOWWINDOW);

	if (nScrollBarHMax*nZoom > rcClientWnd.Width()) {
		info.nMax = nScrollBarHMax*nZoom;
		info.nPage = (nScrollBarHMax*nZoom) / 10;
		info.nPos = nHorizontalScrollBar;
		info.nTrackPos = nHorizontalScrollBar;

		scrollBarH.SetScrollInfo(&info);

		scrollBarH.ShowWindow(SW_SHOW);
	}
	else {
		scrollBarH.ShowWindow(SW_HIDE);
	}

	if (nScrollBarVMax*nZoom > rcClientWnd.Height()) {
		info.nMax = nScrollBarVMax*nZoom;
		info.nPage = (nScrollBarVMax*nZoom) / 10;
		info.nPos = nVerticalScrollBar;
		info.nTrackPos = nVerticalScrollBar;

		scrollBarV.SetScrollInfo(&info);

		scrollBarV.ShowWindow(SW_SHOW);
	}
	else {
		scrollBarV.ShowWindow(SW_HIDE);
	}
}

void CDialogPane::OnPressCloseButton()
{
	CDockablePane::OnPressCloseButton();
	bIsClosed = TRUE;
}

void CDialogPane::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	return;
}


void CDialogPane::setup(const std::string & machineId, bool bCouninueLot)
{
	if(!bCouninueLot)
		productionLotDetails.reset();
	productionLotDetails.machineId = machineId;
	productionLotDetails.date = getDate3();
//	productionLotDetails.woId = woId;
//	CString strDate = getTime().c_str();
	productionLotDetails.lotId = getDate2();					//Save all details with this time folder. 
	m_pApp->m_strLotInfo = productionLotDetails.lotId.c_str();
//	productionLotDetails.partId = partId;
	productionLotDetails.operatorId = m_pApp->m_UserAccInfo.m_strCurUserName;
}

std::vector <int > CDialogPane::getFailUnitId() const
{
	std::vector <int > failUnitId;

	//int columnCount = nDeviceNoPerSubstrateX * nDieNoPerDeviceX;
	//int rowCount = nTotalNo / columnCount;

	//for (int column = 0; column < columnCount; ++column) {
	//	int realColumn = _inverseX ? columnCount - column - 1 : column;

	//	for (int row = 0; row < rowCount; ++row) {
	//		bool inverseY = _inverseY;

	//		if (column % 2 == 1)
	//			inverseY = !inverseY;

	//		int realRow = inverseY ? rowCount - row - 1 : row;

	//		int id = column * rowCount + row + 1;
	//		int realId = realRow * columnCount + realColumn;

	//		if (_deviceCode[realId] != GOOD_UNIT && _deviceCode[realId] != NOT_INSPECTED)
	//			failUnitId.push_back(id);
	//	}
	//}
	return failUnitId;
}

void CDialogPane::SetBarcodeID(CString strBarcode)
{
	productionLotDetails.panel.push_back(PartInfo(strBarcode.GetString(), _deviceCode.size(), _dieCode.size(), getTime(), getDate(), theApp.m_strConfigFile.GetString()));

//	Invalidate();
}

int CDialogPane::SavePartSummary(bool bLotEnd)
{
	if (!lotInfo().panel.empty()) {
		const PartInfo & panel = lotInfo().panel.back();
		std::vector <int > passedDie;
		CString path;
		path.Format("%s\\Summary\\%s\\%s\\%s\\", theApp.m_LotSummaryRootDir, theApp.m_strConfigFile, lotInfo().lotId.c_str(), panel.id.c_str());
		CreateDirectoryTree(path);

		std::ofstream file(path + "PANEL Summary" + ".csv");

		if (file.is_open()) {
			file << "Machine ID:\t" << productionLotDetails.machineId << std::endl;
			file << "PANEL ID:\t" << panel.id << std::endl;
			file << "Start Time:\t" << panel.date << " " << panel.startTime << std::endl;
			file << "End Time:\t" << panel.endTime << std::endl;
			file << "Operator:\t" << productionLotDetails.operatorId << std::endl;

			file << std::endl << "<SUMMARY_REV>" << std::endl;

			file << "Dies Inspected:\t" << panel.dieCode.size() << std::endl;
			file << "Dies Pass:\t" << panel.goodDieCount << std::endl;
			file << "Dies Fail:\t" << panel.badDieCount << std::endl;

			if (panel.goodDieCount > 0)
				file << "Die Yield:\t" << 100.0 * (panel.goodDieCount) / panel.dieCode.size() << "%" << std::endl;
			else
				file << "Yield:\t0%" << std::endl;

			if (panel.goodDieCount > 0) {
				file << std::endl << "<GOOD DIE DETAILS>" << std::endl;
				file << "Die ID" << std::endl;
				for (int i = 0; i < panel.dieCode.size(); i++) {
					auto v = std::find(panel.badDieinPart.begin(), panel.badDieinPart.end(), i);
					if(v == panel.badDieinPart.end())
						file << i << std::endl;
				}
				file << std::endl;
			}

			if (panel.badDieCount > 0) {
				file << std::endl << "<FAIL DIE DETAILS>" << std::endl;

				CString strTempKey;
				int nTempRowId, nTempColumnId, nTempDieId, nTempFov;

				file << "Die ID\tFov ID\tError Code" << std::endl;
				for (int die = 0; die < panel.dieCode.size(); die++) {
					for (int fov = 0; fov < panel.dieCode[die].size(); fov++) {
						if (panel.dieCode[die][fov] != GOOD_UNIT) {
							
							int nErrorCode = panel.dieCode[die][fov];
							

							file << std::setfill('0') << std::setw(3) << die << " \t" << std::setfill('0') << std::setw(3) << fov << " \t" 
								<< getCode(/*panel.dieCode[die][fov]*/nErrorCode) << std::endl;
						} 
					}
				}
			}
		}

		if (!bLotEnd) {
			CString strDeviceCodePath;
			strDeviceCodePath.Format("%sDevice Code_%s.dat", path, getTimeWithoutColon().c_str());

			std::ofstream file1(strDeviceCodePath.GetString());
			if (file1.is_open()) {
				file1 << "PANEL Id:" << panel.id << endl;
				file1 << "Date:" << panel.date << endl;
				file1 << "Start Time:" << panel.startTime << endl;
				file1 << "End Time:" << panel.endTime << endl;
				file1 << "Recipe:" << panel.strRecipe << endl;	//Recipe
				file1 << "Size :" << panel.deviceCode.size() << endl;

				for (int i = 0; i < panel.dieTopCode.size(); i++) {
					for (int j = 0; j < panel.dieTopCode[i].size(); j++)
						file1 << i << ":" << j << ":" << panel.dieTopCode[i][j] << endl;
				}
				for (int i = 0; i < panel.dieThrCode.size(); i++) {
					for (int j = 0; j < panel.dieThrCode[i].size(); j++)
						file1 << i << ":" << j << ":" << panel.dieThrCode[i][j] << endl;
				}
				for (int i = 0; i < panel.encapTopCode.size(); i++) {
					for (int j = 0; j < panel.encapTopCode[i].size(); j++)
						file1 << i << ":" << j << ":" << panel.encapTopCode[i][j] << endl;
				}
				for (int i = 0; i < panel.deviceCode.size(); i++) {
					file1 << i << ":" << panel.deviceCode[i] << endl;
				}
			}
		}
	}
	return 0;
}


void CDialogPane::savePrintBarSummaryLogFile() const
{
	CString logFilePath;
	logFilePath.Format("%s\\Summary\\Logfile_SFT\\", theApp.m_LotSummaryRootDir);
	CreateDirectoryTree(logFilePath);

	std::ofstream file(logFilePath + theApp.m_strMachineID.GetString() +"_"+ productionLotDetails.date.c_str() + "_SFT.csv");

	if (file.is_open()) {
		//file << "MID" << "," << "Date(DD/MM/YY)" << "," << "Time" << "," << "Pass = 1/Fail = 0";  
		file << "Date" << ","  << "Time" << "," <<"Model"<<","<< "Die Id " << "," << "Pen PF" << "," << "Die Number " << "," << "TOP Fov" << "," << "THR Fov " <<","<<"Fail Code" << "," << (theApp.m_bSubstrateMappingStyle ? "Pretray ID" : "MID");
		file << endl;
		for (std::list < PartInfo >::const_iterator p = productionLotDetails.panel.begin(); p != productionLotDetails.panel.end(); ++p) {

			CString strTempKey;
			CString strTemp = p->id.c_str();
			CString str;
			CString str1;
			if (m_pApp->motionParameters.isBarcodeEnabled())
				strTemp = strTemp.Left(4);

			str.Format("%s: %s ", strTemp, p->strRecipe.c_str());
			for (int DieNo = 0; DieNo < p->dieThrCode.size(); DieNo++) {

				strTemp.Format("%d", DieNo);

				for (int Fov = 0; Fov < p->dieTopCode[DieNo].size() ; Fov++){
					CString str1;
					str1.Format("%d", Fov);

					file << p->date << "," << p->startTime << "," << str.GetString() << "," << "" << "," << ((p->dieErrorCode[DieNo] == GOOD_UNIT) ? "0" : "1") << "," << strTemp << "," << str1 << "," << " ";
				
					file << "," << getInfo(getId(p->dieTopCode[DieNo][Fov]))->code.c_str();

					file << "," << p->id.c_str()<< endl;;
				}

				for (int Fov = 0; Fov < p->dieThrCode[DieNo].size(); Fov++) {
					CString str1;
					str1.Format("%d", Fov);

					file << p->date << "," << p->startTime << "," << str.GetString() << "," << "" << "," << ((p->dieErrorCode[DieNo] == GOOD_UNIT) ? "0" : "1") << "," << strTemp << "," << " " << "," << str1;

					file << "," << getInfo(getId(p->dieThrCode[DieNo][Fov]))->code.c_str();


					file << "," << p->id.c_str() << endl;;
				}
				
			}
		}
	}

	logFilePath.Format("%s\\Summary\\Logfile\\", theApp.m_LotSummaryRootDir);
	CreateDirectoryTree(logFilePath);

	std::ofstream file2(logFilePath + theApp.m_strMachineID.GetString() + "_" + productionLotDetails.date.c_str() + ".csv");
	
	if (file2.is_open()) {
		//file << "MID" << "," << "Date(DD/MM/YY)" << "," << "Time" << "," << "Pass = 1/Fail = 0";
		file2 << "Date(DD/MM/YY)" << "," << "Time" << "," << "Model" << "," << "MID" << "," << "Pen PF";

		file2 << endl;
		for (std::list < PartInfo >::const_iterator p = productionLotDetails.panel.begin(); p != productionLotDetails.panel.end(); ++p) {


			CString strTemp = p->id.c_str();
			CString str;
			if (m_pApp->motionParameters.isBarcodeEnabled())
				strTemp = strTemp.Left(4);

			str.Format("%s: %s ", strTemp, p->strRecipe.c_str());
			//		file << p->id.c_str() << "," << p->date << "," << p->startTime << "," << "Pass = " << p->goodDieCount << "/" << "Fail = " << p->badDieCount << endl;
			file2 << p->date << "," << p->startTime << "," << str.GetString() << "," << p->id.c_str() << "," << ((p->badDieCount > 0) ? "1" : "0");

			file2 << endl;
		}
	}
}


void CDialogPane::saveLotSummaryFile(const std::string & path) const
{
	std::ofstream file(path + productionLotDetails.lotId + ".txt");
	int nCount;
	if (file.is_open()) {
		file << "Machine ID:\t" << productionLotDetails.machineId << std::endl;
		file << "Start Time:\t" << productionLotDetails.startTime << std::endl;
		file << "End Time:\t" << productionLotDetails.endTime << std::endl;
		file << "Operator:\t" << productionLotDetails.operatorId << std::endl;

		file << std::endl << "<SUMMARY_REV>" << std::endl;

		file << "PANEL Inspected:\t" << productionLotDetails.panel.size() << std::endl;
		file << "PANEL Pass:\t" << productionLotDetails.goodPanelCount() << std::endl;
		file << "PANEL Fail:\t" << productionLotDetails.panel.size() - lotInfo().goodPanelCount() << std::endl;
		if (productionLotDetails.goodPanelCount() > 0)
			file << "Strip Yield:\t" << 100.0 * (productionLotDetails.goodPanelCount()) /
			productionLotDetails.panel.size() << "%" << std::endl;
		else
			file << "Yield:\t0%" << std::endl;

		file << "Dies Inspected:\t" << productionLotDetails.totalDieCount() << std::endl;
		file << "Dies Pass:\t" << productionLotDetails.goodDieCount() << std::endl;
		file << "Dies Fail:\t" << productionLotDetails.totalDieCount() - lotInfo().goodDieCount() << std::endl;
		if (productionLotDetails.goodPanelCount() > 0)
			file << "Die Yield:\t" << 100.0 * (productionLotDetails.goodPanelCount()) /
			productionLotDetails.totalDieCount() << "%" << std::endl;
		else
			file << "Yield:\t0%" << std::endl;

		file << std::endl;
		file << "Inspected Units:\t" << productionLotDetails.totalDeviceCount() << std::endl;
		file << "Units Pass:\t" << productionLotDetails.goodDeviceCount() << std::endl;
		file << "Units Fail:\t" << productionLotDetails.badDeviceCount() << std::endl;
		if (productionLotDetails.totalDeviceCount() > 0)
			file << "Unit Yield:\t" << 100.0 * (productionLotDetails.totalDeviceCount() - productionLotDetails.badDeviceCount()) /
			productionLotDetails.totalDeviceCount() << "%" << std::endl;
		else
			file << "Yield:\t0%" << std::endl;

	//	file << std::endl << "<UPH>" << std::endl;
		// static_cast<int>(3600000  / dAverageStripTimeMs)
//		file << "Parts Per Min:\t" << static_cast<int>(3600000 / theApp.m_pStatsView->getAvgStripTimeMs()) << std::endl;

	/*	file << std::endl << "<DEFECT TYPES>" << std::endl;

		nCount = 0;
		for (std::list < StripInfo >::const_iterator p = productionLotDetails.panel.begin(); p != _productionLotDetails.panel.end(); ++p) {
			nCount += p->deviceCodeCategory[SUBSTRATE];
		}
		file << "Substrate:\t" << nCount << std::endl;

		nCount = 0;
		for (std::list < StripInfo >::const_iterator p = _productionLotDetails.panel.begin(); p != _productionLotDetails.panel.end(); ++p) {
			nCount += p->deviceCodeCategory[DIE];
		}
		file << "Die:\t" << nCount << std::endl;

		nCount = 0;
		for (std::list < StripInfo >::const_iterator p = _productionLotDetails.panel.begin(); p != _productionLotDetails.panel.end(); ++p) {
			nCount += p->deviceCodeCategory[EPOXY];
		}
		file << "Epoxy:\t" << nCount << std::endl;

		nCount = 0;
		for (std::list < StripInfo >::const_iterator p = _productionLotDetails.panel.begin(); p != _productionLotDetails.panel.end(); ++p) {
			nCount += p->deviceCodeCategory[PVI];
		}
		file << "PVI:\t" << nCount << std::endl;

		nCount = 0;
		for (std::list < StripInfo >::const_iterator p = _productionLotDetails.panel.begin(); p != _productionLotDetails.panel.end(); ++p) {
			nCount += p->deviceCodeCategory[WIRE];
		}
		file << "Wire:\t" << nCount << std::endl;

		nCount = 0;
		for (std::list < StripInfo >::const_iterator p = _productionLotDetails.panel.begin(); p != _productionLotDetails.panel.end(); ++p) {
			nCount += p->deviceCodeCategory[BOND];
		}
		file << "Bond:\t" << nCount << std::endl;

		file << std::endl << "Incoming rejects:\t" << _productionLotDetails.rejectDeviceCount() << std::endl;*/

		file << std::endl << "<FAIL DETAILS>" << std::endl;
		file << "PANEL ID\tDie No." << std::endl;
		for (std::list < PartInfo >::const_iterator p = productionLotDetails.panel.begin(); p != productionLotDetails.panel.end(); ++p) {

			if (p->badDeviceCount) {
				file << p->id << "\t";
				
				for (int i = 0; i < p->badDieinPart.size(); i++)
					file << p->badDieinPart[i] << "\t";
				file << std::endl;
			/*	for (int i = 0; i < DEVICE_CATEGORIES; i++) {
					file << p->deviceCodeCategory[i] << "\t";
				}*/

			//	file << p->badDeviceCount << std::endl;
			}
		}

		file << std::endl << "<PASS DETAILS>" << std::endl;
		file << "PANEL ID" << std::endl;

		for (std::list < PartInfo >::const_iterator p = productionLotDetails.panel.begin(); p != productionLotDetails.panel.end(); ++p) {
			if (!p->badDeviceCount) {
				file << p->id << std::endl;
			}
		}
	}
}

void CDialogPane::setLotStartTime()
{
	productionLotDetails.startTime = getTime();
}

void CDialogPane::setLotEndTime()
{
	productionLotDetails.endTime = getDetailedTime();
}

std::vector<int > CDialogPane::getDefectCode()
{
	return _deviceCode;
}

void CDialogPane::setDefectCodes(std::vector <int> _defCode)
{
	_deviceCode = _defCode;
}

void CDialogPane::UpdateDieCount()
{
	int rowId, columnId, dieId, nFov;
	_badDieNo.clear();
	for (int i = 0; i < _deviceCode.size(); i++) {
		m_pApp->motionParameters.getImageMappingDetails(i, rowId, columnId, dieId, nFov);
		_setDieStatus(dieId, nFov, _deviceCode[i]);
	}
}

void CDialogPane::setStripEndTime()
{

	int nErrorCode = 0;
	if (!productionLotDetails.panel.empty()) {
		productionLotDetails.panel.back().endTime = getTime();
		productionLotDetails.panel.back().badDeviceCount = badDeviceCount();
		productionLotDetails.panel.back().goodDeviceCount = goodDeviceCount();
		productionLotDetails.panel.back().rejectDeviceCount = rejectDeviceCount();
		productionLotDetails.panel.back().totalDeviceCount = totalDeviceCount();
		productionLotDetails.panel.back().codeReport = getCodeReport();
		productionLotDetails.panel.back().rejectReport = getRejectReport();
		productionLotDetails.panel.back().failUnitId = getFailUnitId();
		productionLotDetails.panel.back().deviceCode = _deviceCode;
		productionLotDetails.panel.back().strRecipe = theApp.m_strConfigFile;

		int rowId, columnId, dieId, nFov;
		_badDieNo.clear();
		for (int i = 0; i < _deviceCode.size(); i++) {		
			m_pApp->motionParameters.getImageMappingDetails(i, rowId, columnId, dieId, nFov);
			_setDieStatus(dieId, nFov, _deviceCode[i]);
		}

		productionLotDetails.panel.back().dieCode = _dieCode;
		productionLotDetails.panel.back().dieTopCode = _dieTopCode;
		productionLotDetails.panel.back().encapTopCode = _encapTopCode;
		productionLotDetails.panel.back().dieThrCode = _dieThrCode;
		productionLotDetails.panel.back().goodDieCount = goodDieCount();
		productionLotDetails.panel.back().badDieCount = badDieCount();
		std::sort(_badDieNo.begin(), _badDieNo.end());
		productionLotDetails.panel.back().badDieinPart = _badDieNo;
		productionLotDetails.panel.back().dieErrorCode = _dieErrorCode;

		if (theApp.m_bEnableReScanOption) {
			theApp.motionParameters.ResetReScanStatus();
			for (int die = 0; die < _badDieNo.size(); die++)
				theApp.motionParameters.SetReScanStatus(_badDieNo[die], true);
		}

		if (theApp.IsSFTEnabled()) {
			ClearSFTMoveOutLog(theApp.getSFTServerAddressPath());
			SetSFTMoveOutLog(theApp.getSFTServerAddressPath(), productionLotDetails.panel.back().id.c_str(), _badDieNo.size() > 0 ? false : true);
		}
		/*for (int i = 0; i < _deviceCode.size(); i++) {

			nErrorCode = _deviceCode[i];

			if (nErrorCode != 0) {

				nErrorCode = nErrorCode / 100;

				if (nErrorCode >= 0 && nErrorCode < 6)
					productionLotDetails.panel.back().deviceCodeCategory[nErrorCode]++;
			}
		}*/

	//	theApp.m_bEndOfCurrentStrip = TRUE;
	}
}

const LotInfo & CDialogPane::lotInfo() const
{
	return productionLotDetails;
}

int CDialogPane::SaveMappingData()
{
	// 1. Check input data
	/*if(strLotID.IsEmpty()) return -1;
	if(strStripID.IsEmpty()) return -1;*/

	//	OutputDebugLogTo(9, TRUE, "Lot %s, StripId %s", strLotID, strStripID);
	// 2. Create configuration folder and statistics file


	if (!lotInfo().panel.empty()) {
		const PartInfo & panel = lotInfo().panel.back();
		CString strTemp;
		CString strFolderPath;
		strFolderPath.Format("%s\\Summary\\%s\\%s\\%s\\", theApp.m_LotSummaryRootDir, theApp.m_strConfigFile, productionLotDetails.lotId.c_str(), panel.id.c_str());
		CreateDirectoryTree(strFolderPath);

		CString strFilePath = strFolderPath + "\\" + panel.id.c_str() + ".csv";

		FILE* fileExcel = fopen(strFilePath, "r");

		if (fileExcel == NULL) {
			fileExcel = fopen(strFilePath, "w");

			if (fileExcel == NULL) return -1;
		}

		// 3. Save information into statistic file
		CString strWrite;
		int nTempRowId, nTempColumnId, nTempDieId, nTempFov;
		CString strTempKey;

		const int nDiePerX = nDeviceNoPerSubstrateX * nDieNoPerDeviceTOPX;
		const int nDiePerY = nDeviceNoPerSubstrateY * nDieNoPerDeviceTOPY;

		//theApp.inspectionResultStripMap.resize(nDiePerY *  nDiePerX);
		std::vector< int > inspectionResultStripMap;
		inspectionResultStripMap.resize(nDiePerY *  nDiePerX);

		int i, j;

		for (i = 0; i < nDiePerX; i++) {
			strTemp.Format("Column %d,", i + 1);
			strWrite.Append(strTemp);
		}

		fprintf(fileExcel, ",,,,, SUMMARY \n");
		fprintf(fileExcel, ",%s\n", strWrite);

		strWrite = "";
		for (i = 0; i < nDiePerY; i++) {
			strTemp.Format("Row %d,", i + 1);
			strWrite.Append(strTemp);

			for (j = 0; j < nDiePerX; j++) {
				int nErrorCode = _deviceCode[i * nDiePerX + j];

				strTemp.Format("%s,", getInfo(getId(nErrorCode))->code.c_str());

				strWrite.Append(strTemp);

				//OutputDebugLogTo(7, FALSE, "Row -  %d, Col - %d, inspectionResultStripMap Index - %d, _deviceCode Index - %d", i, j, i * nDiePerX + j, i * nDiePerX + j);
				inspectionResultStripMap[i * nDiePerX + j] = _deviceCode[i * nDiePerX + j];
				
			}

			fprintf(fileExcel, "%s\n", strWrite);
			strWrite = "";
		}

		fclose(fileExcel);

		// 4. Create bitmap
		HDC hDC = ::GetDC(0);
		HDC hMemDC = ::CreateCompatibleDC(hDC);

		int nImageWidth = _deviceRoi[nTotalNo - 1].right + 25;
		int nImageHeight = _deviceRoi[nTotalNo - 1].bottom + 58;

		if (nImageWidth < _deviceRoi[0].left + 400)
			nImageWidth = _deviceRoi[0].left + 400;

		HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, nImageWidth, nImageHeight);
		HBITMAP hBitmapOld = (HBITMAP)::SelectObject(hMemDC, hBitmap);

		::FillRect(hMemDC, CRect(0, 0, nImageWidth, nImageHeight), brushBackground);

		std::vector < CRect >::const_iterator roi = _deviceRoi.begin();
		std::vector < int >::const_iterator code = _deviceCode.begin();

		
		for (i = 0; i < nTotalNo; i++, ++code, ++roi) {
			int nErrorCode = *code;

			::FillRect(hMemDC, *roi, getInfo(getId(nErrorCode))->brush);
		}

		//--. Draw statistics
		::SetBkColor(hMemDC, RGB(220, 220, 220));

		CString strText;

		::SelectObject(hMemDC, &m_FontText);

		//if(!strStripID.IsEmpty())
		//	strText.Format("Bad units: %d, Good units: %d, Remained units: %d, Barcode ID: %s",
		//		badDeviceCount(), goodDeviceCount(), remainDeviceCount(), strStripID);
		//else
		//	strText.Format("Bad units: %d, Good units: %d, Remained units: %d",
		//		badDeviceCount(), goodDeviceCount(), remainDeviceCount() );

		if (!theApp.strInspectDeviceBarcodeID.IsEmpty())
			strText.Format("Bad units: %d, Good units: %d, Remained units: %d, Barcode ID: %s",
				badDeviceCount() + rejectDeviceCount(), goodDeviceCount() - rejectDeviceCount(), remainDeviceCount(), theApp.strInspectDeviceBarcodeID);
		else
			strText.Format("Bad units: %d, Good units: %d, Remained units: %d",
				badDeviceCount() + rejectDeviceCount(), goodDeviceCount() - rejectDeviceCount(), remainDeviceCount());

		CRect rcRect = CRect(_deviceRoi[0].left,
			_deviceRoi[nTotalNo - 1].bottom + 10,
			_deviceRoi[0].left + 800,
			_deviceRoi[nTotalNo - 1].bottom + 25);

		::DrawTextA(hMemDC, strText, -1, rcRect, DT_LEFT);

		if (_badDeviceCount == 0 && _goodDeviceCount == _deviceRoi.size()) {
			strText.Format("PANEL Status: PASS");
			::SetBkColor(hMemDC, RGB(147, 255, 128));

		}
		else if (_badDeviceCount > 0 || _badDeviceCountTop > 0) {
			if (strDefectName != "") {
				strText.Format("PANEL Status: FAIL (%s)", strDefectName.c_str());
			}
			else {
				strText.Format("PANEL Status: FAIL");
			}
			
			::SetBkColor(hMemDC, RGB(255, 153, 153));
		}
		else {
			strText.Format("PANEL Status:");
			::SetBkColor(hMemDC, RGB(204, 221, 255));
		}

		rcRect = CRect(_deviceRoi[0].left   * nZoom - nHorizontalScrollBar,
			_deviceRoi[nTotalNo - 1].bottom * nZoom - nVerticalScrollBar + 40,
			_deviceRoi[0].left   * nZoom - nHorizontalScrollBar + 840,
			_deviceRoi[nTotalNo - 1].bottom * nZoom - nVerticalScrollBar + 55);

		::DrawTextA(hMemDC, strText, -1, rcRect, DT_LEFT);

	//	dcMemory.SelectObject(pOldFont);

	
		CImage image;
		image.Attach(hBitmap);

		//strFilePath = strFolderPath + "\\" + strStripID + ".bmp";
		//image.Save(strFilePath, Gdiplus::ImageFormatBMP);

		strFilePath = strFolderPath + "\\" + panel.id.c_str() + ".jpg";
		image.Save(strFilePath, Gdiplus::ImageFormatJPEG);

		::SelectObject(hMemDC, hBitmapOld);
		::DeleteObject(hBitmap);
		::DeleteDC(hMemDC);
		::ReleaseDC(0, hDC);
	}
	return 0;
}

int CDialogPane::ApplyVerificationResultsToDie(int nDie, std::string strDefectCode)
{
	long nCameraGrabId;
	int nDieId;
	int nTotalFov = theApp.motionParameters.GetTotalFOVPerDieTHR();
	int nCode = getCode(strDefectCode);
	for (int i = 0; i < nTotalFov; i++) {
		theApp.motionParameters.GetCameraGrabIDTHR(0, nDie, i, nCameraGrabId);
		theApp.motionParameters.getCameraGrabIdTHR(nCameraGrabId, nDieId);
	
		if (nCode != _deviceCode[nDieId]) {
			if (nCode == GOOD_UNIT) {
				_goodDeviceCount++;

				if(_deviceCode[nDieId]!= NOT_INSPECTED)
					_badDeviceCount--;
			}
			else if (nCode != GOOD_UNIT && _deviceCode[nDieId] == GOOD_UNIT && nCode != NOT_INSPECTED) {
				_goodDeviceCount--;
				_badDeviceCount++;
			}
			else if (nCode == NOT_INSPECTED) {
				if (_deviceCode[nDieId] == GOOD_UNIT)
					_goodDeviceCount--;
				else
					_badDeviceCount--;

			}
			_deviceCode[nDieId] = nCode;
		}

	}

	Invalidate();
	return 0;
}

int CDialogPane::ApplyVerificationResultsToFov(int nDie, int nFov, std::string strDefectCode)
{
	long nCameraGrabId;
	int nDieId;
	theApp.motionParameters.GetCameraGrabIDTHR(0, nDie, nFov, nCameraGrabId);
	theApp.motionParameters.getCameraGrabIdTHR(nCameraGrabId, nDieId);

	int nCode = getCode(strDefectCode);
	if (nCode != _deviceCode[nDieId]) {

		if (nCode == GOOD_UNIT) {
			_goodDeviceCount++;
			
			if (_deviceCode[nDieId] != NOT_INSPECTED)
				_badDeviceCount--;
		}
		else if (nCode != GOOD_UNIT && _deviceCode[nDieId] == GOOD_UNIT && nCode != NOT_INSPECTED) {
			_goodDeviceCount--;
			_badDeviceCount++;

		}
		else if (nCode == NOT_INSPECTED) {
			if (_deviceCode[nDieId] == GOOD_UNIT)
				_goodDeviceCount--;
			else
				_badDeviceCount--;

		}

		_deviceCode[nDieId] = nCode;
		Invalidate();
	}

	return 0;
}

CString CDialogPane::GetDefectName(int nError)
{

	CString strDefectName;
	strDefectName=	getInfo(getId(nError))->code.c_str();
	return strDefectName;
}

void CDialogPane::ReadLotSummayFile(const std::string & path, bool & bLotFound)			//Girish
{

	std::ifstream file(path.c_str() + getDate2() + ".txt");

	if (file.is_open()) {
		bLotFound = TRUE;
		productionLotDetails.reset();

		std::string data;
		CString strTemp;

		std::getline(file, data);

		std::getline(file, data);
		if (!data.empty()) {
			std::size_t pos = data.find(':');
			if (pos != std::string::npos && pos > 0) {
				strTemp = data.substr(pos + 2).c_str();
				strTemp = strTemp.TrimRight('\r');
				productionLotDetails.startTime = strTemp;
			}
		}
		productionLotDetails.lotId = getDate2().c_str();
		file.close();
	}
}

void CDialogPane::AddStripDetails(const std::string & path, std::string strBarcode)		//Girish
{
	std::vector <int> statisticInspectionResult;
	std::ifstream file(path);
	_reset();
	int columnCount = nDeviceNoPerSubstrateX * nDieNoPerDeviceTOPX;
	int rowCount = nTotalNo / columnCount;
	int id2 = 0;
	int realId = 0;
	int nErrorCode = 0;
	CString strTemp;
	std::vector <int > tempDeviceCode;

	if (file.is_open()) {
		statisticInspectionResult.clear();
		productionLotDetails.panel.push_back(PartInfo(strBarcode, _deviceCode.size(), _dieCode.size(), getTime(), getDate(),""));
		std::string data;

		std::getline(file, data);		
		std::getline(file, data);		//Start date :
		if (!data.empty()) {
			std::size_t pos = data.find(':');
			if (pos != std::string::npos && pos > 0) {
				strTemp = data.substr(pos + 1).c_str();
				strTemp = strTemp.TrimRight('\r');
				productionLotDetails.panel.back().date = strTemp;

				if (productionLotDetails.panel.size() == 1) {
					productionLotDetails.date = strTemp;
				}
			}
			else {
				productionLotDetails.panel.pop_back();
				file.close();
				return;
			}
		}
		else {
			productionLotDetails.panel.pop_back();
			file.close();
			return;
		}

		std::getline(file, data);		//Start Time:
		if (!data.empty()) {
			std::size_t pos = data.find(':');
			if (pos != std::string::npos && pos > 0) {
				strTemp = data.substr(pos + 1).c_str();
				strTemp = strTemp.TrimRight('\r');
				productionLotDetails.panel.back().startTime = strTemp;

				if (productionLotDetails.panel.size() == 1) {
					productionLotDetails.startTime = strTemp;
				}
			}
		}

		std::getline(file, data);		//End Time:
		if (!data.empty()) {
			std::size_t pos = data.find(':');
			if (pos != std::string::npos && pos > 0) {
				strTemp = data.substr(pos + 1).c_str();
				strTemp = strTemp.TrimRight('\r');
				productionLotDetails.panel.back().endTime = strTemp;
			}
		}

		std::getline(file, data);		//Recipe:
		if (!data.empty()) {
			std::size_t pos = data.find(':');
			if (pos != std::string::npos && pos > 0) {
				strTemp = data.substr(pos + 1).c_str();
				strTemp = strTemp.TrimRight('\r');
				productionLotDetails.panel.back().strRecipe = strTemp;
			}
		}

		std::getline(file, data);			//Total device count
		if (!data.empty()) {
			std::size_t pos = data.find(':');
			if (pos != std::string::npos && pos > 0) {
				strTemp = data.substr(pos + 1).c_str();
				strTemp = strTemp.TrimRight('\r');
				productionLotDetails.panel.back().totalDeviceCount = atoi(strTemp);
			}
		}

		tempDeviceCode.resize(productionLotDetails.panel.back().totalDeviceCount);

		int Die, Fov, Code;
		for (int i = 0; i < _dieTopCode.size(); i++) {
			for (int j = 0; j < _dieTopCode[i].size(); j++) {
				std::getline(file, data);
				vector <string> tokens;
				stringstream check1(data);
				std::string intermediate;
				getline(check1, intermediate, ':');
				Die = atoi(intermediate.c_str());
				getline(check1, intermediate, ':');
				Fov = atoi(intermediate.c_str());
				getline(check1, intermediate, ':');
				Code = atoi(intermediate.c_str());
				_dieTopCode[Die][Fov] = Code;
			}
		}

		for (int i = 0; i < _dieThrCode.size(); i++) {
			for (int j = 0; j < _dieThrCode[i].size(); j++) {
				std::getline(file, data);
				vector <string> tokens;
				stringstream check1(data);
				std::string intermediate;
				getline(check1, intermediate, ':');
				Die = atoi(intermediate.c_str());
				getline(check1, intermediate, ':');
				Fov = atoi(intermediate.c_str());
				getline(check1, intermediate, ':');
				Code = atoi(intermediate.c_str());
				_dieThrCode[Die][Fov] = Code;
			}
		}

		for (int i = 0; i < _encapTopCode.size(); i++) {
			for (int j = 0; j < _encapTopCode[i].size(); j++) {
				std::getline(file, data);
				vector <string> tokens;
				stringstream check1(data);
				std::string intermediate;
				getline(check1, intermediate, ':');
				Die = atoi(intermediate.c_str());
				getline(check1, intermediate, ':');
				Fov = atoi(intermediate.c_str());
				getline(check1, intermediate, ':');
				Code = atoi(intermediate.c_str());
				_encapTopCode[Die][Fov] = Code;
			}
		}


		int nCount = 0;
		while (std::getline(file, data)) {
			CString strNum;
			if (!data.empty()) {
				std::size_t pos = data.find(':');
				if (pos != std::string::npos && pos > 0) {
					strTemp = data.substr(pos + 1).c_str();
					strTemp = strTemp.TrimRight('\r');

					strNum = data.substr(0, pos).c_str();
					int n = atoi(strNum);
					tempDeviceCode[n] = atoi(strTemp);
					_setDeviceStatus(n, tempDeviceCode[n]);
				}
			}
		}

		if (!productionLotDetails.panel.empty()) {
			productionLotDetails.panel.back().badDeviceCount = badDeviceCount();
			productionLotDetails.panel.back().goodDeviceCount = goodDeviceCount();
			productionLotDetails.panel.back().rejectDeviceCount = rejectDeviceCount();
			productionLotDetails.panel.back().totalDeviceCount = totalDeviceCount();
			productionLotDetails.panel.back().codeReport = getCodeReport();
			productionLotDetails.panel.back().rejectReport = getRejectReport();
			productionLotDetails.panel.back().failUnitId = getFailUnitId();
			productionLotDetails.panel.back().deviceCode = tempDeviceCode;

			int rowId, columnId, dieId, nFov;
			_badDieNo.clear();
			for (int i = 0; i < tempDeviceCode.size(); i++) {
				m_pApp->motionParameters.getImageMappingDetails(i, rowId, columnId, dieId, nFov);
				_setDieStatus(dieId, nFov, tempDeviceCode[i]);
			}

			productionLotDetails.panel.back().dieCode = _dieCode;
			productionLotDetails.panel.back().dieTopCode = _dieTopCode;
			productionLotDetails.panel.back().encapTopCode = _encapTopCode;
			productionLotDetails.panel.back().dieThrCode = _dieThrCode;
			productionLotDetails.panel.back().goodDieCount = goodDieCount();
			productionLotDetails.panel.back().badDieCount = badDieCount();
			std::sort(_badDieNo.begin(), _badDieNo.end());
			productionLotDetails.panel.back().badDieinPart = _badDieNo;
			productionLotDetails.panel.back().dieErrorCode = _dieErrorCode;

		}

		file.close();
	}
}

void CDialogPane::ReadStripSummary(const std::string & path)			//Girish
{
	CString strPath = path.c_str();
	CString strTempPath;
	CFileFind findFile;
	BOOL bNextFile = findFile.FindFile(strPath + "*.*");
	std::string panelId;
	CString strFileName;
	CString strFilePath;
	CString strExt, strTemp;

	while (bNextFile) {

		bNextFile = findFile.FindNextFile();

		if (findFile.IsDots())
			continue;

		strFileName = findFile.GetFileName();

		if (findFile.IsDirectory()) {
			strTempPath.Format("%s%s\\", strPath, strFileName);
			ReadStripSummary(strTempPath.GetString());
		}

		strFileName = findFile.GetFileName();
		strFilePath = findFile.GetFilePath();
		strExt = strFileName.Right(3);
		strFileName = strFileName.Left(strFileName.Find('.'));
		if (strExt.CompareNoCase("dat"))
			continue;
		strTemp = strFilePath.Left(strFilePath.ReverseFind('\\'));
		int n = strTemp.ReverseFind('\\');
		strTemp = strTemp.Right(strTemp.GetLength() - strTemp.ReverseFind('\\') - 1);

		if (/*strTemp.Find(strFileName) >= 0 &&*/ StrCmpN("Device Code",strFileName.GetString(),11)==0) {
//			compareStripId(strTemp.GetString());
			AddStripDetails(strFilePath.GetString(), strTemp.GetString());
		}
	}
}

void CDialogPane::compareStripId(const std::string & stripId)			//Girish
{
	std::list < PartInfo >::const_iterator p = std::find_if(productionLotDetails.panel.begin(), productionLotDetails.panel.end(),
		[stripId](const PartInfo & strip) { return strcmp(stripId.c_str(), strip.id.c_str()) == 0; });

	if (p != productionLotDetails.panel.end()) {
		std::vector <int> deviceCode;
		deviceCode = p->deviceCode;
	/*	if (!theApp.pStatisticsWnd->bStatisticsResetDone) {
			theApp.pStatisticsWnd->RemoveFromStatistics(deviceCode);
			theApp.pStatisticsWnd->DecreaseStripNumber();
		}*/
		productionLotDetails.panel.erase(p);
	}
}

void CDialogPane::ClearSFTMoveInLog(std::string strPath)
{
	std::ofstream file(strPath + "\\MoveIn\\" + "SFTMoveInLog" + ".txt");
	if (file.is_open()) {
		file << "MID" << "," << "MoveType" << std::endl;
		file.close();
	}

}

void CDialogPane::ClearSFTMoveOutLog(std::string strPath)
{
	std::ofstream file(strPath + "\\MoveOut\\" + "SFTMoveoutLog" + ".txt");
	if (file.is_open()) {
		file << "MID" << "," << "MoveType" << "," << "PenPF" << std::endl;
		file.close();
	}
}



void CDialogPane::SetSFTMoveInLog(std::string strPath, std::string strBarcodeId)
{
	std::ofstream file(strPath + "\\MoveIn\\" + "SFTMoveInLog" + ".txt");
	if (file.is_open()) {
		file << "MID" << "," << "MoveType" << std::endl;
		file << strBarcodeId.c_str() << "," << theApp.getSFTMoveTypeMoveIn() << std::endl;
		file.close();

	}
}

void CDialogPane::SetSFTMoveOutLog(std::string strPath, std::string strbarcodId, bool bResults)
{
	std::ofstream file(strPath + "\\MoveOut\\" + "SFTMoveoutLog" + ".txt");
	if (file.is_open()) {
		file << "MID" << "," << "MoveType" << "," << "PenPF" << std::endl;
		file << strbarcodId.c_str() << "," << theApp.getSFTMoveTypeMoveOut() << "," <<bResults << std::endl;
		file.close();
	}
}

int CDialogPane::GetSFTMoveInResults(std::string strPath, bool &Results)
{
	std::ifstream file(strPath + "\\MoveIn\\" + "Result\\" + "MoveInResult" + ".txt");
	std::string data;
	if (file.is_open()) {
		std::getline(file, data);

		if (!data.empty())
			Results = atoi(data.c_str());
		else
			return -2;

		file.close();
	}
	else
		return -1;

	return 0;
}

void CDialogPane::ClearSFTMoveInResults(std::string strPath)
{
	std::ofstream file(strPath + "\\MoveIn\\" + "Result\\" + "MoveInResult" + ".txt");
	if (file.is_open()) {
		file << "" << std::endl;
		file.close();
	}

}

BOOL CDialogPane::IsPreTrayRepeat(CString BarcodeId)
{
		BOOL bIsRepeat = FALSE;
		nRepeatGoodDieCount = 0;
		nRepeatBadDieCount = 0;
		nRepeatunitInspected[STATION_TOP] = 0;
		nRepeatpassedUnit[STATION_TOP] = 0;
		nRepeatFailedunit[STATION_TOP] = 0;
		nRepeatunitInspected[STATION_THR] = 0;
		nRepeatpassedUnit[STATION_THR] = 0;
		nRepeatFailedunit[STATION_THR] = 0;
	for (std::list < PartInfo >::const_iterator p = productionLotDetails.panel.begin(); p != productionLotDetails.panel.end(); ++p) {
		if (!strcmp(p->id.c_str(), BarcodeId)) {

			nRepeatunitInspected[STATION_TOP] = 0;
			nRepeatpassedUnit[STATION_TOP] = 0;
			nRepeatFailedunit[STATION_TOP] = 0;
			nRepeatunitInspected[STATION_THR] = 0;
			nRepeatpassedUnit[STATION_THR] = 0;
			nRepeatFailedunit[STATION_THR] = 0;
			nRepeatGoodDieCount = p->goodDieCount;
			nRepeatBadDieCount = p->badDieCount;
			for (int i = 0; i < p->dieTopCode.size(); i++) {
				for (int j = 0; j < p->dieTopCode[i].size(); j++) {
					if (p->dieTopCode[i][j] == NOT_INSPECTED)
						continue;
					nRepeatunitInspected[STATION_TOP]++;
					if (p->dieTopCode[i][j] == GOOD_UNIT)
						nRepeatpassedUnit[STATION_TOP]++;
					else
						nRepeatFailedunit[STATION_TOP]++;
				}

			}
			for (int i = 0; i < p->dieThrCode.size(); i++) {
				for (int j = 0; j < p->dieThrCode[i].size(); j++) {
					if (p->dieThrCode[i][j] == NOT_INSPECTED)
						continue;
						nRepeatunitInspected[STATION_THR]++;
						if (p->dieThrCode[i][j] == GOOD_UNIT )
							nRepeatpassedUnit[STATION_THR]++;
						else
							nRepeatFailedunit[STATION_THR]++;
					
				}
			}
			bIsRepeat = TRUE;
		}
	}

	return bIsRepeat;
}


void CDialogPane::UpdateStatsOnRepeatScan()
{

		for (int j = 0; j < theApp.m_nTotalTracks; j++) {
			theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsInspected -= nRepeatunitInspected[j];
			theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsPass -= nRepeatpassedUnit[j];
			theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsFail -= nRepeatFailedunit[j];

			if (theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsInspected < 0)
				theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsInspected = 0;

			if (theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsPass < 0)
				theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsPass = 0;

			if (theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsFail < 0)
				theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsFail = 0;

			if (theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsInspected != (theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsPass + theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsFail))
				theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsInspected = theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsPass + theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsFail;

			if (theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsInspected > 0) {
				theApp.m_LotSummary.m_TotalDisplaySummary[j].dTotalYield = double(theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsPass) /
					double(theApp.m_LotSummary.m_TotalDisplaySummary[j].lTotalUnitsInspected) * 100 + 0.04;
			}
			else
				theApp.m_LotSummary.m_TotalDisplaySummary[j].dTotalYield = 0;
		}
		theApp.UpdateStatisticsWindow();

}