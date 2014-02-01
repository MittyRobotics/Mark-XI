#ifndef _TURNATOM_H
#define _TURNATOM_H

#include "Atom.h"
#include "../component/TKOGyro.h"
#include "../Definitions.h"

class TurnAtom: public Atom {
public:
	// For angle, positive is (left/right) in relation to the front of the robot
	TurnAtom(float ang, CANJaguar* drive1, CANJaguar* drive2,
			CANJaguar* drive3, CANJaguar* drive4, TKOGyro* gyro);
	~TurnAtom();
	void run();
private:
	CANJaguar* _drive1;
	CANJaguar* _drive2;
	CANJaguar* _drive3;
	CANJaguar* _drive4;
	TKOGyro* _gryo;
	float _angle;
private:
	bool turn(double target);
	bool reachedTarget;
	void resetEncoders();

};

#endif
