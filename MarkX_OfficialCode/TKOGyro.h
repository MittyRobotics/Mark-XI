//Last edited by Andrew Light
//on 02/01/2013
#ifndef __TKOGYRO_H
#define __TKOGYRO_H

#include "Definitions.h"

class TKOGyro
{
public:
	TKOGyro();
	float targetAngle;
	~TKOGyro();
	void reset();
	void setTargetAngle(float a);
	float getTargetAngle();
	float GetAngle();
	float getError();
	TKOGyro* getInstance();
private:
	Gyro gyro;
};
#endif
