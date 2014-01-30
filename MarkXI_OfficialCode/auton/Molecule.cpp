//Last edited by Ishan Shah and Alex Parks
#include <queue>
#include "Molecule.h"


Molecule::Molecule():
drive1(DRIVE_L1_ID, CANJaguar::kPosition), //Initialization of all of the motors, creation of a list which can be formed into a molecule, the list is comprised of various atoms. 
drive2(DRIVE_L2_ID, CANJaguar::kVoltage),
drive3(DRIVE_R1_ID, CANJaguar::kPosition),
drive4(DRIVE_R2_ID, CANJaguar::kVoltage),
_list()

{}

void Molecule::MoleculeInit() {
	//All of this is setting the Jaguars in various modes, and putting the Encoders as 250 tick Encoders. This also disables the safety, and makes the second and fourth motors slaves. 
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


void Molecule::Test() //Testing to make sure the motors are working and that the code is working on the CRio
{
		drive1.Set(100);
		drive2.Set(drive1.GetOutputVoltage());
		drive3.Set(100);
		drive4.Set(drive3.GetOutputVoltage());
		printf("drive %f %f %f\n", drive1.GetPosition(), drive3.GetPosition(), drive1.GetOutputVoltage());
}


void Molecule::addAtom(Atom *a) //Adds an atom to the end of the Molecule, to be run with the rest of the molecule. 
{
		_list.push(a);
}


void Molecule::start() 	//This runs the the molecule, and then deletes that member of the molecule, so it doesn't run in an infinite cycle. 
{
	int i = 0;
	int anmt = _list.size();
	for ( ; i < anmt; i++ )
	{
		Atom* a = _list.front();
		a->run();
		_list.pop();
		delete a;
	}
}
