//Last edited by Ritwik Dutta
//on 01/04/2014
#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "../Definitions.h"

class tkoShooter
{
	public:
		tkoShooter();
		~tkoShooter();
		static tkoShooter* inst();
		static bool isShooterReady();
		void startShooter();
		void stopShooter();
	private:
		Task *shooterTask;
		static tkoShooter* newShooterInstance;
		static void runShooterTask();
		static int runStateMachine();
		static void ~tkoShooter();
		void shooterProcess();
		
};

#endif
