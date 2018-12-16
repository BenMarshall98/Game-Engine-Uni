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
private:
	static LoggingManager * instance;
	static bool hasSevereMessage;
public:

	static void LogMessage(MESSAGE_TYPE type, string & message);

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

