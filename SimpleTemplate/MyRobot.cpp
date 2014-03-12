#include "WPILib.h"

class RobotDemo : public SimpleRobot
{
	CANJaguar _arm;
public:
	RobotDemo():
		_arm(7, CANJaguar::kPercentVbus)
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
