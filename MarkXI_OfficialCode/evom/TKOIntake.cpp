//Last Edited by Zach Light and Ben Kim
// on 01/31/2014
#include "TKOIntake.h"

TKOIntake* TKOIntake::m_Instance = NULL;

/*
 * Initializes the 2 roller jaguars in percent Vbus mode
 *  \parm int port 1 - roller 1 Jaguar ID
 *  \parm int port 2- roller 2 Jaguar ID
 */

TKOIntake::TKOIntake() :
			//jaguar numbers undefined
			_roller1(6, CANJaguar::kPercentVbus),
			_roller2(7, CANJaguar::kPercentVbus),
			_arm1(8, CANJaguar::kPercentVbus), limitSwitchArm(1), // Optical limit switch
			stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
			stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
			stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
			stick4(STICK_4_PORT), // initialize joystick 4 < second EVOM joystick-m,
			armEncoder(1, 8, 1, 9, true) {
	printf("Initializing intake\n");
	intakeTask = new Task("TKOIntake", (FUNCPTR) IntakeRunner);
	_roller1.SetSafetyEnabled(true);
	_roller2.SetSafetyEnabled(true);
	AddToSingletonList();
	armEncoder.Start();
}

TKOIntake::~TKOIntake() {
	m_Instance = NULL;
}

TKOIntake* TKOIntake::inst() {
	if (!m_Instance) {
		printf("TKOIntake instance is null\n");
		m_Instance = new TKOIntake;
	}
	return m_Instance;
}
void TKOIntake::init() {
	armEncoder.Reset();
	_arm1.Set(-1);
	if (limitSwitchArm.Get() == 0) {
		_arm1.Set(0);
		encoderValueBack = armEncoder.Get();
	}
	_arm1.Set(1);
	if (limitSwitchArm.Get() == 0) {
		_arm1.Set(0);
		encoderValueFront = armEncoder.Get();
	}
	encoderValueMid = (encoderValueBack + encoderValueFront) /2;
	
}


void TKOIntake::armMoveFront() { // Arm move code to move it to the lowest position


	_arm1.Set(-1); // The arm starts going in a downwards fashion. This can be changed if needed.
	if (limitSwitchArm.Get() == 1) {
		_arm1.Set(0);
		printf("The arm is at the front.\n");
	}
}

void TKOIntake::armMoveMiddle() {
	if (armEncoder.Get() < encoderValueMid) {//If it is under the second limit switch
		_arm1.Set(1);
	}
	if (armEncoder.Get() > encoderValueMid) {//If it is under the second limit switch
		_arm1.Set(-1); 
	}
	if (armEncoder.Get() == (encoderValueBack + encoderValueFront)/2) {
		_arm1.Set(0);
		printf("The arm is high as fuuuuuuuuuu\n");
	}

}

void TKOIntake::armMoveBack() {
	_arm1.Set(1);
	if (limitSwitchArm.Get() == 1) {//The highest limit switch is triggered, and the arm is off.
		_arm1.Set(0);
		printf("The arm is at the back.\n");
	}
}
void TKOIntake::armMoveManual() {
	if(limitSwitchArm.Get() == 0)
	{
		if(armEncoder.Get() > encoderValueMid)
		{
			if(-stick3.GetY() < 0)
			{
				_arm1.Set(stick3.GetY());
			}
		}
		else if(armEncoder.Get() < encoderValueMid)
		{
			if(-stick3.GetY() > 0)
			{
				_arm1.Set(stick3.GetY());
			}
		}
		else
			_arm1.Set(0);
	}
	else{
		_arm1.Set(-stick3.GetY());
	}
}


