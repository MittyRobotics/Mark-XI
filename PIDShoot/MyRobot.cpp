#include "WPILib.h"
#include "Definitions.h"
#include "evom/StateMachine.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	CANJaguar l_f, l_b, r_f, r_b;
	Joystick stick1, stick2, stick3, stick4;
	StateMachine* shoot;
	float kP, kI, kD;
	bool set;

public:
	RobotDemo(void):
		l_f(DRIVE_L1_ID), l_b(DRIVE_L2_ID), r_f(DRIVE_R1_ID), r_b(DRIVE_R2_ID),
		stick1(STICK_1_PORT), stick2(STICK_2_PORT),
		stick3(STICK_3_PORT), stick4(STICK_4_PORT),// as they are declared above.
		shoot()
	{
		DisableSafety();
		SetSpeedReference();
		kP = 75.0;
		kI = 0;
		kD = 0.0;
		SetPID();
		set = false;
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		while (IsOperatorControl()&&IsEnabled())
		{
			if (stick1.GetTrigger()) {
				set = true;
			}
			
			if (set) {
				if () {
					shoot = StateMachine::GetInstance();
				}
				
			}
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

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
			l_f.EnableControl();
			r_f.EnableControl();
		}
		void DisableSafety() {
				l_f.SetSafetyEnabled(false);
				l_b.SetSafetyEnabled(false);
				r_f.SetSafetyEnabled(false);
				r_b.SetSafetyEnabled(false);
			}
};

START_ROBOT_CLASS(RobotDemo);

