//on 02/18/2013
#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

/*! \file Definitions.h
 \brief Defines constants and macros used by the other files in the project.
 */
#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Math.h"
#include "stdlib.h"

/*!
 \def DSClear()
 Clears the Driver Station LCD of all text.
 */
/*!
 \def DSLog(line, msg, ...)
 Formatted printing of msg to the specified line on the Driver Station LCD.
 */
#define DSClear()				DriverStationLCD::GetInstance()->Clear(); \
								DriverStationLCD::GetInstance()->UpdateLCD();
#define DSLog(line, msg, ...)	DriverStationLCD::GetInstance()->Printf(DriverStationLCD::GetInstance()->kUser_Line##line, 1, msg, ##__VA_ARGS__); \
								DriverStationLCD::GetInstance()->UpdateLCD();

#define space printf("\n");
#define GO_AWAY NULL;

extern char message[10240];
extern float data[10240];
extern short indx;
//#define writeM(c) ;
//#define writeMD(c,f) ;

//hue
/*! \mainpage Welcome to TKO 1351's MarkIX's Code Documentation!
 *
 * 	This is the Robot Code for the 2013 Season of Ultimate Ascent
 *
 *  If you haven't already, please visit our <a href="http://www.amhsrobotics.com">website</a>.
 */
//Joystick ports
const int STICK_1_PORT = 1;
const int STICK_2_PORT = 2;
const int STICK_3_PORT = 3;
const int STICK_4_PORT = 4;

//Jaguar ports
const int DRIVE_L1_ID = 2;
const int DRIVE_L2_ID = 3;
const int DRIVE_R1_ID = 4;
const int DRIVE_R2_ID = 5;
const int TURRET_ID = 10;
const int SHOOTER_PORT = 8;
const int WINCH_1_PORT = 6;
const int WINCH_2_PORT = 7;
const int WINCH_ENC_PORT_A = 9;
const int WINCH_ENC_PORT_B = 10;

// Spinner constants
const float RADIUS_WHEELS = 4;
const float TAN_SPINNER_THETA = tan(57.5);
const float SPINNER_DIV = 4250.0;
const float HALF_G = .5 * 9.8 * 100. / 2.54;
const float MID_FENDER = 0.425;
const float TOP_FENDER = 0.585;

//PID constants
const float SPINNER_kP = 0.2; //working P: 0.20, working I: .0034
const float SPINNER_kI = 0.004;
const float SPINNER_kD = 0.0;
const float DRIVE_kP = 0.2; //working P: 0.20, working I: .0034
const float DRIVE_kI = 0.0015;
const float DRIVE_kD = 0.0;
const float WINCH_kP = .0075;
const float WINCH_kI = 0.000001;
const float WINCH_kD = 0.0;
const float REVS_PER_METER = 2.51;
const int ENCODER_REVS = 250;
const bool JAG_SAFETY = true;
const CANJaguar::SpeedReference JAG_SPEEDREF = CANJaguar::kSpeedRef_QuadEncoder;
const CANJaguar::PositionReference JAG_POSREF = CANJaguar::kPosRef_QuadEncoder;

//Drive constants
const float kMAX_DRIVE_RPM = 700;
const float deadzone = 0.1; 
const int kBURNOUT = 500;
const int kBURNOUT_CYCLES = 100;

//Miscelaneous
const int GYRO_PORT = 1;

// Pneumatics constants
//PN stands for pneumatics, R stands for relay, S stands for solenoid
const int COMPRESSOR_ID = 1; //Compressor ID
const int PRESSURE_SWITCH_PORT = 1;

//const int PN_R1_ID = 4; //Loader Wrist
//const int PN_R2_ID = 3; //Loader Lift
//const int PN_R3_ID = 2; //Loader Ratchet
//
//const int PN_S1R_ID = 8; //Top Dumper Retract
//const int PN_S1E_ID = 7; //Top Dumper Extend
//const int PN_S2R_ID = 6; //Shooter Retract
//const int PN_S2E_ID = 5; //Shooter Extend
//const int PN_S3R_ID = 4; //Clips Retract
//const int PN_S3E_ID = 3; //Clips Extend
//const int PN_S4R_ID = 2; //Arm Retract
//const int PN_S4E_ID = 1; //Arm Extend
const int PN_R1_ID = 4; //Loader Wrist
const int PN_R2_ID = 3; //Loader Lift
const int PN_R3_ID = 2; //Loader Ratchet

const int PN_S1R_ID = 7; //Top Dumper Retract
const int PN_S1E_ID = 8; //Top Dumper Extend
const int PN_S3R_ID = 4; //Clips Retract
const int PN_S3E_ID = 3; //Clips Extend
const int PN_S4R_ID = 2; //Arm Retract
const int PN_S4E_ID = 1; //Arm Extend

// Camera Constants
const int BORDER_SIZE = 3;
const int CAMERA_SERVO_PORT = 1;
const int CAMERA_PWM_INCREMENT = 1;	

const float PI = 3.14159265;
const float DEGREES_PER_RADIAN = 180 / PI;

//WINCH CONSTANTS
const double SETPOINT_BEGINNING = 1000.;
const double SETPOINT_ARM_BACK = 2000;
const double SETPOINT_ARM_BACK_LVL2 = 1400; //USED TO BE 800
const double SETPOINT_CLIP_BACK = 2200;
const double TOLERANCE = 0.;
const double MAXSPEED = .3;
const double MANSPEED = 1.0;
const double LOOPTIME = .01;
const double TIME_BW_SP = 3;
const double STICK_DEADZONE = .1;
const double LIFT_INCREMENT = .5; //POTENTIALLY CHANGE TO .0001
const double LIFT_INCREMENT_RATCHET = LIFT_INCREMENT / 20;
const double STRNOCLIPMIN = -.4;
const double STRYESCLIPMIN = -1.0;
const double STRMAX = 1.0;

///state machine constants
const int INITIAL_STATE = 2;
const int OPERATOR_CONTROL = 1;
const int ROBOT_PULLED_UP = 2;
const int RETRACTING_RATCHET = 3;
const int CHANGE_SETPOINT_MOVE_HOOKS_DOWN = 4;
const int DEPLOY_CLIPS = 5;
const int MOVE_HOOKS_UP = 6;
const int MOVE_ARM_FORWARD = 7;
const int DEPLOYING_RATCHET = 8;
const int MOVE_HOOKS_DOWN = 9;
const int RETRACTING_CLIPS = 10;
const int WE_MADE_IT = 12;
const int OH_SHIT = 13;
const int WTF = 13;
const int PYRAMID_SIZE = 3;
const int TIMEOUT2 = 15;
const int TIMEOUT3 = 1;
const int TIMEOUT4 = 3;
const int TIMEOUT5 = 1;
const int TIMEOUT6 = 10;
const int TIMEOUT7 = 1;
const int TIMEOUT8 = 3;
const double TIMEOUT8DELTA = .5;
const int TIMEOUT9 = 1;
const int TIMEOUT10 = 2;
const int DECIDE_TIMEOUT = 5;

//Autonomous Constants
const float DRIVE_CURRENT_CUTOFF = 20.0;
const float RAMP_RATE = 0.005;
const float RAMP_RATE_2 = 0.0005;

#endif
