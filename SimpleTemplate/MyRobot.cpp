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
		Joystick stick1, stick2;
		
		DriverStationLCD* ds;

public:
	RobotDemo():
		l_f(DRIVE_L1_ID, CANJaguar::kPercentVbus), l_b(DRIVE_L2_ID, CANJaguar::kPercentVbus), r_f(DRIVE_R1_ID, CANJaguar::kPercentVbus), r_b(DRIVE_R2_ID, CANJaguar::kPercentVbus),
		stick1(STICK_1_PORT), stick2(STICK_2_PORT), ds()
	{
		l_f.ConfigEncoderCodesPerRev(ENCODER_REVS);
		r_f.ConfigEncoderCodesPerRev(ENCODER_REVS);
		l_f.SetPositionReference(JAG_POSREF);
		r_f.SetPositionReference(JAG_POSREF);
		ds = DriverStationLCD::GetInstance();
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
		while (IsOperatorControl()&& IsEnabled())
		{
			l_f.Set(1.0);
			l_b.Set(1.0);
			ds->Clear();
			char pos[50];
			sprintf(pos, "%f", l_f.GetPosition());
			ds->PrintfLine(DriverStationLCD::kUser_Line1, pos);
			ds->UpdateLCD();
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

