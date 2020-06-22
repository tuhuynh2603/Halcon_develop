/// CInspection: Blister Inspection class
/// Udupa; Shell Teach/Location; Jan'2015

#include "StdAfx.h"
#include "Inspection.h"
#include "ExternalFunctions.h"
#include "AppDef.h"

#include "StdAfx.h"
#include "Inspection.h"
#include "ExternalFunctions.h"
#include "AppDef.h"


CSharedData::CSharedData()
{
	for(int i=0; i<MAX_BUFFERS; i++) {
		pProcBuffers[i] = NULL;
		pImageViews[i] = NULL;
		pCalibrations[i] = NULL;
	}
}


void CSharedData::Initialize(int nIndex, CImageBuffer* pBuffer,IMAGEVIEW_INFO* pView, CDotCalibration* pCalib, int nTrackNo)
{
	if(nIndex >= MAX_BUFFERS)
		return;

	nTracks[nIndex] = nTrackNo;
	pProcBuffers[nIndex] = pBuffer;
	pImageViews[nIndex] = pView;
	pCalibrations[nIndex] = pCalib;
	nSize = nIndex+1;
}


void CSharedData::Initialize(CSharedData* pSharedData)
{
	int i;
	nSize = pSharedData->nSize;
	for(i=0;i<MAX_BUFFERS;i++) {
		nTracks[i] = pSharedData->nTracks[i];
		pProcBuffers[i] = pSharedData->pProcBuffers[i];
		pProcDispBuffers[i] = pSharedData->pProcDispBuffers[i];
		pImageViews[i] = pSharedData->pImageViews[i];
		pCalibrations[i] = pSharedData->pCalibrations[i];
	}
}

CInspection::CInspection()
{
	bInspectAll = FALSE;
	//bIsPattern = FALSE;
}

CInspection::~CInspection()
{
}

void CInspection::Initialize(int nTrack, BOOL bParallel)
{
	if(nTrack==0) {
		try {
			HTuple hSystem;
			GetSystem("temporary_mem_cache", &hSystem);
			if(hSystem == "true") {
				//SetSystem("temporary_mem_cache", "false");
			}
			else {
				SetSystem("temporary_mem_cache", "true");
			}

			GetSystem("reentrant", &hSystem);
			if(hSystem == "false") {
				SetSystem("reentrant", "true");
			}
			else {
				//SetSystem("reentrant", "false");
			}

			GetSystem("parallelize_operators", &hSystem);
			if(hSystem == "true") {
				SetSystem("parallelize_operators", "false");
			}
			else {
				//SetSystem("parallelize_operators", "true");
			}
			
			GetSystem("thread_pool", &hSystem);
			if(hSystem == "false") {
				SetSystem("thread_pool", "true ");
			}
			else {
				//SetSystem("thread_pool", "false ");
			}

			GetSystem("image_cache_capacity", &hSystem);
			if(hSystem != 4194304) {
				SetSystem("image_cache_capacity", 4194304);
			}
			else {
				//SetSystem("image_cache_capacity", 0);
			}

			GetSystem("global_mem_cache", &hSystem);
			if(hSystem != "exclusive") {
				SetSystem("global_mem_cache", "exclusive");
			}
			else {
				//SetSystem("global_mem_cache", "idle");
			}

			GetSystem("alloctmp_max_blocksize", &hSystem);
			if(hSystem != -1) {
				SetSystem("alloctmp_max_blocksize", -1);
			}
			else {
				//SetSystem("alloctmp_max_blocksize", 2);
			}
		}
		catch(HOperatorException ex) {
			CString str;
			str.Format("[Inspection::Initialize] %s", ex.ErrorMessage().Text());
			LogMessage(9, str);
			AfxMessageBox("Unregistered IPIe-H Inspection module");
		}
		catch(...) {}
	}
		
	CInspectionCore::Initialize(bParallel);
}

void CInspection::CleanDefectRegions()
{
	m_arrDefectRegions.RemoveAll();
	m_arrDefectRects.RemoveAll();
}

/// Udupa; Dec'2013
int CInspection::GetDefectRegions(IMAGEVIEW_INFO *pView, int nResult, CVisionResult& result, int& nSize)
{
	result.nCursors = 0;
	if(nResult) {
		result.nDefectRects = (int) m_arrDefectRects.GetSize();
		result.nDefectRegions = (int) m_arrDefectRegions.GetSize();
	}
	nSize = result.Create(NULL, nResult);

	for(int i=0; i<m_arrDefectRects.GetSize(); i++)
		result.pDefectRects[i] = m_arrDefectRects[i];
	for(int i=0; i<m_arrDefectRegions.GetSize(); i++)
		result.pDefectRegions[i] = m_arrDefectRegions[i];

	return 0;
}

