#include "stdafx.h"
#include "Basler_cameralink.h"
#include "DebugLog.h"
#include "BitmapFunctions.h"
#include "PerformanceTimer.h"
#include "DSapi.h"
#include "BFErApi.h"

namespace Basler_Camera_Interface
{
	Basler_beA4000_62km::Basler_beA4000_62km()
	{
		_opened_framegrabber = false;

		_image_count = NUM_BUFFERS;
		_allocated_images = false;

		_run_mode = false;

		_width = 0;
		_height = 0;
	}

	Basler_beA4000_62km::~Basler_beA4000_62km()
	{
		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::Destroyed!!!!");
		//_close_framegrabber();
	}

	int Basler_beA4000_62km::open()
	{
		// open any framegrabber board
		//_close_framegrabber();

		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::open(): Query Board Type, Board Number...");
		/* Prompt user for the board to open. If only one board is installed open it */
		if (DoBrdOpenDialog(TRUE, FF_BITFLOW_MODERN, &BoardType, &BoardNum, &Init, &SerNum) == IDCANCEL)
		{
			OutputDebugLogTo(1, TRUE, "----- ERROR : Cannot DoBrdOpenDialog BitFlow (?) framegrabber");
			return -1;
		}
		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::open(): BoardType %d, BoardNum %d, Init %d, SerNum %d", BoardType, BoardNum, Init, SerNum);

		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::open(): BiBrdOpen...");
		/* Open board */
		BFRC returnCode = BiBrdOpen(BoardType, BoardNum, &_framegrabber );
		if( returnCode != BI_OK ) {
			OutputDebugLogTo( 0, TRUE, "----- ERROR %d: Cannot open BitFlow (?) framegrabber", returnCode );
			return -1;
		}
		_opened_framegrabber = true;

		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::open(): BiBufferAllocCam: Image Count %d...", _image_count);
		/* Allocate memory for buffers */
		BFU32 NumBuffer = _image_count;
		returnCode = BiBufferAllocCam( _framegrabber, &_images, NumBuffer);
		if( returnCode != BI_OK ) {
			OutputDebugLogTo( 0, TRUE, "----- ERROR %d: Cannot allocate images for BitFlow (?) framegrabber", returnCode );
			return -1;
		}
		_allocated_images = true;

		return 0;
	}

	int Basler_beA4000_62km::open2()
	{
		// open any framegrabber board
		//_close_framegrabber();

		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::open(): Query Board Type, Board Number...");
		/* Prompt user for the board to open. If only one board is installed open it */
		if (DoBrdOpenDialog(TRUE, FF_BITFLOW_MODERN, &BoardType, &BoardNum, &Init, &SerNum) == IDCANCEL)
		{
			OutputDebugLogTo(1, TRUE, "----- ERROR : Cannot DoBrdOpenDialog BitFlow (?) framegrabber");
			return -1;
		}
		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::open(): BoardType %d, BoardNum %d, Init %d, SerNum %d", BoardType, BoardNum, Init, SerNum);

		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::open(): BiBrdOpen...");
		/* Open board */
		BFRC returnCode = BiBrdOpen(BoardType, BoardNum, &_framegrabber);
		if (returnCode != BI_OK) {
			OutputDebugLogTo(0, TRUE, "----- ERROR %d: Cannot open BitFlow (?) framegrabber", returnCode);
			return -1;
		}
		_opened_framegrabber = true;

		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::open(): BiBufferAllocCam: Image Count %d...", _image_count);
		/* Allocate memory for buffers */
		BFU32 NumBuffer = _image_count;
		returnCode = BiBufferAllocCam(_framegrabber, &_images, NumBuffer);
		if (returnCode != BI_OK) {
			OutputDebugLogTo(0, TRUE, "----- ERROR %d: Cannot allocate images for BitFlow (?) framegrabber", returnCode);
			return -1;
		}
		_allocated_images = true;

		return 0;
	}

	int Basler_beA4000_62km::set_up()
	{
		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::set_up(): BiSeqAqSetup...Image Count %d", _image_count);

		// Setup for sequence capture 
		BFRC returnCode = BiSeqAqSetup(_framegrabber, &_images, BiAqEngJ | NoResetOnError);
		if (returnCode != BI_OK) {
			OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot setup seq acquisition parameters for BitFlow (?) framegrabber", returnCode);
			BiErrorShow(_framegrabber, returnCode);
			BiBufferFree(_framegrabber, &_images);
			BiBrdClose(_framegrabber);
			return -1;
		}

		/*Get Width & Height Information */
		BFU32 width, height;
		returnCode = BiBrdInquire(_framegrabber, BiCamInqXSize, &width);
		if (returnCode != BI_OK) {
			OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot inquire _width ID for BitFlow (?) framegrabber", returnCode);
			return -1;
		}
		_width = width;

		returnCode = BiBrdInquire(_framegrabber, BiCamInqYSize0, &height);
		if (returnCode != BI_OK) {
			OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot inquire _height ID for BitFlow (?) framegrabber", returnCode);
			return -1;
		}
		_height = height;

		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::set_up(): After BiBrdInquire...Width %d Height %d - Image Count %d",
			_width, _height, _image_count);

