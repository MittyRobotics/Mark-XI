//Last edited by Vadim Korolik
//on 01/12/2014
#include "TKOShooter.h"

TKOShooter*  TKOShooter::_instance = NULL;

state_t run_state( state_t cur_state, instance_data_t *data ) {
    return state_table[ cur_state ]( data );
};

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
		printf("No tkoShooterInstance. Creating.");
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
	cur_state = STATE_INITIAL;
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
	instance_data_t data;
	cur_state = run_state( cur_state, &data );
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
