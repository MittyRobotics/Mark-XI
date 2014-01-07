#ifndef _MOLECULE_H
#define _MOLECULE_H

#include "Atom.h"
#include <vector>

class Molecule {
	
	public:
		void addAtom(Atom*);
		void start();
		Molecule();
		~Molecule();
	private:
		std::vector<Atom*> list;
	
};

#endif
