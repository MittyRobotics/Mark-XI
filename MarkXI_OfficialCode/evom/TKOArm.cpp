//Last Edited by Arjun Biju, Murad Awad, and Ishan Shah
// on 01/31/2014
#include "TKOArm.h"

TKOArm* TKOArm::m_Instance = NULL;

/*
 * Initializes the 2 roller jaguars in percent Vbus mode
 *  \param int port 1 - roller 1 Jaguar ID
 *  \param int port 2- roller 2 Jaguar ID
 *  
 *  When match starts, our arm is looking up, tape in switch
 *  If not in tape, arm cannot work.
 *  We know forward is x units to floor, y units to back floor (y is negative, x positive)
 *  Range is from y to x, 0 is center (start position)
 */

TKOArm::TKOArm() :
	minArmPos(ARM_MINIMUM_POSITION), //TODO Critical, find out what these are...
	maxArmPos(ARM_MAXIMUM_POSITION),
	_arm(ARM_JAGUAR_ID, CANJaguar::kPercentVbus), 
	limitSwitchArm(ARM_OPTICAL_SWITCH), // Optical limit switch
	_roller(ROLLER_1_JAGUAR_ID, ROLLER_2_JAGUAR_ID),
	stick3(STICK_3_PORT),
	stick4(STICK_4_PORT)
{
	printf("Initializing intake\n");
	_arm.SetSafetyEnabled(true);
	_arm.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
	_arm.SetVoltageRampRate(0.0);
	_arm.ConfigFaultTime(0.1); 
	_arm.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_arm.ConfigEncoderCodesPerRev(250);
	_arm.EnableControl();
	armTask = new Task("TKOArm", (FUNCPTR) ArmRunner);
	armEnabled = true;
	if (limitSwitchArm.Get())
	{
		//kill the arm? because init position not in center
		//TODO Maybe remove this for testing
		/*armEnabled = false;
		_arm.StopMotor();*/
	}
	AddToSingletonList();
}

TKOArm::~TKOArm() 
{
	m_Instance = NULL;
}

TKOArm* TKOArm::inst() 
{
	if (!m_Instance) 
	{
		printf("TKOArm instance is null\n");
		m_Instance = new TKOArm;
		printf("TKOArm initialized\n");
	}
	return m_Instance;
}
void TKOArm::ArmRunner()
{
	while (DriverStation::GetInstance()->IsEnabled())
	{
		m_Instance->runManualArm();
	}
}
bool TKOArm::Start()
{
	if (not armTask->Verify())
		if (armTask->Start())
			return true;
	return false;
}
bool TKOArm::Stop()
{
	if (armTask->Verify())
		if (armTask->Stop())
			return true;
	return false;
}
void TKOArm::runManualArm()
{	
	_roller.rollerSimpleMove();
	//_roller.rollerManualMove();

	if (DriverStation::GetInstance()->GetDigitalIn(3))//if shooter running
	{
		if (not StateMachine::armCanMove or not armEnabled)
		{
			_arm.Set(0);
			return;
		}
	}
	DSLog(1, "Arm Pos: %f", _arm.GetPosition());
	DSLog(2, "Arm Volt: %f", _arm.GetOutputVoltage());
	DSLog(3, "Arm Curr %f", _arm.GetOutputCurrent());
	
	/*if (_arm.GetPosition() > minArmPos) //if we are farther back than we can be, only go forward
	{
		if (stick4.GetY() < 0)
			_arm.Set(stick4.GetY() * ARM_SPEED_MULTIPLIER);
		else
			_arm.Set(0);
	}
	else if (_arm.GetPosition() < maxArmPos)
	{
		if (stick4.GetY() > 0)
			_arm.Set(stick4.GetY() * ARM_SPEED_MULTIPLIER);
		else
			_arm.Set(0);
	}
	else*/
	{
		_arm.Set(stick4.GetY() * ARM_SPEED_MULTIPLIER);
	}
}
void TKOArm::moveToFront()
{
	TKOArm::switchToPositionMode();
	_arm.Set(maxArmPos);
}
void TKOArm::moveToMid()
{
	TKOArm::switchToPositionMode();
	_arm.Set(0.);
}
void TKOArm::moveToBack()
{
	TKOArm::switchToPositionMode();
	_arm.Set(minArmPos);
}
bool TKOArm::armInFiringRange()
{
	if (not limitSwitchArm.Get())
		return false;
	if (_arm.GetPosition() <= ARM_FIRING_LEFT_BOUND and _arm.GetPosition() >= ARM_FIRING_RIGHT_BOUND)
		return true;
	return false;
}
void TKOArm::switchToPositionMode()
{
	_arm.ChangeControlMode(_arm.kPosition);
	_arm.SetVoltageRampRate(3.); //TODO maybe don't need ramping voltage with pid
	_arm.ConfigSoftPositionLimits(maxArmPos, minArmPos);
}

