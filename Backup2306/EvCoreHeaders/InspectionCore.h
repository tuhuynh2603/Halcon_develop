/// CInspectionCore: Vision Inspection base class
/// Udupa; Feb'2015

#pragma once

#include "HalconCpp.h"
#include "HDevThread.h"
#include "ParameterCore.h"
#include "Parameters.h"

#define PVI_DEFECT_COUNT 11
// Contact
#define CONTACT_MAX_REGION_MASK 5
#define CONTACT_MAX_TRACING_MASK 5
#define CONTACT_MAX_POLYGON_POINT 20

// Flex
#define FLEX_MAX_SUBSTRATE_MASK 5
#define FLEX_MAX_TRACING_MASK 5
#define FLEX_MAX_DAMAGE_MASK 5
#define FLEX_MAX_POLYGON_POINT 20
// Encap
#define ENCAP_MAX_MASK_SURFACE 5
#define DARK_BLUE_ENCAP_ROI_POINT 8
#define ENCAP_GLUE_MASK_POINT 10
#define DARK_BLUE_ENCAP_INNER_INSUFFICIENT_MASK_POINT 6
#define DARK_ENCAP_INNER_INSUFFICIENT_MASK_POINT 6
using namespace HalconCpp;


#ifdef _LS3_IF
#define	DECLSPEC_LS3_IF _declspec(dllexport)
#else
#define DECLSPEC_LS3_IF _declspec(dllimport)
#endif

#define DEBUG_STEP_MODE 0x00000200

//
////Starting enum defect code is 3 because in "ImageSelectionDlg.h" they already define this enum
//enum DefectInfo
//{
//	NOT_INSPECTED = -2,
//	UNKNOWN_DEFECT = -1,
//	GOOD_UNIT = 0,
//	ERROR_PROCESS = -99
//};

enum {
	GOOD_UNIT,
	DEVICE_LOCATION = 3,
	DATUM_LOCATION,
	ENCAP_INSUFFICIENT,
	ENCAP_EXCESS,
	ENCAP_CRACK,
	ENCAP_BLOWHOLE,
	ENCAP_CONTAMINATION,
	ENCAP_VOID,
	ENCAP_WIREANDIC,
	ENCAP_BULGE,
	ENCAP_GLUE,
	// Contact Error
	CONTACT_DAMAGE,
	CONTACT_CONTAMINATION,
	// Flex Error
	FLEX_OVERCUT,
	FLEX_DAMAGE,
	FLEX_CONTAMINATION,
	FLEX_SCRATCHES,
	CHICKLET_ROUNDNESS,
	CHICKLET_BURR,
	PVI_INSP,
	RESULT_INVALID,
	USER_MODIFIED,
	NO_IMAGE,
	ERROR_CODES,
	MISSING_DIE
};


enum {
	THREAD_DIE_EDGE,
	THREAD_COUNT
};

enum {
	THREAD_DEBUG_DIE_EDGE,
	THREAD_DEBUG_COUNT
};


enum Areatype {
	RECTANGLE,
	POLYGON,
	ECLIPSE,
	POLYLINE
};

enum ReferencePointType {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	ENCAP_EDGE_LEFT,
	ENCAP_EDGE_RIGHT,
	CENTER,
	///
	/// Add new inspection codes here
	///
	REF_POINT_TYPE_COUNT
};

enum DeviceLocationType {
	BLOB_DETECTION,
	EDGE_DETECTION,
	///
	/// Add new inspection codes here
	///
	DEVICE_LOCATION_TYPE_COUNT
};


enum PatternType {
	CORRELATION_BASED,
	SHAPE_BASED,
	///
	/// Add new pattern type here
	///
	PATTERN_TYPE_COUNT
};

enum DirectionType {
	LEFT_TO_RIGHT,
	TOP_TO_BOTTOM,
	RIGHT_TO_LEFT,
	BOTTOM_TO_TOP,
	NONE,
	DIRECTION_TYPE_COUNT
};

enum ScratchDirectionFilter {
	FILTER_NONE,
	FILTER_HORIZONTAL_ONLY,
	FILTER_VERTICAL_ONLY,
	FILTER_HORIZONTAL_VERTICAL,
	FILTER_DIRECTION_COUNT
};

enum CharacteristicType {
	DEFECT_BLACK,
	DEFECT_WHITE,
	BOTH,
	CHARACTERISTIC_TYPE_COUNT
};
class DECLSPEC_LS3_IF CPVIDefect
{
public:
	CPVIDefect();

	int m_nID;
	int m_nErrorCodeID;
	int m_nInspPriority;
	CString strDefectName;

	BOOL bEnable;
	int nDefectCharacteristics;
	int m_nSize;
	int nContrast;
	int nMinLength;
	int nMinSquareSize;
	int nMinInnerWidth;
	int nMinCount;

	//1. Dot Filter 
	BOOL bDotDefectFilter;
	int nDotContrast;
	int nMinDotCircularity;
	int nMinDotLength;
	int nMinDotCount;

	//2. Scratch Filter
	BOOL bScratchDefectFilter;
	int nScratchCharacteristics;
	int nScratchContrast;
	ScratchDirectionFilter nScratchFilterDirection;
	int nScratchLength;
	int nScratchInnerWidth;
	int nMinScratchCount;
};

class DECLSPEC_LS3_IF CPVIInspectionSetup
{
public:
	CPVIInspectionSetup();
	~CPVIInspectionSetup();

	BOOL		bEnable;
	//BOOL		bNonUniformSurface;
	//int			nDeviceMaskThickness;
	//Areatype	nAreatype;
	Areatype	nDeviceAreaType;
	int			nDeviceMaskNumber;
	Areatype	nDeviceMaskArea[20];
	int			nDefectCount;
	double dDeviceEdgeWidth;
	double dDeviceEdgeOffset;

	//For Edge
	BOOL bEdgeEnable;
	int nMinEdgeContrast;
	int nMaxEdgeContrast;
	double dEdgeRegionOpen;
	double dEdgeWidth;
	double dEdgeOffset;

	BOOL bPVIRecheckEnable;
	int nMinGVDiff;
	int nIntensity;

