#include "WPILib.h"
class TKOShifter {
	public:
		CANJaguar drive1, drive2, drive3, drive4;
		Joystick stick1, stick2;
		Solenoid s1, s2;
		TKORelay comp;
		
		void AutoShift();
		void shiftToHighGear();
		void shiftToLowGear();
		void ManualShift();
		void ShiftStopPos();
		void ShiftControl();
	private:
		bool a_shift; // Autonomous Shifting (True means that A Shift is ON)
		const float MAX_RPM_1;
		const float MIN_RPM_1;
}
