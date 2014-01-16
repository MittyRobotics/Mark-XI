//
//  statemachine.cpp
//  statemachine
//
//  Created by Hails on 1/12/14.
//  Copyright (c) 2014 Matt Pleva. All rights reserved.
//

#include "statemachine.h"

int statemachine::count = 0;

statemachine::statemachine()
{
    state_table[STATE_PISTON_RETRACTED] = do_state_pr;
    state_table[STATE_PISTON_EXTENDED] = do_state_pe;
    state_table[STATE_LATCH_LOCK] = do_state_ll;
    state_table[STATE_LATCH_UNLOCK] = do_state_lu;
    count = 0;
    std::cout << "random seed " << 1 << std::endl;
}

statemachine::~statemachine()
{

}

void statemachine::setRandSeed(int i)
{
    srand(i);
}

state_t statemachine::run_state( state_t cur_state, instance_data_t *data ) {
    return state_table[ cur_state ]( data );
};


state_t statemachine::do_state_pr( instance_data_t *data )
{
    count = 0;
//    printf("Initial state transitioning to 2nd state\n");
    std::cout << "Initial state transitioning to 2nd state\n";
    std::cout << "Pistons retacted moving...\n";
    int random = count = 0;
    while(count < 5 && data->state == 0x1)
    {
        random = rand() % 100;
        if( random < 84 )
        {
            data->state = 0x2;
            return STATE_PISTON_EXTENDED;
        }
//        printf("pistions in process of moving rechecking %d\n",rand);
        std::cout << "pistons in process of moving rechecking " << random << std::endl;
        count ++;
        Sleep(1000);
    }
    return STATE_ERR;
}

state_t statemachine::do_state_pe( instance_data_t *data )
{
    std::cout << "2nd state transitioning to 3rd state\n";
    std::cout << "pistons extended locking latch...\n";
    count = 0;
    int random = 0;
    while(count < 5 && data->state == 0x2)
    {
        random = rand() % 100;
        if( random < 35 )
        {
            data->state = 0x4;
            return STATE_LATCH_LOCK;
        }
        std::cout << "latch engaging rechecking " << random << std::endl;
        count ++;
        Sleep(1000);
    }

    return STATE_ERR;
}

state_t statemachine::do_state_ll( instance_data_t *data )
{
    std::cout << "3rd state transitioning to 4th state\n";
    std::cout << "Latch locked unlocking ...\n";
    count = 0;
    int random = 0;
    while(count < 5 && data->state == 0x4)
    {
        random = rand() % 100;
        if( random < 35 )
        {
            data->state = 0x1;
            return STATE_LATCH_UNLOCK;
        }
        std::cout << "latch unlocking rechecking " << random << std::endl;
        count ++;
        Sleep(1000);
    }
    return STATE_ERR;
}
state_t statemachine::do_state_lu( instance_data_t *data )
{
    std::cout << "3rd state transitioning to 4th state\n";
    std::cout << "Latch unlocked firing\n";
    count = 0;
    int random = 0;
    while(count < 5 && data->state == 0x8)
    {
        random = rand() % 100;
        if( random < 35 )
        {
            data->state = 0x1;
            return STATE_PISTON_RETRACTED;
        }
        std::cout << "fire for effect! rechecking " << random << std::endl;
        count ++;
        Sleep(1000);
    }
    return STATE_ERR;
}


state_t statemachine::do_err_state(instance_data_t *data)
{
    std::cout << "Welp thats not good.  We are in the error state\nHere is the sensor data " << data->state << " and now we will exit\n";
//    printf("Welp thats not good.  We are in the error state \
//           \nHere is the sensor data %x and now we will exit\n",data->state);
    return STATE_ERR;
}
