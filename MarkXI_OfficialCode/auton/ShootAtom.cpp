#include "ShootAtom.h"
#include "../evom/StateMachine.h"

ShootAtom::ShootAtom()
{
	
}

ShootAtom::~ShootAtom()
{
	
}

void ShootAtom::run()
{
	instance_data_t _data;
	state_t _cur_state;
	StateMachine s;
	bool _autonFired;
	_cur_state = TKOShooter::inst()->initStateMachine(&_data);
	while(DriverStation::GetInstance()->IsAutonomous() and DriverStation::GetInstance()->IsEnabled())
	{
		string state = s.state_to_string(&_data);
		if (state == "Ready to Fire" and StateMachine::armCanMove)
			s.forceFire = true;
		
		if (state == "ERROR STATE!!!")
			break;
		_cur_state = s.run_state(_cur_state,&_data);
		if (s.autonFired)
		{
			_autonFired = true;
			break;
		}
	}
}
