//Last edited by Vadim Korolik
//on 02/04/2014
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
 * FOR 2014 OffSeason: Possibly add different log files for different tasks?
 * 
 * Figure out which solenoid corresponds with which id. (forward vs reverse)
 * 		Latch lock
 * 			Forward:
 * 			Reverse:
 * 		Catapult piston
 * 			Forward:
 * 			Reverse:
 * 		Drive shifter
 * 			Forward:
 * 			Reverse:
 * -----------------------------LAST DONE-------------------------------*
 * 02/05
 * 		A lot....... integrated all branches, ready for testing on robot.
 * 		Follow git commits.
 */

class MarkXI: public SimpleRobot
{
	public:
		Joystick stick1, stick2, stick3, stick4; // define joysticks
		DriverStation *ds; // define driver station object
		Encoder enc;
		Compressor compressor;
		CANJaguar canTest;
		Solenoid s1, s2, s3, s4, s5, s6;
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
			compressor(3, 1),
			canTest(7, CANJaguar::kPercentVbus),
			s1(1), s2(2), s3(3), s4(4), s5(5), s6(6)
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
	float lastSTog = GetTime();
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
	//TKOShooter::inst()->Start();
	printf("Started shooter, logger \n");
	TKOLogger::inst()->addMessage("STARTED SHOOTER, LOGGER IN TEST");
	while (IsEnabled())
	{
		DriverStation::GetInstance()->SetDigitalOut(1, s1.Get());
		DriverStation::GetInstance()->SetDigitalOut(2, s2.Get());
		DriverStation::GetInstance()->SetDigitalOut(3, s3.Get());
		DriverStation::GetInstance()->SetDigitalOut(4, s4.Get());
		DriverStation::GetInstance()->SetDigitalOut(5, s5.Get());
		DriverStation::GetInstance()->SetDigitalOut(6, s6.Get());
		DSLog(2, "Enc Val: %f", enc.Get());
		DSLog(3, "Enc Raw: %f", enc.GetRaw());
		DSLog(4, "Test: %f", canTest.GetSpeed())
		printf("Test: %f\n", canTest.GetSpeed());
		if (GetTime() - lastSTog < 1.) //1. is the constant for min delay between shifts
			return; 
		if (stick4.GetTrigger())
		{
			s1.Set(!s1.Get());
			lastSTog = GetTime();
		}
		if (stick4.GetRawButton(4))
		{
			s2.Set(!s2.Get());
			lastSTog = GetTime();
		}
		if (stick4.GetRawButton(3))
		{
			s3.Set(!s3.Get());
			lastSTog = GetTime();
		}
		if (stick4.GetRawButton(5))
		{
			s4.Set(!s4.Get());
			lastSTog = GetTime();
		}
		if (stick4.GetRawButton(8))
		{
			s5.Set(!s5.Get());
			lastSTog = GetTime();
		}
		if (stick4.GetRawButton(9))
		{
			s6.Set(!s6.Get());
			lastSTog = GetTime();
		}
	}
	printf("Stopping shooter, logger \n");
	//TKOShooter::inst()->Stop();
	compressor.Stop();
	enc.Stop();
	printf("Stopped testing \n");
	TKOLogger::inst()->addMessage("ENDED TEST MODE");
	TKOLogger::inst()->Stop();
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
	compressor.Stop();
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
	Molecule* turnRightBox = new Molecule();
		turnRightBox->MoleculeInit();
	//	printf("Test start");
	//	turnRightBox->Test();
//	//	printf("Test done");
//	DSLog(1, "Gyro Value: %f", TKOGyro::inst()->GetAngle());
		for(int i = 0; i < 1; i++){
			Atom* driveStraightTwoFeet = new DriveAtom(5.0f, &(turnRightBox->drive1), &(turnRightBox->drive2), &(turnRightBox->drive3), &(turnRightBox->drive4));
			//Atom* turnRightAngleR = new Turn_Atom(90.0f, &(turnRightBox->drive1), &(turnRightBox->drive2), &(turnRightBox->drive3), &(turnRightBox->drive4), TKOGyro::inst());
			turnRightBox->addAtom(driveStraightTwoFeet);
			//turnRightBox->addAtom(turnRightAngleR);
		}
		turnRightBox->start();

	//	TKOAutonomous::inst()->initAutonomous();
	//	TKOAutonomous::inst()->setDrivePID(DRIVE_kP, DRIVE_kP, DRIVE_kI);
	//	TKOAutonomous::inst()->setDriveTargetStraight(ds->GetAnalogIn(1) * 10 * REVS_PER_METER);
	//	TKOAutonomous::inst()->startAutonomous();

	TKOVision::inst()->StopProcessing();
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
	compressor.Start();
	//TKOShooter::inst()->Start();
	//TKOVision::inst()->StartProcessing();  //NEW VISION START
	RegDrive(); //Choose here between kind of drive to start with
	Timer loopTimer;
	loopTimer.Start();

	TKOLogger::inst()->addMessage("--------------Teleoperated started-------------");
	
	while (IsOperatorControl() && IsEnabled())
	{
		MarkXI::Operator();
		if (loopTimer.Get() > 0.1) {
			TKOLogger::inst()->addMessage(
					"!!!CRITICAL Operator loop very long, length",
					loopTimer.Get());
			printf("!!!CRITICAL Operator loop very long, %f%s\n",
					loopTimer.Get(), " seconds.");
		}
		//DSLog(1, "Dist: %f\n", TKOVision::inst()->getLastDistance());
		//DSLog(2, "Hot: %i\n", TKOVision::inst()->getLastTargetReport().Hot);
		DSLog(3, "G_ang: %f\n", TKOGyro::inst()->GetAngle());
		DSLog(4, "Clock %f\n", GetClock());
		Wait(LOOPTIME - loopTimer.Get());
		loopTimer.Reset();
	}
	
	loopTimer.Stop();
	printf("Ending OperatorControl \n");
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
