#include "WPILib.h"
#include "Definitions.h"
#include <sstream>

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methodsl at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	CANJaguar l_f, l_b, r_f, r_b;
	Joystick stick1, stick2, stick3; // only joystick
	DriverStationLCD* dsl;
	DriverStation* ds;
	bool usePID;
	float kP, kI, kD;

public:
	RobotDemo():
		l_f(DRIVE_L1_ID, CANJaguar::kSpeed), l_b(DRIVE_L2_ID, CANJaguar::kPercentVbus), r_f(DRIVE_R1_ID, CANJaguar::kSpeed), r_b(DRIVE_R2_ID, CANJaguar::kPercentVbus),
		stick1(STICK_1_PORT), stick2(STICK_2_PORT), stick3(STICK_3_PORT), dsl(), ds()		// as they are declared above.
	{
		l_f.EnableControl(0);
		r_f.EnableControl(0);
		dsl = DriverStationLCD::GetInstance();
		ds = DriverStation::GetInstance();
		SetPID();
		DisableSafety();
		SetSpeedReference();
		usePID = true;
		kP = 0.1;
		kI = 0.002;
		kD = 0.0;
	}

	/**
	 * Drive left & right motors for 2 secondsl then stop
	 */
	void Autonomous()
	{

	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(){
		
		dsl->PrintfLine(DriverStationLCD::kUser_Line1,"HI");
		dsl->UpdateLCD();
		
		while (IsOperatorControl()&&IsEnabled())
		{
			EnablePIDControl();
			/*
			if (stick1.GetRawButton(3))
				usePID = !usePID;
				*/
			while(kP != ds->GetAnalogIn(1)*0.1) {
				kP = ds->GetAnalogIn(1)*0.1;
				//PID_Station();
				TankDrive();
			}
			while(kI != ds->GetAnalogIn(2)*0.0008) {
				kI = ds->GetAnalogIn(2)*0.0008;
				//PID_Station();
				TankDrive();
			}
			while(kD != ds->GetAnalogIn(3)*0.0004) {
				kD = ds->GetAnalogIn(3)*0.0004;
				//PID_Station();
				TankDrive();
			}
			TankDrive();
		}
	}
	
	void TankDrive() {
			SetPID();
			if (l_f.GetControlMode() == CANJaguar::kPercentVbus)
				l_f.ChangeControlMode(CANJaguar::kSpeed);
			
			/*
			if (l_b.GetControlMode() == CANJaguar::kPercentVbus)
				l_b.ChangeControlMode(CANJaguar::kSpeed);
				*/
			
			if (r_f.GetControlMode() == CANJaguar::kPercentVbus)
				r_f.ChangeControlMode(CANJaguar::kSpeed);
			
			/*
			if (r_b.GetControlMode() == CANJaguar::kPercentVbus)
				r_b.ChangeControlMode(CANJaguar::kSpeed);
				*/
			
			//EnablePIDControl();
			l_f.Set(-stick1.GetY()*max_speed);
			r_f.Set(stick2.GetY()*max_speed);
			l_b.Set(l_f.Get());
			r_b.Set(r_f.Get());
			float speed1 = l_f.GetSpeed();
			float speed2 = r_f.GetSpeed();
			dsl->Clear();
			char v1[50], v2[50];
			sprintf(v1, "%f", speed1);
			sprintf(v2, "%f", speed2);
			dsl->PrintfLine(DriverStationLCD::kUser_Line1, "Left Speed:");
			dsl->PrintfLine(DriverStationLCD::kUser_Line2, v1);
			dsl->PrintfLine(DriverStationLCD::kUser_Line3, "Right Speed:");
			dsl->PrintfLine(DriverStationLCD::kUser_Line4, v2);
			l_f.EnableControl(0);
			r_f.EnableControl(0);
		/*
		
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
		*/
	}
	
	void Test() {

	}
	
	void SetPID() {
			l_f.SetPID(kP, kI, kD);
			//l_b.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
			r_f.SetPID(kP, kI, kD);
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
		r_f.EnableControl();
	}
	
	void DisablePIDControl() {
		l_f.DisableControl();
		r_f.DisableControl();
	}
	
	void DisableSafety() {
		l_f.SetSafetyEnabled(false);
		l_b.SetSafetyEnabled(false);
		r_f.SetSafetyEnabled(false);
		r_b.SetSafetyEnabled(false);
	}
	
	void PID_Station() {
		dsl->Clear();
		char pv[50], iv[50], dv[50];
		sprintf(pv, "%f", kP);
		sprintf(iv, "%f", kI);
		sprintf(dv, "%f", kD);
		dsl->PrintfLine(DriverStationLCD::kUser_Line1, "P(3):");
		dsl->PrintfLine(DriverStationLCD::kUser_Line2, pv);
		dsl->PrintfLine(DriverStationLCD::kUser_Line3, "I(1):");
		dsl->PrintfLine(DriverStationLCD::kUser_Line4, iv);
		dsl->PrintfLine(DriverStationLCD::kUser_Line5, "D(2):");
		dsl->PrintfLine(DriverStationLCD::kUser_Line6, dv);
		dsl->UpdateLCD();
	}
};

START_ROBOT_CLASS(RobotDemo);

