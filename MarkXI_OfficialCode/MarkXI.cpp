//Last edited by Vadim Korolik
//on 03/01/2014
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
#include "auton/DriveAtomUsonic.h"
#include "auton/DriveAndShootUsonicAtom.h"
#include "auton/Molecule.h"
#include "auton/TurnAtom.h"
#include "auton/ShootAtom.h"
#include "auton/CameraShootAtom.h"

/*---------------MarkXI-Things-to-Do(TODO)---------------------* 
 * At beginning of auton, shift to high gear	DONE
 * 
 * Add driving jaguar current logging/burnout
 * ADD BATERY VOLTAGE LOGGING
 * 
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
 * 	Creating duplicate static pneumatics objects in statemachine and in markxi
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
//	void RegDrive();
//	void GyroDrive();

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
	/*{
		printf("----------------------\n");
		printf("Deleting log...\n");
		remove("logT.txt");
		printf("Digital input 1 true\n");
	}*/
	{
		printf("Renaming previous log file\n");		
		string fileNameDefault = "logT.txt";
		stringstream fileName;
		fileName << fileNameDefault << rand();
		rename(fileNameDefault.c_str(), fileName.str().c_str());
		printf("Completed with renaming previous log file\n");
		//maybe use file finding? (find())
	}
	
	TKOLogger::inst()->addMessage("----------ROBOT BOOT-----------TIMESTAMP: %f", GetFPGATime());
	TKOGyro::inst()->reset();
	//AxisCamera::GetInstance(); //boot up camera, maybe add check to see if it worked?
	printf("Initialized the MarkXI class \n");
}

void MarkXI::Test()
{	
	printf("Calling test function \n");
	TKOLogger::inst()->Start();
	if (DriverStation::GetInstance()->GetDigitalIn(2))
		compressor.Start();
	while (IsEnabled())
	{
		DSClear();
		StateMachine::updateDriverStationSwitchDisplay();
		//StateMachine::
	}
	printf("Stopped testing \n");
	compressor.Stop();
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
		if (DriverStation::GetInstance()->GetAlliance() == DriverStation::GetInstance()->kBlue)
			TKOLogger::inst()->addMessage("BLUE ALLIANCE!");
		if (DriverStation::GetInstance()->GetAlliance() == DriverStation::GetInstance()->kRed)
			TKOLogger::inst()->addMessage("RED ALLIANCE!");
	}

	/* ---TODO for auton---
	 * insert new PID values
	 * during auton: shoot & drive forward, calibrate arm?
	 */
	float driveDist = DriverStation::GetInstance()->GetAnalogIn(3);
	//TKOVision::inst()->StartProcessing();
	TKOShooter::inst()->Start();
	TKOArm::inst()->Start();
	TKOArm::inst()->setArmTarget(0.);
	/*TKOArm::inst()->forwardCalibration();
	Wait(0.1);*/
	Wait(.5);
	
	Molecule* molecule = new Molecule();
	Atom* driveForward = new DriveAtomUsonic(driveDist, TKOArm::inst()->getUsonic(), &molecule->drive1, &molecule->drive2, &molecule->drive3, &molecule->drive4);
	//Atom* driveAndShoot = new DriveAndShootUsonicAtom(driveDist, TKOArm::inst()->getUsonic(), &molecule->drive1, &molecule->drive2, &molecule->drive3, &molecule->drive4, TKODrive::inst()->getShifterDoubleSolenoid());
	//Atom* cameraWait = new CameraShootAtom(TKOArm::inst()->getUsonic());
	Atom* shoot = new ShootAtom();

	//molecule->addAtom(cameraWait);
	//molecule->addAtom(driveAndShoot);
	molecule->addAtom(driveForward);
	molecule->addAtom(shoot);
	molecule->MoleculeInit();
	
	molecule->start();

	printf("Ending Autonomous \n");
	//delete cameraWait;
	//delete driveForward;
	//delete driveAndShoot;
	//delete molecule;
	//delete shoot;
	
	//TKOVision::inst()->StopProcessing();
	while (IsEnabled())
	{}
	TKOShooter::inst()->Stop();
	TKOArm::inst()->Stop();
	//TKOLogger::inst()->addMessage("--------------Autonomous ended-------------");
	printf("Ended autonomous\n");
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
	TKODrive::inst()->Start();
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
		
		if (DriverStation::GetInstance()->GetBatteryVoltage() < 8.)
		{
			TKOLogger::inst()->addMessage("Battery voltage very low: %f\n", DriverStation::GetInstance()->GetBatteryVoltage());
		}
	}

	TKOShooter::inst()->Stop();
	TKODrive::inst()->Stop();
	TKOArm::inst()->Stop();
	loopTimer.Stop();
	compressor.Stop();	
	printf("Ending OperatorControl \n");
	TKOLogger::inst()->addMessage("Ending OperatorControl");
}

void MarkXI::Operator()
{
	/*if (stick4.GetTrigger() and stick4.GetRawButton(10))
		TKOArm::inst()->calibrateArm();*/
	
	// stick 1 button 10 will use analog input 4 to set drive motors (see TKODrive.cpp)
	// stick 4 button 9  will use analog input 3 to set arm position (see TKOArm.cpp)
	
	//if (stick1. GetRawButton(11))
	//	TKOGyro::inst()->reset();

}

/*void MarkXI::RegDrive()
{
	//TKOGDrive::inst()->Stop();
	TKODrive::inst()->Start();
}
void MarkXI::GyroDrive()
{
	TKODrive::inst()->Stop();
	//TKOGDrive::inst()->Start();
}*/

START_ROBOT_CLASS(MarkXI);
