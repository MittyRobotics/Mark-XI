#include "WPILib.h"
class TKOShifter {
	public:
	TKOShifter();
	~TKOShifter();
		Solenoid s1, s2;
		
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
};
