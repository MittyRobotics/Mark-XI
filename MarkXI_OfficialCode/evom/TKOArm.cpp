//Last Edited by Arjun Biju, Murad Awad, and Ishan Shah
// on 01/31/2014
#include "TKOArm.h"

TKOArm* TKOArm::m_Instance = NULL;

/*
 * Initializes the 2 roller jaguars in percent Vbus mode
 *  \param int port 1 - roller 1 Jaguar ID
 *  \param int port 2- roller 2 Jaguar ID
 */

TKOArm::TKOArm() :
			_arm(ARM_JAGUAR_ID, CANJaguar::kPercentVbus), 
			limitSwitchArm(ARM_OPTICAL_SWITCH), // Optical limit switch
			limitSwitchBall(BALL_LIMIT_SWITCH),
			_roller(ROLLER_1_JAGUAR_ID, ROLLER_2_JAGUAR_ID),
			stick3(STICK_3_PORT),
			stick4(STICK_4_PORT)
{
	printf("Initializing intake\n");
	_arm.SetSafetyEnabled(false);
	_arm.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
	_arm.SetVoltageRampRate(0.0);
	_arm.ConfigFaultTime(0.1); 
	_arm.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_arm.ConfigEncoderCodesPerRev(250);
	_arm.EnableControl();
	armTask = new Task("TKOArm", (FUNCPTR) ArmRunner);
	AddToSingletonList();
}

TKOArm::~TKOArm() {
	m_Instance = NULL;
}

TKOArm* TKOArm::inst() {
	if (!m_Instance) {
		printf("TKOIntake instance is null\n");
		m_Instance = new TKOArm;
	}
	return m_Instance;
}
void TKOArm::ArmRunner()
{
	
}

