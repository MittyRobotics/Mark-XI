#ifndef _MOLECULE_H
#define _MOLECULE_H

#include "Atom.h"
#include "../Definitions.h"
#include <queue>

class Molecule {
	
	public:
		void addAtom(Atom*);
		void start();
		void MoleculeInit();
		Molecule();
		~Molecule();
	private:
		std::queue<Atom*> list;
		CANJaguar drive1, drive2, drive3, drive4;
	
};

#endif