	CRect m_rectPviArea;
	CRect m_rectPviMaskRegions[20];

	//For PVI Polygon
	CPoint m_polygonPviArea[20];
	int nPolygonPointNumber;

	//For PVI Edge
	CPoint m_polylinePviArea[20];
	int nPolylinePointNumber;

	//For PVI Polygon Masking
	CPoint m_polygonPviMask[20][20];
	int nPolygonPviMaskPointNumber[20];

	CPVIDefect	*m_pPviDefect;
};



class DECLSPEC_LS3_IF CEncap : public CParameterCore
{
public:
	CEncap();

	// // // // Blue Encap
	BOOL bEnableBlueEncap;
	double dBlueEncapErosionOffset;
	double dBlueEncapMaxLayOnAllowedPercentage;//Check Maximum Percentage of length of the defect lay on the encap border allowed
	CharacteristicType nCharacteristic;
	// Insuficient
	double dMinInSuffPercentageRightBlueEncap;
	double dMinInSuffPercentageLeftBlueEncap;
	double dMinSizeMidBlueEncap;
	// Excess
	int nContrastExcessBlueEncap;
	int nCircleContrastExcessBlueEncap;
	double dEdgeOffsetExcessBlueEncap;
	double dMinSizeExcessBlueEncap;
	// Glue - Mason
	double dBEncapMaskOffsetGlue;
	double dBEncapRegionOffsetGlue;
	double dBSmoothMaskSizeGlue;
	double dBMaskSizeGlue;
	double dBStandardDeviationGlue;
	int nBDiffMeanGrayGlue;
	int nBMinMeanGrayGlue;
	double dBMinSizeGlue;
	double dBMinConvexityGlue;
	double dBMaxInnerAreaPercentGlue;
	double dBMaxFuzzySizeGlue;
	int nBMaxDiffMeanGrayGlue;

	int nGluePriority;

	// Comtamination - Mason
	double dBMaskSizeCT;
	double dBStandardDeviationCT;
	int nBDiffMeanGrayCT;
	int nBBottomMaskContrastCT;
	int nBFlexMaskContrastCT;
	int nBMinGrayCT;
	double dBMinSizeCT;
	int nBMinDiffMeanGrayCT;
	int nBMinMeanEdgeMaskGrayCT;
	int nBMinMeanFlexMaskGrayCT;
	int nBMinMeanBorderedGrayCT;
	double dBMaxDiffAreaPercentCT;
	int nBMinDefectCountCT;

	int nContaminationPriority;

	//Encap Void
	double dBlueEncapVoidMinMaskSize;
	int nBlueEncapVoidMinDefectContrast;
	double dBlueEncapVoidMinSize;
	double dBlueEncapVoidMinCircularity;
	double dBlueEncapVoidMinLength;
	double dBlueEncapVoidMinSquareSize;
	int nBlueEncapVoidGrayMean;
	int nBlueEncapVoidGrayDeviation;
	int nBlueEncapVoidMinMeanGVDiffContamination;
	int nVoidPriority;
	double dBlueEncapFuzzyVoidMinSize;
	//Blue Encap Wire And IC
	int nBlueEncapWireAndIC;
	int nBlueEncapTopWireIntensity;
	double dBlueEncapTopWireMaxLineWidth;
	double dBlueEncapTopWireMinArea;
	int nBlueEncapBottomWireIntensity;
	double dBlueEncapBottomWireMaxLineWidth;
	double dBlueEncapBottomWireMinArea;
	double dBlueEncapMinAreaIC;
	int nWireAndICPriority;
	// Blue Encap BlowHole
	int nBlueEncapBHIntensity;
	double dBlueEncapBHMinCircularity;
	double dBlueEncapBHMinPercentLightArea;
	double dBlueEncapBHMinArea;
	double dBlueEncapBHSlideWindowSide;
	int nBlowHolePriority;

	// Bulge Blue Encap- Magnus
	int nBlueBulgeScaleContrastEncap;
	int nBlueBulgeValueContrastDefect;
	int nBlueBulgeScaleContrastDefect;
	double dBlueBulgeWidthLocalSearch;
	double dBlueBulgeHeightLocalSearch;
	double dBlueBulgeOffsetEncap;
	double dBlueBulgeClosingRadiusDefect;
	double dBlueBulgeMinAreaDefect;
	int nBlueBulgeCircularityDefect;
	int nBlueBulgeHWRatioDefect;
	int nBlueBulgeInnerDefectPercent;
	int nBlueBulgeOffsetCheckDefect;
	int nBlueBulgeMaxContrastInOutDefect;
	int nBlueBulgeMinGrayDefect;
	int nBlueBulgeMaxGrayDefect;
	int nBlueBulgeMaxDeviationInnerDefect;
	int nBulgePriority;// End Magnus

	// Dark Encap -magnus
	BOOL bEnableDarkEncap;
	//Excess 
	double dDarkEncapExcessOffset;
	double dDarkEncapExcessMinAreaDefect;
	int nDarkEncapExcessMinContrastDefect;
	int nDarkEncapExcessMaxContrastDefect;

	//Insufficient 
	int    nDarkEncapInsuffContrast;
	int    nDarkEncapInsuffVariationContrast;
	int    nDarkEncapInsuffWhiteContrast;
	double dDarkEncapInsuffLeftOffset;
	double dDarkEncapInsuffRightOffset;
	double dDarkEncapInsuffMinPercentLeftError;
	double dDarkEncapInsuffMinPercentRightError;
	double dDarkEncapInsuffMinAreaMidError;// End magnus
	//Void
	// // Small Void
	double dSmallVoidMinSizeDarkEncap;
	double dSmallVoidMinCircularityDarkEncap;
	double dSmallVoidGrayDeviationDarkEncap;
	int    nSmallVoidIntensityDarkEncap;
	double dSmallVoidMaxLayOnPartPercentageAllowDarkEncap;
	// // Large Void
	double dLargeVoidMinSizeDarkEncap;
	double dLargeVoidMinCircularityDarkEncap;
	int    nLargeVoidIntensityDarkEncap;
	double dLargeVoidMinGrayDeviationDarkEncap;
	double dLargeVoidMaxLayOnPartPercentageAllowDarkEncap;
	// // Fuzzy Void
	double dFuzzyVoidMinMeanGVDiffDarkEncap;
	int    nFuzzyVoidMinIntensityDarkEncap;
	double dFuzzyVoidMinSizeDarkEncap;
	double dFuzzyVoidMinAnisometryDarkEncap;
	double dFuzzyVoidMinGrayMeanDarkEncap;
	double dFuzzyVoidMinGrayDeviationDarkEncap;
	double dFuzzyVoidMaxLayOnPartPercentageAllow;
	// BlowHole
	double dBlowHoleSlideWindowSideDarkEncap;
	double dLargeBlowHoleAnisometryDarkEncap;
	double dSmallBlowHoleAnisometryDarkEncap;
	double dSmallBlowMaxGrayMeanDarkEncap;
	// Wire And IC
	double dWireMinLengthDarkEncap;
	int	   nWireIntensityDarkEncap;
	double dWireMaxLineWidthDarkEncap;
	int	   nWireContrastDarkEncap;
	double dMaxWireCircularityDarkEncap;

