//Last Edited by Arjun Biju
#include "TKOIntake.h"
/*
 * Initializes the 2 roller jaguars in percent Vbus mode
 *  \parm int port 1 - roller 1 Jaguar ID
 *  \parm int port 2- roller 2 Jaguar ID
 */

TKOIntake::TKOIntake(int port1, int port2, int port3, int module2, int channel1) :
	_switch1(module2, channel1),
	_roller1(port1, CANJaguar::kPercentVbus),
	_roller2(port2, CANJaguar::kPercentVbus),
	_arm(port3, CANJaguar::kPercentVbus){
}
TKOIntake::~TKOIntake() {
}

void TKOIntake::RollerMove() 
{
	if (stick3. GetRawButton(1)) {	//intake of ball.
		_roller1.Set(1);
		_roller2.Set(-1);
		_switch1.Get;
		if (_switch1 = true) {
			_roller1.Set(0);
			_roller2.Set(0);
					
		}
	}
	
	if (stick3. GetRawButton(2)) {  //passing a ball.
		_roller1.Set(-1);
		_roller2.Set(1);
	} else {
		_roller1.Set(0);
		_roller2.Set(0);
	}
}

void TKOIntake::ArmMove(float y) {
	_arm.Set(y);
}
void TKOIntake::Limitswitchrandomness()
{ 
	_switch1.Get;
	if (_switch1 = true)
	{ 
		printf("It worked! AMERICA!!!!");
	}
	
}

