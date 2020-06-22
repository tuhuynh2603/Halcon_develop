/// CInspectionHandler: Vision Inspection manager class
/// Udupa; Jan'2015

#include "stdAfx.h"
#include "InspectionHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif






CInspectionHandler::CInspectionHandler()
{
	for (int i = 0; i < 6; i++)
		rcTeachLocation[i] = CRect(100, 100, 300, 300);
	strArrayShapeType.RemoveAll();
	strArrayShapeType.Add("Rectangle");
	strArrayShapeType.Add("Polygon");
	strArrayShapeType.Add("Ellipse");

	strArrayPatternSearchType.RemoveAll();
	strArrayPatternSearchType.Add("Correlation");
	strArrayPatternSearchType.Add("Shape");

	m_strErrorMsg.Empty();
}

CInspectionHandler::~CInspectionHandler()
{
	CleanUp();
	CleanUpTeach();
	CleanUpPVI();
	strArrayShapeType.RemoveAll();
	strArrayPatternSearchType.RemoveAll();
}

int CInspectionHandler::LoadRecipe(CSharedData* pData,//IMAGEVIEW_INFO *pView,
	CImageBuffer *pImgBufSrc,
	CString strRecipeFolder,
	CString strRecipeFile,
	int nTrack,
	int nDocCount,
	int nFOV,
	int nDoc)
{
	LoadInspectParameters(strRecipeFolder, strRecipeFile, nFOV);
	LoadTeachParameters(strRecipeFolder, strRecipeFile, nFOV);
	GetPVIDefectParameters(strRecipeFolder, nTrack, nFOV);

	return 0;
}

int CInspectionHandler::Teach(CSharedData* pData,//IMAGEVIEW_INFO *pView,
	CImageBuffer *pImgBufSrc,
	CString strRecipeFolder,
	CString strRecipeFile,
	BOOL bAutoTeach,
	long lDebugFlag)

{
	int nError;
	try {
		nError = 0;
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[InspectionHandler::Teach] %s", ex.ErrorMessage().Text());
		LogMessage(0, str);
		return -1;
	}
	catch (...) {
		LogMessage(0, "[InspectionHandler::Teach]");
		return -1;
	}

	return nError;
}

int CInspectionHandler::Teach(CSharedData* pData,//IMAGEVIEW_INFO *pView,
	CImageBuffer *pImgBufSrc,
	CString strRecipeFolder,
	CString strRecipeFile,
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
	CRect& rectEncapLocrect)
{
	try {
		int nError;
		CString strTeachImageFile;

		nError = m_Inspection.Teach(pData, pImgBufSrc, strRecipeFolder,
			nTeachStep, bEnable, bAutoTeach,
			nCurTrack, nDoc, nFOV, nTotalFOV,
			pTrainingData, pCalibData, bEncapLoc, rectEncapLocrect);
		return nError;
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[InspectionHandler::Teach] %s", ex.ErrorMessage().Text());
		LogMessage(0, str);
		return -1;
	}
	catch (...) {
		LogMessage(0, "[InspectionHandler::Teach]");
		return -1;
	}

	return 0;
}

int CInspectionHandler::AutoTeach(CSharedData* pData,//IMAGEVIEW_INFO *pView,
	CImageBuffer *pImgBufSrc,
	CString strRecipeFolder,
	CString strRecipeFile,
	BOOL bRegionTeach,
	int nCurTrack,
	int nDoc,
	int nFOV,
	int nTotalFOV,
	CCalibrationData* pCalibData)
{
	int nError;
	try {
		nError = m_Inspection.AutoTeach(pData, pImgBufSrc, strRecipeFolder, bRegionTeach, nCurTrack, nDoc, nFOV, nTotalFOV, pCalibData);
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[InspectionHandler::AutoTeach] %s", ex.ErrorMessage().Text());
		LogMessage(0, str);
		return -1;
	}
	catch (...) {
		LogMessage(0, "[InspectionHandler::AutoTeach]");
		return -1;
	}

	return nError;
}

int CInspectionHandler::ClearTeachParam(CTrainingData* pTrainingData)
{
	int nError;
	try {
		nError = m_Inspection.ClearTeachParam(pTrainingData);
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[InspectionHandler::ClearTeachParam] %s", ex.ErrorMessage().Text());
		LogMessage(0, str);
		return -PROCESS_ERROR;
	}
	catch (...) {
		LogMessage(0, "[InspectionHandler::ClearTeachParam]");
		return -PROCESS_ERROR;
	}

	return nError;
}

int CInspectionHandler::ClearInspParam(CInspectinonData* pInspData, BOOL bInspClearAll)
{
	int nError;
	try {
		nError = m_Inspection.ClearInspParam(pInspData, bInspClearAll);
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[InspectionHandler::ClearInspParam] %s", ex.ErrorMessage().Text());
		LogMessage(0, str);
		return -PROCESS_ERROR;
	}
	catch (...) {
		LogMessage(0, "[InspectionHandler::ClearInspParam]");
		return -PROCESS_ERROR;
	}

	return nError;
}

int CInspectionHandler::ClearCalibParam(CCalibrationData* pCalibData, BOOL bClearAvg, BOOL bClearInsp)
{
	int nError;
	try {
		nError = m_Inspection.ClearCalibParam(pCalibData, bClearAvg, bClearInsp);
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[InspectionHandler::ClearCalibParam] %s", ex.ErrorMessage().Text());
		LogMessage(0, str);
		return -PROCESS_ERROR;
	}
	catch (...) {
		LogMessage(0, "[InspectionHandler::ClearCalibParam]");
		return -PROCESS_ERROR;
	}

	return nError;
}

int CInspectionHandler::GetCalibDataForInsp(CCalibrationData* pCalibData)
{
	int nError;
	try {
		nError = m_Inspection.GetCalibDataForInsp(pCalibData);
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[InspectionHandler::GetCalibDataForInsp] %s", ex.ErrorMessage().Text());
		LogMessage(0, str);
		return -PROCESS_ERROR;
	}
	catch (...) {
		LogMessage(0, "[InspectionHandler::GetCalibDataForInsp]");
		return -PROCESS_ERROR;
	}

	return nError;
}

int CInspectionHandler::SetTrainingData()
{
	CTrainingData* pData = &m_Inspection.m_TrainingData;

	//pData->nCuvetteRadiusRa = rcTeachLocation[0].Width()/2;
	//pData->nCuvetteRadiusRb = rcTeachLocation[0].Width()/2;
	//
	//pData->ptCuvetteTopLeftRect = rcTeachLocation[0].TopLeft();//
	//pData->ptCuvetteBotRightRect = rcTeachLocation[0].BottomRight();//

	//int nTeachSteps = m_Inspection.m_LensLocationParam.nNumberOfLayer;
	//for(int i=0; i<nTeachSteps+1; i++)
	//{
	//	pData->nPatternCols[i] = rcTeachLocation[i+1].CenterPoint().x;
	//	pData->nPatternRows[i] = rcTeachLocation[i+1].CenterPoint().y;
	//	pData->nPatternRads[i] = (rcTeachLocation[i+1].Width()/2 + rcTeachLocation[i+1].Height()/2)/2;
	//}

	//pData->nLensRadius      = rcTeachLocation[1].Width()/2;
	//pData->nPatternOuterRadius      = rcTeachLocation[2].Width()/2;
	//pData->nPatternInnerRadius      = rcTeachLocation[3].Width()/2;
	return 0;
}

int CInspectionHandler::Inspect(CSharedData* pData,
	long lDebugFlag, BOOL *m_bStepSelection)
{
	int nError;
	try {
		nError = 0;
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[InspectionHandler::Inspect] %s", ex.ErrorMessage().Text());
		LogMessage(0, str);
		return -PROCESS_ERROR;
	}
	catch (...) {
		LogMessage(0, "[InspectionHandler::Inspect]");
		return -PROCESS_ERROR;
	}

	return nError;
}

int CInspectionHandler::Inspect(CSharedData* pData,
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
	int nError;
	try {
		nError = m_Inspection.Inspect(pData, pImgBufSrc, pTeachImgBufSrc,
			lDebugFlag, bRegionInsp,
			nCurTrack, nCurDoc, nTotalDoc, nCurFOV, nTotalFOV,
			pTrainingData, pInspData, pCalibData,
			strArrayInspValues);
	}
	catch (HOperatorException ex) {
		CString str;
		str.Format("[InspectionHandler::Inspect] %s", ex.ErrorMessage().Text());
		LogMessage(0, str);
		return -PROCESS_ERROR;
	}
	catch (...) {
		LogMessage(0, "[InspectionHandler::Inspect]");
		return -PROCESS_ERROR;
	}

	return nError;
}

