//Created by Ishan Shah 
#include "TurnAtom.h"

Turn_Atom::Turn_Atom(float ang, CANJaguar* drive1, CANJaguar* drive2,
		CANJaguar* drive3, CANJaguar* drive4, Encoder* encoderLeft,
		Encoder* encoderRight) {
	//	 const float REVS_PER_FOOT = 0.7652439024;
	/* NOTE: Everything in this comment block, unless noted otherwise, is from Mark X! Data must be changed later to Mark XI or later revisions for pratical use. 
	 * Please keep original data for others to use until robot in question is destroyed, never to be rebuilt the same way. 
	 * average inches per rotation is about 12.925 inches on Mark X, measurements taken by Zach
	 * the distance between the wheels is ~26.51 inches
	 * circumferance of circle created with that distance is ~83.28362125 inches
	 * inches/degree = 0.231
	 * revolutions/inch = ~0.07736943907156673114119922630561
	 * revolutions/degrees = ~0.0178989085 
	 */

	_angle = ang * .0178989085;
	_drive1 = drive1;
	_drive2 = drive2;
	_drive3 = drive3;
	_drive4 = drive4;
	_encoderLeft = encoderLeft;
	_encoderRight = encoderRight;
}

Turn_Atom::~Turn_Atom() {
}

void Turn_Atom::resetEncoders() {
	_drive1->EnableControl(0);
	_drive3->EnableControl(0);
}

void Turn_Atom::run() {

	//	TKOGyro::inst()->reset();
	// do really cool things with the motors here
	// like turn in circles here

	while (turn( _angle)) {

		_drive1->Set(9001/*this value is only for testing*/); //same, but for jag 3 since only 1 and 3 have encoders
		_drive2->Set(-_drive1->GetOutputVoltage() / _drive1->GetBusVoltage()); //sets second and fourth jags in slave mode

		_drive3->Set(9001/*this value is only for testing*/); //same, but for jag 3 since only 1 and 3 have encoders
		_drive4->Set(-_drive3->GetOutputVoltage() / _drive3->GetBusVoltage()); //sets second and fourth jags in slave mode

	}

}

bool Turn_Atom::turn(double target) {
	/*
	 * Counterclockwise is a positive value, clockwise is a negative value
	 * revolutions/degrees = ~0.0178989085 
	 * inches/encoderTic(250 tic encoder) = 0.333134485
	 */

	float currAngle = _encoderRight->Get() * 0.333134485;
	float _target = target * .0178989085;
	//The hopeful end result of this will be that each value is set up in revolutions, and that we can continue to run until we hit the correct revolution setting.

	if (_drive1->GetOutputCurrent() > DRIVE_CURRENT_CUTOFF
			or _drive3->GetOutputCurrent() > DRIVE_CURRENT_CUTOFF) {
		printf("Crashed into wall...");
		return true;
	}

	if (currAngle < _target - 1 && !(_target < currAngle)) {
		printf("Turning right \n");
		_drive2->Set(0.7);
		_drive4->Set(0.7);
	}
	if (currAngle + 2.0 >= _target) {
		if (currAngle - 2.0 <= _target) {
			_drive2->Set(0);
			_drive4->Set(0);
			printf("Reached goal, breaking out \n");

			if (!reachedTarget) {
				reachedTarget = true;
				resetEncoders();
				printf("Reset position\n"); //All this resetting does not work properly, needs to be fixed
				//Cannot go without it because when the robot turns, it affects the position
			}
			return true;
		}
	}
	if (currAngle > _target && !(_target > currAngle)) {
		printf("Past target \n");
		_drive2->Set(-0.8);
		_drive4->Set(-0.8);
	}
	return false;

}
