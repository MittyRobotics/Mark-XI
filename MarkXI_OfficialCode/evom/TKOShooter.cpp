//Last edited by Vadim Korolik
//on 01/04/2014
#include "TKOShooter.h"

TKOShooter* TKOShooter::tkoShooterInstance = NULL;
///Constructor for the TKOShooter class

TKOShooter::TKOShooter()
{
}
TKOShooter* TKOShooter::inst()
{
	if (!tkoShooterInstance)
	{
		printf("TKOVision instance is null\n");
		tkoShooterInstance = new TKOShooter;
	}
	return tkoShooterInstance;
}

void TKOShooter::ShooterTaskRunner()
{
	while (true)
	{
		tkoShooterInstance->shooterProcess();
	}
}

void TKOShooter::shooterProcess()
{
	if (not needToShoot)
		return;
	if (shootTarget == 0)
		return;
	
	needToShoot = true;
	shootTarget = 0;
}

void TKOShooter::Start()
{
	shooterTask->Start();
}

void TKOShooter::Stop()
{
	if (shooterTask->Verify())
		shooterTask->Stop();
}

void TKOShooter::shootDist(double distance)
{
	shootTarget = distance;
	needToShoot = true;
}

///Destructor for the TKOAutonomous class
TKOShooter::~TKOShooter()
{
}
