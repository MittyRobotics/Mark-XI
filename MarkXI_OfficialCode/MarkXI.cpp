//Last edited by Vadim Korolik
//on 06/03/2014
#include "Definitions.h"
#include "component/TKORelay.h"
#include "log/TKOLogger.h"
#include "drive/TKODrive.h"
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
 * Add driving jaguar current logging/burnout
 * ADD BATERY VOLTAGE LOGGING
 * 
 * FOR 2014 OffSeason: take over scouting?
 * 
 * Test the wait time for how long the roller spins before the shooter fires
 * 
 * Determine default pneumatic states on initialization
 * 
 * !!! Netconsole jaguar error messages
 * 
 * !!! Use network tables
 * 
 * Vision creashes on file write
 * 
 * Arm jerking
 * 		line 171 arm, calibration 
 * 		line 212 arm current timeout
 * 		
 * AUTOMATIC CALIBRATION OR DOCUMENTED PROCEDURE???
 * 
 * -----------------------------LAST DONE-------------------------------*
 * Last Match SVR
 * 
 * 	CRITICAL: StateMachine initialization conflicts with MarkXI solenoid initialization
 * 	Does it fail because of static object initialization, or is it due to conflict? DONE
 * 	
 * 	At beginning of auton, shift to high gear	DONE
 * 
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
	
	int fileNum = 0;
	stringstream fileName;
	while (true)
	{
		fileName << "logT_" << fileNum << ".txt";
		if (access(fileName.str().c_str(), F_OK) != -1)
		{
			//file exists
			fileNum += 1;
		}
		else
			break;
	}

	printf("Renaming previous log file\n");		
	string fileNameDefault = "logT.txt";
	rename(fileNameDefault.c_str(), fileName.str().c_str());
	printf("Completed with renaming previous log file %s\n", fileName.str().c_str());
	
	TKOLogger::inst()->addMessage("----------ROBOT BOOT-----------TIMESTAMP: %f", GetFPGATime());
	TKOGyro::inst()->reset();
	TKORoller::inst();
	AxisCamera::GetInstance(); //boot up camera, maybe add check to see if it worked?
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
	TKOVision::inst()->StopProcessing();
	TKOLogger::inst()->Stop();
	printf("Robot successfully died!\n");
	while (IsDisabled())
	{
		Wait(.1);
	}
}

void MarkXI::Autonomous(void)
{
	printf("Starting Autonomous \n");

	float driveDist = 6.5;//DriverStation::GetInstance()->GetAnalogIn(3); //currently 6.5
	Molecule* molecule = new Molecule();
	Atom* driveForward = new DriveAtomUsonic(driveDist, TKOArm::inst()->getUsonic(), &molecule->drive1, &molecule->drive2, &molecule->drive3, &molecule->drive4);
	//Atom* driveAndShoot = new DriveAndShootUsonicAtom(driveDist, TKOArm::inst()->getUsonic(), &molecule->drive1, &molecule->drive2, &molecule->drive3, &molecule->drive4, TKODrive::inst()->getShifterDoubleSolenoid());
	//Atom* cameraWait = new CameraShootAtom(TKOArm::inst()->getUsonic());
	Atom* shoot = new ShootAtom();
	//molecule->addAtom(cameraWait);
	//molecule->addAtom(driveAndShoot);
	molecule->addAtom(driveForward);
	//if (DriverStation::GetInstance()->GetDigitalIn(8))
	molecule->addAtom(shoot);
	molecule->MoleculeInit();
	
	TKOLogger::inst()->Start();
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
	
	///INITIALIZATION COMPLETE ^; START ACTUAL ACTIONS
	
	TKOShooter::inst()->Start();
	TKOArm::inst()->Start();
	Wait(.25);
	TKOArm::inst()->setArmTarget(ARM_MID_POSITION);
	Wait(.5);
	molecule->start();
	//MOLECULE RUNS HERE
	delete molecule;
	
	while (IsEnabled()) //Once complete with molecule, remain in this loop until match switches to teleop or disabled
	{}
	TKOShooter::inst()->Stop();
	TKOArm::inst()->Stop();
	TKOLogger::inst()->addMessage("--------------Autonomous ended-------------");
	printf("Ended autonomous\n");
}

void MarkXI::OperatorControl()
{	
	printf("Starting OperatorControl \n");
	TKOLogger::inst()->Start();
	TKOGyro::inst()->reset();
	compressor.Start();
	TKOShooter::inst()->Start();
	TKOArm::inst()->Start();
	TKOVision::inst()->StartProcessing();  //NEW VISION START
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
		Wait(LOOPTIME - loopTimer.Get());
		loopTimer.Reset();
		
		if (DriverStation::GetInstance()->GetBatteryVoltage() < 8.)
		{
			TKOLogger::inst()->addMessage("ASC Battery voltage very low: %f\n", DriverStation::GetInstance()->GetBatteryVoltage());
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
	// stick 1 button 10 will use analog input 4 to set drive motors (see TKODrive.cpp)
	// stick 4 button 9  will use analog input 3 to set arm position (see TKOArm.cpp)
}

START_ROBOT_CLASS(MarkXI);
