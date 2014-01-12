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
		bool startStateMachine();
		bool stopStateMachine();
		bool startShooter();
		bool stopShooter();
		bool shooterDoAction(int action);
		
		static void shooterTaskRunner();
		static void stateMachineTaskRunner();
		
		static TKOShooter* _instance;
		
};

#endif
