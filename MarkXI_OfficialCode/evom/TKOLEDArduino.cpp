//Last edited by Vadim Korolik
//on 02/06/2014
#include "TKOLEDArduino.h"

TKOLEDArduino* TKOLEDArduino::m_Instance = NULL;

TKOLEDArduino::TKOLEDArduino():
		data1(2, Relay::kBothDirections),
		data2(3, Relay::kBothDirections),
		data3(4, Relay::kBothDirections),
		reset(5, Relay::kBothDirections)
{	
	printf("Initializing drive\n");
	driveTask = new Task("TKOLEDArduino", (FUNCPTR) TaskRunner, 50);

	printf("Finished initializing drive\n");
	AddToSingletonList();
}
TKOLEDArduino* TKOLEDArduino::inst()
{
	if (!m_Instance)
	{
		printf("Drive instance is null\n");
		m_Instance = new TKOLEDArduino;
	}
	return m_Instance;
}
void TKOLEDArduino::TaskRunner()
{
	while (true)
	{
		m_Instance->processData();
	}
}

void TKOLEDArduino::processData()
{
	reset.Set(reset.kForward);
	data1.Set(data1.kForward);
	data2.Set(data2.kForward);
	data3.Set(data3.kForward);
	//^ default values
	Wait(0.1);
}

void TKOLEDArduino::sendData(int mode)
{
	bitset<16> bitData = mode;
	
	printf("Seding data\n");
	if (bitData[0])
		data1.Set(data1.kForward);
	else
		data1.Set(data1.kOff);
	if (bitData[1])
		data2.Set(data1.kForward);
	else
		data2.Set(data1.kOff);
	if (bitData[2])
		data3.Set(data1.kForward);
	else
		data3.Set(data1.kOff);
	reset.Set(reset.kOff);//record data on arduino
	Wait(0.1);
	reset.Set(reset.kForward);
	Wait(2.);
}
void TKOLEDArduino::Start()
{
	if (not driveTask->Verify() or driveTask->IsSuspended())
		driveTask->Start();
}
void TKOLEDArduino::Stop()
{
	if (driveTask->Verify())
		driveTask->Stop();
}

TKOLEDArduino::~TKOLEDArduino()
{
	driveTask->Stop();
	m_Instance = NULL;
}

