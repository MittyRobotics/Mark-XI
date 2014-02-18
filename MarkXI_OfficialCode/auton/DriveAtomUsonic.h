#ifndef _DRIVEUSONICATOM_H
#define _DRIVEUSONICATOM_H

#include "Atom.h"
#include "../Definitions.h"

class DriveAtomUsonic: public Atom {

public:
	// For feet, positive is moving forward in relation to the robot
	DriveAtomUsonic(float tarD, AnalogChannel* usonicPointer, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4);
	~DriveAtomUsonic();
	float kP;
	float kI;
	float kD;
	void SetPID(float _kP, float _kI, float _kD);
	void SetP(float _kP);
	void SetI(float _kI);
	void SetD(float _kD);
	float GetP(); 
	float GetI(); 
	float GetD(); 
	void run();
private:
	float tarDist;
	CANJaguar* _drive1;
	CANJaguar* _drive2;
	CANJaguar* _drive3;
	CANJaguar* _drive4;
	AnalogChannel* usonic;

};

#endif
