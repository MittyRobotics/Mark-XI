/*//Last edited by Vadim Korolik
//on 01/04/2014
#include "TKOShooter.h"

TKOShooter* TKOShooter::m_Instance = NULL;
///Constructor for the TKOShooter class

TKOShooter::TKOShooter()
{
}
TKOShooter* TKOShooter::inst()
{
	if (!m_Instance)
	{
		printf("TKOVision instance is null\n");
		m_Instance = new TKOShooter;
	}
	return m_Instance;
}

void TKOShooter::ShooterTaskRunner()
{
	while (true)
	{
		m_Instance->shooterProcess();
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
*/
