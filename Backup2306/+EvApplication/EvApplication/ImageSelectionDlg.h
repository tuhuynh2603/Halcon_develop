#pragma once
#include "PictureCtrl.h"
#include "resource.h"

// CImageSelectionDlg dialog
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <vector>

struct GroupInfo
{
	GroupInfo()
		: id(0)
		, name("unknown")
	{ };

	GroupInfo(const std::string & aName, int anId)
		: id(anId)
		, name(aName)
	{ };

	int id;
	std::string name;
};

struct ItemInfo
{
	ItemInfo()
		: description("no description")
		, code("unknown code")
		, id(1)
		, groupId(0)
		, color(RGB(128, 128, 128))
		, good(false)
	{
		brush.CreateSolidBrush(color);
	};

	ItemInfo(const std::string & aDescription, const std::string & aCode, int anId, int aGroupId, COLORREF aColor, bool aGood)
		: description(aDescription)
		, code(aCode)
		, id(anId)
		, groupId(aGroupId)
		, color(aColor)
		, good(aGood)
	{
		brush.CreateSolidBrush(color);
	};

	ItemInfo(const ItemInfo & info)
		: description(info.description)
		, code(info.code)
		, id(info.id)
		, groupId(info.groupId)
		, color(info.color)
		, good(info.good)
	{
		brush.CreateSolidBrush(color);
	};

	ItemInfo & operator =(const ItemInfo & info)
	{
		description = info.description;
		code = info.code;
		id = info.id;
		groupId = info.groupId;
		color = info.color;
		good = info.good;

		brush.DeleteObject();
		brush.CreateSolidBrush(color);

		return *this;
	}

	~ItemInfo()
	{
		brush.DeleteObject();
	}

	void set(const ItemInfo & info)
	{
		description = info.description;
		code = info.code;
		color = info.color;
		brush.DeleteObject();
		brush.CreateSolidBrush(color);
	}

	std::string description;
	std::string code;
	int id;
	int groupId;
	CBrush brush;
	COLORREF color;
	bool good;
};

//enum DefectInfo
//{
//	NOT_INSPECTED = -2,
//	UNKNOWN_DEFECT = -1,
//	GOOD_UNIT = 0
//};

class DefectContainer
{
public:
	void addGroup(const std::string & name, int id);

	void addDefect(const std::string & description, const std::string & code, int id, int groupId, bool good = false);
	void addDefect(const std::string & description, const std::string & code, int id, int groupId, COLORREF color, bool good = false);

	void setDefect(const ItemInfo & info);
	const std::vector < ItemInfo > & getDefect() const;

	GroupInfo getGroup(int id) const;
	const std::vector < GroupInfo > & getGroup() const;

	std::string getCode(int id) const;
	int getCode(std::string) const;				//Girish

	void load(const std::string & name);
	void save(const std::string & name) const;
protected:
	int getId(int id) const;
	std::vector < ItemInfo >::iterator       getInfo(int id);
	std::vector < ItemInfo >::const_iterator getInfo(int id) const;
	bool isGood(int id) const;
private:
	std::vector < ItemInfo > _defect;
	std::vector < GroupInfo > _group;
};

class DeviceContainer : public DefectContainer
{
public:
	DeviceContainer();
	~DeviceContainer();

	size_t goodDeviceCount()   const;
	size_t badDeviceCount()   const;
	size_t remainDeviceCount() const;
	size_t totalDeviceCount()  const;
	size_t rejectDeviceCount() const;
	size_t rejectDieCount() const;
	size_t goodDieCount() const;
	size_t badDieCount() const;

	// this function returns pair of ItemInfo ID and number of devices with this number
	// so you can consider this as histogram
	std::vector < std::pair < int, size_t > > getDefectReport() const;

	// this function returns map with key as defect code and value as a count of defects
	std::map < std::string, size_t > getCodeReport() const;

	// this function returns map with key as defect code for rejects and value as a count of defects
	std::map < std::string, size_t > getRejectReport() const;

	// this function returns true in case if subsequent number of devices are bad and this number is more or equal input value
	bool areContinuousBadDevices(size_t minimumCount) const;
protected:
	std::vector < int >   _deviceCode;
	std::vector < CRect > _deviceRoi;
	std::vector < CRect > _dieRoi;
	std::vector < CRect > _deviceErrorCode;
	std::vector <int > _deviceCountDone;
	std::vector < std::vector <int >> _dieCode;
	std::vector < std::vector <int >> _dieTopCode;
	std::vector < std::vector <int >> _dieThrCode;
	std::vector < std::vector <int >> _encapTopCode;

