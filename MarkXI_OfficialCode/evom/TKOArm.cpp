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
	_arm(ARM_JAGUAR_ID, CANJaguar::kPosition), 
	limitSwitchArm(ARM_OPTICAL_SWITCH), // Optical limit switch
	usonic(ULTRASONIC_PORT),
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
	//_arm.ConfigSoftPositionLimits(maxArmPos, minArmPos);
	_arm.SetPID(-10000., -1., 0.);
	_arm.EnableControl(0.);
	lastInc = GetTime();
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
		m_Instance->currentTimeout();
		Wait(0.01);
	}
}
float TKOArm::getDistance()
{
	return (usonic.GetVoltage() * 512. / 12.);
}
AnalogChannel* TKOArm::getUsonic()
{
	return &usonic;
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
	float avr = 0.;
	while (usonicVals.size() < 10)
	{
		usonicVals.push(usonic.GetVoltage() * 512/12.);
		usonicAvr += usonic.GetVoltage() * 512/12.;
	}
	
	usonicAvr -= usonicVals.back();
	usonicAvr += usonic.GetVoltage() * 512/12.;
	usonicVals.push(usonic.GetVoltage() * 512/12.);
	usonicVals.pop();
	avr = usonicAvr / usonicVals.size();
	
	DSClear();
	DSLog(1, "Arm Pos: %f", _arm.GetPosition());
	DSLog(2, "Arm Lim: %f", limitSwitchArm.Get());
	DSLog(3, "Arm Curr %f", _arm.GetOutputCurrent());
	DSLog(4, "Arm Tar %f", _arm.Get());
	DSLog(5, "DistR %f", avr); //gets feet
	DSLog(6, "Dist %f", usonic.GetVoltage() * 512/12.); //gets feet
}
void TKOArm::currentTimeout()
{
	if (_arm.GetOutputCurrent() >= ARM_CURRENT_THRESHOLD)
	{
		printf("Arm current timeout\n");
		Timer timeout;
		timeout.Start();
		/*while (timeout.Get() <= ARM_CURRENT_TIMEOUT)
		{
			_arm.Set(_arm.GetPosition());
		}*/
		timeout.Stop();
		//_arm.EnableControl();
	}
}
void TKOArm::runManualArm()
{	
	if (stick4.GetRawButton(8))
	{
		_arm.DisableControl();
		_arm.EnableControl(0.);
		printf("Reset encoder\n");
	}
	
	TKORoller::inst()->rollerSimpleMove();
	//TKORoller::inst()->rollerManualMove();
	if (_arm.GetPosition() <= -0.05)
		TKORoller::inst()->rollerIn();
	
	if (DriverStation::GetInstance()->GetDigitalIn(5))//if override running
	{
		_arm.Set(stick4.GetY() * ARM_SPEED_MULTIPLIER);
		return;
	}
	if (not StateMachine::armCanMove or not armEnabled)
	{
		printf("Arm can't move\n");
		_arm.Set(_arm.GetPosition());
		return;
	}
	
	/*if (stick4.GetRawButton(9))
	{
		while (limitSwitchArm.Get() and DriverStation::GetInstance()->IsEnabled())
		{
			_arm.Set(.9);
			if (_arm.GetOutputCurrent() >= 30.)
				break;
		}
		_arm.Set(_arm.Get());
		return;
	}*/
	
	if (stick4.GetRawButton(5))
		moveToFront();
	if (stick4.GetRawButton(2))
		moveToMid();
	if (stick4.GetRawButton(4))
		moveToBack();
	if (stick4.GetRawButton(3))
		moveToDSTarget();
	if (GetTime() - lastInc <= 1.){}
	else
	{
		if (stick4.GetRawButton(6))
		{
			_arm.Set(_arm.Get() + 0.005);
			lastInc = GetTime();
		}
		if (stick4.GetRawButton(7))
		{
			_arm.Set(_arm.Get() - 0.005);
			lastInc = GetTime();
		}
	}
	
	_arm.Set(_arm.Get());
}
void TKOArm::moveToFront()
{
	/*if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();*/
	_arm.Set(maxArmPos);
}
void TKOArm::moveToMid()
{
	/*if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();*/
	_arm.Set(0.);
}
void TKOArm::moveToBack()
{
	/*if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();*/
	_arm.Set(minArmPos);
}
void TKOArm::moveToDSTarget()
{
	/*if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();*/
	_arm.Set(DriverStation::GetInstance()->GetAnalogIn(4));
}
bool TKOArm::armInFiringRange()
{
	/*if (not limitSwitchArm.Get())
		return false;*/
	return true;
	if (_arm.GetPosition() >= ARM_FIRING_LEFT_BOUND and _arm.GetPosition() <= ARM_FIRING_RIGHT_BOUND)
		return true;
	return false;
}
void TKOArm::switchToPositionMode()
{
	printf("switching to position\n");
	//_arm.ChangeControlMode(_arm.kPosition);
	_arm.SetSafetyEnabled(false);
	_arm.ConfigNeutralMode(CANJaguar::kNeutralMode_Coast);  
	_arm.SetVoltageRampRate(0.0);
	_arm.ConfigFaultTime(0.1); 
	_arm.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_arm.ConfigEncoderCodesPerRev(250);
	_arm.SetPID(-10000., -1., 0.);
	//_arm.EnableControl(0.);
	//_arm.SetVoltageRampRate(3.); //TODO maybe don't need ramping voltage with pid
}
