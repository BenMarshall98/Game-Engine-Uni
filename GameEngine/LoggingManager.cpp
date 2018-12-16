#include "LoggingManager.h"


LoggingManager::LoggingManager()
{
}


LoggingManager::~LoggingManager()
{
}

void LoggingManager::LogMessage(MESSAGE_TYPE type, string & message)
{
	if (type == SEVERE)
	{
		hasSevereMessage = true;
		cout << "Severe Error: " << message << endl;
	}
	else
	{
		cout << "Log Message: " << message << endl;
	}
}

LoggingManager * LoggingManager::instance = nullptr;

bool LoggingManager::hasSevereMessage = false;
