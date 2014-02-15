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
	stick3(STICK_3_PORT),
	stick4(STICK_4_PORT)
{
	printf("Initializing intake\n");
	TKORoller::inst();
	_arm.SetSafetyEnabled(false);
	_arm.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
	_arm.SetVoltageRampRate(0.0);
	_arm.ConfigFaultTime(0.1); 
	_arm.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_arm.ConfigEncoderCodesPerRev(250);
	_arm.EnableControl(0.);
	//switchToPositionMode();
	armTask = new Task("TKOArm", (FUNCPTR) ArmRunner, 1);
	armEnabled = true;
	if (limitSwitchArm.Get())
	{
		printf("ARM NOT IN SWITCH\n");
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
	while (true)
	{
		m_Instance->runManualArm();
		m_Instance->printDSMessages();
		Wait(0.05);
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
void TKOArm::printDSMessages()
{
	DSLog(1, "Arm Pos: %f", _arm.GetPosition());
	DSLog(2, "Arm Volt: %f", _arm.GetOutputVoltage());
	DSLog(3, "Arm Curr %f", _arm.GetOutputCurrent());
}
void TKOArm::runManualArm()
{	
	if (_arm.GetControlMode() == _arm.kPosition)
		switchToVBusMode();
	
	if (stick4.GetRawButton(8))
	{
		_arm.DisableControl();
		_arm.EnableControl(0.);
		_arm.SetPositionReference(_arm.kPosRef_QuadEncoder);
		printf("Reset encoder\n");
	}
	
	TKORoller::inst()->rollerSimpleMove();
	//TKORoller::inst()->rollerManualMove();
	
	if (DriverStation::GetInstance()->GetDigitalIn(5))//if shooter running
	{
		_arm.Set(stick4.GetY() * ARM_SPEED_MULTIPLIER);
		return;
	}
	
	if (not StateMachine::armCanMove or not armEnabled)
	{
		_arm.Set(0);
		return;
	}
	
	if (_arm.GetPosition() > minArmPos) //if we are farther back than we can be, only go forward
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
	else
	{
		_arm.Set(stick4.GetY() * ARM_SPEED_MULTIPLIER);
	}
}
void TKOArm::moveToFront()
{
	if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();
	_arm.Set(maxArmPos);
}
void TKOArm::moveToMid()
{
	if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();
	_arm.Set(0.);
}
void TKOArm::moveToBack()
{
	if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();
	_arm.Set(minArmPos);
}
bool TKOArm::armInFiringRange()
{
	/*if (not limitSwitchArm.Get())
		return false;*/
	return true;
	if (_arm.GetPosition() <= ARM_FIRING_LEFT_BOUND and _arm.GetPosition() >= ARM_FIRING_RIGHT_BOUND)
		return true;
	return false;
}
void TKOArm::switchToPositionMode()
{
	_arm.ChangeControlMode(_arm.kPosition);
	_arm.SetPID(10., 0.001, 0.);
	_arm.SetVoltageRampRate(3.); //TODO maybe don't need ramping voltage with pid
	_arm.ConfigSoftPositionLimits(maxArmPos, minArmPos);
}
void TKOArm::switchToVBusMode()
{
	_arm.ChangeControlMode(_arm.kPercentVbus);
	_arm.SetVoltageRampRate(0.0);
	_arm.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_arm.ConfigEncoderCodesPerRev(250);
}
