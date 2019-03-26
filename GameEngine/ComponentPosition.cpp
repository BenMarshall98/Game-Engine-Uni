#include "ComponentPosition.h"

ComponentPosition::~ComponentPosition()
{

}

void ComponentPosition::RenderSwap()
{
	renderPosition = updatePosition;
}