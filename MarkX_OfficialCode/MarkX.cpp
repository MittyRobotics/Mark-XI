//Last edited by Vadim Korolik
//on 02/18/2013
#include "Definitions.h"
#include "TKOAutonomous.h"
#include "TKOClimber.h"
#include "TKORelay.h"

/*---------------MarkX-Thing-to-Do(TODO)---------------------*
 * Implement logger!!!
 * Test implementing a global timer/using system time to timestamp logger messages
 * Add system that realizes when robot hits a wall
 */

class MarkX: public SimpleRobot
{
	public:
		int burnoutIndexLeft;
		int burnoutIndexRight;
		float kDRIVE_STRAIGHT, kDRIVE_ROTATION;
		float left_joystick_y, right_joystick_y;
		float final_velocity_left, final_velocity_right;
		bool controllerDrive;
		CANJaguar drive1, drive2, drive3, drive4; // define motors
		Joystick stick1, stick2, stick3, stick4; // define joysticks
		DriverStation *ds; // define driver station object
		AnalogChannel sonar1, sonar2;
		TKOAutonomous auton;
		TKOClimber climber;
		PWM cameraServo;
		Compressor comp;

		Timer jukeTimer, globalOCTimer;
		void Disabled();
		void Autonomous();
		void OperatorControl();
		void Operator();
		void TKODrive();
		void JukeL();
		void JukeR();
		void Test();
		MarkX() :
			drive1(DRIVE_L1_ID, CANJaguar::kSpeed), // initialize motor 1 < first left drive motor
			        drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus), // initialize motor 2 < second left drive motor
			        drive3(DRIVE_R1_ID, CANJaguar::kSpeed), // initialize motor 3 < first right drive motor
			        drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus), // initialize motor 4 < second right drive motor

			        stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
			        stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
			        stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
			        stick4(STICK_4_PORT), // initialize joystick 4 < first EVOM joystick-m,

			        sonar1(3), sonar2(4), auton(DRIVE_L1_ID, DRIVE_L2_ID, DRIVE_R1_ID, DRIVE_R2_ID),

			        climber(WINCH_1_PORT, WINCH_2_PORT), cameraServo(CAMERA_SERVO_PORT), comp(PRESSURE_SWITCH_PORT, COMPRESSOR_ID)
		{
			ds = DriverStation::GetInstance(); // Pulls driver station information
			drive1.EnableControl(); //critical for these jags because they are in speed mode
			drive1.SetSafetyEnabled(false);
			drive2.SetSafetyEnabled(false);
			drive3.EnableControl(); //critical for these jags because they are in speed mode
			drive3.SetSafetyEnabled(false);
			drive4.SetSafetyEnabled(false);
			drive1.SetSpeedReference(JAG_SPEEDREF);
			drive1.ConfigEncoderCodesPerRev(ENCODER_REVS);
			drive1.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
			drive3.SetSpeedReference(JAG_SPEEDREF);
			drive3.ConfigEncoderCodesPerRev(ENCODER_REVS);
			drive3.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
			auton.rsFrontLoaderLift.SetOn(0);
			auton.rsFrontLoaderWrist.SetOn(0);
			controllerDrive = false;
			comp.Start();
			printf("Initialized the MarkX class \n");
		}

};

void MarkX::Test()
{
	printf("Calling test function \n");
	climber.Test();

	return;
	while (IsEnabled())
	{
		/*DSLog(2, "LStick %f", stick1.GetY());
		 DSLog(3, "RStick %f", stick2.GetY());
		 DSLog(4, "LPos %f", drive1.GetPosition());
		 DSLog(5, "RPos %f", drive3.GetPosition());
		 drive1.Set(kMAX_DRIVE_RPM);
		 drive3.Set(kMAX_DRIVE_RPM);*/

		if (stick3.GetRawButton(3))
			auton.rsFrontLoaderLift.SetOn(1);
		else if (stick3.GetRawButton(2))
			auton.rsFrontLoaderLift.SetOn(0);

		if (stick3.GetRawButton(5))
			auton.rsFrontLoaderWrist.SetOn(1);
		else if (stick3.GetRawButton(4))
			auton.rsFrontLoaderWrist.SetOn(0);

	}
}
//! Notifies driver if robot is disabled. Prints "Robot Died!" to console if it is disabled
/*!
 */