void CInspectionHandler::CreateParameterList(int nTrack, int nDoc, int nFOV)
{
	CleanUp();
	CParameters* pParm;

	CStringArray strArrayCharacteristicType;
	strArrayCharacteristicType.Add("Black");
	strArrayCharacteristicType.Add("White");
	strArrayCharacteristicType.Add("Any Color");

	pParm = new CParameters();
	pParm->strInspection = "Blue Encap";
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.bEnableBlueEncap, "Enable", TBOOL, "", 0, 1, "", "Enable Excess / Insufficient Encap inspection"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapErosionOffset, "Edge Offset", TDOUBLE, "", 0, 600, "Microns", "Inspection offset from encap edge (Border Lay On Recheck)"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapMaxLayOnAllowedPercentage, "Max. Lay On", TDOUBLE, "", 0, 100, "%", "Maximum length of the defect lay on the encap border allowed"));
	// Insufficient
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dMinInSuffPercentageRightBlueEncap, "Min Percentage Right", TDOUBLE, "Insufficient", 0, 100, "", "Percentage between Non Encap Right Region and Right Teach Region is lager than this ratio that is insufficient"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dMinInSuffPercentageLeftBlueEncap, "Min Percentage Left", TDOUBLE, "Insufficient", 0, 100, "", "Percentage between Non Encap Left Region and Left Teach Region is lager than this ratio that is insufficient"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dMinSizeMidBlueEncap, "Min Size Middle", TDOUBLE, "Insufficient", 0, 9999999, "Micron2", "Minimum Area of Insufficient defect region on Middle Encap"));
	// Excess
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nContrastExcessBlueEncap, "Intensity", TINT, "Excess", 0, 255, "levels", "Maximum contrast of Excess defect Region"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nCircleContrastExcessBlueEncap, "Circle Intensity", TINT, "Excess", 0, 255, "levels", "Maximum contrast of Black Circle Region"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dEdgeOffsetExcessBlueEncap, "Edge Offset", TDOUBLE, "Excess", 0, 9999999, "Micron", "The offset of Excess Mask using to check position"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dMinSizeExcessBlueEncap, "Min Size", TDOUBLE, "Excess", 0, 9999999, "Micron2", "Minimun total Area of Excess defect Region"));
	// Glue - Mason
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBEncapMaskOffsetGlue, "Encap Mask Offset", TDOUBLE, "Glue", 0, 99999, "Microns", "The offset of Encap Mask using to check position of Glue defect."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBEncapRegionOffsetGlue, "Encap Region Offset", TDOUBLE, "Glue", 0, 99999, "Microns", "The offset of Encap Region using to check position of Glue defect."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBSmoothMaskSizeGlue, "Smooth Mask Size", TDOUBLE, "Glue", 0, 99999, "Microns", "The Size of Mask which is used to make defect smoother."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBMaskSizeGlue, "Mask Size", TDOUBLE, "Glue", 0, 99999, "Microns", "The size of window using to catch Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBStandardDeviationGlue, "Standard Deviation", TDOUBLE, "Glue", 0, 99999, "", "Factor for the standard deviation of the Gray values."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBDiffMeanGrayGlue, "Diff Mean Gray", TINT, "Glue", 0, 255, "", "Minimum Gray value difference from the mean."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBMinMeanGrayGlue, "Min. Mean Gray", TINT, "Glue", 0, 255, "", "Minimum mean Gray of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBMinSizeGlue, "Min. Size", TDOUBLE, "Glue", 0, 99999, "Microns2", "Minimum Area of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBMinConvexityGlue, "Min. Convexity", TDOUBLE, "Glue", 0, 1, "", "Minimum Convexity of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBMaxInnerAreaPercentGlue, "Max. Inner Area Percent", TDOUBLE, "Glue", 0, 100, "", "Maximum percent of Glue defect region/Encap region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBMaxFuzzySizeGlue, "Max. Fuzzy Size", TDOUBLE, "Glue", 0, 99999, "Microns", "Maximum Size of Fuzzy Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBMaxDiffMeanGrayGlue, "Max. Diff Mean Gray", TINT, "Glue", 0, 255, "", "Maximum difference mean Gray between Outside and Inside of Contamination defect region."));

	// Contamination - Mason
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBMaskSizeCT, "Mask Size", TDOUBLE, "Contamination", 0, 99999, "Microns", "The size of window using to catch Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBStandardDeviationCT, "Standard Deviation", TDOUBLE, "Contamination", 0, 99999, "", "Factor for the standard deviation of the Gray values."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBDiffMeanGrayCT, "Diff Mean Gray", TINT, "Contamination", 0, 255, "", "Minimum Gray value difference from the mean."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBBottomMaskContrastCT, "Bottom Mask Contrast", TINT, "Contamination", 0, 255, "", "Minimun Contrast of Contamination Defect Region in the Bottom mask."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBFlexMaskContrastCT, "Flex Mask Contrast", TINT, "Contamination", 0, 255, "", "Minimun Contrast of Contamination Defect Region in the Flex mask."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBMinGrayCT, "Min. Gray", TINT, "Contamination", 0, 255, "", "Minimum Gray of Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBMinSizeCT, "Min. Size", TDOUBLE, "Contamination", 0, 99999, "Microns2", "Minimum Area of Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBMinDiffMeanGrayCT, "Min. Diff Mean Gray", TINT, "Contamination", 0, 255, "", "Minimum difference mean Gray between Outside and Inside of Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBMinMeanEdgeMaskGrayCT, "Min. Mean Edge Mask Gray", TINT, "Contamination", 0, 255, "", "Minimum mean Gray of Contamination defect region in the Edge of Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBMinMeanFlexMaskGrayCT, "Min. Mean Flex Mask Gray", TINT, "Contamination", 0, 255, "", "Minimum mean Gray of Contamination defect region in the Edge between Flex and Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBMinMeanBorderedGrayCT, "Min. Mean Bordered Gray", TINT, "Contamination", 0, 255, "", "Minimum mean Gray of Bordered of Contamination defect region in Encap region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBMaxDiffAreaPercentCT, "Max. Diff Area Percent", TDOUBLE, "Contamination", 0, 100, "", "Maximum Percent of Bordered Defect/Defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBMinDefectCountCT, "Min. Defect Count", TINT, "Contamination", 1, 99999, "", "-Minimum Defect of Contamination is allowed.\n-If the number of Defect is equal or more than this value, the Defect will be classified in Contamination."));

	
	//	Encap Void
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueEncapVoidMinDefectContrast, "Intensity", TINT, "Void", 0, 255, "", "Intensity of ther blur defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapVoidMinMaskSize, "Slide Window Size", TDOUBLE, "Void", 0, 999999999, "Microns", "-The Size of Window to catch the blur defect."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapVoidMinSize, "Min. Size", TDOUBLE, "Void", 0, 9999999, "Microns2", "Minimum area of Void"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapVoidMinCircularity, "Min. Circularity", TDOUBLE, "Void", 0, 1, "", "Minimum circularity of Void Is Allowed; 1=Perfect circle.\n-If the Shape of Defect is different from a circle, the Defect will be classified in Void.\n-Should setup this value equal to Min. Circularity of Void value."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapVoidMinLength, "Min. Length", TDOUBLE, "Void", 0, 99999, "Microns", "-Minimum length of Void.\n-If the Length of Defect is less than Min. Length value, the Defect will be classified in Void.\n-If this value is increase too high, any Defects will not be classified in Void. Otherwise, all of Defect will be classfied in Void."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapVoidMinSquareSize, "Min. Square Size", TDOUBLE, "Void", 0, 99999, "Microns", "-Minimum width & height of Void\n-If the Width and Height of Defect are less than Min. Square Size value, the Defect will be classified in Void.\n-If this value is increase too high, any Defects will not classified in Void. Otherwise, all of Defect will be classfied in Void."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueEncapVoidGrayMean, "Min. Contrast", TINT, "Void", 0, 255, "", "Minimum contrast of Void"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueEncapVoidGrayDeviation, "Min. Contrast Deviation", TINT, "Void", 0, 255, "", "Minimum Contrast Deviation of Void"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueEncapVoidMinMeanGVDiffContamination, "Min. GVDiff", TINT, "Void", 0, 255, "", "Minimum contrast of GV Difference"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapFuzzyVoidMinSize, "Min. Size Fuzzy Void", TDOUBLE, "Void", 0, 9999999, "Microns2", "Minimum area of Void"));

	// Blue Encap Blow Hole
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueEncapBHIntensity, "Blow Hole Contrast", TINT, "Blow Hole", 0, 255, "levels", "Minimum contrast of Blow Hole"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapBHSlideWindowSide, "Slide Window Side", TDOUBLE, "Blow Hole", 0, 99999, "Microns", "-The Size of Window to catch the blur defect."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapBHMinArea, "Min. Size", TDOUBLE, "Blow Hole", 0, 99999, "Microns2", "Minimum area of Blow Hole"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapBHMinCircularity, "Min. Circularity", TDOUBLE, "Blow Hole", 0, 1, "", "-Minimum Circularity of Blow Hole; 1=Perfect circle.\n-If the Shape of Defect is similar to a circle, the Defect will be classified in Blow Hole.\n-Should setup this value equal to Max. Circularity of Blow Hole value."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapBHMinPercentLightArea, "Min. Percent", TDOUBLE, "Blow Hole", 0, 99999, "", "if the Region has Light Area Per Dark Area less than Min Percentage that is Defect "));
	

	// Blue Encap Wire And IC
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueEncapTopWireIntensity, "Top Wire Intensity", TINT, "Wire And IC", 0, 255, "", "Intensity of ther blur defect on Top Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapTopWireMaxLineWidth, "Top Wire Max Line Width", TDOUBLE, "Wire And IC", 0, 999999, "Microns", "Maximum of Line Width of Wire defect on Top Encap "));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapTopWireMinArea, "Top Wire Min Area", TDOUBLE, "Wire And IC", 0, 999999, "Microns2", "Minimum Area of Wire Defect on Top Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueEncapBottomWireIntensity, "Bottom Wire Intensity", TINT, "Wire And IC", 0, 255, "", "Intensity of ther blur defect on Bottom Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapBottomWireMaxLineWidth, "Bottom Wire Max Line Width", TDOUBLE, "Wire And IC", 0, 999999, "Microns", "Maximum of Line Width of Wire defect on Top Encap "));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapBottomWireMinArea, "Bottom Min Area", TDOUBLE, "Wire And IC", 0, 999999, "Microns2", "Minimum Area of Wire Defect on Bottom Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueEncapMinAreaIC, "IC Min Area", TDOUBLE, "Wire And IC", 0, 999999, "Microns2", "Minimum Area of IC Defect on Bottom Encap"));

	// Blue Encap Bulge Magnus 
	//pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueBulgeScaleContrastEncap, "Min. Gray Defect", TINT, "Bulge", 0, 255, "", "If mean gray of defect > Min Gray, it will be selected"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueBulgeValueContrastDefect, "Min. Contrast threshold", TINT, "Bulge", 0, 255, "", "Difference in brightness between defects and encap. Small gray value changes in homogeneous surroundings can be ignored"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueBulgeScaleContrastDefect, "Scale Contrast defect", TINT, "Bulge", 0, 1000, "", "The local standard deviation is used as a measure of noise in the image. It can be scaled by StdDevScale to reflect the desired sensitivity. A higher value means that only pixels that are very different from their surrounding are selected."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueBulgeWidthLocalSearch, "Max. width size of the objects", TDOUBLE, "Bulge", 1, 99999, "Microns", "The size of the filter mask defined by MaskWidth and MaskHeight determines the maximum size of the objects to be segmented. However, if the mask is chosen too large, objects that are very close might be merged."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueBulgeHeightLocalSearch, "Max. height size of the objects", TDOUBLE, "Bulge", 1, 99999, "Microns", "The size of the filter mask defined by MaskWidth and MaskHeight determines the maximum size of the objects to be segmented. However, if the mask is chosen too large, objects that are very close might be merged."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueBulgeOffsetEncap, "Offset Encap", TDOUBLE, "Bulge", 0, 99999, "Microns", "offset encap region for inspection"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueBulgeClosingRadiusDefect, "Closing Radius defect ", TDOUBLE, "Bulge", 0, 999, "Microns2", "To connect defect region that are close to each other"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlueBulgeMinAreaDefect, "Min. Area defect", TDOUBLE, "Bulge", 0, 999999, "Microns2", "Minimum area of Bulge defect on encap"));
	//pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueCircularityDefect, "Width local search", TINT, "Bulge", 0, 999999, "Microns2", "Value for search around defect, smaller width defect, smaller Width value"));
	//pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueHWRatioDefect, "Height local search", TINT, "Bulge", 0, 999999, "Microns2", "Value for search around defect, smaller height defect, smaller Height value"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueBulgeInnerDefectPercent, "Min. Percent Inner of Defect", TINT, "Bulge", 0, 100, "", "Min % Area of Inner Defect region/Defect region "));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueBulgeOffsetCheckDefect, "Offset Defect", TINT, "Bulge", 0, 100, "%", "% Inner Radius of defect to offset (for get type of error)"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueBulgeMaxContrastInOutDefect, "Max. Difference Contrast Inner-Outer ", TINT, "Bulge", 0, 255, "", "Difference in brightness between inner and outer of defect, if value < Max contrast, it will be classified in Bulge."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueBulgeMinGrayDefect, "Min. Contrast", TINT, "Bulge", 0, 255, "", "Minimum contrast of Bulge"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueBulgeMaxGrayDefect, "Max. Contrast", TINT, "Bulge", 0, 255, "", "Maximum contrast of Bulge"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueBulgeMaxDeviationInnerDefect, "Max. brightness noise inner", TINT, "Bulge", 0, 255, "", "For get type error, if brightness noise inner of defect < Max brightness, it will be classified in Bulge."));
	m_Parameters.Add(pParm);

	// Dark Encap Vision Parameters
	pParm = new CParameters();
	pParm->strInspection = "Dark Encap";
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.bEnableDarkEncap, "Enable", TBOOL, "", 0, 1, "", "Enable Dark Encap Inspection."));

	// Excess -magnus
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkEncapExcessOffset, "Edge Offset", TDOUBLE, "Excess", 0, 99999, "Microns", "The offset of Excess Mask using to check position of Excess defect."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkEncapExcessMinAreaDefect, "Min. Area", TDOUBLE, "Excess", 0, 999999, "Microns", "Minimum total Area of Excess defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkEncapExcessMinContrastDefect, "Min. Contrast", TINT, "Excess", 0, 255, "", "Minimum contrast of Excess defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkEncapExcessMaxContrastDefect, "Max. Contrast", TINT, "Excess", 0, 255, "", "Maximum contrast of Excess defect region."));

	//Insuff - magnus
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkEncapInsuffContrast, "Intensity", TINT, "Insufficient", 0, 255, "levels", "Intensity For Find Border Base"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkEncapInsuffVariationContrast, "Min. Smooth Insufficient", TINT, "Insufficient", 0, 255, "", "Minimum Smooth of Insufficient Area using to re-check Insufficient on encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkEncapInsuffWhiteContrast, "White Contrast", TINT, "Insufficient", 0, 255, "", "Minimum contrast of White Insufficient region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkEncapInsuffLeftOffset, "Left Edge Offset", TDOUBLE, "Insufficient", 0, 99999, "Microns", "The offset of Left Reference Line using to check Insufficient in the Left Edge of Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkEncapInsuffRightOffset, "Right Edge Offset", TDOUBLE, "Insufficient", 0, 99999, "Microns", "The offset of Right Reference Line using to check Insufficient in the Right Edge of Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkEncapInsuffMinPercentLeftError, "Min Percentage Left", TDOUBLE, "Insufficient", 0, 100, "", "Percentage between Non Encap Left Region and Left Teach Region is lager than this ratio that is insufficient"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkEncapInsuffMinPercentRightError, "Min Percentage Right", TDOUBLE, "Insufficient", 0, 100, "", "Percentage between Non Encap Right Region and Right Teach Region is lager than this ratio that is insufficient"));
	// end magnus
	// Glue - Mason
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkContrastGlue, "Contrast", TINT, "Glue", 0, 255, "", "Minimum contrast of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkMaskSizeGlue, "Mask Size", TDOUBLE, "Glue", 0, 99999, "Microns", "The size of window using to catch Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkEncapRegionOffsetGlue, "Encap Region Offset", TDOUBLE, "Glue", 0, 99999, "Microns", "The offset of Encap region using to check position of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkMinSizeGlue, "Min. Size", TDOUBLE, "Glue", 0, 99999, "Microns2", "Minimum Area of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkMinMeanGrayGlue, "Min. Mean Gray", TINT, "Glue", 0, 255, "", "Minimum mean Gray of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkMinConvexityGlue, "Min. Convexity", TDOUBLE, "Glue", 0, 1, "", "Minimum Convexity of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkMinDiffMeanGrayGlue, "Min. Diff Mean Gray", TINT, "Glue", 0, 255, "", "Minimun difference mean Gray between Outside and Inside of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkMaxFuzzySizeGlue, "Max. Fuzzy Size", TDOUBLE, "Glue", 0, 99999, "Microns2", "Minimum Area of Fuzzy Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkMaxWhiteAreaGlue, "Max. White Area", TDOUBLE, "Glue", 0, 99999, "Microns2", "Minimum Area of White region inside Glue defect region."));

	// Contamination - Mason
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkMaskSizeCT, "Mask Size", TDOUBLE, "Contamination", 0, 99999, "Microns", "The size of window using to catch Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkStandardDeviationCT, "Standard Deviation", TDOUBLE, "Contamination", 0, 99999, "", "Factor for the standard deviation of the Gray values."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkDiffMeanGrayCT, "Diff Mean Gray", TINT, "Contamination", 0, 255, "", "Minimum Gray value difference from the mean."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkFuzzyContrastCT, "Fuzzy Contrast", TINT, "Contamination", 0, 255, "", "Minimum contrast of Fuzzy Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkMinGrayCT, "Min. Gray", TINT, "Contamination", 0, 255, "", "Minimum Gray of Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkMinSizeCT, "Min. Size", TDOUBLE, "Contamination", 0, 99999, "Microns2", "Minimum Area of Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkEncapRegionOffsetCT, "Encap Region Offset", TDOUBLE, "Contamination", 0, 99999, "Microns", "The offset of Encap region using to check mean Gray of Contamination in the Edge of Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkMinDiffMeanGrayCT, "Min. Diff Mean Gray", TINT, "Contamination", 0, 255, "", "Difference Mean Gray between Inside Borderd and Outside Bordered using for re-check Contamination."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkMinMeanEdgeMaskGrayCT, "Min. Mean Edge Mask Gray", TINT, "Contamination", 0, 255, "", "Minimum mean Gray of Contamination defect region in the Edge of Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkMaxDiffAreaPercentCT, "Max. Diff Area Percent", TDOUBLE, "Contamination", 0, 100, "", "Maximum Percent of Bordered Defect/Defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkMinFuzzyGrayCT, "Min. Fuzzy Gray", TINT, "Contamination", 0, 255, "", "Minimum Gray of Fuzzy Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkMinDiffMeanFuzzyGrayCT, "Min. Diff Mean Fuzzy Gray", TINT, "Contamination", 0, 255, "", "Difference Mean Gray between Inside Borderd and Outside Bordered using for re-check Fuzzy Contamination."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkMinDefectCountCT, "Min. Defect Count", TINT, "Contamination", 1, 99999, "", "-Minimum Defect of Contamination is allowed.\n-If the number of Defect is equal or more than this value, the Defect will be classified in Contamination."));
	
	//Void
	// // Small Void
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dSmallVoidMinSizeDarkEncap, "SV Min. Size", TDOUBLE, "Void", 0, 99999999, "Micron2", "Minimum Size of Small Void Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dSmallVoidMinCircularityDarkEncap, "SV Min. Circularity", TDOUBLE, "Void", 0, 1, "", "-Minimum Circularity of Small Void; 1=Perfect circle.\n-If the Shape of Defect is similar to a circle, the Defect will be classified in Small Void.\n-Should setup this value equal to Max. Circularity of Small Void value."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dSmallVoidGrayDeviationDarkEncap, "SV Min. Gray Deviation", TDOUBLE, "Void", 0, 255, "", "Minimum Gray Deviation of Small Void Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nSmallVoidIntensityDarkEncap, "SV Min. Intensity", TINT, "Void", 0, 255, "", "Minimum contrast of Small Void Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dSmallVoidMaxLayOnPartPercentageAllowDarkEncap, "SV Max. Lay On", TDOUBLE, "Void", 0, 100, "%", "Maximum length of the defect lay on the encap border allowed"));
	// // Large Void
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dLargeVoidMinSizeDarkEncap, "LV Min. Size", TDOUBLE, "Void", 0, 99999999, "Micron2", "Minimum Size of Large Void Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dLargeVoidMinCircularityDarkEncap, "LV Min. Circularity", TDOUBLE, "Void", 0, 1, "", "-Minimum Circularity of Large Void; 1=Perfect circle.\n-If the Shape of Defect is similar to a circle, the Defect will be classified in Large Void.\n-Should setup this value equal to Max. Circularity of Large Void value."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dLargeVoidMinGrayDeviationDarkEncap, "LV Min. Gray Deviation", TDOUBLE, "Void", 0, 255, "", "Minimum Gray Deviation of Large Void Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nLargeVoidIntensityDarkEncap, "LV Min. Intensity", TINT, "Void", 0, 255, "", "Minimum contrast of Large Void Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dLargeVoidMaxLayOnPartPercentageAllowDarkEncap, "LV Max. Lay On", TDOUBLE, "Void", 0, 100, "%", "Maximum length of the defect lay on the encap border allowed"));
	// // Fuzzy Void
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dFuzzyVoidMinSizeDarkEncap, "FV Min. Size", TDOUBLE, "Void", 0, 99999999, "Micron2", "Minimum Size of Fuzzy Void Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dFuzzyVoidMinGrayDeviationDarkEncap, "FV Min. Gray Deviation", TDOUBLE, "Void", 0, 255, "", "Minimum Gray Deviation of Fuzzy Void Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nFuzzyVoidMinIntensityDarkEncap, "FV Min. Intensity", TINT, "Void", 0, 255, "", "Minimum contrast of Fuzzy Void Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dFuzzyVoidMaxLayOnPartPercentageAllow, "FV Max. Lay On", TDOUBLE, "Void", 0, 100, "%", "Maximum length of the defect lay on the encap border allowed"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dFuzzyVoidMinMeanGVDiffDarkEncap, "FV Min. GVDiff", TDOUBLE, "Void", 0, 255, "", "Minimum contrast of GV Difference"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dFuzzyVoidMinAnisometryDarkEncap, "FV Min. Anisometry", TDOUBLE, "Void", 0, 999999, "", "Anisometry is ratio between Long and Short Edges of Object"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dFuzzyVoidMinGrayMeanDarkEncap, "FV Min. Gray Mean", TDOUBLE, "Void", 0, 255, "", "Minimum Gray Mean of Object"));

	// BlowHole
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dBlowHoleSlideWindowSideDarkEncap, "Slide Window Side", TDOUBLE, "Blow Hole", 0, 99999, "Microns", "-The Size of Window to catch the blur defect."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dLargeBlowHoleAnisometryDarkEncap, "LBH Min. Anisometry", TDOUBLE, "Blow Hole", 0, 999999, "", "Anisometry is ratio between Long and Short Edges of Object"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dSmallBlowHoleAnisometryDarkEncap, "SBH Min. Anisometry", TDOUBLE, "Blow Hole", 0, 999999, "", "Anisometry is ratio between Long and Short Edges of Object"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dSmallBlowMaxGrayMeanDarkEncap, "SBH Max. Gray Mean", TDOUBLE, "Blow Hole", 0, 255, "", "Maximum Gray Mean of Object"));

	// Wire And IC
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nWireContrastDarkEncap, "Min. Wire Contrast", TINT, "Wire And IC", 0, 255, "", "Minimun Contrast of Wire Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dWireMaxLineWidthDarkEncap, "Max. Wire Width", TDOUBLE, "Wire And IC", 0, 999999, "Microns", "Maximum of Line Width of Wire defect on Dark Encap "));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dMaxWireCircularityDarkEncap, "Max. Wire Circularity", TDOUBLE, "Wire And IC", 0, 999999, "", "-Minimum Circularity of Wire; 1=Perfect circle.\n-If the Shape of Defect is similar to a circle, the Defect will be classified in Wire.\n-Should setup this value equal to Max. Circularity of Wire value."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dWireMinLengthDarkEncap, "Min. Wire Length", TDOUBLE, "Wire And IC", 0, 999999, "Microns", "Minimum Length of Wire defect"));

	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nICIntensityDarkEncap, "Min. IC Contrast", TINT, "Wire And IC", 0, 999999, "", "Minimun Contrast of IC Defect"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dICMinAreaDarkEncap, "Min. IC Area", TDOUBLE, "Wire And IC", 0, 999999, "Microns2", "Minimum Area of IC Defect"));
	m_Parameters.Add(pParm);	
	
	// Dark Blue Encap Vision Parameters
	pParm = new CParameters();
	pParm->strInspection = "Dark Blue Encap";
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.bEnableDarkBlueEncap, "Enable", TBOOL, "", 0, 1, "", "Enable Dark Blue Encap Inspection."));

	// Excess - Mason
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBContrastEX, "Contrast", TINT, "Excess", 0, 255, "", "Maximum contrast of Excess defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBCircleContrastEX, "Circle Contrast", TINT, "Excess", 0, 255, "", "Maximum contrast of black Circle region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBEdgeOffsetEX, "Edge Offset", TDOUBLE, "Excess", 0, 99999, "Microns", "The offset of Excess Mask using to check position of Excess defect."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBMinAreaEX, "Min. Area", TDOUBLE, "Excess", 0, 99999, "Microns", "Minimum total Area of Excess defect region."));

	// Insufficient - Mason
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBWhiteContrastIS, "White Contrast", TINT, "Insufficient", 0, 255, "", "Minimum contrast of White Insufficient region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBBlackContrastIS, "Black Contrast", TINT, "Insufficient", 0, 255, "", "Maximum contrast of Black Insufficient region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBEdgeContrastIS, "Edge Contrast", TINT, "Insufficient", 0, 255, "", "Maximum contrast of Edge Black region using to check Insufficient in the Edge of Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBInnerEdgeOffsetIS, "Inner Edge Offset", TDOUBLE, "Insufficient", 0, 99999, "Microns", "The offset of Inner Mask using to re-check Insufficient in the Edge of Inner Mask."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBLeftEdgeOffsetIS, "Left Edge Offset", TDOUBLE, "Insufficient", 0, 99999, "Microns", "The offset of Left Reference Line using to check Insufficient in the Left Edge of Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBRightEdgeOffsetIS, "Right Edge Offset", TDOUBLE, "Insufficient", 0, 99999, "Microns", "The offset of Right Reference Line using to check Insufficient in the Right Edge of Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBMinDiffMeanGrayIS, "Min. Diff Mean Gray", TINT, "Insufficient", 0, 255, "", "Difference Mean Gray between Inside Borderd and Outside Bordered using for re-check inner Insufficient region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBMinInnerAreaIS, "Min. Inner Area", TDOUBLE, "Insufficient", 0, 99999, "Microns", "Minimum total Area of Insufficient defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBMinSmoothAreaPercentIS, "Min. Smooth Area Percent", TDOUBLE, "Insufficient", 0, 100, "", "Minimum Smooth Area/Area percent using to re-check Insufficient in the Edge of Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBMinEdgePercentIS, "Min. Edge Percent", TDOUBLE, "Insufficient", 0, 100, "", "Minimum Percent of Insufficient in the Edge of Encap."));

	// Glue - Mason
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBContrastGlue, "Contrast", TINT, "Glue", 0, 255, "", "Minimum contrast of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBEncapMaskOffsetGlue, "Encap Mask Offset", TDOUBLE, "Glue", 0, 99999, "Microns", "The offset of Encap Mask using to check position of Glue defect."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBFlexMaskOffsetGlue, "Flex Mask Offset", TDOUBLE, "Glue", 0, 99999, "Microns", "The offset of Flex Mask using to check position of Glue defect between Flex and Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBSmoothMaskSizeGlue, "Smooth Mask Size", TDOUBLE, "Glue", 0, 99999, "Microns", "The Size of Mask which is used to make defect smoother."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBMinSizeGlue, "Min. Size", TDOUBLE, "Glue", 0, 99999, "Microns2", "Minimum Area of Glue defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBMinConvexityGlue, "Min. Convexity", TDOUBLE, "Glue", 0, 1, "", "Minimum Convexity of Glue defect region."));

	// Contamination - Mason
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBMaskSizeCT, "Mask Size", TDOUBLE, "Contamination", 0, 99999, "Microns", "The size of window using to catch Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBStandardDeviationCT, "Standard Deviation", TDOUBLE, "Contamination", 0, 99999, "", "Factor for the standard deviation of the Gray values."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBDiffMeanGrayCT, "Diff Mean Gray", TINT, "Contamination", 0, 255, "", "Minimum Gray value difference from the mean."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBBottomMaskContrastCT, "Bottom Mask Contrast", TINT, "Contamination", 0, 255, "", "Minimun Contrast of Contamination Defect Region in the Bottom mask."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBFlexMaskContrastCT, "Flex Mask Contrast", TINT, "Contamination", 0, 255, "", "Minimun Contrast of Contamination Defect Region in the Flex mask."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBMinGrayCT, "Min. Gray", TINT, "Contamination", 0, 255, "", "Minimum Gray of Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBMinSizeCT, "Min. Size", TDOUBLE, "Contamination", 0, 99999, "Microns2", "Minimum Area of Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBMinDiffMeanGrayCT, "Min. Diff Mean Gray", TINT, "Contamination", 0, 255, "", "Minimun difference mean Gray between Outside and Inside of Contamination defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBMinMeanEdgeMaskGrayCT, "Min. Mean Edge Mask Gray", TINT, "Contamination", 0, 255, "", "Minimum mean Gray of Contamination defect region in the Edge of Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBMinMeanFlexMaskGrayCT, "Min. Mean Flex Mask Gray", TINT, "Contamination", 0, 255, "", "Minimum mean Gray of Contamination defect region in the Edge between Flex and Encap."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBMinMeanBorderedGrayCT, "Min. Mean Bordered Gray", TINT, "Contamination", 0, 255, "", "Minimum mean Gray of Bordered of Contamination defect region in Encap region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBMaxDiffAreaPercentCT, "Max. Diff Area Percent", TDOUBLE, "Contamination", 0, 100, "", "Maximum Percent of Bordered Defect/Defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBMinDefectCountCT, "Min. Defect Count", TINT, "Contamination", 1, 99999, "", "-Minimum Defect of Contamination is allowed.\n-If the number of Defect is equal or more than this value, the Defect will be classified in Contamination."));

	// Void
	//pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nBlueVoidScaleContrastEncap, "Min. Gray Defect", TINT, "Void", 0, 255, "", "If mean gray of defect > Min Gray, it will be selected"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBlueVoidValueContrastDefect, "Min. Contrast threshold", TINT, "Void", 0, 255, "", "Difference in brightness between defects and encap. Small gray value changes in homogeneous surroundings can be ignored"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBlueVoidScaleContrastDefect, "Scale Contrast defect", TINT, "Void", 0, 1000, "", "The local standard deviation is used as a measure of noise in the image. It can be scaled by 'Scale Contrast defect' to reflect the desired sensitivity. A higher value means that only pixels that are very different from their surrounding are selected."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBlueVoidWidthLocalSearch, "Max. width size of the objects", TDOUBLE, "Void", 1, 99999, "Microns", "The size of the filter mask defined by MaskWidth and MaskHeight determines the maximum size of the objects to be segmented. However, if the mask is chosen too large, objects that are very close might be merged."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBlueVoidHeightLocalSearch, "Max. height size of the objects", TDOUBLE, "Void", 1, 99999, "Microns", "The size of the filter mask defined by MaskWidth and MaskHeight determines the maximum size of the objects to be segmented. However, if the mask is chosen too large, objects that are very close might be merged."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBlueVoidOffsetEncap, "Offset Encap", TDOUBLE, "Void", 0, 99999, "Microns", "offset encap region for inspection"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBlueVoidClosingRadiusDefect, "Closing Radius defect ", TDOUBLE, "Void", 0, 999, "Microns2", "To connect defect region that are close to each other"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDBlueVoidMinAreaDefect, "Min. Area defect", TDOUBLE, "Void", 0, 999999, "Microns2", "Minimum area of VOID defect on encap"));
	//pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBlueCircularityDefect, "Width local search", TINT, "Void", 0, 999999, "Microns2", "Value for search around defect, smaller width defect, smaller Width value"));
	//pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBlueHWRatioDefect, "Height local search", TINT, "Void", 0, 999999, "Microns2", "Value for search around defect, smaller height defect, smaller Height value"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBlueVoidInnerDefectPercent, "Min. Percent Inner of Defect", TINT, "Void", 0, 100, "", "Min % Area of Inner Defect region/Defect region "));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBlueVoidOffsetCheckDefect, "Offset Defect  ", TINT, "Void", 0, 100, "%", "% Inner Radius of defect to offset (for get type of error)"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBlueVoidMaxContrastInOutDefect, "Max. Difference Contrast Inner-Outer ", TINT, "Void", 0, 255, "", "Difference in brightness between inner and outer of defect, if value < Max contrast, it will be classified in Void." ));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBlueVoidMinGrayDefect, "Min. Contrast", TINT, "Void", 0, 255, "", "Minimum contrast of Void"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBlueVoidMaxGrayDefect, "Max. Contrast", TINT, "Void", 0, 255, "", "Maximum contrast of Void"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDBlueVoidMaxDeviationInnerDefect, "Max. brightness noise inner", TINT, "Void", 0, 255, "", "For get type error, if brightness noise inner of defect < Max brightness, it will be classified in Void."));

	// Blow Hole
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueErosionRecHeight, "Erosion Rec Height", TDOUBLE, "Blow Hole", 0, 999999, "Microns", "Height Offset from encap edge for inspect BlowHole"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueMinMaskSizeSmallBH, "SmallBH Slide Window Side", TDOUBLE, "Blow Hole", 0, 999999, "Microns", "-The Size of Window to catch the blur defect."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueMinSizeSmallBH, "SmallBH Min Size", TDOUBLE, "Blow Hole", 0, 999999, "Microns2", "Minimum Area of BH Defect on Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueMinCircularitySmallBH, "SmallBH Min Circularity", TDOUBLE, "Blow Hole", 0, 1, "", "-Minimum Circularity of Blow Hole; 1=Perfect circle.\n-If the Shape of Defect is similar to a circle, the Defect will be classified in Blow Hole.\n-Should setup this value equal to Max. Circularity of Contamination value."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueMaxLayOnPartPercentageAllowSmallBH, "SmallBH Max Lay On Part Percentage Allow", TDOUBLE, "Blow Hole", 0, 999999, "Microns2", "Maximum length of the defect lay on the encap border allowed"));

	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueMinMaskSizeLargeBH, "LargeBH Slide Window Side", TDOUBLE, "Blow Hole", 0, 999999, "Microns", "-The Size of Window to catch the blur defect."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueMinSizeLargeBH, "LargeBH Min Size", TDOUBLE, "Blow Hole", 0, 999999, "Microns2", "Minimum Area of BH Defect on Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueMinCircularityLargeBH, "LargeBH Min Circularity", TDOUBLE, "Blow Hole", 0, 1, "", "-Minimum Circularity of Blow Hole; 1=Perfect circle.\n-If the Shape of Defect is similar to a circle, the Defect will be classified in Blow Hole.\n-Should setup this value equal to Max. Circularity of Contamination value."));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueMaxLayOnPartPercentageAllowLargeBH, "LargeBH Max Lay On Part Percentage Allow", TDOUBLE, "Blow Hole", 0, 999999, "Microns2", "Maximum length of the defect lay on the encap border allowed"));

	//Wire And IC
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkBlueEncapLightWireContrastWAI, "Light Wire Intensity", TINT, "Wire And IC", 0, 255, "", "Intensity of ther blur defect on Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueEncapLightWireMaxLineWidthWAI, "Light Wire Max Line Width", TDOUBLE, "Wire And IC", 0, 999999, "Microns", "Maximum of Line Width of Light Wire defect on Encap "));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueEncapLightWireMinAreaWAI, "Light Wire Min Area", TDOUBLE, "Wire And IC", 0, 999999, "Microns2", "Minimum Area of Light Wire Defect on Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueEncapLightWireMinLengthWAI, "Light Wire Min Length", TDOUBLE, "Wire And IC", 0, 999999, "Microns", "Minimum Length of Light Wire Defect on Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueEncapLightWireMinMeanGVDiffWAI, "Light Wire Min. GVDiff", TDOUBLE, "Wire And IC", 0, 255, "", "Minimum contrast of GV Difference"));

	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.nDarkBlueEncapFuzzyWireContrastWAI, "Fuzzy Wire Intensity", TINT, "Wire And IC", 0, 255, "", "Intensity of ther blur defect on Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueEncapFuzzyWireMaxLineWidthWAI, "Fuzzy Wire Max Line Width", TDOUBLE, "Wire And IC", 0, 999999, "Microns", "Maximum of Line Width of Fuzzy Wire defect on Encap "));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueEncapFuzzyWireMinAreaWAI, "Fuzzy Min Area", TDOUBLE, "Wire And IC", 0, 999999, "Microns2", "Minimum Area of Fuzzy Wire Defect on Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueEncapFuzzyWireMinLengthWAI, "Fuzzy Wire Min Length", TDOUBLE, "Wire And IC", 0, 999999, "Microns", "Minimum Length of Fuzzy Wire Defect on Encap"));
	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueEncapFuzzyWireMinMeanGVDiffWAI, "Fuzzy Wire Min. GVDiff", TDOUBLE, "Wire And IC", 0, 255, "", "Minimum contrast of GV Difference"));

	pParm->Add(new CParmInfo(&m_Inspection.m_EncapParm.dDarkBlueEncapICMinAreaWAI, "IC Min Area", TDOUBLE, "Wire And IC", 0, 999999, "Microns2", "Minimum Area of IC Defect on Bottom Encap"));	m_Parameters.Add(pParm);

	// Black Contact Vision Parameters
	pParm = new CParameters();
	pParm->strInspection = "Black Contact";
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.bEnableBlackContact, "Enable", TBOOL, "", 0, 1, "", "Enable Black Contact Inspection."));

	// Damage
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dBContactMaskSizeDM, "Mask Size", TDOUBLE, "Damage", 0, 99999, "Microns", "The size of window using to increase contrast and catch Damage defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dBContactContactRegionOffsetDM, "Contact Region Offset", TDOUBLE, "Damage", 0, 99999, "Microns", "The offset using to remove the noise in the edge of Contact region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dBContactMaxLineWidthDM, "Max. Line Width", TDOUBLE, "Damage", 0, 99999, "Microns", "Maximum width of Damge in Contact Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactLowContrastDM, "Low Contrsat", TINT, "Damage", 0, 255, "", "Minimum Contrast of Damage Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactHighContrastDM, "High Contrsat", TINT, "Damage", 0, 255, "", "Maximum Contrast of Damage Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dBContactMinLengthDM, "Min. Length", TDOUBLE, "Damage", 0, 99999, "Microns", "Minimum Length of Damage Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dBContactMaxCircularityDM, "Max. Circularity", TDOUBLE, "Damage", 0, 1, "", "Maximum Circularity of Damage Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactMinDefectCountDM, "Min. Defect Count", TINT, "Damage", 0, 99999, "", "-Minimum Defect of Damage is allowed.\n-If the number of Defect is equal or more than this value, the Defect will be classified in Damage."));

	// Contamination
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactHighContrastCT, "High Contrast", TINT, "Contamination", 0, 255, "", "Minimum Contrast using to catch Contamination Defect region which is high contrast."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dBContactSmoothImageCT, "Smooth Image", TDOUBLE, "Contamination", 0, 99999, "", "This value using to make the image more smoother to reduce noise."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dBContactContactRegionOffsetCT, "Contact Region Offset", TDOUBLE, "Contamination", 0, 99999, "Microns", "The offset using to remove the noise in the edge of Contact region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactContrastOffsetCT, "Contrast Offset", TINT, "Contamination", 0, 255, "", "The offset of Dynamic Contrast using to cacth Contamination Defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactMinHoleNumberCT, "Min. Hole Number", TINT, "Contamination", 0, 99999, "", "If the region after thresholding with Dynamic Contrast has the number holes larger than this value, we will add Hole Number Contrast Offset to threshold again."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactHoleNumberContrastOffsetCT, "Hole Number Contrast Offset", TINT, "Contamination", 0, 255, "", "The offset add to Dynamic Contrast if the Contact Region has a lot of Hole after thresholding."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactVeryHighRoughDotRegionCT, "Very High Rough Dot Region", TINT, "Contamination", 0, 99999, "", "This Value using to check the Rough level of Contact surface, we will add Very High Rough Contrast Offset to Dynamic Contrast to threshold if the surface is very Rough."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactHighRoughDotRegionCT, "High Rough Dot Region", TINT, "Contamination", 0, 99999, "", "This Value using to check the Rough level of Contact surface, we will add High Rough Contrast Offset to Dynamic Contrast to threshold if the surface is normal Rough."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactVeryHighRoughContrastOffsetCT, "Very High Rough Contrast Offset", TINT, "Contamination", 0, 255, "", "The offset add to Dynamic Contrast if the Contact surface is very Rough."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactHighRoughContrastOffsetCT, "High Rough Contrast Offset", TINT, "Contamination", 0, 255, "", "The offset add to Dynamic Contrast if the Contact surface is normal Rough."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dBContactMinStainSizeCT, "Min. Stain Size", TDOUBLE, "Contamination", 0, 99999, "Microns2", "The Minimum Area of Stain Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactMinMeanEdgeGrayCT, "Min. Mean Edge Gray", TINT, "Contamination", 0, 255, "", "The Minimum Mean Gray of Contamination Defect Region in the Edge of Contact Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dBContactMinSizeCT, "Min. Size", TDOUBLE, "Contamination", 0, 99999, "Microns2", "The Minimum Area of Contamination Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactMinMeanGrayCT, "Min. Mean Gray", TINT, "Contamination", 0, 255, "", "The Minimum Mean Gray of Contamination Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dBContactMinAreaTracingPercentCT, "Min. Area Tracing Percent", TDOUBLE, "Contamination", 0, 100, "", "The Minimum Area Tracing/Reference Area of Contact Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nBContactMinDefectCountCT, "Min. Defect Count", TINT, "Contamination", 0, 99999, "", "-Minimum Defect of Contamination is allowed.\n-If the number of Defect is equal or more than this value, the Defect will be classified in Contamination."));

	m_Parameters.Add(pParm);

	// Black Contact Vision Parameters
	pParm = new CParameters();
	pParm->strInspection = "White Contact";
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.bEnableWhiteContact, "Enable", TBOOL, "", 0, 1, "", "Enable White Contact Inspection."));

	// Damage
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dWContactMaskSizeDM, "Mask Size", TDOUBLE, "Damage", 0, 99999, "Microns", "The size of window using to increase contrast and catch Damage defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dWContactContactRegionOffsetDM, "Contact Region Offset", TDOUBLE, "Damage", 0, 99999, "Microns", "The offset using to remove the noise in the edge of Contact region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dWContactMaxLineWidthDM, "Max. Line Width", TDOUBLE, "Damage", 0, 99999, "Microns", "Maximum width of Damge in Contact Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nWContactLowContrastDM, "Low Contrsat", TINT, "Damage", 0, 255, "", "Minimum Contrast of Damage Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nWContactHighContrastDM, "High Contrsat", TINT, "Damage", 0, 255, "", "Maximum Contrast of Damage Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dWContactMinLengthDM, "Min. Length", TDOUBLE, "Damage", 0, 99999, "Microns", "Minimum Length of Damage Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dWContactMaxCircularityDM, "Max. Circularity", TDOUBLE, "Damage", 0, 1, "", "Maximum Circularity of Damage Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nWContactMinDefectCountDM, "Min. Defect Count", TINT, "Damage", 0, 99999, "", "-Minimum Defect of Damage is allowed.\n-If the number of Defect is equal or more than this value, the Defect will be classified in Damage."));

	// Contamination
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dWContactSmoothImageCT, "Smooth Image", TDOUBLE, "Contamination", 0, 99999, "", "This value using to make the image more smoother to reduce noise."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dWContactContactRegionOffsetCT, "Contact Region Offset", TDOUBLE, "Contamination", 0, 99999, "Microns", "The offset using to remove the noise in the edge of Contact region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nWContactWhiteDefectContrastOffsetCT, "White Defect Contrast Offset", TINT, "Contamination", 0, 255, "", "The offset add to Dynamic Contrast using to threshold and catch White Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nWContactMeanWhiteDefectGrayOffsetCT, "Mean White Defect Gray Offset", TINT, "Contamination", 0, 255, "", "The offset add to Dynamic Contrast using to classify Min Mean Gray of Candidate White Defect Region after thresholding with Dynamic Contrast."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nWContactMinMeanWhiteDefectGrayCT, "Min. Mean White Defect Gray", TINT, "Contamination", 0, 255, "", "The Minimum Mean Gray of final White Defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nWContactBlackDefectContrastOffsetCT, "Black Defect Contrast Offset", TINT, "Contamination", 0, 255, "", "The offset add to Dynamic Contrast using to threshold and catch Black Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nWContactMeanBlackDefectGrayOffsetCT, "Mean Black Defect Gray Offset", TINT, "Contamination", 0, 255, "", "The offset add to Dynamic Contrast using to classify Min Mean Gray of Candidate Black Defect Region after thresholding with Dynamic Contrast."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nWContactMaxMeanBlackDefectGrayCT, "Min. Mean Black Defect Gray", TINT, "Contamination", 0, 255, "", "The Maximum Mean Gray of final Black Defect region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dWContactMinSizeCT, "Min. Size", TDOUBLE, "Contamination", 0, 99999, "Microns2", "The Minimum Area of Contamination Defect Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.dWContactMinAreaTracingPercentCT, "Min. Area Tracing Percent", TDOUBLE, "Contamination", 0, 100, "", "The Minimum Area Tracing/Reference Area of Contact Region."));
	pParm->Add(new CParmInfo(&m_Inspection.m_ContactParm.nWContactMinDefectCountCT, "Min. Defect Count", TINT, "Contamination", 0, 99999, "", "-Minimum Defect of Contamination is allowed.\n-If the number of Defect is equal or more than this value, the Defect will be classified in Contamination."));

	m_Parameters.Add(pParm);

	// Flex Vision Parameters
	pParm = new CParameters();
	pParm->strInspection = "Flex";
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.bEnable, "Enable", TBOOL, "", 0, 1, "", "Enable Flex Inspection."));

	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.bEnableCircuitLineCutInspection, "Circuit Line Cut", TBOOL, "Overcut", 0, 1, "", "Enable Circuit Line Cut Inspection."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.bEnableCircuitLineLinkedInspection, "Circuit Line Linked", TBOOL, "Overcut", 0, 1, "", "Enable Circuit Line Linked Inspection."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.dMinCircuitLineWidthOV, "Min. Circuit Line Width", TDOUBLE, "Overcut", 0, 99999, "Microns", "Minimum Width of Circuit Line in Flex."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.dMaxCircuitLineWidthOV, "Max. Circuit Line Width", TDOUBLE, "Overcut", 0, 99999, "Microns", "Maximum Width of Circuit Line in Flex."));

	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.bEnableCircuitLineDamageInspection, "Circuit Line Damage", TBOOL, "Damage", 0, 1, "", "Enable Circuit Line Damage Inspection."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.nMaxContrastDM, "Contrast", TINT, "Damage", 0, 255, "", "Maximum contrast of Damage"));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.dMinSizeDM, "Min. Size", TDOUBLE, "Damage", 0, 99999, "Microns2", "Minimum Area Of Damage."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.nMinDefectCountDM, "Min. Defect Count", TINT, "Damage", 1, 99999, "", "-Minimum Defect of Damage is allowed.\n-If the number of Defect is equal or more than this value, the Defect will be classified in Damage."));

	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.nMinContrastCT, "Contrast", TINT, "Contamination", 0, 255, "", "Minimum contrast of Contamination in background of Flex"));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.nMinSubstrateContrastCT, "Substrate Contrast", TINT, "Contamination", 0, 255, "", "Minimum contrast of Contamination in Substrate Mask"));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.dMinSizeCT, "Min. Size", TDOUBLE, "Contamination", 0, 99999, "Microns2", "Minimum Area Of Contamination."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.dMinSquareSizeCT, "Min. Square Size", TDOUBLE, "Contamination", 0, 99999, "Microns", "-Minimum Width and Height of Contamination\n-If The Width and Height of Defect are more than Min. Square Size value, the Defect will be classified in Contamination.\n-If this value is increase too high, any Defects will not classified in Contamination. Otherwise, all of Defect will be classfied in Contamination."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.dMinLengthCT, "Min. Length", TDOUBLE, "Contamination", 0, 99999, "Microns", "-Minimum Length of Contamination.\n-If the Length of Defect is more than Min. Length value, the Defect will be classified in Contamination.\n-If this value is increase too high, any Defects will not be classified in Contamination. Otherwise, all of Defect will be classfied in Contamination."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.dMaxCircularityCT, "Max. Circularity", TDOUBLE, "Contamination", 0, 1, "", "-Maximum Circularity of Contamination is allowed; 1=Perfect circle.\n-If the Shape of Defect is different from a circle, the Defect will be classified in Contamination."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.nMinDefectCountCT, "Min. Defect Count", TINT, "Contamination", 1, 99999, "", "-Minimum Defect of Contamination is allowed.\n-If the number of Defect is equal or more than this value, the Defect will be classified in Contamination."));

	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.nMinContrastSC, "Contrast", TINT, "Scratches", 0, 255, "", "Minimum contrast of Scratches"));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.dMaxWidthSC, "Max. Scratches Width", TDOUBLE, "Scratches", 0, 99999, "Microns", "Maximum Width of Scratches in Flex."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.dMinLengthSC, "Min. Length", TDOUBLE, "Scratches", 0, 99999, "Microns", "-Minimum Length of Scratches.\n-If the Length of Defect is more than Min. Length value, the Defect will be classified in Scratches.\n-If this value is increase too high, any Defects will not be classified in Scratches. Otherwise, all of Defect will be classfied in Scratches."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.dMaxCircularitySC, "Max. Circularity", TDOUBLE, "Scratches", 0, 1, "", "-Maximum Circularity of Scratches is allowed; 1=Perfect circle.\n-If the Shape of Defect is different from a circle, the Defect will be classified in Scratches."));
	pParm->Add(new CParmInfo(&m_Inspection.m_FlexParm.nMinDefectCountSC, "Min. Defect Count", TINT, "Scratches", 1, 99999, "", "-Minimum Defect of Scratches is allowed.\n-If the number of Defect is equal or more than this value, the Defect will be classified in Scratches."));

	m_Parameters.Add(pParm);


	// Chicklet Swage Hole
	pParm = new CParameters();
	pParm->strInspection = "Chicket Swage Hole";
	pParm->Add(new CParmInfo(&m_Inspection.m_ChickletSwageHole.bEnable, "Enable", TBOOL, "", 0, 1, "", "Enable Chicklet Swage Hole inspection"));
	pParm->Add(new CParmInfo(&m_Inspection.m_ChickletSwageHole.nRoundnessValueDefect, "Min Roundness ", TINT, "Roundness", 0, 100, "", "Current Roundness value < Min Roundness, Roundness error = True"));
	pParm->Add(new CParmInfo(&m_Inspection.m_ChickletSwageHole.nBurrValueForRoundness, "Min Burr ", TINT, "Roundness", 0, 100, "", "Min Burr value for calculate Roundness. If current Burr value < Min Burr, Value will be ignored. "));

	pParm->Add(new CParmInfo(&m_Inspection.m_ChickletSwageHole.nBurrValueDefect, "Min Burr", TINT, "Burr ", 0, 100, "", "Current Burr value > Min Burr, Burr Error = True "));
	m_Parameters.Add(pParm);

}