/// Udupa; Dec'2013
int CInspection::SetDefectRegions(IMAGEVIEW_INFO *pView, CVisionResult& result)
{
	LOGPEN RedPen;
	RedPen.lopnColor = RGB(255,0,0);

	m_arrDefectRects.RemoveAll();
	m_arrDefectRegions.RemoveAll();
	int nError = result.nResult;

	if(nError) {
		//if(nError==EDGE_GAP || nError==EDGE_TEAR) {
		//	for(int i=0; i<result.nDefectRegions; i++) {
		//		m_arrDefectRegions.Add(result.pDefectRegions[i]);
		//		m_arrDefectRegions[i].Draw(pView, &RedPen);
		//	}
		//}
		//else {
		//	if(nError==INLENS_LENT_DEFECT || nError==INLENS_BLACK_DEFECT || nError==INLENS_WHITE_DEFECT || nError==FOREIGN_OBJ) {
		//		for(int i=0; i<result.nDefectRects; i++) {
		//			m_arrDefectRects.Add(result.pDefectRects[i]);
		//			DrwRect(pView, &m_arrDefectRects[i], &RedPen);
		//		}
		//	}
		//}
	}

	return nError;
}

/// Defect regions from all inspections are supposed to be referenced in this function
/// Udupa; Dec'2013
//int CInspection::GetDefectArrays(int nResult, 
//	CArray<CRectArray*, CRectArray*>& rects, 
//	CArray<CRegionArray*, CRegionArray*>& regions,
//	CArray<CPoint2D, CPoint2D>& cursors)
//{
//	if(nResult) {
//		if(m_arrDefectRegions.GetSize() > 0)
//			regions.Add(&m_arrDefectRegions);
//		else if(m_arrDefectRects.GetSize() > 0)
//			rects.Add(&m_arrDefectRects);
//		//if(nResult==INSP_FAIL_LENS_ARC || nResult==INSP_FAIL_LENS_SCRATCH) {
//		//	if(pLensNonCircular->CBlockArcDefectArray.GetSize() > 0)
//		//		regions.Add(&pLensNonCircular->CBlockArcDefectArray);
//		//}
//		//else if(nResult==INSP_FAIL_LENS_GAP) {
//		//	if(pLensGap->CBlockArcDefectArray.GetSize() > 0)
//		//		regions.Add(&pLensGap->CBlockArcDefectArray);
//		//}
//		//else if(nResult==INSP_FAIL_LENS_SCRATCH) {
//		//	for(int i=0; i<6; i++) {
//		//		if(LensTear[i].CBlockArcDefectArray.GetSize() > 0)
//		//			regions.Add(&LensTear[i].CBlockArcDefectArray);
//		//	}
//		//}
//		//else if(nResult==INSP_FAIL_INLENS_SCRATCH) {
//		//	if(LensPattern.AryrcDefect.GetSize() > 0)
//		//		rects.Add(&LensPattern.AryrcDefect);
//		//}
//	}
//
//	return 0;
//}

int CInspection::GenRegionPoints(HObject* pRegion, CArray<CPoint2D>& arrayPoints)
{
	HTuple rows, cols;
	try {
		GetRegionContour(*pRegion, &rows, &cols);
	}
	catch(...) {
		return -1;
	}

	int nLength = (int) rows.Length();
	int nStep = nLength / 360;
	if(nStep < 1)
		nStep = 1;
	arrayPoints.RemoveAll();
	float dRow, dCol;
	for(int i=0; i<nLength; i+=nStep) {
		dCol = (float) cols[i];
		dRow = (float) rows[i];
		arrayPoints.Add(CPoint2D(dCol, dRow));
	}

	return 0;
}

int DrawRegion(IMAGEVIEW_INFO *pView, COverlayData& overlay, CPoint* pptOffset)
{
	return DrawRegion(pView, overlay.region, pptOffset, overlay.color);
}

int DrawRegion(IMAGEVIEW_INFO *pView, HObject hRegion, CPoint* pptOffset, COLORREF color)
{
	try {
		int nCount = (int) hRegion.CountObj();
		if(!nCount)
			return -1;

		HTuple rows, cols;
		int nLength;
		double dStep, dIndex;
		LOGPEN PenInfo;
		PenInfo.lopnColor = color;
		PenInfo.lopnStyle = PS_SOLID;
		PenInfo.lopnWidth.x = 4;

		int nOffsetX=0, nOffsetY=0;
		if(pptOffset) {
			nOffsetX = pptOffset->x;
			nOffsetY = pptOffset->y;
		}

		HRegion hRegionConn;
		Connection(hRegion, &hRegionConn);
		nCount = (int)hRegionConn.CountObj();
		for(int i=1; i<=nCount; i++) {
			HRegion hSelectedObj;
			SelectObj(hRegionConn, &hSelectedObj, i);
			GetRegionContour(hSelectedObj, &rows, &cols);
			nLength = (int) rows.Length();
			if(nLength < 2)
				continue;

			dStep = nLength / 360.0;
			if(dStep < 1)
				dStep = 1;

			dIndex;
			for(dIndex=dStep; dIndex<nLength; dIndex+=dStep) {
				DrwLine(pView, &CPoint(cols[(int) (dIndex-dStep)]+nOffsetX, rows[(int) (dIndex-dStep)]+nOffsetY), 
					&CPoint((int) cols[(int) dIndex]+nOffsetX, (int) rows[(int) dIndex]+nOffsetY), &PenInfo);
			}
			DrwLine(pView, &CPoint(cols[(int) (dIndex-dStep)]+nOffsetX, rows[(int) (dIndex-dStep)]+nOffsetY), 
				&CPoint(cols[0]+nOffsetX, rows[0]+nOffsetY), &PenInfo);
		}
	}
	catch(...) {	
		return -1;
	}

	return 0;
}

