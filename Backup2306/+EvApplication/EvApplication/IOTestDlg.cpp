// IOTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Application.h"
#include "IOTestDlg.h"
#include "afxdialogex.h"

static const UINT_PTR IO_CHECK_TIMER = 110;

// CIOTestDlg dialog

IMPLEMENT_DYNAMIC(CIOTestDlg, CDialogEx)

CIOTestDlg::CIOTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_HARDWAREIOTEST, pParent)
	, m_strDisplayStatus(_T(""))
{
	m_nNoOfInputSegments = INPUT_STATUS_COUNT;
	m_nNoOfOutSegments = 27;
	m_pApp = (CApplication*)AfxGetApp();
	m_brushChecked.CreateSolidBrush(RGB(50, 250, 50));
	m_brushUnchecked.CreateSolidBrush(RGB(220, 50, 50));
	m_brushBorder.CreateSolidBrush(RGB(0, 0, 0));
	isCheckSensorTimerOn = FALSE;
}

CIOTestDlg::~CIOTestDlg()
{
	m_brushChecked.DeleteObject();
	m_brushUnchecked.DeleteObject();
	m_brushBorder.DeleteObject();
	for(int i = 0; i < m_nNoOfInputSegments; i++) {
		m_prgnInputStatusSegment[i].DeleteObject();
	}
	for(int i = 0; i < m_nNoOfOutSegments; i++) {
		m_prgnOutStatusSegment[i].DeleteObject();
	}
	m_pbrushStatus[0].DeleteObject();
	m_pbrushStatus[1].DeleteObject();
}

void CIOTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_STATIC_1, m_CtrlStatus1);
	DDX_Text(pDX, IDC_DISPLAY_STATUS, m_strDisplayStatus);
	
}


BEGIN_MESSAGE_MAP(CIOTestDlg, CDialogEx)
	//ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_INPUT_1, IDC_BUTTON_INPUT_27, OnButtonClicked)
	
	ON_BN_CLICKED(IDC_BUTTON_XYZ_EXT, &CIOTestDlg::OnBnClickedButtonXyzExt)
	ON_BN_CLICKED(IDC_BUTTON_XYZ_RET, &CIOTestDlg::OnBnClickedButtonXyzRet)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_19, &CIOTestDlg::OnBnClickedButtonInput19)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CIOTestDlg message handlers



