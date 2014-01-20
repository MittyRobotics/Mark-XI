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
	Joystick l_d_stick, r_d_stick; // only joystick

public:
	RobotDemo():
		l_f(DRIVE_L1_ID), l_b(DRIVE_L2_ID), r_f(DRIVE_R1_ID), r_b(DRIVE_R2_ID),
		l_d_stick(STICK_1_PORT), r_d_stick(STICK_2_PORT)
	{
		l_f.SetPID(DRIVE_kP, DRIVE_kI,DRIVE_kD);
		l_b.SetPID(DRIVE_kP, DRIVE_kI,DRIVE_kD);
		r_f.SetPID(DRIVE_kP, DRIVE_kI,DRIVE_kD);
		r_b.SetPID(DRIVE_kP, DRIVE_kI,DRIVE_kD);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous() {}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		while (IsOperatorControl()&&IsEnabled())
		{
			TankDrive();
		}
	}
	
	void TankDrive()
	{
		 if (l_d_stick.GetRawButton(4))
		{
			l_f.Set(-l_d_stick.GetY() * 0.6);
			l_b.Set(-l_d_stick.GetY() * 0.6);
			r_f.Set(r_d_stick.GetY() * 0.6);
			r_b.Set(r_d_stick.GetY() * 0.6);
		}
		else if (l_d_stick.GetRawButton(2))
		{
			l_f.Set(-l_d_stick.GetY() * 0.8);
			l_b.Set(-l_d_stick.GetY() * 0.8);
			r_f.Set(r_d_stick.GetY() * 0.8);
			r_b.Set(r_d_stick.GetY() * 0.8);
		}
		else
		{
			l_f.Set(-l_d_stick.GetY());
			l_b.Set(-l_d_stick.GetY());
			r_f.Set(r_d_stick.GetY());
			r_b.Set(r_d_stick.GetY());
			
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

