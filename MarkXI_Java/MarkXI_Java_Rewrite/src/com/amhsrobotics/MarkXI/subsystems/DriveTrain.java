/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.amhsrobotics.MarkXI.subsystems;

import com.amhsrobotics.MarkXI.main.RobotMap;
import edu.wpi.first.wpilibj.CANJaguar;
import edu.wpi.first.wpilibj.can.CANTimeoutException;
import edu.wpi.first.wpilibj.command.Subsystem;

/**
 *
 * @author Vadim
 */
public class DriveTrain extends Subsystem
{

    CANJaguar drive1;
    CANJaguar drive2;
    CANJaguar drive3;
    CANJaguar drive4;

    public DriveTrain()
    {
        try
        {
            this.drive4 = new CANJaguar(RobotMap.DRIVE_R2_ID, CANJaguar.ControlMode.kPercentVbus);
            this.drive3 = new CANJaguar(RobotMap.DRIVE_R1_ID, CANJaguar.ControlMode.kPercentVbus);
            this.drive2 = new CANJaguar(RobotMap.DRIVE_L2_ID, CANJaguar.ControlMode.kPercentVbus);
            this.drive1 = new CANJaguar(RobotMap.DRIVE_L1_ID, CANJaguar.ControlMode.kPercentVbus);
        } catch (CANTimeoutException ex)
        {
            ex.printStackTrace();
            System.out.println(ex.getMessage());
        }
    }

    // Put methods for controlling this subsystem
    // here. Call these from Commands.
    public void initDefaultCommand()
    {
        // Set the default command for a subsystem here.
        //setDefaultCommand(new MySpecialCommand());
    }
}