	std::vector <int > _dieErrorCode;

	size_t _blockXCount;
	size_t _blockYCount;
	size_t _deviceXCount;
	size_t _deviceYCount;

	void _reset();
	void _setReScan();
	void _resetResults();

	bool _setDieStatus(int dieId, int nFov, int code);
	bool _setDeviceStatus(size_t id, int code, bool bCount = true, bool bOverWriteResults = false);

	void _setTopThrDeviceStatus(long CameraId, int code, int StationId, int nDocId);

	bool _updateDieResults(int nDie, int nErrorCode);
	void _resize(size_t blockXCount, size_t blockYCount, size_t deviceXCount, size_t deviceYCount);
	
protected:
	size_t _goodDeviceCount;
	size_t _badDeviceCount;
	size_t _badDeviceCountTop;			//To update status of panel
	std::string strDefectName;
	std::vector <int > _badDieNo;
};

struct PartInfo
{
	PartInfo()
		: id("Unknown")
		, goodDeviceCount(0)
		, badDeviceCount(0)
		, totalDeviceCount(0)
		, rejectDeviceCount(0)
		, strRecipe("")
	{ };

	PartInfo(const std::string & _id, size_t _totalDeviceCount, size_t _totalDieCount, const std::string & _startTime, const std::string & _date, const std::string & _recipe)
		: id(_id)
		, startTime(_startTime)
		, goodDeviceCount(_totalDeviceCount)
		, badDeviceCount(0)
		, totalDeviceCount(_totalDeviceCount)
		, rejectDeviceCount(0)
		,goodDieCount(_totalDieCount)
		,badDieCount(0)
		,totalDieCount(_totalDieCount)
		,date(_date)
		,strRecipe(_recipe)
	{
		for (int i = 0; i < 6; i++)
			deviceCodeCategory[i] = 0;
	};

	std::string id;

	std::string startTime;
	std::string endTime;
	std::string date;
	std::string strRecipe;
	size_t goodDeviceCount;
	size_t badDeviceCount;
	size_t totalDeviceCount;
	size_t rejectDeviceCount;
	std::vector <int> deviceCode;

	//Die level
	size_t goodDieCount;
	size_t badDieCount;
	size_t totalDieCount;
	std::vector <int> badDieinPart;
	std::vector < std::vector <int> > dieCode;
	std::vector < std::vector <int> > dieTopCode;
	std::vector < std::vector <int> > encapTopCode;
	std::vector < std::vector <int> > dieThrCode;
	std::vector < int > dieErrorCode;

	int deviceCodeCategory[6];

	std::string getCodes() const
	{
		std::string codes;

		for (std::map < std::string, size_t >::const_iterator code = codeReport.begin(); code != codeReport.end(); ++code) {
			if (code->second > 0)
				codes = codes + code->first;
		}

		return codes;
	}

	std::vector < int > failUnitId;					//Girish
	std::map < std::string, size_t > codeReport;
	std::map < std::string, size_t > rejectReport;
};

struct LotBasicInfo
{
	LotBasicInfo()
		: machineId("Unknown")
		, lotId("Unknown")
		, woId("Unknown")
		, partId("Unknown")
		, operatorId("Unknown")
	{ };


	std::string machineId;
	std::string woId;
	std::string lotId;
	std::string partId;
	std::string operatorId;
};

struct LotInfo : public LotBasicInfo
{
	LotInfo()
	{ };

	void reset()
	{
		panel.clear();
		startTime.clear();
		endTime.clear();
	};

	size_t goodDieCount() const
	{
		size_t count = 0;

		for (std::list < PartInfo >::const_iterator p = panel.begin(); p != panel.end(); ++p)
			count += p->goodDieCount;

		return count;
	}

	size_t badDieCount() const
	{
		size_t count = 0;

		for (std::list < PartInfo >::const_iterator p = panel.begin(); p != panel.end(); ++p)
			count += p->badDieCount;

		return count;
	}

	size_t totalDieCount() const
	{
		size_t count = 0;

		for (std::list < PartInfo >::const_iterator p = panel.begin(); p != panel.end(); ++p)
			count += p->totalDieCount;

		return count;
	}

	size_t goodDeviceCount() const
	{
		size_t count = 0;

		for (std::list < PartInfo >::const_iterator p = panel.begin(); p != panel.end(); ++p)
			count += p->goodDeviceCount;

		return count;
	}

