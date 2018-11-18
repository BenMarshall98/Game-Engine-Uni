#pragma once

#include <string>
#include <iostream>

using namespace std;

enum MESSAGE_TYPE
{
	SEVERE,
	LOG
};

class LoggingManager
{
	static LoggingManager * instance;
	static bool hasSevereMessage;
	LoggingManager();
public:
	
	static LoggingManager * Instance()
	{
		if (instance == nullptr)
		{
			instance = new LoggingManager();
		}
		return instance;
	}

	static void LogMessage(MESSAGE_TYPE type, string message);

	inline bool HasSevereMessage()
	{
		if (hasSevereMessage)
		{
			cout << "An Error Has Occured! Please press enter to exit";
			cin.get();
		}

		return hasSevereMessage;
	}

	~LoggingManager();
};

