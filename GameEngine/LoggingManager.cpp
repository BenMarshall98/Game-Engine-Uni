#include "LoggingManager.h"

void LoggingManager::LogMessage(const MESSAGE_TYPE type, const string & message)
{
	if (type == MESSAGE_TYPE::SEVERE)
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
