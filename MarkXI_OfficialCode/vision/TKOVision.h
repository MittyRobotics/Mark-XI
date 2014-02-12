//Last edited by Vadim Korolik
//on 01/04/2014

// for help with vision: http://wpilib.screenstepslive.com/s/3120/m/8731/c/39930

#ifndef __TKOVISION_H
#define __TKOVISION_H

#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "Math.h"
#include "../Definitions.h"
#include "VisionFunc.h"

class TKOVision : public SensorBase
{	
	public:
		static TKOVision* inst();
		bool ProccessImageFromCamera();
		void StartProcessing();
		void StopProcessing();
		double getLastTimestamp();
		double getLastProcessingTime();
		double getLastDistance();
		TargetReport getLastTargetReport();
		vector<ParticleAnalysisReport> * getLastParticleReport();
		
		double lastTimestamp;
		double lastProcessingTime;
		double lastDist;
		TargetReport lastTarget;
		vector<ParticleAnalysisReport> *lastParticleReport;
		
	private:
		TKOVision();
		~TKOVision();

		Joystick stick4;
		Scores *scores;
		Task *picProcessT;
		
		ColorImage *rawImage;			// raw image is the original camera image
		BinaryImage *thresholdImage;	// threshold image gets just the green target pixels
		BinaryImage *convexHullImage;	// convex hull image fills in any particles
		BinaryImage *filteredImage;		// filtered image removes any particles that don't match criteria for targets
		
		static TKOVision* m_Instance;
		
		static void ProcessRunner();
		DISALLOW_COPY_AND_ASSIGN(TKOVision);
};


#endif
