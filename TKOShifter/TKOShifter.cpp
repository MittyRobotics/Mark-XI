#include "TKOShifter.h"

class TKOShifter
{

public:
	TKOShifter():
		drive1(DRIVE_L1_ID, CANJaguar::kPercentVbus),
		drive2(DRIVE_L2_ID, CANJaguar::kPercentVbus),
		drive3(DRIVE_R1_ID, CANJaguar::kPercentVbus),
		drive4(DRIVE_R2_ID, CANJaguar::kPercentVbus),
		stick1(STICK_1_PORT), stick2(STICK_2_PORT),
		s1(1), s2(2),
		comp(2)
	{
		drive1.SetSafetyEnabled(false);
		drive2.SetSafetyEnabled(false);
		drive3.SetSafetyEnabled(false);
		drive4.SetSafetyEnabled(false);
		SetSpeedReference();
		a_shift = true;
	}

	
	
	void AutoShift() { 
		// if all drives are faster than max_rpm_1 then shifts to high gear
		if (min(fabs(drive1.GetSpeed()),fabs(drive2.GetSpeed())) > MAX_RPM_1
			&& min(fabs(drive3.GetSpeed()),fabs(drive4.GetSpeed())) > MAX_RPM_1) {
			shiftToHighGear();
		}
		
		// if drives 1 and 2 or drives 3 and 4 are slower than min_rpm_1 then shifts to low gear
		else if(max(fabs(drive1.GetSpeed()),fabs(drive2.GetSpeed())) < MIN_RPM_1
		   || max(fabs(drive3.GetSpeed()),fabs(drive4.GetSpeed())) < MIN_RPM_1) {
			shiftToLowGear();
		}
	}
	
	void shiftToHighGear() {
		s1.Set(true);
		s2.Set(true);
	}
	
	void shiftToLowGear() {
		s1.Set(false);
		s2.Set(false);
	}
	
	void ManualShift() {
		if (stick2.GetRawButton(3)) {
			s1.Set(!s1.Get());
			s2.Set(!s2.Get());
		}
	}
	
	void ShiftStopPos() {
		s1.Set(false);
		s2.Set(false);
	}

	void ShiftControl() {
		switch (a_shift) {
			case true:
				AutoShift();
				break;
			case false:
				ManualShift();
				break;
		}
	}
	
	private:
		bool a_shift; 
};
