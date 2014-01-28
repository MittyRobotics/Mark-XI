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
	Joystick stick1, stick2; // only joystick

public:
	RobotDemo():
		l_f(DRIVE_L1_ID), l_b(DRIVE_L2_ID), r_f(DRIVE_R1_ID), r_b(DRIVE_R2_ID),
		stick1(STICK_1_PORT), stick2(STICK_2_PORT)		// as they are declared above.
	{
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{

	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(){
		while (IsOperatorControl()&&IsEnabled())
		{
			TankDrive();
		}
	}
	
	void TankDrive() {
		if(stick1.GetY()>0.0||stick2.GetY()>0.0) {
				l_f.Set(-stick1.GetY() * 0.8);
				r_f.Set(stick2.GetY() * 0.8);
				l_b.Set(-stick1.GetY() * 0.8);
				r_b.Set(stick2.GetY() * 0.8);
			}
		else {
				l_b.Set(-stick1.GetY() * 0.8);
				r_b.Set(stick2.GetY() * 0.8);
				l_f.Set(-stick1.GetY() * 0.8);
				r_f.Set(stick2.GetY() * 0.8);
		}
	}
	
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

