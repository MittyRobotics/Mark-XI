//Last edited by Vadim Korolik
#include "ShootAtom.h"

ShootAtom::ShootAtom()
{
	
}

ShootAtom::~ShootAtom()
{
	
}

void ShootAtom::run() // TODO write this
{
	printf("Starting to shoot auton\n");
	while (!StateMachine::armCanMove and DriverStation::GetInstance()->IsEnabled())
	{
		
	}
	printf("Done with manual fire\n");
	StateMachine::manualFire();
	Wait(1.);
}
