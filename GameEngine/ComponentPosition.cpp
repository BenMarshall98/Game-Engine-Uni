#include "ComponentPosition.h"

ComponentPosition::~ComponentPosition()
{

}

//Keeps the component concurrency complient
void ComponentPosition::RenderSwap()
{
	renderPosition = updatePosition;
}