void MarkX::Disabled()
{
	std::ofstream logFile;
	logFile.open("log.txt", std::ofstream::trunc);

	//this part in robot disabled
	int i = 0;
	for (; i <= indx; i++)
	{
		switch (message[i])
		{
			case 2:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "Average Drive Current" << endl;
					logFile << "Data: " << data[i] << endl;
				}
				break;

			case 52:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "State 2" << endl;
					//					switch(data[i]){
					//                        case 1.0:
					//                            logFile << "SUCCESS: Setpoint reached, retracting ratchet"<< endl; break;
					//                        case 2.0:
					//                            logFile << "SHIT: Your clip limit switches were triggered while they were retracted. Broken switch"<< endl; break;
					//                        case 3.0:
					//                            logFile << "SHIT: One of your hooks jumped off"<< endl; break;
					//                        case 4.0:
					//                            logFile << "SHIT: Your hooks reached the bottom, and its too late to remove ratchet"<< endl; break;
					//                        case 5.0:
					//                            logFile << "UH OH; Ratchet was disengaged. Re-engaging ratchet"<< endl; break;
					//                        case 5.1:
					//                            logFile << "SHIT: Ratchet did not engage in half a second"<< endl; break;
					//                        case 6.0:
					//                            logFile << "DECIDE: Your hooks took too long to reach the bottom"<< endl; break;
					//                    }
				}
				break;

			case 53:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "State 2" << endl;
					//					switch(data[i]){
					//                        case 1.0:
					//                            logFile << "SUCCESS: Ratchet retracted, continuing to move down"<< endl; break;
					//                        case 2.0:
					//                            logFile << "SHIT: One of the hooks came off"<< endl; break;
					//                        case 3.0:
					//                            logFile << "SHIT: One of the clips sensed the bar; broken limit switch"<< endl; break;
					//                        case 4.0:
					//                            logFile << "SHIT: Top switch is broken"<< endl; break;
					//                        case 5.0:
					//                            logFile << "SHIT; Hit the bottom of the bar"<< endl; break;
					//                        case 6.0:
					//                            logFile << "SHIT: Timeout error"<< endl; break;
					//                    }
				}
				break;

			case 54:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "State 2" << endl;
					//					switch(data[i]){
					//                        case 1.0:
					//                            logFile << "SUCCESS: Setpoint reached, deploying clips"<< endl; break;
					//                        case 2.0:
					//                            logFile << "SHIT: Timeout error"<< endl; break;
					//                        case 3.0:
					//                            logFile << "SHIT: One of the hooks came off"<< endl; break;
					//                        case 4.0:
					//                            logFile << "SHIT: One of the clips sensed the bar. Broken switch"<< endl; break;
					//                        case 5.0:
					//                            logFile << "SHIT; Hit the bottom of the bar"<< endl; break;
					//                        case 6.0:
					//                            logFile << "REWIND: Ratchet was re-engaged. Went back a state and try to retract ratchet again"<< endl; break;
					//                    }
				}
				break;

			case 55:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "State 2" << endl;
					//					switch(data[i]){
					//                        case 1.0:
					//                            logFile << "SUCCESS: Clips deployed, moving hooks up"<< endl; break;
					//                        case 2.0:
					//                            logFile << "SHIT: Timeout error"<< endl; break;
					//                        case 3.0:
					//                            logFile << "SHIT: One of the hooks came off"<< endl; break;
					//                        case 4.0:
					//                            logFile << "SHIT: Hit bottom or top of bar"<< endl; break;
					//                        case 5.0:
					//                            logFile << "UH OH: Ratchet was re-engaged, attempting to retract"<< endl; break;
					//                        case 5.1:
					//                            logFile << "WHEW: Ratchet was retracted successfully, moving on"<< endl; break;
					//                    }
				}
				break;

			case 56:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "State 2" << endl;
					//					switch(data[i]){
					//                        case 1.0:
					//                            logFile << "SUCCESS: Hooks reached top, arm moving forward"<< endl; break;
					//                        case 2.0:
					//                            logFile << "SHIT: Timeout error"<< endl; break;
					//                        case 3.0:
					//                            logFile << "SHIT: One of the clips came off"<< endl; break;
					//                        case 4.0:
					//                            logFile << "SHIT: Hit bottom or top of bar"<< endl; break;
					//                        case 5.0:
					//                            logFile << "UH OH: Ratchet was re-engaged, attempting to retract"<< endl; break;
					//                        case 5.1:
					//                            logFile << "WHEW: Ratchet was retracted successfully, moving on"<< endl; break;
					//                    }
				}
				break;

			case 57:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "State 2" << endl;
					//					switch(data[i]){
					//                        case 1.0:
					//                            logFile << "SUCCESS: Arm was (hopefully) moved forward (No switch to detect it). Engaging ratchet"<< endl; break;
					//                        case 2.0:
					//                            logFile << "SHIT: Clips disengaged"<< endl; break;
					//                        case 4.0:
					//                            logFile << "SHIT: Hit bottom or top of bar"<< endl; break;
					//                        case 5.0:
					//                            logFile << "UH OH: Ratchet was re-engaged, attempting to retract"<< endl; break;
					//                        case 5.1:
					//                            logFile << "WHEW: Ratchet was retracted successfully, moving on"<< endl; break;
					//                    }
				}
				break;

			case 58:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "State 2" << endl;
					//					switch(data[i]){
					//                        case 1.0:
					//                            logFile << "SUCCESS: Ratchet successfully engaged, moving hooks down"<< endl; break;
					//                        case 2.0:
					//                            logFile << "SHIT: Timeout error"<< endl; break;
					//                        case 3.0:
					//                            logFile << "SHIT: One of the hooks is engaged, probably a broken limit switch"<< endl; break;
					//                        case 4.0:
					//                            logFile << "SHIT: One of the clips disengaged"<< endl; break;
					//                        case 5.0:
					//                            logFile << "SHIT: You hit either the top or bottom of the lift"<< endl; break;
					//                        case 6.0:
					//                            logFile << "SHIT: You hit either the top or bottom of the lift"<< endl; break;
					//                    }
				}
				break;

			case 59:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "State 2" << endl;
					//					switch(data[i]){
					//                        case 1.0:
					//                            logFile << "SUCCESS: Both hooks clipped on, retracting clips"<< endl; break;
					//                        case 2.0:
					//                            logFile << "SHIT: Timeout error"<< endl; break;
					//                        case 3.0:
					//                            logFile << "UH OH: Only one of your hooks engaged. Continuing to move for a bit"<< endl; break;
					//                        case 3.1:
					//                            logFile << "WHEW: Got both hooks on. Moving on "<< endl; break;
					//                        case 3.2:
					//                            logFile << "SHIT: Couldn't get both hooks on in a safe amount of time"<< endl; break;
					//                        case 4.0:
					//                            logFile << "SHIT: One of the clips disengaged"<< endl; break;
					//                        case 5.0:
					//                            logFile << "SHIT: You hit either the top or bottom of the lift"<< endl; break;
					//                        case 6.0:
					//                            logFile << "REWIND: Your hooks missed the bar completely, going back to state 6 (Move hooks up)"<< endl; break;
					//                    }
				}
				break;

			case 60:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "State 2" << endl;
					//					switch(data[i]){
					//                        case 1.0:
					//                            logFile << "LEVEL UP: Clips retracted, going back to state 2"<< endl; break;
					//                        case 1.1:
					//                            logFile << "VICTORY: We made it to the top. Moving on to dumping"<< endl; break;
					//                        case 2.0:
					//                            logFile << "SHIT: Timeout error"<< endl; break;
					//                        case 3.0:
					//                            logFile << "SHIT: One of your hooks came off"<< endl; break;
					//                        case 4.0:
					//                            logFile << "SHIT: You hit either the top or bottom of the lift"<< endl; break;
					//                        case 5.0:
					//                            logFile << "SHIT: Ratchet came out"<< endl; break;
					//                    }
				}
				break;

			case 62:
				if (logFile.is_open())
				{
					if (ds->IsOperatorControl())
						logFile << endl << "TELE Timer: " << globalOCTimer.Get() << endl;
					logFile << "State 2" << endl;
					//					switch(data[i]){
					//                        case 1.0:
					//                            logFile << "SUCCESS: We dumped the frisbees"<< endl; break;
					//                    }
				}
				break;

			default:
				//do nothing
				break;
		}

		logFile.close();
		printf("Robot Died!");
	}
}