int CInspection::Teach(CSharedData* pData,//IMAGEVIEW_INFO *pView,
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
					   CRect& rectEncapLoc)
{
	int nError;
	try{
		CImageBuffer** pBufs = pData->pProcBuffers;
		CRect rcFOV = CRect(CPoint(0,0), pImgBufSrc->GetImgSize());
		CSize szImage = rcFOV.Size();
		LOGPEN PenInfoDark, PenInfoBright, PenInfo;
		PenInfoDark.lopnColor = RGB(51, 153, 255);
		PenInfoDark.lopnStyle = PS_SOLID;
		PenInfoDark.lopnWidth.x = 1;

		PenInfoBright.lopnColor = RGB(255, 127, 0);
		PenInfoBright.lopnStyle = PS_SOLID;
		PenInfoBright.lopnWidth.x = 1;
	
		PenInfo.lopnColor = RGB(0, 50, 255);
		PenInfo.lopnStyle = PS_SOLID;
		PenInfo.lopnWidth.x = 1;

		hImageTeach.GenImage1("byte", pImgBufSrc->GetPitch(), szImage.cy, pImgBufSrc->GetImgPtr());

		nError = CInspectionCore::Teach(hImageTeach, 
										nTeachStep, bEnable, bAutoTeach,
										nCurTrack, nDoc, nFOV, nTotalFOV,
										pTrainingData, pCalibData, bEncapLoc, rectEncapLoc);
	
		DrwDel(pData->pImageViews[nDoc]);
		/*if (bEncapLoc) {
			int nLast = m_arrayOverlayTeach.GetCount();
			if(!m_arrayOverlayTeach[nLast-1].hImage.IsInitialized())
				DrawRegion(pData->pImageViews[nDoc], m_arrayOverlayTeach[nLast - 1]);
		}
		else {*/
			for (int j = 0; j < m_arrayOverlayTeach.GetCount(); j++)
				if (!m_arrayOverlayTeach[j].hImage.IsInitialized())
					DrawRegion(pData->pImageViews[nDoc], m_arrayOverlayTeach[j]);
		//}
		DrwUpdate(pData->pImageViews[nDoc]);
	}
	catch(HOperatorException ex) {
 			CString str;
			str.Format("[CInspection::Teach] %s", ex.ErrorMessage().Text());
			OutputDebugLogTo(9, TRUE, "%s", str);
			return -1;
	}

	return nError;
}

int CInspection::AutoTeach(CSharedData* pData,//IMAGEVIEW_INFO *pView,
					   CImageBuffer* pImgBufSrc,
					   CString strTeachFolder,
					   BOOL bRegionTeach,
					   int nCurTrack,
					   int nDoc,
					   int nFOV,
					   int nTotalFOV,
					   CCalibrationData* pCalibData)
{
	int nError;
	try{
		CImageBuffer** pBufs = pData->pProcBuffers;
		CRect rcFOV = CRect(CPoint(0,0), pImgBufSrc->GetImgSize());
		CSize szImage = rcFOV.Size();

		hImageTeach.GenImage1("byte", pImgBufSrc->GetPitch(), szImage.cy, pImgBufSrc->GetImgPtr());

		nError = CInspectionCore::AutoTeach(hImageTeach, bRegionTeach, nCurTrack, nDoc, nFOV, nTotalFOV, pCalibData);
	}
	catch(HOperatorException ex) {
			CString str;
			str.Format("[CInspection::AutoTeach] %s", ex.ErrorMessage().Text());
			OutputDebugLogTo(9, TRUE, "%s", str);
			return -1;
	}

	return nError;
}

int CInspection::ClearTeachParam(CTrainingData* pTrainingData)
{
	int nError;
	try{
		nError = CInspectionCore::ClearTeachParam(pTrainingData);
	}
	catch(HOperatorException ex) {
			CString str;
			str.Format("[CInspection::ClearTeachParam] %s", ex.ErrorMessage().Text());
			OutputDebugLogTo(9, TRUE, "%s", str);
			return -1;
	}

	return nError;
}

