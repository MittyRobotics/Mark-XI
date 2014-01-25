#include "WPILib.h"
#include "Definitions.h"
#include "TKORelay.h"

//Shifter code was made by Babu

class ShifterDemo : public SimpleRobot
{

public:
	CANJaguar l_f, l_b, r_f, r_b;
	Joystick stick1, stick2;
	DoubleSolenoid s1, s2;
	Compressor comp;
	bool a_shift;
	ShifterDemo():
		l_f(DRIVE_L1_ID, CANJaguar::kPercentVbus),
		l_b(DRIVE_L2_ID, CANJaguar::kPercentVbus),
		r_f(DRIVE_R1_ID, CANJaguar::kPercentVbus),
		r_b(DRIVE_R2_ID, CANJaguar::kPercentVbus),
		stick1(STICK_1_PORT), stick2(STICK_2_PORT),
		s1(2,1,2), s2(2,3,4),
		comp(PRESSURE_SWITCH_PORT, COMPRESSOR_ID)
	{
		l_f.SetSafetyEnabled(false);
		l_b.SetSafetyEnabled(false);
		r_f.SetSafetyEnabled(false);
		r_b.SetSafetyEnabled(false);
		SetSpeedReference();
		a_shift = false;
	}

	void Autonomous()
	{
		while (IsAutonomous()&&IsEnabled()) {
			/*
			 * 		SetPID();
			if (stick2.GetTrigger()) {
				Stop();
				break;
			}
			FullDrive();
			Wait(25);
			SlowestDrive();
			Wait(25);
			Stop();
			*/
		}
	}
	
	void RobotInit()
	{
		comp.Start();
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
		l_f.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
		l_b.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
		r_f.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
		r_b.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	}
	
	void SetSpeedReference() {
		l_f.SetSpeedReference(JAG_SPEEDREF);
		l_b.SetSpeedReference(JAG_SPEEDREF);
		r_f.SetSpeedReference(JAG_SPEEDREF);
		r_b.SetSpeedReference(JAG_SPEEDREF);
	}
	
	void AutoShift() {
		if (fabs(l_f.GetSpeed()) > MAX_RPM_1
			&& fabs(r_f.GetSpeed()) > MAX_RPM_1) {
			shiftToHighGear();
		}
		else if(fabs(l_f.GetSpeed()) < MIN_RPM_1
		   || fabs(r_f.GetSpeed()) < MIN_RPM_1) {
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
			if (s1.Get() == s1.kReverse)
				s1.Set(s1.kForward);
			else
				s1.Set(s1.kReverse);
			if (s2.Get() == s2.kReverse)
				s2.Set(s2.kForward);
			else
				s2.Set(s2.kReverse);
		}
		if (stick1.GetTrigger()) {
			s1.Set(s1.kOff);
			s2.Set(s2.kOff);
		}
		//Wait(1); ///TODO TEMP fix, need so holding down button wont toggle back and forth
		
	}
	
	void FullDrive() {
		l_f.Set(-1.0);
		l_b.Set(-1.0);
		r_f.Set(1.0);
		r_b.Set(1.0);
		AutoShift();
	}
	
	void SlowestDrive() {
		l_f.Set(-0.35);
		l_b.Set(-0.35);
		r_f.Set(0.35);
		r_b.Set(0.35);
		AutoShift();
	}
	
	void Stop() {
		shiftToLowGear();
		Wait(0.5);
		l_f.Set(0);
		l_b.Set(0);
		r_f.Set(0);
		r_b.Set(0);
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
		if(stick1.GetY()>0.0||stick2.GetY()>0.0) {
					l_f.Set(-stick1.GetY() * 0.9);
					r_f.Set(stick2.GetY() * 0.9);
					l_b.Set(-stick1.GetY() * 0.9);
					r_b.Set(stick2.GetY() * 0.9);
					ShiftControl();
				}
				else {
					l_b.Set(-stick1.GetY() * 0.9);
					r_b.Set(stick2.GetY() * 0.9);
					l_f.Set(-stick1.GetY() * 0.9);
					r_f.Set(stick2.GetY() * 0.9);
					ShiftControl();
				}
	}
};

START_ROBOT_CLASS(ShifterDemo);

