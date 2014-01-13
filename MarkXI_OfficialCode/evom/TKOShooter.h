//Last edited by Vadim Korolik
//on 01/12/2014

//USEFULL LINK: http://stackoverflow.com/questions/133214/is-there-a-typical-state-machine-implementation-pattern
#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "../Definitions.h"



/*
    These are your states as defined by the sensors mounted to the catapult
*/
typedef enum
{
	STATE_INITIAL,
	STATE_FOO,
	STATE_BAR,
	NUM_STATES // this is here so you do not have to count and create another variable
} state_t;


/*
    This is the data that is used in between states so that states can find
    out what states we were at last and how to proceed
*/
typedef struct instance_data  //TODO figure out what data we need
{
    // for instance we should have a state variable so we know that state we
    // were in previously
    int state_of_sensors;

	bool init;
	bool dummy;
} instance_data_t;

/*
    This statement sets the state_t type to be a function pointer.
    This function takes on variable: the instance data

    NOTE: this definition is probably not right
*/
typedef state_t state_func_t( instance_data_t *data );


/*
    Here we define the various state functions.
    Please note that each function should provide some way of moving forward in
    the state machine or going to error state.
    (use while loop to wait for mechanical parts to move should exit after x seconds)
*/
state_t do_state_initial( instance_data_t *data );
state_t do_state_foo( instance_data_t *data );
state_t do_state_bar( instance_data_t *data );

/*
    This is the state table.
    In other words it tells us what states there are and
    what functions go with what state.
    Example usage could be state_table[STATE_INITIAL](&inst_data);
*/
state_func_t* const state_table[ NUM_STATES ] = {
    do_state_initial, do_state_foo, do_state_bar
};

class TKOShooter
{
	public:
		TKOShooter();
		~TKOShooter();
		static TKOShooter* inst();
		bool Start();
		bool Stop();

	private:
		Task *shooterTask;
		Task *stateMachineTask;
		void initStateMachine();
		bool startStateMachine();
		bool stopStateMachine();
		int runStateMachine();
		bool startShooter();
		bool stopShooter();
		bool shooterDoAction(int action);

		static void shooterTaskRunner();
		static void stateMachineTaskRunner();

		static TKOShooter* _instance;
		state_t cur_state;
		instance_data_t stateData;
};

#endif

/*
 * int main( void ) {
    state_t cur_state = STATE_INITIAL; // set our state
    instance_data_t data; // set up some data to use

    while ( 1 ) {
        cur_state = run_state( cur_state, &data ); // start the state machine

        // do other program logic, run other state machines, etc
		}
	}
 */
