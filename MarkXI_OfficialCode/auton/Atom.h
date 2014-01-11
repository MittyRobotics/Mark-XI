#ifndef _ATOM_h
#define _ATOM_H

class Atom {
	
	public:
		Atom();
		virtual void run() = 0;
		virtual ~Atom(){}
};
#endif
