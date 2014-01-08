//Last edited by Vadim Korolik
//on 11/27/2013
#include "Definitions.h"
#include "evom/TKOClimber.h"
#include "component/TKORelay.h"
#include "log/TKOLogger.h"
#include "drive/TKODrive.h"
#include "drive/TKOGDrive.h"
#include "component/TKOGyro.h"
#include "auton/TKOAutonomous.h"
#include "vision/TKOVision.h"

/*---------------MarkX-Thing-to-Do(TODO)---------------------*
 * Auton, vision tests
 * Cleanup imports everywhere?
 * Change auton to tasked.
 * 		Add joystick class for sticks to be implemented everywhere
 * Add logging to more stuff, auton and vision
 * Test auton, rewrite?
 * 
 * -----------------------------LAST DONE-------------------------------*
 * Singleton implementation works, no longer causes kernal exceptions
 * !!!TO SEE SINGLETON USAGE, SEE THE /docs/singletonUsage.txt FILE IN PROJECT
 * 
 * Gyro class works, but needs to be fixed electronically
 * Files organized into folders.
 * 
 * 11/27
 * 		Added printing of log file size in logger init
 * 		Rewrote regular drive with inheritance, DriveBase
 * 		Added while loop to disabled for future
 */

class MarkX: public SimpleRobot
{
	public:
		Joystick stick1, stick2, stick3, stick4; // define joysticks
		DriverStation *ds; // define driver station object
		TKOAutonomous auton;
		void Disabled();
		void Autonomous();
		void RobotInit();
		void OperatorControl();
		void Operator();
		void Test();
		void RegDrive();
		void GyroDrive();
		
		MarkX::MarkX() :
			stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
			stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
			stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
			stick4(STICK_4_PORT), // initialize joystick 4 < first EVOM joystick-m,
			
			auton(DRIVE_L1_ID, DRIVE_L2_ID, DRIVE_R1_ID, DRIVE_R2_ID)

		{
		}
};
void MarkX::Test()
{
	printf("Calling test function \n");
}

void MarkX::RobotInit()
{
	printf("Initializing MarkX class \n");
	TKOGyro::inst()->reset();
//	AxisCamera::GetInstance(); //boot up camera, maybe add check to see if it worked?
	printf("Initialized the MarkX class \n");
}

void MarkX::Disabled()
{
	printf("Robot Dying!\n");
	TKOLogger::inst()->Stop();
	TKODrive::inst()->Stop();
	TKOGDrive::inst()->Stop();
	TKOVision::inst()->StopProcessing();
	printf("Robot successfully died!\n");
	while (IsDisabled())
	{
		
	}
}

void MarkX::Autonomous(void)
{
	printf("Starting Autonomous \n");
	ds = DriverStation::GetInstance();
	TKOLogger::inst()->addMessage("--------------Autonomous started-------------");
	if (ds->IsFMSAttached())
	{
		TKOLogger::inst()->addMessage("-----------FMS DETECTED------------");
		TKOLogger::inst()->addMessage("PROBABLY A SERIOUS MATCH");
		if (ds->GetAlliance() == ds->kBlue);
			TKOLogger::inst()->addMessage("BLUE ALLIANCE!");
		if (ds->GetAlliance() == ds->kRed);
			TKOLogger::inst()->addMessage("RED ALLIANCE!");
	}
	Wait(1.);
	
	//INSERT REST OF AUTON CODE HERE
	Wait(.1);
	printf("Starting Autonomous \n");
	auton.initAutonomous();
	auton.setDrivePID(DRIVE_kP, DRIVE_kP, DRIVE_kI);
	auton.setDriveTargetStraight(ds->GetAnalogIn(1) * 10 * REVS_PER_METER);
	auton.startAutonomous();

	while (auton.autonTimer.Get() < 10 && auton.runningAuton)
	{
		if (!ds->IsAutonomous())
			auton.stopAutonomous();

//		auton.autonomousCode();
		if (ds->GetDigitalIn(1))
			auton.autonSetup1();
		else if (ds->GetDigitalIn(2))
			auton.autonSetup2();
		else if (ds->GetDigitalIn(3))
			auton.autonSetup3();
		else
		{
			printf("Error, no starting setup selected");
			DSLog(1, "Error, no starting setup selected");
			TKOLogger::inst()->addMessage("No auton mode selected.");
		}
	}
	auton.stopAutonomous();
	
	printf("Ending Autonomous \n");
}

void MarkX::OperatorControl()
{
	printf("Starting OperatorControl \n");
	ds = DriverStation::GetInstance();
	TKOGyro::inst()->reset();
	TKOLogger::inst()->Start();
	TKOVision::inst()->StartProcessing();  //NEW VISION START
	RegDrive(); //Choose here between kind of drive to start with
	Timer loopTimer;
	loopTimer.Start();
	
//	if (not climber.ranCalibration)
//		climber.calibrateWinch();
	TKOLogger::inst()->addMessage("--------------Teleoperated started-------------");
	
	while (IsOperatorControl() && IsEnabled())
	{
		loopTimer.Reset();
		DSClear();
		MarkX::Operator();
		if (loopTimer.Get() > 0.1)
		{
			//TKOLogger::inst()->addMessage("!!!CRITICAL Operator loop very long, length", loopTimer.Get());
			printf("!!!CRITICAL Operator loop very long, %f%s\n", loopTimer.Get(), " seconds.");
		}
		DSLog(1, "%f", TKOVision::inst()->lastDist);
		DSLog(3, "G_ang: %f\n", TKOGyro::inst()->GetAngle());
		DSLog(4, "Clock %f\n", GetClock());
		DSLog(5, "%f", GetTime());
		Wait(LOOPTIME - loopTimer.Get());
		loopTimer.Reset();
	}
	printf("Ending OperatorControl \n");
	TKOLogger::inst()->addMessage("Ending OperatorControl");
}

void MarkX::Operator()
{
	if (stick4.GetRawButton(6))
	{
		printf("%f", GetFPGATime());
	}
	if (stick1. GetRawButton(11))
		TKOGyro::inst()->reset();
	if (stick1.GetRawButton(8))
		RegDrive();
	if (stick1.GetRawButton(9))
		GyroDrive();
	
}

void MarkX::RegDrive()
{
	TKOGDrive::inst()->Stop();
	TKODrive::inst()->Start();
}
void MarkX::GyroDrive()
{
	TKODrive::inst()->Stop();
	TKOGDrive::inst()->Start();
}

START_ROBOT_CLASS(MarkX);
