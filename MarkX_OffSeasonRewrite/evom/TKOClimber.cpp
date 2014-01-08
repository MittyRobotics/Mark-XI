//Last edited by Vadim Korolik
//on 09/08/2013
#include "TKOClimber.h"
//Constructor for the TKOClimber class

/*
 * SetVoltageRampRate() When in PercentVbus or Voltage output mode, the rate at which the voltage changes can be limited to reduce current spikes.  
 * ConfigFaultTime()
 * Configure how long the Jaguar waits in the case of a fault before resuming operation.
 * Faults include over temerature, over current, and bus under voltage.
 * The default is 3.0 seconds, but can be reduced to as low as 0.5 seconds.
 */

TKOClimber::TKOClimber(int port1, int port2) :
	_stick3(3), sDumperR(PN_S1R_ID), sDumperE(PN_S1E_ID), 
	sClipsR(PN_S3R_ID), sClipsE(PN_S3E_ID), 
	sArmR(PN_S4R_ID), sArmE(PN_S4E_ID), 
	comp(PRESSURE_SWITCH_PORT, COMPRESSOR_ID),
	rsRatchet(PN_R3_ID),
	winch1(port1, CANJaguar::kPercentVbus), winch2(port2, CANJaguar::kPercentVbus),

	winchEncoder(WINCH_ENC_PORT_A, WINCH_ENC_PORT_B),

	hookLeft(7), hookRight(8), clipLeft(5), clipRight(6), armTop(4), armBottom(3), ratchet(2),

	winch1PID(WINCH_kP, WINCH_kI, WINCH_kD, &winchEncoder, &winch1), winch2PID(WINCH_kP, WINCH_kI, WINCH_kD, &winchEncoder, &winch2)
{
	printf("Initializing climber \n");
	winchEncoder.Start();
	ds = DriverStation::GetInstance(); // Pulls driver station information
	comp.Start();
	winch1PID.Enable();
	winch2PID.Enable();
	winch1.ConfigFaultTime(.5);
	winch2.ConfigFaultTime(.5);
	ranCalibration = false;
	abortTrig = false;
	climbCount = 0;
	ClipBack();
	ArmBack();
	RatchetBack();
	printf("Finished Initializing climber \n");
}

void TKOClimber::ArmBack()
{
	sArmE.Set(false);
	sArmR.Set(true);
}

void TKOClimber::ArmForward()
{
	ClipForward();
	sArmR.Set(false);
	sArmE.Set(true);
}

void TKOClimber::ClipBack()
{
	sClipsR.Set(false);
	sClipsE.Set(true);
}

void TKOClimber::ClipForward()
{
	sClipsE.Set(false);
	sClipsR.Set(true);
}

void TKOClimber::RatchetBack()
{
	rsRatchet.SetOn(1);
}

void TKOClimber::RatchetForward()
{
	rsRatchet.SetOn(0);
}

void TKOClimber::RetractDump()
{
	sDumperE.Set(false);
	sDumperR.Set(true);
}

void TKOClimber::Dump()
{
	sDumperR.Set(false);
	sDumperE.Set(true);
}

void TKOClimber::calibrateWinch()
{
	TKOLogger::inst()->addMessage("CALIB: Starting winch calibration");
	winch1PID.Disable();
	winch2PID.Disable();
	RatchetBack();
	while (ds->IsEnabled())
	{
		RatchetBack();
		winch1.Set((-1) * MAXSPEED); //go up
		winch2.Set((-1) * MAXSPEED);
		if (not armTop.Get()) //NOT ARMTOP MEANS THAT THE WINCH IS AT THE LIMIT SWITCH
		{
			winch1.Set(0);
			winch2.Set(0);
			winchEncoder.Reset();
			SETPOINT_TOP = winchEncoder.PIDGet() + TOLERANCE;
			break;
		}
	}
	TKOLogger::inst()->addMessage("CALIB: Done going up, at the top.");
	while (ds->IsEnabled())
	{
		winch1.Set((1) * MAXSPEED); //go down
		winch2.Set((1) * MAXSPEED);
		if (not armBottom.Get())
		{
			winch1.Set(0);
			winch2.Set(0);
			SETPOINT_BOTTOM = winchEncoder.PIDGet() + (ds->GetAnalogIn(1) * 100); ///USED TO BE 10
			break;
		}
	}
	TKOLogger::inst()->addMessage("CALIB: Done going down, at the bottom.");
	winch1.Set((-1) * MAXSPEED); //go up
	winch2.Set((-1) * MAXSPEED);
	Wait(.5);
	TKOLogger::inst()->addMessage("CALIB: At start point.");
	winch1.Set(0);
	winch2.Set(0);
	Wait(.5);
	oldSetpoint = winchEncoder.PIDGet();
	setpoint = winchEncoder.PIDGet();
	SETPOINT_RATCHET_RETRACT = SETPOINT_BOTTOM + 2.0;
	SETPOINT_LAST = SETPOINT_TOP - 2.0;
	SETPOINT_CENTER = (SETPOINT_TOP + SETPOINT_BOTTOM) / 2;
	printf("Top Setpoint: %f", SETPOINT_TOP);
	printf("Bottom Setpoint: %f", SETPOINT_BOTTOM);
	deltaSetpoint = LOOPTIME * (SETPOINT_TOP - SETPOINT_BOTTOM) / TIME_BW_SP;
	ranCalibration = true;
}

