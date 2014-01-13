//Last edited by Vadim Korolik
//on 11/27/2013
#ifndef __TKODRIVE_H
#define __TKODRIVE_H

#include "../Definitions.h"
#include "../log/TKOLogger.h"
#include "TKODriveBase.h"

class TKODrive: public TKODriveBase
{
public:
	static TKODrive* inst();
private:
	TKODrive();
	~TKODrive();
	static TKODrive* m_Instance;
	static void DriveRunner();
};
#endif
