#if !defined(_MYDATA_H)
#define _MYDATA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyData.h : header file
//

#include "PropertyListCtrl.h"

#define ID_PD_INSPECTION_TEACH				1
#define ID_PD_INSPECTION_INSPECT			2
#define ID_PD_INSPECTION_INSPECTCYCLE		3
#define ID_PD_INSPECTION_CAMENABLE			4
#define ID_PD_INSPECTION_INSPSEQ			5
#define ID_PD_INSPECTION_INSPSAVEIMG		6
#define ID_PD_INSPECTION_STEPSELECTION		7
#define ID_PD_INSPECTION_STEP_INSPECT		8
#define ID_PD_INSPECTION_SET_FOLDER			9
#define ID_PD_INSPECTION_SAVEPASSIMG		10
#define ID_PD_INSPECTION_SAVEFAILIMG		11
#define ID_PD_INSPECTION_INSPLOOP			12
#define ID_PD_INSPECTION_CALIBRATION		13
#define ID_PD_INSPECTION_INSPSEQSAVEIMG		14
#define ID_PD_INSPECTION_INSPSEQLOOP		15

#define ID_PD_ONLINE_OPENLOT				16
#define ID_PD_ONLINE_CLOSELOT				17
#define ID_PD_ONLINE_PRODMODE				18
#define ID_PD_ONLINE_SAVEPASSIMG			19
#define ID_PD_ONLINE_SAVEFAILIMG			20
#define ID_PD_ONLINE_BYPASSRESULT			21
#define ID_PD_ONLINE_JPEGFORMAT				22
#define ID_PD_ONLINE_BMPFORMAT				23
#define ID_PD_ONLINE_VERIFICATIONMODE	    24
#define ID_PD_ONLINE_VERIFICATIONWINDOW		25


#define ID_PD_CAMLIGHT_SNAP					26
#define ID_PD_CAMLIGHT_STREAM				27
#define ID_PD_CAMLIGHT_LIGHTSET				28
#define ID_PD_CAMLIGHT_SNAPALL				29

#define ID_PD_PARM_VISION					30
#define ID_PD_PARM_TEACH					31
#define ID_PD_PARM_DEFECT					32
#define ID_PD_PARM_MANAGERECIPE				33
#define ID_PD_PARM_CREATERECIPE				34
#define ID_PD_PARM_PVIDEFECT				35
#define ID_PD_PARM_OPT_DEVCONFG				36
#define ID_PD_PARM_OPT_TEACHCONFG			37
#define ID_PD_PARM_OPT_DEFECTCONFG			38

#define ID_PD_TOOLS_LOADIMG					39
#define ID_PD_TOOLS_LOADIMG_ALL				40
#define ID_PD_TOOLS_SAVEIMG					41
#define ID_PD_TOOLS_SAVEIMG_ALL				42
#define ID_PD_TOOLS_LOADTEACHIMG			43
#define ID_PD_TOOLS_LOADTEACHIMG_ALL		44
#define ID_PD_TOOLS_RESETSTATS				45
#define ID_PD_TOOLS_SHOWDIECONFIG			46
#define ID_PD_TOOLS_SHOWSTATS				47
#define ID_PD_TOOLS_DEFAULTSETTINGS			48
#define ID_PD_TOOLS_PIXELRULER				49
#define ID_PD_TOOLS_MOTION_CONTROL			50
#define ID_PD_TOOLS_OPTICS_PANEL			51
#define ID_PD_TOOLS_HW_CAMERA				52
#define ID_PD_TOOLS_HW_IO					53
#define ID_PD_TOOLS_DRYRUN					54
#define ID_PD_TOOLS_OUTPUT_LOG				55
#define ID_PD_TOOLS_MOTOR_TRIGGER			56
#define ID_PD_TOOLS_TRIANGULAR_SENSOR		57
#define ID_PD_TOOLS_BARCODE_READER			58


#define ID_PD_VIEW_BINARIZE					59
#define ID_PD_VIEW_ZOOM_IN					60
#define ID_PD_VIEW_ZOOM_OUT					61
#define ID_PD_VIEW_ZOOM_FIT					62
#define ID_PD_VIEW_ZOOM_100					63
#define ID_PD_VIEW_ZOOM_ALL					64
#define ID_PD_VIEW_SETUGRID					65
#define ID_PD_VIEW_SHOW_OVERLAY				66
#define ID_PD_VIEW_AUDIT_TRAIL				67
#define ID_PD_VIEW_SHOW_MAGINIFIER			68


