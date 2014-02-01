//Last Edited by Ishan Shah and Alex Parks
#include "DriveAtom.h"
#include <cstring>
// new branch

Drive_Atom::Drive_Atom(float feet, CANJaguar* drive1, CANJaguar* drive2,
		CANJaguar* drive3, CANJaguar* drive4) {
	const float REVS_PER_FOOT = 0.7652439024;
	_distance = feet * REVS_PER_FOOT;
	_drive1 = drive1;
	_drive2 = drive2;
	_drive3 = drive3;
	_drive4 = drive4;
}

Drive_Atom::~Drive_Atom() {
}

void Drive_Atom::run() {
	ds = DriverStation::GetInstance();
	//don't forget to divide number of rotations by REVS_PER_FOOT in order to get feet traveled
	_drive1->EnableControl(0);
	_drive3->EnableControl(0);

	while ((_drive1->GetPosition() >= _distance - .2) or (_drive1->GetPosition() <= _distance + .2)
			and (_drive3->GetPosition() >= _distance - .2) or (_drive3->GetPosition() <= _distance + .2)
			and ds->IsEnabled()) {
		//need a +/- difference for distance
		printf("drive 1: %f, drive 3: %f, distance: %f\n",
				_drive1->GetPosition(), _drive3->GetPosition(), _distance);
		_drive1->Set(_distance);
		_drive2->Set(-_drive1->GetOutputVoltage() / _drive1->GetBusVoltage()); //sets second jag to slave			
		_drive3->Set(-(_distance));
		_drive4->Set(-_drive3->GetOutputVoltage() / _drive3->GetBusVoltage()); //sets fourth jag to slave
	}

}
