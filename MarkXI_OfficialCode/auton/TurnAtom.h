#ifndef _TURN_ATOM_H
#define _TURN_ATOM_H

#include "Atom.h"
#include "../component/TKOGyro.h"
#include "../Definitions.h"

class Turn_Atom : public Atom{
	
	// add in the motors and jags some where
	
	public:
	// For angle, positive is (left/right) in relation to the front of the robot
	Turn_Atom(float ang, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4);
	~Turn_Atom();
	void run();
	private:
	CANJaguar* _drive1;
	CANJaguar* _drive2;
	CANJaguar* _drive3;
	CANJaguar* _drive4;
	TKOGryo* _gryo;
	float _angle;
		
};

#endif
