#include "ShootAtom.h"
#include "../evom/StateMachine.h"

ShootAtom::ShootAtom()
{
	
}

ShootAtom::~ShootAtom()
{
	
}

void ShootAtom::run()
{
	printf("Starting to shoot auton\n");
	while (!StateMachine::armCanMove and DriverStation::GetInstance()->IsEnabled())
	{
		
	}
	printf("Done with manual fire\n");
	StateMachine::manualFire();
	Wait(1.);
}
