#include "WPILib.h"

class RobotDemo : public SimpleRobot
{
	Relay relay2, relay3, relay4, relay5;
	Compressor comp;
public:
	RobotDemo():
		relay2(2, Relay::kBothDirections),
		relay3(3, Relay::kBothDirections),
		relay4(4, Relay::kBothDirections),
		relay5(5, Relay::kBothDirections),
		comp(3,1)
	{}

	void Autonomous()
	{

	}

	void OperatorControl()
	{
		printf("STARTING OPERATOR\n");
		while (IsOperatorControl() && IsEnabled())
		{
			for (int i = 0; i < 1; i++)
			{
				if (true)
				{
					printf("Seding data\n");
				
					relay2.Set(relay2.kForward);
					relay3.Set(relay3.kForward);
					relay4.Set(relay4.kOff);
					relay5.Set(relay5.kOff);//record data on arduino
					Wait(0.1);
					relay5.Set(relay5.kForward);
					Wait(2.);
				}
				relay5.Set(relay5.kForward);
				relay2.Set(relay2.kForward);
				relay3.Set(relay3.kForward);
				relay4.Set(relay4.kForward);
				Wait(0.1);
			}
			Wait(10.);
			for (int i = 0; i < 1; i++)
			{
				if (true)
				{
					printf("Seding data\n");
					relay2.Set(relay2.kForward);
					relay3.Set(relay3.kOff);
					relay4.Set(relay4.kOff);
					relay5.Set(relay5.kOff);//record data on arduino
					Wait(0.1);
					relay5.Set(relay5.kForward);
					Wait(2.);
				}
				relay5.Set(relay5.kForward);
				relay2.Set(relay2.kForward);
				relay3.Set(relay3.kForward);
				relay4.Set(relay4.kForward);
				Wait(0.1);
			}
			Wait(10.);
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
