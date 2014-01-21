//
//  StateMachine.cpp
//
//  Please run this on the main thread!!!
//

#include "StateMachine.h"

/*
 static DoubleSolenoid* _piston_retract_extend;
 static DoubleSolenoid* _latch_lock_unlock;
 static DigitalInput* _piston_retract;
 static DigitalInput* _piston_extend;
 static DigitalInput* _latch_unlock;
 static DigitalInput* _latch_lock;
 */


StateMachine::StateMachine(Joystick *triggerJoystick) :
    _timer(),
    _piston_retract(PISTON_SWITCH_RETRACT_CHANNEL),
    _piston_extend(PISTON_SWITCH_EXTEND_CHANNEL),
    _latch_lock(LATCH_PISTON_LOCK_CHANNEL),
    _is_cocked(IS_COCKED_SWITCH_CHANNEL)
{
    _state_table[STATE_PISTON_RETRACT] = do_state_piston_retract;
    _state_table[STATE_LATCH_LOCK] = do_state_latch_lock;
    _state_table[STATE_PISTON_EXTEND] = do_state_piston_extend;
    _state_table[STATE_READY_TO_FIRE] = do_state_ready_to_fire;
    _state_table[STATE_LATCH_UNLOCK] = do_state_latch_unlock;
    _state_table[STATE_ERR] = do_err_state;
    _triggerJoystick = triggerJoystick;
}

StateMachine::~StateMachine()
{
}

state_t StateMachine::run_state( state_t cur_state, instance_data_t *data ) {
    return _state_table[ cur_state ]( data );
};


int StateMachine::GetSensorData(instance_data_t *data)
{
    // gets data and puts it in to data so we have it
    // needs to be in this format:
    // piston retact
    // piston extend
    // is cocked
    // latch locked


    // TODO fix the values and flip the logic only return true in one case
    data->state[0] = (_piston_retract.Get() != 0);
    data->state[1] = (_piston_extend.Get() != 0);
    data->state[2] = (_latch_lock.Get() != 0);
    data->state[3] = (_is_cocked.Get() != 0);

    return createIntFromBoolArray(data);
}

int StateMachine::createIntFromBoolArray(instance_data_t *data)
{
    int i = 0;
    int num = 0;
    for (; i<NUM_STATES; i++) {
        if (data->state[i]) {
            num |= 1 << i;
        }
    }
}

state_t StateMachine::init(state_t* cur_state, instance_data_t *data)
{
    int sensors = GetSensorData(data);
    switch (sensors) {
      case DONE_FIRING:
        *cur_state = STATE_PISTON_RETRACT;
        break;
      case PISTON_RETRACTED:
        *cur_state = STATE_LATCH_LOCK;
        break;
      case LATCH_LOCKED_PISTON_RETRACTED:
        *cur_state = STATE_PISTON_EXTEND;
        break;
      default:
        *cur_state = STATE_ERR;
        break;
    }
}

state_t StateMachine::do_state_piston_retract(instance_data_t *data)
{
    // reason is that 0b0010 = 2 is piston extended
    if (createIntFromBoolArray(data) != DONE_FIRING) {
        return STATE_ERR;
    }
    _timer.Start();

    //TODO add in code to make pistons reteact
    // _piston_retract_extend.Set(DoubleSolenoid::kReverse?)

    int sensors = 0;
    // reason for 8 is that piston is retracted then
    while (sensors = GetSensorData(data), sensors != PISTON_RETRACTED && (sensors == 0 || sensors == DONE_FIRING) ) {
        if (_timer.Get() > 25.) {
            _timer.Stop();
            _timer.Reset();
            return STATE_ERR;
        }
    }

    _timer.Stop();
    _timer.Reset();

    if (sensors != PISTON_RETRACTED)
    {
        return STATE_ERR;
    }

    // move to next state
    return STATE_LATCH_LOCK;
}

