//Last edited by Vadim Korolik
//on 11/07/2013
#include "TKOVision.h"

//TODO
//Add logging
//Debug, test
//Add writing to cam settings resolution, fps, comp, wb, brightness etc


//FIX DATA ACCESS EXCEPTION


TKOVision* TKOVision::m_Instance = NULL;

TKOVision::TKOVision():
		stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
		stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
		stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
		stick4(STICK_4_PORT) // initialize joystick 4 < first EVOM joystick-m,
{
	picProcessT = new Task("TKOVisionProcessing", (FUNCPTR) ProcessRunner);
	picProcessT->SetPriority(255); //lowest priority, lower than driving etc.
	AxisCamera::GetInstance().WriteBrightness(20); //add setting writing
//	rawImage = new RGBImage();
//	thresholdImage = new BinaryImage(); // get just the green target pixels
//	convexHullImage = new BinaryImage();
//	filteredImage = new BinaryImage();
//	redImage = new MonoImage();
}
TKOVision::~TKOVision()
{
	
}
TKOVision* TKOVision::inst()
{
	if (!m_Instance)
	{
		printf("TKOVision instance is null\n");
		m_Instance = new TKOVision;
	}
	return m_Instance;
}
bool TKOVision::ProccessImageFromCamera()
{
	if (not stick3.GetTrigger()) //if we want to run whenever press trigger
		return false;
	rawImage = new RGBImage();
	thresholdImage = new BinaryImage(); // get just the green target pixels
	convexHullImage = new BinaryImage();
	filteredImage = new BinaryImage();
	redImage = new MonoImage();
	printf("Starting proccessing actually\n");
	
	Threshold threshold(60,140,70,140,70,140); //USED IN RGB Threshold
	ParticleFilterCriteria2 criteria[] = {{IMAQ_MT_AREA, AREA_MINIMUM, 65535, false, false}};			

	if (rawImage == NULL)
	{
		printf("Raw Image null\n");
		return false;
	}
	printf("Raw Image not null\n");
	if (AxisCamera::GetInstance().GetImage(rawImage) == false)
		return false;
	printf("Get Image returned true.\n");
	if (rawImage->GetHeight() == 0 or rawImage->GetWidth() == 0 or rawImage->StatusIsFatal())
	{
		printf("Camera received invalid image.\n");
		return false;
	}
	printf("Camera recived a valid image.\n");
		
//	rawImage->Write("/pics/rawImage.bmp"); //stack some num pics back?
	
//	printf("Wrote raw image\n");
	
//	redImage = rawImage->GetRedPlane();
//	redImage->Write("/pics/processed/redImage.bmp");
	
	thresholdImage = rawImage->ThresholdRGB(threshold);
//	thresholdImage->Write("/pics/processed/thresholdImage.bmp");
	printf("Prosseced RGB Threshold\n");
	convexHullImage = thresholdImage->ConvexHull(false); //check difference between true and false
//	convexHullImage->Write("/pics/processed/hullImage.bmp");
	printf("Prosseced Convex Hull\n");
	filteredImage = convexHullImage->ParticleFilter(criteria, 1);	//Remove small particles	
//	filteredImage->Write("/pics/processed/filteredImage.bmp");
	printf("Prosseced Particle Filter\n");
	printf("Done Prossecing\n");
	{
		vector<ParticleAnalysisReport> *reports = filteredImage->GetOrderedParticleAnalysisReports();  //get a particle analysis report for each particle
		scores = new Scores[reports->size()];
		
		//Iterate through each particle, scoring it and determining whether it is a target or not
		for (unsigned i = 0; i < reports->size(); i++) {
			ParticleAnalysisReport *report = &(reports->at(i));
			
			scores[i].rectangularity = VisionFunc::inst()->scoreRectangularity(report);
			scores[i].aspectRatioOuter = VisionFunc::inst()->scoreAspectRatio(filteredImage, report, true);
			scores[i].aspectRatioInner = VisionFunc::inst()->scoreAspectRatio(filteredImage, report, false);			
			scores[i].xEdge = VisionFunc::inst()->scoreXEdge(thresholdImage, report);
			scores[i].yEdge = VisionFunc::inst()->scoreYEdge(thresholdImage, report);
			
			if(VisionFunc::inst()->scoreCompare(scores[i], false))
			{
				printf("particle: %d  is a High Goal  centerX: %f  centerY: %f \n", i, report->center_mass_x_normalized, report->center_mass_y_normalized);
				printf("Distance: %f \n", VisionFunc::inst()->computeDistance(thresholdImage, report, false));
			} else if (VisionFunc::inst()->scoreCompare(scores[i], true)) {
				printf("particle: %d  is a Middle Goal  centerX: %f  centerY: %f \n", i, report->center_mass_x_normalized, report->center_mass_y_normalized);
				printf("Distance: %f \n", VisionFunc::inst()->computeDistance(thresholdImage, report, true));
			} else {
				printf("particle: %d  is not a goal  centerX: %f  centerY: %f \n", i, report->center_mass_x_normalized, report->center_mass_y_normalized);
			}
			printf("rect: %f  ARinner: %f \n", scores[i].rectangularity, scores[i].aspectRatioInner);
			printf("ARouter: %f  xEdge: %f  yEdge: %f  \n", scores[i].aspectRatioOuter, scores[i].xEdge, scores[i].yEdge);	
		}
		printf("\n");
	
	}
	delete scores;	
	
//	delete rawImage;
//	delete thresholdImage;
//	delete convexHullImage;
//	delete filteredImage;
//	delete redImage;
				
	printf("Processed image from camera\n");
	return true;
}
void TKOVision::StartProcessing()
{
	if (not picProcessT->Verify() or picProcessT->IsSuspended())
		picProcessT->Start();
}
void TKOVision::StopProcessing()
{
	if (picProcessT->Verify() or not picProcessT->IsSuspended())
		picProcessT->Stop();
}
void TKOVision::ProcessRunner()
{
	Timer processingTime;
	while (true) //add task isReady check somewhere
	{
		if (not AxisCamera::GetInstance().IsFreshImage())
		{
			printf("Stale image...\n");
			continue;
		}
		else
		{
			printf("Running vision processing in the thread. \n");
			processingTime.Reset();
			processingTime.Start();
			if (m_Instance->ProccessImageFromCamera())
			{
				printf("PROCESSING SUCCESS\n ");
				TKOLogger::inst()->addCMessage("Vision processing took ", processingTime.Get());
				printf("Vision processing took: %f\n", processingTime.Get());
			}
			processingTime.Stop();
		}
		
		Wait(5.);
	}
}
