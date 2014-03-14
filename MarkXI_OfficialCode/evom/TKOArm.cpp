//Last edited by Vadim Korolik
//on 03/01/14
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
	stick1(STICK_1_PORT),
	stick2(STICK_2_PORT),
	stick3(STICK_3_PORT),
	stick4(STICK_4_PORT)
{
	printf("Initializing intake\n");
	_arm.SetSafetyEnabled(false);
	_arm.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);  
	_arm.SetVoltageRampRate(0.0);
	_arm.ConfigFaultTime(0.1); 
	_arm.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_arm.ConfigEncoderCodesPerRev(250);
	//_arm.ConfigSoftPositionLimits(maxArmPos, minArmPos);
	//_arm.SetPID(-10000., -1., 0.);
	_arm.SetPID(-5000., -.5, 0.);
	_arm.EnableControl(0.);
	lastInc = GetTime();
	lastCalib = GetTime();
	//switchToPositionMode();
	armTask = new Task("TKOArm", (FUNCPTR) ArmRunner, 1);
	armEnabled = true;
	armTargetCurrent = _arm.Get();
	armTargetFinal = armTargetCurrent;
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
		m_Instance->armTargetUpdate();
		Wait(0.005);
	}
}
float TKOArm::getDistance()
{
	return (usonic.GetVoltage() / ULTRASONIC_CONVERSION_TO_FEET);
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
	float tempVal = usonic.GetVoltage() / ULTRASONIC_CONVERSION_TO_FEET;
	/*float avr = 0.;
	while (usonicVals.size() < 10)
	{
		usonicVals.push(usonic.GetVoltage() / ULTRASONIC_CONVERSION_TO_FEET);
		usonicAvr += tempVal;
	}
	
	usonicAvr -= usonicVals.back();
	usonicAvr += tempVal;
	usonicVals.push(tempVal);
	usonicVals.pop();
	avr = usonicAvr / usonicVals.size();*/
	DriverStation::GetInstance()->SetDigitalOut(8,limitSwitchArm.Get());
	DSClear();
	DSLog(1, "Arm Pos: %f", _arm.GetPosition());
	DSLog(3, "Arm Curr %f", _arm.GetOutputCurrent());
	DSLog(4, "Arm Tar %f", _arm.Get());
	//DSLog(5, "DistR %f", avr); //gets feet
	DSLog(6, "Dist %f", tempVal); //gets feet
	
}
void TKOArm::forwardCalibration()
{
	if (GetTime() - lastCalib < 1.)
		return;
	printf("Running front calib\n");
	armEnabled = false;
	resetEncoder();
	printf("arm tar %f\n", _arm.Get());
	printf("arm pos %f\n", _arm.GetPosition());
	_arm.Set(0.);
	Timer _temp;
	_temp.Start();
	//float val = _arm.Get();
	Wait(1.);
	printf("arm tar %f\n", _arm.Get());
	printf("arm pos %f\n", _arm.GetPosition());
	_arm.Set(0.);
	while (_temp.Get() < 5. and limitSwitchArm.Get())
	{
		//val += 0.00001;
		printf("arm tar %f\n", _arm.Get());
		printf("arm pos %f\n", _arm.GetPosition());
		_arm.Set(_arm.Get() - 0.0001);
		//_arm.Set(val);
	}
	printf("Out of while loop calib\n");
	_arm.Set(_arm.GetPosition());
	_temp.Stop();
	resetEncoder();
	lastCalib = GetTime();
	armEnabled = true;
	printf("Done with front calib!\n");
}
void TKOArm::reverseCalibration()
{
	if (GetTime() - lastCalib < 1.)
		return;
	printf("Running rev calib\n");
	armEnabled = false;
	resetEncoder();
	printf("arm tar %f\n", _arm.Get());
	printf("arm pos %f\n", _arm.GetPosition());
	_arm.Set(0.);
	Timer _temp;
	_temp.Start();
	//float val = _arm.Get();
	Wait(1.);
	printf("arm tar %f\n", _arm.Get());
	printf("arm pos %f\n", _arm.GetPosition());
	_arm.Set(0.);
	while (_temp.Get() < 5. and limitSwitchArm.Get())
	{
		//val += 0.00001;
		printf("arm tar %f\n", _arm.Get());
		printf("arm pos %f\n", _arm.GetPosition());
		_arm.Set(_arm.Get() + 0.0001);
		//_arm.Set(val);
	}
	printf("Out of while loop calib\n");
	_arm.Set(_arm.GetPosition());
	_temp.Stop();
	resetEncoder();
	lastCalib = GetTime();
	armEnabled = true;
	printf("Done with rev calib!\n");
}
void TKOArm::setArmTarget(float target)
{
	armTargetFinal = target;
}
void TKOArm::armTargetUpdate()
{
	if (not armEnabled)
		return;
	if (armTargetFinal < armTargetCurrent)
	{
		armTargetCurrent -= ARM_TARGET_RAMP_INCREMENT; //TODO Arm increment, going forward
	}
	else if (armTargetFinal > armTargetCurrent)
	{
		armTargetCurrent += ARM_TARGET_RAMP_INCREMENT;
	}
	_arm.Set(armTargetCurrent);
}
void TKOArm::currentTimeout()
{
	if (_arm.GetOutputCurrent() >= ARM_CURRENT_THRESHOLD)
	{
		printf("Arm current timeout\n");
		//Timer timeout;
		//timeout.Start();
		/*while (timeout.Get() <= ARM_CURRENT_TIMEOUT)
		{
			_arm.Set(_arm.GetPosition());
			//TODO Do something here
		}*/
		//timeout.Stop();
		//_arm.EnableControl();
	}
}
void TKOArm::resetEncoder()
{
	_arm.DisableControl();
	_arm.EnableControl(0.);
	armTargetCurrent = 0.;
	armTargetFinal = 0.;
	printf("Reset encoder\n");
}
void TKOArm::runManualArm()
{	
	if (stick4.GetRawButton(8))
	{
		resetEncoder();
	}
	if (stick2.GetRawButton(10))
	{
		reverseCalibration();
		return;
	}
	if (stick2.GetRawButton(11))
	{
		forwardCalibration();
		return;
	}
	
	TKORoller::inst()->rollerSimpleMove();
	//TKORoller::inst()->rollerManualMove();
	/*if (_arm.GetPosition() <= -0.05)
		TKORoller::inst()->rollerIn();*/
	//TODO ^
	
	if (DriverStation::GetInstance()->GetDigitalIn(5))//if override running
	{
		setArmTarget(stick4.GetY() * ARM_SPEED_MULTIPLIER);
		return;
	}
	if (/*not StateMachine::armCanMove or*/ StateMachine::getCockingSwitch()->Get() or not armEnabled)
	{
		printf("Arm can't move, \n");
		setArmTarget(_arm.GetPosition());
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
	/*else
		moveToMid();*/
	if (GetTime() - lastInc <= 1.){}
	else
	{
		if (stick4.GetRawButton(6))
		{
			setArmTarget(_arm.Get() + ARM_MANUAL_DRIVE_INREMENT);
			lastInc = GetTime();
		}
		if (stick4.GetRawButton(7))
		{
			setArmTarget(_arm.Get() - ARM_MANUAL_DRIVE_INREMENT);
			lastInc = GetTime();
		}
	}
	
	//_arm.Set(_arm.Get()); //todo DO WE NEED THIS
	
}
void TKOArm::moveToFront()
{
	/*if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();*/
	setArmTarget(maxArmPos);
}
void TKOArm::moveToMid()
{
	/*if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();*/
	setArmTarget(0.);
}
void TKOArm::moveToBack()
{
	/*if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();*/
	setArmTarget(minArmPos);
}
void TKOArm::moveToDSTarget()
{
	/*if (_arm.GetControlMode() == _arm.kPercentVbus)
		TKOArm::switchToPositionMode();*/
	setArmTarget(DriverStation::GetInstance()->GetAnalogIn(4));
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
	_arm.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);  
	_arm.SetVoltageRampRate(0.0);
	_arm.ConfigFaultTime(0.1); 
	_arm.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	_arm.ConfigEncoderCodesPerRev(250);
	_arm.SetPID(-10000., -1., 0.);
	//_arm.EnableControl(0.);
	//_arm.SetVoltageRampRate(3.); //TODO maybe don't need ramping voltage with pid
}
