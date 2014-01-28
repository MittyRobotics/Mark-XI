#include "TKOShifter.h"

class TKOShifter
{

public:
	TKOShifter::TKOShifter():
		s1(1), s2(2)
	{
		a_shift = true;
		MAX_RPM_1 = 169.85;
		MIN_RPM_1 = 139.85;
	}

	TKOShifter::~TKOShifter() {
		DELETE(s1);
		DELETE(s2);
		DELETE(a_shift);
		DELETE(MAX_RPM_1);
		DELETE(MIN_RPM_1);
	}
	
	void TKOShifter::AutoShift() { 
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
	
	void TKOShifter::shiftToHighGear() {
		s1.Set(true);
		s2.Set(true);
	}
	
	void TKOShifter::shiftToLowGear() {
		s1.Set(false);
		s2.Set(false);
	}
	
	void TKOShifter::ManualShift() {
			s1.Set(!s1.Get());
			s2.Set(!s2.Get());
	}
	
	void TKOShifter::ShiftStopPos() {
		s1.Set(false);
		s2.Set(false);
	}

	void TKOShifter::ShiftControl() {
		switch (a_shift) {
			case true:
				AutoShift();
				break;
			case false:
				ManualShift();
				break;
		}
	} 
	
};
