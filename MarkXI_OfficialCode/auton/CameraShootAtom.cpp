#include "CameraShootAtom.h"
#include <cstring>

CameraShootAtom::CameraShootAtom(AnalogChannel* usonicPointer)
{
	usonic = usonicPointer;
}

CameraShootAtom::~CameraShootAtom() {}

void CameraShootAtom::run() 
{
	TKOVision::inst()->StartProcessing();
	float _camTS = 0., _camDist = 0.;
	bool _camHot = false;
	while (DriverStation::GetInstance()->IsEnabled()) 
	{
		_camTS = TKOVision::inst()->getLastTimestamp();
		_camDist = TKOVision::inst()->getLastDistance();
		_camHot = TKOVision::inst()->getLastTargetReport().Hot;
		
		if (_camTS > 0.)
		{
			if (_camDist > 0.)
			{
				if (_camHot)
				{
					break;
				}
			}
		}
	}

	TKOVision::inst()->StopProcessing();
}
