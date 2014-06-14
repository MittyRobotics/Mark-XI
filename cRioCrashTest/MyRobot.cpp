#include "WPILib.h"
#include "swag.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */

class RobotDemo: public SimpleRobot {
	swag test;

public:
	RobotDemo() :
		test()
	{}

	void Disabled()
	{
		printf("Robot successfully died!\n");
		while (IsDisabled())
		{
			Wait(.1);
		}
	}

	void Autonomous() {}
	
	void OperatorControl()
	{
		printf("Starting the swag\n");
		Wait(0.5);

		printf("Hello world! Inside MyRobot.cpp \n");
		
		swag::getPointer()->swagOut(); //prints out "0.000000 swagging out", code does not crash
		
		printf("Dividing by zero...\t%f", 1.0/0.0); //prints out "Inf", code does not crash
		
		while (IsOperatorControl() and IsEnabled())
		{
			Wait(0.005); // wait for a motor update time
		}

	}

	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo)
;
