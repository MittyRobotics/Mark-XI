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
	private:
		DISALLOW_COPY_AND_ASSIGN(TKOArm);
		static TKOArm* m_Instance;
		static void ArmRunner();
		Task *armTask;
	
		CANJaguar _arm;
		DigitalInput limitSwitchArm, limitSwitchBall;
		TKORoller _roller;
		Joystick stick3, stick4;
};
#endif
