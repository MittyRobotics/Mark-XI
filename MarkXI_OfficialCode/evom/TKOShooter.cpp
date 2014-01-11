//Last edited by Ritwik Dutta
//on 01/04/2014
#include "TKOShooter.h"

TKOShooter*  TKOShooter::tkoShooterInstance = NULL;
///Constructor for the tkoShooter class

TKOShooter::TKOShooter ()
{	
}

TKOShooter* TKOShooter::newShooterInstance ()
{
	if (!tkoShooterInstance) {
		printf("No tkoShooterInstance. Creating.");
		tkoShooterInstance = new TKOShooter;
	}
	return tkoShooterInstance;
}

void throwError( int errorCode)
{
}

int TKOShooter::runStateMachine ()
{

    //Logic tree


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
    

    
 
	return 0;
}

void TKOShooter::startShooter ()
{
}

void TKOShooter::stopShooter ()
{
}

void TKOShooter::launchShooter ()
{
}

void TKOShooter::~TKOShooter ()
{
}
