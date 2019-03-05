#include "ComponentInput.h"

ComponentInput::~ComponentInput()
{
	delete inputs;
}

ComponentType ComponentInput::GetComponentName()
{
	return ComponentType::COMPONENT_INPUT;
}

void ComponentInput::RenderSwap()
{

}
