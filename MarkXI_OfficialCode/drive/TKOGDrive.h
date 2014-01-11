#ifndef __TKOGDRIVE_H
#define __TKOGDRIVE_H

#include "../Definitions.h"
#include "../log/TKOLogger.h"
#include "../component/TKOGyro.h"

class TKOGDrive
{
public:
	static TKOGDrive* inst();
	void Start();
	void Stop();
	bool VerifyJags();
private:
	TKOGDrive();
	~TKOGDrive();
	CANJaguar drive1, drive2, drive3, drive4; // define motors
	Joystick stick1, stick2, stick3, stick4; // define joysticks
	RobotDrive rDrive;
	Task *driveTask;
	static TKOGDrive* m_Instance;
	DriverStation *ds;
	float TURN_kP;
	static void DriveRunner();
	void gyroDrive();
};
#endif
