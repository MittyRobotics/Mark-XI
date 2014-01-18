//Last edited by Vadim Korolik
//on 01/08/2014
//test
#include "TKOVision.h"

//TODO
//Add logging
//Debug, test
//Add writing to cam settings resolution, fps, comp, wb, brightness etc


//Throw custom exceptions?


TKOVision* TKOVision::m_Instance = NULL;

TKOVision::TKOVision():
		stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
		stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
		stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
		stick4(STICK_4_PORT) // initialize joystick 4 < first EVOM joystick-m,
{
	picProcessT = new Task("TKOVisProc", (FUNCPTR) ProcessRunner);
	//picProcessT->SetPriority(200); //lowest priority, lower than driving etc.
	lastDist = 0.;
	lastProcessingTime = 0.;
	lastTimestamp = 0.;
	AxisCamera::GetInstance().WriteBrightness(30); //add setting writing
	AxisCamera::GetInstance().WriteMaxFPS(30);
	AxisCamera::GetInstance().WriteCompression(30);
	
	AddToSingletonList();
	
//	rawImage = new RGBImage();
//	thresholdImage = new BinaryImage(); // get just the green target pixels
//	convexHullImage = new BinaryImage();
//	filteredImage = new BinaryImage();
}
TKOVision::~TKOVision()
{
	m_Instance = NULL;
}
TKOVision* TKOVision::inst()
{
	if (!m_Instance)
	{
		printf("TKOVision instance is null");
		m_Instance = new TKOVision;
	}
	return m_Instance;
}
bool TKOVision::ProccessImageFromCamera()
{
	double timeStart = GetClock();
	/////////////////////////////
	/*if (not stick3.GetTrigger()) //if we want to run whenever press trigger
		return false;*/
	///////////^^^^^////////////// remove
	if (not picProcessT->IsReady())
	{
		printf("Pic task not ready...\n");
		return false;
	}
	rawImage = new RGBImage();
	thresholdImage = new BinaryImage(); // get just the green target pixels
	convexHullImage = new BinaryImage();
	filteredImage = new BinaryImage();

	printf("Starting proccessing actually\n");
	
	Threshold redThreshRGB(60,140,70,140,70,140); //USED IN RGB Threshold
	Threshold redThreshHSV(220,255,60,255,100,255);
	Threshold redThreshHSV2(220,255,20,255,140,255);
	//Threshold redThreshHSV(84,255,64,255,93,255);
	///this is a red threshold ^
	///this is a green threshold
	Threshold greenThresh(105, 137, 230, 255, 133, 183);	//HSV threshold criteria, ranges are in that order ie. Hue is 60-100
	ParticleFilterCriteria2 criteria[] = 
	{
			{IMAQ_MT_AREA, AREA_MINIMUM, 65535, false, false}
	};//Particle filter criteria, used to filter out small particles

	if (rawImage == NULL)
	{
		printf("Raw Image null\n");
		return false;
	}
	printf("Raw Image not null\n");
	if (AxisCamera::GetInstance().GetImage(rawImage) == false) //TODO Check that camera is set to proper res in browser
		return false;
	printf("Get Image returned true.\n");
	if (rawImage->GetHeight() == 0 or rawImage->GetWidth() == 0 or rawImage->StatusIsFatal())
	{
		printf("Camera received invalid image.\n");
		return false;
	}
	printf("Camera recived a valid image.\n");
		
	
	//validated image, up to here only raw image processing
	
	thresholdImage = rawImage->ThresholdHSV(redThreshHSV2);
	//	thresholdImage->Write("/pics/processed/thresholdImage.bmp");
	printf("Prosseced HSV Threshold\n");
	convexHullImage = thresholdImage->ConvexHull(true); //check difference between true and false
	//	convexHullImage->Write("/pics/processed/hullImage.bmp");
	printf("Prosseced Convex Hull\n");
	filteredImage = convexHullImage->ParticleFilter(criteria, 1);	//Remove small particles	
	//filteredImage->RemoveSmallObjects(true, 20);
	printf("Convex Hull success!\n");
	if (stick3.GetTrigger())
	{
		Wait(1.);
		filteredImage->Write("/pics/filteredImage.bmp");
		rawImage->Write("/pics/rawImage.bmp"); //stack some num pics back?
		printf("Wrote files.\n");
		Wait(5.);
	}
	
	printf("Prosseced Particle Filter\n");
	printf("Done Prossecing\n");
	
	Scores *scores;
	int verticalTargets[MAX_PARTICLES];
	int horizontalTargets[MAX_PARTICLES];
	int verticalTargetCount, horizontalTargetCount;

	vector<ParticleAnalysisReport> *reports = filteredImage->GetOrderedParticleAnalysisReports();  //get a particle analysis report for each particle

	verticalTargetCount = horizontalTargetCount = 0;
	//Iterate through each particle, scoring it and determining whether it is a target or not
	if(reports->size() > 0)
	{
		scores = new Scores[reports->size()];
		for (unsigned int i = 0; i < MAX_PARTICLES && i < reports->size(); i++) {
			ParticleAnalysisReport *report = &(reports->at(i));
			
			//Score each particle on rectangularity and aspect ratio
			scores[i].rectangularity = VisionFunc::inst()->scoreRectangularity(report);
			scores[i].aspectRatioVertical = VisionFunc::inst()->scoreAspectRatio(filteredImage, report, true);
			scores[i].aspectRatioHorizontal = VisionFunc::inst()->scoreAspectRatio(filteredImage, report, false);			
			
			//Check if the particle is a horizontal target, if not, check if it's a vertical target
			if(VisionFunc::inst()->scoreCompare(scores[i], false))
			{
				printf("particle: %d  is a Horizontal Target centerX: %d  centerY: %d \n", i, report->center_mass_x, report->center_mass_y);
				horizontalTargets[horizontalTargetCount++] = i; //Add particle to target array and increment count
			} else if (VisionFunc::inst()->scoreCompare(scores[i], true)) {
				printf("particle: %d  is a Vertical Target centerX: %d  centerY: %d \n", i, report->center_mass_x, report->center_mass_y);
				verticalTargets[verticalTargetCount++] = i;  //Add particle to target array and increment count
			} else {
				printf("particle: %d  is not a Target centerX: %d  centerY: %d \n", i, report->center_mass_x, report->center_mass_y);
			}
			printf("Scores rect: %f  ARvert: %f \n", scores[i].rectangularity, scores[i].aspectRatioVertical);
			printf("ARhoriz: %f \n", scores[i].aspectRatioHorizontal);	
			printf("Area: %f \n", report->particleArea);	
		}

		//Zero out scores and set verticalIndex to first target in case there are no horizontal targets
		lastTarget.totalScore = lastTarget.leftScore = lastTarget.rightScore = lastTarget.tapeWidthScore = lastTarget.verticalScore = 0;
		lastTarget.verticalIndex = verticalTargets[0];
		for (int i = 0; i < verticalTargetCount; i++)
		{
			ParticleAnalysisReport *verticalReport = &(reports->at(verticalTargets[i]));
			for (int j = 0; j < horizontalTargetCount; j++)
			{
				ParticleAnalysisReport *horizontalReport = &(reports->at(horizontalTargets[j]));
				double horizWidth, horizHeight, vertWidth, leftScore, rightScore, tapeWidthScore, verticalScore, total;

				//Measure equivalent rectangle sides for use in score calculation
				imaqMeasureParticle(filteredImage->GetImaqImage(), horizontalReport->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &horizWidth);
				imaqMeasureParticle(filteredImage->GetImaqImage(), verticalReport->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &vertWidth);
				imaqMeasureParticle(filteredImage->GetImaqImage(), horizontalReport->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &horizHeight);
				
				//Determine if the horizontal target is in the expected location to the left of the vertical target
				leftScore = VisionFunc::inst()->ratioToScore(1.2*(verticalReport->boundingRect.left - horizontalReport->center_mass_x)/horizWidth);
				//Determine if the horizontal target is in the expected location to the right of the  vertical target
				rightScore = VisionFunc::inst()->ratioToScore(1.2*(horizontalReport->center_mass_x - verticalReport->boundingRect.left - verticalReport->boundingRect.width)/horizWidth);
				//Determine if the width of the tape on the two targets appears to be the same
				tapeWidthScore = VisionFunc::inst()->ratioToScore(vertWidth/horizHeight);
				//Determine if the vertical location of the horizontal target appears to be correct
				verticalScore = VisionFunc::inst()->ratioToScore(1-(verticalReport->boundingRect.top - horizontalReport->center_mass_y)/(4*horizHeight));
				total = leftScore > rightScore ? leftScore:rightScore;
				total += tapeWidthScore + verticalScore;
				
				//If the target is the best detected so far store the information about it
				if(total > lastTarget.totalScore)
				{
					lastTarget.horizontalIndex = horizontalTargets[j];
					lastTarget.verticalIndex = verticalTargets[i];
					lastTarget.totalScore = total;
					lastTarget.leftScore = leftScore;
					lastTarget.rightScore = rightScore;
					lastTarget.tapeWidthScore = tapeWidthScore;
					lastTarget.verticalScore = verticalScore;
				}
			}
			//Determine if the best target is a Hot target
			lastTarget.Hot = VisionFunc::inst()->hotOrNot(lastTarget);
		}
		
		if(verticalTargetCount > 0)
		{
			//Information about the target is contained in the "target" structure
			//To get measurement information such as sizes or locations use the
			//horizontal or vertical index to get the particle report as shown below
			ParticleAnalysisReport *distanceReport = &(reports->at(lastTarget.verticalIndex));
			double distance = VisionFunc::inst()->computeDistance(filteredImage, distanceReport);
			lastDist = distance;
			if(lastTarget.Hot)
			{
				printf("Hot target located \n");
				printf("Distance: %f\n", distance);
			} else {
				printf("No hot target present \n");
				printf("Distance: %f\n", distance);
			}
		}
		lastParticleReport = reports;

		printf("Targets were in image\n");
		printf("Targets were found in image\n");
		printf("Distance from target %f\n", lastDist);
		lastTimestamp = GetClock();
		lastProcessingTime = lastTimestamp - timeStart;
	}
	else
	{
		printf("Processed image from camera\n");
		printf("No targets found.\n");
	}
	
	// be sure to delete images after using them
	delete filteredImage;
	delete thresholdImage;
	delete rawImage;
	delete convexHullImage;
	
	//delete allocated reports and Scores objects also
	delete scores;
	delete reports;
	

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
double TKOVision::getLastTimestamp()
{
	return lastTimestamp;
}
double TKOVision::getLastProcessingTime()
{
	return lastProcessingTime;
}
double TKOVision::getLastDistance()
{
	return lastDist;
}
TargetReport TKOVision::getLastTargetReport()
{
	return lastTarget;
}
vector<ParticleAnalysisReport> * TKOVision::getLastParticleReport()
{
	return lastParticleReport;
}
void TKOVision::ProcessRunner()
{
	Timer processingTime;
	while (true) //add task isReady check somewhere
	{
		if (not AxisCamera::GetInstance().IsFreshImage())
		{
			printf("Stale image...");
			continue;
		}
		else
		{
			printf("Running vision processing in the thread. \n");
			processingTime.Reset();
			processingTime.Start();
			if (m_Instance->ProccessImageFromCamera())
			{
				printf("PROCESSING SUCCESS \n");
				printf("Vision processing took %f\n", processingTime.Get());
			}
			processingTime.Stop();
		}
		//Wait(5. - processingTime.Get()); //wait for 5 secs total
	}
}
