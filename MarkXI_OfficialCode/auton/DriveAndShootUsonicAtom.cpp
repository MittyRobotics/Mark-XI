#include "DriveAndShootUsonicAtom.h"
#include <cstring>

DriveAndShootUsonicAtom::DriveAndShootUsonicAtom(float tarD, AnalogChannel* usonicPointer, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4)
{
	tarDist = tarD;
	usonic = usonicPointer;
	_drive1 = drive1;
	_drive2 = drive2;
	_drive3 = drive3;
	_drive4 = drive4;
}

DriveAndShootUsonicAtom::~DriveAndShootUsonicAtom() {}

void DriveAndShootUsonicAtom::run() {
	
	//don't forget to divide number of rotations by REVS_PER_FOOT in order to get feet traveled

	while (usonic->GetVoltage() / ULTRASONIC_CONVERSION_TO_FEET > tarDist && DriverStation::GetInstance()->IsEnabled()) {

//		while(kP != ds->GetAnalogIn(1)*0.2) {
			//kP = ds->GetAnalogIn(1)*0.2;
//			_drive1->SetPID(kP, kI, kD);
//			_drive3->SetPID(kP, kI, kD);
//		}
//		while(kI != ds->GetAnalogIn(2)*0.0008) {
//			kI = ds->GetAnalogIn(2)*0.0008;
//			_drive1->SetPID(kP, kI, kD);
//			_drive3->SetPID(kP, kI, kD);
//		}
				
		_drive1->Set(_drive1->Get() - 5);
		_drive2->Set(_drive1->GetOutputVoltage() / _drive1->GetBusVoltage()); //sets second jag to slave			
		_drive3->Set(_drive3->Get() + 5);
		_drive4->Set(_drive3->GetOutputVoltage() / _drive3->GetBusVoltage()); //sets fourth jag to slave
		
	}
	StateMachine::manualFire();
	Wait(0.5);
	_drive1->DisableControl();
	_drive2->Set(0);
	_drive3->DisableControl();
	_drive4->Set(0);
	Wait(1.0); 
	
}
