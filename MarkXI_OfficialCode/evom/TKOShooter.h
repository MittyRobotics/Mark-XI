//Last edited by Vadim Korolik
//on 01/04/2014
#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "../Definitions.h"

class TKOShooter
{
	public:
		tkoShooter();
		~tkoShooter();
		void shootDist(double distance);
		static tkoShooter* inst();
		void Start();
		void Stop();
		void runStateMachine()
	private:
		Task *shooterTask;
		static tkoShooter* newShooterInstance;
		static void runShooterTask();
		void shooterProcess();
		bool needToShoot;
		double shootTarget;
};

#endif
