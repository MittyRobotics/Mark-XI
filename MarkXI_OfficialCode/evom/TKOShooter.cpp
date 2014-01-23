//Last edited by Vadim Korolik
//on 01/12/2014
#include "TKOShooter.h"

TKOShooter*  TKOShooter::_instance = NULL;

TKOShooter::TKOShooter ()
{	
	shooterTask = new Task("Shooter", (FUNCPTR) shooterTaskRunner);
	stateMachineTask = new Task("StateMachine", (FUNCPTR) stateMachineTaskRunner);
	shooterTask->SetPriority(Task::kDefaultPriority);
	stateMachineTask->SetPriority(Task::kDefaultPriority);
}

TKOShooter* TKOShooter::inst ()
{
	if (!_instance) {
		printf("No TKOShooter instance. Creating.");
		_instance = new TKOShooter;
	}
	return _instance;
}

bool TKOShooter::Start()
{
	if (startStateMachine() and startShooter())
		return true;
	return false;
}
bool TKOShooter::Stop()
{
	if (stopStateMachine() and stopShooter())
			return true;
	return false;
}

void TKOShooter::initStateMachine()
{
	cur_state = s.init(&data);
}
bool TKOShooter::startStateMachine()
{
	initStateMachine();
	if (not stateMachineTask->Verify())
		if (stateMachineTask->Start())
			return true;
	
	return false;
}
bool TKOShooter::stopStateMachine()
{
	if (stateMachineTask->Verify())
		if (stateMachineTask->Stop())
			return true;
	
	return false;
}
int TKOShooter::runStateMachine()
{
	cur_state = s.run_state(cur_state,&data);
	return -1;
}

bool TKOShooter::startShooter()
{
	return false;
}
bool TKOShooter::stopShooter()
{
	return false;
}
bool TKOShooter::shooterDoAction(int action)
{
	return false;
}

void TKOShooter::shooterTaskRunner()
{
	while (true)
	{
		
	}
}
void TKOShooter::stateMachineTaskRunner()
{
	while (true)
	{
		_instance->runStateMachine();
	}
}

TKOShooter::~TKOShooter ()
{
}
