#include "ComponentPosition.h"

ComponentType ComponentPosition::GetComponentName()
{
	return ComponentType::COMPONENT_POSITION;
}

void ComponentPosition::Swap()
{
	currentPosition = newPosition;
}