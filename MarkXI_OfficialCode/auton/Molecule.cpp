#include <vector>
#include "Molecule.h"


Molecule::Molecule(){}
	
Molecule::~Molecule(){}


void Molecule::addAtom(Atom *a)
{
	list.push_back(a);
}


void Molecule::start()
{
	int i = 0;
	for ( ; i < list.size(); i++ )
	{
		list[i]->run();
	}
}