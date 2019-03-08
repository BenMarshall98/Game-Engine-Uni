#include "ComponentModel.h"

ComponentModel::~ComponentModel()
{
	ResourceManager::RemoveModel(model);
}

ComponentType ComponentModel::GetComponentName()
{
	return ComponentType::COMPONENT_MODEL;
}

void ComponentModel::RenderSwap()
{

}
