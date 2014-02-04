//Last Edited by Zach Light
#include "TKORoller.h"
/*
 * Initializes the 2 roller jaguars in percent Vbus mode
 *  \parm int port 1 - roller 1 Jaguar ID
 *  \parm int port 2- roller 2 Jaguar ID
 */

//roller is here ya

TKORoller::TKORoller(int rollerPort1, int rollerPort2):
	_roller1(rollerPort1, CANJaguar::kPercentVbus),
	_roller2(rollerPort2, CANJaguar::kPercentVbus), stick3(3),
	rollerTop(4), rollerBottom(5)	// currently dummy values
{
}

TKORoller::~TKORoller() {
}

void TKORoller::rollerManualMove() {
	if (stick3.GetRawButton(4)) {
		_roller1.Set(stick3.GetY());
		_roller1.Set(stick3.GetY());
		_roller2.Set(-stick3.GetY());
		_roller2.Set(-stick3.GetY());
	}
	if (rollerTop.Get() == 0 && rollerBottom.Get() == 0) {	// both should detect the ball
		_roller1.Set(0);
		_roller1.Set(0);
		_roller2.Set(0);
		_roller2.Set(0);
	}
	printf("%f %f Current currents:", _roller1.GetOutputCurrent(), _roller2.GetOutputCurrent());
}

