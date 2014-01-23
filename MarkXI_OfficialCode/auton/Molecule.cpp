#include <queue>
#include "Molecule.h"


Molecule::Molecule():
	drive1(DRIVE_L1_ID, CANJaguar::kPosition),
	drive2(DRIVE_L2_ID, CANJaguar::kVoltage),
	drive3(DRIVE_R1_ID, CANJaguar::kPosition),
	drive4(DRIVE_R2_ID, CANJaguar::kVoltage),
	_list()

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

Molecule::~Molecule(){
	
	while(_list.size()>0)
	{
		Atom *a = _list.front();
		_list.pop();
		delete a;
	}
}


void Molecule::Test()
{
	drive1.Set(100);
	drive2.Set(drive1.GetOutputVoltage());
	printf("drive %f %f %f\n", drive1.GetPosition(), drive3.GetPosition(), drive1.GetOutputVoltage());
}


void Molecule::addAtom(Atom *a)
{
	_list.push(a);
}


void Molecule::start()
{
	int i = 0;
	int anmt = _list.size();
	for ( ; i < anmt; i++ )
	{
		Atom* a = _list.front();
		a->run();
		printf("its working");
		_list.pop();
		delete a;
	}
	printf("start works lol\n");
}
