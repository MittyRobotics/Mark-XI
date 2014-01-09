//Last edited by Vadim Korolik
//on 01/06/2014
#include "Definitions.h"
#include "vision/TKOVision.h"

/*---------------MarkXI-Thing-to-Do(TODO)---------------------* 
 * Auton, vision tests
 * Add logging to more stuff, auton and vision
 * 
 * For shooter, create calibration curve, best fit equation of shoot power VS. ball flight dist
 * Use equation to calculate needed power for desired distance 
 * -----------------------------LAST DONE-------------------------------*
 * 01/04 
 * 		Preparing as a core for build season
 * 		Updated vision to 2014 vision sample
 * 		improving logging, adding in more places
 * 		GetFPGATime() return long, accurate sys time
 * 		
 */

class MarkXI: public SimpleRobot
{
	public:
		Joystick stick1, stick2, stick3, stick4; // define joysticks
		DriverStation *ds; // define driver station object
		void Disabled();
		void Autonomous();
		void RobotInit();
		void OperatorControl();
		void Operator();
		void Test();
		void RegDrive();
		void GyroDrive();
		
		MarkXI::MarkXI() :
			stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
			stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
			stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
			stick4(STICK_4_PORT) // initialize joystick 4 < first EVOM joystick-m,
		{
		}
};
void MarkXI::Test()
{
	printf("Calling test function \n");
}

void MarkXI::RobotInit()
{
	printf("Initializing MarkXI class \n");
//	AxisCamera::GetInstance(); //boot up camera, maybe add check to see if it worked?
	printf("Initialized the MarkXI class \n");
}

void MarkXI::Disabled()
{
	printf("Robot Dying!\n");
	TKOVision::inst()->StopProcessing();
	printf("Robot successfully died!\n");
	while (IsDisabled())
	{
		
	}
}

void MarkXI::Autonomous(void)
{
	printf("Starting Autonomous \n");
	printf("Ending Autonomous \n");
}

void MarkXI::OperatorControl()
{
	printf("Starting OperatorControl \n");
	ds = DriverStation::GetInstance();
	remove("/pics/rawImage.bmp");
	remove("/pics/filteredImage.bmp");
	printf("Removed files\n");
	TKOVision::inst()->StartProcessing();
	
	while (IsOperatorControl() && IsEnabled())
	{
		DSClear();
		
		DSLog(1, "Dist: %f", TKOVision::inst()->lastDist);
		DSLog(2, "Hot: %d", TKOVision::inst()->lastTargets.Hot);
		DSLog(3, "Proc %f", TKOVision::inst()->lastProcessingTime);
		DSLog(4, "Clock %f", GetClock());
		DSLog(5, "LastT %f", TKOVision::inst()->lastTimestamp);
		Wait(LOOPTIME);
	}
	printf("Ending OperatorControl \n");
}

void MarkXI::Operator()
{
	if (stick1.GetRawButton(8))
		RegDrive();
	if (stick1.GetRawButton(9))
		GyroDrive();
	if (stick3.GetTrigger())
	{
		if ((GetFPGATime() - TKOVision::inst()->lastTimestamp) <= 1000)
		{
			//TKOShooter::inst()->shootDist(TKOVision::inst()->lastDist);
		}
	}
}

void MarkXI::RegDrive()
{
	
}
void MarkXI::GyroDrive()
{
	
}

START_ROBOT_CLASS(MarkXI);
