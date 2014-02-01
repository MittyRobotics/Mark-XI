//Last edited by Ishan Shah and Alex Parks
#include "TurnAtom.h"
#include <cstring>

// positive turns the robot clockwise.
// q

Turn_Atom::Turn_Atom(float ang, CANJaguar* drive1, CANJaguar* drive2,
		CANJaguar* drive3, CANJaguar* drive4, TKOGyro* gyro) {
	_angle = ang /* * REVS_PER_FOOT*/;
	_drive1 = drive1;
	_drive2 = drive2;
	_drive3 = drive3;
	_drive4 = drive4;
	_gryo = gyro;
}

Turn_Atom::~Turn_Atom() {
}

void Turn_Atom::resetEncoders() {
	_drive1->EnableControl(0);
	_drive3->EnableControl(0);
}

void Turn_Atom::run() {

	TKOGyro::inst()->reset();

	while (turn(_angle)) {

		//		_drive1->Set(9001/*this value is only for testing*/); //same, but for jag 3 since only 1 and 3 have encoders
		//		_drive2->Set(-_drive1->GetOutputVoltage() / _drive1->GetBusVoltage()); //sets second and fourth jags in slave mode
		//			
		//		_drive3->Set(9001/*this value is only for testing*/); //same, but for jag 3 since only 1 and 3 have encoders
		//		_drive4->Set(-_drive3->GetOutputVoltage() / _drive3->GetBusVoltage()); //sets second and fourth jags in slave mode

	}
}

bool Turn_Atom::turn(double target) {
	double currAngle = TKOGyro::inst()->GetAngle();

	if (_drive1->GetOutputCurrent() > DRIVE_CURRENT_CUTOFF
			|| _drive3->GetOutputCurrent() > DRIVE_CURRENT_CUTOFF) {
		printf("Crashed into wall...");
		return true;
	}

	if (currAngle < target - 1 && !(target < currAngle)) {
		printf("Turning right \n");
		_drive2->Set(0.7);
		_drive4->Set(0.7);
	}
	if (currAngle + 2.0 >= target) {
		if (currAngle - 2.0 <= target) {
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
	if (currAngle > target && !(target > currAngle)) {
		printf("Past target \n");
		_drive2->Set(-0.8);
		_drive4->Set(-0.8);
	}
	return false;

}
