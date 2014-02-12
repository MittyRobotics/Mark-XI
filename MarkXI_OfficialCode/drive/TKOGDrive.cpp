#include "TKOGDrive.h"

TKOGDrive* TKOGDrive::m_Instance = NULL;

TKOGDrive::TKOGDrive() :
	drive1(DRIVE_L1_ID, CANJaguar::kPercentVbus), // initialize motor 1 < first left drive motor
	drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus), // initialize motor 2 < second left drive motor
	drive3(DRIVE_R1_ID, CANJaguar::kPercentVbus), // initialize motor 3 < first right drive motor
	drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus), // initialize motor 4 < second right drive motor
	stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
	stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
	stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
	stick4(STICK_4_PORT), // initialize joystick 4 < first EVOM joystick-m,
	rDrive(&drive1, &drive2, &drive3, &drive4)
{	
	//TRY USE ALL THIS IS kSPEED MODE
	printf("Initializing gyro drive\n");
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
	rDrive.SetExpiration(0.1);
	rDrive.SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
	rDrive.SetInvertedMotor(RobotDrive::kRearRightMotor, true);
	rDrive.SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);
	rDrive.SetInvertedMotor(RobotDrive::kRearLeftMotor, true);
	ds = DriverStation::GetInstance();
	driveTask = new Task("Driving", (FUNCPTR) DriveRunner, 1);
//	if (driveTask->SetPriority(5))
//		printf("gyro driving priority set to 5\n");
//	else
//		printf("gyro driving priority not set\n");
	TURN_kP = 1/360.;
	printf("Finished initializing gyro drive\n");
	AddToSingletonList();
}
TKOGDrive* TKOGDrive::inst()
{
	if (!m_Instance)
		m_Instance = new TKOGDrive;
	return m_Instance;
}
void TKOGDrive::Start()
{
	if (not driveTask->Verify() or driveTask->IsSuspended())
		driveTask->Start();
}
void TKOGDrive::Stop()
{
	if (driveTask->Verify() or not driveTask->IsSuspended())
		driveTask->Stop();
}

void TKOGDrive::DriveRunner()
{
	TKOGyro::inst()->reset();
	while (true)
	{
		m_Instance->gyroDrive();
//		m_Instance->VerifyJags();
		Wait(0.005);
	}
}

void TKOGDrive::gyroDrive()
{
//	float targetAngle = TKOGyro::inst()->GetAngle();
	while (ds->IsEnabled())
	{
//		targetAngle += stick2.GetY();
//		if (targetAngle > 360.)
//			targetAngle = 0.;
//		if (targetAngle < 0.)
//			targetAngle = 360.;
		
		rDrive.Drive(stick1.GetY(), -TKOGyro::inst()->GetAngle() * TURN_kP);	//check kp, - signs
//		rDrive.ArcadeDrive(&stick1);
	}
}

bool TKOGDrive::VerifyJags() //if returns false, jag problems
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


TKOGDrive::~TKOGDrive()
{
	driveTask->Stop();
	m_Instance = NULL;
}

