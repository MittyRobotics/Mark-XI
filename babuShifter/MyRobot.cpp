#include "WPILib.h"
#include "Definitions.h"
#include "TKORelay.h"

//Shifter code was made by Babu

class ShifterDemo : public SimpleRobot
{

public:
	CANJaguar drive1, drive2, drive3, drive4;
	Joystick stick1, stick2;
	DoubleSolenoid s1, s2;
	Compressor comp;
	bool a_shift;
	ShifterDemo():
		drive1(DRIVE_L1_ID, CANJaguar::kPercentVbus),
		drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus),
		drive3(DRIVE_R1_ID, CANJaguar::kPercentVbus),
		drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus),
		stick1(STICK_1_PORT), stick2(STICK_2_PORT),
		s1(2,1,2), s2(2,3,4),
		comp(PRESSURE_SWITCH_PORT, COMPRESSOR_ID)
	{
		drive1.SetSafetyEnabled(false);
		drive2.SetSafetyEnabled(false);
		drive3.SetSafetyEnabled(false);
		drive4.SetSafetyEnabled(false);
		SetSpeedReference();
		a_shift = true;
	}

	void Autonomous()
	{
		SetPID();
		while (IsAutonomous()&&IsEnabled()) {
			if (stick2.GetTrigger()) {
				Stop();
				break;
			}
			FullDrive();
			Wait(25);
			SlowestDrive();
			Wait(25);
			Stop();
		}
	}
	
	void RobotInit()
	{
		//comp.Start();
	}

	void OperatorControl()
	{
		while (IsOperatorControl()&&IsEnabled()) {
			if (stick2.GetTrigger()) {
				Stop();
				break;
			}
			if (stick2.GetRawButton(4))
				a_shift = !a_shift;
			TankDrive();
		}
	}
	
	void Test() {

	}
	
	void SetPID() {
		drive1.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
		drive2.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
		drive3.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
		drive4.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	}
	
	void SetSpeedReference() {
		drive1.SetSpeedReference(JAG_SPEEDREF);
		drive2.SetSpeedReference(JAG_SPEEDREF);
		drive3.SetSpeedReference(JAG_SPEEDREF);
		drive4.SetSpeedReference(JAG_SPEEDREF);
	}
	
	void AutoShift() {
		if (min(fabs(drive1.GetSpeed()),fabs(drive2.GetSpeed())) > MAX_RPM_1
			&& min(fabs(drive3.GetSpeed()),fabs(drive4.GetSpeed())) > MAX_RPM_1) {
			shiftToHighGear();
		}
		else if(max(fabs(drive1.GetSpeed()),fabs(drive2.GetSpeed())) < MIN_RPM_1
		   || max(fabs(drive3.GetSpeed()),fabs(drive4.GetSpeed())) < MIN_RPM_1) {
			shiftToLowGear();
		}
	}
	
	void shiftToHighGear() {
		s1.Set(s1.kForward);
		s2.Set(s2.kForward);
	}
	
	void shiftToLowGear() {
		s1.Set(s1.kReverse);
		s2.Set(s2.kReverse);
	}
	
	void ManualShift() {
		if (stick2.GetRawButton(3)) {
			if (s1.Get() == s1.kForward)
				s1.Set(s1.kReverse);
			else
				s1.Set(s1.kForward);
			if (s2.Get() == s2.kForward)
				s2.Set(s2.kReverse);
			else
				s2.Set(s2.kForward);
		}
	}
	
	void FullDrive() {
		drive1.Set(-1.0);
		drive2.Set(-1.0);
		drive3.Set(1.0);
		drive4.Set(1.0);
		AutoShift();
	}
	
	void SlowestDrive() {
		drive1.Set(-0.35);
		drive2.Set(-0.35);
		drive3.Set(0.35);
		drive4.Set(0.35);
		AutoShift();
	}
	
	void Stop() {
		shiftToLowGear();
		Wait(0.5);
		drive1.Set(0);
		drive2.Set(0);
		drive3.Set(0);
		drive4.Set(0);
	}
	
	void ShiftControl() {
		switch (a_shift) {
			case true:
				AutoShift();
				break;
			case false:
				ManualShift();
				break;
		}
	}
	
	void TankDrive() {
		if (stick1.GetTrigger())
			{
				drive1.Set(-stick1.GetY() * 0.4);
				drive2.Set(-stick1.GetY() * 0.4);
				drive3.Set(stick2.GetY() * 0.4);
				drive4.Set(stick2.GetY() * 0.4);
				ShiftControl();
			}
			else if (stick1.GetRawButton(2))
			{
				drive1.Set(-stick1.GetY() * 0.2);
				drive2.Set(-stick1.GetY() * 0.2);
				drive3.Set(stick2.GetY() * 0.2);
				drive4.Set(stick2.GetY() * 0.2);
				ShiftControl();
			}
			else if (stick1.GetRawButton(4))
			{
				drive1.Set(-stick1.GetY());
				drive2.Set(-stick1.GetY());
				drive3.Set(stick2.GetY());
				drive4.Set(stick2.GetY());
				ShiftControl();
			}
			else if (stick2.GetRawButton(2))
			{
				drive1.Set(-stick1.GetY());
				drive2.Set(-stick1.GetY());
				drive3.Set(stick1.GetY());
				drive4.Set(stick1.GetY());
				ShiftControl();
			}
			else
			{
				drive1.Set(-stick1.GetY() * 0.8);
				drive2.Set(-stick1.GetY() * 0.8);
				drive3.Set(stick2.GetY() * 0.8);
				drive4.Set(stick2.GetY() * 0.8);
				ShiftControl();
			}
	}
};

START_ROBOT_CLASS(ShifterDemo);

