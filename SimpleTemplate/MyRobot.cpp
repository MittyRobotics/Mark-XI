#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	Joystick stick1; // only joystick
	//PWM set1, set2, set3, set4;
	DigitalOutput cool;

public:
	RobotDemo():
		stick1(1),		// as they are declared above.
		//set1(2,2), set2(2,3), set3(2,4), set4(2,5)
		cool(2,3)
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
	void OperatorControl()
	{
		while (IsEnabled())
		{
			/*if(stick1.GetRawButton(1)) {
				set1.SetRaw(255);
			}
			else
				set1.SetRaw(0);
			if(stick1.GetRawButton(2)) {
							set2.SetRaw(255);
						}
						else
							set2.SetRaw(0);
			if(stick1.GetRawButton(3)) {
				set3.SetRaw(255);
			}
			else
				set3.SetRaw(0);
			if(stick1.GetRawButton(4)) {
				set4.SetRaw(255);
			}
			else
				set4.SetRaw(0);
				*/
//			if (stick1.GetRawButton(1))
//				set1.SetRaw(255);
//			else
//				set1.SetRaw(255);
			if(stick1.GetRawButton(1))
			cool.Set(0);
			else
				cool.Set(1);
			Wait(0.01);
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

