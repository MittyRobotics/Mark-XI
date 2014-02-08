#ifndef __TKOROLLER_H
#define __TKOROLLER_H

#include "../Definitions.h"

//Code for intake roller

class TKORoller 
{
	public: 
		TKORoller(int rollerPort1, int rollerPort2);
		~TKORoller();
		void rollerManualMove();
		void rollerSimpleMove();
	private:
		CANJaguar _roller1, _roller2;
		Joystick stick3;
};

#endif

#ifndef __TKOARM_H
#define __TKOARM_H

#include "StateMachine.h"
#include "../Definitions.h"

//Code for intake roller and arm movement

class TKOArm: public SensorBase
{
	public:
		TKOArm();
		~TKOArm();
		static TKOArm* inst();
		bool Start();
		bool Stop();
		void switchToPositionMode();
		void moveToFront();
		void moveToMid();
		void moveToBack();
		bool armInFiringRange();
	private:
		DISALLOW_COPY_AND_ASSIGN(TKOArm);
		static TKOArm* m_Instance;
		static void ArmRunner();
		const float minArmPos, maxArmPos;
		void runManualArm();
		Task *armTask;
		bool armEnabled;
	
		CANJaguar _arm;
		DigitalInput limitSwitchArm, limitSwitchBall;
		TKORoller _roller;
		Joystick stick3, stick4;
};
#endif