int CInspection::ClearInspParam(CInspectinonData* pInspData, BOOL bInspClearAll)
{
	int nError;
	try{
		nError = CInspectionCore::ClearInspParam(pInspData, bInspClearAll);
	}
	catch(HOperatorException ex) {
			CString str;
			str.Format("[CInspection::ClearInspParam] %s", ex.ErrorMessage().Text());
			OutputDebugLogTo(9, TRUE, "%s", str);
			return -1;
	}

	return nError;
}

int CInspection::ClearAllNccModels()
{
	int nError=0;
	try {
		int nLength = m_TrainingData.hvec_TupleVectorLocation[16].Length();
		for (int i = 0; i < nLength; i++)
			ClearNccModel(m_TrainingData.hvec_TupleVectorLocation[16][i]);
		m_TrainingData.hvec_TupleVectorLocation[16].Clear();
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[CInspection::ClearInspParam] %s", ex.ErrorMessage().Text());
		OutputDebugLogTo(9, TRUE, "%s", str);
		return -1;
	}

	return nError;
}


int CInspection::ClearCalibParam(CCalibrationData* pCalibData, BOOL bClearAvg, BOOL bClearInsp)
{
	int nError;
	try {
		nError = CInspectionCore::ClearCalibParam(pCalibData, bClearAvg, bClearInsp);
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[CInspection::ClearCalibParam] %s", ex.ErrorMessage().Text());
		OutputDebugLogTo(9, TRUE, "%s", str);
		return -1;
	}

	return nError;
}

int CInspection::GetCalibDataForInsp(CCalibrationData* pCalibData)
{
	int nError;
	try {
		nError = CInspectionCore::GetCalibDataForInsp(pCalibData);
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[CInspection::GetCalibDataForInsp] %s", ex.ErrorMessage().Text());
		OutputDebugLogTo(9, TRUE, "%s", str);
		return -1;
	}

	return nError;
}

int CInspection::Inspect(CSharedData* pData,
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
						CStringArray& strArrayInspValues)
{
	LOGPEN PenInfo;
	PenInfo.lopnColor = RGB(0, 50, 255);
	PenInfo.lopnStyle = PS_SOLID;
	PenInfo.lopnWidth.x = 1;
	//CImageBuffer** pBufs = pData->pProcBuffers;
	//CRect rcFOV = CRect(CPoint(0,0), pBufs[0]->GetImgSize());
	CRect rcFOV = CRect(CPoint(0,0), pImgBufSrc->GetImgSize());
	CSize szImage = rcFOV.Size();
	CRect rcTeachFOV = CRect(CPoint(0,0), pTeachImgBufSrc->GetImgSize());
	CSize szTeachImage = rcTeachFOV.Size();

	int nError = -DEVICE_LOCATION;

	HImage hImage, hTeachImage;
	hImage.GenImage1("byte", pImgBufSrc->GetPitch(), szImage.cy, pImgBufSrc->GetImgPtr());
	if(szTeachImage.cx > 0 && szTeachImage.cy > 0)
		hTeachImage.GenImage1("byte", pTeachImgBufSrc->GetPitch(), szTeachImage.cy, pTeachImgBufSrc->GetImgPtr());
	
	try{
		nError = CInspectionCore::Inspect(hImage, hTeachImage, lDebugFlag, bRegionInsp, 
										nCurTrack, nCurDoc, nTotalDoc, nCurFOV, nTotalFOV,
										pTrainingData, pInspData, pCalibData, 
										strArrayInspValues);
		
		/*DrwDel(pData->pImageViews[nCurDoc]);
		for(int j=0; j<m_arrayOverlayInspection.GetCount(); j++)
			if(!m_arrayOverlayInspection[j].hImage.IsInitialized())
				DrawRegion(pData->pImageViews[nCurDoc], m_arrayOverlayInspection[j]);		
		DrwUpdate(pData->pImageViews[nCurDoc]);*/
	}
	catch(HOperatorException ex) {
			CString str;
			str.Format("[CInspection::Inspect] %s", ex.ErrorMessage().Text());
			OutputDebugLogTo(9, TRUE, "%s", str);
			return -PROCESS_ERROR;
	}
	//for(int i=0; i</*MAX_BUFFERS*/nDocCount; i++)
	//	hImages[i].Clear();

	ReleaseMemory();

	return nError;
}

