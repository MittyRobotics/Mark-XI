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
	Joystick stick1; // only joystick
	DriverStationLCD* ds;

public:
	RobotDemo():
		l_f(DRIVE_L1_ID), l_b(DRIVE_L2_ID), r_f(DRIVE_R1_ID), r_b(DRIVE_R2_ID),
		stick1(STICK_1_PORT), ds()		// as they are declared above.
	{
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
	void OperatorControl(){
		while (IsOperatorControl()&&IsEnabled())
		{
			TankDrive();
		}
	}
	
	void TankDrive() {
		if(stick1.GetY()<0.0||stick1.GetThrottle()<0.0) {
				l_f.Set(-stick1.GetY() * 0.8);
				r_f.Set(stick1.GetThrottle() * 0.8);
				l_b.Set(-stick1.GetY() * 0.8);
				r_b.Set(stick1.GetThrottle() * 0.8);
				UpdateJag_pV_Val();
			}
		else {
				l_b.Set(-stick1.GetY() * 0.8);
				r_b.Set(stick1.GetThrottle() * 0.8);
				l_f.Set(-stick1.GetY() * 0.8);
				r_f.Set(stick1.GetThrottle() * 0.8);
				UpdateJag_pV_Val();
		}
	}
	
	void Test() {

	}
	
	void UpdateJag_pV_Val() {
		char f1[50], f2[50], r1[50], r2[50];
		sprintf(f1, "%f", l_f.Get());
		sprintf(f2, "%f", l_b.Get());
		sprintf(r1, "%f", r_f.Get());
		sprintf(r2, "%f", r_b.Get());
		ds->Clear();
		ds->PrintfLine(DriverStationLCD::kUser_Line1, f1);
		ds->PrintfLine(DriverStationLCD::kUser_Line2, f2);
		ds->PrintfLine(DriverStationLCD::kUser_Line3, r1);
		ds->PrintfLine(DriverStationLCD::kUser_Line4, r2);
		ds->UpdateLCD();
	}
};

START_ROBOT_CLASS(RobotDemo);

