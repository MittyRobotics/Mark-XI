	//Last Edited by Ishan Shah and Murad Awad
#ifndef __TKOINTAKE_H
#define __TKOINTAKE_H

#include "../Definitions.h"

//Code for intake roller and arm movement

class TKOIntake: public SensorBase {
public:
	TKOIntake();
	~TKOIntake();
	static TKOIntake* inst();
	void init();
	void RollerMove();
	void ArmMoveFront();
	void ArmMoveMiddle();
	void ArmMoveBack();
	void LimitSwitchTest();
	void ArmMoveManual();
	float encoderValueBack, encoderValueFront, encoderValueMid;
private:
	DISALLOW_COPY_AND_ASSIGN(TKOIntake);
	static TKOIntake* m_Instance;
	static void IntakeRunner();
	Task *intakeTask;

	CANJaguar _roller1, _roller2, _arm1;
	DigitalInput LimitSwitchArm, limitIn;
	Joystick stick1, stick2, stick3, stick4;
	Encoder ArmEncoder; 
};
#endif
