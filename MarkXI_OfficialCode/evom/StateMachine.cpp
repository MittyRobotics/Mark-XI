//Last edited by Vadim Korolik
//on 02/06/2014
//@author Matthew Pleva

/*
 * TODO 6
 * arm must be in fire pos or shouldn't fire (btn for that)???
 * also arm doesnt move unless this is in ready to fire state
 * change the timings for that while loops
 * check state in ready to fire
 * */

#include "StateMachine.h"

Timer* StateMachine::_timer = new Timer();

DigitalInput* StateMachine::_piston_retract = new DigitalInput(PISTON_SWITCH_RETRACT_CHANNEL);
DigitalInput* StateMachine::_piston_extend = new DigitalInput(PISTON_SWITCH_EXTEND_CHANNEL);
DigitalInput* StateMachine::_latch_lock = new DigitalInput(LATCH_PISTON_LOCK_CHANNEL);
DigitalInput* StateMachine::_is_cocked = new DigitalInput(IS_COCKED_SWITCH_CHANNEL);
Joystick* StateMachine::_triggerJoystick = NULL;

DoubleSolenoid* StateMachine::_piston_retract_extend = new DoubleSolenoid(PISTON_RETRACT_SOLENOID_A, PISTON_RETRACT_SOLENOID_B);
DoubleSolenoid* StateMachine::_latch_lock_unlock = new DoubleSolenoid(LATCH_RETRACT_SOLENOID_A, LATCH_RETRACT_SOLENOID_B);
float StateMachine::lastSensorStringPrint = 0.;
bool StateMachine::armCanMove = false;
SEM_ID StateMachine::_armSem = semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);

StateMachine::StateMachine()
{
    _state_table[STATE_PISTON_RETRACT] = do_state_piston_retract;
    _state_table[STATE_LATCH_LOCK] = do_state_latch_lock;
    _state_table[STATE_PISTON_EXTEND] = do_state_piston_extend;
    _state_table[STATE_READY_TO_FIRE] = do_state_ready_to_fire;
    _state_table[STATE_LATCH_UNLOCK] = do_state_latch_unlock;
    _state_table[STATE_ERR] = do_err_state;
    
    lastSensorStringPrint = GetTime();
}

StateMachine::~StateMachine()
{
	semDelete(_armSem);
}

bool StateMachine::canArmMove()
{
	bool tmp; 
	{
		Synchronized sem(_armSem);
		tmp = armCanMove;
	}
	return tmp;
}

void StateMachine::setArmMoveable(bool tmp)
{
	{
		Synchronized sem(_armSem);
		armCanMove = tmp;
	}
}

state_t StateMachine::run_state( state_t cur_state, instance_data_t *data ) {
    return _state_table[ cur_state ]( data );
};


int StateMachine::getSensorData(instance_data_t *data)
{
    // TODO what is off or on in terms of numbers?
    data->state[0] = (_piston_retract->Get() == 0);
    data->state[1] = (_piston_extend->Get() == 0);
    data->state[2] = (_latch_lock->Get() == 0);
    data->state[3] = (_is_cocked->Get() == 0);
    
    DriverStation::GetInstance()->SetDigitalOut(4, !_is_cocked->Get());
    DriverStation::GetInstance()->SetDigitalOut(5, !_latch_lock->Get());
    DriverStation::GetInstance()->SetDigitalOut(6, !_piston_extend->Get());
    DriverStation::GetInstance()->SetDigitalOut(7, !_piston_retract->Get());

    return createIntFromBoolArray(data);
}

int StateMachine::createIntFromBoolArray(instance_data_t *data)
{
    int i = 0;
    int num = 0;
    for (; i < NUM_STATES - 1; i++) {
        if (data->state[i]) {
            num |= 1 << i;
        }
    }
    return num;
}

state_t StateMachine::init(instance_data_t *data, Joystick *stick3)
{
	_triggerJoystick = stick3;
    int sensors = getSensorData(data);
    printf("Initializing state machine \n");
    TKOLogger::inst()->addMessage("Initializing state machine");
    //sensors_to_string(data);
    printf("\n %d \n", sensors);
    TKOLogger::inst()->addMessage("\n Sensors at initialization: %d \n", sensors);
    
    switch (sensors) {
      case DONE_FIRING:
        return STATE_PISTON_RETRACT;
        break;
      case PISTON_RETRACTED:
    	return STATE_LATCH_LOCK;
        break;
      case LATCH_LOCKED_PISTON_RETRACTED:
    	return STATE_PISTON_EXTEND;
        break;
      case CONST_READY_TO_FIRE:
    	return STATE_READY_TO_FIRE;
        break;
      default:
    	return STATE_ERR;
        break;
    }
}

