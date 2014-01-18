//
//  StateMachine.h
//  


#ifndef ____StateMachine__
#define ____StateMachine__

#include "../Definitions.h"
// might not be needed if included in wpilib
#ifndef WPILIB_H_
#include <Timer.h>
#include <DoubleSolenoid.h>
#endif

// Define the states
typedef enum {
    STATE_PISTON_RETRACT,
    STATE_LATCH_LOCK,
    STATE_PISTON_EXTEND,
    STAET_READY_TO_FIRE,
    STATE_LATCH_UNLOCK,
    STATE_ERR,
    NUM_STATES
} state_t;

// create the instance data type
typedef struct instance_data
{
    bool state[NUM_STATES];
}instance_data_t;

// function pointer stuff
typedef state_t state_func_t( instance_data_t *data );

class StateMachine {
public:
    StateMachine(Joystick* triggerJoystick);
    ~StateMachine();
    state_t run_state(state_t, instance_data_t*);
    void setRandSeed(int);
private:
    static state_t do_state_piston_retract(instance_data_t *data);
    static state_t do_state_piston_extend(instance_data_t *data);
    static state_t do_state_latch_lock(instance_data_t *data);
    static state_t do_state_latch_unlock(instance_data_t *data);
    static state_t do_state_ready_to_fire(instance_data_t *data);
    static state_t do_err_state(instance_data_t *data);
    static int createIntFromBoolArray(instance_data_t *data);
    state_func_t*  _state_table[NUM_STATES];
    static Timer _timer;
    static Joystick* _triggerJoystick;
    // TODO Fix the solenoids to something more useful
    static DoubleSolenoid _piston_retract_extend;
    static DoubleSolenoid _latch_lock_unlock;

    static DigitalInput _piston_retract;
    static DigitalInput _piston_extend;
    static DigitalInput _latch_lock;
    static DigitalInput _is_cocked;
};


#endif /* defined(____StateMachine__) */