//! Autonomous code
/*!
 */
void MarkX::Autonomous(void) //Choose autonomous mode between 4 options though DS
{
	printf("Starting Autonomous \n");
	auton.initAutonomous();
	auton.setDrivePID(DRIVE_kP, DRIVE_kP, DRIVE_kI);
	//	auton.setDriveTargetStraight(ds->GetAnalogIn(1) * 10 * REVS_PER_METER);
	auton.startAutonomous();

	while (auton.autonTimer.Get() < 15 && auton.runningAuton)
	{
		if (!ds->IsAutonomous())
			auton.stopAutonomous();

		//auton.autonomousCode();
		if (ds->GetDigitalIn(1))
			auton.autonSetup1();
		else if (ds->GetDigitalIn(2))
			auton.autonSetup2();
		else if (ds->GetDigitalIn(3))
			auton.autonSetup3();
		else
		{
			printf("Error, no starting setup selected");
			DSLog(1, "Error, no starting setup selected");
		}
	}
	auton.stopAutonomous();
	printf("Ending Autonomous \n");
}
//! Operator Control Initialize and runs the Operator Control loop
/*!
 Initializes drive motors, Prints number and location of balls and shooter's speed to DSLog
 */

void MarkX::OperatorControl()
{
	printf("Starting OperatorControl \n");
	Timer loopTimer;
	float startLoopT, endLoopT, loopTime;
	loopTimer.Start();
	globalOCTimer.Start();
	while (IsOperatorControl() && ds->IsEnabled())
	{
		startLoopT = loopTimer.Get();
		DSClear();

		MarkX::Operator();
		MarkX::TKODrive();
		DSLog(4, "Avr d1,3 cur: %f", ((drive1.GetOutputCurrent() + drive3.GetOutputCurrent()) / 2));
		writeMD(2, (drive1.GetOutputCurrent() + drive3.GetOutputCurrent()) / 2)

		endLoopT = loopTimer.Get();
		loopTime = endLoopT - startLoopT;
		printf("Operator Loop Time, excluding Wait: %f", endLoopT);
		space
		loopTimer.Reset();
	}
	globalOCTimer.Stop();
	printf("Ending OperatorControl \n");
}
//! Driving and EVOM code
/*!
 */
