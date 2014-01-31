//Last edited by Vadim Korolik
//on 07/17/2013
#ifndef __TKOCLIMBER_H
#define __TKOCLIMBER_H

#define disableIfOutOfRange() if(not armBottom.Get() or not armTop.Get()){winch1.Disable();winch2.Disable();}

#define gg ;

#include "../Definitions.h"
#include "../component/TKORelay.h"
#include "../log/TKOLogger.h"
#include <Task.h>
#include <ctime>

//ONE SETPOINT = 6.28"

class TKOClimber
{

	private:
		Timer timer1;
		Timer time2;
		Timer loopTime;
		DriverStation *ds;
		Joystick _stick3;

	public:
		Solenoid sDumperR, sDumperE, sClipsR, sClipsE, sArmR, sArmE;
		Compressor comp;
		TKORelay rsRatchet;
		CANJaguar winch1, winch2;
		Encoder winchEncoder;
		DigitalInput hookLeft, hookRight, clipLeft, clipRight, armTop, armBottom, ratchet;
		PIDController winch1PID, winch2PID;
		//double setpoint;
		double SETPOINT_TOP;
		double SETPOINT_BOTTOM;
		double SETPOINT_LAST;
		double SETPOINT_RATCHET_RETRACT;
		double SETPOINT_CENTER;
		double climbStart, climbEnd, climbTime;
		double oldSetpoint;
		double setpoint;
		int climbCount;
		double deltaSetpoint;
		//double SETPOINT_CENTER;
		bool ranCalibration;
		bool abortTrig;
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
		
//		static void TaskRunner(TKOClimber *instance);
//		void checkAbort();

		void MoveWinchWithStick();

		void winchStop();

		void testMoveWinch();

		bool LevelOneClimb();
		bool LevelTwoOrMoreClimb();
		///Destructor for the TKOClimber class
		/*!
		 */
		~TKOClimber();
	
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

		void winchMove(double SP);
		
		void winchMoveSlow(double SP, double factor);

		void winchMove(double SP, double deltaSetPoint);

		///The test method for TKOClimber class
		/*!
		 Tests pneumatics and winch positions.
		 */
		void Test();
		
		void ohGod();

		///The dumper pneumatic extend method for TKOClimber class
		/*!
		 Extends the top dumper pneumatic, dumping the frisbees into the top goal
		 */
		void calibrateWinch();

		void Dump();

		void RetractDump();

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
