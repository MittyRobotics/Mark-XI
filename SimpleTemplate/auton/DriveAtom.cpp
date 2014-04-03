//Last edited by Ishan Shah
#include "DriveAtom.h"
#include <cstring>

DriveAtom::DriveAtom(float feet, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4) {
	_distance = feet; //* REVS_PER_FOOT;
	_drive1 = drive1;
	_drive2 = drive2;
	_drive3 = drive3;
	_drive4 = drive4;
	ds = DriverStation::GetInstance();
}

DriveAtom::~DriveAtom() {
}

void DriveAtom::run()
{
	TKOLogger::inst()->addMessage("Starting run for driving, auton shot distance");
	Timer test;
	//don't forget to divide number of rotations by REVS_PER_FOOT in order to get feet traveled
	_drive1->SetPID(-1000, -1., DRIVE_kD);
	_drive3->SetPID(-1000, -1., DRIVE_kD);
	_drive1->EnableControl(0);
	_drive3->EnableControl(0);
	test.Start();
	while (_drive1->Get() > -_distance and _drive3->Get() < _distance and DriverStation::GetInstance()->IsEnabled()) 
	{
		_drive1->Set(_drive1->Get() - DRIVE_POSITION_INCREMENT);
		_drive2->Set(_drive1->GetOutputVoltage() / _drive1->GetBusVoltage()); //sets second jag to slave			
		_drive3->Set(_drive3->Get() + DRIVE_POSITION_INCREMENT);
		_drive4->Set(_drive3->GetOutputVoltage() / _drive3->GetBusVoltage()); //sets fourth jag to slave
		//printf("Drive 1: %f\t Drive 3: %f\n", _drive1->GetSpeed(), _drive3->GetSpeed());
		printf("Timer: %f\n", test.Get());
	}
	TKOLogger::inst()->addMessage("Done ramping drive targets");
	TKOLogger::inst()->addMessage("Drive 1: %f\t Drive 3: %f\n", _drive1->GetPosition(), _drive3->GetPosition());
	TKOLogger::inst()->addMessage("Drive 2: %f\t Drive 4: %f\n", _drive2->GetPosition(), _drive4->GetPosition());
		
	while (_drive1->GetPosition() > -_distance + .5 and _drive3->GetPosition() < _distance - .5 and DriverStation::GetInstance()->IsEnabled())
	{
		_drive1->Set(_drive1->Get());
		_drive2->Set(_drive1->GetOutputVoltage() / _drive1->GetBusVoltage()); //sets second jag to slave			
		_drive3->Set(_drive3->Get());
		_drive4->Set(_drive3->GetOutputVoltage() / _drive3->GetBusVoltage()); //sets fourth jag to slave
	}

	TKOLogger::inst()->addMessage("Drive past targets");
	TKOLogger::inst()->addMessage("Drive 1: %f\t Drive 3: %f\n", _drive1->GetPosition(), _drive3->GetPosition());
	TKOLogger::inst()->addMessage("Drive 2: %f\t Drive 4: %f\n", _drive2->GetPosition(), _drive4->GetPosition());
	test.Stop();
	TKOLogger::inst()->addMessage("Reached target, stopping");
	printf("Reached target, firing\n");
	//Wait(0.5);
	_drive1->DisableControl();
	_drive2->Set(0);
	_drive3->DisableControl();
	_drive4->Set(0);
	Wait(1.0); 
}
