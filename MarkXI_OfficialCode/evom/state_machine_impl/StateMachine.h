//
//  StateMachine.h
//


#ifndef ____StateMachine__
#define ____StateMachine__

#include "../../Definitions.h"
// might not be needed if included in wpilib
#ifndef WPILIB_H_
#include <Timer.h>
#include <DoubleSolenoid.h>
#endif


#define DONE_FIRING 2
#define PISTON_RETRACTED 1
#define LATCH_LOCKED_PISTON_RETRACTED 5
#define READY_TO_FIRE 14


// Define the states
typedef enum {
    STATE_PISTON_RETRACT,
    STATE_LATCH_LOCK,
    STATE_PISTON_EXTEND,
    STATE_READY_TO_FIRE,
    STATE_LATCH_UNLOCK,
    NUM_STATES,
    STATE_ERR
} state_t;

// create the instance data type
typedef struct instance_data
{
    state_t curState;
    bool state[NUM_STATES];
}instance_data_t;

// function pointer stuff
typedef state_t state_func_t( instance_data_t *data );

class StateMachine {
public:
    StateMachine(Joystick* triggerJoystick);
    ~StateMachine();

    state_t run_state(state_t, instance_data_t*);
    state_t init(state_t* cur_state, instance_data_t * data);
private:
    static state_t do_state_piston_retract(instance_data_t *data);
    static state_t do_state_piston_extend(instance_data_t *data);
    static state_t do_state_latch_lock(instance_data_t *data);
    static state_t do_state_latch_unlock(instance_data_t *data);
    static state_t do_state_ready_to_fire(instance_data_t *data);
    static state_t do_err_state(instance_data_t *data);

    static int GetSensorData(instance_data_t *data);
    static int createIntFromBoolArray(instance_data_t *data);

    static string state_to_string(instance_data_t *data);
    static string sensors_to_string(instance_data_t *data);

    state_func_t*  _state_table[NUM_STATES + 1];

    static Timer* _timer;

    static Joystick* _triggerJoystick;

    static DoubleSolenoid* _piston_retract_extend;
    static DoubleSolenoid* _latch_lock_unlock;

    static DigitalInput* _piston_retract;
    static DigitalInput* _piston_extend;
    static DigitalInput* _latch_lock;
    static DigitalInput* _is_cocked;
};


#endif /* defined(____StateMachine__) */
