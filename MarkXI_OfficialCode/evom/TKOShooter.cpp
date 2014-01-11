//Edited by Ritwik Dutta
#include "TKOShooter.h"

/*This is the TKOShooter class, which
provides a system of readying the shooter,
checking for important problems, and 
shooting it. 

//TODO Finish implementing state machine
//TODO Write comment explaining state machine 
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
    //TBA

    if (keepGoing) {
        for (int i = 0; i < 5; i++) {
            ballState = true; //Get ball state here
            if (ballState) {
                //In case not first attempt
                keepGoing = true;
                break;
            }
            else {
                //Don't keep going - stop
                keepGoing = false;
            }
        }
    }
    else {
        //Error code 1 - ball
        return 1;
    }

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
                keepGoing = false;
            }
        }
    }
    else { 
        //Error code 2 - arms
        return 2;
    }

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
        //Error code 3 - solenoids
        return 3;
    }

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
        //Error code 4 - latch
        return 3;
    }
    

    //Rest of state machine logic
 
	return 0;
}

//Launch the shooter
int TKOShooter::launchShooter ()
{
    int readyShoot = tkoShooterInstance->runStateMachine();
    if (readyShoot == 0) {
        //Open latch
        bool latchOpen = false; //Check if latch opened
        if (latchOpen) {
            return 9;
        }
        else {
            //Error code 10 - shooting latch unlock error
            return 10;
        }
    }
    else {
        //Return error to help driver figure out problem
        return readyShoot;
    }
}

//TKOShooter class destructor
void TKOShooter::~TKOShooter ()
{
}