void TKOClimber::print()
{
	printf("HookLeft %d \n", hookLeft.Get());
	printf("HookRight %d \n", hookRight.Get());
	printf("Clip left %d \n", clipLeft.Get());
	printf("Clip Right %d\n", clipRight.Get());
	printf("Arm Top %d \n", armTop.Get());
	printf("Arm Bottom %d\n", armBottom.Get());
	printf("Ratchet %d\n", ratchet.Get());
	printf("\n");
}

void TKOClimber::MoveWinchWithStick()
{
	DSClear();
	winch1PID.Enable();
	winch2PID.Enable();

	if (_stick3.GetRawButton(4))
		TKOClimber::ArmBack();
	if (_stick3.GetRawButton(5))
		TKOClimber::ArmForward();
	if (_stick3.GetRawButton(2))
		TKOClimber::ClipBack();
	if (_stick3.GetRawButton(3))
		TKOClimber::ClipForward();
	if (_stick3.GetRawButton(8))
		TKOClimber::RatchetBack();
	if (_stick3.GetRawButton(9))
		TKOClimber::RatchetForward();
	if (_stick3.GetRawButton(11))
		TKOClimber::calibrateWinch();
	if (_stick3.GetRawButton(10))
	{
		climbStart = ds->GetMatchTime();
		TKOLogger::inst()->addCMessage("LVL 1 Climb started at (s)", climbStart);
		TKOClimber::LevelOneClimb();
		climbEnd = ds->GetMatchTime();
		climbTime = climbEnd - climbStart;
		TKOLogger::inst()->addMessage("Done with level one climb");
		TKOLogger::inst()->addCMessage("LVL 1 Climb took (s)", climbTime);
		printf("LVL 1 Climb took (s) %f\n", climbTime);
	}
	if (_stick3.GetRawButton(7))
	{
		climbStart = ds->GetMatchTime();
		TKOLogger::inst()->addCMessage("LVL 2 Climb started at (s)", climbStart);
		if (TKOClimber::LevelOneClimb())
			TKOClimber::LevelTwoOrMoreClimb();
		climbEnd = ds->GetMatchTime();
		climbTime = climbEnd - climbStart;
		TKOLogger::inst()->addMessage("Done with level two climb");
		TKOLogger::inst()->addCMessage("LVL 2 Climb took (s)", climbTime);
		printf("LVL 2 Climb took (s) %f\n", climbTime);
	}
	if (_stick3.GetRawButton(6))
	{
		climbStart = ds->GetMatchTime();
		TKOLogger::inst()->addCMessage("LVL 3 Climb started at (s)", climbStart);
		if (TKOClimber::LevelOneClimb())
		{
			TKOClimber::LevelTwoOrMoreClimb();
			Wait(.1);
			TKOClimber::LevelTwoOrMoreClimb();
		}
		climbEnd = ds->GetMatchTime();
		climbTime = climbEnd - climbStart;
		TKOLogger::inst()->addMessage("Done with level three climb");
		TKOLogger::inst()->addCMessage("LVL 3 Climb took (s)", climbTime);
		printf("LVL 3 Climb took (s) %f\n", climbTime);
	}
	if (comp.Enabled())
		DSLog(1, "Compressor running.");
	if (comp.GetPressureSwitchValue() == 0)    ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CHECK IF 1 or 0
		DSLog(2, "NOT max pressure.");
	
	if (_stick3.GetThrottle() <= -.75)
		comp.Stop();
	else
		comp.Start();
//	if (not clipLeft.Get())
//		DSLog(1, "Left Clip Engaged");
//	if (not clipRight.Get())
//		DSLog(2, "Right Clip Engaged");
//	if (not hookLeft.Get())
//		DSLog(3, "Left Hook Engaged");
//	if (not hookRight.Get())
//		DSLog(4, "Right Hook Engaged");
	DSLog(5, "SETP: %f", oldSetpoint);
	DSLog(6, "Winch L: %f", winchEncoder.PIDGet());

	if (_stick3.GetY() < -STICK_DEADZONE and (oldSetpoint - (-_stick3.GetY() * deltaSetpoint)) < SETPOINT_BOTTOM)
	{
		oldSetpoint = oldSetpoint - (-_stick3.GetY() * deltaSetpoint);
	}

	if (_stick3.GetY() > STICK_DEADZONE and (oldSetpoint - (-_stick3.GetY() * deltaSetpoint)) > SETPOINT_TOP)
	{
		oldSetpoint = oldSetpoint - (-_stick3.GetY() * deltaSetpoint);
	}

}

