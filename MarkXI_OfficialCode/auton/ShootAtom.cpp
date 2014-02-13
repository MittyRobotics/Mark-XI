#include "ShootAtom.h"

ShootAtom::ShootAtom()
{
	
}

ShootAtom::~ShootAtom()
{
	
}

void ShootAtom::run()
{
	if (!StateMachine::canAutonShoot())
	{
		TKOShooter::inst()->Start();
		StateMachine::setAutonShoot(false);
		TKOShooter::inst()->Stop();
	}
}
