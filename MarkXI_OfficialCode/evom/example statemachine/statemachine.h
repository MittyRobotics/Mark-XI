//
//  statemachine.h
//  statemachine
//
//  Created by Hails on 1/12/14.
//  Copyright (c) 2014 Matt Pleva. All rights reserved.
//

#ifndef __statemachine__statemachine__
#define __statemachine__statemachine__

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "Windows.h"
#include "WinBase.h"


typedef enum {
    STATE_PISTON_RETRACTED,
    STATE_PISTON_EXTENDED,
    STATE_LATCH_LOCK,
    STATE_LATCH_UNLOCK,
    STATE_ERR,
    NUM_STATES
} state_t;

typedef struct instance_data
{
    int state;
    int sensors;
}instance_data_t;

typedef state_t state_func_t( instance_data_t *data );

class statemachine {
public:
    statemachine();
    ~statemachine();
    state_t run_state(state_t, instance_data_t*);
    void setRandSeed(int);
private:
    static state_t do_state_pr(instance_data_t *data);
    static state_t do_state_pe(instance_data_t *data);
    static state_t do_state_ll(instance_data_t *data);
    static state_t do_state_lu(instance_data_t *data);
    static state_t do_err_state(instance_data_t *data);
    state_func_t*  state_table[NUM_STATES];
    static int count;

};



#endif /* defined(__statemachine__statemachine__) */