int CInspection::DebugStep(IMAGEVIEW_INFO *pView, CImageBuffer* pImgBufSrc, int nIndex,int nRegion)
{
	DrwDel(pView);
	COverlayData* pData = &m_arrayOverlay[nRegion].GetAt(nIndex);
	CString strMessageBox = pData->strDebugMessage;
	HImage* pImage = &pData->hImage;
	if(pImage->IsInitialized()) {
		HString strType;
		Hlong nWidth, nHeight;
		BYTE* pBuf = (BYTE*) pImage->GetImagePointer1(&strType, &nWidth, &nHeight);
		if(pBuf) {
			CSize szSrc = CSize((int) nWidth, (int) nHeight);
			CRect rectSrc = CRect(CPoint(0,0), szSrc);
			CImageBuffer buf;
			buf.AllocImgIndirect(szSrc, pBuf);

			CSize szDst = pImgBufSrc->GetImgSize();
			CRect rectDst = CRect(CPoint(0,0), szDst);
			CRect rect;
			rect.IntersectRect(rectSrc, rectDst);
			ImgCopy(&buf, &rect, pImgBufSrc, &rect);
		}
	}
	DrawRegion(pView, *pData);
	UpdateStatusBar(&strMessageBox);
	DrwUpdate(pView);

	return 0;
}


int CInspection::ZoomImage(CImageBuffer* bufSrc, CImageBuffer* bufDst,int nDoc,BOOL bDispBufImgResize)
{
	try{
	CRect rcZoomImgFOV = CRect(CPoint(0,0), bufSrc->GetImgSize());
	CSize szZoomImg = rcZoomImgFOV.Size();
	HImage hZoomImgSrc, hZoomImgDst;
	int nWidth, nHeight;

	nWidth = bDispBufImgResize==TRUE ? bufDst->GetImgSize().cx : 500;
	nHeight = bDispBufImgResize==TRUE ? bufDst->GetImgSize().cy : 500;

	hZoomImgSrc.GenImage1("byte", bufSrc->GetPitch(), szZoomImg.cy, bufSrc->GetImgPtr());
	////hZoomImgDst.GenImage1("byte", bufDst->GetPitch(), szZoomImg.cy/dZoomRatio, bufDst->GetImgPtr());
	
	CInspectionCore::ZoomImage(hZoomImgSrc, hZoomImgDst, nWidth, nHeight,nDoc);	
	//CInspectionCore::ZoomImage(hZoomImgSrc, hZoomImgDst, szZoomImg.cy/20, szZoomImg.cy/20);
	
	}catch(...){
		LogMessage(9, "[Inspection::ZoomImage]");
	}
	return 0;
}


//int CInspection::LightCalibrationMode(CSharedData* pData,CImageBuffer* pBuffer,int nTrack,int nDoc,int nGreyValue,
//										int* nCalibRange,double* dCalibDeviation,BOOL bIsFirstLightCalibration)
//{
//	 int nError = PROCESS_ERROR;
//	 CRect rcFOV = CRect(CPoint(0,0), pBuffer->GetImgSize()); 
//	 CSize szImage = rcFOV.Size();
//
//	 try {
//	  HImage hImage;
//	  HTuple hFound;
//	  HTuple hGreyValue;
//	 /* if(nTrack ==2 && bIsFirstLightCalibration){
//		  DrwTrackRect(pData->pImageViews[nDoc], &m_CalibrationData.rectCalibSideTrack);
//		  DrwUpdate(pData->pImageViews[nDoc]);
//		  if(!WaitResponse(pData->pImageViews[nDoc]))
//			m_CalibrationData.rectCalibSideTrack = GetTrackRect(pData->pImageViews[nDoc],0);
//	  }*/
//	  hImage.GenImage1("byte", rcFOV.Width(), rcFOV.Height(), pBuffer->GetImgPtr());
//	 
//	  nError = CInspectionCore::LightCalibrationMode(hImage,nTrack,nGreyValue,nCalibRange,dCalibDeviation,m_CalibrationData.rectCalibSideTrack);
//
//	 }
//	 catch(HOperatorException ex) {
//	  CString str;
//	  str.Format("[Inspection::LightCalibrationMode] %s", ex.ErrorMessage().Text());
//	  LogMessage(9, str);
//	  return PROCESS_ERROR;
//	 }
//	 catch(...) {
//	  LogMessage(9, "[Inspection::LightCalibrationMode] Error");
//	  return PROCESS_ERROR;
//	 }
//	 return nError;
//}

