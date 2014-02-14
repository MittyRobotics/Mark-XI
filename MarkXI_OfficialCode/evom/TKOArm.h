#ifndef __TKOROLLER_H
#define __TKOROLLER_H

#include "../Definitions.h"

//Code for intake roller

class TKORoller 
{
	public: 
		~TKORoller();
		void rollerManualMove();
		void rollerSimpleMove();
		static TKORoller* inst();
		CANJaguar _roller1, _roller2;
		bool override;
	private:
		TKORoller(int rollerPort1, int rollerPort2);
		DigitalInput limitSwitchBall;
		Joystick stick4;
		static TKORoller*  _instance;
};

#endif
//End of TKORoller .h^^^
#ifndef __TKOARM_H
#define __TKOARM_H

#include "StateMachine.h"
#include "../Definitions.h"

//Code for intake roller and arm movement

class TKOArm: public SensorBase
{
	public:
		~TKOArm();
		static TKOArm* inst();
		bool Start();
		bool Stop();
		void switchToPositionMode();
		void switchToVBusMode();
		void moveToFront();
		void moveToMid();
		void moveToBack();
		void toggleMode();
		bool armInFiringRange();
	private:
		TKOArm();
		DISALLOW_COPY_AND_ASSIGN(TKOArm);
		static TKOArm* m_Instance;
		static void ArmRunner();
		const float minArmPos, maxArmPos;
		float lastToggle;
		void runManualArm();
		void runPositionArm();
		Task *armTask;
		bool armEnabled;
		bool runningVBus;
	
		CANJaguar _arm;
		DigitalInput limitSwitchArm;
		Joystick stick3, stick4;
};
#endif
