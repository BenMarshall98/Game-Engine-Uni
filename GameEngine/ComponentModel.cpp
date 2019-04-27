#include "ComponentModel.h"

ComponentModel::~ComponentModel()
{
	ResourceManager::RemoveModel(model);
}

//Does nothing
void ComponentModel::RenderSwap()
{

}
