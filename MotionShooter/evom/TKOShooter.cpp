//Last edited by Vadim Korolik
//on 01/12/2014
#include "TKOShooter.h"

TKOShooter*  TKOShooter::_instance = NULL;

TKOShooter::TKOShooter ():
		stick1(1), stick2(2), stick3(3), stick4(4)
{	
	shooterTask = new Task("Shooter", (FUNCPTR) shooterTaskRunner, 50);
	stateMachineTask = new Task("StateMachine", (FUNCPTR) stateMachineTaskRunner, 50);
	/*if (shooterTask->SetPriority(4))
			printf("shooterTask priority set to 4\n");
		else
			printf("shooterTask priority not set\n");
	if (stateMachineTask->SetPriority(5))
			printf("stateMachineTask priority set to 5\n");
		else
			printf("stateMachineTask priority not set\n");*/
	//shooterTask->SetPriority(Task::kDefaultPriority);
	//stateMachineTask->SetPriority(Task::kDefaultPriority);
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
bool TKOShooter::stateMachineRunning()
{
	if (stateMachineTask->Verify())
		return true;
	return false;
}

state_t TKOShooter::initStateMachine(instance_data_t* temp_data)
{
	cur_state = s.init(temp_data, &stick3);
	return cur_state;
}
bool TKOShooter::startStateMachine()
{
	initStateMachine(&data);
	if (not stateMachineTask->Verify())
		if (stateMachineTask->Start())
			return true;
	
	return false;
}
bool TKOShooter::stopStateMachine()
{
	//if (stateMachineTask->Verify())
		if (stateMachineTask->Stop())
			return true;
	
	return false;
}
int TKOShooter::runStateMachine()
{
	DSLog(4, "");
	DSLog(5, "");
	DSLog(6, "");
	cur_state = s.run_state(cur_state,&data);
	DSLog(6, "State: %s", s.state_to_string(&data).c_str());
	//logging here
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
		Wait(0.05);
	}
}
void TKOShooter::stateMachineTaskRunner()
{
	while (true)
	{
		_instance->runStateMachine();
		Wait(0.05);
	}
}

TKOShooter::~TKOShooter ()
{
}
