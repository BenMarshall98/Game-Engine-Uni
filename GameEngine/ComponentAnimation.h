#pragma once

#include"iComponent.h"
#include <string>

class ComponentAnimation : public iComponent
{
private:
	std::string function;

public:
	explicit ComponentAnimation(std::string &pFunction);
	~ComponentAnimation();

	void RenderSwap() override;

	std::string & GetFunction();
};

