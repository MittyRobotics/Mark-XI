//Last Edited by Arjun Biju
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
			_arm1(8, CANJaguar::kPercentVbus),
			_arm2(9, CANJaguar::kPercentVbus), limitLow(1), // Lowest limit switch
			limitMiddle(2), // Middle limit switch
			limitHigh(3),// Top limit switch
			limitIn(4), stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
			stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
			stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
			stick4(STICK_4_PORT) // initialize joystick 4 < second EVOM joystick-m,
{
	printf("Initializing intake\n");
	intakeTask = new Task("TKOIntake", (FUNCPTR) IntakeRunner);
	printf("intake priority not set\n");
	AddToSingletonList();
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

void TKOIntake::RollerMove() {
	//intake of ball.
	_roller1.Set(1);
	_roller2.Set(-1);
	if (limitIn.Get() == 0) {
		_roller1.Set(0);
		_roller2.Set(0);

	}
}

void TKOIntake::RollerMoveOut() {
	//passing a ball.
	_roller1.Set(-1);
	_roller2.Set(1);
}
/*else {
 _roller1.Set(0);
 _roller2.Set(0);*/

void TKOIntake::ArmMoveLow() { // Arm move code to move it to the lowest position

	_arm1.Set(1); // The arm starts going in a downwards fashion. This can be changed if needed.
	_arm2.Set(1);
	if (limitLow.Get() == 0) {
		_arm1.Set(0);
		_arm2.Set(0);
		printf("The arm is at the lowest position.");

	}
}

void TKOIntake::ArmMoveMiddle() {
	_arm1.Set(1); //Arm first moves in downward fashion in case it is above the second limit switch
	_arm2.Set(1);
	if (limitLow.Get() == 0) {//If it is under the second limit switch, it  hits the first one, and reverts to start moving up
		_arm1.Set(-1);
		_arm2.Set(1);
	}
	if (limitMiddle.Get() == 0) { //If it reaches the second limit switch
		_arm1.Set(0);
		_arm2.Set(0);
		printf("The arm has reached the middle position.");
	}
}

void TKOIntake::ArmMoveHigh() {
	_arm1.Set(-1);
	_arm2.Set(-1);
	if (limitHigh.Get() == 1) {//The highest limit switch is triggered, and the arm is off.
		_arm1.Set(0);
		_arm2.Set(0);
		printf("The arm has reached the highest position");
	}
}

void TKOIntake::IntakeRunner() {
	while (true) {

	}
}

