#pragma once

#include "iComponent.h"
#include <string>
#include <map>

class ComponentState : public iComponent
{
private:
	std::map<std::string, std::string> stateValues = std::map<std::string, std::string>();
	
public:
	ComponentState();
	~ComponentState();



	std::string GetValue(std::string & valueName, std::string & defaultValue);
	void SetValue(std::string & valueName, std::string & value);

	void RenderSwap() override;
};

