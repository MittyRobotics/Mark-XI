#include "DriveAtom.h"
#include <cstring>

// add in the motors and jags some where near here
//This is a change

Drive_Atom::Drive_Atom(float feet, float distanceAlreadyTraveled, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4)
{
	const float REVS_PER_FOOT = 0.7652439024;
	_distance = feet * REVS_PER_FOOT; 
	_drive1 = drive1; 
	_drive2 = drive2;
	_drive3 = drive3; 
	_drive4 = drive4; 
}

Drive_Atom::~Drive_Atom(){}


void Drive_Atom::run()
{
	//don't forget to divide number of rotations by REVS_PER_FOOT in order to get feet traveled
	_drive1->EnableControl(0);
	_drive3->EnableControl(0);

	// do really cool things with the motors here
	// like drive in a straight line
	
	while(_drive1->GetPosition() != _distance  and _drive3->GetPosition() != _distance){
		_drive1->Set(_distance); //same, but for jag 3 since only 1 and 3 have encoders
		_drive2->Set(-_drive1->GetOutputVoltage() / _drive1->GetBusVoltage()); //sets second and fourth jags in slave mode
			
		_drive3->Set(_distance); //same, but for jag 3 since only 1 and 3 have encoders
		_drive4->Set(-_drive3->GetOutputVoltage() / _drive3->GetBusVoltage()); //sets second and fourth jags in slave mode
			
	}
	
	
}
