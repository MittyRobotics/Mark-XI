//Last edited by Vadim Korolik
//on 03/01/14
#include "DriveAtomUsonic.h"
#include <cstring>

DriveAtomUsonic::DriveAtomUsonic(float tarD, AnalogChannel* usonicPointer, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4)
{
	tarDist = tarD;
	usonic = usonicPointer;
	_drive1 = drive1;
	_drive2 = drive2;
	_drive3 = drive3;
	_drive4 = drive4;
}

DriveAtomUsonic::~DriveAtomUsonic() {
}

void DriveAtomUsonic::run() {
	
	//don't forget to divide number of rotations by REVS_PER_FOOT in order to get feet traveled
	float tarDistE = 18. - tarDist; //We need to drive until we pass x feet, so that we are tarDist away from wall (starting position 18 feet)
	float AUTON_DRIVE_MAXOUTPUT_VOLTAGE = 12.;
	_drive1->SetPID(-500., DRIVE_kI, DRIVE_kD);
	_drive3->SetPID(-500., DRIVE_kI, DRIVE_kD);
	_drive1->ConfigMaxOutputVoltage(AUTON_DRIVE_MAXOUTPUT_VOLTAGE);
	_drive2->ConfigMaxOutputVoltage(AUTON_DRIVE_MAXOUTPUT_VOLTAGE);
	_drive3->ConfigMaxOutputVoltage(AUTON_DRIVE_MAXOUTPUT_VOLTAGE);
	_drive4->ConfigMaxOutputVoltage(AUTON_DRIVE_MAXOUTPUT_VOLTAGE);
	_drive1->EnableControl(0);
	_drive3->EnableControl(0);
	while ((usonic->GetVoltage() / ULTRASONIC_CONVERSION_TO_FEET > tarDist and (_drive1->GetPosition() > -tarDistE + 0.1 and _drive3->GetPosition() < tarDistE - 0.1))&& DriverStation::GetInstance()->IsEnabled()) 
	{
		//while current real distance is over target distance and ultrasonic confirms
		_drive1->Set(_drive1->Get() - 0.1);		
		_drive3->Set(_drive3->Get() + 0.1);
		_drive2->Set(_drive1->GetOutputVoltage() / _drive1->GetBusVoltage()); //sets second jag to slave	
		_drive4->Set(_drive3->GetOutputVoltage() / _drive3->GetBusVoltage()); //sets fourth jag to slave
		
		while (true)
		{
			if (_drive1->GetPosition() < (_drive1->Get() + 0.01))
				break;
			if (_drive3->GetPosition() > (_drive3->Get() - 0.01))
				break;
			//while we havent reached our new small samped target, dont do anything. Otherwise ramp again
		}
		
	}
	_drive1->DisableControl();
	_drive2->Set(0);
	_drive3->DisableControl();
	_drive4->Set(0);
	Wait(1.0); 
	
}

/*void DriveAtomUsonic::run() {
	
	//don't forget to divide number of rotations by REVS_PER_FOOT in order to get feet traveled
	_drive1->SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	_drive3->SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	_drive1->EnableControl(0);
	_drive3->EnableControl(0);
	while (usonic->GetVoltage() / ULTRASONIC_CONVERSION_TO_FEET > tarDist && DriverStation::GetInstance()->IsEnabled()) {
		//while current distance is greater than target distance

		_drive1->Set(_drive1->Get() - DRIVE_POSITION_INCREMENT);
		_drive2->Set(_drive1->GetOutputVoltage() / _drive1->GetBusVoltage()); //sets second jag to slave			
		_drive3->Set(_drive3->Get() + DRIVE_POSITION_INCREMENT);
		_drive4->Set(_drive3->GetOutputVoltage() / _drive3->GetBusVoltage()); //sets fourth jag to slave
		
	}
	_drive1->DisableControl();
	_drive2->Set(0);
	_drive3->DisableControl();
	_drive4->Set(0);
	Wait(1.0); 
	
}*/
