/// Udupa; Image Queue Handler to manage and save images in a queue
/// Udupa; Dec'2013; Defect overlay queuing option

#include "stdafx.h"
#include "Application.h"
#include "ImageQueueHandler.h"
#include "DisplayOverlay.h"
#include "TrackHandler.h"
#include "AppDoc.h"
#include <direct.h>

CImageQueueHandler::CImageQueueHandler()
{
}

CImageQueueHandler::~CImageQueueHandler()
{
	m_bActive = FALSE;
	SetEvent(m_hPushed);
	if(WaitForSingleObject(m_hImageSaveThread, 1000) != WAIT_OBJECT_0)
		TerminateThread(m_hImageSaveThread, 0);

	SetEvent(m_hPushedInspect);
	if (WaitForSingleObject(m_hImageInspectThread, 1000) != WAIT_OBJECT_0)
		TerminateThread(m_hImageInspectThread, 0);

	DeleteCriticalSection(&m_csQueue);
	//delete[] m_strImageFolders;
	CloseHandle(m_hPushed);
	CloseHandle(m_hPushedInspect);
}

int CImageQueueHandler::Initialize(CSize szImage)
{
	m_pApp = (CApplication*) AfxGetApp();
	int nTracks = m_pApp->m_nTotalTracks;

	m_bActive = TRUE;
	m_hPushed = CreateEventA(NULL, FALSE, FALSE, NULL);
	m_hPushedInspect = CreateEventA(NULL, FALSE, FALSE, NULL);
	InitializeCriticalSection(&m_csQueue);

//	m_hImageSaveThread = m_pApp->m_ThreadManager.CreateThread(ImageSaveThread, this, IMAGE_SAVE_THREAD);

	m_hImageSaveThread = AfxBeginThread(ImageSaveThread, this, THREAD_PRIORITY_LOWEST)->m_hThread;
	SetThreadAffinityMask(m_hImageSaveThread, 0x80); 

	m_hImageInspectThread = AfxBeginThread(ImageInspectThread, this, THREAD_PRIORITY_LOWEST)->m_hThread;
	SetThreadAffinityMask(m_hImageInspectThread, 0x80);
	//// create insp thread

	//m_strImageFolders = new CString[nTracks];
	CString strTemp, strTemp1;
	/*m_strImageFolders.Format("%s\\Online Images", m_pApp->m_ImageSaveFolder); for issue on 29-9-2017 
	_mkdir(m_strImageFolders)*/;

	//for(int i=0; i<nTracks; i++) {
	//	m_strImageFolders[i].Format("%s\\Track %d", strTemp, i+1);
	//	_mkdir(m_strImageFolders[i]);
	//}
	//	strTemp1.Format("%s\\Passed Lens", m_strImageFolders[i]);
	//	_mkdir(strTemp1);
	//	strTemp1.Format("%s\\Failed Lens", m_strImageFolders[i]);
	//	_mkdir(strTemp1);
	//}
	m_BufDefect.Allocate(szImage, true, TRUE);
	m_BufDefect.ClrImg();

	return 0;
}

int CImageQueueHandler::FlushQueueBuffers()
{
	EnterCriticalSection(&m_csQueue);
	while(!m_BackImageQueue.empty()) {
		CQueueBuffer* pBuf = m_BackImageQueue.front();
		m_BackImageQueue.pop();
		delete pBuf;
	}
	LeaveCriticalSection(&m_csQueue);

	return 0;
}

int CImageQueueHandler::PushImageBuffer(CImageBuffer* pImageBuffer, int nTrackIndex,int nDocIndex,int nDieIndex,int nFovIndex, int nErrorCode, CRectArray& rects, CRegionArray& regions)
{

	CPerformanceTimer timer;
	timer.Reset();

	EnterCriticalSection(&m_csQueue);
	CQueueBuffer* pBuf = new CQueueBuffer(
		pImageBuffer, 
		nTrackIndex, 
		nDocIndex,
		nDieIndex,
		nFovIndex,
		nErrorCode,
		rects,
		regions);
	m_BackImageQueue.push(pBuf);
	SetEvent(m_hPushed);
	LeaveCriticalSection(&m_csQueue);

	OutputDebugLogTo(6, TRUE, "[Track%d] Image Push Time: %5.2fms", nTrackIndex, timer.GetElapsedMSec());

	return 0;
}

int CImageQueueHandler::PushInspectImageBuffer(CImageBuffer* pImageBuffer, int nTrackIndex, int nDocIndex, int nDieIndex, int nFovIndex)
{

	//CPerformanceTimer timer;
	//timer.Reset();

	//EnterCriticalSection(&m_csQueue);
	//CQueueBuffer* pBuf = new CQueueBuffer(
	//	pImageBuffer,
	//	nTrackIndex,
	//	nDocIndex,
	//	nDieIndex,
	//	nFovIndex);
	//m_BackImageQueue.push(pBuf);

	//SetEvent(m_hPushedInspect);
	//LeaveCriticalSection(&m_csQueue);

	//OutputDebugLogTo(6, TRUE, "[Track%d] Image Push Time: %5.2fms", nTrackIndex, timer.GetElapsedMSec());

	return 0;
}


