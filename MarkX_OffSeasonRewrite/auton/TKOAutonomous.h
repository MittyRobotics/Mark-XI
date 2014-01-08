//Last edited by Vadim Korolik
//on 02/09/2013
#ifndef __TKOAUTONOMOUS_H
#define __TKOAUTONOMOUS_H

#include "../Definitions.h"
#include "../component/TKOGyro.h"
#include "../evom/TKOClimber.h"

class TKOAutonomous
{
	public:
		TKOAutonomous(int port1, int port2, int port3, int port4);
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
		TKORelay rsFrontLoaderWrist, rsFrontLoaderLift; //Front loader
	private:
		CANJaguar drive1, drive2, drive3, drive4;
		DriverStation *ds;
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
