#include "ComponentDirection.h"

ComponentDirection::~ComponentDirection()
{

}

ComponentType ComponentDirection::GetComponentName()
{
	return ComponentType::COMPONENT_DIRECTION;
}

void ComponentDirection::RenderSwap()
{
	renderDirection = updateDirection;
}
