#include "WPILib.h"
#include "Definitions.h"
#include "evom/StateMachine.h"
#include "evom/TKOShooter.h"

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
	Ultrasonic u1;
	float kP, kI, kD;
	instance_data_t _data;
	state_t _cur_state;
	StateMachine s;
	bool _autonFired;

public:
	RobotDemo(void):
		l_f(DRIVE_L1_ID, CANJaguar::kSpeed), l_b(DRIVE_L2_ID, CANJaguar::kPercentVbus),
		r_f(DRIVE_R1_ID, CANJaguar::kSpeed), r_b(DRIVE_R2_ID, CANJaguar::kPercentVbus),
		stick1(STICK_1_PORT), stick2(STICK_2_PORT),
		stick3(STICK_3_PORT), stick4(STICK_4_PORT),// as they are declared above.
		u1(2, 1, 2, 2)
	{
		DisableSafety();
		SetSpeedReference();
		kP = 75.0;
		kI = 0;
		kD = 0.0;
		SetPID();
		_cur_state = TKOShooter::inst()->initStateMachine(&_data);
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
		l_f.EnableControl();
		r_f.EnableControl();
		while (IsOperatorControl()&&IsEnabled())
		{	
			if (stick1.GetTrigger()) {
				if (((l_f.GetSpeed()<=119.334&&r_f.GetSpeed()<=119.334) && (u1.GetRangeInches()>=18 && u1.GetRangeInches()<=36))|| 
				    (((l_f.GetSpeed()<=MAX_RPM_1&&l_f.GetSpeed()>((u1.GetRangeInches()-36)*(1.0/18))&&
				(r_f.GetSpeed()<=MAX_RPM_1&&r_f.GetSpeed()>((u1.GetRangeInches()-36)*(1.0/18))))
				&&(u1.GetRangeInches()>36&& u1.GetRangeInches()<=72)))) {
						while(DriverStation::GetInstance()->IsOperatorControl() and DriverStation::GetInstance()->IsEnabled())
						{
							string state = s.state_to_string(&_data);
							if (state == "Ready to Fire" and StateMachine::armCanMove)
								s.forceFire = true;

							if (state == "ERROR STATE!!!") {
								break;
							}
							_cur_state = s.run_state(_cur_state,&_data);
							if (s.autonFired)
							{
								_autonFired = true;
								break;
							}
						}
				}
				
				else {
					if (u1.GetRangeInches()<18) {
						l_f.Set(0.0);
						l_b.Set(0.0);
						r_f.Set(0.0);
						r_b.Set(0.0);
					}
					else if(((l_f.GetSpeed()<((u1.GetRangeInches()-36)*(1.0/18)))
							||r_f.GetSpeed()<((u1.GetRangeInches()-36)*(1.0/18)))&&(u1.GetRangeInches()>36)) {
								l_f.Set(MAX_RPM_1*0.75);
								l_b.Set(l_f.GetOutputVoltage()/l_f.GetBusVoltage());
								r_f.Set(MAX_RPM_1*0.75);
								r_b.Set(r_f.GetOutputVoltage()/r_f.GetBusVoltage());
							}
					else {
						l_f.Set(MAX_RPM_1*0.25);
						l_b.Set(l_f.GetOutputVoltage()/l_f.GetBusVoltage());
						r_f.Set(MAX_RPM_1*0.25);
						r_b.Set(r_f.GetOutputVoltage()/r_f.GetBusVoltage());
					}
				}
				
				
			}
			
			else {
				TankDrive();
			}
		}
	}
	
	void TankDrive() {
			l_f.Set(kMAX_DRIVE_RPM*stick1.GetY());
			r_f.Set(kMAX_DRIVE_RPM*stick2.GetY());
			r_b.Set(r_f.GetOutputVoltage() / r_f.GetBusVoltage());
			l_b.Set(l_f.GetOutputVoltage() / l_f.GetBusVoltage());
					
			//char lf[50], lb[50], rf[50], rb[50];
			
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

