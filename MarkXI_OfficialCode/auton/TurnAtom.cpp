#include "TurnAtom.h"
#include <cstring>

// add in the motors and jags some where near here

Turn_Atom::Turn_Atom(float ang, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4, TKOGyro* gyro)
{
	// const float REVS_PER_FOOT = 0.7652439024;
	_angle = ang /* * REVS_PER_FOOT*/; 
	_drive1 = drive1; 
	_drive2 = drive2;
	_drive3 = drive3; 
	_drive4 = drive4; 
	_gryo = gyro;
}

Turn_Atom::~Turn_Atom(){}


void Turn_Atom::run()
{
	
	
	// do really cool things with the motors here
	// like turn in circles here
	
	while(_gryo.GetAngle() != _angle){
		_drive1->Set(/*Set a var here idk what a good set point is*/); //same, but for jag 3 since only 1 and 3 have encoders
		_drive2->Set(-_drive1->GetOutputVoltage() / _drive1->GetBusVoltage()); //sets second and fourth jags in slave mode
			
		_drive3->Set(-/*Set a var here idk what a good set point is (remember that this is negitive of the other or you wont turn) */); //same, but for jag 3 since only 1 and 3 have encoders
		_drive4->Set(-_drive3->GetOutputVoltage() / _drive3->GetBusVoltage()); //sets second and fourth jags in slave mode
			
	}
	
	
}
