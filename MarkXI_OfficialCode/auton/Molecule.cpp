#include <queue>
#include "Molecule.h"


Molecule::Molecule():
drive1(DRIVE_L1_ID, CANJaguar::kPosition), drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus), drive3(DRIVE_R1_ID, CANJaguar::kPosition), drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus)

{}

void Molecule::MoleculeInit() {
	drive1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	drive1.ConfigEncoderCodesPerRev(250);
	drive1.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive1.EnableControl();
	drive1.SetSafetyEnabled(false); //new before true

	drive3.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	drive3.ConfigEncoderCodesPerRev(250);
	drive3.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive3.EnableControl();
	drive3.SetSafetyEnabled(false);

	drive2.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive2.EnableControl();
	drive2.SetSafetyEnabled(false);

	drive4.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive4.EnableControl();
	drive4.SetSafetyEnabled(false);
	
	

}

Molecule::~Molecule(){}


void Molecule::addAtom(Atom *a)
{
	list.push(a);
}


void Molecule::start()
{
	unsigned int i = 0;
	for ( ; i < list.size(); i++ )
	{
		Atom* a = list.front();
		a->run();
		list.pop();
	}
}
