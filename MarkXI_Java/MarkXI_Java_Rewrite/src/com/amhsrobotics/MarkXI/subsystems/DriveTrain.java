/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.amhsrobotics.MarkXI.subsystems;

import com.amhsrobotics.MarkXI.main.OI;
import com.amhsrobotics.MarkXI.main.Definitions;
import edu.wpi.first.wpilibj.CANJaguar;
import edu.wpi.first.wpilibj.DoubleSolenoid;
import edu.wpi.first.wpilibj.can.CANTimeoutException;
import edu.wpi.first.wpilibj.command.Subsystem;

/**
 *
 * @author Vadim
 */
public class DriveTrain extends Subsystem {

    public CANJaguar drive1, drive2, drive3, drive4;
    public DoubleSolenoid shifterDoubleSolenoid = new DoubleSolenoid(Definitions.DRIVE_SHIFTER_SOLENOID_A, Definitions.DRIVE_SHIFTER_SOLENOID_B);

    private long driveLogCounter = 0;
    public double maxDrive1RPM = 0., maxDrive3RPM = 0.;

    public DriveTrain() {
        //Jaguar special init
        try {
            drive1 = new CANJaguar(Definitions.DRIVE_L1_ID, CANJaguar.ControlMode.kPercentVbus);
            drive2 = new CANJaguar(Definitions.DRIVE_L2_ID, CANJaguar.ControlMode.kPercentVbus);
            drive3 = new CANJaguar(Definitions.DRIVE_R1_ID, CANJaguar.ControlMode.kPercentVbus);
            drive4 = new CANJaguar(Definitions.DRIVE_R2_ID, CANJaguar.ControlMode.kPercentVbus);
            drive1.setSafetyEnabled(false);
            drive2.setSafetyEnabled(false);
            drive3.setSafetyEnabled(false);
            drive4.setSafetyEnabled(false);
            drive1.configNeutralMode(CANJaguar.NeutralMode.kCoast);
            drive2.configNeutralMode(CANJaguar.NeutralMode.kCoast);
            drive3.configNeutralMode(CANJaguar.NeutralMode.kCoast);
            drive4.configNeutralMode(CANJaguar.NeutralMode.kCoast);
            drive1.setVoltageRampRate(0.0);
            drive2.setVoltageRampRate(0.0);
            drive3.setVoltageRampRate(0.0);
            drive4.setVoltageRampRate(0.0);
            drive1.configFaultTime(0.1);
            drive2.configFaultTime(0.1);
            drive3.configFaultTime(0.1);
            drive4.configFaultTime(0.1);
            drive1.setExpiration(5.0);
            drive2.setExpiration(5.0);
            drive3.setExpiration(5.0);
            drive4.setExpiration(5.0);            
            drive1.setSpeedReference(CANJaguar.SpeedReference.kQuadEncoder);
            drive3.setSpeedReference(CANJaguar.SpeedReference.kQuadEncoder);
            drive1.configEncoderCodesPerRev(250);
            drive3.configEncoderCodesPerRev(250);
            drive1.enableControl();
            drive3.enableControl();
        } catch (CANTimeoutException ex) {
            System.out.println(ex.getMessage());
        }
    }

    // Put methods for controlling this subsystem
    // here. Call these from Commands.
    public void initDefaultCommand() {
        // Set the default command for a subsystem here.
        //setDefaultCommand(new MySpecialCommand());
    }

    public void logData() {

    }

    public void manualShift(boolean direction)//false is shift backwards, true is shift forwards (up)
    {
        if (direction) {
            shifterDoubleSolenoid.set(DoubleSolenoid.Value.kForward);
            System.out.println("Manually shifted forwards (high gear)\n");
        } else {
            shifterDoubleSolenoid.set(DoubleSolenoid.Value.kReverse);
            System.out.println("Manually shifted backwards (low gear)\n");
        }
    }

    public void autoShift() {
    }

    public boolean verifyJaguars() {
        return false;
    }
}
