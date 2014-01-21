#include "WPILib.h"
#include "Definitions.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class SuperDrive : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick1, stick2; // only joystick

public:
	SuperDrive():
		myRobot(DRIVE_L1_ID, DRIVE_L2_ID, DRIVE_R1_ID, DRIVE_R2_ID),	// these must be initialized in the same order
		stick1(STICK_1_PORT), stick2(STICK_2_PORT)		// as they are declared above.
	{}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl()&&IsEnabled())
		{	
				myRobot.TankDrive(stick1, stick2);
			Wait(0.005);				// wait for a motor update time
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(SuperDrive);

