#include "AIStateMachine.h"

AIStateMachine::~AIStateMachine()
{
}

string AIStateMachine::GetValue(string valueName, string defaultValue)
{
	map<string, string>::iterator it = stateValues.find(valueName);

	if (it != stateValues.end())
	{
		return it->second;
	}
	else
	{
		stateValues.insert(pair<string, string>(valueName, defaultValue));
		return defaultValue;
	}
}