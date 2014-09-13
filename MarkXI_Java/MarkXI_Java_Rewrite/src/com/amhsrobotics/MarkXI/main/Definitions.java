package com.amhsrobotics.MarkXI.main;

/**
 * The Definitions is a mapping from the ports sensors and actuators are wired into
 to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
public class Definitions
{
    // For example to map the left and right motors, you could define the
    // following variables to use with your drivetrain subsystem.
    // public static final int leftMotor = 1;
    // public static final int rightMotor = 2;

    // If you are using multiple modules, make sure to define both the port
    // number and the module. For example you with a rangefinder:
    // public static final int rangefinderPort = 1;
    // public static final int rangefinderModule = 1;
    //Jaguar ports
    public static final int DRIVE_L1_ID = 1;
    public static final int DRIVE_L2_ID = 4;
    public static final int DRIVE_R1_ID = 2;
    public static final int DRIVE_R2_ID = 3;
    public static final int ROLLER_1_JAGUAR_ID = 5;
    public static final int ROLLER_2_JAGUAR_ID = 6;
    public static final int ARM_JAGUAR_ID = 7;
    public static final double DRIVE_SHIFT_RPM = 165.;

    //Joystick ports
    public static final int STICK_1_PORT = 1;
    public static final int STICK_2_PORT = 2;
    public static final int STICK_3_PORT = 3;
    public static final int STICK_4_PORT = 4;

//Arm constants
    public static final double ARM_MINIMUM_POSITION = 0.12;
    public static final double ARM_MAXIMUM_POSITION = -0.18;
    public static final double ARM_SPEED_MULTIPLIER = -.9;
    public static final double ARM_FIRING_LEFT_BOUND = -0.03;//-0.015; // -0.015991   
    public static final double ARM_FIRING_RIGHT_BOUND = 0.;// -0.025;
    public static final double ARM_CURRENT_THRESHOLD = 50.;
    public static final double ARM_CURRENT_TIMEOUT = 5.;
    public static final double ARM_TARGET_RAMP_INCREMENT = 0.002;
    public static final double ARM_MANUAL_DRIVE_INCREMENT = 0.0025;
    public static final double ARM_MID_POSITION = -0.014;//0.0025;
// Spinner constants
    public static final double RADIUS_WHEELS = 4;
    public static final double TAN_SPINNER_THETA = Math.tan(57.5);
    public static final double SPINNER_DIV = 4250.0;
    public static final double HALF_G = .5 * 9.8 * 100. / 2.54;
    public static final double MID_FENDER = 0.425;
    public static final double TOP_FENDER = 0.585;

//PID constants
    public static final double SPINNER_kP = 0.2; //working P: 0.20, working I: .0034
    public static final double SPINNER_kI = 0.004;
    public static final double SPINNER_kD = 0.0;
    public static final double DRIVE_kP = -65.0; //working P: 0.20, working I: .0034
    public static final double DRIVE_kI = 0.046;
    public static final double DRIVE_kD = 0.0;
    public static final double WINCH_kP = .0075;
    public static final double WINCH_kI = 0.000001;
    public static final double WINCH_kD = 0.0;
    public static final double REVS_PER_METER = 2.51;
    public static final int ENCODER_REVS = 250;
    public static final boolean JAG_SAFETY = true;

//Drive constants
    public static final double kMAX_DRIVE_RPM = 700;
    public static final double deadzone = 0.1;
    public static final int kBURNOUT = 500;
    public static final int kBURNOUT_CYCLES = 100;
    public static final double DRIVE_POSITION_INCREMENT = 100.;

//Miscelaneous
    public static final int GYRO_PORT = 1;
    public static final double LOOPTIME = .01;
    public static final double ULTRASONIC_CONVERSION_TO_FEET = (0.009765625 * 12.);

// Pneumatics constants
//PN stands for pneumatics, R stands for relay, S stands for solenoid
    public static final int COMPRESSOR_ID = 1; //Compressor ID
    public static final int PRESSURE_SWITCH_PORT = 3;

    public static final int PISTON_RETRACT_SOLENOID_A = 6;
    public static final int PISTON_RETRACT_SOLENOID_B = 3;
    public static final int LATCH_RETRACT_SOLENOID_A = 2;
    public static final int LATCH_RETRACT_SOLENOID_B = 5;
    public static final int DRIVE_SHIFTER_SOLENOID_A = 7;
    public static final int DRIVE_SHIFTER_SOLENOID_B = 4;

    public static final int PISTON_SWITCH_RETRACT_CHANNEL = 7;
    public static final int PISTON_SWITCH_EXTEND_CHANNEL = 6;
    public static final int LATCH_PISTON_LOCK_SWITCH_CHANNEL = 1;
    public static final int IS_COCKED_SWITCH_CHANNEL = 2;
    public static final int BALL_LIMIT_SWITCH = 10;
    public static final int ARM_OPTICAL_SWITCH = 11;
    public static final int ULTRASONIC_PORT = 7;

//Autonomous Constants
    public static final double DRIVE_CURRENT_CUTOFF = 20.0;
    public static final double RAMP_RATE = 0.005;
    public static final double RAMP_RATE_2 = 0.0005;

}