//void TKOClimber::TaskRunner(TKOClimber *instance)
//{
//	instance->checkAbort();
//	////////////////MAY NEED WAIT OF PAUSE OF SOME SORT
//	Wait(0.05);
//}

//void TKOClimber::checkAbort()
//{
//	if (_stick3.GetRawButton(2) and _stick3.GetRawButton(4))
//	{
//		abortTrig = true;
//	}
//	if (abortTrig == true)
//	{
//		RatchetForward();
//		winch1PID.Disable();
//		winch2PID.Disable();
//		winchStop();
//		printf("Testing the abort, shutting down winches. \n");
//	}
//}

void TKOClimber::winchStop()
{
	winch1.Set(0);
	winch2.Set(0);
	winch1.StopMotor();
	winch2.StopMotor();
	winch1PID.Disable();
	winch2PID.Disable();
	if (winch1.GetOutputVoltage() > 0 or winch2.GetOutputVoltage() > 0)
		printf("WINCH NOT ACTUALLY STOPPED \n");
}

void TKOClimber::Test() //pneumatics test
{
}

void TKOClimber::winchMove(double SP) //
{
	setpoint = SP;
	printf("-----------------IN WINCH MOVE -----------------------------, %f \n", SP);
	loopTime.Start();
	double deltaSetPoint = LOOPTIME * (SETPOINT_TOP - SETPOINT_BOTTOM) / TIME_BW_SP;
	bool alreadyRan = false;
	winch1PID.Enable();
	winch2PID.Enable();
	printf("Encoder Location: %f \n", winchEncoder.PIDGet());
	if (SP < winch1PID.GetSetpoint())
	{
		RatchetBack();
	}
	if ((int) oldSetpoint == (int) SP)
	{
		return;
	}
	if (oldSetpoint < SP) //MOVING DOWN
	{
		printf("CURRENT POSITION IS: %f            ", winchEncoder.PIDGet());
		printf("SETPOINT IS: %f \n", oldSetpoint);
		oldSetpoint = oldSetpoint - deltaSetPoint;
	}

	if (oldSetpoint > SP and alreadyRan == false)
	{
		printf("CURRENT POSITION IS: %f            ", winchEncoder.PIDGet());
		printf("SETPOINT IS: %f \n", oldSetpoint);
		oldSetpoint = oldSetpoint + deltaSetPoint;
	}
	return;
}

void TKOClimber::winchMoveSlow(double SP, double factor) //
{
	setpoint = SP;
	printf("-----------------IN WINCH MOVE -----------------------------, %f \n", SP);
	loopTime.Start();
	double deltaSetPoint = LOOPTIME * (SETPOINT_TOP - SETPOINT_BOTTOM) / TIME_BW_SP;
	double delta = deltaSetPoint / factor;
	bool alreadyRan = false;
	winch1PID.Enable();
	winch2PID.Enable();
	printf("Encoder Location: %f \n", winchEncoder.PIDGet());
	if (SP < winch1PID.GetSetpoint())
	{
		RatchetBack();
	}
	if ((int) oldSetpoint == (int) SP)
	{
		return;
	}

	if (oldSetpoint < SP) //MOVING DOWN
	{
		printf("CURRENT POSITION IS: %f            ", winchEncoder.PIDGet());
		printf("SETPOINT IS: %f \n", oldSetpoint);
		oldSetpoint = oldSetpoint - delta;
	}

	if (oldSetpoint > SP and alreadyRan == false)
	{
		printf("CURRENT POSITION IS: %f            ", winchEncoder.PIDGet());
		printf("SETPOINT IS: %f \n", oldSetpoint);
		oldSetpoint = oldSetpoint + delta;
	}
	return;
}

