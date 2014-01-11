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
		void startShooter();
		void launchShooter();
		void stopShooter();
	private:
		static void runShooterTask();
		static int runStateMachine();
		
		static TKOShooter* _instance;
		
};

#endif