	int	   nICIntensityDarkEncap;
	double dICMinAreaDarkEncap;
	// Glue - Mason
	int nDarkContrastGlue;
	double dDarkMaskSizeGlue;
	double dDarkEncapRegionOffsetGlue;
	double dDarkMinSizeGlue;
	int nDarkMinMeanGrayGlue;
	double dDarkMinConvexityGlue;
	int nDarkMinDiffMeanGrayGlue;
	double dDarkMaxFuzzySizeGlue;
	double dDarkMaxWhiteAreaGlue;

	// Contamination - Mason
	double dDarkMaskSizeCT;
	double dDarkStandardDeviationCT;
	int nDarkDiffMeanGrayCT;
	int nDarkFuzzyContrastCT;
	int nDarkMinGrayCT;
	double dDarkMinSizeCT;
	double dDarkEncapRegionOffsetCT;
	int nDarkMinDiffMeanGrayCT;
	int nDarkMinMeanEdgeMaskGrayCT;
	double dDarkMaxDiffAreaPercentCT;
	int nDarkMinFuzzyGrayCT;
	int nDarkMinDiffMeanFuzzyGrayCT;
	int nDarkMinDefectCountCT;	
	// // // // 	
	// Dark Blue Encap
	BOOL bEnableDarkBlueEncap;

	// Excess - Mason
	int nDBContrastEX;
	int nDBCircleContrastEX;
	double dDBEdgeOffsetEX;
	double dDBMinAreaEX;

	// Insufficient - Mason
	int nDBWhiteContrastIS;
	int nDBBlackContrastIS;
	int nDBEdgeContrastIS;
	double dDBInnerEdgeOffsetIS;
	double dDBLeftEdgeOffsetIS;
	double dDBRightEdgeOffsetIS;
	int nDBMinDiffMeanGrayIS;
	double dDBMinInnerAreaIS;
	double dDBMinSmoothAreaPercentIS;
	double dDBMinEdgePercentIS;

	// Glue - Mason
	int nDBContrastGlue;
	double dDBEncapMaskOffsetGlue;
	double dDBFlexMaskOffsetGlue;
	double dDBSmoothMaskSizeGlue;
	double dDBMinSizeGlue;
	double dDBMinConvexityGlue;

	// Comtamination - Mason
	double dDBMaskSizeCT;
	double dDBStandardDeviationCT;
	int nDBDiffMeanGrayCT;
	int nDBBottomMaskContrastCT;
	int nDBFlexMaskContrastCT;
	int nDBMinGrayCT;
	double dDBMinSizeCT;
	int nDBMinDiffMeanGrayCT;
	int nDBMinMeanEdgeMaskGrayCT;
	int nDBMinMeanFlexMaskGrayCT;
	int nDBMinMeanBorderedGrayCT;
	double dDBMaxDiffAreaPercentCT;
	int nDBMinDefectCountCT;

	// Void Dark Blue Encap- Magnus
	int nDBlueVoidScaleContrastEncap ;
	int nDBlueVoidValueContrastDefect;
	int nDBlueVoidScaleContrastDefect ;
	double dDBlueVoidWidthLocalSearch ;
	double dDBlueVoidHeightLocalSearch;
	double dDBlueVoidOffsetEncap;
	double dDBlueVoidClosingRadiusDefect;
	double dDBlueVoidMinAreaDefect;
	int nDBlueVoidCircularityDefect;
	int nDBlueVoidHWRatioDefect;
	int nDBlueVoidInnerDefectPercent;
	int nDBlueVoidOffsetCheckDefect;
	int nDBlueVoidMaxContrastInOutDefect;
	int nDBlueVoidMinGrayDefect;
	int nDBlueVoidMaxGrayDefect;
	int nDBlueVoidMaxDeviationInnerDefect;// End Magnus

	// Blow Hole
	double dDarkBlueErosionRecHeight = 25;
	double dDarkBlueMinMaskSizeSmallBH = 9;
	double dDarkBlueMinSizeSmallBH = 140;
	double dDarkBlueMinCircularitySmallBH = 0.25;
	double dDarkBlueMaxLayOnPartPercentageAllowSmallBH = 60;

	double dDarkBlueMinMaskSizeLargeBH = 34;
	double dDarkBlueMinSizeLargeBH = 1800;
	double dDarkBlueMinCircularityLargeBH = 0.15;
	double dDarkBlueMaxLayOnPartPercentageAllowLargeBH = 35;

	//Wire And IC
	int nDarkBlueEncapLightWireMinIntensityWAI = 200;
	double dDarkBlueEncapLightWireMaxLineWidthWAI = 10;
	int nDarkBlueEncapLightWireContrastWAI = 30;
	double dDarkBlueEncapLightWireMinAreaWAI = 100;
	double dDarkBlueEncapLightWireMinLengthWAI = 25;
	double dDarkBlueEncapLightWireMinMeanGVDiffWAI = 15;

	double dDarkBlueEncapFuzzyWireMaxLineWidthWAI = 10;
	int nDarkBlueEncapFuzzyWireContrastWAI = 50;
	double dDarkBlueEncapFuzzyWireMinAreaWAI = 235;
	double dDarkBlueEncapFuzzyWireMinLengthWAI = 15;
	double dDarkBlueEncapFuzzyWireMinMeanGVDiffWAI = 7;

