//Last edited by Ritwik Dutta
//on 01/04/2014
#include "TKOShooter.h"

TKOShooter *
  TKOShooter::tkoShooterInstance = NULL;
///Constructor for the tkoShooter class

TKOShooter::TKOShooter ()
{	
}

TKOShooter * TKOShooter::newShooterInstance ()
{
	if (!tkoShooterInstance) {
		printf("No tkoShooterInstance. Creating.");
		tkoShooterInstance = new TKOShooter;
	} 
}

void
TKOShooter::runShooterTask ()
{
}

int
TKOShooter::runStateMachine ()
{
  return 0;
}

void
TKOShooter::startShooter ()
{
}

void
TKOShooter::stopShooter ()
{
}

void
TKOShooter::launchShooter ()
{
}

TKOShooter::~TKOShooter ()
{
}
