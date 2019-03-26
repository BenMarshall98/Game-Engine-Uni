#include "ComponentAnimation.h"

ComponentAnimation::ComponentAnimation(std::string & pFunction) :
	iComponent(ComponentType::COMPONENT_ANIMATION), function(pFunction)
{
}

ComponentAnimation::~ComponentAnimation()
{
}

void ComponentAnimation::RenderSwap()
{
}

std::string & ComponentAnimation::GetFunction()
{
	return function;
}