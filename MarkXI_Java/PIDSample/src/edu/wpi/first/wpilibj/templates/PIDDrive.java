/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
package edu.wpi.first.wpilibj.templates;



import edu.wpi.first.wpilibj.SimpleRobot;
import edu.wpi.first.wpilibj.Encoder;
import edu.wpi.first.wpilibj.SpeedController;
import edu.wpi.first.wpilibj.PIDController;
import edu.wpi.first.wpilibj.Jaguar; //If you use Victors, import them instead
import edu.wpi.first.wpilibj.Timer; //Make sure to use this version, not the java.util version of timer.

/**
 * This program initializes a PID Controller for the wheels and drives
 * 3 feet using the encoders.  PID works by changing the output of the 
 * motors by PID constants, based on how far away you are from the target.
 * PID values need to be tuned for your robot.
 * 
 * @author: Fredric Silberberg
 */

public class PIDDrive extends SimpleRobot {

    
//    This class is required to invert the direction of the motor.
//    It negates the speed, so that the robot will drive forward.
     
    class MotorDrive extends Jaguar {

        public MotorDrive(int port) {
            super(port);
        }

        //Calls the super set method and gives it the negated speed.
        public void set(double speed) {
            super.set(-speed);
        }
    }
    //Initializes the motors.
    private final SpeedController left = new MotorDrive(2);
    private final SpeedController right = new Jaguar(1);
    
    //Initializes the Encoders.
    private final Encoder leftEncoder = new Encoder(1, 2);
    private final Encoder rightEncoder = new Encoder(4, 3);
    
    //Proportional, Integral, and Dervative constants.
    //These values will need to be tuned for your robot.
    private final double Kp = 0.3;
    private final double Ki = 0.0;
    private final double Kd = 0.0;
    
    //This must be fully initialized in the constructor, after the settings
    //for the encoders have been done.
    private final PIDController leftPID;
    private final PIDController rightPID;

    public PIDDrive() {
        //Sets the distance per pulse in inches.
        leftEncoder.setDistancePerPulse(.000623);
        rightEncoder.setDistancePerPulse(.000623);
        
        //Starts the encoders.
        leftEncoder.start();
        rightEncoder.start();
        
        //Sets the encoders to use distance for PID.
        //If this is not done, the robot may not go anywhere.
        //It is also possible to use rate, by changing kDistance to kRate.
        leftEncoder.setPIDSourceParameter(Encoder.PIDSourceParameter.kDistance);
        rightEncoder.setPIDSourceParameter(Encoder.PIDSourceParameter.kDistance);
        
        //Initializes the PID Controllers
        leftPID = new PIDController(Kp, Ki, Kd, leftEncoder, left);
        rightPID = new PIDController(Kp, Ki, Kd, rightEncoder, right);
        
        //Enables the PID Controllers.
        leftPID.enable();
        rightPID.enable();
        
        //Sets the input range of the PID Controller.
        //These will change, and you should change them based on how far
        //your robot will be driving.
        //For this example, we set them at 100 inches.
        leftPID.setInputRange(0, 100);
        rightPID.setInputRange(0, 100);
    }

    /**
     * This function is called once each time the robot enters operator control.
     * Teleop commands are put in here
     */
    public void operatorControl() {
        //Sets the left and the right motors to 
        //drive forward 60 inches, or 5 feet
        leftPID.setSetpoint(60);
        rightPID.setSetpoint(60);
        
        //This will wait 10 seconds before the end of operator control
        //is reached, so that the robot has time to drive the full 5 feet.
        //You could have other tasks running here as well.
        Timer.delay(10);
    }
}
