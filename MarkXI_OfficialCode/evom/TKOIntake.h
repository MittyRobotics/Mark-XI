//Last Edited by Ishan Shah and Murad Awad
#ifndef __TKOINTAKE_H
#define __TKOINTAKE_H

#include "../Definitions.h"

//Code for intake roller and arm movement

class TKOIntake {
public:
TKOIntake();
~TKOIntake();
void RollerMove();
void ArmMove();
void LimitSwitchTest();
//DigitalInput limit1 (int module2, int channel1);
private:
CANJaguar _roller1, _roller2;
CANJaguar _arm;
DigitalInput limit1, limit2, limit3;
Joystick stick1, stick2, stick3, stick4;
};
#endif
