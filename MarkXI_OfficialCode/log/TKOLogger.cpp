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
	AddToSingletonList();
	
	printf("Initialized logger\n");
}
TKOLogger::~TKOLogger()
{
	delete logTask;
	m_Instance = NULL;
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
	//before actually stopping or closing file, make sure that buffer is emtied and flushed
	while (messBuffer.size() > 0)
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
		if(messBuffer.size() > 0)
		{
			logFile << messBuffer.front();
			logFile << "\n";
			messBuffer.pop();
		}
	}
	if (logFile.bad() or not logFile.is_open())
	{
		printf("LOG FILE BAD OR CLOSED WHILE WRITING\n");
		Stop();
	}

}
void TKOLogger::addToBuf(string message)
{
	std::ostringstream newMess;
	newMess << "Time:   " << GetTime() << "          Message: " << message;
	messBuffer.push(newMess.str());
}

void TKOLogger::addCMessage(string message, float val)
{
	ostringstream temp;
	temp << "Time:   " << GetTime() << "        " << message << ":   " << val;
	addToBuf(temp.str());
}

void TKOLogger::addMessage(const char *format, ...)
{
	char s[256];
	va_list args;
	va_start(args, format);
	vsprintf(s, format, args);
	va_end(args);
	string temp = s;
	{
		Synchronized sem(_printSem);
		addToBuf(temp);
	}
}
