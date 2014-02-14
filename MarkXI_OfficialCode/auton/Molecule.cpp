//Last edited by Ishan Shah
#include <queue>
#include "Molecule.h"


Molecule::Molecule():
	/*
	 * Initialization of motors, creating a list of atoms to form a molecule
	 */ 
	drive1(DRIVE_L1_ID, CANJaguar::kPosition),
	drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus),
	drive3(DRIVE_R1_ID, CANJaguar::kPosition),
	drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus),
	_list()

{
}

void Molecule::MoleculeInit() {
	
	//putting the Encoders as 250 tick Encoders, setting the second and fourth motors as slaves 
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
	drive2.SetSafetyEnabled(false);

	drive4.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive4.SetSafetyEnabled(false);

	drive1.SetExpiration(0.1);
	drive2.SetExpiration(0.1);
	drive3.SetExpiration(0.1);
	drive4.SetExpiration(0.1);

	drive1.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);
	drive3.SetPID(DRIVE_kP, DRIVE_kI, DRIVE_kD);

}

Molecule::~Molecule() {

	while (_list.size() > 0) {
		Atom *a = _list.front();
		_list.pop();
		delete a;
	}
}

void Molecule::Test()
{
	drive1.Set(100);
	drive2.Set(drive1.GetOutputVoltage());
	drive3.Set(100);
	drive4.Set(drive3.GetOutputVoltage());
	printf("drive %f %f %f\n", drive1.GetPosition(), drive3.GetPosition(),
			drive1.GetOutputVoltage());
}

void Molecule::addAtom(Atom *a) //atom added to end of molecule
{
	_list.push(a);
}

void Molecule::start() //runs through atoms of molecule, then deletes that member to prevent infinite loop
{
	int i = 0;
	int anmt = _list.size();
	for (; i < anmt; i++) {
		Atom* a = _list.front();
		a->run();
		_list.pop();
		delete a;
	}
}






