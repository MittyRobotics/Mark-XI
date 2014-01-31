//Last edited by Vadim Korolik
//on 02/09/2013|
#include "TKOAutonomous.h"

///Constructor for the TKOAutonomous class

TKOAutonomous::TKOAutonomous(int port1, int port2, int port3, int port4) :
	rsFrontLoaderWrist(PN_R1_ID), rsFrontLoaderLift(PN_R2_ID), drive1(port1, CANJaguar::kPosition), drive2(port2, CANJaguar::kPercentVbus), drive3(port3, CANJaguar::kPosition), drive4(port4, CANJaguar::kPercentVbus)
{
	initAutonomous();
}
void TKOAutonomous::initAutonomous()
{
	printf("Initializing Autonomous Class");
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
	drive2.EnableControl();
	drive2.SetSafetyEnabled(false);

	drive4.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	drive4.EnableControl();
	drive4.SetSafetyEnabled(false);

	runningAuton = false;
	rampRate = RAMP_RATE;
	rampRate2 = RAMP_RATE_2;
	reachedTarget = false;
	ds = DriverStation::GetInstance();
	printf("Initialized Autonomous Class\n");
}

void TKOAutonomous::startAutonomous()
{
	autonTimer.Reset();
	autonTimer.Start();
	TKOGyro::inst()->reset();
	rightTarget = 0;
	leftTarget = 0;
	runningAuton = true;
	reachedTarget = false;
	printf("Started Autonomous \n");
}
void TKOAutonomous::stopAutonomous()
{
	runningAuton = false;
	autonTimer.Reset();
	printf("Stopped Autonomous \n");
	//stop evom tasks
}
void TKOAutonomous::autonSetup1() //right corner
{
	/*
	 * 21, autonSetup atom 1, turn to data
	 * 22, autonSetup atom 2, drive to data
	 * 23, autonSetup atom 1, turn to data
	 * 24, dump front loader
	 * 25, crashed into wall, data = 1 means while driving, data = 2 means while turning
	 */
	if (ds->IsDisabled())
		stopAutonomous();
	if (!ds->IsAutonomous())
		stopAutonomous();

	{ //this is one "Atom" that performs one task, in this case turns 90 degrees
		//writeMD(21, 30.);
		setTargetAngle(30);
		if (not turn(targetAngle) and not autonOption[0])
			return;
		else
			autonOption[0] = true;
	}
	//second "Atom"
	//writeMD(22, 3 * REVS_PER_METER);
	setDriveTargetStraight(3 * REVS_PER_METER);
	if (not PIDDriveStraight() and not autonOption[1])
		return;
	else
		autonOption[1] = true;

	//third "Atom"
	setTargetAngle(30);
	//writeMD(23, 30.);
	if (not turn(targetAngle) and not autonOption[2])
		return;
	else
		autonOption[2] = true;

	if (not autonOption[3])
	{
		//writeMD(24);
		rsFrontLoaderWrist.SetOn(1);
		rsFrontLoaderLift.SetOn(1);
		Wait(1);
		rsFrontLoaderWrist.SetOn(0);
		rsFrontLoaderLift.SetOn(0);
		autonOption[3] = false;
	}
	printf("Finished autonSetup 1\n");
	DSLog(3, "Finished autonSetup 1\n");

	//Stringing them in a row like so
}
void TKOAutonomous::autonSetup2() //right corner
{
	if (ds->IsDisabled())
		stopAutonomous();
	if (!ds->IsAutonomous())
		stopAutonomous();

	{ //this is one "Atom" that performs one task, in this case turns 90 degrees
		setTargetAngle(45);
		if (not turn(targetAngle) and not autonOption[0])
			return;
		else
			autonOption[0] = true;
	}
	//second "Atom"
	setDriveTargetStraight(2 * REVS_PER_METER);
	if (not PIDDriveStraight() and not autonOption[1])
		return;
	else
		autonOption[1] = true;

	//third "Atom"
	setTargetAngle(30);
	if (not turn(targetAngle) and not autonOption[2])
		return;
	else
		autonOption[2] = true;

	//fourth "Atom"
	setDriveTargetStraight(1 * REVS_PER_METER);
	if (not PIDDriveStraight() and not autonOption[3])
		return;
	else
		autonOption[3] = true;

	if (not autonOption[4])
	{
		rsFrontLoaderWrist.SetOn(1);
		rsFrontLoaderLift.SetOn(1);
		Wait(1);
		rsFrontLoaderWrist.SetOn(0);
		rsFrontLoaderLift.SetOn(0);
		autonOption[4] = false;
	}
	printf("Finished autonSetup 2\n");
	DSLog(3, "Finished autonSetup 2\n");

	//Stringing them in a row like so
}
void TKOAutonomous::autonSetup3() //right corner
{
	if (ds->IsDisabled())
		stopAutonomous();
	if (!ds->IsAutonomous())
		stopAutonomous();

	{ //this is one "Atom" that performs one task, in this case turns 90 degrees
		setTargetAngle(90);
		if (not turn(targetAngle) and not autonOption[0])
			return;
		else
			autonOption[0] = true;
	}
	//second "Atom"
	setDriveTargetStraight(1 * REVS_PER_METER);
	if (not PIDDriveStraight() and not autonOption[1])
		return;
	else
		autonOption[1] = true;

	//third "Atom"
	setTargetAngle(0);
	if (not turn(targetAngle) and not autonOption[2])
		return;
	else
		autonOption[2] = true;

	//fourth "Atom"
	setDriveTargetStraight(3 * REVS_PER_METER);
	if (not PIDDriveStraight() and not autonOption[3])
		return;
	else
		autonOption[3] = true;

	//fifth "Atom"
	setTargetAngle(35);
	if (not turn(targetAngle) and not autonOption[4])
		return;
	else
		autonOption[4] = true;

	//sixth "Atom"
	setDriveTargetStraight(1.5 * REVS_PER_METER);
	if (not PIDDriveStraight() and not autonOption[5])
		return;
	else
		autonOption[5] = true;

	if (not autonOption[6])
	{
		rsFrontLoaderWrist.SetOn(1);
		rsFrontLoaderLift.SetOn(1);
		Wait(1);
		rsFrontLoaderWrist.SetOn(0);
		rsFrontLoaderLift.SetOn(0);
		autonOption[6] = false;
	}
	printf("Finished autonSetup 2\n");
	DSLog(3, "Finished autonSetup 2\n");

	//Stringing them in a row like so
}
void TKOAutonomous::autonomousCode()
{
	if (ds->IsDisabled())
		stopAutonomous();
	if (!ds->IsAutonomous())
		stopAutonomous();

	printMessages();

	if (autonTimer.Get() > 3)
	{
		rampRate += rampRate2;
		printf("About to start turning \n");
		if (turn(ds->GetAnalogIn(3) * 100))
		{
			PIDDriveStraight();
		}
		else
		{
			printf("Needs to turn \n");
		}
	}
}
bool TKOAutonomous::driveLeft()
{
	if (leftTarget > -rampTargetLeft)
	{
		leftTarget -= rampRate;
		drive1.Set(leftTarget); //sets pid drive target
		printf("Setting left target to: %f", leftTarget);
		printf("\n");
		drive2.Set(-drive1.GetOutputVoltage() / drive1.GetBusVoltage());
	}
	else if (leftTarget <= rampTargetLeft)
	{
		return true;
	}
	return false;
}
bool TKOAutonomous::driveRight()
{
	if (rightTarget > -rampTargetRight)
	{
		rightTarget -= rampRate;
		drive3.Set(rightTarget); //same, but for jag 3 since only 1 and 3 have encoders
		drive4.Set(-drive3.GetOutputVoltage() / drive3.GetBusVoltage()); //sets second and fourth jags in slave mode
	}
	else if (rightTarget <= rampTargetRight)
	{
		return true;
	}
	return false;
}

