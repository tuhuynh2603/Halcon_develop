#pragma once

#include <WinDef.h>
#include "ImageBuffer.h"

class CAppDoc;
class CStationHandler;

UINT ScanThread(LPVOID); 

UINT ZSensorSequence(LPVOID);

UINT GrabThread(LPVOID pParam);
UINT GrabThreadTHR(LPVOID pParam);

UINT InspectOnlineThread(LPVOID pParam);

UINT DuplicateInspectionThread(LPVOID pParam);

UINT FocusZAxis(LPVOID pParam);

UINT YMotorToDatumOffset(LPVOID pParam);

//int SecondStationGrab(long nGrabID, CStationHandler & station);

//UINT InspectTHRThread(LPVOID);

//UINT InspectTopThread(LPVOID);

UINT SaveImageThread(LPVOID);
UINT SaveDeepLeanrningImageThread(LPVOID);


UINT LatchThread(LPVOID pParam);

//UINT ClearInspectionData(LPVOID pParam);



