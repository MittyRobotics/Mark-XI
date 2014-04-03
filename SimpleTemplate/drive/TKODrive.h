//Last edited by Vadim Korolik
//on 03/01/2014
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
	DoubleSolenoid* getShifterDoubleSolenoid();
	double maxDrive1RPM, maxDrive3RPM;
private:
	CANJaguar* drive1, *drive2, *drive3, *drive4; // define motors
	RobotDrive *robotDrive;
	Joystick stick1, stick2, stick3, stick4; // define joysticks
	DoubleSolenoid shifterDS;
	Task *driveTask;
	
	long driveLogCounter;
	float lastShift, lastDataLog, lastFire;
	const float speedShiftRPM;
	void initJaguars();
	void destroyJaguars();
	void TankDrive();
	void ArcadeDrive();
	void LogData();
	void ManualShift();
	void AutoShift();
	TKODrive();
	~TKODrive();
	static TKODrive* m_Instance;
	static void DriveRunner();
	DISALLOW_COPY_AND_ASSIGN(TKODrive);
};
#endif
