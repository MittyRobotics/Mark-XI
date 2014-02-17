#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	Joystick stick; // only joystick
	CANJaguar armTest;

public:
	RobotDemo():
		stick(3),	// as they are declared above.
		armTest(7, CANJaguar::kPosition)
	{
		armTest.SetSafetyEnabled(true);
		armTest.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
		armTest.SetVoltageRampRate(0.0);
		armTest.ConfigFaultTime(0.1); 
		armTest.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		armTest.ConfigEncoderCodesPerRev(250);
		armTest.EnableControl(0.);
		printf("Robot init\n");
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
		
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {
		printf("Starting test\n");
		while (IsEnabled())
		{
			armTest.Set(stick.GetY());
			printf("Pos: %f\n", armTest.GetPosition());
			printf("Tar: %f\n", armTest.Get());
			Wait(0.01);				// wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