int CInspection::LightCalibrationMode(CImageBuffer* pBuffer, int nGreyValue, 
										CImageBuffer* pBufferCalibPos, CImageBuffer* pBufferCalibNeg, 
										BOOL bFirst, BOOL bRefImg, BOOL bAvgImg, BOOL bLightCorrection,
										CCalibrationData* pCalibData)
{
	int nError;
	try {
		HImage hImage, hImageCalibPos, hImageCalibNeg, hImageCalibResult;
		CRect rcFOV = CRect(CPoint(0, 0), pBuffer->GetImgSize());
		CSize szImage = rcFOV.Size();

		hImage.GenImage1("byte", rcFOV.Width(), rcFOV.Height(), pBuffer->GetImgPtr());

		nError = CInspectionCore::LightCalibrationMode(hImage, nGreyValue, 
														&hImageCalibPos, &hImageCalibNeg, &hImageCalibResult,
														bFirst, bRefImg, bAvgImg, bLightCorrection,
														pCalibData);

		if (bLightCorrection) {
			if (hImageCalibPos.IsInitialized()) {
				HString strType;
				Hlong nWidth, nHeight;
				BYTE* pBuf = (BYTE*)hImageCalibPos.GetImagePointer1(&strType, &nWidth, &nHeight);
				if (pBuf) {
					CSize szSrc = CSize((int)nWidth, (int)nHeight);
					CRect rectSrc = CRect(CPoint(0, 0), szSrc);
					CImageBuffer buf;
					buf.AllocImgIndirect(szSrc, pBuf);

					CSize szDst = pBufferCalibPos->GetImgSize();
					CRect rectDst = CRect(CPoint(0, 0), szDst);
					CRect rect;
					rect.IntersectRect(rectSrc, rectDst);
					ImgCopy(&buf, &rect, pBufferCalibPos, &rect);
				}
			}
			if (hImageCalibNeg.IsInitialized()) {
				HString strType;
				Hlong nWidth, nHeight;
				BYTE* pBuf = (BYTE*)hImageCalibNeg.GetImagePointer1(&strType, &nWidth, &nHeight);
				if (pBuf) {
					CSize szSrc = CSize((int)nWidth, (int)nHeight);
					CRect rectSrc = CRect(CPoint(0, 0), szSrc);
					CImageBuffer buf;
					buf.AllocImgIndirect(szSrc, pBuf);

					CSize szDst = pBufferCalibNeg->GetImgSize();
					CRect rectDst = CRect(CPoint(0, 0), szDst);
					CRect rect;
					rect.IntersectRect(rectSrc, rectDst);
					ImgCopy(&buf, &rect, pBufferCalibNeg, &rect);
				}
			}
			if (hImageCalibResult.IsInitialized()) {
				HString strType;
				Hlong nWidth, nHeight;
				BYTE* pBuf = (BYTE*)hImageCalibResult.GetImagePointer1(&strType, &nWidth, &nHeight);
				if (pBuf) {
					CSize szSrc = CSize((int)nWidth, (int)nHeight);
					CRect rectSrc = CRect(CPoint(0, 0), szSrc);
					CImageBuffer buf;
					buf.AllocImgIndirect(szSrc, pBuf);

					CSize szDst = pBuffer->GetImgSize();
					CRect rectDst = CRect(CPoint(0, 0), szDst);
					CRect rect;
					rect.IntersectRect(rectSrc, rectDst);
					ImgCopy(&buf, &rect, pBuffer, &rect);
				}
			}
		}
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[Inspection::LightCalibrationMode] %s", ex.ErrorMessage().Text());
		LogMessage(9, str);
		return -PROCESS_ERROR;
	}
	catch (...) {
		LogMessage(9, "[Inspection::LightCalibrationMode] Error");
		return -PROCESS_ERROR;
	}
	return nError;
}


