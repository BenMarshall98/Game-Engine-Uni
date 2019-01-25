#pragma once

#include <string>
#include <iostream>

using namespace std;

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

	static void LogMessage(const MESSAGE_TYPE type, const string & message);

	static inline bool HasSevereMessage()
	{
		if (hasSevereMessage)
		{
			cout << "An Error Has Occured! Please press enter to exit";
			cin.get();
		}

		return hasSevereMessage;
	}
};

