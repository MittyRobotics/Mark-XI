//Last Edited by Ben Kim
// on 02/03/2014
#include "TKOIntake.h"
TKOIntake* TKOIntake::m_Instance = NULL;

// TODO: determine if 0 is limit triggered

TKOIntake::TKOIntake() :
			//jaguar numbers undefined
			_roller1(6, CANJaguar::kPercentVbus),
			_roller2(7, CANJaguar::kPercentVbus),
			_arm1(8, CANJaguar::kPercentVbus), 
			armLimitSwitch(3), // Optical limit switch
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
void TKOIntake::init() {	// calibration for top and bottom setpoints
	armEncoder.Reset();
	_arm1.Set(-1);		// move arm down
	if (armLimitSwitch.Get() == 0) { // top triggered
		_arm1.Set(0);
		encoderValueBack = armEncoder.Get();
	}
	_arm1.Set(1);	// move arm back up
	if (armLimitSwitch.Get() == 0) { // bottom triggered
		_arm1.Set(0);
		encoderValueFront = armEncoder.Get();
	}
	encoderValueMid = (encoderValueBack + encoderValueFront) / 2;

}

void TKOIntake::armMoveFront() {
	_arm1.Set(-1);
	if (armLimitSwitch.Get() == 0) {	// arm limit switch returns same value for either side
		_arm1.Set(0);
		printf("The arm is at the front.\n");
	}
}

void TKOIntake::armMoveMiddle() {
	if (armEncoder.Get() < encoderValueMid) {	// moves up if too far forward
		_arm1.Set(1);
	}
	if (armEncoder.Get() > encoderValueMid) {	// moves down if too far back
		_arm1.Set(-1);
	}
	if (armEncoder.Get() == encoderValueMid) {	// stops once arm reaches middle
		_arm1.Set(0);
		printf("The arm is at the middle");
	}

}

void TKOIntake::armMoveBack() {
	_arm1.Set(1);
	if (armLimitSwitch.Get() == 0) {	// arm limit switch returns same value for either side
		_arm1.Set(0);
		printf("The arm is at the back.\n");
	}
}
void TKOIntake::armMoveManual() {
	if (armLimitSwitch.Get() == 1) {
		if (armEncoder.Get() > encoderValueMid) {	// arm is back
			if (-stick3.GetY() < 0) {			// arm should not go back further
				_arm1.Set(stick3.GetY());
			}
		} else if (armEncoder.Get() < encoderValueMid) {	// arm is forward
			if (-stick3.GetY() > 0) {		// arm should not go forward further
				_arm1.Set(stick3.GetY());
			}
		} else {			// arm is at top/middle
			_arm1.Set(0);
		}
	} else {
		_arm1.Set(-stick3.GetY());
	}
}

