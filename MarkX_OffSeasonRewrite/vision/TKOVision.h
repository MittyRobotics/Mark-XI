//Last edited by Vadim Korolik
//on 11/07/2013
#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "Math.h"
#include "../Definitions.h"
#include "../log/TKOLogger.h"
#include "VisionFunc.h"

class TKOVision
{	
	public:
		static TKOVision* inst();
		bool ProccessImageFromCamera();
		void StartProcessing();
		void StopProcessing();
		
	private:
		TKOVision();
		~TKOVision();

		Joystick stick1, stick2, stick3, stick4; // define joysticks
		Scores *scores;
		Task *picProcessT;
		
		ColorImage *rawImage;
		BinaryImage *thresholdImage; // get just the green target pixels
		BinaryImage *convexHullImage;
		BinaryImage *filteredImage;
		MonoImage *redImage;
		
		static TKOVision* m_Instance;
		
		static void ProcessRunner();
};
