//Last edited by Vadim Korolik
//on 11/27/2013
#include "TKODrive.h"

TKODrive* TKODrive::m_Instance = NULL;

TKODrive::TKODrive() :
	drive1(DRIVE_L1_ID, CANJaguar::kPercentVbus), // initialize motor 1 < first left drive motor
	drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus), // initialize motor 2 < second left drive motor
	drive3(DRIVE_R1_ID, CANJaguar::kPercentVbus), // initialize motor 3 < first right drive motor
	drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus), // initialize motor 4 < second right drive motor
	stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
	stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
	stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
	stick4(STICK_4_PORT), // initialize joystick 4 < first EVOM joystick-m,	
	shifterDS(1,2)
{	
	printf("Initializing drive\n");
	driveTask = new Task("Driving", (FUNCPTR) DriveRunner);
	/*if (driveTask->SetPriority(0))
		printf("driving priority set to 0\n");
	else
		printf("driving priority not set\n");*/

	drive1.SetSafetyEnabled(false);
	drive2.SetSafetyEnabled(false);
	drive3.SetSafetyEnabled(false);
	drive4.SetSafetyEnabled(false);
	drive1.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
	drive2.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);   
	drive3.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
	drive4.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
	drive1.SetVoltageRampRate(0.0);
	drive2.SetVoltageRampRate(0.0);
	drive3.SetVoltageRampRate(0.0);
	drive4.SetVoltageRampRate(0.0);
	drive1.ConfigFaultTime(0.1);
	drive2.ConfigFaultTime(0.1);
	drive3.ConfigFaultTime(0.1);
	drive4.ConfigFaultTime(0.1);
	
	drive1.SetSpeedReference(CANJaguar::kSpeedRef_Encoder); 
	drive3.SetSpeedReference(CANJaguar::kSpeedRef_Encoder);
	//drive1.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	//drive3.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	drive1.ConfigEncoderCodesPerRev(250);
	drive3.ConfigEncoderCodesPerRev(250);
	drive1.EnableControl();
	drive3.EnableControl();
	
	maxDrive1RPM = 0;
	maxDrive3RPM = 0;
	driveLogCounter = 0;
	lastShift = GetTime();
	lastDataLog = GetTime();
	
	printf("Finished initializing drive\n");
	AddToSingletonList();
}
TKODrive* TKODrive::inst()
{
	if (!m_Instance)
	{
		printf("Drive instance is null\n");
		m_Instance = new TKODrive;
	}
	return m_Instance;
}
void TKODrive::DriveRunner()
{
	while (DriverStation::GetInstance()->IsEnabled())
	{
		m_Instance->TankDrive();
		m_Instance->LogData();
		//m_Instance->VerifyJags();
		//Wait(0.001);
	}
}

void TKODrive::Start()
{
	if (not driveTask->Verify() or driveTask->IsSuspended())
		driveTask->Start();
}
void TKODrive::Stop()
{
	if (driveTask->Verify())
		driveTask->Stop();
	
	TKOLogger::inst()->addMessage("Drive 1 Max Speed: %f", maxDrive1RPM);
	TKOLogger::inst()->addMessage("Drive 3 Max Speed: %f", maxDrive3RPM);
}