	double dDarkBlueEncapICMinAreaWAI = 400;};

// Flex Class Vison Parameters
class DECLSPEC_LS3_IF CFlex : public CParameterCore
{
public:
	CFlex();

	BOOL bEnable;

	// Overcut
	BOOL bEnableCircuitLineCutInspection;
	BOOL bEnableCircuitLineLinkedInspection;
	double dMinCircuitLineWidthOV;
	double dMaxCircuitLineWidthOV;
	int nOvercutPriority;

	// Damage
	BOOL bEnableCircuitLineDamageInspection;
	int nMaxContrastDM;
	double dMinSizeDM;
	int nMinDefectCountDM;
	int nDamagePriority;

	// Contamination
	int nMinContrastCT;
	int nMinSubstrateContrastCT;
	double dMinSizeCT;
	double dMinSquareSizeCT;
	double dMinLengthCT;
	double dMaxCircularityCT;
	int nMinDefectCountCT;
	int nComtaminationPriority;

	// Scratches
	int nMinContrastSC;
	double dMaxWidthSC;
	double dMinLengthSC;
	double dMaxCircularitySC;
	int nMinDefectCountSC;
	int nScratchesPriority;
};


class DECLSPEC_LS3_IF CChickletSwageHole : public CParameterCore
{
public:
	CChickletSwageHole();


	BOOL bEnable;
	//Roundness 100%
	int nRoundnessValueDefect;
	int nBurrValueForRoundness;
	//Burr area 
	int nBurrValueDefect;


};

// Contact Class Vison Parameters
class DECLSPEC_LS3_IF CContact : public CParameterCore
{
public:
	CContact();

	int nDamagePriority;
	int nComtaminationPriority;

	// // // // Black Contact
	BOOL bEnableBlackContact;

	// Damage
	double dBContactMaskSizeDM;
	double dBContactContactRegionOffsetDM;
	double dBContactMaxLineWidthDM;
	int nBContactLowContrastDM;
	int nBContactHighContrastDM;
	double dBContactMinLengthDM;
	double dBContactMaxCircularityDM;
	int nBContactMinDefectCountDM;

	// Contamination
	int nBContactHighContrastCT;
	double dBContactSmoothImageCT;
	double dBContactContactRegionOffsetCT;
	int nBContactContrastOffsetCT;
	int nBContactMinHoleNumberCT;
	int nBContactHoleNumberContrastOffsetCT;
	int nBContactVeryHighRoughDotRegionCT;
	int nBContactHighRoughDotRegionCT;
	int nBContactVeryHighRoughContrastOffsetCT;
	int nBContactHighRoughContrastOffsetCT;
	double dBContactMinStainSizeCT;
	int nBContactMinMeanEdgeGrayCT;
	double dBContactMinSizeCT;
	int nBContactMinMeanGrayCT;
	double dBContactMinAreaTracingPercentCT;
	int nBContactMinDefectCountCT;

	// // // // White Contact
	BOOL bEnableWhiteContact;

	// Damage
	double dWContactMaskSizeDM;
	double dWContactContactRegionOffsetDM;
	double dWContactMaxLineWidthDM;
	int nWContactLowContrastDM;
	int nWContactHighContrastDM;
	double dWContactMinLengthDM;
	double dWContactMaxCircularityDM;
	int nWContactMinDefectCountDM;

	// Contamination
	double dWContactSmoothImageCT;
	double dWContactContactRegionOffsetCT;
	int nWContactWhiteDefectContrastOffsetCT;
	int nWContactMeanWhiteDefectGrayOffsetCT;
	int nWContactMinMeanWhiteDefectGrayCT;
	int nWContactBlackDefectContrastOffsetCT;
	int nWContactMeanBlackDefectGrayOffsetCT;
	int nWContactMaxMeanBlackDefectGrayCT;
	double dWContactMinSizeCT;
	double dWContactMinAreaTracingPercentCT;
	int nWContactMinDefectCountCT;
};


//class DECLSPEC_LS3_IF CDieInsp : public CParameterCore
//{
//public:
//	CDieInsp();
//
//	BOOL bEnable;
//};

class DECLSPEC_LS3_IF CTeachInspectData
{
public:
	CTeachInspectData();
};


class DECLSPEC_LS3_IF CCalibrationData
{
public:
	CCalibrationData();

	CRect rectCalibSideTrack;
	CRect rectDotCalib;

	int nCalibTolerence;
	int nTargetGreyDeviation;
	int nTargetGreyMeanValue;

	int nTargetGreyMeanValueSide;
	int nCalibTolerenceSide;

	int nInputAlongX;
	int nInputAlongY;
	double dResolutionAlongXInMicronPerPixel;
	double dResolutionAlongYInMicronPerPixel;

	int nInputMinRadiusDotTargetPixel;

	HImage hCalibSumImage;
	HImage hCalibAvgImage;
	HTuple hResolutionYInput;
};

class DECLSPEC_LS3_IF CTrainingData
{
public:
	CTrainingData();

	// Device Location //
	BOOL bDeviceLocationEnable;
	DeviceLocationType nDeviceLocationType;//use to determine Algo Type for Detect Device Location

	int	nNoOfLocationTeachRegion;
	int	nMinDeviceContrast;
	double dDeviceLocWidth;
	double dDeviceLocHeight;
	double dMaskSize;
	BOOL bSobelAmp;
	ReferencePointType nDeviceReferencePointType;//use to determine the reference point used for compute the shifted/rotated between teach and inspect image
	int nDeviceLocSizeVariation;
	CRect m_rectDeviceLoc;		//// Max. rectangle draw for Device Loaction = 1

	//Using the Edge Detection instead of Device location Algo
	int nDeviceHorizontalContrast;
	DirectionType nDeviceHorizontalDirection;

