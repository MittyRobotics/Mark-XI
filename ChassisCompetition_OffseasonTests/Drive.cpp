//Last edited by Vadim Korolik
//on 07/29/2013
#include "Drive.h"
#include "Definitions.h"

Drive *_instance;

Drive::Drive() :
	LeftMotor(1, CANJaguar::kPercentVbus), RightMotor(2, CANJaguar::kPercentVbus),
	drive(&LeftMotor, &RightMotor),
	stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
	stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
	stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
	stick4(STICK_4_PORT)// initialize joystick 4 < first EVOM joystick-m,
{
	if (_instance == NULL)
		_instance = this;
	driveTask = new Task("Drive", (FUNCPTR) DriveRunner);
	driveSetting = 0; //0 is tank, 1 is arcade, 2 is arcade 1 stick, 3 is pid?
}
Drive* Drive::getInstance()
{
	if (_instance == NULL)
		_instance = this;
	return _instance;
}
void Drive::DriveRunner()
{
	while (true)
	{
		_instance->DrivePicker();
		Wait(0.005);
	}
}
void Drive::StartDrive()
{
	driveTask->Start();
	driveTask->SetPriority(0);
}
void Drive::StopDrive()
{
	if (driveTask->Verify())
		driveTask->Stop();
	if (!driveTask->Verify())
		printf("Successfully shutdown drive task!\n");
}
void Drive::DrivePicker()
{
	if (stick1.GetRawButton(4))
		driveSetting = 0;
	else if (stick1.GetRawButton(3))
		driveSetting = 1;
	else if (stick1.GetRawButton(5))
		driveSetting = 2;
	else if (stick1.GetRawButton(2))
		driveSetting = 3;
		
	if (driveSetting == 0)
		TankDrive();
	else if (driveSetting == 1)
		ArcadeDrive();
	else if (driveSetting == 2)
		ArcadeDriveSingle();
}
void Drive::TankDrive()
{
//	drive.TankDrive(&stick1, &stick2);
//	LeftMotor.Set(stick1.GetY());
//	RightMotor.Set(-stick2.GetY());
//	
	if (stick2.GetTrigger())
	{
		LeftMotor.Set(stick1.GetY() * 0.4);
		RightMotor.Set(-stick2.GetY() * 0.4);
	}
	else if (stick2.GetRawButton(2))
	{
		LeftMotor.Set(stick1.GetY() * 0.2);
		RightMotor.Set(-stick2.GetY() * 0.2);
	}
	else if (stick2.GetRawButton(4))
	{
		LeftMotor.Set(stick1.GetY());
		RightMotor.Set(-stick2.GetY());
	}
	else
	{
		LeftMotor.Set(stick1.GetY() * 0.8);
		RightMotor.Set(-stick2.GetY() * 0.8);
	}
}
void Drive::ArcadeDrive()
{
	drive.ArcadeDrive(stick1.GetY(), stick2.GetX());
}
void Drive::ArcadeDriveSingle()
{
	drive.ArcadeDrive(&stick1);
}
Drive::~Drive()
{
	driveTask->Stop();
	delete this;
}