void TKODrive::LogData()
{
	if (drive1.GetSpeed() > maxDrive1RPM)
		maxDrive1RPM = drive1.GetSpeed();
	if (drive3.GetSpeed() > maxDrive3RPM)
		maxDrive3RPM = drive3.GetSpeed();
	
	if (!DriverStation::GetInstance()->IsEnabled()) return;
	if (GetTime() - lastDataLog <= 1.) return; //TODO 1.0 means logs every 1 second
	
	TKOLogger::inst()->addMessage("-----DRIVE DATA------\n");
	
	TKOLogger::inst()->addMessage("Drive 1 Vbus Percent Output: %f", drive1.Get());
	TKOLogger::inst()->addMessage("Drive 2 Vbus Percent Output: %f", drive2.Get());
	TKOLogger::inst()->addMessage("Drive 3 Vbus Percent Output: %f", drive3.Get());
	TKOLogger::inst()->addMessage("Drive 4 Vbus Percent Output: %f\n", drive4.Get());
	
	TKOLogger::inst()->addMessage("Drive 1 Voltage Output: %f", drive1.GetOutputVoltage());
	TKOLogger::inst()->addMessage("Drive 2 Voltage Output: %f", drive2.GetOutputVoltage());
	TKOLogger::inst()->addMessage("Drive 3 Voltage Output: %f", drive3.GetOutputVoltage());
	TKOLogger::inst()->addMessage("Drive 4 Voltage Output: %f\n", drive4.GetOutputVoltage());
	
	TKOLogger::inst()->addMessage("Drive 1 Current Output: %f", drive1.GetOutputCurrent());
	TKOLogger::inst()->addMessage("Drive 2 Current Output: %f", drive2.GetOutputCurrent());
	TKOLogger::inst()->addMessage("Drive 3 Current Output: %f", drive3.GetOutputCurrent());
	TKOLogger::inst()->addMessage("Drive 4 Current Output: %f\n", drive4.GetOutputCurrent());
	
	TKOLogger::inst()->addMessage("Drive 1 Speed: %f", drive1.GetSpeed());
	TKOLogger::inst()->addMessage("Drive 3 Speed: %f\n", drive3.GetSpeed());
	
	printf("-----DRIVE DATA------\n");
		
	printf("Drive 1 Vbus Percent Output: %f\n", drive1.Get());
	printf("Drive 2 Vbus Percent Output: %f\n", drive2.Get());
	printf("Drive 3 Vbus Percent Output: %f\n", drive3.Get());
	printf("Drive 4 Vbus Percent Output: %f\n\n", drive4.Get());
	
	printf("Drive 1 Voltage Output: %f\n", drive1.GetOutputVoltage());
	printf("Drive 2 Voltage Output: %f\n", drive2.GetOutputVoltage());
	printf("Drive 3 Voltage Output: %f\n", drive3.GetOutputVoltage());
	printf("Drive 4 Voltage Output: %f\n\n", drive4.GetOutputVoltage());
	
	printf("Drive 1 Current Output: %f\n", drive1.GetOutputCurrent());
	printf("Drive 2 Current Output: %f\n", drive2.GetOutputCurrent());
	printf("Drive 3 Current Output: %f\n", drive3.GetOutputCurrent());
	printf("Drive 4 Current Output: %f\n\n", drive4.GetOutputCurrent());
	
	printf("Drive 1 Speed: %f\n", drive1.GetSpeed());
	printf("Drive 3 Speed: %f\n\n", drive3.GetSpeed());
		
	driveLogCounter++;
	lastDataLog = GetTime();
}

void TKODrive::TankDrive()
{
	if (!DriverStation::GetInstance()->IsEnabled()) return;
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
		drive1.Set(stick1.GetY() * 0.4);
		drive2.Set(stick1.GetY() * 0.4);
		drive3.Set(-stick2.GetY() * 0.4);
		drive4.Set(-stick2.GetY() * 0.4);
	}
	else if (stick1.GetRawButton(2))
	{
		drive1.Set(stick1.GetY() * 0.2);
		drive2.Set(stick1.GetY() * 0.2);
		drive3.Set(-stick2.GetY() * 0.2);
		drive4.Set(-stick2.GetY() * 0.2);
	}
	else if (stick1.GetRawButton(4))
	{
		drive1.Set(stick1.GetY());
		drive2.Set(stick1.GetY());
		drive3.Set(-stick2.GetY());
		drive4.Set(-stick2.GetY());
	}
	else
	{
		drive1.Set(stick1.GetY() * 0.8);
		drive2.Set(stick1.GetY() * 0.8);
		drive3.Set(-stick2.GetY() * 0.8);
		drive4.Set(-stick2.GetY() * 0.8);
	}
	TKODrive::ManualShift();
}
void TKODrive::ManualShift()
{
	if (GetTime() - lastShift < 1.) //1. is the constant for min delay between shifts
		return; 

	//printf("Manual shifting\n");
	if (stick2.GetRawButton(4)) 
	{
		shifterDS.Set(shifterDS.kForward);
		lastShift = GetTime();
	}
	if (stick2.GetRawButton(2)) 
	{
		shifterDS.Set(shifterDS.kReverse);
		lastShift = GetTime();
	}
}
bool TKODrive::VerifyJags() //if returns false, jag problems
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
void TKODrive::switchToSpeed()
{
	drive1.ChangeControlMode(CANJaguar::kSpeed);
	drive3.ChangeControlMode(CANJaguar::kSpeed);
	drive1.SetSpeedReference(CANJaguar::kSpeedRef_Encoder);
	drive3.SetSpeedReference(CANJaguar::kSpeedRef_Encoder);
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

TKODrive::~TKODrive()
{
	driveTask->Stop();
	m_Instance = NULL;
}

