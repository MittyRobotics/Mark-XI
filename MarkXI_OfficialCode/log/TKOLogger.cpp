/*!!!!!!!
 * TO USE:
 * SEE HEADER FILE COMMENTS
 */
//Last edited by Vadim Korolik
//on 01/04/2014
#include "TKOLogger.h"

TKOLogger* TKOLogger::_instance = NULL;
/*!!!!!!!
 * TO USE:
 * SEE HEADER FILE COMMENTS
 */

TKOLogger::TKOLogger()
{
	// This should be the first singleton to be constructed
	printf("Constructing logger\n");
	_logTask = new Task("Logging", (FUNCPTR) LogRunner); // create a new task called Logging which runs LogRunner
	_logTask->SetPriority(254); // use the constants first/wpilib provides?
	_logFile.open("logT.txt", ios::app); // open logT.txt in append mode
	printf("Done initializing logger\n");
	if (_logFile.is_open())
	{
		printf("Logfile OPEN ON BOOT!!!!\n");
		_logFile.close();
	}
	struct stat filestatus;
	stat("logT.txt", &filestatus);
	printf("File: %i%s", (int)filestatus.st_size, " bytes\n");
	addMessage("-------Logger booted---------");
	_printSem = semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);
	_bufSem = semMCreate(SEM_Q_PRIORITY | SEM_DELETE_SAFE | SEM_INVERSION_SAFE);
	AddToSingletonList();

	printf("Initialized logger\n");
}

TKOLogger::~TKOLogger()
{
	// If semaphore is being used by a task when semDelete is called,
	// the task will unblock and return ERROR
	Stop();        // TODO: is this necessary?
	semDelete(_printSem);
	semDelete(_bufSem);
	delete _logTask;
	_instance = NULL;
}

void TKOLogger::Start()
{
	// This should be the first class to be Started after enabling
	_logFile.open("logT.txt", ios::app); // open logT.txt in append mode
	if (_logFile.is_open())
		this->printMessage("Logfile OPEN!!!!\n");
	else
		this->printMessage("FILE CLOSED!!!!\n");
	this->printMessage("Logger started\n");
	_logTask->Start();
}

void TKOLogger::Stop()
{
	// This should be the last class to be Stopped after disabling
	Synchronized sem(_bufSem);
	if (_logTask->Verify()) {
		_logTask->Stop();
	}
	if (_logFile.fail()) {
		this->printMessage("LOG FILE FAILED WHILE WRITING\n"); // TODO: is it okay to take 2 semaphores at the same time?
	} else if (!_logFile.is_open()) {
		this->printMessage("LOG FILE CLOSED WHILE WRITING\n");
	} else {
		while (_messBuffer.size() > 0) {
			_logFile << _messBuffer.front();
			_logFile << "\n";
			_messBuffer.pop();
		}
		_logFile.flush();
		_logFile.close();
	}

	this->printMessage("Logger stopped\n");
}

void TKOLogger::LogRunner()
{
	while (true) {
		if (!_instance) {
			printf("Invalid Logger instance\n");
			break;
		}
		{
			Synchronized sem(_instance->_bufSem);
			if (_instance->_logFile.fail()) {
				_instance->printMessage("LOG FILE FAILED WHILE WRITING\n");
			} else if (!_instance->_logFile.is_open()) {
				_instance->printMessage("LOG FILE CLOSED WHILE WRITING\n");
			} else {
				if (_instance->_messBuffer.size() > 0) {
					_instance->_logFile << _instance->_messBuffer.front();
					_instance->_logFile << "\n";
					_instance->_messBuffer.pop();
				}
			}
		}
		Wait(0.025);
	}
}

TKOLogger* TKOLogger::inst()
{
	if (!_instance)
		_instance = new TKOLogger;
	return _instance;
}

void TKOLogger::addMessage(const char *format, ...) //TODO Figure out why code crashes on cRio
{
	int nBytes;
	char s[_MAX_BUF_LENGTH + 1];        // Allocate extra character for '\0'
	nBytes = sprintf(s, "Time: %f     Message: ", GetTime());
	va_list args;
	va_start(args, format);
	nBytes += vsnprintf(s + nBytes, _MAX_BUF_LENGTH + 1 - nBytes, format, args);
	va_end(args);
	if (nBytes > _MAX_BUF_LENGTH) {
		nBytes = _MAX_BUF_LENGTH;
	}
	string s2(s, nBytes);

	{
		Synchronized sem(_bufSem);     // TODO: make other uses of _messBuffer thread-safe with _bufSem
		_messBuffer.push(s2);
	}
}

void TKOLogger::printMessage(const char *format, ...)
{
	char s[_MAX_BUF_LENGTH + 1];
	va_list args;
	va_start(args, format);
	vsnprintf(s, _MAX_BUF_LENGTH + 1, format, args);   // Ignore return value
	va_end(args);

	{
		Synchronized sem(_printSem);
		fputs(s, stdout);
	}
}
