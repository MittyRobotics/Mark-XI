//Edited by Ritwik Dutta
#include "TKOShooter.h"

/*This is the TKOShooter class, which
provides a system of readying the shooter,
checking for important problems, and 
shooting it. 

//TODO Write shooter handler with tasks/auton
//TODO Write error handling function

*/

//Shooter instance var
  TKOShooter::tkoShooterInstance = NULL;


TKOShooter::TKOShooter ()
{	
}

//Shooter instance generator
TKOShooter* TKOShooter::newShooterInstance ()
{
	if (!tkoShooterInstance) {
		printf("No tkoShooterInstance. Creating.");
		tkoShooterInstance = new TKOShooter;
	}
	return tkoShooterInstance;
}

//Error throw function
void throwError( int errorCode)
{
}

//State machine function
int TKOShooter::runStateMachine ()
{

    //Logic tree

    //Section structure
    /* 
    
    if (keepGoing) {    Check if the code has failed so that an error message can be thrown
        for (int i = 0; i < 5; i++) { We run the loop five times in case a temporary error can be fixed (e.g., arms in motion, connection flicker)
            objState = getObjState(); Get the state of the object for the next check
            if (objState) {
                keepGoing = true; If it's not the first attempt, we don't want to stop if the process ends up succeeding
                break; No need to continue the loop
            }
            else {
                keepGoing = false; Assume the worst
                setObjState(); Try to fix the problem
            }
        }
    }
    else {
        return n; Return error code for previous state, since it set keepGoing to false. A bit confusing, but theoretically sound.
    }

    Error Codes
    +++++++++++

    1 - Ball not in arms
        No code runs
    2 - Arms not in position
        No code run
    3 - Solenoids not set to retract
        Try to set solenoids to retract
    4 - Latch not open
        Try to open latch
    5 - Pistons not extended
        Try to extend pistons
    6 - Catapult not pushed back (spring not extended)
        Try to extend spring
    7 - Latch not locked yet
        Try to lock the latch
    8 - Latch not unlocked when shooting
        Try to unlock latch
        
        

    */

    bool keepGoing = true;

    //Ball code
    for (int i = 0; i < 5; i++) {
        ballState = true; //Get ball state here
        if (ballState) {
            //In case not first attempt
            keepGoing = true;
            break;
        }
        else {
            //Don't keep going - stop
            //No ball interaction
            keepGoing = false;
        }
    }


    //Arm code
    if (keepGoing) {
        for (int i = 0; i < 5; i++) {
            armsState = true; //Get arms state here
            if (armsState) {
                //In case not first attempt
                keepGoing = true;
                break;
            }
            else {
                //Don't keep going - stop
                //No arm interaction
                keepGoing = false;
            }
        }
    }
    else { 
        //Error code 1 - ball
        return 1;
    }

    //Solenoid code
    if (keepGoing) {
        for (int i = 0; i < 5; i++) {
            solenoidState = true; //Get solenoid state here
            if (solenoidState) {
                //Just in case not first try
                keepGoing = true;
                break;
            }
            else {
                //Try and set solenoids to retract
                keepGoing = false;
            }
        }
    }
    else {
        //Error code 2 - arms
        return 2;
    }

    //Latch open code
    if (keepGoing) {
        for (int i = 0; i < 5; i++) {
            latchState = true; //Get latch state here
            if (latchState) {
                //Just in case not first try
                keepGoing = true;
                break;
            }
            else {
                //Try and open latch
                keepGoing = false;
            }
        }
    }
    else {
        //Error code 3 - solenoids
        return 3;
    }

    //Piston code
    if (keepGoing) {
        for (int i = 0; i < 5; i++) {
            pistonState = true; //Get piston state here
            if (pistonState) {
                //Just in case not first try
                keepGoing = true;
                break;
            }
            else {
                //Try and extend pistons
                keepGoing = false;
            }
        }
    }
    else {
        //Error code 4 - latch
        return 4;
    }

    //Catapult code
    if (keepGoing) {
        for (int i = 0; i < 5; i++) {
            catState = true; //Get catapult state here
            if (catState) {
                //Just in case not first try
                keepGoing = true;
                break;
            }
            else {
                //Try and push catapult back
                keepGoing = false;
            }
        }
    }
    else {
        //Error code 5 - pistons
        return 5;
    }

    //Latch state
    if (keepGoing) {
        for (int i = 0; i < 5; i++) {
            latchState = true; //Get latch state here
            if (latchState) {
                //Just in case not first try
                keepGoing = true;
                break;
            }
            else {
                //Try and lock latch
                keepGoing = false;
            }
        }
    }
    else {
        //Error code 6 - catapult
        return 6;
    }   

    //Rest of state machine logic
    if (keepGoing) {
        return 0;
    }
    else {
        //Error code 7 - Lock latch
        return 7;
    }
}

//Launch the shooter
int TKOShooter::launchShooter ()
{
    int readyShoot = tkoShooterInstance->runStateMachine();
    if (readyShoot == 0) {
        //Open shooter
        bool shootSuccess = true;
        for (int i = 0; i < 5; i++) {
            bool latchOpen = false; //Check if latch is open
            if (latchOpen) {
                shootSuccess = true;
                break;
            }   
            else {
                shootSuccess = false;
                //Open shooter    
            }
        }
        if (shootSuccess) {
            return 9; 
        }
        else {
            return 8; 
        }
    }
    else {
        //Return error from state machine to help driver figure out problem
        return readyShoot;
    }
}

//TKOShooter class destructor
void TKOShooter::~TKOShooter ()
{
}
