#include "ComponentState.h"
#include <iostream>

ComponentState::ComponentState() : iComponent(ComponentType::COMPONENT_STATE)
{
}

ComponentState::~ComponentState()
{
}

//Gets the required value by valuename (read variable), creating the value and valuename if it doesn't exist
std::string ComponentState::GetValue(std::string & valueName, std::string & defaultValue)
{
	const std::map<std::string, std::string>::iterator it = stateValues.find(valueName);

	if (it != stateValues.end())
	{
		return it->second;
	}
	else
	{
		stateValues.insert(std::pair<std::string, std::string>(valueName, defaultValue));
		return defaultValue;
	}
}

//Sets valuename to the required value, creating the value and valuename if it doesn't exist
void ComponentState::SetValue(std::string & valueName, std::string & value)
{
	const std::map<std::string, std::string>::iterator it = stateValues.find(valueName);

	if (it != stateValues.end())
	{
		it->second = value;
	}
	else
	{
		stateValues.insert(std::pair<std::string, std::string>(valueName, value));
	}
}

//Does nothing
void ComponentState::RenderSwap()
{
}