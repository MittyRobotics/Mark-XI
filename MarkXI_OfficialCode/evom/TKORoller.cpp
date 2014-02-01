//Last Edited by Zach Light
#include "TKORoller.h"
/*
 * Initializes the 2 roller jaguars in percent Vbus mode
 *  \parm int port 1 - roller 1 Jaguar ID
 *  \parm int port 2- roller 2 Jaguar ID
 */

TKORoller::TKORoller(int rollerPort1,int rollerPort2):
_roller1(rollerPort1, CANJaguar::kPercentVbus),
_roller2(rollerPort2, CANJaguar::kPercentVbus),
stick3(3)
{}

TKORoller::~TKORoller() {}

void TKORoller::rollerManualMove()
	{
		if(stick3.GetRawButton(4))
		{
			_roller1.Set(stick3.GetY());
			_roller1.Set(stick3.GetY());
			_roller2.Set(-stick3.GetY());
			_roller2.Set(-stick3.GetY());
		}
		printf("%f %f Current currents:", _roller1.GetOutputCurrent(), _roller2.GetOutputCurrent());
	}