	int nDeviceVerticalContrast;
	DirectionType nDeviceVerticalDirection;
	CRect m_rectDeviceHorizontalLineLoc;		//// Rectangle draw for Horizontal Search Line
	CRect m_rectDeviceVerticalLineLoc;		//// Rectangle draw for Vertical Search Line

// Dattam/Pattern Search //
	BOOL bDattamSearchEnable;
	double dDattamSearchAreaX;
	double dDattamSearchAreaY;
	int nDattamSearchMaxRotAngle;
	int nDattamSearchNoOfDattams;
	double dDattamSearchMinAcceptanceScore;
	PatternType nDatumPatternType;//Type Of pattern Search: --> Use ncc model or shape based model etc...

	CRect m_rectDattamSearch[4];		//// Max. rectangle draw for Dattam/Pattern Search = 4

// Region Search //
	BOOL bRegionSearchEnable;
	int nRegionSearchNoOfRegions;	//// Max. rectangle draw for Region Search = 10
	int nNoOfPviArea;
	// // // //	------------------------ Encap Tracing BLUE ENCAP ------------------------ // // // //
	// // // // Teach Region for Tracing Encap
	int nEncapTracingPolygonPointNumber;
	CPoint m_EncapTracingPolygonPointArea[20];

	// // Region Encap Top 
	int nEncapTopPolygonPointNumber;
	CPoint m_EncapTopPolygonPointArea[20];

	// // Region for Tracing Encap Bottom
	int nEncapBottomPolygonPointNumber;
	CPoint m_EncapBottomPolygonPointArea[20];

	// // Region Find Left Line Flex for Generating Masking Encap
	CRect m_rectEncapMaskingLeftPolygonPointArea;

	// //  Region Find Right Line Flex for Generating Masking Encap
	CRect m_rectEncapMaskingRightPolygonPointArea;

	// // Region Encap Masking Limit 
	CRect m_rectEncapMaskingLimitPolygonPointArea;
	// // // // Insufficient
	// // Left Encap Region For Checking Insufficient
	CRect m_rectBlueEncapInsuffLeftRegionPolygonPointArea;

	// // Right Encap Region For Checking Insufficient
	CRect m_rectBlueEncapInsuffRightRegionPolygonPointArea;

	// // Region For Find border of the base
	CRect m_rectBlueEncapInsuffBoderBaseRegionPolygonPointArea;

	// // Middle Encap Region For Checking Insufficient
	int nBlueEncapInsuffMidRegionPolygonPointNumber;
	CPoint m_BlueEncapInsuffMidRegionPolygonPointArea[20];

	// // // // Excess
	// // Left Encap Region For Checking Excess
	CRect m_rectBlueEncapExcessLeftRegionPolygonPointArea;
	// // Right Encap Region For Checking Excess
	CRect m_rectBlueEncapExcessRightRegionPolygonPointArea;
	// // // // Glue - Mason
	CPoint m_polygonBEncapGlueMask[ENCAP_GLUE_MASK_POINT];
	HObject hBEncapGlueMask;

	// // // // Contamination - Mason
	CRect m_rectBEncapFlexEdgeMaskROI;
	HObject hBEncapFlexEdgeMaskROI;

	// // // // Wire And IC
	// // Region for Inspect Wire
	int nEncapWirePolygonPointNumber;
	CPoint m_EncapWirePolygonPointArea[20];

	// // Region for Inspect Wire (Bottom)
	int nEncapWireBottomPolygonPointNumber;
	CPoint m_EncapWireBottomPolygonPointArea[20];

	// // Region for Inspect Wire (Bottom)
	int nEncapICPolygonPointNumber;
	CPoint m_EncapICPolygonPointArea[20];
	// Encap Region //
	BOOL bEnableEncap;
	//Tracing Encap Blue //
	BOOL bEnableEncapBlue;
	double dErosionRadiusEncapTracing;
	int nSmoothingIndexEncapTracing;
	int nIntensityEncapTracing;
	double dSlideWindowSideEncapTracing;
	int nEdgeFlexContrastEncapTracing;
	double dSizeMaskingEncapTracing;
	int nIntensityForFindBorderBaseBlueEncap;
	double dOffsetLeftBlueEncap;
	double dOffsetRightBlueEncap;

	CRect m_rectDarkTopEncap;
	int m_nDarkTopRectWidth;
	int m_nDarkTopRectHeight;

	CRect m_rectDarkBottomEncap;
	int m_nDarkBottomRectWidth;
	int m_nDarkBottomRectHeight;

	CRect m_rectDarkLeftEncap;
	int m_nDarkLeftRectWidth;
	int m_nDarkLeftRectHeight;

	CRect m_rectDarkRightEncap;
	int m_nDarkRightRectWidth;
	int m_nDarkRightRectHeight;
	//Masking for encap tracing
	int nEncapTracingMaskNumber;
	int nEncapTracingMaskPolygonPointNumber[5];
	CPoint m_EncapTracingMaskPolygonPointArea[5][20];
	//Masking for encap surface inspection

	CArray<HTuple> hvec_TupleVectorLocation;
	CArray<HTuple> hvec_TupleVectorPVI[32];
	CArray<double> darrayPVI[32][10];

	CArray<HTuple> hvec_TupleVectorEncap;
	CArray<double> darrayEncap[15];

	HObject hObjectDeviceLocation;
	HTuple hTupleProjectionParam;
	HTuple hTupleTeachDoc;
	HTuple hTupleEdgeTeachTuple;
	HTuple hTupleDatumTeachTuple;
	HTuple hTupleDatumCenterParam;

	// Contact Region //
	BOOL bEnableContact;

	// Black Contact
	BOOL bEnableBlackContact;
	int nBContactContactContrast;
	int nBContactCircleContrast;

	// White Contact
	BOOL bEnableWhiteContact;
	int nWContactContactContrast;
	int nWContactCircleContrast;

	// Contact Mask
	int nContactMaskNumber;
	CRect m_rectContactMask[CONTACT_MAX_REGION_MASK];

	// Contact Region Mask;
	int nContactRegionMaskNumber;
	CRect m_rectContactRegionMask[CONTACT_MAX_REGION_MASK];

	// Tracing Mask
	int nContactTracingMaskNumber;
	int nContactTracingMaskPolygonPointNumber[CONTACT_MAX_TRACING_MASK];
	CPoint m_polygonContactTracingMask[CONTACT_MAX_TRACING_MASK][CONTACT_MAX_POLYGON_POINT];

