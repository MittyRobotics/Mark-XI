#include "WPILib.h"
#include "Definitions.h"
#include "TKORelay.h"

class ShifterDemo : public SimpleRobot
{
	CANJaguar drive1, drive2, drive3, drive4;
	Joystick stick1, stick2;
	Solenoid s1, s2;
	TKORelay comp;

public:
	ShifterDemo():
		drive1(DRIVE_L1_ID, CANJaguar::kPercentVbus),
		drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus),
		drive3(DRIVE_R1_ID, CANJaguar::kPercentVbus),
		drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus),
		stick1(STICK_1_PORT), stick2(STICK_2_PORT),
		s1(1), s2(2),
		comp(2)
	{
		drive1.SetSafetyEnabled(false);
		drive2.SetSafetyEnabled(false);
		drive3.SetSafetyEnabled(false);
		drive4.SetSafetyEnabled(false);
		SetReference();
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{
		SetPID();
		while (IsAutonomous()&&IsEnabled()) {
			if (stick1.GetRawButton(3))
				break;
			comp.SetOn(1);
			
		}
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		while (IsOperatorControl()&&IsEnabled())
		{
			
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
	
	void SetPID() {
		drive1.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
		drive2.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
		drive3.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
		drive4.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	}
	
	void SetReference() {
		drive1.SetSpeedReference(JAG_SPEEDREF);
		drive2.SetSpeedReference(JAG_SPEEDREF);
		drive3.SetSpeedReference(JAG_SPEEDREF);
		drive4.SetSpeedReference(JAG_SPEEDREF);
	}
	
	void AutoShift() {
		if (min(drive1.GetSpeed(),drive2.GetSpeed())>15 && min(drive3.GetSpeed(),drive4.GetSpeed())>15) {
			s1.Set(true);
			s2.Set(true);
		}
		if(max(drive1.GetSpeed(),drive2.GetSpeed())<15 || max(drive3.GetSpeed(),drive4.GetSpeed())<15) {
			s1.Set(false);
			s2.Set(false);
		}
	}
};

START_ROBOT_CLASS(ShifterDemo);