		//--------------- Using BiSeq to capture images --------------- 

		_image_count = NUM_BUFFERS;
		// Setup Sequence acquire parameters 
		returnCode = BiSeqParameters(_framegrabber, &_images, 0, _image_count, 0);
		if (returnCode != BI_OK) {
			OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot BiSeqParameters for BitFlow (?) framegrabber", returnCode);
			BiErrorShow(_framegrabber, returnCode);
			BiSeqCleanUp(_framegrabber, &_images);
			BiBufferFree(_framegrabber, &_images);
			BiBrdClose(_framegrabber);
			return -1;
		}

		// Set a timeout value for internal operations
		BFU32	TimeoutValue = 1000;					// Timeout of 1sec.
		BiInternalTimeoutSet(_framegrabber, &_images, TimeoutValue);
		return 0;
	}

	int Basler_beA4000_62km::set_up2()
	{
		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::set_up(): BiCircAqSetup...Image Count %d", _image_count);

		BFRC returnCode = BiCircAqSetup(_framegrabber, &_images, CirErStop, BiAqEngJ | NoResetOnError);
		if (returnCode != BI_OK) {
			OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot setup Circ acquisition parameters for BitFlow (?) framegrabber", returnCode);
			BiErrorShow(_framegrabber, returnCode);
			BiBufferFree(_framegrabber, &_images);
			BiBrdClose(_framegrabber);
			return -1;
		}

		/*Get Width & Height Information */
		BFU32 width, height;
		returnCode = BiBrdInquire(_framegrabber, BiCamInqXSize, &width);
		if (returnCode != BI_OK) {
			OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot inquire _width ID for BitFlow (?) framegrabber", returnCode);
			return -1;
		}
		_width = width;

		returnCode = BiBrdInquire(_framegrabber, BiCamInqYSize0, &height);
		if (returnCode != BI_OK) {
			OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot inquire _height ID for BitFlow (?) framegrabber", returnCode);
			return -1;
		}
		_height = height;

		OutputDebugLogTo(1, TRUE, "Basler_beA4000_62km::set_up(): After BiBrdInquire...Width %d Height %d - Image Count %d",
			_width, _height, _image_count);

		// Set a timeout value for internal operations
		BFU32	TimeoutValue = 1000;					// Timeout of 1sec.
		BiInternalTimeoutSet(_framegrabber, &_images, TimeoutValue);
		return 0;
	}

	int Basler_beA4000_62km::close()
	{
		if (_opened_framegrabber) {
			OutputDebugLogTo(1, TRUE, "_close_framegrabber(): BiBrdClose...");

			_opened_framegrabber = false;

			BFRC returnCode = BiBrdClose(_framegrabber);
			if (returnCode != BI_OK) {
				OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot close BitFlow (?) framegrabber", returnCode);
				BiErrorShow(_framegrabber, returnCode);
			}
		}

		return 0;
	}

	int Basler_beA4000_62km::_close_framegrabber()
	{
		stop_acquisition();

		if( _allocated_images ) {
			_allocated_images = false;

			//Method 1: Using BiCirc
			/*BFRC returnCode = BiCircCleanUp(_framegrabber, &_images);
			if( returnCode != BI_OK ) {
				OutputDebugLogTo( 0, TRUE, "----- ERROR %d: Cannot cleanup information about images for BitFlow (?) framegrabber", returnCode );
				return;
			}

			returnCode = BiBufferFree(_framegrabber, &_images);
			if( returnCode != BI_OK ) {
				OutputDebugLogTo( 0, TRUE, "----- ERROR %d: Cannot deallocate images for BitFlow (?) framegrabber", returnCode );
				return;
			}*/

			//Method 2: Using BiSeq
			OutputDebugLogTo(1, TRUE, "_close_framegrabber(): BiSeqCleanUp & BiBufferFree....");

			BFRC returnCode = BiSeqCleanUp(_framegrabber, &_images);
			if( returnCode != BI_OK ) {
				OutputDebugLogTo( 0, TRUE, "----- ERROR %d: Cannot cleanup information about images for BitFlow (?) framegrabber", returnCode );
				return -1;
			}

			returnCode = BiBufferFree(_framegrabber, &_images);
			if( returnCode != BI_OK ) {
				OutputDebugLogTo( 0, TRUE, "----- ERROR %d: Cannot deallocate images for BitFlow (?) framegrabber", returnCode );
				return -1;
			}
		}

		if( _opened_framegrabber ) {
			OutputDebugLogTo(1, TRUE, "_close_framegrabber(): BiBrdClose...");

			_opened_framegrabber = false;

			BFRC returnCode = BiBrdClose( _framegrabber );
			if( returnCode != BI_OK ) {
				OutputDebugLogTo( 0, TRUE, "----- ERROR %d: Cannot close BitFlow (?) framegrabber", returnCode );
				return -1;
			}
		}

		return 0;
	}

	void Basler_beA4000_62km::getImage(CImageBuffer & image)
	{
		if( _opened_framegrabber && _run_mode ) {

			if( image.GetWidth() != getWidth() || image.GetHeight() != getHeight() ) {
				OutputDebugLogTo( 0, TRUE, "----- WARNING: Image size {%d, %d} is different than framegrabber size {%u, %u}",
								  image.GetWidth(), image.GetHeight(), getWidth(), getHeight());

				image.Allocate( getWidth(), getHeight(), image.IsReadyForDisplay(), image.IsColor() );
			}
			
			//1. Wait Done Frame
			OutputDebugLogTo(1, TRUE, "getImage(): BiSeqWaitDoneFrame...");
			Sleep(1000);
			// Wait here until the sequence has been captured.
			/*BFRC returnCode = BiSeqWaitDoneFrame(_framegrabber, &_images, 1000);
			if (returnCode != BI_OK) {
				OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot BiSeqWaitDone for BitFlow (?) framegrabber", returnCode);
			}*/

			
			//Query Captured Information
			BFU32	FramesCaptured = 0, FramesMissed = 0;
			BiCaptureStatusGet(_framegrabber, &_images, &FramesCaptured, &FramesMissed);
			OutputDebugLogTo(1, TRUE, "getImage(): Missed %d Frames - Captured %d Frames", FramesMissed, FramesCaptured);
			
			//Copy Buffer Image From Bitflow
			OutputDebugLogTo(1, TRUE, "getImage(): Get buffer from Bitflow...");
			PBFU8		pSrc8;
			PBFU32		*pBufferArray;
			BiBufferArrayGet(_framegrabber, &_images, &pBufferArray);

			pSrc8 = (PBFU8)pBufferArray[0];
			int nWidth = image.GetWidth();
			int nHeight = image.GetHeight();

			OutputDebugLogTo(1, TRUE, "getImage(): Copy buffer from Bitflow to Buffer Image...");
			BYTE * image_bufferPtr = image.GetImgPtr();
			memcpy(image_bufferPtr, pSrc8, nHeight * nWidth);
			OutputDebugLogTo(1, TRUE, "-------------------------------------------------------");
		}
	}

	void Basler_beA4000_62km::getImage2(CImageBuffer & image)
	{
		if (_opened_framegrabber && _run_mode) {

			if (image.GetWidth() != getWidth() || image.GetHeight() != getHeight()) {
				OutputDebugLogTo(0, TRUE, "----- WARNING: Image size {%d, %d} is different than framegrabber size {%u, %u}",
					image.GetWidth(), image.GetHeight(), getWidth(), getHeight());

				image.Allocate(getWidth(), getHeight(), image.IsReadyForDisplay(), image.IsColor());
			}

			//1. Wait Done Frame
			OutputDebugLogTo(1, TRUE, "getImage(): BiCirWaitDoneFrame...");
			// Wait here until the sequence has been captured.
			BiCirHandle CirHandle;
			BFRC returnCode = BiCirWaitDoneFrame(_framegrabber, &_images, 1000, &CirHandle);
			if (returnCode != BI_OK) {
				OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot BiCirWaitDoneFrame for BitFlow (?) framegrabber", returnCode);
				BiErrorShow(_framegrabber, returnCode);
				_run_mode = false;
				return;
			}

			_run_mode = false;
			/*returnCode = BiCirControl(_framegrabber, &_images, BIPAUSE, BiWait);
			if (returnCode != BI_OK) {
				OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot BiCirControl::BIPAUSE for BitFlow (?) framegrabber", returnCode);
				_run_mode = false;
			}*/

			//Query Captured Information
			BFU32	FramesCaptured = 0, FramesMissed = 0;
			BiCaptureStatusGet(_framegrabber, &_images, &FramesCaptured, &FramesMissed);
			OutputDebugLogTo(1, TRUE, "getImage(): Missed %d Frames - Captured %d Frames", FramesMissed, FramesCaptured);

			//Copy Buffer Image From Bitflow
			OutputDebugLogTo(1, TRUE, "getImage(): Get buffer from Bitflow...");

			const BYTE * imagePtr = reinterpret_cast<BYTE *>(CirHandle.pBufData);

			int pitch = image.GetPitch();

			BYTE * image_bufferPtr = image.GetImgPtr();
			const BYTE * image_bufferEnd = image_bufferPtr + _height * pitch;

			for (; image_bufferPtr != image_bufferEnd; image_bufferPtr += pitch, imagePtr += _width)
				memcpy(image_bufferPtr, imagePtr, _width * sizeof(BYTE));

			returnCode = BiCirStatusSet(_framegrabber, &_images, CirHandle, BIAVAILABLE);
			if (returnCode != BI_OK) {
				OutputDebugLogTo(1, TRUE, "----- ERROR %d: Failed to reset buffer availability in BitFlow (?) framegrabber", returnCode);
				return;
			}


			/*PBFU8		pSrc8;
			PBFU32		*pBufferArray;
			BiBufferArrayGet(_framegrabber, &_images, &pBufferArray);

			pSrc8 = (PBFU8)pBufferArray[0];
			int nWidth = image.GetWidth();
			int nHeight = image.GetHeight();

			OutputDebugLogTo(1, TRUE, "getImage(): Copy buffer from Bitflow to Buffer Image...");
			BYTE * image_bufferPtr = image.GetImgPtr();
			memcpy(image_bufferPtr, pSrc8, nHeight * nWidth);*/
			OutputDebugLogTo(1, TRUE, "-------------------------------------------------------");
		}
	}

	void Basler_beA4000_62km::start_acquisition()
	{
		_run_mode = false;
		if (!_run_mode) {
			OutputDebugLogTo(1, TRUE, "start_acquisition(): BiSeqControl: BISTART...");
			// Start Sequence Acquisition 
			BFRC returnCode = BiSeqControl(_framegrabber, &_images, BISTART, BiWait);
			if (returnCode != BI_OK) {
				OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot BiSeqControl::BISTART for BitFlow (?) framegrabber", returnCode);
				BiErrorShow(_framegrabber, returnCode);
				return;
			}
			
			//OutputDebugLogTo(1, TRUE, "start_acquisition(): BiSeqWaitDone...");
			//// Wait here until the sequence has been captured.
			//returnCode = BiSeqWaitDone(_framegrabber, &_images, 10000);
			//if (returnCode != BI_OK) {
			//	OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot BiSeqWaitDone for BitFlow (?) framegrabber", returnCode);
			//	BiErrorShow(_framegrabber, returnCode);
			//	return;
			//}

			_run_mode = true;
		}
		else {
			OutputDebugLogTo(1, TRUE, "start_acquisition(): BiSeqControl: BIRESUME...");
			// Start Sequence Acquisition 
			BFRC returnCode = BiSeqControl(_framegrabber, &_images, BIRESUME, BiWait);
			if (returnCode != BI_OK) {
				OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot BiSeqControl::BIRESUME for BitFlow (?) framegrabber", returnCode);
				BiErrorShow(_framegrabber, returnCode);
				return;
			}
		}
		
	}

	void Basler_beA4000_62km::start_acquisition2()
	{
		if (!_run_mode) {
			OutputDebugLogTo(1, TRUE, "start_acquisition(): BiCirControl: BISTART...");
			// Start Sequence Acquisition 
			BFRC returnCode = BiCirControl(_framegrabber, &_images, BISTART, BiWait);
			if (returnCode != BI_OK) {
				OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot BiCirControl::BISTART for BitFlow (?) framegrabber", returnCode);
				BiErrorShow(_framegrabber, returnCode);
				return;
			}
			_run_mode = true;
		}
		else {
			OutputDebugLogTo(1, TRUE, "start_acquisition(): BiCirControl: BIRESUME...");
			// Start Sequence Acquisition 
			BFRC returnCode = BiCirControl(_framegrabber, &_images, BIRESUME, BiWait);
			if (returnCode != BI_OK) {
				OutputDebugLogTo(1, TRUE, "----- ERROR %d: Cannot BiCirControl::BIRESUME for BitFlow (?) framegrabber", returnCode);
				BiErrorShow(_framegrabber, returnCode);
				return;
			}
		}

	}

	void Basler_beA4000_62km::stop_acquisition()
	{
		OutputDebugLogTo(1, TRUE, "stop_acquisition()...");
		if( _run_mode ) {
			OutputDebugLogTo(1, TRUE, "stop_acquisition(): BiSeqControl::BISTOP...");

			BFRC returnCode = BiSeqControl(_framegrabber, &_images, BISTOP, BiWait);
			if (returnCode != BI_OK) {
				OutputDebugLogTo(1, TRUE, "----- ERROR : Cannot stop acquisition mode for BitFlow (?) framegrabber");
				return;
			}

			_run_mode = false;
		}
	}

	unsigned int Basler_beA4000_62km::getWidth() const
	{
		return _width;
	}

	unsigned int Basler_beA4000_62km::getHeight() const
	{
		return _height;
	}
};
