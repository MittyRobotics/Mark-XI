#include "WPILib.h"

class RobotDemo : public SimpleRobot
{
	CANJaguar _arm;
	DigitalInput armOpt;
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
		armOpt(11)
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
		}
	}

	void Test() 
	{

	}
};

START_ROBOT_CLASS(RobotDemo);
