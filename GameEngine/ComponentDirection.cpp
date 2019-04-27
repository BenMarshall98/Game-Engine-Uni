#include "ComponentDirection.h"

ComponentDirection::~ComponentDirection()
{

}


//Keeps the component concurrency complient
void ComponentDirection::RenderSwap()
{
	renderDirection = updateDirection;
}
