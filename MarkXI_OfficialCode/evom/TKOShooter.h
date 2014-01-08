//Last edited by Vadim Korolik
//on 01/04/2014
#ifndef __TKOSHOOTER_H
#define __TKOSHOOTER_H

#include "../Definitions.h"
#include "../log/TKOLogger.h"

class TKOShooter
{
	public:
		TKOShooter();
		~TKOShooter();
		void shootDist(double distance);
		static TKOShooter* inst();
		void Start();
		void Stop();
	private:
		Task *shooterTask;
		static TKOShooter* m_Instance;
		static void ShooterTaskRunner();
		void shooterProcess();
		bool needToShoot;
		double shootTarget;
};

#endif