BOOL CIOTestDlg::OnInitDialog()
{
	
	CDialogEx::OnInitDialog();

	m_strOutputStatus[0]= "Start Lamp ";
	m_strOutputStatus[1] = "Cycle Stop Lamp ";
	m_strOutputStatus[2] = "Immediate Stop Lamp "; 
	m_strOutputStatus[3] = "Reset Lamp "; 
	m_strOutputStatus[4] = "K1(SERVO )";
	m_strOutputStatus[5] = "Tower Light Blue ";
	m_strOutputStatus[6] = "Tower Light Red ";
	m_strOutputStatus[7] = "Tower Light Yellow ";
	m_strOutputStatus[8] = "Tower Light Green ";
	m_strOutputStatus[9] = "Buzzer ";

	m_strOutputStatus[10] = "X Biasing Cylinder EXT";
	m_strOutputStatus[11] = "X Biasing Cylinder RET";
	m_strOutputStatus[12] = "Y Biasing Cylinder EXT";
	m_strOutputStatus[13] = "Y Biasing Cylinder RET";
	m_strOutputStatus[14] = "Z Biasing Cylinder EXT";
	m_strOutputStatus[15] = "Z Biasing Cylinder RET";
	m_strOutputStatus[16] = "Scanner Cylinder EXT";
	m_strOutputStatus[17] = "Scanner Cylinder RET";
	m_strOutputStatus[18] = "Ionizer valve ";
	m_strOutputStatus[19] = "Ionizer Dis. Stop";
	m_strOutputStatus[20] = "Ionizer Reset";
	m_strOutputStatus[21] = "Gaurd Door Lock";
	m_strOutputStatus[22] = "Curtain mute 1";
	m_strOutputStatus[23] = "Curtain mute 2";
	m_strOutputStatus[24] = "K2 (MTR On)";
	m_strOutputStatus[25] = "Scanner Cylinder 02 EXT";
	m_strOutputStatus[26] = "Scanner Cylinder 02 RET";
	///m_prgnStatusSegment = NULL;

	 m_strInputStatus[0] = "Cycle Start";
	 m_strInputStatus[1] = "Cycle Stop";
	 m_strInputStatus[2] = "immediate Stop";
	 m_strInputStatus[3] = "Reset";
	 m_strInputStatus[4] = "Emergency";
	 m_strInputStatus[5] = "Door Switch" ;
	 m_strInputStatus[6] = "Door Bypass";
	 m_strInputStatus[7] = "Part Present Sensor";
	 m_strInputStatus[8] = "Light curtain";
	 m_strInputStatus[9] = "Ionizer Error";
	 m_strInputStatus[10] = "Incoming Air Pressure";
	 m_strInputStatus[11] = "Scanner Cylinder 01 Ext";
	 m_strInputStatus[12] = "Scanner Cylinder 01 Ret";
	 m_strInputStatus[13] = "Guard Door healthy";
	 m_strInputStatus[14] = "Scanner Cylinder 02 Ext";
	 m_strInputStatus[15] = "Scanner Cylinder 02 Ret";
	 m_strInputStatus[16] = "Crash Prevention Sensor";
	 m_strInputStatus[17] = "Part Present Sensor 01";

	 m_strInputStatus[18] = "Left Air Cylinder Open";
	 m_strInputStatus[19] = "Left Air Cylinder Close";
	 m_strInputStatus[20] = "Right Air Cylinder Open";
	 m_strInputStatus[21] = "Right Air Cylinder Close";
	 m_strInputStatus[22] = "Left Clamp Close";
	 m_strInputStatus[23] = "Right Clamp Close";



	int nCardNo = 0;
	int nPortNo = 1;
	UINT pDataIn = 0;  




	CRect rect;
		
	for (int i = 0; i<m_nNoOfInputSegments; i++) {
		m_bInputChecked[i] = FALSE;
		GetDlgItem(IDC_STATIC_INPUT_1 + i)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		m_prgnInputStatusSegment[i].CreateEllipticRgn(rect.left + 8, rect.top + 8, rect.right - 8, rect.bottom - 8);
	}
	for (int i = 0; i<m_nNoOfOutSegments; i++) {

		if(IDC_STATIC_OUTPUT1 +i == IDC_STATIC_OUTPUT5 || IDC_STATIC_OUTPUT1 + i == IDC_STATIC_OUTPUT24 || IDC_STATIC_OUTPUT1 + i == IDC_STATIC_OUTPUT25)
			continue;

		m_bChecked[i] = FALSE;
		m_bStart[i] = TRUE;
		GetDlgItem(IDC_STATIC_OUTPUT1 + i)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		m_prgnOutStatusSegment[i].CreateEllipticRgn(rect.left + 8, rect.top + 8, rect.right - 8, rect.bottom - 8);
	}



	

	//m_pApp->m_MotorResource.InPortReadBit(nCardNo, nPortNo, OUT_MOTOR_CONTACTOR_CONTROL, &pDataIn);
	//if (pDataIn && OUT_MOTOR_CONTACTOR_CONTROL)
	//	m_bChecked[1] = TRUE;

	//GetDlgItem(IDC_IOSTATUS_STATIC)->SetWindowText(str);




	SetTimer(0, 1000, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CIOTestDlg::OnPaint()
{
	CPaintDC dc(this); 
	CDC* pDC = &dc;
	CRect rect, rectChannelMap;
	CString str;


	for (int i = 0; i<m_nNoOfInputSegments; i++) {
		pDC->FillRgn(&m_prgnInputStatusSegment[i], m_bInputChecked[i] ? &m_brushChecked : &m_brushUnchecked);
		pDC->FrameRgn(&m_prgnInputStatusSegment[i], &m_brushBorder, 1, 1);
		SetBkMode(dc, TRANSPARENT);
		GetDlgItem(IDC_STATIC_INPUT_1 + i)->GetWindowRect(&rectChannelMap);
		rectChannelMap.top += 2;
		rectChannelMap.left += 2;
		rectChannelMap.right -= 2;
		rectChannelMap.bottom -= 2;
		ScreenToClient(&rectChannelMap);
//		str.Format("I%d",i+1);
//		pDC->DrawText(str, rectChannelMap, DT_CENTER);
	}
	
	for (int i = 0; i<m_nNoOfOutSegments; i++) {

		if(IDC_STATIC_OUTPUT1 + i == IDC_STATIC_OUTPUT5 || IDC_STATIC_OUTPUT1 + i == IDC_STATIC_OUTPUT24 || IDC_STATIC_OUTPUT1 + i == IDC_STATIC_OUTPUT25)
			continue;


		pDC->FillRgn(&m_prgnOutStatusSegment[i], m_bChecked[i] ? &m_brushChecked : &m_brushUnchecked);
		pDC->FrameRgn(&m_prgnOutStatusSegment[i], &m_brushBorder, 1, 1);
		SetBkMode(dc, TRANSPARENT);
		GetDlgItem(IDC_STATIC_OUTPUT1 + i)->GetWindowRect(&rectChannelMap);
		rectChannelMap.top += 2;
		rectChannelMap.left += 2;
		rectChannelMap.right -= 2;
		rectChannelMap.bottom -= 2;
		ScreenToClient(&rectChannelMap);
//		str.Format("O%d", i + 1);
//		pDC->DrawText(str, rectChannelMap, DT_CENTER);
	}
}


void CIOTestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	

	/*int nIndex = -1;
	CRect rect;
	for(int i=0; i<m_nNoOfSegments; i++) {
	if(m_prgnStatusSegment[i].PtInRegion(point)) {
	nIndex = i;
	break;
	}
	}

	if (nIndex != -1) {
	m_bChecked[nIndex] = !m_bChecked[nIndex];
	int nError = m_pApp->m_MotorResource.OutPortWriteBit(0,1, OUT_MOTOR_CONTACTOR_CONTROL,m_bChecked[nIndex]);
	if (!nError) {
	OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_MOTOR_CONTACTOR_CONTROL);
	}
	CRect rect;
	GetDlgItem(IDC_STATIC1 + nIndex)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	InvalidateRect(&rect);
	}
*/
	CDialogEx::OnLButtonDown(nFlags, point);
}




void CIOTestDlg::OnMouseMove(UINT nFlags, CPoint point)
{


	CString strOn, strOf;
	strOn = " On";
	strOf = " Off";
	// TODO: Add your message handler code here and/or call default
	int nIndex = -1;
	CRect rect;
	//for (int i = 0; i<m_nNoOfOutSegments; i++) {
	//	if (m_prgnOutStatusSegment[i].PtInRegion(point)) {
	//		nIndex = i;
	//		break;
	//	}
	//}

			
	if (nIndex != -1) {
		if (m_bChecked[nIndex] == 1)
			{
			m_strDisplayStatus = m_strOutputStatus[nIndex] + strOn;
			}
		else if (m_bChecked[nIndex] == 0)
		{
			/*m_strDisplayStatus = m_strOutputStatus[nIndex+ m_nNoOfOutSegments];*/
			m_strDisplayStatus = m_strOutputStatus[nIndex] + strOf;
		}
		}


	int nInputindex = -1;

	for (int i = 0; i<m_nNoOfInputSegments; i++) {
		if (m_prgnInputStatusSegment[i].PtInRegion(point)) {
			nInputindex = i;
			break;
		}
	}


	if (nInputindex != -1) {
		if (m_bInputChecked[nInputindex] == 1)
		{
			m_strDisplayStatus = m_strInputStatus[nInputindex]+strOn;
		}
		else if (m_bInputChecked[nInputindex] == 0)
		{
			m_strDisplayStatus = m_strInputStatus[nInputindex]+strOf;
		}
	}
		
		
	
	UpdateData(FALSE);
	CDialogEx::OnMouseMove(nFlags, point);
}

void CIOTestDlg::OnButtonClicked(UINT nID)
{
	int nButton = nID - IDC_BUTTON_INPUT_1;
	int rem = nButton % 2;
	int nStutsusId = nButton /*/ 2*/;


	if(m_bStart[nStutsusId]==TRUE)
	/*if (rem == 0)*/ {
		m_bChecked[nStutsusId] = 1;
		/*CRect rect;
		GetDlgItem(IDC_STATIC_OUTPUT1 + nStutsusId)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);*/
	}

	if (m_bStart[nStutsusId] == FALSE)
	/*if (rem != 0)*/ {
		m_bChecked[nStutsusId] = 0;
		/*CRect rect;
		GetDlgItem(IDC_STATIC_OUTPUT1 + nStutsusId)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);*/
	}
	BOOL bError = false;
	OutputStatus(nStutsusId, m_bChecked[nStutsusId], bError);

	if(bError) {
		m_bChecked[nStutsusId] = false;

		
	}
	else {
		m_bStart[nStutsusId] = !m_bStart[nStutsusId];
		CRect rect;
		GetDlgItem(IDC_STATIC_OUTPUT1 + nStutsusId)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
	}

}
void CIOTestDlg::InputStatus()
{
	//int nError;
	//int nCardNo = 0;
	//int nPortNo = 1;
	//UINT pDataIn = 0;
	//BOOL bBitOn;

	////Read Input Status for Module 1
	//nError=	m_pApp->m_MotorResource.InPortRead(nCardNo, MODULE_1, &pDataIn);
	//if (nError) {
	//	OutputDebugLogTo(9, TRUE, "Unable To read Bit of Module 1");
	//}

	//for (int i = 0; i < MAX_INPUT_STATUS_MODULE_1; i++) {

	//	bBitOn = pDataIn & (1 << i);
	//	if ((bBitOn && !m_bInputChecked[i]) || (!bBitOn && m_bInputChecked[i])) {
	//		//if(i == 1 || i == 2) //Reverse logic
	//		//	bBitOn = !bBitOn;

	//		m_bInputChecked[i] = bBitOn;
	//		InvalidateRgn(&m_prgnInputStatusSegment[i], FALSE);
	//	}
	//}

	////Read Input Status for Module 2
	//nError = m_pApp->m_MotorResource.InPortRead(nCardNo, MODULE_2, &pDataIn);
	//if (nError) {
	//	OutputDebugLogTo(9, TRUE, "Unable To read Bit Of Module 2");
	//}

	//int nIndex = MAX_INPUT_STATUS_MODULE_1;
	////Crash Sensor
	//bBitOn = pDataIn & (1 << IN_CRASH_PREVENSION_SENSOR);
	//if ((bBitOn && !m_bInputChecked[nIndex]) || (!bBitOn && m_bInputChecked[nIndex])) {
	//	m_bInputChecked[nIndex] = bBitOn;
	//	InvalidateRgn(&m_prgnInputStatusSegment[nIndex], FALSE);
	//}
	//nIndex++;
	//
	////Part Present Sensor 01
	//bBitOn = pDataIn & (1 << IN_PART_PRESENT_SENSOR_01);
	//if ((bBitOn && !m_bInputChecked[nIndex]) || (!bBitOn && m_bInputChecked[nIndex])) {
	//	m_bInputChecked[nIndex] = bBitOn;
	//	InvalidateRgn(&m_prgnInputStatusSegment[nIndex], FALSE);
	//}
	//nIndex++;

	////Left air cylinder open
	//bBitOn = pDataIn & (1 << IN_LEFT_AIR_CYLINDER_OPEN);
	//if ((bBitOn && !m_bInputChecked[nIndex]) || (!bBitOn && m_bInputChecked[nIndex])) {
	//	m_bInputChecked[nIndex] = bBitOn;
	//	InvalidateRgn(&m_prgnInputStatusSegment[nIndex], FALSE);
	//}
	//nIndex++;

	////Left air cylinder close
	//bBitOn = pDataIn & (1 << IN_LEFT_AIR_CYLINDER_CLOSE);
	//if ((bBitOn && !m_bInputChecked[nIndex]) || (!bBitOn && m_bInputChecked[nIndex])) {
	//	m_bInputChecked[nIndex] = bBitOn;
	//	InvalidateRgn(&m_prgnInputStatusSegment[nIndex], FALSE);
	//}
	//nIndex++;

	////Right air cylinder open
	//bBitOn = pDataIn & (1 << IN_RIGHT_AIR_CYLINDER_OPEN);
	//if ((bBitOn && !m_bInputChecked[nIndex]) || (!bBitOn && m_bInputChecked[nIndex])) {
	//	m_bInputChecked[nIndex] = bBitOn;
	//	InvalidateRgn(&m_prgnInputStatusSegment[nIndex], FALSE);
	//}
	//nIndex++;

	////Right air cylinder close
	//bBitOn = pDataIn & (1 << IN_RIGHT_AIR_CYLINDER_CLOSE);
	//if ((bBitOn && !m_bInputChecked[nIndex]) || (!bBitOn && m_bInputChecked[nIndex])) {
	//	m_bInputChecked[nIndex] = bBitOn;
	//	InvalidateRgn(&m_prgnInputStatusSegment[nIndex], FALSE);
	//}
	//nIndex++;

	////Left clamp close
	//bBitOn = pDataIn & (1 << IN_LEFT_CLAMP_CLOSE);
	//if ((bBitOn && !m_bInputChecked[nIndex]) || (!bBitOn && m_bInputChecked[nIndex])) {
	//	m_bInputChecked[nIndex] = bBitOn;
	//	InvalidateRgn(&m_prgnInputStatusSegment[nIndex], FALSE);
	//}
	//nIndex++;

	////right clamp close
	//bBitOn = pDataIn & (1 << IN_RIGHT_CLAMP_CLOSE);
	//if ((bBitOn && !m_bInputChecked[nIndex]) || (!bBitOn && m_bInputChecked[nIndex])) {
	//	m_bInputChecked[nIndex] = bBitOn;
	//	InvalidateRgn(&m_prgnInputStatusSegment[nIndex], FALSE);
	//}
	//nIndex++;
}

void CIOTestDlg::OutputStatus(int nAction, BOOL bOn, BOOL &bError)
{
	/*int nError;
	switch(nAction) {

	case START_LAMP:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_START_LAMP, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_START_LAMP);
			}
			break;

	case CYCLE_STOP_LAMP:

			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_CYCLE_STOP_LAMP, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_CYCLE_STOP_LAMP);
			}
			break;

	case IMMEDIATE_STOP_LAMP:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_IMMIDIATE_STOP_LAMP, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_IMMIDIATE_STOP_LAMP);
			}
			break;

	case RESET_LAMP:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_RESET_LAMP, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_RESET_LAMP);
			}
			break;

	case K1_SERVO:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_K1_SERVO_ON, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_K1_SERVO_ON);
			}
			break;
	case TOWER_LIGHT_BLUE:

			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_TOWER_LIGHT_BLUE, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_TOWER_LIGHT_BLUE);
			}
			break;

	case TOWER_LIGHT_RED:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_TOWER_LIGHT_RED, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_TOWER_LIGHT_RED);
			}
			break;

	case TOWER_LIGHT_YELLOW:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_TOWER_LIGHT_YELLOW, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_TOWER_LIGHT_YELLOW);
			}
			break;
	case TOWER_LIGHT_GREEN:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_TOWER_LIGHT_GREEN, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_TOWER_LIGHT_GREEN);
			}
			break;
	case BUZZER:

			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_BUZZER, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_BUZZER);
			}
			break;
	case X_BIASING_CYLINDER_EXT:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_X_BIASING_CYLINDER_EXT, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_X_BIASING_CYLINDER_EXT);
			}
			break;
	case X_BIASING_CYLINDER_RET:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_X_BIASING_CYLINDER_RET, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_X_BIASING_CYLINDER_RET);
			}
			break;
	case Y_BIASING_CYLINDER_EXT:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_Y_BIASING_CYLINDER_EXT, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_Y_BIASING_CYLINDER_EXT);
			}
			break;
	case Y_BIASING_CYLINDER_RET:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_Y_BIASING_CYLINDER_RET, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_Y_BIASING_CYLINDER_RET);
			}
			break;
	case Z_BIASING_CYLINDER_EXT:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_Z_BIASING_CYLINDER_EXT, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_Z_BIASING_CYLINDER_EXT);
			}
			break;
	case Z_BIASING_CYLINDER_RET:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, 1, OUT_Z_BIASING_CYLINDER_RET, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Bit", OUT_Z_BIASING_CYLINDER_RET);
			}
			break;
	case SCANNER_CYLINDER_EXT:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_SCANNER_CYLINDER_EXT, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_SCANNER_CYLINDER_EXT);

			}
			break;
	case SCANNER_CYLINDER_RET:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_SCANNER_CYLINDER_RET, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_SCANNER_CYLINDER_RET);

			}
			break;

	case IONIZER_VALVE_ON:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_IONIZER_VALVE_ON, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_IONIZER_VALVE_ON);

			}
			break;

	case IONIZER_DIS_STOP:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_IONIZER_DIST_STOP, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_IONIZER_DIST_STOP);

			}
			break;

	case IONIZER_RESET:

			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_IONIZER_RESET, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_IONIZER_RESET);

			}
			break;

	case GAURD_DOOR_LOCK:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_GAURD_DOOR_LOOK, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_GAURD_DOOR_LOOK);

			}
			break;
	case CURTAIN_MUTE1:
	case CURTAIN_MUTE2:
			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_CURTAIN_MUTE1, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_CURTAIN_MUTE1);

			}

			Sleep(700);

			nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_CURTAIN_MUTE2, bOn);
			if(nError) {
				bError = TRUE;
				OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_CURTAIN_MUTE2);

			}
			break;
	case SCANNER_CYLINDER_02_EXT:
		nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_SCANNER_CYLINDER_02_EXT, bOn);
		if(nError) {
			bError = TRUE;
			OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_SCANNER_CYLINDER_EXT);

		}
		break;
	case SCANNER_CYLINDER_02_RET:
		nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_SCANNER_CYLINDER_02_RET, bOn);
		if(nError) {
			bError = TRUE;
			OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_SCANNER_CYLINDER_RET);

		}
		break;

	}*/

}

void CIOTestDlg::ReadOutputStatus()
{


}


HBRUSH CIOTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	m_pBrush = &m_pbrushStatus[2];
	if (pWnd->GetDlgCtrlID() == IDC_BUTTON_INPUT_1) {
		m_pBrush = &m_pbrushStatus[0];
		pDC->SetBkMode(TRANSPARENT);
		
		pDC->SetTextColor(RGB(0, 0, 255));
	
	}
	hbr = *m_pBrush;

	return hbr;
}


