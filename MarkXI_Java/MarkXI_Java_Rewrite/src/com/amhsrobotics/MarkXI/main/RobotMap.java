package com.amhsrobotics.MarkXI.main;

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
public class RobotMap {
    // For example to map the left and right motors, you could define the
    // following variables to use with your drivetrain subsystem.
    // public static final int leftMotor = 1;
    // public static final int rightMotor = 2;
    
    // If you are using multiple modules, make sure to define both the port
    // number and the module. For example you with a rangefinder:
    // public static final int rangefinderPort = 1;
    // public static final int rangefinderModule = 1;
    //Jaguar ports
    //Jaguar ports
    public static int DRIVE_L1_ID = 1;
    public static int DRIVE_L2_ID = 4;
    public static int DRIVE_R1_ID = 2;
    public static int DRIVE_R2_ID = 3;
    public static int ROLLER_1_JAGUAR_ID = 5;
    public static int ROLLER_2_JAGUAR_ID = 6;
    public static int ARM_JAGUAR_ID = 7;
}
