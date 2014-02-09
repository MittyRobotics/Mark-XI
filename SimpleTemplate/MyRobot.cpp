#include "WPILib.h"
#include "Definitions.h"
#include <sstream>

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	CANJaguar drive1, drive2, drive3, drive4; // robot drive system
	Joystick stick; // only joystick
	DriverStationLCD* ds;
	int run;

public:
	RobotDemo():
		drive1(DRIVE_L1_ID, CANJaguar::kPercentVbus), drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus), drive3(DRIVE_R1_ID, CANJaguar::kPercentVbus), drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus),	// these must be initialized in the same order
		stick(STICK_1_PORT)		// as they are declared above.
	{
		ds = DriverStationLCD::GetInstance();
		drive1.EnableControl(0);
				drive2.EnableControl(0);
				drive3.EnableControl(0);
				drive4.EnableControl(0);
		SetSpeedReference();
		run = 0;
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
			if (stick.GetRawButton(1))
				run = 0;
			else if(stick.GetRawButton(2))
				run = 1;
			else if(stick.GetRawButton(3))
				run = 2;
			else if(stick.GetRawButton(4))
				run = 3;
			
			switch(run) {
			case 0:
				drive1.Set(-1.0);
				drive2.Set(-1.0);
				drive3.Set(1.0);
				drive4.Set(1.0);
				break;
			case 1:
				drive1.Set(-0.75);
				drive2.Set(-0.75);
				drive3.Set(0.75);
				drive4.Set(0.75);
				break;
			case 2:
				drive1.Set(-0.5);
				drive2.Set(-0.5);
				drive3.Set(0.5);
				drive4.Set(0.5);
				break;
			case 3:
				drive1.Set(-0.25);
				drive2.Set(-0.25);
				drive3.Set(0.25);
				drive4.Set(0.25);
				break;
			}
			UpdateJag_pV_Val();
		}
	}
	
	void UpdateJag_pV_Val() {
						char f1[50], f2[50], r1[50], r2[50];
						sprintf(f1, "%f", drive1.GetSpeed());
						sprintf(r1, "%f", drive3.GetSpeed());
						ds->Clear();
						ds->PrintfLine(DriverStationLCD::kUser_Line1, f1);
						ds->PrintfLine(DriverStationLCD::kUser_Line2, f2);
						ds->PrintfLine(DriverStationLCD::kUser_Line3, r1);
						ds->PrintfLine(DriverStationLCD::kUser_Line4, r2);
						ds->UpdateLCD();
					}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
	void SetSpeedReference() {
				drive1.ConfigEncoderCodesPerRev(ENCODER_REVS);
				drive3.ConfigEncoderCodesPerRev(ENCODER_REVS);
				drive1.SetSpeedReference(drive1.kSpeedRef_QuadEncoder);
				drive3.SetSpeedReference(drive3.kSpeedRef_QuadEncoder);
			}
};

START_ROBOT_CLASS(RobotDemo);

