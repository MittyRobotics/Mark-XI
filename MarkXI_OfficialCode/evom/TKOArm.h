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
		Joystick stick3;
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
		void printDSMessages();
		void switchToVBusMode();
		void moveToFront();
		void moveToMid();
		void moveToBack();
		bool armInFiringRange();
		void runManualArm();
	private:
		TKOArm();
		DISALLOW_COPY_AND_ASSIGN(TKOArm);
		static TKOArm* m_Instance;
		static void ArmRunner();
		const float minArmPos, maxArmPos;
		Task *armTask;
		bool armEnabled;
	
		CANJaguar _arm;
		DigitalInput limitSwitchArm;
		AnalogChannel usonic;
		Joystick stick3, stick4;
};
#endif
