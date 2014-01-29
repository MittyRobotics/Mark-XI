//Last Edited by Zach Light
#include "TKOIntake.h"
/*
 * Initializes the 2 roller jaguars in percent Vbus mode
 *  \parm int port 1 - roller 1 Jaguar ID
 *  \parm int port 2- roller 2 Jaguar ID
 */

TKOIntake::TKOIntake(int port1,int port2):
_roller1(port1, CANJaguar::kPercentVbus),
_roller2(port2, CANJaguar::kPercentVbus)
{}

TKOIntake::~TKOIntake() {}

void TKOIntake::rollerManualMove(Joystick stick)
	{
		if(stick.GetRawButton(4))
		{
			_roller1.Set(stick.GetY());
			_roller1.Set(stick.GetY());
			_roller2.Set(-stick.GetY());
			_roller2.Set(-stick.GetY());
		}
	}