	size_t badDeviceCount() const
	{
		size_t count = 0;

		for (std::list < PartInfo >::const_iterator p = panel.begin(); p != panel.end(); ++p)
			count += p->badDeviceCount;

		return count;
	}

	size_t goodPanelCount() const
	{
		size_t count = 0;

		for (std::list <PartInfo>::const_iterator p = panel.begin(); p != panel.end(); ++p) {
			if (!p->badDeviceCount)
				count++;
		}

		return count;
	}

	size_t totalDeviceCount() const
	{
		size_t count = 0;

		for (std::list < PartInfo >::const_iterator p = panel.begin(); p != panel.end(); ++p)
			count += p->totalDeviceCount;

		return count;
	}

	size_t rejectDeviceCount() const
	{
		size_t count = 0;

		for (std::list < PartInfo >::const_iterator p = panel.begin(); p != panel.end(); ++p)
			count += p->rejectDeviceCount;

		return count;
	}

	std::map < std::string, size_t > codeReport() const
	{
		std::map < std::string, size_t > report;

		for (std::list < PartInfo >::const_iterator p = panel.begin(); p != panel.end(); ++p) {
			for (std::map < std::string, size_t >::const_iterator r = p->codeReport.begin(); r != p->codeReport.end(); ++r) {
				std::map < std::string, size_t >::iterator v = report.find(r->first);
				if (v != report.end())
					v->second = v->second + r->second;
				else
					report.insert(*r);
			}
		}

		return report;
	};

	std::string date;
	std::string startTime;
	std::string endTime;
	std::list < PartInfo > panel;
	std::vector <int> failUnitId;
};

enum DefectInfo
{
	NOT_INSPECTED = -2,
	UNKNOWN_DEFECT = -1,
	ERROR_PROCESS = -99	
};

class CDialogPane : public CDockablePane, public DeviceContainer
{
	DECLARE_DYNAMIC(CDialogPane)
public:CApplication* m_pApp;
	   //CAppDoc* pDoc[MAX_DOCS];

	   int nPVIGroupId;
	   int nMaxNoDisplay;
	   int nImageSets;
	   CFont m_fontList;
	   BOOL bCloseLButtonThread;
	   CString strToolTipText;
	   BOOL bActivatedToolTip;
	   BOOL *bFovSelected;
	   CPictureCtrl* m_ctrlDieImages;
	   CRgn* m_prgnDieMapPos;

	   CBrush m_brushUncheck;
	   CBrush m_brushBorder;
	   CBrush m_brushBorderSelected;
	   CScrollBar scrollBarH;
	   CScrollBar scrollBarV;
	   CComboBox comboDefectSelect;
	   CButton buttonApplytoDie;
	   CButton buttonApplytoFov;
	   CButton buttonRevert;

	   int m_nDisplayImgSet;
	   int nPosNew;
	   int nPosOld;
	   int nDeviceNo;
	   int nDieViewNo;
	   int nFOV;

	   //	HANDLE m_hLButtonDownThread;///
	   //	HANDLE m_hPicCtrlImgHistory;///
	   CPoint ptLButtonDownPoint;
	   CBrush m_pBrushTemp[3];
	   CBrush* m_pBrush;
	   BOOL bColorFailPass[MAX_DIES];
	   CPoint ptInRect;

	   //Girish 
	   CFont m_FontText;
	   CFont m_FontDieStatus;
	   CFont m_FontStatusText;

	   int nDeviceNoPerSubstrateX;
	   int nDeviceNoPerSubstrateY;
	   int nDieNoPerDeviceX;
	   int nDieNoPerDeviceY;
	   int nDieNoPerDeviceTOPX;
	   int nDieNoPerDeviceTOPY;

	   int nTotalNo;
	   int nRepeatGoodDieCount;
	   int nRepeatBadDieCount;

	   int nRepeatunitInspected[TOTAL_STATIONS];
	   int nRepeatpassedUnit[TOTAL_STATIONS];
	   int	nRepeatFailedunit[TOTAL_STATIONS];

	   //	int * pnDieState;

	/*   std::vector < int >   _deviceCode;
	   std::vector < CRect > _deviceRoi;
	   std::vector <bool> _deviceCountDone;

	   size_t _blockXCount;
	   size_t _blockYCount;
	   size_t _deviceXCount;
	   size_t _deviceYCount;

	   size_t _goodDeviceCount;
	   size_t _badDeviceCount;*/
	 //  std::vector <bool> _deviceCountDone;

	   CBrush brushCell[3];

	   CBrush brushBackground;