void MarkX::Operator()
{
	//TEST
	//	if (stick1.GetTrigger())
	//		return;
	//	if (stick1.GetRawButton(4))
	//		climber.ClipBack();
	//	if (stick1.GetRawButton(5))
	//		climber.ClipForward();
	//	if (stick1.GetRawButton(2))
	//		climber.ArmBack();
	//	if (stick1.GetRawButton(3))
	//		climber.ArmForward();
	//	if (stick1.GetRawButton(8))
	//		climber.RatchetBack();
	//	if (stick1.GetRawButton(9))
	//		climber.RatchetForward();
	//END OF TEST STATEMENT

	if (stick2.GetRawButton(6))
		controllerDrive = not controllerDrive;
	if (stick3.GetRawButton(3))
		auton.rsFrontLoaderLift.SetOn(1);
	else if (stick3.GetRawButton(2))
		auton.rsFrontLoaderLift.SetOn(0);

	if (stick3.GetRawButton(5))
		auton.rsFrontLoaderWrist.SetOn(1);
	else if (stick3.GetRawButton(4))
		auton.rsFrontLoaderWrist.SetOn(0);

	if (stick3.GetRawButton(10) and stick3.GetX() > 0)
		cameraServo.SetRaw(cameraServo.GetRaw() + CAMERA_PWM_INCREMENT);

	if (stick3.GetRawButton(10) and stick3.GetX() < 0)
		cameraServo.SetRaw(cameraServo.GetRaw() - CAMERA_PWM_INCREMENT);

	if (stick1.GetRawButton(10))
		JukeR();
	if (stick1.GetRawButton(7))
		JukeL();

	if (stick1.GetTrigger())
	{
		kDRIVE_STRAIGHT = 0.4;
		kDRIVE_ROTATION = 0.2;
	}
	else if (stick1.GetRawButton(2))
	{
		kDRIVE_STRAIGHT = 0.2;
	}
	else
	{
		kDRIVE_STRAIGHT = 1.2;
		kDRIVE_ROTATION = 0.7;
	}

	if (stick2.GetTrigger())
	{
		kDRIVE_STRAIGHT = -kDRIVE_STRAIGHT;
		kDRIVE_ROTATION = -kDRIVE_ROTATION;
	}
	else
	{
		kDRIVE_STRAIGHT = fabs(kDRIVE_STRAIGHT);
		kDRIVE_ROTATION = fabs(kDRIVE_ROTATION);
	}
	if (stick3.GetTrigger())
	{
		drive1.Set(kMAX_DRIVE_RPM);
		drive3.Set(kMAX_DRIVE_RPM);
	}
	if (stick4.GetTrigger())
	{
		drive2.Set(1);
		drive4.Set(1);
	}

	if (stick3.GetRawButton(4))
	{
		//TKOShooter.shoot(-stick3.GetY());
		DSLog(6, "Manual shoot pow: %f", -stick3.GetY());
	}
	if (stick4.GetRawButton(6))
	{
		//TKOShooter.autoShoot();
		DSLog(6, "Autoshoot");
	}
	if (stick3.GetRawButton(9) and stick4.GetRawButton(9))
	{
		climber.ArmForward();
		climber.Climb();
		DSLog(6, "Autoclimbing");
	}
}

