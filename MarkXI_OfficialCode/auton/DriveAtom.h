#ifndef _DRIVE_ATOM_H
#define _DRIVE_ATOM_H

#include "Atom.h"

class Drive_Atom : public Atom{
	
	// add in the motors and jags some where
	
	public:
		Drive_Atom* getInstance();
		void run();
	private:
		Drive_Atom();
		~Drive_Atom();
		Drive_Atom* _inst;
};

#endif