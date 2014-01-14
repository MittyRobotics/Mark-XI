//Last Edited by Arjun Biju
#include "TKOIntake.h"
/*
 * Initializes the 2 roller jaguars in percent Vbus mode
 *  \parm int port 1 - roller 1 Jaguar ID
 *  \parm int port 2- roller 2 Jaguar ID
 */

TKOIntake::TKOIntake(int port_1,int port_2):
_roller1(port1, CANJaguar::kPercentVbus),
_roller2(port2, CANJaguar::kPercentVbus)
{}

TKOIntake::~TKOIntake() {}

void TKOIntake::RollerMove(bool trigger){
	if (trigger) {
		_roller1.Set(ROLLER_ON);
		_roller2.Set(ROLLER_ON);
	}
	else {
		_roller1.Set(ROLLER_OFF);
		_roller2.Set(ROLLER_OFF);
	}
}

	
