//Last edited by Vadim Korolik
//on 01/12/2014
#include "TKOShooter.h"

TKOShooter*  TKOShooter::_instance = NULL;

state_t do_state_initial( instance_data_t *data );
state_t do_state_foo( instance_data_t *data );
state_t do_state_bar( instance_data_t *data );

state_func_t* const state_table[ NUM_STATES ] = {
    do_state_initial, do_state_foo, do_state_bar
};

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
	return false;
}
bool TKOShooter::Stop()
{
	return false;
}

bool TKOShooter::startStateMachine()
{
	return false;
}
bool TKOShooter::stopStateMachine()
{
	return false;
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
	
}

TKOShooter::~TKOShooter ()
{
}
