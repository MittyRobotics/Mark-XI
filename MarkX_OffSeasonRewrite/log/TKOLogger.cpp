/*!!!!!!!
 * TO USE:
 * SEE HEADER FILE COMMENTS
 */
//Last edited by Vadim Korolik
//on 11/27/2013
#include "TKOLogger.h"
ofstream logFile;
queue<string> messageBuffer;

TKOLogger* TKOLogger::m_Instance = NULL;
/*!!!!!!!
 * TO USE:
 * SEE HEADER FILE COMMENTS
 */

TKOLogger::TKOLogger()
{
	printf("Constructing logger\n");
	logTask = new Task("Logging", (FUNCPTR) LogRunner);
	logFile.open("logT.txt", ios::app);
	printf("Done initializing logger\n");
	if (logFile.is_open())
	{
		printf("Logfile OPEN ON BOOT!!!!\n");
		logFile.close();
	}
	struct stat filestatus;
	stat("logT.txt", &filestatus);
	printf("File: %i%s", (int)filestatus.st_size, " bytes\n");
	addMessage("-------Logger booted---------");
	
	printf("Initialized logger\n");
}
TKOLogger::~TKOLogger()
{
	delete logTask;
}
void TKOLogger::Start()
{
	logFile.open("logT.txt", ios::app);
	_ds = DriverStation::GetInstance();
	logTask->Start();
	if (logFile.is_open())
		printf("Logfile OPEN!!!!\n");
	else
		printf("FILE CLOSED!!!!\n");
	printf("Logger started\n");
}
void TKOLogger::Stop()
{
	while (messageBuffer.size() > 0)
	{
		writeBuffer();
	}
	if (logTask->Verify())
		logTask->Stop();
	if (logFile.is_open())
	{
		logFile.flush();
		logFile.close();
	}
	else
		printf("Logger stopping but log file already CLOSED!!!\n");
	printf("Logger stopped\n");
}
void TKOLogger::LogRunner()
{
	while(true)
	{
		if (!m_Instance)
		{
			printf("Invalid Logger instance\n");
			break;
		}
		m_Instance->writeBuffer();
		Wait(0.025);
	}
}
TKOLogger* TKOLogger::inst()
{
	if (!m_Instance)
		m_Instance = new TKOLogger;
	return m_Instance;
}
void TKOLogger::writeBuffer()
{
	if (logFile.is_open())
	{
		if(messageBuffer.size() > 0)
		{
			logFile << messageBuffer.front();
			logFile << "\n";
			messageBuffer.pop();
		}
	}
	if (logFile.bad() or not logFile.is_open())
	{
		printf("LOG FILE BAD OR CLOSED WHILE WRITING\n");
		Stop();
	}

}
void TKOLogger::addMessage(string message)
{
	std::ostringstream newMess;
	newMess << "Time:   " << GetTime() << "          Message: " << message;
	messageBuffer.push(newMess.str());
//	printf("Buffer size: %i\n", messageBuffer->size());
}

void TKOLogger::addCMessage(string message, float val)
{
	ostringstream temp;
	temp << message << ":   " << val;
	addMessage(temp.str());
}
