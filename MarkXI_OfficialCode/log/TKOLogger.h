//Last edited by Vadim Korolik
//on 07/27/2013
#ifndef __TKOLOGGER_H
#define __TKOLOGGER_H

#include "../Definitions.h"
#include "Base.h"
#include "SensorBase.h"
#include <fstream>
#include <Task.h>
#include <string>
#include <queue>
#include <sstream>
#include <iostream>
#include <ostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;
/*
 * TO USE!!!
 * Create a regular object for the class in MarkMain.cpp;
	*  TKOLogger* log;
	*  log = log->getInstance();
	*	log->addMessage("Blah blah, test message");
 *	OR SOMETHING LIKE
	*	std::ostringstream mess;
	*	mess << "MOTOR SPEED: " << 105214124.232;
	*	log->addMessage(mess.str());
 *	TAKES A STRING AS A PARAMETER, NO RETURN!
 *	
 * Implements tasks and filestream.
 * Creates an fstream object to write to the log file of choice.
 * The logger implements a vector buffer system in case the 
 * writing thread does not keep up with messages, also prevents
 * excessive CPU load by spacing out the buffer writing with the task.
 * Initializes a task to manage the buffer writing to the log file
 * using the static LogRunner, calling the buffer writing.
 * Buffer writing, at any moment in time, takes the last value
 * of the buffer vector, writes it to the end of the log, and
 * deletes it from the buffer.
 */
class TKOLogger: public SensorBase
{
	public:
		static TKOLogger* inst();
		void addCMessage(string message, float val);
		void addMessage(const char *format, ...);
		void Start();
		void Stop();
		
	private:
		DISALLOW_COPY_AND_ASSIGN(TKOLogger);
		SEM_ID _printSem;
		TKOLogger();
		~TKOLogger();
		Task *logTask;
		static void LogRunner();
		void addToBuf(string message);
		void writeBuffer();
		static TKOLogger* m_Instance;
		ofstream logFile;
		queue<string> messBuffer;
};
#endif
