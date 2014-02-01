#ifndef _DRIVE_ATOM_H
#define _DRIVE_ATOM_H

#include "Atom.h"
#include "../Definitions.h"

class Drive_Atom: public Atom {

public:
	// For feet, positive is moving forward in relation to the robot
	Drive_Atom(float feet, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4);
	~Drive_Atom();
	void run();
	DriverStation *ds;
private:
	CANJaguar* _drive1;
	CANJaguar* _drive2;
	CANJaguar* _drive3;
	CANJaguar* _drive4;
	float _distance;

};

#endif