bool TKOAutonomous::PIDDriveStraight()
{
	printf("Driving straight!\n");
	driveLeft();
	driveRight();
	if (drive1.GetOutputCurrent() > DRIVE_CURRENT_CUTOFF or drive3.GetOutputCurrent() > DRIVE_CURRENT_CUTOFF)
	{
		printf("Crashed into wall...");
		//writeMD(25, 1);
		return true;
	}

	if (driveLeft() and driveRight())
	{
		resetEncoders();
		return true;
	}
	return false;
}
bool TKOAutonomous::turn(double target)//takes negative values
{
	double currAngle = TKOGyro::inst()->GetAngle();
//	double currAngle = 0;

	if (drive1.GetOutputCurrent() > DRIVE_CURRENT_CUTOFF or drive3.GetOutputCurrent() > DRIVE_CURRENT_CUTOFF)
	{
		printf("Crashed into wall...");
		TKOLogger::inst()->addMessage("Crashed into a wall in auton...");
		return true;
	}

	if (currAngle < target - 1 && !(target < currAngle))
	{
		printf("Turning right \n");
		drive2.Set(0.7);
		drive4.Set(0.7);
	}
	if (currAngle + 2.0 >= target)
	{
		if (currAngle - 2.0 <= target)
		{
			drive2.Set(0);
			drive4.Set(0);
			printf("Reached goal, breaking out \n");

			if (!reachedTarget)
			{
				reachedTarget = true;
				resetEncoders();
				printf("Reset position\n"); //All this resetting does not work properly, needs to be fixed
				//Cannot go without it because when the robot turns, it affects the position
			}
			return true;
		}
	}
	if (currAngle > target && !(target > currAngle))
	{
		printf("Past target \n");
		drive2.Set(-0.8);
		drive4.Set(-0.8);
	}
	return false;
}

