#include "ComponentPosition.h"

ComponentType ComponentPosition::GetComponentName()
{
	return ComponentType::COMPONENT_POSITION;
}

void ComponentPosition::RenderSwap()
{
	renderPosition = vec3(updatePosition);
}