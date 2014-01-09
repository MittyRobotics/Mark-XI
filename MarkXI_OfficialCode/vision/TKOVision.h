//Last edited by Vadim Korolik
//on 01/04/2014
#ifndef __TKOVISION_H
#define __TKOVISION_H

#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "Math.h"
#include "../Definitions.h"
#include "VisionFunc.h"

class TKOVision
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

		Joystick stick1, stick2, stick3, stick4; // define joysticks
		Scores *scores;
		Task *picProcessT;
		
		ColorImage *rawImage;
		BinaryImage *thresholdImage; // get just the green target pixels
		BinaryImage *convexHullImage;
		BinaryImage *filteredImage;
		
		static TKOVision* m_Instance;
		
		static void ProcessRunner();
};


#endif
