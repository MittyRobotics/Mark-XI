#include "WPILib.h"

class RobotDemo : public SimpleRobot
{
	Relay relay2, relay3, relay4, relay5;
	CANJaguar d1, d2, d3, d4;
	Compressor comp;
public:
	RobotDemo():
		relay2(2, Relay::kBothDirections),
		relay3(3, Relay::kBothDirections),
		relay4(4, Relay::kBothDirections),
		relay5(5, Relay::kBothDirections),
		d1(1, CANJaguar::kPosition),
		d2(4, CANJaguar::kPercentVbus),
		d3(2, CANJaguar::kPosition),
		d4(3, CANJaguar::kPercentVbus),
		comp(3,1)
	{
		d1.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
		d1.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		d1.ConfigEncoderCodesPerRev(250);
		d1.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		d1.SetSafetyEnabled(false); //new before true

		d3.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
		d3.SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		d3.ConfigEncoderCodesPerRev(250);
		d3.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		d3.SetSafetyEnabled(false);

		d2.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		d2.SetSafetyEnabled(false);

		d4.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		d4.SetSafetyEnabled(false);

		d1.SetExpiration(0.1);
		d2.SetExpiration(0.1);
		d3.SetExpiration(0.1);
		d4.SetExpiration(0.1);
		
		d1.SetVoltageRampRate(24.0);
		d2.SetVoltageRampRate(24.0);
		d3.SetVoltageRampRate(24.0);
		d4.SetVoltageRampRate(24.0);
	}

	void Autonomous()
	{

	}

	void OperatorControl()
	{
		printf("STARTING OPERATOR\n");
		while (IsOperatorControl() && IsEnabled())
		{
			printf("d1 pos: %f\n", d1.GetPosition());
			printf("d3 pos: %f\n", d3.GetPosition());
		}
	}

	void Test() 
	{
		comp.Start();
		while (IsEnabled())
		{
		}
		comp.Stop();
	}
};

START_ROBOT_CLASS(RobotDemo);
