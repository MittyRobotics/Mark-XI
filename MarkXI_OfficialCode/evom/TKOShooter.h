//Last edited by Ritwik Dutta
//on 01/11/2014
#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "../Definitions.h"

class TKOShooter
{
	public:
		//Constructor
		TKOShooter();
		//Destructor
		~TKOShooter();
		//Instance Creator
		static TKOShooter* newShooterInstance();
		//Instance variable
		static TKOShooter* tkoShooterInstance;
		//Shooter launcher
		void launchShooter();
		void startShooter();
		void stopShooter();
		
		//State machine
        static int runStateMachine();
		
		static TKOShooter* _instance;
		
};

#endif
