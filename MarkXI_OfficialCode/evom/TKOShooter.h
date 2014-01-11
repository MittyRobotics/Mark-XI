//Last edited by Ritwik Dutta
//on 01/04/2014
#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "../Definitions.h"

class TKOShooter
{
	public:
		TKOShooter();
		~TKOShooter();
		static TKOShooter* newShooterInstance();
		void launchShooter();
        static int runStateMachine();
		
};

#endif
