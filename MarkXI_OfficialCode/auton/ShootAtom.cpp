#include "ShootAtom.h"

ShootAtom::ShootAtom()
{
	
}

ShootAtom::~ShootAtom()
{
	
}

void ShootAtom::run()
{
	if (!StateMachine::hasShot)
	{
		TKOShooter::inst()->Start();
		StateMachine::setAutonShoot(false);
		TKOShooter::inst()->Stop();
	}
}
