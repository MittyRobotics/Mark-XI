//Last edited by Ritwik Dutta
//on 01/04/2014
#include "TKOShooter.h"

TKOShooter*  TKOShooter::_instance = NULL;
///Constructor for the tkoShooter class

TKOShooter::TKOShooter ()
{	
}

TKOShooter* TKOShooter::newShooterInstance ()
{
	if (!_instance) {
		printf("No tkoShooterInstance. Creating.");
		_instance = new TKOShooter;
	}
	return _instance;
}

int TKOShooter::runStateMachine ()
{
	return 0;
}

void TKOShooter::startShooter ()
{
}

void TKOShooter::stopShooter ()
{
}

void TKOShooter::launchShooter ()
{
}

TKOShooter::~TKOShooter ()
{
}
