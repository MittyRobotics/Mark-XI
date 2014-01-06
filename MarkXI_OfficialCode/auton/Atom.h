#ifndef _ATOM_h
#define _ATOM_H

class Atom {
	
	public:
		virtual Atom* getInstance();
		virtual void run();
	private:
		virtual ~Atom(){}
};
#endif