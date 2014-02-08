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
	RobotDrive myRobot; // robot drive system
	Joystick stick1, stick2; // only joystick1
	bool tank;

public:
	RobotDemo():
		l_f(DRIVE_L1_ID, CANJaguar::kPercentVbus),
		l_b(DRIVE_L2_ID, CANJaguar::kPercentVbus),
		r_f(DRIVE_R1_ID, CANJaguar::kPercentVbus),
		r_b(DRIVE_R2_ID, CANJaguar::kPercentVbus),
		myRobot(&l_f, &l_b, &r_f, &r_b),	// these must be initialized in the same order
		stick1(STICK_1_PORT), stick2(STICK_2_PORT)		// as they are declared above.
	{
		myRobot.SetSafetyEnabled(false);
		tank = false;
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
		while (IsOperatorControl())
		{
			if (stick1.GetTrigger()) {
				tank = !tank;
				Wait(0.5);
			}
			
			if (tank == false) {
				ArcadeDrive();
			}
			else {
				TankDrive();
			}

			Wait(0.005);				// wait for a motor update time
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
	
	void ArcadeDrive() {
		myRobot.ArcadeDrive(stick1); // drive with arcade style (use left drive stick)
	}
	
	void TankDrive() {
			if(stick1.GetY()>0.0||stick2.GetY()>0.0) {	// use both drive stick to drive left/right motors respectively
					l_f.Set(-stick2.GetY() * 0.8);
					r_f.Set(stick1.GetY() * 0.8);
					l_b.Set(-stick2.GetY() * 0.8);
					r_b.Set(stick1.GetY() * 0.8);
				}
			else {
					l_b.Set(-stick2.GetY() * 0.8);
					r_b.Set(stick1.GetY() * 0.8);
					l_f.Set(-stick2.GetY() * 0.8);
					r_f.Set(stick1.GetY() * 0.8);
			}
		}
};

START_ROBOT_CLASS(RobotDemo);

