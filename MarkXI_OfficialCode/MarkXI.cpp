//Last edited by Vadim Korolik
//on 01/06/2014
#include "Definitions.h"
#include "component/TKORelay.h"
#include "log/TKOLogger.h"
#include "drive/TKODrive.h"
//#include "drive/TKOGDrive.h"
#include "component/TKOGyro.h"
#include "component/TKORelay.h"
#include "vision/TKOVision.h"
#include "evom/TKOShooter.h"
#include "evom/StateMachine.h"
#include "auton/Atom.h"
#include "auton/DriveAtom.h"
#include "auton/Molecule.h"
#include "auton/TurnAtom.h"

/*---------------MarkXI-Thing-to-Do(TODO)---------------------* 
 * Auton, vision tests
 * Add logging to more stuff, auton and vision
 * 
 * For shooter, create calibration curve, best fit equation of shoot power VS. ball flight dist
 * Use equation to calculate needed power for desired distance 
 * 
 * Possibly add different log files for different tasks?
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
		Encoder enc;
		Compressor compressor;
		CANJaguar canTest;
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
			stick4(STICK_4_PORT),
			enc(1, 2, false, Encoder::k4X), //TODO Figure out what encoder we use...
			compressor(6, 1),
			canTest(7, CANJaguar::kPercentVbus)
		{
			printf("Robot boot\n");
			canTest.SetSafetyEnabled(false);
			canTest.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
			canTest.SetVoltageRampRate(0.0);
			canTest.ConfigFaultTime(0.1);
			canTest.SetSpeedReference(CANJaguar::kSpeedRef_Encoder); 
			canTest.SetSpeedReference(CANJaguar::kSpeedRef_Encoder);
			canTest.ConfigEncoderCodesPerRev(250);
			canTest.EnableControl();
			TKOLogger::inst()->addMessage("----------ROBOT BOOT-----------");
		}
};
void MarkXI::Test()
{
	if (!IsEnabled())
		return;
	printf("Calling test function \n");
	TKOLogger::inst()->addMessage("STARTING TEST MODE");
	if (DriverStation::GetInstance()->GetDigitalIn(2))
		compressor.Start();
	else
		compressor.Stop();
	enc.Start();
	if (DriverStation::GetInstance()->GetDigitalIn(1))
	{
		printf("----------------------\n");
		printf("Deleting log...\n");
		remove("logT.txt");
		printf("Digital input 1 true\n");
	}
	
	printf("Starting tasks \n");
	TKOLogger::inst()->Start();
	TKOShooter::inst()->Start();
	printf("Started shooter, logger \n");
	TKOLogger::inst()->addMessage("STARTED SHOOTER, LOGGER IN TEST");
	while (IsEnabled())
	{
		DSLog(2, "Enc Val: %f", enc.Get());
		DSLog(3, "Enc Raw: %f", enc.GetRaw());
		DSLog(4, "Test: %f", canTest.GetSpeed())
		printf("Test: %f\n", canTest.GetSpeed());
	}
	printf("Stopping shooter, logger \n");
	TKOShooter::inst()->Stop();
	compressor.Stop();
	enc.Stop();
	TKOLogger::inst()->Stop();
	printf("Stopped testing \n");
	TKOLogger::inst()->addMessage("ENDED TEST MODE");
}

void MarkXI::RobotInit()
{
	printf("Initializing MarkXI class \n");
	TKOGyro::inst()->reset();
//	AxisCamera::GetInstance(); //boot up camera, maybe add check to see if it worked?
	printf("Initialized the MarkXI class \n");
}

void MarkXI::Disabled()
{
	printf("Robot Dying!\n");
	TKOLogger::inst()->addMessage("Robot disabled.");
	TKOShooter::inst()->Stop();
	TKODrive::inst()->Stop();
	//TKOGDrive::inst()->Stop();
	//TKOVision::inst()->StopProcessing();
	TKOLogger::inst()->Stop();
	printf("Robot successfully died!\n");
	while (IsDisabled())
	{
		
	}
}

void MarkXI::Autonomous(void)
{
	printf("Starting Autonomous \n");
	//TKOVision::inst()->StartProcessing();
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
	Wait(.1);
	
//	TKOAutonomous::inst()->initAutonomous();
//	TKOAutonomous::inst()->setDrivePID(DRIVE_kP, DRIVE_kP, DRIVE_kI);
//	TKOAutonomous::inst()->setDriveTargetStraight(ds->GetAnalogIn(1) * 10 * REVS_PER_METER);
//	TKOAutonomous::inst()->startAutonomous();
	
	//TKOVision::inst()->StopProcessing();
	printf("Ending Autonomous \n");
}

void MarkXI::OperatorControl()
{	
	printf("Starting OperatorControl \n");
	ds = DriverStation::GetInstance();
	TKOLogger::inst()->Start();
	TKOGyro::inst()->reset();
	//TKOShooter::inst()->Start();
	//TKOVision::inst()->StartProcessing();  //NEW VISION START
	RegDrive(); //Choose here between kind of drive to start with
	
	TKOLogger::inst()->addMessage("--------------Teleoperated started-------------");
	
	while (IsOperatorControl() && IsEnabled())
	{
		MarkXI::Operator();
		
		/*DSLog(1, "Dist: %f\n", TKOVision::inst()->getLastDistance());
		DSLog(2, "Hot: %i\n", TKOVision::inst()->getLastTargetReport().Hot);*/
		//DSLog(3, "G_ang: %f\n", TKOGyro::inst()->GetAngle());
		DSLog(4, "Clock %f\n", GetClock());
		Wait(LOOPTIME);
	}
	printf("Ending OperatorControl \n");
	TKODrive::inst()->Stop();
	TKOLogger::inst()->addMessage("Ending OperatorControl");
}

void MarkXI::Operator()
{
	if (stick1. GetRawButton(11))
		TKOGyro::inst()->reset();
	if (stick1.GetRawButton(8))
		RegDrive();
	if (stick1.GetRawButton(9))
		GyroDrive();
	if (stick3.GetTrigger())
	{
		/*if ((GetFPGATime() - TKOVision::inst()->lastTimestamp) <= 1000)
		{
			//TKOShooter::inst()->shootDist(TKOVision::inst()->lastDist);
		}*/
	}
}

void MarkXI::RegDrive()
{
	//TKOGDrive::inst()->Stop();
	TKODrive::inst()->Start();
}
void MarkXI::GyroDrive()
{
	TKODrive::inst()->Stop();
	//TKOGDrive::inst()->Start();
}

START_ROBOT_CLASS(MarkXI);
