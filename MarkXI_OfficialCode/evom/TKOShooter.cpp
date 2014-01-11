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

    //Boolean clause declarations
        //Conditional code goes here
    bool ballState = false;
    bool armsState = false;
    bool solenoidState = false;
    int solenoidLoop = 0;
    bool latchState = false;
    int latchLoop1 = 0;
    int latchLoop2 = 0;
    int latchLoop3 = 0;
    bool pistonState = false;
    int pistonLoop = 0;
    bool catapultState = false;
    int catapultLoop = 0;
    //Creating logic tree
    

    
 
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
