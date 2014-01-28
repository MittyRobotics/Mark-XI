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
	Joystick stick1, stick2; // only joystick
	bool usePID;

public:
	RobotDemo():
		l_f(DRIVE_L1_ID, CANJaguar::kSpeed), l_b(DRIVE_L2_ID, CANJaguar::kSpeed), r_f(DRIVE_R1_ID, CANJaguar::kSpeed), r_b(DRIVE_R2_ID, CANJaguar::kSpeed),
		stick1(STICK_1_PORT), stick2(STICK_2_PORT)		// as they are declared above.
	{
		DisableSafety();
		SetSpeedReference();
		usePID = true;
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
		
		SetPID();
		
		while (IsOperatorControl()&&IsEnabled())
		{
			if (stick1.GetRawButton(3))
				usePID = !usePID;
			TankDrive();
		}
	}
	
	void TankDrive() {
		if (usePID == true) {
			if (l_f.GetControlMode() == CANJaguar::kPercentVbus)
				l_f.ChangeControlMode(CANJaguar::kSpeed);
			
			if (l_b.GetControlMode() == CANJaguar::kPercentVbus)
				l_b.ChangeControlMode(CANJaguar::kSpeed);
			
			if (r_f.GetControlMode() == CANJaguar::kPercentVbus)
				r_f.ChangeControlMode(CANJaguar::kSpeed);
			
			if (r_b.GetControlMode() == CANJaguar::kPercentVbus)
				r_b.ChangeControlMode(CANJaguar::kSpeed);
			
			EnablePIDControl();
			
			l_f.Set(-stick1.GetY()*max_speed);
			r_f.Set(stick2.GetY()*max_speed);
			l_b.Set(l_f.GetSpeed());
			r_b.Set(r_b.GetSpeed());
		}
		
		else {
			if (l_f.GetControlMode() == CANJaguar::kSpeed)
				l_f.ChangeControlMode(CANJaguar::kPercentVbus);
			
			if (l_b.GetControlMode() == CANJaguar::kSpeed)
				l_b.ChangeControlMode(CANJaguar::kPercentVbus);
						
			if (r_f.GetControlMode() == CANJaguar::kSpeed)
				r_f.ChangeControlMode(CANJaguar::kPercentVbus);
			
			if (r_b.GetControlMode() == CANJaguar::kSpeed)
				r_b.ChangeControlMode(CANJaguar::kPercentVbus);
			
			DisablePIDControl();
			
			if(stick1.GetY()>0.0||stick2.GetY()>0.0) {
								l_f.Set(-stick1.GetY() * 0.9);
								r_f.Set(stick2.GetY() * 0.9);
								l_b.Set(-stick1.GetY() * 0.9);
								r_b.Set(stick2.GetY() * 0.9);
							}
							else {
								l_b.Set(-stick1.GetY() * 0.9);
								r_b.Set(stick2.GetY() * 0.9);
								l_f.Set(-stick1.GetY() * 0.9);
								r_f.Set(stick2.GetY() * 0.9);
							}
		}
	}
	
	void Test() {

	}
	
	void SetPID() {
			l_f.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
			//l_b.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
			r_f.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
			//r_b.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
		}
		
		void SetSpeedReference() {
			l_f.ConfigEncoderCodesPerRev(ENCODER_REVS);
			r_f.ConfigEncoderCodesPerRev(ENCODER_REVS);
			l_f.SetSpeedReference(JAG_SPEEDREF);
			r_f.SetSpeedReference(JAG_SPEEDREF);
		}
	void EnablePIDControl() {
		l_f.EnableControl();
		l_b.EnableControl();
		r_f.EnableControl();
		r_b.EnableControl();
	}
	
	void DisablePIDControl() {
		l_f.DisableControl();
		l_b.DisableControl();
		r_f.DisableControl();
		r_b.DisableControl();
	}
	
	void DisableSafety() {
		l_f.SetSafetyEnabled(false);
		l_b.SetSafetyEnabled(false);
		r_f.SetSafetyEnabled(false);
		r_b.SetSafetyEnabled(false);
	}
};

START_ROBOT_CLASS(RobotDemo);

