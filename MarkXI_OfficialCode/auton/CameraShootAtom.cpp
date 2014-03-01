#include "CameraShootAtom.h"
#include <cstring>

CameraShootAtom::CameraShootAtom(AnalogChannel* usonicPointer, CANJaguar* drive1, CANJaguar* drive2, CANJaguar* drive3, CANJaguar* drive4)
{
	usonic = usonicPointer;
	_drive1 = drive1;
	_drive2 = drive2;
	_drive3 = drive3;
	_drive4 = drive4;
}

CameraShootAtom::~CameraShootAtom() {}

void CameraShootAtom::run() 
{
	TKOVision::inst()->StartProcessing();
	float _camTS, _camDist;
	bool _camHot;
	while (DriverStation::GetInstance()->IsEnabled()) 
	{
		_camTS = TKOVision::inst()->getLastTimestamp();
		_camDist = TKOVision::inst()->getLastDistance();
		_camHot = TKOVision::inst()->getLastTargetReport().Hot;
		
		
	}
	printf("Reached target, firing\n");
	StateMachine::manualFire();
	printf("Done firing\n");
	Wait(0.5);
	_drive1->DisableControl();
	_drive2->Set(0);
	_drive3->DisableControl();
	_drive4->Set(0);
	Wait(1.0); 
	TKOVision::inst()->StopProcessing();
}
