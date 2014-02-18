//Last Edited by Ishan Shah
#include "DriveAtomUsonic.h"
#include <cstring>
// new branch
DriveAtomUsonic::DriveAtomUsonic(float tarD, AnalogChannel* usonicPointer, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4)
{
	tarDist = tarD;
	usonic = usonicPointer;
	_drive1 = drive1;
	_drive2 = drive2;
	_drive3 = drive3;
	_drive4 = drive4;
//	kP = 0.7;
//	kI = 0.00;//23;
//	kD = 0.0;
}

DriveAtomUsonic::~DriveAtomUsonic() {
}

void DriveAtomUsonic::run() {
	
	
	//don't forget to divide nu--mber of rotations by REVS_PER_FOOT in order to get feet traveled

	_drive1->SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	_drive3->SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	_drive1->EnableControl(0);
	_drive3->EnableControl(0);
	while (usonic->GetVoltage() / 0.009765625 / 12. > tarDist && DriverStation::GetInstance()->IsEnabled()) {

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
	_drive1->DisableControl();
	_drive2->Set(0);
	_drive3->DisableControl();
	_drive4->Set(0);
	Wait(1.0); 
	
	
}