void CInspectionHandler::CreateTeachParmList(int nTrack/*int nTrack, int nDoc, int nFOV*/)
{
	CleanUpTeach();

	CParameters* pParm;
	CTrainingData* pData = &m_Inspection.m_TrainingData;

	pParm = new CParameters();
	pParm->strInspection = "Device Location";
	pParm->Add(new CParmInfo(&pData->bDeviceLocationEnable, "Enable", TBOOL, "", 0, 1, "", "Specifies pattern search enable/disable for device location inspection"));

	CStringArray strArrayDeviceLocationType;
	strArrayDeviceLocationType.Add("Blob Detection");
	strArrayDeviceLocationType.Add("Edge Detection");
	pParm->Add(new CParmInfo(&pData->nDeviceLocationType, "Method", TENUM, "", 0, DEVICE_LOCATION_TYPE_COUNT - 1, "",
		"Specifies strategy using to detection Device Location.\n1. Blob Detection: Using blob analysis to find the location of device.\n.2. Edge Detection: finding the cross reference point based on the horizontal and vertical line of the device.\nFrom the cross reference point we calculate the shifted information between teach and inspect image", &strArrayDeviceLocationType));

	pParm->Add(new CParmInfo(&pData->dDeviceLocWidth, "Width", TDOUBLE, "Blob Detection", 0, 15000, "Microns", "Minimum Device Width"));
	pParm->Add(new CParmInfo(&pData->dDeviceLocHeight, "Height", TDOUBLE, "Blob Detection", 0, 15000, "Microns", "Min Device Height"));
	pParm->Add(new CParmInfo(&pData->dMaskSize, "Device Gap Mask", TDOUBLE, "Blob Detection", 0, 15000, "Microns", "Min Gap Allowed on Device"));
	pParm->Add(new CParmInfo(&pData->nMinDeviceContrast, "Intensity", TINT, "Blob Detection", 0, 255, "levels", "Min Device Intensity / Grey Level"));
	pParm->Add(new CParmInfo(&pData->bSobelAmp, "Use Sobel Operator", TBOOL, "Blob Detection", 0, 1, "", ""));

	CStringArray strArrayReferenceType;
	strArrayReferenceType.Add("Top Left");
	strArrayReferenceType.Add("Top Right");
	strArrayReferenceType.Add("Bottom Left");
	strArrayReferenceType.Add("Bottom Right");
	strArrayReferenceType.Add("Encap Edge Left");
	strArrayReferenceType.Add("Encap Edge Right");
	strArrayReferenceType.Add("Center");
	pParm->Add(new CParmInfo(&pData->nDeviceReferencePointType, "Device Reference Type", TENUM, "Blob Detection", 0, REF_POINT_TYPE_COUNT - 1, "", "Specifies reference point type to calculate the shifted information between teach and inspect image", &strArrayReferenceType));


	//Edge Detection Option
	CStringArray strArrayDirectionType;
	strArrayDirectionType.Add("Left To Right");
	strArrayDirectionType.Add("Top Top Bottom");
	strArrayDirectionType.Add("Right To Left");
	strArrayDirectionType.Add("Bottom To Top");
	strArrayDirectionType.Add("None");

	pParm->Add(new CParmInfo(&pData->nDeviceHorizontalContrast, "Horizontal Line Contrast", TINT, "Edge Detection", 0, 255, "levels", "Min Edge Contrast To Find The Horizontal Line Of The Device / Grey Level"));
	pParm->Add(new CParmInfo(&pData->nDeviceHorizontalDirection, "Horizontal Line Search Direction", TENUM, "Edge Detection", 0, DIRECTION_TYPE_COUNT - 1, "", "Specifies The Starting Search Direction For The Horizontal Line Of The Device", &strArrayDirectionType));

	pParm->Add(new CParmInfo(&pData->nDeviceVerticalContrast, "Vertical Line Contrast", TINT, "Edge Detection", 0, 255, "levels", "Min Edge Contrast To Find The Vertical Line Of The Device / Grey Level"));
	pParm->Add(new CParmInfo(&pData->nDeviceVerticalDirection, "Vertical Line Search Direction", TENUM, "Edge Detection", 0, DIRECTION_TYPE_COUNT - 1, "", "Specifies The Starting Search Direction For The Vertical Line Of The Device", &strArrayDirectionType));


	m_ParametersTeach.Add(pParm);

	pParm = new CParameters();
	pParm->strInspection = "Datum/Pattern Search";
	pParm->Add(new CParmInfo(&pData->bDattamSearchEnable, "Enable", TBOOL, "", 0, 1, "", "Specifies pattern search enable/disable for pattern inspection"));
	pParm->Add(new CParmInfo(&pData->nDattamSearchNoOfDattams, "Number Of Device Pattern", TINT, "", 1, 4, "", "Specifies number of dattam/pattern for device inspection"));
	pParm->Add(new CParmInfo(&pData->dDattamSearchAreaX, "Search Area Along X", TDOUBLE, "", 0, 6000, "Microns", "Specifies pattern search area in X direction"));
	pParm->Add(new CParmInfo(&pData->dDattamSearchAreaY, "Search Area Along Y", TDOUBLE, "", 0, 6000, "Microns", "Specifies pattern search area in Y direction"));
	pParm->Add(new CParmInfo(&pData->nDattamSearchMaxRotAngle, "Max. Rotation Angle", TINT, "", 0, 360, "degrees", "Specifies maximum allowed device rotation angle"));
	pParm->Add(new CParmInfo(&pData->dDattamSearchMinAcceptanceScore, "Min. Acceptance Score", TDOUBLE, "", 0, 100, "%", "Specifies minimum acceptance for pattern matching"));
	pParm->Add(new CParmInfo(&pData->nDatumPatternType, "Pattern Search Type", TENUM, "", 0, PATTERN_TYPE_COUNT - 1, "", "Specifies pattern searching method use to inspect the datum/pattern", &strArrayPatternSearchType));

	m_ParametersTeach.Add(pParm);

	/* Encap Teach Param */
	pParm = new CParameters();
	pParm->strInspection = "Encap";
	pParm->Add(new CParmInfo(&pData->bEnableEncap, "Enable", TBOOL, "", 0, 1, "", "Enable Encap Teach"));
	// Blue Encap
	pParm->Add(new CParmInfo(&pData->bEnableEncapBlue, "Enable", TBOOL, "Blue Encap Tracing", 0, 1, "", "Enable Blue Encap Teach"));
	pParm->Add(new CParmInfo(&pData->nEncapTracingPolygonPointNumber, "Tracing Polygon Point", TINT, "Blue Encap Tracing", 0, 20, "", "Number of Polygon Point for Tracing Encap"));
	pParm->Add(new CParmInfo(&pData->nEncapTopPolygonPointNumber, "Top Polygon Point", TINT, "Blue Encap Tracing", 0, 20, "", "Number of Polygon Point for Tracing the Top part of Encap"));
	pParm->Add(new CParmInfo(&pData->nEncapBottomPolygonPointNumber, "Bottom Polygon Point", TINT, "Blue Encap Tracing", 0, 20, "Points", "Number of Polygon Point for Tracing the Bottom part of Encap"));
	pParm->Add(new CParmInfo(&pData->dErosionRadiusEncapTracing, "Edge Offset", TDOUBLE, "Blue Encap Tracing", 0, 9999, "Microns", "Inspection offset from encap edge"));
	pParm->Add(new CParmInfo(&pData->dSlideWindowSideEncapTracing, "Slide Window Side", TDOUBLE, "Blue Encap Tracing", 0, 9999, "Microns", "The Size of texture on Base"));
	pParm->Add(new CParmInfo(&pData->nSmoothingIndexEncapTracing, "Smoothing Index", TINT, "Blue Encap Tracing", 0, 9999, "", "The border of Encap Increasingly smooth if increased"));
	pParm->Add(new CParmInfo(&pData->nIntensityEncapTracing, "Max. Intensity", TINT, "Blue Encap Tracing", 0, 255, "Levels", "-Maximum Intensity of Encap.\n-If the Gray Level value of Encap is between Min. Intensity (=0) and Max. Intensity value, the Encap Location will be found.\n-If this value is increased too high, Encap Location will be found in mistake. Otherwise, Encap Location will be not found."));
	pParm->Add(new CParmInfo(&pData->nEdgeFlexContrastEncapTracing, "Edge Flex Contrast", TINT, "Blue Encap Tracing", 0, 255, "", "The contrast for find Edge Flex"));
	pParm->Add(new CParmInfo(&pData->dSizeMaskingEncapTracing, "Edge Offset Masking Encap", TDOUBLE, "Blue Encap Tracing", 0, 255, "", "Inspection offset from encap masking"));
	pParm->Add(new CParmInfo(&pData->nIntensityForFindBorderBaseBlueEncap, "Intensity Border Base", TINT, "Blue Encap Tracing", 0, 255, "levels", "Intensity For Find Border of Base"));
	pParm->Add(new CParmInfo(&pData->dOffsetLeftBlueEncap, "Offset Left Edge Border Base", TDOUBLE, "Blue Encap Tracing", -99999, 99999, "Micron", "Offset for Left Edge Border of Base"));
	pParm->Add(new CParmInfo(&pData->dOffsetRightBlueEncap, "Offset Right Edge Border Base", TDOUBLE, "Blue Encap Tracing", -99999, 99999, "Micron", "Offset for Right Edge Border of Base"));
	
	// Dark Encap
	pParm->Add(new CParmInfo(&pData->bEnableDarkEncap, "Enable", TBOOL, "Dark Encap Tracing", 0, 1, "", "Enable Dark Encap Teach"));
	pParm->Add(new CParmInfo(&pData->m_nLeftRightContrastDarkEncap, "Left Right Contrast ", TINT, "Dark Encap Tracing", 0, 255, "", "Contrast on left right region "));
	pParm->Add(new CParmInfo(&pData->m_nWhiteContrastDarkEncap, "White Contrast", TINT, "Dark Encap Tracing", 0, 255, "", "Value for threshold White region"));
	pParm->Add(new CParmInfo(&pData->m_nRoughContrastDarkEncap, "Rough Deviation Contrast", TINT, "Dark Encap Tracing", 0, 255, "", "Value for threshold deviation gray image of rough region "));
	pParm->Add(new CParmInfo(&pData->m_nContrastFlexDarkEncap, "Flex Difference Contrast", TINT, "Dark Encap Tracing", 0, 255, "", " Difference gray value between flex and encap region"));
	pParm->Add(new CParmInfo(&pData->m_nMinContrastFlexDarkEncap, "Min Encap Contrast", TINT, "Dark Encap Tracing", 0, 255, "", "The Encap contrast for tracing encap on Flex"));
	pParm->Add(new CParmInfo(&pData->m_nMaxContrastFlexDarkEncap, "Max Encap Contrast", TINT, "Dark Encap Tracing", 0, 255, "", "The Encap contrast for tracing encap on Flex"));

	// Dark Blue Encap
	pParm->Add(new CParmInfo(&pData->bEnableDarkBlueEncap, "Enable", TBOOL, "Dark Blue Encap Tracing", 0, 1, "", "Enable Dark Blue Encap Teach"));
	pParm->Add(new CParmInfo(&pData->nDBEncapEdgeEncapContrast, "Edge Encap Contrast", TINT, "Dark Blue Encap Tracing", 0, 255, "Levels", "Minimum Contrast of Edge Encap"));
	pParm->Add(new CParmInfo(&pData->nDBEncapEdgeFlexContrast, "Edge Flex Contrast", TINT, "Dark Blue Encap Tracing", 0, 255, "Levels", "Minimum Contrast of Edge Flex"));
	pParm->Add(new CParmInfo(&pData->nDBEncapMinEncapContrast, "Min Encap Contrast", TINT, "Dark Blue Encap Tracing", 0, 255, "Levels", "Minimum Contrast of Encap"));
	pParm->Add(new CParmInfo(&pData->nDBEncapMaxEncapContrast, "Max Encap Contrast", TINT, "Dark Blue Encap Tracing", 0, 255, "Levels", "Maximum Contrast of Encap"));

	// Mask Tracing
	pParm->Add(new CParmInfo(&pData->nEncapTracingMaskNumber, "Tracing Mask Number", TINT, "Encap Surface Masking", 0, ENCAP_MAX_MASK_SURFACE, "", "Number of Area to be Masked For Encap Tracing"));
	for (int nMask = 0; nMask < ENCAP_MAX_MASK_SURFACE; nMask++) {
		CString strTemp2;
		strTemp2.Format("Polygon Points at Mask %d", nMask + 1);
		pParm->Add(new CParmInfo(&pData->nEncapTracingMaskPolygonPointNumber[nMask], strTemp2, TINT, "Encap Surface Masking", 0, 20, "Points", "Number of " + strTemp2));
	}
	m_ParametersTeach.Add(pParm);

	// Contact Teach Parameters
	pParm = new CParameters();
	pParm->strInspection = "Contact";
	pParm->Add(new CParmInfo(&pData->bEnableContact, "Enable", TBOOL, "", 0, 1, "", "Enable Contact Teach"));
	pParm->Add(new CParmInfo(&pData->nContactMaskNumber, "Contact Mask Number", TINT, "", 0, CONTACT_MAX_REGION_MASK, "", "Number of Mask using to Tracing Contact Region."));
	pParm->Add(new CParmInfo(&pData->nContactRegionMaskNumber, "Contact Region Mask Number", TINT, "", 0, CONTACT_MAX_REGION_MASK, "", "Number of Mask using to Check area of Contact Region after Tracing."));
	
	pParm->Add(new CParmInfo(&pData->bEnableBlackContact, "Enable", TBOOL, "Black Contact", 0, 1, "", "Enable Black Contact Teach"));
	pParm->Add(new CParmInfo(&pData->nBContactContactContrast, "Contact Contrast", TINT, "Black Contact", 0, 255, "Levels", "Maximum Contrast of Black Contact Region"));
	pParm->Add(new CParmInfo(&pData->nBContactCircleContrast, "Circle Contrast", TINT, "Black Contact", 0, 255, "Levels", "Minimum Contrast of Circle Region inside the Black Contact Region."));

	pParm->Add(new CParmInfo(&pData->bEnableWhiteContact, "Enable", TBOOL, "White Contact", 0, 1, "", "Enable Black Contact Teach"));
	pParm->Add(new CParmInfo(&pData->nWContactContactContrast, "Contact Contrast", TINT, "White Contact", 0, 255, "Levels", "Minimum Contrast of White Contact Region"));
	pParm->Add(new CParmInfo(&pData->nWContactCircleContrast, "Circle Contrast", TINT, "White Contact", 0, 255, "Levels", "Maximum Contrast of Circle Region inside the White Contact Region."));

	pParm->Add(new CParmInfo(&pData->nContactTracingMaskNumber, "Tracing Mask Number", TINT, "Contact Surface Masking", 0, CONTACT_MAX_TRACING_MASK, "", "Number of Area to be Masked For Contact Tracing"));
	for (int nMask = 0; nMask < CONTACT_MAX_TRACING_MASK; nMask++) {
		CString strTemp2;
		strTemp2.Format("Polygon Points at Mask %d", nMask + 1);
		pParm->Add(new CParmInfo(&pData->nContactTracingMaskPolygonPointNumber[nMask], strTemp2, TINT, "Contact Surface Masking", 0, 20, "Points", "Number of " + strTemp2));
	}

	m_ParametersTeach.Add(pParm);

	// Flex Teach Parameters
	pParm = new CParameters();
	pParm->strInspection = "Flex";
	pParm->Add(new CParmInfo(&pData->bFlexEnable, "Enable", TBOOL, "", 0, 1, "", "Enable Flex Teach"));
	pParm->Add(new CParmInfo(&pData->bFlexEnableDynamicEdge, "Enable Dynamic Edge", TBOOL, "", 0, 1, "", "Enable Dynamic Left And Right Edge In Flex Tracing"));
	pParm->Add(new CParmInfo(&pData->dFlexEdgeOffset, "Edge Flex Offset", TDOUBLE, "", 0, 99999, "Microns", "Offset Of Edge In Flex Tracing"));
	pParm->Add(new CParmInfo(&pData->nFlexEdgeContrast, "Edge Flex Contrast", TINT, "", 0, 255, "Levels", "Minimum Contrast of Edge Flex"));
	pParm->Add(new CParmInfo(&pData->nFlexCircuitLineContrast, "Circuit Line Contrast", TINT, "", 0, 255, "Levels", "Minimum Contrast of Circuit Line in Flex"));
	pParm->Add(new CParmInfo(&pData->nFlexEncapContrast, "Encap Contrast", TINT, "", 0, 255, "Levels", "Minimum Contrast of Encap in Flex"));
	pParm->Add(new CParmInfo(&pData->dFlexOffsetMaskSize, "Offset Mask Size", TDOUBLE, "", 0, 99999, "Microns", "Radius of Offset Mask for Region of Mask Tracing"));
	pParm->Add(new CParmInfo(&pData->nFlexStraightCircuitLineMaskPolygonPointNumber, "Straight Circuit Line Mask Point", TINT, "", 3, 20, "Points", "Number of Polygon Points at Straight Circuit Line Mask"));

	pParm->Add(new CParmInfo(&pData->nFlexTracingMaskNumber, "Tracing Mask Number", TINT, "Flex Surface Masking", 0, FLEX_MAX_TRACING_MASK, "", "Number of Area to be Masked For Flex Tracing"));
	for (int nMask = 0; nMask < FLEX_MAX_TRACING_MASK; nMask++) {
		CString strTemp2;
		strTemp2.Format("Polygon Points at Mask %d", nMask + 1);
		pParm->Add(new CParmInfo(&pData->nFlexTracingMaskPolygonPointNumber[nMask], strTemp2, TINT, "Flex Surface Masking", 3, 20, "Points", "Number of " + strTemp2));
	}

	pParm->Add(new CParmInfo(&pData->nFlexDamageMaskNumber, "Damage Mask Number", TINT, "Flex Damage Masking", 0, FLEX_MAX_DAMAGE_MASK, "", "Number of Circuit Line Area to be Masked For Damage Inspection"));
	for (int nMask = 0; nMask < FLEX_MAX_DAMAGE_MASK; nMask++) {
		CString strTemp2;
		strTemp2.Format("Polygon Points at Mask %d", nMask + 1);
		pParm->Add(new CParmInfo(&pData->nFlexDamageMaskPolygonPointNumber[nMask], strTemp2, TINT, "Flex Damage Masking", 3, 20, "Points", "Number of " + strTemp2));
	}

	pParm->Add(new CParmInfo(&pData->nFlexSubstrateMaskNumber, "Substrate Mask Number", TINT, "Flex Substrate Masking", 0, FLEX_MAX_DAMAGE_MASK, "", "Number of Circuit Line Area to be Masked For Substrate Inspection"));
	for (int nMask = 0; nMask < FLEX_MAX_DAMAGE_MASK; nMask++) {
		CString strTemp2;
		strTemp2.Format("Polygon Points at Mask %d", nMask + 1);
		pParm->Add(new CParmInfo(&pData->nFlexSubstrateMaskPolygonPointNumber[nMask], strTemp2, TINT, "Flex Substrate Masking", 3, 20, "Points", "Number of " + strTemp2));
	}

	m_ParametersTeach.Add(pParm);



	/* Chicklet Swage Hole Teach Param*/

	pParm = new CParameters();
	pParm->strInspection = "Chicklet Swage Hole";
	pParm->Add(new CParmInfo(&pData->bEnableChickletSwageHole, "Enable", TBOOL, "", 0, 1, "", "Enable Chicket Swage Hole Teach"));
	pParm->Add(new CParmInfo(&pData->m_nChickletMinContrast, "Min. Intensity", TINT, "", 0, 255, "Value", "Minimum Contrast of Chicket Swage Hole"));
	pParm->Add(new CParmInfo(&pData->m_nChickletMaxContrast, "Max. Intensity", TINT, "", 0, 255, "Value", "Maximum Contrast of Chicket Swage Hole"));
	pParm->Add(new CParmInfo(&pData->m_nChickletNumberOfHoles, "Number of Holes", TINT, "", 0, 10, "Value", "Number of Holes"));
	//	pParm->Add(new CParmInfo(&pData->m_nChickletOffsettRadius, "Hole offset (0 - 1000) ", TINT, "", 0, 1000, "Value", "Number of Holes in Chicklet Swage Hole"));

	m_ParametersTeach.Add(pParm);

}