void TKOClimber::testMoveWinch()
{
	setpoint = SETPOINT_CENTER;
	while (setpoint > oldSetpoint + 10 and ds->IsEnabled()) //moving down
	{ //while where you want it to go is below than its ramping setpoint
		printf("@@@@@@@@@@@@@@@@@@  NOW IN TESTMOVEWINCH  @@@@@@@@@@@@@@@@@@@@@@@ \n");
		time2.Reset();
		winchMove(SETPOINT_CENTER); //sets setpoint to argument, increments oldsetpoint
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
		if (oldSetpoint > setpoint + 3)
			break;
	}

	while (setpoint < oldSetpoint + 10 and ds->IsEnabled()) //moving up
	{ //while where you want it to go is below than its ramping setpoint
		printf("@@@@@@@@@@@@@@@@@@  NOW IN TESTMOVEWINCH  @@@@@@@@@@@@@@@@@@@@@@@ \n");
		time2.Reset();
		winchMove(SETPOINT_CENTER); //sets setpoint to argument, increments oldsetpoint
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
		if (oldSetpoint < setpoint + 3)
			break;
	}

}

void TKOClimber::ohGod()
{
	printf("UH OH");
	while (ds->IsEnabled())
	{
		winch1PID.Disable();
		winch2PID.Disable();
		RatchetForward();
	}
}

