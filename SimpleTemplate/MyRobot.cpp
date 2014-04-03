#include "WPILib.h"
#include "drive/TKODrive.h"
#include "auton/DriveAtom.h"
#include "auton/Molecule.h"

class RobotDemo : public SimpleRobot
{
	Joystick s1, s2;
public:
	RobotDemo():
		s1(1),
		s2(2)
	{}

	void Autonomous()
	{
		printf("Auton started\n");
		Molecule* molecule = new Molecule();
		Atom* driveForward = new DriveAtom(15., &molecule->drive1, &molecule->drive2, &molecule->drive3, &molecule->drive4);
		//Atom* driveAndShoot = new DriveAndShootUsonicAtom(driveDist, TKOArm::inst()->getUsonic(), &molecule->drive1, &molecule->drive2, &molecule->drive3, &molecule->drive4, TKODrive::inst()->getShifterDoubleSolenoid());
		//Atom* driveAndShootNoShift = new DriveAndShootUsonicAtom(driveDist, TKOArm::inst()->getUsonic(), &molecule->drive1, &molecule->drive2, &molecule->drive3, &molecule->drive4, NULL);
		//Atom* cameraWait = new CameraShootAtom(TKOArm::inst()->getUsonic());
		//Atom* shoot = new ShootAtom();

		//molecule->addAtom(cameraWait);
		//molecule->addAtom(driveAndShoot);
		molecule->addAtom(driveForward);
		molecule->MoleculeInit();
		
		molecule->start();
		printf("Auton done\n");
	}

	void OperatorControl()
	{
		printf("STARTING OPERATOR\n");
		TKODrive::inst()->Start();
		TKOLogger::inst()->Start();
		while (IsOperatorControl() && IsEnabled())
		{
			
		}
		TKODrive::inst()->Stop();
		TKOLogger::inst()->Stop();
	}

	void Test() 
	{
		while (IsEnabled())
		{
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
