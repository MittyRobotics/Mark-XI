//Last Edited by Zach Light and Ben Kim
// on 01/31/2014
#include "Definitions.h"
#include "component/TKORelay.h"
#include "log/TKOLogger.h"
#include "drive/TKODrive.h"
#include "drive/TKOGDrive.h"
#include "component/TKOGyro.h"
//#include "auton/TKOAutonomous.h"
#include "vision/TKOVision.h"
#include "evom/TKOShooter.h"
#include "evom/TKOIntake.h"
#include "auton/Molecule.h"
#include "auton/Atom.h"
#include "auton/TurnAtom.h"
#include "auton/DriveAtom.h"

/*---------------MarkXI-Thing-to-Do(TODO)---------------------* 
 * Auton, vision tests
 * Add logging to more stuff, auton and vision
 * 
 * For shooter, create calibration curve, best fit equation of shoot power VS. ball flight dist
 * Use equation to calculate needed power for desired distance 
 * 
 */
//TODO CRITICAL Figure out why Logger, Auton crash on cRio on boot (After deploy) Code compiles
/*
 * -----------------------------LAST DONE-------------------------------*
 * 01/04 
 * 		Preparing as a core for build season
 * 		Updated vision to 2014 vision sample
 * 		improving logging, adding in more places
 * 		GetFPGATime() return long, accurate sys time
 * 		
 */

class MarkXI: public SimpleRobot {
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
void MarkXI::Test() {
	printf("Calling test function \n");
}

void MarkXI::RobotInit() {
	printf("Initializing MarkXI class \n");
	//TKOGyro::inst()->reset();
	//	AxisCamera::GetInstance(); //boot up camera, maybe add check to see if it worked?
	printf("Initialized the MarkXI class \n");
}

void MarkXI::Disabled() {
	printf("Robot Dying!\n");
	//TKOShooter::inst()->Stop();
	TKODrive::inst()->Stop();
	TKOGDrive::inst()->Stop();
	TKOVision::inst()->StopProcessing();
	TKOLogger::inst()->Stop();
	printf("Robot successfully died!\n");
	while (IsDisabled()) {

	}
}

void MarkXI::Autonomous(void) {
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
	printf("Ending Autonomous \n");
}

void MarkXI::OperatorControl() {
	printf("Starting OperatorControl \n");
	ds = DriverStation::GetInstance();
	TKOLogger::inst()->Start();
	TKOGyro::inst()->reset();
	//TKOShooter::inst()->Start();
	TKOVision::inst()->StartProcessing(); //NEW VISION START
	RegDrive(); //Choose here between kind of drive to start with
	Timer loopTimer;
	loopTimer.Start();

	TKOLogger::inst()->addMessage(
			"--------------Teleoperated started-------------");

	while (IsOperatorControl() && IsEnabled()) {
		loopTimer.Reset();
		DSClear();

		MarkXI::Operator();
		if (loopTimer.Get() > 0.1) {
			TKOLogger::inst()->addMessage(
					"!!!CRITICAL Operator loop very long, length",
					loopTimer.Get());
			printf("!!!CRITICAL Operator loop very long, %f%s\n",
					loopTimer.Get(), " seconds.");
		}
		DSLog(1, "Dist: %f\n", TKOVision::inst()->getLastDistance());
		DSLog(2, "Hot: %i\n", TKOVision::inst()->getLastTargetReport().Hot);
		DSLog(3, "G_ang: %f\n", TKOGyro::inst()->GetAngle());
		DSLog(4, "Clock %f\n", GetClock());
		DSLog(5, "")
		Wait(LOOPTIME - loopTimer.Get());
		loopTimer.Reset();
	}
	printf("Ending OperatorControl \n");
	TKOLogger::inst()->addMessage("Ending OperatorControl");
}

void MarkXI::Operator() {
	if (stick1. GetRawButton(11))
		TKOGyro::inst()->reset();
	if (stick1.GetRawButton(8))
		RegDrive();
	if (stick1.GetRawButton(9))
		GyroDrive();
	if (stick3.GetTrigger()) {
		if ((GetFPGATime() - TKOVision::inst()->lastTimestamp) <= 1000) {
			//TKOShooter::inst()->shootDist(TKOVision::inst()->lastDist);
		}
	}
}

void MarkXI::RegDrive() {
	TKOGDrive::inst()->Stop();
	TKODrive::inst()->Start();
}
void MarkXI::GyroDrive() {
	TKODrive::inst()->Stop();
	TKOGDrive::inst()->Start();
}

START_ROBOT_CLASS(MarkXI)
;