bool TKOClimber::LevelOneClimb()
{
	TKOLogger::inst()->addMessage("LVL 1 Starting climb");
	climbCount = 0;
	if (ranCalibration == false)
	{
		TKOLogger::inst()->addMessage("LVL 1 Climb called without calibration...");
		calibrateWinch();
		MoveWinchWithStick();
		return false;
	}
	winch1PID.Enable();
	winch2PID.Enable();

	ClipForward();

	RatchetBack();

	Wait(.5);

	setpoint = SETPOINT_BEGINNING;
	while (setpoint > oldSetpoint + 10 and ds->IsEnabled())
	{
		time2.Reset();
		winchMoveSlow(SETPOINT_BEGINNING, 1.); //sets setpoint to argument, increments oldsetpoint
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}
	setpoint = SETPOINT_BEGINNING;
	while (setpoint < oldSetpoint - 10 and ds->IsEnabled()) ///MAYBE THIS > SIGN NEEDS TO BE <
	{ //while where you want it to go is below than its ramping setpoint
		printf("@@@@@@@@@@@@@@@@@@@@MOVING WINCH TO SETPOINT \n");
		//disableIfOutOfRange()
		time2.Reset();
		winchMoveSlow(SETPOINT_BEGINNING, 1.); //sets setpoint to argument, increments oldsetpoint
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}
	TKOLogger::inst()->addMessage("LVL 1: Winch should now be at climb start position.");
	//--------------WINCH SHOULD NOW BE AT STARTING POSITION-------------
	ArmForward();
	Wait(3.);
	RatchetForward();

	Wait(.1);
	TKOLogger::inst()->addMessage("LVL 1: Arm moved forward towards pyramid.");

	setpoint = SETPOINT_ARM_BACK;
	while (setpoint > oldSetpoint + 10 and ds->IsEnabled())
	{ //while where you want it to go is below than its ramping setpoint
		printf("TESTING CURRENT ENC POS: %f\n", winchEncoder.PIDGet());
		disableIfOutOfRange()
		if (winchEncoder.PIDGet() >= 1600)
		{
			if (hookLeft.Get() or hookRight.Get())
			{
				winch1PID.SetSetpoint(winchEncoder.PIDGet());
				winch2PID.SetSetpoint(winchEncoder.PIDGet());
				RatchetBack();
				TKOLogger::inst()->addMessage(">>>CRITICAL LVL 1: IMPROPERLY ALLIGNED? WHEN STARTING FIRST PULL HOOKS DID NOT ATTACH");
				Wait(0.25);
				TKOLogger::inst()->addMessage(">>>CRITICAL LVL 1: ABORTING LVL 1 CLIMB, RETURNING OUT OF CLIMB FUNCTION.");
				printf("CRITICAL LVL 1: IMPROPERLY ALLIGNED? WHEN STARTING FIRST PULL HOOKS DID NOT ATTACH\n");
				return false;
			}
		}
		time2.Reset();
		winchMoveSlow(SETPOINT_ARM_BACK, 1.); //sets setpoint to argument, increments oldsetpoint
		//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}
	TKOLogger::inst()->addMessage("LVL 1: Finished pulling up to arm back setpoint");
	ArmBack();

	Wait(2.);
	TKOLogger::inst()->addMessage("LVL 1: Arm moved back.");

	setpoint = SETPOINT_CLIP_BACK;
	while (setpoint > oldSetpoint + 10 and ds->IsEnabled())
	{ //while where you want it to go is below than its ramping setpoint
		disableIfOutOfRange()
		time2.Reset();
		winchMoveSlow(SETPOINT_CLIP_BACK, 1.); //sets setpoint to argument, increments oldsetpoint
		//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}
	TKOLogger::inst()->addMessage("LVL 1: Finished pulling up to clip back setpoint");
	ClipBack();
	Wait(.5);
	TKOLogger::inst()->addMessage("LVL 1: Clips moved back");

	setpoint = SETPOINT_BOTTOM + 500;
	while (ds->IsEnabled() and armBottom.Get())
	{ //while where you want it to go is below than its ramping setpoint
		time2.Reset();
		printf("Le going down more than bottom lel\n");
		printf("PID writing: %f\n", winch1PID.Get());
		if (not armBottom.Get())
			break;
		printf("oldSP = %f%s", oldSetpoint, "\n");
		winchMoveSlow(setpoint, 2); //sets setpoint to argument, increments oldsetpoint
		//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}
	TKOLogger::inst()->addMessage("LVL 1: Winch now at bottom limit switch");
	Wait(.25);

	setpoint = SETPOINT_BOTTOM - 300;
	RatchetBack();
	ClipForward();
	Wait(.5);
	TKOLogger::inst()->addMessage("LVL 1: Clip is forward, transferring weight");
	while (setpoint < oldSetpoint - 10 and ds->IsEnabled()) //UP TO SETPOINT_BOTTOM FROM THE LIMIT SWITCH
	{ //while where you want it to go is below than its ramping setpoint
		printf("Le going up back to setpoint bottom lel");
		DSLog(1, "LAST SETPOINT %f", setpoint);
		time2.Reset();
		if (not clipLeft.Get() and not clipRight.Get())
		{
			TKOLogger::inst()->addMessage("LVL 1: CLIPS ENGAGED");
			break;
		}
		winchMoveSlow(setpoint, 10); //sets setpoint to argument, increments oldsetpoint
		//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}

	Wait(1.);

	if (clipLeft.Get() or clipRight.Get())
	{
		DSClear();
		if (not clipLeft.Get())
		{
			DSLog(1, "Left Clip Engaged");
			TKOLogger::inst()->addMessage("LVL 1: Left clip engaged");
		}
		if (not clipRight.Get())
		{
			DSLog(2, "Right Clip Engaged");
			TKOLogger::inst()->addMessage("LVL 1: Right clip engaged");
		}
		if (not hookLeft.Get())
		{
			DSLog(3, "Left Hook Engaged");
			TKOLogger::inst()->addMessage("LVL 1: Left hook engaged");
		}
		if (not hookRight.Get())
		{
			DSLog(4, "Right Hook Engaged");
			TKOLogger::inst()->addMessage("LVL 1: Right hook engaged");
		}
		while ((clipLeft.Get() or clipRight.Get()) and ds->IsEnabled())
		{
			RatchetForward();
			winchStop();
			DSLog(6, "ERROR NO CLIP");
			TKOLogger::inst()->addMessage("LVL 1: CLIPS NOT ENGAGED");
			Wait(5.);
		}
	}

	RatchetBack();

	Wait(.25);

	setpoint = SETPOINT_TOP + 30;
	TKOLogger::inst()->addMessage("LVL 1: Weight transferred, moving hooks back up to top.");

	while (setpoint < oldSetpoint - 1 and ds->IsEnabled())
	{ //while where you want it to go is below than its ramping setpoint
		if (not clipLeft.Get())
			DSLog(1, "Left Clip Engaged");
		if (not clipRight.Get())
			DSLog(2, "Right Clip Engaged");
		if (not hookLeft.Get())
			DSLog(3, "Left Hook Engaged");
		if (not hookRight.Get())
			DSLog(4, "Right Hook Engaged");
		disableIfOutOfRange()
		DSLog(1, "LAST SETPOINT %f", setpoint);
		time2.Reset();
		winchMoveSlow(setpoint, 0.5); //sets setpoint to argument, increments oldsetpoint
		//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}
	DSLog(3, "Done with autoclimb")
	TKOLogger::inst()->addMessage("LVL 1: DONE WITH LVL 1 CLIMB");
	printf("Done with autoclimb");
	climbCount++;
	return true;
}

bool TKOClimber::LevelTwoOrMoreClimb()
{
	winch1PID.Enable();
	winch2PID.Enable();
	ArmForward();
	Wait(2);
	//	if (hookLeft.Get() or hookRight.Get()) //MAYBE REMOVE HOOK CHECK
	//		ohGod();

	RatchetForward();
	
	setpoint = SETPOINT_ARM_BACK_LVL2;
	bool dumped = false;
	while (setpoint > oldSetpoint + 10 and ds->IsEnabled())
	{ //while where you want it to go is below than its ramping setpoint
		if ((climbCount == (int) ds->GetAnalogIn(2) - 1) and winchEncoder.PIDGet() > 500 and not dumped)
		{
			Dump();
			Wait(0.25);
			RetractDump();
			Wait(0.25);
			Dump();
			Wait(0.25);
			RetractDump();
			Wait(0.25);
			dumped = true;
		}
		{
			std::ostringstream oss;
			oss << "LVL2: W1Temp:   " << winch1.GetTemperature();
			TKOLogger::inst()->addMessage(oss.str());
			oss.str("");
			oss << "LVL2: W1Curr:   " << winch1.GetOutputCurrent();
			TKOLogger::inst()->addMessage(oss.str());
			oss.str("");
			oss << "LVL2: W1BUSV:   " << winch1.GetBusVoltage();
			TKOLogger::inst()->addMessage(oss.str());
			oss.str("");
			oss << "LVL2: W1FAULTS:   " << winch1.GetFaults();
			TKOLogger::inst()->addMessage(oss.str());
			TKOLogger::inst()->addMessage("\n");
			TKOLogger::inst()->addMessage("\n");
		}
		time2.Reset();
		winchMoveSlow(SETPOINT_ARM_BACK_LVL2, 1); //sets setpoint to argument, increments oldsetpoint
		//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}
	comp.Stop();

	ArmBack();
	Wait(.5);

	setpoint = SETPOINT_ARM_BACK;
//	int tempCount = 0;
	while (setpoint > oldSetpoint + 10 and ds->IsEnabled())
	{ //while where you want it to go is below than its ramping setpoint
//		if (tempCount % 10 == 0)
		{
			std::ostringstream oss;
			oss << "LVL2: W1Temp:   " << winch1.GetTemperature();
			TKOLogger::inst()->addMessage(oss.str());
			oss.str("");
			oss << "LVL2: W1Curr:   " << winch1.GetOutputCurrent();
			TKOLogger::inst()->addMessage(oss.str());
			oss.str("");
			oss << "LVL2: W1BUSV:   " << winch1.GetBusVoltage();
			TKOLogger::inst()->addMessage(oss.str());
			oss.str("");
			oss << "LVL2: W1FAULTS:   " << winch1.GetFaults();
			TKOLogger::inst()->addMessage(oss.str());
			TKOLogger::inst()->addMessage("\n");
			TKOLogger::inst()->addMessage("\n");
		}
		time2.Reset();
		winchMoveSlow(SETPOINT_ARM_BACK, 0.75); //sets setpoint to argument, increments oldsetpoint
		//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
//		tempCount++;
	}

	ArmBack();

	Wait(.5);

	setpoint = SETPOINT_CLIP_BACK;
	while (setpoint > oldSetpoint + 10 and ds->IsEnabled())
	{ //while where you want it to go is below than its ramping setpoint
		{
			std::ostringstream oss;
			oss << "LVL2: W1Temp:   " << winch1.GetTemperature();
			TKOLogger::inst()->addMessage(oss.str());
			oss.str("");
			oss << "LVL2: W1Curr:   " << winch1.GetOutputCurrent();
			TKOLogger::inst()->addMessage(oss.str());
			oss.str("");
			oss << "LVL2: W1BUSV:   " << winch1.GetBusVoltage();
			TKOLogger::inst()->addMessage(oss.str());
			oss.str("");
			oss << "LVL2: W1FAULTS:   " << winch1.GetFaults();
			TKOLogger::inst()->addMessage(oss.str());
			TKOLogger::inst()->addMessage("\n");
			TKOLogger::inst()->addMessage("\n");
		}
		time2.Reset();
		winchMoveSlow(SETPOINT_CLIP_BACK, 0.75); //sets setpoint to argument, increments oldsetpoint
		//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}

	ClipBack();
	Wait(.5);

	setpoint = SETPOINT_BOTTOM + 500;
	while (ds->IsEnabled() and armBottom.Get())
	{ //while where you want it to go is below than its ramping setpoint
		time2.Reset();
		printf("Le going down more than bottom lel\n");
		if (not armBottom.Get())
			break;
		printf("oldSP = %f%s", oldSetpoint, "\n");
		winchMoveSlow(setpoint, 2); //sets setpoint to argument, increments oldsetpoint
		//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}

	Wait(.25);
	comp.Start();

	setpoint = SETPOINT_BOTTOM - 300;
	RatchetBack();
	ClipForward();

	Wait(.5);

	while (setpoint < oldSetpoint - 10 and ds->IsEnabled()) //UP TO SETPOINT_BOTTOM FROM THE LIMIT SWITCH
	{ //while where you want it to go is below than its ramping setpoint
		printf("Le going up back to setpoint bottom lel");
		DSLog(1, "LAST SETPOINT %f", setpoint);
		time2.Reset();
		if (not clipLeft.Get() and not clipRight.Get())
			break;
		winchMoveSlow(setpoint, 10); //sets setpoint to argument, increments oldsetpoint
		//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
		winch1PID.SetSetpoint(oldSetpoint);
		winch2PID.SetSetpoint(oldSetpoint);
		Wait(LOOPTIME - time2.Get());
	}

	//RatchetForward();
	Wait(.25);

	if (clipLeft.Get() or clipRight.Get())
	{
		DSClear();
		if (not clipLeft.Get())
			DSLog(1, "Left Clip Engaged");
		if (not clipRight.Get())
			DSLog(2, "Right Clip Engaged");
		if (not hookLeft.Get())
			DSLog(3, "Left Hook Engaged");
		if (not hookRight.Get())
			DSLog(4, "Right Hook Engaged");
		while ((clipLeft.Get() or clipRight.Get()) and ds->IsEnabled())
		{
			RatchetForward();
			winchStop();
			DSLog(6, "ERROR NO CLIP");
		}
	}

	RatchetBack();

	Wait(.25);

	if (climbCount <= (ds->GetAnalogIn(2) - 2)) //MAKES IT SO IF WE ARE AT LEVEL THREE, WE DONT BRING THE HOOKS TO THE TOP  ******** CHANGE TO <= WHEN CLIMBING A 3 LVL PYRAMID
	{
		setpoint = SETPOINT_TOP + 30;

		while (setpoint < oldSetpoint - 1 and ds->IsEnabled())
		{ //while where you want it to go is below than its ramping setpoint
			if (not clipLeft.Get())
				DSLog(1, "Left Clip Engaged");
			if (not clipRight.Get())
				DSLog(2, "Right Clip Engaged");
			if (not hookLeft.Get())
				DSLog(3, "Left Hook Engaged");
			if (not hookRight.Get())
				DSLog(4, "Right Hook Engaged");
			disableIfOutOfRange()
			DSLog(1, "LAST SETPOINT %f", setpoint);
			time2.Reset();
			winchMoveSlow(setpoint, 1); //sets setpoint to argument, increments oldsetpoint
			//!!!!!!!!!!!!!!THE SETPOINT SHOULD BE SLIGHTLY BELOW THE BAR LOCATION!!!!!
			winch1PID.SetSetpoint(oldSetpoint);
			winch2PID.SetSetpoint(oldSetpoint);
			Wait(LOOPTIME - time2.Get());
		}
	}
	DSLog(3, "Done with autoclimb")
	printf("Done with autoclimb");
	climbCount++;
	return true;
}

//Destructor for the TKOAutonomous class
TKOClimber::~TKOClimber()
{
}
