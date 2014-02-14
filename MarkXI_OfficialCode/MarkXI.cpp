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
#include "auton/ShootAtom.h"

/*---------------MarkXI-Things-to-Do(TODO)---------------------* 
 * FOR 2014 OffSeason: take over scouting? Swag troubleshooting lookup (MYSQL)
 * 
 * Test the wait time for how long the roller spins before the shooter fires
 * VxWorks Articles:
 * 		http://touro.ligo-la.caltech.edu/~cparames/CDS/vxWorks_commands.html
 * 		http://csg.lbl.gov/pipermail/vxwexplo/2003-March/000762.html
 * 
 * -----------------------------LAST DONE-------------------------------*
 * 02/13
 *  Shoot atom, arm additions, state machine improvements
 * 02/07
 *  StateMachine::initPneumatics() --> sets pneumatics to default positions
 * 	Creating duplicate static neumatics objects in statemachine and in markxi
 * 	Similar for joystick objects in StateMachine
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
	AxisCamera::GetInstance(); //boot up camera, maybe add check to see if it worked?
	printf("Initialized the MarkXI class \n");
}

void MarkXI::Test()
{	
	printf("Calling test function \n");
	TKOLogger::inst()->Start();
	
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
	TKOVision::inst()->StopProcessing();
	TKOLogger::inst()->Stop();
	printf("Robot successfully died!\n");
	while (IsDisabled())
	{}
}

void MarkXI::Autonomous(void)
{
	printf("Starting Autonomous \n");
	TKOLogger::inst()->addMessage("--------------Autonomous started-------------");
	TKOVision::inst()->StartProcessing();
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
	Molecule* turnRightBox = new Molecule();
	Molecule* fire = new Molecule();
	turnRightBox->MoleculeInit();
	
	Atom* driveStraightTwentyFeet = new DriveAtom(2.0f, &(turnRightBox->drive1), &(turnRightBox->drive2), &(turnRightBox->drive3), &(turnRightBox->drive4));
	Atom* shootAtom = new ShootAtom();	
	
	turnRightBox->addAtom(driveStraightTwentyFeet);
	//turnRightBox->start();
	
	fire->addAtom(shootAtom);
	fire->start();

	TKOVision::inst()->StopProcessing();
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
	TKOVision::inst()->StartProcessing();  //NEW VISION START
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
		DSLog(1, "Dist: %f\n", TKOVision::inst()->getLastDistance());
		DSLog(2, "Hot: %i\n", TKOVision::inst()->getLastTargetReport().Hot);
		//DSLog(3, "G_ang: %f\n", TKOGyro::inst()->GetAngle());
		//Wait(LOOPTIME - loopTimer.Get());
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
	if (stick4.GetRawButton(8))
		TKOArm::inst()->toggleMode();
	if (stick4.GetRawButton(3))
		TKOArm::inst()->moveToFront();
	if (stick4.GetRawButton(2))
		TKOArm::inst()->moveToMid();
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
