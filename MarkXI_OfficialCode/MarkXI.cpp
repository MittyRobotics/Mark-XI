//Last edited by Vadim Korolik
//on 02/06/2014
#include "Definitions.h"
#include "component/TKORelay.h"
#include "log/TKOLogger.h"
#include "drive/TKODrive.h"
//#include "drive/TKOGDrive.h"
#include "component/TKOGyro.h"
#include "component/TKORelay.h"
//#include "vision/TKOVision.h"
#include "evom/TKOShooter.h"
#include "evom/StateMachine.h"
#include "auton/Atom.h"
#include "auton/DriveAtom.h"
#include "auton/Molecule.h"
#include "auton/TurnAtom.h"

/*---------------MarkXI-Thing-to-Do(TODO)---------------------* 
 * FOR 2014 OffSeason: take over scouting?
 * 
 * -----------------------------LAST DONE-------------------------------*
 * 02/06
 *  Figured out pneumatics
 * 		Latch lock
 * 			Forward:2 (latch forward, locked)
 * 			Reverse:5 (latch backward, unlocked)
 * 		Catapult piston
 * 			Forward:3
 * 			Reverse:6
 * 		Drive shifter
 * 			Forward:7
 * 			Reverse:4
 * 02/05
 * 		A lot....... integrated all branches, ready for testing on robot.
 * 		Follow git commits.
 */

class MarkXI: public SimpleRobot
{
	public:
		Joystick stick1, stick2, stick3, stick4; // define joysticks
		Compressor compressor;
		CANJaguar canTest;
		DoubleSolenoid _piston_retract_extend;
		DoubleSolenoid _latch_lock_unlock;
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
			compressor(PRESSURE_SWITCH_PORT, COMPRESSOR_ID),
			canTest(7, CANJaguar::kPercentVbus),
			_piston_retract_extend(PISTON_RETRACT_SOLENOID_A, PISTON_RETRACT_SOLENOID_B),
			_latch_lock_unlock(LATCH_RETRACT_SOLENOID_A, LATCH_RETRACT_SOLENOID_B)
		{/*DO NOT USE THIS!!! USE RobotInit()*/}
};
void MarkXI::RobotInit()
{
	printf("Initializing MarkXI class \n");
	canTest.SetSafetyEnabled(false);
	canTest.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
	canTest.SetVoltageRampRate(0.0);
	canTest.ConfigFaultTime(0.1); 
	canTest.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	canTest.ConfigEncoderCodesPerRev(250);
	canTest.EnableControl();
	if (DriverStation::GetInstance()->GetDigitalIn(1))
	{
		printf("----------------------\n");
		printf("Deleting log...\n");
		remove("logT.txt");
		printf("Digital input 1 true\n");
	}
	TKOLogger::inst()->addMessage("----------ROBOT BOOT-----------");
	TKOGyro::inst()->reset();
//	AxisCamera::GetInstance(); //boot up camera, maybe add check to see if it worked?
	printf("Initialized the MarkXI class \n");
}

void MarkXI::Test()
{
	TKOLogger::inst()->addMessage("STARTING TEST MODE");

	if (DriverStation::GetInstance()->GetDigitalIn(1))
	{
		printf("----------------------\n");
		printf("Deleting log...\n");
		remove("logT.txt");
		printf("Digital input 1 true\n");
	}
	printf("Calling test function \n");
	printf("Starting tasks \n");
	TKOLogger::inst()->Start();
	TKOShooter::inst()->Start();
	printf("Started shooter, logger \n");
	TKOLogger::inst()->addMessage("STARTED SHOOTER, LOGGER IN TEST");
	while (IsEnabled())
	{
		DSClear();
//		camera.Set(stick1.GetX());
//		camera.EnableDeadbandElimination(true);
	}
	printf("Stopping shooter, logger \n");
	TKOShooter::inst()->Stop();
	TKOLogger::inst()->Stop();
	printf("Stopped testing \n");
	TKOLogger::inst()->addMessage("ENDED TEST MODE");
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

	//TKOVision::inst()->StopProcessing();
	//TKOVision::inst()->StartProcessing();
	TKOLogger::inst()->addMessage("--------------Autonomous started-------------");
	if (DriverStation::GetInstance()->IsFMSAttached())
	{
		TKOLogger::inst()->addMessage("-----------FMS DETECTED------------");
		TKOLogger::inst()->addMessage("PROBABLY A SERIOUS MATCH");
		if (DriverStation::GetInstance()->GetAlliance() == DriverStation::GetInstance()->kBlue);
			TKOLogger::inst()->addMessage("BLUE ALLIANCE!");
		if (DriverStation::GetInstance()->GetAlliance() == DriverStation::GetInstance()->kRed);
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
			TKOLogger::inst()->addMessage("!!!CRITICAL Operator loop very long, %f%s\n",loopTimer.Get(), " seconds.");
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
