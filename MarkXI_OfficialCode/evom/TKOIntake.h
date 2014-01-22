//Last Edited by Arjun Biju
#ifndef __TKOINTAKE_H
#define __TKOINTAKE_H

#include "../Definitions.h"

//Code for intake roller

class TKOIntake {
public: 
	TKOIntake(int port1, int port2);
	~TKOIntake();
	void RollerMove(bool trigger);
private:
	CANJaguar _roller1, _roller2;
	
};

#endif
