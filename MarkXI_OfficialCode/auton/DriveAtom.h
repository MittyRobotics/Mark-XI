#ifndef _DRIVEATOM_H
#define _DRIVEATOM_H

#include "Atom.h"
#include "../Definitions.h"

class DriveAtom: public Atom {

public:
	// For feet, positive is moving forward in relation to the robot
	DriveAtom(float feet, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4);
	~DriveAtom();
	float kP;
	float kI;
	float kD;
	DriverStation* ds;
	void SetPID(float _kP, float _kI, float _kD);
	void SetP(float _kP);
	void SetI(float _kI);
	void SetD(float _kD);
	float GetP(); 
	float GetI(); 
	float GetD(); 
	void run();
private:
	CANJaguar* _drive1;
	CANJaguar* _drive2;
	CANJaguar* _drive3;
	CANJaguar* _drive4;
	float _distance;

};

#endif
