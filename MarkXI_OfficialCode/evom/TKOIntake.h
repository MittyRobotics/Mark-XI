//Last Edited by Swag
#ifndef __TKOINTAKE_H
#define __TKOINTAKE_H

#include "../Definitions.h"

//Code for intake roller

class TKOIntake {
public: 
	TKOIntake(int rollerPort1, int rollerPort2);
	~TKOIntake();
	void rollerManualMove(Joystick stick);
private:
	CANJaguar _roller1, _roller2;
};

#endif
