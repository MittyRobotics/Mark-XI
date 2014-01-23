//Last Edited by Ishan Shah and Murad Awad
#include "TKOIntake.h"

/*
* Initializes the 2 roller jaguars in percent Vbus mode
*  \parm int port 1 - roller 1 Jaguar ID
*  \parm int port 2- roller 2 Jaguar ID
*/

TKOIntake::TKOIntake():
_roller1(6, CANJaguar::kPercentVbus),
_roller2(7, CANJaguar::kPercentVbus),
_arm(8, CANJaguar::kPercentVbus),
//_switch1(2,1),
limit1 (1), 
stick1(STICK_1_PORT), // initialize joystick 1 < first drive joystick
stick2(STICK_2_PORT), // initialize joystick 2 < second drive joystick
stick3(STICK_3_PORT), // initialize joystick 3 < first EVOM joystick
stick4(STICK_4_PORT) // initialize joystick 4 < second EVOM joystick-m,

{}

TKOIntake::~TKOIntake() {}


void TKOIntake::RollerMove() 
{
if (stick3.GetRawButton(1)) {	//intake of ball.
_roller1.Set(1);
_roller2.Set(-1);
if (limit1.Get() == true) {
_roller1.Set(0);
_roller2.Set(0);

}
}

if (stick3.GetRawButton(2)) {  //passing a ball.
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


void TKOIntake::LimitSwitchTest()
{ 
if (limit1.Get() == 0)
{ 
printf("Sensor tripped \n");
}


}
