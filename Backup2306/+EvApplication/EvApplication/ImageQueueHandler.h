/// Udupa; Image Queue Handler to manage and save images in a queue
/// Udupa; Dec'2013; Defect overlay queuing option

#pragma once

#include <queue>
using namespace std;


class CQueueBuffer
{
public:
	CImageBuffer* m_pImageBuffer;
	//CImageBuffer* m_pImageBufferInspect[5];
	int m_nTrack;
	int m_nDoc;
	int m_nDie;
	int m_nFov;
	int m_nErrorCode;
	int m_nTotalDocs[5];
	CRectArray arrRects;
	CRegionArray arrRegions;

	CQueueBuffer()
	{
		m_pImageBuffer = NULL;
		m_nTrack = 0;
		m_nDoc = 0;
		m_nDie = 0;
		m_nFov = 0;
		m_nErrorCode = 0;
	}

	CQueueBuffer(CImageBuffer* pImageBuffer, 
		int nTrack, 
		int nDoc, 
		int nDie, 
		int nFov,
		int nErrorCode, 
		CRectArray& rects, 
		CRegionArray& regions)
	{
		m_pImageBuffer = new CImageBuffer();
		m_pImageBuffer->Allocate(pImageBuffer->GetImgSize());
		CRect rect = CRect(CPoint(0, 0), pImageBuffer->GetImgSize());
		ImgCopy(pImageBuffer, &rect, m_pImageBuffer, &rect);

		m_nTrack = nTrack;
		m_nDoc = nDoc;
		m_nDie = nDie;
		m_nFov = nFov;
		m_nErrorCode = nErrorCode;

		for(int i=0; i<rects.GetCount(); i++)
			arrRects.Add(rects[i]);
		for(int i=0; i<regions.GetCount(); i++)
			arrRegions.Add(regions[i]);
	}

	//CQueueBuffer(CImageBuffer* pImageBuffer,
	//	int nTrack,
	//	int nDocTotal,
	//	int nDie,
	//	int nFov)
	//{
	//	m_nTrack = nTrack;
	//	m_nTotalDocs[nTrack] = nDocTotal;
	//	m_nDie = nDie;
	//	m_nFov = nFov;

	//	for (int nDoc = 0; nDoc < nDocTotal; nDoc++) {
	//		m_pImageBufferInspect[nDoc] = new CImageBuffer();
	//		m_pImageBufferInspect[nDoc]->Allocate(pImageBuffer[nDoc].GetImgSize());
	//		CRect rect = CRect(CPoint(0, 0), pImageBuffer[nDoc].GetImgSize());
	//		ImgCopy(&pImageBuffer[nDoc], &rect, m_pImageBufferInspect[nDoc], &rect);
	//	}
	//}

	~CQueueBuffer()
	{
		if(m_pImageBuffer != NULL) {
			m_pImageBuffer->Free();
			delete m_pImageBuffer;
		}

		//for (int i = 0; i < 5; i++) {
		//	if (m_pImageBufferInspect[i] != NULL) {
		//		m_pImageBufferInspect[i]->Free();
		//		delete m_pImageBufferInspect[i];
		//	}
		//}
	}
};

class CImageQueueHandler
{
public:
	CImageQueueHandler();
	~CImageQueueHandler();
	int Initialize(CSize szImage);

	int PushImageBuffer(CImageBuffer* pImageBuffer, int nTrackIndex,int nDocIndex,int nDieIndex,int nFovIndex, int nErrorCode, CRectArray& rects, CRegionArray& regions);

	int PushInspectImageBuffer(CImageBuffer* pImageBuffer, int nTrackIndex, int nDocIndex, int nDieIndex, int nFovIndex);

	int SaveImage(CQueueBuffer* pBuf);
	int InspectImage(CQueueBuffer* pBuf);
	int FlushQueueBuffers();

	queue<CQueueBuffer*> m_BackImageQueue;
	queue<CQueueBuffer*> m_ImageQueue;

	void DrawResultText(CQueueBuffer* pBuf, SYSTEMTIME* pSysTime);
	CImageBuffer m_BufDefect;

	CRITICAL_SECTION m_csQueue;
	HANDLE m_hPushed;
	HANDLE m_hImageSaveThread;

	HANDLE m_hPushedInspect;
	HANDLE m_hImageInspectThread;


	CApplication* m_pApp;
	CString m_strImageFolders;

	BOOL m_bActive;
};

UINT ImageSaveThread(LPVOID pParam);
UINT ImageInspectThread(LPVOID pParam);

//// inspect thread
