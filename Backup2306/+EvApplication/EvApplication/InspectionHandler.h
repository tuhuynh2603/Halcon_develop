/// CInspectionHandler: Vision Inspection manager class
/// Udupa; Jan'2015

#pragma once

#include "SupportFunction.h"
#include "AppDef.h"
#include "InspectionCore.h"
#include "Inspection.h"
#include "Parameters.h"

static CString READ_REGPATH = "Software\\Vision\\Sherlock_Spyglass\\";
static CString WRITE_REGPATH = "Software\\Vision\\Sherlock_Spyglass\\";



class CInspectionHandler : public CInspectionCore
{
public :
	CInspection m_Inspection;
	
	CInspectionHandler();
	~CInspectionHandler();
		
	virtual int Teach(CSharedData* pData,//IMAGEVIEW_INFO *pView,
				 CImageBuffer* pImgBufSrc,
				 CString strRecipeFolder,
				 CString strRecipeFile,
				 BOOL bAutoTeach,
				 long lDebugFlag);
	virtual int Inspect(CSharedData* pData,
				 long lDebugFlag,BOOL *m_bStepSelection);

	int LoadRecipe(CSharedData* pData,//IMAGEVIEW_INFO *pView,
					CImageBuffer *pImgBufSrc,
					CString strRecipeDir,
					CString strRecipeFile,
					int nTrack,
					int nDocCount,
					int nFOV,
					int nDoc);

	int Teach(CSharedData* pData,//IMAGEVIEW_INFO *pView,
				 CImageBuffer* pImgBufSrc,
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
				 CRect& rectEncapLoc = CRect(0,0,0,0));

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
				 CString strRecipeFolder,
				 CString strRecipeFile,
				 BOOL bRegionTeach,
				 int nCurTrack,
				 int nDoc,
				 int nFOV,
				 int nTotalFOV,
				 CCalibrationData* pCalibData);

	int SetTrainingData();
	int LoadTeachParameters(CString strRecipeFolder, CString strRecipeFile, int nFOV);
	int SaveTeachParameters(CString strRecipeFolder, CString strRecipeFile, int nFOV);

	int LoadInspectParameters(CString strRecipeFolder, CString strRecipeFile, int nFOV);
	int SaveInspectParameters(CString strRecipeFolder, CString strRecipeFile, int nFOV);

	int GetPVIDefectParameters(CString strRecipeFolder, int nTrack, int nFov);
	int SetPVIDefectParameters(CString strRecipeFolder, int nTrack, int nFov);
	int CopyPVIParameters(CString strRecipeFolder, CString strFileName, CInspectionHandler* pInspHandlerCopy, int nTrack, int nFOV);

	int LoadCalibrationParameters(CString strRecipeFolder, CString strRecipeFile, int nTrack, int nFOV);
	int SaveCalibrationParameters(CString strRecipeFolder, CString strRecipeFile, int nTrack, int nFOV);

	int LoadDotCalibrationParameters(CString strRecipeFolder, CString strRecipeFile, int nTrack, int nFOV=0);
	int SaveDotCalibrationParameters(CString strRecipeFolder, CString strRecipeFile, int nTrack, int nFOV=0);

	int ClearTeachParam(CTrainingData* pTrainingData);
	int ClearInspParam(CInspectinonData* pInspData, BOOL bInspClearAll);
	int ClearCalibParam(CCalibrationData* pCalibData, BOOL bClearAvg, BOOL bClearInsp=FALSE);
	int GetCalibDataForInsp(CCalibrationData* pCalibData);

	virtual void CreateParameterList(int nTrack, int nDoc, int nFOV);
	virtual void CreateTeachParmList(int nTrack/*int nTrack, int nDoc, int nFOV*/);
	void CleanUp();
	void CleanUpTeach();
	void CleanUpPVI();

	CArray<CParameters*> m_Parameters;
	CArray<int, int> nDefectErrorCode;
	CString m_strErrorMsg;

	CArray<CParameters*> m_ParametersTeach;
	CStringArray strArrayShapeType;
	CStringArray strArrayPatternSearchType;
	CRect rcTeachLocation[6];
	
	

protected:
    virtual int GetTeachParameters(CIniFile *ptrIniFile);
	virtual int SetTeachParameters(CIniFile *ptrIniFile);

};