void CInspectionHandler::CleanUp()
{
	CParameters *pParmsTemp;
	if (!m_Parameters.IsEmpty()) {
		for (int i = 0; i < m_Parameters.GetCount(); i++)
		{
			pParmsTemp = m_Parameters.GetAt(i);
			for (int j = 0; j < pParmsTemp->arrParmInfo.GetCount(); j++) {
				delete pParmsTemp->arrParmInfo[j];
			}
			pParmsTemp->Clean();
			delete pParmsTemp;
		}
		m_Parameters.RemoveAll();
	}
}

void CInspectionHandler::CleanUpTeach()
{
	CParameters *pParmsTemp;
	if (!m_ParametersTeach.IsEmpty()) {
		for (int i = 0; i < m_ParametersTeach.GetCount(); i++)
		{
			pParmsTemp = m_ParametersTeach.GetAt(i);
			for (int j = 0; j < pParmsTemp->arrParmInfo.GetCount(); j++) {
				delete pParmsTemp->arrParmInfo[j];
			}
			pParmsTemp->Clean();
			delete pParmsTemp;
		}
		m_ParametersTeach.RemoveAll();
	}
}

void CInspectionHandler::CleanUpPVI()
{
	/*CParameters *pParmsTemp;
	if(!m_ParametersInspSetUP.IsEmpty()) {
		for (int i = 0; i < m_ParametersInspSetUP.GetCount(); i++) {
			pParmsTemp = m_ParametersInspSetUP.GetAt(i);
			for (int j = 0; j < pParmsTemp->arrParmInfo.GetCount(); j++) {
				delete pParmsTemp->arrParmInfo[j];
			}
			pParmsTemp->Clean();
			delete pParmsTemp;
		}
		m_ParametersInspSetUP.RemoveAll();
	}
	if (!m_ParametersEdgePviInspSetUP.IsEmpty()) {
		for (int i = 0; i < m_ParametersEdgePviInspSetUP.GetCount(); i++) {
			pParmsTemp = m_ParametersEdgePviInspSetUP.GetAt(i);
			for (int j = 0; j < pParmsTemp->arrParmInfo.GetCount(); j++) {
				delete pParmsTemp->arrParmInfo[j];
			}
			pParmsTemp->Clean();
			delete pParmsTemp;
		}
		m_ParametersEdgePviInspSetUP.RemoveAll();
	}

	if (!m_Inspection.m_pPviEdgeInspection.m_ParametersDefect.IsEmpty()) {
		for (int i = 0; i < m_Inspection.m_pPviEdgeInspection.m_ParametersDefect.GetCount(); i++) {
			pParmsTemp = m_Inspection.m_pPviEdgeInspection.m_ParametersDefect.GetAt(i);
			for (int i = 0; i < pParmsTemp->arrParmInfo.GetCount(); i++) {
				delete pParmsTemp->arrParmInfo[i];
			}
			pParmsTemp->Clean();
			delete pParmsTemp;
		}

		m_Inspection.m_pPviEdgeInspection.m_ParametersDefect.RemoveAll();
	}
	for (int i = 0; i < 10; i++) {
		if (!m_Inspection.m_pPviInspectionSetup[i].m_ParametersDefect.IsEmpty()) {
			for (int k = 0; k < m_Inspection.m_pPviInspectionSetup[i].m_ParametersDefect.GetCount(); k++) {
				pParmsTemp = m_Inspection.m_pPviInspectionSetup[i].m_ParametersDefect.GetAt(k);
				for (int j = 0; j < pParmsTemp->arrParmInfo.GetCount(); j++) {
					delete pParmsTemp->arrParmInfo[j];
				}
				pParmsTemp->Clean();
				delete pParmsTemp;
			}
		}
		m_Inspection.m_pPviInspectionSetup[i].m_ParametersDefect.RemoveAll();
	}*/
}

int CInspectionHandler::SaveInspectParameters(CString strDirectoryName, CString strFileName, int nFOV)
{
	CIniFile iniFile;
	CString strDataFileName;

	strDataFileName.Format("%s\\InspectionParameters_Pos%d.insp", strDirectoryName, nFOV + 1);
	iniFile.SetPath(strDataFileName);

	CString strSec, strVariable;
	strSec.Format("Inspection Data");
	CInspectinonData* pData = &m_Inspection.m_InspectionData;

	int nCount = (int)m_Parameters.GetSize();
	for (int i = 0; i < nCount; i++)
		(m_Parameters.GetAt(i))->SaveParameters(&iniFile);

	iniFile.WriteFile();

	return 0;
}

int CInspectionHandler::LoadInspectParameters(CString strDirectoryName, CString strFileName, int nFOV)
{
	CIniFile iniFile;
	CString strDataFileName;

	strDataFileName.Format("%s\\InspectionParameters_Pos%d.insp", strDirectoryName, nFOV + 1);
	iniFile.SetPath(strDataFileName);

	if (iniFile.ReadFile()) {
		CString strSec, strVariable;
		strSec.Format("Inspection Data");
		CInspectinonData* pData = &m_Inspection.m_InspectionData;

		int nCount = (int)m_Parameters.GetSize();
		for (int i = 0; i < nCount; i++)
			(m_Parameters.GetAt(i))->LoadParameters(&iniFile);
	}
	/*else {
		OutputDebugLog("%s not found. Default values assumed", strFileName);
		SaveInspectParameters(strDirectoryName, strFileName);
	}*/

	return 0;
}

int CInspectionHandler::SaveTeachParameters(CString strDirectoryName, CString strFileName, int nFOV)
{
	CIniFile iniFile;
	CString strDataFileName;

	strDataFileName.Format("%s\\TeachParameters_Pos%d.tch", strDirectoryName, nFOV + 1);
	iniFile.SetPath(strDataFileName);
	SetTeachParameters(&iniFile);
	iniFile.WriteFile();

	return 0;
}

int CInspectionHandler::LoadTeachParameters(CString strDirectoryName, CString strFileName, int nFOV)
{
	CIniFile iniFile;
	CString strDataFileName;

	strDataFileName.Format("%s\\TeachParameters_Pos%d.tch", strDirectoryName, nFOV + 1);
	iniFile.SetPath(strDataFileName);

	if (iniFile.ReadFile()) {
		GetTeachParameters(&iniFile);
	}
	else {
		// File Not Found
		/*OutputDebugLog("%s not found. Default values assumed",strDataFileName);
		SetTeachParameters(&iniFile);
		iniFile.WriteFile();*/
		return -1;
	}

	return 0;
}

