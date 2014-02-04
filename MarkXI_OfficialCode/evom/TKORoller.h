//Last Edited by Swag
#ifndef __TKOROLLER_H
#define __TKOROLLER_H

#include "../Definitions.h"

//Code for intake roller

class TKORoller {
public: 
	TKORoller(int rollerPort1, int rollerPort2);
	~TKORoller();
	void rollerManualMove();
private:
	CANJaguar _roller1, _roller2;
	Joystick stick3;
};

#endif
