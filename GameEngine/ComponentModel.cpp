#include "ComponentModel.h"

ComponentModel::~ComponentModel()
{
	ResourceManager::RemoveModel(model);
}

void ComponentModel::RenderSwap()
{

}
