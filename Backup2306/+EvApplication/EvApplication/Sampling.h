#pragma once
#include "resource.h"
#include "MotionParameters.h"
//#include "Application.h"
#include "afxwin.h"


// CSampling dialog

class CSampling : public CDialogEx
{
	DECLARE_DYNAMIC(CSampling)

public:
	CSampling(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSampling();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SAMPLING };
#endif
private:
	CFont m_FontText;

	int nFovNoPerDieX;
	int nFovNoPerDieY;
	int nDieNoPerDeviceX;
	int nDieNoPerDeviceY;
	int nTotalDieNo;
	int nTotalFOVNo;

	int * pnDieState;
	int * pnFOVState;

	CBrush brushCell[3];
	CBrush brushBackground;
	CBrush brushSelectedCell;
	CRect * prcDie;
	CRect * prcFOV;

	int nDieRectSize;

	BOOL bRedrawing;

	int nAllowedDie;
	int nNotAllowedDie;
	int nSelectedDie;

	int nAllowedFOV;
	int nNotAllowedFOV;
	int nSelectedFOV;

	CRITICAL_SECTION CriticalSectionData;
	CRITICAL_SECTION CriticalSectionDraw;

	int nWindowOffset;

	CRect rcClientWnd;
	int nClientHeight;
	int nClientWidth;

	BOOL bMouseLButtonPressed;
	CPoint ptMouseInitPos;

	CToolTipCtrl m_ToolTip;

	int nGrabColumnNo;
	int nGrabRowNo;

	int nSelectedGrabID;

	CString strToolTipText;
	BOOL bActivatedToolTip;

	int nMouseSelectedDie;

	MotionParameters * pMotion;
	MotionParameters motionParameters;

	CRect rcMappingRelative;
	CRect rcDieRelative;

	CRgn rgDieMappingRgn;
	CRgn dieMappingRgn;

	CRgn rgFOVMappingRgn;
	CRgn fovMappingRgn;

	CRgn rg23DMappingAndScrollBarRegion;
	CRgn rg23DMappingScrollBarAndDieMappingRegion;

	BOOL   isSelectingDie;
	BOOL   isSelectingDieMouseMoved;
	CPoint selectingStartPos;
	CPoint selectingEndPos;
	CRect  selectingRect;
	int    dieStateAtSelectingStartPos;
	
	BOOL   isSelectingFOV;
	BOOL   isSelectingFOVMouseMoved;
	CPoint selectingStartPosFOV;
	CPoint selectingEndPosFOV;
	CRect  selectingRectFOV;
	int    dieStateAtSelectingStartPosFOV;

	int m_nTrackID;
	int m_nDocID;

public:
	int CreateTemplate(int nDieNoX, int nDieNoY, int nFOVNoX, int nFOVNoY);
	int SetDieStates(std::vector <int> vnFOVStatus);
	int ResetDieStates();
	int SetIntensityID(int nTrackID, int nDocID);
	afx_msg void OnPaint();
	//afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg LRESULT OnNcHitTest(CPoint point);

	BOOL PreTranslateMessage(MSG* pMsg);

//	void Zoom(bool bZoomIn);
	//	afx_msg void OnBnClickedOk();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetMotionParameters(MotionParameters * pMotionIn);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	
	CComboBox m_ctlrIntensitySelection;

	afx_msg void OnBnClickedButtonSaveSampling();
	afx_msg void OnCbnSelchangeComboIntensitySelction();
};
