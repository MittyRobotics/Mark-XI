//Last Edited by Arjun Biju
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
limit1 (1), // Lowest limit switch
limit2 (2), // Middle limit switch
limit3(3),// Top limit switch
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
if (limit1.Get() == 0) {
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


void TKOIntake::ArmMove(){ // Arm move code to move it to the lowest position
	{
while (stick4.GetRawButton(1));
_arm.Set(1); // The arm starts going in a downwards fashion. This can be changed if needed.
		if (limit1.Get()== 1);
		_arm.Set(0);
		printf ("The arm is at the lowest position.");
	}
	{
while (stick4.GetRawButton(2)); // Arm move to middle position
_arm.Set(1); //Arm first moves in downward fashion in case it is above the second limit switch
		if (limit1.Get()==1); //If it is under the second limit switch, it  hits the first one, and reverts to start moving up
		_arm.Set(-1);
			if (limit2.Get()==1); //If it reaches the second limit switch
			_arm.Set(0);
			printf ("The arm has reached the middle position.");
	}
	{
while (stick4.GetRawButton(3)); //Arm move to highest position
_arm.Set(-1);
	if (limit3.Get()==1); //The highest limit switch is triggered, and the arm is off.
	_arm.Set(0);
	printf ("The arm has reached the highest position");
	}
	}