void TKOAutonomous::resetEncoders()
{
	drive1.EnableControl(0);
	drive3.EnableControl(0);
}

void TKOAutonomous::printMessages()
{
//	DSLog(2, "Gyro Angle: %f", _gyro->GetAngle());
	DSLog(3, "Timer: %f", autonTimer.Get());
	DSLog(4, "Drive1 pos: %f", getPosition(1)); //uses the TKOAutonomous getPosition
	DSLog(5, "Drive3 pos: %f", getPosition(3)); //uses the TKOAutonomous getPosition
	DSLog(6, "Jag 3 Target: %f", getTarget(3));
	printf("\n");
//	printf("Gyro Angle: %f", _gyro->GetAngle());
	printf("\n");
	printf("Timer: %f", autonTimer.Get());
	printf("\n");
	printf("Drive1 pos: %f", getPosition(1)); //uses the TKOAutonomous getPosition
	printf("\n");
	printf("Drive3 pos: %f", getPosition(3)); //uses the TKOAutonomous getPosition
	printf("\n");
	printf("Jag 3 Target: %f", getTarget(3));
	printf("\n");
}
void TKOAutonomous::setDrivePID(float P, float I, float D) //Sets drive1 and drive3 PID because only they have encoders
{
	drive1.SetPID(P, I, D);
	drive3.SetPID(P, I, D);
}
void TKOAutonomous::setDriveTargetStraight(float target)
{
	rampTargetLeft = target;
	rampTargetRight = target;
}
void TKOAutonomous::setTargetAngle(float target)
{
	targetAngle = target;
}
float TKOAutonomous::getPosition(int jaguar) //1 and 3 are only ones with encoders
{
	if (jaguar == 1)
		return drive1.GetPosition();
	else if (jaguar == 3)
		return drive3.GetPosition();
	else
		return 13.51; //if it gives 13.51, then you entered wrong jaguar id
}
float TKOAutonomous::getTarget(int jaguar) //1 and 3 are only ones with encoders
{
	if (jaguar == 1)
		return leftTarget;
	else if (jaguar == 3)
		return rightTarget;
	else
		return 13.51; //if it gives 13.51, then you entered wrong jaguar id
}
///Destructor for the TKOAutonomous class
TKOAutonomous::~TKOAutonomous()
{
}
