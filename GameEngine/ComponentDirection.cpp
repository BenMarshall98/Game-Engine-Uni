#include "ComponentDirection.h"

ComponentType ComponentDirection::GetComponentName()
{
	return ComponentType::COMPONENT_DIRECTION;
}

void ComponentDirection::RenderSwap()
{
	renderDirection = updateDirection;
}
