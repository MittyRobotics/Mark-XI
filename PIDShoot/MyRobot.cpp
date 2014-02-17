#include "WPILib.h"
#include "Definitions.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	CANJaguar l_f, l_b, r_f, r_b;
	Joystick stick1, stick2, stick3, stick4; // only joystick

public:
	RobotDemo(void):
		l_f(DRIVE_L1_ID), l_b(DRIVE_L2_ID), r_f(DRIVE_R1_ID), r_b(DRIVE_R2_ID),
		stick1(SRICk_1_PORT), stick2(STICK_2_PORT), stick3(STICK_3_PORT), stick4(STICK_4_PORT)
	{
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		while (IsOperatorControl())
		{
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