void CIOTestDlg::OnBnClickedButtonXyzExt()
{
	//m_pApp->ioControl.biasingSequence();
}


void CIOTestDlg::OnBnClickedButtonXyzRet()
{
	//m_pApp->ioControl.unBiasingSequence();
}


void CIOTestDlg::OnBnClickedButtonInput19()
{
	// TODO: Add your control notification handler code here
}


void CIOTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == IO_CHECK_TIMER) {
		if(isCheckSensorTimerOn) {
			InputStatus();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CIOTestDlg::startCheckSensorTimer()
{
	if(!isCheckSensorTimerOn) {
		isCheckSensorTimerOn = true;
		SetTimer(IO_CHECK_TIMER, 500, NULL);
	}
}

void CIOTestDlg::stopCheckSensorTimer()
{
	if(isCheckSensorTimerOn) {
		KillTimer(IO_CHECK_TIMER);
		isCheckSensorTimerOn = false;
	}
}

void CIOTestDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if(bShow)
		startCheckSensorTimer();
	else
		stopCheckSensorTimer();
}

void CIOTestDlg::SafeDoorLock()
{

	/*int nError;
	int nCardNo = 0;
	int nPortNo = 1;
	UINT pDataIn = 0;
	BOOL bBitOn;
	nError = m_pApp->m_MotorResource.InPortRead(nCardNo, nPortNo, &pDataIn);
	if (nError) {
		OutputDebugLogTo(9, TRUE, "Unable To read Bit");
	}

	bBitOn = pDataIn & (1 << 13);
	while (bBitOn)
	{
		MessageBox( "Gaurd Door Not Healthy. Please Close the Door","Gaurd Door", MB_OK | MB_ICONWARNING);
		nError = m_pApp->m_MotorResource.InPortRead(nCardNo, nPortNo, &pDataIn);
		if (nError) {
			OutputDebugLogTo(9, TRUE, "Unable To read Bit");
		}

		bBitOn = pDataIn & (1 << 13);
		Sleep(1500);
	}


	if (!bBitOn) {
		nError = m_pApp->m_MotorResource.OutPortWriteBit(0, MODULE_2, OUT_GAURD_DOOR_LOOK, 1);
		if (nError) {

			OutputDebugLogTo(9, TRUE, "Unable To Write %d Module %d Bit", MODULE_2, OUT_GAURD_DOOR_LOOK);

		}
		m_bChecked[GAURD_DOOR_LOCK] = 1;
		m_bStart[GAURD_DOOR_LOCK] = !m_bStart[GAURD_DOOR_LOCK];
		Invalidate();
	}*/

}