int CInspectionHandler::GetTeachParameters(CIniFile *ptrIniFile)
{
	CString strSec, strVariable;
	strSec.Format("Training Data");

	CTrainingData* pData = &m_Inspection.m_TrainingData;

	//pData->nCuvetteRadiusRa = ptrIniFile->GetInteger(strSec, "Cuvette Radius_Ra", pData->nCuvetteRadiusRa);
	//pData->nCuvetteRadiusRb = ptrIniFile->GetInteger(strSec, "Cuvette Radius_Rb", pData->nCuvetteRadiusRb);
	//pData->nLensRadius      = ptrIniFile->GetInteger(strSec, "Lens Radius",       pData->nLensRadius);

	int nCount = (int)m_ParametersTeach.GetSize();
	for (int i = 0; i < nCount; i++)
		(m_ParametersTeach.GetAt(i))->LoadParameters(ptrIniFile);

	// Device Loc //
	strVariable.Format("TeachDeviceLoc Rect");
	pData->m_rectDeviceLoc = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDeviceLoc);

	// Device Horizontal Line Loc //
	strVariable.Format("TeachDeviceHorizontalLineLoc Rect");
	pData->m_rectDeviceHorizontalLineLoc = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDeviceHorizontalLineLoc);

	// Device Vertical Line Loc //
	strVariable.Format("TeachDeviceVerticalLineLoc Rect");
	pData->m_rectDeviceVerticalLineLoc = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDeviceVerticalLineLoc);

	// Dattam/Pattern Search //
	for (int nRectDattam = 0; nRectDattam < 4; nRectDattam++) {
		strVariable.Format("TeachDattam/PatternSearch Rect%d", nRectDattam + 1);
		pData->m_rectDattamSearch[nRectDattam] = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDattamSearch[nRectDattam]);
	}

	// // // Blue Encap Region // // //
	// // Blue Encap Tracing
	strVariable.Format("Encap Tracing Polygon Point Count");
	pData->nEncapTracingPolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nEncapTracingPolygonPointNumber);
	if (pData->nEncapTracingPolygonPointNumber > 20)
		pData->nEncapTracingPolygonPointNumber = 20;
	else if (pData->nEncapTracingPolygonPointNumber < 3)
		pData->nEncapTracingPolygonPointNumber = 3;
	for (int j = 0; j < pData->nEncapTracingPolygonPointNumber; j++) {
		strVariable.Format("Encap Tracing PolygonPoint%d", j + 1);
		pData->m_EncapTracingPolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_EncapTracingPolygonPointArea[j]);
	}

	strVariable.Format("Encap Top Polygon Point Count");
	pData->nEncapTopPolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nEncapTopPolygonPointNumber);
	if (pData->nEncapTopPolygonPointNumber > 20)
		pData->nEncapTopPolygonPointNumber = 20;
	else if (pData->nEncapTopPolygonPointNumber < 3)
		pData->nEncapTopPolygonPointNumber = 3;
	for (int j = 0; j < pData->nEncapTopPolygonPointNumber; j++) {
		strVariable.Format("Encap Top PolygonPoint%d", j + 1);
		pData->m_EncapTopPolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_EncapTopPolygonPointArea[j]);
	}

	strVariable.Format("Encap Bottom Polygon Point Count");
	pData->nEncapBottomPolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nEncapBottomPolygonPointNumber);
	if (pData->nEncapBottomPolygonPointNumber > 20)
		pData->nEncapBottomPolygonPointNumber = 20;
	else if (pData->nEncapBottomPolygonPointNumber < 3)
		pData->nEncapBottomPolygonPointNumber = 3;

	for (int j = 0; j < pData->nEncapBottomPolygonPointNumber; j++) {
		strVariable.Format("Encap Bottom PolygonPoint%d", j + 1);
		pData->m_EncapBottomPolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_EncapBottomPolygonPointArea[j]);
	}

	strVariable.Format("Rect Find Left Line Flex for Generating Masking Encap");
	pData->m_rectEncapMaskingLeftPolygonPointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectEncapMaskingLeftPolygonPointArea);

	strVariable.Format("Rect Find Right Line Flex for Generating Masking Encap");
	pData->m_rectEncapMaskingRightPolygonPointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectEncapMaskingRightPolygonPointArea);

	strVariable.Format("Rect Find Limit Line Flex for Generating Masking Encap");
	pData->m_rectEncapMaskingLimitPolygonPointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectEncapMaskingLimitPolygonPointArea);
	// // Insufficient
	// Left Encap Region For Checking Insufficient
	strVariable.Format("Rect Left Encap Region For Checking Insufficient Blue Encap");
	pData->m_rectBlueEncapInsuffLeftRegionPolygonPointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectBlueEncapInsuffLeftRegionPolygonPointArea);

	// Right Encap Region For Checking Insufficient
	strVariable.Format("Rect Right Encap Region For Checking Insufficient Blue Encap");
	pData->m_rectBlueEncapInsuffRightRegionPolygonPointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectBlueEncapInsuffRightRegionPolygonPointArea);

	//  Region For Find border of the base
	strVariable.Format("Rect Region For Find border of the base Insufficient Blue Encap");
	pData->m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea);

	// Middle Encap Region For Checking Insufficient
	strVariable.Format("Middle Blue Encap Region For Checking Insufficient Polygon Point Count");
	pData->nBlueEncapInsuffMidRegionPolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nBlueEncapInsuffMidRegionPolygonPointNumber);
	if (pData->nBlueEncapInsuffMidRegionPolygonPointNumber > 20)
		pData->nBlueEncapInsuffMidRegionPolygonPointNumber = 20;
	else if (pData->nBlueEncapInsuffMidRegionPolygonPointNumber < 3)
		pData->nBlueEncapInsuffMidRegionPolygonPointNumber = 3;

	for (int j = 0; j < pData->nBlueEncapInsuffMidRegionPolygonPointNumber; j++) {
		strVariable.Format("Middle Blue Encap Region For Checking Insufficient PolygonPoint%d", j + 1);
		pData->m_BlueEncapInsuffMidRegionPolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_BlueEncapInsuffMidRegionPolygonPointArea[j]);
	}
	// // // // Excess
	// // Left Encap Region For Checking Excess
	strVariable.Format("Rect Left Encap Region For Checking Excess Blue Encap");
	pData->m_rectBlueEncapExcessLeftRegionPolygonPointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectBlueEncapExcessLeftRegionPolygonPointArea);
	// // Right Encap Region For Checking Excess
	strVariable.Format("Rect Right Encap Region For Checking Excess Blue Encap");
	pData->m_rectBlueEncapExcessRightRegionPolygonPointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectBlueEncapExcessRightRegionPolygonPointArea);

	// Glue - Mason
	for (int k = 0; k < ENCAP_GLUE_MASK_POINT; k++) {
		strVariable.Format("TeachBEncapGlueMask PolygonPoint%d", k + 1);
		pData->m_polygonBEncapGlueMask[k] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonBEncapGlueMask[k]);
	}

	// Contamination - Mason
	strVariable.Format("TeachBEncap Flex Edge Mask Rect");
	pData->m_rectBEncapFlexEdgeMaskROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectBEncapFlexEdgeMaskROI);

	// // Blue Encap Wire And Ic
	strVariable.Format("Encap Wire Polygon Point Count");
	pData->nEncapWirePolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nEncapWirePolygonPointNumber);
	if (pData->nEncapWirePolygonPointNumber > 20)
		pData->nEncapWirePolygonPointNumber = 20;
	else if (pData->nEncapWirePolygonPointNumber < 3)
		pData->nEncapWirePolygonPointNumber = 3;

	for (int j = 0; j < pData->nEncapWirePolygonPointNumber; j++) {
		strVariable.Format("Encap Wire PolygonPoint%d", j + 1);
		pData->m_EncapWirePolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_EncapWirePolygonPointArea[j]);
	}

	strVariable.Format("Encap Wire Bottom Polygon Point Count");
	pData->nEncapWireBottomPolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nEncapWireBottomPolygonPointNumber);
	if (pData->nEncapWireBottomPolygonPointNumber > 20)
		pData->nEncapWireBottomPolygonPointNumber = 20;
	else if (pData->nEncapWireBottomPolygonPointNumber < 3)
		pData->nEncapWireBottomPolygonPointNumber = 3;

	for (int j = 0; j < pData->nEncapWireBottomPolygonPointNumber; j++) {
		strVariable.Format("Encap Wire Bottom PolygonPoint%d", j + 1);
		pData->m_EncapWireBottomPolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_EncapWireBottomPolygonPointArea[j]);
	}

	strVariable.Format("Encap IC Polygon Point Count");
	pData->nEncapICPolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nEncapICPolygonPointNumber);
	if (pData->nEncapICPolygonPointNumber > 20)
		pData->nEncapICPolygonPointNumber = 20;
	else if (pData->nEncapICPolygonPointNumber < 3)
		pData->nEncapICPolygonPointNumber = 3;

	for (int j = 0; j < pData->nEncapICPolygonPointNumber; j++) {
		strVariable.Format("Encap IC PolygonPoint%d", j + 1);
		pData->m_EncapICPolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_EncapICPolygonPointArea[j]);
	}
	// // // // 
	strVariable.Format("TeachPVIEncap  DarkTopRect");
	pData->m_rectDarkTopEncap = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkTopEncap);
	strVariable.Format("TeachPVIEncap  DarkTopWidth");
	pData->m_nDarkTopRectWidth = ptrIniFile->GetInteger(strSec, strVariable, pData->m_nDarkTopRectWidth);
	strVariable.Format("TeachPVIEncap  DarkTopHeight");
	pData->m_nDarkTopRectHeight = ptrIniFile->GetInteger(strSec, strVariable, pData->m_nDarkTopRectHeight);

	strVariable.Format("TeachPVIEncap  DarkBottomRect");
	pData->m_rectDarkBottomEncap = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkBottomEncap);
	strVariable.Format("TeachPVIEncap  DarkBottomWidth");
	pData->m_nDarkBottomRectWidth = ptrIniFile->GetInteger(strSec, strVariable, pData->m_nDarkBottomRectWidth);
	strVariable.Format("TeachPVIEncap  DarkBottomHeight");
	pData->m_nDarkBottomRectHeight = ptrIniFile->GetInteger(strSec, strVariable, pData->m_nDarkBottomRectHeight);

	strVariable.Format("TeachPVIEncap  DarkLeftRect");
	pData->m_rectDarkLeftEncap = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkLeftEncap);
	strVariable.Format("TeachPVIEncap  DarkLeftWidth");
	pData->m_nDarkLeftRectWidth = ptrIniFile->GetInteger(strSec, strVariable, pData->m_nDarkLeftRectWidth);
	strVariable.Format("TeachPVIEncap  DarkLeftHeight");
	pData->m_nDarkLeftRectHeight = ptrIniFile->GetInteger(strSec, strVariable, pData->m_nDarkLeftRectHeight);

	strVariable.Format("TeachPVIEncap  DarkRightRect");
	pData->m_rectDarkRightEncap = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkRightEncap);
	strVariable.Format("TeachPVIEncap  DarkRightWidth");
	pData->m_nDarkRightRectWidth = ptrIniFile->GetInteger(strSec, strVariable, pData->m_nDarkRightRectWidth);
	strVariable.Format("TeachPVIEncap  DarkRightHeight");
	pData->m_nDarkRightRectHeight = ptrIniFile->GetInteger(strSec, strVariable, pData->m_nDarkRightRectHeight);

	for (int k = 0; k < ENCAP_MAX_MASK_SURFACE; k++) {
		strVariable.Format("Encap Surface Mask Polygon Point Count%d", k + 1);
		pData->nEncapTracingMaskPolygonPointNumber[k] = ptrIniFile->GetInteger(strSec, strVariable, pData->nEncapTracingMaskPolygonPointNumber[k]);
		if (pData->nEncapTracingMaskPolygonPointNumber[k] > 20)
			pData->nEncapTracingMaskPolygonPointNumber[k] = 20;
		else if (pData->nEncapTracingMaskPolygonPointNumber[k] < 3)
			pData->nEncapTracingMaskPolygonPointNumber[k] = 3;

		for (int j = 0; j < pData->nEncapTracingMaskPolygonPointNumber[k]; j++) {
			strVariable.Format("Encap Surface Mask PolygonPos%dPoint%d", k + 1, j + 1);
			pData->m_EncapTracingMaskPolygonPointArea[k][j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_EncapTracingMaskPolygonPointArea[k][j]);
		}
	}

	// // // Contact Region // // //
	// Contact Mask
	for (int k = 0; k < CONTACT_MAX_REGION_MASK; k++) {
		strVariable.Format("TeachContact Contact Mask Rect%d", k + 1);
		pData->m_rectContactMask[k] = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectContactMask[k]);
	}

	// Contact Region Mask
	for (int k = 0; k < CONTACT_MAX_REGION_MASK; k++) {
		strVariable.Format("TeachContact Contact Region Mask Rect%d", k + 1);
		pData->m_rectContactRegionMask[k] = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectContactRegionMask[k]);
	}

	// Contact Tracing Mask
	for (int k = 0; k < CONTACT_MAX_TRACING_MASK; k++) {
		strVariable.Format("TeachContact Tracing Mask Polygon Point Count%d", k + 1);
		pData->nContactTracingMaskPolygonPointNumber[k] = ptrIniFile->GetInteger(strSec, strVariable, pData->nContactTracingMaskPolygonPointNumber[k]);
		if (pData->nContactTracingMaskPolygonPointNumber[k] > CONTACT_MAX_POLYGON_POINT)
			pData->nContactTracingMaskPolygonPointNumber[k] = CONTACT_MAX_POLYGON_POINT;
		else if (pData->nContactTracingMaskPolygonPointNumber[k] < 3)
			pData->nContactTracingMaskPolygonPointNumber[k] = 3;

		for (int j = 0; j < pData->nContactTracingMaskPolygonPointNumber[k]; j++) {
			strVariable.Format("TeachContact Tracing Mask PolygonPos%dPoint%d", k + 1, j + 1);
			pData->m_polygonContactTracingMask[k][j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonContactTracingMask[k][j]);
		}
	}


	// // // Flex Region // // //
	// Load Substrate Pattern And Search Substrate Pattern ROI
	strVariable.Format("TeachFlex Substrate Pattern ROI 1 Rect");
	pData->m_rectFlexSubstratePatternROI1 = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectFlexSubstratePatternROI1);
	strVariable.Format("TeachFlex Substrate Pattern ROI 2 Rect");
	pData->m_rectFlexSubstratePatternROI2 = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectFlexSubstratePatternROI2);
	strVariable.Format("TeachFlex Substrate Search Pattern ROI Rect");
	pData->m_rectFlexSubstrateSearchPatternROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectFlexSubstrateSearchPatternROI);

	// Load Edge Line Of Flex
	for (int j = 0; j < 2; j++) {
		strVariable.Format("TeachFlex Top Edge Line Point%d", j + 1);
		pData->m_polygonFlexTopEdgeLine[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonFlexTopEdgeLine[j]);
	}
	for (int j = 0; j < 2; j++) {
		strVariable.Format("TeachFlex Bottom Edge Line Point%d", j + 1);
		pData->m_polygonFlexBottomEdgeLine[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonFlexBottomEdgeLine[j]);
	}
	for (int j = 0; j < 2; j++) {
		strVariable.Format("TeachFlex Left Edge Line Point%d", j + 1);
		pData->m_polygonFlexLeftEdgeLine[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonFlexLeftEdgeLine[j]);
	}
	for (int j = 0; j < 2; j++) {
		strVariable.Format("TeachFlex Right Edge Line Point%d", j + 1);
		pData->m_polygonFlexRightEdgeLine[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonFlexRightEdgeLine[j]);
	}

	// Load Flex Edge Line Mask
	strVariable.Format("TeachFlex Left Edge Line Mask Rect");
	pData->m_rectFlexLeftEdgeLineMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectFlexLeftEdgeLineMask);
	strVariable.Format("TeachFlex Right Edge Line Mask Rect");
	pData->m_rectFlexRightEdgeLineMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectFlexRightEdgeLineMask);

	// Load Flex Bottom ROI
	strVariable.Format("TeachFlex Bottom Rect");
	pData->m_rectFlexBottomROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectFlexBottomROI);

	// Load Flex Straight Circuit Line Mask
	for (int j = 0; j < pData->nFlexStraightCircuitLineMaskPolygonPointNumber; j++) {
		strVariable.Format("TeachFlex Straight Circuit Line Mask Point%d", j + 1);
		pData->m_polygonFlexStraightCircuitLineMask[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonFlexStraightCircuitLineMask[j]);
	}

	// Load Flex Number Mask
	strVariable.Format("TeachFlex Number Mask Rect");
	pData->m_rectFlexNumberMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectFlexNumberMask);

	// Load Flex Tracing Mask
	for (int k = 0; k < FLEX_MAX_TRACING_MASK; k++) {
		// Tracing Mask Polygon
		strVariable.Format("TeachFlex Tracing Mask Polygon Point Count%d", k + 1);
		pData->nFlexTracingMaskPolygonPointNumber[k] = ptrIniFile->GetInteger(strSec, strVariable, pData->nFlexTracingMaskPolygonPointNumber[k]);
		if (pData->nFlexTracingMaskPolygonPointNumber[k] > FLEX_MAX_POLYGON_POINT)
			pData->nFlexTracingMaskPolygonPointNumber[k] = FLEX_MAX_POLYGON_POINT;
		else if (pData->nFlexTracingMaskPolygonPointNumber[k] < 3)
			pData->nFlexTracingMaskPolygonPointNumber[k] = 3;

		for (int j = 0; j < pData->nFlexTracingMaskPolygonPointNumber[k]; j++) {
			strVariable.Format("TeachFlex Tracing Mask PolygonPos%dPoint%d", k + 1, j + 1);
			pData->m_polygonFlexTracingMask[k][j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonFlexTracingMask[k][j]);
		}
	}

	// Load Flex Damage Mask
	for (int k = 0; k < FLEX_MAX_DAMAGE_MASK; k++) {
		// Damage Mask Polygon
		strVariable.Format("TeachFlex Damage Mask Polygon Point Count%d", k + 1);
		pData->nFlexDamageMaskPolygonPointNumber[k] = ptrIniFile->GetInteger(strSec, strVariable, pData->nFlexDamageMaskPolygonPointNumber[k]);
		if (pData->nFlexDamageMaskPolygonPointNumber[k] > FLEX_MAX_POLYGON_POINT)
			pData->nFlexDamageMaskPolygonPointNumber[k] = FLEX_MAX_POLYGON_POINT;
		else if (pData->nFlexDamageMaskPolygonPointNumber[k] < 3)
			pData->nFlexDamageMaskPolygonPointNumber[k] = 3;

		for (int j = 0; j < pData->nFlexDamageMaskPolygonPointNumber[k]; j++) {
			strVariable.Format("TeachFlex Damage Mask PolygonPos%dPoint%d", k + 1, j + 1);
			pData->m_polygonFlexDamageMask[k][j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonFlexDamageMask[k][j]);
		}
	}

	// Load Flex Substrate Mask
	for (int k = 0; k < FLEX_MAX_SUBSTRATE_MASK; k++) {
		// Substrate Mask Polygon
		strVariable.Format("TeachFlex Substrate Mask Polygon Point Count%d", k + 1);
		pData->nFlexSubstrateMaskPolygonPointNumber[k] = ptrIniFile->GetInteger(strSec, strVariable, pData->nFlexSubstrateMaskPolygonPointNumber[k]);
		if (pData->nFlexSubstrateMaskPolygonPointNumber[k] > FLEX_MAX_POLYGON_POINT)
			pData->nFlexSubstrateMaskPolygonPointNumber[k] = FLEX_MAX_POLYGON_POINT;
		else if (pData->nFlexSubstrateMaskPolygonPointNumber[k] < 3)
			pData->nFlexSubstrateMaskPolygonPointNumber[k] = 3;

		for (int j = 0; j < pData->nFlexSubstrateMaskPolygonPointNumber[k]; j++) {
			strVariable.Format("TeachFlex Substrate Mask PolygonPos%dPoint%d", k + 1, j + 1);
			pData->m_polygonFlexSubstrateMask[k][j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonFlexSubstrateMask[k][j]);
		}
	}



	// Chicklet Swage Hole 
	for (int k = 0; k < pData->m_nChickletNumberOfHoles; k++) {
		strVariable.Format("ChickletROI  Chicklet Swage Hole ROI %d", k + 1);
		pData->m_rectChickletROI[k] = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectChickletROI[k]);
	}

	// // // Dark Encap Region // // //
