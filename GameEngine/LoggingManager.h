#pragma once

#include <string>
#include <iostream>

enum class MESSAGE_TYPE
{
	SEVERE,
	LOG
};

class LoggingManager
{
private:
	static LoggingManager * instance;
	static bool hasSevereMessage;

	LoggingManager() {}
public:

	static void LogMessage(const MESSAGE_TYPE type, const std::string & message);

	//Returns true, if error has occurred
	static inline bool HasSevereMessage()
	{
		if (hasSevereMessage)
		{
			std::cout << "An Error Has Occured! Please press enter to exit";
			std::cin.get();
		}

		return hasSevereMessage;
	}
};