#define ID_PD_SEQUENCE_RUN					69
#define	ID_PD_SEQUENCE_NESTREPLACEMENT		70
#define	ID_PD_SEQUENCE_BARCODE				71
#define	ID_PD_SEQUENCE_CLAMP				72
#define	ID_PD_SEQUENCE_UNCLAMP				73
#define	ID_PD_SEQUENCE_OPTIONS				74
#define ID_PD_DUMMY							76





class CMyStaticPropertyItemManger;
class CMyAdaptablePropertyItemManager;
class CAdvSecurityDlg;

// Only as an example
/////////////////////////////////////////////////////////////////////////////
// CMyData

class CMyData
{
	CMyData(const CMyData& d);
	CMyData& operator=(const CMyData& d);

public:
	void ResetDefaults();
	CMyData();
	~CMyData();

	// Data


	int m_nOpenLot;
	int m_nCloseLot;
	int m_nProdMode;
	int m_nSaveOnlinePassImages;
	int m_nSaveOnlineFailImages;
	int m_nSaveOnlineJpegImages;
	int m_nSaveOnlineBMPImages;
	int m_nByPassResult;
	int m_nVerificationMode;
	int m_nVerificationWindow;

	int m_nTeach;
	int m_nStepSelection;
	int m_nStepDebug;
	int m_nInspect;
	int m_nInspectSavedImage;
	int m_nSetFolder;
	int m_nSaveOfflinePassImages;
	int m_nSaveOfflineFailImages;
	int m_nInspectLoop;
	int m_nInspectCycle;
	int m_nCamEnable;
	int m_nInspectionSeq;
	int m_nCalibSettings;
	int m_nInspSeqLoop;
	int m_nSaveInspSeqImages;
		

	int m_nGrab;
	int m_nLive;
	int m_nGrabAll;
	int m_nLightSet;
	
	int m_nVisionParm;
	int m_nTeachParm;
	int m_nDefectParm;
	int m_nManageCfg;
	int m_nCreateConfig;
	int m_nPviDefectParm;
	int m_nCameraCfg;
	int m_nIOTest;
	int m_nDeviceCfg;
	int m_nTeachInspLoc;
	int m_nDefectPriority;

	int m_nLoadImg;
	int m_nSaveImg;
	int m_nLoadTeachImg;
	int m_nLoadImgAll;
	int m_nSaveImgAll;
	int m_nLoadTeachImgAll;
	int m_nResetStatCounters;
	int m_nShowDieCfg;
	int m_nShowStats;
	int m_nDefaultSettings;
	int m_nPixelRuler;
	int m_nMotionControl;
	int m_nOpticalPanel;
	int m_nDryRun;
	int m_nOutPutLog;
	int m_nMotorTrigger;
	int m_nTriangularSensor;
	int m_nBarcodeReader;
	

	int m_nBinarize;
	int m_nZoomIn;
	int m_nZoomOut;
	int m_nZoomFit;
	int m_nZoom100;
	int m_nZoomAll;
	int m_nSetupGrid;
	int m_nShowOverLay;
	int m_nAuditTrail;
	int m_nShowMaginifier;


	int m_nSequenceRun;
	int m_nNestReplaceMent;
	int m_nBarCode;
	int m_nClamp;
	int m_nUnClamp;
	int m_nScanningOptions;

private:
	friend CMyStaticPropertyItemManger;
	friend CMyAdaptablePropertyItemManager;
	friend CAdvSecurityDlg;
};

inline CMyData::~CMyData()
{
}


/////////////////////////////////////////////////////////////////////////////
// CMyStaticPropertyItemManger

class CMyStaticPropertyItemManger : public CPropertyItemManger
{
	CMyStaticPropertyItemManger(const CMyStaticPropertyItemManger& d);
	CMyStaticPropertyItemManger& operator=(const CMyStaticPropertyItemManger& d);

public:
	CMyStaticPropertyItemManger();
	~CMyStaticPropertyItemManger();

	void SetData(const CMyData& data);
	void GetData(CMyData& data) const;
};


/////////////////////////////////////////////////////////////////////////////
// CMyAdaptablePropertyItemManager

class CMyAdaptablePropertyItemManager : public CPropertyItemManagerAdaptable
{
	CMyAdaptablePropertyItemManager(const CMyAdaptablePropertyItemManager& d);
	CMyAdaptablePropertyItemManager& operator=(const CMyAdaptablePropertyItemManager& d);

public:
	CMyAdaptablePropertyItemManager();
	~CMyAdaptablePropertyItemManager();

	void SetData(const CMyData& data);
	void GetData(CMyData& data) const;

	void OnDataChanged(CPropertyItem* pPropertyItem, CPropertyListCtrl* pWndPropertyListCtrl, int nIndex);

};

#endif // !defined(AFX_PROPERTYLISTCTRLDLG_H__466FB4E7_6072_11D3_A7E3_006008C8B630__INCLUDED_)