int CImageQueueHandler::SaveImage(CQueueBuffer* pBuf)
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString strTemp;
	CString strFile, strImageDesc;
	strImageDesc.Format("HP_FCI_THA%d_Pos%d_",pBuf->m_nDie+1,pBuf->m_nFov+1);
	//strImageDesc = m_pApp->GetFailedStr(pBuf->m_nErrorCode,FALSE);
	switch(pBuf->m_nTrack){	
	case 0:		strTemp.Format("T%d",pBuf->m_nDoc+1);
			strImageDesc.Append(strTemp);
			break;
	case 1: strTemp.Format("B%d",pBuf->m_nDoc+1);
			strImageDesc.Append(strTemp);
			break;
	case 2:	strTemp.Format("S%d",pBuf->m_nDoc+1);
			strImageDesc.Append(strTemp);
			break;
	}
	m_strImageFolders.Format("%s\\Online Images", m_pApp->m_ImageSaveFolder);
	_mkdir(m_strImageFolders);

	CString strFolder, strRecipeFolder, strImage;
	strRecipeFolder.Format("%s\\%s", m_strImageFolders,m_pApp->m_strConfigFile);
	_mkdir(strRecipeFolder);
	strTemp.Format("%s\\%s", strRecipeFolder, m_pApp->m_strLotInfo);
	_mkdir(strTemp);
	BOOL bPass = !IS_FAIL(pBuf->m_nErrorCode);
	if(!bPass)
		strFolder.Format("%s\\FAIL", strTemp);
	else
		strFolder.Format("%s\\PASS", strTemp);
	_mkdir(strFolder);
	
	strTemp.Format("%s\\%s", strFolder, m_pApp->m_strPartFlex[pBuf->m_nDie]);
	_mkdir(strTemp);
	strFile.Format("%s\\%s",strTemp,strImageDesc);
	if ((bPass && m_pApp->m_bSaveOnlinePassImages && m_pApp->m_bSaveOnlineBMPImages) || (!bPass && m_pApp->m_bSaveOnlineFailImages && m_pApp->m_bSaveOnlineBMPImages)) {
		SaveGrayScaleBMPFile(strFile + ".bmp", *pBuf->m_pImageBuffer);
		OutputDebugLog("%s saved!", strFile);
	}
	if ((bPass && m_pApp->m_bSaveOnlinePassImages && m_pApp->m_bSaveOnlineJpegImages) || (!bPass && m_pApp->m_bSaveOnlineFailImages && m_pApp->m_bSaveOnlineJpegImages)) {
		DrawResultText(pBuf, &sysTime);
		SaveJPGFile(strFile + ".jpg", m_BufDefect, m_pApp->m_nSaveJpegQualityImages);
	}

	return 0;
}

void CImageQueueHandler::DrawResultText(CQueueBuffer* pBuf, SYSTEMTIME* pSysTime)
{
	LOGPEN TextColor;
	TextColor.lopnStyle = PS_SOLID;
	TextColor.lopnWidth.x = 10;
	TextColor.lopnColor = IS_FAIL(pBuf->m_nErrorCode) ? RGB(50,50,230) : RGB(50,230,50);

	CString strDate;
	strDate.Format("%02u/%02u/%04u  @%02u:%02u:%02u", 
		pSysTime->wMonth, pSysTime->wDay, pSysTime->wYear, 
		pSysTime->wHour, pSysTime->wMinute, pSysTime->wSecond);

	CSize szImage = m_BufDefect.GetImgSize();
	CRect rcROI = CRect(CPoint(), szImage);
	ImgCopy(pBuf->m_pImageBuffer, &rcROI, &m_BufDefect, &rcROI);
	CDC* pDC = m_BufDefect.GetDrawContext();
	if(pDC) {
		DrawOverlayText(pDC, szImage, m_pApp->GetFailedStr(pBuf->m_nErrorCode, FALSE), CPoint(50, 0), TextColor, 100);
		DrawOverlayText(pDC, szImage, strDate, CPoint(szImage.cx-1150, 0), TextColor, 100);
		//DrawOverlayText(pDC, szImage, strText, CPoint(50, m_Buffer.GetImgSize().cy - 150), TextColor, 100);
		DrawDefectRegions(pDC, szImage, pBuf->m_nErrorCode, pBuf->arrRects, pBuf->arrRegions);
		m_BufDefect.ReleaseDrawContext();
	}
}

