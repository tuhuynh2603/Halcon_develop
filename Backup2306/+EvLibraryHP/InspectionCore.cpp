/// CInspectionCore: Vision Inspection base class
/// Udupa; Feb'2015

#include "StdAfx.h"
#include "InspectionCore.h"
#include "Sherlock.cpp"
#include "AppDef.h"
#include "PerformanceTimer.h"
#include "DebugLog.h"
#include "MathAlgo.h"
#include <map>
#include <vector>

CTeachInspectData::CTeachInspectData()
{
}

CPVIDefect::CPVIDefect()
{
	m_nID = 0;
	m_nErrorCodeID = 0;
	m_nInspPriority = 0;
	strDefectName = "";

	bEnable = FALSE;
	nDefectCharacteristics = 0;////0-Black; 1-White;
	m_nSize = 200;
	nContrast = 50;
	nMinLength = 30;
	nMinSquareSize = 20;
	nMinInnerWidth = 10;
	nMinCount = 1;

	bDotDefectFilter = FALSE;
	nDotContrast = 50;
	nMinDotCircularity = 50;
	nMinDotLength = 30;
	nMinDotCount = 1;

	bScratchDefectFilter = FALSE;
	nScratchCharacteristics = 0;////0-Black; 1-White;
	nScratchContrast = 50;
	nScratchFilterDirection = FILTER_HORIZONTAL_ONLY;
	nScratchLength = 200;
	nScratchInnerWidth = 17;
	nMinScratchCount = 1;
};

CPVIInspectionSetup::~CPVIInspectionSetup()
{
	if (m_pPviDefect != NULL)
		delete[] m_pPviDefect;
}

CPVIInspectionSetup::CPVIInspectionSetup()
{
	bEnable = FALSE;
	//bNonUniformSurface = TRUE;
	//nDeviceMaskThickness = 10;
	nDeviceMaskNumber = 0;
	nDeviceAreaType = RECTANGLE;
	nDefectCount = 0;
	dDeviceEdgeWidth = 0;
	dDeviceEdgeOffset = 0;

	bEdgeEnable = FALSE;
	nMinEdgeContrast = 60;
	nMaxEdgeContrast = 255;
	dEdgeRegionOpen = 28;
	dEdgeWidth = 0;
	dEdgeOffset = 0;

	bPVIRecheckEnable = TRUE;
	nMinGVDiff = 10;
	nIntensity = 40;

	m_pPviDefect = new CPVIDefect[PVI_DEFECT_COUNT];
	for (int i = 0; i < 20; i++)
		nDeviceMaskArea[i] = RECTANGLE;
	m_rectPviArea = CRect(0, 0, 0, 0);//CRect(100,100,300,300);
	for (int j = 0; j < 20; j++)
		m_rectPviMaskRegions[j] = CRect(0, 0, 0, 0);//CRect(100,100,300,300);

	nPolylinePointNumber = 2;
	for (int j = 0; j < 20; j++)
		m_polylinePviArea[j] = CPoint(700 + 50 * j, 700);

	nPolygonPointNumber = 3;
	for (int j = 0; j < 20; j++)
		m_polygonPviArea[j] = CPoint(700 + 50 * j, 700);

	for (int i = 0; i < 20; i++) {
		nPolygonPviMaskPointNumber[i] = 3;
		for (int j = 0; j < 20; j++)
			m_polygonPviMask[i][j] = CPoint(700 + 50 * j, 700);
	}
}


CEncap::CEncap() : CParameterCore("Encap", "Encap", -ENCAP_INSUFFICIENT)
{

	// // // // Blue Encap
	bEnableBlueEncap = FALSE;
	dBlueEncapErosionOffset = 0;
	dBlueEncapMaxLayOnAllowedPercentage = 65;

	nCharacteristic = DEFECT_WHITE;
	// // // Blue Encap Insufficient
	dMinInSuffPercentageRightBlueEncap = 9;
	dMinInSuffPercentageLeftBlueEncap = 8.5;
	dMinSizeMidBlueEncap = 2750;
	// // // Blue Encap Excess
	nContrastExcessBlueEncap = 230;
	nCircleContrastExcessBlueEncap = 130;
	dEdgeOffsetExcessBlueEncap = 82;
	dMinSizeExcessBlueEncap = 150000;
	// // // Glue - Mason
	dBEncapMaskOffsetGlue = 165;
	dBEncapRegionOffsetGlue = 110;
	dBSmoothMaskSizeGlue = 8.25;
	dBMaskSizeGlue = 1650;
	dBStandardDeviationGlue = 1.5;
	nBDiffMeanGrayGlue = 80;
	nBMinMeanGrayGlue = 200;
	dBMinSizeGlue = 4537.5;
	dBMinConvexityGlue = 0.5;
	dBMaxInnerAreaPercentGlue = 30;
	dBMaxFuzzySizeGlue = 15125;
	nBMaxDiffMeanGrayGlue = 70;

	// // // Comtamination - Mason
	dBMaskSizeCT = 1100;
	dBStandardDeviationCT = 1.5;
	nBDiffMeanGrayCT = 25;
	nBBottomMaskContrastCT = 230;
	nBFlexMaskContrastCT = 215;
	nBMinGrayCT = 150;
	dBMinSizeCT = 1512.5;
	nBMinDiffMeanGrayCT = 18;
	nBMinMeanEdgeMaskGrayCT = 231;
	nBMinMeanFlexMaskGrayCT = 231;
	nBMinMeanBorderedGrayCT = 140;
	dBMaxDiffAreaPercentCT = 30;
	nBMinDefectCountCT = 1;

	dBlueEncapVoidMinMaskSize = 147.8;
	nBlueEncapVoidMinDefectContrast = 55;
	dBlueEncapVoidMinSize = 14975;
	dBlueEncapVoidMinCircularity = 0.2;
	dBlueEncapVoidMinLength = 109.5;
	dBlueEncapVoidMinSquareSize = 109.5;
	nBlueEncapVoidGrayMean = 6600;
	nBlueEncapVoidGrayDeviation = 24;
	nBlueEncapVoidMinMeanGVDiffContamination = 3;
	dBlueEncapFuzzyVoidMinSize = 220;

	nBlueEncapBHIntensity = 30;
	dBlueEncapBHMinCircularity = 0.6;
	dBlueEncapBHMinPercentLightArea = 9;
	dBlueEncapBHMinArea = 2695;
	dBlueEncapBHSlideWindowSide = 38.5;

	nBlueEncapTopWireIntensity = 240;
	dBlueEncapTopWireMaxLineWidth = 82.5;
	dBlueEncapTopWireMinArea = 3000;
	nBlueEncapBottomWireIntensity = 230;
	dBlueEncapBottomWireMaxLineWidth = 27.5;
	dBlueEncapBottomWireMinArea = 3300;
	dBlueEncapMinAreaIC = 4500;

	nGluePriority = 0;
	nContaminationPriority = 1;
	nBlowHolePriority = 2;
	nVoidPriority = 3;
	nWireAndICPriority = 4;

	// // // // Dark Encap -magnus
	// Excess
	bEnableDarkEncap = FALSE;
	dDarkEncapExcessOffset = 10 * 5.47;
	dDarkEncapExcessMinAreaDefect = 2000 * 5.47 * 5.47;
	nDarkEncapExcessMinContrastDefect = 50;
	nDarkEncapExcessMaxContrastDefect = 235;
	//Insuff
	nDarkEncapInsuffContrast = 50;
	nDarkEncapInsuffVariationContrast = 25;
	nDarkEncapInsuffWhiteContrast = 230;
	dDarkEncapInsuffLeftOffset = 20 * 5.47;
	dDarkEncapInsuffRightOffset = 10 * 5.47;
	dDarkEncapInsuffMinPercentLeftError = 7.0;
	dDarkEncapInsuffMinPercentRightError = 30.0;
	dDarkEncapInsuffMinAreaMidError = 3000 * 5.47* 5.47;// End magnus

	// Glue - Mason
	nDarkContrastGlue = 210;
	dDarkMaskSizeGlue = 100*5.5;
	dDarkEncapRegionOffsetGlue = 5.5*5.5;
	dDarkMinSizeGlue = 350*5.5*5.5;
	nDarkMinMeanGrayGlue = 155;
	dDarkMinConvexityGlue = 0.8;
	nDarkMinDiffMeanGrayGlue = 32;
	dDarkMaxFuzzySizeGlue = 1200*5.5*5.5;
	dDarkMaxWhiteAreaGlue = 50*5.5*5.5;

	// Contamination - Mason
	dDarkMaskSizeCT = 100*5.5;
	dDarkStandardDeviationCT = 2.5;
	nDarkDiffMeanGrayCT = 30;
	nDarkFuzzyContrastCT = 185;
	nDarkMinGrayCT = 200;
	dDarkMinSizeCT = 20*5.5*5.5;
	dDarkEncapRegionOffsetCT = 5*5.5;
	nDarkMinDiffMeanGrayCT = 25;
	nDarkMinMeanEdgeMaskGrayCT = 200;
	dDarkMaxDiffAreaPercentCT = 66;
	nDarkMinFuzzyGrayCT = 130;
	nDarkMinDiffMeanFuzzyGrayCT = 21;
	nDarkMinDefectCountCT = 1;
	// // Small Void
	dSmallVoidMinSizeDarkEncap = 6000;
	dSmallVoidMinCircularityDarkEncap = 0.25;
	dSmallVoidGrayDeviationDarkEncap = 0;
	nSmallVoidIntensityDarkEncap = 40;
	dSmallVoidMaxLayOnPartPercentageAllowDarkEncap = 25;
	// // Large Void
	dLargeVoidMinSizeDarkEncap = 12000;
	dLargeVoidMinCircularityDarkEncap = 0.15;
	nLargeVoidIntensityDarkEncap = 35;
	dLargeVoidMinGrayDeviationDarkEncap = 30;
	dLargeVoidMaxLayOnPartPercentageAllowDarkEncap = 55;
	// // Fuzzy Void
	dFuzzyVoidMinMeanGVDiffDarkEncap = 2.5;
	nFuzzyVoidMinIntensityDarkEncap = 35;
	dFuzzyVoidMinSizeDarkEncap = 8400;
	dFuzzyVoidMinAnisometryDarkEncap = 3.1;
	dFuzzyVoidMinGrayMeanDarkEncap= 120;
	dFuzzyVoidMinGrayDeviationDarkEncap = 23;
	dFuzzyVoidMaxLayOnPartPercentageAllow = 30;
	// BlowHole
	dBlowHoleSlideWindowSideDarkEncap = 16.45;
	dLargeBlowHoleAnisometryDarkEncap = 5;
	dSmallBlowHoleAnisometryDarkEncap = 4.3;
	dSmallBlowMaxGrayMeanDarkEncap = 71;
	// Wire And IC
	dWireMinLengthDarkEncap =93;
	nWireIntensityDarkEncap=220;
	dWireMaxLineWidthDarkEncap = 27.37;
	nWireContrastDarkEncap = 30;
	dMaxWireCircularityDarkEncap = 0.2;
	nICIntensityDarkEncap = 230;
	dICMinAreaDarkEncap = 6000;
	// // // // Dark Blue Encap
	bEnableDarkBlueEncap = FALSE;

	// Excess - Mason
	nDBContrastEX = 230;
	nDBCircleContrastEX = 100;
	dDBEdgeOffsetEX = 15*5.5;
	dDBMinAreaEX = 5000*5.5*5.5;

	// Insufficient - Mason
	nDBWhiteContrastIS = 240;
	nDBBlackContrastIS = 210;
	nDBEdgeContrastIS = 170;
	dDBInnerEdgeOffsetIS = 100*5.5;
	dDBLeftEdgeOffsetIS = 10*5.5;
	dDBRightEdgeOffsetIS = 10*5.5;
	nDBMinDiffMeanGrayIS = 27;
	dDBMinInnerAreaIS = 5000*5.5*5.5;
	dDBMinSmoothAreaPercentIS = 95;
	dDBMinEdgePercentIS = 75;

	// Glue - Mason
	nDBContrastGlue = 210;
	dDBEncapMaskOffsetGlue = 0;
	dDBFlexMaskOffsetGlue = 0;
	dDBSmoothMaskSizeGlue = 30;
	dDBMinSizeGlue = 6500;
	dDBMinConvexityGlue = 0.75;

	// Comtamination - Mason
	dDBMaskSizeCT = 440;
	dDBStandardDeviationCT = 1.5;
	nDBDiffMeanGrayCT = 25;
	nDBBottomMaskContrastCT = 200;
	nDBFlexMaskContrastCT = 200;
	nDBMinGrayCT = 150;
	dDBMinSizeCT = 275;
	nDBMinDiffMeanGrayCT = 20;
	nDBMinMeanEdgeMaskGrayCT = 200;
	nDBMinMeanFlexMaskGrayCT = 230;
	nDBMinMeanBorderedGrayCT = 150;
	dDBMaxDiffAreaPercentCT = 45;
	nDBMinDefectCountCT = 1;

	// Void Dark Blue Encap- Magnus
	nDBlueVoidScaleContrastEncap = 3;
	nDBlueVoidValueContrastDefect = 60;
	nDBlueVoidScaleContrastDefect = 10;
	dDBlueVoidWidthLocalSearch = 200 * 5.47;
	dDBlueVoidHeightLocalSearch = 200 * 5.47;
	dDBlueVoidOffsetEncap = 1 * 5.47;
	dDBlueVoidClosingRadiusDefect = 3.5 * 5.47;
	dDBlueVoidMinAreaDefect = 200 * 5.47 * 5.47;
	nDBlueVoidCircularityDefect = 10;
	nDBlueVoidHWRatioDefect = 10;
	nDBlueVoidInnerDefectPercent = 20;
	nDBlueVoidOffsetCheckDefect = 50;
	nDBlueVoidMaxContrastInOutDefect = 75;
	nDBlueVoidMinGrayDefect = 160;
	nDBlueVoidMaxGrayDefect = 225;
	nDBlueVoidMaxDeviationInnerDefect = 25; //End magnus

	// Bulge Blue Encap- Magnus
	nBlueBulgeScaleContrastEncap = 1;
	nBlueBulgeValueContrastDefect = 50;
	nBlueBulgeScaleContrastDefect = 140;
	dBlueBulgeWidthLocalSearch = 100 *5.47;
	dBlueBulgeHeightLocalSearch = 200 * 5.47;
	dBlueBulgeOffsetEncap = 10 * 5.47;
	dBlueBulgeClosingRadiusDefect = 3.5 * 5.47;
	dBlueBulgeMinAreaDefect = 150 * 5.47 * 5.47;
	nBlueBulgeCircularityDefect = 10;
	nBlueBulgeHWRatioDefect = 10;
	nBlueBulgeInnerDefectPercent = 50;
	nBlueBulgeOffsetCheckDefect = 50;
	nBlueBulgeMaxContrastInOutDefect = 75;
	nBlueBulgeMinGrayDefect = 150;
	nBlueBulgeMaxGrayDefect = 240;
	nBlueBulgeMaxDeviationInnerDefect = 25;// End magnus
	nBulgePriority = 5;
	// Blow Hole

	dDarkBlueErosionRecHeight = 137;

	dDarkBlueMinMaskSizeSmallBH = 49.3;
	dDarkBlueMinSizeSmallBH = 4193;
	dDarkBlueMinCircularitySmallBH = 0.25;
	dDarkBlueMaxLayOnPartPercentageAllowSmallBH = 60;

	dDarkBlueMinMaskSizeLargeBH = 186.1;
	dDarkBlueMinSizeLargeBH = 54905;
	dDarkBlueMinCircularityLargeBH = 0.15;
	dDarkBlueMaxLayOnPartPercentageAllowLargeBH = 35;

	//Wire And IC
	nDarkBlueEncapLightWireMinIntensityWAI = 200;
	dDarkBlueEncapLightWireMaxLineWidthWAI = 54.8;
	nDarkBlueEncapLightWireContrastWAI = 30;
	dDarkBlueEncapLightWireMinAreaWAI = 3000;
	dDarkBlueEncapLightWireMinLengthWAI = 136.8;
	dDarkBlueEncapLightWireMinMeanGVDiffWAI = 15;

	dDarkBlueEncapFuzzyWireMaxLineWidthWAI = 54.8;
	nDarkBlueEncapFuzzyWireContrastWAI = 50;
	dDarkBlueEncapFuzzyWireMinAreaWAI = 7038;
	dDarkBlueEncapFuzzyWireMinLengthWAI = 82.1;
	dDarkBlueEncapFuzzyWireMinMeanGVDiffWAI = 7;

	dDarkBlueEncapICMinAreaWAI = 12000;}

// Flex Contructor
CFlex::CFlex() : CParameterCore("Flex", "Flex", -FLEX_OVERCUT)
{
	bEnable = FALSE;

	//Overcut
	bEnableCircuitLineCutInspection = FALSE;
	bEnableCircuitLineLinkedInspection = FALSE;
	dMinCircuitLineWidthOV = 2*5.5;
	dMaxCircuitLineWidthOV = 14*5.5;
	nOvercutPriority = 0;

	// Damage
	bEnableCircuitLineDamageInspection = FALSE;
	nMaxContrastDM = 100;
	dMinSizeDM = 20*5.5;
	nMinDefectCountCT = 1;
	nDamagePriority = 1;

	// Contamination
	nMinContrastCT = 60;
	nMinSubstrateContrastCT = 45;
	dMinSizeCT = 0;
	dMinSquareSizeCT = 0;
	dMinLengthCT = 0;
	dMaxCircularityCT = 1;
	nMinDefectCountCT = 1;
	nComtaminationPriority = 2;

	// Scratches
	nMinContrastSC = 25;
	dMaxWidthSC = 1*5.5;
	dMinLengthSC = 50*5.5;
	dMaxCircularitySC = 0.3;
	nMinDefectCountSC = 1;
	nScratchesPriority = 3;
}

CChickletSwageHole::CChickletSwageHole() : CParameterCore("ChickletSwageHole", "ChickletSwageHole", -CHICKLET_ROUNDNESS)
{
	bEnable = FALSE;
	nRoundnessValueDefect = 80;
	nBurrValueForRoundness = 7;
	nBurrValueDefect = 13;

}

CContact::CContact() : CParameterCore("Contact", "Contact", -CONTACT_DAMAGE)
{
	nDamagePriority = 0;
	nComtaminationPriority = 1;

	// // // // Black Contact
	bEnableBlackContact = FALSE;

	// Damage
	dBContactMaskSizeDM = 10 * 5.5;
	dBContactContactRegionOffsetDM = 13 * 5.5;
	dBContactMaxLineWidthDM = 15 * 5.5;
	nBContactLowContrastDM = 35;
	nBContactHighContrastDM = 45;
	dBContactMinLengthDM = 100 * 5.5;
	dBContactMaxCircularityDM = 0.5;
	nBContactMinDefectCountDM = 1;

	// Contamination
	nBContactHighContrastCT = 100;
	dBContactSmoothImageCT = 0.5;
	dBContactContactRegionOffsetCT = 35 * 5.5;
	nBContactContrastOffsetCT = 15;
	nBContactMinHoleNumberCT = 500;
	nBContactHoleNumberContrastOffsetCT = 30;
	nBContactVeryHighRoughDotRegionCT = 150;
	nBContactHighRoughDotRegionCT = 30;
	nBContactVeryHighRoughContrastOffsetCT = 15;
	nBContactHighRoughContrastOffsetCT = 10;
	dBContactMinStainSizeCT = 50 * 5.5 * 5.5;
	nBContactMinMeanEdgeGrayCT = 75;
	dBContactMinSizeCT = 20 * 5.5 * 5.5;
	nBContactMinMeanGrayCT = 40;
	dBContactMinAreaTracingPercentCT = 90;
	nBContactMinDefectCountCT = 1;

	// // // // White Contact
	bEnableWhiteContact = FALSE;

	// Damage
	dWContactMaskSizeDM = 3 * 5.5;
	dWContactContactRegionOffsetDM = 35 * 5.5;
	dWContactMaxLineWidthDM = 22 * 5.5;
	nWContactLowContrastDM = 20;
	nWContactHighContrastDM = 20;
	dWContactMinLengthDM = 50 * 5.5;
	dWContactMaxCircularityDM = 0.5;
	nWContactMinDefectCountDM = 1;

	// Contamination
	dWContactSmoothImageCT = 0.5;
	dWContactContactRegionOffsetCT = 20 * 5.5;
	nWContactWhiteDefectContrastOffsetCT = 10;
	nWContactMeanWhiteDefectGrayOffsetCT = 10;
	nWContactMinMeanWhiteDefectGrayCT = 220;
	nWContactBlackDefectContrastOffsetCT = 10;
	nWContactMeanBlackDefectGrayOffsetCT = 20;
	nWContactMaxMeanBlackDefectGrayCT = 255;
	dWContactMinSizeCT = 50 * 5.5 * 5.5;
	dWContactMinAreaTracingPercentCT = 90;
	nWContactMinDefectCountCT = 1;


}

//CDieInsp::CDieInsp() : CParameterCore("Die", "Die", -DIE_INSP)
//{
//	bEnable = TRUE;
//}

CTrainingData::CTrainingData()
{

	// Device Location //
	bDeviceLocationEnable = FALSE;//TRUE;
	nNoOfLocationTeachRegion = 1;
	nMinDeviceContrast = 50;
	dDeviceLocWidth = 5500;
	dDeviceLocHeight = 2750;
	dMaskSize = 360;
	bSobelAmp = FALSE;
	nDeviceLocSizeVariation = 1;
	m_rectDeviceLoc = CRect(0, 0, 0, 0);

	nDeviceReferencePointType = TOP_LEFT;
	nDeviceLocationType = EDGE_DETECTION;

	nDeviceHorizontalContrast = 40;
	nDeviceHorizontalDirection = TOP_TO_BOTTOM;
	nDeviceVerticalContrast = 40;
	nDeviceVerticalDirection = LEFT_TO_RIGHT;
	m_rectDeviceHorizontalLineLoc = CRect(0, 0, 0, 0);
	m_rectDeviceVerticalLineLoc = CRect(0, 0, 0, 0);


	// Dattam/Pattern Search //
	bDattamSearchEnable = FALSE;//TRUE;
	dDattamSearchAreaX = 100.0;
	dDattamSearchAreaY = 100.0;
	nDattamSearchMaxRotAngle = 10;
	nDattamSearchNoOfDattams = 2;
	dDattamSearchMinAcceptanceScore = 60;// 0.6;
	nDatumPatternType = CORRELATION_BASED;
	for (int nRectDattam = 0; nRectDattam < 4; nRectDattam++)
		m_rectDattamSearch[nRectDattam] = CRect(0, 0, 0, 0);


	nEncapTracingPolygonPointNumber = 3;
	for (int j = 0; j < 20; j++) {
		m_EncapTracingPolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}

	nEncapTopPolygonPointNumber = 3;
	for (int j = 0; j < 20; j++) {
		m_EncapTopPolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}

	nEncapBottomPolygonPointNumber = 3;
	for (int j = 0; j < 20; j++) {
		m_EncapBottomPolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}

	nEncapWirePolygonPointNumber = 8;
	for (int j = 0; j < 20; j++) {
		m_EncapWirePolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}

	nEncapWireBottomPolygonPointNumber = 4;
	for (int j = 0; j < 20; j++) {
		m_EncapWireBottomPolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}

	nEncapICPolygonPointNumber = 8;
	for (int j = 0; j < 20; j++) {
		m_EncapICPolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}
	
	m_rectEncapMaskingLeftPolygonPointArea = CRect(300, 300, 500, 500);
	m_rectEncapMaskingRightPolygonPointArea = CRect(300, 300, 500, 500);
	m_rectEncapMaskingLimitPolygonPointArea = CRect(300, 300, 500, 500);
	// // // // Blue Encap Insufficient
	// // Left Encap Region For Checking Insufficient
	m_rectBlueEncapInsuffLeftRegionPolygonPointArea = CRect(100, 100, 400, 400);

	// // Right Encap Region For Checking Insufficient
	m_rectBlueEncapInsuffRightRegionPolygonPointArea = CRect(100, 100, 400, 400);

	// // Region For Find border of the base
	m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea = CRect(100, 100, 400, 400);

	// // Middle Encap Region For Checking Insufficient
	nBlueEncapInsuffMidRegionPolygonPointNumber = 8;
	for (int j = 0; j < 20; j++) {
		m_BlueEncapInsuffMidRegionPolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}

	
	// // // // Blue Encap Excess
	// // Left Encap Region For Checking Excess
	m_rectBlueEncapExcessLeftRegionPolygonPointArea = CRect(100, 100, 400, 400);
	// // Right Encap Region For Checking Excess
	m_rectBlueEncapExcessRightRegionPolygonPointArea = CRect(100, 100, 400, 400);

	// Glue - Mason
	for (int k = 0; k < ENCAP_GLUE_MASK_POINT; k++) {
		m_polygonBEncapGlueMask[k] = CPoint(700 + 50 * k, 700);
	}
	GenEmptyObj(&hBEncapGlueMask);

	// Contamination - Mason
	m_rectBEncapFlexEdgeMaskROI = CRect(0, 0, 0, 0);
	GenEmptyObj(&hBEncapFlexEdgeMaskROI);

	// Encap region //
	bEnableEncap = FALSE;
	bEnableEncapBlue = FALSE;
	dErosionRadiusEncapTracing = 5.47;
	nSmoothingIndexEncapTracing = 251;
	nIntensityEncapTracing = 70;
	dSlideWindowSideEncapTracing = 38.3;
	nEdgeFlexContrastEncapTracing = 35;
	dSizeMaskingEncapTracing = 109.45;
	nIntensityForFindBorderBaseBlueEncap = 74;
	dOffsetLeftBlueEncap = 0;
	dOffsetRightBlueEncap = 0;

	m_rectDarkBottomEncap = CRect(0, 0, 0, 0);
	m_nDarkBottomRectWidth = 0;
	m_nDarkBottomRectHeight = 0;

	nEncapTracingMaskNumber = 0;
	for (int k = 0; k < ENCAP_MAX_MASK_SURFACE; k++) {
		nEncapTracingMaskPolygonPointNumber[k] = 3;
		for (int j = 0; j < 20; j++) {
			m_EncapTracingMaskPolygonPointArea[k][j] = CPoint(700 + 50 * j, 700);
		}
	}

	// Contact Region //
	bEnableContact = FALSE;

	// Contact Mask
	nContactMaskNumber = 0;
	for (int k = 0; k < CONTACT_MAX_REGION_MASK; k++) {
		m_rectContactMask[k] = CRect(0, 0, 0, 0);
	}

	// Contact Mask
	nContactRegionMaskNumber = 0;
	for (int k = 0; k < CONTACT_MAX_REGION_MASK; k++) {
		m_rectContactRegionMask[k] = CRect(0, 0, 0, 0);
	}

	// Black Contact
	bEnableBlackContact = FALSE;
	nBContactContactContrast = 65;
	nBContactCircleContrast = 55;

	// White Contact
	bEnableWhiteContact = FALSE;
	nWContactContactContrast = 150;
	nWContactCircleContrast = 175;

	// Contact Mask Tracing
	nContactTracingMaskNumber = 0;
	for (int k = 0; k < CONTACT_MAX_TRACING_MASK; k++) {
		nContactTracingMaskPolygonPointNumber[k] = 3;
		for (int j = 0; j < CONTACT_MAX_POLYGON_POINT; j++) {
			m_polygonContactTracingMask[k][j] = CPoint(700 + 50 * j, 700);
		}
	}

	// Contact Teaching Data
	GenEmptyObj(&hContactMask);
	GenEmptyObj(&hContactRegionMask);
	GenEmptyObj(&hContactTracingMask);


	// Flex Region //
	bFlexEnable = FALSE;
	bFlexEnableSubstrateMask = FALSE;
	bFlexEnableDynamicEdge = FALSE;
	dFlexEdgeOffset = 0;
	nFlexEdgeContrast = 60;
	nFlexCircuitLineContrast = 95;
	nFlexEncapContrast = 27;
	dFlexOffsetMaskSize = 3.5*5.5;
	nFlexStraightCircuitLineMaskPolygonPointNumber = 3;

	m_rectFlexSubstratePatternROI1 = CRect(0, 0, 0, 0);
	m_rectFlexSubstratePatternROI2 = CRect(0, 0, 0, 0);
	m_rectFlexSubstrateSearchPatternROI = CRect(0, 0, 0, 0);

	for (int j = 0; j < 2; j++) {
		m_polygonFlexTopEdgeLine[j] = CPoint(700 + 50 * j, 700);
	}
	for (int j = 0; j < 2; j++) {
		m_polygonFlexBottomEdgeLine[j] = CPoint(700 + 50 * j, 700);
	}
	for (int j = 0; j < 2; j++) {
		m_polygonFlexLeftEdgeLine[j] = CPoint(700 + 50 * j, 700);
	}
	for (int j = 0; j < 2; j++) {
		m_polygonFlexRightEdgeLine[j] = CPoint(700 + 50 * j, 700);
	}

	m_rectFlexLeftEdgeLineMask = CRect(0, 0, 0, 0);
	m_rectFlexRightEdgeLineMask = CRect(0, 0, 0, 0);

	m_rectFlexBottomROI = CRect(0, 0, 0, 0);

	for (int j = 0; j < nFlexStraightCircuitLineMaskPolygonPointNumber; j++) {
		m_polygonFlexStraightCircuitLineMask[j] = CPoint(700 + 50 * j, 700);
	}

	m_rectFlexNumberMask = CRect(0, 0, 0, 0);

	nFlexTracingMaskNumber = 0;
	for (int k = 0; k < FLEX_MAX_TRACING_MASK; k++) {
		nFlexTracingMaskPolygonPointNumber[k] = 3;
		for (int j = 0; j < FLEX_MAX_POLYGON_POINT; j++) {
			m_polygonFlexTracingMask[k][j] = CPoint(700 + 50 * j, 700);
		}
	}

	nFlexDamageMaskNumber = 0;
	for (int k = 0; k < FLEX_MAX_DAMAGE_MASK; k++) {
		nFlexDamageMaskPolygonPointNumber[k] = 3;
		for (int j = 0; j < FLEX_MAX_POLYGON_POINT; j++) {
			m_polygonFlexDamageMask[k][j] = CPoint(700 + 50 * j, 700);
		}
	}

	nFlexSubstrateMaskNumber = 0;
	for (int k = 0; k < FLEX_MAX_SUBSTRATE_MASK; k++) {
		nFlexSubstrateMaskPolygonPointNumber[k] = 3;
		for (int j = 0; j < FLEX_MAX_POLYGON_POINT; j++) {
			m_polygonFlexSubstrateMask[k][j] = CPoint(700 + 50 * j, 700);
		}
	}

	// Data Teaching Of Flex
	GenEmptyObj(&hFlexSubstrateSearchPatternROI);

	GenEmptyObj(&hFlexTopEdgeLine);
	GenEmptyObj(&hFlexBottomEdgeLine);
	GenEmptyObj(&hFlexLeftEdgeLine);
	GenEmptyObj(&hFlexRightEdgeLine);

	GenEmptyObj(&hFlexBottomMask);
	GenEmptyObj(&hFlexStraightCircuitLineMask);

	GenEmptyObj(&hFlexNumberMask);
	GenEmptyObj(&hFlexTracingMask);
	GenEmptyObj(&hFlexDamageMask);
	GenEmptyObj(&hFlexSubstrateMask);

	hFlexSubstrateModelID = 0;
	hFlexSubstrateDatumMatchRowRef = 0;
	hFlexSubstrateDatumMatchColRef = 0;
	hFlexSubstrateMatchAngleRef = 0;
	GenEmptyObj(&hFlexTracingMaskRegion);
	hFlexLeftWidthReference = 0;
	hFlexRightWidthReference = 0;
	

	hvec_TupleVectorLocation.RemoveAll();
	for (int i = 0; i < 32; i++) {
		if (i < 30)
			hvec_TupleVectorLocation.Add(HTuple());
		hvec_TupleVectorPVI[i].RemoveAll();
		for (int j = 0; j < 10; j++)
			darrayPVI[i][j].RemoveAll();
	}


	hvec_TupleVectorEncap.RemoveAll();

	for (int k = 0; k < 15; k++) {
		darrayEncap[k].RemoveAll();
	}

	/* Chicklet Swage Hole Teach Param */
	bEnableChickletSwageHole = FALSE;
	m_nChickletMinContrast = 15;
	m_nChickletMaxContrast = 50;
	m_nChickletOffsettRadius = 5; // % Radius
	m_nChickletNumberOfHoles = 1;
	for (int k = 0; k < 10; k++)
	{
		m_rectChickletROI[k] = CRect(0, 0, 0, 0);
	}

	// DarkEncap Teach Param 
	bEnableDarkEncap = false;
	m_nSmoothDarkEncap = 15;
	m_nLeftRightContrastDarkEncap = 35;
	m_nWhiteContrastDarkEncap = 240;
	m_nAreaWhiteRemoveDarkEncap = 20;
	m_nRoughContrastDarkEncap = 50;
	m_nContrastFlexDarkEncap = 15;
	m_nMinContrastFlexDarkEncap = 10;
	m_nMaxContrastFlexDarkEncap = 190;

	// DarkEncap Teack region
	m_rectAllRegionDarkEncap = CRect(0, 0, 0, 0);
	m_rectTopFlexDarkEncap = CRect(0, 0, 0, 0);
	m_rectBotWhiteDarkEncap = CRect(0, 0, 0, 0);
	nDarkEncapOutsidePolygonPointNumber = 12;
	for (int j = 0; j < 20; j++) {
		m_EncapTracingPolygonPointArea[j] = CPoint(700 + 50 * j, 700);

	}
	nDarkEncapInsidePolygonPointNumber = 10;
	for (int j = 0; j < 20; j++) {
		m_EncapTopPolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}

	// Excess - Magnus
	m_rectDarkExcessLeftMask = CRect(0, 0, 0, 0);
	m_rectDarkExcessRightMask = CRect(0, 0, 0, 0);

	// Insufficient - Magnus
	m_rectDarkInsufficientCoverMask = CRect(0, 0, 0, 0);

	for (int k = 0; k < DARK_ENCAP_INNER_INSUFFICIENT_MASK_POINT; k++) {
		m_polygonDarkInsufficientMidMask[k] = CPoint(700 + 50 * k, 700);
	}

	m_rectDarkInsufficientLeftMask = CRect(0, 0, 0, 0);


	m_rectDarkInsufficientRightMask = CRect(0, 0, 0, 0);

	// Glue - Mason
	m_rectDarkGlueMask = CRect(0, 0, 0, 0);
	GenEmptyObj(&hDarkGlueMask);

	// Contamination - Mason
	m_rectDarkFlexEdgeMask = CRect(0, 0, 0, 0);
	m_rectDarkBottomEdgeMask = CRect(0, 0, 0, 0);
	m_rectDarkRightEdgeMask = CRect(0, 0, 0, 0);
	m_rectDarkInnerMask = CRect(0, 0, 0, 0);
	GenEmptyObj(&hDarkFlexEdgeMask);
	GenEmptyObj(&hDarkBottomEdgeMask);
	GenEmptyObj(&hDarkRightEdgeMask);
	GenEmptyObj(&hDarkInnerMask);	// // Void
	m_rectDarkEncapVoidMaskingLeftDiePointArea = CRect(0, 0, 200, 200);
	m_rectDarkEncapVoidMaskingRightDiePointArea = CRect(0, 0, 200, 200);

	// // Wire And IC
	nDarkEncapWirePolygonPointNumber = 12;
	for (int j = 0; j < 20; j++) {
		m_DarkEncapWirePolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}

	nDarkEncapICPolygonPointNumber = 12;
	for (int j = 0; j < 20; j++) {
		m_DarkEncapICPolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}

	// Dark Blue Encap Region //
	bEnableDarkBlueEncap = false;
	nDBEncapEdgeEncapContrast = 85;
	nDBEncapEdgeFlexContrast = 140;
	nDBEncapMinEncapContrast = 130;
	nDBEncapMaxEncapContrast = 220;

	for (int k = 0; k < DARK_BLUE_ENCAP_ROI_POINT; k++) {
		m_polygonDBEncapExtractROI[k] = CPoint(700 + 50 * k, 700);
	}

	m_rectDBEncapInnerROI = CRect(0, 0, 0, 0);
	m_rectDBEncapTopEdgeMaskROI = CRect(0, 0, 0, 0);
	m_rectDBEncapBottomEdgeMaskROI = CRect(0, 0, 0, 0);
	m_rectDBEncapLeftEdgeMaskROI = CRect(0, 0, 0, 0);
	m_rectDBEncapRightEdgeMaskROI = CRect(0, 0, 0, 0);
	m_rectDBEncapFlexEdgeMaskROI = CRect(0, 0, 0, 0);

	// Data Teaching Of Dark Blue Encap
	GenEmptyObj(&hDBEncapExtractROI);
	GenEmptyObj(&hDBEncapInnerROI);
	GenEmptyObj(&hDBEncapTopEdgeMaskROI);
	GenEmptyObj(&hDBEncapBottomEdgeMaskROI);
	GenEmptyObj(&hDBEncapLeftEdgeMaskROI);
	GenEmptyObj(&hDBEncapRightEdgeMaskROI);
	GenEmptyObj(&hDBEncapFlexEdgeMaskROI);

	// Excess - Mason
	m_rectDBLeftExcessMask = CRect(0, 0, 0, 0);
	m_rectDBRightExcessMask = CRect(0, 0, 0, 0);
	GenEmptyObj(&hDBLeftExcessMask);
	GenEmptyObj(&hDBRightExcessMask);

	// Insufficient - Mason
	m_rectDBCoverInsufficientMask = CRect(0, 0, 0, 0);
	GenEmptyObj(&hDBCoverInsufficientMask);

	for (int k = 0; k < DARK_BLUE_ENCAP_INNER_INSUFFICIENT_MASK_POINT; k++) {
		m_polygonDBInnerInsufficientMask[k] = CPoint(700 + 50 * k, 700);
	}
	GenEmptyObj(&hDBInnerInsufficientMask);

	m_rectDBLeftInsufficientMask = CRect(0, 0, 0, 0);
	GenEmptyObj(&hDBLeftInsufficientMask);

	m_rectDBRightInsufficientMask = CRect(0, 0, 0, 0);
	GenEmptyObj(&hDBRightInsufficientMask);

	// Glue - Mason
	for (int k = 0; k < ENCAP_GLUE_MASK_POINT; k++) {
		m_polygonDBEncapGlueMask[k] = CPoint(700 + 50 * k, 700);
	}
	GenEmptyObj(&hDBEncapGlueMask);

	//Dark Blue Region for Void encap Inspection- magnus
	m_rectDBlueVoidROI = CRect(0, 0, 0, 0);
	// Region for Sliver encap Inspection
	m_rectDBlueSliverROI = CRect(0, 0, 0, 0);
	m_rectDBlueSliverTopROI = CRect(0, 0, 0, 0);
	m_rectDBlueSliverEncapROI = CRect(0, 0, 0, 0);

	//Blue Region for Bulge encap Inspection
	m_rectBlueBulgeROI = CRect(0, 0, 0, 0);
	//Blue Region for Sliver encap Inspection
	m_rectBlueSliverROI = CRect(0, 0, 0, 0);
	m_rectBlueSliverTopROI = CRect(0, 0, 0, 0);
	m_rectBlueSliverEncapROI = CRect(0, 0, 0, 0);// End magnus
	// Dark Blue Encap BlowHole
	m_rectDarkEncapBHMaskingLeftPolygonPointArea = CRect(0, 0, 0, 0);
	m_rectDarkEncapBHMaskingRightPolygonPointArea = CRect(0, 0, 0, 0);
	// Dark Blue Encap Wire And IC

	nDarkBlueEncapWireBottomPolygonPointNumber = 4;
	for (int j = 0; j < 20; j++) {
		m_DarkBlueEncapWireBottomPolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}

	nDarkBlueEncapICPolygonPointNumber = 8;
	for (int j = 0; j < 20; j++) {
		m_DarkBlueEncapICPolygonPointArea[j] = CPoint(700 + 50 * j, 700);
	}}


CCalibrationData::CCalibrationData()
{
	rectCalibSideTrack = CRect(100, 100, 300, 300);
	rectDotCalib = CRect(100, 100, 300, 300);

	nCalibTolerence = 5;
	nTargetGreyDeviation = 10;
	nTargetGreyMeanValue = 230;

	nTargetGreyMeanValueSide = 150;
	nCalibTolerenceSide = 5;

	nInputAlongX = 500;
	nInputAlongY = 500;
	dResolutionAlongXInMicronPerPixel = 0.0;
	dResolutionAlongYInMicronPerPixel = 0.0;

	nInputMinRadiusDotTargetPixel = 10;

	hResolutionYInput = HTuple();

	GenEmptyObj(&hCalibSumImage);
	GenEmptyObj(&hCalibAvgImage);
}


CInspectinonData::CInspectinonData()
{
	hStoreTeachModelRowCentre = HTuple();
	hStoreTeachModelColCentre = HTuple();
	hStoreMatchModelCentreRow = HTuple();
	hStoreMatchModelCentreCol = HTuple();


	hInspectShiftAlongXMicron = HTuple();
	hInspectShiftAlongYMicron = HTuple();
}

CInspectionCore::CInspectionCore()
{
	m_bIsPatternLens = FALSE;

	m_dPixelSize = 8.33;
	m_bParallelism = FALSE;
	m_pShare = NULL;

	m_bInspectionPriority = FALSE;
}


CInspectionCore::~CInspectionCore()
{
	/*for(int i=0; i<THREAD_COUNT; i++)
		CloseHandle(handlesDone[i]);*/
	for (int i = 0; i < 2; i++)
		m_arrayOverlay[i].RemoveAll();

	m_arrayOverlayInspection.RemoveAll();
	m_arrayOverlayTeach.RemoveAll();
	m_arrayOverlayDotCalibration.RemoveAll();
	m_arrayOverlayPVIInsp.RemoveAll();
}

int CInspectionCore::Teach(HImage hImages,
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
	COLORREF colorRed = RGB(255, 0, 0);
	COLORREF colorGreen = RGB(0, 255, 0);
	COLORREF colorBlue = RGB(0, 0, 255);
	COLORREF colorYellow = RGB(255, 255, 0);
	COLORREF colorOrange = RGB(255, 165, 0);
	int nStepDebug = 0;

	//Device/Datum Teach
	if (!nTeachStep) {
		m_arrayOverlayTeach.RemoveAll();

		HTuple hDoc = 0;
		HTuple hUniqueNoPatternsRows, hUniqueNoPatternsCols;
		HTuple hLocationTeachRows, hLocationTeachCols;
		HTuple hRefRotationAngles, hRefPointRows, hRefPointCols, hModelIDs;
		HTuple hGotRotationAngleDev = 0;
		HTuple hGotShiftAlongRow = 0;
		HTuple hGotShiftAlongCol = 0;

		HTuple hIsDatum, hMatchModelScore, hDatumRefRotationAngle, hDatumRefPointRow, hDatumRefPointCol, hDebugMessageOut;
		HImage hDebugImageOut;
		HRegion hMatchModelRegion, hDebugRegionOut;

		// Unique Pattern
		HTuple hDatumSearchEnable, hNoOfUniqueRegions, hMaxRotationAngle, hPatternSearchAreaAlongX, hPatternSearchAreaAlongY, hMinAcceptanceScore, hPatternSearchType;
		hDatumSearchEnable = m_TrainingData.bDattamSearchEnable;
		hNoOfUniqueRegions = m_TrainingData.nDattamSearchNoOfDattams;
		hMaxRotationAngle = m_TrainingData.nDattamSearchMaxRotAngle;
		hPatternSearchAreaAlongX = m_TrainingData.dDattamSearchAreaX / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		hPatternSearchAreaAlongY = m_TrainingData.dDattamSearchAreaY / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		hMinAcceptanceScore = (m_TrainingData.dDattamSearchMinAcceptanceScore / 100.0);
		hPatternSearchType = m_TrainingData.nDatumPatternType;
		if (hDatumSearchEnable == 1) {
			hDoc = 1;
			for (int i = 0; i < m_TrainingData.nDattamSearchNoOfDattams; i++) {
				TupleConcat(hUniqueNoPatternsRows, m_TrainingData.m_rectDattamSearch[i].top, &hUniqueNoPatternsRows);
				TupleConcat(hUniqueNoPatternsRows, m_TrainingData.m_rectDattamSearch[i].bottom, &hUniqueNoPatternsRows);
				TupleConcat(hUniqueNoPatternsCols, m_TrainingData.m_rectDattamSearch[i].left, &hUniqueNoPatternsCols);
				TupleConcat(hUniqueNoPatternsCols, m_TrainingData.m_rectDattamSearch[i].right, &hUniqueNoPatternsCols);
			}

			_FCI_TeachUniquePattern(hImages,
				hNoOfUniqueRegions, hMaxRotationAngle,
				hUniqueNoPatternsRows, hUniqueNoPatternsCols, hPatternSearchType,
				&hModelIDs);

			_FCI_Inspect_UniquePattern(hImages,
				&hMatchModelRegion, &hDebugImageOut, &hDebugRegionOut,
				nStepDebug, hPatternSearchAreaAlongX, hPatternSearchAreaAlongY, hMaxRotationAngle,
				hMinAcceptanceScore, hPatternSearchType, hModelIDs, hUniqueNoPatternsRows, hUniqueNoPatternsCols,
				&hIsDatum, &hMatchModelScore, &hDatumRefRotationAngle, &hDatumRefPointRow, &hDatumRefPointCol, &hDebugMessageOut);




		}

		HRegion hDeviceLocation;
		HTuple hDeviceLocationEnable, hNoOfLocationRegions, hDeviceLocWidth, hDeviceLocHeight, hDeviceMaskSize, hMinDeviceContrast, hSobelAmp, hReferencePointType;
		HTuple hDebugMsgOut, hIsPass, hEdgeRefRotationAngle, hEdgeRefPointRow, hEdgeRefPointCol;
		HTuple hDeviceEdgeDetectionEnable, hDeviceHorizontalContrast, hDeviceHorizontalDirection, hDeviceVerticalContrast, hDeviceVerticalDirection;
		HTuple hDeviceEdgeLocationTeachRows, hDeviceEdgeLocationTeachCols;


		hDeviceLocationEnable = m_TrainingData.bDeviceLocationEnable;
		hNoOfLocationRegions = m_TrainingData.nNoOfLocationTeachRegion;
		hDeviceLocWidth = m_TrainingData.dDeviceLocWidth / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		hDeviceLocHeight = m_TrainingData.dDeviceLocHeight / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		hDeviceMaskSize = m_TrainingData.dMaskSize / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		hMinDeviceContrast = m_TrainingData.nMinDeviceContrast;
		hSobelAmp = m_TrainingData.bSobelAmp;
		hReferencePointType = m_TrainingData.nDeviceReferencePointType;

		hDeviceEdgeDetectionEnable = m_TrainingData.nDeviceLocationType == EDGE_DETECTION ? 1 : 0;
		hDeviceHorizontalContrast = m_TrainingData.nDeviceHorizontalContrast;
		hDeviceHorizontalDirection = m_TrainingData.nDeviceHorizontalDirection;
		hDeviceVerticalContrast = m_TrainingData.nDeviceVerticalContrast;
		hDeviceVerticalDirection = m_TrainingData.nDeviceVerticalDirection;

		HTuple hFOV, hFOVs, hTrack;
		hFOV = nFOV;
		hFOVs = nTotalFOV;
		hTrack = nCurTrack + 1;

		GenEmptyRegion(&hDeviceLocation);
		if (hDeviceLocationEnable == 1) {
			hDoc = 1;
			for (int i = 0; i < m_TrainingData.nNoOfLocationTeachRegion; i++) {
				TupleConcat(hLocationTeachRows, m_TrainingData.m_rectDeviceLoc.top, &hLocationTeachRows);
				TupleConcat(hLocationTeachRows, m_TrainingData.m_rectDeviceLoc.bottom, &hLocationTeachRows);
				TupleConcat(hLocationTeachCols, m_TrainingData.m_rectDeviceLoc.left, &hLocationTeachCols);
				TupleConcat(hLocationTeachCols, m_TrainingData.m_rectDeviceLoc.right, &hLocationTeachCols);
			}

			//Use edge detection to detect the device edges instead of blob detection Algo
			if (hDeviceEdgeDetectionEnable == 1) {

				TupleConcat(hDeviceEdgeLocationTeachRows, m_TrainingData.m_rectDeviceHorizontalLineLoc.top, &hDeviceEdgeLocationTeachRows);
				TupleConcat(hDeviceEdgeLocationTeachRows, m_TrainingData.m_rectDeviceHorizontalLineLoc.bottom, &hDeviceEdgeLocationTeachRows);
				TupleConcat(hDeviceEdgeLocationTeachRows, m_TrainingData.m_rectDeviceVerticalLineLoc.top, &hDeviceEdgeLocationTeachRows);
				TupleConcat(hDeviceEdgeLocationTeachRows, m_TrainingData.m_rectDeviceVerticalLineLoc.bottom, &hDeviceEdgeLocationTeachRows);

				TupleConcat(hDeviceEdgeLocationTeachCols, m_TrainingData.m_rectDeviceHorizontalLineLoc.left, &hDeviceEdgeLocationTeachCols);
				TupleConcat(hDeviceEdgeLocationTeachCols, m_TrainingData.m_rectDeviceHorizontalLineLoc.right, &hDeviceEdgeLocationTeachCols);
				TupleConcat(hDeviceEdgeLocationTeachCols, m_TrainingData.m_rectDeviceVerticalLineLoc.left, &hDeviceEdgeLocationTeachCols);
				TupleConcat(hDeviceEdgeLocationTeachCols, m_TrainingData.m_rectDeviceVerticalLineLoc.right, &hDeviceEdgeLocationTeachCols);


				_FCI_Inspect_Cross_Point(hImages, &hDebugImageOut, &hDebugRegionOut, hDeviceHorizontalContrast, hDeviceHorizontalDirection,
					hDeviceVerticalContrast, hDeviceVerticalDirection, hDeviceEdgeLocationTeachRows, hDeviceEdgeLocationTeachCols, hMaxRotationAngle,
					1, &hEdgeRefRotationAngle, &hEdgeRefPointRow, &hEdgeRefPointCol, &hDebugMessageOut, &hIsPass);
			}
			else {
				_FCI_Inspect_EdgeLocation(hImages,
					&hDeviceLocation,
					hSobelAmp, hNoOfLocationRegions, hDeviceLocWidth, hDeviceLocHeight,
					hMinDeviceContrast, hLocationTeachRows, hLocationTeachCols, hDeviceMaskSize, hFOV, hFOVs, hTrack, hReferencePointType,
					&hEdgeRefRotationAngle, &hEdgeRefPointRow, &hEdgeRefPointCol);
			}

		}
		else {
			hDoc = 1;
			for (int i = 0; i < m_TrainingData.nNoOfLocationTeachRegion; i++) {
				TupleConcat(hLocationTeachRows, m_TrainingData.m_rectDeviceLoc.top, &hLocationTeachRows);
				TupleConcat(hLocationTeachRows, m_TrainingData.m_rectDeviceLoc.bottom, &hLocationTeachRows);
				TupleConcat(hLocationTeachCols, m_TrainingData.m_rectDeviceLoc.left, &hLocationTeachCols);
				TupleConcat(hLocationTeachCols, m_TrainingData.m_rectDeviceLoc.right, &hLocationTeachCols);
			}
			GenRectangle1(&hDeviceLocation, HTuple(hLocationTeachRows[0]), HTuple(hLocationTeachCols[0]),
				HTuple(hLocationTeachRows[1]), HTuple(hLocationTeachCols[1]));

			HTuple hv_Area, hv_Dummy, hv_Dummx, hv_DeviceAngle, hv_Device2DCenter, hv_DeviceSize;
			AreaCenter(hDeviceLocation, &hv_Area, &hv_Dummy, &hv_Dummx);
			if (0 != (HTuple(hv_Area > 0).TupleAnd((hv_Area.TupleLength()) > 0))) {
				EllipticAxis(hDeviceLocation, &hv_Dummy, &hv_Dummx, &hEdgeRefRotationAngle);

				TopReferencePoints(hImages, hDeviceLocation, hFOV, hFOVs, hReferencePointType, &hEdgeRefPointRow,
					&hEdgeRefPointCol, &hv_DeviceAngle, &hv_Device2DCenter, &hv_DeviceSize);
			}
		}

		//Show the top reference point inspected
		HObject hCross, hCrossRegion, hDebugCrossRegion;
		HTuple hCrossRow, hCrossCol;
		GenCrossContourXld(&hCross, hEdgeRefPointRow, hEdgeRefPointCol, 200, 0);
		GetContourXld(hCross, &hCrossRow, &hCrossCol);
		GenRegionPolygon(&hCrossRegion, hCrossRow, hCrossCol);

		m_arrayOverlayTeach.Add(hCrossRegion, colorYellow, "Top Reference Point Found By Inspect Edge Device Location");


		HTuple hRefRotationAngle, hRefPointRow, hRefPointCol;
		HTuple hGotRotationAngle = 0;
		if (hDeviceLocationEnable == 1 || hDatumSearchEnable == 1)
		{
			if (hDatumSearchEnable == 1)
			{
				hRefRotationAngle = hDatumRefRotationAngle;
				hRefPointRow = hDatumRefPointRow;
				hRefPointCol = hDatumRefPointCol;
			}
			else
			{
				hRefRotationAngle = hEdgeRefRotationAngle;
				hRefPointRow = hEdgeRefPointRow;
				hRefPointCol = hEdgeRefPointCol;
			}

			HTuple hTopRefGotRotationAngle, hTopRefGotShiftAlongRow, hTopRefGotShiftAlongCol;

			_FCI_Inspect_DeviceLocation(hImages,
				&hDeviceLocation, &hMatchModelRegion,
				&hDebugImageOut, &hDebugRegionOut,
				hDatumSearchEnable, hDeviceLocationEnable, nStepDebug,
				hSobelAmp, hNoOfLocationRegions,
				hDeviceLocWidth, hDeviceLocHeight, hMinDeviceContrast,
				hNoOfUniqueRegions, hMaxRotationAngle,
				hPatternSearchAreaAlongX, hPatternSearchAreaAlongY, hMinAcceptanceScore, hPatternSearchType,
				hModelIDs, hRefRotationAngle,
				hRefPointRow, hRefPointCol,
				hEdgeRefRotationAngle, hEdgeRefPointRow, hEdgeRefPointCol,
				hUniqueNoPatternsRows, hUniqueNoPatternsCols,
				hLocationTeachRows, hLocationTeachCols,
				hDeviceMaskSize, hFOV, hFOVs, hTrack, hReferencePointType,
				hDeviceEdgeDetectionEnable,
				hDeviceHorizontalContrast, hDeviceHorizontalDirection,
				hDeviceVerticalContrast, hDeviceVerticalDirection,
				hDeviceEdgeLocationTeachRows, hDeviceEdgeLocationTeachCols,
				&hIsPass, &hIsDatum, &hGotRotationAngle, &hGotShiftAlongRow, &hGotShiftAlongCol,
				&hMatchModelScore,
				&hTopRefGotRotationAngle, &hTopRefGotShiftAlongRow, &hTopRefGotShiftAlongCol,
				&hDebugMsgOut);

			if (0 != hIsPass)
				m_arrayOverlayTeach.Add(hDeviceLocation, colorGreen);
			if (0 != hIsDatum)
				m_arrayOverlayTeach.Add(hMatchModelRegion, colorGreen);
		}
		ConcatObj(pTrainingData->hObjectDeviceLocation, hDeviceLocation, &pTrainingData->hObjectDeviceLocation);
		TupleConcat(pTrainingData->hTupleProjectionParam, (hGotRotationAngle.TupleConcat(hGotShiftAlongRow)).TupleConcat(hGotShiftAlongCol), &pTrainingData->hTupleProjectionParam);
		TupleConcat(pTrainingData->hTupleTeachDoc, hDoc, &pTrainingData->hTupleTeachDoc);

		TupleConcat(pTrainingData->hTupleDatumCenterParam, (hDatumRefRotationAngle.TupleConcat(hDatumRefPointRow)).TupleConcat(hDatumRefPointCol), &pTrainingData->hTupleDatumCenterParam);


		int nIndex = 0;
		//Device Location Param
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceLocationEnable);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hNoOfLocationRegions);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceLocWidth);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceLocHeight);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceMaskSize);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hMinDeviceContrast);

		//Datum/Pattern Search
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDatumSearchEnable);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hNoOfUniqueRegions);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hMaxRotationAngle);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hPatternSearchAreaAlongX);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hPatternSearchAreaAlongY);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hMinAcceptanceScore);

		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hUniqueNoPatternsRows);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hUniqueNoPatternsCols);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hLocationTeachRows);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hLocationTeachCols);

		//Datum/Pattern Training Data
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hModelIDs);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hRefRotationAngle);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hRefPointRow);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hRefPointCol);

		//Device Location Training Data
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hEdgeRefRotationAngle);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hEdgeRefPointRow);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hEdgeRefPointCol);

		//For Device Edge Detection option - Param
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceEdgeDetectionEnable);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceHorizontalContrast);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceHorizontalDirection);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceVerticalContrast);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceVerticalDirection);

		//For Device Edge Detection option - Training Data
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceEdgeLocationTeachRows);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceEdgeLocationTeachCols);

	}
	//PVI Teach
	else if (nTeachStep == 1) {
		m_arrayOverlayTeach.RemoveAll();

		HRegion hDeviceLocationSelected;
		HTuple hIsSelected, hInspecRotationAngle, hInspectShiftAlongRow, hInspectShiftAlongCol;
		HTuple hCount, hArea, hDeviceCenterRow, hDeviceCenterCol;

		_FCI_Extract_LocationParameterData(pTrainingData->hTupleProjectionParam, pTrainingData->hTupleTeachDoc,
			pTrainingData->hTupleEdgeTeachTuple, pTrainingData->hTupleDatumTeachTuple, nDoc,
			&hIsSelected, &hInspecRotationAngle, &hInspectShiftAlongRow, &hInspectShiftAlongCol);

		_FCI_Select_DeviceLocation(pTrainingData->hObjectDeviceLocation,
			&hDeviceLocationSelected,
			nDoc, &hDeviceCenterRow, &hDeviceCenterCol);


		CountObj(hDeviceLocationSelected, &hCount);
		if (hCount < 1)
			return 0;

		//AreaCenter(hDeviceLocationSelected, &hArea, &hDeviceCenterRow, &hDeviceCenterCol);


		//// ---- Region ---- ////
		HTuple hPVIArea, hRegionCenterRow, hRegionCenterColumn;

		hPVIArea = m_TrainingData.nNoOfPviArea;
		HTuple hPVIAngle, hPVI2DCenter, hPVISize, hPVIPolyLinePoints, hPVIPolygonMaskPoint;
		hPVIAngle = hPVI2DCenter = hPVISize = hPVIPolyLinePoints = HTuple();

		//// ---- Region Mask ---- ////
		HTuple hMaskNumber;
		hMaskNumber = 0;

		HTuple hMask2DCenterPoints, hMaskAngleConcat, hMaskSize;
		hMask2DCenterPoints = hMaskAngleConcat = hMaskSize = HTuple();

		m_TrainingData.darrayPVI[0][0].Add(hPVIArea.D());
		m_TrainingData.hvec_TupleVectorPVI[0].InsertAt(0, hPVIArea);

		HRegion hFitPVIRegion;
		HRegion hPVIRegion;
		for (int nArea = 0; nArea < hPVIArea; nArea++)
		{
			hPVIPolyLinePoints = hPVIPolygonMaskPoint = HTuple();
			if (hPVIArea != 0) {
				if (m_pPviInspectionSetup[nArea].bEnable || m_pPviInspectionSetup[nArea].bEdgeEnable) {

					HTuple hRow1, hCol1, hRow2, hCol2;
					hRow1 = m_pPviInspectionSetup[nArea].m_rectPviArea.top;
					hCol1 = m_pPviInspectionSetup[nArea].m_rectPviArea.left;
					hRow2 = m_pPviInspectionSetup[nArea].m_rectPviArea.bottom;
					hCol2 = m_pPviInspectionSetup[nArea].m_rectPviArea.right;

					GetRegionCenter(&hPVIRegion, hRow1, hCol1, hRow2, hCol2, &hRegionCenterRow, &hRegionCenterColumn);

					HTuple hPVITopRow, hPVITopCol;
					TopReferencePoints(hImages, hPVIRegion, nFOV, nTotalFOV, TOP_LEFT,
						&hPVITopRow, &hPVITopCol, &hPVIAngle, &hPVI2DCenter, &hPVISize);


					//// ---- Mask Region ---- //// 

					hMaskNumber = m_pPviInspectionSetup[nArea].nDeviceMaskNumber;
					hMask2DCenterPoints = hMaskAngleConcat = hMaskSize = HTuple();

					HRegion hConcatMaskRgn;
					GenEmptyObj(&hConcatMaskRgn);
					for (int nMask = 0; nMask < hMaskNumber; nMask++) {
						HTuple hMaskCenterRow, hMaskCenterColumn;
						HRegion hMaskRegion;

						hRow1 = m_pPviInspectionSetup[nArea].m_rectPviMaskRegions[nMask].top;
						hCol1 = m_pPviInspectionSetup[nArea].m_rectPviMaskRegions[nMask].left;
						hRow2 = m_pPviInspectionSetup[nArea].m_rectPviMaskRegions[nMask].bottom;
						hCol2 = m_pPviInspectionSetup[nArea].m_rectPviMaskRegions[nMask].right;

						GetRegionCenter(&hMaskRegion, hRow1, hCol1, hRow2, hCol2, &hMaskCenterRow, &hMaskCenterColumn);

						HTuple hMaskTopRow, hMaskTopCol, hMaskAngle, hMask2DCenter, hMaskSizeEach;
						TopReferencePoints(hImages, hMaskRegion, nFOV, nTotalFOV, TOP_LEFT,
							&hMaskTopRow, &hMaskTopCol, &hMaskAngle, &hMask2DCenter, &hMaskSizeEach);

						TupleConcat(hMask2DCenterPoints, hMask2DCenter, &hMask2DCenterPoints);
						TupleConcat(hMaskAngleConcat, hMaskAngle, &hMaskAngleConcat);
						TupleConcat(hMaskSize, hMaskSizeEach, &hMaskSize);

						HRegion hRectTemp;
						if (m_pPviInspectionSetup[nArea].nDeviceMaskArea[nMask] == POLYGON) {
							HTuple hPoint;
							HTuple hRows, hCols;
							for (int nPointId = 0; nPointId < m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[nMask]; nPointId++) {
								TupleConcat(hRows, m_pPviInspectionSetup[nArea].m_polygonPviMask[nMask][nPointId].y, &hRows);
								TupleConcat(hCols, m_pPviInspectionSetup[nArea].m_polygonPviMask[nMask][nPointId].x, &hCols);

								hPoint.Clear();
								hPoint.Append(m_pPviInspectionSetup[nArea].m_polygonPviMask[nMask][nPointId].y);
								hPoint.Append(m_pPviInspectionSetup[nArea].m_polygonPviMask[nMask][nPointId].x);

								TupleConcat(hPVIPolygonMaskPoint, hPoint, &hPVIPolygonMaskPoint);
							}
							GenRegionPolygonFilled(&hRectTemp, hRows, hCols);
						}
						else {
							GenRectangle2(&hRectTemp, hMask2DCenter[0], hMask2DCenter[1], hMaskAngle, hMaskSizeEach[0], hMaskSizeEach[1]);
						}
						ConcatObj(hConcatMaskRgn, hRectTemp, &hConcatMaskRgn);
					}

					HRegion hFitPVIMaskRegion, hDebugRegionOut;

					Union1(hConcatMaskRgn, &hFitPVIMaskRegion);
					if (m_pPviInspectionSetup[nArea].nDeviceAreaType == RECTANGLE)
						GenRectangle2(&hFitPVIRegion, hPVI2DCenter[0], hPVI2DCenter[1], hPVIAngle, hPVISize[0], hPVISize[1]);
					else if (m_pPviInspectionSetup[nArea].nDeviceAreaType == ECLIPSE)
						GenEllipse(&hFitPVIRegion, m_pPviInspectionSetup[nArea].m_rectPviArea.CenterPoint().y, m_pPviInspectionSetup[nArea].m_rectPviArea.CenterPoint().x, 0,
							m_pPviInspectionSetup[nArea].m_rectPviArea.Width() / 2, m_pPviInspectionSetup[nArea].m_rectPviArea.Height() / 2);
					else if (m_pPviInspectionSetup[nArea].nDeviceAreaType == POLYGON) {
						HTuple hPoint;
						HTuple hRows, hCols;
						for (int nPointId = 0; nPointId < m_pPviInspectionSetup[nArea].nPolygonPointNumber; nPointId++) {
							TupleConcat(hRows, m_pPviInspectionSetup[nArea].m_polygonPviArea[nPointId].y, &hRows);
							TupleConcat(hCols, m_pPviInspectionSetup[nArea].m_polygonPviArea[nPointId].x, &hCols);

							hPoint.Clear();
							hPoint.Append(m_pPviInspectionSetup[nArea].m_polygonPviArea[nPointId].y);
							hPoint.Append(m_pPviInspectionSetup[nArea].m_polygonPviArea[nPointId].x);

							TupleConcat(hPVIPolyLinePoints, hPoint, &hPVIPolyLinePoints);
						}
						GenRegionPolygonFilled(&hFitPVIRegion, hRows, hCols);
					}


					m_arrayOverlayTeach.Add(hFitPVIRegion, colorYellow);
					m_arrayOverlayTeach.Add(hFitPVIMaskRegion, colorRed);
				}

				//For PVI Edge, PolyLine type
				if (m_pPviInspectionSetup[nArea].nDeviceAreaType == POLYLINE && m_pPviInspectionSetup[nArea].bEdgeEnable) {
					HTuple hPoint;
					for (int nPointId = 0; nPointId < m_pPviInspectionSetup[nArea].nPolylinePointNumber; nPointId++) {
						hPoint.Clear();
						hPoint.Append(m_pPviInspectionSetup[nArea].m_polylinePviArea[nPointId].y);
						hPoint.Append(m_pPviInspectionSetup[nArea].m_polylinePviArea[nPointId].x);

						TupleConcat(hPVIPolyLinePoints, hPoint, &hPVIPolyLinePoints);
					}
				}

				int nIndex = 1;

				for (int i = 0; i < hPVIAngle.Length(); i++)
					m_TrainingData.darrayPVI[nIndex][nArea].Add(hPVIAngle[i].D());
				m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hPVIAngle);

				for (int i = 0; i < hPVI2DCenter.Length(); i++)
					m_TrainingData.darrayPVI[nIndex][nArea].Add(hPVI2DCenter[i].D());
				m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hPVI2DCenter);

				for (int i = 0; i < hPVISize.Length(); i++)
					m_TrainingData.darrayPVI[nIndex][nArea].Add(hPVISize[i].D());
				m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hPVISize);

				for (int i = 0; i < hMaskNumber.Length(); i++)
					m_TrainingData.darrayPVI[nIndex][nArea].Add(hMaskNumber[i].D());
				m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hMaskNumber);

				for (int i = 0; i < hMask2DCenterPoints.Length(); i++)
					m_TrainingData.darrayPVI[nIndex][nArea].Add(hMask2DCenterPoints[i].D());
				m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hMask2DCenterPoints);

				for (int i = 0; i < hMaskAngleConcat.Length(); i++)
					m_TrainingData.darrayPVI[nIndex][nArea].Add(hMaskAngleConcat[i].D());
				m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hMaskAngleConcat);

				for (int i = 0; i < hMaskSize.Length(); i++)
					m_TrainingData.darrayPVI[nIndex][nArea].Add(hMaskSize[i].D());
				m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hMaskSize);

				//PVI EDGE
				for (int i = 0; i < hPVIPolyLinePoints.Length(); i++)
					m_TrainingData.darrayPVI[nIndex][nArea].Add(hPVIPolyLinePoints[i].D());
				m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hPVIPolyLinePoints);

				//PVI Polygon Masking
				for (int i = 0; i < hPVIPolygonMaskPoint.Length(); i++)
					m_TrainingData.darrayPVI[nIndex][nArea].Add(hPVIPolygonMaskPoint[i].D());
				m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hPVIPolygonMaskPoint);
			}
		}
	}
	//Encap Tracing Teach
	else if (nTeachStep == 2) {

		//// ---- encap teach part ---- ////
		m_arrayOverlayTeach.RemoveAll();

		HRegion hDeviceLocationSelected;
		HTuple hIsSelected, hInspectRotationAngle, hInspectShiftAlongRow, hInspectShiftAlongCol;
		HTuple hCount, hArea, hDeviceCenterRow, hDeviceCenterCol;

		_FCI_Extract_LocationParameterData(pTrainingData->hTupleProjectionParam, pTrainingData->hTupleTeachDoc,
			pTrainingData->hTupleEdgeTeachTuple, pTrainingData->hTupleDatumTeachTuple, nDoc,
			&hIsSelected, &hInspectRotationAngle, &hInspectShiftAlongRow, &hInspectShiftAlongCol);

		_FCI_Select_DeviceLocation(pTrainingData->hObjectDeviceLocation,
			&hDeviceLocationSelected,
			nDoc, &hDeviceCenterRow, &hDeviceCenterCol);

		CountObj(hDeviceLocationSelected, &hCount);
		if (hCount < 1)
			return 0;
		// Blue Encap Tracing Region
		if (m_TrainingData.bEnableEncapBlue) {
			// // Teach Region for Tracing Encap
			HTuple hPoint;
			HTuple hRows, hCols;
			HRegion hEncapRgn;
			for (int nPointId = 0; nPointId < m_TrainingData.nEncapTracingPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorOrange);

			// // Region Encap Above 
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nEncapTopPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_EncapTopPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_EncapTopPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_EncapTopPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_EncapTopPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorGreen);

			// // Region for Tracing Encap Below
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nEncapBottomPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);

			HTuple hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2;
			// // Region Find Left Line Flex for Generating Masking Encap
			hEncapRow1 = m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hEncapRgn, colorYellow);

			// // Region Find Right Line Flex for Generating Masking Encap
			hEncapRow1 = m_TrainingData.m_rectEncapMaskingRightPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectEncapMaskingRightPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectEncapMaskingRightPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectEncapMaskingRightPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hEncapRgn, colorYellow);

			// // Region Encap Masking Limit 
			hEncapRow1 = m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hEncapRgn, colorYellow);
			// // // Insuficient
			// // Left Encap Region For Checking Insufficient
			hEncapRow1 = m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);
	
			// // Right Encap Region For Checking Insufficient
			hEncapRow1 = m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);

			// // Region For Find border of the base
			hEncapRow1 = m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);

			// // Middle Encap Region For Checking Insufficient
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nBlueEncapInsuffMidRegionPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);
			// // // // Excess
			// // Left Encap Region For Checking Excess
			hEncapRow1 = m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);
			// // Right Encap Region For Checking Excess
			hEncapRow1 = m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);
			// Glue - Mason
			// Get Glue Mask Polygon
			HObject hBEncapGlueMask;
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < ENCAP_GLUE_MASK_POINT; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonBEncapGlueMask[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonBEncapGlueMask[nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hBEncapGlueMask, hRows, hCols);
			m_arrayOverlayTeach.Add(hBEncapGlueMask, colorBlue);
			m_TrainingData.hBEncapGlueMask = hBEncapGlueMask;

			// Contamination - Mason
			// Get Encap Flex Edge Mask ROI Rectangle
			HObject hBEncapFlexEdgeMaskROI;
			GenRectangle1(&hBEncapFlexEdgeMaskROI,
				m_TrainingData.m_rectBEncapFlexEdgeMaskROI.top,
				m_TrainingData.m_rectBEncapFlexEdgeMaskROI.left,
				m_TrainingData.m_rectBEncapFlexEdgeMaskROI.bottom,
				m_TrainingData.m_rectBEncapFlexEdgeMaskROI.right);
			m_arrayOverlayTeach.Add(hBEncapFlexEdgeMaskROI, colorBlue);
			m_TrainingData.hBEncapFlexEdgeMaskROI = hBEncapFlexEdgeMaskROI;

			// // Region for Inspect Wire
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nEncapWirePolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_EncapWirePolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_EncapWirePolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_EncapWirePolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_EncapWirePolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);

			// // Region for Inspect Wire Bottom
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nEncapWireBottomPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);

			// // Region for Inspect IC
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nEncapICPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_EncapICPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_EncapICPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_EncapICPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_EncapICPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);

			//Region for Inspect Bulge -Magnus
			HObject hBlueEncapBulgeROI;
			GenRectangle1(&hBlueEncapBulgeROI,
				m_TrainingData.m_rectBlueBulgeROI.top,
				m_TrainingData.m_rectBlueBulgeROI.left,
				m_TrainingData.m_rectBlueBulgeROI.bottom,
				m_TrainingData.m_rectBlueBulgeROI.right);
			m_arrayOverlayTeach.Add(hBlueEncapBulgeROI, colorYellow); // End Bulge region -magnus

		}
		// Dark Encap Tracing
		else if (m_TrainingData.bEnableDarkEncap) {
			m_arrayOverlayTeach.RemoveAll();
			HTuple hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2;
			// // Region Tracing Encap  
			HRegion hRegionTracingDarkEncap;
			hEncapRow1 = m_TrainingData.m_rectAllRegionDarkEncap.top;
			hEncapCol1 = m_TrainingData.m_rectAllRegionDarkEncap.left;
			hEncapRow2 = m_TrainingData.m_rectAllRegionDarkEncap.bottom;
			hEncapCol2 = m_TrainingData.m_rectAllRegionDarkEncap.right;
			GenRectangle1(&hRegionTracingDarkEncap, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hRegionTracingDarkEncap, colorYellow);

			// Region for Tracing Encap on flex
			HRegion hTopFlexDarkEncapRgn;
			// // Region Find Left Line Flex for Generating Masking Encap
			hEncapRow1 = m_TrainingData.m_rectTopFlexDarkEncap.top;
			hEncapCol1 = m_TrainingData.m_rectTopFlexDarkEncap.left;
			hEncapRow2 = m_TrainingData.m_rectTopFlexDarkEncap.bottom;
			hEncapCol2 = m_TrainingData.m_rectTopFlexDarkEncap.right;
			GenRectangle1(&hTopFlexDarkEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hTopFlexDarkEncapRgn, colorYellow);

			// // Region for remove Bottom White region
			HRegion hBotWhiteDarkEncap;
			hEncapRow1 = m_TrainingData.m_rectBotWhiteDarkEncap.top;
			hEncapCol1 = m_TrainingData.m_rectBotWhiteDarkEncap.left;
			hEncapRow2 = m_TrainingData.m_rectBotWhiteDarkEncap.bottom;
			hEncapCol2 = m_TrainingData.m_rectBotWhiteDarkEncap.right;
			GenRectangle1(&hBotWhiteDarkEncap, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hBotWhiteDarkEncap, colorYellow);

			// // Outside Encap Region
			HTuple hPoint;
			HTuple hRows, hCols;
			HRegion hOutsideDarkEncapRgn;
			for (int nPointId = 0; nPointId < m_TrainingData.nDarkEncapOutsidePolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hOutsideDarkEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hOutsideDarkEncapRgn, colorYellow);

			// // Inside Encap Region 
			hRows = HTuple();
			hCols = HTuple();
			HRegion hInsideDarkEncapRgn;
			for (int nPointId = 0; nPointId < m_TrainingData.nDarkEncapInsidePolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hInsideDarkEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hInsideDarkEncapRgn, colorYellow);


			// Excess - magnus
			// Get Left Excess Mask Rectangle
			HObject hDarkExcessLeftMask;
			GenRectangle1(&hDarkExcessLeftMask,
				m_TrainingData.m_rectDarkExcessLeftMask.top,
				m_TrainingData.m_rectDarkExcessLeftMask.left,
				m_TrainingData.m_rectDarkExcessLeftMask.bottom,
				m_TrainingData.m_rectDarkExcessLeftMask.right);
			m_arrayOverlayTeach.Add(hDarkExcessLeftMask, colorBlue);
			
			// Get Right Excess Mask Rectangle
			HObject hDarkExcessRightMask;
			GenRectangle1(&hDarkExcessRightMask,
				m_TrainingData.m_rectDarkExcessRightMask.top,
				m_TrainingData.m_rectDarkExcessRightMask.left,
				m_TrainingData.m_rectDarkExcessRightMask.bottom,
				m_TrainingData.m_rectDarkExcessRightMask.right);
			m_arrayOverlayTeach.Add(hDarkExcessRightMask, colorBlue);

			// Insufficient - magnus
			// Get Cover Insufficient Mask Rectangle
			HObject hDarkInsufficientCoverMask;
			GenRectangle1(&hDarkInsufficientCoverMask,
				m_TrainingData.m_rectDarkInsufficientCoverMask.top,
				m_TrainingData.m_rectDarkInsufficientCoverMask.left,
				m_TrainingData.m_rectDarkInsufficientCoverMask.bottom,
				m_TrainingData.m_rectDarkInsufficientCoverMask.right);
			m_arrayOverlayTeach.Add(hDarkInsufficientCoverMask, colorBlue);

			// Get Inner Insufficient Mask Polygon
			HObject hDarkInsufficientMidMask;
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < DARK_ENCAP_INNER_INSUFFICIENT_MASK_POINT; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonDarkInsufficientMidMask[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonDarkInsufficientMidMask[nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hDarkInsufficientMidMask, hRows, hCols);
			m_arrayOverlayTeach.Add(hDarkInsufficientMidMask, colorBlue);

			// Get Left Insufficient Mask Rectangle
			HObject hDarkInsufficientLeftMask;
			GenRectangle1(&hDarkInsufficientLeftMask,
				m_TrainingData.m_rectDarkInsufficientLeftMask.top,
				m_TrainingData.m_rectDarkInsufficientLeftMask.left,
				m_TrainingData.m_rectDarkInsufficientLeftMask.bottom,
				m_TrainingData.m_rectDarkInsufficientLeftMask.right);
			m_arrayOverlayTeach.Add(hDarkInsufficientLeftMask, colorBlue);

			// Get Right Insufficient Mask Rectangle
			HObject hDarkInsufficientRightMask;
			GenRectangle1(&hDarkInsufficientRightMask,
				m_TrainingData.m_rectDarkInsufficientRightMask.top,
				m_TrainingData.m_rectDarkInsufficientRightMask.left,
				m_TrainingData.m_rectDarkInsufficientRightMask.bottom,
				m_TrainingData.m_rectDarkInsufficientRightMask.right);
			m_arrayOverlayTeach.Add(hDarkInsufficientRightMask, colorBlue);// End magnus

			// Glue - Mason
			// Get Glue Mask Rectangle
			HObject hDarkGlueMask;
			GenRectangle1(&hDarkGlueMask,
				m_TrainingData.m_rectDarkGlueMask.top,
				m_TrainingData.m_rectDarkGlueMask.left,
				m_TrainingData.m_rectDarkGlueMask.bottom,
				m_TrainingData.m_rectDarkGlueMask.right);
			m_arrayOverlayTeach.Add(hDarkGlueMask, colorBlue);
			m_TrainingData.hDarkGlueMask = hDarkGlueMask;

			// Contamination - Mason
			// Get Flex Edge Mask Rectangle
			HObject hDarkFlexEdgeMask;
			GenRectangle1(&hDarkFlexEdgeMask,
				m_TrainingData.m_rectDarkFlexEdgeMask.top,
				m_TrainingData.m_rectDarkFlexEdgeMask.left,
				m_TrainingData.m_rectDarkFlexEdgeMask.bottom,
				m_TrainingData.m_rectDarkFlexEdgeMask.right);
			m_arrayOverlayTeach.Add(hDarkFlexEdgeMask, colorBlue);
			m_TrainingData.hDarkFlexEdgeMask = hDarkFlexEdgeMask;

			// Get Bottom Edge Mask Rectangle
			HObject hDarkBottomEdgeMask;
			GenRectangle1(&hDarkBottomEdgeMask,
				m_TrainingData.m_rectDarkBottomEdgeMask.top,
				m_TrainingData.m_rectDarkBottomEdgeMask.left,
				m_TrainingData.m_rectDarkBottomEdgeMask.bottom,
				m_TrainingData.m_rectDarkBottomEdgeMask.right);
			m_arrayOverlayTeach.Add(hDarkBottomEdgeMask, colorBlue);
			m_TrainingData.hDarkBottomEdgeMask = hDarkBottomEdgeMask;

			// Get Right Edge Mask Rectangle
			HObject hDarkRightEdgeMask;
			GenRectangle1(&hDarkRightEdgeMask,
				m_TrainingData.m_rectDarkRightEdgeMask.top,
				m_TrainingData.m_rectDarkRightEdgeMask.left,
				m_TrainingData.m_rectDarkRightEdgeMask.bottom,
				m_TrainingData.m_rectDarkRightEdgeMask.right);
			m_arrayOverlayTeach.Add(hDarkRightEdgeMask, colorBlue);
			m_TrainingData.hDarkRightEdgeMask = hDarkRightEdgeMask;

			// Get Inner Mask Rectangle
			HObject hDarkInnerMask;
			GenRectangle1(&hDarkInnerMask,
				m_TrainingData.m_rectDarkInnerMask.top,
				m_TrainingData.m_rectDarkInnerMask.left,
				m_TrainingData.m_rectDarkInnerMask.bottom,
				m_TrainingData.m_rectDarkInnerMask.right);
			m_arrayOverlayTeach.Add(hDarkInnerMask, colorBlue);
			m_TrainingData.hDarkInnerMask = hDarkInnerMask;
			// // // // Void 
			HObject hDarkEncapVoidMaskingLeftDie;
			GenRectangle1(&hDarkEncapVoidMaskingLeftDie,
				m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea.top,
				m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea.left,
				m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea.bottom,
				m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea.right);
			m_arrayOverlayTeach.Add(hDarkEncapVoidMaskingLeftDie, colorYellow);

			HObject hDarkEncapVoidMaskingRightDie;
			GenRectangle1(&hDarkEncapVoidMaskingRightDie,
				m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea.top,
				m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea.left,
				m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea.bottom,
				m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea.right);
			m_arrayOverlayTeach.Add(hDarkEncapVoidMaskingRightDie, colorYellow);

			// // // // Wire And Ic
			HObject hEncapRgn;
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nDarkEncapWirePolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);

			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nDarkEncapICPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);		}
		// Dark Blue Encap Tracing Region
		else if (m_TrainingData.bEnableDarkBlueEncap) {
			m_arrayOverlayTeach.RemoveAll();

			HObject hDBEncapExtractROI;
			HObject hDBEncapInnerROI;
			HObject hDBEncapTopEdgeMaskROI;
			HObject hDBEncapBottomEdgeMaskROI;
			HObject hDBEncapLeftEdgeMaskROI;
			HObject hDBEncapRightEdgeMaskROI;
			HObject hDBEncapFlexEdgeMaskROI;

			// Get Encap Extract ROI Polygon
			HTuple hRows, hCols;
			for (int nPointId = 0; nPointId < DARK_BLUE_ENCAP_ROI_POINT; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonDBEncapExtractROI[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonDBEncapExtractROI[nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hDBEncapExtractROI, hRows, hCols);

			// Get Encap Inner ROI Rectangle
			GenRectangle1(&hDBEncapInnerROI,
				m_TrainingData.m_rectDBEncapInnerROI.top,
				m_TrainingData.m_rectDBEncapInnerROI.left,
				m_TrainingData.m_rectDBEncapInnerROI.bottom,
				m_TrainingData.m_rectDBEncapInnerROI.right);

			// Get Encap Top Edge Mask ROI Rectangle
			GenRectangle1(&hDBEncapTopEdgeMaskROI,
				m_TrainingData.m_rectDBEncapTopEdgeMaskROI.top,
				m_TrainingData.m_rectDBEncapTopEdgeMaskROI.left,
				m_TrainingData.m_rectDBEncapTopEdgeMaskROI.bottom,
				m_TrainingData.m_rectDBEncapTopEdgeMaskROI.right);

			// Get Encap Bottom Edge Mask ROI Rectangle
			GenRectangle1(&hDBEncapBottomEdgeMaskROI,
				m_TrainingData.m_rectDBEncapBottomEdgeMaskROI.top,
				m_TrainingData.m_rectDBEncapBottomEdgeMaskROI.left,
				m_TrainingData.m_rectDBEncapBottomEdgeMaskROI.bottom,
				m_TrainingData.m_rectDBEncapBottomEdgeMaskROI.right);

			// Get Encap Left Edge Mask ROI Rectangle
			GenRectangle1(&hDBEncapLeftEdgeMaskROI,
				m_TrainingData.m_rectDBEncapLeftEdgeMaskROI.top,
				m_TrainingData.m_rectDBEncapLeftEdgeMaskROI.left,
				m_TrainingData.m_rectDBEncapLeftEdgeMaskROI.bottom,
				m_TrainingData.m_rectDBEncapLeftEdgeMaskROI.right);

			// Get Encap Right Edge Mask ROI Rectangle
			GenRectangle1(&hDBEncapRightEdgeMaskROI,
				m_TrainingData.m_rectDBEncapRightEdgeMaskROI.top,
				m_TrainingData.m_rectDBEncapRightEdgeMaskROI.left,
				m_TrainingData.m_rectDBEncapRightEdgeMaskROI.bottom,
				m_TrainingData.m_rectDBEncapRightEdgeMaskROI.right);

			// Get Encap Flex Edge Mask ROI Rectangle
			GenRectangle1(&hDBEncapFlexEdgeMaskROI,
				m_TrainingData.m_rectDBEncapFlexEdgeMaskROI.top,
				m_TrainingData.m_rectDBEncapFlexEdgeMaskROI.left,
				m_TrainingData.m_rectDBEncapFlexEdgeMaskROI.bottom,
				m_TrainingData.m_rectDBEncapFlexEdgeMaskROI.right);

			// Display
			m_arrayOverlayTeach.Add(hDBEncapExtractROI, colorGreen);
			m_arrayOverlayTeach.Add(hDBEncapInnerROI, colorGreen);
			m_arrayOverlayTeach.Add(hDBEncapTopEdgeMaskROI, colorYellow);
			m_arrayOverlayTeach.Add(hDBEncapBottomEdgeMaskROI, colorYellow);
			m_arrayOverlayTeach.Add(hDBEncapLeftEdgeMaskROI, colorYellow);
			m_arrayOverlayTeach.Add(hDBEncapRightEdgeMaskROI, colorYellow);
			m_arrayOverlayTeach.Add(hDBEncapFlexEdgeMaskROI, colorYellow);

			// Save Data
			m_TrainingData.hDBEncapExtractROI = hDBEncapExtractROI;
			m_TrainingData.hDBEncapInnerROI = hDBEncapInnerROI;
			m_TrainingData.hDBEncapTopEdgeMaskROI = hDBEncapTopEdgeMaskROI;
			m_TrainingData.hDBEncapBottomEdgeMaskROI = hDBEncapBottomEdgeMaskROI;
			m_TrainingData.hDBEncapLeftEdgeMaskROI = hDBEncapLeftEdgeMaskROI;
			m_TrainingData.hDBEncapRightEdgeMaskROI = hDBEncapRightEdgeMaskROI;
			m_TrainingData.hDBEncapFlexEdgeMaskROI = hDBEncapFlexEdgeMaskROI;

			// Excess - Mason
			// Get Left Excess Mask Rectangle
			HObject hDBLeftExcessMask;
			GenRectangle1(&hDBLeftExcessMask,
				m_TrainingData.m_rectDBLeftExcessMask.top,
				m_TrainingData.m_rectDBLeftExcessMask.left,
				m_TrainingData.m_rectDBLeftExcessMask.bottom,
				m_TrainingData.m_rectDBLeftExcessMask.right);
			m_arrayOverlayTeach.Add(hDBLeftExcessMask, colorBlue);
			m_TrainingData.hDBLeftExcessMask = hDBLeftExcessMask;

			// Get Right Excess Mask Rectangle
			HObject hDBRightExcessMask;
			GenRectangle1(&hDBRightExcessMask,
				m_TrainingData.m_rectDBRightExcessMask.top,
				m_TrainingData.m_rectDBRightExcessMask.left,
				m_TrainingData.m_rectDBRightExcessMask.bottom,
				m_TrainingData.m_rectDBRightExcessMask.right);
			m_arrayOverlayTeach.Add(hDBRightExcessMask, colorBlue);
			m_TrainingData.hDBRightExcessMask = hDBRightExcessMask;

			// Insufficient - Mason
			// Get Cover Insufficient Mask Rectangle
			HObject hDBCoverInsufficientMask;
			GenRectangle1(&hDBCoverInsufficientMask,
				m_TrainingData.m_rectDBCoverInsufficientMask.top,
				m_TrainingData.m_rectDBCoverInsufficientMask.left,
				m_TrainingData.m_rectDBCoverInsufficientMask.bottom,
				m_TrainingData.m_rectDBCoverInsufficientMask.right);
			m_arrayOverlayTeach.Add(hDBCoverInsufficientMask, colorBlue);
			m_TrainingData.hDBCoverInsufficientMask = hDBCoverInsufficientMask;

			// Get Inner Insufficient Mask Polygon
			HObject hDBInnerInsufficientMask;
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < DARK_BLUE_ENCAP_INNER_INSUFFICIENT_MASK_POINT; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonDBInnerInsufficientMask[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonDBInnerInsufficientMask[nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hDBInnerInsufficientMask, hRows, hCols);
			m_arrayOverlayTeach.Add(hDBInnerInsufficientMask, colorBlue);
			m_TrainingData.hDBInnerInsufficientMask = hDBInnerInsufficientMask;

			// Get Left Insufficient Mask Rectangle
			HObject hDBLeftInsufficientMask;
			GenRectangle1(&hDBLeftInsufficientMask,
				m_TrainingData.m_rectDBLeftInsufficientMask.top,
				m_TrainingData.m_rectDBLeftInsufficientMask.left,
				m_TrainingData.m_rectDBLeftInsufficientMask.bottom,
				m_TrainingData.m_rectDBLeftInsufficientMask.right);
			m_arrayOverlayTeach.Add(hDBLeftInsufficientMask, colorBlue);
			m_TrainingData.hDBLeftInsufficientMask = hDBLeftInsufficientMask;

			// Get Right Insufficient Mask Rectangle
			HObject hDBRightInsufficientMask;
			GenRectangle1(&hDBRightInsufficientMask,
				m_TrainingData.m_rectDBRightInsufficientMask.top,
				m_TrainingData.m_rectDBRightInsufficientMask.left,
				m_TrainingData.m_rectDBRightInsufficientMask.bottom,
				m_TrainingData.m_rectDBRightInsufficientMask.right);
			m_arrayOverlayTeach.Add(hDBRightInsufficientMask, colorBlue);
			m_TrainingData.hDBRightInsufficientMask = hDBRightInsufficientMask;

			// Glue - Mason
			// Get Glue Mask Polygon
			HObject hDBEncapGlueMask;
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < ENCAP_GLUE_MASK_POINT; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonDBEncapGlueMask[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonDBEncapGlueMask[nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hDBEncapGlueMask, hRows, hCols);
			m_arrayOverlayTeach.Add(hDBEncapGlueMask, colorBlue);
			m_TrainingData.hDBEncapGlueMask = hDBEncapGlueMask;

			// Inspect Blow Hole
			HRegion hEncapRgn;
			HTuple hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2;
			// // Region for tracing Left Mask to inspect BlowHole 
			hEncapRow1 = m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);
			m_TrainingData.hDarkBlueEncapBHMaskingLeft = hEncapRgn;

			// // Region for tracing Right Mask to inspect BlowHole 
			hEncapRow1 = m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_arrayOverlayTeach.Add(hEncapRgn, colorBlue);
			m_TrainingData.hDarkBlueEncapBHMaskingLeft = hEncapRgn;
			// Inspect Wire And IC
			// // Region for Inspect Wire And IC on Dark Blue Encap (Bottom)
			HTuple hPoint;
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nDarkBlueEncapWireBottomPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorYellow);
			m_TrainingData.hDarkBlueEncapWireBottom = hEncapRgn;
			// // Region for Inspect IC on Dark Blue Encap
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nDarkBlueEncapICPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_arrayOverlayTeach.Add(hEncapRgn, colorYellow);
			m_TrainingData.hDarkBlueEncapIC = hEncapRgn;


		}
		// // Encap Surface Mark 
		HRegion hConcatEncapMaskRgn;
		HTuple hEncapPolygonPoints;
		GenEmptyObj(&hConcatEncapMaskRgn);
		for (int nMask = 0; nMask < m_TrainingData.nEncapTracingMaskNumber; nMask++) {
			HRegion hEncapTracingMask;
			hEncapPolygonPoints = HTuple();
			HTuple hPoint;
			HTuple hRows, hCols;
			for (int nPointId = 0; nPointId < m_TrainingData.nEncapTracingMaskPolygonPointNumber[nMask]; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMask][nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMask][nPointId].x, &hCols);

				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMask][nPointId].y);
				hPoint.Append(m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMask][nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapTracingMask, hRows, hCols);
			ConcatObj(hConcatEncapMaskRgn, hEncapTracingMask, &hConcatEncapMaskRgn);
		}
		m_arrayOverlayTeach.Add(hConcatEncapMaskRgn, colorYellow);

		//Void Region Inspection - magnus
		HObject hDBEncapVoidROI;
		GenRectangle1(&hDBEncapVoidROI,
			m_TrainingData.m_rectDBlueVoidROI.top,
			m_TrainingData.m_rectDBlueVoidROI.left,
			m_TrainingData.m_rectDBlueVoidROI.bottom,
			m_TrainingData.m_rectDBlueVoidROI.right);
		m_arrayOverlayTeach.Add(hDBEncapVoidROI, colorYellow); // End Void region -magnus
		
	}
	//Encap Surface Teach
	else if (nTeachStep == 3) {

		if (bEnable[2]) {

			HTuple hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2;
			HRegion hEncapRgn, hConCatEncapRgn;
			HTuple hWireColor;
			hWireColor = nCurTrack;

			HImage hDebugImg;
			HRegion hEncapLocation, hDebugRgn;
			HTuple hIsEncap, hDebugMsg;
		}

		int nIndexEncap = 2;
		//for (int i = 0; i < hEncapInspCenterRow.Length(); i++)
		//	m_TrainingData.darrayEncap[nIndexEncap].Add(hEncapInspCenterRow[i].D());
		//m_TrainingData.hvec_TupleVectorEncap.InsertAt(nIndexEncap++, hEncapInspCenterRow);

	}
	//Holder For Another Inspection Type
	else if (nTeachStep == 4) {

		int nChickletRoi = m_TrainingData.m_nChickletNumberOfHoles;
		HTuple nHoleOffSet = m_TrainingData.m_nChickletOffsettRadius;
		HTuple nBurrValueDefect = m_ChickletSwageHole.nBurrValueDefect;
		HTuple nMinContrast = m_TrainingData.m_nChickletMinContrast;
		HTuple nMaxContrast = m_TrainingData.m_nChickletMaxContrast;
		HTuple nRoundnessValueDefect = m_ChickletSwageHole.nRoundnessValueDefect;
		HTuple 	nBurrValueForRoundness = m_ChickletSwageHole.nBurrValueForRoundness;

		HImage hDebugImg;
		HTuple hIsBurr, hDebugMsg;
		HRegion hRectChickletROI, BurrRegion, HoleLocation, hDebugRgn;
		HTuple hChickletROIRow1;
		HTuple hChickletROICol1;
		HTuple hChickletROIRow2;
		HTuple hChickletROICol2;
		HTuple hRadiusInit;
		HTuple hIsRoundness;
		HTuple hRadiusOut;
		HTuple hDebugMessageOut;
		for (int i = 0; i < nChickletRoi; i++)
		{
			hChickletROIRow1 = m_TrainingData.m_rectChickletROI[i].top;
			hChickletROICol1 = m_TrainingData.m_rectChickletROI[i].left;
			hChickletROIRow2 = m_TrainingData.m_rectChickletROI[i].bottom;
			hChickletROICol2 = m_TrainingData.m_rectChickletROI[i].right;
			//hRadiusInit = m_TrainingData.m_rectChickletROI[i].Height()/4;
			hRadiusInit = 125.0;
			GenRectangle1(&hRectChickletROI, hChickletROIRow1, hChickletROICol1, hChickletROIRow2, hChickletROICol2);

			_HP_Inspect_ChickletHole_BurrRoundness(hRectChickletROI, hImages, &BurrRegion,
				&HoleLocation, &hDebugImg, &hDebugRgn, nStepDebug, nBurrValueDefect,
				nMinContrast, nMaxContrast, hRadiusInit, nBurrValueForRoundness,
				nRoundnessValueDefect, &hIsBurr, &hIsRoundness, &hRadiusOut, &hDebugMessageOut);

			m_arrayOverlayTeach.Add(hRectChickletROI, colorGreen);
			m_arrayOverlayTeach.Add(HoleLocation, colorGreen);
		}

	}
	//Flex Region Teach
	else if (nTeachStep == 5) {
		m_arrayOverlayTeach.RemoveAll();

		HObject hFlexSubstrateSearchPatternROI;
		HObject hFlexTopEdgeLine;
		HObject hFlexBottomEdgeLine;
		HObject hFlexLeftEdgeLine;
		HObject hFlexRightEdgeLine;

		HObject hFlexLeftEdgeLineMask;
		HObject hFlexRightEdgeLineMask;

		HObject hFlexBottomMask;
		HObject hFlexStraightCircuitLineMask;

		HObject hFlexNumberMask;
		HObject hFlexTracingMask;
		HObject hFlexDamageMask;
		HObject hFlexSubstrateMask;

		// Get Substrate Pattern ROI
		HObject hFlexSubstratePatternROI1;
		GenRectangle1(&hFlexSubstratePatternROI1,
			m_TrainingData.m_rectFlexSubstratePatternROI1.top,
			m_TrainingData.m_rectFlexSubstratePatternROI1.left,
			m_TrainingData.m_rectFlexSubstratePatternROI1.bottom,
			m_TrainingData.m_rectFlexSubstratePatternROI1.right);
		HObject hFlexSubstratePatternROI2;
		GenRectangle1(&hFlexSubstratePatternROI2,
			m_TrainingData.m_rectFlexSubstratePatternROI2.top,
			m_TrainingData.m_rectFlexSubstratePatternROI2.left,
			m_TrainingData.m_rectFlexSubstratePatternROI2.bottom,
			m_TrainingData.m_rectFlexSubstratePatternROI2.right);
		HObject hFlexSubstratePatternROI;
		Union2(hFlexSubstratePatternROI1, hFlexSubstratePatternROI2, &hFlexSubstratePatternROI);
			
		// Get Substrate Search Pattern ROI
		GenRectangle1(&hFlexSubstrateSearchPatternROI,
			m_TrainingData.m_rectFlexSubstrateSearchPatternROI.top,
			m_TrainingData.m_rectFlexSubstrateSearchPatternROI.left,
			m_TrainingData.m_rectFlexSubstrateSearchPatternROI.bottom,
			m_TrainingData.m_rectFlexSubstrateSearchPatternROI.right);

		// Get Flex Top Edge Line Polygon
		HTuple hRows, hCols;
		for (int nPointId = 0; nPointId < 2; nPointId++) {
			TupleConcat(hRows, m_TrainingData.m_polygonFlexTopEdgeLine[nPointId].y, &hRows);
			TupleConcat(hCols, m_TrainingData.m_polygonFlexTopEdgeLine[nPointId].x, &hCols);
		}
		GenContourPolygonXld(&hFlexTopEdgeLine, hRows, hCols);

		// Get Flex Bottom Edge Line Polygon
		hRows = HTuple();
		hCols = HTuple();
		for (int nPointId = 0; nPointId < 2; nPointId++) {
			TupleConcat(hRows, m_TrainingData.m_polygonFlexBottomEdgeLine[nPointId].y, &hRows);
			TupleConcat(hCols, m_TrainingData.m_polygonFlexBottomEdgeLine[nPointId].x, &hCols);
		}
		GenContourPolygonXld(&hFlexBottomEdgeLine, hRows, hCols);

		// Get Flex Left Edge Line Polygon
		hRows = HTuple();
		hCols = HTuple();
		for (int nPointId = 0; nPointId < 2; nPointId++) {
			TupleConcat(hRows, m_TrainingData.m_polygonFlexLeftEdgeLine[nPointId].y, &hRows);
			TupleConcat(hCols, m_TrainingData.m_polygonFlexLeftEdgeLine[nPointId].x, &hCols);
		}
		GenContourPolygonXld(&hFlexLeftEdgeLine, hRows, hCols);

		// Get Flex Right Edge Line Polygon
		hRows = HTuple();
		hCols = HTuple();
		for (int nPointId = 0; nPointId < 2; nPointId++) {
			TupleConcat(hRows, m_TrainingData.m_polygonFlexRightEdgeLine[nPointId].y, &hRows);
			TupleConcat(hCols, m_TrainingData.m_polygonFlexRightEdgeLine[nPointId].x, &hCols);
		}
		GenContourPolygonXld(&hFlexRightEdgeLine, hRows, hCols);

		// Get Flex Left Edge Line Mask
		GenRectangle1(&hFlexLeftEdgeLineMask,
			m_TrainingData.m_rectFlexLeftEdgeLineMask.top,
			m_TrainingData.m_rectFlexLeftEdgeLineMask.left,
			m_TrainingData.m_rectFlexLeftEdgeLineMask.bottom,
			m_TrainingData.m_rectFlexLeftEdgeLineMask.right);

		// Get Flex Right Edge Line Mask
		GenRectangle1(&hFlexRightEdgeLineMask,
			m_TrainingData.m_rectFlexRightEdgeLineMask.top,
			m_TrainingData.m_rectFlexRightEdgeLineMask.left,
			m_TrainingData.m_rectFlexRightEdgeLineMask.bottom,
			m_TrainingData.m_rectFlexRightEdgeLineMask.right);

		// Get Flex Bottom ROI Rectangle
		GenRectangle1(&hFlexBottomMask,
			m_TrainingData.m_rectFlexBottomROI.top,
			m_TrainingData.m_rectFlexBottomROI.left,
			m_TrainingData.m_rectFlexBottomROI.bottom,
			m_TrainingData.m_rectFlexBottomROI.right);

		// Get Flex Straight Line ROI Rectangle
		hRows = HTuple();
		hCols = HTuple();
		for (int nPointId = 0; nPointId < m_TrainingData.nFlexStraightCircuitLineMaskPolygonPointNumber; nPointId++) {
			TupleConcat(hRows, m_TrainingData.m_polygonFlexStraightCircuitLineMask[nPointId].y, &hRows);
			TupleConcat(hCols, m_TrainingData.m_polygonFlexStraightCircuitLineMask[nPointId].x, &hCols);
		}
		GenRegionPolygonFilled(&hFlexStraightCircuitLineMask, hRows, hCols);

		// Get Flex Number Mask
		GenRectangle1(&hFlexNumberMask,
			m_TrainingData.m_rectFlexNumberMask.top,
			m_TrainingData.m_rectFlexNumberMask.left,
			m_TrainingData.m_rectFlexNumberMask.bottom,
			m_TrainingData.m_rectFlexNumberMask.right);

		// Get Flex Tracing Mask
		GenEmptyObj(&hFlexTracingMask);
		for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nFlexTracingMaskNumber; nMaskIdx++) {
			HObject hTmpMaskRegion;
			HTuple hRows, hCols;
			for (int nPointId = 0; nPointId < m_TrainingData.nFlexTracingMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonFlexTracingMask[nMaskIdx][nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonFlexTracingMask[nMaskIdx][nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hTmpMaskRegion, hRows, hCols);
			
			ConcatObj(hFlexTracingMask, hTmpMaskRegion, &hFlexTracingMask);
		}
		Union1(hFlexTracingMask, &hFlexTracingMask);

		// Get Flex Damage Mask
		GenEmptyObj(&hFlexDamageMask);
		for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nFlexDamageMaskNumber; nMaskIdx++) {
			HObject hTmpMaskRegion;
			HTuple hRows, hCols;
			for (int nPointId = 0; nPointId < m_TrainingData.nFlexDamageMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonFlexDamageMask[nMaskIdx][nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonFlexDamageMask[nMaskIdx][nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hTmpMaskRegion, hRows, hCols);
			
			ConcatObj(hFlexDamageMask, hTmpMaskRegion, &hFlexDamageMask);
		}
		Union1(hFlexDamageMask, &hFlexDamageMask);

		// Get Flex Substrate Mask 
		GenEmptyObj(&hFlexSubstrateMask);
		if (m_TrainingData.bFlexEnableSubstrateMask) {
			for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nFlexSubstrateMaskNumber; nMaskIdx++) {
				HObject hTmpMaskRegion;
				HTuple hRows, hCols;
				for (int nPointId = 0; nPointId < m_TrainingData.nFlexSubstrateMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
					TupleConcat(hRows, m_TrainingData.m_polygonFlexSubstrateMask[nMaskIdx][nPointId].y, &hRows);
					TupleConcat(hCols, m_TrainingData.m_polygonFlexSubstrateMask[nMaskIdx][nPointId].x, &hCols);
				}
				GenRegionPolygonFilled(&hTmpMaskRegion, hRows, hCols);
				ConcatObj(hFlexSubstrateMask, hTmpMaskRegion, &hFlexSubstrateMask);
			}
			Union1(hFlexSubstrateMask, &hFlexSubstrateMask);
		}

		// Teach Procesing
		HTuple hFlexSubstrateModelID;
		HTuple hFlexSubstrateDatumMatchRowRef;
		HTuple hFlexSubstrateDatumMatchColRef;
		HTuple hFlexSubstrateMatchAngleRef;
		HTuple hFlexLeftWidthReference;
		HTuple hFlexRightWidthReference;
		HObject hFlexTracingMaskRegion;

		// Parameters
		double dResolution = (pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2;
		HTuple hEnableSubstrateMask = m_TrainingData.bFlexEnableSubstrateMask;
		HTuple hEnableDynamicEdgeFlex = m_TrainingData.bFlexEnableDynamicEdge;
		HTuple hMinCircuitLineContrast = m_TrainingData.nFlexCircuitLineContrast;
		HTuple hEdgeFlexContrast = m_TrainingData.nFlexEdgeContrast;
		HTuple hOffsetMaskSize = m_TrainingData.dFlexOffsetMaskSize / dResolution;

		_HP_Flex_Teach(hImages,
			hFlexSubstratePatternROI,
			hFlexTracingMask,
			hFlexLeftEdgeLineMask,
			hFlexRightEdgeLineMask,
			&hFlexTracingMaskRegion,
			hEnableSubstrateMask,
			hEnableDynamicEdgeFlex,
			hMinCircuitLineContrast,
			hEdgeFlexContrast,
			hOffsetMaskSize,
			&hFlexSubstrateModelID,
			&hFlexSubstrateDatumMatchRowRef,
			&hFlexSubstrateDatumMatchColRef,
			&hFlexSubstrateMatchAngleRef,
			&hFlexLeftWidthReference,
			&hFlexRightWidthReference);


		// Display
		HObject hFlexTopEdgeLineRegion;
		HObject hFlexBottomEdgeLineRegion;
		HObject hFlexLeftEdgeLineRegion;
		HObject hFlexRightEdgeLineRegion;
		GenRegionContourXld(hFlexTopEdgeLine, &hFlexTopEdgeLineRegion, "filled");
		GenRegionContourXld(hFlexBottomEdgeLine, &hFlexBottomEdgeLineRegion, "filled");
		GenRegionContourXld(hFlexLeftEdgeLine, &hFlexLeftEdgeLineRegion, "filled");
		GenRegionContourXld(hFlexRightEdgeLine, &hFlexRightEdgeLineRegion, "filled");

		m_arrayOverlayTeach.Add(hFlexTopEdgeLineRegion, colorGreen);
		m_arrayOverlayTeach.Add(hFlexBottomEdgeLineRegion, colorGreen);
		m_arrayOverlayTeach.Add(hFlexLeftEdgeLineRegion, colorGreen);
		m_arrayOverlayTeach.Add(hFlexRightEdgeLineRegion, colorGreen);

		m_arrayOverlayTeach.Add(hFlexBottomMask, colorBlue);
		m_arrayOverlayTeach.Add(hFlexStraightCircuitLineMask, colorBlue);

		m_arrayOverlayTeach.Add(hFlexNumberMask, colorYellow);
		m_arrayOverlayTeach.Add(hFlexTracingMask, colorYellow);
		m_arrayOverlayTeach.Add(hFlexDamageMask, colorYellow);
		m_arrayOverlayTeach.Add(hFlexSubstrateMask, colorYellow);

		// Save Data
		m_TrainingData.hFlexSubstrateSearchPatternROI = hFlexSubstrateSearchPatternROI;
		m_TrainingData.hFlexTopEdgeLine = hFlexTopEdgeLine;
		m_TrainingData.hFlexBottomEdgeLine = hFlexBottomEdgeLine;
		m_TrainingData.hFlexLeftEdgeLine = hFlexLeftEdgeLine;
		m_TrainingData.hFlexRightEdgeLine = hFlexRightEdgeLine;
		m_TrainingData.hFlexLeftEdgeLineMask = hFlexLeftEdgeLineMask;
		m_TrainingData.hFlexRightEdgeLineMask = hFlexRightEdgeLineMask;
		m_TrainingData.hFlexBottomMask = hFlexBottomMask;
		m_TrainingData.hFlexStraightCircuitLineMask = hFlexStraightCircuitLineMask;
		m_TrainingData.hFlexNumberMask = hFlexNumberMask;
		m_TrainingData.hFlexTracingMask = hFlexTracingMask;
		m_TrainingData.hFlexDamageMask = hFlexDamageMask;
		m_TrainingData.hFlexSubstrateMask = hFlexSubstrateMask;

		m_TrainingData.hFlexSubstrateModelID = hFlexSubstrateModelID;
		m_TrainingData.hFlexSubstrateDatumMatchRowRef = hFlexSubstrateDatumMatchRowRef;
		m_TrainingData.hFlexSubstrateDatumMatchColRef = hFlexSubstrateDatumMatchColRef;
		m_TrainingData.hFlexSubstrateMatchAngleRef = hFlexSubstrateMatchAngleRef;
		m_TrainingData.hFlexTracingMaskRegion = hFlexTracingMaskRegion;
		m_TrainingData.hFlexLeftWidthReference = hFlexLeftWidthReference;
		m_TrainingData.hFlexRightWidthReference = hFlexRightWidthReference;

	}
	//Contact Region Teact
	else if (nTeachStep == 6) {
		m_arrayOverlayTeach.RemoveAll();

		HObject hContactMask;
		HObject hContactRegionMask;
		HObject hContactTracingMask;

		// Get Contact Mask
		GenEmptyObj(&hContactMask);
		for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nContactMaskNumber; nMaskIdx++) {
			HObject hTmpMaskRegion;
			GenRectangle1(&hTmpMaskRegion,
				m_TrainingData.m_rectContactMask[nMaskIdx].top,
				m_TrainingData.m_rectContactMask[nMaskIdx].left,
				m_TrainingData.m_rectContactMask[nMaskIdx].bottom,
				m_TrainingData.m_rectContactMask[nMaskIdx].right);
			ConcatObj(hContactMask, hTmpMaskRegion, &hContactMask);
		}
		m_arrayOverlayTeach.Add(hContactMask, colorGreen);
		m_TrainingData.hContactMask = hContactMask;

		// Get Contact Region Mask
		GenEmptyObj(&hContactRegionMask);
		for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nContactRegionMaskNumber; nMaskIdx++) {
			HObject hTmpMaskRegion;
			GenRectangle1(&hTmpMaskRegion,
				m_TrainingData.m_rectContactRegionMask[nMaskIdx].top,
				m_TrainingData.m_rectContactRegionMask[nMaskIdx].left,
				m_TrainingData.m_rectContactRegionMask[nMaskIdx].bottom,
				m_TrainingData.m_rectContactRegionMask[nMaskIdx].right);
			ConcatObj(hContactRegionMask, hTmpMaskRegion, &hContactRegionMask);
		}
		m_arrayOverlayTeach.Add(hContactRegionMask, colorBlue);
		m_TrainingData.hContactRegionMask = hContactRegionMask;

		// Get Contact Tracing Mask
		GenEmptyObj(&hContactTracingMask);
		for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nContactTracingMaskNumber; nMaskIdx++) {
			HObject hTmpMaskRegion;
			HTuple hRows, hCols;
			for (int nPointId = 0; nPointId < m_TrainingData.nContactTracingMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonContactTracingMask[nMaskIdx][nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonContactTracingMask[nMaskIdx][nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hTmpMaskRegion, hRows, hCols);

			ConcatObj(hContactTracingMask, hTmpMaskRegion, &hContactTracingMask);
		}
		Union1(hContactTracingMask, &hContactTracingMask);
		m_arrayOverlayTeach.Add(hContactTracingMask, colorBlue);
		m_TrainingData.hContactTracingMask = hContactTracingMask;
	}
	return 0;
}

int CInspectionCore::AutoTeach(HImage hImages,
	BOOL bRegionTeach,
	int nCurTrack,
	int nDoc,
	int nFOV,
	int nTotalFOV,
	CCalibrationData* pCalibData)
{
	int nStepDebug = 0;

	if (!bRegionTeach) {

		HTuple hUniqueNoPatternsRows, hUniqueNoPatternsCols;
		HTuple hLocationTeachRows, hLocationTeachCols = HTuple();
		HTuple hModelIDs;
		HTuple hGotRotationAngleDev, hGotShiftAlongRow, hGotShiftAlongCol;

		HTuple hIsDatum, hMatchModelScore, hDatumRefRotationAngle, hDatumRefPointRow, hDatumRefPointCol, hDebugMessageOut;
		HImage hDebugImageOut;
		HRegion hMatchModelRegion, hDebugRegionOut;

		// Unique Pattern
		HTuple hDatumSearchEnable, hNoOfUniqueRegions, hMaxRotationAngle, hPatternSearchAreaAlongX, hPatternSearchAreaAlongY, hMinAcceptanceScore, hPatternSearchType;
		hDatumSearchEnable = m_TrainingData.bDattamSearchEnable;
		hNoOfUniqueRegions = m_TrainingData.nDattamSearchNoOfDattams;
		hMaxRotationAngle = m_TrainingData.nDattamSearchMaxRotAngle;
		hPatternSearchAreaAlongX = m_TrainingData.dDattamSearchAreaX / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		hPatternSearchAreaAlongY = m_TrainingData.dDattamSearchAreaY / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		hMinAcceptanceScore = (m_TrainingData.dDattamSearchMinAcceptanceScore / 100.0);
		hPatternSearchType = m_TrainingData.nDatumPatternType;

		if (hDatumSearchEnable == 1) {
			for (int i = 0; i < m_TrainingData.nDattamSearchNoOfDattams; i++) {
				TupleConcat(hUniqueNoPatternsRows, m_TrainingData.m_rectDattamSearch[i].top, &hUniqueNoPatternsRows);
				TupleConcat(hUniqueNoPatternsRows, m_TrainingData.m_rectDattamSearch[i].bottom, &hUniqueNoPatternsRows);
				TupleConcat(hUniqueNoPatternsCols, m_TrainingData.m_rectDattamSearch[i].left, &hUniqueNoPatternsCols);
				TupleConcat(hUniqueNoPatternsCols, m_TrainingData.m_rectDattamSearch[i].right, &hUniqueNoPatternsCols);
			}

			_FCI_TeachUniquePattern(hImages,
				hNoOfUniqueRegions, hMaxRotationAngle,
				hUniqueNoPatternsRows, hUniqueNoPatternsCols, hPatternSearchType,
				&hModelIDs);

			_FCI_Inspect_UniquePattern(hImages,
				&hMatchModelRegion, &hDebugImageOut, &hDebugRegionOut,
				nStepDebug, hPatternSearchAreaAlongX, hPatternSearchAreaAlongY, hMaxRotationAngle,
				hMinAcceptanceScore, hPatternSearchType, hModelIDs, hUniqueNoPatternsRows, hUniqueNoPatternsCols,
				&hIsDatum, &hMatchModelScore, &hDatumRefRotationAngle, &hDatumRefPointRow, &hDatumRefPointCol, &hDebugMessageOut);
		}

		HRegion hDeviceLocation;
		HTuple hDeviceLocationEnable, hNoOfLocationRegions, hDeviceLocWidth, hDeviceLocHeight, hDeviceMaskSize, hMinDeviceContrast, hSobelAmp, hReferencePointType;
		HTuple hDebugMsgOut, hIsPass, hEdgeRefRotationAngle, hEdgeRefPointRow, hEdgeRefPointCol;

		HTuple hDeviceEdgeDetectionEnable, hDeviceHorizontalContrast, hDeviceHorizontalDirection, hDeviceVerticalContrast, hDeviceVerticalDirection;
		HTuple hDeviceEdgeLocationTeachRows, hDeviceEdgeLocationTeachCols;

		hDeviceLocationEnable = m_TrainingData.bDeviceLocationEnable;
		hNoOfLocationRegions = m_TrainingData.nNoOfLocationTeachRegion;
		hDeviceLocWidth = m_TrainingData.dDeviceLocWidth / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		hDeviceLocHeight = m_TrainingData.dDeviceLocHeight / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		hDeviceMaskSize = m_TrainingData.dMaskSize / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		hMinDeviceContrast = m_TrainingData.nMinDeviceContrast;
		hSobelAmp = m_TrainingData.bSobelAmp;
		hReferencePointType = m_TrainingData.nDeviceReferencePointType;

		hDeviceEdgeDetectionEnable = m_TrainingData.nDeviceLocationType == EDGE_DETECTION ? 1 : 0;
		hDeviceHorizontalContrast = m_TrainingData.nDeviceHorizontalContrast;
		hDeviceHorizontalDirection = m_TrainingData.nDeviceHorizontalDirection;
		hDeviceVerticalContrast = m_TrainingData.nDeviceVerticalContrast;
		hDeviceVerticalDirection = m_TrainingData.nDeviceVerticalDirection;


		HTuple hFOV, hFOVs, hTrack;
		hFOV = nFOV;
		hFOVs = nTotalFOV;
		hTrack = nCurTrack + 1;

		if (hDeviceLocationEnable == 1) {
			for (int i = 0; i < m_TrainingData.nNoOfLocationTeachRegion; i++) {
				TupleConcat(hLocationTeachRows, m_TrainingData.m_rectDeviceLoc.top, &hLocationTeachRows);
				TupleConcat(hLocationTeachRows, m_TrainingData.m_rectDeviceLoc.bottom, &hLocationTeachRows);
				TupleConcat(hLocationTeachCols, m_TrainingData.m_rectDeviceLoc.left, &hLocationTeachCols);
				TupleConcat(hLocationTeachCols, m_TrainingData.m_rectDeviceLoc.right, &hLocationTeachCols);
			}
			//Use edge detection to detect the device edges instead of blob detection Algo
			if (hDeviceEdgeDetectionEnable == 1) {

				TupleConcat(hDeviceEdgeLocationTeachRows, m_TrainingData.m_rectDeviceHorizontalLineLoc.top, &hDeviceEdgeLocationTeachRows);
				TupleConcat(hDeviceEdgeLocationTeachRows, m_TrainingData.m_rectDeviceHorizontalLineLoc.bottom, &hDeviceEdgeLocationTeachRows);
				TupleConcat(hDeviceEdgeLocationTeachRows, m_TrainingData.m_rectDeviceVerticalLineLoc.top, &hDeviceEdgeLocationTeachRows);
				TupleConcat(hDeviceEdgeLocationTeachRows, m_TrainingData.m_rectDeviceVerticalLineLoc.bottom, &hDeviceEdgeLocationTeachRows);

				TupleConcat(hDeviceEdgeLocationTeachCols, m_TrainingData.m_rectDeviceHorizontalLineLoc.left, &hDeviceEdgeLocationTeachCols);
				TupleConcat(hDeviceEdgeLocationTeachCols, m_TrainingData.m_rectDeviceHorizontalLineLoc.right, &hDeviceEdgeLocationTeachCols);
				TupleConcat(hDeviceEdgeLocationTeachCols, m_TrainingData.m_rectDeviceVerticalLineLoc.left, &hDeviceEdgeLocationTeachCols);
				TupleConcat(hDeviceEdgeLocationTeachCols, m_TrainingData.m_rectDeviceVerticalLineLoc.right, &hDeviceEdgeLocationTeachCols);


				_FCI_Inspect_Cross_Point(hImages, &hDebugImageOut, &hDebugRegionOut, hDeviceHorizontalContrast, hDeviceHorizontalDirection,
					hDeviceVerticalContrast, hDeviceVerticalDirection, hDeviceEdgeLocationTeachRows, hDeviceEdgeLocationTeachCols, hMaxRotationAngle,
					0, &hEdgeRefRotationAngle, &hEdgeRefPointRow, &hEdgeRefPointCol, &hDebugMessageOut, &hIsPass);
			}
			else {
				_FCI_Inspect_EdgeLocation(hImages,
					&hDeviceLocation,
					hSobelAmp, hNoOfLocationRegions, hDeviceLocWidth, hDeviceLocHeight,
					hMinDeviceContrast, hLocationTeachRows, hLocationTeachCols, hDeviceMaskSize, hFOV, hFOVs, hTrack, hReferencePointType,
					&hEdgeRefRotationAngle, &hEdgeRefPointRow, &hEdgeRefPointCol);
			}

		}
		else {
			for (int i = 0; i < m_TrainingData.nNoOfLocationTeachRegion; i++) {
				TupleConcat(hLocationTeachRows, m_TrainingData.m_rectDeviceLoc.top, &hLocationTeachRows);
				TupleConcat(hLocationTeachRows, m_TrainingData.m_rectDeviceLoc.bottom, &hLocationTeachRows);
				TupleConcat(hLocationTeachCols, m_TrainingData.m_rectDeviceLoc.left, &hLocationTeachCols);
				TupleConcat(hLocationTeachCols, m_TrainingData.m_rectDeviceLoc.right, &hLocationTeachCols);
			}
			GenRectangle1(&hDeviceLocation, HTuple(hLocationTeachRows[0]), HTuple(hLocationTeachCols[0]),
				HTuple(hLocationTeachRows[1]), HTuple(hLocationTeachCols[1]));

			HTuple hv_Area, hv_Dummy, hv_Dummx, hv_DeviceAngle, hv_Device2DCenter, hv_DeviceSize;
			AreaCenter(hDeviceLocation, &hv_Area, &hv_Dummy, &hv_Dummx);
			if (0 != (HTuple(hv_Area > 0).TupleAnd((hv_Area.TupleLength()) > 0))) {
				EllipticAxis(hDeviceLocation, &hv_Dummy, &hv_Dummx, &hEdgeRefRotationAngle);

				TopReferencePoints(hImages, hDeviceLocation, hFOV, hFOVs, hReferencePointType, &hEdgeRefPointRow,
					&hEdgeRefPointCol, &hv_DeviceAngle, &hv_Device2DCenter, &hv_DeviceSize);
			}
		}
		//else {
		//	hLocationTeachRows = hLocationTeachCols = HTuple();//0;
		//}
		HTuple hRefRotationAngle, hRefPointRow, hRefPointCol, hGotRotationAngle;
		if (hDatumSearchEnable == 1 || hDeviceLocationEnable == 1)
		{
			if (hDatumSearchEnable == 1)
			{
				hRefRotationAngle = hDatumRefRotationAngle;
				hRefPointRow = hDatumRefPointRow;
				hRefPointCol = hDatumRefPointCol;
			}
			else
			{
				hRefRotationAngle = hEdgeRefRotationAngle;
				hRefPointRow = hEdgeRefPointRow;
				hRefPointCol = hEdgeRefPointCol;
			}

			HTuple hTopRefGotRotationAngle, hTopRefGotShiftAlongRow, hTopRefGotShiftAlongCol;

			_FCI_Inspect_DeviceLocation(hImages,
				&hDeviceLocation, &hMatchModelRegion,
				&hDebugImageOut, &hDebugRegionOut, hDatumSearchEnable, hDeviceLocationEnable, nStepDebug,
				hSobelAmp, hNoOfLocationRegions,
				hDeviceLocWidth, hDeviceLocHeight, hMinDeviceContrast,
				hNoOfUniqueRegions, hMaxRotationAngle,
				hPatternSearchAreaAlongX, hPatternSearchAreaAlongY, hMinAcceptanceScore, hPatternSearchType,
				hModelIDs, hRefRotationAngle,
				hRefPointRow, hRefPointCol,
				hEdgeRefRotationAngle, hEdgeRefPointRow, hEdgeRefPointCol,
				hUniqueNoPatternsRows, hUniqueNoPatternsCols,
				hLocationTeachRows, hLocationTeachCols,
				hDeviceMaskSize, hFOV, hFOVs, hTrack, hReferencePointType,
				hDeviceEdgeDetectionEnable,
				hDeviceHorizontalContrast, hDeviceHorizontalDirection,
				hDeviceVerticalContrast, hDeviceVerticalDirection,
				hDeviceEdgeLocationTeachRows, hDeviceEdgeLocationTeachCols,
				&hIsPass, &hIsDatum, &hGotRotationAngle, &hGotShiftAlongRow, &hGotShiftAlongCol,
				&hMatchModelScore,
				&hTopRefGotRotationAngle, &hTopRefGotShiftAlongRow, &hTopRefGotShiftAlongCol,
				&hDebugMsgOut);
		}


		int nIndex = 0;
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceLocationEnable);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hNoOfLocationRegions);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceLocWidth);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceLocHeight);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceMaskSize);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hMinDeviceContrast);

		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDatumSearchEnable);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hNoOfUniqueRegions);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hMaxRotationAngle);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hPatternSearchAreaAlongX);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hPatternSearchAreaAlongY);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hMinAcceptanceScore);

		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hUniqueNoPatternsRows);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hUniqueNoPatternsCols);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hLocationTeachRows);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hLocationTeachCols);

		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hModelIDs);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hRefRotationAngle);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hRefPointRow);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hRefPointCol);

		//For Device Reference Point
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hEdgeRefRotationAngle);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hEdgeRefPointRow);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hEdgeRefPointCol);

		//For Device Edge Detection option
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceEdgeDetectionEnable);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceHorizontalContrast);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceHorizontalDirection);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceVerticalContrast);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceVerticalDirection);

		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceEdgeLocationTeachRows);
		m_TrainingData.hvec_TupleVectorLocation.SetAt(nIndex++, hDeviceEdgeLocationTeachCols);


	}
	else {
		HTuple hPVIArea;
		//OutputDebugLogTo(9, TRUE, "Auto Teach Start");//// for testing

		int nIndex = 0;
		for (int i = 0; i < m_TrainingData.darrayPVI[nIndex][0].GetCount(); i++)
			TupleConcat(hPVIArea, m_TrainingData.darrayPVI[nIndex][0][i], &hPVIArea);
		m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(0, hPVIArea);

		for (int nArea = 0; nArea < hPVIArea; nArea++) {
			//OutputDebugLogTo(9, TRUE, "Auto Teach PVIArea %.2f", hPVIArea.D());//// for testing
			HTuple hPVIAngle, hPVI2DCenter, hPVISize, hPolyLinePoints, hPVIPolygonMaskPoint;
			HTuple hMaskNumber, hMask2DCenterPoints, hMaskAngleConcat, hMaskSize;

			nIndex = 1;
			for (int i = 0; i < m_TrainingData.darrayPVI[nIndex][nArea].GetCount(); i++)
				TupleConcat(hPVIAngle, m_TrainingData.darrayPVI[nIndex][nArea][i], &hPVIAngle);
			m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hPVIAngle);

			for (int i = 0; i < m_TrainingData.darrayPVI[nIndex][nArea].GetCount(); i++)
				TupleConcat(hPVI2DCenter, m_TrainingData.darrayPVI[nIndex][nArea][i], &hPVI2DCenter);
			m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hPVI2DCenter);

			for (int i = 0; i < m_TrainingData.darrayPVI[nIndex][nArea].GetCount(); i++)
				TupleConcat(hPVISize, m_TrainingData.darrayPVI[nIndex][nArea][i], &hPVISize);
			m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hPVISize);

			for (int i = 0; i < m_TrainingData.darrayPVI[nIndex][nArea].GetCount(); i++)
				TupleConcat(hMaskNumber, m_TrainingData.darrayPVI[nIndex][nArea][i], &hMaskNumber);
			m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hMaskNumber);

			for (int i = 0; i < m_TrainingData.darrayPVI[nIndex][nArea].GetCount(); i++)
				TupleConcat(hMask2DCenterPoints, m_TrainingData.darrayPVI[nIndex][nArea][i], &hMask2DCenterPoints);
			m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hMask2DCenterPoints);

			for (int i = 0; i < m_TrainingData.darrayPVI[nIndex][nArea].GetCount(); i++)
				TupleConcat(hMaskAngleConcat, m_TrainingData.darrayPVI[nIndex][nArea][i], &hMaskAngleConcat);
			m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hMaskAngleConcat);

			for (int i = 0; i < m_TrainingData.darrayPVI[nIndex][nArea].GetCount(); i++)
				TupleConcat(hMaskSize, m_TrainingData.darrayPVI[nIndex][nArea][i], &hMaskSize);
			m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hMaskSize);

			//PVI Edge
			for (int i = 0; i < m_TrainingData.darrayPVI[nIndex][nArea].GetCount(); i++)
				TupleConcat(hPolyLinePoints, m_TrainingData.darrayPVI[nIndex][nArea][i], &hPolyLinePoints);
			m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hPolyLinePoints);

			//PVI Polygon Mask
			for (int i = 0; i < m_TrainingData.darrayPVI[nIndex][nArea].GetCount(); i++)
				TupleConcat(hPVIPolygonMaskPoint, m_TrainingData.darrayPVI[nIndex][nArea][i], &hPVIPolygonMaskPoint);
			m_TrainingData.hvec_TupleVectorPVI[nIndex++].InsertAt(nArea, hPVIPolygonMaskPoint);
		}

		//// ---- encap auto teach part ---- ////
		HTuple hMinEncapContrast, hMaxEncapContrast;
		int nIndexEncap = 0;
		for (int i = 0; i < m_TrainingData.darrayEncap[nIndexEncap].GetCount(); i++)
			TupleConcat(hMinEncapContrast, m_TrainingData.darrayEncap[nIndexEncap][i], &hMinEncapContrast);
		m_TrainingData.hvec_TupleVectorEncap.InsertAt(nIndexEncap++, hMinEncapContrast);

		for (int i = 0; i < m_TrainingData.darrayEncap[nIndexEncap].GetCount(); i++)
			TupleConcat(hMaxEncapContrast, m_TrainingData.darrayEncap[nIndexEncap][i], &hMaxEncapContrast);
		m_TrainingData.hvec_TupleVectorEncap.InsertAt(nIndexEncap++, hMaxEncapContrast);

		//// ---- Contact Auto Teach ---- ////
		if (m_TrainingData.bEnableContact) {
			HObject hContactMask;
			HObject hContactRegionMask;
			HObject hContactTracingMask;

			// Get Contact Mask
			GenEmptyObj(&hContactMask);
			for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nContactMaskNumber; nMaskIdx++) {
				HObject hTmpMaskRegion;
				GenRectangle1(&hTmpMaskRegion,
					m_TrainingData.m_rectContactMask[nMaskIdx].top,
					m_TrainingData.m_rectContactMask[nMaskIdx].left,
					m_TrainingData.m_rectContactMask[nMaskIdx].bottom,
					m_TrainingData.m_rectContactMask[nMaskIdx].right);
				ConcatObj(hContactMask, hTmpMaskRegion, &hContactMask);
			}
			m_TrainingData.hContactMask = hContactMask;

			// Get Contact Region Mask
			GenEmptyObj(&hContactRegionMask);
			for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nContactRegionMaskNumber; nMaskIdx++) {
				HObject hTmpMaskRegion;
				GenRectangle1(&hTmpMaskRegion,
					m_TrainingData.m_rectContactRegionMask[nMaskIdx].top,
					m_TrainingData.m_rectContactRegionMask[nMaskIdx].left,
					m_TrainingData.m_rectContactRegionMask[nMaskIdx].bottom,
					m_TrainingData.m_rectContactRegionMask[nMaskIdx].right);
				ConcatObj(hContactRegionMask, hTmpMaskRegion, &hContactRegionMask);
			}
			m_TrainingData.hContactRegionMask = hContactRegionMask;

			// Get Contact Tracing Mask
			GenEmptyObj(&hContactTracingMask);
			for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nContactTracingMaskNumber; nMaskIdx++) {
				HObject hTmpMaskRegion;
				HTuple hRows, hCols;
				for (int nPointId = 0; nPointId < m_TrainingData.nContactTracingMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
					TupleConcat(hRows, m_TrainingData.m_polygonContactTracingMask[nMaskIdx][nPointId].y, &hRows);
					TupleConcat(hCols, m_TrainingData.m_polygonContactTracingMask[nMaskIdx][nPointId].x, &hCols);
				}
				GenRegionPolygonFilled(&hTmpMaskRegion, hRows, hCols);

				ConcatObj(hContactTracingMask, hTmpMaskRegion, &hContactTracingMask);
			}
			Union1(hContactTracingMask, &hContactTracingMask);
			m_TrainingData.hContactTracingMask = hContactTracingMask;
		}

		//// ---- Flex Auto Teach ---- ////
		if (m_TrainingData.bFlexEnable) {
			HObject hFlexSubstrateSearchPatternROI;
			HObject hFlexTopEdgeLine;
			HObject hFlexBottomEdgeLine;
			HObject hFlexLeftEdgeLine;
			HObject hFlexRightEdgeLine;

			HObject hFlexLeftEdgeLineMask;
			HObject hFlexRightEdgeLineMask;

			HObject hFlexBottomMask;
			HObject hFlexStraightCircuitLineMask;

			HObject hFlexNumberMask;
			HObject hFlexTracingMask;
			HObject hFlexDamageMask;
			HObject hFlexSubstrateMask;
			// Check Substrate Mask Enable
			if (m_TrainingData.nFlexSubstrateMaskNumber > 0) {
				m_TrainingData.bFlexEnableSubstrateMask = TRUE;
			}
			else {
				m_TrainingData.bFlexEnableSubstrateMask = FALSE;
			}

			// Get Substrate Pattern ROI
			HObject hFlexSubstratePatternROI1;
			GenRectangle1(&hFlexSubstratePatternROI1,
				m_TrainingData.m_rectFlexSubstratePatternROI1.top,
				m_TrainingData.m_rectFlexSubstratePatternROI1.left,
				m_TrainingData.m_rectFlexSubstratePatternROI1.bottom,
				m_TrainingData.m_rectFlexSubstratePatternROI1.right);
			HObject hFlexSubstratePatternROI2;
			GenRectangle1(&hFlexSubstratePatternROI2,
				m_TrainingData.m_rectFlexSubstratePatternROI2.top,
				m_TrainingData.m_rectFlexSubstratePatternROI2.left,
				m_TrainingData.m_rectFlexSubstratePatternROI2.bottom,
				m_TrainingData.m_rectFlexSubstratePatternROI2.right);
			HObject hFlexSubstratePatternROI;
			Union2(hFlexSubstratePatternROI1, hFlexSubstratePatternROI2, &hFlexSubstratePatternROI);

			// Get Substrate Search Pattern ROI
			GenRectangle1(&hFlexSubstrateSearchPatternROI,
				m_TrainingData.m_rectFlexSubstrateSearchPatternROI.top,
				m_TrainingData.m_rectFlexSubstrateSearchPatternROI.left,
				m_TrainingData.m_rectFlexSubstrateSearchPatternROI.bottom,
				m_TrainingData.m_rectFlexSubstrateSearchPatternROI.right);

			// Get Flex Top Edge Line Polygon
			HTuple hRows, hCols;
			for (int nPointId = 0; nPointId < 2; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonFlexTopEdgeLine[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonFlexTopEdgeLine[nPointId].x, &hCols);
			}
			GenContourPolygonXld(&hFlexTopEdgeLine, hRows, hCols);

			// Get Flex Bottom Edge Line Polygon
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < 2; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonFlexBottomEdgeLine[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonFlexBottomEdgeLine[nPointId].x, &hCols);
			}
			GenContourPolygonXld(&hFlexBottomEdgeLine, hRows, hCols);

			// Get Flex Left Edge Line Polygon
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < 2; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonFlexLeftEdgeLine[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonFlexLeftEdgeLine[nPointId].x, &hCols);
			}
			GenContourPolygonXld(&hFlexLeftEdgeLine, hRows, hCols);

			// Get Flex Right Edge Line Polygon
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < 2; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonFlexRightEdgeLine[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonFlexRightEdgeLine[nPointId].x, &hCols);
			}
			GenContourPolygonXld(&hFlexRightEdgeLine, hRows, hCols);

			// Get Flex Left Edge Line Mask
			GenRectangle1(&hFlexLeftEdgeLineMask,
				m_TrainingData.m_rectFlexLeftEdgeLineMask.top,
				m_TrainingData.m_rectFlexLeftEdgeLineMask.left,
				m_TrainingData.m_rectFlexLeftEdgeLineMask.bottom,
				m_TrainingData.m_rectFlexLeftEdgeLineMask.right);

			// Get Flex Right Edge Line Mask
			GenRectangle1(&hFlexRightEdgeLineMask,
				m_TrainingData.m_rectFlexRightEdgeLineMask.top,
				m_TrainingData.m_rectFlexRightEdgeLineMask.left,
				m_TrainingData.m_rectFlexRightEdgeLineMask.bottom,
				m_TrainingData.m_rectFlexRightEdgeLineMask.right);

			// Get Flex Bottom ROI Rectangle
			GenRectangle1(&hFlexBottomMask,
				m_TrainingData.m_rectFlexBottomROI.top,
				m_TrainingData.m_rectFlexBottomROI.left,
				m_TrainingData.m_rectFlexBottomROI.bottom,
				m_TrainingData.m_rectFlexBottomROI.right);

			// Get Flex Straight Circuit Line Mask
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nFlexStraightCircuitLineMaskPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonFlexStraightCircuitLineMask[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonFlexStraightCircuitLineMask[nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hFlexStraightCircuitLineMask, hRows, hCols);

			// Get Flex Number Mask
			GenRectangle1(&hFlexNumberMask,
				m_TrainingData.m_rectFlexNumberMask.top,
				m_TrainingData.m_rectFlexNumberMask.left,
				m_TrainingData.m_rectFlexNumberMask.bottom,
				m_TrainingData.m_rectFlexNumberMask.right);

			// Get Flex Tracing Mask
			GenEmptyObj(&hFlexTracingMask);
			for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nFlexTracingMaskNumber; nMaskIdx++) {
				HObject hTmpMaskRegion;
				HTuple hRows, hCols;
				for (int nPointId = 0; nPointId < m_TrainingData.nFlexTracingMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
					TupleConcat(hRows, m_TrainingData.m_polygonFlexTracingMask[nMaskIdx][nPointId].y, &hRows);
					TupleConcat(hCols, m_TrainingData.m_polygonFlexTracingMask[nMaskIdx][nPointId].x, &hCols);
				}
				GenRegionPolygonFilled(&hTmpMaskRegion, hRows, hCols);

				ConcatObj(hFlexTracingMask, hTmpMaskRegion, &hFlexTracingMask);
			}
			Union1(hFlexTracingMask, &hFlexTracingMask);

			// Get Flex Damage Mask
			GenEmptyObj(&hFlexDamageMask);
			for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nFlexDamageMaskNumber; nMaskIdx++) {
				HObject hTmpMaskRegion;
				HTuple hRows, hCols;
				for (int nPointId = 0; nPointId < m_TrainingData.nFlexDamageMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
					TupleConcat(hRows, m_TrainingData.m_polygonFlexDamageMask[nMaskIdx][nPointId].y, &hRows);
					TupleConcat(hCols, m_TrainingData.m_polygonFlexDamageMask[nMaskIdx][nPointId].x, &hCols);
				}
				GenRegionPolygonFilled(&hTmpMaskRegion, hRows, hCols);

				ConcatObj(hFlexDamageMask, hTmpMaskRegion, &hFlexDamageMask);
			}
			Union1(hFlexDamageMask, &hFlexDamageMask);

			// Get Flex Substrate Mask 
			GenEmptyObj(&hFlexSubstrateMask);
			if (m_TrainingData.bFlexEnableSubstrateMask) {
				for (int nMaskIdx = 0; nMaskIdx < m_TrainingData.nFlexSubstrateMaskNumber; nMaskIdx++) {
					HObject hTmpMaskRegion;
					HTuple hRows, hCols;
					for (int nPointId = 0; nPointId < m_TrainingData.nFlexSubstrateMaskPolygonPointNumber[nMaskIdx]; nPointId++) {
						TupleConcat(hRows, m_TrainingData.m_polygonFlexSubstrateMask[nMaskIdx][nPointId].y, &hRows);
						TupleConcat(hCols, m_TrainingData.m_polygonFlexSubstrateMask[nMaskIdx][nPointId].x, &hCols);
					}
					GenRegionPolygonFilled(&hTmpMaskRegion, hRows, hCols);
					ConcatObj(hFlexSubstrateMask, hTmpMaskRegion, &hFlexSubstrateMask);
				}
				Union1(hFlexSubstrateMask, &hFlexSubstrateMask);
			}

			// Teach Procesing
			HTuple hFlexSubstrateModelID;
			HTuple hFlexSubstrateDatumMatchRowRef;
			HTuple hFlexSubstrateDatumMatchColRef;
			HTuple hFlexSubstrateMatchAngleRef;
			HTuple hFlexLeftWidthReference;
			HTuple hFlexRightWidthReference;
			HObject hFlexTracingMaskRegion;

			// Parameters
			double dResolution = (pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2;
			HTuple hEnableSubstrateMask = m_TrainingData.bFlexEnableSubstrateMask;
			HTuple hEnableDynamicEdgeFlex = m_TrainingData.bFlexEnableDynamicEdge;
			HTuple hMinCircuitLineContrast = m_TrainingData.nFlexCircuitLineContrast;
			HTuple hEdgeFlexContrast = m_TrainingData.nFlexEdgeContrast;
			HTuple hOffsetMaskSize = m_TrainingData.dFlexOffsetMaskSize / dResolution;

			_HP_Flex_Teach(hImages,
				hFlexSubstratePatternROI,
				hFlexTracingMask,
				hFlexLeftEdgeLineMask,
				hFlexRightEdgeLineMask,
				&hFlexTracingMaskRegion,
				hEnableSubstrateMask,
				hEnableDynamicEdgeFlex,
				hMinCircuitLineContrast,
				hEdgeFlexContrast,
				hOffsetMaskSize,
				&hFlexSubstrateModelID,
				&hFlexSubstrateDatumMatchRowRef,
				&hFlexSubstrateDatumMatchColRef,
				&hFlexSubstrateMatchAngleRef,
				&hFlexLeftWidthReference,
				&hFlexRightWidthReference);

			// Save Data
			m_TrainingData.hFlexSubstrateSearchPatternROI = hFlexSubstrateSearchPatternROI;
			m_TrainingData.hFlexTopEdgeLine = hFlexTopEdgeLine;
			m_TrainingData.hFlexBottomEdgeLine = hFlexBottomEdgeLine;
			m_TrainingData.hFlexLeftEdgeLine = hFlexLeftEdgeLine;
			m_TrainingData.hFlexRightEdgeLine = hFlexRightEdgeLine;
			m_TrainingData.hFlexLeftEdgeLineMask = hFlexLeftEdgeLineMask;
			m_TrainingData.hFlexRightEdgeLineMask = hFlexRightEdgeLineMask;
			m_TrainingData.hFlexBottomMask = hFlexBottomMask;
			m_TrainingData.hFlexStraightCircuitLineMask = hFlexStraightCircuitLineMask;
			m_TrainingData.hFlexNumberMask = hFlexNumberMask;
			m_TrainingData.hFlexTracingMask = hFlexTracingMask;
			m_TrainingData.hFlexDamageMask = hFlexDamageMask;
			m_TrainingData.hFlexSubstrateMask = hFlexSubstrateMask;

			m_TrainingData.hFlexSubstrateModelID = hFlexSubstrateModelID;
			m_TrainingData.hFlexSubstrateDatumMatchRowRef = hFlexSubstrateDatumMatchRowRef;
			m_TrainingData.hFlexSubstrateDatumMatchColRef = hFlexSubstrateDatumMatchColRef;
			m_TrainingData.hFlexSubstrateMatchAngleRef = hFlexSubstrateMatchAngleRef;
			m_TrainingData.hFlexTracingMaskRegion = hFlexTracingMaskRegion;
			m_TrainingData.hFlexLeftWidthReference = hFlexLeftWidthReference;
			m_TrainingData.hFlexRightWidthReference = hFlexRightWidthReference;
		}

		//// ---- Blue Encap Auto Teach ---- ////
		if (m_TrainingData.bEnableEncapBlue) {
			// Glue - Mason
			// Get Glue Mask Polygon
			HObject hBEncapGlueMask;
			HTuple hRows;
			HTuple hCols;
			for (int nPointId = 0; nPointId < ENCAP_GLUE_MASK_POINT; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonBEncapGlueMask[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonBEncapGlueMask[nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hBEncapGlueMask, hRows, hCols);
			m_TrainingData.hBEncapGlueMask = hBEncapGlueMask;

			// Contamination - Mason
			// Get Encap Flex Edge Mask ROI Rectangle
			HObject hBEncapFlexEdgeMaskROI;
			GenRectangle1(&hBEncapFlexEdgeMaskROI,
				m_TrainingData.m_rectBEncapFlexEdgeMaskROI.top,
				m_TrainingData.m_rectBEncapFlexEdgeMaskROI.left,
				m_TrainingData.m_rectBEncapFlexEdgeMaskROI.bottom,
				m_TrainingData.m_rectBEncapFlexEdgeMaskROI.right);
			m_TrainingData.hBEncapFlexEdgeMaskROI = hBEncapFlexEdgeMaskROI;
		}
		//// ---- Dark Encap Auto Teach ---- ////
		else if (m_TrainingData.bEnableDarkEncap) {
			// Glue - Mason
			// Get Glue Mask Rectangle
			HObject hDarkGlueMask;
			GenRectangle1(&hDarkGlueMask,
				m_TrainingData.m_rectDarkGlueMask.top,
				m_TrainingData.m_rectDarkGlueMask.left,
				m_TrainingData.m_rectDarkGlueMask.bottom,
				m_TrainingData.m_rectDarkGlueMask.right);
			m_TrainingData.hDarkGlueMask = hDarkGlueMask;

			// Contamination - Mason
			// Get Flex Edge Mask Rectangle
			HObject hDarkFlexEdgeMask;
			GenRectangle1(&hDarkFlexEdgeMask,
				m_TrainingData.m_rectDarkFlexEdgeMask.top,
				m_TrainingData.m_rectDarkFlexEdgeMask.left,
				m_TrainingData.m_rectDarkFlexEdgeMask.bottom,
				m_TrainingData.m_rectDarkFlexEdgeMask.right);
			m_TrainingData.hDarkFlexEdgeMask = hDarkFlexEdgeMask;

			// Get Bottom Edge Mask Rectangle
			HObject hDarkBottomEdgeMask;
			GenRectangle1(&hDarkBottomEdgeMask,
				m_TrainingData.m_rectDarkBottomEdgeMask.top,
				m_TrainingData.m_rectDarkBottomEdgeMask.left,
				m_TrainingData.m_rectDarkBottomEdgeMask.bottom,
				m_TrainingData.m_rectDarkBottomEdgeMask.right);
			m_TrainingData.hDarkBottomEdgeMask = hDarkBottomEdgeMask;

			// Get Right Edge Mask Rectangle
			HObject hDarkRightEdgeMask;
			GenRectangle1(&hDarkRightEdgeMask,
				m_TrainingData.m_rectDarkRightEdgeMask.top,
				m_TrainingData.m_rectDarkRightEdgeMask.left,
				m_TrainingData.m_rectDarkRightEdgeMask.bottom,
				m_TrainingData.m_rectDarkRightEdgeMask.right);
			m_TrainingData.hDarkRightEdgeMask = hDarkRightEdgeMask;

			// Get Inner Mask Rectangle
			HObject hDarkInnerMask;
			GenRectangle1(&hDarkInnerMask,
				m_TrainingData.m_rectDarkInnerMask.top,
				m_TrainingData.m_rectDarkInnerMask.left,
				m_TrainingData.m_rectDarkInnerMask.bottom,
				m_TrainingData.m_rectDarkInnerMask.right);
			m_TrainingData.hDarkInnerMask = hDarkInnerMask;
		}
		//// ---- Dark Blue Encap Auto Teach ---- ////
		else if (m_TrainingData.bEnableDarkBlueEncap) {
			HObject hDBEncapExtractROI;
			HObject hDBEncapInnerROI;
			HObject hDBEncapTopEdgeMaskROI;
			HObject hDBEncapBottomEdgeMaskROI;
			HObject hDBEncapLeftEdgeMaskROI;
			HObject hDBEncapRightEdgeMaskROI;
			HObject hDBEncapFlexEdgeMaskROI;

			// Get Encap Extract ROI Polygon
			HTuple hRows, hCols;
			for (int nPointId = 0; nPointId < DARK_BLUE_ENCAP_ROI_POINT; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonDBEncapExtractROI[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonDBEncapExtractROI[nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hDBEncapExtractROI, hRows, hCols);

			// Get Encap Inner ROI Rectangle
			GenRectangle1(&hDBEncapInnerROI,
				m_TrainingData.m_rectDBEncapInnerROI.top,
				m_TrainingData.m_rectDBEncapInnerROI.left,
				m_TrainingData.m_rectDBEncapInnerROI.bottom,
				m_TrainingData.m_rectDBEncapInnerROI.right);

			// Get Encap Top Edge Mask ROI Rectangle
			GenRectangle1(&hDBEncapTopEdgeMaskROI,
				m_TrainingData.m_rectDBEncapTopEdgeMaskROI.top,
				m_TrainingData.m_rectDBEncapTopEdgeMaskROI.left,
				m_TrainingData.m_rectDBEncapTopEdgeMaskROI.bottom,
				m_TrainingData.m_rectDBEncapTopEdgeMaskROI.right);

			// Get Encap Bottom Edge Mask ROI Rectangle
			GenRectangle1(&hDBEncapBottomEdgeMaskROI,
				m_TrainingData.m_rectDBEncapBottomEdgeMaskROI.top,
				m_TrainingData.m_rectDBEncapBottomEdgeMaskROI.left,
				m_TrainingData.m_rectDBEncapBottomEdgeMaskROI.bottom,
				m_TrainingData.m_rectDBEncapBottomEdgeMaskROI.right);

			// Get Encap Left Edge Mask ROI Rectangle
			GenRectangle1(&hDBEncapLeftEdgeMaskROI,
				m_TrainingData.m_rectDBEncapLeftEdgeMaskROI.top,
				m_TrainingData.m_rectDBEncapLeftEdgeMaskROI.left,
				m_TrainingData.m_rectDBEncapLeftEdgeMaskROI.bottom,
				m_TrainingData.m_rectDBEncapLeftEdgeMaskROI.right);

			// Get Encap Right Edge Mask ROI Rectangle
			GenRectangle1(&hDBEncapRightEdgeMaskROI,
				m_TrainingData.m_rectDBEncapRightEdgeMaskROI.top,
				m_TrainingData.m_rectDBEncapRightEdgeMaskROI.left,
				m_TrainingData.m_rectDBEncapRightEdgeMaskROI.bottom,
				m_TrainingData.m_rectDBEncapRightEdgeMaskROI.right);

			// Get Encap Flex Edge Mask ROI Rectangle
			GenRectangle1(&hDBEncapFlexEdgeMaskROI,
				m_TrainingData.m_rectDBEncapFlexEdgeMaskROI.top,
				m_TrainingData.m_rectDBEncapFlexEdgeMaskROI.left,
				m_TrainingData.m_rectDBEncapFlexEdgeMaskROI.bottom,
				m_TrainingData.m_rectDBEncapFlexEdgeMaskROI.right);

			// Excess - Mason
			// Get Left Excess Mask Rectangle
			HObject hDBLeftExcessMask;
			GenRectangle1(&hDBLeftExcessMask,
				m_TrainingData.m_rectDBLeftExcessMask.top,
				m_TrainingData.m_rectDBLeftExcessMask.left,
				m_TrainingData.m_rectDBLeftExcessMask.bottom,
				m_TrainingData.m_rectDBLeftExcessMask.right);
			m_TrainingData.hDBLeftExcessMask = hDBLeftExcessMask;

			// Get Right Excess Mask Rectangle
			HObject hDBRightExcessMask;
			GenRectangle1(&hDBRightExcessMask,
				m_TrainingData.m_rectDBRightExcessMask.top,
				m_TrainingData.m_rectDBRightExcessMask.left,
				m_TrainingData.m_rectDBRightExcessMask.bottom,
				m_TrainingData.m_rectDBRightExcessMask.right);
			m_TrainingData.hDBRightExcessMask = hDBRightExcessMask;

			// Insufficient - Mason
			// Get Cover Insufficient Mask Rectangle
			HObject hDBCoverInsufficientMask;
			GenRectangle1(&hDBCoverInsufficientMask,
				m_TrainingData.m_rectDBCoverInsufficientMask.top,
				m_TrainingData.m_rectDBCoverInsufficientMask.left,
				m_TrainingData.m_rectDBCoverInsufficientMask.bottom,
				m_TrainingData.m_rectDBCoverInsufficientMask.right);
			m_TrainingData.hDBCoverInsufficientMask = hDBCoverInsufficientMask;

			// Get Inner Insufficient Mask Polygon
			HObject hDBInnerInsufficientMask;
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < DARK_BLUE_ENCAP_INNER_INSUFFICIENT_MASK_POINT; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonDBInnerInsufficientMask[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonDBInnerInsufficientMask[nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hDBInnerInsufficientMask, hRows, hCols);
			m_TrainingData.hDBInnerInsufficientMask = hDBInnerInsufficientMask;

			// Get Left Insufficient Mask Rectangle
			HObject hDBLeftInsufficientMask;
			GenRectangle1(&hDBLeftInsufficientMask,
				m_TrainingData.m_rectDBLeftInsufficientMask.top,
				m_TrainingData.m_rectDBLeftInsufficientMask.left,
				m_TrainingData.m_rectDBLeftInsufficientMask.bottom,
				m_TrainingData.m_rectDBLeftInsufficientMask.right);
			m_TrainingData.hDBLeftInsufficientMask = hDBLeftInsufficientMask;

			// Get Right Insufficient Mask Rectangle
			HObject hDBRightInsufficientMask;
			GenRectangle1(&hDBRightInsufficientMask,
				m_TrainingData.m_rectDBRightInsufficientMask.top,
				m_TrainingData.m_rectDBRightInsufficientMask.left,
				m_TrainingData.m_rectDBRightInsufficientMask.bottom,
				m_TrainingData.m_rectDBRightInsufficientMask.right);
			m_TrainingData.hDBRightInsufficientMask = hDBRightInsufficientMask;

			// Glue - Mason
			// Get Glue Mask Polygon
			HObject hDBEncapGlueMask;
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < ENCAP_GLUE_MASK_POINT; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_polygonDBEncapGlueMask[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_polygonDBEncapGlueMask[nPointId].x, &hCols);
			}
			GenRegionPolygonFilled(&hDBEncapGlueMask, hRows, hCols);
			m_TrainingData.hDBEncapGlueMask = hDBEncapGlueMask;
			
			// Inspect Blow Hole
			HRegion hEncapRgn;
			HTuple hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2;
			// // Region for tracing Left Mask to inspect BlowHole 
			hEncapRow1 = m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectDarkEncapBHMaskingLeftPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_TrainingData.hDarkBlueEncapBHMaskingLeft = hEncapRgn;

			// // Region for tracing Right Mask to inspect BlowHole 
			hEncapRow1 = m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea.top;
			hEncapCol1 = m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea.left;
			hEncapRow2 = m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea.bottom;
			hEncapCol2 = m_TrainingData.m_rectDarkEncapBHMaskingRightPolygonPointArea.right;
			GenRectangle1(&hEncapRgn, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
			m_TrainingData.hDarkBlueEncapBHMaskingRight = hEncapRgn;
			// Inspect Wire And IC
			// // Region for Inspect Wire And IC on Dark Blue Encap (Bottom)
			HTuple hPoint;
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nDarkBlueEncapWireBottomPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_DarkBlueEncapWireBottomPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_TrainingData.hDarkBlueEncapWireBottom = hEncapRgn;
			// // Region for Inspect IC on Dark Blue Encap
			hRows = HTuple();
			hCols = HTuple();
			for (int nPointId = 0; nPointId < m_TrainingData.nDarkBlueEncapICPolygonPointNumber; nPointId++) {
				TupleConcat(hRows, m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].y, &hRows);
				TupleConcat(hCols, m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].x, &hCols);
				hPoint.Clear();
				hPoint.Append(m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].y);
				hPoint.Append(m_TrainingData.m_DarkBlueEncapICPolygonPointArea[nPointId].x);
			}
			GenRegionPolygonFilled(&hEncapRgn, hRows, hCols);
			m_TrainingData.hDarkBlueEncapIC = hEncapRgn;

			// Save Data
			m_TrainingData.hDBEncapExtractROI = hDBEncapExtractROI;
			m_TrainingData.hDBEncapInnerROI = hDBEncapInnerROI;
			m_TrainingData.hDBEncapTopEdgeMaskROI = hDBEncapTopEdgeMaskROI;
			m_TrainingData.hDBEncapBottomEdgeMaskROI = hDBEncapBottomEdgeMaskROI;
			m_TrainingData.hDBEncapLeftEdgeMaskROI = hDBEncapLeftEdgeMaskROI;
			m_TrainingData.hDBEncapRightEdgeMaskROI = hDBEncapRightEdgeMaskROI;
			m_TrainingData.hDBEncapFlexEdgeMaskROI = hDBEncapFlexEdgeMaskROI;
		}
	}
	return 0;
}

int CInspectionCore::ClearTeachParam(CTrainingData* pTrainingData)
{
	if (pTrainingData != NULL) {
		GenEmptyObj(&pTrainingData->hObjectDeviceLocation);
		pTrainingData->hTupleDatumCenterParam.Clear();
		pTrainingData->hTupleDatumCenterParam = HTuple();
		pTrainingData->hTupleProjectionParam.Clear();
		pTrainingData->hTupleProjectionParam = HTuple();
		pTrainingData->hTupleTeachDoc.Clear();
		pTrainingData->hTupleTeachDoc = HTuple();
		pTrainingData->hTupleEdgeTeachTuple.Clear();
		pTrainingData->hTupleEdgeTeachTuple = HTuple();
		pTrainingData->hTupleDatumTeachTuple.Clear();
		pTrainingData->hTupleDatumTeachTuple = HTuple();

	}
	else {
		for (int i = 0; i < 32; i++) {
			m_TrainingData.hvec_TupleVectorPVI[i].RemoveAll();
			for (int j = 0; j < 10; j++)
				m_TrainingData.darrayPVI[i][j].RemoveAll();
		}


		m_TrainingData.hvec_TupleVectorEncap.RemoveAll();

		for (int k = 0; k < 15; k++) {
			m_TrainingData.darrayEncap[k].RemoveAll();
		}
	}
	return 0;
}

int CInspectionCore::ClearInspParam(CInspectinonData* pInspData, BOOL bInspClearAll)
{
	if (!bInspClearAll) {
		//Clear Tilt Height Data
	}
	else {

		pInspData->hStoreTeachModelRowCentre.Clear();
		pInspData->hStoreTeachModelRowCentre = HTuple();
		pInspData->hStoreTeachModelColCentre.Clear();
		pInspData->hStoreTeachModelColCentre = HTuple();
		pInspData->hStoreMatchModelCentreRow.Clear();
		pInspData->hStoreMatchModelCentreRow = HTuple();
		pInspData->hStoreMatchModelCentreCol.Clear();
		pInspData->hStoreMatchModelCentreCol = HTuple();

		pInspData->hInspectShiftAlongXMicron.Clear();
		pInspData->hInspectShiftAlongXMicron = HTuple();

		pInspData->hInspectShiftAlongYMicron.Clear();
		pInspData->hInspectShiftAlongYMicron = HTuple();


		//OutputDebugLogTo(9, TRUE, "ClearInspParam All");//// for testing
	}

	return 0;
}

int CInspectionCore::ClearCalibParam(CCalibrationData* pCalibData, BOOL bClearAvg, BOOL bClearInsp)
{
	if (!bClearInsp) {
		GenEmptyObj(&pCalibData->hCalibSumImage);
		if (bClearAvg)
			GenEmptyObj(&pCalibData->hCalibAvgImage);
	}
	else if (bClearInsp) {
		pCalibData->hResolutionYInput.Clear();
		pCalibData->hResolutionYInput = HTuple();
		//OutputDebugLogTo(9, TRUE, "Clear Calib Parm");//// for testing
	}

	return 0;
}


int CInspectionCore::GetCalibDataForInsp(CCalibrationData* pCalibData)
{
	HTuple hResolutionInputY = m_CalibrationData.dResolutionAlongYInMicronPerPixel;
	TupleConcat(pCalibData->hResolutionYInput, hResolutionInputY, &pCalibData->hResolutionYInput);

	//int nCount = pCalibData->hResolutionYInput.Length();
	//OutputDebugLogTo(9, TRUE, "GetCalibDataForInsp Count %d", nCount);//// for testing

	return 0;
}

int CInspectionCore::Initialize(BOOL bParallel)
{
	m_bParallelism = bParallel;
	m_bParallelismRegistry = bParallel;

	m_bKillInspectThreads = FALSE;
	m_bInitiateClearCache = FALSE;
	m_bClearThreadCache = FALSE;

	m_bKillInspectThreads = FALSE;

	for (int i = 0; i < THREAD_COUNT; i++) {
		m_ThreadParms[i].m_hRun = CreateEventA(NULL, FALSE, FALSE, NULL);
		m_ThreadParms[i].m_hDone = CreateEventA(NULL, FALSE, FALSE, NULL);
		m_ThreadParms[i].bPass = FALSE;
	}

	m_nThreadIndex = 0;
	return 0;
}

int CInspectionCore::Close()
{
	m_bKillInspectThreads = TRUE;
	for (int i = 0; i < THREAD_COUNT; i++)
		SetEvent(m_ThreadParms[i].m_hRun);

	return 0;
}

int CInspectionCore::InitializeInspection()
{
	for (int i = 0; i < 2; i++)
		m_arrayOverlay[i].RemoveAll();

	m_nProcessErrorLocal = 0;
	m_arrayOverlayInspection.RemoveAll();

	for (int i = 0; i < THREAD_COUNT; i++) {
		ResetEvent(m_ThreadParms[i].m_hDone);
		m_hDoneEvents[i] = NULL;
		m_ThreadParms[i].bPass = TRUE;
	}

	m_pShare = new SHARED_PARM();
	return 0;
}

int CInspectionCore::InitializeRegionInspection()
{
	m_nProcessErrorLocal = 0;

	for (int i = 0; i < THREAD_COUNT; i++) {
		ResetEvent(m_ThreadParms[i].m_hDone);
		m_hDoneEvents[i] = NULL;
	}

	m_pShare = new SHARED_PARM();
	return 0;
}

void CInspectionCore::ReleaseMemory()
{
	if (!m_bInitiateClearCache) {
		int nPendingHandles = 0;

		for (int i = 0; i < THREAD_COUNT; i++) {
			if (m_hDoneEvents[i])
				handlesDone[nPendingHandles++] = m_hDoneEvents[i];
		}

		if (nPendingHandles)
			WaitForMultipleObjects(nPendingHandles, handlesDone, TRUE, INFINITE);

		if (m_pShare) {
			delete m_pShare;
			m_pShare = NULL;
		}
	}

	if (m_bInitiateClearCache)
		ClearCache();
}

void CInspectionCore::ClearCache()
{
	CPerformanceTimer timer;
	SetSystem("temporary_mem_cache", "false");
	SetSystem("temporary_mem_cache", "true");

	m_bClearThreadCache = TRUE;

	/*for(int i=0; i<THREAD_COUNT; i++) {
		SetEvent(m_ThreadParms[i].m_hRun);
		handlesDone[i] = m_ThreadParms[i].m_hDone;
	}
	WaitForMultipleObjects(THREAD_COUNT, handlesDone, TRUE, INFINITE);*/

	m_bInitiateClearCache = FALSE;
	m_bClearThreadCache = FALSE;
	LogMessage(0, "Clear Time: %.1f", timer.GetElapsedMSec());

	//SetSystem("global_mem_cache", "cleanup");
	/**/
}



UINT InspectionThread(LPVOID pParam)
{
	try {
		CInspectionCore* pInsp = (CInspectionCore*)pParam;
		int nThreadIndex = pInsp->m_nThreadIndex++;
		THREAD_PARM* pThreadParm = (THREAD_PARM*)&pInsp->m_ThreadParms[nThreadIndex];
		SHARED_PARM *pShare = pInsp->m_pShare;
		CPerformanceTimer timer;

		while (1) {
			WaitForSingleObject(pThreadParm->m_hRun, INFINITE);
			if (pInsp->m_bKillInspectThreads)
				return 0;
			ResetEvent(pThreadParm->m_hDone);

			if (pInsp->m_bClearThreadCache) {
				SetSystem("temporary_mem_cache", "false");
				SetSystem("temporary_mem_cache", "true");
			}
			else {
				pInsp->Inspect(nThreadIndex, pThreadParm->bPass);
			}

			SetEvent(pThreadParm->m_hDone);
		}
	}
	catch (...) {
		OutputDebugLogTo(9, TRUE, "InspectionThread");
		return -1;
	}

	return 0;
}

int CInspectionCore::Inspect(HImage hImage,
	HImage hTeachImage,
	int lDebugFlag,
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
	COLORREF colorRed = RGB(255, 0, 0);
	COLORREF colorGreen = RGB(0, 255, 0);
	COLORREF colorYellow = RGB(255, 255, 0);
	COLORREF colorBlue = RGB(0, 0, 255);
	COLORREF colorOrange = RGB(255, 165, 0);
	COLORREF colorCyan = RGB(0, 255, 255);
	HImage hDebugImage;
	HRegion hDebugRegion;
	HTuple hDebugMsg;
	HTuple bPass;

	if (!bRegionInsp)
		InitializeInspection();
	else
		InitializeRegionInspection();

	lDebugFlag && DEBUG_STEP_MODE ? m_bParallelism = FALSE : m_bParallelism = m_bParallelismRegistry;

	// No Image Inspection check
	HRegion hDummyRegion;
	HTuple hDummyArea, hDummyRow, hDummyColumn;
	Threshold(hImage, &hDummyRegion, 1, 255);
	AreaCenter(hDummyRegion, &hDummyArea, &hDummyRow, &hDummyColumn);
	if (hDummyArea == 0) {
		return -NO_IMAGE;
	}

	int nStepDebug = 0;
	if (lDebugFlag && DEBUG_STEP_MODE)
		nStepDebug = 1;

	CString strOutPutLog;

	switch (nCurTrack) {
	case 0: strOutPutLog.Format("TOP Die");
		break;
	case 1: strOutPutLog.Format("THR Die");
		break;
	}

	if (!bRegionInsp) {
		HTuple hSobelAmp, hDeviceLocationEnable, hNoOfLocationRegions, hDeviceLocWidth, hDeviceLocHeight, hDeviceMaskSize, hReferencePointType;
		HTuple hMinDeviceContrast, hDatumSearchEnable, hNoOfUniqueRegions, hMaxRotationAngle;
		HTuple hPatternSearchAreaAlongX, hPatternSearchAreaAlongY, hMinAcceptanceScore, hPatternSearchType;
		HTuple hModelID, hRefRotationAngle, hUniqueNoPatternsRows, hUniqueNoPatternsCols;
		HTuple hLocationTeachRows, hLocationTeachCols;
		HTuple hRefPointRow, hRefPointCol;
		HTuple hMatchModelCenterRow, hMatchModelCenterCol;

		HTuple hEdgeRefPointRow, hEdgeRefPointCol, hEdgeRefRotationAngle;

		HTuple hDeviceEdgeDetectionEnable, hDeviceHorizontalContrast, hDeviceHorizontalDirection, hDeviceVerticalContrast, hDeviceVerticalDirection;
		HTuple hDeviceEdgeLocationTeachRows, hDeviceEdgeLocationTeachCols;

		int nIndex = 0;
		hDeviceLocationEnable = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hNoOfLocationRegions = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hDeviceLocWidth = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hDeviceLocHeight = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hDeviceMaskSize = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hMinDeviceContrast = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		//hSobelAmp = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hSobelAmp = m_TrainingData.bSobelAmp;
		hReferencePointType = m_TrainingData.nDeviceReferencePointType;

		hDatumSearchEnable = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hNoOfUniqueRegions = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hMaxRotationAngle = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hPatternSearchAreaAlongX = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hPatternSearchAreaAlongY = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hMinAcceptanceScore = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);

		hUniqueNoPatternsRows = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hUniqueNoPatternsCols = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hLocationTeachRows = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hLocationTeachCols = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);

		hModelID = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hRefRotationAngle = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hRefPointRow = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hRefPointCol = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);

		hEdgeRefRotationAngle = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hEdgeRefPointRow = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hEdgeRefPointCol = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);

		//For Device Edge Detection option
		hDeviceEdgeDetectionEnable = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hDeviceHorizontalContrast = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hDeviceHorizontalDirection = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hDeviceVerticalContrast = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hDeviceVerticalDirection = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);

		hDeviceEdgeLocationTeachRows = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);
		hDeviceEdgeLocationTeachCols = m_TrainingData.hvec_TupleVectorLocation.GetAt(nIndex++);

		if (hDeviceLocationEnable == 0 && hDatumSearchEnable == 0)
		{
			HRegion hEmptyRegion;
			GenEmptyRegion(&hEmptyRegion);
			ConcatObj(pTrainingData->hObjectDeviceLocation, hEmptyRegion, &pTrainingData->hObjectDeviceLocation);
			TupleConcat(pTrainingData->hTupleProjectionParam, (HTuple(0).TupleConcat(0)).TupleConcat(0), &pTrainingData->hTupleProjectionParam);
			TupleConcat(pTrainingData->hTupleDatumCenterParam, (HTuple(0).TupleConcat(0)).TupleConcat(0), &pTrainingData->hTupleDatumCenterParam);
			TupleConcat(pTrainingData->hTupleTeachDoc, 0, &pTrainingData->hTupleTeachDoc);
			TupleConcat(pTrainingData->hTupleDatumTeachTuple, 0, &pTrainingData->hTupleDatumTeachTuple);
			TupleConcat(pTrainingData->hTupleEdgeTeachTuple, 0, &pTrainingData->hTupleEdgeTeachTuple);
			return 0;
		}

		HRegion hDieLocation, hFindModelRegion;
		HTuple hIsPass, hIsFindDatam, hMatchModelScore, hAngleOfDeviation;

		GenEmptyRegion(&hDieLocation);
		hAngleOfDeviation = 0;//HTuple();

		HTuple hTopRefGotRotationAngle, hTopRefGotShiftAlongRow, hTopRefGotShiftAlongCol;
		hPatternSearchType = m_TrainingData.nDatumPatternType;

		_FCI_Inspect_DeviceLocation(hImage,
			&hDieLocation, &hFindModelRegion,
			&hDebugImage, &hDebugRegion, hDatumSearchEnable, hDeviceLocationEnable, nStepDebug,
			hSobelAmp, hNoOfLocationRegions,
			hDeviceLocWidth, hDeviceLocHeight, hMinDeviceContrast,
			hNoOfUniqueRegions, hMaxRotationAngle,
			hPatternSearchAreaAlongX, hPatternSearchAreaAlongY, hMinAcceptanceScore, hPatternSearchType,
			hModelID, hRefRotationAngle,
			hRefPointRow, hRefPointCol,
			hEdgeRefRotationAngle, hEdgeRefPointRow, hEdgeRefPointCol,
			hUniqueNoPatternsRows, hUniqueNoPatternsCols,
			hLocationTeachRows, hLocationTeachCols, hDeviceMaskSize, nCurFOV, nTotalFOV, nCurTrack + 1, hReferencePointType,
			hDeviceEdgeDetectionEnable,
			hDeviceHorizontalContrast, hDeviceHorizontalDirection,
			hDeviceVerticalContrast, hDeviceVerticalDirection,
			hDeviceEdgeLocationTeachRows, hDeviceEdgeLocationTeachCols,
			&hIsPass, &hIsFindDatam,
			&hAngleOfDeviation, &hMatchModelCenterRow, &hMatchModelCenterCol,
			&hMatchModelScore,
			&hTopRefGotRotationAngle, &hTopRefGotShiftAlongRow, &hTopRefGotShiftAlongCol,
			&hDebugMsg);


		CString str;
		double dMatchModelScore = (hMatchModelScore.D() * 100);

		str.Format("Position[%d] %s Datum Location Score [%3.2f %%]", nCurFOV, strOutPutLog, dMatchModelScore);
		strArrayInspValues.Add(str);

		HTuple hDatumCenterAngleInspect, hDatumeCenterRowInspect, hDatumCenterColInspect;
		hDatumCenterAngleInspect = hRefRotationAngle - hAngleOfDeviation;
		hDatumeCenterRowInspect = hRefPointRow - hMatchModelCenterRow;
		hDatumCenterColInspect = hRefPointCol - hMatchModelCenterCol;

		ConcatObj(pTrainingData->hObjectDeviceLocation, hDieLocation, &pTrainingData->hObjectDeviceLocation);
		TupleConcat(pTrainingData->hTupleProjectionParam, (hAngleOfDeviation.TupleConcat(hMatchModelCenterRow)).TupleConcat(hMatchModelCenterCol), &pTrainingData->hTupleProjectionParam);
		TupleConcat(pTrainingData->hTupleDatumCenterParam, (hDatumCenterAngleInspect.TupleConcat(hDatumeCenterRowInspect)).TupleConcat(hDatumCenterColInspect), &pTrainingData->hTupleDatumCenterParam);
		TupleConcat(pTrainingData->hTupleTeachDoc, 1, &pTrainingData->hTupleTeachDoc);
		TupleConcat(pTrainingData->hTupleDatumTeachTuple, hDatumSearchEnable, &pTrainingData->hTupleDatumTeachTuple);

		if (hDatumSearchEnable == 1)
			TupleConcat(pTrainingData->hTupleEdgeTeachTuple, hIsFindDatam/*hDeviceLocationEnable*/, &pTrainingData->hTupleEdgeTeachTuple);
		else
			TupleConcat(pTrainingData->hTupleEdgeTeachTuple, hIsPass/*hDeviceLocationEnable*/, &pTrainingData->hTupleEdgeTeachTuple);


		if (hIsPass == TRUE && hDeviceLocationEnable == TRUE) {
			//Show the top reference point inspected
			HObject hCross, hCrossRegion, hDebugCrossRegion;
			HTuple hCrossRow, hCrossCol;
			GenCrossContourXld(&hCross, (hEdgeRefPointRow - hTopRefGotShiftAlongRow), (hEdgeRefPointCol - hTopRefGotShiftAlongCol), 200, 0);
			GetContourXld(hCross, &hCrossRow, &hCrossCol);
			GenRegionPolygon(&hCrossRegion, hCrossRow, hCrossCol);

			m_arrayOverlayInspection.Add(hCrossRegion, colorYellow, "Top Reference Point Found By Inspect Edge Device Location");
		}

		if (hIsFindDatam == TRUE)
			m_arrayOverlayInspection.Add(hFindModelRegion, colorGreen);

		if (nStepDebug) {
			StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
			OutputDebugLogTo(5, TRUE, "Top Ref Shifted: (%.2f, %.2f) - Datum Ref Shifted: (%.2f, %.2f) (pixel)", hTopRefGotShiftAlongCol.D(), hTopRefGotShiftAlongRow.D(), hMatchModelCenterCol.D(), hMatchModelCenterRow.D());
		}
	} // End of Device Location
	else if (bRegionInsp)
	{
		//Passing shared param to thread param incase using multi thread for each inspection criteria
		m_pShare->hImageThread = hImage;
		m_pShare->hTeachImageThread = hTeachImage;
		m_pShare->nCurTrackThread = nCurTrack;
		m_pShare->nCurDocThread = nCurDoc;
		m_pShare->nTotalDocThread = nTotalDoc;
		m_pShare->nCurFOVThread = nCurFOV;
		m_pShare->nTotalFOVThread = nTotalFOV;
		m_pShare->lDebugFlagThread = lDebugFlag;
		m_pShare->bRegionInspThread = bRegionInsp;
		m_pShare->pCalibDataThread = pCalibData;
		m_pShare->nStepDebug = nStepDebug;

		//// Find location params
		HRegion hDeviceLocationSelected;
		HTuple hIsSelected, hInspectRotationAngle, hInspectShiftAlongRow, hInspectShiftAlongCol;
		HTuple hCount, hArea, hDeviceCenterRow, hDeviceCenterCol;
		HTuple hDatumCenterRotationAngle, hDatumCenterRow, hDatumCenterCol;

		_FCI_Extract_LocationParameterData(pTrainingData->hTupleProjectionParam, pTrainingData->hTupleTeachDoc,
			pTrainingData->hTupleEdgeTeachTuple, pTrainingData->hTupleDatumTeachTuple, nCurDoc,
			&hIsSelected, &hInspectRotationAngle, &hInspectShiftAlongRow, &hInspectShiftAlongCol);

		m_pShare->hInspectRotationAngleThread = hInspectRotationAngle;
		m_pShare->hInspectShiftAlongRowThread = hInspectShiftAlongRow;
		m_pShare->hInspectShiftAlongColThread = hInspectShiftAlongCol;

		//Update shifted information to pass to the THR scanning
		m_InspectionData.hInspectShiftAlongXMicron = hInspectShiftAlongCol * ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
		m_InspectionData.hInspectShiftAlongYMicron = hInspectShiftAlongRow * ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);

		if (0 != (hIsSelected == 0)) {
			return -DEVICE_LOCATION;
		}

		_FCI_Extract_LocationParameterData(pTrainingData->hTupleDatumCenterParam, pTrainingData->hTupleTeachDoc,
			pTrainingData->hTupleEdgeTeachTuple, pTrainingData->hTupleDatumTeachTuple, nCurDoc,
			&hIsSelected, &hDatumCenterRotationAngle, &hDatumCenterRow, &hDatumCenterCol);


		HTuple hConcatArea, hRow2, hColumn2, hMaxArea = 0;
		AreaCenter(pTrainingData->hObjectDeviceLocation, &hConcatArea, &hRow2, &hColumn2);
		if (hConcatArea.TupleLength() > 0)
			TupleMax(hConcatArea, &hMaxArea);
		if (0 != (hMaxArea == 0)) {
			return -DEVICE_LOCATION;
		}

		CountObj(pTrainingData->hObjectDeviceLocation, &hCount);
		_FCI_Select_DeviceLocation(pTrainingData->hObjectDeviceLocation,
			&hDeviceLocationSelected,
			nCurDoc, &hDeviceCenterRow, &hDeviceCenterCol);
		CountObj(hDeviceLocationSelected, &hCount);
		if (hCount < 1)
			return 0;

		m_pShare->hDeviceLocationSelectedThread = hDeviceLocationSelected;

		//// ********** Encap Insp ********** ////
		HTuple hImgIndex = -1;
		if (m_TrainingData.bEnableEncap) {
		
			// // Encap Surface Mask 
			HRegion hConcatEncapMaskRgn;
			HTuple hEncapPolygonPoints;
			GenEmptyObj(&hConcatEncapMaskRgn);
			for (int nMask = 0; nMask < m_TrainingData.nEncapTracingMaskNumber; nMask++) {
				HRegion hEncapTracingMask;
				hEncapPolygonPoints = HTuple();
				HTuple hPoint;
				HTuple hRows, hCols;
				for (int nPointId = 0; nPointId < m_TrainingData.nEncapTracingMaskPolygonPointNumber[nMask]; nPointId++) {
					TupleConcat(hRows, m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMask][nPointId].y, &hRows);
					TupleConcat(hCols, m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMask][nPointId].x, &hCols);
					hPoint.Clear();
					hPoint.Append(m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMask][nPointId].y);
					hPoint.Append(m_TrainingData.m_EncapTracingMaskPolygonPointArea[nMask][nPointId].x);
					GenRegionPolygonFilled(&hEncapTracingMask, hRows, hCols);
				}
				ConcatObj(hConcatEncapMaskRgn, hEncapTracingMask, &hConcatEncapMaskRgn);
			}
			Union1(hConcatEncapMaskRgn, &hConcatEncapMaskRgn);
			
			// Blue Encap Inspection
			if (m_EncapParm.bEnableBlueEncap) {
				if (!m_TrainingData.bEnableEncapBlue)
					return -ENCAP_INSUFFICIENT;
				//Encap Tracing Region
				// // Teach Region for Tracing Encap
				HTuple hPointEncapRgn;
				HTuple hRowsEncapRgn, hColsEncapRgn;
				HRegion hEncapRgnTemp;

				for (int nPointId = 0; nPointId < m_TrainingData.nEncapTracingPolygonPointNumber; nPointId++) {
					TupleConcat(hRowsEncapRgn, m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].y, &hRowsEncapRgn);
					TupleConcat(hColsEncapRgn, m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].x, &hColsEncapRgn);
					hPointEncapRgn.Clear();
					hPointEncapRgn.Append(m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].y);
					hPointEncapRgn.Append(m_TrainingData.m_EncapTracingPolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hEncapRgnTemp, hRowsEncapRgn, hColsEncapRgn);

				HTuple hPointEncapTop;
				HTuple hRowsEncapTop, hColsEncapTop;
				HRegion hEncapRgnTopTemp;
				// // Region Encap Top 
				for (int nPointId = 0; nPointId < m_TrainingData.nEncapTopPolygonPointNumber; nPointId++) {
					TupleConcat(hRowsEncapTop, m_TrainingData.m_EncapTopPolygonPointArea[nPointId].y, &hRowsEncapTop);
					TupleConcat(hColsEncapTop, m_TrainingData.m_EncapTopPolygonPointArea[nPointId].x, &hColsEncapTop);
					hPointEncapTop.Clear();
					hPointEncapTop.Append(m_TrainingData.m_EncapTopPolygonPointArea[nPointId].y);
					hPointEncapTop.Append(m_TrainingData.m_EncapTopPolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hEncapRgnTopTemp, hRowsEncapTop, hColsEncapTop);

				// // Region Encap Below

				HRegion hEncapRgnBottomTemp;
				HTuple hPointEncapBottom;
				HTuple hRowsEncapBottom, hColsEncapBottom;
				for (int nPointId = 0; nPointId < m_TrainingData.nEncapBottomPolygonPointNumber; nPointId++) {
					TupleConcat(hRowsEncapBottom, m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].y, &hRowsEncapBottom);
					TupleConcat(hColsEncapBottom, m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].x, &hColsEncapBottom);
					hPointEncapBottom.Clear();
					hPointEncapBottom.Append(m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].y);
					hPointEncapBottom.Append(m_TrainingData.m_EncapBottomPolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hEncapRgnBottomTemp, hRowsEncapBottom, hColsEncapBottom);

				HTuple hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2;
				// // Region Find Left Line Flex for Generating Masking Encap 
				HRegion hEncapRgnMaskingLeftTemp;
				hEncapRow1 = m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea.top;
				hEncapCol1 = m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea.left;
				hEncapRow2 = m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea.bottom;
				hEncapCol2 = m_TrainingData.m_rectEncapMaskingLeftPolygonPointArea.right;
				GenRectangle1(&hEncapRgnMaskingLeftTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);

				// //Region Find Right Line Flex for Generating Masking Encap
				HRegion hEncapRgnMaskingRightTemp;
				hEncapRow1 = m_TrainingData.m_rectEncapMaskingRightPolygonPointArea.top;
				hEncapCol1 = m_TrainingData.m_rectEncapMaskingRightPolygonPointArea.left;
				hEncapRow2 = m_TrainingData.m_rectEncapMaskingRightPolygonPointArea.bottom;
				hEncapCol2 = m_TrainingData.m_rectEncapMaskingRightPolygonPointArea.right;
				GenRectangle1(&hEncapRgnMaskingRightTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);

				// //  Region Encap Masking Limit 
				HRegion hEncapRgnMaskingLimitTemp;
				hEncapRow1 = m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea.top;
				hEncapCol1 = m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea.left;
				hEncapRow2 = m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea.bottom;
				hEncapCol2 = m_TrainingData.m_rectEncapMaskingLimitPolygonPointArea.right;
				GenRectangle1(&hEncapRgnMaskingLimitTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
				// // // Insuficient
				// // Left Encap Region For Checking Insufficient
				HRegion hBlueEncapInsuffLeftRegionTemp;
				hEncapRow1 = m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea.top;
				hEncapCol1 = m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea.left;
				hEncapRow2 = m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea.bottom;
				hEncapCol2 = m_TrainingData.m_rectBlueEncapInsuffLeftRegionPolygonPointArea.right;
				GenRectangle1(&hBlueEncapInsuffLeftRegionTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);

				// // Right Encap Region For Checking Insufficient
				HRegion hBlueEncapInsuffRightRegionTemp;
				hEncapRow1 = m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea.top;
				hEncapCol1 = m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea.left;
				hEncapRow2 = m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea.bottom;
				hEncapCol2 = m_TrainingData.m_rectBlueEncapInsuffRightRegionPolygonPointArea.right;
				GenRectangle1(&hBlueEncapInsuffRightRegionTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);

				// // Region For Find border of the base
				HRegion hBlueEncapInsuffBoderBaseRegionTemp;
				hEncapRow1 = m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea.top;
				hEncapCol1 = m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea.left;
				hEncapRow2 = m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea.bottom;
				hEncapCol2 = m_TrainingData.m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea.right;
				GenRectangle1(&hBlueEncapInsuffBoderBaseRegionTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);

				// // Middle Encap Region For Checking Insufficient
				HRegion hBlueEncapInsuffMidRegionTemp;
				HTuple hRowsBlueEncapInsuffMidRegion;
				HTuple hColsBlueEncapInsuffMidRegion;
				HTuple hPointBlueEncapInsuffMidRegion;
				for (int nPointId = 0; nPointId < m_TrainingData.nBlueEncapInsuffMidRegionPolygonPointNumber; nPointId++) {
					TupleConcat(hRowsBlueEncapInsuffMidRegion, m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].y, &hRowsBlueEncapInsuffMidRegion);
					TupleConcat(hColsBlueEncapInsuffMidRegion, m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].x, &hColsBlueEncapInsuffMidRegion);
					hPointBlueEncapInsuffMidRegion.Clear();
					hPointBlueEncapInsuffMidRegion.Append(m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].y);
					hPointBlueEncapInsuffMidRegion.Append(m_TrainingData.m_BlueEncapInsuffMidRegionPolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hBlueEncapInsuffMidRegionTemp, hRowsBlueEncapInsuffMidRegion, hColsBlueEncapInsuffMidRegion);

				// // // // Excess
				// // Left Encap Region For Checking Excess
				HRegion hBlueEncapExcessLeftRegionTemp;
				hEncapRow1 = m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea.top;
				hEncapCol1 = m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea.left;
				hEncapRow2 = m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea.bottom;
				hEncapCol2 = m_TrainingData.m_rectBlueEncapExcessLeftRegionPolygonPointArea.right;
				GenRectangle1(&hBlueEncapExcessLeftRegionTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
				// // Right Encap Region For Checking Excess
				HRegion hBlueEncapExcessRightRegionTemp;
				hEncapRow1 = m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea.top;
				hEncapCol1 = m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea.left;
				hEncapRow2 = m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea.bottom;
				hEncapCol2 = m_TrainingData.m_rectBlueEncapExcessRightRegionPolygonPointArea.right;
				GenRectangle1(&hBlueEncapExcessRightRegionTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);

				// // // // Blue Encap Wire And IC
				// Region for Inspect Wire
				HTuple hPointEncapWire;
				HTuple hRowsEncapWire, hColsEncapWire;
				HRegion hEncapWireTemp;

				for (int nPointId = 0; nPointId < m_TrainingData.nEncapWirePolygonPointNumber; nPointId++) {
					TupleConcat(hRowsEncapWire, m_TrainingData.m_EncapWirePolygonPointArea[nPointId].y, &hRowsEncapWire);
					TupleConcat(hColsEncapWire, m_TrainingData.m_EncapWirePolygonPointArea[nPointId].x, &hColsEncapWire);
					hPointEncapWire.Clear();
					hPointEncapWire.Append(m_TrainingData.m_EncapWirePolygonPointArea[nPointId].y);
					hPointEncapWire.Append(m_TrainingData.m_EncapWirePolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hEncapWireTemp, hRowsEncapWire, hColsEncapWire);

				// Region for Inspect Wire (Bottom)
				HTuple hPointEncapWireBottom;
				HTuple hRowsEncapWireBottom, hColsEncapWireBottom;
				HRegion hEncapWireBottomTemp;

				for (int nPointId = 0; nPointId < m_TrainingData.nEncapWireBottomPolygonPointNumber; nPointId++) {
					TupleConcat(hRowsEncapWireBottom, m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].y, &hRowsEncapWireBottom);
					TupleConcat(hColsEncapWireBottom, m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].x, &hColsEncapWireBottom);
					hPointEncapWireBottom.Clear();
					hPointEncapWireBottom.Append(m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].y);
					hPointEncapWireBottom.Append(m_TrainingData.m_EncapWireBottomPolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hEncapWireBottomTemp, hRowsEncapWireBottom, hColsEncapWireBottom);

				// Region for Inspect IC
				HTuple hPointEncapIC;
				HTuple hRowsEncapIC, hColsEncapIC;
				HRegion hEncapICTemp;

				for (int nPointId = 0; nPointId < m_TrainingData.nEncapICPolygonPointNumber; nPointId++) {
					TupleConcat(hRowsEncapIC, m_TrainingData.m_EncapICPolygonPointArea[nPointId].y, &hRowsEncapIC);
					TupleConcat(hColsEncapIC, m_TrainingData.m_EncapICPolygonPointArea[nPointId].x, &hColsEncapIC);
					hPointEncapIC.Clear();
					hPointEncapIC.Append(m_TrainingData.m_EncapICPolygonPointArea[nPointId].y);
					hPointEncapIC.Append(m_TrainingData.m_EncapICPolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hEncapICTemp, hRowsEncapIC, hColsEncapIC);

				// Region Projection
					//	//Blue Encap Tracing
				HTuple hDebugMsgOutEncap, hIsPass;
				HImage hDebugImageOutEncap;
				HRegion hDebugRegionOutEncap;
				HRegion hEncapRgn;
				HRegion hEncapRgnAbove;
				HRegion hEncapRgnBelow;
				HRegion hEncapRegionInspected;
				HRegion hEncapRgnMaskingLeft;
				HRegion hEncapRgnMaskingRight, hEncapRgnMaskingLimit;
				HRegion hEncapRgnMaskingSurface;

				HRegion hBlueEncapInsuffBoderBaseRegion;
				HRegion hBlueEncapInsuffRightRegion;
				HRegion hBlueEncapInsuffLeftRegion;
				HRegion hBlueEncapInsuffMidRegion;
				HRegion hBlueEncapExcessLeftRegion;
				HRegion hBlueEncapExcessRightRegion;

				GenEmptyObj(&hEncapRgnMaskingSurface);
				HTuple hHomMat2DIdentityEncapVoid;
				HomMat2dIdentity(&hHomMat2DIdentityEncapVoid);
				HomMat2dRotate(hHomMat2DIdentityEncapVoid, -hInspectRotationAngle, hInspectShiftAlongRow + hDatumCenterRow, hInspectShiftAlongCol + hDatumCenterCol, &hHomMat2DIdentityEncapVoid);
				HomMat2dTranslate(hHomMat2DIdentityEncapVoid, -hInspectShiftAlongRow, -hInspectShiftAlongCol, &hHomMat2DIdentityEncapVoid);
				AffineTransRegion(hEncapRgnTemp, &hEncapRgn, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hEncapRgnTopTemp, &hEncapRgnAbove, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hEncapRgnBottomTemp, &hEncapRgnBelow, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hEncapRgnMaskingLeftTemp, &hEncapRgnMaskingLeft, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hEncapRgnMaskingRightTemp, &hEncapRgnMaskingRight, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hEncapRgnMaskingLimitTemp, &hEncapRgnMaskingLimit, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hConcatEncapMaskRgn, &hEncapRgnMaskingSurface, hHomMat2DIdentityEncapVoid, "nearest_neighbor");

				AffineTransRegion(hBlueEncapInsuffBoderBaseRegionTemp, &hBlueEncapInsuffBoderBaseRegion, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hBlueEncapInsuffRightRegionTemp, &hBlueEncapInsuffRightRegion, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hBlueEncapInsuffLeftRegionTemp, &hBlueEncapInsuffLeftRegion, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hBlueEncapInsuffMidRegionTemp, &hBlueEncapInsuffMidRegion, hHomMat2DIdentityEncapVoid, "nearest_neighbor");

				AffineTransRegion(hBlueEncapExcessLeftRegionTemp, &hBlueEncapExcessLeftRegion, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hBlueEncapExcessRightRegionTemp, &hBlueEncapExcessRightRegion, hHomMat2DIdentityEncapVoid, "nearest_neighbor");

				if (nStepDebug) {
					HTuple hMsg = "Region Projection [Tracing]";
					StepDebug(hImage, hEncapRgn, colorGreen, hMsg, bRegionInsp);
					StepDebug(hImage, hEncapRgnAbove, colorYellow, hMsg, bRegionInsp);
					StepDebug(hImage, hEncapRgnBelow, colorYellow, hMsg, bRegionInsp);
					StepDebug(hImage, hEncapRgnMaskingLeft, colorYellow, hMsg, bRegionInsp);
					StepDebug(hImage, hEncapRgnMaskingRight, colorYellow, hMsg, bRegionInsp);
					StepDebug(hImage, hEncapRgnMaskingLimit, colorYellow, hMsg, bRegionInsp);
					StepDebug(hImage, hEncapRgnMaskingSurface, colorYellow, hMsg, bRegionInsp);
				}
				// // // // Find Border Of Base
				HTuple hIntensityForFindBorderBaseBlueEncap = m_TrainingData.nIntensityForFindBorderBaseBlueEncap;
				HTuple hOffsetLeftBlueEncap = m_TrainingData.dOffsetLeftBlueEncap / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hOffsetRightBlueEncap = m_TrainingData.dOffsetRightBlueEncap / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				_HP_BlueEncap_BorderOfBase(hImage, hBlueEncapInsuffBoderBaseRegion, hEncapRgn,&hDebugImageOutEncap, &hDebugRegionOutEncap, &hEncapRgn, nStepDebug, hIntensityForFindBorderBaseBlueEncap, hOffsetLeftBlueEncap, hOffsetRightBlueEncap,&hDebugMsgOutEncap);

				if (nStepDebug) {
					StepDebug(hDebugImageOutEncap, hDebugRegionOutEncap, colorCyan, hDebugMsgOutEncap, bRegionInsp);
				}

				// // // // Blue Encap Tracing 
				HTuple hErosionRadiusEncapTracing, hSmoothingIndexEncapTracing, hIntensityEncapTracing, hSlideWindowSideEncapTracing;

				hErosionRadiusEncapTracing = m_TrainingData.dErosionRadiusEncapTracing / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				hSmoothingIndexEncapTracing = m_TrainingData.nSmoothingIndexEncapTracing;
				hIntensityEncapTracing = m_TrainingData.nIntensityEncapTracing;
				hSlideWindowSideEncapTracing = m_TrainingData.dSlideWindowSideEncapTracing / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hEdgeFlexContrastEncapTracing = m_TrainingData.nEdgeFlexContrastEncapTracing;
				HTuple hSizeMaskingEncapTracing = m_TrainingData.dSizeMaskingEncapTracing / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HRegion EncapWithOutMask;
				_HP_BlueEncap_Tracing(hImage, hEncapRgn, hEncapRgnAbove, hEncapRgnBelow, hEncapRgnMaskingLeft, hEncapRgnMaskingRight, hEncapRgnMaskingLimit, hEncapRgnMaskingSurface, &hDebugImageOutEncap, &hDebugRegionOutEncap, &hEncapRegionInspected,&EncapWithOutMask, hIntensityEncapTracing, hSlideWindowSideEncapTracing, hEdgeFlexContrastEncapTracing, hSizeMaskingEncapTracing, hErosionRadiusEncapTracing, nStepDebug, &hDebugMsgOutEncap, &hIsPass);

				m_arrayOverlayInspection.Add(hEncapRegionInspected, colorGreen);

				if (nStepDebug) {
					StepDebug(hDebugImageOutEncap, hDebugRegionOutEncap, colorCyan, hDebugMsgOutEncap, bRegionInsp);
				}
				// // // // Insufficient

				HTuple hMinInSuffPercentageRightBlueEncap = m_EncapParm.dMinInSuffPercentageRightBlueEncap;
				HTuple hMinInSuffPercentageLeftBlueEncap = m_EncapParm.dMinInSuffPercentageLeftBlueEncap;
				HTuple hMinSizeMidBlueEncap = m_EncapParm.dMinSizeMidBlueEncap / (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));
				HTuple hDebugMsgOutBlueEncapInsuff;
				HImage hDebugImageOutBlueEncapInsuff;
				HRegion hDebugRegionOutBlueEncapInsuff;

				HRegion hInsuffBlueEncap;
				HRegion	hRegionTransBlueEncap;
				_HP_Inspect_BlueEncap_Insufficient(hImage, hBlueEncapInsuffLeftRegion, hBlueEncapInsuffRightRegion, hBlueEncapInsuffMidRegion,	  EncapWithOutMask, hEncapRgn,
					&hDebugImageOutBlueEncapInsuff, &hDebugRegionOutBlueEncapInsuff, &hInsuffBlueEncap,
					hMinInSuffPercentageLeftBlueEncap, hMinInSuffPercentageRightBlueEncap,hMinSizeMidBlueEncap, nStepDebug, &hDebugMsgOutBlueEncapInsuff, &hIsPass);
				if (nStepDebug) {
					StepDebug(hDebugImageOutBlueEncapInsuff, hDebugRegionOutBlueEncapInsuff, colorRed, hDebugMsgOutBlueEncapInsuff, bRegionInsp);
				}
				m_arrayOverlayInspection.Add(hRegionTransBlueEncap, colorYellow);
				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hInsuffBlueEncap, colorRed);
					return -ENCAP_INSUFFICIENT;
				}
				// // // // Excess
				HTuple hContrastExcessBlueEncap = m_EncapParm.nContrastExcessBlueEncap;
				HTuple hCircleContrastExcessBlueEncap = m_EncapParm.nCircleContrastExcessBlueEncap;
				HTuple hEdgeOffsetExcessBlueEncap = m_EncapParm.dEdgeOffsetExcessBlueEncap / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hMinSizeExcessBlueEncap = m_EncapParm.dMinSizeExcessBlueEncap / (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));
				HTuple hDebugMsgOutBlueEncapExcess;
				HImage hDebugImageOutBlueEncapExcess;
				HRegion hDebugRegionOutBlueEncapExcess;
				HRegion hExcessRegionBlueEncap;

				_HP_Inspect_BlueEncap_Excess(hImage, hBlueEncapExcessLeftRegion, hBlueEncapExcessRightRegion, &hDebugImageOutBlueEncapExcess, &hDebugRegionOutBlueEncapExcess,
					&hExcessRegionBlueEncap, hEdgeOffsetExcessBlueEncap, hCircleContrastExcessBlueEncap, hContrastExcessBlueEncap, hMinSizeExcessBlueEncap,
					nStepDebug, &hDebugMsgOutBlueEncapExcess, &hIsPass);

				if (nStepDebug) {
					StepDebug(hDebugImageOutBlueEncapExcess, hDebugRegionOutBlueEncapExcess, colorRed, hDebugMsgOutBlueEncapExcess, bRegionInsp);
				}
				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hExcessRegionBlueEncap, colorRed);
					return -ENCAP_EXCESS;
				}
				// Glue - Mason
				HObject hBEncapGlueMask;
				AffineTransRegion(m_TrainingData.hBEncapGlueMask, &hBEncapGlueMask, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hBEncapGlueMask, colorGreen, "Mask Using To Find Glue Defect Region [Glue]", bRegionInsp);
				}

				double dResolution = ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hBEncapMaskOffsetGlue = m_EncapParm.dBEncapMaskOffsetGlue / dResolution;
				HTuple hBEncapRegionOffsetGlue = m_EncapParm.dBEncapRegionOffsetGlue / dResolution;
				HTuple hBSmoothMaskSizeGlue = m_EncapParm.dBSmoothMaskSizeGlue / dResolution;
				HTuple hBMaskSizeGlue = m_EncapParm.dBMaskSizeGlue / dResolution;
				HTuple hBStandardDeviationGlue = m_EncapParm.dBStandardDeviationGlue;
				HTuple hBDiffMeanGrayGlue = m_EncapParm.nBDiffMeanGrayGlue;
				HTuple hBMinMeanGrayGlue = m_EncapParm.nBMinMeanGrayGlue;
				HTuple hBMinSizeGlue = m_EncapParm.dBMinSizeGlue / (dResolution*dResolution);
				HTuple hBMinConvexityGlue = m_EncapParm.dBMinConvexityGlue;
				HTuple hBMaxInnerAreaPercentGlue = m_EncapParm.dBMaxInnerAreaPercentGlue;
				HTuple hBMaxFuzzySizeGlue = m_EncapParm.dBMaxFuzzySizeGlue / (dResolution*dResolution);
				HTuple hBMaxDiffMeanGrayGlue = m_EncapParm.nBMaxDiffMeanGrayGlue;

				HObject hBEncapGlueDefectRegion;
				_HP_Inspect_BlueEncap_Glue(hImage,
					hEncapRegionInspected,
					hEncapRgn,
					hBEncapGlueMask,
					&hDebugImage,
					&hDebugRegion,
					&hBEncapGlueDefectRegion,
					nStepDebug,
					hBEncapMaskOffsetGlue,
					hBEncapRegionOffsetGlue,
					hBSmoothMaskSizeGlue,
					hBMaskSizeGlue,
					hBSmoothMaskSizeGlue,
					hBDiffMeanGrayGlue,
					hBMinMeanGrayGlue,
					hBMinSizeGlue,
					hBMinConvexityGlue,
					hBMaxInnerAreaPercentGlue,
					hBMaxFuzzySizeGlue,
					hBMaxDiffMeanGrayGlue,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hBEncapGlueDefectRegion, colorRed, "Glue Defect Region");
					return -ENCAP_GLUE;
				}

				// Contamination - Mason
				HObject hBEncapFlexEdgeMaskROI;
				AffineTransRegion(m_TrainingData.hBEncapFlexEdgeMaskROI, &hBEncapFlexEdgeMaskROI, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hBEncapFlexEdgeMaskROI, colorGreen, "Mask Using To Find Contamination Defect Region Between Flex And Encap [Contamination]", bRegionInsp);
				}

				HObject hBEncapDieMaskCT;
				GenEmptyObj(&hBEncapDieMaskCT);

				HTuple hBMaskSizeCT = m_EncapParm.dBMaskSizeCT / dResolution;
				HTuple hBStandardDeviationCT = m_EncapParm.dBStandardDeviationCT;
				HTuple hBDiffMeanGrayCT = m_EncapParm.nBDiffMeanGrayCT;
				HTuple hBBottomMaskContrastCT = m_EncapParm.nBBottomMaskContrastCT;
				HTuple hBFlexMaskContrastCT = m_EncapParm.nBFlexMaskContrastCT;
				HTuple hBMinGrayCT = m_EncapParm.nBMinGrayCT;
				HTuple hBMinSizeCT = m_EncapParm.dBMinSizeCT / (dResolution*dResolution);
				HTuple hBMinDiffMeanGrayCT = m_EncapParm.nBMinDiffMeanGrayCT;
				HTuple hBMinMeanEdgeMaskGrayCT = m_EncapParm.nBMinMeanEdgeMaskGrayCT;
				HTuple hBMinMeanFlexMaskGrayCT = m_EncapParm.nBMinMeanFlexMaskGrayCT;
				HTuple hBMinMeanBorderedGrayCT = m_EncapParm.nBMinMeanBorderedGrayCT;
				HTuple hBMaxDiffAreaPercentCT = m_EncapParm.dBMaxDiffAreaPercentCT;
				HTuple hBMinDefectCountCT = m_EncapParm.nBMinDefectCountCT;

				HObject hBEncapContaminationDefectRegion;
				_HP_Inspect_BlueEncap_Contamination(hImage,
					hEncapRegionInspected,
					hEncapRgnBelow,
					hBEncapFlexEdgeMaskROI,
					hBEncapDieMaskCT,
					&hDebugImage,
					&hDebugRegion,
					&hBEncapContaminationDefectRegion,
					nStepDebug,
					hBMaskSizeCT,
					hBStandardDeviationCT,
					hBDiffMeanGrayCT,
					hBBottomMaskContrastCT,
					hBFlexMaskContrastCT,
					hBMinGrayCT,
					hBMinSizeCT,
					hBMinDiffMeanGrayCT,
					hBMinMeanEdgeMaskGrayCT,
					hBMinMeanFlexMaskGrayCT,
					hBMinMeanBorderedGrayCT,
					hBMaxDiffAreaPercentCT,
					hBMinDefectCountCT,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hBEncapContaminationDefectRegion, colorRed, "Contamination Defect Region");
					return -ENCAP_CONTAMINATION;
				}

				// Blue Encap Void
				HTuple hDebugMsgOutEncapVoid;
				HImage hDebugImageOutEncapVoid;
				HRegion hDebugRegionOutEncapVoid;
				HRegion hRegionVoidInspect;
				HRegion hVoidDefectRegion;
				HTuple hEncapVoidMinMaskSize = m_EncapParm.dBlueEncapVoidMinMaskSize / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hEncapVoidMinDefectContrast = m_EncapParm.nBlueEncapVoidMinDefectContrast;
				HTuple hEncapVoidMinSize = m_EncapParm.dBlueEncapVoidMinSize / (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));
				HTuple hEncapVoidMinCircularity = m_EncapParm.dBlueEncapVoidMinCircularity;
				HTuple hEncapVoidMinLength = m_EncapParm.dBlueEncapVoidMinLength / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hEncapVoidMinSquareSize = m_EncapParm.dBlueEncapVoidMinSquareSize / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hEncapVoidGrayMean = m_EncapParm.nBlueEncapVoidGrayMean;
				HTuple hEncapVoidMinMeanGVDiffContamination = m_EncapParm.nBlueEncapVoidMinMeanGVDiffContamination;
				HTuple hEncapVoidGrayDeviation = m_EncapParm.nBlueEncapVoidGrayDeviation;
				HTuple hEncapFuzzyVoidMinSize = m_EncapParm.dBlueEncapFuzzyVoidMinSize / (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));
				ReduceDomain(hImage, hEncapRegionInspected, &hRegionVoidInspect);

				_HP_Inspect_BlueEncap_Void(hRegionVoidInspect, &hDebugImageOutEncapVoid, &hDebugRegionOutEncapVoid, &hVoidDefectRegion, hEncapVoidMinMaskSize, hEncapVoidMinDefectContrast, hEncapVoidMinSize, nStepDebug, hEncapVoidMinCircularity, hEncapVoidMinLength, hEncapVoidMinSquareSize, hEncapVoidGrayMean, hEncapVoidGrayDeviation, hEncapVoidMinMeanGVDiffContamination, hEncapFuzzyVoidMinSize, &hDebugMsgOutEncapVoid, &hIsPass);

				if (nStepDebug) {
					StepDebug(hDebugImageOutEncapVoid, hDebugRegionOutEncapVoid, colorCyan, hDebugMsgOutEncapVoid, bRegionInsp);
				}
				HTuple hErosionOffSet = m_EncapParm.dBlueEncapErosionOffset / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);;
				HTuple hMaxLayOnPartPercentageAllow = m_EncapParm.dBlueEncapMaxLayOnAllowedPercentage;
				if (hIsPass == FALSE) {
					_FCI_Encap_Border_LayOn_Recheck(hImage, hEncapRegionInspected, hVoidDefectRegion, &hVoidDefectRegion, &hDebugImageOutEncapVoid, &hDebugRegionOutEncapVoid,
						nStepDebug, hErosionOffSet, hMaxLayOnPartPercentageAllow, &hDebugMsgOutEncapVoid, &hIsPass);

					if (nStepDebug) {
						StepDebug(hDebugImageOutEncapVoid, hDebugRegionOutEncapVoid, colorCyan, hDebugMsgOutEncapVoid, bRegionInsp);
					}

					if (hIsPass == FALSE) {
						m_arrayOverlayInspection.Add(hVoidDefectRegion, colorRed);
						return -ENCAP_VOID;
					}
				}

				HTuple hDebugMsgOutEncapBlowHole;
				HImage hDebugImageOutEncapBlowHole;
				HRegion hDebugRegionOutEncapBlowHole;
				HRegion hBlowHoleDefectRegion;

				HTuple hBHIntensity = m_EncapParm.nBlueEncapBHIntensity;
				HTuple hBHMinCircularity = m_EncapParm.dBlueEncapBHMinCircularity;
				HTuple hBHMinPercentLightArea = m_EncapParm.dBlueEncapBHMinPercentLightArea;
				HTuple hBHMinArea = m_EncapParm.dBlueEncapBHMinArea / (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));;
				HTuple hBHSlideWindowSide = m_EncapParm.dBlueEncapBHSlideWindowSide / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);;;
				_HP_Inspect_BlueEncap_BlowHole(hImage, hEncapRegionInspected, &hDebugImageOutEncapBlowHole, &hDebugRegionOutEncapBlowHole, &hBlowHoleDefectRegion, hBHIntensity, hBHSlideWindowSide, hBHMinArea, hBHMinCircularity, hBHMinPercentLightArea, nStepDebug, &hDebugMsgOutEncapBlowHole, &hIsPass);

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hBlowHoleDefectRegion, colorRed);
					return -ENCAP_BLOWHOLE;
				}
				//	//Wire And IC
				HRegion hEncapWire;
				HRegion hEncapWireBottom;
				HRegion hEncapIC;
				HRegion hBlueEncapWireAndIC;
				AffineTransRegion(hEncapWireTemp, &hEncapWire, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hEncapWireBottomTemp, &hEncapWireBottom, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				AffineTransRegion(hEncapICTemp, &hEncapIC, hHomMat2DIdentityEncapVoid, "nearest_neighbor");

				HTuple hTopWireIntensity = m_EncapParm.nBlueEncapTopWireIntensity;
				HTuple hTopWireMaxLineWidth = m_EncapParm .dBlueEncapTopWireMaxLineWidth/ ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hTopWireMinArea = m_EncapParm .dBlueEncapTopWireMinArea/ (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));
				HTuple hBottomWireIntensity = m_EncapParm.nBlueEncapBottomWireIntensity;
				HTuple hBottomWireMaxLineWidth = m_EncapParm .dBlueEncapBottomWireMaxLineWidth/ ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hBottomWireMinArea = m_EncapParm .dBlueEncapBottomWireMinArea/ (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));
				HTuple hMinAreaIC = m_EncapParm .dBlueEncapMinAreaIC/ (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));

				_HP_Inspect_BlueEncap_WireAndIC(hImage, hEncapWire, hEncapWireBottom, hEncapIC, &hDebugImageOutEncap, &hDebugRegionOutEncap, &hBlueEncapWireAndIC, hTopWireIntensity, hTopWireMinArea, hTopWireMaxLineWidth, hBottomWireIntensity, hBottomWireMinArea, hBottomWireMaxLineWidth, hMinAreaIC, nStepDebug, &hDebugMsgOutEncap, &hIsPass);

				if (nStepDebug) {
					StepDebug(hDebugImageOutEncap, hDebugRegionOutEncap, colorCyan, hDebugMsgOutEncap, bRegionInsp);
				}
				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hBlueEncapWireAndIC, colorRed);
					return -ENCAP_WIREANDIC;
				}

				// Bulge (Blue Encap) -magnus
				// // Region for search Bulge
				HRegion hBlueBulgeROITemp, hBlueBulgeROI;
				int nRow1 = m_TrainingData.m_rectBlueBulgeROI.top;
				int nCol1 = m_TrainingData.m_rectBlueBulgeROI.left;
				int nRow2 = m_TrainingData.m_rectBlueBulgeROI.bottom;
				int nCol2 = m_TrainingData.m_rectBlueBulgeROI.right;
				GenRectangle1(&hBlueBulgeROITemp, nRow1, nCol1, nRow2, nCol2);
				AffineTransRegion(hBlueBulgeROITemp, &hBlueBulgeROI, hHomMat2DIdentityEncapVoid, "nearest_neighbor");
				if (nStepDebug)
				{
					HTuple hMsg = "Region for Bulge Inspection [Bulge]";
					StepDebug(hImage, hBlueBulgeROI, colorGreen, hMsg, bRegionInsp);
				}
				double pixel = ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hBlueBulgeScaleContrastEncap = m_EncapParm.nBlueBulgeScaleContrastEncap;
				HTuple hBlueBulgeValueContrastDefect = m_EncapParm.nBlueBulgeValueContrastDefect;
				HTuple hBlueBulgeScaleContrastDefect = m_EncapParm.nBlueBulgeScaleContrastDefect;
				HTuple hBlueBulgeWidthLocalSearch = m_EncapParm.dBlueBulgeWidthLocalSearch / pixel;
				HTuple hBlueBulgeHeightLocalSearch = m_EncapParm.dBlueBulgeHeightLocalSearch / pixel;
				HTuple hBlueBulgeOffsetEncap = m_EncapParm.dBlueBulgeOffsetEncap / pixel;
				HTuple hBlueBulgeClosingRadiusDefect = m_EncapParm.dBlueBulgeClosingRadiusDefect / pixel;
				HTuple hBlueBulgeMinAreaDefect = m_EncapParm.dBlueBulgeMinAreaDefect / pixel / pixel;
				HTuple hBlueBulgeCircularityDefect = m_EncapParm.nBlueBulgeCircularityDefect / 100.0;
				HTuple hBlueBulgeHWRatioDefect = m_EncapParm.nBlueBulgeHWRatioDefect;
				HTuple hBlueBulgeInnerDefectPercent = m_EncapParm.nBlueBulgeInnerDefectPercent;
				HTuple hBlueBulgeOffsetCheckDefect = m_EncapParm.nBlueBulgeOffsetCheckDefect;
				HTuple hBlueBulgeMaxContrastInOutDefect = m_EncapParm.nBlueBulgeMaxContrastInOutDefect;
				HTuple hBlueBulgeMinGrayDefect = m_EncapParm.nBlueBulgeMinGrayDefect;
				HTuple hBlueBulgeMaxGrayDefect = m_EncapParm.nBlueBulgeMaxGrayDefect;
				HTuple hBlueBulgeMaxDeviationInnerDefect = m_EncapParm.nBlueBulgeMaxDeviationInnerDefect; //End magnus
				HTuple hBlueBulgeIsPass;
				HTuple hBlueBulgeDebugMessageOut;
				HImage hBlueBulgeDebugImageOut;
				HRegion hBlueBulgeDebugRegionOut;
				HRegion hBlueBulgeDefectsRegion;
				_HP_Inspect_BlueEncap_Bulge(hImage, hEncapRegionInspected,
					hBlueBulgeROI, &hBlueBulgeDebugImageOut, &hBlueBulgeDebugRegionOut,
					&hBlueBulgeDefectsRegion, hBlueBulgeScaleContrastEncap, hBlueBulgeValueContrastDefect,
					hBlueBulgeScaleContrastDefect, hBlueBulgeWidthLocalSearch, hBlueBulgeHeightLocalSearch,
					hBlueBulgeOffsetEncap, hBlueBulgeClosingRadiusDefect, hBlueBulgeMinAreaDefect,
					hBlueBulgeCircularityDefect, hBlueBulgeHWRatioDefect, hBlueBulgeInnerDefectPercent,
					hBlueBulgeOffsetCheckDefect, hBlueBulgeMaxContrastInOutDefect, hBlueBulgeMinGrayDefect,
					hBlueBulgeMaxGrayDefect, hBlueBulgeMaxDeviationInnerDefect, nStepDebug,
					&hBlueBulgeDebugMessageOut, &hBlueBulgeIsPass);
				if (nStepDebug) {
					StepDebug(hBlueBulgeDebugImageOut, hBlueBulgeDebugRegionOut, colorGreen, hBlueBulgeDebugMessageOut, bRegionInsp);
				}
				if (hBlueBulgeIsPass == FALSE)
				{
					m_arrayOverlayInspection.Add(hBlueBulgeDefectsRegion, colorRed, "Bulge Region");
					return -ENCAP_BULGE;
				}//End magnus
				
			}
			// Dark Encap Inspection
			else if (m_EncapParm.bEnableDarkEncap) {
				if (!m_TrainingData.bEnableDarkEncap)
					return -ENCAP_INSUFFICIENT;
				//Encap Tracing Region
				// // Teach Region for Tracing Encap
				HTuple hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2;
				// // Region Tracing Encap  
				HRegion hTracingDarkEncapRgnTemp;
				hEncapRow1 = m_TrainingData.m_rectAllRegionDarkEncap.top;
				hEncapCol1 = m_TrainingData.m_rectAllRegionDarkEncap.left;
				hEncapRow2 = m_TrainingData.m_rectAllRegionDarkEncap.bottom;
				hEncapCol2 = m_TrainingData.m_rectAllRegionDarkEncap.right;
				GenRectangle1(&hTracingDarkEncapRgnTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);

				// Region for Tracing Encap on flex
				HRegion hTopFlexDarkEncapRgnTemp;
				// // Region Find Left Line Flex for Generating Masking Encap
				hEncapRow1 = m_TrainingData.m_rectTopFlexDarkEncap.top;
				hEncapCol1 = m_TrainingData.m_rectTopFlexDarkEncap.left;
				hEncapRow2 = m_TrainingData.m_rectTopFlexDarkEncap.bottom;
				hEncapCol2 = m_TrainingData.m_rectTopFlexDarkEncap.right;
				GenRectangle1(&hTopFlexDarkEncapRgnTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);

				// // Region for remove Bottom White region
				HRegion hBotWhiteDarkEncapTemp;
				hEncapRow1 = m_TrainingData.m_rectBotWhiteDarkEncap.top;
				hEncapCol1 = m_TrainingData.m_rectBotWhiteDarkEncap.left;
				hEncapRow2 = m_TrainingData.m_rectBotWhiteDarkEncap.bottom;
				hEncapCol2 = m_TrainingData.m_rectBotWhiteDarkEncap.right;
				GenRectangle1(&hBotWhiteDarkEncapTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);

				// // Outside Encap Region
				HTuple hPoint;
				HTuple hRows, hCols;
				HRegion hOutsideDarkEncapRgnTemp;
				for (int nPointId = 0; nPointId < m_TrainingData.nDarkEncapOutsidePolygonPointNumber; nPointId++) {
					TupleConcat(hRows, m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].y, &hRows);
					TupleConcat(hCols, m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].x, &hCols);
					hPoint.Clear();
					hPoint.Append(m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].y);
					hPoint.Append(m_TrainingData.m_DarkEncapOutsidePolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hOutsideDarkEncapRgnTemp, hRows, hCols);

				// // Inside Encap Region 
				hRows = HTuple();
				hCols = HTuple();
				HRegion hInsideDarkEncapRgnTemp;
				for (int nPointId = 0; nPointId < m_TrainingData.nDarkEncapInsidePolygonPointNumber; nPointId++) {
					TupleConcat(hRows, m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].y, &hRows);
					TupleConcat(hCols, m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].x, &hCols);
					hPoint.Clear();
					hPoint.Append(m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].y);
					hPoint.Append(m_TrainingData.m_DarkEncapInsidePolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hInsideDarkEncapRgnTemp, hRows, hCols);

				BOOL bEnableDarkEncap;
				double pixel = ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple	hSmoothDarkEncap = m_TrainingData.m_nSmoothDarkEncap,
						hLeftRightContrastDarkEncap = m_TrainingData.m_nLeftRightContrastDarkEncap,
						hWhiteContrastDarkEncap = m_TrainingData.m_nWhiteContrastDarkEncap,
						hAreaWhiteRemoveDarkEncap = m_TrainingData.m_nAreaWhiteRemoveDarkEncap,
						hRoughContrastDarkEncap = m_TrainingData.m_nRoughContrastDarkEncap,
						hContrastFlexDarkEncap = m_TrainingData.m_nContrastFlexDarkEncap,
						hMinContrastFlexDarkEncap = m_TrainingData.m_nMinContrastFlexDarkEncap,
						hMaxContrastFlexDarkEncap = m_TrainingData.m_nMaxContrastFlexDarkEncap,
						hIsPass, hDebugMessageOut,
						hDilationHeight = 27,
						hClosingRadiusGround = 2.5,
						hClosingRadiusDarkEncap = 3.5,
						hGrayDarkThreshDarkEncap = 2* hLeftRightContrastDarkEncap;
				HImage hDebugImageOutDarkEncap;
				HRegion hDebugRegionOutDarkEncap,
					hTracingDarkEncapRgn,
					hInsideDarkEncapRgn,
					hOutsideDarkEncapRgn,
					hBotWhiteDarkEncap,
					hTopFlexDarkEncapRgn,
					hConcatDarkEncapMaskRgn,
					hDarkEncapRgn;

				// Region Projection
				HTuple hHomMat2DIdentityDarkEncap;
				HomMat2dIdentity(&hHomMat2DIdentityDarkEncap);
				HomMat2dRotate(hHomMat2DIdentityDarkEncap, -hInspectRotationAngle, hInspectShiftAlongRow + hDatumCenterRow, hInspectShiftAlongCol + hDatumCenterCol, &hHomMat2DIdentityDarkEncap);
				HomMat2dTranslate(hHomMat2DIdentityDarkEncap, -hInspectShiftAlongRow, -hInspectShiftAlongCol, &hHomMat2DIdentityDarkEncap);
				AffineTransRegion(hTracingDarkEncapRgnTemp, &hTracingDarkEncapRgn, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hInsideDarkEncapRgnTemp, &hInsideDarkEncapRgn, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hOutsideDarkEncapRgnTemp, &hOutsideDarkEncapRgn, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hBotWhiteDarkEncapTemp, &hBotWhiteDarkEncap, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hTopFlexDarkEncapRgnTemp, &hTopFlexDarkEncapRgn, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hConcatEncapMaskRgn, &hConcatDarkEncapMaskRgn, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				
				if (nStepDebug) {
					HTuple hMsg = "Region Projection [Tracing]";
					StepDebug(hImage, hTracingDarkEncapRgn, colorYellow, hMsg, bRegionInsp);
					StepDebug(hImage, hInsideDarkEncapRgn, colorYellow, hMsg, bRegionInsp);
					StepDebug(hImage, hOutsideDarkEncapRgn, colorYellow, hMsg, bRegionInsp);
					StepDebug(hImage, hBotWhiteDarkEncap, colorYellow, hMsg, bRegionInsp);
					StepDebug(hImage, hTopFlexDarkEncapRgn, colorYellow, hMsg, bRegionInsp);
					StepDebug(hImage, hConcatDarkEncapMaskRgn, colorYellow, hMsg, bRegionInsp);
				}

				_HP_DarkEncap_Tracing(hImage, hTracingDarkEncapRgn,
					hTopFlexDarkEncapRgn, hBotWhiteDarkEncap, hInsideDarkEncapRgn,
					hOutsideDarkEncapRgn, hConcatDarkEncapMaskRgn, &hDebugImageOutDarkEncap, &hDebugRegionOutDarkEncap,
					&hDarkEncapRgn, hDilationHeight, hSmoothDarkEncap,
					hWhiteContrastDarkEncap, hAreaWhiteRemoveDarkEncap, hLeftRightContrastDarkEncap,
					hClosingRadiusGround, hClosingRadiusDarkEncap, hRoughContrastDarkEncap,
					hContrastFlexDarkEncap, hMinContrastFlexDarkEncap, hMaxContrastFlexDarkEncap, hGrayDarkThreshDarkEncap,
					nStepDebug, &hDebugMessageOut, &hIsPass);
				m_arrayOverlayInspection.Add(hDarkEncapRgn, colorGreen);
				if (nStepDebug) {
					StepDebug(hDebugImageOutDarkEncap, hDebugRegionOutDarkEncap, colorCyan, hDebugMessageOut, bRegionInsp);
				}

				// Dark Encap Inspection //

				// // // // Excess
				// // Left Encap Region For Checking Excess
				HRegion hDarkExcessLeftMaskTemp;
				hEncapRow1 = m_TrainingData.m_rectDarkExcessLeftMask.top;
				hEncapCol1 = m_TrainingData.m_rectDarkExcessLeftMask.left;
				hEncapRow2 = m_TrainingData.m_rectDarkExcessLeftMask.bottom;
				hEncapCol2 = m_TrainingData.m_rectDarkExcessLeftMask.right;
				GenRectangle1(&hDarkExcessLeftMaskTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
				// // Right Encap Region For Checking Excess
				HRegion hDarkExcessRightMaskTemp;
				hEncapRow1 = m_TrainingData.m_rectDarkExcessRightMask.top;
				hEncapCol1 = m_TrainingData.m_rectDarkExcessRightMask.left;
				hEncapRow2 = m_TrainingData.m_rectDarkExcessRightMask.bottom;
				hEncapCol2 = m_TrainingData.m_rectDarkExcessRightMask.right;
				GenRectangle1(&hDarkExcessRightMaskTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);

				HRegion hDarkExcessLeftMask;
				HRegion hDarkExcessRightMask;
				AffineTransRegion(hDarkExcessLeftMaskTemp, &hDarkExcessLeftMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hDarkExcessRightMaskTemp, &hDarkExcessRightMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");

				HTuple  hDarkEncapExcessOffset = m_EncapParm.dDarkEncapExcessOffset/ pixel;
				HTuple  hDarkEncapExcessMinAreaDefect = m_EncapParm.dDarkEncapExcessMinAreaDefect / pixel / pixel;
				HTuple  hDarkEncapExcessMinContrastDefect = m_EncapParm.nDarkEncapExcessMinContrastDefect;
				HTuple  hDarkEncapExcessMaxContrastDefect = m_EncapParm.nDarkEncapExcessMaxContrastDefect;
				HObject	hDarkEncapExcessDebugImage;
				HObject	hDarkEncapExcessDebugRegion;
				HObject	hDarkEncapExcessRegion;
				HTuple hExcessIsPass;
				HTuple hDarkEncapExcessDebugMessageOut;
				_HP_Inspect_DarkEncap_Excess(hImage, hDarkExcessLeftMask,hDarkExcessRightMask, &hDarkEncapExcessDebugImage,
											&hDarkEncapExcessDebugRegion, &hDarkEncapExcessRegion,hDarkEncapExcessOffset,
											hDarkEncapExcessMinAreaDefect, hDarkEncapExcessMinContrastDefect,hDarkEncapExcessMaxContrastDefect,
											nStepDebug, &hDarkEncapExcessDebugMessageOut, &hExcessIsPass);

				if (nStepDebug) {
					StepDebug(hDarkEncapExcessDebugImage, hDarkEncapExcessDebugRegion, colorRed, hDarkEncapExcessDebugMessageOut, bRegionInsp);
				}
				if (hExcessIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hDarkEncapExcessRegion, colorRed);
					return -ENCAP_EXCESS;
				}

				// // // // Insufficient
				// // Left Encap Region For Checking Insufficient
				HRegion hDarkInsufficientLeftMaskTemp;
				hEncapRow1 = m_TrainingData.m_rectDarkInsufficientLeftMask.top;
				hEncapCol1 = m_TrainingData.m_rectDarkInsufficientLeftMask.left;
				hEncapRow2 = m_TrainingData.m_rectDarkInsufficientLeftMask.bottom;
				hEncapCol2 = m_TrainingData.m_rectDarkInsufficientLeftMask.right;
				GenRectangle1(&hDarkInsufficientLeftMaskTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
				// // Right Encap Region For Checking Insufficient
				HRegion hDarkInsufficientRightMaskTemp;
				hEncapRow1 = m_TrainingData.m_rectDarkInsufficientRightMask.top;
				hEncapCol1 = m_TrainingData.m_rectDarkInsufficientRightMask.left;
				hEncapRow2 = m_TrainingData.m_rectDarkInsufficientRightMask.bottom;
				hEncapCol2 = m_TrainingData.m_rectDarkInsufficientRightMask.right;
				GenRectangle1(&hDarkInsufficientRightMaskTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
				// // Region For Find border of the base
				HRegion hDarkInsufficientCoverMaskTemp;
				hEncapRow1 = m_TrainingData.m_rectDarkInsufficientCoverMask.top;
				hEncapCol1 = m_TrainingData.m_rectDarkInsufficientCoverMask.left;
				hEncapRow2 = m_TrainingData.m_rectDarkInsufficientCoverMask.bottom;
				hEncapCol2 = m_TrainingData.m_rectDarkInsufficientCoverMask.right;
				GenRectangle1(&hDarkInsufficientCoverMaskTemp, hEncapRow1, hEncapCol1, hEncapRow2, hEncapCol2);
				// // Middle Encap Region For Checking Insufficient
				HRegion hDarkInsufficientMidMaskTemp;
				HTuple hRowsDarkEncapInsuffMidRegion;
				HTuple hColsDarkEncapInsuffMidRegion;
				HTuple hPointDarkEncapInsuffMidRegion;
				for (int nPointId = 0; nPointId < DARK_ENCAP_INNER_INSUFFICIENT_MASK_POINT; nPointId++) {
					TupleConcat(hRowsDarkEncapInsuffMidRegion, m_TrainingData.m_polygonDarkInsufficientMidMask[nPointId].y, &hRowsDarkEncapInsuffMidRegion);
					TupleConcat(hColsDarkEncapInsuffMidRegion, m_TrainingData.m_polygonDarkInsufficientMidMask[nPointId].x, &hColsDarkEncapInsuffMidRegion);
					hPointDarkEncapInsuffMidRegion.Clear();
					hPointDarkEncapInsuffMidRegion.Append(m_TrainingData.m_polygonDarkInsufficientMidMask[nPointId].y);
					hPointDarkEncapInsuffMidRegion.Append(m_TrainingData.m_polygonDarkInsufficientMidMask[nPointId].x);
				}
				GenRegionPolygonFilled(&hDarkInsufficientMidMaskTemp, hRowsDarkEncapInsuffMidRegion, hColsDarkEncapInsuffMidRegion);
				HRegion hDarkInsufficientLeftMask;
				HRegion hDarkInsufficientRightMask;
				HRegion hDarkInsufficientCoverMask;
				HRegion hDarkInsufficientMidMask;
				AffineTransRegion(hDarkInsufficientLeftMaskTemp, &hDarkInsufficientLeftMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hDarkInsufficientRightMaskTemp, &hDarkInsufficientRightMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hDarkInsufficientCoverMaskTemp, &hDarkInsufficientCoverMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hDarkInsufficientMidMaskTemp, &hDarkInsufficientMidMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
			
				HTuple  hDarkEncapInsuffContrast = m_EncapParm.nDarkEncapInsuffContrast;
				HTuple  hDarkEncapInsuffVariationContrast = m_EncapParm.nDarkEncapInsuffVariationContrast;
				HTuple  hDarkEncapInsuffWhiteContrast = m_EncapParm.nDarkEncapInsuffWhiteContrast;
				HTuple  hDarkEncapInsuffLeftOffset = m_EncapParm.dDarkEncapInsuffLeftOffset / pixel;
				HTuple  hDarkEncapInsuffRightOffset = m_EncapParm.dDarkEncapInsuffRightOffset / pixel;
				HTuple  hDarkEncapInsuffMinPercentLeftError = m_EncapParm.dDarkEncapInsuffMinPercentLeftError;
				HTuple  hDarkEncapInsuffMinPercentRightError = m_EncapParm.dDarkEncapInsuffMinPercentRightError;
				HTuple  hDarkEncapInsuffMinAreaMidError = m_EncapParm.dDarkEncapInsuffMinAreaMidError / pixel / pixel;
				HObject	hDarkEncapInsuffDebugImage;
				HObject	hDarkEncapInsuffDebugRegion;
				HObject	hDarkEncapInsuffRegion;
				HObject	hDarkEncapROIRemoveBorderRegion;// for tracing dark encap
				HTuple hInsuffIsPass;
				HTuple hDarkEncapInsuffDebugMessageOut;
				_HP_Inspect_DarkEncap_Insufficient(hImage, hDarkInsufficientCoverMask,
									hDarkInsufficientLeftMask, hDarkInsufficientRightMask, hDarkInsufficientMidMask,
									&hDarkEncapInsuffDebugImage, &hDarkEncapInsuffDebugRegion,
									&hDarkEncapInsuffRegion, &hDarkEncapROIRemoveBorderRegion, hDarkEncapInsuffContrast,
									hDarkEncapInsuffVariationContrast, hDarkEncapInsuffWhiteContrast,
									hDarkEncapInsuffLeftOffset, hDarkEncapInsuffRightOffset,
									hDarkEncapInsuffMinAreaMidError, hDarkEncapInsuffMinPercentLeftError,
									hDarkEncapInsuffMinPercentRightError, nStepDebug, &hDarkEncapInsuffDebugMessageOut,
									&hInsuffIsPass);

				if (nStepDebug) {
					StepDebug(hDarkEncapInsuffDebugImage, hDarkEncapInsuffDebugRegion, colorRed, hDarkEncapInsuffDebugMessageOut, bRegionInsp);
				}
				if (hInsuffIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hDarkEncapInsuffRegion, colorRed);
					return -ENCAP_INSUFFICIENT;
				}
			
				// // // // Void 
				HObject hDarkEncapVoidMaskingLeftDieTemp;
				GenRectangle1(&hDarkEncapVoidMaskingLeftDieTemp,
					m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea.top,
					m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea.left,
					m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea.bottom,
					m_TrainingData.m_rectDarkEncapVoidMaskingLeftDiePointArea.right);

				HObject hDarkEncapVoidMaskingRightDieTemp;
				GenRectangle1(&hDarkEncapVoidMaskingRightDieTemp,
					m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea.top,
					m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea.left,
					m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea.bottom,
					m_TrainingData.m_rectDarkEncapVoidMaskingRightDiePointArea.right);

				HRegion hDarkEncapVoidMaskingLeftDie;
				HRegion hDarkEncapVoidMaskingRightDie;
				AffineTransRegion(hDarkEncapVoidMaskingLeftDieTemp, &hDarkEncapVoidMaskingLeftDie, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hDarkEncapVoidMaskingRightDieTemp, &hDarkEncapVoidMaskingRightDie, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				HRegion hDarkEncapVoidMaskingDie;
				ConcatObj(hDarkEncapVoidMaskingLeftDie, hDarkEncapVoidMaskingRightDie, &hDarkEncapVoidMaskingDie);
				Union1(hDarkEncapVoidMaskingDie, &hDarkEncapVoidMaskingDie);
				HObject	hDarkEncapVoidDebugImage;
				HRegion	hDarkEncapVoidDebugRegion;
				HTuple hDarkEncapVoidDebugMess, hIsPassVoid;
				HRegion hDarkEncapRegionForVoidAndBH;
				hIsPassVoid = TRUE;
				_HP_DarkEncap_ProcessEncapRegionForVoidAndBH(hImage, hDarkEncapRgn, hDarkEncapVoidMaskingDie, hConcatDarkEncapMaskRgn, &hDarkEncapVoidDebugImage, &hDarkEncapVoidDebugRegion, &hDarkEncapRegionForVoidAndBH, nStepDebug, &hDarkEncapVoidDebugMess, &hIsPassVoid);
				if (nStepDebug) {
					StepDebug(hDarkEncapVoidDebugImage, hDarkEncapVoidDebugRegion, colorRed, hDarkEncapVoidDebugMess, bRegionInsp);
				}
				// Glue - Mason
				HObject hDarkGlueMask;
				AffineTransRegion(m_TrainingData.hDarkGlueMask, &hDarkGlueMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hDarkGlueMask, colorGreen, "Mask Using To Find Glue Defect Region [Glue]", bRegionInsp);
				}

				HTuple hDarkContrastGlue = m_EncapParm.nDarkContrastGlue;
				HTuple hDarkMaskSizeGlue = m_EncapParm.dDarkMaskSizeGlue / pixel;
				HTuple hDarkEncapRegionOffsetGlue = m_EncapParm.dDarkEncapRegionOffsetGlue / pixel;
				HTuple hDarkMinSizeGlue = m_EncapParm.dDarkMinSizeGlue / (pixel*pixel);
				HTuple hDarkMinMeanGrayGlue = m_EncapParm.nDarkMinMeanGrayGlue;
				HTuple hDarkMinConvexityGlue = m_EncapParm.dDarkMinConvexityGlue;
				HTuple hDarkMinDiffMeanGrayGlue = m_EncapParm.nDarkMinDiffMeanGrayGlue;
				HTuple hDarkMaxFuzzySizeGlue = m_EncapParm.dDarkMaxFuzzySizeGlue / (pixel*pixel);
				HTuple hDarkMaxWhiteAreaGlue = m_EncapParm.dDarkMaxWhiteAreaGlue / (pixel*pixel);;

				HObject hDarkEncapGlueDefectRegion;
				_HP_Inspect_DarkEncap_Glue(hImage,
					hDarkGlueMask,
					hDarkEncapRgn,
					&hDebugImage,
					&hDebugRegion,
					&hDarkEncapGlueDefectRegion,
					nStepDebug,
					hDarkContrastGlue,
					hDarkMaskSizeGlue,
					hDarkMinSizeGlue,
					hDarkMinMeanGrayGlue,
					hDarkMinConvexityGlue,
					hDarkEncapRegionOffsetGlue,
					hDarkMinDiffMeanGrayGlue,
					hDarkMaxFuzzySizeGlue,
					hDarkMaxWhiteAreaGlue,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hDarkEncapGlueDefectRegion, colorRed, "Glue Defect Region");
					return -ENCAP_GLUE;
				}

				// Contamination - Mason
				HObject hDarkEncapDieMaskCT;
				GenEmptyObj(&hDarkEncapDieMaskCT);

				HObject hDarkFlexEdgeMask;
				HObject hDarkBottomEdgeMask;
				HObject hDarkRightEdgeMask;
				HObject hDarkInnerMask;
				AffineTransRegion(m_TrainingData.hDarkFlexEdgeMask, &hDarkFlexEdgeMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDarkBottomEdgeMask, &hDarkBottomEdgeMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDarkRightEdgeMask, &hDarkRightEdgeMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDarkInnerMask, &hDarkInnerMask, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hDarkFlexEdgeMask, colorGreen, "Mask Using To Re-Check Contamination Between Flex And Encap [Contamination]", bRegionInsp);
					StepDebug(hImage, hDarkBottomEdgeMask, colorGreen, "Mask Using To Re-Check Contamination In The Bottom Of Encap [Contamination]", bRegionInsp);
					StepDebug(hImage, hDarkRightEdgeMask, colorGreen, "Mask Using To Re-Check Contamination In The Right Of Encap [Contamination]", bRegionInsp);
					StepDebug(hImage, hDarkInnerMask, colorGreen, "Mask Using To Find Fuzzy Contamination In The Inner Of Encap [Contamination]", bRegionInsp);
				}

				HTuple hDarkMaskSizeCT = m_EncapParm.dDarkMaskSizeCT / pixel;
				HTuple hDarkStandardDeviationCT = m_EncapParm.dDarkStandardDeviationCT;
				HTuple hDarkDiffMeanGrayCT = m_EncapParm.nDarkDiffMeanGrayCT;
				HTuple hDarkFuzzyContrastCT = m_EncapParm.nDarkFuzzyContrastCT;
				HTuple hDarkMinGrayCT = m_EncapParm.nDarkMinGrayCT;
				HTuple hDarkMinSizeCT = m_EncapParm.dDarkMinSizeCT / (pixel*pixel);
				HTuple hDarkEncapRegionOffsetCT = m_EncapParm.dDarkEncapRegionOffsetCT / pixel;
				HTuple hDarkMinDiffMeanGrayCT = m_EncapParm.nDarkMinDiffMeanGrayCT;
				HTuple hDarkMinMeanEdgeMaskGrayCT = m_EncapParm.nDarkMinMeanEdgeMaskGrayCT;
				HTuple hDarkMaxDiffAreaPercentCT = m_EncapParm.dDarkMaxDiffAreaPercentCT;
				HTuple hDarkMinFuzzyGrayCT = m_EncapParm.nDarkMinFuzzyGrayCT;
				HTuple hDarkMinDiffMeanFuzzyGrayCT = m_EncapParm.nDarkMinDiffMeanFuzzyGrayCT;
				HTuple hDBMinDefectCountCT = m_EncapParm.nDBMinDefectCountCT;

				HObject hDarkEncapContaminationDefectRegion;
				_HP_Inspect_DarkEncap_Contamination(hImage,
					hDarkEncapRgn,
					hDarkEncapDieMaskCT,
					hDarkFlexEdgeMask,
					hDarkBottomEdgeMask,
					hDarkRightEdgeMask,
					hDarkInnerMask,
					&hDebugImage,
					&hDebugRegion,
					&hDarkEncapContaminationDefectRegion,
					nStepDebug,
					hDarkMaskSizeCT,
					hDarkStandardDeviationCT,
					hDarkDiffMeanGrayCT,
					hDarkMinGrayCT,
					hDarkMinSizeCT,
					hDarkEncapRegionOffsetCT,
					hDarkMinDiffMeanGrayCT,
					hDarkMinMeanEdgeMaskGrayCT,
					hDarkMaxDiffAreaPercentCT,
					hDarkFuzzyContrastCT,
					hDarkMinFuzzyGrayCT,
					hDarkMinDiffMeanFuzzyGrayCT,
					hDBMinDefectCountCT,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hDarkEncapContaminationDefectRegion, colorRed, "Contamination Defect Region");
					return -ENCAP_CONTAMINATION;
				}
				HObject ImageForInspectVoidAndBH;
				HRegion hDefectVoidRegion;
				ReduceDomain(hImage, hDarkEncapRegionForVoidAndBH, &ImageForInspectVoidAndBH);
				// ***Small Void
				HTuple hSmallVoidMinSizeDarkEncap = m_EncapParm.dSmallVoidMinSizeDarkEncap/ (pixel* pixel);
				HTuple hSmallVoidMinCircularityDarkEncap = m_EncapParm.dSmallVoidMinCircularityDarkEncap;
				HTuple hSmallVoidGrayDeviationDarkEncap = m_EncapParm.dSmallVoidGrayDeviationDarkEncap;
				HTuple hSmallVoidIntensityDarkEncap = m_EncapParm.nSmallVoidIntensityDarkEncap;
				HRegion hSmallVoidRegion;
				HTuple hIsPassSmallVoid = TRUE;
				GenEmptyObj(&hSmallVoidRegion);
				_HP_DarkEncap_Inspect_SmallVoid(ImageForInspectVoidAndBH, hImage, &hDarkEncapVoidDebugImage,&hDarkEncapVoidDebugRegion,&hSmallVoidRegion, hSmallVoidIntensityDarkEncap, hSmallVoidMinSizeDarkEncap, hSmallVoidMinCircularityDarkEncap, hSmallVoidGrayDeviationDarkEncap, nStepDebug, &hDarkEncapVoidDebugMess, &hIsPassSmallVoid);
				if (nStepDebug) {
					StepDebug(hDarkEncapVoidDebugImage, hDarkEncapVoidDebugRegion, colorRed, hDarkEncapVoidDebugMess, bRegionInsp);
				}
				HTuple hSmallVoidErosionOffSetDarkEncap = 0;
				HTuple hSmallVoidMaxLayOnPartPercentageAllowDarkEncap = m_EncapParm.dSmallVoidMaxLayOnPartPercentageAllowDarkEncap;
				// Remove overkill by Border Lay On Recheck

				_FCI_Encap_Border_LayOn_Recheck(hImage, hDarkEncapRegionForVoidAndBH, hSmallVoidRegion, &hSmallVoidRegion, &hDarkEncapVoidDebugImage, &hDarkEncapVoidDebugRegion, nStepDebug, hSmallVoidErosionOffSetDarkEncap, hSmallVoidMaxLayOnPartPercentageAllowDarkEncap, &hDarkEncapVoidDebugMess, &hIsPassSmallVoid);
				if (nStepDebug) {
					StepDebug(hDarkEncapVoidDebugImage, hDarkEncapVoidDebugRegion, colorRed, hDarkEncapVoidDebugMess, bRegionInsp);
				}
				// ***Large Void
				HTuple hLargeVoidMinSizeDarkEncap = m_EncapParm.dLargeVoidMinSizeDarkEncap / (pixel* pixel);
				HTuple hLargeVoidMinCircularityDarkEncap = m_EncapParm.dLargeVoidMinCircularityDarkEncap;
				HTuple hLargeVoidIntensityDarkEncap = m_EncapParm.nLargeVoidIntensityDarkEncap;
				HTuple hLargeVoidMinGrayDeviationDarkEncap = m_EncapParm.dLargeVoidMinGrayDeviationDarkEncap;
				HRegion hLargeVoidRegion;
				GenEmptyObj(&hLargeVoidRegion);
				HTuple hIsPassLargeVoid = TRUE;

				_HP_DarkEncap_Inspect_LargeVoid(ImageForInspectVoidAndBH, hImage, &hDarkEncapVoidDebugImage, &hDarkEncapVoidDebugRegion, &hLargeVoidRegion, hLargeVoidIntensityDarkEncap, hLargeVoidMinSizeDarkEncap, hLargeVoidMinCircularityDarkEncap, hLargeVoidMinGrayDeviationDarkEncap, nStepDebug, &hDarkEncapVoidDebugMess, &hIsPassLargeVoid);
				if (nStepDebug) {
					StepDebug(hDarkEncapVoidDebugImage, hDarkEncapVoidDebugRegion, colorRed, hDarkEncapVoidDebugMess, bRegionInsp);
				}
				// Remove overkill by Border Lay On Recheck
				HTuple hLargeVoidErosionOffSetDarkEncap = 0;
				HTuple hLargeVoidMaxLayOnPartPercentageAllowDarkEncap = m_EncapParm.dLargeVoidMaxLayOnPartPercentageAllowDarkEncap;

				_FCI_Encap_Border_LayOn_Recheck(hImage, hDarkEncapRegionForVoidAndBH, hLargeVoidRegion, &hLargeVoidRegion, &hDarkEncapVoidDebugImage, &hDarkEncapVoidDebugRegion, nStepDebug, hLargeVoidErosionOffSetDarkEncap, hLargeVoidMaxLayOnPartPercentageAllowDarkEncap, &hDarkEncapVoidDebugMess, &hIsPassLargeVoid);
				if (nStepDebug) {
					StepDebug(hDarkEncapVoidDebugImage, hDarkEncapVoidDebugRegion, colorRed, hDarkEncapVoidDebugMess, bRegionInsp);
				}
				// **Fuzzy Void
				HTuple hFuzzyVoidMinMeanGVDiffDarkEncap = m_EncapParm.dFuzzyVoidMinMeanGVDiffDarkEncap;
				HTuple hFuzzyVoidMinIntensityDarkEncap = m_EncapParm.nFuzzyVoidMinIntensityDarkEncap;
				HTuple hFuzzyVoidMinSizeDarkEncap = m_EncapParm.dFuzzyVoidMinSizeDarkEncap / (pixel* pixel);
				HTuple hFuzzyVoidMinAnisometryDarkEncap = m_EncapParm.dFuzzyVoidMinAnisometryDarkEncap;
				HTuple hFuzzyVoidMinGrayMeanDarkEncap = m_EncapParm.dFuzzyVoidMinGrayMeanDarkEncap;
				HTuple hFuzzyVoidMinGrayDeviationDarkEncap = m_EncapParm.dFuzzyVoidMinGrayDeviationDarkEncap;
				HRegion hFuzzyVoidRegion,hRegionErosionDarkEncap;
				GenEmptyObj(&hFuzzyVoidRegion);
				HTuple hIsPassFuzzyVoid = TRUE;
				_HP_DarkEncap_Inspect_FuzzyVoid(hDarkEncapRegionForVoidAndBH, hImage, &hDarkEncapVoidDebugImage, &hDarkEncapVoidDebugRegion,
					&hRegionErosionDarkEncap, &hFuzzyVoidRegion, hFuzzyVoidMinIntensityDarkEncap, hFuzzyVoidMinSizeDarkEncap,
					hFuzzyVoidMinGrayMeanDarkEncap, hFuzzyVoidMinGrayDeviationDarkEncap, hFuzzyVoidMinMeanGVDiffDarkEncap,
					nStepDebug, &hDarkEncapVoidDebugMess, &hIsPassFuzzyVoid);
				if (nStepDebug) {
					StepDebug(hDarkEncapVoidDebugImage, hDarkEncapVoidDebugRegion, colorRed, hDarkEncapVoidDebugMess, bRegionInsp);
				}
				//**Remove overkill by Border Lay On Recheck
				HTuple hFuzzyVoidErosionOffSet = 0;
				HTuple hFuzzyVoidMaxLayOnPartPercentageAllow = m_EncapParm.dFuzzyVoidMaxLayOnPartPercentageAllow;
				_FCI_Encap_Border_LayOn_Recheck(hImage, hDarkEncapRegionForVoidAndBH, hFuzzyVoidRegion, &hFuzzyVoidRegion, &hDarkEncapVoidDebugImage, &hDarkEncapVoidDebugRegion, nStepDebug, hFuzzyVoidErosionOffSet, hFuzzyVoidMaxLayOnPartPercentageAllow, &hDarkEncapVoidDebugMess, &hIsPassFuzzyVoid);
				if (nStepDebug) {
					StepDebug(hDarkEncapVoidDebugImage, hDarkEncapVoidDebugRegion, colorRed, hDarkEncapVoidDebugMess, bRegionInsp);
				}

				if (hIsPassFuzzyVoid == FALSE || hIsPassLargeVoid == FALSE || hIsPassSmallVoid == FALSE) {
					HRegion hVoidDefectDarkEncap;
					GenEmptyObj(&hVoidDefectDarkEncap);
					ConcatObj(hFuzzyVoidRegion, hLargeVoidRegion, &hVoidDefectDarkEncap);
					ConcatObj(hVoidDefectDarkEncap, hSmallVoidRegion, &hVoidDefectDarkEncap);
					Union1(hVoidDefectDarkEncap, &hVoidDefectDarkEncap);
					m_arrayOverlayInspection.Add(hVoidDefectDarkEncap, colorRed);
					return -ENCAP_VOID;
				}
				// // // // BlowHole
				HTuple hBlowHoleSlideWindowSideDarkEncap = m_EncapParm.dBlowHoleSlideWindowSideDarkEncap /pixel;
				HTuple hLargeBlowHoleAnisometryDarkEncap = m_EncapParm.dLargeBlowHoleAnisometryDarkEncap;
				HTuple hSmallBlowHoleAnisometryDarkEncap = m_EncapParm.dSmallBlowHoleAnisometryDarkEncap;
				HTuple hSmallBlowMaxGrayMeanDarkEncap = m_EncapParm.dSmallBlowMaxGrayMeanDarkEncap;
				HRegion hBHRegion;
				GenEmptyObj(&hBHRegion);
				HTuple hIsPassBH = TRUE;

				_HP_DarkEncap_Inspect_BlowHole(hDarkEncapRegionForVoidAndBH, hImage, &hDarkEncapVoidDebugImage, &hDarkEncapVoidDebugRegion, &hBHRegion,hBlowHoleSlideWindowSideDarkEncap, hLargeBlowHoleAnisometryDarkEncap, hSmallBlowHoleAnisometryDarkEncap, hSmallBlowMaxGrayMeanDarkEncap, nStepDebug, &hDarkEncapVoidDebugMess, &hIsPassBH);
				if (nStepDebug) {
					StepDebug(hDarkEncapVoidDebugImage, hDarkEncapVoidDebugRegion, colorRed, hDarkEncapVoidDebugMess, bRegionInsp);
				}

				if (hIsPassBH == FALSE) {
					m_arrayOverlayInspection.Add(hBHRegion, colorRed);
					return -ENCAP_BLOWHOLE;
				}
	
				// // // // Wire And Ic
				HObject hDarkEncapRgnWireTemp;
				hRows = HTuple();
				hCols = HTuple();
				for (int nPointId = 0; nPointId < m_TrainingData.nDarkEncapWirePolygonPointNumber; nPointId++) {
					TupleConcat(hRows, m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].y, &hRows);
					TupleConcat(hCols, m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].x, &hCols);
					hPoint.Clear();
					hPoint.Append(m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].y);
					hPoint.Append(m_TrainingData.m_DarkEncapWirePolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hDarkEncapRgnWireTemp, hRows, hCols);

				HObject hDarkEncapRgnICTemp;
				hRows = HTuple();
				hCols = HTuple();
				for (int nPointId = 0; nPointId < m_TrainingData.nDarkEncapICPolygonPointNumber; nPointId++) {
					TupleConcat(hRows, m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].y, &hRows);
					TupleConcat(hCols, m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].x, &hCols);
					hPoint.Clear();
					hPoint.Append(m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].y);
					hPoint.Append(m_TrainingData.m_DarkEncapICPolygonPointArea[nPointId].x);
				}
				GenRegionPolygonFilled(&hDarkEncapRgnICTemp, hRows, hCols);

				HObject hDarkEncapRgnIC;
				HObject hDarkEncapRgnWire;
				AffineTransRegion(hDarkEncapRgnICTemp, &hDarkEncapRgnIC, hHomMat2DIdentityDarkEncap, "nearest_neighbor");
				AffineTransRegion(hDarkEncapRgnWireTemp, &hDarkEncapRgnWire, hHomMat2DIdentityDarkEncap, "nearest_neighbor");

				// Wire And IC
				HTuple hWireMinLengthDarkEncap = m_EncapParm.dWireMinLengthDarkEncap /pixel;
				HTuple hWireIntensityDarkEncap = 220;
				HTuple hWireMaxLineWidthDarkEncap = m_EncapParm.dWireMaxLineWidthDarkEncap / pixel;
				HTuple hWireContrastDarkEncap = m_EncapParm.nWireContrastDarkEncap;
				HTuple hMaxWireCircularityDarkEncap = m_EncapParm.dMaxWireCircularityDarkEncap;

				HTuple hICIntensityDarkEncap = m_EncapParm.nICIntensityDarkEncap;
				HTuple hICMinAreaDarkEncap = m_EncapParm.dICMinAreaDarkEncap / (pixel* pixel);
				HObject	hDarkEncapWireAndICDebugImage;
				HRegion	hDarkEncapWireAndICDebugRegion;
				HTuple hDarkEncapWireAndICDebugMess, hIsPassWireAndIC;
				HRegion hWireAndIcDefectDarkEncap;
				hIsPassWireAndIC = TRUE;
				_HP_DarkEncap_Inspect_WireAndIC(hImage, hDarkEncapRgnWire, hDarkEncapRgnIC, &hDarkEncapWireAndICDebugImage,
					&hDarkEncapWireAndICDebugRegion, &hWireAndIcDefectDarkEncap, hWireIntensityDarkEncap, hMaxWireCircularityDarkEncap,
					hWireMaxLineWidthDarkEncap, hWireContrastDarkEncap, hWireMinLengthDarkEncap, hICIntensityDarkEncap, hICMinAreaDarkEncap,
					nStepDebug, &hDarkEncapWireAndICDebugMess, &hIsPassWireAndIC);
				if (nStepDebug) {
					StepDebug(hDarkEncapWireAndICDebugImage, hDarkEncapWireAndICDebugRegion, colorRed, hDarkEncapWireAndICDebugMess, bRegionInsp);
				}
				if (hIsPassWireAndIC == FALSE) {
					m_arrayOverlayInspection.Add(hWireAndIcDefectDarkEncap, colorRed);
					return -ENCAP_WIREANDIC;
				}					
			}
			// Dark Blue Encap Inspection
			else if (m_EncapParm.bEnableDarkBlueEncap) {
				if (!m_TrainingData.bEnableDarkBlueEncap)
					return -ENCAP_INSUFFICIENT;
				
				HTuple hIsPass = TRUE;

				// ROI For Inspect
				HObject hDBEncapExtractROI;
				HObject hDBEncapInnerROI;
				HObject hDBEncapTopEdgeMaskROI;
				HObject hDBEncapBottomEdgeMaskROI;
				HObject hDBEncapLeftEdgeMaskROI;
				HObject hDBEncapRightEdgeMaskROI;
				HObject hDBEncapFlexEdgeMaskROI;
				HObject hDBEncapMaskTracingROI;

				// Using For Region Projection
				HTuple hMatchPatternAngle = hDatumCenterRotationAngle;
				HTuple hMatchPatternRow = hDatumCenterRow;
				HTuple hMatchPatternCol = hDatumCenterCol;

				HTuple hRotationAngle = hInspectRotationAngle;
				HTuple hShiftAlongRow = hInspectShiftAlongRow;
				HTuple hShiftAlongCol = hInspectShiftAlongCol;

				HTuple hRefPatternAngle = hRotationAngle + hMatchPatternAngle;
				HTuple hRefPatternRow = hShiftAlongRow + hMatchPatternRow;
				HTuple hRefPatternCol = hShiftAlongCol + hMatchPatternCol;

				// Vision Parameters
				HTuple hDBEncapEdgeEncapContrast = m_TrainingData.nDBEncapEdgeEncapContrast;
				HTuple hDBEncapEdgeFlexContrast = m_TrainingData.nDBEncapEdgeFlexContrast;
				HTuple hDBEncapMinEncapContrast = m_TrainingData.nDBEncapMinEncapContrast;
				HTuple hDBEncapMaxEncapContrast = m_TrainingData.nDBEncapMaxEncapContrast;

				// Region Projection
				HTuple hHomMat2DIdentity;
				HomMat2dIdentity(&hHomMat2DIdentity);
				HomMat2dRotate(hHomMat2DIdentity, -hRotationAngle, hRefPatternRow, hRefPatternCol, &hHomMat2DIdentity);
				HomMat2dTranslate(hHomMat2DIdentity, -hShiftAlongRow, -hShiftAlongCol, &hHomMat2DIdentity);
				AffineTransRegion(m_TrainingData.hDBEncapExtractROI, &hDBEncapExtractROI, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDBEncapInnerROI, &hDBEncapInnerROI, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDBEncapTopEdgeMaskROI, &hDBEncapTopEdgeMaskROI, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDBEncapBottomEdgeMaskROI, &hDBEncapBottomEdgeMaskROI, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDBEncapLeftEdgeMaskROI, &hDBEncapLeftEdgeMaskROI, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDBEncapRightEdgeMaskROI, &hDBEncapRightEdgeMaskROI, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDBEncapFlexEdgeMaskROI, &hDBEncapFlexEdgeMaskROI, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(hConcatEncapMaskRgn, &hDBEncapMaskTracingROI, hHomMat2DIdentity, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hDBEncapExtractROI, colorGreen, "Encap Extract ROI Projection [Tracing]", bRegionInsp);
					StepDebug(hImage, hDBEncapInnerROI, colorGreen, "Encap Inner ROI Projection [Tracing]", bRegionInsp);
					StepDebug(hImage, hDBEncapTopEdgeMaskROI, colorYellow, "Encap Top Edge Mask Projection [Tracing]", bRegionInsp);
					StepDebug(hImage, hDBEncapBottomEdgeMaskROI, colorYellow, "Encap Bottom Edge Mask Projection [Tracing]", bRegionInsp);
					StepDebug(hImage, hDBEncapLeftEdgeMaskROI, colorYellow, "Encap Left Edge Mask Projection [Tracing]", bRegionInsp);
					StepDebug(hImage, hDBEncapRightEdgeMaskROI, colorYellow, "Encap Right Edge Mask Projection [Tracing]", bRegionInsp);
					StepDebug(hImage, hDBEncapFlexEdgeMaskROI, colorYellow, "Encap Flex Edge Mask Projection [Tracing]", bRegionInsp);
					StepDebug(hImage, hDBEncapMaskTracingROI, colorYellow, "Encap Tracing Mask Projection [Tracing]", bRegionInsp);
				}

				// Prepare Data For Inspection
				HObject hEncapRegion;

				_HP_DarkBlueEncap_Tracing(hImage,
					hDBEncapExtractROI,
					hDBEncapInnerROI,
					hDBEncapFlexEdgeMaskROI,
					hDBEncapTopEdgeMaskROI,
					hDBEncapBottomEdgeMaskROI,
					hDBEncapLeftEdgeMaskROI,
					hDBEncapRightEdgeMaskROI,
					hDBEncapMaskTracingROI,
					&hEncapRegion,
					&hDebugImage,
					&hDebugRegion,
					nStepDebug,
					hDBEncapEdgeEncapContrast,
					hDBEncapEdgeFlexContrast,
					hDBEncapMinEncapContrast,
					hDBEncapMaxEncapContrast,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				// Check Result And Display Image Region If Get Error
				if (hIsPass == FALSE) {
					HObject RegionImage;
					GetDomain(hImage, &RegionImage);
					m_arrayOverlayInspection.Add(RegionImage, colorRed, "Encap Tracing Failed");
					return -ENCAP_INSUFFICIENT;
				}

				// Display Encap Location When Inspecting
				m_arrayOverlayInspection.Add(hEncapRegion, colorGreen, "Encap Region");

				// Excess - Mason
				HObject hDBLeftExcessMask;
				AffineTransRegion(m_TrainingData.hDBLeftExcessMask, &hDBLeftExcessMask, hHomMat2DIdentity, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hDBLeftExcessMask, colorGreen, "Mask Using To Find Excess Region In The Left Of Encap [Excess]", bRegionInsp);
				}

				HObject hDBRightExcessMask;
				AffineTransRegion(m_TrainingData.hDBRightExcessMask, &hDBRightExcessMask, hHomMat2DIdentity, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hDBRightExcessMask, colorGreen, "Mask Using To Find Excess Region In The Right Of Encap [Excess]", bRegionInsp);
				}

				double dResolution = ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hDBContrastEX = m_EncapParm.nDBContrastEX;
				HTuple hDBCircleContrastEX = m_EncapParm.nDBCircleContrastEX;
				HTuple hDBEdgeOffsetEX = m_EncapParm.dDBEdgeOffsetEX / dResolution;
				HTuple hDBMinAreaEX = m_EncapParm.dDBMinAreaEX / (dResolution*dResolution);

				HObject hDBEncapExcessDefectRegion;
				_HP_Inspect_DarkBlueEncap_Excess(hImage,
					hDBLeftExcessMask,
					hDBRightExcessMask,
					&hDebugImage,
					&hDebugRegion,
					&hDBEncapExcessDefectRegion,
					nStepDebug,
					hDBContrastEX,
					hDBCircleContrastEX,
					hDBEdgeOffsetEX,
					hDBMinAreaEX,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hDBEncapExcessDefectRegion, colorRed, "Excess Defect Region");
					return -ENCAP_EXCESS;
				}

				// Insufficient - Mason
				HObject hDBCoverInsufficientMask;
				AffineTransRegion(m_TrainingData.hDBCoverInsufficientMask, &hDBCoverInsufficientMask, hHomMat2DIdentity, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hDBCoverInsufficientMask, colorGreen, "Mask Using To Find Reference Edge Line And Check Insufficient In The Edge Of Encap [Insufficient]", bRegionInsp);
				}

				HObject hDBInnerInsufficientMask;
				AffineTransRegion(m_TrainingData.hDBInnerInsufficientMask, &hDBInnerInsufficientMask, hHomMat2DIdentity, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hDBInnerInsufficientMask, colorGreen, "Mask Using To Find Insufficient In The Center Of Encap [Insufficient]", bRegionInsp);
				}

				HObject hDBLeftInsufficientMask;
				AffineTransRegion(m_TrainingData.hDBLeftInsufficientMask, &hDBLeftInsufficientMask, hHomMat2DIdentity, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hDBLeftInsufficientMask, colorGreen, "Mask Using To Find Insufficient In The Left Edge Of Encap [Insufficient]", bRegionInsp);
				}

				HObject hDBRightInsufficientMask;
				AffineTransRegion(m_TrainingData.hDBRightInsufficientMask, &hDBRightInsufficientMask, hHomMat2DIdentity, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hDBRightInsufficientMask, colorGreen, "Mask Using To Find Insufficient In The Right Edge Of Encap [Insufficient]", bRegionInsp);
				}

				HTuple hDBWhiteContrastIS = m_EncapParm.nDBWhiteContrastIS;
				HTuple hDBBlackContrastIS = m_EncapParm.nDBBlackContrastIS;
				HTuple hDBEdgeContrastIS = m_EncapParm.nDBEdgeContrastIS;
				HTuple hDBInnerEdgeOffsetIS = m_EncapParm.dDBInnerEdgeOffsetIS / dResolution;
				HTuple hDBLeftEdgeOffsetIS = m_EncapParm.dDBLeftEdgeOffsetIS / dResolution;
				HTuple hDBRightEdgeOffsetIS = m_EncapParm.dDBRightEdgeOffsetIS / dResolution;
				HTuple hDBMinDiffMeanGrayIS = m_EncapParm.nDBMinDiffMeanGrayIS;
				HTuple hDBMinInnerAreaIS = m_EncapParm.dDBMinInnerAreaIS / (dResolution*dResolution);
				HTuple hDBMinSmoothAreaPercentIS = m_EncapParm.dDBMinSmoothAreaPercentIS;
				HTuple hDBMinEdgePercentIS = m_EncapParm.dDBMinEdgePercentIS;

				HObject hDBEncapInsufficientDefectRegion;
				_HP_Inspect_DarkBlueEncap_Insufficient(hImage,
					hDBCoverInsufficientMask,
					hDBInnerInsufficientMask,
					hDBLeftInsufficientMask,
					hDBRightInsufficientMask,
					&hDebugImage,
					&hDebugRegion,
					&hDBEncapInsufficientDefectRegion,
					nStepDebug,
					hDBWhiteContrastIS,
					hDBBlackContrastIS,
					hDBInnerEdgeOffsetIS,
					hDBMinDiffMeanGrayIS,
					hDBMinInnerAreaIS,
					hDBLeftEdgeOffsetIS,
					hDBRightEdgeOffsetIS,
					hDBEdgeContrastIS,
					hDBMinSmoothAreaPercentIS,
					hDBMinEdgePercentIS,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hDBEncapInsufficientDefectRegion, colorRed, "Insufficient Defect Region");
					return -ENCAP_INSUFFICIENT;
				}

				// Glue - Mason
				HObject hDBEncapGlueMask;
				AffineTransRegion(m_TrainingData.hDBEncapGlueMask, &hDBEncapGlueMask, hHomMat2DIdentity, "nearest_neighbor");
				if (nStepDebug) {
					StepDebug(hImage, hDBEncapGlueMask, colorGreen, "Mask Using To Find Glue Defect Region [Glue]", bRegionInsp);
				}

				HTuple hDBContrastGlue = m_EncapParm.nDBContrastGlue;
				HTuple hDBEncapMaskOffsetGlue = m_EncapParm.dDBEncapMaskOffsetGlue / dResolution;
				HTuple hDBFlexMaskOffsetGlue = m_EncapParm.dDBFlexMaskOffsetGlue / dResolution;
				HTuple hDBSmoothMaskSizeGlue = m_EncapParm.dDBSmoothMaskSizeGlue / dResolution;
				HTuple hDBMinSizeGlue = m_EncapParm.dDBMinSizeGlue / (dResolution*dResolution);
				HTuple hDBMinConvexityGlue = m_EncapParm.dDBMinConvexityGlue;

				HObject hDBEncapGlueDefectRegion;
				_HP_Inspect_DarkBuleEncap_Glue(hImage,
					hDBEncapFlexEdgeMaskROI,
					hDBEncapExtractROI,
					hDBEncapGlueMask,
					&hDebugImage,
					&hDebugRegion,
					&hDBEncapGlueDefectRegion,
					nStepDebug,
					hDBEncapMaskOffsetGlue,
					hDBContrastGlue,
					hDBSmoothMaskSizeGlue,
					hDBMinSizeGlue,
					hDBMinConvexityGlue,
					hDBFlexMaskOffsetGlue,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hDBEncapGlueDefectRegion, colorRed, "Glue Defect Region");
					return -ENCAP_GLUE;
				}

				// Contamination - Mason
				HObject hDBEncapDieMaskCT;
				GenEmptyObj(&hDBEncapDieMaskCT);

				HTuple hDBMaskSizeCT = m_EncapParm.dDBMaskSizeCT / dResolution;
				HTuple hDBStandardDeviationCT = m_EncapParm.dDBStandardDeviationCT;
				HTuple hDBDiffMeanGrayCT = m_EncapParm.nDBDiffMeanGrayCT;
				HTuple hDBBottomMaskContrastCT = m_EncapParm.nDBBottomMaskContrastCT;
				HTuple hDBFlexMaskContrastCT = m_EncapParm.nDBFlexMaskContrastCT;
				HTuple hDBMinGrayCT = m_EncapParm.nDBMinGrayCT;
				HTuple hDBMinSizeCT = m_EncapParm.dDBMinSizeCT / (dResolution*dResolution);
				HTuple hDBMinDiffMeanGrayCT = m_EncapParm.nDBMinDiffMeanGrayCT;
				HTuple hDBMinMeanEdgeMaskGrayCT = m_EncapParm.nDBMinMeanEdgeMaskGrayCT;
				HTuple hDBMinMeanFlexMaskGrayCT = m_EncapParm.nDBMinMeanFlexMaskGrayCT;
				HTuple hDBMinMeanBorderedGrayCT = m_EncapParm.nDBMinMeanBorderedGrayCT;
				HTuple hDBMaxDiffAreaPercentCT = m_EncapParm.dDBMaxDiffAreaPercentCT;
				HTuple hDBMinDefectCountCT = m_EncapParm.nDBMinDefectCountCT;

				HObject hDBEncapContaminationDefectRegion;
				_HP_Inspect_DarkBlueEncap_Contamination(hImage,
					hEncapRegion,
					hDBEncapBottomEdgeMaskROI,
					hDBEncapFlexEdgeMaskROI,
					hDBEncapDieMaskCT,
					&hDebugImage,
					&hDebugRegion,
					&hDBEncapContaminationDefectRegion,
					nStepDebug,
					hDBMaskSizeCT,
					hDBStandardDeviationCT,
					hDBDiffMeanGrayCT,
					hDBBottomMaskContrastCT,
					hDBFlexMaskContrastCT,
					hDBMinGrayCT,
					hDBMinSizeCT,
					hDBMinDiffMeanGrayCT,
					hDBMinMeanEdgeMaskGrayCT,
					hDBMinMeanFlexMaskGrayCT,
					hDBMinMeanBorderedGrayCT,
					hDBMaxDiffAreaPercentCT,
					hDBMinDefectCountCT,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hDBEncapContaminationDefectRegion, colorRed, "Contamination Defect Region");
					return -ENCAP_CONTAMINATION;
				}

				// Void (Dark Blue Encap)

				// // Region for search Void
				HRegion hDBEncapVoidROITemp, hDBEncapVoidROI;
				int nRow1 = m_TrainingData.m_rectDBlueVoidROI.top;
				int nCol1 = m_TrainingData.m_rectDBlueVoidROI.left;
				int nRow2 = m_TrainingData.m_rectDBlueVoidROI.bottom;
				int nCol2 = m_TrainingData.m_rectDBlueVoidROI.right;
				GenRectangle1(&hDBEncapVoidROITemp, nRow1, nCol1, nRow2, nCol2);
				AffineTransRegion(hDBEncapVoidROITemp, &hDBEncapVoidROI, hHomMat2DIdentity, "nearest_neighbor");
				if (nStepDebug)
				{
					HTuple hMsg = "Region for Void Inspection [Void]";
					StepDebug(hImage, hDBEncapVoidROI, colorGreen, hMsg, bRegionInsp);
				}
			
				double pixel = ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hDBlueVoidScaleContrastEncap = m_EncapParm.nDBlueVoidScaleContrastEncap;
				HTuple hDBlueValueContrastDefect = m_EncapParm.nDBlueVoidValueContrastDefect;
				HTuple hDBlueScaleContrastDefect = m_EncapParm.nDBlueVoidScaleContrastDefect;
				HTuple hDBlueWidthLocalSearch = m_EncapParm.dDBlueVoidWidthLocalSearch / pixel;
				HTuple hDBlueHeightLocalSearch = m_EncapParm.dDBlueVoidHeightLocalSearch/ pixel;
				HTuple hDBlueOffsetEncap = m_EncapParm.dDBlueVoidOffsetEncap / pixel;
				HTuple hDBlueClosingRadiusDefect = m_EncapParm.dDBlueVoidClosingRadiusDefect / pixel;
				HTuple hDBlueMinAreaDefect = m_EncapParm.dDBlueVoidMinAreaDefect /pixel / pixel;
				HTuple hDBlueCircularityDefect = m_EncapParm.nDBlueVoidCircularityDefect/100.0;
				HTuple hDBlueHWRatioDefect = m_EncapParm.nDBlueVoidHWRatioDefect;
				HTuple hDBlueInnerDefectPercent = m_EncapParm.nDBlueVoidInnerDefectPercent;
				HTuple hDBlueOffsetCheckDefect = m_EncapParm.nDBlueVoidOffsetCheckDefect;
				HTuple hDBlueMaxContrastInOutDefect = m_EncapParm.nDBlueVoidMaxContrastInOutDefect;
				HTuple hDBlueMinGrayDefect = m_EncapParm.nDBlueVoidMinGrayDefect;
				HTuple hDBlueMaxGrayDefect = m_EncapParm.nDBlueVoidMaxGrayDefect;
				HTuple hDBlueMaxDeviationInnerDefect =  m_EncapParm.nDBlueVoidMaxDeviationInnerDefect; //End magnus

				// Void Dark blue encap Inspection
				HTuple IsPass;
				HTuple DebugMessageOut;
				HImage DebugImageOut;
				HRegion DebugRegionOut;
				HRegion hVoidDefectsRegion;
				_HP_Inspect_DarkBlueEncap_Void(hImage, hEncapRegion,
					hDBEncapVoidROI, &DebugImageOut, &DebugRegionOut,
					&hVoidDefectsRegion, hDBlueVoidScaleContrastEncap, hDBlueValueContrastDefect,
					hDBlueScaleContrastDefect, hDBlueWidthLocalSearch, hDBlueHeightLocalSearch,
					hDBlueOffsetEncap, hDBlueClosingRadiusDefect, hDBlueMinAreaDefect,
					hDBlueCircularityDefect, hDBlueHWRatioDefect, hDBlueInnerDefectPercent,
					hDBlueOffsetCheckDefect, hDBlueMaxContrastInOutDefect, hDBlueMinGrayDefect,
					hDBlueMaxGrayDefect, hDBlueMaxDeviationInnerDefect, nStepDebug,
					&DebugMessageOut, &IsPass);
				if (nStepDebug) {
					StepDebug(DebugImageOut, DebugRegionOut, colorGreen, DebugMessageOut, bRegionInsp);
				}
				if (IsPass ==FALSE)
				{
					m_arrayOverlayInspection.Add(hVoidDefectsRegion, colorRed, "Void Region");
					return -ENCAP_VOID;
				}
				//End dark Blue Void Inspection				// Inspect BlowHole 
				HObject hDarkBlueEncapBHMaskingLeft;
				HObject hDarkBlueEncapBHMaskingRight;
				HObject hDarkBlueEncapWireBottom;
				HObject hDarkBlueEncapIC;

				AffineTransRegion(m_TrainingData.hDarkBlueEncapBHMaskingLeft, &hDarkBlueEncapBHMaskingLeft, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDarkBlueEncapBHMaskingRight, &hDarkBlueEncapBHMaskingRight, hHomMat2DIdentity, "nearest_neighbor");
				
				HTuple hDarkBlueErosionRecHeight = m_EncapParm.dDarkBlueErosionRecHeight / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);

				HTuple hDarkBlueMinMaskSizeSmallBH = m_EncapParm.dDarkBlueMinMaskSizeSmallBH / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hDarkBlueMinSizeSmallBH = m_EncapParm.dDarkBlueMinSizeSmallBH / (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));
				;
				HTuple hDarkBlueMinCircularitySmallBH = m_EncapParm.dDarkBlueMinCircularitySmallBH;
				HTuple hDarkBlueMaxLayOnPartPercentageAllowSmallBH = m_EncapParm.dDarkBlueMaxLayOnPartPercentageAllowSmallBH;

				HTuple hDarkBlueMinMaskSizeLargeBH = m_EncapParm.dDarkBlueMinMaskSizeLargeBH / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hDarkBlueMinSizeLargeBH = m_EncapParm.dDarkBlueMinSizeLargeBH / (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));
				;
				HTuple hDarkBlueMinCircularityLargeBH = m_EncapParm.dDarkBlueMinCircularityLargeBH;
				HTuple hDarkBlueMaxLayOnPartPercentageAllowLargeBH = m_EncapParm.dDarkBlueMaxLayOnPartPercentageAllowLargeBH;

				HRegion hDarkBlueEncapRegionForBH;
				HImage hDarkBlueBHDebugImageOut;
				HRegion hDarkBlueBHDebugRegionOut, hBHDefectRegion;
				HTuple hDarkBlueBHDebugMessageOut;

				Difference(hEncapRegion, hDBEncapMaskTracingROI, &hDarkBlueEncapRegionForBH);
				ErosionCircle(hDarkBlueEncapRegionForBH,&hDarkBlueEncapRegionForBH,5); 

				_HP_Inspect_DarkBlueEncap_BlowHole(hDarkBlueEncapRegionForBH, hImage, hDarkBlueEncapBHMaskingLeft, hDarkBlueEncapBHMaskingRight,
					&hDarkBlueBHDebugImageOut, &hDarkBlueBHDebugRegionOut, &hBHDefectRegion, hDarkBlueErosionRecHeight,
					hDarkBlueMinMaskSizeSmallBH, hDarkBlueMinSizeSmallBH, hDarkBlueMinCircularitySmallBH,
					hDarkBlueMaxLayOnPartPercentageAllowSmallBH, hDarkBlueMinMaskSizeLargeBH,
					hDarkBlueMinSizeLargeBH, hDarkBlueMinCircularityLargeBH, hDarkBlueMaxLayOnPartPercentageAllowLargeBH, \
					nStepDebug, &hDarkBlueBHDebugMessageOut, &hIsPass);

				if (nStepDebug) {
					StepDebug(hDarkBlueBHDebugImageOut, hDarkBlueBHDebugRegionOut, colorGreen, hDarkBlueBHDebugMessageOut, bRegionInsp);
				}

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hBHDefectRegion, colorRed);
					return -ENCAP_BLOWHOLE;
				}

				//Wire And IC
				AffineTransRegion(m_TrainingData.hDarkBlueEncapWireBottom, &hDarkBlueEncapWireBottom, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hDarkBlueEncapIC, &hDarkBlueEncapIC, hHomMat2DIdentity, "nearest_neighbor");

				HTuple hDarkBlueEncapLightWireMaxLineWidthWAI = m_EncapParm.dDarkBlueEncapLightWireMaxLineWidthWAI / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hDarkBlueEncapLightWireContrastWAI = m_EncapParm.nDarkBlueEncapLightWireContrastWAI;
				HTuple hDarkBlueEncapLightWireMinAreaWAI = m_EncapParm.dDarkBlueEncapLightWireMinAreaWAI / (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));
				HTuple hDarkBlueEncapLightWireMinLengthWAI = m_EncapParm.dDarkBlueEncapLightWireMinLengthWAI / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hDarkBlueEncapLightWireMinMeanGVDiffWAI = m_EncapParm.dDarkBlueEncapLightWireMinMeanGVDiffWAI;

				HTuple hDarkBlueEncapFuzzyWireMaxLineWidthWAI = m_EncapParm.dDarkBlueEncapFuzzyWireMaxLineWidthWAI / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hDarkBlueEncapFuzzyWireContrastWAI = m_EncapParm.nDarkBlueEncapFuzzyWireContrastWAI;
				HTuple hDarkBlueEncapFuzzyWireMinAreaWAI = m_EncapParm.dDarkBlueEncapFuzzyWireMinAreaWAI / (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));
				HTuple hDarkBlueEncapFuzzyWireMinLengthWAI = m_EncapParm.dDarkBlueEncapFuzzyWireMinLengthWAI / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
				HTuple hDarkBlueEncapFuzzyWireMinMeanGVDiffWAI = m_EncapParm.dDarkBlueEncapFuzzyWireMinMeanGVDiffWAI;
				HTuple hDarkBlueEncapICMinAreaWAI = m_EncapParm.dDarkBlueEncapICMinAreaWAI / (((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2)* ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2));

				HRegion hDarkBlueEncapRegionForWAI;
				HImage hWAIDebugImageOut;
				HRegion hWAIDebugRegionOut, hWAIDefectRegion;
				HTuple hWAIDebugMessageOut;

				Difference(hEncapRegion, hDBEncapMaskTracingROI, &hDarkBlueEncapRegionForWAI);

				_HP_Inspect_DarkBlueEncap_WireAndIC(hImage, hEncapRegion, hDarkBlueEncapWireBottom,
					hDBEncapMaskTracingROI, hDarkBlueEncapIC,
					&hWAIDebugImageOut, &hWAIDebugRegionOut, &hWAIDefectRegion,
					hDarkBlueEncapLightWireMinAreaWAI, hDarkBlueEncapLightWireMaxLineWidthWAI, hDarkBlueEncapLightWireContrastWAI,
					hDarkBlueEncapLightWireMinLengthWAI, hDarkBlueEncapLightWireMinMeanGVDiffWAI, hDarkBlueEncapFuzzyWireMaxLineWidthWAI,
					hDarkBlueEncapFuzzyWireContrastWAI, hDarkBlueEncapFuzzyWireMinLengthWAI, hDarkBlueEncapFuzzyWireMinAreaWAI,
					hDarkBlueEncapFuzzyWireMinMeanGVDiffWAI, hDarkBlueEncapICMinAreaWAI, nStepDebug,
					&hWAIDebugMessageOut, &hIsPass);

				if (nStepDebug) {
					StepDebug(hWAIDebugImageOut, hWAIDebugRegionOut, colorGreen, hWAIDebugMessageOut, bRegionInsp);
				}

				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hWAIDefectRegion, colorRed);
					return -ENCAP_WIREANDIC;
				}			
			}
		}

		//// ********** Contact Insp ********** ////
		if (m_TrainingData.bEnableContact) {
			// Black Contact Inspection
			if (m_ContactParm.bEnableBlackContact) {
				if (!m_TrainingData.bEnableBlackContact)
					return -CONTACT_DAMAGE;

				// // Contact: Tracing
				HTuple hIsPass = TRUE;

				HObject hContactMask;
				HObject hContactRegionMask;
				HObject hContactTracingMask;

				// Using For Region Projection
				HTuple hMatchPatternAngle = hDatumCenterRotationAngle;
				HTuple hMatchPatternRow = hDatumCenterRow;
				HTuple hMatchPatternCol = hDatumCenterCol;

				HTuple hRotationAngle = hInspectRotationAngle;
				HTuple hShiftAlongRow = hInspectShiftAlongRow;
				HTuple hShiftAlongCol = hInspectShiftAlongCol;

				HTuple hRefPatternAngle = hRotationAngle + hMatchPatternAngle;
				HTuple hRefPatternRow = hShiftAlongRow + hMatchPatternRow;
				HTuple hRefPatternCol = hShiftAlongCol + hMatchPatternCol;

				// Teach Parameters
				HTuple hBContactContactContast = m_TrainingData.nBContactContactContrast;
				HTuple hBContactCircleContrast = m_TrainingData.nBContactCircleContrast;

				// Region Projection
				HTuple hHomMat2DIdentity;
				HomMat2dIdentity(&hHomMat2DIdentity);
				HomMat2dRotate(hHomMat2DIdentity, -hRotationAngle, hRefPatternRow, hRefPatternCol, &hHomMat2DIdentity);
				HomMat2dTranslate(hHomMat2DIdentity, -hShiftAlongRow, -hShiftAlongCol, &hHomMat2DIdentity);
				AffineTransRegion(m_TrainingData.hContactMask, &hContactMask, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hContactRegionMask, &hContactRegionMask, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hContactTracingMask, &hContactTracingMask, hHomMat2DIdentity, "nearest_neighbor");

				if (nStepDebug) {
					HObject hContactMaskDebug;
					HObject hContactRegionMaskDebug;
					Union1(hContactMask, &hContactMaskDebug);
					Union1(hContactRegionMask, &hContactRegionMaskDebug);
					StepDebug(hImage, hContactMaskDebug, colorGreen, "Mask Using To Tracing Contact Region [Tracing]", bRegionInsp);
					StepDebug(hImage, hContactRegionMaskDebug, colorGreen, "Mask Using To Check Area Percent After Tracing [Tracing]", bRegionInsp);
					StepDebug(hImage, hContactTracingMask, colorYellow, "Mask Using To Remove Circle Region Inside Contact Region [Tracing]", bRegionInsp);
				}

				// Tracing
				HObject hContactRegion;
				HObject hCircleRegionDebug;

				_HP_BlackContact_Tracing(hImage,
					hContactMask,
					hContactTracingMask,
					&hDebugImage,
					&hDebugRegion,
					&hContactRegion,
					&hCircleRegionDebug,
					nStepDebug,
					hRotationAngle,
					hMatchPatternRow,
					hMatchPatternCol,
					hBContactContactContast,
					hBContactCircleContrast, &hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				// Check Result And Display Image Region If Get Error
				if (hIsPass == FALSE) {
					HObject RegionImage;
					GetDomain(hImage, &RegionImage);
					m_arrayOverlayInspection.Add(RegionImage, colorRed, "Contact Tracing Failed");
					return -CONTACT_DAMAGE;
				}

				// Display Contact Location When Inspecting
				m_arrayOverlayInspection.Add(hContactRegion, colorGreen, "Contact Region");
				m_arrayOverlayInspection.Add(hCircleRegionDebug, colorGreen, "Circle Region Removed");
				
				// // Contact: Damage
				// Get Vision Parameters
				double dResolution = (pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2; // Resolution: Microns/Pixel

				HTuple hBContactMaskSizeDM = m_ContactParm.dBContactMaskSizeDM / dResolution;
				HTuple hBContactContactRegionOffsetDM = m_ContactParm.dBContactContactRegionOffsetDM / dResolution;
				HTuple hBContactMaxLineWidthDM = m_ContactParm.dBContactMaxLineWidthDM / dResolution;
				HTuple hBContactLowContrastDM = m_ContactParm.nBContactLowContrastDM;
				HTuple hBContactHighContrastDM = m_ContactParm.nBContactHighContrastDM;
				HTuple hBContactMinLengthDM = m_ContactParm.dBContactMinLengthDM / dResolution;
				HTuple hBContactMaxCircularityDM = m_ContactParm.dBContactMaxCircularityDM;
				HTuple hBContactMinDefectCountDM = m_ContactParm.nBContactMinDefectCountDM;

				// Inspect
				HObject hBContactDamageDefectRegion;
				_HP_Inspect_BlackContact_Damage(hImage,
					hContactRegion,
					&hDebugImage,
					&hDebugRegion,
					&hBContactDamageDefectRegion,
					nStepDebug,
					hBContactMaskSizeDM,
					hBContactContactRegionOffsetDM,
					hBContactMaxLineWidthDM,
					hBContactHighContrastDM,
					hBContactLowContrastDM,
					hBContactMinLengthDM,
					hBContactMaxCircularityDM,
					hBContactMinDefectCountDM,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}
				// Check Result And Display Error
				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hBContactDamageDefectRegion, colorRed, "Contact Damage Defect Region");
					return -CONTACT_DAMAGE;
				}

				// // Contact: Contamination
				// Get Vision Parameters
				HTuple hBContactHighContrastCT = m_ContactParm.nBContactHighContrastCT;
				HTuple hBContactSmoothImageCT = m_ContactParm.dBContactSmoothImageCT;
				HTuple hBContactContactRegionOffsetCT = m_ContactParm.dBContactContactRegionOffsetCT / dResolution;
				HTuple hBContactContrastOffsetCT = m_ContactParm.nBContactContrastOffsetCT;
				HTuple hBContactMinHoleNumberCT = m_ContactParm.nBContactMinHoleNumberCT;
				HTuple hBContactHoleNumberContrastOffsetCT = m_ContactParm.nBContactHoleNumberContrastOffsetCT;
				HTuple hBContactVeryHighRoughDotRegionCT = m_ContactParm.nBContactVeryHighRoughDotRegionCT;
				HTuple hBContactHighRoughDotRegionCT = m_ContactParm.nBContactHighRoughDotRegionCT;
				HTuple hBContactVeryHighRoughContrastOffsetCT = m_ContactParm.nBContactVeryHighRoughContrastOffsetCT;
				HTuple hBContactHighRoughContrastOffsetCT = m_ContactParm.nBContactHighRoughContrastOffsetCT;
				HTuple hBContactMinStainSizeCT = m_ContactParm.dBContactMinStainSizeCT / (dResolution*dResolution);
				HTuple hBContactMinMeanEdgeGrayCT = m_ContactParm.nBContactMinMeanEdgeGrayCT;
				HTuple hBContactMinSizeCT = m_ContactParm.dBContactMinSizeCT / (dResolution*dResolution);
				HTuple hBContactMinMeanGrayCT = m_ContactParm.nBContactMinMeanGrayCT;
				HTuple hBContactMinAreaTracingPercentCT = m_ContactParm.dBContactMinAreaTracingPercentCT;
				HTuple hBContactMinDefectCountCT = m_ContactParm.nBContactMinDefectCountCT;

				// Inspect
				HObject hBContactContaminationDefectRegion;
				_HP_Inspect_BlackContact_Contamination(hImage,
					hContactRegion,
					hContactRegionMask,
					&hDebugImage,
					&hDebugRegion,
					&hBContactContaminationDefectRegion,
					nStepDebug,
					hBContactHighContrastCT,
					hBContactSmoothImageCT,
					hBContactContrastOffsetCT,
					hBContactMinHoleNumberCT,
					hBContactHoleNumberContrastOffsetCT,
					hBContactVeryHighRoughDotRegionCT,
					hBContactVeryHighRoughContrastOffsetCT,
					hBContactHighRoughDotRegionCT,
					hBContactHighRoughContrastOffsetCT,
					hBContactMinSizeCT,
					hBContactContactRegionOffsetCT,
					hBContactMinMeanEdgeGrayCT,
					hBContactMinSizeCT,
					hBContactMinMeanGrayCT,
					hBContactMinAreaTracingPercentCT,
					hBContactMinDefectCountCT,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}
				// Check Result And Display Error
				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hBContactContaminationDefectRegion, colorRed, "Contact Contamination Defect Region");
					return -CONTACT_CONTAMINATION;
				}
			}
			else if (m_ContactParm.bEnableWhiteContact) {
				if (!m_TrainingData.bEnableWhiteContact)
					return -CONTACT_DAMAGE;

				// // Contact: Tracing
				HTuple hIsPass = TRUE;

				HObject hContactMask;
				HObject hContactRegionMask;
				HObject hContactTracingMask;

				// Using For Region Projection
				HTuple hMatchPatternAngle = hDatumCenterRotationAngle;
				HTuple hMatchPatternRow = hDatumCenterRow;
				HTuple hMatchPatternCol = hDatumCenterCol;

				HTuple hRotationAngle = hInspectRotationAngle;
				HTuple hShiftAlongRow = hInspectShiftAlongRow;
				HTuple hShiftAlongCol = hInspectShiftAlongCol;

				HTuple hRefPatternAngle = hRotationAngle + hMatchPatternAngle;
				HTuple hRefPatternRow = hShiftAlongRow + hMatchPatternRow;
				HTuple hRefPatternCol = hShiftAlongCol + hMatchPatternCol;

				// Teach Parameters
				HTuple hWContactContactContast = m_TrainingData.nWContactContactContrast;
				HTuple hWContactCircleContrast = m_TrainingData.nWContactCircleContrast;

				// Region Projection
				HTuple hHomMat2DIdentity;
				HomMat2dIdentity(&hHomMat2DIdentity);
				HomMat2dRotate(hHomMat2DIdentity, -hRotationAngle, hRefPatternRow, hRefPatternCol, &hHomMat2DIdentity);
				HomMat2dTranslate(hHomMat2DIdentity, -hShiftAlongRow, -hShiftAlongCol, &hHomMat2DIdentity);
				AffineTransRegion(m_TrainingData.hContactMask, &hContactMask, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hContactRegionMask, &hContactRegionMask, hHomMat2DIdentity, "nearest_neighbor");
				AffineTransRegion(m_TrainingData.hContactTracingMask, &hContactTracingMask, hHomMat2DIdentity, "nearest_neighbor");

				if (nStepDebug) {
					HObject hContactMaskDebug;
					HObject hContactRegionMaskDebug;
					Union1(hContactMask, &hContactMaskDebug);
					Union1(hContactRegionMask, &hContactRegionMaskDebug);
					StepDebug(hImage, hContactMaskDebug, colorGreen, "Mask Using To Tracing Contact Region [Tracing]", bRegionInsp);
					StepDebug(hImage, hContactRegionMaskDebug, colorGreen, "Mask Using To Check Area Percent After Tracing [Tracing]", bRegionInsp);
					StepDebug(hImage, hContactTracingMask, colorYellow, "Mask Using To Remove Circle Region Inside Contact Region [Tracing]", bRegionInsp);
				}

				// Tracing
				HObject hContactRegion;
				HObject hCircleRegionDebug;

				_HP_WhiteContact_Tracing(hImage,
					hContactMask,
					hContactTracingMask,
					&hDebugImage,
					&hDebugRegion,
					&hContactRegion,
					&hCircleRegionDebug,
					nStepDebug,
					hRotationAngle,
					hMatchPatternRow,
					hMatchPatternCol,
					hWContactContactContast,
					hWContactCircleContrast, &hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}

				// Check Result And Display Image Region If Get Error
				if (hIsPass == FALSE) {
					HObject RegionImage;
					GetDomain(hImage, &RegionImage);
					m_arrayOverlayInspection.Add(RegionImage, colorRed, "Contact Tracing Failed");
					return -CONTACT_DAMAGE;
				}

				// Display Contact Location When Inspecting
				m_arrayOverlayInspection.Add(hContactRegion, colorGreen, "Contact Region");
				m_arrayOverlayInspection.Add(hCircleRegionDebug, colorGreen, "Circle Region Removed");

				// // Contact: Damage
				// Get Vision Parameters
				double dResolution = (pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2; // Resolution: Microns/Pixel

				HTuple hWContactMaskSizeDM = m_ContactParm.dWContactMaskSizeDM / dResolution;
				HTuple hWContactContactRegionOffsetDM = m_ContactParm.dWContactContactRegionOffsetDM / dResolution;
				HTuple hWContactMaxLineWidthDM = m_ContactParm.dWContactMaxLineWidthDM / dResolution;
				HTuple hWContactLowContrastDM = m_ContactParm.nWContactLowContrastDM;
				HTuple hWContactHighContrastDM = m_ContactParm.nWContactHighContrastDM;
				HTuple hWContactMinLengthDM = m_ContactParm.dWContactMinLengthDM / dResolution;
				HTuple hWContactMaxCircularityDM = m_ContactParm.dWContactMaxCircularityDM;
				HTuple hWContactMinDefectCountDM = m_ContactParm.nWContactMinDefectCountDM;

				// Inspect
				HObject hWContactDamageDefectRegion;
				_HP_Inspect_WhiteContact_Damage(hImage,
					hContactRegion,
					&hDebugImage,
					&hDebugRegion,
					&hWContactDamageDefectRegion,
					nStepDebug,
					hWContactMaskSizeDM,
					hWContactContactRegionOffsetDM,
					hWContactMaxLineWidthDM,
					hWContactHighContrastDM,
					hWContactLowContrastDM,
					hWContactMinLengthDM,
					hWContactMaxCircularityDM,
					hWContactMinDefectCountDM,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}
				// Check Result And Display Error
				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hWContactDamageDefectRegion, colorRed, "Contact Damage Defect Region");
					return -CONTACT_DAMAGE;
				}

				// // Contact: Contamination
				// Get Vision Parameters
				HTuple hWContactSmoothImageCT = m_ContactParm.dWContactSmoothImageCT;
				HTuple hWContactContactRegionOffsetCT = m_ContactParm.dWContactContactRegionOffsetCT / dResolution;
				HTuple hWContactWhiteDefectContrastOffsetCT = m_ContactParm.nWContactWhiteDefectContrastOffsetCT;
				HTuple hWContactMeanWhiteDefectGrayOffsetCT = m_ContactParm.nWContactMeanWhiteDefectGrayOffsetCT;
				HTuple hWContactMinMeanWhiteDefectGrayCT = m_ContactParm.nWContactMinMeanWhiteDefectGrayCT;
				HTuple hWContactBlackDefectContrastOffsetCT = m_ContactParm.nWContactBlackDefectContrastOffsetCT;
				HTuple hWContactMeanBlackDefectGrayOffsetCT = m_ContactParm.nWContactMeanBlackDefectGrayOffsetCT;
				HTuple hWContactMaxMeanBlackDefectGrayCT = m_ContactParm.nWContactMaxMeanBlackDefectGrayCT;
				HTuple hWContactMinSizeCT = m_ContactParm.dWContactMinSizeCT / (dResolution*dResolution);
				HTuple hWContactMinAreaTracingPercentCT = m_ContactParm.dWContactMinAreaTracingPercentCT;
				HTuple hWContactMinDefectCountCT = m_ContactParm.nWContactMinDefectCountCT;

				// Inspect
				HObject hWContactContaminationDefectRegion;
				_HP_Inspect_WhiteContact_Contamination(hImage,
					hContactRegion,
					hContactRegionMask,
					&hDebugImage,
					&hDebugRegion,
					&hWContactContaminationDefectRegion,
					nStepDebug,
					hWContactSmoothImageCT,
					hWContactContactRegionOffsetCT,
					hWContactWhiteDefectContrastOffsetCT,
					hWContactMeanWhiteDefectGrayOffsetCT,
					hWContactBlackDefectContrastOffsetCT,
					hWContactMeanBlackDefectGrayOffsetCT,
					hWContactMinMeanWhiteDefectGrayCT,
					hWContactMaxMeanBlackDefectGrayCT,
					hWContactMinSizeCT,
					hWContactMinAreaTracingPercentCT,
					hWContactMinDefectCountCT,
					&hIsPass,
					&hDebugMsg);

				if (nStepDebug) {
					StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
				}
				// Check Result And Display Error
				if (hIsPass == FALSE) {
					m_arrayOverlayInspection.Add(hWContactContaminationDefectRegion, colorRed, "Contact Contamination Defect Region");
					return -CONTACT_CONTAMINATION;
				}
			}
		}
		
		//// ********** Flex Insp ********** ////
		if (m_FlexParm.bEnable) {
			if (!m_TrainingData.bFlexEnable)
				return -FLEX_OVERCUT;

			// Using Timer To Calculate Total Time
			CPerformanceTimer Timer;
			double dTime1 = Timer.GetElapsedMSec();

			HTuple hIsPass = TRUE;

			// ROI For Inspect
			HObject hFlexSubstrateSearchPatternROI;
			HObject hFlexTopEdgeLine;
			HObject hFlexBottomEdgeLine;
			HObject hFlexLeftEdgeLine;
			HObject hFlexRightEdgeLine;
			HObject hFlexLeftEdgeLineMask;
			HObject hFlexRightEdgeLineMask;
			HObject hFlexBottomMask;
			HObject hFlexStraightCircuitLineMask;
			HObject hFlexNumberMask;
			HObject hFlexTracingMask;
			HObject hFlexDamageMask;
			HObject hFlexSubstrateMask;
			
			HTuple hFlexSubstrateModelID = m_TrainingData.hFlexSubstrateModelID;
			HTuple hFlexSubstrateDatumMatchRowRef = m_TrainingData.hFlexSubstrateDatumMatchRowRef;
			HTuple hFlexSubstrateDatumMatchColRef = m_TrainingData.hFlexSubstrateDatumMatchColRef;
			HTuple hFlexSubstrateMatchAngleRef = m_TrainingData.hFlexSubstrateMatchAngleRef;
			HObject hFlexTracingMaskRegion = m_TrainingData.hFlexTracingMaskRegion;
			HTuple hFlexLeftWidthReference = m_TrainingData.hFlexLeftWidthReference;
			HTuple hFlexRightWidthReference = m_TrainingData.hFlexRightWidthReference;

			// Using For Flex Region Projection
			HTuple hMatchAngle = hDatumCenterRotationAngle;
			HTuple hDatumMatchRow = hDatumCenterRow;
			HTuple hDatumMatchCol = hDatumCenterCol;

			HTuple hRotationAngle = hInspectRotationAngle;
			HTuple hShiftAlongRow = hInspectShiftAlongRow;
			HTuple hShiftAlongCol = hInspectShiftAlongCol;

			HTuple hMatchAngleRef = hRotationAngle + hMatchAngle;
			HTuple hDatumMatchRowRef = hShiftAlongRow + hDatumMatchRow;
			HTuple hDatumMatchColRef = hShiftAlongCol + hDatumMatchCol;

			// Resolution: Microns/Pixel		
			double dResolution = (pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2;

			// Vision Parameters
			HTuple hFlexEnableSubstrateMask = m_TrainingData.bFlexEnableSubstrateMask;
			HTuple hFlexEnableDynamicEdge = m_TrainingData.bFlexEnableDynamicEdge;
			HTuple hEdgeOffset = m_TrainingData.dFlexEdgeOffset / dResolution;
			HTuple hMinEdgeContrast = m_TrainingData.nFlexEdgeContrast;
			HTuple hMinCircuitLineContrast = m_TrainingData.nFlexCircuitLineContrast;
			HTuple hMinEncapContrast = m_TrainingData.nFlexEncapContrast;	

			HTuple hEnableCircuitLineCutInspection = m_FlexParm.bEnableCircuitLineCutInspection;
			HTuple hEnableCircuitLineLinkedInspection = m_FlexParm.bEnableCircuitLineLinkedInspection;
			HTuple hMinCircuitLineWidthOV = m_FlexParm.dMinCircuitLineWidthOV / dResolution;
			HTuple hMaxCircuitLineWidthOV = m_FlexParm.dMaxCircuitLineWidthOV / dResolution;

			HTuple bEnableCircuitLineDamageInspection = m_FlexParm.bEnableCircuitLineDamageInspection;
			HTuple hMaxDefectContrastDM = m_FlexParm.nMaxContrastDM;
			HTuple hMinSizeDM = m_FlexParm.dMinSizeDM / (dResolution * dResolution);
			HTuple hMinDefectCountDM = m_FlexParm.nMinDefectCountDM;

			HTuple hMinDefectContrastCT = m_FlexParm.nMinContrastCT;
			HTuple hMinDefectSubstrateContrastCT = m_FlexParm.nMinSubstrateContrastCT;
			HTuple hMinSizeCT = m_FlexParm.dMinSizeCT / (dResolution * dResolution);
			HTuple hMinSquareSizeCT = m_FlexParm.dMinSquareSizeCT / dResolution;
			HTuple hMinLengthCT = m_FlexParm.dMinLengthCT / dResolution;
			HTuple hMaxCircularityCT = m_FlexParm.dMaxCircularityCT;
			HTuple hMinDefectCountCT = m_FlexParm.nMinDefectCountCT;

			HTuple hMinDefectContrastSC = m_FlexParm.nMinContrastSC;
			HTuple hMaxWidthSC = m_FlexParm.dMaxWidthSC / dResolution;
			HTuple hMinLengthSC = m_FlexParm.dMinLengthSC / dResolution;
			HTuple hMaxCircularitySC = m_FlexParm.dMaxCircularitySC;
			HTuple hMinDefectCountSC = m_FlexParm.nMinDefectCountSC;

			// Flex Region Projection
			HTuple hFlexHomMat2DIdentity;
			HomMat2dIdentity(&hFlexHomMat2DIdentity);
			HomMat2dRotate(hFlexHomMat2DIdentity, -hRotationAngle, hDatumMatchRowRef, hDatumMatchColRef, &hFlexHomMat2DIdentity);
			HomMat2dTranslate(hFlexHomMat2DIdentity, -hShiftAlongRow, -hShiftAlongCol, &hFlexHomMat2DIdentity);
			AffineTransContourXld(m_TrainingData.hFlexTopEdgeLine, &hFlexTopEdgeLine, hFlexHomMat2DIdentity);
			AffineTransContourXld(m_TrainingData.hFlexBottomEdgeLine, &hFlexBottomEdgeLine, hFlexHomMat2DIdentity);
			AffineTransContourXld(m_TrainingData.hFlexLeftEdgeLine, &hFlexLeftEdgeLine, hFlexHomMat2DIdentity);
			AffineTransContourXld(m_TrainingData.hFlexRightEdgeLine, &hFlexRightEdgeLine, hFlexHomMat2DIdentity);
			AffineTransRegion(m_TrainingData.hFlexLeftEdgeLineMask, &hFlexLeftEdgeLineMask, hFlexHomMat2DIdentity, "nearest_neighbor");
			AffineTransRegion(m_TrainingData.hFlexRightEdgeLineMask, &hFlexRightEdgeLineMask, hFlexHomMat2DIdentity, "nearest_neighbor");
			AffineTransRegion(m_TrainingData.hFlexBottomMask, &hFlexBottomMask, hFlexHomMat2DIdentity, "nearest_neighbor");
			AffineTransRegion(m_TrainingData.hFlexStraightCircuitLineMask, &hFlexStraightCircuitLineMask, hFlexHomMat2DIdentity, "nearest_neighbor");
			AffineTransRegion(m_TrainingData.hFlexNumberMask, &hFlexNumberMask, hFlexHomMat2DIdentity, "nearest_neighbor");
			AffineTransRegion(m_TrainingData.hFlexTracingMask, &hFlexTracingMask, hFlexHomMat2DIdentity, "nearest_neighbor");
			AffineTransRegion(m_TrainingData.hFlexDamageMask, &hFlexDamageMask, hFlexHomMat2DIdentity, "nearest_neighbor");
			AffineTransRegion(m_TrainingData.hFlexTracingMaskRegion, &hFlexTracingMaskRegion, hFlexHomMat2DIdentity, "nearest_neighbor");

			// Substrate Region Projection
			GenEmptyObj(&hFlexSubstrateMask);
			if (m_TrainingData.bFlexEnableSubstrateMask) {
				AffineTransRegion(m_TrainingData.hFlexSubstrateSearchPatternROI, &hFlexSubstrateSearchPatternROI, hFlexHomMat2DIdentity, "nearest_neighbor");

				HObject hImageReduced;
				HTuple hSubstrateMatchAngle;
				HTuple hSubstrateDatumMatchRow;
				HTuple hSubstrateDatumMatchCol;
				HTuple hSubstrateDatumMatchScore;

				ReduceDomain(hImage, hFlexSubstrateSearchPatternROI, &hImageReduced);
				FindNccModel(hImageReduced, hFlexSubstrateModelID, -0.39, 0.79, 0, 1, 0.5, "true", 0, &hSubstrateDatumMatchRow, &hSubstrateDatumMatchCol, &hSubstrateMatchAngle, &hSubstrateDatumMatchScore);
				
				HTuple hSubstrateRotationAngle = hFlexSubstrateMatchAngleRef - hSubstrateMatchAngle;
				HTuple hSubstrateShiftAlongRow = hFlexSubstrateDatumMatchRowRef - hSubstrateDatumMatchRow;
				HTuple hSubstrateShiftAlongCol = hFlexSubstrateDatumMatchColRef - hSubstrateDatumMatchCol;

				HTuple hSubstrateHomMat2DIdentity;
				HomMat2dIdentity(&hSubstrateHomMat2DIdentity);
				HomMat2dRotate(hSubstrateHomMat2DIdentity, -hSubstrateRotationAngle, hFlexSubstrateDatumMatchRowRef, hFlexSubstrateDatumMatchColRef, &hSubstrateHomMat2DIdentity);
				HomMat2dTranslate(hSubstrateHomMat2DIdentity, -hSubstrateShiftAlongRow, -hSubstrateShiftAlongCol, &hSubstrateHomMat2DIdentity);
				AffineTransRegion(m_TrainingData.hFlexSubstrateMask, &hFlexSubstrateMask, hSubstrateHomMat2DIdentity, "nearest_neighbor");
			}

			if (nStepDebug) {
				StepDebug(hImage, hFlexBottomMask, colorGreen, "Bottom Mask Projection [Tracing]", bRegionInsp);
				StepDebug(hImage, hFlexStraightCircuitLineMask, colorGreen, "Straight Circuit Line Mask Projection [Tracing]", bRegionInsp);
				StepDebug(hImage, hFlexTracingMaskRegion, colorGreen, "Tracing Mask Region Projection [Tracing]", bRegionInsp);
				StepDebug(hImage, hFlexLeftEdgeLineMask, colorBlue, "Left Edge Line Mask Porjection [Tracing]", bRegionInsp);
				StepDebug(hImage, hFlexRightEdgeLineMask, colorBlue, "Right Edge Line Mask Projection [Tracing]", bRegionInsp);
				StepDebug(hImage, hFlexNumberMask, colorYellow, "Number Mask Projection [Tracing]", bRegionInsp);
				StepDebug(hImage, hFlexTracingMask, colorYellow, "Tracing Mask Projection [Tracing]", bRegionInsp);
				StepDebug(hImage, hFlexDamageMask, colorYellow, "Damage Mask Projection [Tracing]", bRegionInsp);
				StepDebug(hImage, hFlexSubstrateMask, colorYellow, "Substrate Mask Projection [Tracing]", bRegionInsp);
			}

			// Flex Tracing
			HObject hFlexLocation;
			_HP_Flex_Tracing(hImage,
				hFlexLeftEdgeLine,
				hFlexRightEdgeLine,
				hFlexTopEdgeLine,
				hFlexBottomEdgeLine,
				hFlexLeftEdgeLineMask,
				hFlexRightEdgeLineMask,
				hFlexBottomMask,
				&hFlexLocation,
				&hDebugImage,
				&hDebugRegion,
				nStepDebug,
				hFlexEnableDynamicEdge,
				hEdgeOffset,
				hMinEdgeContrast,
				hMinEncapContrast,
				hFlexLeftWidthReference,
				hFlexRightWidthReference,
				hRotationAngle,
				hDatumMatchRowRef,
				hDatumMatchColRef,
				&hIsPass,
				&hDebugMsg);

			if (nStepDebug) {
				StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
			}
			// Check Result And Display Flex ROI If Get Error
			if (hIsPass == FALSE) {
				HObject RegionImage;
				GetDomain(hImage, &RegionImage);
				m_arrayOverlayInspection.Add(RegionImage, colorRed, "Flex Tracing Failed");
				return -FLEX_OVERCUT;
			}

			// Display Flex Location When Inspecting
			m_arrayOverlayInspection.Add(hFlexLocation, colorGreen, "Flex Location");

			// Flex Circuit Line Tracing
			HObject hFlexCircuitLineROI;
			HObject hMaxCircuitLineRegion;
			HObject hMinCircuitLineRegion;

			_HP_Flex_CircuitLineTracing(hImage,
				hFlexLocation,
				hFlexStraightCircuitLineMask,
				hFlexTracingMask,
				&hFlexCircuitLineROI,
				&hMinCircuitLineRegion,
				&hMaxCircuitLineRegion,
				&hDebugImage,
				&hDebugRegion,
				nStepDebug,
				hMinEncapContrast,
				hMinCircuitLineContrast,
				hMinCircuitLineWidthOV,
				hMaxCircuitLineWidthOV,
				hRotationAngle,
				hDatumMatchRowRef,
				hDatumMatchColRef,
				&hIsPass,
				&hDebugMsg);
				
			if (nStepDebug) {
				StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
			}
			// Check Result And Display Flex ROI If Get Error
			if (hIsPass == FALSE) {
				m_arrayOverlayInspection.Add(hFlexLocation, colorRed, "Circuit Line Tracing Failed");
				return -FLEX_OVERCUT;
			}

			// Flex Inspection: Overcut
			HObject hOvercutDefectRegion;
			_HP_Flex_InspectOvercut(hImage,
				hFlexLocation,
				hFlexTracingMask,
				hFlexCircuitLineROI,
				hMinCircuitLineRegion,
				hMaxCircuitLineRegion,
				&hDebugImage,
				&hDebugRegion,
				&hOvercutDefectRegion,
				nStepDebug,
				hEnableCircuitLineCutInspection,
				hEnableCircuitLineLinkedInspection,
				hMinCircuitLineContrast,
				&hIsPass,
				&hDebugMsg);

			if (nStepDebug) {
				StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
			}
			// Check Result And Display Error
			if (hIsPass == FALSE) {
				m_arrayOverlayInspection.Add(hOvercutDefectRegion, colorRed, "Flex Overcut Defect Region");
				return -FLEX_OVERCUT;
			}

			// Flex Inspection: Damage
			HObject hDamageDefectRegion;
			_HP_Flex_InspectDamage(hImage,
				hFlexLocation,
				hFlexDamageMask,
				&hDebugImage,
				&hDebugRegion,
				&hDamageDefectRegion,
				nStepDebug,
				bEnableCircuitLineDamageInspection,
				hMaxDefectContrastDM,
				hMinSizeDM,
				hMinDefectCountDM,
				&hIsPass,
				&hDebugMsg);

			if (nStepDebug) {
				StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
			}
			// Check Result And Display Error
			if (hIsPass == FALSE) {
				m_arrayOverlayInspection.Add(hDamageDefectRegion, colorRed, "Flex Damage Defect Region");
				return -FLEX_DAMAGE;
			}

			// Flex Inspection: Contamination
			HObject hContaminationDefectRegion;
			_HP_Flex_InspectContamination(hImage,
				hFlexNumberMask,
				hFlexSubstrateMask,
				hFlexTracingMaskRegion,
				hFlexLocation,
				hFlexCircuitLineROI,
				&hDebugImage,
				&hDebugRegion,
				&hContaminationDefectRegion,
				nStepDebug,
				hMinDefectContrastCT,
				hMinDefectSubstrateContrastCT,
				hMinSizeCT,
				hMinSquareSizeCT,
				hMinLengthCT,
				hMaxCircularityCT,
				hMinDefectCountCT,
				&hIsPass,
				&hDebugMsg);

			if (nStepDebug) {
				StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
			}
			// Check Result And Display Error
			if (hIsPass == FALSE) {
				m_arrayOverlayInspection.Add(hContaminationDefectRegion, colorRed, "Flex Contamination Defect Region");
				return -FLEX_CONTAMINATION;
			}

			// Flex Inspection: Scratches
			HObject hScratchesDefectRegion;
			_HP_Flex_InspectScratches(hImage,				
				hFlexTracingMask,
				hFlexSubstrateMask,
				hFlexLocation,
				hMaxCircuitLineRegion,
				&hDebugImage,
				&hDebugRegion,
				&hScratchesDefectRegion,
				nStepDebug,
				hMinCircuitLineContrast,
				hMinDefectContrastSC,
				hMaxWidthSC,
				hMinLengthSC,
				hMaxCircularitySC,
				hMinDefectCountSC,
				&hIsPass,
				&hDebugMsg);

			if (nStepDebug) {
				StepDebug(hDebugImage, hDebugRegion, colorGreen, hDebugMsg, bRegionInsp);
			}
			// Check Result And Display Error
			if (hIsPass == FALSE) {
				m_arrayOverlayInspection.Add(hScratchesDefectRegion, colorRed, "Flex Scratches Defect Region");
				return -FLEX_SCRATCHES;
			}

			double dTime2 = Timer.GetElapsedMSec();
			double dTotalTimeInspection = (dTime2 - dTime1);
			OutputDebugLogTo(4, TRUE, "Total Time For Flex Inspection: %.2f ms", dTotalTimeInspection);
		}

		//// ********** Chicklet Swage Hole Inspection ********** ////
		if (m_ChickletSwageHole.bEnable) {

			if (!m_TrainingData.bEnableChickletSwageHole)
				return -CHICKLET_ROUNDNESS;

			int nChickletRoi = m_TrainingData.m_nChickletNumberOfHoles;
			HTuple nHoleOffSet = m_TrainingData.m_nChickletOffsettRadius;
			HTuple nMinContrast = m_TrainingData.m_nChickletMinContrast;
			HTuple nMaxContrast = m_TrainingData.m_nChickletMaxContrast;
			HTuple nBurrValueDefect = m_ChickletSwageHole.nBurrValueDefect;
			HTuple nRoundnessValueDefect = m_ChickletSwageHole.nRoundnessValueDefect;
			HTuple nBurrValueForRoundness = m_ChickletSwageHole.nBurrValueForRoundness;
			HTuple hIsBurr, hIsRoundness, hBurrFinal = 0, hRoundnessFinal = 0, hDebugMessageOutChicklet;
			HImage hDebugImageOutChicklet;
			HRegion hRectChickletROITemp, hRectChickletROI, BurrRegion, HoleLocation, hDebugRegionOutChicklet;
			HTuple hChickletROIRow1;
			HTuple hChickletROICol1;
			HTuple hChickletROIRow2;
			HTuple hChickletROICol2;
			HTuple nRadiusInit;
			HTuple hRadiusOut;
			// Region Projection
			HTuple hMatchAngle = hDatumCenterRotationAngle;
			HTuple hDatumMatchRow = hDatumCenterRow;
			HTuple hDatumMatchCol = hDatumCenterCol;

			HTuple hRotationAngle = hInspectRotationAngle;
			HTuple hShiftAlongRow = hInspectShiftAlongRow;
			HTuple hShiftAlongCol = hInspectShiftAlongCol;

			HTuple hMatchAngleRef = hRotationAngle + hMatchAngle;
			HTuple hDatumMatchRowRef = hShiftAlongRow + hDatumMatchRow;
			HTuple hDatumMatchColRef = hShiftAlongCol + hDatumMatchCol;

			HTuple hHomMat2DIdentityChicklet;
			HomMat2dIdentity(&hHomMat2DIdentityChicklet);
			HomMat2dRotate(hHomMat2DIdentityChicklet, -hRotationAngle, hDatumMatchRowRef, hDatumMatchColRef, &hHomMat2DIdentityChicklet);
			HomMat2dTranslate(hHomMat2DIdentityChicklet, -hShiftAlongRow, -hShiftAlongCol, &hHomMat2DIdentityChicklet);
			

			for (int i = 0; i < nChickletRoi; i++) {
				hChickletROIRow1 = m_TrainingData.m_rectChickletROI[i].top;
				hChickletROICol1 = m_TrainingData.m_rectChickletROI[i].left;
				hChickletROIRow2 = m_TrainingData.m_rectChickletROI[i].bottom;
				hChickletROICol2 = m_TrainingData.m_rectChickletROI[i].right;
				nRadiusInit = (m_TrainingData.m_rectChickletROI[i].bottom - m_TrainingData.m_rectChickletROI[i].top) / 3;
				GenRectangle1(&hRectChickletROITemp, hChickletROIRow1, hChickletROICol1, hChickletROIRow2, hChickletROICol2);
				//hDebugMessageInChicklet = "Projected Chicklet ROI Teach Region [Chicklet Swage Hole]";
				//_HP_RegionProjection(hImage, hRectChickletROI, &hChickletROIProjected, &hDebugImageOutChicklet, &hDebugRegionOutChicklet, 0,
				//	hInspectRotationAngle, (hChickletROIRow2 + hChickletROIRow1) / 2, (hChickletROICol1 + hChickletROICol2) / 2,
				//	hInspectShiftAlongRow, hInspectShiftAlongCol, hDebugMessageInChicklet, &hDebugMessageOutChicklet);
				AffineTransRegion(hRectChickletROITemp, &hRectChickletROI, hHomMat2DIdentityChicklet, "nearest_neighbor");
				if (nStepDebug) {
					hDebugMessageOutChicklet = "Projected Chicklet ROI Teach Region [Chicklet Swage Hole]";
					StepDebug(hImage, hRectChickletROI, colorCyan, hDebugMessageOutChicklet, bRegionInsp);
				}
				
				_HP_Inspect_ChickletHole_BurrRoundness(hRectChickletROI, hImage,
					&BurrRegion, &HoleLocation, &hDebugImageOutChicklet, &hDebugRegionOutChicklet, nStepDebug, nBurrValueDefect,
					nMinContrast, nMaxContrast, nRadiusInit, nBurrValueForRoundness,
					nRoundnessValueDefect, &hIsBurr, &hIsRoundness, &hRadiusOut, &hDebugMessageOutChicklet);

				if (nStepDebug) {
					StepDebug(hDebugImageOutChicklet, hDebugRegionOutChicklet, colorCyan, hDebugMessageOutChicklet, bRegionInsp);
				}

				if (hIsBurr != 0 || hIsRoundness != 0) {
					m_arrayOverlayInspection.Add(hRectChickletROI, colorRed);
					m_arrayOverlayInspection.Add(BurrRegion, colorRed);
					if (hIsBurr != 0)
						hBurrFinal = 1;
					if (hIsRoundness != 0)
						hRoundnessFinal = 1;
				}
				else m_arrayOverlayInspection.Add(HoleLocation, colorYellow);
			}
			if (hBurrFinal != 0) {
				return -CHICKLET_BURR;
			}
			if (hRoundnessFinal != 0) {
				return -CHICKLET_ROUNDNESS;
			}
		}



		//// ********* PVI Inspection  ********** ////
		HTuple hPVIArea, hPVIAngle, hPVI2DCenter, hPVISize = 0;
		if (m_TrainingData.hvec_TupleVectorPVI[0].GetSize() > 0)
			hPVIArea = m_TrainingData.hvec_TupleVectorPVI[0][0];

		if (hPVIArea != 0 && hPVIArea.Length() > 0) {
			BOOL bPVIEnable = FALSE;
			for (int i = 0; i < hPVIArea; i++) {
				if (m_pPviInspectionSetup[i].bEnable)
					bPVIEnable = TRUE;
			}
			if (bPVIEnable) {
				HImage hImageForPVI_Inspection, hImageRotate, hImageRotateShift;
				HRegion hRegionForPVI_Inspection, hRegionAffineTrans, hRegionAffineRotateShift;
				HTuple hDebugMsgOutPviRegion;
				HImage hDebugImageOutPviRegion;
				HRegion hDebugRegionOutPviRegion;
				HTuple hMaskNumber;
				HTuple hMask2DCenterPoints, hMaskAngleConcat, hMaskSize, hPolyLinePoints, hPVIPolygonMaskPoint;
				HTuple hTeachModelRowCentre, hTeachModelColCentre;
				HTuple hMatchModelCentreRow, hMatchModelCentreCol;

				HTuple hLocationSelectedRow, hLocationSelectedCol, hLocationSelectedPhi, hLocationSelectedLen1, hLocationSelectedLen2;
				SmallestRectangle2(hDeviceLocationSelected, &hLocationSelectedRow, &hLocationSelectedCol, &hLocationSelectedPhi, &hLocationSelectedLen1, &hLocationSelectedLen2);
				HTuple hDebugMsgOutPviRegion_T;
				HImage hDebugImageOutPviRegion_T;
				HRegion hDebugRegionOutPviRegion_T;

				Projection_Function(hImage, hDeviceLocationSelected, &hRegionAffineRotateShift, &hImageRotateShift,
					&hDebugImageOutPviRegion_T, &hDebugRegionOutPviRegion_T, nStepDebug,
					hInspectRotationAngle, hDatumCenterRow, hDatumCenterCol, hInspectShiftAlongRow,
					hInspectShiftAlongCol, 0, 0, &hDebugMsgOutPviRegion_T);

				HImage hImageAbsDiff;
				AbsDiffImage(hTeachImage, hImageRotateShift, &hImageAbsDiff, 1);

				//Inspection Priority Check - PVI Areas Level
				std::vector<int> nPVIAreaPriority;
				for (int nAreaIdx = 0; nAreaIdx < hPVIArea; nAreaIdx++) {
					nPVIAreaPriority.push_back(nAreaIdx);
				}
				if (m_bInspectionPriority) {
					std::map<int, int> mapPVIAreaPriority;
					for (int nArea = 0; nArea < hPVIArea; nArea++) {
						int nMinPriority = -1;
						for (int nDefCount = 0; nDefCount < m_pPviInspectionSetup[nArea].nDefectCount; nDefCount++) {
							if (m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].m_nInspPriority < nMinPriority || nMinPriority == -1) {
								nMinPriority = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].m_nInspPriority;
							}
						}
						mapPVIAreaPriority.insert(std::pair<int, int>(nMinPriority, nArea));
					}

					int nIdxTemp = 0;
					for (std::map<int, int>::iterator it = mapPVIAreaPriority.begin(); it != mapPVIAreaPriority.end(); it++) {
						nPVIAreaPriority[nIdxTemp] = it->second;
						nIdxTemp++;
					}
				}


				for (int nAreaIdx = 0; nAreaIdx < hPVIArea; nAreaIdx++) {
					int nArea = nPVIAreaPriority[nAreaIdx];
					if (!m_pPviInspectionSetup[nArea].bEnable && !m_pPviInspectionSetup[nArea].bEdgeEnable)
						continue;


					int nIndex = 1;
					hPVIAngle = m_TrainingData.hvec_TupleVectorPVI[nIndex++][nArea];
					hPVI2DCenter = m_TrainingData.hvec_TupleVectorPVI[nIndex++][nArea];
					hPVISize = m_TrainingData.hvec_TupleVectorPVI[nIndex++][nArea];
					hMaskNumber = m_TrainingData.hvec_TupleVectorPVI[nIndex++][nArea];
					hMask2DCenterPoints = m_TrainingData.hvec_TupleVectorPVI[nIndex++][nArea];
					hMaskAngleConcat = m_TrainingData.hvec_TupleVectorPVI[nIndex++][nArea];
					hMaskSize = m_TrainingData.hvec_TupleVectorPVI[nIndex++][nArea];
					hPolyLinePoints = m_TrainingData.hvec_TupleVectorPVI[nIndex++][nArea];
					hPVIPolygonMaskPoint = m_TrainingData.hvec_TupleVectorPVI[nIndex++][nArea];

					HRegion hConcatMask, hRectMask;
					HRegion hPviTeachRegion;
					GenEmptyObj(&hPviTeachRegion);

					if (m_pPviInspectionSetup[nArea].bEdgeEnable && m_pPviInspectionSetup[nArea].nDeviceAreaType == POLYLINE) {
						HTuple hRows, hCols;
						for (int nPointId = 0; nPointId < m_pPviInspectionSetup[nArea].nPolylinePointNumber; nPointId++) {
							TupleConcat(hRows, hPolyLinePoints[2 * nPointId], &hRows);
							TupleConcat(hCols, hPolyLinePoints[2 * (nPointId + 1) - 1], &hCols);
						}
						GenRegionPolygon(&hRegionForPVI_Inspection, hRows, hCols);
						HTuple hEdgeWidth = m_pPviInspectionSetup[nArea].dEdgeWidth > 0 ? m_pPviInspectionSetup[nArea].dEdgeWidth / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2) : m_pPviInspectionSetup[nArea].dEdgeWidth;
						if (hEdgeWidth < 1) {
							hEdgeWidth = 1;
						}
						DilationCircle(hRegionForPVI_Inspection, &hRegionForPVI_Inspection, hEdgeWidth);
					}
					else {
						if (m_pPviInspectionSetup[nArea].nDeviceAreaType == RECTANGLE)
							GenRectangle2(&hRegionForPVI_Inspection, hPVI2DCenter[0], hPVI2DCenter[1], hPVIAngle, hPVISize[0], hPVISize[1]);
						else if (m_pPviInspectionSetup[nArea].nDeviceAreaType == ECLIPSE)
							GenEllipse(&hRegionForPVI_Inspection, hPVI2DCenter[0], hPVI2DCenter[1], 0,
								m_pPviInspectionSetup[nArea].m_rectPviArea.Width() / 2, m_pPviInspectionSetup[nArea].m_rectPviArea.Height() / 2);
						else if (m_pPviInspectionSetup[nArea].nDeviceAreaType == POLYGON) {
							HTuple hRows, hCols;
							for (int nPointId = 0; nPointId < m_pPviInspectionSetup[nArea].nPolygonPointNumber; nPointId++) {
								TupleConcat(hRows, hPolyLinePoints[2 * nPointId], &hRows);
								TupleConcat(hCols, hPolyLinePoints[2 * (nPointId + 1) - 1], &hCols);
							}
							GenRegionPolygonFilled(&hRegionForPVI_Inspection, hRows, hCols);
						}


					}

					ConcatObj(hPviTeachRegion, hRegionForPVI_Inspection, &hPviTeachRegion);
					GenEmptyObj(&hConcatMask);
					int nMaskPointId = 0;
					for (int nMaskIndex = 0; nMaskIndex < hMaskNumber; nMaskIndex++)
					{
						if (m_pPviInspectionSetup[nArea].nDeviceMaskArea[nMaskIndex] == POLYGON) {
							HTuple hRows, hCols;
							for (int nPointId = 0; nPointId < m_pPviInspectionSetup[nArea].nPolygonPviMaskPointNumber[nMaskIndex]; nPointId++) {
								TupleConcat(hRows, hPVIPolygonMaskPoint[2 * nMaskPointId], &hRows);
								TupleConcat(hCols, hPVIPolygonMaskPoint[2 * (nMaskPointId + 1) - 1], &hCols);
								nMaskPointId++;
							}
							GenRegionPolygonFilled(&hRectMask, hRows, hCols);
						}
						else {
							GenRectangle2(&hRectMask, hMask2DCenterPoints[2 * nMaskIndex], hMask2DCenterPoints[2 * (nMaskIndex + 1) - 1],
								hMaskAngleConcat[nMaskIndex], hMaskSize[2 * nMaskIndex], hMaskSize[2 * (nMaskIndex + 1) - 1]);
						}

						ConcatObj(hConcatMask, hRectMask, &hConcatMask);
					}

					//	m_arrayOverlayInspection.Add(hConcatMask, colorYellow);//TEsting 
					HRegion hFitMaskRegion;
					GenEmptyObj(&hFitMaskRegion);
					if (hMaskNumber > 0)
					{
						Union1(hConcatMask, &hFitMaskRegion);
						Difference(hRegionForPVI_Inspection, hFitMaskRegion, &hRegionForPVI_Inspection);
					}

					HImage hImageRotateShiftBack, hImageSub;
					HRegion hRegionAffineRotateShiftBack;

					HTuple hDebugMsgOutPviRegionArea;
					HImage hDebugImageOutPviRegionArea;
					HRegion hDebugRegionOutPviRegionArea;

					Projection_Back_Function(hImage, hRegionForPVI_Inspection, hImageAbsDiff, hFitMaskRegion, hPviTeachRegion, &hRegionForPVI_Inspection, &hImageSub,
						&hDebugImageOutPviRegionArea, &hDebugRegionOutPviRegionArea, nStepDebug,
						-hInspectShiftAlongRow, -hInspectShiftAlongCol, 0, 0, -hInspectRotationAngle, hDatumCenterRow, hDatumCenterCol,
						nArea, &hDebugMsgOutPviRegionArea);

					HTuple hNumHole = 0;
					HRegion hMaskRegion;
					RegionFeatures(hRegionForPVI_Inspection, "holes_num", &hNumHole);
					if (hNumHole > 0)
					{
						HRegion hFillUp;
						FillUp(hRegionForPVI_Inspection, &hFillUp);
						Difference(hFillUp, hRegionForPVI_Inspection, &hMaskRegion);
						m_arrayOverlayInspection.Add(hMaskRegion, colorYellow);
					}

					if (m_pPviInspectionSetup[nArea].bEdgeEnable == FALSE || m_pPviInspectionSetup[nArea].nDeviceAreaType == POLYLINE)
						m_arrayOverlayInspection.Add(hRegionForPVI_Inspection, colorOrange);

					if (nStepDebug) {
						StepDebug(hDebugImageOutPviRegionArea, hDebugRegionOutPviRegionArea, colorCyan, hDebugMsgOutPviRegionArea, bRegionInsp);
					}


					//// PVI Insp ////
					if (m_pPviInspectionSetup[nArea].bEnable) {
						ReduceDomain(hImageSub, hRegionForPVI_Inspection, &hImageForPVI_Inspection);
					}

					if (m_pPviInspectionSetup[nArea].bEdgeEnable && m_pPviInspectionSetup[nArea].nDeviceAreaType != POLYLINE)
					{
						//// Edge insp ///
						HTuple hEdgeWidth = m_pPviInspectionSetup[nArea].dEdgeWidth > 0 ? m_pPviInspectionSetup[nArea].dEdgeWidth / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2) : m_pPviInspectionSetup[nArea].dEdgeWidth;
						HTuple hEdgeOffset = m_pPviInspectionSetup[nArea].dEdgeOffset > 0 ? m_pPviInspectionSetup[nArea].dEdgeOffset / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2) : m_pPviInspectionSetup[nArea].dEdgeOffset;

						HTuple hMinEdgeContrast, hMaxEdgeContrast, hEdgeRgnOpen;
						HTuple hIsEdgePVIPass, hDebugMessageOut;

						HImage hDebugImageOut;
						HRegion hDebugRegionOut;

						hMinEdgeContrast = m_pPviInspectionSetup[nArea].nMinEdgeContrast;
						hMaxEdgeContrast = m_pPviInspectionSetup[nArea].nMaxEdgeContrast;
						hEdgeRgnOpen = m_pPviInspectionSetup[nArea].dEdgeRegionOpen / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
						if (hEdgeRgnOpen < 1)
							hEdgeRgnOpen = 1;

						_FCI_ExtractExactEdge(hImage, hRegionForPVI_Inspection,
							&hRegionForPVI_Inspection, &hDebugImageOut, &hDebugRegionOut,
							nStepDebug, hMinEdgeContrast, hMaxEdgeContrast, hEdgeRgnOpen, nArea,
							&hIsEdgePVIPass, &hDebugMessageOut);

						if (nStepDebug) {
							StepDebug(hDebugImageOut, hDebugRegionOut, colorCyan, hDebugMessageOut, bRegionInsp);
						}

						HImage hDebugImageForPVI_Inspection;
						HRegion hDebugRegionForPVI_Inspection;

						_FCI_OffsetRegions(hRegionForPVI_Inspection, hImageSub,
							&hRegionForPVI_Inspection, &hImageForPVI_Inspection,
							&hDebugImageForPVI_Inspection, &hDebugRegionForPVI_Inspection,
							nStepDebug, hEdgeOffset, hEdgeWidth, nArea, FALSE,
							&hDebugMessageOut);

						if (nStepDebug) {
							StepDebug(hDebugImageForPVI_Inspection, hDebugRegionForPVI_Inspection, colorCyan, hDebugMessageOut, bRegionInsp);
						}

						if (hIsEdgePVIPass > 0) {
							m_arrayOverlayInspection.Add(hRegionForPVI_Inspection, colorCyan);

							HRegion hFillUp, hHole;
							FillUp(hRegionForPVI_Inspection, &hFillUp);
							Difference(hFillUp, hRegionForPVI_Inspection, &hHole);
							m_arrayOverlayInspection.Add(hHole, colorCyan);

						}
					}

					//Inspection Priority Check - Defect Characteristics Level
					std::vector<int> nPVIDefectPriority;
					for (int nDefCountIdx = 0; nDefCountIdx < m_pPviInspectionSetup[nArea].nDefectCount; nDefCountIdx++) {
						nPVIDefectPriority.push_back(nDefCountIdx);
					}
					if (m_bInspectionPriority) {
						std::map<int, int> mapPVIDefectPriority;
						for (int nDefCountIdx = 0; nDefCountIdx < m_pPviInspectionSetup[nArea].nDefectCount; nDefCountIdx++) {
							mapPVIDefectPriority.insert(std::pair<int, int>(m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCountIdx].m_nInspPriority, nDefCountIdx));
						}

						int nIdxTempt = 0;
						for (std::map<int, int>::iterator it = mapPVIDefectPriority.begin(); it != mapPVIDefectPriority.end(); it++) {
							nPVIDefectPriority[nIdxTempt] = it->second;
							nIdxTempt++;
						}
					}

					BOOL bPVIPassAll = TRUE;
					for (int nDefCount = 0; nDefCount < m_pPviInspectionSetup[nArea].nDefectCount; nDefCount++)
					{
						if (m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].bEnable ||
							m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].bDotDefectFilter ||
							m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].bScratchDefectFilter) {
							HTuple hDefectCharecterstics, hContrast, hMinSize, hMinLength, hMinSquareSize, hMinInnerWidth, hMinCount;
							HTuple hResolutionMicronPerPixel;

							hDefectCharecterstics = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nDefectCharacteristics;
							hContrast = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nContrast;
							hMinSize = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].m_nSize / (pCalibData->dResolutionAlongXInMicronPerPixel*pCalibData->dResolutionAlongYInMicronPerPixel);
							hMinLength = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nMinLength / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
							hMinSquareSize = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nMinSquareSize / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
							hMinInnerWidth = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nMinInnerWidth / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
							hMinCount = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nMinCount;



							hResolutionMicronPerPixel = (pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2;
							HTuple hPass;
							HRegion hPVIDefectRegion, hActualPVIDefectRegion;

							//1. Dot Defect Filter 
							if (m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].bDotDefectFilter) {
								HTuple hDotContrast, hMinDotCircularity, hMinDotLength;
								hDotContrast = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nDotContrast;
								hMinDotCircularity = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nMinDotCircularity / 100.0;
								hMinDotLength = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nMinDotLength / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
								hMinCount = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nMinDotCount;

								_FCI_PVI_Dot_Inspection(hImageForPVI_Inspection, hRegionForPVI_Inspection, hImage,
									&hPVIDefectRegion, &hDebugImage, &hDebugRegion,
									nStepDebug, hDotContrast, hMinDotCircularity, hMinDotLength, hMinCount,
									nCurDoc, nDefCount + 1, nArea,
									&hPass, &hDebugMsg);
							}
							//2. Scratch Defect Filter 
							else if (m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].bScratchDefectFilter) {
								HTuple hScratchContrast, hScratchLength, hScratchInnerWidth, hScratchDirectionFilter;
								hScratchContrast = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nScratchContrast;
								hScratchLength = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nScratchLength / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
								hScratchInnerWidth = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nScratchInnerWidth / ((pCalibData->dResolutionAlongXInMicronPerPixel + pCalibData->dResolutionAlongYInMicronPerPixel) / 2);
								hScratchDirectionFilter = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nScratchFilterDirection;
								hMinCount = m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].nMinScratchCount;


								_HP_PVI_Scratch_Inspection(hImageForPVI_Inspection, hRegionForPVI_Inspection, hImage,
									&hPVIDefectRegion,
									&hDebugImage, &hDebugRegion,
									hDefectCharecterstics, hScratchContrast, hScratchLength, hScratchInnerWidth, hScratchDirectionFilter, hMinCount,
									nDefCount + 1, nArea,
									nStepDebug, &hPass, &hDebugMsg);
							}
							else {
								//Normal PVI Defect
								_Tesla_PVI_Inspection(hImageForPVI_Inspection, hRegionForPVI_Inspection, hImage,
									&hPVIDefectRegion,
									&hDebugImage, &hDebugRegion,
									hDefectCharecterstics, hContrast,
									hMinLength, hMinSize, hMinSquareSize, hMinInnerWidth, hMinCount,
									nStepDebug,
									nDefCount + 1, nArea, hResolutionMicronPerPixel, &hPass, &hDebugMsg);
							}


							if (nStepDebug) {
								StepDebug(hDebugImage, hDebugRegion, colorRed, hDebugMsg, bRegionInsp);
							}

							if (hPass == FALSE) {

								HTuple hAllPVIDefectMinSize, hAllPVIDefectMinLength, hAllPVIDefectMinSqSize;

								HTuple hMinMeanGVDiff = m_pPviInspectionSetup[nArea].nMinGVDiff;
								HTuple hIntensityRecheck = m_pPviInspectionSetup[nArea].nIntensity;
								BOOL bRecheckEn = m_pPviInspectionSetup[nArea].bPVIRecheckEnable;

								PVI_Inspection_ReCheck(hImage, hTeachImage, hPVIDefectRegion, hRegionForPVI_Inspection,
									&hActualPVIDefectRegion, &hDebugImage, &hDebugRegion,
									nStepDebug, hDatumCenterRow, hDatumCenterCol, hInspectRotationAngle,
									hInspectShiftAlongRow, hInspectShiftAlongCol, hDefectCharecterstics, nDefCount + 1, nArea,
									hMinMeanGVDiff, hIntensityRecheck, bRecheckEn, FALSE,
									&hPass, &hAllPVIDefectMinSize, &hAllPVIDefectMinLength, &hAllPVIDefectMinSqSize, &hDebugMsg);

								if (nStepDebug) {
									StepDebug(hDebugImage, hDebugRegion, colorRed, hDebugMsg, bRegionInsp);
								}


								HTuple hCount = 0;
								Connection(hActualPVIDefectRegion, &hActualPVIDefectRegion);
								CountObj(hActualPVIDefectRegion, &hCount);
								if (0 != (hCount >= hMinCount)) {
									if (m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].bDotDefectFilter) {
										ShapeTrans(hActualPVIDefectRegion, &hActualPVIDefectRegion, "outer_circle");
									}
									m_arrayOverlayInspection.Add(hActualPVIDefectRegion, colorRed);

									return -m_pPviInspectionSetup[nArea].m_pPviDefect[nDefCount].m_nErrorCodeID;
								}
							}
						}
					}

				}
			}
		} // End of PVI
	}

	return GOOD_UNIT;
}

void CInspectionCore::StepDebug(HImage hImage, HRegion hDebugRgn, UINT color, HTuple hDebugMsg, int nRegion)
{
	CString strTemp, strDebugMsgTemp = "";
	int nPos;
	int nCOunt = hDebugRgn.CountObj();
	for (int i = 0; i < hDebugRgn.CountObj(); i++) {
		strTemp = hDebugMsg[i].S();
		nPos = strTemp.Find(':') + 1;
		strDebugMsgTemp = strTemp.Mid(nPos);
		strTemp = strTemp.GetAt(0);
		m_arrayOverlay[nRegion].Add(hImage.SelectObj(i + 1), hDebugRgn.SelectObj(i + 1), color, strDebugMsgTemp);
	}
}


void CInspectionCore::ZoomImage(HImage hImgSrc, HImage hImgDst, int nWidth, int nHeight, int nDoc)
{
	HRegion hRgn;
	hRgn.GenEmptyObj();
	ZoomImageSize(hImgSrc, &hImgDst, nWidth, nHeight, "nearest_neighbor");
}


int CInspectionCore::Inspect(int nThreadIndex, HTuple& bPass)
{


	THREAD_PARM* pThreadParm = &m_ThreadParms[nThreadIndex];
	CString strInspResult;
	CPerformanceTimer timer;
	COLORREF colorRed = RGB(255, 0, 0);
	COLORREF colorCyan = RGB(0, 255, 255);
	COLORREF colorPink = RGB(255, 0, 255);
	bPass = TRUE;

	int nStepDebug = 0;
	if (m_pShare->lDebugFlagThread & DEBUG_STEP_MODE)
		nStepDebug = 1;

	try {
		// Die Edge Inspection
		if (nThreadIndex == THREAD_DIE_EDGE) {
		}
	}

	catch (HOperatorException ex) {
		bPass = FALSE;
		LogMessage(9, "[InspectionCore] %s", ex.ErrorMessage().Text());
		OutputDebugLogTo(8, TRUE, "The Inspection Process Error occurs at %d", nThreadIndex);
		return PROCESS_ERROR;
	}
	catch (...) {
		bPass = FALSE;
		LogMessage(9, "[InspectionCore->Inspect] %d", nThreadIndex);
		OutputDebugLogTo(8, TRUE, "The Inspection Process Error occurs at %d", nThreadIndex);
		return PROCESS_ERROR;
	}
	return 0;
}


//int CInspectionCore::LightCalibrationMode(HImage hImage,int nTrack,int nGreyValue,
//										int* nCalibRange,double* dCalibDeviation,CRect rectSideTrack)
//{
//	HTuple hCalibRange, hCalibDeviation;
//	HTuple hTargetGrayMeanValue, hTolerance, hTargetGrayDeviation;
//	hTargetGrayDeviation = m_CalibrationData.nTargetGreyDeviation;
//
//
//	//if(nTrack==2) {
//	//	hTargetGrayMeanValue = m_CalibrationData.nTargetGreyMeanValueSide;
//	//	hTolerance = m_CalibrationData.nCalibTolerenceSide;
//
//	//	_FCI_SideCameraCalibration(hImage,
//	//								rectSideTrack.top, rectSideTrack.left, rectSideTrack.bottom, rectSideTrack.right,
//	//								hTargetGrayMeanValue, hTolerance,
//	//								&hCalibRange, &hCalibDeviation);
//
//	//}
//	//else{
//	//	HTuple hDifference, hMaxDeviationValue;
//
//	//	hTargetGrayMeanValue = m_CalibrationData.nTargetGreyMeanValue;
//	//	hTolerance = m_CalibrationData.nCalibTolerence;
//	//	hTargetGrayDeviation = m_CalibrationData.nTargetGreyDeviation;
//
//	//	_FCI_SetOptimumPulseWidth(hImage, hTargetGrayMeanValue, hTolerance, hTargetGrayDeviation,
//	//						&hCalibRange, &hDifference, &hMaxDeviationValue);
//
//	//	HImage hTargetImage, hResultantImage, hSubImage;
//	//	
//	//	//_FCI_LightCalibration(hImages[0], hImages[1], hImages[2], hImages[3],
//	//	//					&hTargetImage, &hResultantImage, &hSubImage, hTargetGrayMeanValue);
//
//	//}
//
//	*nCalibRange = hCalibRange.I();
//	*dCalibDeviation = hCalibDeviation.D();
//	OutputDebugLogTo(9,TRUE,"nCalibRange = %d , dCalibDeviation = %.2f",hCalibRange.I(),hCalibDeviation.D());
//
//	return 0;
//}

int CInspectionCore::LightCalibrationMode(HImage hImage, int nGreyValue,
	HImage* hImageCalibPos, HImage* hImageCalibNeg, HImage* hImageCalibResult,
	BOOL bFirst, BOOL bRefImg, BOOL bAvgImg, BOOL bLightCorrection,
	CCalibrationData* pCalibData)
{
	if (bRefImg) {
		if (bFirst) {
			GenImageProto(hImage, &pCalibData->hCalibSumImage, 0);
			AddImage(hImage, pCalibData->hCalibSumImage, &pCalibData->hCalibSumImage, 1, 0);
			ConvertImageType(pCalibData->hCalibSumImage, &pCalibData->hCalibSumImage, "uint2");
		}
		else {
			AddImage(hImage, pCalibData->hCalibSumImage, &pCalibData->hCalibSumImage, 1, 0);
		}
	}
	if (bAvgImg) {
		HImage hAvgImage;

		_Average_Reference_Image(pCalibData->hCalibSumImage, &hAvgImage);
		ConcatObj(pCalibData->hCalibAvgImage, hAvgImage, &pCalibData->hCalibAvgImage);
	}
	if (bLightCorrection) {
		HImage hResultantImage, hTargetUniformImage, hLightCorrectionImage, hPosContrastImg, hNegContrastImg;
		HTuple hTargetGrayValue = nGreyValue;

		HImage hImg1, hImg2, hImg3, hImg4;
		hImg1 = pCalibData->hCalibAvgImage.SelectObj(1);
		hImg2 = pCalibData->hCalibAvgImage.SelectObj(2);
		hImg3 = pCalibData->hCalibAvgImage.SelectObj(3);
		hImg4 = pCalibData->hCalibAvgImage.SelectObj(4);

		_FCI_LightCorrection(hImg1, hImg2, hImg3, hImg4,
			&hResultantImage, &hTargetUniformImage, &hLightCorrectionImage,
			&hPosContrastImg, &hNegContrastImg,
			hTargetGrayValue);

		CopyImage(hPosContrastImg, hImageCalibPos);
		CopyImage(hNegContrastImg, hImageCalibNeg);
		CopyImage(hResultantImage, hImageCalibResult);
	}

	return 0;
}

int CInspectionCore::DotCalibrationMode(HImage hImage, CRect rectDotCalib, HTuple &hDotWidths, HTuple &hDotHeights,
	HTuple &hDotRows, HTuple &hDotColumns)
{
	COLORREF colorGreen = RGB(0, 255, 0);

	HRegion hDotRegions;
	HTuple hResolutionAlongX, hResolutionAlongY;
	HTuple hInputAlongX, hInputAlongY, hInputMinRadiusDotTarget;

	hInputAlongX = m_CalibrationData.nInputAlongX;
	hInputAlongY = m_CalibrationData.nInputAlongY;
	hInputMinRadiusDotTarget = m_CalibrationData.nInputMinRadiusDotTargetPixel;

	CRect rectDotAdjacentCalib;

	_FCI_DotCalibration(hImage, &hDotRegions,
		rectDotAdjacentCalib.top, rectDotAdjacentCalib.left,
		rectDotAdjacentCalib.bottom, rectDotAdjacentCalib.right,
		rectDotCalib.top, rectDotCalib.left,
		rectDotCalib.bottom, rectDotCalib.right,
		hInputAlongX, hInputAlongY, hInputMinRadiusDotTarget,
		&hDotWidths, &hDotHeights,
		&hDotRows, &hDotColumns,
		&hResolutionAlongX, &hResolutionAlongY);

	m_arrayOverlayDotCalibration.Add(hDotRegions, colorGreen);
	if (hResolutionAlongX.Length() > 0)
		m_CalibrationData.dResolutionAlongXInMicronPerPixel = hResolutionAlongX.D();
	if (hResolutionAlongY.Length() > 0)
		m_CalibrationData.dResolutionAlongYInMicronPerPixel = hResolutionAlongY.D();

	return 0;
}