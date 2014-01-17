//Last Edited by Arjun Biju
#include "TKOIntake.h"
/*
 * Initializes the 2 roller jaguars in percent Vbus mode
 *  \parm int port 1 - roller 1 Jaguar ID
 *  \parm int port 2- roller 2 Jaguar ID
 */

TKOIntake::TKOIntake(int port1,int port2, int port3):
_roller1(port1, CANJaguar::kPercentVbus),
_roller2(port2, CANJaguar::kPercentVbus),
_arm1(port3, CANJaguar::kPercentVbus)
{}

TKOIntake::~TKOIntake() {}

void TKOIntake::RollerMove(bool trigger, bool trigger2){
	if (stick3. GetRawButton(1)) {
		_roller1.Set(-1);
		_roller2.Set(-1);
	}
	
	if (stick3. GetRawButton(2)){
		_roller1.Set(1);
		_roller2.Set(1);
	}
	else { 
		_roller1.Set(0);
		_roller2.Set(0);
	}
}

void TKOIntake::ArmMove(float y){
	_arm.Set(y);
}
	
	
