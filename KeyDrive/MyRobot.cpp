//Last Edited by Shreyas Vaidya (Finally!)
#include "WPILib.h"
#include "RobotDrive.h"
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
	Joystick keyboard;
	Gyro turn;

public:
	RobotDemo():
		l_f(DRIVE_L1_ID), l_b(DRIVE_L2_ID), r_f(DRIVE_R1_ID), r_b(DRIVE_R2_ID),
		keyboard(STICK_1_PORT),	// as they are declared above.
		turn(1, GYRO_PORT)
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
	
		void TankDrive() {
				if (keyboard.GetRawButton(3)) {
					l_f.Set(-0.8);
					r_f.Set(0.8);
					l_b.Set(-0.8);
					r_b.Set(0.8);
				}
				if(keyboard.GetRawButton(4)) {
					l_b.Set(0.8);
					r_b.Set(-0.8);
					l_f.Set(0.8);
					r_f.Set(-0.8);
				}
				if(keyboard.GetRawButton(1)) {
					l_f.Set(0.8);
					l_b.Set(0.8);
					r_f.Set(0.8);
					r_b.Set(0.8);
					printf("angle: %f", turn.GetAngle());
					Wait(0.1);
				}
				if(keyboard.GetRawButton(2)) {
					r_f.Set(-0.8);
					r_b.Set(-0.8);
					l_f.Set(-0.8);
					l_b.Set(-0.8);
					printf("angle: %f", turn.GetAngle());
					Wait(0.1);
				}
				printf("angle: %f", turn.GetAngle());
				Wait(0.1);
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

//spam
//soap