int CInspection::DotCalibrationMode(CSharedData* pData,CImageBuffer* pBuffer,int nTrack,int nDoc)
{
	 int nError = -PROCESS_ERROR;
	 CRect rcFOV = CRect(CPoint(0,0), pBuffer->GetImgSize()); 
	 CSize szImage = rcFOV.Size();
	 LOGPEN TextColor;
	 TextColor.lopnColor = RGB(255,0,0);
	 TextColor.lopnStyle = PS_SOLID;
	 TextColor.lopnWidth.x = 1;

	 try {
	  HImage hImage;
	  HTuple hFound;
	  HTuple hGreyValue;
	  HTuple hDotWidth;
	  HTuple hDotHeight;
	  HTuple hDotRows;
	  HTuple hDotColumns;
	
	 /* DrwTrackRect(pData->pImageViews[nDoc], &m_CalibrationData.rectDotCalib);
	  DrwUpdate(pData->pImageViews[nDoc]);
	  if(!WaitResponse(pData->pImageViews[nDoc]))
		m_CalibrationData.rectDotCalib = GetTrackRect(pData->pImageViews[nDoc],0);*/
	  
	  m_arrayOverlayDotCalibration.RemoveAll();

	  hImage.GenImage1("byte", rcFOV.Width(), rcFOV.Height(), pBuffer->GetImgPtr());
	 
	  nError = CInspectionCore::DotCalibrationMode(hImage,m_CalibrationData.rectDotCalib,hDotWidth,hDotHeight,
										hDotRows,hDotColumns);

	  //DrwDel(pData->pImageViews[nDoc]);
	  //DrwUpdate(pData->pImageViews[nDoc]);

	  for(int j=0; j<m_arrayOverlayDotCalibration.GetCount(); j++)
			if(!m_arrayOverlayDotCalibration[j].hImage.IsInitialized())
				DrawRegion(pData->pImageViews[nDoc], m_arrayOverlayDotCalibration[j]);

	  //for(int i=0;i<hDotWidth.Length();i++){
		 // //int ptx = hDotRows[i].D();/*-hDotWidth[i].I();*/
		 // //int pty = hDotColumns[i].D();
		 // //int pStr = hDotWidth[i].D();
		 // //int pStr1 = hDotHeight[i].D();
		 // //CString strTemp;
		 // //strTemp.Format("(%d,%d)",pStr,pStr1);
		 // //DrwStr(pData->pImageViews[nDoc],&strTemp,&CPoint(pty-13,ptx+10),&TextColorRed,22);

		 // int ptx = hDotRows[i].D()-hDotWidth[i].D();
		 // int pty = hDotColumns[i].D();
		 // int pStr = hDotWidth[i].D();
		 // CString strTemp;
		 // strTemp.Format("%d",pStr);
		 // DrwStr(pData->pImageViews[nDoc],&strTemp,&CPoint(pty-10,ptx),&TextColorRed,22);

		 // ptx = hDotRows[i].D();
		 // pty = hDotColumns[i].D()-hDotHeight[i].D();
		 // pStr = hDotHeight[i].D();
		 // strTemp.Format("%d",pStr);

		 // DrwStr(pData->pImageViews[nDoc],&strTemp,&CPoint(pty,ptx-10),&TextColorRed,22);
	  //}
	  CString str;
	  str.Format("Resolution Along X [%.4f] (micronsperpixel)", m_CalibrationData.dResolutionAlongXInMicronPerPixel);
	  DrwStr(pData->pImageViews[nDoc], &str, &CPoint(180, 100), &TextColor, 80);
	  OutputDebugLogTo(8, TRUE, "%s", str);

	  str.Format("Resolution Along Y [%.4f] (micronsperpixel)", m_CalibrationData.dResolutionAlongYInMicronPerPixel);
	  DrwStr(pData->pImageViews[nDoc], &str, &CPoint(180, 200), &TextColor, 80);
	  OutputDebugLogTo(8, TRUE, "%s", str);

	  //DrwUpdate(pData->pImageViews[nDoc]);
	 }
	 catch(HOperatorException ex) {
	  CString str;
	  str.Format("[Inspection::DotCalibrationMode] %s", ex.ErrorMessage().Text());
	  LogMessage(9, str);
	  return -PROCESS_ERROR;
	 }
	 catch(...) {
	  LogMessage(9, "[Inspection::DotCalibrationMode] Error");
	  return -PROCESS_ERROR;
	 }
	 return nError;
}

int CInspection::DisplayPVIDefectImage(IMAGEVIEW_INFO *pView, CImageBuffer* pImgBufSrc, int nArea)
{
	try {
		int nCount = m_arrayOverlayPVIInsp.GetCount();
		if (!nCount)
			return 0;

		DrwDel(pView);
		for(int nRgn=0; nRgn<nCount; nRgn++) {
			if (nArea == nRgn) {
				HImage* pImage = &m_arrayOverlayPVIInsp.GetAt(nRgn).hImage;

				//HTuple hArea, hRow, hCol;
				//AreaCenter(*pImage, &hArea, &hRow, &hCol);
				//if (hArea <= 0)
				//	return -1;
				if (pImage->IsInitialized()) {
					HString strType;
					Hlong nWidth, nHeight;
					BYTE* pBuf = (BYTE*)pImage->GetImagePointer1(&strType, &nWidth, &nHeight);
					if (pBuf) {
						CSize szSrc = CSize((int)nWidth, (int)nHeight);
						CRect rectSrc = CRect(CPoint(0, 0), szSrc);
						CImageBuffer buf;
						buf.AllocImgIndirect(szSrc, pBuf);

						CSize szDst = pImgBufSrc->GetImgSize();
						CRect rectDst = CRect(CPoint(0, 0), szDst);
						CRect rect;
						rect.IntersectRect(rectSrc, rectDst);
						ImgCopy(&buf, &rect, pImgBufSrc, &rect);
					}
				}
				//DrawRegion(pView, m_arrayOverlayPVIInsp.GetAt(nRgn).region, NULL, m_arrayOverlayPVIInsp.GetAt(nRgn).color);
			}
		}
		DrwUpdate(pView);
	}
	catch(HOperatorException ex) {
		CString str;
		str.Format("[Inspection::DisplayPVIDefectImage] %s", ex.ErrorMessage().Text());
		LogMessage(9, str);
		return -PROCESS_ERROR;
	}
	catch(...) {
		LogMessage(9, "[Inspection::DisplayPVIDefectImage] Error");
		return -PROCESS_ERROR;
	}
	return 0;
}


