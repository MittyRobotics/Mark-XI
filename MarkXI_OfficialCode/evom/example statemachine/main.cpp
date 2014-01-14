//
//  main.cpp
//  statemachine
//
//  Created by Hails on 1/12/14.
//  Copyright (c) 2014 Matt Pleva. All rights reserved.
//

#include <iostream>
#include "statemachine.h"

int main(int argc, const char * argv[])
{

    /*
     
        Two ways to do it one is this way which is waiting in the functions until you transition
        the other way is to set it on its way set the state in the main loop and execute that state
     
     */
    statemachine s;
    state_t state = STATE_PISTON_RETRACTED;
    instance_data_t data;
    int count = 0;
    data.state = 0x1;
    s.setRandSeed(1);
    while(count < 5)
    {
        state = s.run_state(state, &data);
        count ++;
    }
    std::cout << "\n------- New Test --------\n\n";
    count = 0;
    data.state = 0x1;
    state = STATE_PISTON_RETRACTED;
    s.setRandSeed(122343567);
    while(count < 7)
    {
        state = s.run_state(state, &data);
        count ++;
    }
/*
    while(count < 7)
    {
        state = s.run_state(state, &data);
        count ++;
    }
    */
    return 0;
}
