/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.amhsrobotics.MarkXI.subsystems;

import com.amhsrobotics.MarkXI.main.OI;
import com.amhsrobotics.MarkXI.main.RobotMap;
import edu.wpi.first.wpilibj.CANJaguar;
import edu.wpi.first.wpilibj.DoubleSolenoid;
import edu.wpi.first.wpilibj.can.CANTimeoutException;
import edu.wpi.first.wpilibj.command.Subsystem;

/**
 *
 * @author Vadim
 */
public class DriveTrain extends Subsystem
{

    private CANJaguar drive1, drive2, drive3, drive4;
    private DoubleSolenoid shifterDoubleSolenoid = new DoubleSolenoid(RobotMap.DRIVE_SHIFTER_SOLENOID_A, RobotMap.DRIVE_SHIFTER_SOLENOID_B);

    private long driveLogCounter = 0;
    public double maxDrive1RPM = 0., maxDrive3RPM = 0.;

    public DriveTrain()
    {
        //Jaguar special init
        try
        {
            drive1 = new CANJaguar(RobotMap.DRIVE_L1_ID, CANJaguar.ControlMode.kPercentVbus);
            drive2 = new CANJaguar(RobotMap.DRIVE_L2_ID, CANJaguar.ControlMode.kPercentVbus);
            drive3 = new CANJaguar(RobotMap.DRIVE_R1_ID, CANJaguar.ControlMode.kPercentVbus);
            drive4 = new CANJaguar(RobotMap.DRIVE_R2_ID, CANJaguar.ControlMode.kPercentVbus);
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
            drive1.setSpeedReference(CANJaguar.SpeedReference.kQuadEncoder); 
            drive3.setSpeedReference(CANJaguar.SpeedReference.kQuadEncoder);
            drive1.configEncoderCodesPerRev(250);
            drive3.configEncoderCodesPerRev(250);
            drive1.enableControl();
            drive3.enableControl();
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

    public void tankDrive() throws CANTimeoutException
    {
        if (OI.stick2.getRawButton(4))
	{
		drive1.setVoltageRampRate(0.0);
		drive2.setVoltageRampRate(0.0);
		drive3.setVoltageRampRate(0.0);
		drive4.setVoltageRampRate(0.0);
	}
	else if (OI.stick2.getRawButton(5))
	{
		drive1.setVoltageRampRate(24.0);
		drive2.setVoltageRampRate(24.0);
		drive3.setVoltageRampRate(24.0);
		drive4.setVoltageRampRate(24.0);
	}
	else if (OI.stick1.getTrigger())
	{
		drive1.setX(OI.stick1.getY() * 0.4);
		drive2.setX(OI.stick1.getY() * 0.4);
		drive3.setX(-OI.stick2.getY() * 0.4);
		drive4.setX(-OI.stick2.getY() * 0.4);
	}
	else if (OI.stick1.getRawButton(2))
	{
		drive1.setX(OI.stick1.getY() * 0.2);
		drive2.setX(OI.stick1.getY() * 0.2);
		drive3.setX(-OI.stick2.getY() * 0.2);
		drive4.setX(-OI.stick2.getY() * 0.2);
	}
	else
	{
		drive1.setX(OI.stick1.getY());
		drive2.setX(OI.stick1.getY());
		drive3.setX(-OI.stick2.getY());
		drive4.setX(-OI.stick2.getY());
	}
    }

    public void logData()
    {

    }

    public void manualShift()
    {

    }

    public void autoShift()
    {

    }

    public boolean verifyJaguars()
    {
        return false;
    }
}
