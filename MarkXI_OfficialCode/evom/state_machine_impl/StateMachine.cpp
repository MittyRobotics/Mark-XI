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
    // lol wut?
    delete _timer;
}

int StateMachine::GetSensorData(instance_data_t *data)
{
    // gets data and puts it in to data so we have it
    // needs to be in this format:
    // piston retact
    // piston extend
    // is cocked
    // latch locked


    // TODO fix the values and flip the logic only return true in one case

    int i = 0;
    for (; i < NUM_STATES; i++) {
        switch (i) {
            case 0:
                data->state[i] = _piston_retract.Get() == 0 ? FALSE : TRUE;
                break;
            case 1:
                data->state[i] = _piston_extend.Get() == 0 ? FALSE : TRUE;
                break;

            case 2:
                data->state[i] = _latch_lock.Get() == 0 ? FALSE : TRUE;
                break;

            case 3:
                data->state[i] = _is_cocked.Get() == 0 ? FALSE : TRUE;
                break;

            default:
                break;
        }
    }


    return createIntFromBoolArray(data);
}

int StateMachine::createIntFromBoolArray(instance_data_t *data)
{
    int i = 0;
    int num = 0;
    for (; i<NUM_STATES; i++) {
        num = data->state[i] << (NUM_STATES - (i + 1))
    }
}

state_t StateMachine::do_state_piston_retract(instance_data_t *data)
{
    // reason is that 0b0100 = 4 is piston extended
    if (createIntFromBoolArray(data) != 4) {
        return STATE_ERR;
    }
    _timer.Start();

    //TODO add in code to make pistons reteact
    // _piston_retract_extend.Set(DoubleSolenoid::kReverse?)

    // reason for 8 is that piston is retracted then
    while (GetSensorData(data) != 8) {
        if (_timer.Get() > 25.) {
            _timer.Stop();
            _timer.Reset();
            return STATE_ERR;
        }
    }

    _timer.Stop();
    _timer.Reset();

    // move to next state
    return STATE_LATCH_LOCK;
}

state_t StateMachine::do_state_latch_lock(instance_data_t * data)
{
    // reason is that 0b0100 = 4 is piston extended
    if (createIntFromBoolArray(data) != 8) {
        return STATE_ERR;
    }
    _timer.Start();

    // TODO add in code to make piston lock
    // _latch_lock_unlock.Set(DoubleSolenoid::kForward?)

    // reason for 8 is that piston is retracted then
    while (GetSensorData(data) != 9) {
        if (_timer.Get() > 5.) {
            _timer.Stop();
            _timer.Reset();
            return STATE_ERR;
        }
    }
    _timer.Stop();
    _timer.Reset();
    // go to next state
    return STATE_PISTON_EXTEND;
}

state_t StateMachine::do_state_piston_extend(instance_data_t * data)
{
    // reason is that 0b0100 = 4 is piston extended
    if (createIntFromBoolArray(data) != 9) {
        return STATE_ERR;
    }
    _timer.Start();

    // TODO add in code to make piston extend
    // _piston_retract_extend.Set(DoubleSolenoid::kForward?)

    // reason for 8 is that piston is retracted then
    while (GetSensorData(data) != 7) {
        if (_timer.Get() > 25.) {
            _timer.Stop();
            _timer.Reset();
            return STATE_ERR;
        }
    }
    _timer.Stop();
    _timer.Reset();
    // go to next state
    return STATE_READY_TO_FIRE;
}

state_t StateMachine::do_state_ready_to_fire(instance_data_t * data)
{
    // reason is that 0b0111 = 7 is piston extended, is cocked, and latch locked
    if (createIntFromBoolArray(data) != 7) {
        return STATE_ERR;
    }

    // wait for the trigger then fire!
    while (!_triggerJoystick.GetTrigger()) {}
    // go to next state
    return STATE_LATCH_UNLOCK;
}

state_t StateMachine::do_state_latch_unlock(instance_data_t * data)
{
    // reason is that 0b0111 = 7 is piston extended, is cocked, and latch locked
    if (createIntFromBoolArray(data) != 7) {
        return STATE_ERR;
    }
    _timer.Start();

    // TODO add in code to make piston unlock
    // _latch_lock_unlock.Set(DoubleSolenoid::kReverse?)

    // reason for 4 is that piston is extended after this step
    while (GetSensorData(data) != 4)
    {
        if (_timer.Get() > 5.) {
            _timer.Stop();
            _timer.Reset();
            return STATE_ERR;
        }
    }
    _timer.Stop();
    _timer.Reset();
    // go to next state
    return STATE_LATCH_UNLOCK;
}
