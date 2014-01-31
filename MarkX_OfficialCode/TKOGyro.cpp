//Last edited by Andrew Light
//on 02/01/2013
#include "TKOGyro.h"
#include "Definitions.h"

TKOGyro *instance;
/*
 * Singleton Gyro Implementation
 * Can utilize Gyro in multiple classes
 *
HOW TO IMPLEMENT
TKOGyro* g;
g = g->getInstance();
g->getAngle();*/

TKOGyro::TKOGyro() : gyro(GYRO_PORT){
	targetAngle = 0;
}
TKOGyro* TKOGyro::getInstance(){
	if(instance == NULL){
		instance = new TKOGyro();
	}
	return instance;
}
void TKOGyro::setTargetAngle(float a){
	targetAngle = a;
}
float TKOGyro::getTargetAngle(){
	return targetAngle;
}
float TKOGyro::getError(){
	float gyroError;
	float angle = gyro.GetAngle();
	gyroError = (targetAngle - angle);
	return gyroError;
}
void TKOGyro::reset(){
	gyro.Reset();
}
float TKOGyro::GetAngle(){
	return gyro.GetAngle();
}
TKOGyro::~TKOGyro(){
	
}