	// Contact Teaching Data
	HObject hContactMask;
	HObject hContactRegionMask;
	HObject hContactTracingMask;

	// Flex Region //
	BOOL bFlexEnable;
	BOOL bFlexEnableSubstrateMask;
	BOOL bFlexEnableDynamicEdge;
	double dFlexEdgeOffset;
	int nFlexEdgeContrast;
	int nFlexCircuitLineContrast;
	int nFlexEncapContrast;
	double dFlexOffsetMaskSize;
	int nFlexStraightCircuitLineMaskPolygonPointNumber;

	CRect m_rectFlexSubstratePatternROI1;
	CRect m_rectFlexSubstratePatternROI2;
	CRect m_rectFlexSubstrateSearchPatternROI;

	CPoint m_polygonFlexTopEdgeLine[2];
	CPoint m_polygonFlexBottomEdgeLine[2];
	CPoint m_polygonFlexLeftEdgeLine[2];
	CPoint m_polygonFlexRightEdgeLine[2];

	CRect m_rectFlexLeftEdgeLineMask;
	CRect m_rectFlexRightEdgeLineMask;

	CRect m_rectFlexBottomROI;
	CPoint m_polygonFlexStraightCircuitLineMask[FLEX_MAX_POLYGON_POINT];

	CRect m_rectFlexNumberMask;

	// Tracing Mask
	int nFlexTracingMaskNumber;
	int nFlexTracingMaskPolygonPointNumber[FLEX_MAX_TRACING_MASK];
	CPoint m_polygonFlexTracingMask[FLEX_MAX_TRACING_MASK][FLEX_MAX_POLYGON_POINT];

	// Damage Mask
	int nFlexDamageMaskNumber;
	int nFlexDamageMaskPolygonPointNumber[FLEX_MAX_DAMAGE_MASK];
	CPoint m_polygonFlexDamageMask[FLEX_MAX_DAMAGE_MASK][FLEX_MAX_POLYGON_POINT];

	// Substrate Mask
	int nFlexSubstrateMaskNumber;
	int nFlexSubstrateMaskPolygonPointNumber[FLEX_MAX_SUBSTRATE_MASK];
	CPoint m_polygonFlexSubstrateMask[FLEX_MAX_SUBSTRATE_MASK][FLEX_MAX_POLYGON_POINT];

	// Flex Data Teaching
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

	HTuple hFlexSubstrateModelID;
	HTuple hFlexSubstrateDatumMatchRowRef;
	HTuple hFlexSubstrateDatumMatchColRef;
	HTuple hFlexSubstrateMatchAngleRef;
	HObject hFlexTracingMaskRegion;
	HTuple hFlexLeftWidthReference;
	HTuple hFlexRightWidthReference;

	/* Chicklet Swage Hole Inspection */
	BOOL bEnableChickletSwageHole;
	CRect m_rectChickletROI[10];
	int m_nChickletMinContrast;
	int m_nChickletMaxContrast;
	int m_nChickletOffsettRadius;
	int m_nChickletNumberOfHoles;

	// Tracing Dark Encap
	BOOL bEnableDarkEncap;
	int m_nSmoothDarkEncap;
	int m_nLeftRightContrastDarkEncap;
	int m_nWhiteContrastDarkEncap;
	int m_nAreaWhiteRemoveDarkEncap;
	int m_nRoughContrastDarkEncap;
	int m_nContrastFlexDarkEncap;
	int m_nMinContrastFlexDarkEncap;
	int m_nMaxContrastFlexDarkEncap;

	CRect m_rectAllRegionDarkEncap;
	CRect m_rectTopFlexDarkEncap;
	CRect m_rectBotWhiteDarkEncap;
	int nDarkEncapOutsidePolygonPointNumber;
	CPoint m_DarkEncapOutsidePolygonPointArea[20];
	int nDarkEncapInsidePolygonPointNumber;
	CPoint m_DarkEncapInsidePolygonPointArea[20];

	// Excess - Magnus
	CRect m_rectDarkExcessLeftMask;
	CRect m_rectDarkExcessRightMask;

	// Insufficient - Magnus
	CRect m_rectDarkInsufficientCoverMask;
	CPoint m_polygonDarkInsufficientMidMask[DARK_ENCAP_INNER_INSUFFICIENT_MASK_POINT];
	CRect m_rectDarkInsufficientLeftMask;
	CRect m_rectDarkInsufficientRightMask;
	// // Void
	CRect m_rectDarkEncapVoidMaskingLeftDiePointArea;
	CRect m_rectDarkEncapVoidMaskingRightDiePointArea;
	// // Wire And IC
	int nDarkEncapWirePolygonPointNumber;
	CPoint m_DarkEncapWirePolygonPointArea[20];
	int nDarkEncapICPolygonPointNumber;
	CPoint m_DarkEncapICPolygonPointArea[20];
	// Glue - Mason
	CRect m_rectDarkGlueMask;
	HObject hDarkGlueMask;

	// Contamination - Mason
	CRect m_rectDarkFlexEdgeMask;
	CRect m_rectDarkBottomEdgeMask;
	CRect m_rectDarkRightEdgeMask;
	CRect m_rectDarkInnerMask;
	HObject hDarkFlexEdgeMask;
	HObject hDarkBottomEdgeMask;
	HObject hDarkRightEdgeMask;
	HObject hDarkInnerMask;

	// Dark Blue Encap Region // // // -------------------------------------- DARK BLUE ENCAP ----------------------------- // // // // 
	bool bEnableDarkBlueEncap;
	int nDBEncapEdgeEncapContrast;
	int nDBEncapEdgeFlexContrast;
	int nDBEncapMinEncapContrast;
	int nDBEncapMaxEncapContrast;

	CPoint m_polygonDBEncapExtractROI[DARK_BLUE_ENCAP_ROI_POINT];
	CRect m_rectDBEncapInnerROI;
	CRect m_rectDBEncapTopEdgeMaskROI;
	CRect m_rectDBEncapBottomEdgeMaskROI;
	CRect m_rectDBEncapLeftEdgeMaskROI;
	CRect m_rectDBEncapRightEdgeMaskROI;
	CRect m_rectDBEncapFlexEdgeMaskROI;

