#ifndef __DRIVE_H
#define __DRIVE_H

#include "Definitions.h"

class Drive
{
public:
	Drive();
	~Drive();
	Drive* getInstance();
	void StartDrive();
	void StopDrive();
	Task *driveTask;
private:
	CANJaguar LeftMotor, RightMotor;
	RobotDrive drive;
	Joystick stick1, stick2, stick3, stick4; // define joysticks
	void DrivePicker();
	void TankDrive();
	void ArcadeDrive();
	void ArcadeDriveSingle();
	static void DriveRunner();
	int driveSetting;
};
#endif
