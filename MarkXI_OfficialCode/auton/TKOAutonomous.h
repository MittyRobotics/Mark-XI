//Last edited by Vadim Korolik
//on 02/09/2013
#ifndef __TKOAUTONOMOUS_H
#define __TKOAUTONOMOUS_H

#include "../Definitions.h"
#include "../component/TKOGyro.h"
#include "../component/TKORelay.h"
#include "../log/TKOLogger.h"

class TKOAutonomous
{
	public:
		TKOAutonomous();
		~TKOAutonomous();
		void autonomousCode();
		void autonSetup1();
		void autonSetup2();
		void autonSetup3();
		void startAutonomous();
		void stopAutonomous();
		void setDrivePID(float P, float I, float D);
		void setDriveTargetStraight(float target);
		void setTargetAngle(float target);
		bool PIDDriveStraight();
		bool driveLeft();
		bool driveRight();
		void resetEncoders();
		bool turn(double target);
		void initAutonomous();
		void printMessages();
		float getPosition(int jaguar);
		float getTarget(int jaguar);
		bool runningAuton;
		bool autonOption[15];
		Timer autonTimer;
		static TKOAutonomous* inst();
	private:
		CANJaguar drive1, drive2, drive3, drive4;
		static DriverStation *ds;
		static TKOAutonomous* m_Instance;
		Task *autonTask;
		static void AutonTaskRunner();
		float rampTargetLeft;
		float rampTargetRight;
		float rightTarget;
		float leftTarget;
		float rampRate;
		float rampRate2;
		float targetAngle;
		bool reachedTarget;
		int autonomousMode;
};

#endif
