//Last Edited by Murad Awad
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
			_arm1(8, CANJaguar::kPercentVbus), LimitSwitchArm(1), // Optical limit switch
			limitIn(2),// Limit switch in arms
			stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
			stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
			stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
			stick4(STICK_4_PORT), // initialize joystick 4 < second EVOM joystick-m,
			ArmEncoder(1, 8, 1, 9, true) {
	printf("Initializing intake\n");
	intakeTask = new Task("TKOIntake", (FUNCPTR) IntakeRunner);
	_roller1.SetSafetyEnabled(true);
	_roller2.SetSafetyEnabled(true);
	AddToSingletonList();
	ArmEncoder.Start();
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
	ArmEncoder.Reset();
	_arm1.Set(-1);
	if (LimitSwitchArm.Get() == 0) {
		_arm1.Set(0);
		encoderValueBack = ArmEncoder.Get();
	}
	_arm1.Set(1);
	if (LimitSwitchArm.Get() == 0) {
		_arm1.Set(0);
		encoderValueFront = ArmEncoder.Get();
	}
	encoderValueMid = (encoderValueBack + encoderValueFront) /2;
	
}


void TKOIntake::ArmMoveFront() { // Arm move code to move it to the lowest position


	_arm1.Set(-1); // The arm starts going in a downwards fashion. This can be changed if needed.
	if (LimitSwitchArm.Get() == 1) {
		_arm1.Set(0);
		printf("The arm is at the front.\n");
	}
}

void TKOIntake::ArmMoveMiddle() {
	if (ArmEncoder.Get() < encoderValueMid) {//If it is under the second limit switch
		_arm1.Set(1);
	}
	if (ArmEncoder.Get() > encoderValueMid) {//If it is under the second limit switch
		_arm1.Set(-1); 
	}
	if (ArmEncoder.Get() == (encoderValueBack + encoderValueFront)/2) {
		_arm1.Set(0);
		printf("The arm is high as fuuuuuuuuuu\n");
	}

}

void TKOIntake::ArmMoveBack() {
	_arm1.Set(1);
	if (LimitSwitchArm.Get() == 1) {//The highest limit switch is triggered, and the arm is off.
		_arm1.Set(0);
		printf("The arm is at the back.\n");
	}
}
void TKOIntake::ArmMoveManual() {
	if(LimitSwitchArm.Get() == 0)
	{
		if(ArmEncoder.Get() > encoderValueMid)
		{
			if(-stick3.GetY() < 0)
			{
				_arm1.Set(stick3.GetY());
			}
		}
		else if(ArmEncoder.Get() < encoderValueMid)
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


