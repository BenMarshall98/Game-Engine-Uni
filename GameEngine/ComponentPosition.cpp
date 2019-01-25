#include "ComponentPosition.h"

ComponentPosition::~ComponentPosition()
{

}

ComponentType ComponentPosition::GetComponentName()
{
	return ComponentType::COMPONENT_POSITION;
}

void ComponentPosition::RenderSwap()
{
	renderPosition = vec3(updatePosition);
}