void MarkX::TKODrive()
{
	printf("TKODriving...");
	if (not controllerDrive)
	{
		if (!stick2.GetTrigger())
		{
			left_joystick_y = stick1.GetY();
			right_joystick_y = stick2.GetY();
		}
		else
		{
			left_joystick_y = stick2.GetY();
			right_joystick_y = stick1.GetY();
		}
	}
	else
	{
		left_joystick_y = stick1.GetRawAxis(2);
		right_joystick_y = stick1.GetRawAxis(5);
	}
	if (fabs(left_joystick_y) > deadzone)
	{
		if ((left_joystick_y > deadzone and right_joystick_y < deadzone) or (right_joystick_y > deadzone and left_joystick_y < deadzone))
		{
			final_velocity_left = left_joystick_y * kMAX_DRIVE_RPM * kDRIVE_ROTATION;
		}
		else
		{
			final_velocity_left = left_joystick_y * kMAX_DRIVE_RPM * kDRIVE_STRAIGHT;
		}
	}

	else
	{
		final_velocity_left = 0;
	}
	if (fabs(right_joystick_y) > deadzone)
	{
		if ((left_joystick_y > deadzone and right_joystick_y < deadzone) or (right_joystick_y > deadzone and left_joystick_y < deadzone))
		{
			final_velocity_right = right_joystick_y * kMAX_DRIVE_RPM * kDRIVE_ROTATION;
		}
		else
		{
			final_velocity_right = right_joystick_y * kMAX_DRIVE_RPM * kDRIVE_STRAIGHT;
		}
	}
	else
	{
		final_velocity_right = 0;
	}
	if ((right_joystick_y >= 0 and left_joystick_y >= 0) or (right_joystick_y <= 0 and left_joystick_y <= 0))
	{
		if (fabs(fabs(left_joystick_y) - fabs(right_joystick_y)) < .1)
		{
			if (final_velocity_right < final_velocity_left)
			{
				final_velocity_left = final_velocity_right;
			}
			else
			{
				final_velocity_right = final_velocity_left;
			}
		}
	}

	if ((final_velocity_left - fabs(drive3.GetSpeed())) > kBURNOUT)
	{
		burnoutIndexLeft++;
	}
	else
	{
		burnoutIndexLeft = 0;
	}

	if ((final_velocity_right - fabs(drive1.GetSpeed())) > kBURNOUT)
	{
		burnoutIndexRight++;
	}
	else
	{
		burnoutIndexRight = 0;
	}

	if (burnoutIndexLeft > kBURNOUT_CYCLES)
	{
		final_velocity_left = 0;
		printf("Burnout Left");
		DSLog(5, "Burnout Left");
	}

	if (burnoutIndexRight > kBURNOUT_CYCLES)
	{
		final_velocity_right = 0;
		printf("Burnout Right");
		DSLog(5, "Burnout Right");
	}
	printf("Finished processing joystick inputs. \n");
	printf("Left velocity to set: %f", final_velocity_left);
	space
	printf("Right velocity to set: %f", final_velocity_right);
	space

	float speedLeft = drive1.GetSpeed() * 3.14159 * 6/*wheel size*// 12
	/*inches in feet*// 60;
	float speedRight = drive3.GetSpeed() * 3.14159 * 6 / 12 / 60;
	DSLog(1, "Speed F/s: %f", fabs(((speedLeft + speedRight) / 2)));
	DSLog(2, "Sonar 1 in: %f", sonar1.GetVoltage() * 100);
	DSLog(3, "Sonar 2 in: %f", sonar2.GetVoltage() * 100);

	// implement processing for left_joystick_x, left_joystick_y, right_joystick_x, and right_joystick_y to account for PID and other factors
	// then we pass these values to the SetLeftRightMotorsOutput() function of TKODrive
	drive1.Set(final_velocity_left);
	drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
	drive3.Set(final_velocity_right);
	drive4.Set(-drive3.GetOutputVoltage() / drive3.GetBusVoltage());

}
void MarkX::JukeR(void)
{
	printf("Juke manuever started\n");
	jukeTimer.Start();
	while (jukeTimer.Get() < 0.6 + 0.65 + 0.3 + 0.6 + 1 + 0.15 + 1.2 && IsEnabled())
	{
		if (jukeTimer.Get() < 0.6)
		{
			drive1.Set(-kMAX_DRIVE_RPM);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(-kMAX_DRIVE_RPM);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		else if (jukeTimer.Get() < 0.6 + 0.65)
		{
			drive1.Set(-kMAX_DRIVE_RPM);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		else if (jukeTimer.Get() < 0.6 + 0.65 + 0.3)
		{
			drive1.Set(-kMAX_DRIVE_RPM);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(-kMAX_DRIVE_RPM);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		else if (jukeTimer.Get() < 0.6 + 0.65 + 0.3 + 0.6)
		{
			drive1.Set(kMAX_DRIVE_RPM);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(kMAX_DRIVE_RPM);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		else if (jukeTimer.Get() < 0.6 + 0.65 + 0.3 + 0.6 + 1)
		{
			drive3.Set(kMAX_DRIVE_RPM);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		else if (jukeTimer.Get() < 0.6 + 0.65 + 0.3 + 0.6 + 1 + 0.15)
		{
			drive1.Set(kMAX_DRIVE_RPM);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(kMAX_DRIVE_RPM);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		else if (jukeTimer.Get() < 0.6 + 0.65 + 0.3 + 0.6 + 1 + 0.15 + 1.2)
		{
			drive1.Set(kMAX_DRIVE_RPM);
			drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
			drive3.Set(-kMAX_DRIVE_RPM);
			drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		}
		jukeTimer.Stop();
	}
}

void MarkX::JukeL(void)
{
	printf("Juke manuever started\n");
	jukeTimer.Start();
	jukeTimer.Reset();
	while (jukeTimer.Get() < .6)
	{
		drive1.Set(-kMAX_DRIVE_RPM);
		drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		drive3.Set(-kMAX_DRIVE_RPM);
		drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
	}
	jukeTimer.Reset();
	while (jukeTimer.Get() < .65)
	{
		drive3.Set(-kMAX_DRIVE_RPM);
		drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
	}
	jukeTimer.Reset();
	while (jukeTimer.Get() < .3)
	{
		drive1.Set(-kMAX_DRIVE_RPM);
		drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		drive3.Set(-kMAX_DRIVE_RPM);
		drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
	}
	jukeTimer.Reset();
	while (jukeTimer.Get() < .6)
	{
		drive1.Set(kMAX_DRIVE_RPM);
		drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		drive3.Set(kMAX_DRIVE_RPM);
		drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
	}
	jukeTimer.Reset();
	while (jukeTimer.Get() < 1)
	{
		drive1.Set(kMAX_DRIVE_RPM);
		drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
	}
	jukeTimer.Reset();
	while (jukeTimer.Get() < .15)
	{
		drive1.Set(kMAX_DRIVE_RPM);
		drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		drive3.Set(kMAX_DRIVE_RPM);
		drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
	}
	jukeTimer.Reset();
	while (jukeTimer.Get() < 1.2)
	{
		drive1.Set(kMAX_DRIVE_RPM);
		drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
		drive3.Set(-kMAX_DRIVE_RPM);
		drive4.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
	}
	jukeTimer.Reset();

	jukeTimer.Stop();
}

START_ROBOT_CLASS(MarkX)
;
