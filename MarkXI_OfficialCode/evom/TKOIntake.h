//Last Edited by Arjun Biju
#ifndef __TKOINTAKE_H
#define __TKOINTAKE_H

#include "Definitions.h"

//Code for intake roller and arm movement

class TKOIntake {
public:
	TKOIntake(int port1, int port2, int port3);
	~TKOIntake();
	void RollerMove();
	void ArmMove(float y);
	void Limitswitchrandomness();
	DigitalInput limit1 (int module2, int channel1);
	~DigitalInput();
private:
	CANJaguar _roller1, _roller2;
	CANJaguar _arm;
	DigitalInput _switch1;
};
#endif