//int ManipulateImagePixel(CImageBuffer *pImgSrc, CRect *prcSrc,
//	                      CImageBuffer *pImgDst, CRect *prcDst,
//					      double *pCoEfficient)
//{
//	int x, y;
//	int nImgPitchSrc, nImgPitchDst;
//	int nWidthSrc, nHeightSrc, nWidthDst;
//	BYTE *pRowSrc, *pColSrc, *pRowDst, *pColDst;
//
//	if (!pImgSrc) {
//		ASSERT(!"pImgSrc is a bad Pointer");
//		return -1;
//	}
//	if (!pImgSrc->IsAllocated()) {
//		ASSERT(!"pImgSrc is not Allocated");
//		return -1;
//	}
//	if (ValidateRect(prcSrc, &(pImgSrc->GetImgSize()))) {
//		ASSERT(!"prcSrc is Invalid");
//		return -1;
//	}
//
//	if (!pImgDst) {
//		ASSERT(!"pImgDst is a bad Pointer");
//		return -1;
//	}
//	if (!pImgDst->IsAllocated()) {
//		ASSERT(!"pImgDst is not Allocated");
//		return -1;
//	}
//	if (ValidateRect(prcDst, &(pImgDst->GetImgSize()))) {
//		ASSERT(!"prcDst is Invalid");
//		return -1;
//	}
//
//	if (prcSrc->Size() != prcDst->Size()) {
//		ASSERT(!"prcSrc Size different from prcDst Size");
//		return -1;
//	}
//
//
//	nImgPitchSrc = pImgSrc->GetPitch();
//	nImgPitchDst = pImgDst->GetPitch();
//
//	nWidthSrc = prcSrc->Width();
//	nHeightSrc = prcSrc->Height();
//	nWidthDst = prcDst->Width();
//
//	pRowSrc = pColSrc = pImgSrc->GetImgPtr() + nImgPitchSrc * prcSrc->top + prcSrc->left;
//	pRowDst = pColDst = pImgDst->GetImgPtr() + nImgPitchDst * prcDst->top + prcDst->left;
//	int nIndex = 0;
//	for (y = 0; y<nHeightSrc; y++, pRowSrc += nImgPitchSrc, pRowDst += nImgPitchDst) {
//		for (x = 0, pColSrc = pRowSrc, pColDst = pRowDst; x<nWidthSrc; x++, pColSrc++, pColDst++) {
//				*pColDst = *pColSrc*pCoEfficient[nIndex++];
//				//pCoEfficient[nIndex++] = *pColSrcInsp;
//		}
//	}
//	return 0;
//}


//int GenerateCoEfficient(CImageBuffer *pImgSrc1, CRect *prcSrc1,
//	                    CImageBuffer *pImgSrc2, CRect *prcSrc2,
//	                    double *pCoEfficient)
//{
//	int x, y;
//	int nImgPitchSrc, nImgPitchDst;
//	int nWidthSrc, nHeightSrc, nWidthDst;
//	BYTE *pRowSrc, *pColSrc, *pRowDst, *pColDst;
//
//	if (!pImgSrc1) {
//		ASSERT(!"pImgSrc is a bad Pointer");
//		return -1;
//	}
//	if (!pImgSrc1->IsAllocated()) {
//		ASSERT(!"pImgSrc is not Allocated");
//		return -1;
//	}
//	if (ValidateRect(prcSrc1, &(pImgSrc1->GetImgSize()))) {
//		ASSERT(!"prcSrc is Invalid");
//		return -1;
//	}
//
//	if (!pImgSrc2) {
//		ASSERT(!"pImgDst is a bad Pointer");
//		return -1;
//	}
//	if (!pImgSrc2->IsAllocated()) {
//		ASSERT(!"pImgDst is not Allocated");
//		return -1;
//	}
//	if (ValidateRect(prcSrc2, &(pImgSrc2->GetImgSize()))) {
//		ASSERT(!"prcDst is Invalid");
//		return -1;
//	}
//
//	if (prcSrc1->Size() != prcSrc2->Size()) {
//		ASSERT(!"prcSrc Size different from prcDst Size");
//		return -1;
//	}
//
//
//	nImgPitchSrc = pImgSrc1->GetPitch();
//	nImgPitchDst = pImgSrc2->GetPitch();
//
//	nWidthSrc = prcSrc1->Width();
//	nHeightSrc = prcSrc1->Height();
//	nWidthDst = prcSrc2->Width();
//
//	pRowSrc = pColSrc = pImgSrc1->GetImgPtr() + nImgPitchSrc * prcSrc1->top + prcSrc1->left;
//	pRowDst = pColDst = pImgSrc2->GetImgPtr() + nImgPitchDst * prcSrc2->top + prcSrc2->left;
//	int nIndex = 0;
//	for (y = 0; y<nHeightSrc; y++, pRowSrc += nImgPitchSrc, pRowDst += nImgPitchDst) {
//		for (x = 0, pColSrc = pRowSrc, pColDst = pRowDst; x<nWidthSrc; x++, pColSrc++, pColDst++) {
//			//*pColDst = *pColSrc*pCoEfficient[nIndex++];
//			pCoEfficient[nIndex++] = (*pColSrc) * (*pColDst);
//		}
//	}
//	return 0;
//}