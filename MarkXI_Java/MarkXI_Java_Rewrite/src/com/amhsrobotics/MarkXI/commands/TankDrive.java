
package com.amhsrobotics.MarkXI.commands;

import com.amhsrobotics.MarkXI.main.OI;
import com.amhsrobotics.MarkXI.subsystems.DriveTrain;
import edu.wpi.first.wpilibj.can.CANTimeoutException;

/**
 *
 * @author bradmiller
 */
public class TankDrive extends CommandBase {

    public TankDrive() {
        // Use requires() here to declare subsystem dependencies
        requires(driveTrain);
    }
    
    public void tankDrive(double left, double right) {
        try {
            if (OI.stick2.getRawButton(4)) {
                driveTrain.drive1.setVoltageRampRate(0.0);
                driveTrain.drive2.setVoltageRampRate(0.0);
                driveTrain.drive3.setVoltageRampRate(0.0);
                driveTrain.drive4.setVoltageRampRate(0.0);
            } else if (OI.stick2.getRawButton(5)) {
                driveTrain.drive1.setVoltageRampRate(24.0);
                driveTrain.drive2.setVoltageRampRate(24.0);
                driveTrain.drive3.setVoltageRampRate(24.0);
                driveTrain.drive4.setVoltageRampRate(24.0);
            } else if (OI.stick1.getTrigger()) {
                driveTrain.drive1.setX(left * 0.4);
                driveTrain.drive2.setX(left * 0.4);
                driveTrain.drive3.setX(-right * 0.4);
                driveTrain.drive4.setX(-right * 0.4);
            } else if (OI.stick1.getRawButton(2)) {
                driveTrain.drive1.setX(left * 0.2);
                driveTrain.drive2.setX(left * 0.2);
                driveTrain.drive3.setX(-right * 0.2);
                driveTrain.drive4.setX(-right * 0.2);
            } else {
                driveTrain.drive1.setX(left);
                driveTrain.drive2.setX(left);
                driveTrain.drive3.setX(-right);
                driveTrain.drive4.setX(-right);
            }
        } catch (CANTimeoutException ex) {
            System.out.println(ex.getMessage());
        }
    }

    // Called just before this Command runs the first time
    protected void initialize() {
    }

    // Called repeatedly when this Command is scheduled to run
    protected void execute() {
        tankDrive(OI.stick1.getY(), OI.stick2.getY());
    }

    // Make this return true when this Command no longer needs to run execute()
    protected boolean isFinished() {
        return false;
    }

    // Called once after isFinished returns true
    protected void end() {
        //maybe drive1.set(0)?
    }

    // Called when another command which requires one or more of the same
    // subsystems is scheduled to run
    protected void interrupted() {
        end();
    }
}
