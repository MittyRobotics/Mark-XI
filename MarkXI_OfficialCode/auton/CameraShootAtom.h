#ifndef _CAMERASHOOTATOM_H
#define _CAMERASHOOTATOM_H

#include "Atom.h"
#include "../Definitions.h"
#include "../evom/StateMachine.h"
#include "../vision/TKOVision.h"

class CameraShootAtom: public Atom {

public:
	// For feet, positive is moving forward in relation to the robot
	CameraShootAtom(AnalogChannel* usonicPointer, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4);
	~CameraShootAtom();
	void run();
private:
	CANJaguar* _drive1;
	CANJaguar* _drive2;
	CANJaguar* _drive3;
	CANJaguar* _drive4;
	AnalogChannel* usonic;
};

#endif
