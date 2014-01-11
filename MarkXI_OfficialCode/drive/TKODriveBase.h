//Last edited by Vadim Korolik
//on 11/27/2013
#ifndef __TKODRIVEBASE_H
#define __TKODRIVEBASE_H

#include "../Definitions.h"
#include "../log/TKOLogger.h"

class TKODriveBase
{
public:
	void switchToSpeed();
	void Start();
	void Stop();
	bool VerifyJags();
protected:
	TKODriveBase();
	~TKODriveBase();
	CANJaguar drive1, drive2, drive3, drive4; // define motors
	Joystick stick1, stick2, stick3, stick4; // define joysticks
	Task *driveTask;
	void TankDrive();
};
#endif
