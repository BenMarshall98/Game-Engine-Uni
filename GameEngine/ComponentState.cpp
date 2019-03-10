#include "ComponentState.h"

ComponentState::ComponentState()
{
}

ComponentState::~ComponentState()
{
}

std::string ComponentState::GetValue(std::string valueName, std::string defaultValue)
{
	std::map<std::string, std::string>::iterator it = stateValues.find(valueName);

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

void ComponentState::SetValue(std::string valueName, std::string value)
{
	std::map<std::string, std::string>::iterator it = stateValues.find(valueName);

	if (it != stateValues.end())
	{
		it->second = value;
	}
	else
	{
		stateValues.insert(std::pair<std::string, std::string>(valueName, value));
	}
}

ComponentType ComponentState::GetComponentName()
{
	return ComponentType::COMPONENT_STATE;
}

void ComponentState::RenderSwap()
{
}