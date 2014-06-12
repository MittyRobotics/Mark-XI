#include "WPILib.h"
#include "swag.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
swag test;
public:
	RobotDemo():
		test()
	{
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous()
	{
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl()
	{
		printf("Starting the swag\n");
		Wait(0.5);
		
	    printf("Hello world! Inside MyRobot.cpp \n");
	    ///swag::getPointer()->swagOut(); <<<<<<<<<<<<<<<< uncomment this to explode, null pointer test
	    ///printf("%f", 1.0/0.0); <<<<<<<<<<<<<<<<<<<<<<<< uncomment this to explode, divide by zero
	    //////BEFORE UNCOMMENTING THE TESTS
	    /*
	     * Make sure the code works (and prints to netconsole) as it is
	     */

		while (IsOperatorControl())
		{
			Wait(0.005);				// wait for a motor update time
		}
		
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

