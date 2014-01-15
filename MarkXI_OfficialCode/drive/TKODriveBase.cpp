//Last edited by Vadim Korolik
//on 11/27/2013
#include "TKODriveBase.h"

TKODriveBase::TKODriveBase() :
	drive1(DRIVE_L1_ID, CANJaguar::kPercentVbus), // initialize motor 1 < first left drive motor
	drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus), // initialize motor 2 < second left drive motor
	drive3(DRIVE_R1_ID, CANJaguar::kPercentVbus), // initialize motor 3 < first right drive motor
	drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus), // initialize motor 4 < second right drive motor
	stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
	stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
	stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
	stick4(STICK_4_PORT) // initialize joystick 4 < first EVOM joystick-m,
{	
	printf("Initializing drive base\n");
	drive1.SetSafetyEnabled(true);
	drive2.SetSafetyEnabled(true);
	drive3.SetSafetyEnabled(true);
	drive4.SetSafetyEnabled(true);
	drive1.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
	drive2.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);   
	drive3.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
	drive4.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
	drive1.SetVoltageRampRate(0.0);
	drive2.SetVoltageRampRate(0.0);
	drive3.SetVoltageRampRate(0.0);
	drive4.SetVoltageRampRate(0.0);
	printf("Finished initializing drive base\n");
}
void TKODriveBase::Start()
{
	if (not driveTask->Verify() or driveTask->IsSuspended())
		driveTask->Start();
}
void TKODriveBase::Stop()
{
	if (driveTask->Verify() or not driveTask->IsSuspended())
		driveTask->Stop();
}

void TKODriveBase::TankDrive()
{
	if (stick2.GetRawButton(4))
	{
		drive1.SetVoltageRampRate(0.0);
		drive2.SetVoltageRampRate(0.0);
		drive3.SetVoltageRampRate(0.0);
		drive4.SetVoltageRampRate(0.0);
	}
	else if (stick2.GetRawButton(5))
	{
		drive1.SetVoltageRampRate(12.0);
		drive2.SetVoltageRampRate(12.0);
		drive3.SetVoltageRampRate(12.0);
		drive4.SetVoltageRampRate(12.0);
	}
	else if (stick2.GetRawButton(2))
	{
//		log->addMessage("DRIVE: Switched to Coast Mode.");
		drive1.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
		drive2.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);   
		drive3.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
		drive4.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
	}
	else if (stick2.GetRawButton(3))
	{
//		log->addMessage("DRIVE: Switched to Brake Mode.");
		drive1.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);  
		drive2.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);   
		drive3.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		drive4.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	}
	else if (stick1.GetTrigger())
	{
		drive1.Set(-stick1.GetY() * 0.4);
		drive2.Set(-stick1.GetY() * 0.4);
		drive3.Set(stick2.GetY() * 0.4);
		drive4.Set(stick2.GetY() * 0.4);
	}
	else if (stick1.GetRawButton(2))
	{
		drive1.Set(-stick1.GetY() * 0.2);
		drive2.Set(-stick1.GetY() * 0.2);
		drive3.Set(stick2.GetY() * 0.2);
		drive4.Set(stick2.GetY() * 0.2);
	}
	else if (stick1.GetRawButton(4))
	{
		drive1.Set(-stick1.GetY());
		drive2.Set(-stick1.GetY());
		drive3.Set(stick2.GetY());
		drive4.Set(stick2.GetY());
	}
	else
	{
		drive1.Set(-stick1.GetY() * 0.8);
		drive2.Set(-stick1.GetY() * 0.8);
		drive3.Set(stick2.GetY() * 0.8);
		drive4.Set(stick2.GetY() * 0.8);
	}
}
bool TKODriveBase::VerifyJags() //if returns false, jag problems
{
	if (drive1.IsAlive() and not drive1.StatusIsFatal())
	{
		if (drive2.IsAlive() and not drive2.StatusIsFatal())
		{
			if (drive3.IsAlive() and not drive3.StatusIsFatal())
			{
				if (drive4.IsAlive() and not drive4.StatusIsFatal())
				{
					return true;
				}
			}
		}
	}
	TKOLogger::inst()->addMessage("DRIVE JAGUAR FAILURE");
	printf("DRIVE JAGUARS DID NOT VERIFY\n");
	return false;
}
void TKODriveBase::switchToSpeed()
{
	drive1.ChangeControlMode(CANJaguar::kSpeed);
	drive3.ChangeControlMode(CANJaguar::kSpeed);
	drive1.SetSpeedReference(JAG_SPEEDREF);
	drive3.SetSpeedReference(JAG_SPEEDREF);
	drive1.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	drive3.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	drive1.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);  
	drive2.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);   
	drive3.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive4.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive1.EnableControl();
	drive3.EnableControl();
	//DONT FORGET TO USE SPEED MODE CORRECTLY
	//when driving use drive1.Set(SOME RPM); then drive2.Set(drive1.GetOutputVoltage());
}

TKODriveBase::~TKODriveBase()
{
	driveTask->Stop();
}

