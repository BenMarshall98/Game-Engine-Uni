#include "ComponentState.h"
#include <iostream>

ComponentState::ComponentState() : iComponent(ComponentType::COMPONENT_STATE)
{
}

ComponentState::~ComponentState()
{
}

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

void ComponentState::RenderSwap()
{
}