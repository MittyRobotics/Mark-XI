#include "WPILib.h"

class RobotDemo : public SimpleRobot
{
	CANJaguar _arm;
	DigitalInput armOpt, pistRetract, pistExtend, latch, isCocked, ballLim;
	/*const int PISTON_SWITCH_RETRACT_CHANNEL = 7;
	const int PISTON_SWITCH_EXTEND_CHANNEL = 6;
	const int LATCH_PISTON_LOCK_SWITCH_CHANNEL = 1;
	const int IS_COCKED_SWITCH_CHANNEL = 2;
	const int BALL_LIMIT_SWITCH = 10;
	const int ARM_OPTICAL_SWITCH = 11;
	const int ULTRASONIC_PORT = 7;*/
	
public:
	RobotDemo():
		_arm(7, CANJaguar::kPercentVbus),
		armOpt(11),
		pistRetract(7),
		pistExtend(6),
		latch(1),
		isCocked(2),
		ballLim(10)
	{}
	
	void RobotInit()
	{
		_arm.SetPositionReference(_arm.kPosRef_QuadEncoder);
		_arm.ConfigEncoderCodesPerRev(250);
	}

	void Autonomous()
	{

	}

	void OperatorControl()
	{
		printf("STARTING OPERATOR\n");
		while (IsOperatorControl() && IsEnabled())
		{
			printf("arm: %f\n", _arm.GetPosition());
			DriverStation::GetInstance()->SetDigitalOut(1, armOpt.Get());
			DriverStation::GetInstance()->SetDigitalOut(2, pistRetract.Get());
			DriverStation::GetInstance()->SetDigitalOut(3, pistExtend.Get());
			DriverStation::GetInstance()->SetDigitalOut(4, latch.Get());
			DriverStation::GetInstance()->SetDigitalOut(5, isCocked.Get());
			DriverStation::GetInstance()->SetDigitalOut(6, ballLim.Get());
		}
	}

	void Test() 
	{

	}
};

START_ROBOT_CLASS(RobotDemo);
