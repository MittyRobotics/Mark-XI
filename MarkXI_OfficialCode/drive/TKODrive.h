//Last edited by Vadim Korolik
//on 11/27/2013
#ifndef __TKODRIVE_H
#define __TKODRIVE_H

#include "../Definitions.h"
#include "../log/TKOLogger.h"

class TKODrive : public SensorBase
{
public:
	static TKODrive* inst();
	void switchToSpeed();
	void Start();
	void Stop();
	bool VerifyJags();
private:
	CANJaguar drive1, drive2, drive3, drive4; // define motors
	Joystick stick1, stick2, stick3, stick4; // define joysticks
	Task *driveTask;
	void TankDrive();
	TKODrive();
	~TKODrive();
	static TKODrive* m_Instance;
	static void DriveRunner();
	DISALLOW_COPY_AND_ASSIGN(TKODrive);
};
#endif
