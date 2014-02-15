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
	printf("Initializing TKOArm\n");
	_arm.SetSafetyEnabled(false);
	_arm.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
	_arm.SetVoltageRampRate(0.0);
	_arm.ConfigFaultTime(0.1); 
	_arm.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_arm.ConfigEncoderCodesPerRev(250);
	_arm.EnableControl(0.);
	armTask = new Task("TKOArm", (FUNCPTR) ArmRunner, 1);
	armEnabled = true;
	runningVBus = true;
	lastToggle = GetTime();
	if (limitSwitchArm.Get())
	{
		//kill the arm? because init position not in center
		//TODO Maybe remove this for testing
		/*armEnabled = false;
		_arm.StopMotor();*/
	}
	AddToSingletonList();
	printf("Initialized TKOArm\n");
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
		TKORoller::inst()->rollerSimpleMove();
		m_Instance->runManualArm();
		m_Instance->runPositionArm();
		Wait(LOOPTIME);
	}
}
bool TKOArm::Start()
{
	printf("Starting TKOArm task\n");
	if (not armTask->Verify())
		if (armTask->Start())
		{
			printf("Started TKOArm task\n");
			return true;
		}
	return false;
}
bool TKOArm::Stop()
{
	printf("Stopping TKOArm task\n");
	if (armTask->Verify())
		if (armTask->Stop())
		{
			printf("Stopped TKOArm task\n");
			return true;
		}
	return false;
}
void TKOArm::runPositionArm()
{
	printf("Running arm position mode\n");
	if (runningVBus)
		return;
	DSLog(1, "Arm Pos: %f", _arm.GetPosition());
	DSLog(2, "Arm Volt: %f", _arm.GetOutputVoltage());
	DSLog(3, "Arm Curr %f", _arm.GetOutputCurrent());
	
	if (_arm.GetControlMode() == _arm.kPercentVbus)
		switchToPositionMode();
	
	float delta = _arm.Get() - _arm.GetPosition();
	
	if (delta < 0.1 and delta > -0.1) //TODO tune this
		runningVBus = true;
	else
		runningVBus = false;
	printf("Ran arm position mode\n");
}
void TKOArm::runManualArm()
{	
	printf("Running arm manual mode\n");
	if (!runningVBus)
		return;
	DSLog(1, "Arm Pos: %f", _arm.GetPosition());
	DSLog(2, "Arm Volt: %f", _arm.GetOutputVoltage());
	DSLog(3, "Arm Curr %f", _arm.GetOutputCurrent());
	
	if (_arm.GetControlMode() == _arm.kPosition)
		switchToVBusMode();
	
	if (stick4.GetRawButton(8))
	{
		_arm.DisableControl();
		_arm.EnableControl(0.);
		_arm.SetPositionReference(_arm.kPosRef_QuadEncoder);
		printf("Reset arm encoder...\n");
	}
	
	/*if (DriverStation::GetInstance()->GetDigitalIn(5))//if override
	{
		_arm.Set(stick4.GetY() * ARM_SPEED_MULTIPLIER); //override
		return;
	}*/
	
	if (not StateMachine::armCanMove or not armEnabled) //safety, state machine safe positions
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
	printf("Ran arm manual mode\n");
}
void TKOArm::moveToFront()
{
	printf("Bringing arm forward\n");
	runningVBus = false;
	if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();
	_arm.Set(maxArmPos);
}
void TKOArm::moveToMid()
{
	printf("Bringing arm mid\n");
	runningVBus = false;
	if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();
	_arm.Set(0.);
}
void TKOArm::moveToBack()
{
	printf("Bringing arm back\n");
	runningVBus = false;
	if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();
	_arm.Set(minArmPos);
}
bool TKOArm::armInFiringRange()
{
	if (not limitSwitchArm.Get())
		return true;

	/*if (_arm.GetPosition() >= ARM_FIRING_LEFT_BOUND and _arm.GetPosition() <= ARM_FIRING_RIGHT_BOUND)
		return true;*/
	return false;
}
void TKOArm::toggleMode()
{
	printf("Arm toggling mode\n");
	if (GetTime() - lastToggle <= 1.) return; //1.0 means logs every 1 second
	runningVBus = ! runningVBus;
	lastToggle = GetTime();
	printf("Arm toggled mode\n");
}
void TKOArm::switchToPositionMode()
{
	printf("Arm switching to position mode\n");
	_arm.ChangeControlMode(_arm.kPosition);
	_arm.SetPID(10., 0.001, 0.);
	_arm.SetVoltageRampRate(3.); //TODO maybe don't need ramping voltage with pid
	_arm.ConfigSoftPositionLimits(maxArmPos, minArmPos);
	_arm.ConfigEncoderCodesPerRev(250);
	_arm.SetSafetyEnabled(false);
	_arm.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
}
void TKOArm::switchToVBusMode()
{
	printf("Arm switching to vBus mode\n");
	_arm.ChangeControlMode(_arm.kPercentVbus);
	_arm.SetVoltageRampRate(0.0);
	_arm.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_arm.ConfigEncoderCodesPerRev(250);
	_arm.SetSafetyEnabled(false);
	_arm.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);
}