//	strVariable.Format("Encap Inside Polygon Point Count");
//	pData->nDarkEncapInsidePolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nDarkEncapInsidePolygonPointNumber);
	for (int j = 0; j < pData->nDarkEncapInsidePolygonPointNumber; j++) {
		strVariable.Format("Encap Inside PolygonPoint%d", j + 1);
		pData->m_DarkEncapInsidePolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_DarkEncapInsidePolygonPointArea[j]);
	}

	//strVariable.Format("Encap Outside Polygon Point Count");
	//ptrIniFile->GetInteger(strSec, strVariable, pData->nDarkEncapOutsidePolygonPointNumber);
	for (int j = 0; j < pData->nDarkEncapOutsidePolygonPointNumber; j++) {
		strVariable.Format("Encap Outside PolygonPoint%d", j + 1);
		pData->m_DarkEncapOutsidePolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_DarkEncapOutsidePolygonPointArea[j]);
	}
	strVariable.Format("Rect Find All Dark Encap Region");
	pData->m_rectAllRegionDarkEncap = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectAllRegionDarkEncap);
	strVariable.Format("Rect Find Flex Dark Encap region");
	pData->m_rectTopFlexDarkEncap = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectTopFlexDarkEncap);
	strVariable.Format("Rect White Region Remove Dark Encap region");
	pData->m_rectBotWhiteDarkEncap = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectBotWhiteDarkEncap);

	// Excess - Magnus
	strVariable.Format("TeachDarkLeftExcessMask Rect");
	pData->m_rectDarkExcessLeftMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkExcessLeftMask);

	strVariable.Format("TeachDarkRightExcessMask Rect");
	pData->m_rectDarkExcessRightMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkExcessRightMask);

	// Insufficient - Magnus
	strVariable.Format("TeachDarkCoverInsufficientMask Rect");
	pData->m_rectDarkInsufficientCoverMask= ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkInsufficientCoverMask);

	for (int k = 0; k < DARK_ENCAP_INNER_INSUFFICIENT_MASK_POINT; k++) {
		strVariable.Format("TeachDarkInnerInsufficientMask PolygonPoint%d", k + 1);
		pData->m_polygonDarkInsufficientMidMask[k] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonDarkInsufficientMidMask[k]);
	}

	strVariable.Format("TeachDarkLeftInsufficientMask Rect");
	pData->m_rectDarkInsufficientLeftMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkInsufficientLeftMask);

	strVariable.Format("TeachDarkRightInsufficientMask Rect");
	pData->m_rectDarkInsufficientRightMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkInsufficientRightMask);
	// Glue - Mason
	strVariable.Format("TeachDarkGlueMask Rect");
	pData->m_rectDarkGlueMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkGlueMask);

	// Contamination - Mason
	strVariable.Format("TeachDarkFlexEdgeMask Rect");
	pData->m_rectDarkFlexEdgeMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkFlexEdgeMask);

	strVariable.Format("TeachDarkBottomEdgeMask Rect");
	pData->m_rectDarkBottomEdgeMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkBottomEdgeMask);

	strVariable.Format("TeachDarkRightEdgeMask Rect");
	pData->m_rectDarkRightEdgeMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkRightEdgeMask);

	strVariable.Format("TeachDarkInnerMask Rect");
	pData->m_rectDarkInnerMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkInnerMask);

	// Void
	strVariable.Format("Teach Dark Encap Void Masking Left Die Point Area Rect");
	pData->m_rectDarkEncapVoidMaskingLeftDiePointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkEncapVoidMaskingLeftDiePointArea);

	strVariable.Format("Teach Dark Encap Void Masking Right Die Point Area Rect");
	pData->m_rectDarkEncapVoidMaskingRightDiePointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkEncapVoidMaskingRightDiePointArea);

	// Wire And IC
	strVariable.Format("Dark Encap Wire Polygon Point Count");
	pData->nDarkEncapWirePolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nDarkEncapWirePolygonPointNumber);
	if (pData->nDarkEncapWirePolygonPointNumber > 20)
		pData->nDarkEncapWirePolygonPointNumber = 20;
	else if (pData->nDarkEncapWirePolygonPointNumber < 3)
		pData->nDarkEncapWirePolygonPointNumber = 3;

	for (int j = 0; j < pData->nDarkEncapWirePolygonPointNumber; j++) {
		strVariable.Format("Dark Encap Wire PolygonPoint%d", j + 1);
		pData->m_DarkEncapWirePolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_DarkEncapWirePolygonPointArea[j]);
	}

	strVariable.Format("Dark Encap IC Polygon Point Count");
	pData->nDarkEncapICPolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nDarkEncapICPolygonPointNumber);
	if (pData->nDarkEncapICPolygonPointNumber > 20)
		pData->nDarkEncapICPolygonPointNumber = 20;
	else if (pData->nDarkEncapICPolygonPointNumber < 3)
		pData->nDarkEncapICPolygonPointNumber = 3;

	for (int j = 0; j < pData->nDarkEncapICPolygonPointNumber; j++) {
		strVariable.Format("Dark Encap IC PolygonPoint%d", j + 1);
		pData->m_DarkEncapICPolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_DarkEncapICPolygonPointArea[j]);
	}	// // // Dark Blue Encap Region // // //
	// Load Encap Extract ROI From Configuration File
	for (int k = 0; k < DARK_BLUE_ENCAP_ROI_POINT; k++) {
		strVariable.Format("TeachDBEncapROI PolygonPoint%d", k + 1);
		pData->m_polygonDBEncapExtractROI[k] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonDBEncapExtractROI[k]);
	}
	// Load Inner Encap ROI
	strVariable.Format("TeachDBEncap Inner Rect");
	pData->m_rectDBEncapInnerROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBEncapInnerROI);

	// Load Encap Top Edge Mask ROI
	strVariable.Format("TeachDBEncap Top Edge Mask Rect");
	pData->m_rectDBEncapTopEdgeMaskROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBEncapTopEdgeMaskROI);

	// Load Encap Bottom Edge Mask ROI
	strVariable.Format("TeachDBEncap Bottom Edge Mask Rect");
	pData->m_rectDBEncapBottomEdgeMaskROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBEncapBottomEdgeMaskROI);

	// Load Encap Left Edge Mask ROI
	strVariable.Format("TeachDBEncap Left Edge Mask Rect");
	pData->m_rectDBEncapLeftEdgeMaskROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBEncapLeftEdgeMaskROI);

	// Load Encap Right Edge Mask ROI
	strVariable.Format("TeachDBEncap Right Edge Mask Rect");
	pData->m_rectDBEncapRightEdgeMaskROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBEncapRightEdgeMaskROI);

	// Load Encap Flex Edge Mask ROI
	strVariable.Format("TeachDBEncap Flex Edge Mask Rect");
	pData->m_rectDBEncapFlexEdgeMaskROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBEncapFlexEdgeMaskROI);

	// Excess - Mason
	strVariable.Format("TeachDBLeftExcessMask Rect");
	pData->m_rectDBLeftExcessMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBLeftExcessMask);

	strVariable.Format("TeachDBRightExcessMask Rect");
	pData->m_rectDBRightExcessMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBRightExcessMask);

	// Insufficient - Mason
	strVariable.Format("TeachDBCoverInsufficientMask Rect");
	pData->m_rectDBCoverInsufficientMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBCoverInsufficientMask);

	for (int k = 0; k < DARK_BLUE_ENCAP_INNER_INSUFFICIENT_MASK_POINT; k++) {
		strVariable.Format("TeachDBInnerInsufficientMask PolygonPoint%d", k + 1);
		pData->m_polygonDBInnerInsufficientMask[k] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonDBInnerInsufficientMask[k]);
	}

	strVariable.Format("TeachDBLeftInsufficientMask Rect");
	pData->m_rectDBLeftInsufficientMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBLeftInsufficientMask);

	strVariable.Format("TeachDBRightInsufficientMask Rect");
	pData->m_rectDBRightInsufficientMask = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBRightInsufficientMask);

	// Glue - Mason
	for (int k = 0; k < ENCAP_GLUE_MASK_POINT; k++) {
		strVariable.Format("TeachDBEncapGlueMask PolygonPoint%d", k + 1);
		pData->m_polygonDBEncapGlueMask[k] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_polygonDBEncapGlueMask[k]);
	}

	// Void Dark Blue Encap ROI Inspection- Magnus
	strVariable.Format("Void Dark Blue Encap Inspection ROI");
	pData->m_rectDBlueVoidROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDBlueVoidROI);
	// Bulge Blue Encap ROI Inspection- Magnus
	strVariable.Format("Bulge Blue Encap Inspection ROI");
	pData->m_rectBlueBulgeROI = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectBlueBulgeROI);	// // Blow Hole
	strVariable.Format("Rect Masking Left Encap For Inspect BlowHole On Dark Blue Encap");
	pData->m_rectDarkEncapBHMaskingLeftPolygonPointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkEncapBHMaskingLeftPolygonPointArea);

	strVariable.Format("Rect Masking Right Encap For Inspect BlowHole On Dark Blue Encap");
	pData->m_rectDarkEncapBHMaskingRightPolygonPointArea = ptrIniFile->GetCRect(strSec, strVariable, pData->m_rectDarkEncapBHMaskingRightPolygonPointArea);
	// // Wire And IC
	strVariable.Format("Dark Blue Encap IC Polygon Point Count");
	pData->nDarkBlueEncapICPolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nDarkBlueEncapICPolygonPointNumber);
	if (pData->nDarkBlueEncapICPolygonPointNumber > 20)
		pData->nDarkBlueEncapICPolygonPointNumber = 20;
	else if (pData->nDarkBlueEncapICPolygonPointNumber < 3)
		pData->nDarkBlueEncapICPolygonPointNumber = 3;

	for (int j = 0; j < pData->nDarkBlueEncapICPolygonPointNumber; j++) {
		strVariable.Format("Dark Blue Encap IC PolygonPoint%d", j + 1);
		pData->m_DarkBlueEncapICPolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_DarkBlueEncapICPolygonPointArea[j]);
	}

	strVariable.Format("Dark Blue Encap Bottom Wire Polygon Point Count");
	pData->nDarkBlueEncapWireBottomPolygonPointNumber = ptrIniFile->GetInteger(strSec, strVariable, pData->nDarkBlueEncapWireBottomPolygonPointNumber);
	if (pData->nDarkBlueEncapWireBottomPolygonPointNumber > 20)
		pData->nDarkBlueEncapWireBottomPolygonPointNumber = 20;
	else if (pData->nDarkBlueEncapWireBottomPolygonPointNumber < 3)
		pData->nDarkBlueEncapWireBottomPolygonPointNumber = 3;

	for (int j = 0; j < pData->nDarkBlueEncapWireBottomPolygonPointNumber; j++) {
		strVariable.Format("Dark Blue Encap Bottom Wire PolygonPoint%d", j + 1);
		pData->m_DarkBlueEncapWireBottomPolygonPointArea[j] = ptrIniFile->GetCPoint(strSec, strVariable, pData->m_DarkBlueEncapWireBottomPolygonPointArea[j]);
	}
	//
	int nLen1, nLen2;
	double dTempValue;
	CString strTemp;

	//// start PVI - 0; end PVI - 31;
	//// start Encap - 32; end Encap - 46;
	//// start CL - 47; end CL - 59;
	//// total - 60

	nLen1 = nLen2 = 0;
	CString str[] = { "PVIArea",
		"PVIAngle", "PVI2DCenter",
		"PVISize",
		"MaskNumber", "Mask2DCenterPoints",
		"MaskAngleConcat", "MaskSize",
		"PVIReserve1", "PVIReserve2",
		"PVIReserve3",
		"PVIReserve4", "PVIReserve5",
		"PVIReserve6", "PVIReserve7",
		"PVIReserve8", "PVIReserve9",
		"PVIReserve10", "PVIReserve11",
		"PVIReserve12", "PVIReserve13",
		"PVIReserve14", "PVIReserve15",
		"PVIReserve16", "PVIReserve17",
		"PVIReserve18", "PVIReserve19",
		"PVIReserve20", "PVIReserve21",
		"PVIReserve22", "PVIReserve23", "PVIReserve24",
		"MinEncapContrast", "MaxEncapContrast",
		"EncapReserve1", "EncapReserve2",
		"EncapReserve3", "EncapReserve4",
		"EncapReserve5", "EncapReserve6",
		"EncapReserve7", "EncapReserve8",
		"EncapReserve9", "EncapReserve10",
		"EncapReserve11", "EncapReserve12",
		"EncapReserve13",
		"CLMaxRotAngle",
		"CLPatternSearchX", "CLPatternSearchY",
		"CLMinAccScore",
		"CLDatumRow", "CLDatumCol",
		"CLModelID",
		"CLUpperCloseDist", "CLUpperFarDist",
		"CLLowerCloseDist", "CLLowerFarDist",
		"CLSideCloseDist", "CLSideFarDist" };

	for (int i = 0; i </*pData->hvec_TupleVectorPVI.GetCount()*/32; i++) {
		//nLen1 = 0;
		//strTemp.Format("PVI Teach %s Count", str[i]);
		//nLen1 = ptrIniFile->GetInteger(strSec, strTemp, nLen1);
		for (int j = 0; j </*nLen1*/10; j++) {
			nLen2 = 0;
			strTemp.Format("PVI Teach %s[%d] Count", str[i], j + 1);
			nLen2 = ptrIniFile->GetInteger(strSec, strTemp, nLen2);
			for (int k = 0; k < nLen2; k++) {
				dTempValue = 0;
				strTemp.Format("PVI Teach %s[%d][%d]", str[i], j + 1, k + 1);
				dTempValue = ptrIniFile->GetDouble(strSec, strTemp, dTempValue);
				pData->darrayPVI[i][j].Add(dTempValue);
				//OutputDebugLogTo(9, TRUE, "Get PVI %s = %.2f", strTemp, dTempValue);//// for testing
			}
		}
	}

	for (int i = 32; i < 47; i++) {
		nLen1 = 0;
		strTemp.Format("PVI Teach Encap %s Count", str[i]);
		nLen1 = ptrIniFile->GetInteger(strSec, strTemp, nLen1);
		for (int j = 0; j < nLen1; j++) {
			strTemp.Format("PVI Teach Encap %s[%d]", str[i], j + 1);
			dTempValue = ptrIniFile->GetDouble(strSec, strTemp, dTempValue);
			pData->darrayEncap[i - 32].Add(dTempValue);
			//OutputDebugLogTo(9, TRUE, "Get Encap %s = %.2f", strTemp, dTempValue);//// for testing
		}
	}


	return 0;
}