UINT ImageSaveThread(LPVOID pParam)
{
	CPerformanceTimer timer;

	CImageQueueHandler* queueHandler = (CImageQueueHandler*) pParam;

	queue<CQueueBuffer*>* pImageQueue = &queueHandler->m_ImageQueue;
	queue<CQueueBuffer*>* pBackImageQueue = &queueHandler->m_BackImageQueue;

	while(TRUE) {
		WaitForSingleObject(queueHandler->m_hPushed, INFINITE);
		if(!queueHandler->m_bActive)
			return 0;

		///Udupa; TODO
		//if(queueHandler->m_pApp->m_pResourceCheckDlg != NULL && !queueHandler->m_pApp->m_pResourceCheckDlg->m_bDiskOk) {
		//	OutputDebugLogTo(9, TRUE, "************* Low Disk space; Not saving image to disk *************");
		//	continue;
		//}

		EnterCriticalSection(&queueHandler->m_csQueue);
		while(!pBackImageQueue->empty()) {
			pImageQueue->push(pBackImageQueue->front());
			pBackImageQueue->pop();
		}
		LeaveCriticalSection(&queueHandler->m_csQueue);

		while(!pImageQueue->empty()) {
			timer.Reset();
			CQueueBuffer* pBuf = pImageQueue->front();
			queueHandler->SaveImage(pBuf);
			pImageQueue->pop();
			OutputDebugLogTo(6, TRUE, "[Track%d] **** Image Save Time: %5.2fms", pBuf->m_nDoc, timer.GetElapsedMSec());
			delete pBuf;
		}

//		if(!queueHandler->m_pApp->m_pResourceCheckDlg->m_bDiskOk) {
//			queueHandler->m_bActive = FALSE;
//			OutputDebugLogTo(9, TRUE, "************* [%s] Low Disk Space (%d)", strLogDir, availableBytes);
//			strDir.ReleaseBuffer();
//			return -1;
//		}
	}

	return 0;
}


UINT ImageInspectThread(LPVOID pParam)
{
	CPerformanceTimer timer;

	CImageQueueHandler* queueHandler = (CImageQueueHandler*)pParam;

	queue<CQueueBuffer*>* pImageQueue = &queueHandler->m_ImageQueue;
	queue<CQueueBuffer*>* pBackImageQueue = &queueHandler->m_BackImageQueue;

	while (TRUE) {
		WaitForSingleObject(queueHandler->m_hPushedInspect, INFINITE);
		if (!queueHandler->m_bActive)
			return 0;

		///Udupa; TODO
		//if(queueHandler->m_pApp->m_pResourceCheckDlg != NULL && !queueHandler->m_pApp->m_pResourceCheckDlg->m_bDiskOk) {
		//	OutputDebugLogTo(9, TRUE, "************* Low Disk space; Not saving image to disk *************");
		//	continue;
		//}

		EnterCriticalSection(&queueHandler->m_csQueue);
		while (!pBackImageQueue->empty()) {
			pImageQueue->push(pBackImageQueue->front());
			pBackImageQueue->pop();
		}
		LeaveCriticalSection(&queueHandler->m_csQueue);

		while (!pImageQueue->empty()) {
			timer.Reset();
			CQueueBuffer* pBuf = pImageQueue->front();
			queueHandler->InspectImage(pBuf);
			pImageQueue->pop();
			OutputDebugLogTo(6, TRUE, "[Track%d] **** Image Save Time: %5.2fms", pBuf->m_nDoc, timer.GetElapsedMSec());
			delete pBuf;
		}

		//		if(!queueHandler->m_pApp->m_pResourceCheckDlg->m_bDiskOk) {
		//			queueHandler->m_bActive = FALSE;
		//			OutputDebugLogTo(9, TRUE, "************* [%s] Low Disk Space (%d)", strLogDir, availableBytes);
		//			strDir.ReleaseBuffer();
		//			return -1;
		//		}
	}

	return 0;
}


int CImageQueueHandler::InspectImage(CQueueBuffer* pBuf)
{
	//int nTrack;
	//int nError;

	//CAppDoc *pDoc;

	//nTrack = pBuf->m_nTrack;
	//pDoc = m_pApp->m_pTracks[nTrack].m_pDoc[0];
	//for (int nDoc = 0; nDoc < pBuf->m_nTotalDocs[nTrack]; nDoc++) {
	//	m_pApp->m_pTracks[nTrack].m_SharedData.pProcBuffers[nDoc]->Allocate(pBuf->m_pImageBufferInspect[nDoc]->GetImgSize());
	//	CRect rect = CRect(CPoint(0, 0), pBuf->m_pImageBufferInspect[nDoc]->GetImgSize());
	//	ImgCopy(pBuf->m_pImageBufferInspect[nDoc], &rect, m_pApp->m_pTracks[nTrack].m_SharedData.pProcBuffers[nDoc], &rect);
	//}
	//pDoc->InspectQueueBuffers(pBuf->m_nTrack,pBuf->m_nTotalDocs[nTrack],pBuf->m_nFov,pBuf->m_nDie);

	//if(pBuf->m_nTrack==m_pApp->m_nTotalTracks-1 && pBuf->m_nFov==m_pApp->m_nNoOfFovX-1 && pBuf->m_nDie==m_pApp->m_nNoOfDies-1)
	//	m_pApp->UpdateResults();

	return 0;
}