	   CBrush brushSelectedCell;
	   CBrush brushSelectedImage;

	   CRect * prcDie;

	   int nZoom;
	   int nPrevZoom;

	   int nDieRectSize;


	   BOOL bRedrawing;

	   int nAllowedDie;
	   int nNotAllowedDie;
	   int nSelectedDie;

	   CRITICAL_SECTION CriticalSectionData;
	   CRITICAL_SECTION CriticalSectionDraw;

	   int nWindowOffset;

	   int nScrollBarHMax;
	   int nScrollBarVMax;

	   int nHorizontalScrollBar;
	   int nVerticalScrollBar;

	   CRect rcSubstrateRelative;
	   CRect rcClientWnd;
	   int nClientHeight;
	   int nClientWidth;

	   BOOL bMouseLButtonPressed;
	   CPoint ptMouseInitPos;

	   CToolTipCtrl m_ToolTip;
	   int nMouseSelectedDie;
	   int nImageSelectedDie;

	   int leftMouseDownSelectedDie;
	   MotionParameters * pMotion;
	   MotionParameters motionParameters;
	   CRect rcMappingRelative;
	   CRect rcDieRelative;
	   CRect scrollBarHRect;
	   CRect scrollBarVRect;
	   CRgn rg23DMappingRgn;
	   CRgn dieMappingRgn;
	   CRgn rg23DMappingAndScrollBarRegion;
	   CRgn rg23DMappingScrollBarAndDieMappingRegion;
	   BOOL   isSelectingDie;
	   BOOL   isSelectingDieMouseMoved;
	   CPoint selectingStartPos;
	   CPoint selectingEndPos;
	   CRect  selectingRect;
	   int    dieStateAtSelectingStartPos;

	   LotInfo productionLotDetails;

	   // Varible using to change Image in Substrat Map by using Arrow Key
	   BOOL isSelectingMap;
	   int nCurrentDieNo;

	DECLARE_MESSAGE_MAP()
public:
	CDialogPane();
	BOOL bIsClosed;
	BOOL bHideEnable;
	virtual ~CDialogPane();
	virtual void OnPressCloseButton();
	void Initialize();
	void VerificationDefects();
	void SetSize(int nType, int cx, int cy);
	
	//Lot  Saving
	std::vector <int > getFailUnitId() const;
	void setup(const std::string & machineId, bool bContinueLot = false);
	void clearLot();
	void SetBarcodeID(CString strBarcodeID);
	void saveLotSummaryFile(const std::string & path) const;
	void savePrintBarSummaryLogFile() const;
	void setLotStartTime();
	void setLotEndTime();
	void setStripEndTime();
	void UpdateDieCount();
	std::vector<int > getDefectCode();
	void setDefectCodes(std::vector <int> _defCode );
	const LotInfo & lotInfo() const;
	int SaveMappingData();
	int SavePartSummary(bool bLotEnd = false);
	int ApplyVerificationResultsToDie(int nDie, std::string strDefectCode);
	int ApplyVerificationResultsToFov(int nDie, int nFov, std::string strDefectCode);

	void ReadLotSummayFile(const std::string & path, bool & bLotFound);					
	void AddStripDetails(const std::string & path, std::string fileName);
	void ReadStripSummary(const std::string & path);
	void compareStripId(const std::string & stripId);			//Girish

	CString GetDefectName(int nError);

	void ClearSFTMoveInLog(std::string strPath);
	void ClearSFTMoveOutLog(std::string strPath);
	void ClearSFTMoveInResults(std::string strPath);
	void SetSFTMoveInLog(std::string strPath, std::string strBarcodeId);
	void SetSFTMoveOutLog(std::string strPath, std::string strbarcodId, bool bResults);
	int GetSFTMoveInResults(std::string strPath, bool &Results);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void InitiDieControls();
	int ResetDieStates();
	int NavigateDieImages(int nKey);
	BOOL m_bCheckPass[5];
	int m_nKeyInErrorCode[5];
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg void OnToolTipNotify(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	
	void Zoom(bool bZoomIn);
	int setDieStatus(std::vector <int>  & result, std::vector <int> & dieColumn, std::vector <int> & dieRow, std::vector <int> & dieNo, std::vector <int>& dieFov, int nStationIDver, int nDocIDver, bool bOverWrite = false);
	bool SetDeviceStatus(int nDieID, int code, bool nCount);
	BOOL IsPreTrayRepeat(CString BarcodeId);
	void UpdateStatsOnRepeatScan();
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lp);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
};