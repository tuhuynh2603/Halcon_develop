#pragma once

#include "BiApi.h"
#include "ImageBuffer.h"

#define NUM_BUFFERS 25

namespace Basler_Camera_Interface
{
	// This camera connect through cameralink interface so image grabbing is made by framegrabber (BitFlow)
	// while camera settings must be made by Pylon
	class Basler_beA4000_62km
	{
	public:
		Basler_beA4000_62km();
		~Basler_beA4000_62km();

		/*Query, Open Board and Allocate memory for buffers Of Framgrabber*/
		int open();
		int open2();

		/*Close Board & Clean Up*/
		int close();

		/*Setup Sequence acquire parameters*/
		int set_up();
		int set_up2();
		
		

		void start_acquisition();
		void start_acquisition2();
		
		void stop_acquisition();

		void getImage(CImageBuffer & image);
		void getImage2(CImageBuffer & image);

		unsigned int getWidth() const;
		unsigned int getHeight() const;

	private:
		Bd _framegrabber;
		bool _opened_framegrabber;

		BIBA _images;
		
		unsigned int _image_count;
		bool _allocated_images;

		bool _run_mode;

		int _close_framegrabber();

		unsigned int _width;
		unsigned int _height;

		/* Board Information */
		BFU32			Init, SerNum;
		BFU32	BoardType, BoardNum, NumBuffers;
	};

};
