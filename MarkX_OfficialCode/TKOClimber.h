//Last edited by Vadim Korolik
//on 02/09/2013
#ifndef __TKOCLIMBER_H
#define __TKOCLIMBER_H

#define armBack() sArmE.Set(false);/*Wait(0.1);*/sArmR.Set(true);/*Wait(0.1);*/
#define armForward() sArmR.Set(false); /*Wait(0.1);*/sArmE.Set(true);/*Wait(0.1);*/
#define ratchetBack() rsRatchet.SetOn(0);/*Wait(0.1);*/
#define ratchetForward() rsRatchet.SetOn(1);/*Wait(0.1);*/
#define clipForward() sClipsR.Set(false); /*Wait(0.3);*/ sClipsE.Set(true); /*Wait(0.3);*/
#define clipBack() sClipsE.Set(false);/*Wait(0.3);*/sClipsR.Set(true);/*Wait(0.3);*/
#define topDumperForward() sDumperR.Set(false);/*Wait(0.1);*/sDumperE.Set(true);/*Wait(0.1);*/
#define gg ;

#include "Definitions.h"
#include "TKORelay.h"
#include "Logger.h"

class TKOClimber
{

	private:
		CANJaguar winch1, winch2;
		DigitalInput hookLeft, hookRight, clipLeft, clipRight, armTop, armBottom, ratchet;
		Timer time;
		Timer time2;
		DriverStation *ds;
		TKORelay rsRatchet;
		Solenoid sDumperR, sDumperE, sClipsR, sClipsE, sArmR, sArmE;
		Joystick _stick1;

		int state; /*!< The state variable for the state machine. 1 is Operator control, 13 is Emergency State */
		float setpointtest;
	public:
	///Constructor for the TKOClimber class
	/*!

    *    - Does the following:
    *        -# Initializes Winch Jaguars
    *        -# Initializes Switches
    *        -# Initializes Pneumatics
    *        -# Initializes DriverStation
    *        -# Sets initial state for State Machine
    *        -# Position mode for winch1
    *        -# Slave mode for winch2
    *        -# Moves back Clips
    *        -# Moves back Arm
    *        -# Moves back Ratchet

        \param int port1 - ID for winch1
        \param int port2 - ID for winch2
	*/
		TKOClimber(int port1, int port2);
    ///Destructor for the TKOClimber class
    /*!
    */
		~TKOClimber();
    ///Decide method for TKOCLimber class
    /*!
        Lets the driver decide whether they are going to continue climbing or not
        There is a certain amount of time they have to decide in, otherwise it
        errors out.

        This will return the original argument if button 4 is pressed
        This will return the original argument plus one if button 6 is pressed

        \param int state - The state you call it from
        \return The state it goes to after this method. Defaults to 13 in 5 seconds

    */
		int Decide(int s);
    ///The print method for TKOClimber class
    /*!
        Prints out all information to netlogger
    */
		void print();
    ///The climbing method for TKOCLimber class
    /*!
        Makes the robot climb up the pyramid via state machine
    */
		void Climb();

    ///The test method for TKOClimber class
    /*!
        Tests pneumatics and winch positions.
    */
		void Test();

    ///The dumper pneumatic extend method for TKOClimber class
    /*!
        Extends the top dumper pneumatic, dumping the frisbees into the top goal
    */
		void Dump();

    ///The arm pneumatic retract method for TKOClimber class
    /*!
        Retracts the arm pneumatic, moving the lift arm back
    */
		void ArmBack();

    ///The arm pneumatic extend method for TKOClimber class
    /*!
        Extends the arm pneumatic, moving the lift arm forward
    */
		void ArmForward();

    ///The clip pneumatic retract method for TKOClimber class
    /*!
        Retracts the clip pneumatic, moving the clips up
    */
		void ClipBack();

    ///The clip pneumatic extend method for TKOClimber class
    /*!
        Extends the clip pneumatic, moving the clips down
    */
		void ClipForward();

    ///The ratchet pneumatic retract method for TKOClimber class
    /*!
        Retracts the ratchet pneumatic, disengaging the safety
    */
		void RatchetBack();

    ///The ratchet pneumatic extend method for TKOClimber class
    /*!
        Extends the ratchet pneumatic, engaging the safety
    */
		void RatchetForward();
};

#endif
