//Last edited by Vadim Korolik
//on 02/07/2014
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
#include "evom/TKOArm.h"
#include "auton/Atom.h"
#include "auton/DriveAtom.h"
#include "auton/Molecule.h"
#include "auton/TurnAtom.h"
//#define PNEUMATICS_TEST_MODE
//#define ARM_TEST_MODE

/*---------------MarkXI-Things-to-Do(TODO)---------------------* 
 * FOR 2014 OffSeason: take over scouting?
 * 
 * Test the wait time for how long the roller spins before the shooter fires
 * 
 * CRITICAL: StateMachine initialization conflicts with MarkXI solenoid initialization
 * Does it fail because of static object initialization, or is it due to conflict?
 * 
 * Determine default pneumatic states on initialization
 * 
 * -----------------------------LAST DONE-------------------------------*
 * 02/07
 *  StateMachine::initPneumatics() --> sets pneumatics to default positions
 * 	Creating duplicate static neumatics objects in statemachine and in markxi
 * 	breaks robot, causes kernel exception? or maybe static objects?
 * 	Added more testing, added TKOArm, uses TKORoller
 * 	DriverStation Digital Inputs in Test mode:
 * 		1: If on, deletes log
 * 		2: If on, turns on compressor
 * 		3: If on, starts TKOShooter
 * 		4: If on, starts TKOArm
 * 	Use #define PNEUMATICS_TEST_MODE and #define ARM_TEST_MODE to control test function actions
 * 	
 * 	Set ALL task priorities
 * 	
 * 02/06
 *  Figured out pneumatics, refer to Definitions.h
 * 02/05
 * 		A lot....... integrated all branches, ready for testing on robot.
 * 		Follow git commits.
 */

class MarkXI: public SimpleRobot
{
public:
	Joystick stick1, stick2, stick3, stick4; // define joysticks
	Compressor compressor;
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
		compressor(PRESSURE_SWITCH_PORT, COMPRESSOR_ID)
	{/*DO NOT USE THIS!!! USE RobotInit()*/}
};
void MarkXI::RobotInit()
{
	printf("Initializing MarkXI class \n");
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
	printf("Calling test function \n");
	TKOLogger::inst()->Start();
#ifdef PNEUMATICS_TEST_MODE
	DoubleSolenoid* _piston_retract_extend = new DoubleSolenoid(PISTON_RETRACT_SOLENOID_A, PISTON_RETRACT_SOLENOID_B);
	DoubleSolenoid* _latch_lock_unlock = new DoubleSolenoid(LATCH_RETRACT_SOLENOID_A, LATCH_RETRACT_SOLENOID_B);
#endif
#ifdef ARM_TEST_MODE
	CANJaguar* armTest = new CANJaguar(7, CANJaguar::kPercentVbus);
	armTest->SetSafetyEnabled(false);
	armTest->ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
	armTest->SetVoltageRampRate(0.0);
	armTest->ConfigFaultTime(0.1); 
	armTest->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	armTest->ConfigEncoderCodesPerRev(250);
	armTest->EnableControl();
#endif
	float lastSTog = GetTime();
	StateMachine::initPneumatics(); //TODO make sure this works; sets pneumatics to default start positions
	printf("Done with test initialization \n");

	printf("Starting tasks \n");
	TKOLogger::inst()->addMessage("STARTING TEST MODE");
	if (DriverStation::GetInstance()->GetDigitalIn(2))
		compressor.Start();
	else
		compressor.Stop();
	if (DriverStation::GetInstance()->GetDigitalIn(3))
		TKOShooter::inst()->Start();
	if (DriverStation::GetInstance()->GetDigitalIn(4)) //TODO Before running this, remove armTest initialization
		TKOArm::inst()->Start();
	printf("Driver station depended actions completed\n");

	TKOLogger::inst()->addMessage("ENTERING TEST LOOP");

	while (IsEnabled())
	{
		DSLog(5, "Arm: %d", TKOArm::inst()->armInFiringRange());
		if (stick4.GetRawButton(2))
			TKOArm::inst()->moveToBack();
		if (stick4.GetRawButton(3))
			TKOArm::inst()->moveToFront();
#ifdef ARM_TEST_MODE
		armTest->Set(stick4.GetY()*-0.5);
#endif
#ifdef PNEUMATICS_TEST_MODE
		DriverStation::GetInstance()->SetDigitalOut(1, _piston_retract_extend->Get());
		DriverStation::GetInstance()->SetDigitalOut(2, _latch_lock_unlock->Get());
#endif
#ifdef ARM_TEST_MODE
		DSLog(1, "Arm Pos: %f", armTest->GetPosition());
		DSLog(2, "Arm Volt: %f", armTest->GetOutputVoltage());
		DSLog(3, "Arm Curr %f", armTest->GetOutputCurrent());
#endif
		if (GetTime() - lastSTog < 1.) //1. is the constant for min delay between shifts
			continue; 
#ifdef PNEUMATICS_TEST_MODE
		if (stick4.GetRawButton(4))
		{
			_piston_retract_extend->Set(_piston_retract_extend->kForward);
			lastSTog = GetTime();
		}
		if (stick4.GetRawButton(5))
		{
			_piston_retract_extend->Set(_piston_retract_extend->kReverse);
			lastSTog = GetTime();
		}
		if (stick4.GetRawButton(3))
		{
			_latch_lock_unlock->Set(_latch_lock_unlock->kForward);
			lastSTog = GetTime();
		}
		if (stick4.GetRawButton(2))
		{
			_latch_lock_unlock->Set(_latch_lock_unlock->kReverse); //reverse if pulled back
			lastSTog = GetTime();
		}
#endif
	}
	printf("Stopping shooter, logger \n");
	TKOShooter::inst()->Stop();
	TKOArm::inst()->Stop();
	compressor.Stop();
	printf("Stopped testing \n");
	TKOLogger::inst()->addMessage("ENDED TEST MODE");
	TKOLogger::inst()->Stop();
}

void MarkXI::Disabled()
{
	printf("Robot Dying!\n");
	compressor.Stop();
	TKOLogger::inst()->addMessage("Robot disabled.");
	TKOShooter::inst()->Stop();
	TKODrive::inst()->Stop();
	TKOArm::inst()->Stop();
	//TKOGDrive::inst()->Stop();
	//TKOVision::inst()->StopProcessing();
	TKOLogger::inst()->Stop();
	printf("Robot successfully died!\n");
	while (IsDisabled())
	{}
}

void MarkXI::Autonomous(void)
{
	printf("Starting Autonomous \n");
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

	/* ---TODO for auton---
	 * insert new PID values
	 * during auton: shoot & drive forward, calibrate arm?
	 */

	//TKOVision::inst()->StopProcessing();
	printf("Ending Autonomous \n");
	TKOLogger::inst()->addMessage("--------------Autonomous ended-------------");
}

void MarkXI::OperatorControl()
{	
	printf("Starting OperatorControl \n");
	TKOLogger::inst()->Start();
	TKOGyro::inst()->reset();
	compressor.Start();
	//StateMachine::initPneumatics();
	TKOShooter::inst()->Start();
	TKOArm::inst()->Start();
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
		//DSLog(3, "G_ang: %f\n", TKOGyro::inst()->GetAngle());
		//DSLog(4, "Clock %f\n", GetClock());
		Wait(LOOPTIME - loopTimer.Get());
		loopTimer.Reset();
	}
	TKODrive::inst()->Stop();
	loopTimer.Stop();
	compressor.Stop();
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
