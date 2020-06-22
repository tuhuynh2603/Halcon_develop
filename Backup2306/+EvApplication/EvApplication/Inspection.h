/// CInspection: Blister Inspection class declaration
/// Udupa; Jan'2015

#pragma once

#include "InspectionCore.h"
#include "Imaging.h"
//#include "DotCalibration.h"
class CDotCalibration;


struct CSharedData
{
public:
	CSharedData();
	void Initialize(int nIndex, CImageBuffer* pBuffer, IMAGEVIEW_INFO* pView, CDotCalibration* pCalib, int nTrackNo);
	void Initialize(CSharedData* pSharedData);

	CImageBuffer* pProcBuffers[/*MAX_BUFFERS*/5];
	CImageBuffer* pProcDispBuffers[/*MAX_BUFFERS*/5];
	IMAGEVIEW_INFO* pImageViews[/*MAX_BUFFERS*/5];
	CDotCalibration* pCalibrations[/*MAX_BUFFERS*/5];
	int nSize;
	int nTracks[/*MAX_BUFFERS*/5];
	HRegion hLocationRegion[8][5];
};

struct CDefectCodes
{
	CDefectCodes() {
		m_nID = -1;
		strDefectName = "";
		nErrorCode = 98765;
		colorDefect = RGB(255, 255, 255);
		brush.CreateSolidBrush(colorDefect);

	};

	CDefectCodes(int id, CString defectName, int errorCode, COLORREF defectColor)
	{
		m_nID = id;
		strDefectName = defectName;
		nErrorCode = errorCode;
		colorDefect = defectColor;
		brush.CreateSolidBrush(colorDefect);
	};

	CDefectCodes(const CDefectCodes &obj)
	{
		m_nID = obj.m_nID;
		strDefectName = obj.strDefectName;
		nErrorCode = obj.nErrorCode;
		colorDefect = obj.colorDefect;
		brush.CreateSolidBrush(colorDefect);
	};

	int m_nID;
	CString strDefectName;
	int nErrorCode;
	COLORREF colorDefect;
	CBrush brush;
};



int DrawRegion(IMAGEVIEW_INFO *pView, COverlayData& overlay, CPoint* pptOffset=NULL);
int DrawRegion(IMAGEVIEW_INFO *pView, HObject hRegion, CPoint* pptOffset=NULL, COLORREF color=RGB(0, 0, 255));

class CInspection : public CInspectionCore
{
public:
	CInspection();
	~CInspection();

	void Initialize(int nTrack, BOOL bParallel);
	int GenRegionPoints(HObject* pRegion, CArray<CPoint2D>& arrayPoints);
	int Teach(CSharedData* pData,//IMAGEVIEW_INFO *pView,
			  CImageBuffer* pImgBufSrc,
			  CString strTeachFolder,
			  int nTeachStep,
			  BOOL *bEnable,
			  BOOL bAutoTeach,
			  int nCurTrack,
			  int nDoc,
			  int nFOV,
			  int nTotalFOV,
			  CTrainingData* pTrainingData,
			  CCalibrationData* pCalibData,
			  BOOL bEncapLoc,
			  CRect& rectEncapLoc);

	int Inspect(CSharedData* pData,
				CImageBuffer* pImgBufSrc,
				CImageBuffer* pTeachImgBufSrc,
				long lDebugFlag, 
				BOOL bRegionInsp,
				int nCurTrack,
				int nCurDoc,
				int nTotalDoc,
				int nCurFOV,
				int nTotalFOV,
				CTrainingData* pTrainingData,
				CInspectinonData* pInspData,
				CCalibrationData* pCalibData,
				CStringArray& strArrayInspValues);

	int AutoTeach(CSharedData* pData,//IMAGEVIEW_INFO *pView,
			  CImageBuffer* pImgBufSrc,
			  CString strTeachFolder,
			  BOOL bRegionTeach,
		      int nCurTrack,
			  int nDoc,
			  int nFOV,
			  int nTotalFOV,
			  CCalibrationData* pCalibData);

	int DebugStep(IMAGEVIEW_INFO *pView, CImageBuffer* pImgBufSrc, int nIndex,int nRegion);
	int ClearTeachParam(CTrainingData* pTrainingData);
	int ClearInspParam(CInspectinonData* pInspData, BOOL bInspClearAll);
	int ClearCalibParam(CCalibrationData* pCalibData, BOOL bClearAvg, BOOL bClearInsp);
	int DisplayPVIDefectImage(IMAGEVIEW_INFO *pView, CImageBuffer* pImgBufSrc, int nArea);
	int GetCalibDataForInsp(CCalibrationData* pCalibData);
	int ClearAllNccModels();

	HRegion m_regionLocationSearch;

	//CArray<CRect> AryrcDefect;
	BOOL bInspectAll;
	BOOL bIsPattern;

	CRectArray m_arrDefectRects;
	CRegionArray m_arrDefectRegions;

	void CleanDefectRegions();
	int GetDefectRegions(IMAGEVIEW_INFO *pView, int nResult, CVisionResult& pResult, int& nSize);
	int SetDefectRegions(IMAGEVIEW_INFO *pView, CVisionResult& result);
	int ZoomImage(CImageBuffer* bufSrc, CImageBuffer* bufDst,int nDoc,BOOL bDispBufImgResize=FALSE);
	int LightCalibrationMode(CImageBuffer* pBuffer, int nGreyValue, 
							CImageBuffer* pBufferCalibPos, CImageBuffer* pBufferCalibNeg,
							BOOL bFirst, BOOL bRefImg, BOOL bAvgImg, BOOL bLightCorrection,
							CCalibrationData* pCalibData);
	int DotCalibrationMode(CSharedData* pData,CImageBuffer* pBuffer,int nTrack,int nDoc);
	HImage hImageTeach;

protected:
	HRegion m_regionROI, m_regionOuter, m_regionInner;
	HTuple m_modelShell, m_rowTeach, m_colTeach;
		
	HRegion m_teachMaskObjects;
	HTuple m_modelID, m_modelRows, m_modelCols;
	//BOOL bIsPattern;
	
};
