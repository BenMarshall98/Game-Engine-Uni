#include "ComponentDirection.h"

ComponentDirection::~ComponentDirection()
{

}

void ComponentDirection::RenderSwap()
{
	renderDirection = updateDirection;
}
