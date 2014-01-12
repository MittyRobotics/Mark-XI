//Last edited by Vadim Korolik
//on 01/12/2014

//USEFULL LINK: http://stackoverflow.com/questions/133214/is-there-a-typical-state-machine-implementation-pattern
#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "../Definitions.h"

typedef enum 
{ 
	STATE_INITIAL, 
	STATE_FOO, 
	STATE_BAR, 
	NUM_STATES 
} 
	state_t;
	
typedef struct instance_data instance_data_t;
typedef state_t state_func_t( instance_data_t *data );

state_t do_state_initial( instance_data_t *data );
state_t do_state_foo( instance_data_t *data );
state_t do_state_bar( instance_data_t *data );

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
		
		
};

#endif

/*
 * int main( void ) {
    state_t cur_state = STATE_INITIAL;
    instance_data_t data;

    while ( 1 ) {
        cur_state = run_state( cur_state, &data );

        // do other program logic, run other state machines, etc
		}
	}
 */
