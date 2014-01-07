#include "DriveAtom.h"
#include <cstring>

// add in the motors and jags some where near here
//This is a change
//This is also a change
Drive_Atom *Drive_Atom::getInstance()
{
	if(_inst == NULL)
		_inst = new Drive_Atom();
	return _inst;
}

void Drive_Atom::run()
{
	// do really cool things with the motors here
	// like drive in a straight line
}