int CInspectionHandler::SetTeachParameters(CIniFile *ptrIniFile)
{
	CString strSec, strVariable;
	strSec.Format("Training Data");

	CTrainingData* pData = &m_Inspection.m_TrainingData;

	//ptrIniFile->SetInteger(strSec, "Cuvette Radius_Ra", pData->nCuvetteRadiusRa);
	//ptrIniFile->SetInteger(strSec, "Cuvette Radius_Rb", pData->nCuvetteRadiusRb);
	//ptrIniFile->SetInteger(strSec, "Lens Radius",       pData->nLensRadius);

	int nCount = (int)m_ParametersTeach.GetSize();
	for (int i = 0; i < nCount; i++)
		(m_ParametersTeach.GetAt(i))->SaveParameters(ptrIniFile);

	// Device Loc //
	strVariable.Format("TeachDeviceLoc Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDeviceLoc);

	// Device Horizontal LineLoc //
	strVariable.Format("TeachDeviceHorizontalLineLoc Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDeviceHorizontalLineLoc);

	// Device Vertical Line Loc //
	strVariable.Format("TeachDeviceVerticalLineLoc Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDeviceVerticalLineLoc);

	// Dattam/Pattern Search //
	for (int nRectDattam = 0; nRectDattam < 4; nRectDattam++) {
		strVariable.Format("TeachDattam/PatternSearch Rect%d", nRectDattam + 1);
		ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDattamSearch[nRectDattam]);
	}

	strVariable.Format("Encap Tracing Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nEncapTracingPolygonPointNumber);
	for (int j = 0; j < pData->nEncapTracingPolygonPointNumber; j++) {
		strVariable.Format("Encap Tracing PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_EncapTracingPolygonPointArea[j]);
	}

	strVariable.Format("Encap Top Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nEncapTopPolygonPointNumber);
	for (int j = 0; j < pData->nEncapTopPolygonPointNumber; j++) {
		strVariable.Format("Encap Top PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_EncapTopPolygonPointArea[j]);
	}

	strVariable.Format("Encap Bottom Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nEncapBottomPolygonPointNumber);
	for (int j = 0; j < pData->nEncapBottomPolygonPointNumber; j++) {
		strVariable.Format("Encap Bottom PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_EncapBottomPolygonPointArea[j]);
	}



	// // Insufficient
	// Left Encap Region For Checking Insufficient
	strVariable.Format("Rect Left Encap Region For Checking Insufficient Blue Encap");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectBlueEncapInsuffLeftRegionPolygonPointArea);

	// Right Encap Region For Checking Insufficient
	strVariable.Format("Rect Right Encap Region For Checking Insufficient Blue Encap");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectBlueEncapInsuffRightRegionPolygonPointArea);

	//  Region For Find border of the base
	strVariable.Format("Rect Region For Find border of the base Insufficient Blue Encap");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea);

	// Middle Encap Region For Checking Insufficient
	strVariable.Format("Middle Blue Encap Region For Checking Insufficient Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nBlueEncapInsuffMidRegionPolygonPointNumber);
	for (int j = 0; j < pData->nBlueEncapInsuffMidRegionPolygonPointNumber; j++) {
		strVariable.Format("Middle Blue Encap Region For Checking Insufficient PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_BlueEncapInsuffMidRegionPolygonPointArea[j]);
	}

	// // // // Excess
	// // Left Encap Region For Checking Excess
	strVariable.Format("Rect Left Encap Region For Checking Excess Blue Encap");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectBlueEncapExcessLeftRegionPolygonPointArea);

	// // Right Encap Region For Checking Excess
	strVariable.Format("Rect Right Encap Region For Checking Excess Blue Encap");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectBlueEncapExcessRightRegionPolygonPointArea);

	// Glue - Mason
	for (int k = 0; k < ENCAP_GLUE_MASK_POINT; k++) {
		strVariable.Format("TeachBEncapGlueMask PolygonPoint%d", k + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonBEncapGlueMask[k]);
	}

	// Contamination - Mason
	strVariable.Format("TeachBEncap Flex Edge Mask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectBEncapFlexEdgeMaskROI);


	strVariable.Format("Encap Wire Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nEncapWirePolygonPointNumber);
	for (int j = 0; j < pData->nEncapWirePolygonPointNumber; j++) {
		strVariable.Format("Encap Wire PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_EncapWirePolygonPointArea[j]);
	}

	strVariable.Format("Encap Wire Bottom Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nEncapWireBottomPolygonPointNumber);
	for (int j = 0; j < pData->nEncapWireBottomPolygonPointNumber; j++) {
		strVariable.Format("Encap Wire Bottom PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_EncapWireBottomPolygonPointArea[j]);
	}

	strVariable.Format("Encap IC Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nEncapICPolygonPointNumber);
	for (int j = 0; j < pData->nEncapICPolygonPointNumber; j++) {
		strVariable.Format("Encap IC PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_EncapICPolygonPointArea[j]);
	}

	strVariable.Format("Rect Find Left Line Flex for Generating Masking Encap");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectEncapMaskingLeftPolygonPointArea);

	strVariable.Format("Rect Find Right Line Flex for Generating Masking Encap");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectEncapMaskingRightPolygonPointArea);

	strVariable.Format("Rect Find Limit Line Flex for Generating Masking Encap");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectEncapMaskingLimitPolygonPointArea);

	strVariable.Format("TeachPVIEncap  DarkTopRect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkTopEncap);
	strVariable.Format("TeachPVIEncap  DarkTopWidth");
	ptrIniFile->SetInteger(strSec, strVariable, pData->m_nDarkTopRectWidth);
	strVariable.Format("TeachPVIEncap  DarkTopHeight");
	ptrIniFile->SetInteger(strSec, strVariable, pData->m_nDarkTopRectHeight);

	strVariable.Format("TeachPVIEncap  DarkBottomRect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkBottomEncap);
	strVariable.Format("TeachPVIEncap  DarkBottomWidth");
	ptrIniFile->SetInteger(strSec, strVariable, pData->m_nDarkBottomRectWidth);
	strVariable.Format("TeachPVIEncap  DarkBottomHeight");
	ptrIniFile->SetInteger(strSec, strVariable, pData->m_nDarkBottomRectHeight);

	strVariable.Format("TeachPVIEncap  DarkLeftRect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkLeftEncap);
	strVariable.Format("TeachPVIEncap  DarkLeftWidth");
	ptrIniFile->SetInteger(strSec, strVariable, pData->m_nDarkLeftRectWidth);
	strVariable.Format("TeachPVIEncap  DarkLeftHeight");
	ptrIniFile->SetInteger(strSec, strVariable, pData->m_nDarkLeftRectHeight);

	strVariable.Format("TeachPVIEncap  DarkRightRect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkRightEncap);
	strVariable.Format("TeachPVIEncap  DarkRightWidth");
	ptrIniFile->SetInteger(strSec, strVariable, pData->m_nDarkRightRectWidth);
	strVariable.Format("TeachPVIEncap  DarkRightHeight");
	ptrIniFile->SetInteger(strSec, strVariable, pData->m_nDarkRightRectHeight);

	for (int k = 0; k < ENCAP_MAX_MASK_SURFACE; k++) {
		strVariable.Format("Encap Surface Mask Polygon Point Count%d", k + 1);
		ptrIniFile->SetInteger(strSec, strVariable, pData->nEncapTracingMaskPolygonPointNumber[k]);
		for (int j = 0; j < pData->nEncapTracingMaskPolygonPointNumber[k]; j++) {
			strVariable.Format("Encap Surface Mask PolygonPos%dPoint%d", k + 1, j + 1);
			ptrIniFile->SetCPoint(strSec, strVariable, pData->m_EncapTracingMaskPolygonPointArea[k][j]);
		}
	}

	// // // Contact Region // // //
	// Contact Mask
	for (int k = 0; k < CONTACT_MAX_REGION_MASK; k++) {
		strVariable.Format("TeachContact Contact Mask Rect%d", k + 1);
		ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectContactMask[k]);
	}

	// Contact Region Mask
	for (int k = 0; k < CONTACT_MAX_REGION_MASK; k++) {
		strVariable.Format("TeachContact Contact Region Mask Rect%d", k + 1);
		ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectContactRegionMask[k]);
	}

	// Contact Tracing Mask
	for (int k = 0; k < CONTACT_MAX_TRACING_MASK; k++) {
		strVariable.Format("TeachContact Tracing Mask Polygon Point Count%d", k + 1);
		ptrIniFile->SetInteger(strSec, strVariable, pData->nContactTracingMaskPolygonPointNumber[k]);
		for (int j = 0; j < pData->nContactTracingMaskPolygonPointNumber[k]; j++) {
			strVariable.Format("TeachContact Tracing Mask PolygonPos%dPoint%d", k + 1, j + 1);
			ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonContactTracingMask[k][j]);
		}
	}

	// Flex Region //
	strVariable.Format("TeachFlex Substrate Pattern ROI 1 Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectFlexSubstratePatternROI1);
	strVariable.Format("TeachFlex Substrate Pattern ROI 2 Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectFlexSubstratePatternROI2);
	strVariable.Format("TeachFlex Substrate Search Pattern ROI Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectFlexSubstrateSearchPatternROI);

	for (int j = 0; j < 2; j++) {
		strVariable.Format("TeachFlex Top Edge Line Point%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonFlexTopEdgeLine[j]);
	}
	for (int j = 0; j < 2; j++) {
		strVariable.Format("TeachFlex Bottom Edge Line Point%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonFlexBottomEdgeLine[j]);
	}
	for (int j = 0; j < 2; j++) {
		strVariable.Format("TeachFlex Left Edge Line Point%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonFlexLeftEdgeLine[j]);
	}
	for (int j = 0; j < 2; j++) {
		strVariable.Format("TeachFlex Right Edge Line Point%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonFlexRightEdgeLine[j]);
	}

	strVariable.Format("TeachFlex Left Edge Line Mask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectFlexLeftEdgeLineMask);
	strVariable.Format("TeachFlex Right Edge Line Mask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectFlexRightEdgeLineMask);

	strVariable.Format("TeachFlex Bottom Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectFlexBottomROI);

	for (int j = 0; j < pData->nFlexStraightCircuitLineMaskPolygonPointNumber; j++) {
		strVariable.Format("TeachFlex Straight Circuit Line Mask Point%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonFlexStraightCircuitLineMask[j]);
	}

	strVariable.Format("TeachFlex Number Mask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectFlexNumberMask);

	for (int k = 0; k < FLEX_MAX_TRACING_MASK; k++) {
		strVariable.Format("TeachFlex Tracing Mask Polygon Point Count%d", k + 1);
		ptrIniFile->SetInteger(strSec, strVariable, pData->nFlexTracingMaskPolygonPointNumber[k]);
		for (int j = 0; j < pData->nFlexTracingMaskPolygonPointNumber[k]; j++) {
			strVariable.Format("TeachFlex Tracing Mask PolygonPos%dPoint%d", k + 1, j + 1);
			ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonFlexTracingMask[k][j]);
		}
	}

	for (int k = 0; k < FLEX_MAX_DAMAGE_MASK; k++) {
		strVariable.Format("TeachFlex Damage Mask Polygon Point Count%d", k + 1);
		ptrIniFile->SetInteger(strSec, strVariable, pData->nFlexDamageMaskPolygonPointNumber[k]);
		for (int j = 0; j < pData->nFlexDamageMaskPolygonPointNumber[k]; j++) {
			strVariable.Format("TeachFlex Damage Mask PolygonPos%dPoint%d", k + 1, j + 1);
			ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonFlexDamageMask[k][j]);
		}
	}

	for (int k = 0; k < FLEX_MAX_SUBSTRATE_MASK; k++) {
		strVariable.Format("TeachFlex Substrate Mask Polygon Point Count%d", k + 1);
		ptrIniFile->SetInteger(strSec, strVariable, pData->nFlexSubstrateMaskPolygonPointNumber[k]);
		for (int j = 0; j < pData->nFlexSubstrateMaskPolygonPointNumber[k]; j++) {
			strVariable.Format("TeachFlex Substrate Mask PolygonPos%dPoint%d", k + 1, j + 1);
			ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonFlexSubstrateMask[k][j]);
		}
	}



	// Chicklet Swage Hole 
	for (int k = 0; k < pData->m_nChickletNumberOfHoles; k++) {
		strVariable.Format("ChickletROI  Chicklet Swage Hole ROI %d", k + 1);
		ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectChickletROI[k]);
	}
	// Dark Encap
	strVariable.Format("Encap Inside Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nDarkEncapInsidePolygonPointNumber);
	for (int j = 0; j < pData->nDarkEncapInsidePolygonPointNumber; j++) {
		strVariable.Format("Encap Inside PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_DarkEncapInsidePolygonPointArea[j]);
	}
	strVariable.Format("Encap Outside Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nDarkEncapOutsidePolygonPointNumber);
	for (int j = 0; j < pData->nDarkEncapOutsidePolygonPointNumber; j++) {
		strVariable.Format("Encap Outside PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_DarkEncapOutsidePolygonPointArea[j]);
	}
	strVariable.Format("Rect Find All Dark Encap Region");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectAllRegionDarkEncap);
	strVariable.Format("Rect Find Flex Dark Encap region");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectTopFlexDarkEncap);
	strVariable.Format("Rect White Region Remove Dark Encap region");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectBotWhiteDarkEncap);

	// Excess - Magnus
	strVariable.Format("TeachDarkLeftExcessMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkExcessLeftMask);

	strVariable.Format("TeachDarkRightExcessMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkExcessRightMask);

	// Insufficient - Magnus
	strVariable.Format("TeachDarkCoverInsufficientMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkInsufficientCoverMask);

	for (int k = 0; k < DARK_ENCAP_INNER_INSUFFICIENT_MASK_POINT; k++) {
		strVariable.Format("TeachDarkInnerInsufficientMask PolygonPoint%d", k + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonDarkInsufficientMidMask[k]);
	}

	strVariable.Format("TeachDarkLeftInsufficientMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkInsufficientLeftMask);

	strVariable.Format("TeachDarkRightInsufficientMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkInsufficientRightMask);

	// Glue - Mason
	strVariable.Format("TeachDarkGlueMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkGlueMask);



	// Contamination - Mason
	strVariable.Format("TeachDarkFlexEdgeMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkFlexEdgeMask);

	strVariable.Format("TeachDarkBottomEdgeMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkBottomEdgeMask);

	strVariable.Format("TeachDarkRightEdgeMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkRightEdgeMask);

	strVariable.Format("TeachDarkInnerMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkInnerMask);
	// Void 
	strVariable.Format("Teach Dark Encap Void Masking Left Die Point Area Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkEncapVoidMaskingLeftDiePointArea);

	strVariable.Format("Teach Dark Encap Void Masking Right Die Point Area Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkEncapVoidMaskingRightDiePointArea);

	// Wire And IC
	strVariable.Format("Dark Encap Wire Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nDarkEncapWirePolygonPointNumber);
	for (int j = 0; j < pData->nDarkEncapWirePolygonPointNumber; j++) {
		strVariable.Format("Dark Encap Wire PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_DarkEncapWirePolygonPointArea[j]);
	}

	strVariable.Format("Dark Encap IC Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nDarkEncapICPolygonPointNumber);
	for (int j = 0; j < pData->nDarkEncapICPolygonPointNumber; j++) {
		strVariable.Format("Dark Encap IC PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_DarkEncapICPolygonPointArea[j]);
	}	// Dark Blue Region //
	for (int k = 0; k < DARK_BLUE_ENCAP_ROI_POINT; k++) {
		strVariable.Format("TeachDBEncapROI PolygonPoint%d", k + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonDBEncapExtractROI[k]);
	}

	strVariable.Format("TeachDBEncap Inner Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBEncapInnerROI);

	strVariable.Format("TeachDBEncap Top Edge Mask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBEncapTopEdgeMaskROI);

	strVariable.Format("TeachDBEncap Bottom Edge Mask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBEncapBottomEdgeMaskROI);

	strVariable.Format("TeachDBEncap Left Edge Mask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBEncapLeftEdgeMaskROI);

	strVariable.Format("TeachDBEncap Right Edge Mask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBEncapRightEdgeMaskROI);

	strVariable.Format("TeachDBEncap Flex Edge Mask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBEncapFlexEdgeMaskROI);

	// Excess - Mason
	strVariable.Format("TeachDBLeftExcessMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBLeftExcessMask);

	strVariable.Format("TeachDBRightExcessMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBRightExcessMask);

	// Insufficient - Mason
	strVariable.Format("TeachDBCoverInsufficientMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBCoverInsufficientMask);

	for (int k = 0; k < DARK_BLUE_ENCAP_INNER_INSUFFICIENT_MASK_POINT; k++) {
		strVariable.Format("TeachDBInnerInsufficientMask PolygonPoint%d", k + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonDBInnerInsufficientMask[k]);
	}

	strVariable.Format("TeachDBLeftInsufficientMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBLeftInsufficientMask);

	strVariable.Format("TeachDBRightInsufficientMask Rect");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBRightInsufficientMask);

	// Glue - Mason
	for (int k = 0; k < ENCAP_GLUE_MASK_POINT; k++) {
		strVariable.Format("TeachDBEncapGlueMask PolygonPoint%d", k + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_polygonDBEncapGlueMask[k]);
	}

	// Void Dark Blue Encap ROI Inspection- Magnus
	strVariable.Format("Void Dark Blue Encap Inspection ROI");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDBlueVoidROI);
	// Bulge Blue Encap ROI Inspection- Magnus
	strVariable.Format("Bulge Blue Encap Inspection ROI");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectBlueBulgeROI);// End magnus// // Blow Hole
	strVariable.Format("Rect Masking Left Encap For Inspect BlowHole On Dark Blue Encap");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkEncapBHMaskingLeftPolygonPointArea);
	strVariable.Format("Rect Masking Right Encap For Inspect BlowHole On Dark Blue Encap");
	ptrIniFile->SetCRect(strSec, strVariable, pData->m_rectDarkEncapBHMaskingRightPolygonPointArea);

	// // Wire And IC
	strVariable.Format("Dark Blue Encap IC Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nDarkBlueEncapICPolygonPointNumber);
	for (int j = 0; j < pData->nDarkBlueEncapICPolygonPointNumber; j++) {
		strVariable.Format("Dark Blue Encap IC PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_DarkBlueEncapICPolygonPointArea[j]);
	}

	strVariable.Format("Dark Blue Encap Bottom Wire Polygon Point Count");
	ptrIniFile->SetInteger(strSec, strVariable, pData->nDarkBlueEncapWireBottomPolygonPointNumber);
	for (int j = 0; j < pData->nDarkBlueEncapWireBottomPolygonPointNumber; j++) {
		strVariable.Format("Dark Blue Encap Bottom Wire PolygonPoint%d", j + 1);
		ptrIniFile->SetCPoint(strSec, strVariable, pData->m_DarkBlueEncapWireBottomPolygonPointArea[j]);
	}


	//
	int nLen1, nLen2;
	double dTempValue;
	CString strTemp;

	//// start PVI - 0; end PVI - 31;
	//// start Encap - 32; end Encap - 46;
	//// start CL - 47; end CL - 59;
	//// total - 60

	nLen1 = nLen2 = 0;
	CString str[] = { "PVIArea",
					"PVIAngle", "PVI2DCenter",
					"PVISize",
					"MaskNumber", "Mask2DCenterPoints",
					"MaskAngleConcat", "MaskSize",
					"PVIReserve1", "PVIReserve2",
					"PVIReserve3",
					"PVIReserve4", "PVIReserve5",
					"PVIReserve6", "PVIReserve7",
					"PVIReserve8", "PVIReserve9",
					"PVIReserve10", "PVIReserve11",
					"PVIReserve12", "PVIReserve13",
					"PVIReserve14", "PVIReserve15",
					"PVIReserve16", "PVIReserve17",
					"PVIReserve18", "PVIReserve19",
					"PVIReserve20", "PVIReserve21",
					"PVIReserve22", "PVIReserve23", "PVIReserve24",
					"MinEncapContrast", "MaxEncapContrast",
					"EncapReserve1", "EncapReserve2",
					"EncapReserve3", "EncapReserve4",
					"EncapReserve5", "EncapReserve6",
					"EncapReserve7", "EncapReserve8",
					"EncapReserve9", "EncapReserve10",
					"EncapReserve11", "EncapReserve12",
					"EncapReserve13",
					"CLMaxRotAngle",
					"CLPatternSearchX", "CLPatternSearchY",
					"CLMinAccScore",
					"CLDatumRow", "CLDatumCol",
					"CLModelID",
					"CLUpperCloseDist", "CLUpperFarDist",
					"CLLowerCloseDist", "CLLowerFarDist",
					"CLSideCloseDist", "CLSideFarDist" };

	for (int i = 0; i </*pData->hvec_TupleVectorPVI.GetCount()*/32; i++) {
		//nLen1 = (int)pData->darrayPVI[i]->GetCount();
		//strTemp.Format("PVI Teach %s Count", str[i]);
		//ptrIniFile->SetInteger(strSec, strTemp, nLen1);
		for (int j = 0; j </*nLen1*/10; j++) {
			nLen2 = (int)pData->darrayPVI[i][j].GetCount();
			strTemp.Format("PVI Teach %s[%d] Count", str[i], j + 1);
			ptrIniFile->SetInteger(strSec, strTemp, nLen2);
			if (nLen2 == 0) {
				dTempValue = 0;
				strTemp.Format("PVI Teach %s[%d]", str[i], j + 1);
				ptrIniFile->SetDouble(strSec, strTemp, dTempValue);
			}
			else {
				for (int k = 0; k < nLen2; k++) {
					dTempValue = pData->darrayPVI[i][j][k];
					strTemp.Format("PVI Teach %s[%d][%d]", str[i], j + 1, k + 1);
					ptrIniFile->SetDouble(strSec, strTemp, dTempValue);
					//OutputDebugLogTo(9, TRUE, "Set PVI %s = %.2f", strTemp, dTempValue);//// for testing
				}
			}
		}
	}

	for (int i = 32; i < 47; i++) {
		nLen1 = (int)pData->darrayEncap[i - 32].GetCount();
		strTemp.Format("PVI Teach Encap %s Count", str[i]);
		ptrIniFile->SetInteger(strSec, strTemp, nLen1);
		for (int j = 0; j < nLen1; j++) {
			dTempValue = pData->darrayEncap[i - 32][j];
			strTemp.Format("PVI Teach Encap %s[%d]", str[i], j + 1);
			ptrIniFile->SetDouble(strSec, strTemp, dTempValue);
			//OutputDebugLogTo(9, TRUE, "Set Encap %s = %.2f", strTemp, dTempValue);//// for testing
		}
	}


	//for (int i = 47; i<60; i++) {
	//	nLen1 = (int)pData->darrayCoverLayer[i - 47].GetCount();
	//	strTemp.Format("PVI Teach CoverLayer %s Count", str[i]);
	//	ptrIniFile->SetInteger(strSec, strTemp, nLen1);
	//	for (int j = 0; j < nLen1; j++) {
	//		dTempValue = pData->darrayCoverLayer[i - 47][j];
	//		strTemp.Format("PVI Teach CoverLayer %s[%d]", str[i], j + 1);
	//		ptrIniFile->SetDouble(strSec, strTemp, dTempValue);
	//		//OutputDebugLogTo(9, TRUE, "Set CoverLayer %s = %.2f", strTemp, dTempValue);//// for testing
	//	}
	//}

	ptrIniFile->WriteFile();
	return 0;
}


int CInspectionHandler::GetPVIDefectParameters(CString strRecipeFolder, int nTrack, int nFov)
{
	CIniFile ptrIniFile;
	CString strRecipeFile;
	CString strDataFileName;
	strRecipeFile.Format("PVIDefectParameters_%d", nFov + 1);
	strDataFileName.Format("%s\\%s.pvi", strRecipeFolder, strRecipeFile);
	ptrIniFile.SetPath(strDataFileName);

	if (ptrIniFile.ReadFile()) {
		m_Inspection.m_TrainingData.nNoOfPviArea = ptrIniFile.GetInteger("PVI Area", "Count", 1);
		for (int nArea = 0; nArea < m_Inspection.m_TrainingData.nNoOfPviArea; nArea++)
		{
			CString str;
			str.Format("PVIArea_%d", nArea + 1);
			m_Inspection.m_pPviInspectionSetup[nArea].bEnable = ptrIniFile.GetBoolean(str, "Enable", 0);
			//m_Inspection.m_pPviInspectionSetup[nArea].bNonUniformSurface = ptrIniFile.GetBoolean(str,"Enable NonUniformSource",0);
			m_Inspection.m_pPviInspectionSetup[nArea].dDeviceEdgeWidth = ptrIniFile.GetDouble(str, "Device Edge Width", m_Inspection.m_pPviInspectionSetup[nArea].dDeviceEdgeWidth);
			m_Inspection.m_pPviInspectionSetup[nArea].dDeviceEdgeOffset = ptrIniFile.GetDouble(str, "Device Edge Offset", m_Inspection.m_pPviInspectionSetup[nArea].dDeviceEdgeOffset);
			m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskNumber = ptrIniFile.GetInteger(str, "Device Mask Number", 0);
			int tempAretype = ptrIniFile.GetInteger(str, "Device Mask Type", 0);
			m_Inspection.m_pPviInspectionSetup[nArea].nDeviceAreaType = tempAretype == 0 ? RECTANGLE : tempAretype == 1 ? POLYGON : tempAretype == 2 ? ECLIPSE : POLYLINE;
			for (int nMask = 0; nMask < m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskNumber; nMask++) {
				CString strTemp;
				strTemp.Format("Device Mask Type_%d", nMask + 1);
				tempAretype = ptrIniFile.GetInteger(str, strTemp, 0);
				m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskArea[nMask] = tempAretype == 0 ? RECTANGLE : tempAretype == 1 ? POLYGON : ECLIPSE;
			}

			m_Inspection.m_pPviInspectionSetup[nArea].bPVIRecheckEnable = ptrIniFile.GetBoolean(str, "Recheck Enable", m_Inspection.m_pPviInspectionSetup[nArea].bPVIRecheckEnable);
			m_Inspection.m_pPviInspectionSetup[nArea].nMinGVDiff = ptrIniFile.GetInteger(str, "Min. Mean GV Diff", m_Inspection.m_pPviInspectionSetup[nArea].nMinGVDiff);
			m_Inspection.m_pPviInspectionSetup[nArea].nIntensity = ptrIniFile.GetInteger(str, "Max Contrast", m_Inspection.m_pPviInspectionSetup[nArea].nIntensity);

			CString strEdge;
			strEdge.Format("%s_Edge", str);
			m_Inspection.m_pPviInspectionSetup[nArea].bEdgeEnable = ptrIniFile.GetBoolean(strEdge, "Enable", 0);
			m_Inspection.m_pPviInspectionSetup[nArea].nMinEdgeContrast = ptrIniFile.GetInteger(strEdge, "Min Contrast", m_Inspection.m_pPviInspectionSetup[nArea].nMinEdgeContrast);
			m_Inspection.m_pPviInspectionSetup[nArea].nMaxEdgeContrast = ptrIniFile.GetInteger(strEdge, "Max Contrast", m_Inspection.m_pPviInspectionSetup[nArea].nMaxEdgeContrast);
			m_Inspection.m_pPviInspectionSetup[nArea].dEdgeRegionOpen = ptrIniFile.GetDouble(strEdge, "Edge Region Open", m_Inspection.m_pPviInspectionSetup[nArea].dEdgeRegionOpen);
			m_Inspection.m_pPviInspectionSetup[nArea].dEdgeWidth = ptrIniFile.GetDouble(strEdge, "Edge Width", m_Inspection.m_pPviInspectionSetup[nArea].dEdgeWidth);
			m_Inspection.m_pPviInspectionSetup[nArea].dEdgeOffset = ptrIniFile.GetDouble(strEdge, "Edge Offset", m_Inspection.m_pPviInspectionSetup[nArea].dEdgeOffset);

			CString strVariable;
			strVariable.Format("TeachPviArea RectPos%d", nArea + 1);
			m_Inspection.m_pPviInspectionSetup[nArea].m_rectPviArea = ptrIniFile.GetCRect(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_rectPviArea);
			for (int j = 0; j < m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskNumber; j++) {
				strVariable.Format("TeachPviAreaMaskRegion RectPos%dMask%d", nArea + 1, j + 1);
				m_Inspection.m_pPviInspectionSetup[nArea].m_rectPviMaskRegions[j] = ptrIniFile.GetCRect(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_rectPviMaskRegions[j]);

				//For Polygon masking
				strVariable.Format("TeachPviAreaMaskRegion Polygon Point Count Pos%dMask%d", nArea + 1, j + 1);
				m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[j] = ptrIniFile.GetInteger(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[j]);
				if (m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[j] > 20)
					m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[j] = 20;
				else if (m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[j] < 3)
					m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[j] = 3;

				for (int k = 0; k < m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[j]; k++) {
					strVariable.Format("TeachPviAreaMaskRegion PolygonPos%dMask%dPoint%d", nArea + 1, j + 1, k + 1);
					m_Inspection.m_pPviInspectionSetup[nArea].m_polygonPviMask[j][k] = ptrIniFile.GetCPoint(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_polygonPviMask[j][k]);
				}
			}

			//PVI Polygon Type, save the polygon points
			strVariable.Format("TeachPviArea Polygon Point Count%d", nArea + 1);
			m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPointNumber = ptrIniFile.GetInteger(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPointNumber);
			if (m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPointNumber > 20)
				m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPointNumber = 20;
			else if (m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPointNumber < 3)
				m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPointNumber = 3;

			for (int j = 0; j < m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPointNumber; j++) {
				strVariable.Format("TeachPviArea PolygonPos%dPoint%d", nArea + 1, j + 1);
				m_Inspection.m_pPviInspectionSetup[nArea].m_polygonPviArea[j] = ptrIniFile.GetCPoint(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_polygonPviArea[j]);
			}

			//PVI Edge, save the polyline points
			strVariable.Format("TeachPviArea Polyline Point Count%d", nArea + 1);
			m_Inspection.m_pPviInspectionSetup[nArea].nPolylinePointNumber = ptrIniFile.GetInteger(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].nPolylinePointNumber);
			if (m_Inspection.m_pPviInspectionSetup[nArea].nPolylinePointNumber > 20)
				m_Inspection.m_pPviInspectionSetup[nArea].nPolylinePointNumber = 20;
			else if (m_Inspection.m_pPviInspectionSetup[nArea].nPolylinePointNumber < 2)
				m_Inspection.m_pPviInspectionSetup[nArea].nPolylinePointNumber = 2;

			for (int j = 0; j < m_Inspection.m_pPviInspectionSetup[nArea].nPolylinePointNumber; j++) {
				strVariable.Format("TeachPviArea PolylinePos%dPoint%d", nArea + 1, j + 1);
				m_Inspection.m_pPviInspectionSetup[nArea].m_polylinePviArea[j] = ptrIniFile.GetCPoint(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_polylinePviArea[j]);
			}

			m_Inspection.m_pPviInspectionSetup[nArea].nDefectCount = ptrIniFile.GetInteger(str, "Defect Count", 0);
			for (int i = 0; i < m_Inspection.m_pPviInspectionSetup[nArea].nDefectCount; i++) {
				CString strKeyIndex;
				strKeyIndex.Format("%d", i + 1);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nID = ptrIniFile.GetInteger(str + "_DefectsIndex", strKeyIndex, m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nID);
				strKeyIndex.Format("%d", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nID);
				CString strTempKey;
				//strTempKey.Format("%s_%s",str,m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName = ptrIniFile.GetCString(str + "_Defects", strKeyIndex, "");
				strTempKey.Format("%s_%s", str, m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].bEnable = ptrIniFile.GetBoolean(strTempKey, "Enable", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].bEnable);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nDefectCharacteristics = ptrIniFile.GetInteger(strTempKey, "DefectCharacteristics", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nDefectCharacteristics);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nSize = ptrIniFile.GetInteger(strTempKey, "Size", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nSize);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nContrast = ptrIniFile.GetInteger(strTempKey, "Contrast", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nContrast);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinLength = ptrIniFile.GetInteger(strTempKey, "MinLength", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinLength);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinSquareSize = ptrIniFile.GetInteger(strTempKey, "MinSquareSize", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinSquareSize);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinInnerWidth = ptrIniFile.GetInteger(strTempKey, "MinInnerWidth", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinInnerWidth);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinCount = ptrIniFile.GetInteger(strTempKey, "MinCount", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinCount);

				//1. Dot Defect Filter
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].bDotDefectFilter = ptrIniFile.GetBoolean(strTempKey, "DotDefectEnable", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].bDotDefectFilter);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nDotContrast = ptrIniFile.GetInteger(strTempKey, "DotContrast", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nDotContrast);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinDotCircularity = ptrIniFile.GetInteger(strTempKey, "MinDotCircularity", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinDotCircularity);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinDotLength = ptrIniFile.GetInteger(strTempKey, "MinDotLength", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinDotLength);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinDotCount = ptrIniFile.GetInteger(strTempKey, "MinDotCount", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinDotCount);

				//2. Scratch Defect Filter
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].bScratchDefectFilter = ptrIniFile.GetBoolean(strTempKey, "ScratchDefectEnable", FALSE);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nScratchCharacteristics = ptrIniFile.GetInteger(strTempKey, "ScratchCharacteristics", 0);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nScratchContrast = ptrIniFile.GetInteger(strTempKey, "ScratchContrast", 50);
				int nScratchFilterDirectionTemp = ptrIniFile.GetInteger(strTempKey, "ScratchDirectionFilter", FILTER_HORIZONTAL_ONLY);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nScratchFilterDirection = (ScratchDirectionFilter)nScratchFilterDirectionTemp;
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nScratchLength = ptrIniFile.GetInteger(strTempKey, "MinScratchLength", 200);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nScratchInnerWidth = ptrIniFile.GetInteger(strTempKey, "MinScratchInnerWidth", 17);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinScratchCount = ptrIniFile.GetInteger(strTempKey, "MinScratchCount", 1);


				//The error code id will create based on the hash ID of the defect name, FOV and track ID
				strTempKey.Format("%s_Track%d_FOV%d", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName, nTrack, nFov);
				int nHasID = std::hash<std::string>()((LPCSTR)strTempKey/*m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName*/);
				m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nErrorCodeID = abs(nHasID);
			}
		}
	}
	else
	{
		//m_ParametersInspSetUP.RemoveAll();
		//CString strSec, strVariable;
		//m_Inspection.m_TrainingData.nNoOfPviArea = 1;
		//m_Inspection.m_pPviInspectionSetup[0].bEnable = TRUE;
		//m_Inspection.m_pPviInspectionSetup[0].bNonUniformSurface = TRUE;
		//m_Inspection.m_pPviInspectionSetup[0].nDeviceMaskNumber = 0;
		//m_Inspection.m_pPviInspectionSetup[0].nDeviceAreaType = RECTANGLE; //0:-Rectangle && 1:-Eclipse
		//ptrIniFile.SetInteger("PVI Area", "Count",m_Inspection.m_TrainingData.nNoOfPviArea);
		//m_Inspection.m_pPviInspectionSetup[0].nDefectCount = 0;
		//for(int nArea=0;nArea<m_Inspection.m_TrainingData.nNoOfPviArea;nArea++)
		//{
		//	CString str;
		//	str.Format("PVIArea_%d",nArea+1);
		//	ptrIniFile.SetBoolean(str,"Enable",m_Inspection.m_pPviInspectionSetup[nArea].bEnable);
		//	ptrIniFile.SetBoolean(str,"Enable NonUniformSource",m_Inspection.m_pPviInspectionSetup[nArea].bNonUniformSurface);
		//	ptrIniFile.SetInteger(str,"Device Mask Number",m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskNumber);
		//	ptrIniFile.SetInteger(str,"Device Mask Type",m_Inspection.m_pPviInspectionSetup[nArea].nDeviceAreaType);
		//	for(int nMask=0;nMask<m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskNumber;nMask++){
		//		CString strTemp;
		//		strTemp.Format("Device Mask Type_%d",nMask+1);
		//		ptrIniFile.SetInteger(str,strTemp,m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskArea[nMask]);
		//	}
		//	CString strVariable;
		//	strVariable.Format("TeachPviArea RectPos%d", nArea+1);
		//	m_Inspection.m_pPviInspectionSetup[nArea].m_rectPviArea = CRect(100,100,300,300);
		//	ptrIniFile.SetCRect(str, strVariable,m_Inspection.m_pPviInspectionSetup[nArea].m_rectPviArea);
		//	for(int j = 0;j<m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskNumber;j++){
		//		strVariable.Format("TeachPviAreaMaskRegion RectPos%dMask%d",nArea+1, j+1);
		//		m_Inspection.m_pPviInspectionSetup[nArea].m_rectPviMaskRegions[j] = CRect(100,100,300,300);
		//		ptrIniFile.SetCRect(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_rectPviMaskRegions[j]);
		//	}
		//	int nCount = m_Inspection.m_pPviInspectionSetup[nArea].m_ParametersDefect.GetSize();
		//	m_Inspection.m_pPviInspectionSetup[nArea].nDefectCount = m_Inspection.m_pPviInspectionSetup[nArea].m_ParametersDefect.GetSize();
		//	ptrIniFile.SetInteger(str,"Defect Count",m_Inspection.m_pPviInspectionSetup[nArea].nDefectCount);
		//	for(int i= 0;i<nCount;i++){
		//		CString strKeyIndex;
		//		strKeyIndex.Format("%d",i);
		//		CString strTempKey;
		//		strTempKey.Format("%s_%s",str,m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName);
		//		ptrIniFile.SetCString("Defects", strKeyIndex,m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName);
		//		ptrIniFile.SetBoolean(strTempKey,"Enable",m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].bEnable);
		//		ptrIniFile.SetInteger(strTempKey,"Size",m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nSize);
		//	}
		//}
		//ptrIniFile.WriteFile();

		return -1;
	}
	return 0;
}


int CInspectionHandler::SetPVIDefectParameters(CString strRecipeFolder, int nTrack, int nFov)
{

	CIniFile ptrIniFile;
	CString strRecipeFile;
	CString strDataFileName;
	strRecipeFile.Format("PVIDefectParameters_%d", nFov + 1);
	strDataFileName.Format("%s\\%s.pvi", strRecipeFolder, strRecipeFile);
	ptrIniFile.SetPath(strDataFileName);

	CString strSec, strVariable;
	CString str;
	ptrIniFile.SetInteger("PVI Area", "Count", m_Inspection.m_TrainingData.nNoOfPviArea);
	for (int nArea = 0; nArea < m_Inspection.m_TrainingData.nNoOfPviArea; nArea++)
	{

		str.Format("PVIArea_%d", nArea + 1);
		ptrIniFile.SetBoolean(str, "Enable", m_Inspection.m_pPviInspectionSetup[nArea].bEnable);
		//ptrIniFile.SetBoolean(str,"Enable NonUniformSource",m_Inspection.m_pPviInspectionSetup[nArea].bNonUniformSurface);
		ptrIniFile.SetDouble(str, "Device Edge Width", m_Inspection.m_pPviInspectionSetup[nArea].dDeviceEdgeWidth);
		ptrIniFile.SetDouble(str, "Device Edge Offset", m_Inspection.m_pPviInspectionSetup[nArea].dDeviceEdgeOffset);
		ptrIniFile.SetInteger(str, "Device Mask Number", m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskNumber);
		ptrIniFile.SetInteger(str, "Device Mask Type", m_Inspection.m_pPviInspectionSetup[nArea].nDeviceAreaType);
		for (int nMask = 0; nMask < m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskNumber; nMask++) {
			CString strTemp;
			strTemp.Format("Device Mask Type_%d", nMask + 1);
			ptrIniFile.SetInteger(str, strTemp, m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskArea[nMask]);
		}


		ptrIniFile.SetInteger(str, "Recheck Enable", m_Inspection.m_pPviInspectionSetup[nArea].bPVIRecheckEnable);
		ptrIniFile.SetInteger(str, "Min. Mean GV Diff", m_Inspection.m_pPviInspectionSetup[nArea].nMinGVDiff);
		ptrIniFile.SetInteger(str, "Max Contrast", m_Inspection.m_pPviInspectionSetup[nArea].nIntensity);

		CString strEdge;
		strEdge.Format("%s_Edge", str);
		ptrIniFile.SetBoolean(strEdge, "Enable", m_Inspection.m_pPviInspectionSetup[nArea].bEdgeEnable);

		ptrIniFile.SetInteger(strEdge, "Min Contrast", m_Inspection.m_pPviInspectionSetup[nArea].nMinEdgeContrast);
		ptrIniFile.SetInteger(strEdge, "Max Contrast", m_Inspection.m_pPviInspectionSetup[nArea].nMaxEdgeContrast);
		ptrIniFile.SetDouble(strEdge, "Edge Region Open", m_Inspection.m_pPviInspectionSetup[nArea].dEdgeRegionOpen);
		ptrIniFile.SetDouble(strEdge, "Edge Width", m_Inspection.m_pPviInspectionSetup[nArea].dEdgeWidth);
		ptrIniFile.SetDouble(strEdge, "Edge Offset", m_Inspection.m_pPviInspectionSetup[nArea].dEdgeOffset);

		CString strVariable;
		strVariable.Format("TeachPviArea RectPos%d", nArea + 1);
		ptrIniFile.SetCRect(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_rectPviArea);
		for (int j = 0; j < m_Inspection.m_pPviInspectionSetup[nArea].nDeviceMaskNumber; j++) {
			strVariable.Format("TeachPviAreaMaskRegion RectPos%dMask%d", nArea + 1, j + 1);
			ptrIniFile.SetCRect(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_rectPviMaskRegions[j]);

			//For Polygon masking
			strVariable.Format("TeachPviAreaMaskRegion Polygon Point Count Pos%dMask%d", nArea + 1, j + 1);
			ptrIniFile.SetInteger(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[j]);

			for (int k = 0; k < m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[j]; k++) {
				strVariable.Format("TeachPviAreaMaskRegion PolygonPos%dMask%dPoint%d", nArea + 1, j + 1, k + 1);
				ptrIniFile.SetCPoint(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_polygonPviMask[j][k]);
			}

		}

		//PVI Polygon Type, Polygon Points
		strVariable.Format("TeachPviArea Polygon Point Count%d", nArea + 1);
		ptrIniFile.SetInteger(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPointNumber);
		for (int j = 0; j < m_Inspection.m_pPviInspectionSetup[nArea].nPolygonPointNumber; j++) {
			strVariable.Format("TeachPviArea PolygonPos%dPoint%d", nArea + 1, j + 1);
			ptrIniFile.SetCPoint(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_polygonPviArea[j]);
		}

		//PVI Edge, Poly Line Points
		strVariable.Format("TeachPviArea Polyline Point Count%d", nArea + 1);
		ptrIniFile.SetInteger(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].nPolylinePointNumber);
		for (int j = 0; j < m_Inspection.m_pPviInspectionSetup[nArea].nPolylinePointNumber; j++) {
			strVariable.Format("TeachPviArea PolylinePos%dPoint%d", nArea + 1, j + 1);
			ptrIniFile.SetCPoint(str, strVariable, m_Inspection.m_pPviInspectionSetup[nArea].m_polylinePviArea[j]);
		}

		//int nCount = m_Inspection.m_pPviInspectionSetup[nArea].m_ParametersDefect.GetSize();
		//m_Inspection.m_pPviInspectionSetup[nArea].nDefectCount = m_Inspection.m_pPviInspectionSetup[nArea].m_ParametersDefect.GetSize();
		ptrIniFile.SetInteger(str, "Defect Count", m_Inspection.m_pPviInspectionSetup[nArea].nDefectCount);
		for (int i = 0; i < m_Inspection.m_pPviInspectionSetup[nArea].nDefectCount; i++) {
			CString strKeyIndex;
			strKeyIndex.Format("%d", i + 1);
			ptrIniFile.SetInteger(str + "_DefectsIndex", strKeyIndex, m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nID);
			strKeyIndex.Format("%d", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nID);
			CString strTempKey;
			//strTempKey.Format("%s_%s",str,m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName);
			ptrIniFile.SetCString(str + "_Defects", strKeyIndex, m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName);
			strTempKey.Format("%s_%s", str, m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName);
			ptrIniFile.SetBoolean(strTempKey, "Enable", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].bEnable);
			ptrIniFile.SetInteger(strTempKey, "DefectCharacteristics", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nDefectCharacteristics);

			ptrIniFile.SetInteger(strTempKey, "Size", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nSize);
			ptrIniFile.SetInteger(strTempKey, "Contrast", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nContrast);
			ptrIniFile.SetInteger(strTempKey, "MinLength", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinLength);
			ptrIniFile.SetInteger(strTempKey, "MinSquareSize", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinSquareSize);
			ptrIniFile.SetInteger(strTempKey, "MinInnerWidth", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinInnerWidth);
			ptrIniFile.SetInteger(strTempKey, "MinCount", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinCount);

			//1. Dot Defect Filter
			ptrIniFile.SetBoolean(strTempKey, "DotDefectEnable", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].bDotDefectFilter);
			ptrIniFile.SetInteger(strTempKey, "DotContrast", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nDotContrast);
			ptrIniFile.SetInteger(strTempKey, "MinDotCircularity", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinDotCircularity);
			ptrIniFile.SetInteger(strTempKey, "MinDotLength", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinDotLength);
			ptrIniFile.SetInteger(strTempKey, "MinDotCount", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinDotCount);

			//2. Scratch Defect Filter
			ptrIniFile.SetBoolean(strTempKey, "ScratchDefectEnable", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].bScratchDefectFilter);
			ptrIniFile.SetInteger(strTempKey, "ScratchCharacteristics", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nScratchCharacteristics);
			ptrIniFile.SetInteger(strTempKey, "ScratchContrast", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nScratchContrast);
			ptrIniFile.SetInteger(strTempKey, "ScratchDirectionFilter", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nScratchFilterDirection);
			ptrIniFile.SetInteger(strTempKey, "MinScratchLength", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nScratchLength);
			ptrIniFile.SetInteger(strTempKey, "MinScratchInnerWidth", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nScratchInnerWidth);
			ptrIniFile.SetInteger(strTempKey, "MinScratchCount", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].nMinScratchCount);

			//The error code id will create based on the hash ID of the defect name, Track ID and FOV
			strTempKey.Format("%s_Track%d_FOV%d", m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName, nTrack, nFov);
			int nHasID = std::hash<std::string>()((LPCSTR)strTempKey/*m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].strDefectName*/);
			m_Inspection.m_pPviInspectionSetup[nArea].m_pPviDefect[i].m_nErrorCodeID = abs(nHasID);
		}
	}

	ptrIniFile.WriteFile();
	return 0;
}

int CInspectionHandler::CopyPVIParameters(CString strRecipeFolder, CString strFileName, CInspectionHandler* pInspHandlerCopy, int nTrack, int nFOV)
{
	SetPVIDefectParameters(strRecipeFolder, nTrack, nFOV);
	CIniFile ptrIniFile;
	CString strDataFileName;

	strDataFileName.Format("%s\\TeachParameters_Pos%d.tch", strRecipeFolder, nFOV + 1);
	ptrIniFile.SetPath(strDataFileName);


	CString strSec, strVariable;
	strSec.Format("Training Data");

	CTrainingData* pData = &pInspHandlerCopy->m_Inspection.m_TrainingData;
	int nCount = (int)m_ParametersTeach.GetSize();
	for (int i = 0; i < nCount; i++)
		(m_ParametersTeach.GetAt(i))->SaveParameters(&ptrIniFile);

	// Device Loc //
	strVariable.Format("TeachDeviceLoc Rect");
	ptrIniFile.SetCRect(strSec, strVariable, pData->m_rectDeviceLoc);

	// Device Horizontal Line Loc //
	strVariable.Format("TeachDeviceHorizontalLineLoc Rect");
	ptrIniFile.SetCRect(strSec, strVariable, pData->m_rectDeviceHorizontalLineLoc);

	// Device Vertical Line Loc //
	strVariable.Format("TeachDeviceVerticalLineLoc Rect");
	ptrIniFile.SetCRect(strSec, strVariable, pData->m_rectDeviceVerticalLineLoc);

	// Dattam/Pattern Search //
	for (int nRectDattam = 0; nRectDattam < 4; nRectDattam++) {
		strVariable.Format("TeachDattam/PatternSearch Rect%d", nRectDattam + 1);
		ptrIniFile.SetCRect(strSec, strVariable, pData->m_rectDattamSearch[nRectDattam]);
	}

	// encap region //
	strVariable.Format("TeachPVIEncap  DarkTopRect");
	ptrIniFile.SetCRect(strSec, strVariable, pData->m_rectDarkTopEncap);
	strVariable.Format("TeachPVIEncap  DarkTopWidth");
	ptrIniFile.SetInteger(strSec, strVariable, pData->m_nDarkTopRectWidth);
	strVariable.Format("TeachPVIEncap  DarkTopHeight");
	ptrIniFile.SetInteger(strSec, strVariable, pData->m_nDarkTopRectHeight);


	strVariable.Format("TeachPVIEncap  DarkBottomRect");
	ptrIniFile.SetCRect(strSec, strVariable, pData->m_rectDarkBottomEncap);
	strVariable.Format("TeachPVIEncap  DarkBottomWidth");
	ptrIniFile.SetInteger(strSec, strVariable, pData->m_nDarkBottomRectWidth);
	strVariable.Format("TeachPVIEncap  DarkBottomHeight");
	ptrIniFile.SetInteger(strSec, strVariable, pData->m_nDarkBottomRectHeight);

	strVariable.Format("TeachPVIEncap  DarkLeftRect");
	ptrIniFile.SetCRect(strSec, strVariable, pData->m_rectDarkLeftEncap);
	strVariable.Format("TeachPVIEncap  DarkLeftWidth");
	ptrIniFile.SetInteger(strSec, strVariable, pData->m_nDarkLeftRectWidth);
	strVariable.Format("TeachPVIEncap  DarkLeftHeight");
	ptrIniFile.SetInteger(strSec, strVariable, pData->m_nDarkLeftRectHeight);

	strVariable.Format("TeachPVIEncap  DarkRightRect");
	ptrIniFile.SetCRect(strSec, strVariable, pData->m_rectDarkRightEncap);
	strVariable.Format("TeachPVIEncap  DarkRightWidth");
	ptrIniFile.SetInteger(strSec, strVariable, pData->m_nDarkRightRectWidth);
	strVariable.Format("TeachPVIEncap  DarkRightHeight");
	ptrIniFile.SetInteger(strSec, strVariable, pData->m_nDarkRightRectHeight);

	int nLen1, nLen2;
	double dTempValue;
	CString strTemp;

	//// start PVI - 0; end PVI - 31;
	//// start Encap - 32; end Encap - 46;
	//// start CL - 47; end CL - 59;
	//// total - 60

	nLen1 = nLen2 = 0;
	CString str[] = { "PVIArea",
		"PVIAngle", "PVI2DCenter",
		"PVISize",
		"MaskNumber", "Mask2DCenterPoints",
		"MaskAngleConcat", "MaskSize",
		"PVIReserve1", "PVIReserve2",
		"PVIReserve3",
		"PVIReserve4", "PVIReserve5",
		"PVIReserve6", "PVIReserve7",
		"PVIReserve8", "PVIReserve9",
		"PVIReserve10", "PVIReserve11",
		"PVIReserve12", "PVIReserve13",
		"PVIReserve14", "PVIReserve15",
		"PVIReserve16", "PVIReserve17",
		"PVIReserve18", "PVIReserve19",
		"PVIReserve20", "PVIReserve21",
		"PVIReserve22", "PVIReserve23", "PVIReserve24",
		"MinEncapContrast", "MaxEncapContrast",
		"EncapReserve1", "EncapReserve2",
		"EncapReserve3", "EncapReserve4",
		"EncapReserve5", "EncapReserve6",
		"EncapReserve7", "EncapReserve8",
		"EncapReserve9", "EncapReserve10",
		"EncapReserve11", "EncapReserve12",
		"EncapReserve13",
		"CLMaxRotAngle",
		"CLPatternSearchX", "CLPatternSearchY",
		"CLMinAccScore",
		"CLDatumRow", "CLDatumCol",
		"CLModelID",
		"CLUpperCloseDist", "CLUpperFarDist",
		"CLLowerCloseDist", "CLLowerFarDist",
		"CLSideCloseDist", "CLSideFarDist" };

	pData = &m_Inspection.m_TrainingData;
	for (int i = 0; i </*pData->hvec_TupleVectorPVI.GetCount()*/32; i++) {
		//nLen1 = (int)pData->darrayPVI[i]->GetCount();
		//strTemp.Format("PVI Teach %s Count", str[i]);
		//ptrIniFile->SetInteger(strSec, strTemp, nLen1);
		for (int j = 0; j </*nLen1*/10; j++) {
			nLen2 = (int)pData->darrayPVI[i][j].GetCount();
			strTemp.Format("PVI Teach %s[%d] Count", str[i], j + 1);
			ptrIniFile.SetInteger(strSec, strTemp, nLen2);
			if (nLen2 == 0) {
				dTempValue = 0;
				strTemp.Format("PVI Teach %s[%d]", str[i], j + 1);
				ptrIniFile.SetDouble(strSec, strTemp, dTempValue);
			}
			else {
				for (int k = 0; k < nLen2; k++) {
					dTempValue = pData->darrayPVI[i][j][k];
					strTemp.Format("PVI Teach %s[%d][%d]", str[i], j + 1, k + 1);
					ptrIniFile.SetDouble(strSec, strTemp, dTempValue);
					//OutputDebugLogTo(9, TRUE, "Set PVI %s = %.2f", strTemp, dTempValue);//// for testing
				}
			}
		}
	}

	for (int i = 32; i < 47; i++) {
		nLen1 = (int)pData->darrayEncap[i - 32].GetCount();
		strTemp.Format("PVI Teach Encap %s Count", str[i]);
		ptrIniFile.SetInteger(strSec, strTemp, nLen1);
		for (int j = 0; j < nLen1; j++) {
			dTempValue = pData->darrayEncap[i - 32][j];
			strTemp.Format("PVI Teach Encap %s[%d]", str[i], j + 1);
			ptrIniFile.SetDouble(strSec, strTemp, dTempValue);
		}
	}

	ptrIniFile.WriteFile();

	return 0;
}

int CInspectionHandler::LoadCalibrationParameters(CString strRecipeFolder, CString strRecipeFile, int nTrack, int nFOV)
{
	CIniFile iniFile;
	CString strCalibFileName, strSec;
	/*CString str[] = { "Top", "Bottom", "Side" };*/
	CString str[] = { "TOP", "THR" };

	//strCalibFileName.Format("%s\\%s_%d.calib", strRecipeFolder, strRecipeFile, nFOV + 1);
	strCalibFileName.Format("%s\\%s.calib", strRecipeFolder, str[nTrack]);
	iniFile.SetPath(strCalibFileName);

	if (iniFile.ReadFile()) {
		CCalibrationData* pCalibData = &m_Inspection.m_CalibrationData;
		strSec.Format("Calibration Data");

		pCalibData->rectCalibSideTrack = iniFile.GetCRect(strSec, "Side Rectangle", pCalibData->rectCalibSideTrack);
		pCalibData->nCalibTolerence = iniFile.GetInteger(strSec, "Tolerance", pCalibData->nCalibTolerence);
		pCalibData->nTargetGreyDeviation = iniFile.GetInteger(strSec, "Gray Deviation", pCalibData->nTargetGreyDeviation);
		pCalibData->nTargetGreyMeanValue = iniFile.GetInteger(strSec, "Gray Mean Value", pCalibData->nTargetGreyMeanValue);
		pCalibData->nCalibTolerenceSide = iniFile.GetInteger(strSec, "Tolerance Side", pCalibData->nCalibTolerenceSide);
		pCalibData->nTargetGreyMeanValueSide = iniFile.GetInteger(strSec, "Gray Mean Value Side", pCalibData->nTargetGreyMeanValueSide);
		//		OutputDebugLogTo(9, TRUE, "LoadCalibrationParameters T%d, FOV%d", nTrack + 1, nFOV + 1);//// for testing
	}

	return 0;
}

int CInspectionHandler::SaveCalibrationParameters(CString strRecipeFolder, CString strRecipeFile, int nTrack, int nFOV)
{
	CIniFile iniFile;
	CString strCalibFileName, strSec;
	/*CString str[] = { "Top", "Bottom", "Side" };*/
	CString str[] = { "TOP", "THR" };

	//strCalibFileName.Format("%s\\%s_%d.calib", strRecipeFolder, strRecipeFile, nFOV + 1);
	strCalibFileName.Format("%s\\%s.calib", strRecipeFolder, str[nTrack]);
	iniFile.SetPath(strCalibFileName);

	CCalibrationData* pCalibData = &m_Inspection.m_CalibrationData;
	strSec.Format("Calibration Data");

	iniFile.SetCRect(strSec, "Side Rectangle", pCalibData->rectCalibSideTrack);
	iniFile.SetInteger(strSec, "Tolerance", pCalibData->nCalibTolerence);
	iniFile.SetInteger(strSec, "Gray Deviation", pCalibData->nTargetGreyDeviation);
	iniFile.SetInteger(strSec, "Gray Mean Value", pCalibData->nTargetGreyMeanValue);
	iniFile.SetInteger(strSec, "Tolerance Side", pCalibData->nCalibTolerenceSide);
	iniFile.SetInteger(strSec, "Gray Mean Value Side", pCalibData->nTargetGreyMeanValueSide);

	iniFile.WriteFile();

	return 0;
}


int CInspectionHandler::SaveDotCalibrationParameters(CString strRecipeFolder, CString strRecipeFile, int nTrack, int nFOV)
{
	CIniFile iniFile;
	CString strCalibFileName, strSec;
	/*CString str[] = { "Top", "Bottom", "Side" };*/
	CString str[] = { "TOP", "THR" };

	//strCalibFileName.Format("%s\\%s_%d.calib", strRecipeFolder, strRecipeFile, nFOV + 1);
	strCalibFileName.Format("%s\\Dot_%s.calib", strRecipeFolder, str[nTrack]);
	iniFile.SetPath(strCalibFileName);

	CCalibrationData* pCalibData = &m_Inspection.m_CalibrationData;
	strSec.Format("Dot Calibration Data");

	iniFile.SetCRect(strSec, "Dot Calib Rectangle", pCalibData->rectDotCalib);
	iniFile.SetInteger(strSec, "Dot Input AlongX", pCalibData->nInputAlongX);
	iniFile.SetInteger(strSec, "Dot Input AlongY", pCalibData->nInputAlongY);
	iniFile.SetDouble(strSec, "Dot ResolutionAlongXInMPerP", pCalibData->dResolutionAlongXInMicronPerPixel);
	iniFile.SetDouble(strSec, "Dot ResolutionAlongYInMPerP", pCalibData->dResolutionAlongYInMicronPerPixel);

	iniFile.SetInteger(strSec, "Minimum Radius Dot Target In Pixel", pCalibData->nInputMinRadiusDotTargetPixel);

	iniFile.WriteFile();

	return 0;

}

int CInspectionHandler::LoadDotCalibrationParameters(CString strRecipeFolder, CString strRecipeFile, int nTrack, int nFOV)
{
	CIniFile iniFile;
	CString strCalibFileName, strSec;
	/*CString str[] = { "Top", "Bottom", "Side" };*/
	CString str[] = { "TOP", "THR" };

	//strCalibFileName.Format("%s\\%s_%d.calib", strRecipeFolder, strRecipeFile, nFOV + 1);
	strCalibFileName.Format("%s\\Dot_%s.calib", strRecipeFolder, str[nTrack]);
	iniFile.SetPath(strCalibFileName);

	if (iniFile.ReadFile()) {
		CCalibrationData* pCalibData = &m_Inspection.m_CalibrationData;
		strSec.Format("Dot Calibration Data");

		pCalibData->rectDotCalib = iniFile.GetCRect(strSec, "Dot Calib Rectangle", pCalibData->rectDotCalib);
		pCalibData->nInputAlongX = iniFile.GetInteger(strSec, "Dot Input AlongX", pCalibData->nInputAlongX);
		pCalibData->nInputAlongY = iniFile.GetInteger(strSec, "Dot Input AlongY", pCalibData->nInputAlongY);
		pCalibData->dResolutionAlongXInMicronPerPixel = iniFile.GetDouble(strSec, "Dot ResolutionAlongXInMPerP", pCalibData->dResolutionAlongXInMicronPerPixel);
		pCalibData->dResolutionAlongYInMicronPerPixel = iniFile.GetDouble(strSec, "Dot ResolutionAlongYInMPerP", pCalibData->dResolutionAlongYInMicronPerPixel);

		pCalibData->nInputMinRadiusDotTargetPixel = iniFile.GetInteger(strSec, "Minimum Radius Dot Target In Pixel", pCalibData->nInputMinRadiusDotTargetPixel);

		//		OutputDebugLogTo(9, TRUE, "LoadDotCalibrationParameters T%d, FOV%d", nTrack+1, nFOV+1);//// for testing
	}

	return 0;
}