state_t StateMachine::do_state_latch_lock(instance_data_t * data)
{
    // reason is that 0b0100 = 4 is piston extended
    if (createIntFromBoolArray(data) != PISTON_RETRACTED) {
        return STATE_ERR;
    }
    _timer.Start();

    // TODO add in code to make piston lock
    // _latch_lock_unlock.Set(DoubleSolenoid::kForward?)

    int sensors = 0;

    // reason for 8 is that piston is retracted then
    while (sensors = GetSensorData(data), sensors != LATCH_LOCKED_PISTON_RETRACTED && (sensors == PISTON_RETRACTED)) {
        if (_timer.Get() > 5.) {
            _timer.Stop();
            _timer.Reset();
            return STATE_ERR;
        }
    }
    _timer.Stop();
    _timer.Reset();

    if (sensors != LATCH_LOCKED_PISTON_RETRACTED)
    {
        return STATE_ERR;
    }

    // go to next state
    return STATE_PISTON_EXTEND;
}

state_t StateMachine::do_state_piston_extend(instance_data_t * data)
{
    // reason is that 0b0100 = 4 is piston extended
    if (createIntFromBoolArray(data) != LATCH_LOCKED_PISTON_RETRACTED) {
        return STATE_ERR;
    }
    _timer.Start();

    // TODO add in code to make piston extend
    // _piston_retract_extend.Set(DoubleSolenoid::kForward?)

    int sensors = 0;

    // reason for 8 is that piston is retracted then
    while (sensors = GetSensorData(data), sensors != READY_TO_FIRE && (sensors == 12 || sensors == LATCH_LOCKED_PISTON_RETRACTED)) {
        if (_timer.Get() > 25.) {
            _timer.Stop();
            _timer.Reset();
            return STATE_ERR;
        }
    }
    _timer.Stop();
    _timer.Reset();

    if (sensors != READY_TO_FIRE)
    {
        return STATE_ERR;
    }

    // go to next state
    return STATE_READY_TO_FIRE;
}

state_t StateMachine::do_state_ready_to_fire(instance_data_t * data)
{
    // reason is that 0b0111 = 7 is piston extended, is cocked, and latch locked
    if (createIntFromBoolArray(data) != READY_TO_FIRE) {
        return STATE_ERR;
    }

    // wait for the trigger then fire!
    while (!_triggerJoystick->GetTrigger()) {}
    // go to next state
    return STATE_LATCH_UNLOCK;
}

state_t StateMachine::do_state_latch_unlock(instance_data_t * data)
{
    // reason is that 0b0111 = 7 is piston extended, is cocked, and latch locked
    if (createIntFromBoolArray(data) != READY_TO_FIRE) {
        return STATE_ERR;
    }
    _timer.Start();

    // TODO add in code to make piston unlock
    // _latch_lock_unlock.Set(DoubleSolenoid::kReverse?)

    int sensors = 0;

    // reason for 4 is that piston is extended after this step
    while (sensors = GetSensorData(data), sensors != DONE_FIRING && (sensors == READY_TO_FIRE || sensors == 10))
    {
        if (_timer.Get() > 5.) {
            _timer.Stop();
            _timer.Reset();
            return STATE_ERR;
        }
    }
    _timer.Stop();
    _timer.Reset();

    if (sensors != READY_TO_FIRE)
    {
        return STATE_ERR;
    }

    // go to next state
    return STATE_PISTON_RETRACT;
}

string state_to_string(instance_data_t *data)
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

        default:
            return "POTATO!";
            break;
    }
}

string sensors_to_string(instance_data_t *data)
{
    printf("0b (ic) (ll) (Pe) (Pr)\n0b");
    int sensors = createIntFromBoolArray(data);
    int i = NUM_STATES;
    for (; i > -1; i--) {
        printf("  %2d ",sensors & (1 << i));
    }
}

state_t StateMachine::do_err_state(instance_data_t *data)
{
    printf("%s\n",state_to_string(data).c_str());
    sensors_to_string(data);
    return STATE_ERR;
}
