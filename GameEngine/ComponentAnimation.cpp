#include "ComponentAnimation.h"

ComponentAnimation::ComponentAnimation(std::string pFunction) : function(pFunction)
{
}

ComponentAnimation::~ComponentAnimation()
{
}

ComponentType ComponentAnimation::GetComponentName()
{
	return ComponentType::COMPONENT_ANIMATION;
}

void ComponentAnimation::RenderSwap()
{
}

std::string ComponentAnimation::GetFunction()
{
	return function;
}