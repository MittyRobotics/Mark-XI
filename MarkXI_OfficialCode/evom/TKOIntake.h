//Last Edited by Arjun Biju
#ifndef __TKOINTAKE_H
#define __TKOINTAKE_H

#include "Definitions.h"

//Code for intake roller and arm movement

class TKOIntake {
public: 
	TKOIntake(int port1, int port2, int port3);
	~TKOIntake();
	void RollerMove(bool trigger, bool trigger2);
	void ArmMove(float y);  
private: 
	CANJaguar _roller1, _roller2;
	CANJaguar _arm;
};
#endif