state_t StateMachine::do_state_piston_retract(instance_data_t *data)
{
	setArmMoveable(false);
	TKOLogger::inst()->addMessage("STATE ENTER Piston retract; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
    // reason is that 0b0010 = 2 is piston extended
    if (createIntFromBoolArray(data) != DONE_FIRING) {
    	TKOLogger::inst()->addMessage("STATE ERROR ENTER Piston retract; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
        return STATE_ERR;
    }
    data->curState = STATE_PISTON_RETRACT;
    _timer->Start();

    _piston_retract_extend->Set(DoubleSolenoid::kReverse);
    TKOLogger::inst()->addMessage("STATE ACTION Piston retract; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));

    int sensors = 0;
    // reason for 8 is that piston is retracted then
    while (sensors = getSensorData(data), sensors != PISTON_RETRACTED && (sensors == 0 || sensors == DONE_FIRING) ) {
    	printf("Piston Retract running: %d\n", sensors != PISTON_RETRACTED && (sensors == 0 || sensors == DONE_FIRING));
        if (_timer->Get() > PISTON_RETRACT_TIMEOUT) {
            _timer->Stop();
            _timer->Reset();
            TKOLogger::inst()->addMessage("STATE ERROR TIMEOUT Piston retract; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
            return STATE_ERR;
        }
    }

    _timer->Stop();
    _timer->Reset();

    if (sensors != PISTON_RETRACTED)
    {
    	TKOLogger::inst()->addMessage("STATE ERROR EXIT Piston retract; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
        return STATE_ERR;
    }
    
    TKOLogger::inst()->addMessage("STATE SUCCESS EXIT Piston retract; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
    return STATE_LATCH_LOCK;
}

state_t StateMachine::do_state_latch_lock(instance_data_t * data)
{
	TKOLogger::inst()->addMessage("STATE ENTER Latch lock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
	setArmMoveable(false);
    // reason is that 0b0100 = 4 is piston extended
    if (createIntFromBoolArray(data) != PISTON_RETRACTED) {
    	TKOLogger::inst()->addMessage("STATE ERROR ENTER Latch lock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
        return STATE_ERR;
    }
    data->curState = STATE_LATCH_LOCK;
    _timer->Start();

    _latch_lock_unlock->Set(DoubleSolenoid::kForward);
    TKOLogger::inst()->addMessage("STATE ACTION Latch lock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));

    int sensors = 0;

    // reason for 8 is that piston is retracted then
    while (sensors = getSensorData(data), sensors != LATCH_LOCKED_PISTON_RETRACTED && (sensors == PISTON_RETRACTED)) {
    	printf("latch_lock running: %d\n", sensors != LATCH_LOCKED_PISTON_RETRACTED && (sensors == PISTON_RETRACTED));
    	      
    	if (_timer->Get() > LATCH_LOCK_FORWARD_TIMEOUT) {
            _timer->Stop();
            _timer->Reset();
            TKOLogger::inst()->addMessage("STATE ERROR TIMEOUT Latch lock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
            return STATE_ERR;
        }
    }
    _timer->Stop();
    _timer->Reset();

    if (sensors != LATCH_LOCKED_PISTON_RETRACTED)
    {
    	TKOLogger::inst()->addMessage("STATE ERROR EXIT Latch lock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
        return STATE_ERR;
    }

    TKOLogger::inst()->addMessage("STATE SUCCESS EXIT Latch lock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
    return STATE_PISTON_EXTEND;
}

state_t StateMachine::do_state_piston_extend(instance_data_t * data)
{
	TKOLogger::inst()->addMessage("STATE ENTER Piston extend; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
	setArmMoveable(false);
	//TODO DOES NOT LEAVE EXTEND WHEN 1110
	
    // reason is that 0b0100 = 4 is piston extended
    if (createIntFromBoolArray(data) != LATCH_LOCKED_PISTON_RETRACTED) {
    	TKOLogger::inst()->addMessage("STATE ERROR ENTER Piston extend; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
        return STATE_ERR;
    }
    data->curState = STATE_PISTON_EXTEND;
    _timer->Start();

    _piston_retract_extend->Set(DoubleSolenoid::kForward);
    TKOLogger::inst()->addMessage("STATE ACTION Piston extend; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));

    int sensors = 0;

    // reason for 8 is that piston is retracted then
    while (sensors = getSensorData(data), sensors != CONST_READY_TO_FIRE && (sensors == 12 || sensors == LATCH_LOCKED_PISTON_RETRACTED || sensors == 4 || sensors == 6)) {
    	printf("piston_extend running: %d\n", sensors != CONST_READY_TO_FIRE && (sensors == 12 || sensors == LATCH_LOCKED_PISTON_RETRACTED || sensors == 4 || sensors == 6));
    	    	 
    	if (_timer->Get() > PISTON_EXTEND_TIMEOUT) {
            _timer->Stop();
            _timer->Reset();
            TKOLogger::inst()->addMessage("STATE ERROR TIMEOUT Piston extend; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
            return STATE_ERR;
        }
    }
    _timer->Stop();
    _timer->Reset();

    if (sensors != CONST_READY_TO_FIRE)
    {
    	TKOLogger::inst()->addMessage("STATE ERROR EXIT Piston extend; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
        return STATE_ERR;
    }

    TKOLogger::inst()->addMessage("STATE SUCCESS EXIT Piston extend; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
    return STATE_READY_TO_FIRE;
}

state_t StateMachine::do_state_ready_to_fire(instance_data_t * data)
{
	setArmMoveable(false);
	TKOLogger::inst()->addMessage("STATE ENTER Ready to Fire; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
    // reason is that 0b0111 = 7 is piston extended, is cocked, and latch locked
    if (createIntFromBoolArray(data) != CONST_READY_TO_FIRE) {
    	TKOLogger::inst()->addMessage("STATE ERROR ENTER Ready to Fire; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
        return STATE_ERR;
    }
    
    data->curState = STATE_READY_TO_FIRE;

	setArmMoveable(true);
    
    // wait for the trigger then fire!
    while (!_triggerJoystick->GetTrigger() && getSensorData(data) == CONST_READY_TO_FIRE)
    {
    	printf("waiting for trigger\n");
    }
    printf("trigger pressed");
    // go to next state
    TKOLogger::inst()->addMessage("STATE SUCCESS EXIT Ready to Fire; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
    return STATE_LATCH_UNLOCK;
}

state_t StateMachine::do_state_latch_unlock(instance_data_t * data)
{
	setArmMoveable(false);
	TKOLogger::inst()->addMessage("STATE ENTER Latch Unlock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
    // reason is that 0b0111 = 7 is piston extended, is cocked, and latch locked
    if (createIntFromBoolArray(data) != CONST_READY_TO_FIRE) {
    	TKOLogger::inst()->addMessage("STATE ERROR ENTER Latch Unlock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
        return STATE_ERR;
    }
    
    data->curState = STATE_LATCH_UNLOCK;
    
    _timer->Start();

    _latch_lock_unlock->Set(DoubleSolenoid::kReverse);
    TKOLogger::inst()->addMessage("STATE ACTION Latch Unlock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));

    int sensors = 0;

    // reason for 4 is that piston is extended after this step
    while (sensors = getSensorData(data), sensors != DONE_FIRING && (sensors == CONST_READY_TO_FIRE || sensors == 10))
    {
        if (_timer->Get() > LATCH_UNLOCK_REVERSE_TIMEOUT) {
            _timer->Stop();
            _timer->Reset();
            TKOLogger::inst()->addMessage("STATE ERROR TIMEOUT Latch Unlock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
            
            return STATE_ERR;
        }
    }
    _timer->Stop();
    _timer->Reset();

    if (sensors != DONE_FIRING)
    {
    	TKOLogger::inst()->addMessage("STATE ERROR EXIT Latch Unlock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
        return STATE_ERR;
    }

    TKOLogger::inst()->addMessage("STATE SUCCESS EXIT Latch Unlock; state: %s; sensors: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
    return STATE_PISTON_RETRACT;
}

string StateMachine::state_to_string(instance_data_t *data)
{
    switch (data->curState) {
        case STATE_PISTON_RETRACT:
            return "Piston Retract";
            break;

        case STATE_LATCH_LOCK:
            return "Latch Lock";
            break;

        case STATE_PISTON_EXTEND:
            return "Piston Extend";
            break;

        case STATE_READY_TO_FIRE:
            return "Ready to Fire";
            break;

        case STATE_LATCH_UNLOCK:
            return "Latch Unlock";
            break;

        case STATE_ERR:
		    return "ERROR STATE!!!";
		    break;
		    
        default:
            return "POTATO!";
            break;
    }
}

void StateMachine::sensors_to_string(instance_data_t *data)
{
	if (GetTime() - lastSensorStringPrint <= 1.) return;
	
	TKOLogger::inst()->addMessage("State: %s",state_to_string(data).c_str());
	
    printf("0b (ic) (ll) (Pe) (Pr)\n0b");
    int sensors = createIntFromBoolArray(data);
    int i = NUM_STATES-2;
    for (; i > -1; i--) {
        printf("  %2d ",(sensors & (1 << i)));
    }
    
    lastSensorStringPrint = GetTime();
}

state_t StateMachine::do_err_state(instance_data_t *data)
{
	//GetSensorData(data);
	if (GetTime() - lastSensorStringPrint > 1.)
	{
		printf("%s\n",state_to_string(data).c_str());
		TKOLogger::inst()->addMessage("STATE ERROR: %s ERROR!!! SENSORS: %d", state_to_string(data).c_str(), createIntFromBoolArray(data));
	}
	sensors_to_string(data);
    return STATE_ERR;
}

