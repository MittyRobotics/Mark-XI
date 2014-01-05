/*!!!!!!!
 * TO USE:
 * SEE HEADER FILE COMMENTS
 */
//Last edited by Vadim Korolik
//on 01/04/2014
#include "TKOLogger.h"

TKOLogger* TKOLogger::m_Instance = NULL;
/*!!!!!!!
 * TO USE:
 * SEE HEADER FILE COMMENTS
 */

TKOLogger::TKOLogger()
{
	printf("Constructing logger\n");
	logTask = new Task("Logging", (FUNCPTR) LogRunner);
	logTask->SetPriority(254);
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
	messageBuffer->clear();
}
void TKOLogger::Start()
{
	logFile.open("logT.txt", ios::app);
	logTask->Start();
	if (logFile.is_open())
		printf("Logfile OPEN!!!!\n");
	else
		printf("FILE CLOSED!!!!\n");
	printf("Logger started\n");
}
void TKOLogger::Stop()
{
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
		if(messageBuffer->size() > 0)
		{
			logFile << messageBuffer->back();
			logFile << "\n";
			messageBuffer->pop_back();
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
	if (messageBuffer->size() >= messageBuffer->max_size())
	{
		printf("CRITICAL LOGGER BUFFER OVERFLOW \n");
		logFile << "CRITICAL LOGGER BUFFER OVERFLOW \n";
		printf("CURRENT BUFFER SIZE: %i", messageBuffer->size());
		printf("\n");
		printf("MAX BUFFER SIZE: %i", messageBuffer->max_size());
		printf("\n");
		return;
	}
	std::ostringstream newMess;
	newMess << "Time:   " << GetTime() << "          Message: " << message;
	messageBuffer->push_back(newMess.str());
//	printf("Buffer size: %i\n", messageBuffer->size());
}

void TKOLogger::addCMessage(string message, float val)
{
	ostringstream temp;
	temp << "Time:   " << GetTime() << "        " << message << ":   " << val;
	addMessage(temp.str());
}
