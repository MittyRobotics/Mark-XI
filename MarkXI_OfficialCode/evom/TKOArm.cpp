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
	_arm(ARM_JAGUAR_ID, CANJaguar::kPosition), 
	limitSwitchArm(ARM_OPTICAL_SWITCH), // Optical limit switch
	usonic(7),
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
	_arm.SetPID(-10000., -1., 0.);
	_arm.EnableControl(0.);
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
	DSClear();
	DSLog(1, "Arm Pos: %f", _arm.GetPosition());
	DSLog(2, "Arm Lim: %f", limitSwitchArm.Get());
	DSLog(3, "Arm Curr %f", _arm.GetOutputCurrent());
	DSLog(4, "Dist %f", usonic.GetVoltage() * 512. / 12.); //gets feet
}
void TKOArm::runManualArm()
{	
	printDSMessages();
	
	if (stick4.GetRawButton(8))
	{
		_arm.DisableControl();
		_arm.EnableControl(0.);
		printf("Reset encoder\n");
	}
	
	TKORoller::inst()->rollerSimpleMove();
	//TKORoller::inst()->rollerManualMove();
	
	/*if (DriverStation::GetInstance()->GetDigitalIn(5))//if override running
	{
		_arm.Set(stick4.GetY() * ARM_SPEED_MULTIPLIER);
		return;
	}
	if (not StateMachine::armCanMove or not armEnabled)
	{
		_arm.Set(_arm.Get());
		return;
	}*/
	
	if (stick4.GetRawButton(9))
	{
		while (limitSwitchArm.Get() and DriverStation::GetInstance()->IsEnabled())
		{
			_arm.Set(DriverStation::GetInstance()->GetAnalogIn(3));
			if (_arm.GetOutputCurrent() >= 30.)
				break;
		}
		_arm.Set(_arm.GetPosition());
		return;
	}
	
	if (stick4.GetRawButton(3))
		moveToFront();
	if (stick4.GetRawButton(2))
		moveToMid();
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
	_arm.SetSafetyEnabled(true);
	_arm.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
	_arm.SetVoltageRampRate(0.0);
	_arm.ConfigFaultTime(0.1); 
	_arm.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_arm.ConfigEncoderCodesPerRev(250);
	_arm.SetPID(-10000., -1., 0.);
	_arm.EnableControl(0.);
	//_arm.SetVoltageRampRate(3.); //TODO maybe don't need ramping voltage with pid
	_arm.ConfigSoftPositionLimits(maxArmPos, minArmPos);
}
void TKOArm::calibrateArm()	//TODO test this
{
//	switchToPositionMode();
	while (limitSwitchArm.Get() == 1)
		_arm.Set(0.3);	// go forward first
	minArmPos = _arm.GetPosition();
	_arm.Set(_arm.GetPosition());
	_arm.Set(0);
	Wait(0.1);
	while (limitSwitchArm.Get() == 1)
		_arm.Set(-0.3);
	maxArmPos = _arm.GetPosition();
	_arm.Set(_arm.GetPosition());
	_arm.Set(0);
}
