//Last edited by Vadim Korolik
//on 02/06/2014
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
	DoubleSolenoid shifterDS;
	Task *driveTask;
	
	long driveLogCounter;
	float lastShift, lastDataLog;
	double maxDrive1RPM, maxDrive3RPM;
	
	void TankDrive();
	void LogData();
	void ManualShift();
	TKODrive();
	~TKODrive();
	static TKODrive* m_Instance;
	static void DriveRunner();
	DISALLOW_COPY_AND_ASSIGN(TKODrive);
};
#endif
