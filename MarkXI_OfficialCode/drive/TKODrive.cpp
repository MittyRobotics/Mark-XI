//Last edited by Vadim Korolik
//on 11/27/2013
#include "TKODrive.h"

TKODrive* TKODrive::m_Instance = NULL;

TKODrive::TKODrive()
{	
	printf("Initializing drive\n");
	driveTask = new Task("Driving", (FUNCPTR) DriveRunner);
	driveTask->SetPriority(5);
	printf("Finished initializing drive\n");
}
TKODrive* TKODrive::inst()
{
	if (!m_Instance)
	{
		printf("Drive instance is null\n");
		m_Instance = new TKODrive;
	}
	return m_Instance;
}
void TKODrive::DriveRunner()
{
	while (true)
	{
		m_Instance->TankDrive();
//		m_Instance->VerifyJags();
		Wait(0.005);
	}
}

TKODrive::~TKODrive()
{
	driveTask->Stop();
}

