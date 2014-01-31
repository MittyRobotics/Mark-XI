#include "WPILib.h"
#include "Drive.h"
#include "vision/AxisCamera.h"
#include "vision/MonoImage.h"
#include "vision/RGBImage.h"
#include "vision/BinaryImage.h"
#include "vision/ColorImage.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */
class Chassis: public SimpleRobot
{
		Joystick stick1, stick2, stick3, stick4; // only joystick
		Drive drive;
		Servo servo;

	public:
		Chassis(void) :
			stick1(1), stick2(2), stick3(3), stick4(4), drive(), servo(2)
		{
		}

		/**
		 * Drive left & right motors for 2 seconds then stop
		 */
		void Autonomous(void)
		{
		}

		/**
		 * Runs the motors with arcade steering. 
		 */
		void OperatorControl(void)
		{
			drive.StartDrive();
			ColorImage *image;
			AxisCamera &camera = AxisCamera::GetInstance();	
//			servo.SetSafetyEnabled(true);
//			servo.StartLiveWindowMode();
			float targAngle = servo.GetAngle();
			while (IsOperatorControl() and IsEnabled())
			{
				servo.SetAngle(targAngle); //make sure not setting to big value
				targAngle += stick3.GetY();
				if (targAngle > servo.GetMaxAngle())
					targAngle = servo.GetMaxAngle();
				if (targAngle < servo.GetMinAngle())
					targAngle = servo.GetMinAngle();
				
				if (stick3.GetRawButton(4))
					servo.Set(0);
				if (stick3.GetRawButton(5))
					servo.Set(1);
				
				if (stick3.GetRawButton(2))
				{
					printf("Doing image stuff\n");
					if (not camera.StatusIsFatal())
					{
						camera.GetImage(image);	
						if (image)
						{
							if (image->GetHeight() == 0 or image->GetWidth() == 0)
								printf("SMALL INVALID IMAGE");
							else
								image->Write("/test1.png");
							printf("yes\n");
		//					MonoImage *redPln = image->GetRedPlane();
		//					redPln->Write("/pics/processed/processed1.bmp");
						}
						else
							printf("INVALID IMAGE");
					}
					else
						printf("INVALID CAMERA");
				}
				DSLog(1, "SAngle %f", servo.GetAngle());
				DSLog(2, "SGet %f", servo.Get());
				DSLog(3, "S Targ %f", targAngle);
				printf("Test");
				Wait(0.005); // wait for a motor update time
			}
			drive.StopDrive();
		}

		/**
		 * Runs during test mode
		 */
		void Test()
		{

		}
};

START_ROBOT_CLASS(Chassis)
;

