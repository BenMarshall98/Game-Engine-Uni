#include "ComponentModel.h"

ComponentModel::~ComponentModel()
{
	delete model;
}

ComponentType ComponentModel::GetComponentName()
{
	return ComponentType::COMPONENT_MODEL;
}

void ComponentModel::RenderSwap()
{

}
