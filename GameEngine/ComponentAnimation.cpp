#include "ComponentAnimation.h"

ComponentAnimation::ComponentAnimation(std::string & pFunction) :
	iComponent(ComponentType::COMPONENT_ANIMATION), function(pFunction)
{
}

ComponentAnimation::~ComponentAnimation()
{
}

//Does nothing
void ComponentAnimation::RenderSwap()
{
}

//Gets the Lua animation function
std::string & ComponentAnimation::GetFunction()
{
	return function;
}