	// Dark Blue Encap Data Teaching
	HObject hDBEncapExtractROI;
	HObject hDBEncapInnerROI;
	HObject hDBEncapTopEdgeMaskROI;
	HObject hDBEncapBottomEdgeMaskROI;
	HObject hDBEncapLeftEdgeMaskROI;
	HObject hDBEncapRightEdgeMaskROI;
	HObject hDBEncapFlexEdgeMaskROI;

	// Excess - Mason
	CRect m_rectDBLeftExcessMask;
	CRect m_rectDBRightExcessMask;
	HObject hDBLeftExcessMask;
	HObject hDBRightExcessMask;

	// Insufficient - Mason
	CRect m_rectDBCoverInsufficientMask;
	CPoint m_polygonDBInnerInsufficientMask[DARK_BLUE_ENCAP_INNER_INSUFFICIENT_MASK_POINT];
	CRect m_rectDBLeftInsufficientMask;
	CRect m_rectDBRightInsufficientMask;
	HObject hDBCoverInsufficientMask;
	HObject hDBInnerInsufficientMask;
	HObject hDBLeftInsufficientMask;
	HObject hDBRightInsufficientMask;

	// Glue - Mason
	CPoint m_polygonDBEncapGlueMask[ENCAP_GLUE_MASK_POINT];
	HObject hDBEncapGlueMask;

	//Dark Blue Region for Void encap Inspection - magnus
	CRect m_rectDBlueVoidROI;
	// Region for Sliver encap Inspection
	CRect m_rectDBlueSliverROI;
	CRect m_rectDBlueSliverTopROI;
	CRect m_rectDBlueSliverEncapROI;

	//Blue Region for Bulge encap Inspection
	CRect m_rectBlueBulgeROI;
	//Blue Region for Sliver encap Inspection
	CRect m_rectBlueSliverROI;
	CRect m_rectBlueSliverTopROI;
	CRect m_rectBlueSliverEncapROI;//End magnus	// Blow Hole
	// // Region for tracing Left Mask to inspect BlowHole
	CRect m_rectDarkEncapBHMaskingLeftPolygonPointArea;
	// // Region for tracing Right Mask to inspect BlowHole
	CRect m_rectDarkEncapBHMaskingRightPolygonPointArea;

	// Wire And IC
	// // Region for Inspect Wire And IC on Dark Blue Encap (Bottom)
	int nDarkBlueEncapWireBottomPolygonPointNumber;
	CPoint m_DarkBlueEncapWireBottomPolygonPointArea[20];
	// Region for Inspect IC on Dark Blue Encap
	int nDarkBlueEncapICPolygonPointNumber;
	CPoint m_DarkBlueEncapICPolygonPointArea[20];

	HObject hDarkBlueEncapBHMaskingLeft;
	HObject hDarkBlueEncapBHMaskingRight;
	HObject hDarkBlueEncapWireBottom;
	HObject hDarkBlueEncapIC;
};


class DECLSPEC_LS3_IF CInspectinonData
{
public:
	CInspectinonData();

	HTuple hStoreTeachModelRowCentre;
	HTuple hStoreTeachModelColCentre;
	HTuple hStoreMatchModelCentreRow;
	HTuple hStoreMatchModelCentreCol;



	HTuple hInspectShiftAlongXMicron;
	HTuple hInspectShiftAlongYMicron;
};

typedef CArray<HRegion> HRegionArray;
typedef CArray<UINT> UINTArray;

class COverlayData
{
public:
	HImage hImage;
	HRegion region;
	UINT color;
	CString strDebugMessage;

	COverlayData()
	{
	}

	COverlayData(const COverlayData& src)
	{
		hImage = src.hImage;
		region = src.region;
		color = src.color;
		COverlayData::strDebugMessage = strDebugMessage;
	}

	COverlayData(HImage hImage, HRegion region, UINT color, CString strDebugMessage)
	{
		COverlayData::hImage = hImage;
		COverlayData::region = region;
		COverlayData::color = color;
		COverlayData::strDebugMessage = strDebugMessage;
	}

	~COverlayData()
	{
	}
};

class COverlayArray : public CArray<COverlayData>
{
public:
	/*HImage hImage;
*/
	void Add(HImage hImage, HRegion region, UINT color, CString strDebugMessage)
	{
		CArray::Add(COverlayData(hImage, region, color, strDebugMessage));
	}

	void Add(HRegion region, UINT color, CString strDebugMessage = "")
	{
		CArray::Add(COverlayData(HRegion(), region, color, strDebugMessage));
	}
};

