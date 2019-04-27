#define _CRT_SECURE_NO_WARNINGS

#include "LoggingManager.h"
#include "SceneManager.h"
#include <ctime>

//Logs a message or error
void LoggingManager::LogMessage(const MESSAGE_TYPE type, const std::string & message)
{
	std::time_t result = std::time(nullptr);
	if (type == MESSAGE_TYPE::SEVERE)
	{
		hasSevereMessage = true;
		SceneManager::GetStream() << std::ctime(&result) << " Severe Error: " << message << std::endl;
	}
	else
	{
		SceneManager::GetStream() << std::ctime(&result) << " Log Message: " << message << std::endl;
	}
}

LoggingManager * LoggingManager::instance = nullptr;

bool LoggingManager::hasSevereMessage = false;
