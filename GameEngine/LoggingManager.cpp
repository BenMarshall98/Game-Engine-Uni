#include "LoggingManager.h"

void LoggingManager::LogMessage(const MESSAGE_TYPE type, const std::string & message)
{
	if (type == MESSAGE_TYPE::SEVERE)
	{
		hasSevereMessage = true;
		std::cout << "Severe Error: " << message << std::endl;
	}
	else
	{
		std::cout << "Log Message: " << message << std::endl;
	}
}

LoggingManager * LoggingManager::instance = nullptr;

bool LoggingManager::hasSevereMessage = false;