typedef struct {

	HImage hImageBF, hImageDF, hImageSS, hImageSSB;
	HRegion hMaskRegions, hOpticalRegion;
	HObject hPrintWithOpticalRegion;
	HRegion hRegionMarksDilate;

	HTuple  hWidth, hHeight;
	HTuple  hLensCenterRow, hLensCenterCol;

	HObject hPatternLayerRegion;
	//-----Len Location-------
	HImage hLensEdgeImage, hInnerLensRegion, hLensRegion;
	HTuple hEdgeThreshold, hInnerRegionOffset;
	HTuple hArea, hRow, hColumn, hRowPolar, hColPolar, hRadiusPolar, hCircularity;
	HTuple hInnerRadius, hOuterRadius, hMaxRadius, hDiameterDiff;
	HRegion hLensEdge, hLensEdgeRegion, hPatternRegion, hCuvetteRegion;

	HRegion hDarkRegionisPTInBF, hBrightRegionInDF;

	HRegion hImageLensRegionBF, hImageLensRegionDF, hImageSPZoom;
	HRegion hImageLensEdgeRegionBF, hImageLensEdgeRegionDF, hImageLensEdgeRegionSS;
	HRegion hImageLensRegion, hLensInnerRegion;
	HRegion hImageLensInnerRegionBF, hImageLensInnerRegionDF, hImageLensInnerRegionSS;
	HObject hLocationMarkRegion;
	//Inner Region
	HRegion hLensTrueInnerRegion, hLensEdgeTrueRegionBF, hLensEdgeTrueRegionDF;
	HImage hLensTrueInnerRegionDF, hLensTrueInnerRegionBF;

	HImage hImageLensTrueInnerRegionDF, hImageLensTrueInnerRegionBF;
	HImage hLensEdgeRegionPT, hImageLensEdgePT;
	HObject hImageLensEdgePT_DF;

	HObject hAllObjectForRoughness, hBrightRegionInDFRoughness, hAllObjectForRoughnessPolar;
	HRegion hLensEdgeRegionInBF;

	int nStepDebug;
	BOOL bInspectAll;
	HTuple  hOverlapAnglePolar, hStartAnglePolar, hEndAnglePolar, hOverlapXY;

	HTuple  hLensRadius, hLensRadiusOffset;
	HTuple  hCuvetteRa, hCuvetteRb;
	HTuple  hCuvetteRow, hCuvetteCol, hCuvetteRadius;
	HTuple hRadiusStartPolar, hRadiusEndPolar, hWidthPolar, hHeightPolar;

	BOOL bIsPattern;
	BOOL bIsLocationMark;

	HTuple hNumberOfLayer;
	HRegion hPolarTransDarkPatternRegion, hPolarTransBrightPatternRegion;
	HRegion hPatternInspRegion, hDarkPatternRegionUnion;

	HObject hLensTrueInnerRegionSS, hLensEdgeTrueRegionSS, hPatternRegionForSurfaceInspSS;
	HRegion hImageCuvetteRegionBF, hImageCuvetteRegionDF, hImageCuvetteRegionSS, hImageCuvetteRegionSSB;

	// Thread Shared Parameters
	HImage hImageThread, hTeachImageThread;
	int nCurTrackThread, nCurDocThread, nTotalDocThread, nCurFOVThread, nTotalFOVThread, lDebugFlagThread;
	HTuple hInspectRotationAngleThread, hInspectShiftAlongRowThread, hInspectShiftAlongColThread;
	BOOL bRegionInspThread;
	CCalibrationData* pCalibDataThread;
	HRegion hDeviceLocationSelectedThread;

} SHARED_PARM;

typedef struct {
	HANDLE m_hThread;
	HANDLE m_hRun;
	HANDLE m_hDone;
	long lDebugFlag;
	HTuple bPass;

} THREAD_PARM;

class DECLSPEC_LS3_IF CInspectionCore
{
public:
	CInspectionCore();
	~CInspectionCore();

	CEncap m_EncapParm;
	// Flex Class
	CFlex m_FlexParm;

	CChickletSwageHole	m_ChickletSwageHole;

	CContact m_ContactParm;

	SHARED_PARM *m_pShare;
	BOOL m_bParallelism;
	BOOL m_bParallelismRegistry;
	THREAD_PARM m_ThreadParms[THREAD_COUNT];
	HANDLE m_hDoneEvents[THREAD_COUNT];
	HANDLE handlesDone[THREAD_COUNT];

	int m_nThreadIndex;
	int m_nProcessErrorLocal;
	BOOL m_bKillInspectThreads;
	BOOL m_bInitiateClearCache;
	BOOL m_bClearThreadCache;

	CTrainingData m_TrainingData;
	CTeachInspectData m_TeachInspectData;
	CCalibrationData m_CalibrationData;

	CInspectinonData m_InspectionData;
	CPVIInspectionSetup m_pPviInspectionSetup[10];

	BOOL m_bInspectionPriority;

	BOOL m_bIsPatternLens;
	double m_dPixelSize;
	BOOL m_bStepDebugSelection[THREAD_COUNT];

	virtual int Initialize(BOOL bParallel);
	virtual int InitializeInspection();
	virtual int InitializeRegionInspection();

	int Close();
	//int Reset();
	/*virtual*/ int Teach(HImage hImage,
		BOOL bRegionTeach,
		BOOL *bEnable,
		int nTeachStep,
		int nCurTrack,
		int nDoc,
		int nFOV,
		int nTotalFOV,
		CTrainingData* pTrainingData,
		CCalibrationData* pCalibData,
		BOOL bEncapLoc,
		CRect& rectEncapLoc);

	int AutoTeach(HImage hImage,
		BOOL bRegionTeach,
		int nCurTrack,
		int nDoc,
		int nFOV,
		int nTotalFov,
		CCalibrationData* pCalibData);

	virtual int Inspect(HImage hImage,
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
		CStringArray& strArrayInspValues);

	int Inspect(int nThreadIndex, HTuple& bPass);
	int ClearTeachParam(CTrainingData* pTrainingData);
	int ClearInspParam(CInspectinonData* pInspData, BOOL bInspClearAll);
	int ClearCalibParam(CCalibrationData* pCalibData, BOOL bClearAvg, BOOL bClearInsp);
	int GetCalibDataForInsp(CCalibrationData* pCalibData);

	int LightCalibrationMode(HImage hImage, int nGreyValue,
		HImage* hImageCalibPos, HImage* hImageCalibNeg, HImage* hImageCalibResult,
		BOOL bFirst, BOOL bRefImg, BOOL bAvgImg, BOOL bLightCorrection,
		CCalibrationData* pCalibData);

	int DotCalibrationMode(HImage hImage, CRect rectDotCalib, HTuple &hDotWidths, HTuple &hDotHeights,
		HTuple &hDotRows, HTuple &hDotColumns);

	COverlayArray m_arrayOverlay[2];
	COverlayArray m_arrayOverlayDotCalibration;
	COverlayArray m_arrayOverlayInspection;
	COverlayArray m_arrayOverlayTeach;
	COverlayArray m_arrayOverlayPVIInsp;
	CStringArray m_strInspectionResult[5][5];
	COverlayArray m_arrayOverlayDisplayLoc;

	void StepDebug(HImage hImage, HRegion hDebugRgn, UINT color, HTuple hDebugMsg, int nRegion);
	void ZoomImage(HImage hImgSrc, HImage hImgDst, int nWidth, int nHeight, int nDoc);
	void ReleaseMemory();
	void ClearCache();
};

UINT DECLSPEC_LS3_IF InspectionThread(LPVOID pParam);
