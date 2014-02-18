#include "WPILib.h"
#include "Definitions.h"
#include "Gamepad.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	Gamepad g1;
	//Joystick stick1, stick2; // only joystick
	CANJaguar l_f, l_b, r_f, r_b;
	float kP, kI, kD;
	DriverStationLCD* dsl;
	
public:
	RobotDemo():
		g1(STICK_2_PORT),
		//stick1(STICK_1_PORT), stick2(STICK_2_PORT),		// as they are declared above.
		l_f(DRIVE_L1_ID, CANJaguar::kSpeed), l_b(DRIVE_L2_ID, CANJaguar::kPercentVbus),
		r_f(DRIVE_R1_ID, CANJaguar::kSpeed), r_b(DRIVE_R2_ID, CANJaguar::kPercentVbus),
		dsl()
		{
//		SetRamping(110);
		DisableSafety();
		SetSpeedReference();
		kP = 25.0;
		kI = 0;
		kD = 0.0;
		SetPID();
		dsl = DriverStationLCD::GetInstance();
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
		l_f.EnableControl();
		r_f.EnableControl();
//			l_b.EnableControl();
		
//				r_b.EnableControl();
		while (IsOperatorControl()&&IsEnabled())
		{
			
			TankDrive();
			printf("\nlfproto: %f", l_f.Get());
			printf( "\nlf: %f", l_f.GetSpeed());
			printf("\nrfproto: %f", r_f.Get()); 
			printf( "\nlb: %f", l_b.Get());
			printf("\nrf: %f", r_f.GetSpeed());
			printf("\nrb: %f", r_b.Get());
			//printf("\nstickl: %f", stick1.GetY());
			//printf("\nstickr: %f", stick2.GetY());
		}
	}
	
	void TankDrive() {
		//l_f.Set(g1.GetLeftY() * kMAX_DRIVE_RPM);
		r_f.Set(-g1.GetLeftY() * kMAX_DRIVE_RPM);
		//r_b.Set(r_f.GetOutputVoltage() / r_f.GetBusVoltage());
		//l_b.Set(l_f.GetOutputVoltage() / l_f.GetBusVoltage());
				
		//char lf[50], lb[50], rf[50], rb[50];
		
	}
	
	void Test() {

	}
	
	void SetRamping(double rate) {
		l_f.SetVoltageRampRate(rate);
		l_b.SetVoltageRampRate(rate);
		r_f.SetVoltageRampRate(rate);
		r_b.SetVoltageRampRate(rate);
	}
	
	
	void SetPID() {
		l_f.SetPID(kP, kI, kD);
//		l_b.SetPID(kP, kI, kD);
		r_f.SetPID(kP, kI, kD);
//		r_b.SetPID(kP, kI, kD);
	}
			
	void SetSpeedReference() {
		l_f.ConfigEncoderCodesPerRev(ENCODER_REVS);
		r_f.ConfigEncoderCodesPerRev(ENCODER_REVS);
		l_f.SetSpeedReference(l_f.kSpeedRef_QuadEncoder);
		r_f.SetSpeedReference(r_f.kSpeedRef_QuadEncoder);
	}
	void EnablePIDControl() {
//		l_f.EnableControl();
//		r_f.EnableControl();
	}
	void DisableSafety() {
			l_f.SetSafetyEnabled(false);
			l_b.SetSafetyEnabled(false);
			r_f.SetSafetyEnabled(false);
			r_b.SetSafetyEnabled(false);
		}
};

START_ROBOT_CLASS(RobotDemo);

